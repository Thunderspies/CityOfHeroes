
#include "UI/uiNet.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiChat.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "UI/uiMission.h"
#include "UI/uiWindows.h"
#include "UI/uiToolTip.h"
#include "UI/uiContact.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "UI/uiSupergroup.h"

#include "language/langClientUtil.h"
#include "entity/costume_client.h"
#include "entity/entVarUpdate.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/teamup.h"
#include <utilitieslib/utils/timing.h>

#include "graphics/ttFont.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

#include <utilitieslib/components/earray.h>

#include "uiListView.h"
#include "UI/uiClipper.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiDialog.h"
#include "UI/uiReticle.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiWindows_init.h"
#include "UI/uiSGRaidList.h"
#include "UI/uiOptions.h"

#include "entity/classes.h"
#include "entity/origins.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/Supergroup.h"
#include "entity/character_inventory.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "player/pmotion.h"

enum
{
    RANK_TIP,
    CLASS_TIP,
    ORIGIN_TIP,
    SUPER_TIP_TOTAL,
};

static ToolTip memberTip[MAX_SUPER_GROUP_MEMBERS][3] = {0};
static ToolTipParent superParent = {0};

static int sgListRebuildRequired = 0;
static int oldSgSelectionDBID = 0;
static UIListView* sgListView;
static ScrollBar sgListViewScrollbar = {WDW_SUPERGROUP,0};

static Supergroup* sgGetPlayerSuperGroup();
static void sgSetSupergroupMode(int mode);
static int sgMemberIsDark(SupergroupStats* item);

/* Function sgListPrepareUpdate()
 *    Should be called when the player's supergroup list changes.
 *    This function sets flag to have the friend listview rebuild its list.  It also
 *    saves the listview's current selection so that the selection can be restored
 *    after the list is rebuilt.
 */
void sgListPrepareUpdate()
{
    if(sgListView && sgListView->selectedItem)
        oldSgSelectionDBID = ((SupergroupStats*)sgListView->selectedItem)->db_id;
    sgListRebuildRequired = 1;
}

void sgListReset()
{
    sgListPrepareUpdate();
    uiLVClear(sgListView);
}

static int sgListUpdateRequired = 0;
void sgTriggerMemberListUpdate()
{
    sgListUpdateRequired = 1;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ContextMenu *allianceContext = 0;
static ContextMenu *allianceSubContext[MAX_SUPERGROUP_ALLIES] = {0};

int allianceCanInviteTarget(void *nodata)
{
    Entity * e = playerPtr();
    int i;

    if (!current_target)
        return CM_HIDE;

    if(!e || !e->supergroup_id || !e->supergroup || e->supergroup_id == current_target->supergroup_id )
        return CM_HIDE;

    if (!sgroup_hasPermission(e, SG_PERM_ALLIANCE))
        return CM_HIDE;

    if (!current_target->supergroup_id)
        return CM_VISIBLE;

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (e->supergroup->allies[i].db_id == current_target->supergroup_id)
            return CM_VISIBLE;
    }

    return CM_AVAILABLE;
}

void allianceInviteTarget(void *nodata)
{
    if (allianceCanInviteTarget(nodata) == CM_AVAILABLE)
    {
        char buf[1024];
        sprintf(buf, "coalition_invite \"%s\"", current_target->name);
        cmdParse(buf);
    }
}

int raidCanInviteTarget(void *nodata)
{
    Entity * e = playerPtr();

    if (!current_target)
        return CM_HIDE;

    if(!e || !e->supergroup_id || !e->supergroup || current_target->supergroup_id == e->supergroup_id )
        return CM_HIDE;

    if (!sgroup_hasPermission(e, SG_PERM_RAID_SCHEDULE))
        return CM_HIDE;

    if (!current_target->supergroup_id)
        return CM_VISIBLE;

    return CM_AVAILABLE;
}

void raidInviteTarget(void *nodata)
{
    if (raidCanInviteTarget(nodata) == CM_AVAILABLE)
    {
        char buf[1024];
        sprintf(buf, "sgraid_invite \"%s\"", current_target->name);
        cmdParse(buf);
    }
}

static int allianceVisible(void *data)
{
    int idx = (int)data;
    Entity * e = playerPtr();

    if(!e || !e->supergroup_id || !e->supergroup)
        return CM_HIDE;

    if (e->supergroup->allies[idx].db_id)
        return CM_AVAILABLE;

    return CM_HIDE;
}

static int allianceHasPermission(void *data)
{
    Entity * e = playerPtr();

    if(!e || !e->supergroup_id || !e->supergroup)
        return CM_HIDE;

    if (sgroup_hasPermission(e, SG_PERM_ALLIANCE))
        return CM_AVAILABLE;

    return CM_HIDE;
}

static int allianceVisibleMember(void *data)
{
    if (allianceHasPermission(NULL) == CM_AVAILABLE)
        return CM_HIDE;

    return allianceVisible(data);
}

static int allianceVisibleLeader(void *data)
{
    if (allianceHasPermission(NULL) == CM_HIDE)
        return CM_HIDE;

    return allianceVisible(data);
}

static int allianceGetMuteOther(void *data)
{
    int idx = (int)data;
    Entity * e = playerPtr();

    if (allianceHasPermission(data) == CM_HIDE || allianceVisible(data) == CM_HIDE )
    {
        if (e->supergroup->allies[idx].minDisplayRank > 0)
            return CM_CHECKED_UNAVAILABLE;

        else
            //return CM_HIDE;
            return CM_VISIBLE;
    }

    if (e->supergroup->allies[idx].minDisplayRank > 0)
        return CM_CHECKED;

    return CM_AVAILABLE;
}

static void allianceSetMuteOther(void *data)
{
    char buffer[512];
    Entity * e = playerPtr();
    int idx = (int)data;

    if (allianceHasPermission(data) == CM_HIDE)
        return;

    if (e->supergroup->allies[idx].db_id == 0)
        return;

    sprintf(buffer, "%s %d %d", "coalition_mintalkrank", e->supergroup->allies[idx].db_id, (e->supergroup->allies[idx].minDisplayRank > 0 ? 0 : 2));
    cmdParse(buffer);
}

static int allianceGetMuteSend(void *data)
{
    int idx = (int)data;
    Entity * e = playerPtr();

    if (allianceHasPermission(data) == CM_HIDE || allianceVisible(data) == CM_HIDE )
    {
        if (e->supergroup->allies[idx].dontTalkTo)
            return CM_CHECKED_UNAVAILABLE;

        else
            //return CM_HIDE;
            return CM_VISIBLE;
    }

    if (e->supergroup->allies[idx].dontTalkTo)
        return CM_CHECKED;

    return CM_AVAILABLE;
}

