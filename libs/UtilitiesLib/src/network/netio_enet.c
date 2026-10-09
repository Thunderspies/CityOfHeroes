/* File netio_enet.c
 *    ENet-backed transport for NLT_ENET links. See netio_enet.h for the wire
 *    layout/channel map and doc/enet-transport.md for deployment requirements.
 *    This is the only translation unit that includes <enet/enet.h>; everything
 *    else sees the host/peer as opaque pointers on NetLink/NetLinkList.
 */

// Include wininclude.h FIRST: it pulls in <winsock2.h> ahead of <windows.h>.
// enet.h includes <winsock2.h> itself; letting a raw <windows.h> in first would
// drag in the legacy <winsock.h> and collide with the winsock2 declarations.
#include "utilitieslib/utils/wininclude.h"

// enet/win32.h defines these unconditionally; the build already defines them
#undef _CRT_SECURE_NO_DEPRECATE
#undef _CRT_SECURE_NO_WARNINGS
#include <enet/enet.h>

#include "utilitieslib/network/netio_enet.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/netio_send.h"
#include "utilitieslib/network/netio_stats.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/net_packetutil.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/components/Queue.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/log.h"

// Channel map (see netio_enet.h)
enum {
	NETENET_CHANNEL_ORDERED =
	    0, // control commands + ordered/compressed traffic (reliable)
	NETENET_CHANNEL_RELIABLE =
	    1, // plain reliable + oversized-unreliable promotions
	NETENET_CHANNEL_UNRELIABLE = 2, // unsequenced
	NETENET_CHANNEL_COUNT = 3,
};

// Overflow bookkeeping shared with lnkAddToSendQueue (netio_core.c)
extern int g_assert_on_netlink_overflow;
extern int total_sendqueue_overflows;

static void netEnetRefreshTimeouts(NetLink *link);
static int netEnetSimHoldsSends(const NetLink *link);
static void netEnetSimHold(NetLink *link, ENetPacket *epak, U8 channel);
static void netEnetSimFlush(NetLink *link, int force);
static void netEnetSimFlushAllDue(void);
static void netEnetSimRemove(NetLink *link, int discard);

#define NETENET_UDPIP_OVERHEAD 28 // matches the legacy stat accounting
#define NETENET_MAX_PACKET_SIZE                                                \
	(8 * 1024 * 1024) // sanity bound; biggest legit packets are ~1.5MB
#define NETENET_CONNECT_SLICE_MS 250

// Slots reserved above a list's expected population (see netInitEnet). A peer
// keeps its slot until its timeout expires, so disconnect/reconnect churn needs
// more slots than there are simultaneous users.
#define NETENET_PEER_HEADROOM 32
#define NETENET_PEER_MINIMUM 64

/* Lifecycle */

static int s_enetStarted = 0;

// Effective MTU for ENet hosts, derived from packetStartup's maxPacketSize
// (the legacy send-buffer/MTU knob). Clamped to ENet's protocol limits at
// host-create time.
static int s_enetMtu = 0;

// Route ENet's internal allocations through the CRT entry points, which the
// memcheck/crtdbg force-include maps to the tracked allocator in debug configs.
static void *ENET_CALLBACK s_enetMallocCallback(size_t size)
{
	return malloc(size);
}

static void ENET_CALLBACK s_enetFreeCallback(void *memory) { free(memory); }

static void ENET_CALLBACK s_enetNoMemoryCallback(void)
{
	assertmsg(0, "ENet: out of memory");
}

void netEnetStartup(int maxPacketSize)
{
	ENetCallbacks callbacks = {0};

	if (s_enetStarted)
		return;

	callbacks.malloc = s_enetMallocCallback;
	callbacks.free = s_enetFreeCallback;
	callbacks.no_memory = s_enetNoMemoryCallback;

	if (enet_initialize_with_callbacks(ENET_VERSION, &callbacks) != 0) {
		assertmsg(0, "ENet: initialization failed");
		return;
	}

	s_enetMtu = maxPacketSize;
	s_enetStarted = 1;
}

void netEnetShutdown(void)
{
	if (!s_enetStarted)
		return;

	enet_deinitialize();
	s_enetStarted = 0;
}

int netEnetStarted(void) { return s_enetStarted; }

static enet_uint32 netEnetHostMtu(void)
{
	int mtu = s_enetMtu;
	if (mtu < ENET_PROTOCOL_MINIMUM_MTU)
		mtu = ENET_PROTOCOL_MINIMUM_MTU;
	if (mtu > ENET_HOST_DEFAULT_MTU)
		mtu = ENET_HOST_DEFAULT_MTU;
	return (enet_uint32)mtu;
}

/* All live hosts are registered so NMMonitor and shutdown paths can service
 * and flush them without exposing ENet types. Access is serialized with netio.
 */
typedef struct NetEnetHostEntry {
	ENetHost *host;
	NetLinkList *ownerList; // NULL for client (single-peer) hosts
} NetEnetHostEntry;

#define NETENET_MAX_HOSTS 16
static NetEnetHostEntry s_hosts[NETENET_MAX_HOSTS];
static int s_hostCount = 0;

static void netEnetRegisterHost(ENetHost *host, NetLinkList *ownerList)
{
	assertmsg(s_hostCount < NETENET_MAX_HOSTS, "ENet: too many hosts");
	s_hosts[s_hostCount].host = host;
	s_hosts[s_hostCount].ownerList = ownerList;
	s_hostCount++;
}

static void netEnetUnregisterHost(ENetHost *host)
{
	int i;
	for (i = 0; i < s_hostCount; i++) {
		if (s_hosts[i].host == host) {
			s_hosts[i] = s_hosts[s_hostCount - 1];
			s_hostCount--;
			return;
		}
	}
}

static NetLinkList *netEnetHostOwner(ENetHost *host)
{
	int i;
	for (i = 0; i < s_hostCount; i++) {
		if (s_hosts[i].host == host)
			return s_hosts[i].ownerList;
	}
	return NULL;
}

int netEnetHostsRegistered(void) { return s_hostCount; }

/* Send path */

// Upper bound for the wrapped size of a payload: 8-byte header + payload +
// flag bits/debug type tags (small, bounded) + blowfish padding slack.
static U32 netEnetWrapBound(const Packet *pak)
{
	return (U32)blowfishPad(8 + pak->stream.size + 160);
}

