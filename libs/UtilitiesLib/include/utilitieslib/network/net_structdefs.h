#ifndef NET_STRUCTDEFS_H
#define NET_STRUCTDEFS_H


#include "../utils/wininclude.h"
#include "../stdtypes.h"
#include "../network/net_typedefs.h"
#include "blowfish.h"
#include "../components/Queue.h"
#include "../components/ArrayOld.h"
#include "../components/SimpleSet.h"
#include "../components/MemoryPool.h"
#include "../utils/memcheck.h"
#include "../components/BitStream.h"

C_DECLARATIONS_BEGIN

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct NetMasterListElement NetMasterListElement;
typedef struct z_stream_s *z_stream_ptr;
typedef struct LinkToMultiplexer LinkToMultiplexer;

/****************************************************************************************************
 * From net_stats.h                                                                                    *
 ****************************************************************************************************/
#define MAX_PKTHIST 16
typedef struct
{
    F32        times[MAX_PKTHIST];
    F32        bytes[MAX_PKTHIST];
    int        idx;
    U32        last_time;

    F32        lastTimeSum;
    F32        lastByteSum;
} PktHist;

#define MAX_PINGHIST 32
typedef struct{
    U16 nextEntry;                // Where should the next entry go?
    U16 size;                    // How large is the ping history right now?
    U16 pings[MAX_PINGHIST];
    
    S32 lastPing;
    S32 lastPingSum;
    U32 inBatch:1;
} PingHistory;

/****************************************************************************************************
 * From net_async.h                                                                                    *
 ****************************************************************************************************/
typedef enum AsyncOpType{
    AOT_None,
    AOT_Send,
    AOT_Receive,
    AOT_StoredRecv,
} AsyncOpType;

typedef struct AsyncOpContext {
    AsyncOpType type;
    int bytesTransferred;
    int time;
    OVERLAPPED ol;
    Packet* pak;
    int inProgress;
} AsyncOpContext;

typedef enum{
    ASU_None,
    ASU_ExtractBody,
    ASU_ExtractBodyComplete,
} AsyncUdpIOState;

/****************************************************************************************************
 * From net_link.h                                                                                    *
 ****************************************************************************************************/
// basic packet types used by the net_link module.

enum
{
    COMM_CONTROLCOMMAND = 0,    // Network control commands, if none specified, assume idle (for backwards compat)
    COMM_CONNECT = 1,            // Connect packet has to be out here for backwards compat =(
    COMM_MAX_CMD = 2,
};

#define COMM_OLDMAX_CMD    7

// low-level network subchannel commands
typedef enum
{
    COMMCONTROL_IDLE=0,
    COMMCONTROL_CONNECT,                // I want to connect
    COMMCONTROL_CONNECT_SERVER_ACK,        // Ok, you can connect
    COMMCONTROL_CONNECT_CLIENT_ACK,        // Right, I'm connected
    COMMCONTROL_CONNECT_SERVER_ACK_ACK,    // needed for encryption handshake
    COMMCONTROL_DISCONNECT,
    COMMCONTROL_DISCONNECT_ACK,
    COMMCONTROL_BUFFER_RESIZE,            // The sender resized his send buffer, you better resize your receive buffer!
    COMMCONTROL_MAX,
} CommControlCommand;
    

enum { LOOK_FOR_COOKIE = 0x7ffffffe };

#define LINK_DISCONNECTED -2

typedef enum NetLinkType
{
    NLT_NONE,
    NLT_TCP,
    NLT_UDP,
} NetLinkType;

typedef enum NetLinkOperationType
{
    NLOT_NONE,
    NLOT_SYNC,
    NLOT_ASYNC,
} NetLinkOperationType;

typedef enum{
    AST_None,                    // Not performing Async TCP IO ops.
    AST_WaitingToStart,            // Waiting to start receive data.
    AST_ExtractHeader,            
    AST_ExtractHeaderComplete,    
    AST_ExtractBody,
    AST_ExtractBodyComplete,
} AsyncTcpIOState;

/* Structure PacketTimeoutGroup
 *    This structure is used to track the approximate timeout of a group of packets.  
 *
 *    These structures will be used to produce a single sample in the packet lost history.  
 *    When the packets are sent, one such structure is created, filled, and stored into the 
 *    pakTimeoutArray.  Another function will check on array each tick.  When the timeout
 *    time expires, a single packet lost rate sample is created by checking on which packets
 *    in the range of "minPacketID" and "maxPacketID" have been acknowledged.
 *
 *    See lnkSamplePacketLostRate();
 *
 *    This is not currently used in the packet code.  If the packet code needs better flow
 *    control, use this structure to get a better idea of packet lost over time.  Note that
 *    this will not work well for apps that have low packet rates.
 */