static void allianceSetMuteSend(void *data)
{
    char buffer[512];
    Entity * e = playerPtr();
    int idx = (int)data;

    if (allianceHasPermission(data) == CM_HIDE)
        return;

    if (e->supergroup->allies[idx].db_id == 0)
        return;

    sprintf(buffer, "%s %d %d", "coalition_nosend", e->supergroup->allies[idx].db_id, (e->supergroup->allies[idx].dontTalkTo ? 0 : 1));
    cmdParse(buffer);
}

static char *breakAlliance_name = NULL;

static void allianceDoBreakAlliance(void * data)
{
    char buffer[512];
    if (breakAlliance_name != NULL)
    {
        sprintf(buffer, "%s %s", "coalition_cancel", breakAlliance_name);
        cmdParse(buffer);

        breakAlliance_name = NULL;
    }
}

static void allianceBreakAlliance(void *data)
{
    Entity * e = playerPtr();
    int idx = (int)data;

    if (allianceHasPermission(data) == CM_HIDE)
        return;

    if (e->supergroup->allies[idx].db_id == 0)
        return;

    breakAlliance_name = e->supergroup->allies[idx].name;

    dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd("ReallyBreakAlliance", e->supergroup->allies[idx].name), NULL, allianceDoBreakAlliance, NULL, NULL, 
        DLGFLAG_GAME_ONLY, NULL, NULL, 2, 0, 0, 0 );
}

static int allianceGetMuteOwn(void *data)
{
    Entity * e = playerPtr();
    int idx = (int)data;

    if (allianceHasPermission(data) == CM_HIDE )
    {
        if ( e->supergroup->alliance_minTalkRank <= 0 )
            //return CM_HIDE;
            return CM_VISIBLE;

        if (e->supergroup->alliance_minTalkRank > 0)
            return CM_CHECKED_UNAVAILABLE;
    }

    if (e->supergroup->alliance_minTalkRank > 0)
        return CM_CHECKED;

    return CM_AVAILABLE;
}

static void allianceSetMuteOwn(void *data)
{
    char buffer[512];
    Entity * e = playerPtr();

    if (allianceHasPermission(data) == CM_HIDE)
        return;

    sprintf(buffer, "%s %d", "coalition_sg_mintalkrank", (e->supergroup->alliance_minTalkRank > 0 ? 0 : 2));
    cmdParse(buffer);
}

static char *allianceName(void *data)
{
    static char noname[] = "<unavailable>";
    int idx = (int)data;
    Entity * e = playerPtr();

    if(!e || !e->supergroup_id || !e->supergroup)
        return noname;

    if (e->supergroup->allies[idx].db_id == 0)
        return noname;

    return e->supergroup->allies[idx].name;
}

static int allianceShowDivider(void *data)
{
    int i;

    if (allianceGetMuteOwn(data) == CM_HIDE)
        return CM_HIDE;

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (allianceVisible((void *)i) != CM_HIDE)
            return CM_AVAILABLE;
    }

    return CM_HIDE;
}

static const char * allianceNameAdapter(void* arg0)
{
    return allianceName((void *)arg0);
}

static void alliance_menu(float x, float y)
{
    static int init = 0;

    if (!init)
    {
        int i;

        allianceContext = contextMenu_Create(NULL);
        contextMenu_addCheckBox(allianceContext, allianceGetMuteOwn, NULL, allianceSetMuteOwn, NULL, "CMMuteOwn" );
        contextMenu_addDividerVisible(allianceContext, allianceShowDivider);

        for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
        {
            allianceSubContext[i] = contextMenu_Create(NULL);
            contextMenu_addCheckBox(allianceSubContext[i], allianceGetMuteOther, (void *)i, allianceSetMuteOther, (void *)i, "CMMuteOthers");
            contextMenu_addCheckBox(allianceSubContext[i], allianceGetMuteSend, (void *)i, allianceSetMuteSend, (void *)i, "CMMuteSend");
            contextMenu_addDividerVisible(allianceSubContext[i], allianceHasPermission);
            contextMenu_addCode(allianceSubContext[i], allianceHasPermission, NULL, allianceBreakAlliance, (void *)i, "CMBreakAlliance", NULL);
            
            contextMenu_addVariableTextCode(allianceContext, allianceVisibleLeader, (void *)i, NULL, NULL, allianceNameAdapter, (void *)i, allianceSubContext[i]);
            //contextMenu_addVariableTextCode(allianceContext, allianceVisibleMember, (void *)i, NULL, NULL, allianceName, (void *)i, NULL);
            contextMenu_addVariableTextCode(allianceContext, allianceVisibleMember, (void *)i, NULL, NULL, allianceNameAdapter, (void *)i, allianceSubContext[i]);
        }

        init = 1;
    }

    contextMenu_set( allianceContext, x, y, NULL );
}

int sgroup_exists(void *foo)
{
    if( playerPtr()->supergroup )
        return 1;
    else
        return 0;
}
//
//
int sgroup_isMember(Entity *e, int db_id )
{
    int i;
    if( !e->supergroup )
        return 0;

    for( i = 0; i < eaSize(&e->supergroup->memberranks); i++ )
    {
        if( e->supergroup->memberranks[i]->dbid == db_id )
            return 1;
    }

    return 0;
}

int sgroup_rank( Entity *e, int db_id )
{
    int i;
    if( !e || !e->supergroup_id || !e->supergroup )
        return 0;

    for( i = 0; i < eaSize(&e->supergroup->memberranks); i++ )
    {
        if( e->supergroup->memberranks[i]->dbid == db_id )
            return e->supergroup->memberranks[i]->rank;
    }

    return 0;
}

int sgroup_canOfferMembership(void *foo)
{
    Entity *e = playerPtr();
    int dbid = targetGetDBID();

    // first make sure i'm on supergroup
    if(!e->supergroup || !e->sgStats || !e->superstat || !dbid)
    {
        return CM_HIDE;
    }

    if(sgroup_isMember(e, dbid))
        return CM_HIDE;

    // next make sure i have rank and supergroup isn't full
    if( !sgroup_hasPermission(e, SG_PERM_INVITE) || eaSize(&e->sgStats) >= MAX_SUPER_GROUP_MEMBERS )
        return CM_HIDE;

    return CM_AVAILABLE;

}

int sgroup_higherRanking(void *foo)
{
    Entity *e = playerPtr();
    int dbid = targetGetDBID();

    // first make sure i'm on supergroup
    if( !e->supergroup || !e->sgStats || !e->superstat || !dbid)
    {
        return CM_HIDE;
    }

    // next make sure target is valid and actually on team
    if(!sgroup_isMember(e, dbid))
        return CM_HIDE;
    
    // next make sure you have rank
    if( current_target && sgroup_rank(e, e->db_id) <= sgroup_rank( e, current_target->db_id ) )
        return CM_HIDE;

    return CM_AVAILABLE; // ok, can kick
}

int sgroup_canKick(void* foo)
{
    Entity* e = playerPtr();
    if( !sgroup_hasPermission(e, SG_PERM_KICK) )
        return CM_HIDE;

    return sgroup_higherRanking(0);
}