// Peek the command number out of a payload stream (first field written by
// pktSendCmd/pktCreateControlCmd). Mirrors the peek in pktWrap.
static int netEnetPeekCmd(Packet *pak)
{
	int cmd;
	bsRewind(&pak->stream);
	cmd = pktGetBitsPack(pak, 1);
	bsRewind(&pak->stream);
	return cmd;
}

// Overflow bookkeeping shared by every reliable-drop path; parity with
// lnkAddToSendQueue (netio_core.c:77-89).
//
// Each of these discards a reliable packet the sender believes it handed over.
// For a large one-shot payload (the ~1.5MB scene packet) the peer is then left
// waiting for data that is never coming, on a link that still looks perfectly
// healthy -- and neither surviving signal reports it:
// g_assert_on_netlink_overflow compiles out of release builds, and
// link->overflow is never read back. Hence the log.
static void netEnetFlagOverflow(NetLink *link, const Packet *pak,
				const char *why)
{
	if (pak->reliable) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: dropped reliable packet (%d bytes) to %s:%d -- %s",
		    pak->stream.size,
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, why);

		if (g_assert_on_netlink_overflow)
			assert(0);
		link->overflow = 1;
		total_sendqueue_overflows++;
	}
}

/* Mirrors pktWrap()+pktSendRaw() minus the ack/sibling fields: builds the
 * wire bytes for one packet directly into buf (an ENetPacket's data area),
 * including checksum and encryption. Returns the padded byte length.
 * NOTE: encryption happens here, at ENQUEUE time -- the legacy transport
 * encrypts at FLUSH time. Any code that flips link->encrypt_on must do so
 * BEFORE queueing the first packet the new state applies to (see the
 * SERVER_ACK_ACK ordering in handleControlCommand).
 */
static U32 netEnetWrapPacket(Packet *pak_in, NetLink *link, int cmd, U8 *buf,
			     U32 bufSize)
{
	Packet pak_buf;
	Packet *pak;
	BitStream stream;
	unsigned int bitLengthPosition;
	U32 length, padded;

	initBitStream(&stream, buf, bufSize, Write, false, NULL);

	// Same dummy-packet trick as pktWrap: reuse the pkt* helpers on a stack
	// copy.
	pak = &pak_buf;
	*pak = *pak_in;
	pak->stream = stream;

	// [0..3] bit length placeholder, written for real at the end
	bitLengthPosition = bsGetCursorBitPosition(&pak->stream);
	bsWriteBits(&pak->stream, 32, 0);

	// [4..7] checksum placeholder, poked after the length is known
	bsWriteBits(&pak->stream, 32, 0);

	pak->stream.data[pak->stream.cursor.byte] = 0;
	assert(pak->stream.cursor.bit == 0);

	bsWriteBits(&pak->stream, 1, pak_in->hasDebugInfo);
	pktSendBits(pak, 32, pak_in->truncatedID);

	if (link->compressionAllowed && cmd != COMM_CONNECT)
		pktSendBits(pak, 1, pak_in->compress);
	else
		assert(!pak_in->compress);

	if (link->orderedAllowed && cmd != COMM_CONNECT) {
		pktSendBits(pak, 1, pak_in->ordered);
		if (pak_in->ordered)
			pktSendBitsAuto(pak, pak_in->ordered_id);
	}

	pktSetByteAlignment(pak, link->compressionAllowed && pak_in->compress);
	pktSendBitsArray(pak, bsGetBitLength(&pak_in->stream),
			 pak_in->stream.data);

	assert((bitLengthPosition & 7) == 0);
	bsSetCursorBitPosition(&pak->stream, bitLengthPosition);
	bsWriteBits(&pak->stream, 32, bsGetBitLength(&pak->stream));

	assert(!pak->stream.errorFlags);

	length = (bsGetBitLength(&pak->stream) + 7) >> 3;
	padded = blowfishPad(length);
	assert(padded <= bufSize);

	// Legacy sent uninitialized pad bytes; no wire-compat constraint here,
	// so zero them.
	if (padded > length)
		memset(buf + length, 0, padded - length);

	{
		U32 checksum = cryptAdler32(buf + 8, padded - 8);
		*((U32 *)&buf[4]) = endianSwapIfBig(U32, checksum);
	}

	if (link->encrypt_on)
		cryptBlowfishEncrypt(&link->blowfish_ctx, buf + 4, padded - 4);

	return padded;
}

