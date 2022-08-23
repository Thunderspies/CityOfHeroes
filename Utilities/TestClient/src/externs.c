#include <stdarg.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/ConvertUtf.h>
#include <utilitieslib/utils/StringUtil.h>
#include "entity/Proficiency.h"
#include <stdio.h>
#include <stdlib.h>
#include "graphics/camera.h"
#include "entity/entity.h"
#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdcommon.h"
#include "entity/costume.h"
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "varutils.h"
#include "group/group.h"
#include <utilitieslib/language/MessageStore.h>
#include "UI/uiKeybind.h"
#include "entity/entrecv.h"
#include "UI/uiEditText.h"
#include "UI/uiCompass.h"
#include "group/groupdyn.h"
#include "graphics/gfxwindow.h"
#include "UI/uiInput.h"
//#include "ttFont.h"
#include "UI/uiCursor.h"
#include "UI/uiReticle.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/memcheck.h>
#include "gridcoll/ctri.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiEmail.h"
#include "testUtil.h"
#include <utilitieslib/utils/error.h>
#include "player/player.h"
#include "entity/powers.h"
#include "UI/uiChat.h"
#include <utilitieslib/utils/SimpleParser.h>
#include "clientcomm/clientcomm.h"
#include "group/groupscene.h"
#include "entity/motion.h"
#include "player/badges.h"
#include "entity/chatSettings.h"
#include "UI/uiAutomap.h"
#include <utilitieslib/components/mailbox.h>
#include "gameData/arenastruct.h"
#include "seq/seq.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include "entity/power_system.h"
#include "storyarc/contactClient.h"
#include "ArenaFighter.h"
#include "entity/Concept.h"
#include "UI/uiUtil.h"
#include "entity/character_net.h"
#include "player/inventory_client.h"
#include "UI/uiComboBox.h"
#include "gameData/costume_data.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "graphics/splat.h"
#include "UI/uiPet.h"
#include "graphics/groupdraw.h"
#include "graphics/sun.h"
#include "entity/ragdoll.h"
#include "UI/uiLogin.h"
#include "testAccountServer.h"
#include "clientcomm/dbclient.h"
#include "gameComm/initClient.h"
#include "bases/bases.h"
#include "UI/uiFriend.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/truetype/ttFontDraw.h"
#include "UI/uiNet.h"
#include "UI/uiClipper.h"
#include "UI/sprite/sprite_base.h"


// Include for redirecting chat to the launcher
#include "chatter.h"
//extern void sendChatToLauncher(const char *fmt, ...);

int globMapLoadedLastTick = 0;
char gLastPrivate[32];
int mission_map=0;
U32 g_timeLastAttack = 0;
U32 g_timeLastPower = 0;
char gMapName[256] = {0};
char world_name[MAX_PATH];
int            email_header_count;
GameState game_state;
ControlState control_state;
ServerVisibleState server_visible_state;
GlobalState global_state;
KeyBindProfile game_binds_profile;
char g_achAccountName[32];
int gPlayerNumber;
int gServerNumber;
GfxWindow gfx_window;
Entity    *current_target=NULL;
char *g_ServerName = NULL;
char g_shardVisitorData[64];
U32 g_shardVisitorDBID;
U32 g_shardVisitorUID;
U32 g_shardVisitorCookie;
U32 g_shardVisitorAddress;
U16 g_shardVisitorPort;

void receiveItemLocations(Packet *pak){return;}
void receiveCritterLocations(Packet *pak){return;}
void souvinerClueAdd( void* clue ){return;}
void updateSouvenirClue( int uid, char * desc ){return;}
void addPlayerCreatedSouvenir(int uid, char * title, char * description ){return;}
void missionReview_Set( int id, int test, char * pchName,  char * pchAuthor, char * pchDesc, int rating, int euro ){return;}
void missionReview_Complete( int id ){return;}

void playerCreatedStoryArc_Send(Packet * pak, void *pArc){return;}
void setClientSideMissionMakerContact(int foo, Costume * pCostume, char * pchName, int bar){return;}

#include "UI/uiOptions_Type.h"
#include "UI/uiOptions_Type.c"

int optionGet(int i){ return 0; }
void optionSet(int i, int val, bool save ){ return; }
F32 optionGetf(int i){ return 0; }
void optionSetf(int i, F32 val, bool save ){ return; }

//void auction_AddToDict( void *p, int i  ){ return; }

//UserOption *userOptionGet(int id){ return &game_options[id]; }
void playerNote_addGlobalName( char * a, char * b ){ return;}
int gTeamComplete = 0;
int gShowSalvage = 0;
F32 click_to_move_dist=1;
int gZoomed_in=0;
int green_blobby_fx_id=0;
int gFakeArachnosSoldier=0;
int gFakeArachnosWidow=0;
//mouse_input gMouseInpBuf[MOUSE_INPUT_SIZE] = { 0 };
mouse_input gMouseInpCur  = {0};
//mouse_input gMouseInpLast = {0};
TTDrawContext game_9, game_12, game_14, game_18, gamebold_9, gamebold_12, gamebold_14, gamebold_18, hybridbold_12;
int title_9, title_12, title_14, title_18, title_24, chatThin, chatFont, chatBold, entityNameText, referenceFont;
void doScrollBarEx( void * v, int i, int i2, int x, int y, int z, void * v2, void * v3, float a, float b ){ return; }
void receiveDebugPower(Packet * pak){ return; }
Mailbox globalEntityFxMailbox;
Cursor    cursor;
int entering_chat_text;
int gSelectedDBID = 0;
Destination **taskDests = NULL; // earray of task locations (includes mission door)
Show g_eShowArchetype       = kShow_OnMouseOver;
Show g_eShowSupergroup      = kShow_OnMouseOver;
Show g_eShowPlayerName      = kShow_Always;
Show g_eShowPlayerBars      = kShow_Selected|kShow_OnMouseOver;
Show g_eShowVillainName     = kShow_Selected|kShow_OnMouseOver;
Show g_eShowVillainBars     = kShow_Selected|kShow_OnMouseOver;
Show g_eShowPlayerReticles  = kShow_Selected|kShow_OnMouseOver;
Show g_eShowVillainReticles = kShow_Selected|kShow_OnMouseOver;
Show g_eShowAssistReticles  = kShow_Selected|kShow_OnMouseOver;
Show g_eShowOwnerName       = kShow_HideAlways;
Costume * gSuperCostume;
int gUseToolTips = TRUE;
int gCurrentClass = 0;
int gCurrentOrigin = 0;
int gCurrentPower[2] = {-1,-1};
int gCurrentPowerSet[2] = {-1,-1};
int gCurrentGender = 1;
int gDivSuperName;
int gDivSuperMap;
int gDivSuperTitle;
int gDivEmailFrom;
int gDivEmailSubject;
int gDivFriendName;
int gDivLfgName;
int gDivLfgMap;
int gLoadRandomPresetCostume;
float gToolTipDelay;
int g_iMaxBadgeIdx;
int gAdvancedPetControls;
int gShowPetControls;
int gPreventPetIconDrag;
//SHARED_MEMORY BadgeDefs g_SgroupBadges;