int sgroup_canPromote(void* foo)
{
    Entity* e = playerPtr();
    if( !sgroup_hasPermission(e, SG_PERM_PROMOTE) )
        return CM_HIDE;

    return sgroup_higherRanking(0);
}

int sgroup_canDemote(void* foo)
{
    Entity* e = playerPtr();
    if( !sgroup_hasPermission(e, SG_PERM_DEMOTE) )
        return CM_HIDE;

    return sgroup_higherRanking(0);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//
//
void sgroup_invite(void *foo)
{
    char buf[256];
    char* targetName = targetGetName();

    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf(buf, "sgi %s", targetName);
    cmdParse( buf );
    sgTriggerMemberListUpdate();
}

static char * s_pchKickMe;
void sgroup_kickYes(void * data)
{
    cmdParsef( "sg_kick_yes %s", s_pchKickMe );
    sgTriggerMemberListUpdate();
}

void sgroup_kickName( char * name )
{ 
    SAFE_FREE(s_pchKickMe);
    s_pchKickMe = strdup(name);
    dialogStd(DIALOG_YES_NO, textStd("ReallySGKick", s_pchKickMe), 0, 0, sgroup_kickYes, 0, 0 ); 
}

//
//
void sgroup_kick(void *foo)
{
    char* targetName = targetGetName();
    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }
    sgroup_kickName( targetName );
}


//
//
void sgroup_promote(void *foo)
{
    char buf[256];
    char* targetName = targetGetName();

    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    // the mapserver will actually print out useful information and ppl can use the slash command
    // anyway, so why validate?
    sprintf( buf, "promote %s", targetName);
    cmdParse( buf );
    sgTriggerMemberListUpdate();
}


//
//
void sgroup_demote(void *foo)
{
    char buf[256];
    char* targetName = targetGetName();

    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "demote %s", targetName);
    cmdParse( buf );
    sgTriggerMemberListUpdate();
}

//
//
void sgroup_quit(void * data)
{
     cmdParse( "sgleave" );
    sgTriggerMemberListUpdate();
    sgSetSupergroupMode(0);
    clearSupergroup( playerPtr()->supergroup );
    srClearAll();

}

//---------------------------------------------------------------------------------------

int sgroup_TargetIsMember(void)
{
    Entity *e = playerPtr();
    int i, id;

    id = targetGetDBID();
    if(!id)
        return FALSE;

    for( i = 0; i < eaSize(&e->sgStats); i++ )
    {
        if( id == e->sgStats[i]->db_id )
            return TRUE;
    }

    return FALSE;
}

static int sgroup_TargetIsDark()
{
    Entity *e;
    int i, id;

    e = playerPtr();
    id = targetGetDBID();
    if (id == 0 || e == NULL)
    {
        return TRUE;
    }

    for (i = eaSize(&e->sgStats) - 1; i >= 0; i--)
    {
        if (e->sgStats[i]->db_id == id)
        {
            return sgMemberIsDark(e->sgStats[i]);
        }
    }
    return TRUE;
}

static int sgroup_TargetRank()
{
    Entity *e;
    int i, id;

    e = playerPtr();
    id = targetGetDBID();
    if (id == 0 || e == NULL || e->supergroup == NULL)
    {
        return 0;
    }

    for (i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
    {
        if (e->supergroup->memberranks[i]->dbid == id)
        {
            return e->supergroup->memberranks[i]->rank;
        }
    }
    return 0;
}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int sgArchCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    return stricmp(stat1->arch, stat2->arch);
}

int sgOrigCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    return stricmp(stat1->origin, stat2->origin);
}

int sgZoneCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    return stricmp(stat1->zone, stat2->zone);
}


int sgNameCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    return stricmp(stat1->name, stat2->name);
}

int sgLevelCompare( const SupergroupStats **s1, const SupergroupStats **s2 )
{
    SupergroupStats *stat1 = *(SupergroupStats**)s1;
    SupergroupStats *stat2 = *(SupergroupStats**)s2;

    int result = stat2->level - stat1->level;

    if( !result )
        return sgNameCompare( s1, s2 );
    else
        return result;
}

int sgTitleCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    int result;
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    result = sgroup_rank(playerPtr(),  stat2->db_id) - sgroup_rank(playerPtr(),  stat1->db_id);
    if(0 == result)
        return sgNameCompare(s1, s2);
    return result;
}

int sgLastOnlineCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    int result;
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    result = stat2->online - stat1->online;
    
    if(0 == result)
    {
        int day1 = (timerSecondsSince2000() - stat1->last_online)/(3600*24);
        int day2 = (timerSecondsSince2000() - stat2->last_online)/(3600*24);

        if( day1 && day2 ) // only sort by last online if they are a day or more
            result = stat2->last_online - stat1->last_online;
        else if( day1 )
            result = 1;
        else if( day2 )
            result = -1;
        else
            return sgNameCompare(s1, s2);
    }
    if(0 == result)
        return sgNameCompare(s1, s2);
    return result;
}

int sgDateJoinedCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    int result;
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    result = stat1->member_since - stat2->member_since;
    if(0 == result)
        return sgNameCompare(s1, s2);
    return result;
}

int sgPresitgeCompare(const SupergroupStats** s1, const SupergroupStats** s2)
{
    int result;
    SupergroupStats* stat1 = *(SupergroupStats**)s1;
    SupergroupStats* stat2 = *(SupergroupStats**)s2;

    result = stat1->prestige - stat2->prestige;
    if(0 == result)
        return sgNameCompare(s1, s2);
    return result;
}

#define SG_BUTTON_AREA_HEIGHT    40
#define SG_INFO_PANE_HEIGHT        85
#define SG_MEMBERS_PANE_HEIGHT    200

/* Function supergroupRunTimedUpdate()
 *    Update supergroup info when timer expires or when the window starts displaying.
 */
void supergroupRunTimedUpdate()
{
#define SG_UPDATE_INTERVAL 60.f
    static float timer;
    static unsigned int reqestSGDataCount = 0;
      int displayWindow = windowUp(WDW_SUPERGROUP);

    // Set the timer to 0 so that as soon as the window starts displaying,
    // a supergroup update is triggerred.
    if(!displayWindow)
        timer = 0.f;

    if(timer < 0.f || (displayWindow && reqestSGDataCount == 0) || sgListUpdateRequired)
    {
        cmdParse("sgstats");
        timer = SG_UPDATE_INTERVAL;
        reqestSGDataCount++;
        sgListUpdateRequired = 0;
    }

    timer -= TIMESTEP/30;
#undef SG_UPDATE_INTERVAL
}