typedef struct{
    unsigned int minPacketID;
    unsigned int maxPacketID;
    unsigned int timeoutTime;    // When will the group timeout?
} PacketTimeoutGroup;

typedef struct{
    unsigned int pakID;
    unsigned int sendTime;
} PacketSendTime;
#define MAX_SENDTIMESSIZE 32

#define MAX_PKTACKS 128 // With bitpacking, this number seems alright

#define NO_TIMEOUT 0
#define POLL -1

struct NetLink
{
    NetLinkType type;                // What kind of NetLink is this?
    NetLinkOperationType opType;    // What kind of operations does this link perform?

    U32                    UID;    // Some number that uniquely identifies the netlink
    SOCKET                socket;    // Which socket does this NetLink send its data on?                
    struct sockaddr_in    addr;    // Where does this link connect to?
    
    U32        lastSendTime;        // When was the last packet sent on this link?
    U32        lastSendTimeSeconds;    // When was the last packet sent on this link? (in the same unites as lastRecvTime
    U32        lastRecvTime;        // When was the last packet received from this link?
    
    // Persistent statistics
    // These statistics are tallied through the lifetime of the link.
    U32        totalBytesSent;        // How many bytes were sent on this link since connection?
    U32        totalBytesRead;        // How many bytes were read on this link since connection?
    U32        totalPacketsSent;    // How many packets were sent on this link since connection?
    U32        totalPacketsRead;    // How many packets were read on this link since connection?
    U32        reliablePacketsQueued;
    U32        totalQueuedBytes;
    F32     runningCompressionRatio; // rough idea of how good the compression is

    // Some stat tracking (for debugging)
    int        idlePacketsReceived;

    // Packet ID assignment and reconstruction related variables.
    U32        nextSibID;
    U32        nextID;                // Current outgoing id.
    U32        idAck;                // Highest acknowledgement id received.
    
    // TCP specific variables
    Packet* pak;
    int        bytesRead;
    int        bytesToRead;

    // Async operation related variables.
    AsyncTcpIOState    asyncTcpState;
    int        asyncTcpBytesRead;

    // EnhanceME!!!
    // The Aysnc networking code is doing more work than it ought to right now.  A single packet is split
    // up into two seperate async reads.  It might improve performance quite a bit if we can determine that
    // the async read for the body is not neccessary because it always arrive together with the header.
    int        asyncReadBodyCount;            // How many times did the link try to async read a packet body?
    int        asyncReadBodyCompletedImm;    // How many times did it succeed immediately?

    Queue    retransmitQueue;    // Packet to be retransmitted.
    Queue    sendQueue2;            // Packets to be sent the next tick. DO NOT set a max queue size on this
    Queue    receiveQueue;        // Packets received this tick.
    int        sendQueuePacketCount;
    int        sendQueuePacketMax;    // Max number of "real" packets to allow queuing

    Array    receivedPacketStorage;

    int        rpaCleanAge;            // How long has it been since the reliable packets array was cleaned?
    Array    reliablePacketsArray;    // Array of reliable packets that are waiting for acknowledgement.
    Array    siblingMergeArray;        // Array of sibling packets that are ready to be merged.

    AsyncOpContext sendContext;        
    AsyncOpContext receiveContext;
    AsyncOpContext storedRecvContext; // received packets that couldn't be handled immediately
    
    Array siblingWaitingArea;    // A set that maps sibling id to array of packets that are waiting for their siblings.
                                    // This allows fast sibling processing.
    SimpleSet* receivedPacketID;

    U32        simulateNetworkCondition    : 1;
    U32        flowControl                    : 1;    // Attempt to do flow control/bandwidth limiting?
    U32        retransmit                    : 1;    // Attempt to retransmit unacknowledged packets
    U32        disableRetransmits            : 1;    // Temporarily disable queueing of unacknowledged packets
    U32        connected                    : 1;
    U32        pending                        : 1;    // Tells whether this link has an asynchronous connect attempt in progress
    U32        connecting                    : 1;    // For a link that's pending, this flags that it's awaiting the peer ack, rather than awaiting the connect(...) completion
    U32        disconnected                : 1;    // Tells whether the link is connected.
    U32        logout_disconnect            : 1;    // If the client timed out or is force logged, this is set so certain dbflags can be cleared.
    U32        removedFromMasterList        : 1;    // Tells whether the link has been removed from the master list after being disconnected.
    U32        destroyed                    : 1;    // destroyCallback has been invoked on this
    U32        waitingForFlush                : 1;    // This link is waiting to be flushed.
    U32        awaitingAsyncSendCompletion : 1;    // This link is waiting for an async send operation to complete.
    U32        alwaysProcessPacket            : 1;    // This link will not use the "stored packet" facility.
    U32        compressionAllowed            : 1;    // This link uses zlib data compression.
    U32        compressionDefault            : 1;    // Defaults to compressions if available