//int gSelectedTask = -1;
int g_IdxSelectedTask = -1;
int g_IdxSelectedContact = -1;
int gWaitingToEnterGame = 0;
TaskStatus zoneEvent;

Base g_base;

int dump_grid_coll_info;
int g_IdStore = 0;

int    collisions_off, collisions_off_for_rest_of_frame;
int    gChatBubbleColor[4];

SceneInfo scene_info;

//SHARED_MEMORY BadgeDefs g_BadgeDefs;
StashTable gShardCommClientCmds;

bool gWebHideStats;
bool gWebHidePowers;
bool gWebHideBadges;
bool gWebHideFriends;
int FreeTailorCount = 0;
bool bFreeTailor = 0;
bool bUsingDayJobCoupon = 0;
bool bUltraTailor = 0;
bool bVetRewardTailor = 0;
int gAuctionDisabled;
ArenaEvent gArenaDetail;
ArenaEvent gArenaLast;
int gArenaInvited = 0;

UIColors uiColors;
ComboBox comboAnims = {0};

EntityRef g_erTaunter;
EntityRef **g_pperPlacators;
// camera.c
CameraInfo    cam_info;
void camSetPyr(Vec3 pyr) {;}
void camSetPlayerPyr(Vec3 pyr) {;}
void markBuffForDeletion(Entity *e, int foo){ return; }
SunLight g_sun;


CmdList game_cmdlist =
{
    {{ client_control_cmds},
    { control_cmds},
    { 0 }}
};



void entDebugAddLine(Vec3 p1, int argb1, Vec3 p2, int argb2) {;}
void entDebugAddText(Vec3 pos, const char* textString, int flags) {;}

void ServerMsg( char *txt ) {
    printf("%s", txt);
    sendChatToLauncher("ServerMsg:0:%s",txt);
}

void conPrintf(char const *fmt, ...)
{
    char str[20000];

    
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);
    //if (strncmp(str, "time ", 5)==0) return;
//    if (strncmp(str, "got player ", strlen("got player "))==0) return;
    {
        char mbcsStr[ARRAY_SIZE( str )];
        strcpy(mbcsStr,str);
        utf8ToMbcs(mbcsStr,ARRAY_SIZE( mbcsStr ));
        printf(" [con:] %s\n", mbcsStr);  
    }
    
    sendChatToLauncher("conPrintf:0:%s",str);
    arenaFighterListen(str);
}

void cprntEx(float x, float y, float z, float xsc, float ysc, int flags, char  * fmt, ...)
{
    char str[20000];
    va_list ap;

    if (!fmt)
        return;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);
    printf("%s\n", str);
    sendChatToLauncher("cprnt:0:%s\n",str);
}

void cprnt(float x, float y, float z, float xsc, float ysc, char  * fmt, ...)
{
    char str[20000];
    va_list ap;

    if (!fmt)
        return;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);
    printf("%s\n", str);
    sendChatToLauncher("cprnt:0:%s\n",str);
}

void dialogRemove(char *pch, void (*code1)(void *data), void (*code2)(void *data) ){ return;}

void font_color(int clr0, int clr1) {;}
void font(void * f) {;}
void font_outl(int i) {;}

void recipeInventoryReparse(){;}
void uiIncarnateReparse();
void team_Quit(void *f){;}
void leagueOrganizeTeamList(League *league){;}
void leagueCheckFocusGroup(Entity *e){;}
void leagueClearFocusGroup(Entity *e){;}
void league_updateTabCount(){;}
typedef struct VisitedStaticMap VisitedStaticMap;
typedef struct VisitedMap VisitedMap;
VisitedStaticMap *automap_getMapStruct(int db_id)    {    return NULL;    }
void automap_addStaticMapCell(VisitedStaticMap *vsm, int db_id, int map_id, int opaque_fog, int num_cells, U32 *cell_array) {;}
void automap_updateStaticMap(VisitedMap *vm) {;}
void automap_clearStaticMaps(VisitedStaticMap *vsm) {;}
void MissionUpdateAll()
{
}

void MissionClearAll()
{
}

void MissionWaypointReceive(Packet* pak)
{
    int newWaypoints = pktGetBits(pak, 1);
    while (pktGetBits(pak, 1))
    {
        pktGetBitsPack(pak, 1);
        if (newWaypoints)
        {
            pktGetBitsAuto(pak);
            pktGetF32(pak);
            pktGetF32(pak);
            pktGetF32(pak);
        }
    }
}

void MissionKeydoorReceive(Packet* pak)
{
}

void init_TextLinkStash(void){}

void MissionEditorReceiveVillains(Packet* pak)
{
}

void MissionEditorReceiveClues(Packet* pak)
{
}

void MissionEditorReceiveUpdate(Packet* pak)
{
}

void MissionEditorReceiveTaskList(Packet* pak)
{
}

void MissionEditorReceiveLoaded(Packet* pak)
{
}

void MissionEditorReceiveStatus(Packet* pak)
{
}

void MissionEditorReceiveSpawnInc(Packet* pak)
{
}

void MissionEditorReceiveCameraPos(Packet* pak)
{
}

void MissionEditorReset()
{
}

void plaquePopup(char* str)
{
}

