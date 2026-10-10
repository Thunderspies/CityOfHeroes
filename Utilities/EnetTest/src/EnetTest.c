/* EnetTest.c
 *    Headless self-test for the NLT_ENET transport (netio_enet.c): runs an
 *    ENet server list and a client link over loopback in one process and
 *    verifies the full facade -- transport connect, the in-band DH/Blowfish
 *    handshake, all three channels, streaming compression, big-packet
 *    fragmentation, app-visible packet ids, cookies, disconnect teardown,
 *    and packet-pool leak balance.
 *
 *    Exit code 0 = all sections passed.
 */

#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utilitiesLib.h"
#include "utilitieslib/network/netio.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/net_masterlist.h"
#include "utilitieslib/network/net_version.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"

#undef _CRT_SECURE_NO_DEPRECATE
#undef _CRT_SECURE_NO_WARNINGS
#include <enet/enet.h>

#include <stdio.h>
#include <string.h>

#define TEST_PORT 42667
#define CMD_ECHO 20	  // client->server: echo this back
#define CMD_ECHO_REPLY 21 // server->client: the echo
#define CMD_BIG 22	  // client->server: big payload, reply with checksum
#define CMD_BIG_REPLY 23
#define CMD_COOKIE_PING 24

#define BIG_PACKET_BYTES (1536 * 1024) // ~1.5MB: the base-editor/MSR case

typedef struct TestClientData {
	int spare;
} TestClientData;

static NetLinkList s_serverLinks;
static NetLink s_clientLink;

static int s_failures = 0;
static int s_serverAllocs = 0;
static int s_serverDestroys = 0;
static int s_serverEchoes = 0;
static U32 s_serverLastEchoPakId = 0;
static U32 s_serverBigChecksum = 0;

#define CHECK(cond, name)                                                      \
	do {                                                                   \
		if (cond) {                                                    \
			printf("PASS: %s\n", name);                            \
		} else {                                                       \
			printf("FAIL: %s\n", name);                            \
			s_failures++;                                          \
		}                                                              \
		fflush(stdout);                                                \
	} while (0)

static U32 simpleChecksum(const U8 *data, int len)
{
	U32 sum = 2166136261u;
	int i;
	for (i = 0; i < len; i++)
		sum = (sum ^ data[i]) * 16777619u;
	return sum;
}

/* Builds a packet the way compressed senders must (see worldSendUpdate,
 * groupnetsend.c): pktSetCompression only works on an empty stream, so the
 * command is written manually instead of via pktCreateEx. Non-compressed
 * packets take the normal pktCreateEx path.
 */
static Packet *createTestPacket(NetLink *link, int cmd, int compress)
{
	Packet *pak;
	if (compress) {
		pak = pktCreate();
		pktSetCompression(pak, 1);
		devassert(pak->compress); // fails if the stream was not empty
		pktSendBitsPack(pak, 1, cmd);
	} else {
		pak = pktCreateEx(link, cmd);
	}
	return pak;
}

/* Server side */

static int serverLinkAlloc(NetLink *link)
{
	s_serverAllocs++;
	return 1;
}

static int serverLinkDestroy(NetLink *link)
{
	s_serverDestroys++;
	return 1;
}

static int serverHandleMsg(Packet *pak, int cmd, NetLink *link)
{
	switch (cmd) {
	case CMD_ECHO: {
		char buf[256];
		Packet *reply;
		int wantOrdered = pktGetBits(pak, 1);
		int wantReliable = pktGetBits(pak, 1);
		int wantCompress = pktGetBits(pak, 1);
		strcpy(buf, pktGetString(pak));

		s_serverEchoes++;
		s_serverLastEchoPakId = pak->id;

		reply = createTestPacket(link, CMD_ECHO_REPLY, wantCompress);
		reply->reliable = wantReliable;
		if (wantOrdered)
			pktSetOrdered(reply, 1);
		pktSendBits(reply, 1, wantOrdered);
		pktSendBits(reply, 1, wantReliable);
		pktSendBits(reply, 1, wantCompress);
		pktSendString(reply, buf);
		pktSend(&reply, link);
		break;
	}

	case CMD_BIG: {
		int len = pktGetBitsAuto(pak);
		static U8 big[BIG_PACKET_BYTES];
		Packet *reply;

		if (len > 0 && len <= BIG_PACKET_BYTES) {
			pktGetBitsArray(pak, len * 8, big);
			s_serverBigChecksum = simpleChecksum(big, len);
		}

		reply = pktCreateEx(link, CMD_BIG_REPLY);
		pktSendBits(reply, 32, s_serverBigChecksum);
		pktSend(&reply, link);
		break;
	}

	case CMD_COOKIE_PING:
		// Nothing to do: the cookie echo rides pktCreateEx/pktSendCmd
		// automatically on cookieEcho links.
		break;

	default:
		printf("server: unexpected cmd %d\n", cmd);
		s_failures++;
		break;
	}
	return 1;
}

