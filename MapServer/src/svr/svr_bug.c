/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>

#include "svr_base.h"
#include "entity/entserver.h"
#include "dbcomm/dbnamecache.h"
#include "comm_game.h"
#include <utilitieslib/version/AppVersion.h>
#include "cmdparse/cmdservercsr.h"
#include "storyarc/storyarcprivate.h"
#include "dbcomm/dbcomm.h"
#include "storyarc/mission.h"
#include "beacon/beaconFile.h"
#include "entity/entity.h"
#include "dbcomm/logcomm.h"

/**********************************************************************func*
 * bugStartReport
 *
 */
char *bugStartReport(char **pestr, int mode, char *desc)
{
    estrClear(pestr);

    // description
    if((mode == BUG_REPORT_NORMAL) || (mode == BUG_REPORT_CSR))
        estrConcatf(pestr, "\n%s\n", desc); // normal /bugs are already formated with Category/Summary/Desc info
    else
        estrConcatf(pestr, "\nDescription: %s\n", desc);

    return *pestr;
}

/**********************************************************************func*
 * bugAppendVersion
 *
 */
char *bugAppendVersion(char **pestr, char *clientVersion)
{
    const char *serverVersion = getExecutablePatchVersion("CoH Server");

    if(0 == stricmp(clientVersion, serverVersion))
    {
        // only print one line if they're identical
        estrConcatf(pestr, "Client/Server Ver: %s\n", clientVersion);
    }
    else
    {
        estrConcatf(pestr, "Client Ver: %s\nServer Ver: %s\n", clientVersion, serverVersion);
    }

    return *pestr;
}

/**********************************************************************func*
* bugAppendMissionLine
*
*/
char* bugAppendMissionLine(char **pestr, Entity* player)
{

    // Only log if the player was actively on a mission map
    if (player && g_activemission)
    {
        const StoryTask* taskdef = TaskDefinition(&g_activemission->sahandle);
        const StoryArc* arcdef = NULL;
        if (isStoryarcHandle(&g_activemission->sahandle))
            arcdef = StoryArcDefinition(&g_activemission->sahandle);
        if (taskdef)
        {
            if (arcdef)
                estrConcatf(pestr, "\n\nMission:\nstartstoryarcmission %s %s %s\n", arcdef->filename, taskdef->logicalname, db_state.map_name);
            else
                estrConcatf(pestr, "\n\nMission:\nstartmission %s %s\n", taskdef->logicalname, db_state.map_name);
        }
    }
    return *pestr;
}

/**********************************************************************func*
 * bugAppendPosition
 *
 */
char *bugAppendPosition(char **pestr, char *mapName, Vec3 pos, Vec3 pyr, float camdist, int third)
{
    // position info (this format was copied from 'setdebugpos')
    estrConcatf(pestr, "\n\nPosition:\nloadmap %s\nsetpospyr %1.2f %1.2f %1.2f %1.4f %1.4f %1.4f\ncamdist %1.4f\nthird %d\n\n",
        mapName,
        pos[0], pos[1], pos[2],
        pyr[0], pyr[1], pyr[2],
        camdist,
        third);

    return *pestr;
}

/**********************************************************************func*
 * bugAppendClientInfo
 *
 */
char *bugAppendClientInfo(char **pestr, ClientLink *client, char *playerName)
{
    Entity *e = entFromDbId(dbPlayerIdFromName(playerName));
    if(e)
    {
        // player info
        csrPlayerInfo(pestr, client, playerName, 1);

        // tray info
        csrPlayerTrayInfo(pestr, client, playerName);

        // specialization info
        csrPlayerSpecializationInfo(pestr, client, playerName);

        csrPlayerTeamInfo(pestr, client, playerName);

        // detailed contact, task & storyarc info
        estrConcatStaticCharArray(pestr, "\n\nStory Info:\n");
        StoryArcDebugPrintHelper(pestr, client, e, 1);
    }

    return *pestr;
}

/**********************************************************************func*
 * bugAppendServerInfo
 *
 */
char *bugAppendServerInfo(char **pestr)
{
    if(beaconGetReadFileVersion())
        estrConcatf(pestr, "Beacon Version: %d\n", beaconGetReadFileVersion());
    else
        estrConcatStaticCharArray(pestr, "Beacon Version: NO BEACON FILE FOR THIS MAP!!!\n");

    return *pestr;
}

/**********************************************************************func*
 * bugEndReport
 *
 */
char *bugEndReport(char **pestr)
{
    estrConcatStaticCharArray(pestr, "@@END\n\n\n\n\n\n\n\n\n\n");
    return *pestr;
}

/**********************************************************************func*
 * bugLogBug
 *
 */
void bugLogBug(char **pestr, int mode, Entity *e)
{
    switch(mode)
    {
    case BUG_REPORT_SURVEY:
            // flag bug as "survey" (for survey responses)
            LOG(LOG_SURVEY, LOG_LEVEL_IMPORTANT, 0, "\n(Survey)%s", *pestr);
            break;

        case BUG_REPORT_LOCAL:
            // creates bug log on client's machine
            if (e)
            {
                START_PACKET(pak, e, SERVER_BUG_REPORT);
                pktSendString(pak, *pestr);
                END_PACKET
            }
            break;

        case BUG_REPORT_INTERNAL:
            // flag bug as "internal" (to allow faster processing of internally-created /bug entries)
            dbLogBug("\n(Internal)%s", *pestr);
            break;

        case BUG_REPORT_NORMAL:
            dbLogBug("\n(Normal)%s", *pestr);
            break;

        case BUG_REPORT_CSR:
            dbLogBug("\n(CSR)%s", *pestr);
            break;

        default:
            dbLogBug("\n(Unknown)%s", *pestr);
    }

    estrDestroy(pestr);
}


/* End of File */