void entMotionFreeColl(Entity *e)
{
    if (!e) return;
    assert(!e->coll_tracker);
    if (!e->coll_tracker)
        return;
    //groupRefGridDel(e->coll_tracker);
    free(e->coll_tracker->def);
    free(e->coll_tracker);
    e->coll_tracker = 0;
}

SeqInst * seqLoadInst( const char * type_name, GfxNode * parent, int load_type, int randSeed, Entity *entParent  ) { return NULL;}
SeqInst * seqFreeInst(SeqInst * seq) { return NULL; }
int seqGetSeqStateBitNum(const char* statename) { return 1; }
int seqSetStateFromString(U32 *state, char *newstatestring) {return 0;}
int getMoveFlags(SeqInst * seq, eMoveFlag flag) { return 0; }
void seqSetState(U32 *state,int on,int bitnum) {;}
void seqOrState(U32 *state,int on,int bitnum) {;}
bool testSparseBit(const SparseBits* bits, U32 bit) {return false;}
void changeBoneScale( SeqInst * seq, F32 newbonescale ) {;}
void changeBoneScaleEx( SeqInst * seq, const Appearance * appearance ) {;}
void seqAssignBonescaleModels( SeqInst *seq, int *randSeed ) {;}

void gridClearAndFreeIdxList(Grid *grid,GridIdxList **grids_used) {;}
void gridFreeAll(Grid *grid) {;}
#ifndef TEST_CLIENT
void groupProcessDefTracker(GroupInfo *group_info, GroupDefTrackerProcessor process) {;}
#endif
//DefTracker *groupFindInside(Vec3 pos,FindInsideOpts find_type, VolumeList * volumeList) {return NULL;}
DefTracker *groupFindInside(const Vec3 pos,FindInsideOpts find_type, VolumeList * volumeList, int * didVolumeListChangePtr ) {return NULL;}

GroupInfo group_info;

void printToLog(int message_priority, char const *fmt, ...) {;}
void printToScreenLog(int message_priority, char const *fmt, ...) {;}

void entDebugReceiveCommands(Packet* pak) {;}
void cmdOldDebugHandle(Packet* pak) {;}

void editNetUpdate() {;}
void finishLoadMap(int tray_link_all) { editNetUpdate(); }

char glob_map_name[MAX_PATH] = {0};

void receiveLoadScreenPages(Packet *pak)
{
    if (pktGetBits(pak, 1))
    {
        int index, count = pktGetBitsAuto(pak);

        for (index = 0; index < count; index++)
        {
            pktGetString(pak);
        }
    }
}

void worldReceiveGroups(Packet *pak) {
    int new_world;
    int editorUndosRemaining; // not used
    int editorRedosRemaining; // not used

    // not used
    editorUndosRemaining = pktGetBitsPack(pak,1);
    editorRedosRemaining = pktGetBitsPack(pak,1);

    
    new_world = pktGetBits(pak,1);
    if (new_world)
    {
        game_state.mission_map = pktGetBits(pak,2);
        game_state.map_instance_id = pktGetBitsPack(pak,1);
        game_state.doNotAutoGroup = pktGetBitsPack(pak,1); //MW added
        game_state.base_map_id = pktGetBitsPack(pak, 1);
        game_state.intro_zone = pktGetBitsAuto(pak);
        game_state.team_area = pktGetBitsPack(pak, 3);

        receiveLoadScreenPages(pak);

        pktGetString(pak); // loadscreen

        strcpy(glob_map_name,pktGetString(pak));
    }

    finishLoadMap(0);
    groupDynBuildBreakableList();
    return;
}

void receiveDynamicDefChanges(Packet *pak) {;}

void windows_initDefaults(void) {;}
int initWindows() { return 1;}
void start_menu( int menu ) {;}
void gameStateLoad() {;}

void receiveKeyBindings( Packet *pak, KeyBind *kbp, int * mouse_invert, F32 * mouse_speed, F32 * turn_speed ) {
    int i;
    for( i = 0; i < BIND_MAX; i++ )    {
        pktGetString( pak );
        pktGetBits( pak, 1 );
    }
    pktGetBits( pak, 1 );
    pktGetF32( pak );
    pktGetF32( pak );
}

void zoClearOccluders(void){;}
void loadUpdate(char *msg,int num_bytes) {;}
void showBgAdd(int val) {;}
int cmdParse(char *str) { commAddInput(str); return 0; }
void setState( int st, int td ) {;}
void setCompassDest( int on, int type, char* name, char* map, Vec3 position, int id, TexBind * icon ) {;}
TexBind *texLoadStd( char * sprite_name ) { return NULL; }
TexBind *texLoad( const char *name, int mode, TexUsage use_purpose) { return NULL; }
AtlasTex *atlasLoadTextureEx(const char *sprite_name, int one_use_only) { return NULL; }
AtlasTex *atlasLoadTexture(const char *sprite_name) { return NULL; }
BasicTexture *texLoadBasic(const char *name, int mode, TexUsage use_purpose) { return NULL; }
BasicTexture *texFind(const char *name) { return NULL; }
TexBind *texFindComposite(const char *name) { return NULL; }
void dock_setLocked(int mode) {;}
void DoE3Continue() {;}
void gfxReload() {;}
void gfxPrepareforLoadingScreen() {;}
void entDebugClearServerPerformanceInfo() {;}
void entDebugReceiveServerPerformanceUpdate(Packet* pak) { assert(!"Test Clients shouldn't be getting debug info!");}
void sndVolumeControl(int sound_type,F32 volume) {;}

// For player.c link:
int editMode() {return 1;}
int mouse_dx,mouse_dy;
int inpIsMouseLocked() {return 1;}
void entMotion(Entity *e, const Mat3 control_mat) {;}
void playerMoveStickyPlayer(void) {;}
void camSetPos(const Vec3 pos, bool interpolate ) {;}

// pmotion.c:
void pmotionSetState(Entity *e,ControlState *controls) {;}
void pmotionSetVel(Entity *e,ControlState *controls,int update_pyr) {;}
void pmotionCheckVolumeTrigger(Entity *e) {;}

void inspirationNewData() {;}