static int sgDrawGroupName(UIBox bounds, float scale, float z)
{
    FormattedText* text;
    Supergroup* group = sgGetPlayerSuperGroup();
    TTDrawContext* drawFont = &gamebold_18;
    TTFontRenderParams oldSettings = drawFont->renderParams;
    int oldDynamicFontSetting = drawFont->dynamic;
    UIBox groupNameBounds;
    AtlasTex* dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );
    int groupNameHeight = 0;
    if(!group)
        return groupNameHeight;

    font(drawFont);
    font_color(CLR_WHITE, CLR_ONLINE_ITEM);
    drawFont->renderParams.renderSize = 18;
    drawFont->dynamic = 0;
    drawFont->renderParams.italicize = 1;

    groupNameBounds = bounds;
    uiBoxAlter(&groupNameBounds, UIBAT_SHRINK, UIBAD_LEFT | UIBAD_RIGHT, dot->width * 1.25*scale);

    text = stFormatText(group->name, groupNameBounds.width, scale);
     if(text)
    {
        int textWidth;
        int textHeight;
        float dotOffset;

         textWidth = stGetTextWidth(text)*scale;
        textHeight = stGetTextHeight(text, scale);
        stDrawFormattedText(groupNameBounds, z, text, scale);

          dotOffset = (groupNameBounds.width - textWidth)/2;
           display_sprite(dot, bounds.x - dot->width*scale + dotOffset,    bounds.y + (float)textHeight/2, z, scale, scale, CLR_WHITE );
        display_sprite(dot, bounds.x + bounds.width - dotOffset,        bounds.y + (float)textHeight/2, z, scale, scale, CLR_WHITE );
        groupNameHeight = textHeight;
    }

    drawFont->renderParams = oldSettings;
    drawFont->dynamic = oldDynamicFontSetting;
    return groupNameHeight;
}

// Todo: fixup this function with Supergroup::motto is in.
static int sgDrawGroupMotto(UIBox bounds, float z, float scale)
{
    FormattedText* text;
    Supergroup* group = sgGetPlayerSuperGroup();
    TTDrawContext* drawFont = &gamebold_18;
    TTFontRenderParams oldSettings = drawFont->renderParams;
    int oldDynamicFontSetting = drawFont->dynamic;
    UIBox groupMottoBounds;
    int groupInfoHeight = 0;
    if(!group)
        return groupInfoHeight;

    font(drawFont);
    font_color(CLR_WHITE, CLR_ONLINE_ITEM);
    drawFont->renderParams.defaultSize = 12;
    drawFont->renderParams.renderSize = 14;
    drawFont->dynamic = 0;
    drawFont->renderParams.italicize = 1;

    groupMottoBounds = bounds;

    text = stFormatText(group->motto, groupMottoBounds.width, scale);
    if(text)
    {
        int textWidth;
        int textHeight;

        textWidth = stGetTextWidth(text);
        textHeight = stGetTextHeight(text, scale);
        stDrawFormattedText(groupMottoBounds, z, text, scale);
        groupInfoHeight = textHeight;
    }

    drawFont->renderParams = oldSettings;
    drawFont->dynamic = oldDynamicFontSetting;
    return groupInfoHeight;
}

void supergroupDrawInfoPane(UIBox bounds, float z, float scale)
{
    AtlasTex* ring = atlasLoadTexture("EnhncComb_result_ring.tga");
    int groupNameHeight;
        
       float ringDrawSize = (SG_INFO_PANE_HEIGHT - PIX3*2)*scale;
    float emblemDrawSize = sqrt(SQR(ringDrawSize/2)*2);    // Get the side of a square that will definitely fit in the ring.
    UIBox paneDrawArea, infoBounds;
    float buttonsize = 100*scale;
    static ScrollBar sb = {WDW_SUPERGROUP, 0 };

    // Draw the supergroup emblem
     {
        Supergroup* group = sgGetPlayerSuperGroup();
        paneDrawArea = bounds;
        uiBoxAlter(&paneDrawArea, UIBAT_SHRINK, UIBAD_TOP | UIBAD_BOTTOM, (bounds.height-ringDrawSize)*scale/2);
        uiBoxAlter(&paneDrawArea, UIBAT_SHRINK, UIBAD_RIGHT | UIBAD_LEFT, 10*scale);
        if(group && group->emblem)
        {
            AtlasTex* emblem;
            char emblemTexName[256];

            if (group->emblem[0] == '!')
            {
                emblemTexName[0] = 0;
            }
            else
            {
                strcpy(emblemTexName, "emblem_");
            }
            strcat(emblemTexName, group->emblem);
            strcat(emblemTexName, ".tga");

            emblem = atlasLoadTexture(emblemTexName);
            if(emblem && stricmp(emblem->name, "white") != 0)
                 display_sprite(emblem, paneDrawArea.x+(ringDrawSize-emblemDrawSize)/2, paneDrawArea.y+(ringDrawSize-emblemDrawSize)/2, z+1, emblemDrawSize/emblem->width, emblemDrawSize/emblem->height, CLR_WHITE);
        }
    }

    // The supergorup mode and coalition buttons
    {
        Entity* e = playerPtr();
        char* sgModeString;
        int stringWidth;
        int flag = 0;
         

         sgModeString = (e->pl->supergroup_mode ? "sgExitMode" : "sgEnterMode");
        stringWidth = str_wd(font_grp, scale, scale, sgModeString);
        if(!sgGetPlayerSuperGroup() || sgroup_isPlayerDark(e))
            flag = BUTTON_LOCKED;

         //    if(D_MOUSEHIT == drawStdButton(pen.x, pen.y, pen.z, stringWidth * 1.25*scale, 26*scale, window_GetColor(WDW_FRIENDS), sgModeString, scale, flag))
           if(D_MOUSEHIT == drawStdButton(  paneDrawArea.x + paneDrawArea.width - buttonsize/2, paneDrawArea.y + 2*paneDrawArea.height/7, z, buttonsize, paneDrawArea.height/3, window_GetColor(WDW_SUPERGROUP), sgModeString, scale, flag))
        {
            sgSetSupergroupMode(!e->pl->supergroup_mode);
        }

        //    if(D_MOUSEHIT == drawStdButton(pen.x, pen.y, pen.z, stringWidth * 1.25*scale, 26*scale, window_GetColor(WDW_FRIENDS), "AllianceMenu", scale, flag))
        if(D_MOUSEHIT == drawStdButton(  paneDrawArea.x + paneDrawArea.width - buttonsize/2, paneDrawArea.y + 5*paneDrawArea.height/7, z, buttonsize, paneDrawArea.height/3, window_GetColor(WDW_SUPERGROUP), "AllianceMenu", scale, flag))
        {
            alliance_menu( paneDrawArea.x + paneDrawArea.width - emblemDrawSize/2, paneDrawArea.y + 2*paneDrawArea.height/3);
        }
    }

    infoBounds = paneDrawArea;
    infoBounds.y -= sb.offset;

     clipperPush(&bounds);

       uiBoxAlter(&infoBounds, UIBAT_SHRINK, UIBAD_LEFT, ringDrawSize);
    uiBoxAlter(&infoBounds, UIBAT_SHRINK, UIBAD_RIGHT, buttonsize);
    groupNameHeight = sgDrawGroupName(infoBounds, scale, z);

    uiBoxAlter(&infoBounds, UIBAT_SHRINK, UIBAD_TOP, groupNameHeight);
    groupNameHeight += sgDrawGroupMotto(infoBounds, z, scale);

    clipperPush(NULL);
    doScrollBar( &sb, ringDrawSize, groupNameHeight, winDefs[WDW_SUPERGROUP].loc.wd*scale, R10*scale, z, 0, &infoBounds );
    clipperPop();
    clipperPop();
}


