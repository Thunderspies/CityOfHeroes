/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "cmdparse/cmdserver.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "group/group.h"
#include "group/grouputil.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include "language/commonLangUtil.h"
#include "comm_game.h"
#include "svr/svr_base.h"

#include "entity/entity.h"
#include "player/badges_server.h"

#include "plaque.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/language/AppLocale.h>

typedef struct Plaque
{
    char *pchName;
    char *pchMessage;
    Vec3 pos;
} Plaque;

static Plaque **g_Plaques = NULL;

/**********************************************************************func*
 * plaque_Create
 *
 */
static Plaque *plaque_Create(void)
{
    return calloc(1, sizeof(Plaque));
}

/**********************************************************************func*
 * plaque_Destroy
 *
 */
static void plaque_Destroy(Plaque *p)
{
    if(p->pchName)
    {
        free(p->pchName);
    }

    free(p);
}

/**********************************************************************func*
 * plaque_LocationRecorder
 *
 */
static int plaque_LocationRecorder(GroupDefTraverser* traverser)
{
    char *pchPlaque;
    Plaque *pPlaque;

    // We are only interested in nodes with properties.
    if(!traverser->def->properties)
    {
        // We're only interested in subtrees with properties.
        if(!traverser->def->child_properties)
        {
            return 0;
        }

        return 1;
    }

    // Does this thing have a plaque?
    pchPlaque = groupDefFindPropertyValue(traverser->def, "Plaque");
    if(pchPlaque==NULL)
    {
        return 1;
    }

    pPlaque = plaque_Create();
    copyVec3(traverser->mat[3], pPlaque->pos);
    pPlaque->pchName = strdup(pchPlaque);

    eaPush(&g_Plaques, pPlaque);

    return 1;
}


MultiMessageStore *g_msgs_Plaque = NULL;

/**********************************************************************func*
 * plaque_Load
 *
 */
static void plaque_DestroyAdapter(void* arg0)
{
    plaque_Destroy((Plaque *)arg0);
}

void plaque_Load(void)
{
    GroupDefTraverser        traverser = {0};
    int                        locale = getCurrentLocale();
    MessageStoreLoadFlags    flags = MSLOAD_SERVERONLY;

    char *dirs[] = {
        "texts/server/plaques",
        NULL
    };

    loadstart_printf("Loading plaque locations.. ");

    // If we're doing a reload, clear out old data first.
    if(g_Plaques)
    {
        eaClearEx(&g_Plaques, plaque_DestroyAdapter);
        eaSetSize(&g_Plaques, 0);
    }
    else
    {
        eaCreate(&g_Plaques);
    }

    if(server_state.create_bins)
    {
        locale = -1; // load all locales
        flags |= MSLOAD_FORCEBINS;
    }

    LoadMultiMessageStore(&g_msgs_Plaque,"plaquemsg","g_msgs_Plaque",locale,NULL,NULL,dirs,installCustomMessageStoreHandlers,flags);

    groupProcessDefExBegin(&traverser, &group_info, plaque_LocationRecorder);

    loadend_printf("%i found", eaSize(&g_Plaques));
}


/**********************************************************************func*
 * plaque_ReceiveReadPlaque
 *
 */
bool plaque_ReceiveReadPlaque(Packet *pak, Entity *e)
{
    char *pch;
    Vec3 pos;
    Plaque *pPlaque;
    int i, found = false;

    // CLIENTINP_READ_PLAQUE
    // string name of plaque
    // position:
    //   float vec3[0]
    //   float vec3[1]
    //   float vec3[2]
    pch = pktGetString(pak);
    pos[0] = pktGetF32(pak);
    pos[1] = pktGetF32(pak);
    pos[2] = pktGetF32(pak);

    for(i=eaSize(&g_Plaques)-1; i>=0; i--)
    {
        if(stricmp(g_Plaques[i]->pchName, pch)==0)
        {
            found = true;
            pPlaque = g_Plaques[i];
            if(distance3Squared(ENTPOS(e), pPlaque->pos) < SQR(20))
            {
                // They can see the plaque.
                plaque_SendPlaque(e, pPlaque->pchName);
                badge_RecordPlaque(e, pPlaque->pchName);
            }
        }
    }

    return found;
}

/**********************************************************************func*
 * plaque_SendPlaque
 *
 */
bool plaque_SendPlaque(Entity *e, char *pchName)
{
    int size;
    char *pch;

    size = msxPrintf(g_msgs_Plaque, NULL, 0, e->playerLocale, pchName, NULL, NULL, 0);
    
    if(size >= 0)
    {
        pch = _alloca(size+1);
        msxPrintf(g_msgs_Plaque, pch, size + 1, e->playerLocale, pchName, NULL, NULL, 0);

        if(stricmp(pchName, pch)!=0)
        {
            // SERVER_RECEIVE_DIALOG
            // text of plaque:
            //    string

            START_PACKET(pak, e, SERVER_PLAQUE_DIALOG)
                pktSendString(pak, pch);
            END_PACKET

            return true;
        }
    }
    
    return false;
}

bool plaque_SendPlaqueNag(Entity *e, char *pchName, int count)
{
    int size;
    char *pch;

    size = msxPrintf(g_msgs_Plaque, NULL, 0, e->playerLocale, pchName, NULL, NULL, 0);

    if(size >= 0)
    {
        pch = _alloca(size+1);
        msxPrintf(g_msgs_Plaque, pch, size + 1, e->playerLocale, pchName, NULL, NULL, 0);

        if(stricmp(pchName, pch)!=0)
        {
            // SERVER_RECEIVE_DIALOG
            // text of plaque:
            //    string

            START_PACKET(pak, e, SERVER_RECEIVE_DIALOG_NAG)
                pktSendString(pak, pch);
                pktSendBitsPack(pak,4,count);
            END_PACKET

            return true;
        }
    }

    return false;
}

void plaque_SendString(Entity *e, char *str)
{
    START_PACKET(pak,e,SERVER_PLAQUE_DIALOG);
    pktSendString(pak,printLocalizedEnt(str,e));
    END_PACKET
}

/* End of File */