int sndIsLoopingSample(char *name) {return 0;}
void sndPlay(char *name,int owner) {;}
void sndPlayEx(char *name,int owner, F32 volRatio, int flags) {;}
void sndStop(int owner, F32 fadeOutSecs) {;}
int isMenu( int menu ) { return TRUE; }
void forceGeoLoaderToComplete(void) {;}

DefTracker *groupFindOnLine(Vec3 start,Vec3 end,Mat4 mat,bool* hitWorld,int use_notselectable) {
    return NULL;
}
void groupLoadIfPreload(GroupDef *gd) {;}

void gfxCursor3d(F32 x,F32 y,F32 len,Vec3 start,Vec3 end) {;}
INLINEDBG F32 LineLineDist(    const Vec3 L1pt, const Vec3 L1dir, F32 L1len, Vec3 L1isect,
                          const Vec3 L2pt, const Vec3 L2dir, F32 L2len, Vec3 L2isect )
{
    return 0.0;
}
//F32 LineLineDist(Vec3 L1pt, Vec3 L1dir, F32 L1len, Vec3 L1isect, Vec3 L2pt, Vec3 L2dir, F32 L2len ) { return 0.0;}
void getCapsule( SeqInst * seq, EntityCapsule * cap ){}
int collGrid(Grid *grid,const Vec3 start,const Vec3 end,CollInfo *coll,F32 radius,int flags) { return 0;}

void toggle_3d_game_modes(int mode) {;}
void texLoadQueueFinish() {;}
void gfxLoadRelevantTextures() {;}
int load_bar_color;
void texLoadQueueStart() {;}
void texDynamicUnload(int enable) {;}
void texDisableThreadedLoading(void) {;}
void texEnableThreadedLoading(void) {;}

void inspiration_setMode( int mode ) {;}
void info_EntityDescUpdate( char *text ) {
    conPrintf("info_EntityDescUpdate(\"%s\")\n", text);
}


void calcTimeStep(void) {
    // Copied from gfx.c showFrameRate()
    U32        curr,delta;
    F32        time;
    static    U32 last_ticks;
    static    U32 frame_count;
    static    U32 last_fps_ticks;
    static  F32 largest_time_step = 0;
    static    F32 smallest_time_step = 100;
    static  F32 sum_time_step = 0;
    static  F32 sum_frames = 0;
    static  F32    longtime;
    static  F32 shorttime;
    static  F32 avgtime;
    extern F32 glob_otherclient_timescale;

    //Calculate the TIMESTEP - length of last frame in 30ths of a sec
    curr = timerCpuTicks();
    delta = curr - last_ticks;
    if(game_state.frame_delay)
    {
        Sleep(min(game_state.frame_delay, 1000));
        last_ticks = timerCpuTicks();
    }
    else
    {
        last_ticks = curr;
    }
    time = (F32)delta / (F32)timerCpuSpeed();
    TIMESTEP = 30.f * time;
    global_state.frame_time_30_real = TIMESTEP;

    //Check for large and small timesteps.
    if(largest_time_step < global_state.frame_time_30)
        largest_time_step = TIMESTEP;
    if(smallest_time_step > TIMESTEP)
        smallest_time_step = TIMESTEP;
    sum_time_step += TIMESTEP;
    sum_frames++;

    //Set Maximum Timestep
    if (TIMESTEP > 30)
        TIMESTEP = 30;

    if (game_state.spin360)
    {
        TIMESTEP = 0.00001;
        global_state.frame_time_30_real = TIMESTEP;
    }
    frame_count++;
}

void window_setMode(int wdw, int mode ) {;}
void window_setDims( int wdw, float x, float y, int width, int height ) {;}

void tray_init(void) {;}
void tray_AddObj( TrayObj *to ) {;}

GfxNode *gfxTreeInsert(GfxNode *parent) { return NULL; }
GfxNode *fxFindGfxNodeGivenBoneNum(SeqInst * seq, int bonenum) { return NULL; }
bool gfxTreeParentIsVisible(GfxNode * node) {return true;}


void restartLoginScreen() {
    if (testAccountIsTransferPending())
    {
        testAccountLoginReconnect();
    }
    else
    {
        testClientLog(TCLOG_FATAL, "Was booted back to login screen\n");
        FatalErrorf("Booted back to login screen");
    }
}

void restartCharacterSelectScreen() {
    if (testAccountIsTransferPending())
    {
        testAccountLoginReconnect();
    }
    else
    {
        testClientLog(TCLOG_FATAL, "Was booted back to character select screen\n");
        FatalErrorf("Booted back to character select screen");
    }
}

int cmdAccessLevel()
{
    Entity    *p = playerPtr();

    if (p)
        return p->access_level;
    return 10;
}

void seqSetOutputs(U32 * state, const U32 * addedState) {;}
void mapperSetVisitedCells(U32 *cells,int num_cells) {;}
int shell_menu() { return 0; }
int loadingScreenVisible() { return 0; }
void status_printf(char const *fmt, ...)
{
    va_list ap;
    char buf[1024];
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    printf("%s\n",buf);
    sendChatToLauncher("status_printf:0:%s\n",buf);
}

void ClearUnclaimedFloaters(void) {;}
void DistributeUnclaimedFloaters(Entity *e) {;}

void bindKey( const char *keyname, const char *command, int save ) {;}
void keybinds_init( const char *profile, KeyBindProfile *kbp, KeyBind *kb ) {;}
void window_closeAlways(void) {;}
bool seqGetMoveIdxFromName( const char * moveName, const SeqInfo * seqInfo, U16* iResult ) { return false; }
bool seqAInterruptsB( SeqMove * a, SeqMove * b ) { return false; }
void seqManageHitPointTriggeredFx( SeqInst * seq, F32 percentHealth, int randomNumber ) {;}

void displayEntDebugInfoTextEnd() {;}
void displayEntDebugInfoTextBegin() {;}
void drawFilledBox(int x1, int y1, int x2, int y2, int argb) {;}

void mapfogSetVisitedCellsFromServer(U32 *cells,int num_cells) {;}
void tray_setMode(int mode) {;}
void gfxSetBG(char *texname) {;}
void gfxSetBGMapname(char *texname) {;}
void camSetFunc(void (*camfunc)()) {;}
void camLock(CameraInfo *ci) {;}

