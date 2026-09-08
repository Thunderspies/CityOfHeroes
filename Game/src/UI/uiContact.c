/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include "gameComm/wdwbase.h"
#include "storyarc/contactClient.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/timing.h>
#include "gameComm/Npc.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/utils.h>

#include "graphics/seqgraphics.h"

#include "UI/uiUtil.h"
#include "UI/uiTabControl.h"
#include "UI/uiInput.h"
#include "UI/uiCompass.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiContact.h"
#include "UI/uiScrollBar.h"
#include "UI/uiComboBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiAutomap.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiOptions.h"

#include "formatter/smf_main.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include "group/groupnetrecv.h"
#include "edit/ClickToSource.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/playerCreatedStoryarcClient.h"
#include "auth/authUserData.h"
#include <utilitieslib/components/StringCache.h>

void ContactSendCellCall(int handle);

typedef int (*CompareFunc)(const int *a, const int *b);

// The selected contact. Selected either by player directly, or server, or
// indirectly by choosing a task.
//int g_IdxSelectedContact = -1;


//------------------------------------------------------------------------

#define CONTACT_HT      64
#define CONTACT_SPACE   5
#define CBOXHEIGHT 21

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taComplete =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x00ff33ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x00ff33ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

// Caches the formatting for each contact. Parsing and formatting are rather
// slow, so it's good to cache it.
static SMFBlock **s_ppBlocks = NULL;
static ToolTip **s_ppToolTips = NULL;
static int *contactSortList = NULL;

static SMFBlock *s_topAlignmentName = NULL;
static SMFBlock *s_midAlignmentName = NULL;
static SMFBlock *s_botAlignmentName = NULL;

static SMFBlock *s_pAlignmentPointTimer = NULL;
static SMFBlock *s_tipProgressLabel = NULL;

static uiTabControl *contactCategories = NULL;

static bool s_bReparse = 0;

typedef enum ContactSortType
{
    kContactSortType_Default = 0,
    kContactSortType_Name,
    kContactSortType_Zone,
    kContactSortType_Relationship,
    kContactSortType_Active,

    // last
    kContactSortType_Count
} SalvageSortType;


void contactReparse(void)
{
    s_bReparse = true;
}

//----------------------------------------------------------------------------------
// Contact sorts
//----------------------------------------------------------------------------------

static int isBrokerContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    if (csa->isNewspaper || csa->tipType)
        return false;

    if(!csa->completeCP && !csa->confidantCP && !csa->friendCP)
    {
        return true;
    } else {
        return false;
    }
}

// corresponds to ContactIsActive on the server
static int isActiveContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    if (csa->tipType)
        return false;

    if (csa->metaContact)
        return true;

    if (csa->wontInteract)
        return false;

    if (csa->isNewspaper)
        return true;

    return (csa->notifyPlayer || csa->hasTask || ((unsigned int) csa->taskcontext > 0));
}

static int isGoingRogueTipContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    return csa->tipType == TIPTYPE_ALIGNMENT || csa->tipType == TIPTYPE_MORALITY;
}

static int isDesignerTipContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    return csa->tipType > TIPTYPE_STANDARD_COUNT;
}

static int isSpecialTipContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    return csa->tipType == TIPTYPE_SPECIAL;
}

static int isNewspaperContact(int id)
{
    ContactStatus *csa = NULL; 
    int contactCount = eaSize(&contacts); 

    if (id < 0 || id >= contactCount)
        return 0;

    csa = contacts[id];

    return csa->isNewspaper;
}


static int compareContactsByActiveThenFunc(const int *a, const int *b, CompareFunc f)
{  
    ContactStatus *csa = NULL;
    ContactStatus *csb = NULL;
    int contactCount = eaSize(&contacts);
    int aHasTask, bHasTask;

    if (*a < 0 || *a >= contactCount || *b < 0 || *b >= contactCount)
        return 0;

    csa = contacts[*a];
    csb = contacts[*b];

    if (!csa || !csb)
        return 0;

    if (csa->isNewspaper || csa->tipType)
        return 1;
    if (csb->isNewspaper || csb->tipType)
        return -1;

    if (csa->metaContact)
        return 1;
    if (csb->metaContact)
        return -1;

    if (!isActiveContact(*a) && !isActiveContact(*b))
        return f(a, b);

    aHasTask = ((unsigned int) csa->taskcontext > 0);
    bHasTask = ((unsigned int) csb->taskcontext > 0);
    if ( aHasTask && bHasTask )
    {
        return f(a, b);
    } else if (aHasTask) {
        return 1;
    } else if (bHasTask) {
        return -1;
    } else {
        aHasTask = (csa->notifyPlayer || csa->hasTask);
        bHasTask = (csb->notifyPlayer || csb->hasTask);
        if ( (aHasTask && bHasTask) || (!aHasTask && !bHasTask))
        {
            return f(a, b);
        } else if (aHasTask) {
            return 1;
        } else {
            return -1;
        }
    }
}

static int compareContactsByName(const int *a, const int *b)
{  
    ContactStatus *csa = NULL;
    ContactStatus *csb = NULL;
    int contactCount = eaSize(&contacts);

    if (*a < 0 || *a >= contactCount || *b < 0 || *b >= contactCount)
        return 0;

    csa = contacts[*a];
    csb = contacts[*b];

    if (!csa || !csb)
        return 0;

    if (csa->isNewspaper || csa->tipType)
        return 1;
    if (csb->isNewspaper || csb->tipType)
        return -1;

    if (csa->name && csb->name)
        return -strcmp(csa->name, csb->name);
    else if (!csb->name)
        return 1;
    else
        return -1;
}