U32 pktSendEnet(Packet **pakptr, NetLink *link)
{
	Packet *pak = *pakptr;
	ENetPeer *peer = link->enet_peer;
	ENetPacket *epak;
	enet_uint32 flags;
	enet_uint32 fragmentLength;
	U8 channel;
	int cmd;
	U32 bound, padded;

	// Transport not fully up (yet/anymore): drop silently, matching legacy
	// sends into a dead/half-open socket. enet_peer_send only accepts
	// CONNECTED peers, so a CONNECTING-state send must not reach it (it
	// would be misreported as an overflow below).
	if (!peer || link->disconnected ||
	    peer->state != ENET_PEER_STATE_CONNECTED) {
		pktFree(*pakptr);
		*pakptr = NULL;
		return 0;
	}

	netEnetRefreshTimeouts(link);

	// Overflow parity with lnkAddToSendQueue: bound the number of queued
	// app packets by sendQueuePacketMax (0 = unlimited).
	// sendQueuePacketCount is our own O(1) counter: once ENet has fully
	// drained its outgoing queues, nothing of ours is still queued, so it
	// self-resets. (Counting ENet's lists directly would count fragments --
	// ~1150 for a 1.5MB packet -- and costs a list walk per send.)
	if (link->sendQueuePacketMax) {
		if (enet_list_empty(&peer->outgoingCommands) &&
		    enet_list_empty(&peer->outgoingSendReliableCommands) &&
		    (!link->enet_sim_holdback ||
		     !qGetSize((Queue)link->enet_sim_holdback)))
			link->sendQueuePacketCount = 0;

		if (link->sendQueuePacketCount >= link->sendQueuePacketMax) {
			netEnetFlagOverflow(link, pak, "send queue full");
			pktFree(*pakptr);
			*pakptr = NULL;
			return 0;
		}
	}

	// Assign the application-visible per-link packet id (see netio_enet.h).
	pak->id = ++link->nextID;
	pak->truncatedID = pak->id;

	if (pak->ordered && verify(!pak->ordered_id && pak->reliable))
		pak->ordered_id = ++link->last_ordered_sent_id;

	cmd = netEnetPeekCmd(pak);

	// Wrap directly into the ENetPacket's buffer (no intermediate copy);
	// the exact size is known only after wrapping, so allocate the upper
	// bound and shrink dataLength afterwards.
	bound = netEnetWrapBound(pak);
	epak = enet_packet_create(NULL, bound, 0);
	if (!epak) {
		netEnetFlagOverflow(link, pak, "enet_packet_create failed");
		pktFree(*pakptr);
		*pakptr = NULL;
		return 0;
	}

	padded = netEnetWrapPacket(pak, link, cmd, epak->data, bound);
	epak->dataLength = padded;

	// ENet fragments anything larger than this (peer.c enet_peer_send); the
	// promotion below must use the same threshold or an oversized
	// unsequenced packet would be transmitted as reliable fragments on the
	// wrong channel.
	fragmentLength = peer->mtu - sizeof(ENetProtocolHeader) -
			 sizeof(ENetProtocolSendFragment);
	if (peer->host->checksum != NULL)
		fragmentLength -= sizeof(enet_uint32);

	// Channel/flag selection. Only ordered packets need the
	// strictly-ordered channel; that includes every STREAM-compressed
	// packet (the shared compression step in pktSendDbg uses the per-link
	// zlib stream only when ordered && reliable, matching pktGet on the
	// receive side). A compressed but unordered packet was compressed
	// statelessly and can decompress in any arrival order, so it routes by
	// reliability like everything else (e.g. worldSendUpdate: reliable +
	// compressed + unordered).
	if (pak->ordered) {
		assert(pak->reliable); // pktSetOrdered forces reliable
		channel = NETENET_CHANNEL_ORDERED;
		flags = ENET_PACKET_FLAG_RELIABLE;
	} else if (cmd < COMM_MAX_CMD) {
		// Control commands: the handshake must be reliable-ordered, but
		// idle keepalives are sent unreliable and must not ride (or
		// block) the ordered stream.
		if (pak->reliable) {
			channel = NETENET_CHANNEL_ORDERED;
			flags = ENET_PACKET_FLAG_RELIABLE;
		} else {
			channel = NETENET_CHANNEL_UNRELIABLE;
			flags = ENET_PACKET_FLAG_UNSEQUENCED;
		}
	} else if (pak->reliable || padded > fragmentLength) {
		// Oversized unreliables are promoted: legacy split them into
		// unreliable siblings that were lost wholesale on any single
		// drop; ENet fragments reliable packets automatically.
		channel = NETENET_CHANNEL_RELIABLE;
		flags = ENET_PACKET_FLAG_RELIABLE;
	} else {
		channel = NETENET_CHANNEL_UNRELIABLE;
		flags = ENET_PACKET_FLAG_UNSEQUENCED;
	}

	epak->flags = flags;

	if (netEnetSimHoldsSends(link)) {
		// Simulated latency: release anything already due (FIFO order),
		// then park this packet until its own release time.
		netEnetSimFlush(link, 0);
		netEnetSimHold(link, epak, channel);
	} else if (enet_peer_send(peer, channel, epak) < 0) {
		enet_packet_destroy(epak);
		netEnetFlagOverflow(
		    link, pak,
		    "enet_peer_send rejected it (over maximumPacketSize or "
		    "fragment count, or bad channel)");
		pktFree(*pakptr);
		*pakptr = NULL;
		return 0;
	}

	link->sendQueuePacketCount++;
	link->lastSendTime = timerCpuTicks();
	link->lastSendTimeSeconds = timerCpuSeconds();
	link->totalBytesSent += padded + NETENET_UDPIP_OVERHEAD;
	link->totalPacketsSent++;
	pktAddHist(&link->sendHistory, padded + NETENET_UDPIP_OVERHEAD, 1);

	if (link->packetSentCallback)
		link->packetSentCallback(link, pak, padded, 1);

	pktFree(*pakptr);
	*pakptr = NULL;
	return 1;
}

/* Receive path */

/* Converts one received ENetPacket into a pooled Packet in the exact state the
 * legacy receive path produces (see pktGetUdp + pktDecryptAndCrc + pktUnwrap),
 * then queues it on the link. The caller destroys the ENetPacket.
 */