void xyprintf(int x,int y,char const *fmt, ...) {;}
void friendListPrepareUpdate() {;}
long texLoadsPending(int includeTexWords) { return 0; }


extern void arenaFighterHandleFloatingText(char * pch);
void attentionText_add(char *pch, int color, int wdw, char *pchSound) { 
    printf("attentionText: %s\n", pch); 
    sendChatToLauncher("attentionText:0:%s",pch);
    arenaFighterHandleFloatingText(pch);
}
void priorityAlertText_add(char *pch, U32 color, char *pchSound)
{
    printf("priorityAlertText: %s\n", pch); 
    sendChatToLauncher("priorityAlertText:0:%s",pch);
    arenaFighterHandleFloatingText(pch);
}
void sgListPrepareUpdate() {;}
void uiReset() {;}

void trade_setInspiration(BasePower * pow, int i, int j) {;}

int status_addNotification(AtlasTex *icon, void (*code)(), char *tooltip) {return 1;}
void status_removeNotification(AtlasTex * icon) {}

void browserOpen() {;}
void browserSetText(char *s) {;}
void browserClose() {;}
int FxDebugGetAttributes( int fxid, int attribs ) { return 0; }
void uiRegisterCopyToEntity(Entity* e, int copyName) {;}

void scrollToSouvenirClue(int uid){;}
void souvenirCluePrepareUpdate(){;}

void scissor_dims(int left, int top, int wd, int ht) {;}
void set_scissor_dbg(int x, char* filename, int lineno) {;}
int drawStdButton( float cx, float cy, float z, float wd, float ht, int color, char *txt, float txtSc, int flags ) { return 0; }
void setHelpOverlay( char *texName ) {;}
void lfg_add( char * name, int archetype, int origin, int level, int lfg, int hidden, int teamsize, int leader, int sameteam, 
             int leaguesize, int leagueleader, int sameleague, char *mapName, int dbid, int arena_map, int mission_map, 
             int other_faction, int faction_same_map, int other_universe, int universe_same_map, char *comment ) {;}
void lfg_clearAll() {;}

void storeOpen(int id, char *pchStore) {;}
void storeClose(void)  {;}

void receiveMissionSurvey(Packet* pak) {;}
void quitToLogin(void * data) { restartLoginScreen(); }
void quitToCharacterSelect(void *data) { restartCharacterSelectScreen(); }
void titleselectOpen(int i) {;}
void srEnterRegistrationScreen() {;}
void dialogClearQueue( int game ) {;}
void setTeamBuffMode( int mode ) {;}

void addSystemChatMsg(char *s, int type, int id)
{
    addToChatMsgs(s, type, NULL);
}
void addSystemChatMsgEx(char *s, int type, float duration, int id, Vec2 position)
{
    addToChatMsgs(s, type, NULL);
}

void addChatMsgDirect(char *s, int type)
{
    addToChatMsgs(s, type, NULL);
}

void combo_ActOnResult( int success, int slot ) {;}
int creatingSupergroup() {return 0;}

void mapSelectReceiveInit(Packet *pak){;}
void mapSelectReceiveUpdate(Packet *pak) {}
void mapSelectReceiveClose(Packet *pak) {;}

void showBgReset() {;}

void loadScreenResetBytesLoaded(void) {;}
void missionSummaryInvalidate(void* set) {;}

void ArenaStart(int iTimeToStart, int iTimeToRun) {;}
void ArenaStop(void) {;}

int tailor_RevertBone( int boneID, int type ) { return 0;}
void tailor_RevertCostumePieces( Entity *e) {;}
void tailor_exit() {;}
int gEnterTailor = 0;
void srClearAll(void) {;}

int seqAddCostumeFx(Entity *e, const CostumePart *costumeFx, const BodyPart *bp, int index, int numColors, int incompatibleFxFlags) { return 1;}
int seqDelAllCostumeFx(SeqInst *seq, int firstIndex) { return 1; }

void lfg_setNameWidth() {;}

MP_DEFINE(MotionState);

MotionState* createMotionState()
{
    MP_CREATE(MotionState, 64);
    return MP_ALLOC(MotionState);
}

void destroyMotionState(MotionState* motion)
{
    MP_FREE(MotionState, motion);
}

void chatSetFilters( int top, int bot ) {;}
void respec_validate(bool success) {;}
void info_clear() {;}

void receiveScriptDebugInfo(Packet *pak) {;}
void addRewardChoice( int i, int j, char *s ) { printf(__FUNCTION__ ": %d,%d,%s\n", i, j, s);}
void setRewardDesc( char *s ) { printf(__FUNCTION__ ": %s\n", s); sendRewardChoice(0); }
void setRewardDisableChooseNothing( bool flag ) {;}
void clearRewardChoices(void) {;}

void updateClothFrame() {;}

char* groupDefFindPropertyValue(GroupDef* def, char* propName) {return NULL;}
int pmotionGetNeighborhoodPropertiesInternal(DefTracker *neighborhood,char **namep,char **musicp) {return 0;}
void pet_update(int i, PetStance s,PetAction a) {;}
void pet_Add( int id ) {;}
void pets_Clear(void) {;}
bool playerIsMasterMind()
{ return 0; }
TexBind *white_tex_bind, *invisible_tex_bind, *grey_tex_bind, *black_tex_bind; 
AtlasTex *white_tex_atlas = 0;
BasicTexture *white_tex, *invisible_tex, *grey_tex, *black_tex; 
bool gShowPets = 0;


void receiveChatSettingsFromServer(Packet * pak, Entity * e)
{
    receiveChatSettings(pak, e);
    START_INPUT_PACKET(pakout, CLIENTINP_SEND_CHAT_SETTINGS);
    sendChatSettings(pakout, e);
    END_INPUT_PACKET
}
void receiveCostumeChangeEmoteList(Packet *pak)
{
}

bool UsingChatServer() {return 0;}

typedef struct StatusParams
{
    int size;    
    int offset;
    int type;

}StatusParams;
#define GFITEM(x)    SIZEOF2(GlobalFriend, x), OFFSETOF(GlobalFriend, x) 
StatusParams gStatusParams[] =
{
    {GFITEM(name),        PARSETYPE_STR},
    {GFITEM(dbid),        PARSETYPE_S32},        
    {GFITEM(map),        PARSETYPE_STR},
    {GFITEM(archetype),    PARSETYPE_STR},
    {GFITEM(origin),    PARSETYPE_STR},
    {GFITEM(teamSize),    PARSETYPE_S32},
    {GFITEM(xpLevel),    PARSETYPE_S32},
    { 0 }
};