#define SG_ICON_SIZE    16.f

static int sgMemberIsOnline(SupergroupStats* item)
{
    if(!item)
        return 0;
    if( item->online)
        return 1;

    return 0;
}

static int sgMemberIsDark(SupergroupStats* item)
{
    Entity *e;

    e = playerPtr();
    if (e == NULL || e->supergroup == NULL || item == NULL)
    {
        return 0;
    }
    return item->playerType != e->supergroup->playerType;
}

UIBox sgListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    SupergroupStats* item = (SupergroupStats*)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    float sc = list->scale;
    int itemRank;
    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    itemRank = sgroup_rank( playerPtr(), item->db_id ); //According to Raoul, item->rank is now deprecated

    // Determine the text color of the item.
    if(item == list->selectedItem)
    {
        font_color(CLR_SELECTED_ITEM, CLR_SELECTED_ITEM);
    }
    else if(sgMemberIsOnline(item))
    {
        if (sgMemberIsDark(item))
        {
            font_color(CLR_DARK_ITEM, CLR_DARK_ITEM);
        }
        else
        {
            font_color(CLR_ONLINE_ITEM, CLR_ONLINE_ITEM);
        }
    }
    else
    {
        font_color(CLR_OFFLINE_ITEM, CLR_OFFLINE_ITEM);
    }

      for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        Supergroup* group = sgGetPlayerSuperGroup();
        if( !group )
            break;
         pen.x = rowOrigin.x + columnIterator.columnStartOffset*sc - PIX3*sc;

          clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y;
        clipBox.height = 20*sc;

          if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
            clipBox.width = 1000*sc;
        else
            clipBox.width = (columnIterator.currentWidth+5)*sc;
        
        if(stricmp(column->name, SG_MEMBER_ARCHETYPE) == 0)
        {
            const CharacterClass *pclass    = classes_GetPtrFromName( &g_CharacterClasses, item->arch );
            if( pclass )
            {
                AtlasTex* archetypeIcon = atlasLoadTexture( pclass->pchIcon );
                UIBox bounds;
                bounds = clipBox;
                if(archetypeIcon)
                {
                    display_sprite(archetypeIcon, bounds.x, bounds.y+4*sc, rowOrigin.z, SG_ICON_SIZE*sc/archetypeIcon->width, SG_ICON_SIZE*sc/archetypeIcon->height, CLR_WHITE);
                    BuildCBox( &memberTip[itemIndex][CLASS_TIP].bounds, bounds.x, bounds.y+4*sc, SG_ICON_SIZE*sc, SG_ICON_SIZE*sc );
                    setToolTip( &memberTip[itemIndex][CLASS_TIP], &memberTip[itemIndex][CLASS_TIP].bounds, pclass->pchDisplayName, &superParent, MENU_GAME, WDW_SUPERGROUP );
                }
            }
        }
        else if(stricmp(column->name, SG_MEMBER_ORIGIN) == 0)
        {
            const CharacterOrigin *porigin = origins_GetPtrFromName( &g_CharacterOrigins, item->origin);
            AtlasTex* originIcon = atlasLoadTexture(porigin->pchIcon);
            UIBox bounds;
            bounds = clipBox;

            if(originIcon)
            {
                display_sprite(originIcon, bounds.x, bounds.y+4*sc, rowOrigin.z, SG_ICON_SIZE*sc/originIcon->width, SG_ICON_SIZE*sc/originIcon->height, CLR_WHITE);
                BuildCBox( &memberTip[itemIndex][ORIGIN_TIP].bounds, bounds.x, bounds.y+4*sc, SG_ICON_SIZE*sc, SG_ICON_SIZE*sc );
                setToolTip( &memberTip[itemIndex][ORIGIN_TIP], &memberTip[itemIndex][ORIGIN_TIP].bounds, porigin->pchDisplayName, &superParent, MENU_GAME, WDW_SUPERGROUP );
            }
        }
        else if(stricmp(column->name, SG_MEMBER_LEVEL) == 0)
        {
            UIBox bounds;
            bounds = clipBox;
             cprntEx(bounds.x, bounds.y+18*sc, rowOrigin.z, sc, sc, NO_MSPRINT, "%d", item->level+1);
        }
        else if(stricmp(column->name, SG_MEMBER_NAME_COLUMN) == 0)
        {
            AtlasTex* titleIcons[] = {NULL,atlasLoadTexture("SGroup_icon_Rank04.tga"),atlasLoadTexture("SGroup_icon_Rank03.tga"),atlasLoadTexture("SGroup_icon_Rank02.tga"),atlasLoadTexture("SGroup_icon_Rank01.tga"),atlasLoadTexture("SGroup_icon_Rank00.tga")};
            UIBox bounds;
            bounds = clipBox;
            clipperPushRestrict(&clipBox);

            // Is there an icon for this member's rank?
            if(itemRank >= 0 && itemRank < NUM_SG_RANKS_VISIBLE && titleIcons[itemRank])
            {
                display_sprite(titleIcons[itemRank], bounds.x, bounds.y+2, rowOrigin.z, sc, sc, CLR_WHITE);
                BuildCBox( &memberTip[itemIndex][RANK_TIP].bounds, bounds.x, bounds.y+2*sc, titleIcons[itemRank]->width, titleIcons[itemRank]->height );

                if( itemRank )
                    setToolTip( &memberTip[itemIndex][RANK_TIP], &memberTip[itemIndex][RANK_TIP].bounds, itemRank==2?group->leaderTitle:group->captainTitle, &superParent, MENU_GAME, WDW_SUPERGROUP );
                else
                    clearToolTip( &memberTip[itemIndex][RANK_TIP] );
            }
            else
                clearToolTip( &memberTip[itemIndex][RANK_TIP] );

            bounds.x += titleIcons[2]->width*sc;

            // Draw the friend name.
            cprntEx(bounds.x, bounds.y+18*sc, rowOrigin.z, sc, sc, NO_MSPRINT, item->name);
            clipperPop();
        }
        else if( stricmp(column->name, SG_MEMBER_ZONE) == 0 )
        {
            UIBox bounds;
             bounds = clipBox;
            clipperPushRestrict(&clipBox);

               if( item->zone && item->zone[0] )
                cprntEx(bounds.x, bounds.y+18*sc, rowOrigin.z, sc, sc, 0, item->zone);
            else
                  cprntEx(bounds.x, bounds.y+18*sc, rowOrigin.z, sc, sc, 0, "FriendOffline");
            clipperPop();
        }
        else if(stricmp(column->name, SG_MEMBER_TITLE_COLUMN) == 0)
        {
            clipperPushRestrict(&clipBox);
            if(group)
                cprntEx(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, NO_MSPRINT, group->rankList[itemRank].name);

            clipperPop();
        }
        else if(stricmp(column->name, SG_MEMBER_LAST_ONLINE_COLUMN) == 0)
        {
            int days;
            clipperPushRestrict(&clipBox);
            if ((unsigned int)item->last_online > timerSecondsSince2000())
                days = 0; // If the time is in the future, dont let it get negative
            else
                days = (timerSecondsSince2000() - item->last_online)/(3600*24);

            if( days > 1)
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "DaysString", days );
            else if (sgMemberIsOnline(item))
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "OnlineString" );
            else
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "TodayString" );
            clipperPop();
        }
        else if(stricmp(column->name, SG_MEMBER_DATE_JOINED_COLUMN) == 0)
        {
            char tmp[256];
            clipperPushRestrict(&clipBox);
            if(item->member_since)
            {
                timerMakeDateStringFromSecondsSince2000(tmp, item->member_since);
                tmp[10] = '\0';
            }
            else
                strcpy( tmp, "TodayString" );

            prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, tmp );
            clipperPop();
        }
        else if(stricmp(column->name, SG_MEMBER_PRESTIGE) == 0)
        {
            clipperPushRestrict(&clipBox);
            prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "%i", item->prestige );
            clipperPop();
        }

    }
    return box;
}