static void netEnetHandleReceive(NetLink *link, U8 channelID, ENetPacket *epak)
{
	Packet *pak;
	PacketHeader header;

	// Every discard below drops a packet that ENet delivered intact, so the
	// sender has no idea it went missing.  On the receiving side that is
	// indistinguishable from the data never having been sent, which is why
	// they all say something -- several of these paths were previously
	// silent, and badPacketLog alone does not record what was lost.
	if (epak->dataLength < sizeof(PacketHeader) ||
	    epak->dataLength > NETENET_MAX_PACKET_SIZE) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet from %s:%d -- implausible "
		    "size %d on channel %d",
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)epak->dataLength, (int)channelID);
		badPacketLog(BADPACKET_SIZEMISMATCH,
			     link->addr.sin_addr.S_un.S_addr,
			     link->addr.sin_port);
		return;
	}

	netEnetRefreshTimeouts(link);

	pak = pktCreate();
	pak->xferTime = timerCpuTicks();

	// Copy the wire bytes in through the bitstream API so oversized packets
	// grow the buffer via pktLargePacketAllocator.
	bsWriteBitsArray(&pak->stream, (int)(epak->dataLength * 8), epak->data);
	if (pak->stream.errorFlags) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet from %s:%d -- could not "
		    "buffer %d bytes on channel %d",
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)epak->dataLength, (int)channelID);
		pktFree(pak);
		return;
	}

	memcpy(&header, pak->stream.data, sizeof(PacketHeader));
	header.packetBitLength = endianSwapIfBig(U32, header.packetBitLength);
	header.packetChecksum = endianSwapIfBig(U32, header.packetChecksum);

	// Same size sanity checks as pktGetUdp.
	if ((((header.packetBitLength + 7) >> 3) > (U32)epak->dataLength) ||
	    (((header.packetBitLength + 7) >> 3) + 7 < (U32)epak->dataLength)) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet from %s:%d -- header "
		    "claims %d bits but %d bytes arrived on channel %d",
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)header.packetBitLength,
		    (int)epak->dataLength, (int)channelID);
		badPacketLog(BADPACKET_SIZEMISMATCH,
			     link->addr.sin_addr.S_un.S_addr,
			     link->addr.sin_port);
		pktFree(pak);
		return;
	}

	// Mirror the stream-state fixup from pktGetUdp.
	pak->stream.bitLength = header.packetBitLength;
	pak->stream.size = (header.packetBitLength + 7) >> 3;
	bsChangeMode(&pak->stream, Read);
	pak->stream.size =
	    (unsigned int)epak->dataLength; // full padded size, for blowfish
	pak->stream.cursor.byte += sizeof(PacketHeader);

	if (!pktDecryptAndCrc(pak, link, &link->addr, true)) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet from %s:%d -- "
		    "decrypt/checksum failed on %d bytes, channel %d",
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)epak->dataLength, (int)channelID);
		pktFree(pak);
		return;
	}

	// Mirror pktUnwrap minus acks/siblings/dup-detection (ENet owns those).
	pktSetByteAlignment(pak, 0);

	pak->hasDebugInfo = bsReadBits(&pak->stream, 1);
	pak->id = pak->truncatedID = pktGetBits(pak, 32);

	if (link->compressionAllowed)
		pak->compress = pktGetBits(pak, 1);
	else
		pak->compress = 0;

	if (link->orderedAllowed) {
		pak->ordered = pktGetBits(pak, 1);
		if (pak->ordered)
			pak->ordered_id = pktGetBitsAuto(pak);
	} else
		pak->ordered = 0;

	if (pak->stream.errorFlags) {
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet from %s:%d -- bad header "
		    "fields in %d bytes, channel %d (compressionAllowed %d, "
		    "orderedAllowed %d)",
		    makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)epak->dataLength, (int)channelID,
		    (int)link->compressionAllowed, (int)link->orderedAllowed);
		badPacketLog(BADPACKET_BADDATA, link->addr.sin_addr.S_un.S_addr,
			     link->addr.sin_port);
		pktFree(pak);
		return;
	}

	pktSetByteAlignment(pak, pak->compress);
	pktAlignBitsArray(pak);

	if (pak->stream.errorFlags) {
		// A compress/alignment disagreement between the two ends lands
		// here: the sender byte-aligned the stream but the receiver did
		// not, or vice versa.
		LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
		    "ENet: discarded received packet id %d from %s:%d -- "
		    "alignment failed on %d bytes, channel %d (compress %d, "
		    "ordered %d)",
		    pak->id, makeIpStr(link->addr.sin_addr.S_un.S_addr),
		    link->addr.sin_port, (int)epak->dataLength, (int)channelID,
		    (int)pak->compress, (int)pak->ordered);
		badPacketLog(BADPACKET_BADDATA, link->addr.sin_addr.S_un.S_addr,
			     link->addr.sin_port);
		pktFree(pak);
		return;
	}

	// pktGet() selects the per-link streaming zlib only for
	// ordered+reliable packets; reflect the channel's reliability so that
	// condition still holds.
	pak->reliable = (channelID != NETENET_CHANNEL_UNRELIABLE);

	// Bridge ENet's smoothed RTT into the legacy ping history so
	// pingAvgRate/pingInstantRate consumers (netgraph, monitoring) keep
	// working.
	if (link->enet_peer)
		pingAddHist(&link->pingHistory,
			    (int)((ENetPeer *)link->enet_peer)->roundTripTime);

	link->lastRecvTime = timerCpuSeconds();
	link->totalPacketsRead++;
	link->totalBytesRead += (U32)epak->dataLength + NETENET_UDPIP_OVERHEAD;
	pktAddHist(&link->recvHistory,
		   pak->stream.size + sizeof(PacketHeader) +
		       NETENET_UDPIP_OVERHEAD,
		   0);

	if (!qEnqueue(link->receiveQueue, pak)) {
		// Receive queues are unlimited for netio links; reliable data
		// must not be dropped here or the ordered/zlib stream would
		// desync.
		assertmsg(0, "ENet: receive queue rejected a packet");
		pktFree(pak);
	}
}