static char* emailBuildDateString(char* datestr, U32 seconds)
{
    __int64 x;
    SYSTEMTIME    t;
    FILETIME    local;
    char* postfix;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    if(t.wHour > 12)
    {
        t.wHour -= 12;
        postfix = "PM";
    }
    else
    {
        postfix = "AM";
    }


    sprintf(datestr,"%02d/%02d/%02d %d:%02d ",t.wMonth,t.wDay,t.wYear-2000,t.wHour,t.wMinute);
    strcat(datestr, postfix);

    return datestr;
}

bool processShardCmd(char **args,int count)
{
    if(!stricmp(args[0], "InvalidUser") && count == 2)
    {
        char *handle = args[1];
        if( ! stricmp(handle, game_state.chatHandle))
            addSystemChatMsg( textStd("InvalidUserSelf"), INFO_USER_ERROR, 0);
        else
            addSystemChatMsg( textStd("InvalidUser %s", handle), INFO_USER_ERROR, 0 );    
    }
    else if(!stricmp(args[0], "WhoGlobal") && count == 3)
    {
        char *globalName = args[1];
        char *status = args[2];
        char buf[1000];
        char *info[100] = {0};

        Strncpyt(buf, unescapeString(status));
        tokenize_line_safe(buf,info,(ARRAY_SIZE(gStatusParams)-1),0);    // skip the first double-quote
        addSystemChatMsg( textStd("%s has the global handle: %s", info[0], globalName ), INFO_SVR_COM, 0 );
    }
    else if(!stricmp(args[0], "CsrMailTotal") && count == 4)
    {
        char *playerName = args[1];
        char *inbox = args[2];
        char *sent = args[3];
        addSystemChatMsg( textStd("GMails for User %s -- %s mails sent with attachments in last 30 days -- %s mails in received in inbox",playerName,sent,inbox), INFO_SVR_COM, 0 );
        addSystemChatMsg( textStd("To return emails to their sender use either /bounce_mail_sent <id> <name> or /bounce_mail_recv <id> <name> commands."), INFO_SVR_COM, 0  );
    }
    else if(!stricmp(args[0], "CsrMailInbox") && count == 6)
    {
        char * id = args[1];
        char * subject = args[2];
        char * attachment = args[3];
        char * from = args[4];
        char * sent = args[5];
        char date[100];
        emailBuildDateString(date,atoi(sent));
        addSystemChatMsg( textStd("ID: %s SENT:%s FROM: %s<br>SUBJECT: %s<br>ATTACHMENT: %s<br><br>", id, date, from, subject, attachment ), INFO_SVR_COM, 0 );
    }
    else if(!stricmp(args[0], "CsrMailSent") && count == 6)
    {
        char * id = args[1];
        char * sent = args[2];
        char * attachment = args[3];
        char * sentTo = args[4];
        char * status = args[5];
        char date[100];
        emailBuildDateString(date,atoi(sent));
        addSystemChatMsg( textStd("ID: %s SENT:%s TO: %s<br>ATTACHMENT: %s<br>STATUS: %s<br><br>", id, date, sentTo, attachment, status ), INFO_SVR_COM, 0 );
    }
    return true;
}

void badgeReparse(void) {;}
void info_addTab( char * name, char * text ) {;}
void info_noResponse(void) {;}
void info_addBadges( Packet * pak ) { assert(!"not handled"); }
void info_addSalvageDrops(Packet *pak) { assert(!"not handled"); }

float camGetPlayerHeight() { return 0.0; }

GlobalMotionState global_motion_state;

void winErrorDialog(char *str, char* title, char* fault) { printf("%s %s %s", str, title, fault); }

// *********************************************************************************
//  uiOptions 
// *********************************************************************************
bool gShowEnemyTells;
bool gShowEnemyBroadcast;
bool gHideEnemyLocal;
bool gDeclineSuperGroupInvite;
bool gDeclineTradeInvite;
bool gDeclineChatChannelInvite;
bool gLogChat = FALSE;
bool gDisableDrag;
bool gHideButtons = FALSE;
bool gHideHeader = FALSE;
bool gShowTimer;
int gShowPetBuffs;
int gChatDisablePetSay;
int gChatEnablePetTeamSay;
int gHideDeletePrompt, gHidePlacePrompt, gHideDeleteSalvagePrompt, gHideDeleteRecipePrompt, gHideCoopPrompt;
int gHideSalvageFull, gHideRecipeFull, gHideEnhancementFull, gHideInspirationFull;
int gContactSort;
int gRecipeHideMissingParts, gRecipeHideUnowned, gRecipeHideMissingPartsBench, gRecipeHideUnownedBench, gStaticColorsPerName;
#ifndef TEST_CLIENT
MapState mapState = {0, 0, 0, 0, 0, 1.f};
#endif


void adjustArms(SeqInst * seq) {}

Destination serverDest = {0};
void ServerWaypointReceive(Packet* pak)
{
    int hasLocation = pktGetBits(pak, 1);
    int id = pktGetBitsPack(pak,1);
    int setCompass = false;
    if(id>0)
    {
        if (hasLocation)
        {
            serverDest.navOn = 1;
            serverDest.type = ICON_SERVER;
            serverDest.id = -id;
            setCompass = pktGetBits(pak, 1);
            serverDest.pulseBIcon = pktGetBits(pak, 1);
            if (pktGetBits(pak, 1))
                serverDest.icon = atlasLoadTexture(pktGetString(pak));
            if (pktGetBits(pak, 1))
                serverDest.iconB = atlasLoadTexture(pktGetString(pak));
            if (pktGetBits(pak, 1))
                strncpyt(serverDest.name, pktGetString(pak), ARRAY_SIZE(serverDest.name));
            if (pktGetBits(pak, 1)) {
                strncpyt(serverDest.mapName, pktGetString(pak), ARRAY_SIZE(serverDest.mapName));
                forwardSlashes(serverDest.mapName);
            }
            if (pktGetBits(pak, 1))
                serverDest.angle = pktGetF32(pak);

            serverDest.world_location[3][0] = pktGetF32(pak);
            serverDest.world_location[3][1] = pktGetF32(pak);
            serverDest.world_location[3][2] = pktGetF32(pak);

            // need to do this after the position has been updated
            if (setCompass || isDestination(&waypointDest, &serverDest))
                compass_SetDestination(&waypointDest, &serverDest);
        }
        else // we may have lost this location, make sure we don't have waypoint
        {
            clearDestination(&serverDest);
            compass_ClearAllMatchingId(-id);
        }
    }
    else
    {
        // don't think we actually need to do anything here?
    }
}

