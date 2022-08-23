///////////////////////////////////////////////////////////////
//
// testClientCmdParse.c

#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/utils.h>
#include "UI/uiTurnstile.h"
#include "testClientCmdParse.h"
#include "turnstile/TurnstileCommon.h"
#include "clientcomm/clientcomm.h"
#include "entity/entity.h"
#include "player/player.h"
#include "testClient/testLevelup.h"
#include "testClientInclude.h"

static void handle_levelup(int argc, char **argv)
{
    if (playerPtr() && argc > 1)
    {
        char buffer1000[1001];
        int    levelNumber = atoi(argv[1]);
        sprintf_s(SAFESTR(buffer1000), "levelupxp %d", levelNumber);
        commAddInput(buffer1000);

        testLevelup(levelNumber);
    }
}

static void handle_quit(int argc, char **argv)
{
    Entity *e = playerPtr();
    if (e) // quit anyway if e is NULL...
        e->logout_login = 0;
    commSendQuitGame(0);
    sendMessageToLauncher("QuitNow:");
}

static void handle_packetlogging(int argc, char **argv)
{
    if (argc > 1)
    {
        int    packetValue = atoi(argv[1]);
        commLogPackets(packetValue);
    }
}

static void handle_lfgRequestEventList(int argc, char **argv)
{
    turnstileGame_generateRequestEventList();
}

static void handle_lfgQueueForEvents(int argc, char **argv)
{
    int i;
    int eventCount = 0;
    int eventList[MAX_EVENT];

    if (argc >= 2)
    {
        for (i = 1; i < argc; i++)
        {
            eventList[eventCount++] = atoi(argv[i]);
        }
        turnstileGame_generateQueueForEvents(eventList, eventCount);
    }
}

static void handle_lfgRemoveFromQueue(int argc, char **argv)
{
    turnstileGame_generateRemoveFromQueue();
}

static void handle_lfgEventResponse(int argc, char **argv)
{
    if (argc >= 2)
    {
        turnstileGame_generateEventResponse(argv[1]);
    }
}

static void handle_accept(int argc, char **argv)
{
    void sendTeamAccept(void *data);

    sendTeamAccept(NULL);
}

static void handle_chan(int argc, char **argv)
{
    if (argc >= 3)
        sendShardCmd(argv[1], "%s %s", argv[2], argc >= 4 ? argv[3] : "0", NULL);
}

#define CMD(cmd)    if (stricmp(keyword, #cmd) == 0) { handle_##cmd(argc, argv); return 1; }

int testClientCmdParse(char *cmdx)
{
    char *cmd;
    char *argv[100];
    int argc;
    int i;
    int j;
    char *keyword;

    strdup_alloca(cmd, cmdx);
    argc = tokenize_line(cmd, argv, 0);
    if (argc == 0)
    {
        return 0;
    }

    // Strip underscores from the command
    strdup_alloca(keyword, argv[0]);
    for (i = j = 0; keyword[i]; i++)
    {
        if (keyword[i] != '_')
        {
            keyword[j++] = keyword[i];
        }
    }
    keyword[j] = 0;

    CMD(levelup);
    CMD(quit);
    CMD(packetlogging);
    CMD(lfgRequestEventList);
    CMD(lfgQueueForEvents);
    CMD(lfgRemoveFromQueue);
    CMD(lfgEventResponse);
    CMD(accept);
    CMD(chan);

    return 0;
}