static void netEnetHandleEvent(ENetHost *host, ENetEvent *event)
{
	NetLink *link;

	switch (event->type) {
	case ENET_EVENT_TYPE_CONNECT: {
		if (event->peer->data) {
			// Client-side connect completion; netEnetConnect polls
			// peer->state.
			link = event->peer->data;
			netEnetApplyTimeouts(link);
		} else {
			// Server-side accept.
			NetLinkList *nlist = netEnetHostOwner(host);
			if (!nlist) {
				LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
				    "ENet: refused connect from %s:%d -- host "
				    "has no owning link list",
				    makeIpStr(event->peer->address.host),
				    (int)event->peer->address.port);
				enet_peer_reset(event->peer);
				break;
			}

			// Same-address reconnect: a fresh ENet connect from an
			// ip:port that already has a live link means the old
			// session is dead (one socket cannot carry two ENet
			// sessions). Legacy nuked the stale link on a new
			// connect_id (netValidateUDPPacket); mirror that so a
			// fast client restart doesn't leave a ghost link until
			// the peer timeout.
			{
				int i;
				for (i = 0; i < nlist->links->size; i++) {
					NetLink *old = nlist->links->storage[i];
					if (old->type == NLT_ENET &&
					    !old->disconnected &&
					    old->addr.sin_addr.s_addr ==
						event->peer->address.host &&
					    old->addr.sin_port ==
						htons((U16)event->peer->address
							  .port)) {
						ENetPeer *oldPeer =
						    old->enet_peer;

						// enet_peer_reset tells the old
						// peer nothing at all. If this
						// ever fires on a link that was
						// in fact still live, that peer
						// is left believing it is
						// connected while this end has
						// no record of it -- it will
						// sit there receiving nothing,
						// not even acks, until its own
						// timeout. Worth knowing about.
						LOG(LOG_NET,
						    LOG_LEVEL_IMPORTANT, 0,
						    "ENet: connect from %s:%d "
						    "displaced an existing "
						    "link on the same address "
						    "(peer %s) -- resetting it "
						    "without notice",
						    makeIpStr(
							event->peer->address
							    .host),
						    (int)event->peer->address
							.port,
						    oldPeer ? "live"
							    : "already gone");

						if (oldPeer) {
							oldPeer->data = NULL;
							enet_peer_reset(
							    oldPeer);
							old->enet_peer = NULL;
						}
						old->logout_disconnect = 1;
						netDiscardDeadLink(old);
						break;
					}
				}
			}

			link = netAddLinkEnetAccept(
			    nlist, event->peer->address.host,
			    (int)event->peer->address.port, host->socket);
			if (!link) {
				// Rejected (private-list gate or allocation
				// failure).
				LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
				    "ENet: rejected connect from %s:%d -- "
				    "netAddLinkEnetAccept refused it "
				    "(private-list gate, or the list is out of "
				    "links: %d in use)",
				    makeIpStr(event->peer->address.host),
				    (int)event->peer->address.port,
				    nlist->links ? nlist->links->size : -1);
				enet_peer_disconnect_now(event->peer, 0);
				break;
			}
			link->enet_host = host;
			link->enet_peer = event->peer;
			event->peer->data = link;
			netEnetApplyTimeouts(link);

			// Once the host is full ENet drops further CONNECTs
			// without telling anyone -- no event, nothing to log,
			// the client just retries until it gives up. The last
			// accepts before that point are the only chance to say
			// so.
			if (host->connectedPeers * 10 >= host->peerCount * 9) {
				LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
				    "ENet: host on port %d is at %d/%d peers "
				    "-- once full, further connects are "
				    "refused silently",
				    (int)host->address.port,
				    (int)host->connectedPeers,
				    (int)host->peerCount);
			}
		}
		break;
	}

	case ENET_EVENT_TYPE_RECEIVE: {
		link = event->peer->data;
		if (link && !link->disconnected)
			netEnetHandleReceive(link, event->channelID,
					     event->packet);
		enet_packet_destroy(event->packet);
		break;
	}

	case ENET_EVENT_TYPE_DISCONNECT: {
		link = event->peer->data;
		event->peer->data = NULL;
		if (link) {
			// The peer slot is recycled by ENet after this event.
			link->enet_peer = NULL;
			if (!link->disconnected) {
				// A transport-level loss the app didn't
				// initiate (timeout, crash, peer reset): the
				// legacy unresponsive-link reap set this so
				// MapServer clears mid-transfer db flags.
				link->logout_disconnect = 1;

				if (link->parentNetLinkList) {
					// Two-phase teardown: flagged here,
					// freed by
					// netLinkListRemoveDisconnected in
					// NMMonitor.
					netDiscardDeadLink(link);
				} else {
					link->disconnected = 1;
					link->connected = 0;
				}
			}
			// else: the in-band COMMCONTROL_DISCONNECT already ran
			// the discard path; nothing more to do.
		}
		break;
	}

	default:
		break;
	}
}

/* Drains all pending events on a host. maxWaitMs > 0 blocks in
 * enet_host_service for up to that long waiting for the first event (servicing
 * ENet's own retransmit/ping timers while waiting); the rest of the drain never
 * blocks.
 */
static int netEnetServiceHost(ENetHost *host, int maxWaitMs)
{
	ENetEvent event;
	int eventCount = 0;
	enet_uint32 wait = (maxWaitMs > 0) ? (enet_uint32)maxWaitMs : 0;

	if (!host)
		return 0;

	while (enet_host_service(host, &event, wait) > 0) {
		wait = 0;
		eventCount++;
		netEnetHandleEvent(host, &event);
	}

	return eventCount;
}

void netEnetServiceLink(NetLink *link, int maxWaitMs)
{
	if (link->enet_host)
		netEnetServiceHost(link->enet_host, maxWaitMs);
}

void netEnetServiceAll(void)
{
	int i;
	netEnetSimFlushAllDue();
	// Event handlers never register/unregister hosts (teardown happens
	// later, from clearNetLink during link reaping), so plain iteration is
	// safe.
	for (i = 0; i < s_hostCount; i++)
		netEnetServiceHost(s_hosts[i].host, 0);
}

void netEnetFlushAll(void)
{
	int i;
	netEnetSimFlushAllDue();
	for (i = 0; i < s_hostCount; i++)
		enet_host_flush(s_hosts[i].host);
}

void netEnetFlushLink(NetLink *link)
{
	if (link->enet_sim_holdback)
		netEnetSimFlush(link, 0);
	if (link->enet_host)
		enet_host_flush((ENetHost *)link->enet_host);
}

/* Link state */

int netEnetLinkAlive(const NetLink *link)
{
	const ENetPeer *peer = link->enet_peer;
	if (!peer || link->disconnected)
		return 0;
	return peer->state >= ENET_PEER_STATE_CONNECTING &&
	       peer->state <= ENET_PEER_STATE_CONNECTED;
}

void netEnetApplyTimeouts(NetLink *link)
{
	ENetPeer *peer = link->enet_peer;
	if (!peer)
		return;

	if (link->notimeout) {
		// Effectively never: parity with the legacy notimeout flag.
		enet_peer_timeout(peer, 0x7fffffff, 24 * 60 * 60 * 1000,
				  24 * 60 * 60 * 1000);
	} else {
		// The legacy 60s quiet-link reap, as a flat 60s.
		//
		// Both of ENet's timeout limbs measure the same thing: how long
		// we have gone without hearing an acknowledgement
		// (enet_protocol_handle_acknowledge clears earliestTimeout on
		// every ack, so a transfer that is merely slow keeps the clock
		// reset -- only a genuine gap counts).  The difference is the
		// bound.  The retry limb fires once a reliable command has been
		// retried through 6 RTO doublings, i.e. after a gap of 63 x the
		// initial RTO: ~5s on a LAN, ~15s at 140ms ping, reaching 60s
		// only if the RTO is already near a second.  So at
		// ENET_PEER_TIMEOUT_LIMIT (32) it always beat timeoutMaximum to
		// the punch, and how much of a gap a link tolerated came out of
		// the player's latency -- the better the connection, the sooner
		// they dropped.
		//
		// Disabling that limb leaves timeoutMaximum as the only bound,
		// which is the flat 60s threshold the legacy transport applied
		// to everyone alike.
		enet_peer_timeout(peer, 0x7fffffff, 5000, 60000);
	}
	link->enet_applied_notimeout = !!link->notimeout;
}

// notimeout is toggled at runtime on both ends (map transfers, dev commands);
// push changes into the peer's ENet timeout policy lazily.
static void netEnetRefreshTimeouts(NetLink *link)
{
	if ((U32) !!link->notimeout != link->enet_applied_notimeout)
		netEnetApplyTimeouts(link);
}