void setSavedDividerSettings(void){;}

char gSelectedHandle[64] = {0};

void imageserver_InitDummyCostume( Costume * costume ) {;}
int imageserver_ReadFromCSV( Costume ** pCostume, void *imageParams, char * csvFileName ) {return 0;}
int imageserver_WriteToCSV( Costume * pCostume, void *imageParams, char * csvFileName ) {return 0;}
int imageserver_ReadFromCSV_EX( Costume ** pCostume, void *imageParams, char * csvFileName, char **static_strings ) { return 0; }

void saveOnExit() {;}

void genderSetScalesFromApperance( Entity * e ) {;}
float doSlider( float lx, float ly, float lz, float wd, float xsc, float ysc, float value, int type, int color ) { return 0;}
void genderFaceScale(float x, float y, float uiscale, int costume, int part) {;}
char *rdrGetSystemSpecCSVString(void) { return "TestClient"; }
void rdrCheckThread(void) {;}
void modelFreeRgbBuffer(U8 *rgbs) {;}

void info_replaceTab( char * name, char * text) {;}
TrickInfo *trickFromName(const char *name,char *fullpath) { return NULL; }
void checkgl(char *fname,int line) {;}
void texWordsFlush(void) {;}
void arenaSelectTab( int tab ) {;}
void arenaUpdateParticipantUI(void) {;}
//void arenaRebuildResultView(ArenaRankingTable * ranking) {;}
void arenaUpdateUI(void) {;}
void arenaListSelectTab( int tab ) {;}
void requestCurrentKiosk() {}
void addArenaChatMsg( char * txt, int type ) {}
void pmotionReceivePhysicsRecording(Packet* pak, Entity* e) {;}
ToolTip * freeToolTip() {return NULL;}

bool inArenaLobby() {return true;}

typedef struct SalvageDictionary SalvageDictionary;
typedef struct SalvageItem SalvageItem;
//SHARED_MEMORY SalvageDictionary g_SalvageDict;
//void salvage_CreateHashes( SalvageDictionary *pdict ) {;}
//TokenizerParseInfo* salvage_GetParseInfo() {return NULL;}
//void salvage_Finalize( SalvageDictionary *pdict ) {;}
//void entReceiveUpdate(Packet *pak,int full_update) {;}
//void character_salvageinv_Send(Packet *pak, Character *p, int rarity, int idx ) {;}
//SalvageItem const* salvage_GetItemById( int id ) {return NULL;}
//SalvageItem const* salvage_GetItemByRarityId( SalvageRarity rarity, int rarityId ) { return NULL;}
//int salvage_ValidId( int id ) {return 0;}
//int salvage_Preprocess(TokenizerParseInfo pti[], SalvageDictionary *pdict ) { return 1; }
//int salvage_ValidRarity( int rarity ) {return 0;}
//SalvageItem const * salvage_GetItemByRarityId( SalvageRarity rarity, int rarityId ) {return NULL;}
//int salvage_Preprocess(TokenizerParseInfo pti[], SalvageDictionary *pdict ) {return 0;}
//void entity_ReceiveInvUpdate(Entity *e, Packet *pak) {;}
//void character_SetInventory(Character *p, InventoryType type, int id, int idx, int amount) {;}
//void character_GetInvIdAndAmount(int *id, int *amount, Character *p, InventoryType type, int idx) {;}
typedef struct ConceptItem ConceptItem;
//SHARED_MEMORY ConceptDictionary g_ConceptDict;
bool gDisablePetNames;
//void concept_CreateHashes( ConceptDictionary *pdict ) {;}
//TokenizerParseInfo * concept_GetParseInfo() {return NULL;}

void DebugLocationShutdown() {}
void DebugLocationReceive() {}

void calcSeqTypeName( SeqInst * seq, int alignment, int praetorian, int isPlayer ){}
void rdrNeedGammaReset(void){}

void mouseDiff( int * xp, int * yp ) {}

int vistrayIsVisible(DefTracker *tray_tracker) { return true; }
int zoTestSphere(Vec3 centerEyeCoords, float radius, int isZClipped) { return 0; }
void initAnimComboBox(int rebuild) {;}

void costume_MatchPart( Entity *e, const CostumePart *cpart, const CostumeOriginSet *cset, int supergroup ) {;}
void costume_ZeroStruct( CostumeOriginSet *cset ) {;}

void receiveSGRaidList(Packet *pak) {;}
void receiveSupergroupList(Packet *pak) {;}
void sgRaidListRefresh(void) {;}

void animCalcObjAndBoneUse( GfxNode * pNode, int seqHandle ) {;}
void costume_setCurrentCostume( Entity *e , int supergroup ){;}

void clientLoadMapStart(char *mapname) {;}
GroupFile *groupLoadFull(char *fname) {return 0;}
void groupReset() {;}
void windows_ResetLocations(void) {;}
void setNewOptions() {};
void leftArenaLobby() {};
bool entIsMyPet(Entity *e) {return 0;}
void windowClientSizeThisFrame(int *width,int *height) {;}
void seqUpdateGroupTrackers(SeqInst *seq, Entity *entParent) {;}

void mapGeneratorPacketHandler(Packet * pak) {;}

void cmdAddServerCommand(char * scmd) {}
char *encrypedKeyIssuedTo=NULL;
int encryptedKeyedAccessLevel() {return 0;}
void lfg_buildSearch(Packet *pak) {}
void baseSetLocalMode(int mode) {};

void sgeval_AddDefaultFuncs(EvalContext *pContext) {}