/* Client side */

typedef struct EchoExpect {
	int ordered, reliable, compress;
	char text[256];
	int matched;
} EchoExpect;

static EchoExpect s_expect[64];
static int s_expectCount = 0;
static int s_clientEchoesSeen = 0;
static U32 s_clientBigReply = 0;
static int s_clientBigReplySeen = 0;
static U32 s_lastClientRecvId = 0;
static int s_clientRecvIdRegressions = 0;
static U32 s_lastOrderedRecvId = 0;
static int s_orderedRecvIdRegressions = 0;

static int clientHandleMsg(Packet *pak, int cmd, NetLink *link)
{
	switch (cmd) {
	case CMD_ECHO_REPLY: {
		int i;
		int ordered = pktGetBits(pak, 1);
		int reliable = pktGetBits(pak, 1);
		int compress = pktGetBits(pak, 1);
		const char *text = pktGetString(pak);

		s_clientEchoesSeen++;

		for (i = 0; i < s_expectCount; i++) {
			if (!s_expect[i].matched &&
			    s_expect[i].ordered == ordered &&
			    s_expect[i].reliable == reliable &&
			    s_expect[i].compress == compress &&
			    strcmp(s_expect[i].text, text) == 0) {
				s_expect[i].matched = 1;
				break;
			}
		}
		if (i == s_expectCount) {
			printf("client: unmatched echo '%s' (o%d r%d c%d)\n",
			       text, ordered, reliable, compress);
			s_failures++;
		}

		// Reliable replies arrive in ascending id order per channel;
		// just track regressions loosely across the whole link.
		if (pak->id < s_lastClientRecvId)
			s_clientRecvIdRegressions++;
		s_lastClientRecvId = pak->id;
		if (ordered && reliable) {
			if (pak->id <= s_lastOrderedRecvId)
				s_orderedRecvIdRegressions++;
			s_lastOrderedRecvId = pak->id;
		}
		break;
	}

	case CMD_BIG_REPLY:
		s_clientBigReply = pktGetBits(pak, 32);
		s_clientBigReplySeen = 1;
		break;

	default:
		printf("client: unexpected cmd %d\n", cmd);
		s_failures++;
		break;
	}
	return 1;
}

/* Pump helpers */

// Pumps the server while the client blocks inside netConnectEx (single-process
// loopback; a real deployment has the server in its own process).
static void serverPumpIdleCallback(F32 timeLeft) { NMMonitor(1); }

static void pumpBoth(int iterations)
{
	int i;
	for (i = 0; i < iterations; i++) {
		NMMonitor(1); // server side
		netLinkMonitor(&s_clientLink, 0,
			       clientHandleMsg); // client side
		netIdle(&s_clientLink, 0,
			5); // client keepalives (as clientcomm does)
		lnkBatchSend(&s_clientLink);
		Sleep(1);
	}
}

static int pumpUntil(int *flag, int target, F32 timeoutSeconds)
{
	int timer = timerAlloc();
	timerStart(timer);
	while (*flag < target && timerElapsed(timer) < timeoutSeconds) {
		NMMonitor(1);
		netLinkMonitor(&s_clientLink, 0, clientHandleMsg);
		lnkBatchSend(&s_clientLink);
		Sleep(1);
	}
	timerFree(timer);
	return *flag >= target;
}

static void sendEcho(int ordered, int reliable, int compress, const char *text)
{
	Packet *pak = createTestPacket(&s_clientLink, CMD_ECHO, compress);
	EchoExpect *e = &s_expect[s_expectCount++];

	e->ordered = ordered;
	e->reliable = reliable;
	e->compress = compress;
	strcpy(e->text, text);
	e->matched = 0;

	pak->reliable = reliable;
	if (ordered)
		pktSetOrdered(pak, 1);
	pktSendBits(pak, 1, ordered);
	pktSendBits(pak, 1, reliable);
	pktSendBits(pak, 1, compress);
	pktSendString(pak, text);
	pktSend(&pak, &s_clientLink);
}