/* Network-condition simulation uses inbound datagram loss so reliable
 * packets retransmit. Outbound application sends wait in a FIFO holdback for
 * lag/jitter; unordered traffic can also be shuffled. Ordered messages keep
 * their original send order because compression precedes ENet sequencing.
 * ENet's own retransmissions bypass the outbound delay.
 */
typedef struct NetEnetHeldPacket {
	ENetPacket *epak;
	U8 channel;
	U32 releaseTicks;
} NetEnetHeldPacket;

// Dev feature: a handful of simulated links at most.
#define NETENET_MAX_SIM_LINKS 8
static NetLink *s_simLinks[NETENET_MAX_SIM_LINKS];
static int s_simLinkCount = 0;

static int netEnetSimFindLink(const NetLink *link)
{
	int i;
	for (i = 0; i < s_simLinkCount; i++)
		if (s_simLinks[i] == link)
			return i;
	return -1;
}

static int netEnetSimHoldsSends(const NetLink *link)
{
	return link->simulateNetworkCondition &&
	       (link->simulatedLagTime > 0 || link->simulatedLagVary > 0);
}

/* Inbound loss: drop raw datagrams from simulated peers before ENet's
 * protocol processes them. Return 1 = discard (as if the wire ate it).
 */
static int ENET_CALLBACK netEnetSimIntercept(ENetHost *host, ENetEvent *event)
{
	int i;
	for (i = 0; i < s_simLinkCount; i++) {
		NetLink *link = s_simLinks[i];
		if (link->enet_host == host &&
		    link->addr.sin_addr.s_addr == host->receivedAddress.host &&
		    ntohs(link->addr.sin_port) == host->receivedAddress.port) {
			if (link->simulatedPacketLostRate > 0 &&
			    randInt(100) < link->simulatedPacketLostRate)
				return 1;
			return 0;
		}
	}
	return 0;
}

// (Un)install the intercept on a host depending on whether any registered
// simulated link still rides it.
static void netEnetSimSyncIntercept(ENetHost *host)
{
	int i;
	if (!host)
		return;
	for (i = 0; i < s_simLinkCount; i++) {
		if (s_simLinks[i]->enet_host == host) {
			host->intercept = netEnetSimIntercept;
			return;
		}
	}
	host->intercept = NULL;
}

static void netEnetSimHold(NetLink *link, ENetPacket *epak, U8 channel)
{
	NetEnetHeldPacket *held;
	int lagMs = link->simulatedLagTime;
	int vary;
	Queue holdback = link->enet_sim_holdback;

	if (!holdback) {
		holdback = createQueue();
		initQueue(holdback, 32);
		link->enet_sim_holdback = holdback;
	}

	vary = randInt(link->simulatedLagVary + 1);
	if (randInt(2))
		vary = -vary;
	lagMs += vary;
	if (lagMs < 0)
		lagMs = 0;

	held = malloc(sizeof(NetEnetHeldPacket));
	held->epak = epak;
	held->channel = channel;
	held->releaseTicks =
	    timerCpuTicks() + (U32)(((F32)lagMs / 1000.0f) * timerCpuSpeed());

	qEnqueue(holdback, held);
}

/* Send every held packet whose time has come (all of them when force is set,
 * e.g. simulation turned off). Head-of-line FIFO like the legacy send queue.
 */
static void netEnetSimFlush(NetLink *link, int force)
{
	Queue holdback = link->enet_sim_holdback;
	ENetPeer *peer;

	if (!holdback)
		return;

	peer = link->enet_peer;

	// ENet assigns sequence numbers when held packets are submitted, after
	// streaming compression has already advanced in application send order.
	// Keep channel 0's slots in FIFO order or its zlib stream is corrupted.
	// Shuffle only unordered traffic using logical offsets in the ring.
	if (link->simulatedOutOfOrderDelivery && qGetSize(holdback) >= 2) {
		int count = qGetSize(holdback);
		for (int i = 0; i < count; i++) {
			void **current = qGetWrappedElement(holdback, i);
			void **other =
			    qGetWrappedElement(holdback, randInt(count));
			NetEnetHeldPacket *a = *current;
			NetEnetHeldPacket *b = *other;
			if (a->channel == NETENET_CHANNEL_ORDERED ||
			    b->channel == NETENET_CHANNEL_ORDERED)
				continue;
			*current = b;
			*other = a;
		}
	}

	while (qGetSize(holdback)) {
		NetEnetHeldPacket *held;

		held = qPeek(holdback);
		if (!force && (int)(timerCpuTicks() - held->releaseTicks) < 0)
			break;

		qDequeue(holdback);

		if (peer && peer->state == ENET_PEER_STATE_CONNECTED) {
			if (enet_peer_send(peer, held->channel, held->epak) < 0)
				enet_packet_destroy(held->epak);
		} else {
			enet_packet_destroy(held->epak);
		}

		free(held);
	}
}

static void netEnetSimFlushAllDue(void)
{
	int i;
	for (i = 0; i < s_simLinkCount; i++)
		netEnetSimFlush(s_simLinks[i], 0);
}

// Unregister a link from the simulator; discard=1 destroys held packets
// (teardown), discard=0 sends them (simulation switched off).
static void netEnetSimRemove(NetLink *link, int discard)
{
	int idx = netEnetSimFindLink(link);
	ENetHost *host = link->enet_host;

	if (idx >= 0) {
		s_simLinks[idx] = s_simLinks[s_simLinkCount - 1];
		s_simLinkCount--;
	}

	if (link->enet_sim_holdback) {
		if (discard) {
			Queue holdback = link->enet_sim_holdback;
			while (qGetSize(holdback)) {
				NetEnetHeldPacket *held = qDequeue(holdback);
				enet_packet_destroy(held->epak);
				free(held);
			}
		} else {
			netEnetSimFlush(link, 1);
		}
		destroyQueue(link->enet_sim_holdback);
		link->enet_sim_holdback = NULL;
	}

	netEnetSimSyncIntercept(host);
}