static Supergroup* sgGetPlayerSuperGroup()
{
    Entity* e = playerPtr();
    if(e)
        return e->supergroup;
    else
        return NULL;
}

static SupergroupStats** sgGetPlayerSGMemberStats()
{
    Entity* e = playerPtr();
    if(e)
        return e->sgStats;
    else
        return NULL;
}

SupergroupStats* sgGetPlayerSGStats(void)
{
    Entity* e = playerPtr();
    int i;
    if(e && e->sgStats && eaSize(&e->sgStats))
    {
        for (i = 0; i < eaSize(&e->sgStats); i++)
            if (e->sgStats[i]->db_id == e->db_id)
                return e->sgStats[i];
    }
    return NULL;
}

static void sgRebuildListView(UIListView* list)
{
    SupergroupStats** stats = sgGetPlayerSGMemberStats();

    // Clear the list.  This also kills the current selection.
    uiLVClear(list);

    if(list && stats)
    {
        int i;
        int j;

        // Iterate over all supergroup memebers.
        for(i = j = 0; i < eaSize(&stats); i++)
        {
            // Add all items into the list except we skip people with the hidden GM rank
            if (stats[i]->rank != NUM_SG_RANKS - 1)
            {
                uiLVAddItem(list, stats[i]);
                j++;
            }

            // Reselect the previously selected item here.
            if(stats[i]->db_id == oldSgSelectionDBID)
            {
                // - 1 because we j++'ed a few lines above
                uiLVSelectItem(list, j - 1);
            }
        }

        // Resort the list
        uiLVSortBySelectedColumn(list);

        sgListRebuildRequired = 0;
    }
}

static void sgDrawMembersPaneScrollBar(UIListView* list, UIBox listDrawArea, ScrollBar* bar)
{

}

static void supergroupInitMembersListView(UIListView** listAddr)
{
    int i, j;

    if(!listAddr)
        return;

    if(!*listAddr)
    {
        Wdw* sgWindow = wdwGetWindow(WDW_SUPERGROUP);
        UIListView* list;

        *listAddr = uiLVCreate();
        list = *listAddr;

        uiLVHAddColumn(list->header, SG_MEMBER_LEVEL, "", 10);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_LEVEL, sgLevelCompare);

        uiLVHAddColumn(list->header, SG_MEMBER_ARCHETYPE, "", 10);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_ARCHETYPE, sgArchCompare);

        uiLVHAddColumn(list->header, SG_MEMBER_ORIGIN, "", 10);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_ORIGIN, sgOrigCompare);

        uiLVHAddColumnEx(list->header, SG_MEMBER_NAME_COLUMN, "sgMemberName", 15, 270, 1);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_NAME_COLUMN, sgNameCompare);

        uiLVHAddColumnEx(list->header, SG_MEMBER_ZONE, "sgMemberZone", 10, 210, 1);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_ZONE, sgZoneCompare);

        uiLVHAddColumnEx(list->header, SG_MEMBER_TITLE_COLUMN, "sgMemberTitle", 10, 500, 1);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_TITLE_COLUMN, sgTitleCompare);

        uiLVHAddColumn(list->header, SG_MEMBER_LAST_ONLINE_COLUMN, "sgMemberLastOnline", 100);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_LAST_ONLINE_COLUMN, sgLastOnlineCompare);
                               
         uiLVHAddColumn(list->header, SG_MEMBER_DATE_JOINED_COLUMN, "sgMemberDateJoined", 100);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_DATE_JOINED_COLUMN, sgDateJoinedCompare);

        uiLVHAddColumn(list->header, SG_MEMBER_PRESTIGE, "sgMemberPrestige", 100);
        uiLVBindCompareFunction(sgListView, SG_MEMBER_PRESTIGE, sgPresitgeCompare);
        
        uiLVFinalizeSettings(list);

        list->displayItem = sgListViewDisplayItem;

        uiLVEnableMouseOver(list, 1);
        uiLVEnableSelection(list, 1);
    }

    //init tooltips
    for( i = 0; i < MAX_SUPER_GROUP_MEMBERS; i++ )
    {
        for( j = 0; j < SUPER_TIP_TOTAL; j++ )
            addToolTip( &memberTip[i][j] );
    }
}