    U32        orderedAllowed                : 1;    // This link uses data ordered.
    U32        orderedDefault                : 1;    // Defaults to ordered if available

    U32        cookieSend                    : 1;    // This link will send a U32 cookie in every pktSendCmd
    U32        cookieEcho                    : 1;    // This link will echo back the latest cookie in every pktSendCmd

    U32        controlCommandsSeparate        : 1;    // This link sends control commands on a separate channel (otherwise it sends inflated command numbers)

    U32        protocolVersion;                    // The version of the network protocol this link uses

    U32        cookie_recv;    // The last cookie received to be echoed (cleared after echo)
    U32        cookie_send;    // The last cookie sent (auto-incremented on every pktCreateEx)
    U32        last_cookie_recv;    // Non-cleared copy of the last non-0 cookie_recv value
    U32        last_cookie_recv_echo;    // Echo of the other side's last non-0 cookie_recv value
    U32        last_control_command_packet_id; // Packet ID of the last control command we received
    U32        last_processed_id;    // Packet ID of the last packet we processed (will be the last_recv_id at the tme of sending the last cookie we're waiting on)


    // FIXME!!
    // Ideally, the "connected" and the "dead" flag should be the same flag.
    // However, since most of the apps do not use the connection negotiation handshake built into the
    // packet code, it is not a good idea to rely on the "connected" flag to decide whether a link is disconnected
    // or not.

    // Simulated Network Condition settings.
    int        simulatedLagTime;    
    int        simulatedLagVary;
    int        simulatedPacketLostRate;
    int        simulatedOutOfOrderDelivery;

    // FIXME!!!
    //    flow control is very poorly implemented right now.
    // Flow control related variables.
    U32        bandwidthUpperLimit;
    U32        bandwidthLowerLimit;
    U32        estimatedBandwidth;        // in bytes per second.
    U32        availableTickBandwidth;
    U32        bandwidthCheckpointID;    // Highest packet ID when bandwidth adjustment was last made.
    //F32        ping;                    // Last round trip time.
    Array    pakTimeoutArray;        // 
    //PacketTimeoutGroup* pakTimeoutGroup;
    
    PacketSendTime pakSendTimes[MAX_SENDTIMESSIZE];
    int        pakSendTimesSize;

    // Packets to
    U32        ids_toack[MAX_PKTACKS];
    int        ack_count;

    U32        recv_acks[MAX_PKTACKS];
    int        recv_ack_count;

    Queue    pkt_temp_store; // temporarily store packets here if they're not the ones you're looking for

    U32        last_recv_id;
    int        lost_packet_sent_count;
    int        lost_packet_recv;
    int        lost_packet_recv_count;
    int        duplicate_packet_recv_count;
    int     ooo_ordered_packet_recv_count;

    U32        last_ordered_sent_id;
    U32        last_ordered_recv_id;
    Array    orderedWaitingPaks;

    U32        last_update_time;
    int        last_world_id;

    U32        connection_id;    // random id generated by connection client

    PktHist    sendHistory;
    PktHist    recvHistory;
    PingHistory pingHistory;

    void*    userData;
    NetLinkList*    parentNetLinkList;    // Messy. But needed to help manage NetLinkLists.
    NetMasterListElement *parentElement; // who owns this in the masterlist

    // Link exception signals.
    int        overflow;
    
    NetLinkTransmitCallback*    transmitCallback;
    NetPacketSentCallback*        packetSentCallback;

    U32        connect_id;
    U8        encrypt_on;
    U8        decrypt_on;
    U8        encrypted;
    U32        private_key[512/32];
    U32        public_key[512/32];
    U32        shared_secret[512/32];
    BLOWFISH_CTX blowfish_ctx;

    U32        sendBufferSize; // Current size of the send buffer (low-level network buffer size)
    U32        recvBufferSize; // Current size of the receive buffer (low-level network buffer size)
    U32        sendBufferMaxSize;
    U32        recvBufferMaxSize;
    U32        lastAutoResizeTime; // To make sure we don't very quickly bloat a link
    
    U32        linkArrayIndex;

    U32        notimeout:1;        // Flag this to not run timeout code (IsDebuggerPresent checks have now been removed)

    z_stream_ptr    zstream_pack;
    z_stream_ptr    zstream_unpack;

    //if this link is used to attach a normal server to a multiplexing server, this points
    //to the manager for that attachment
    LinkToMultiplexer *pLinkToMultiplexer;

    // Asynchronous connect
    int        asyncTimer;            // timer used to detect timeout during an async connect attempt
    float    asyncTimeout;        // timeout duration
};

/****************************************************************************************************
 * From net_linklist.h                                                                                *
 ****************************************************************************************************/


struct NetLinkList
{
    SOCKET                    listen_sock;
    SOCKET                    udp_sock;
    int                        expectedLinks;
    int                        encrypted;
    int                        keepAlive;        // Sends all links keep alive messages
    int                        inNetMasterList;    // Is true if the list has been added to the NetMasterList.