/* A transport-only client must not reserve listener slots indefinitely, even
 * while answering protocol pings and with the application's notimeout set.
 */
static void testIncompleteHandshakes(void)
{
	ENetHost *server = s_serverLinks.enet_host;
	ENetHost *raw[ENET_PROTOCOL_MAXIMUM_PEER_ID] = {0};
	ENetAddress address;
	ENetEvent event;
	int count = (int)server->peerCount;
	int allocs = s_serverAllocs, destroys = s_serverDestroys;
	int connected = 0, timer, i, activePings = 0;
	enet_uint32 lastPing;

	enet_address_set_host_ip(&address, "127.0.0.1");
	address.port = TEST_PORT;
	for (i = 0; i < count; i++) {
		// Separate sockets model distinct clients; a shared
		// address/port would deliberately replace the server's previous
		// link.
		raw[i] = enet_host_create(NULL, 1, 3, 0, 0);
		CHECK(raw[i] && enet_host_connect(raw[i], &address, 3, 0),
		      "handshake: transport-only peer created");
		if (!raw[i])
			break;
	}
	if (i < count) {
		while (i-- > 0)
			enet_host_destroy(raw[i]);
		return;
	}

	timer = timerAlloc();
	timerStart(timer);
	while ((connected < count || s_serverLinks.links->size < count) &&
	       timerElapsed(timer) < 3.0f) {
		for (i = 0; i < count; i++)
			while (enet_host_service(raw[i], &event, 0) > 0) {
				if (event.type == ENET_EVENT_TYPE_CONNECT)
					connected++;
				if (event.type == ENET_EVENT_TYPE_RECEIVE)
					enet_packet_destroy(event.packet);
			}
		NMMonitor(1);
		Sleep(1);
	}
	CHECK(connected == count && server->connectedPeers == count &&
		  s_serverLinks.links->size == count,
	      "handshake: transport-only peers fill listener capacity");
	for (i = 0; i < s_serverLinks.links->size; i++) {
		NetLink *link = s_serverLinks.links->storage[i];
		CHECK(!link->connected, "handshake: application not connected");
		link->notimeout = 1;
		netEnetApplyTimeouts(link);
	}

	lastPing = enet_time_get();
	timerStart(timer);
	while (timerElapsed(timer) < 6.0f) {
		if ((enet_uint32)(enet_time_get() - lastPing) >= 100) {
			for (i = 0; i < count; i++)
				if (raw[i]->peers[0].state ==
				    ENET_PEER_STATE_CONNECTED)
					enet_peer_ping(&raw[i]->peers[0]);
			lastPing = enet_time_get();
		}
		for (i = 0; i < count; i++)
			while (enet_host_service(raw[i], &event, 0) > 0)
				if (event.type == ENET_EVENT_TYPE_RECEIVE)
					enet_packet_destroy(event.packet);
		NMMonitor(1);
		if (timerElapsed(timer) > 2.0f && timerElapsed(timer) < 3.0f) {
			activePings = 0;
			for (i = 0; i < count; i++) {
				ENetPeer *peer = &raw[i]->peers[0];
				enet_uint32 age =
				    enet_time_get() - peer->lastReceiveTime;

				if (peer->state == ENET_PEER_STATE_CONNECTED &&
				    age < 500)
					activePings++;
			}
		}
		Sleep(1);
	}
	CHECK(activePings == count,
	      "handshake: every incomplete peer answers protocol pings");
	CHECK(s_serverAllocs == allocs + count &&
		  s_serverDestroys == destroys + count &&
		  s_serverLinks.links->size == 0,
	      "handshake: incomplete links expire exactly once despite pings");
	CHECK(server->connectedPeers == 0,
	      "handshake: all transport slots reclaimed");
	for (i = 0; i < count; i++)
		enet_host_destroy(raw[i]);

	CHECK(netConnect(&s_clientLink, "127.0.0.1", TEST_PORT, NLT_ENET, 15.0f,
			 serverPumpIdleCallback),
	      "handshake: normal client connects after capacity is reclaimed");
	pumpBoth(5);
	CHECK(s_serverLinks.links->size == 1 && s_clientLink.connected,
	      "handshake: normal application handshake completes");
	netSendDisconnect(&s_clientLink, 2.0f);
	for (i = 0; i < 50 && s_serverLinks.links->size; i++)
		NMMonitor(1);
	CHECK(s_serverDestroys == destroys + count + 1 &&
		  s_serverLinks.links->size == 0,
	      "handshake: normal client teardown fires once");
	timerFree(timer);
}