static void supergroupDrawMembersPane(UIBox listViewDrawArea, float scale, float z)
{
    PointFloatXYZ pen;

    if(!sgListView)
        return;

    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
    pen.z = z;

    if(sgListRebuildRequired)
        sgRebuildListView(sgListView);

    // Disable mouse over on window resize.
    {
        Wdw* sgWindow = wdwGetWindow(WDW_SUPERGROUP);
        if(sgWindow->drag_mode)
            uiLVEnableMouseOver(sgListView, 0);
        else
            uiLVEnableMouseOver(sgListView, 1);
    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(sgListView, window_GetColor(WDW_SUPERGROUP));
       uiLVSetHeight(sgListView, listViewDrawArea.height);
    uiLVHFinalizeSettings(sgListView->header);
    uiLVHSetWidth(sgListView->header, MAX( 710, listViewDrawArea.width));
    uiLVSetScale( sgListView, scale );

    clipperPushRestrict(&listViewDrawArea);
    uiLVDisplay(sgListView, pen);
    clipperPop();

    // Handle list view input.
    uiLVHandleInput(sgListView, pen);
}

static void sgSetSupergroupMode(int mode)
{
    Entity* e = playerPtr();

    mode = (mode ? 1 : 0);
    if(e->pl->supergroup_mode == mode)
        return;
    e->pl->supergroup_mode = mode;

    if( e->pl->supergroup_mode )
    {
        char* str = textStd("sgInSGMode");
        addSystemChatMsg( str, INFO_DEBUG_INFO, 0 );
    }
    else
    {
        char* str = textStd("sgExitSGMode");
        addSystemChatMsg( str, INFO_DEBUG_INFO, 0 );
    }

    sendSupergroupModeToggle();
}

typedef enum
{
    SGBC_PROMOTE    = (1 << 0),
    SGBC_DEMOTE        = (1 << 1),
    SGBC_REMOVE        = (1 << 2),
    SGBC_MOTD        = (1 << 3),
    SGBC_SETTINGS    = (1 << 4),
    SGBC_RAID        = (1 << 5),
    SGBC_QUIT        = (1 << 6),
} SGButtonCommands;

static SGButtonCommands supergroupDrawCommandButtons(UIBox buttonDrawArea, float z, float sc)
{
    char* caption;
    int buttonGroupGap = 5*sc;
    int buttonGap = 5*sc;
    UIBox group1Bounds;        // promote, demote, remove, motd buttons
    int group1ButtonCount = 4;

    UIBox group2Bounds;        // settings, quit buttons
    int group2ButtonCount = 3;

    float group1WidthPercentage = 4.0/7.0;
    UIBox buttonBox;
    int defaultButtonFlag = 0;
    int buttonFlag = 0;
    int promoteButtonLockout;

    int intendedButtonHeight = 26*sc;
    SGButtonCommands result = 0;

    if(!sgGetPlayerSuperGroup())
        defaultButtonFlag |= BUTTON_LOCKED;

    buttonDrawArea.y += (SG_BUTTON_AREA_HEIGHT*sc - intendedButtonHeight)/2;
    buttonDrawArea.height = intendedButtonHeight;

    group1Bounds = buttonDrawArea;
    group2Bounds = buttonDrawArea;

      group1Bounds.width = buttonDrawArea.width*group1WidthPercentage - buttonGroupGap/2;
       group2Bounds.width = buttonDrawArea.width*(1-group1WidthPercentage) - buttonGroupGap/2;
       group2Bounds.x = group1Bounds.x + group1Bounds.width + buttonGroupGap;

    buttonBox = group1Bounds;
    buttonBox.width = ((float)group1Bounds.width - ((group1ButtonCount-1) * buttonGap))/group1ButtonCount;

    // Draw group 1 buttons
    // Draw the promote button
    buttonFlag = 0;
    if(!sgroup_TargetIsMember() || OfflineTarget())
        buttonFlag |= BUTTON_LOCKED;

    promoteButtonLockout = 0;
    if (sgroup_TargetIsDark() && sgroup_TargetRank() == NUM_SG_RANKS - 3)
        promoteButtonLockout |= BUTTON_LOCKED;

    caption = "sgButtonPromote";
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_SUPERGROUP), caption, sc, defaultButtonFlag | buttonFlag | promoteButtonLockout))
    {
        result |= SGBC_PROMOTE;
    }

    // Draw the demote button
    caption = "sgButtonDemote";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_SUPERGROUP), caption, sc, defaultButtonFlag | buttonFlag))
    {
        result |= SGBC_DEMOTE;
    }

    // Draw the remove button
    buttonFlag = 0;
    if(!sgroup_TargetIsMember() )
        buttonFlag |= BUTTON_LOCKED;
    caption = "sgButtonRemove";
     buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_SUPERGROUP), caption, sc, defaultButtonFlag | buttonFlag))
    {
        result |= SGBC_REMOVE;
    }

    // Draw the Refresh button
    caption = "RefreshString";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_SUPERGROUP), caption, sc, 0))
        cmdParse( "sgstats" );


    // Draw group 2 buttons
    buttonBox = group2Bounds;
    buttonBox.width = ((float)group2Bounds.width - ((group2ButtonCount-1) * buttonGap))/group2ButtonCount;

    // Draw the raid button
    caption = "sgButtonRaid";
     if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_FRIENDS), caption, sc, defaultButtonFlag))
    {
        result |= SGBC_RAID;
    }

    // Draw the team settings button
    buttonFlag = 0;
    {
        Entity *player = ownedPlayerPtr();
        if (entMode(player, ENT_DEAD))
        {
            buttonFlag |= BUTTON_LOCKED;
        }
    }
     caption = "sgButtonSettings";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_FRIENDS), caption, sc, defaultButtonFlag | buttonFlag))
    {
        result |= SGBC_SETTINGS;
    }

    // Draw the quit group button
    caption = "sgButtonQuit";
    buttonBox.x += buttonBox.width + buttonGap;
     if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, CLR_RED, caption, sc, defaultButtonFlag))
    {
        result |= SGBC_QUIT;
    }

    return result;
}

void supergroupHandleCommandButtons(SGButtonCommands commands)
{
    switch(commands)
    {
    case SGBC_PROMOTE:
        sgroup_promote(NULL);
        break;
    case SGBC_DEMOTE:
        sgroup_demote(NULL);
        break;
    case SGBC_REMOVE:
        {
            char* targetName = NULL;
            char* msg = NULL;

            targetName = targetGetName();
            msg = textStd("sgRemoveConfirm", targetName);
            dialogStd(DIALOG_YES_NO, msg, NULL, NULL, sgroup_kick, NULL,1);
        }
        break;

    case SGBC_MOTD:
        {
            Supergroup* group = sgGetPlayerSuperGroup();
            dialogStd(DIALOG_OK, group->msg, NULL, NULL, NULL, NULL,1);
        }
        break;

    case SGBC_SETTINGS:
        {
            srEnterRegistrationScreen();
        }
        break;

    case SGBC_RAID:
        sgRaidListToggle();
        break;

    case SGBC_QUIT:
        {
            if (character_CountInventory(playerPtr()->pchar,kInventoryType_BaseDetail) > 0)
                dialogStd(DIALOG_YES_NO, "sgQuitConfirmDetails", NULL, NULL, sgroup_quit, NULL,1);
            else
                dialogStd(DIALOG_YES_NO, "sgQuitConfirm", NULL, NULL, sgroup_quit, NULL,1);
        }
        break;
    }
}