    AsyncUdpIOState         asyncUdpState;
    int                        asyncUdpBytesRead;
    struct sockaddr_in        asyncRecvAddr;
    int                        asyncRecvAddrLen;
    AsyncOpContext            receiveContext;

    Array*                    links;
    MemoryPool                 netLinkMemoryPool;
    MemoryPool                 userDataMemoryPool;
    
    NetLinkAllocCallback    *allocCallback;
    NetLinkDestroyCallback    *destroyCallback;
    U32                        destroyLinkOnDiscard:1; // invoke destroyCallback when link is discarded
    NetPacketCallback        *packetCallback;
    
    StashTable                netLinkTable;

    U32                        sendBufferSize; // Current size of the send buffer (low-level network buffer size)
    U32                        recvBufferSize; // Current size of the receive buffer (low-level network buffer size)
    U32                        sendBufferMaxSize;
    U32                        recvBufferMaxSize;
    U32                        lastAutoResizeTime; // To make sure we don't very quickly bloat a link

    U32                        hasDisconnectedLinks:1; // Flag set when a link disconnects
    U32                        publicAccess:1;            // non-private IPs are allowed to connect (default is no)
};



/****************************************************************************************************
 * From net_masterlist.h                                                                            *
 ****************************************************************************************************/


/***************************************************************************************************
 * Structure NetMasterListElement
 *    An NetMasterListElement represents a single element in the NetMasterList.
 *
 *    Fields:
 *        type - indicates the type of construct pointed to by the "storage" field.
 *        packetCallback - function to be called with a NetLink or a NetLinkList receives a packet.
 *        storage - points to either a NetLink, a NetLinkList, or NMSock depending on the "type" field.
 *        key - key value passed to IO Completion Ports, looked up through net_masterlistref.h
 */
typedef enum{
    NM_Link,
    NM_LinkList,
    NM_Sock,
} NetMasterListElementType;

typedef struct NetMasterListElement{
    NetMasterListElementType type;
    NetPacketCallback* packetCallback;
    void* storage;
    U32 key;
} NetMasterListElement;

/***************************************************************************************************
 * Structure NMSock
 *    An NMSock is socket with enough additional information for the net master list
 *    to know what to do with it once some data arrives on the socket.
 *
 *    Fields:
 *        sock - The socket the NMSock represents.
 *        processSocket - function to be called when data arrives on the socket.
 *        userData -    some user data to be associated with the socket so that socket identity or
 *                    state can be established.
 *
 */
struct NMSock;
typedef void NMSockCallback(struct NMSock* sock);
typedef struct NMSock{
    SOCKET sock;
    NMSockCallback* processSocket;
    void* userData;
} NMSock;




/****************************************************************************************************
 * From net_packet.h                                                                                *
 ****************************************************************************************************/


typedef struct Packet
{
    U32        UID;                     // unique across all links. For debugging purposes only.
    U32        id;                      // unique across a single link.
    U32        truncatedID;
    
    U32        reliable            : 1;
    U32        hasDebugInfo        : 1;
    U32        inRetransmitQueue   : 1;
    U32        inSendQueue         : 1; // For debugging only
    U32        compress            : 1; // Packet should be compressed
                                        // when sending (and the payload
                                        // byteAligned). for UDP must be
                                        // reliable and ordered too.
    U32        ordered             : 1; // packets with this bit will be handled in order
    U32        ordered_id;

    S32        sib_id;
    S32        sib_count;
    S32        sib_partnum;

    U32        xferTime;
    U32        creationTime;
    U32        retransCount;
    
    // Packet data
    U32        checksum;
    BitStream stream;
    void*    userData;    //if you expect whatever this is pointing to to get destroyed on packet destroy, you better assign a delUserDataCallback.
    PacketUserDataDestroyCallback *delUserDataCallback;

    NetLink    *link;            // link this packet was created on, if created using pktCreateEx
#ifdef TRACK_PACKET_ALLOC
    char    stack_dump[2000];
#endif
} Packet;

typedef struct{
    U32    packetBitLength;    // total packet size in bits to expect
    U32    packetChecksum;        // packet checksum
} PacketHeader;
//#define PACKET_HEADER_SIZE    sizeof(PacketHeader)


/****************************************************************************************************
 * From net_socket.h                                                                                *
 ****************************************************************************************************/
typedef enum{
    SendBuffer=1,
    ReceiveBuffer=2,
    BothBuffers=SendBuffer|ReceiveBuffer,
} NetLinkBufferType;



#define NETLINK_SENDQUEUE_INIT_SIZE        128
#define NETLINK_SENDQUEUE_MAX_SIZE        16384

C_DECLARATIONS_END

#endif