void dialogChooseNameImmediate(int first_time) {}
void dialogClearTextEntry() {}
char* dialogGetTextEntry() {return NULL;}

void updateSplatTextureAndColors(SplatParams* splatParams, F32 width, F32 height) {;}
void gfxReapplySettings(void){;}
int gLoggingIn;
void lightmapManagerLoadAllRequested(void){;}
void resetVillainHeroDependantStuff(int changeCostume){;}

void MissionControlHandleResponse() {;}
void lightmapManagerFlush(bool cancel_load, bool wait_until_finished) {;}

GroupDraw group_draw={0};

void promptQuit(char *reason) {;}
void unsetCustomDepthOfField(DOFSwitchType type, SunLight OUT *sun) {;}
void setCustomDepthOfField(DOFSwitchType type, F32 focusDistance, F32 focusValue, F32 nearDist, F32 nearValue, F32 farDist, F32 farValue, F32 duration, SunLight OUT *sun) {;}
void sunSetSkyFadeClient(int sky1, int sky2, F32 weight) {;}

int gBuffSettings;
void clearMarkedPowerBuff(){}
void addUpdatePowerBuff(){}
void markPowerBuff(){}
void freePowerBuff(){}

void setCritterCostume(int i){;}
void sgRaidListRefresh();
int windowUp(int i){return 0;}

int gCostumeCritter;

S32 game_runningCohClientCount(void){return 1;}
bool alSetFavoriteWeaponAnimListOnEntity( Entity * e, const char * animListName ){return 0;}
void alClientAssignAnimList( Entity * e, const char * newAnimList ){;} 
bool clientHasCaughtUpToRagdollStart( Ragdoll* ragdoll, U32 uiClientTime ){return 0;}
void positionCapsule( const Mat4 entMat, EntityCapsule * cap, Mat4 collMat ){return;}
void uiRaidResultReceive(Packet * pak) {};
void uiAuctionHouseItemInfoReceive(Packet *pak) {};
void uiAuctionHouseBatchItemInfoReceive(Packet *pak){};
void uiAuctionHouseListItemInfoReceive(Packet *pak){};
void uiAuctionHouseHistoryReceive(Packet *pak){};
int uiRaidResultDisplay() {return 0;}

void scriptUIReceiveDetach(Packet *pak) {pktGetBits(pak, 1);}

void demoFreeEntityRecordInfo(Entity* e) { return; }
int demoIsRecording() { return 0; }
void demoRecord(char const *fmt, ...) { return; }
void demoSetEntIdx(int ent_idx) { return; }
void demoSetAbsTime(unsigned int time) { return; }
void demoCheckPlay() { return; }
void demoStartPlay(char *fname) { return; }
void demoStartRecord(char *fname) { return; }
void demoStop() { return; }
void demoRecordAppearance(Entity *e) { return; }
void demoRecordStatChanges(Entity* e) { return; }
void demoRecordFx(NetFx *netfx) { return; }
void demoRecordMove(Entity *e,int move_idx,int move_bits) { return; }
void demoRecordPos(const Vec3 pos) { return; }
void demoRecordPyr(Vec3 pyr) { return; }
void demoRecordDelete() { return; }
void demoRecordCreate(char *name) { return; }
void demoSetAbsTimeOffset(int abs_time_offset) { return; }
U32 demoGetAbsTime() { return 0; }
void demoPlay() { return; }
void demoRecordCamera(Mat4 mat) { return; }
void demoRecordClientInfo() { return; }
int demoIsPlaying() { return 0; }
void manageEntityHitPointTriggeredfx( Entity * e ){return;}
void demoRecordDynGroups( void ){return;}
void demoRecordSkyFile(int sky1, int sky2, F32 weight){return;}
int fxIsCameraSpace(const char *fxname){return 0;}

int cfg_IsBetaShard()                            {    return 0;    }
int cfg_NagGoingRoguePurchase()                    {    return 0;    }
void cfg_setNagGoingRoguePurchase(int data)        {    return;        }
void cfg_setIsBetaShard(int data)                {    return;        }
void cfg_setIsVIPShard(int data)                {    return;        }

typedef struct UIEdit UIEdit;
UIEdit *sellItemEdit = NULL;

typedef enum GRNagContext GRNagContext;
int okToShowGoingRogueNag(GRNagContext ctx) { return 0; }
void dialogGoingRogueNag(GRNagContext ctx) { return; }

void hwlightSignalLevelUp() { return; }
void hwlightSignalMissionComplete() { return; }

void updateCertifications(int fromMap){ return; }
void AccountServer_ClientCommand(Cmd *cmd, int message_dbid, char *authname, int dbid, char *source_str) {}

int drawHybridBar(HybridElement * hb, int selected, F32 x, F32 cy, F32 z, F32 wd, F32 scale, int flags, F32 alpha, ScreenScaleOption sso, int ha, int va ){return 0;}
int drawHybridButton( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags ){return 0;}
int drawHybridNext( int dir, int locked, char * text ){return 0;}
void makeValidCharacter(){}
int drawHybridCommon(int menuType){return 0;}
void loadCostume_start(int foo){}
void saveCostume_start(int foo){}
void scaleAvatar(){}
void resetCreationMenus(){}
void genderChangeMenuStart(){}
void display_sprite_ex(AtlasTex *atex, BasicTexture *btex, float xp, float yp, float zp, float xscale, float yscale, int rgba, int rgba2, int rgba3, int rgba4, float u1, float v1, float u2, float v2, float angle, int additive, Clipper2D *clipper, ScreenScaleOption useScreenScale, HAlign ha, VAlign va ) {}
Clipper2D* clipperGetCurrent() {return 0;}
void calculatePositionScales(F32 * xposSc, F32 * yposSc) {*xposSc=1.f; *yposSc=1.f;}
int getScreenScaleOption() {return 0;}

void unlockCharacterRightAwayIfPossible(void){}

void receiveConvertEnhancement(Packet *pak){}

void uiSalvage_ReceiveSalvageImmediateUseResp( const char* salvageName, U32 flags /*SalvageImmediateUseStatus*/, const char* msgId ) {}
void setHeroVillainEventChannel(int isHero){};

void playerStartForcedFollow(Packet *pak) {}