/* Main */

int main(int argc, char **argv)
{
	size_t paksBefore, bsBefore, paksAfter, bsAfter;
	int i;

	printf("EnetTest: NLT_ENET transport self-test\n");

	// Initialize allocator tracking and utilities before packet startup.
	memCheckInit();
	utilitiesLibStartup();

	packetStartup(0, 1); // encryption support on

	packetGetAllocationCounts(&paksBefore, &bsBefore);

	// ---- Server listen ----
	memset(&s_serverLinks, 0, sizeof(s_serverLinks));
	netLinkListAlloc(&s_serverLinks, 16, sizeof(TestClientData),
			 serverLinkAlloc);
	s_serverLinks.destroyCallback = serverLinkDestroy;
	s_serverLinks.encrypted = 1; // exercise the DH/Blowfish handshake
	CHECK(netInitEnet(&s_serverLinks, TEST_PORT, 0),
	      "server: netInitEnet binds");
	NMAddLinkList(&s_serverLinks, serverHandleMsg);

	// ---- Client connect (transport + in-band handshake) ----
	// netConnectEx blocks while connecting; in this single-process test the
	// server side is pumped from the idle callback it invokes between
	// slices.
	memset(&s_clientLink, 0, sizeof(s_clientLink));
	CHECK(netConnect(&s_clientLink, "127.0.0.1", TEST_PORT, NLT_ENET, 15.0f,
			 serverPumpIdleCallback),
	      "client: netConnect (transport + COMMCONTROL handshake)");

	CHECK(s_clientLink.receivedPacketID == NULL,
	      "client: ENet bypasses legacy duplicate-ID allocation");
	CHECK(s_clientLink.connected,
	      "client: link->connected set by handshake");
	CHECK(s_clientLink.encrypted && s_clientLink.encrypt_on &&
		  s_clientLink.decrypt_on,
	      "client: encryption negotiated on");
	CHECK(s_clientLink.protocolVersion == (U32)getDefaultNetworkVersion(),
	      "client: protocol version negotiated");
	CHECK(s_serverAllocs == 1, "server: allocCallback fired once");
	pumpBoth(5);
	CHECK(s_serverLinks.links && s_serverLinks.links->size == 1,
	      "server: one live link");
	if (s_serverLinks.links && s_serverLinks.links->size == 1) {
		NetLink *svrLink = s_serverLinks.links->storage[0];
		CHECK(svrLink->connected,
		      "server: link->connected set by handshake");
		CHECK(svrLink->encrypt_on && svrLink->decrypt_on,
		      "server: encryption on");
		CHECK(svrLink->type == NLT_ENET,
		      "server: accepted link is NLT_ENET");
	}

	// ---- Echo matrix across channels ----
	sendEcho(0, 1, 0, "plain reliable hello");     // ch1
	sendEcho(0, 0, 0, "unreliable hello");	       // ch2 unsequenced
	sendEcho(1, 1, 0, "ordered hello");	       // ch0
	sendEcho(1, 1, 1, "compressed ordered hello"); // ch0 + zlib stream
	sendEcho(1, 1, 1,
		 "compressed ordered hello again"); // stream continuity
	sendEcho(0, 1, 1,
		 "compressed unordered reliable");  // ch1 + stateless zlib
						    // (worldSendUpdate shape)
	sendEcho(0, 0, 1, "compressed unreliable"); // ch2 + stateless zlib
	sendEcho(0, 1, 0, "second plain reliable");

	CHECK(pumpUntil(&s_clientEchoesSeen, 8, 10.0f),
	      "echo matrix: all 8 replies received");
	for (i = 0; i < s_expectCount; i++) {
		if (!s_expect[i].matched) {
			printf("FAIL: echo not matched: '%s'\n",
			       s_expect[i].text);
			s_failures++;
		}
	}
	CHECK(s_serverEchoes == 8, "server: saw all 8 echoes");

	// ---- App-visible packet id ----
	// The server reads pak->id off the wire; it must equal the client's
	// send counter (nextID) for the most recent packet, ids assigned in
	// send order.
	CHECK(s_serverLastEchoPakId != 0 &&
		  s_serverLastEchoPakId <= s_clientLink.nextID,
	      "packet ids: wire id matches sender counter range");
	CHECK(s_clientLink.last_recv_id != 0,
	      "packet ids: client tracks last_recv_id");

	// ---- Big packet (ENet fragmentation; the legacy sibling path's job)
	// ----
	{
		static U8 big[BIG_PACKET_BYTES];
		U32 expect;
		Packet *pak;

		for (i = 0; i < BIG_PACKET_BYTES; i++)
			big[i] = (U8)(i * 2654435761u >> 13);
		expect = simpleChecksum(big, BIG_PACKET_BYTES);

		pak = pktCreateEx(&s_clientLink, CMD_BIG);
		pak->reliable = 1;
		pktSendBitsAuto(pak, BIG_PACKET_BYTES);
		pktSendBitsArray(pak, BIG_PACKET_BYTES * 8, big);
		pktSend(&pak, &s_clientLink);
		CHECK(
		    netLinkReliableBacklogPackets(&s_clientLink) > 0 &&
			netLinkReliableBacklogPackets(&s_clientLink) < 10,
		    "big packet: backlog counts packets rather than fragments");

		CHECK(pumpUntil(&s_clientBigReplySeen, 1, 20.0f),
		      "big packet: 1.5MB round trip");
		CHECK(s_clientBigReplySeen && s_clientBigReply == expect,
		      "big packet: checksum matches");
	}

	// ---- Reliable ordering sanity ----
	CHECK(s_orderedRecvIdRegressions == 0,
	      "ordered replies preserve packet ID sequence");

	// ---- Idle keepalives keep lastRecvTime fresh (no app traffic for 12s)
	// ---- ENet's protocol pings don't surface as receive events, so this
	// only works if in-band COMMCONTROL_IDLE traffic flows; app code (e.g.
	// Common/ClientLogin) reaps links whose lastRecvTime goes stale.
	{
		int timer = timerAlloc();
		timerStart(timer);
		while (timerElapsed(timer) < 12.0f) {
			NMMonitor(1);
			netLinkMonitor(&s_clientLink, 0, clientHandleMsg);
			netIdle(&s_clientLink, 0, 5);
			lnkBatchSend(&s_clientLink);
			Sleep(1);
		}
		timerFree(timer);
		CHECK(s_serverLinks.links->size == 1 && s_serverDestroys == 0,
		      "idle: link survives 12s with no app traffic");
		if (s_serverLinks.links->size == 1) {
			NetLink *svrLink = s_serverLinks.links->storage[0];
			CHECK(timerCpuSeconds() - svrLink->lastRecvTime < 8,
			      "idle: server lastRecvTime kept fresh by "
			      "keepalives");
		}
		CHECK(s_clientLink.idlePacketsReceived > 0,
		      "idle: client received server keepalives");
	}

	// ---- Simulated loss + lag (lnkSimulateNetworkConditions ENet shim)
	// ---- 15% inbound loss at the client (server->client datagrams dropped
	// before ENet's protocol sees them, so reliable replies must retransmit
	// through it), 80ms +/-30ms outbound lag with reordering on
	// client->server sends.
	{
		int before = s_clientEchoesSeen;
		int timer = timerAlloc();
		F32 firstReplyElapsed;
		char buf[64];

		lnkSimulateNetworkConditions(&s_clientLink, 80, 30, 15, 1);
		timerStart(timer);
		for (i = 0; i < 20; i++) {
			sprintf(buf, "lossy echo %d", i);
			sendEcho(0, 1, 0, buf);
		}
		for (i = 0; i < 16; i++) {
			sprintf(buf, "ordered lossy echo %d", i);
			sendEcho(1, 1, i % 2, buf);
		}
		// Guard against the shim silently not engaging: with an
		// 80ms(+/-30) outbound holdback the first reply cannot arrive
		// in under ~50ms.
		while (s_clientEchoesSeen == before &&
		       timerElapsed(timer) < 30.0f) {
			NMMonitor(1);
			netLinkMonitor(&s_clientLink, 0, clientHandleMsg);
			lnkBatchSend(&s_clientLink);
			Sleep(1);
		}
		firstReplyElapsed = timerElapsed(timer);
		CHECK(firstReplyElapsed >= 0.05f,
		      "sim: latency holdback actually delayed traffic");

		CHECK(
		    pumpUntil(&s_clientEchoesSeen, before + 36, 30.0f),
		    "sim: 36 reliable echoes survive loss, lag and reordering");
		CHECK(s_orderedRecvIdRegressions == 0,
		      "sim: ordered replies stay in sequence");
		timerFree(timer);

		lnkSimulateNetworkConditions(&s_clientLink, 0, 0, 0, 0);
		before = s_clientEchoesSeen;
		sendEcho(0, 1, 0, "post-sim echo");
		CHECK(pumpUntil(&s_clientEchoesSeen, before + 1, 10.0f),
		      "sim: clean recovery after disabling");
	}

	// ---- Ping bridge ----
	CHECK(pingAvgRate(&s_clientLink.pingHistory) >= 0,
	      "ping: history bridged");

	// ---- Accessors ----
	CHECK(netLinkSendQueueDepth(&s_clientLink) >= 0 &&
		  netLinkReliableBacklogPackets(&s_clientLink) >= 0 &&
		  netLinkUnackedCount(&s_clientLink) >= 0 &&
		  netLinkReliableBacklogBytes(&s_clientLink) >= 0,
	      "accessors: ENet branches respond");

	// ---- Disconnect + teardown ----
	// Both disconnect signals (in-band COMMCONTROL_DISCONNECT and the
	// transport-level ENet disconnect) reach the server here; the destroy
	// callback must still fire exactly once.
	netSendDisconnect(&s_clientLink, 2.0f);
	for (i = 0; i < 50 && s_serverDestroys < 1; i++)
		NMMonitor(1);
	for (i = 0; i < 20; i++)
		NMMonitor(1); // drain any straggling second signal
	CHECK(s_serverDestroys == 1,
	      "server: destroyCallback fired exactly once on disconnect");
	CHECK(s_serverLinks.links->size == 0, "server: link removed from list");

	// ---- Reconnect (fresh session on the same NetLink) ----
	CHECK(netConnect(&s_clientLink, "127.0.0.1", TEST_PORT, NLT_ENET, 15.0f,
			 serverPumpIdleCallback),
	      "reconnect: second netConnect succeeds");
	pumpBoth(5);
	CHECK(s_serverAllocs == 2 && s_serverLinks.links->size == 1,
	      "reconnect: server has one fresh link");
	{
		int before = s_clientEchoesSeen;
		sendEcho(0, 1, 0, "post-reconnect echo");
		CHECK(pumpUntil(&s_clientEchoesSeen, before + 1, 10.0f),
		      "reconnect: echo round trip");
	}
	netSendDisconnect(&s_clientLink, 2.0f);
	for (i = 0; i < 50 && s_serverDestroys < 2; i++)
		NMMonitor(1);
	CHECK(s_serverDestroys == 2 && s_serverLinks.links->size == 0,
	      "reconnect: clean second teardown");

	CHECK(netLinkTransportBytesReceived(&s_clientLink) == 0,
	      "teardown: client host released");
	CHECK(s_clientLink.enet_peer == NULL && s_clientLink.enet_host == NULL,
	      "teardown: client transport handles cleared");
	CHECK(s_clientLink.reliablePacketsArray.size == 0 &&
		  s_clientLink.ack_count == 0,
	      "teardown: no legacy reliability state retained");
	testIncompleteHandshakes();

	netLinkListDisconnect(&s_serverLinks);
	CHECK(!netEnetHostsRegistered(), "teardown: all ENet hosts released");

	// ---- Leak balance ----
	packetGetAllocationCounts(&paksAfter, &bsAfter);
	CHECK(paksAfter == paksBefore && bsAfter == bsBefore,
	      "leaks: packet pools balanced");

	packetShutdown();
	CHECK(!netEnetStarted(), "shutdown: ENet deinitialized");
	printf("Unsequenced packet ID regressions: %d\n",
	       s_clientRecvIdRegressions);
	printf("\nEnetTest: %s (%d failure%s)\n", s_failures ? "FAILED" : "OK",
	       s_failures, s_failures == 1 ? "" : "s");
	return s_failures ? 1 : 0;
}