void netEnetSimApply(NetLink *link)
{
	if (link->type != NLT_ENET)
		return;

	if (link->simulateNetworkCondition) {
		if (netEnetSimFindLink(link) < 0) {
			if (!verify(s_simLinkCount < NETENET_MAX_SIM_LINKS))
				return;
			s_simLinks[s_simLinkCount++] = link;
		}
		netEnetSimSyncIntercept(link->enet_host);
	} else {
		netEnetSimRemove(link, 0);
	}
}

void netEnetLinkTeardown(NetLink *link)
{
	ENetPeer *peer = link->enet_peer;
	ENetHost *host = link->enet_host;

	netEnetSimRemove(link, 1);

	if (peer) {
		int wasLive = (peer->state >= ENET_PEER_STATE_CONNECTING &&
			       peer->state <= ENET_PEER_STATE_CONNECTED);

		// A live peer gets disconnect_now, which at least attempts to
		// say so; that is the ordinary path and not worth a line.
		// Resetting a peer tells the far end nothing whatsoever, and if
		// it turns out that peer was still there it is left holding a
		// link it believes is up. Only that case gets logged.
		if (!wasLive) {
			LOG(LOG_NET, LOG_LEVEL_IMPORTANT, 0,
			    "ENet: silently reset link to %s:%d (peer state "
			    "%d) -- far end will not be told",
			    makeIpStr(link->addr.sin_addr.S_un.S_addr),
			    link->addr.sin_port, (int)peer->state);
		}

		peer->data = NULL;
		if (wasLive) {
			// Transport-level notice so the other side gets a
			// DISCONNECT event promptly (the in-band
			// COMMCONTROL_DISCONNECT usually preceded this).
			enet_peer_disconnect_now(peer, 0);
		} else {
			enet_peer_reset(peer);
		}
		link->enet_peer = NULL;
	}

	if (host && link->owns_enet_host) {
		netEnetUnregisterHost(host);
		enet_host_destroy(host);
	}
	link->enet_host = NULL;
	link->owns_enet_host = 0;
	link->socket = 0;
}

/* Client connect */

int netEnetConnect(NetLink *link, const char *ip_str, int port,
		   F32 sliceSeconds)
{
	ENetHost *host = link->enet_host;
	ENetPeer *peer = link->enet_peer;
	int sliceMs = (int)(sliceSeconds * 1000.0f);

	if (sliceMs <= 0)
		sliceMs = NETENET_CONNECT_SLICE_MS;

	if (!host) {
		ENetAddress eaddr;
		U32 ip = ipFromString(ip_str);

		if (INADDR_NONE == ip)
			return 0;

		host = enet_host_create(NULL, 1, NETENET_CHANNEL_COUNT, 0, 0);
		if (!host)
			return 0;

		host->mtu = netEnetHostMtu();
		host->maximumPacketSize = NETENET_MAX_PACKET_SIZE;

		eaddr.host = ip; // both already in network byte order
		eaddr.port =
		    (enet_uint16)port; // ENetAddress.port is host byte order

		peer =
		    enet_host_connect(host, &eaddr, NETENET_CHANNEL_COUNT, 0);
		if (!peer) {
			enet_host_destroy(host);
			return 0;
		}

		initNetLink(link);
		link->type = NLT_ENET;
		// clearNetLink on a failed earlier attempt memsets the link, so
		// re-establish everything a fresh connect needs -- including
		// opType, which NMAddLink later keys the sync-servicing list
		// on.
		link->opType = NLOT_SYNC;
		link->socket = host->socket;
		link->enet_host = host;
		link->enet_peer = peer;
		link->owns_enet_host = 1;
		link->retransmit = 0;
		link->flowControl = 0;
		sockSetAddr(&link->addr, ip, port);
		peer->data = link;

		// Fail fast while connecting so a dead server produces a
		// DISCONNECT event and the caller's retry loop can start a
		// fresh attempt; netEnetApplyTimeouts sets the real policy once
		// connected.
		enet_peer_timeout(peer, ENET_PEER_TIMEOUT_LIMIT, 1000, 5000);

		netEnetRegisterHost(host, NULL);
	}

	netEnetServiceHost(host, sliceMs);

	// The DISCONNECT handler clears enet_peer on a failed/refused attempt.
	peer = link->enet_peer;
	if (!peer || peer->state == ENET_PEER_STATE_DISCONNECTED ||
	    peer->state == ENET_PEER_STATE_ZOMBIE) {
		// Full clear (not just transport teardown): initNetLink ran on
		// this attempt, and the retry will run it again.
		clearNetLink(link);
		return 0;
	}

	return peer->state == ENET_PEER_STATE_CONNECTED;
}

/* Server listen */

int netInitEnet(NetLinkList *nlist, int enet_port, int tcp_port)
{
	ENetHost *host;
	ENetAddress eaddr;
	size_t peerCount;

	assert(netEnetStarted()); // packetStartup must run first

	// expectedLinks is a legacy memory-pool hint, NOT a limit --
	// netLinkListAlloc divides it by 4 for pool sizing and nlist->links is
	// a growable array, so under the old transport exceeding it cost an
	// allocation and nothing else. ENet's peerCount is a hard ceiling, and
	// a host at capacity answers further CONNECTs with silence:
	// enet_protocol_handle_connect returns NULL internally, no event
	// reaches us, and the client simply retries until it times out. Using
	// the hint directly therefore converted a soft number into an invisible
	// wall (MapServer's was 100). Size off the hint, but leave real room
	// above it.
	peerCount = nlist->expectedLinks > 0 ? (size_t)nlist->expectedLinks : 0;
	peerCount += peerCount / 2 + NETENET_PEER_HEADROOM;

	if (peerCount < NETENET_PEER_MINIMUM)
		peerCount = NETENET_PEER_MINIMUM;
	if (peerCount > ENET_PROTOCOL_MAXIMUM_PEER_ID)
		peerCount =
		    ENET_PROTOCOL_MAXIMUM_PEER_ID; // enet_host_create rejects
						   // anything above this

	eaddr.host = ENET_HOST_ANY;
	eaddr.port = (enet_uint16)enet_port;

	host = enet_host_create(&eaddr, peerCount, NETENET_CHANNEL_COUNT, 0, 0);
	if (!host)
		return 0; // port in use: callers port-scan on a 0 return, like
			  // netInit

	host->mtu = netEnetHostMtu();
	host->maximumPacketSize = NETENET_MAX_PACKET_SIZE;

	nlist->enet_host = host;
	netEnetRegisterHost(host, nlist);

	// netInit sets the list's socket-buffer defaults and, when tcp_port is
	// nonzero, opens the vestigial TCP accept path (-tcp) on the same list.
	if (!netInit(nlist, 0, tcp_port)) {
		nlist->enet_host = NULL;
		netEnetUnregisterHost(host);
		enet_host_destroy(host);
		return 0;
	}

	return 1;
}

