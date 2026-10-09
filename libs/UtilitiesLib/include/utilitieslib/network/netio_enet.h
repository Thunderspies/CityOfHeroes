/* ENet transport behind the existing netio and Packet/BitStream APIs.
 *
 * ENet owns transport sequence numbers, acknowledgements, retransmission,
 * fragmentation and flow control. The in-band COMMCONTROL handshake retains
 * version negotiation and DH/Blowfish key exchange. Application packet IDs
 * remain available for input sequencing, validation and packet logging.
 *
 * Each ENetPacket carries two little-endian U32 values: exact wire bit length
 * and Adler-32 of the padded body. The body contains the debug flag, packet
 * ID, negotiated compression/ordering fields and serialized payload. When
 * encryption is enabled, the checksum and padded body are Blowfish-encrypted;
 * the wire bit length remains readable. COMM_CONNECT omits optional fields.
 *
 * Channel 0: reliable handshake and ordered traffic, including streaming zlib.
 * Channel 1: reliable unordered application traffic and oversized unreliable
 *            packets promoted to reliable delivery for fragmentation.
 * Channel 2: unsequenced unreliable traffic and idle keepalives.
 * Stateless compressed packets follow their reliability/ordering policy.
 *
 * These functions are internal transport integration points. Serialize access
 * with the network critical section, or use them from the single network
 * thread. Initialization/shutdown must not overlap any other network calls.
 * Pointers must be live and non-NULL unless a function documents otherwise.
 * No function transfers ownership of NetLink or NetLinkList storage.
 */
#ifndef NETIO_ENET_H
#define NETIO_ENET_H

#include "../stdtypes.h"
#include "net_structdefs.h"

C_DECLARATIONS_BEGIN

/* Initialize ENet once after socket startup. maxPacketSize is the requested
 * MTU in bytes, clamped to ENet limits when a host is created. Allocations use
 * the engine CRT callbacks. Initialization failure triggers an assertion and
 * leaves netEnetStarted() false. Repeated successful startup is a no-op.
 */
void netEnetStartup(int maxPacketSize);

/* Deinitialize ENet after all links/list hosts are torn down. A no-op when
 * stopped; does not destroy live hosts or release caller-owned link storage.
 */
void netEnetShutdown(void);

/* Return nonzero if ENet initialization succeeded and has not been shut down.
 */
int netEnetStarted(void);

/* Advance an NLT_ENET client connection on initialized, caller-owned link.
 * ip_str is a non-NULL address/hostname, port is in 1..65535, and sliceSeconds
 * is a service budget (nonpositive selects the default slice). Startup must
 * have completed. Creates a link-owned host/peer on the first attempt.
 * Returns 1 when transport connects; 0 while pending or on failure. A failed
 * peer attempt clears the link so a later call may retry. Does not complete
 * the application handshake; netConnectEx drives that separately.
 */
int netEnetConnect(NetLink *link, const char *ip_str, int port,
		   F32 sliceSeconds);

/* Bind an ENet listener on enet_port (1..65535) after startup and
 * netLinkListAlloc. nlist must have no ENet host. expectedLinks sizes peer
 * capacity with reconnect headroom; tcp_port optionally retains a TCP listener
 * (0 disables it). Returns 1 on success, 0 on bind/setup failure. nlist owns
 * the registered host and releases it through netLinkListDisconnect.
 */
int netInitEnet(NetLinkList *nlist, int enet_port, int tcp_port);

/* Detach peers and destroy/unregister nlist's host, if any. The ordinary list
 * disconnect/removal flow remains responsible for the links and callbacks.
 */
void netEnetListShutdown(NetLinkList *nlist);

/* Return nonzero when at least one server or client host is registered. */
int netEnetHostsRegistered(void);

/* Poll all registered hosts, advance simulation queues and process transport
 * events. Receives queue application packets; connect events may add links.
 * Application dispatch and link reaping remain the caller's responsibility.
 */
void netEnetServiceAll(void);

/* Release due simulated packets and flush outgoing traffic on every host. */
void netEnetFlushAll(void);

/* Service link's host, if present. maxWaitMs must be nonnegative; 0 polls.
 * Events for other links sharing this host are also processed.
 */
void netEnetServiceLink(NetLink *link, int maxWaitMs);

/* Release due simulated sends and flush link's host, if present. */
void netEnetFlushLink(NetLink *link);

/* Submit an already compressed packet to an NLT_ENET link. pakptr and
 * *pakptr must be non-NULL. Consumes/frees *pakptr and sets it to NULL on
 * success and failure. Returns 1 when queued, 0 when dropped/rejected.
 * Updates packet IDs/statistics and invokes packetSentCallback on success;
 * overflow follows the existing netio overflow policy. Does not flush.
 */
U32 pktSendEnet(Packet **pakptr, NetLink *link);

/* Return nonzero while link has a connecting/connected transport peer.
 * This does not establish that the application handshake has completed.
 */
int netEnetLinkAlive(const NetLink *link);

/* Discard held simulation packets, detach/reset the peer and destroy the
 * host only if link owns it. Clears transport handles, leaving a shared
 * server host intact. clearNetLink handles the remaining packet queues.
 */
void netEnetLinkTeardown(NetLink *link);

/* Apply link->notimeout to its peer, if present. Ordinary connections tolerate
 * a 60-second acknowledgement gap; notimeout uses an extended timeout.
 */
void netEnetApplyTimeouts(NetLink *link);

/* Request socket buffer size in bytes on nlist's host, if present. type is
 * a SendBuffer/ReceiveBuffer mask. OS adjustment failure is not reported.
 */
void netEnetSetSocketBufferSize(NetLinkList *nlist, int type, int size);

/* Synchronize link's simulation settings. Inbound loss drops raw datagrams
 * before ENet sees them. Outbound lag/jitter delays application packets;
 * reordering shuffles only unordered traffic. Ordered sends remain FIFO.
 * ENet retransmissions bypass the delay. Disabling simulation releases held
 * packets and removes interception.
 */
void netEnetSimApply(NetLink *link);

/* Approximate reliable application packets queued/in flight. A fragmented
 * packet spanning pending and sent lists may count twice, rather than once
 * per fragment. Legacy links report their reliable packet array size.
 * All accessors below accept NULL and return 0 for absent transport state.
 */
int netLinkReliableBacklogPackets(const NetLink *link);

/* Reliable bytes in flight for ENet; buffered reliable payload bytes for
 * legacy links. Units differ with transport framing and fragmentation.
 */
int netLinkReliableBacklogBytes(const NetLink *link);

/* Unacknowledged sent reliable commands for ENet (fragments count separately),
 * or the difference between sent and acknowledged packet IDs for legacy links.
 */
int netLinkUnackedCount(const NetLink *link);

/* Outgoing ENet command queue depth, including fragments, or legacy send
 * queue depth. This excludes ENet commands already awaiting acknowledgement.
 */
int netLinkSendQueueDepth(const NetLink *link);

/* Raw ENet-host receive bytes, including traffic for every peer on a shared
 * listener. Client hosts have one peer. Returns 0 for non-ENet links.
 */
U32 netLinkTransportBytesReceived(const NetLink *link);

/* Raw ENet-host sent bytes, including retransmissions and control traffic.
 * Counts every peer on a shared listener; returns 0 for non-ENet links.
 */
U32 netLinkTransportBytesSent(const NetLink *link);

/* ENet peer waiting-data bytes, including pending fragment reassembly.
 * Returns 0 for non-ENet links.
 */
U32 netLinkReassemblyBytesPending(const NetLink *link);

C_DECLARATIONS_END

#endif
