#include "testNetworkStats.h"
#include <utilitieslib/network/netio_enet.h>
#include "clientcomm/clientcomm.h"
#include <stdio.h>
#include "testUtil.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>

static int nwstats_enabled=0;

void closeNetworkStats(void) {
    printf("\n");
}

void openNetworkStats(void) {

}

void enableNetworkStats(int enable)
{
    int newflag = enable;
    if (newflag==-1) newflag = !nwstats_enabled;
    if (!newflag && nwstats_enabled) {
        closeNetworkStats();
    } else if (newflag && !nwstats_enabled) {
        openNetworkStats();
    }
    nwstats_enabled = newflag;
}

#define NUM_PACKETS_BEFORE_DUMP 100

void calcNetworkStats(void) {
    static int dumped=0;
    static int inited=0;
    static int timer=-1;
    int firsttime=0;
    if (!inited) {
        timer = timerAlloc();
        inited = 1;
        firsttime=1;
    }
    if (!nwstats_enabled)
        return;

    if (!dumped && comm_link.totalPacketsRead >= NUM_PACKETS_BEFORE_DUMP) {
        char buf[1024];
        sprintf(buf, "Received %d packets in %1.3gs, stats:\n", NUM_PACKETS_BEFORE_DUMP, timerElapsed(timer));
        strcatf(buf, "  Total bytes/packets read/sent: %d(%d) / %d(%d)  \n", comm_link.totalBytesRead, comm_link.totalPacketsRead, comm_link.totalBytesSent, comm_link.totalPacketsSent);
        {
            U32 effbytesread = comm_link.totalBytesRead * NUM_PACKETS_BEFORE_DUMP / comm_link.totalPacketsRead;
            strcatf(buf, "  Effective bytes read on first %d packets: %d\n", NUM_PACKETS_BEFORE_DUMP, effbytesread);
        }
        printf("%s", buf);
        testClientLog(TCLOG_STATS, "%s", buf);
        dumped=1;
    }

    if (!firsttime) {
	    printf("Total bytes/pkts read/sent: %d(%d) / %d(%d)  lost r/s: "
		   "%d/%d  reliable backlog: %d send queue: %d in %1.3gs\r",
		   comm_link.totalBytesRead, comm_link.totalPacketsRead,
		   comm_link.totalBytesSent, comm_link.totalPacketsSent,
		   comm_link.lost_packet_recv_count,
		   comm_link.lost_packet_sent_count,
		   netLinkReliableBacklogPackets(&comm_link),
		   netLinkSendQueueDepth(&comm_link), timerElapsed(timer));
    } else {
	    printf("Total bytes/pkts read/sent: %d(%d) / %d(%d)\r",
		   comm_link.totalBytesRead, comm_link.totalPacketsRead,
		   comm_link.totalBytesSent, comm_link.totalPacketsSent);
    }
}