/* netLinkListDisconnect branch: destroy the list's ENet host. The links riding
 * it are reclaimed through the normal disconnect flow.
 */
void netEnetListShutdown(NetLinkList *nlist)
{
	ENetHost *host = nlist->enet_host;
	int i;

	if (!host)
		return;

	// Detach any live links from their peers first; the host owns the
	// peers.
	for (i = 0; i < nlist->links->size; i++) {
		NetLink *link = nlist->links->storage[i];
		if (link->enet_peer)
			((ENetPeer *)link->enet_peer)->data = NULL;
		link->enet_peer = NULL;
		link->enet_host = NULL;
	}

	nlist->enet_host = NULL;
	netEnetUnregisterHost(host);
	enet_host_destroy(host);
}

/* netLinkListSetBufferSize branch */

void netEnetSetSocketBufferSize(NetLinkList *nlist, int type, int size)
{
	ENetHost *host = nlist->enet_host;
	if (!host)
		return;
	if (type & SendBuffer)
		enet_socket_set_option(host->socket, ENET_SOCKOPT_SNDBUF, size);
	if (type & ReceiveBuffer)
		enet_socket_set_option(host->socket, ENET_SOCKOPT_RCVBUF, size);
}

/* Link/queue depth accessors */

/* ENet queues one command per MTU-sized fragment, so a raw command count is not
 * comparable to the per-application-packet counts callers compare against --
 * the legacy send queue explicitly excluded sibling packets for the same reason
 * (see lnkAddToSendQueue).  A 1.5MB scene packet is one packet but ~1150
 * commands, which sails past thresholds like svrTickCheckDisconnect's 400 and
 * gets a perfectly healthy client kicked.
 * enet_peer_send enqueues a packet's fragments contiguously and they stay that
 * way, so counting the points where the owning ENetPacket changes recovers the
 * application packet count in a single pass -- the same cost as enet_list_size.
 * Commands with no packet (acks, pings, disconnects) each stand alone.
 */
static int netEnetCountAppPackets(const ENetList *list)
{
	ENetList *mutable_list = (ENetList *)list;
	ENetListIterator node;
	const void *prev = NULL;
	int count = 0;

	for (node = enet_list_begin(mutable_list);
	     node != enet_list_end(mutable_list); node = enet_list_next(node)) {
		const ENetOutgoingCommand *cmd =
		    (const ENetOutgoingCommand *)node;

		if (!cmd->packet || cmd->packet != prev)
			count++;
		prev = cmd->packet;
	}

	return count;
}

int netLinkReliableBacklogPackets(const NetLink *link)
{
	if (!link)
		return 0;

	if (link->type == NLT_ENET) {
		ENetPeer *peer = link->enet_peer;
		if (!peer)
			return 0;
		// A packet mid-transmission can have fragments in both lists,
		// so it may be counted twice.  Off by one per in-flight packet
		// beats off by ~1150.
		return netEnetCountAppPackets(
			   &peer->outgoingSendReliableCommands) +
		       netEnetCountAppPackets(&peer->sentReliableCommands);
	}

	return link->reliablePacketsArray.size;
}

int netLinkReliableBacklogBytes(const NetLink *link)
{
	int i;
	int bytes = 0;

	if (!link)
		return 0;

	if (link->type == NLT_ENET) {
		const ENetPeer *peer = link->enet_peer;
		return peer ? (int)peer->reliableDataInTransit : 0;
	}

	for (i = 0; i < link->reliablePacketsArray.size; i++) {
		const Packet *pak = link->reliablePacketsArray.storage[i];
		if (pak)
			bytes += pak->stream.size;
	}

	return bytes;
}

/* link->totalBytesRead only moves when a whole packet has been reassembled and
 * handed up, so a large fragmented payload (the ~1.5MB scene packet is ~1150
 * fragments) reads as zero right up until it lands -- which is
 * indistinguishable from nothing having been sent.  These read ENet's own
 * counters so a transfer can be watched while it is still in flight.
 * Note the received-bytes figure is per HOST, not per link.  On a client, whose
 * host carries exactly one peer, that is this connection; on a server it is all
 * clients sharing the listen host.
 */
U32 netLinkTransportBytesReceived(const NetLink *link)
{
	const ENetHost *host;

	if (!link || link->type != NLT_ENET)
		return 0;

	host = link->enet_host;
	return host ? (U32)host->totalReceivedData : 0;
}

/* Counterpart of netLinkTransportBytesReceived. link->totalBytesSent only
 * counts what pktSendEnet handed over; ENet's own retransmissions and acks
 * never pass through there. This is the raw wire total, so a stalled link can
 * be told apart from a silent one: retransmissions make this climb even when
 * nothing new is being queued.
 */
U32 netLinkTransportBytesSent(const NetLink *link)
{
	const ENetHost *host;

	if (!link || link->type != NLT_ENET)
		return 0;

	host = link->enet_host;
	return host ? (U32)host->totalSentData : 0;
}

U32 netLinkReassemblyBytesPending(const NetLink *link)
{
	const ENetPeer *peer;

	if (!link || link->type != NLT_ENET)
		return 0;

	peer = link->enet_peer;
	return peer ? (U32)peer->totalWaitingData : 0;
}

int netLinkUnackedCount(const NetLink *link)
{
	if (!link)
		return 0;

	if (link->type == NLT_ENET) {
		ENetPeer *peer = link->enet_peer;
		return peer ? (int)enet_list_size(&peer->sentReliableCommands)
			    : 0;
	}

	return (int)(link->nextID - link->idAck);
}

int netLinkSendQueueDepth(const NetLink *link)
{
	if (!link)
		return 0;

	if (link->type == NLT_ENET) {
		ENetPeer *peer = link->enet_peer;
		if (!peer)
			return 0;
		return (
		    int)(enet_list_size(&peer->outgoingCommands) +
			 enet_list_size(&peer->outgoingSendReliableCommands));
	}

	if (!link->sendQueue2)
		return 0;

	return qGetSize(link->sendQueue2);
}