//
//
int supergroupWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    static int init = 0;
    static AtlasTex * arrow_up;
    static AtlasTex * arrow_down;
    AtlasTex * arrow;
    static float getData = TRUE;
     Entity *e = playerPtr();
    UIBox sgWindowDrawArea, infoPaneBounds, membersPaneBounds, commandButtonsBounds;

    // Update supergroup info when timer expires or when the window starts displaying.
    supergroupRunTimedUpdate();

    // Make sure the list view is initialized.
    supergroupInitMembersListView(&sgListView);

     if(!window_getDims(WDW_SUPERGROUP, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor))
        return 0;

    if(!init)
    {
        arrow_up = atlasLoadTexture( "Jelly_arrow_up.tga" );
        arrow_down = atlasLoadTexture( "Jelly_arrow_down.tga" );
    }

    BuildCBox( &superParent.box, x, y, wd, ht );

    sgWindowDrawArea.x = x;
     sgWindowDrawArea.y = y;
      sgWindowDrawArea.width = wd;
    sgWindowDrawArea.height = ht;

    if( optionGet(kUO_HideHeader) )
    {
        drawFrameWithBounds( PIX3, R10, x, y, z, wd, ht, color, bcolor, &membersPaneBounds );
        uiBoxAlter(&sgWindowDrawArea, UIBAT_SHRINK, UIBAD_TOP, PIX3);
        clipperPushRestrict(&sgWindowDrawArea);
    }
    else
    {
         drawJoinedFrame2Panel( PIX3, R10, x, y, z, scale, wd, SG_INFO_PANE_HEIGHT*scale, ht - SG_INFO_PANE_HEIGHT*scale, color, bcolor, &infoPaneBounds, &membersPaneBounds);

        font(&game_12);
        uiBoxAlter(&sgWindowDrawArea, UIBAT_SHRINK, UIBAD_TOP, PIX3);
        clipperPushRestrict(&sgWindowDrawArea);
        supergroupDrawInfoPane(infoPaneBounds, z+1, scale);
    }

    clipperPop();
    font(&game_12);

    // Figure out where the members pane should be drawn.
     uiBoxAlter(&membersPaneBounds, UIBAT_SHRINK, UIBAD_BOTTOM, SG_BUTTON_AREA_HEIGHT*scale);

    // Draw a scroll bar next to it if needed.
    {
        int fullDrawHeight = uiLVGetFullDrawHeight(sgListView);
        int currentHeight = uiLVGetHeight(sgListView);
        if(sgListView)
        {
            if(fullDrawHeight > currentHeight)
            {
                doScrollBar(&sgListViewScrollbar, currentHeight, fullDrawHeight, wd, membersPaneBounds.y - y + PIX3+R10, z+2, 0, &membersPaneBounds );
                sgListView->scrollOffset = sgListViewScrollbar.offset;
            }
            else
            {
                sgListView->scrollOffset = 0;
            }
        }
    }
      clipperPushRestrict(&sgWindowDrawArea);

    // Display the supergroup members pane contents.
      supergroupDrawMembersPane(membersPaneBounds, scale, z+1);

    // Did the user pick someone from the members list?
     if(sgListView->newlySelectedItem && sgListView->selectedItem)
    {
        // Which entity was selected?
        SupergroupStats* item = sgListView->selectedItem;
         Entity* player = entFromDbId(item->db_id);

         if(player)
        {
            if(player != playerPtr())
                targetSelect(player);
        }
        else
            targetSelect2(item->db_id, item->name);

        sgListView->newlySelectedItem = 0;
        oldSgSelectionDBID = 0;
    }

    // don't highlight player
    if( sgListView->mouseOverItem )
    {
        SupergroupStats* item = sgListView->mouseOverItem;
        Entity* player = entFromDbId(item->db_id);

        if( player && player == playerPtr() )
            sgListView->mouseOverItem = 0;
    }

    // Did the user select some entity in the world?
    if(sgroup_TargetIsMember())
    {
        int i;
        if(current_target)
        {
            for( i = 0; i < eaSizeUnsafe(&sgListView->items); i++ )
            {
                SupergroupStats *item = sgListView->items[i];
                if( item->db_id == current_target->db_id )
                    sgListView->selectedItem = item;
            }
        }
    }
    else
    {
        sgListView->selectedItem = 0;
        oldSgSelectionDBID = 0;
    }

    if(sgListView->selectedItem)
    {
        int index;
        SupergroupStats *item;
        index = eaFind(&sgListView->items, sgListView->selectedItem);
        
        item = sgListView->items[index];
        if( item->db_id == playerPtr()->db_id )
        {
            sgListView->selectedItem = 0;
            oldSgSelectionDBID = 0;
        }

        if(index != -1 && uiMouseCollision(sgListView->itemWindows[index]) && mouseRightClick())
        {    
            if(current_target)
                contextMenuForTarget(current_target);
            else
                contextMenuForOffMap();
        }
    }


    if( optionGet(kUO_HideHeader) && optionGet(kUO_HideButtons) )
        arrow = arrow_up;
    else
        arrow = arrow_down;

    
    // Draw and handle super group button presses.
     if( !optionGet(kUO_HideButtons) )
    {
        SGButtonCommands result;
         commandButtonsBounds = membersPaneBounds;
        commandButtonsBounds.y = membersPaneBounds.y + membersPaneBounds.height;
        commandButtonsBounds.height = SG_BUTTON_AREA_HEIGHT*scale;
         result = supergroupDrawCommandButtons(commandButtonsBounds, z+1, scale);
        supergroupHandleCommandButtons(result);
    }

    clipperPop();

     if(window_getMode(WDW_SUPERGROUP) == WINDOW_DISPLAYING )
     {
        int hit = false;
        Wdw *wdw = wdwGetWindow( WDW_SUPERGROUP );
        if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
        {
             if( D_MOUSEHIT == drawGenericButton( arrow, x + wd - 75*scale, y - (JELLY_HT + arrow->height)*scale/2, z, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
                hit = true;
        }
        else
        {
            if( D_MOUSEHIT == drawGenericButton( arrow, x + wd - 75*scale, y + ht + (JELLY_HT - arrow->height)*scale/2, z, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
                hit = true;
        }

          if( hit )
         {
            if( !optionGet(kUO_HideHeader) )
            {    
                optionSet(kUO_HideHeader, 1, 0 );
                window_setDims( WDW_SUPERGROUP, x, y, wd, MINMAX( ht - SG_INFO_PANE_HEIGHT, winDefs[WDW_SUPERGROUP].min_ht, winDefs[WDW_SUPERGROUP].max_ht));
            }
            else if( !optionGet(kUO_HideButtons) )
            {
                optionSet(kUO_HideButtons, 1, 0 );
                window_setDims( WDW_SUPERGROUP, x, y, wd, MINMAX(ht - SG_BUTTON_AREA_HEIGHT, winDefs[WDW_SUPERGROUP].min_ht, winDefs[WDW_SUPERGROUP].max_ht));
            }
            else
            {
                optionSet(kUO_HideHeader, 0, 0 );
                optionSet(kUO_HideButtons, 0, 0 );
                window_setDims( WDW_SUPERGROUP, x, y, wd, MINMAX(ht + SG_BUTTON_AREA_HEIGHT + SG_INFO_PANE_HEIGHT, winDefs[WDW_SUPERGROUP].min_ht, winDefs[WDW_SUPERGROUP].max_ht));
            }

            sendOptions(0);
        }
    }
    return 0;
}