static int compareContactsByLocationDescription(const int *a, const int *b)
{
    ContactStatus *csa = NULL;
    ContactStatus *csb = NULL;
    int contactCount = eaSize(&contacts);
    int locationSort;

    if (*a < 0 || *a >= contactCount || *b < 0 || *b >= contactCount)
        return 0;

    csa = contacts[*a];
    csb = contacts[*b];

    if (!csa || !csb)
        return 0;

    if (csa->isNewspaper || csa->tipType)
        return 1;
    if (csb->isNewspaper || csb->tipType)
        return -1;

    if (csa->locationDescription && csb->locationDescription)
    {
        locationSort = -strcmp(csa->locationDescription, csb->locationDescription);
        if (locationSort != 0)
            return locationSort;
        else 
            return compareContactsByName(a, b);
    }
    else if (!csb->locationDescription)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

static int compareContactsByZone(const int *a, const int *b)
{  
    ContactStatus *csa = NULL;
    ContactStatus *csb = NULL;
    int contactCount = eaSize(&contacts);

    if (*a < 0 || *a >= contactCount || *b < 0 || *b >= contactCount)
        return 0;

    csa = contacts[*a];
    csb = contacts[*b];

    if (!csa || !csb)
        return 0;

    if (csa->isNewspaper || csa->tipType)
        return 1;
    if (csb->isNewspaper || csb->tipType)
        return -1;

    if (csa->location.id == 0 && csb->location.id == 0)
    {
        return compareContactsByLocationDescription(a, b);
    } else if (csa->location.id != 0 && csb->location.id != 0) {
        int aCurr, bCurr;

        aCurr = (stricmp(csa->location.mapName, gMapName) != 0);
        bCurr = (stricmp(csb->location.mapName, gMapName) != 0);

        if ((aCurr && bCurr) || (!aCurr && !bCurr))
        {
            return compareContactsByLocationDescription(a, b);
        } else if (aCurr) {
            return 1;
        } else {
            return -1;
        }
    } else if (csa->location.id != 0 && csb->location.id == 0) {
        if (stricmp(csa->location.mapName, gMapName) == 0)
            return 1;
        else
            return compareContactsByLocationDescription(a, b);
    } else {
        if (stricmp(csb->location.mapName, gMapName) == 0)
            return 1;
        else
            return compareContactsByLocationDescription(a, b);
    }
}

static int compareContactsByRelationship(const int *a, const int *b)
{  
    ContactStatus *csa = NULL; 
    ContactStatus *csb = NULL;
    int contactCount = eaSize(&contacts);
    float aRel, bRel;
    int completea = 0;
    int completeb = 0;

    if (*a < 0 || *a >= contactCount || *b < 0 || *b >= contactCount)
        return 0;

    csa = contacts[*a];
    csb = contacts[*b];

    if (!csa || !csb)
        return 0;

    completea = csa->completeCP?csa->completeCP:csa->heistCP;
    completeb = csb->completeCP?csb->completeCP:csb->heistCP;

    if (completea == 0 && completeb == 0)
        return 0;

    if (csa->isNewspaper)
        return 1;
    if (csb->isNewspaper)
        return -1;

    if (completea == 0)
        aRel = 0.0f;
    else 
        aRel = (float) csa->currentCP / (float) completea;

    if (completeb == 0)
        bRel = 0.0f;
    else 
        bRel = (float) csb->currentCP / (float) completeb;

    if (aRel == bRel)
    {
        return compareContactsByName(a, b);
    } else if (aRel > bRel) {
        return 1;
    } else {
        return -1;
    }
}


static int compareContactsByActiveShell(const void* aData, const void* bData)
{
    const int * a = (const int *)aData;
    const int * b = (const int *)bData;
    return compareContactsByActiveThenFunc(a, b, compareContactsByName);
}

static int compareContactsByNameShell(const void* aData, const void* bData)
{
    const int * a = (const int *)aData;
    const int * b = (const int *)bData;
    return compareContactsByActiveThenFunc(a, b, compareContactsByName);
}

static int compareContactsByRelationshipShell(const void* aData, const void* bData)
{
    const int * a = (const int *)aData;
    const int * b = (const int *)bData;
    return compareContactsByActiveThenFunc(a, b, compareContactsByRelationship);
}

static int compareContactsByZoneShell(const void* aData, const void* bData)
{
    const int * a = (const int *)aData;
    const int * b = (const int *)bData;
    return compareContactsByActiveThenFunc(a, b, compareContactsByZone);
}


static void contactApplySortList(int *pList, int sortOrder)
{

    int contactCount = eaiSize(&pList);

    switch (sortOrder)
    {
    case kContactSortType_Zone:
        qsort(contactSortList, contactCount, sizeof(int), compareContactsByZoneShell);
        break;
    default:
    case kContactSortType_Default:
    case kContactSortType_Active:
        qsort(contactSortList, contactCount, sizeof(int), compareContactsByActiveShell);
        break;
    case kContactSortType_Relationship:
        qsort(contactSortList, contactCount, sizeof(int), compareContactsByRelationshipShell);
        break;
    case kContactSortType_Name:
        qsort(contactSortList, contactCount, sizeof(int), compareContactsByNameShell);
        break;
    }
}

//----------------------------------------------------------------------------------
// Contact context menu
//----------------------------------------------------------------------------------
ContextMenu* contactContextMenu = 0;
//static int pendingContactHandle = 0;

static CMVisType contactCMHasTask(int pendingContactSelection)
{
    ContactStatus* contact;

    // Make sure a valid contact is being selected.
    contact = eaGet(&contacts, pendingContactSelection);
    if (!contact)
        return CM_HIDE;

    // Which task is the contact associated with?
    if (contact->taskcontext > 0)
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

static void contactCMSetTask(int pendingContactSelection)
{
    ContactStatus* contact;
    TaskStatus* task;

    contact = eaGet(&contacts, pendingContactSelection);
    if (!contact) return;
    task = TaskStatusFromContact(contact);
    if (task)
        compass_SetTaskDestination(&waypointDest, task);
}

static CMVisType contactCMHasLocation(int pendingContactSelection)
{
    // Make sure a valid contact is being selected.
    if(pendingContactSelection < 0 || pendingContactSelection >= eaSize(&contacts))
        return CM_HIDE;

    // Set the contact as the way point.
    if(contacts[pendingContactSelection]->hasLocation)
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

static void contactCMSetWaypoint(int pendingContactSelection)
{
    // Make sure a valid contact is being selected.
    if(pendingContactSelection < 0 || pendingContactSelection >= eaSize(&contacts))
        return;

    // Set the contact as the way point.
    if(contacts[pendingContactSelection]->hasLocation)
        compass_SetContactDestination(&waypointDest, contacts[pendingContactSelection]);
    else
        clearDestination(&waypointDest);
}

static void contactContextMenuSetup(int idx)
{
    if(!contactContextMenu)
        contactContextMenu = contextMenu_Create(NULL);

    contextMenu_setCode(contactContextMenu, 
        (CMVisible)contactCMHasTask, (void*)idx, 
        (CMCode)contactCMSetTask, (void*)idx, 
        "CMSelectTask", 0);

    contextMenu_setCode(contactContextMenu, 
        (CMVisible)contactCMHasLocation, (void*)idx, 
        (CMCode)contactCMSetWaypoint, (void*)idx, 
        "CMSelectContact", 0);
}


/**********************************************************************func*
 * displayRelationshipBar
 *
 */
void displayRelationshipBar(float x, float y, float z, float sc, ContactStatus *pcs)
{
#define BAR_WIDTH  212
#define BAR_HEIGHT 5
    AtlasTex *bar = white_tex_atlas;
    AtlasTex *cap = atlasLoadTexture("healthbar_framemain_R.tga");
    float fScale;
    char *pch;

    fScale = (float)(BAR_WIDTH-2)/(float)(pcs->completeCP?pcs->completeCP:pcs->heistCP);

    display_sprite(bar, x, y, z, sc*BAR_WIDTH/bar->width, sc*BAR_HEIGHT/bar->height, 0x00feffa0);
    display_sprite(bar, x+sc*1, y+sc*1, z, sc*(BAR_WIDTH-2)/bar->width, sc*(BAR_HEIGHT-2)/bar->height, CLR_BLACK);

    display_sprite(bar, x+sc*1, y+sc*1, z, sc*pcs->currentCP*fScale/bar->width, sc*(BAR_HEIGHT-2)/bar->height, pcs->completeCP? 0x2b5cd2ff : 0xc00000ff);

    if(pcs->friendCP<pcs->confidantCP)
    {
        display_sprite(bar, x+sc*pcs->friendCP*fScale, y+sc*1, z, 2.0f/bar->width, sc*(BAR_HEIGHT-2)/bar->height, 0x00feffa0);
    }
    if(pcs->confidantCP<pcs->completeCP)
    {
        display_sprite(bar, x+sc*pcs->confidantCP*fScale, y+sc*1, z, 2.0f/bar->width, sc*(BAR_HEIGHT-2)/bar->height, 0x00feffa0);
    }

    if(!pcs->completeCP && !pcs->confidantCP && !pcs->friendCP)
    {
        if (pcs->alliance == ALLIANCE_HERO)
            pch = "Relbar_Detective";
        else 
            pch = "Relbar_Broker";
    }
    else if(pcs->currentCP>=pcs->completeCP)
        pch = "Relbar_Confidant";
    else if(pcs->currentCP>=pcs->confidantCP)
        pch = "Relbar_Friend";
    else if(pcs->currentCP>=pcs->friendCP)
        pch = "Relbar_Collegue";
    else
        pch = "Relbar_Acquaintance";

    font(&game_9);
    font_color(CLR_PARAGON, CLR_PARAGON);
    cprnt(x+sc*(BAR_WIDTH+30), y+sc*6, z, sc*.8f, sc*.8f, textStd(pch));
}

static int newspaperAnim = 0;
static int newsOpen = 0;
static void displayNewspaper( int idx, int xorig, float *y, int z, int wdorig, float sc, int color, int draw)
{
    ContactStatus *cs = contacts[idx];
    CBox box;
    static AtlasTex *newspaper = 0;
    static AtlasTex *newspaper_halffold = 0;
    static AtlasTex *newspaper_folded = 0;
    static AtlasTex *police_a = 0;
    static AtlasTex *police_b = 0;
    int ht = 0;
    static float timer = 0;

    if (!cs->name || !cs->name[0]) // we may have a NULL entry for the mission
        return;

    if (!draw && !s_bReparse) {
        *y += (64+PIX3*2+4+4+CONTACT_SPACE) * sc;
        return;
    }
    if (CTS_SHOW_STORY)
        ClickToSourceDisplay(xorig+92*sc, *y+70*sc, z, 0, 0xffffffff, cs->filename, NULL, CTS_TEXT_REGULAR);

    if (!newspaper)
    {
        newspaper = atlasLoadTexture("Newspapercontact_title_a.tga");
        newspaper_halffold = atlasLoadTexture("Newspapercontact_title_b.tga");
        newspaper_folded = atlasLoadTexture("Newspapercontact_title_c.tga");
        police_a = atlasLoadTexture("police_scanner_01a.tga");
        police_b = atlasLoadTexture("police_scanner_01b.tga");        
    }

    if (cs->alliance == ALLIANCE_HERO)
    {
        if (newspaperAnim == 0 || newspaperAnim == 1)
            display_sprite(police_a, xorig, *y, z, sc, sc, CLR_WHITE);
        else
            display_sprite(police_b, xorig, *y, z, sc, sc, CLR_WHITE);
    }
    else
    {
        if (newspaperAnim == 0)
            display_sprite(newspaper, xorig, *y, z, sc, sc, CLR_WHITE);
        else if (newspaperAnim == 1)
            display_sprite(newspaper_halffold, xorig, *y, z, sc, sc, CLR_WHITE);
        else
            display_sprite(newspaper_folded, xorig, *y, z, sc, sc, CLR_WHITE);
    }

    ht = max(ht+(38+CONTACT_SPACE)*sc, max((64+PIX3*2+4+4)*sc, CONTACT_HT*sc));
    BuildCBox( &box, xorig, *y, wdorig, ht);
    if( mouseCollision( &box ))
    {
        newspaperAnim++;
        newspaperAnim = min(newspaperAnim, 2);
        if(mouseDown(MS_LEFT))
        {
            ContactSendCellCall(cs->handle);
            //cmdParse("newspaper");
            newsOpen = 1;
        }
    }
    else
    {
        newspaperAnim--;
        newspaperAnim = max(newspaperAnim, 0);
    }

    if (newsOpen)
        newspaperAnim = 2;
    
    if (window_getMode( WDW_CONTACT_DIALOG ) == WINDOW_SHRINKING)
        newsOpen = 0;


    *y += ht;
    *y += CONTACT_SPACE*sc; // Space between contacts
}

static ToolTip uiContact_alignmentTooltip;

void displayAlignmentStatus(Entity *e, int x, float *y, int z, int wd, float scale, int color, int isSelf, ToolTip *alignmentTooltip)
{
    static AtlasTex *heroIcon = 0, *vigilanteIcon = 0, *villainIcon = 0, *rogueIcon = 0;
    static AtlasTex *smallHeroIcon = 0, *smallVigilanteIcon = 0, *smallVillainIcon = 0, *smallRogueIcon = 0, *glowIcon = 0;
    AtlasTex *bar = white_tex_atlas;
    AtlasTex *cap = atlasLoadTexture("healthbar_framemain_R.tga");
    AtlasTex* icon = NULL;
    AtlasTex* topIcon = NULL;
    AtlasTex* midIcon = NULL;
    AtlasTex* botIcon = NULL;
    int heroPoints, vigilantePoints, villainPoints, roguePoints;
    int topValue = 0, topMax = 0;
    float topPerc = 0;
    int midValue = 0, midMax = 0;
    float midPerc = 0;
    int botValue = 0, botMax = 0;
    float botPerc = 0;
    char* topAlignmentName = NULL;
    char* midAlignmentName = NULL;
    char* botAlignmentName = NULL;
    static CBox alignBox_self, alignBox_other;
    static CBox topBox_self, topBox_other;
    static CBox midBox_self, midBox_other;
    static CBox botBox_self, botBox_other;
    int glowAlpha = 0xffffff00 + ((int) ((float) abs(1000 - (timerMsecsSince2000() % 2000)) / 1000 * 0xa0));

    clearToolTip(alignmentTooltip); 

    if (!e || !e->pl)
    {
        return;
    }

    if (!s_topAlignmentName)
    {
        s_topAlignmentName = smfBlock_Create();
        smf_SetFlags(s_topAlignmentName, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, 0, SMAlignment_Left, 0, 0, 0);
        s_midAlignmentName = smfBlock_Create();
        smf_SetFlags(s_midAlignmentName, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, 0, SMAlignment_Left, 0, 0, 0);
        s_botAlignmentName = smfBlock_Create();
        smf_SetFlags(s_botAlignmentName, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, 0, SMAlignment_Left, 0, 0, 0);
    }

    if (isSelf)
    {
        BuildCBox(&alignBox_self, x + 5*scale, *y + 5*scale, 70*scale, 70*scale);
        BuildCBox(&topBox_self, x + 92*scale, *y + 3*scale, wd - 105*scale, 19*scale);
        BuildCBox(&midBox_self, x + 92*scale, *y + 28*scale, wd - 105*scale, 19*scale);
        BuildCBox(&botBox_self, x + 92*scale, *y + 53*scale, wd - 105*scale, 19*scale);
    }
    else
    {
        BuildCBox(&alignBox_other, x + 5*scale, *y + 5*scale, 70*scale, 70*scale);
        BuildCBox(&topBox_other, x + 92*scale, *y + 3*scale, wd - 105*scale, 19*scale);
        BuildCBox(&midBox_other, x + 92*scale, *y + 28*scale, wd - 105*scale, 19*scale);
        BuildCBox(&botBox_other, x + 92*scale, *y + 53*scale, wd - 105*scale, 19*scale);
    }


    // Determine data
    if (!heroIcon)
    {
        heroIcon = atlasLoadTexture("Tips_Hero_large.texture");
        vigilanteIcon = atlasLoadTexture("Tips_Vigilante_large.texture");
        villainIcon = atlasLoadTexture("Tips_Villain_large.texture");
        rogueIcon = atlasLoadTexture("Tips_Rogue_large.texture");
        smallHeroIcon = atlasLoadTexture("Tips_Hero_22px.texture");
        smallVigilanteIcon = atlasLoadTexture("Tips_Vigilante_22px.texture");
        smallVillainIcon = atlasLoadTexture("Tips_Villain_22px.texture");
        smallRogueIcon = atlasLoadTexture("Tips_Rogue_22px.texture");
        glowIcon = atlasLoadTexture("Glow_ring.texture");
    }

    if (isSelf)
    {
        heroPoints = heroAlignmentPoints_self;
        vigilantePoints = vigilanteAlignmentPoints_self;
        villainPoints = villainAlignmentPoints_self;
        roguePoints = rogueAlignmentPoints_self;
    }
    else
    {
        heroPoints = heroAlignmentPoints_other;
        vigilantePoints = vigilanteAlignmentPoints_other;
        villainPoints = villainAlignmentPoints_other;
        roguePoints = rogueAlignmentPoints_other;
    }

    if (e->pl->playerType == kPlayerType_Hero)
    {
        if (e->pl->playerSubType == kPlayerSubType_Paragon)
        {
            // Paragon, NYI
        }
        else if (e->pl->playerSubType == kPlayerSubType_Rogue)
        {
            smf_SetRawText(s_topAlignmentName, textStd("VigilanteLookingAtHeroString"), false);
            smf_SetRawText(s_midAlignmentName, textStd("VigilanteLookingAtVigilanteString"), false);
            smf_SetRawText(s_botAlignmentName, textStd("VigilanteLookingAtVillainString"), false);

            topAlignmentName = textStd("HeroAlignment");
            midAlignmentName = textStd("VigilanteAlignment");
            botAlignmentName = textStd("VillainAlignment");

            topValue = heroPoints;
            topMax = maxAlignmentPoints_Hero;
            topPerc = ((float) heroPoints) / maxAlignmentPoints_Hero;
            midValue = vigilantePoints;
            midMax = maxAlignmentPoints_Vigilante;
            midPerc = ((float) vigilantePoints) / maxAlignmentPoints_Vigilante;
            botValue = villainPoints;
            botMax = maxAlignmentPoints_Villain;
            botPerc = ((float) villainPoints) / maxAlignmentPoints_Villain;

            icon = vigilanteIcon;
            topIcon = smallHeroIcon;
            midIcon = smallVigilanteIcon;
            botIcon = smallVillainIcon;
        }
        else
        {
            smf_SetRawText(s_topAlignmentName, "", false);
            smf_SetRawText(s_midAlignmentName, textStd("HeroLookingAtHeroString"), false);
            smf_SetRawText(s_botAlignmentName, textStd("HeroLookingAtVigilanteString"), false);

            topAlignmentName = "";
            midAlignmentName = textStd("HeroAlignment");
            botAlignmentName = textStd("VigilanteAlignment");

            topValue = 0;
            topMax = 0;
            topPerc = 0.0f;
            midValue = heroPoints;
            midMax = maxAlignmentPoints_Hero;
            midPerc = ((float) heroPoints) / maxAlignmentPoints_Hero;
            botValue = vigilantePoints;
            botMax = maxAlignmentPoints_Vigilante;
            botPerc = ((float) vigilantePoints) / maxAlignmentPoints_Vigilante;

            icon = heroIcon;
            topIcon = 0;
            midIcon = smallHeroIcon;
            botIcon = smallVigilanteIcon;
        }
    }
    else
    {
        if (e->pl->playerSubType == kPlayerSubType_Paragon)
        {
            // Tyrant, NYI
        }
        else if (e->pl->playerSubType == kPlayerSubType_Rogue)
        {
            smf_SetRawText(s_topAlignmentName, textStd("RogueLookingAtVillainString"), false);
            smf_SetRawText(s_midAlignmentName, textStd("RogueLookingAtRogueString"), false);
            smf_SetRawText(s_botAlignmentName, textStd("RogueLookingAtHeroString"), false);

            topAlignmentName = textStd("VillainAlignment");
            midAlignmentName = textStd("RogueAlignment");
            botAlignmentName = textStd("HeroAlignment");

            topValue = villainPoints;
            topMax = maxAlignmentPoints_Villain;
            topPerc = ((float) villainPoints) / maxAlignmentPoints_Villain;
            midValue = roguePoints;
            midMax = maxAlignmentPoints_Rogue;
            midPerc = ((float) roguePoints) / maxAlignmentPoints_Rogue;
            botValue = heroPoints;
            botMax = maxAlignmentPoints_Hero;
            botPerc = ((float) heroPoints) / maxAlignmentPoints_Hero;

            icon = rogueIcon;
            topIcon = smallVillainIcon;
            midIcon = smallRogueIcon;
            botIcon = smallHeroIcon;
        }
        else
        {
            smf_SetRawText(s_topAlignmentName, "", false);
            smf_SetRawText(s_midAlignmentName, textStd("VillainLookingAtVillainString"), false);
            smf_SetRawText(s_botAlignmentName, textStd("VillainLookingAtRogueString"), false);

            topAlignmentName = "";
            midAlignmentName = textStd("VillainAlignment");
            botAlignmentName = textStd("RogueAlignment");

            topValue = 0;
            topMax = 0;
            topPerc = 0.0f;
            midValue = villainPoints;
            midMax = maxAlignmentPoints_Villain;
            midPerc = ((float) villainPoints) / maxAlignmentPoints_Villain;
            botValue = roguePoints;
            botMax = maxAlignmentPoints_Rogue;
            botPerc = ((float) roguePoints) / maxAlignmentPoints_Rogue;

            icon = villainIcon;
            topIcon = 0;
            midIcon = smallVillainIcon;
            botIcon = smallRogueIcon;
        }
    }


    topPerc = min(1.0f, topPerc);
    midPerc = min(1.0f, midPerc);
    botPerc = min(1.0f, botPerc);

    // Draw

    if (isSelf)
    {
        if (mouseCollision(&alignBox_self))
        {
            setToolTipEx(alignmentTooltip, &alignBox_self, textStd("MyAlignmentTooltipMessage", midAlignmentName), NULL, MENU_GAME, WDW_CONTACT, 1, TT_NOTRANSLATE);
            addToolTip(alignmentTooltip);
        }
    }
    else
    {
        if (mouseCollision(&alignBox_other))
        {
            setToolTipEx(alignmentTooltip, &alignBox_other, textStd("OtherAlignmentTooltipMessage", e->namePtr, midAlignmentName), NULL, MENU_GAME, WDW_INFO, 1, TT_NOTRANSLATE);
            addToolTip(alignmentTooltip);
        }
    }

    display_sprite(icon, x + 5*scale, *y + 15*scale, z + 1, scale, scale, 0xffffffff);

    smf_Display(s_topAlignmentName, x + 82*scale, *y + 1*scale + (scale >= 0.9f ? (18 * (scale - 1)) : (8 * (scale - .6))), z + 1, wd - 97*scale, 0, 0, 0, (scale >= 0.9f) ? &gTextAttr_White12 : &gTextAttr_White9, 0);
    if (s_topAlignmentName->outputString && strcmp(s_topAlignmentName->outputString, ""))
    {
        if (isSelf)
        {
            if (mouseCollision(&topBox_self))
            {
                setToolTipEx(alignmentTooltip, &topBox_self, textStd("AlignmentPointsTooltipMessage", topAlignmentName, topValue, topMax), NULL, MENU_GAME, WDW_CONTACT, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }
        else
        {
            if (mouseCollision(&topBox_other))
            {
                setToolTipEx(alignmentTooltip, &topBox_other, textStd("AlignmentPointsTooltipMessage", topAlignmentName, topValue, topMax), NULL, MENU_GAME, WDW_INFO, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }

        display_sprite(bar, x + 82*scale, *y + 18*scale, z, (wd - 95*scale) / bar->width, scale * BAR_HEIGHT / bar->height, 0x00feffa0);
        display_sprite(bar, x + 82*scale + 1, *y + 18*scale + 1, z, (wd - 95*scale - 2) / bar->width, (scale * BAR_HEIGHT - 2) / bar->height, CLR_BLACK);
        display_sprite(bar, x + 82*scale + 1, *y + 18*scale + 1, z, ((wd - 95*scale - 2) / bar->width) * topPerc, (scale * BAR_HEIGHT - 2) / bar->height, 0x2b5cd2ff);
        if (topPerc >= 1.0f)
        {
            display_sprite(glowIcon, x + wd - 32.5*scale, *y + 3.5*scale, z, scale * 0.67f, scale * 0.67f, glowAlpha);
        }
        display_sprite(topIcon, x + wd - 27*scale, *y + 9*scale, z, scale, scale, 0xffffffff);
    }

    smf_Display(s_midAlignmentName, x + 82*scale, *y + 26*scale + (scale >= 0.9f ? (18 * (scale - 1)) : (8 * (scale - .6))), z + 1, wd - 97*scale, 0, 0, 0, (scale >= 0.9f) ? &gTextAttr_White12 : &gTextAttr_White9, 0);
    if (s_midAlignmentName->outputString && strcmp(s_midAlignmentName->outputString, ""))
    {
        if (isSelf)
        {
            if (mouseCollision(&midBox_self))
            {
                setToolTipEx(alignmentTooltip, &midBox_self, textStd("AlignmentPointsTooltipMessage", midAlignmentName, midValue, midMax), NULL, MENU_GAME, WDW_CONTACT, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }
        else
        {
            if (mouseCollision(&midBox_other))
            {
                setToolTipEx(alignmentTooltip, &midBox_other, textStd("AlignmentPointsTooltipMessage", midAlignmentName, midValue, midMax), NULL, MENU_GAME, WDW_INFO, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }

        display_sprite(bar, x + 82*scale, *y + 43*scale, z, (wd - 95*scale) / bar->width, scale * BAR_HEIGHT / bar->height, 0x00feffa0);
        display_sprite(bar, x + 82*scale + 1, *y + 43*scale + 1, z, (wd - 95*scale - 2) / bar->width, (scale * BAR_HEIGHT - 2) / bar->height, CLR_BLACK);
        display_sprite(bar, x + 82*scale + 1, *y + 43*scale + 1, z, ((wd - 95*scale - 2) / bar->width) * midPerc, (scale * BAR_HEIGHT - 2) / bar->height, 0x2b5cd2ff);
        if (midPerc >= 1.0f)
        {
            display_sprite(glowIcon, x + wd - 32.5*scale, *y + 28.5*scale, z, scale * 0.67f, scale * 0.67f, glowAlpha);
        }
        display_sprite(midIcon, x + wd - 27*scale, *y + 34*scale, z, scale, scale, 0xffffffff);
    }

    smf_Display(s_botAlignmentName, x + 82*scale, *y + 51*scale + (scale >= 0.9f ? (18 * (scale - 1)) : (8 * (scale - .6))), z + 1, wd - 97*scale, 0, 0, 0, (scale >= 0.9f) ? &gTextAttr_White12 : &gTextAttr_White9, 0);
    if (s_botAlignmentName->outputString && strcmp(s_botAlignmentName->outputString, ""))
    {
        if (isSelf)
        {
            if (mouseCollision(&botBox_self))
            {
                setToolTipEx(alignmentTooltip, &botBox_self, textStd("AlignmentPointsTooltipMessage", botAlignmentName, botValue, botMax), NULL, MENU_GAME, WDW_CONTACT, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }
        else
        {
            if (mouseCollision(&botBox_other))
            {
                setToolTipEx(alignmentTooltip, &botBox_other, textStd("AlignmentPointsTooltipMessage", botAlignmentName, botValue, botMax), NULL, MENU_GAME, WDW_INFO, 1, TT_NOTRANSLATE);
                addToolTip(alignmentTooltip);
            }
        }

        display_sprite(bar, x + 82*scale, *y + 68*scale, z, (wd - 95*scale) / bar->width, scale * BAR_HEIGHT / bar->height, 0x00feffa0);
        display_sprite(bar, x + 82*scale + 1, *y + 68*scale + 1, z, (wd - 95*scale - 2) / bar->width, (scale * BAR_HEIGHT - 2) / bar->height, CLR_BLACK);
        display_sprite(bar, x + 82*scale + 1, *y + 68*scale + 1, z, ((wd - 95*scale - 2) / bar->width) * botPerc, (scale * BAR_HEIGHT - 2) / bar->height, 0x2b5cd2ff);
        if (botPerc >= 1.0f)
        {
            display_sprite(glowIcon, x + wd - 32.5*scale, *y + 53.5*scale, z, scale * 0.67f, scale * 0.67f, glowAlpha);
        }
        display_sprite(botIcon, x + wd - 27*scale, *y + 59*scale, z, scale, scale, 0xffffffff);
    }

    *y += 86*scale;
}

void displayAlignmentPoints(Entity *e, int x, float *y, int z, int wd, float scale, int color, int isSelf, ToolTip *alignmentTooltip)
{
    static AtlasTex *heroIcon = 0, *vigilanteIcon = 0, *villainIcon = 0, *rogueIcon = 0, *emptyIcon = 0, *glowIcon = 0, *ringIcon = 0;
    static CBox boxes[ALIGNMENT_POINT_MAX_COUNT];
    int seconds[ALIGNMENT_POINT_MAX_COUNT];
    float scales[ALIGNMENT_POINT_MAX_COUNT];
    AtlasTex *icons[ALIGNMENT_POINT_MAX_COUNT];
    int glowAlpha = 0xffffff00 + ((int) ((float) abs(1000 - (timerMsecsSince2000() % 2000)) / 1000 * 0xa0));
    U32 now = timerServerTime();
    int bubbleIndex;

    if (!heroIcon)
    {
        heroIcon = atlasLoadTexture("Tips_Hero_small.texture");
        vigilanteIcon = atlasLoadTexture("Tips_Vigilante_small.texture");
        villainIcon = atlasLoadTexture("Tips_Villain_small.texture");
        rogueIcon = atlasLoadTexture("Tips_Rogue_small.texture");
        emptyIcon = atlasLoadTexture("EnhncTray_RingHole.tga");
        glowIcon = atlasLoadTexture("Glow_ring.texture");
        ringIcon = atlasLoadTexture("tray_ring_power.tga");
    }

    if (!s_tipProgressLabel)
    {
        s_tipProgressLabel = smfBlock_Create();
        smf_SetFlags(s_tipProgressLabel, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace,
            0, 0, 0, 0, SMFDisplayMode_AllCharacters, SMFContextMenuMode_Copy, SMAlignment_Right, 0, 0, 0);
        smf_SetRawText(s_tipProgressLabel, textStd("AlignmentPointsHeaderString"), false);
    }
    smf_Display(s_tipProgressLabel, x + 0*scale, *y + 10*scale + (scale >= 0.9f ? (8 * (scale - 1)) : (12 * (scale - .6))), z, 126*scale, 0, 0, 0, (scale >= 0.9f) ? &gTextAttr_White12 : &gTextAttr_White9, 0);

    for (bubbleIndex = 0; bubbleIndex < ALIGNMENT_POINT_MAX_COUNT; bubbleIndex++)
    {
        seconds[bubbleIndex] = timeWhenCanEarnAlignmentPoint[bubbleIndex] - now;
        
        if (earnedAlignments[bubbleIndex] == 1)
        {
            icons[bubbleIndex] = heroIcon;
        }
        else if (earnedAlignments[bubbleIndex] == 2)
        {
            icons[bubbleIndex] = vigilanteIcon;
        }
        else if (earnedAlignments[bubbleIndex] == 3)
        {
            icons[bubbleIndex] = villainIcon;
        }
        else if (earnedAlignments[bubbleIndex] == 4)
        {
            icons[bubbleIndex] = rogueIcon;
        }
        else // default just in case
        {
            icons[bubbleIndex] = heroIcon;
        }
        
        scales[bubbleIndex] = MINMAX(((float) seconds[bubbleIndex] / ALIGNMENT_POINT_COOLDOWN_TIMER), 0.0f, 1.0f);
        
        display_sprite(ringIcon, x + (135 + 45*bubbleIndex)*scale, *y + 0*scale, z, scale, scale, color);
        display_sprite(emptyIcon, x + (138 + 45*bubbleIndex)*scale, *y + 2.5*scale, z, scale * .5, scale * .5, color);
        
        if (scales[bubbleIndex] != 0.0f)
        {
            display_sprite(glowIcon, x + (127.5 + 45*bubbleIndex)*scale, *y - 8*scale, z, scale * 1.10, scale * 1.10, glowAlpha);
        }
        
        display_sprite(icons[bubbleIndex], x + (135.5 + 45*bubbleIndex)*scale + 20*scale*(1.0f-scales[bubbleIndex]), *y + 0*scale + 20*scale*(1.0f-scales[bubbleIndex]), z, scale * scales[bubbleIndex], scale * scales[bubbleIndex], 0xffffffff);
        
        BuildCBox(&boxes[bubbleIndex], x + (135 + 45*bubbleIndex)*scale, *y + 0*scale, 40*scale, 40*scale);
        if (mouseCollision(&boxes[bubbleIndex]))
        {
            char *message, *submessage;

            if (scales[bubbleIndex] == 0.0f)
            {
                message = textStd("AlignmentPointDetailNone");
            }
            else if (seconds[bubbleIndex] > (ALIGNMENT_POINT_COOLDOWN_TIMER - (1 * 60 * 60))) // less than one hour has passed since the point was earned
            {
                if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_HERO)
                {
                    submessage = "HeroPointDetailOn";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VIGILANTE)
                {
                    submessage = "VigilantePointDetailOn";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VILLAIN)
                {
                    submessage = "VillainPointDetailOn";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_ROGUE)
                {
                    submessage = "RoguePointDetailOn";
                }
                else // default just in case
                {
                    submessage = "HeroPointDetailOn";
                }

                message = textStd(submessage);
            }
            else if (seconds[bubbleIndex] < (1 * 60 * 60)) // less than one hours left until you can earn the alignment point again
            {
                int minutes = (seconds[bubbleIndex] + 59) / 60; // round up
                int minutePart = minutes % 60;
                int hours = minutes / 60;

                if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_HERO)
                {
                    submessage = "HeroPointDetailRecentFuture";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VIGILANTE)
                {
                    submessage = "VigilantePointDetailRecentFuture";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VILLAIN)
                {
                    submessage = "VillainPointDetailRecentFuture";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_ROGUE)
                {
                    submessage = "RoguePointDetailRecentFuture";
                }
                else // default just in case
                {
                    submessage = "HeroPointDetailRecentFuture";
                }

                message = textStd(submessage, minutes);
            }
            else
            {
                int minutes = (ALIGNMENT_POINT_COOLDOWN_TIMER - seconds[bubbleIndex]) / 60;
                int minutePart = minutes % 60;
                int hours = minutes / 60;

                if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_HERO)
                {
                    submessage = "HeroPointDetailRecentPast";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VIGILANTE)
                {
                    submessage = "VigilantePointDetailRecentPast";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_VILLAIN)
                {
                    submessage = "VillainPointDetailRecentPast";
                }
                else if (earnedAlignments[bubbleIndex] == ALIGNMENT_POINT_ROGUE)
                {
                    submessage = "RoguePointDetailRecentPast";
                }
                else // default just in case
                {
                    submessage = "HeroPointDetailRecentPast";
                }

                message = textStd(submessage, hours, minutePart);
            }

            setToolTipEx(alignmentTooltip, &boxes[bubbleIndex], message, NULL, MENU_GAME, WDW_CONTACT, 1, TT_NOTRANSLATE);
            addToolTip(alignmentTooltip);
        }
    }

    *y += 45*scale;
}

int contactLoadLimiter = 2;

//
//
static void displayContact( int idx, int xorig, float *y, int z, int wdorig, float sc, int color, int draw)
{
#define FADE_WIDTH  (150.0f)
#define FADE_HEIGHT (38.0f)
#define CALL_HEIGHT (20.0f)
#define CALL_WIDTH  (40.0f)
#define ICON_WIDTH  (64.0f)
#define ICON_HEIGHT (64.0f)

      //ContactStatus *cs = contactDisplayList[idx];
    ContactStatus *cs = contacts[idx];
    CBox box;
    AtlasTex *icon = NULL;
    AtlasTex *fade;
    AtlasTex *mid;
    AtlasTex *storyArcInProgress;
    AtlasTex *miniArcInProgress;
    int ht = 0;
    int x = xorig;
    int wd = wdorig;
    float xscale;
    float yscale;
    int foreColor = CLR_NORMAL_FOREGROUND;
    int backColor = CLR_NORMAL_BACKGROUND;
    const cCostume *pCostume = NULL;
    TaskStatus *task;
    int suppressMenu = 0;

    static float timer = 0;

    if (!cs->name || !cs->name[0]) // we may have a NULL entry for the mission
        return;


    *y += PIX3*sc;
    x  += (PIX3+6)*sc;
    wd -= (PIX3*2+6)*sc;

    if (cs->imageOverride && stricmp(cs->imageOverride, "none"))
    {
        icon = atlasLoadTexture(cs->imageOverride);
    }
    else if (!cs->imageOverride && cs->npcNum)
    {
        pCostume = npcDefsGetCostume(cs->npcNum, 0);
    }
    
    if (pCostume!=NULL)
    {
        if(cs->npcNum == gMissionMakerContact.npc_idx )
            icon = seqGetHeadshot( pCostume, 0 ); // Custom critter contact isn't cached since the costume keeps getting overwritten
        else
            icon = seqGetHeadshotCached(pCostume, 0); // only use the icon if it is already cached.
    }

    // limit how many contacts per frame are loaded
    if ( !icon )
    {
        if ( contactLoadLimiter <= 0 )
        {
            contactLoadLimiter = 2;
            if(pCostume)
                seqRequestHeadshot(pCostume, 0);
            else if(cs->imageOverride && stricmp(cs->imageOverride, "none"))
                icon = atlasLoadTexture(cs->imageOverride);
            else if(cs->imageOverride && !stricmp(cs->imageOverride, "none"))
            {
                //do nothing
            }
            else
                Errorf("contact %s has no costume.",cs->name);
        }
    }

    if (!draw && !s_bReparse) {
        *y += (ICON_HEIGHT+PIX3*2+4+4+CONTACT_SPACE) * sc;
        return;
    }

    fade = atlasLoadTexture("Contact_fadebar_R.tga");
    xscale = FADE_WIDTH*sc/fade->width;
    yscale = FADE_HEIGHT*sc/fade->height;
    display_sprite(fade, x + wd - FADE_WIDTH*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

    if (icon)
    {
        F32 iconScale = MIN(ICON_WIDTH*sc/icon->width, ICON_HEIGHT*sc/icon->height);
        mid = atlasLoadTexture("Contact_fadebar_MID.tga");
        xscale = (wd + (-FADE_WIDTH - ICON_WIDTH)*sc)/mid->width;
        yscale = FADE_HEIGHT*sc/mid->height;
        display_sprite(mid, x+(ICON_WIDTH)*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

        display_sprite(icon, x, *y+4*sc, z, iconScale, iconScale, CLR_WHITE );

        // TODO: Hack for non-rounded pictures
         drawFlatFrame(PIX2, R4, xorig+(PIX3+6-PIX2)*sc, *y+(4-PIX2)*sc, z, (ICON_WIDTH+PIX2*2)*sc, (ICON_HEIGHT+PIX2*2)*sc, sc, 0x000000ff, 0);

        x  += (CONTACT_SPACE*sc + MAX(icon->width*iconScale, icon->height*iconScale));
         wd -= (CONTACT_SPACE*sc + MAX(icon->width*iconScale, icon->height*iconScale));
    }
    else
    {
        mid = atlasLoadTexture("Contact_fadebar_MID.tga");
        xscale = (wd - (FADE_WIDTH+3))*sc/mid->width; 
        yscale = FADE_HEIGHT*sc/mid->height;
        display_sprite(mid, x+3*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

        // don't show as much margin here (margin accounted for distance from icon)
        x -= 5*sc;
        wd += 5*sc;
    }

    if(cs->canUseCell || cs->tipType)
    {
        char *defaultCall = cs->tipType?"TipCallString":"CallString";
        char *callstring = cs->callOverride?cs->callOverride:defaultCall;
        if(D_MOUSEHIT == drawStdButton(x + wd - CALL_WIDTH*sc*1.5 + 9*sc , *y + 46.5*sc + CALL_HEIGHT/2*sc, z,
            CALL_WIDTH*sc*2, CALL_HEIGHT*sc, color, callstring, .75f*sc, 0))
        {
            ContactSendCellCall(cs->handle);
        }
    }

    if (CTS_SHOW_STORY)
        suppressMenu = ClickToSourceDisplay(x+10*sc, *y+70*sc, z, 0, 0xffffffff, cs->filename, NULL, CTS_TEXT_REGULAR);

    font( &game_14 );
    font_color(0x00deffff, 0x00deffff);
    prnt( x+10*sc, *y + 18*sc, z, .90f*sc, .90f*sc, cs->name );

    font( &game_12 );
    if (cs->locationDescription[0])
    {
        prnt( x+10*sc, *y + 29*sc, z, 0.80f*sc, 0.80f*sc, cs->locationDescription );
    }

    if(!cs->tipType && !cs->metaContact)
        displayRelationshipBar(x+10*sc, *y+(FADE_HEIGHT-BAR_HEIGHT)*sc, z, sc, cs);

    task = TaskStatusFromContact(cs);
    if (task)
    {
        TextAttribs *pta = task->isComplete ? &s_taComplete : &s_taDefaults;

        if (cs->onStoryArc || cs->onMiniArc)
        {
            x += 30.0f;
            wd -= 30.0f;
        }
        pta->piScale = (int *)((int)(sc*SMF_FONT_SCALE*.75f));

        if(task->isComplete)
        {
            int top = font_color_get_top();
            int bottom = font_color_get_bottom();

            font_color(0x00ff33ff, 0x00ff33ff);
            ht += smf_ParseAndDisplay(s_ppBlocks[idx],
                textStd("TaskComplete"),
                x+10*sc, *y+ht+(FADE_HEIGHT+4)*sc, z,
                wd-(10+CONTACT_SPACE)*sc - CALL_WIDTH*sc*2 - 10, 98*sc,
                s_bReparse, false, pta, NULL, 
                0, true);
            font_color(top, bottom);
        }
        else
        {
            ht += smf_ParseAndDisplay(s_ppBlocks[idx],
                task->description,
                x+10*sc, *y+ht+(FADE_HEIGHT+4)*sc, z,
                wd-(10+CONTACT_SPACE)*sc - CALL_WIDTH*sc*2 - 10, 98*sc,
                s_bReparse, false, pta, NULL, 
                0, true);
        }

        if (cs->onStoryArc || cs->onMiniArc)
        {
            x -= 30.0f;
            wd += 30.0f;
        }
        pta->piScale = (int *)((int)(1.0f*SMF_FONT_SCALE));
    }

    // add storyarc/miniarc display
    if (cs->onStoryArc)
    { 
        storyArcInProgress = atlasLoadTexture("storyarc_button_major.tga");
        display_sprite(storyArcInProgress, x, *y + 45.0*sc, z, 1.00f*sc, 1.00f*sc, 0xffffffff);
//        font_color(0xdddd00ff, 0xdddd00ff);
//        prnt( x+10*sc, *y+(FADE_HEIGHT+ht+15)*sc, z, 1.00f*sc, 1.00f*sc, "OnStoryArc" ); 
        BuildCBox( &box, x, *y + 45.0*sc, 30.0*sc, 30.0*sc );
        setToolTip( s_ppToolTips[idx], &box, "OnStoryArc", NULL, MENU_GAME, WDW_CONTACT );
        addToolTip( s_ppToolTips[idx] );
    } 
    else if (cs->onMiniArc) 
    {
        miniArcInProgress = atlasLoadTexture("storyarc_button_minor.tga");
        display_sprite(miniArcInProgress, x+10*sc, *y + 45*sc, z, 1.00f*sc, 1.00f*sc, 0xffffffff);
//        font_color(0xdddd00ff, 0xdddd00ff);
//        prnt( x+10*sc, *y+(FADE_HEIGHT+ht+15)*sc, z, 1.00f*sc, 1.00f*sc, "OnMiniArc" );
        BuildCBox( &box, x, *y + 45.0*sc, 30.0*sc, 30.0*sc );
        setToolTip( s_ppToolTips[idx], &box, "OnMiniArc", NULL, MENU_GAME, WDW_CONTACT );
        addToolTip( s_ppToolTips[idx] );
    } else {
        clearToolTip( s_ppToolTips[idx] );
    }

    ht = max(ht+(FADE_HEIGHT+CONTACT_SPACE)*sc, max((ICON_HEIGHT+PIX3*2+4+4)*sc, (CONTACT_HT-5)*sc));
    //ht = (ICON_HEIGHT+PIX3*2+4+4)*sc;
    BuildCBox( &box, xorig, *y, wdorig, ht);
    if( mouseCollision( &box ))
    {
        foreColor = CLR_MOUSEOVER_FOREGROUND;
        backColor = CLR_MOUSEOVER_BACKGROUND;
        if(mouseClickHit(&box, MS_LEFT))
        {
            ContactSelect(idx, 1);
        }
        else if(mouseClickHit(&box, MS_RIGHT) && !suppressMenu)
        {
            contactContextMenuSetup(idx);
            contextMenu_display(contactContextMenu);
        }

        drawFlatFrame(PIX2, R10, xorig, *y-PIX3*sc, z-1, wdorig, ht, sc, foreColor, backColor);
    }

    if (DestinationIsAContact(&waypointDest)
        && waypointDest.handle == cs->handle)
    {
        foreColor = CLR_SELECTION_FOREGROUND;
        backColor = CLR_SELECTION_BACKGROUND;
    }

     drawFlatFrame(PIX2, R10, xorig, *y-PIX3*sc, z-1, wdorig, ht, sc, foreColor, backColor);

    *y += ht;
    *y += CONTACT_SPACE*sc; // Space between contacts
}

#define BORDER_SPACE    15
//
//
int contactsWindow()
{
    float x, y, z, wd, ht, scale;
    int i, color, bcolor;
    float oldy;
    char *curTab;
    static ComboBox sortBox = {0};
    static bool sortBoxInitialized = false;
    static bool isTipsTabDisplayed = false;
    static PlayerType currentlyDisplayedPlayerType = kPlayerType_Count;
    int contactCount = eaSize(&contacts);
    Entity *e = playerPtr();

    static ScrollBar sb = { WDW_CONTACT, 0 };

    if(!s_ppBlocks)
        eaCreate(&s_ppBlocks);
    if(!s_ppToolTips)
        eaCreate(&s_ppToolTips);

    // cleanup tooltips
    for (i = 0; i < eaSize(&s_ppToolTips); i++)
    {
        clearToolTip(s_ppToolTips[i]);
    }

    if (!window_getDims(WDW_CONTACT, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor)
        || !e || !e->pchar || !e->pl)
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////

    // update tab list
    if (!contactCategories)  
    {
        contactCategories = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
        uiTabControlRemoveAll(contactCategories);
        uiTabControlAdd(contactCategories, "ActiveString", (uiTabData)allocAddString("Active"));
        uiTabControlAdd(contactCategories, "InactiveString", (uiTabData)allocAddString("Inactive"));
        uiTabControlAdd(contactCategories, "DetectiveString", (uiTabData)allocAddString("Broker"));
    }

    if (e->pchar->playerTypeByLocation != currentlyDisplayedPlayerType)
    {
        if (e->pchar->playerTypeByLocation == kPlayerType_Hero)
        {
            uiTabControlRename(contactCategories, "DetectiveString", (uiTabData)allocAddString("Broker"));
        }
        else
        {
            uiTabControlRename(contactCategories, "BrokerString", (uiTabData)allocAddString("Broker"));
        }
        currentlyDisplayedPlayerType = e->pchar->playerTypeByLocation;
    }

    if (!isTipsTabDisplayed && e->pchar->iLevel >= 19 && !ENT_IS_IN_PRAETORIA(e))
    {
        uiTabControlAdd(contactCategories, "TipsString", (uiTabData)allocAddString("Tips"));
        isTipsTabDisplayed = true;
    }
    else if (isTipsTabDisplayed && (e->pchar->iLevel < 19 || ENT_IS_IN_PRAETORIA(e)))
    {
        uiTabControlRemove(contactCategories, (uiTabData)allocAddString("Tips"));
        isTipsTabDisplayed = false;
    }

    if (!game_state.intro_zone)
    {
        if (D_MOUSEHIT == drawStdButton(x + 88*scale, y + 39*scale, z, 150*scale, 25*scale, color, "FindContactButton", scale, 0))
        {
            cmdParse("contactfinder_showcurrent");
        }
    }

    if (!sortBoxInitialized)
    {
        sortBoxInitialized = true;

        comboboxClear(&sortBox);
        comboboxTitle_init(&sortBox, 0, 0, 0, 150, CBOXHEIGHT, 400, WDW_CONTACT);
        comboboxTitle_add(&sortBox, 0, NULL, "SortByNameString", "SortByNameString", kContactSortType_Name, CLR_WHITE, 0,0);
        comboboxTitle_add(&sortBox, 0, NULL, "SortByZoneString", "SortByZoneString", kContactSortType_Zone, CLR_WHITE, 0,0);
//        comboboxTitle_add(&sortBox, 1, NULL, "SortByActiveString", "SortByActiveString", kContactSortType_Active, CLR_WHITE, 0);
        comboboxTitle_add(&sortBox, 0, NULL, "SortByRelationshipString", "SortByRelationshipString", kContactSortType_Relationship, CLR_WHITE, 0,0);

        if (optionGet(kUO_ContactSort) == kContactSortType_Default)
            optionSet(kUO_ContactSort, kContactSortType_Active,1);
        combobox_setChoiceCur(&sortBox, optionGet(kUO_ContactSort) ); 
    }

    if (contactsDirty || contactSortList == NULL)
    {    
        // update the contact display list
        if (contactSortList != NULL)
            eaiDestroy(&contactSortList);

        // recreate sort array
        eaiCreate(&contactSortList);
        for (i = 0; i < contactCount; i++)
        {
            eaiPush(&contactSortList, i);
        }

        // sort the list
        {
            ComboCheckboxElement *element = eaGet(&sortBox.elements, sortBox.currChoice);

            if (element)
            {
                contactApplySortList(contactSortList, element->id);
            }
        }
            
        // clear the dirty bits
        contactsDirty = false;
    }

    oldy = y;

    drawFrame( PIX3, R10, x, y, z-2, wd, ht, scale, color, bcolor );
    curTab = drawTabControl(contactCategories, x+R10*scale, y, z + 3.0f, wd - 2*R10*scale, TAB_HEIGHT, scale, color, color, TabDirection_Horizontal );

    //////////////////////////////////////////////////////////////////////////
    // draw sort box
    sortBox.x = wd/scale - 160 - PIX3*2*scale;
    sortBox.y = 27; 
    sortBox.z = 0; 
    combobox_display(&sortBox);

    // check to see if sort has changed
    if( sortBox.newlySelectedItem )
    {
        ComboCheckboxElement *element = eaGet(&sortBox.elements,sortBox.currChoice);

        if (element)
        {
            contactApplySortList(contactSortList, element->id);
            optionSet(kUO_ContactSort, element->id,1);
            sendOptions(0);
        }


        sortBox.newlySelectedItem = FALSE;
    }

    //////////////////////////////////////////////////////////////////////////

    set_scissor( 1 );
    scissor_dims( x + PIX3*scale, y + PIX3*scale + (PIX3+TAB_HEIGHT)*scale + 33*scale, wd - PIX3*2*scale, ht - (PIX3*3+TAB_HEIGHT)*scale - 33*scale );

     y += (BORDER_SPACE*scale - sb.offset) + (PIX3+TAB_HEIGHT)*scale + 22*scale;

    // Make sure the formatting array is large enough.
    i = contactCount-eaSize(&s_ppBlocks);
    while(i>0)
    {
        eaPush(&s_ppBlocks, smfBlock_Create());
        i--;
    }

    // Make sure the tooltip array is large enough.
    i = contactCount-eaSize(&s_ppToolTips);
    while(i>0)
    {
        eaPush(&s_ppToolTips, calloc(sizeof(ToolTip), 1));
        i--;
    }

    // Add the header to the Tips tab
    if (!stricmp("Tips", curTab))
    {
        y -= 2*scale;

        // Current alignment status
        displayAlignmentStatus(playerPtr(), x + BORDER_SPACE, &y, z, wd - BORDER_SPACE*2, scale, color, 1, &uiContact_alignmentTooltip);

        y -= 3*scale;

        // Timers until you can earn another alignment point
        displayAlignmentPoints(playerPtr(), x, &y, z, wd, scale, color, 1, &uiContact_alignmentTooltip);
    }

    contactLoadLimiter--;

    if (!stricmp("Tips", curTab))
    {
        // Display Designer tips at the start of the list.
        for (i = contactCount - 1; i >= 0; i--)
        {
            int contactIndex = contactSortList[i];
            int rightTab = false;
            int    draw = ((y < oldy+ht) && (y +(ICON_HEIGHT+PIX3*3+4+4+CONTACT_SPACE)*scale > oldy));

            if (isDesignerTipContact(contactIndex)) // don't check correct alliance since bad alliance tips should not get sent to client
            {
                displayContact( contactIndex, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale, color,draw);
            }
        }
    }

    for( i = contactCount - 1; i >= 0; i-- )
    {
        int contactIndex = contactSortList[i];
        int rightTab = false;
        int    draw = ((y < oldy+ht) && (y +(ICON_HEIGHT+PIX3*3+4+4+CONTACT_SPACE)*scale > oldy));

        if (!stricmp("Active", curTab))
        {
            if (isActiveContact(contactIndex)
                && ((isNewspaperContact(contactIndex) || isBrokerContact(contactIndex) 
                        ? ContactCorrectAlliance(SAFE_MEMBER(e->pchar,playerTypeByLocation), contacts[contactIndex]->alliance)
                        : ContactCorrectAlliance(SAFE_MEMBER(e->pl,playerType), contacts[contactIndex]->alliance))
                    || SAFE_MEMBER(e->pl,taskforce_mode)))
                rightTab = true;
        } else if (!stricmp("Inactive", curTab)) {
            if ((!isActiveContact(contactIndex) && !isBrokerContact(contactIndex) && !isGoingRogueTipContact(contactIndex) && !isSpecialTipContact(contactIndex) && !isDesignerTipContact(contactIndex)) 
                || ((isNewspaperContact(contactIndex) || isBrokerContact(contactIndex) 
                        ? !ContactCorrectAlliance(SAFE_MEMBER(e->pchar,playerTypeByLocation), contacts[contactIndex]->alliance)
                        : !ContactCorrectAlliance(SAFE_MEMBER(e->pl,playerType), contacts[contactIndex]->alliance))
                    && !SAFE_MEMBER(e->pl,taskforce_mode)))
                rightTab = true;
        } else if (!stricmp("Broker", curTab)) {
            if (isBrokerContact(contactIndex) && ContactCorrectAlliance(SAFE_MEMBER(e->pchar,playerTypeByLocation), contacts[contactIndex]->alliance))
                rightTab = true;
        } else if (!stricmp("Tips", curTab)) {
            if (isGoingRogueTipContact(contactIndex)) // don't check correct alliance since bad alliance tips should not get sent to client
                rightTab = true;
        }

        if (rightTab)
        {
            if (contacts[contactIndex]->isNewspaper)
                displayNewspaper( contactIndex, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale, color,draw);
            else 
                displayContact( contactIndex, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale, color,draw);
        }
    }

    if (!stricmp("Tips", curTab))
    {
        // Display special tips at the end of the list.
        for (i = contactCount - 1; i >= 0; i--)
        {
            int contactIndex = contactSortList[i];
            int rightTab = false;
            int    draw = ((y < oldy+ht) && (y +(ICON_HEIGHT+PIX3*3+4+4+CONTACT_SPACE)*scale > oldy));

            if (isSpecialTipContact(contactIndex)) // don't check correct alliance since bad alliance tips should not get sent to client
            {
                displayContact( contactIndex, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale, color,draw);
            }
        }
    }

    s_bReparse = false;
    set_scissor( 0 );


       doScrollBar( &sb, ht - (PIX3*2+R10*2)*scale - 53*scale, y - oldy + sb.offset - 76*scale, wd, (PIX3+R10)*scale + 53*scale, z, 0, 0 );

    return 0;
}

/* End of File */
