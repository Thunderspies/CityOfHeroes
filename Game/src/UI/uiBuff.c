
#include "gameComm/wdwbase.h"

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiToolTip.h"
#include "UI/uiContextMenu.h"
#include "UI/uiInclude.h"
#include "UI/uiBuff.h"
#include "UI/uiGame.h"
#include "UI/uiPet.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "UI/uiInfo.h"
#include "UI/uiCombatNumbers.h"
#include "UI/uiOptions.h"
#include "entity/character_base.h"
#include "entity/classes.h"

#include "graphics/textureatlas.h"
#include "entity/powers.h"
#include "entity/entity.h"
#include "graphics/ttFontUtil.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include "player/player.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/attrib_description.h"
#include <utilitieslib/utils/file.h>
#include "cmdparse/cmdgame.h"

typedef struct PowerBuff
{
    const BasePower * ppow;
    AttribDescription **ppDesc;
    F32 *ppfMag;
    ToolTip tip;
    int iBlink;
    float timer;
    int uid;
    int delete_mark;
    int window;
}PowerBuff;

void markBuffForDeletion(Entity *e, int uid)
{
    int i;

    for( i=eaSize(&e->ppowBuffs)-1; i>=0; i-- )
    {
        if( e->ppowBuffs[i]->uid == uid )
        {
            e->ppowBuffs[i]->delete_mark = 1;    
            break;
        }
    }
}

PowerBuff * findPowerBuff(Entity *e, int uid, const BasePower *ppow)
{
    int i;

    if (!e)
        return NULL;

    if( uid ) // if there is uid look for match first
    {
        for( i=eaSize(&e->ppowBuffs)-1; i>=0; i-- )
        {
            if( e->ppowBuffs[i]->uid == uid )
            {
                return e->ppowBuffs[i];
            }
        }
    }
    else
    {
        for( i=eaSize(&e->ppowBuffs)-1; i>=0; i-- )
        {
            if( e->ppowBuffs[i]->ppow == ppow )
            {
                return e->ppowBuffs[i];
            }
        }
    }


    return NULL;
}

ContextMenu *gPowerBuffcm = 0;
ContextMenu *gPowerBuffsub = 0;

// leaves a little room to expand both ways
#define BUFF_WINDOW_SHIFT 8

int gPowerBuffContextWindow;

void setBuffSetting(  BuffWindow window, BuffSettings setting )
{
    int tmp = optionGet(kUO_BuffSettings);
    int on = tmp&(1<<(setting+window*BUFF_WINDOW_SHIFT));

    if( !on )
        tmp |= (1<<(setting+window*BUFF_WINDOW_SHIFT));
    else
        tmp &= ~(1<<(setting+window*BUFF_WINDOW_SHIFT));

    if( setting == kBuff_NoStack && !on ) // turn off numeric stack if no stack is turned on
        tmp &= ~(1<<(kBuff_NumStack+window*BUFF_WINDOW_SHIFT));

    if( setting == kBuff_NumStack && !on ) // and vice-versa
        tmp &= ~(1<<(kBuff_NoStack+window*BUFF_WINDOW_SHIFT));

    optionSet(kUO_BuffSettings, tmp,1);
}

int isBuffSetting( BuffWindow window, BuffSettings setting )
{
    int buff = optionGet(kUO_BuffSettings);
     return !!(buff&(1<<(setting+window*BUFF_WINDOW_SHIFT))); // The not-not is to make this a bool
                                                             // casting to bool only looks at first byte
}

const BasePower** basePowersFromPowerBuffs(void ** buff)
{
    static const BasePower** basePowers = 0;
    PowerBuff** powerBuffs = (PowerBuff**)buff;
    int i, n = eaSize(&powerBuffs);

    if (!basePowers)
        eaCreateConst(&basePowers);
    eaSetSizeConst(&basePowers, n);

    for (i = 0; i < n; i++)
        basePowers[i] = powerBuffs[i]->ppow;
    return basePowers;
}

static char * getModString( AttribDescription *pDesc, F32 fVal )
{
    static char tmp[128];
    Entity * e = playerPtr();
    tmp[0] = '\0';
    strcat( tmp, textStd(pDesc->pchDisplayName)); 
     strcat( tmp, " " );

    if( pDesc->offAttrib == offsetof(CharacterAttributes,fSpeedRunning) || 
        pDesc->offAttrib == offsetof(CharacterAttributes,fSpeedFlying) ||
        pDesc->offAttrib == offsetof(CharacterAttributes,fSpeedJumping) )
    {
        F32 *pf = ATTRIB_GET_PTR(e->pchar->pclass->ppattrBase[0],pDesc->offAttrib);
        fVal -= *pf;
    }
    strcat( tmp, attribDesc_GetModString(pDesc, fVal, e->pchar) );
    return tmp;
}

static const char* buffcm_PowerText( BasePower * data ){ if(!data) return ""; return data->pchDisplayName;}
static const char* buffcm_PowerInfo( BasePower * pow  )
{
    if(pow->pchDisplayTargetShortHelp && *pow->pchDisplayTargetShortHelp && pow->eType != kPowerType_Toggle)
        return pow->pchDisplayTargetShortHelp;
    else
        return pow->pchDisplayShortHelp;
}

static int s_ContextBuffID = 0;
static const BasePower *s_ContextBuffPower = 0;
static Entity *s_ContextBuffEnt = 0;

static char *buffcm_PowerNumbersInfo( int idx )
{
    PowerBuff * buff = findPowerBuff(s_ContextBuffEnt, s_ContextBuffID, s_ContextBuffPower);
    
    idx--; //had to fool null check

    if(!buff || idx < 0 || idx > eaSize(&buff->ppDesc)-1 ) 
        return NULL;
    else
        return getModString( buff->ppDesc[idx], buff->ppfMag[idx] );
}

static int buffcm_PowerNumbersVisible( int idx )
{
    PowerBuff * buff = findPowerBuff(s_ContextBuffEnt, s_ContextBuffID, s_ContextBuffPower);

    idx--; //had to fool null check

    if(!buff || idx < 0 || idx > eaSize(&buff->ppDesc)-1 ) 
        return CM_HIDE;
    else
        return CM_VISIBLE;
}


static void buffcm_Info( BasePower * data ){ if(!data)return; info_Power( data, data->eType!= kPowerType_Toggle ); }

static void buffcm_Cancel( BasePower * data ){ 
    char temp[200];
    if(!data || !s_ContextBuffEnt)return; 

    sprintf(temp, "powers_cancel %i %s", s_ContextBuffEnt->svr_idx, data->pchFullName);
    cmdParse(temp);
}

static void buffcm_Auto( void * data)   { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_HideAuto ); }
static void buffcm_Toggle( void * data) { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_HideToggle ); }
static void buffcm_Blink( void * data)  { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_NoBlink ); }
static void buffcm_NoNum( void * data)  { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_NoNumbers ); }
static void buffcm_NoSend( void * data)  { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_NoSend ); }


static void buffcm_NoStack( void * data)  { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_NoStack ); }
static void buffcm_NumStack( void * data) { if(!data)return; setBuffSetting( gPowerBuffContextWindow, kBuff_NumStack ); }
static void buffcm_Stack( void * data)
{ 
    if(!data)
        return;
    if( isBuffSetting(gPowerBuffContextWindow,kBuff_NoStack) )
        setBuffSetting( gPowerBuffContextWindow, kBuff_NoStack ); 
    if( isBuffSetting(gPowerBuffContextWindow,kBuff_NumStack) )
        setBuffSetting( gPowerBuffContextWindow, kBuff_NumStack ); 
}

static int buffcm_CancelCheck( BasePower * data)
{
    Entity *e = playerPtr();
    
    if(!data || !s_ContextBuffEnt) return CM_HIDE;
    if(e && e->access_level >= ACCESS_DEBUG) return CM_AVAILABLE;
    
    if(e != s_ContextBuffEnt && !entIsMyPet(s_ContextBuffEnt)) return CM_HIDE;

    if(!data->bCancelable) return CM_HIDE;
    
    return CM_AVAILABLE;
}

static int buffcm_AutoCheck( void * data)     { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_HideAuto)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_ToggleCheck( void * data)     { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_HideToggle)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_NoNumCheck( void * data)     { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_NoNumbers)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_NoSendCheck( void * data)     { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_NoSend)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_BlinkCheck( void * data)     { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_NoBlink)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_NoStackCheck( void * data) { if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_NoStack)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_NumStackCheck( void * data){ if(!data)return CM_VISIBLE; return isBuffSetting(gPowerBuffContextWindow,kBuff_NumStack)?CM_CHECKED:CM_AVAILABLE; }
static int buffcm_StackCheck( void * data)
{ 
    if(!data)
        return CM_VISIBLE;
    if( !isBuffSetting(gPowerBuffContextWindow,kBuff_NoStack) && 
        !isBuffSetting(gPowerBuffContextWindow,kBuff_NumStack) )
    {
        return CM_CHECKED;
    }
    else
        return CM_AVAILABLE;
}

static const char * buffcm_PowerTextAdapter(void* arg0)
{
    return buffcm_PowerText((BasePower *)arg0);
}

static const char * buffcm_PowerInfoAdapter(void* arg0)
{
    return buffcm_PowerInfo((BasePower *)arg0);
}

static int buffcm_PowerNumbersVisibleAdapter(void* arg0)
{
    return buffcm_PowerNumbersVisible((int)(intptr_t)arg0);
}

static const char * buffcm_PowerNumbersInfoAdapter(void* arg0)
{
    return buffcm_PowerNumbersInfo((int)(intptr_t)arg0);
}

static void buffcm_InfoAdapter(void* arg0)
{
    buffcm_Info((BasePower *)arg0);
}

static int buffcm_CancelCheckAdapter(void* arg0)
{
    return buffcm_CancelCheck((BasePower *)arg0);
}

static void buffcm_CancelAdapter(void* arg0)
{
    buffcm_Cancel((BasePower *)arg0);
}

static void initPowerBuffContext(void)
{
    int i;
    gPowerBuffcm = contextMenu_Create( NULL );
    gPowerBuffsub = contextMenu_Create( NULL );
    contextMenu_addVariableTitle( gPowerBuffcm, buffcm_PowerTextAdapter, 0);
    contextMenu_addVariableText( gPowerBuffcm, buffcm_PowerInfoAdapter, 0);

    for( i=1; i <= 31; i++ )
        contextMenu_addVariableTitleVisibleNoTrans( gPowerBuffcm, buffcm_PowerNumbersVisibleAdapter, (void*)i, buffcm_PowerNumbersInfoAdapter, (void*)i );

    contextMenu_addCode( gPowerBuffcm, alwaysAvailable, 0, buffcm_InfoAdapter, 0, "CMInfoString", 0  );
    contextMenu_addDividerVisible( gPowerBuffcm, buffcm_CancelCheckAdapter);
    contextMenu_addCode( gPowerBuffcm, buffcm_CancelCheckAdapter, 0, buffcm_CancelAdapter, 0, "CMCancelString", 0  );
    contextMenu_addDivider( gPowerBuffcm );
    contextMenu_addCheckBox( gPowerBuffcm, buffcm_AutoCheck, 0, buffcm_Auto, 0, "CMAuto" );
    contextMenu_addCheckBox( gPowerBuffcm, buffcm_BlinkCheck, 0, buffcm_Blink, 0, "CMBlink" );
    contextMenu_addCode( gPowerBuffcm, alwaysAvailable, 0, NULL, 0, "CMStacking", gPowerBuffsub );

    contextMenu_addCheckBox( gPowerBuffsub, buffcm_NoStackCheck, 0, buffcm_NoStack, 0, "CMNoStack" );
    contextMenu_addCheckBox( gPowerBuffsub, buffcm_StackCheck, 0, buffcm_Stack, 0, "CMStack" );
    contextMenu_addCheckBox( gPowerBuffsub, buffcm_NumStackCheck, 0, buffcm_NumStack, 0, "CMNumStack" );
    contextMenu_addDivider( gPowerBuffcm );
    contextMenu_addCheckBox( gPowerBuffcm, buffcm_NoNumCheck, 0, buffcm_NoNum, 0, "CMNoNumbers" );
    contextMenu_addCheckBox( gPowerBuffcm, buffcm_NoSendCheck, 0, buffcm_NoSend, 0, "CMNoSending" );
}


void addUpdatePowerBuff(Entity * e, const BasePower *ppow, int iBlink, int uid, AttribDescription **ppDesc, F32 *ppfMag )
{
    PowerBuff * buff=0;
    int push = 1;

    if(!playerPtr())
        return;

    buff = findPowerBuff( e, uid, ppow );
    if( buff ) // if there is uid look for match first
    {
        push = 0;
        buff->delete_mark = 0;
    }

    if( !buff )
        buff = calloc(1,sizeof(PowerBuff));

    buff->ppow = ppow;
    buff->iBlink = iBlink;
    buff->uid = uid;

    if(ppDesc)
    {
        eaDestroy(&buff->ppDesc);
        eaCopy( &buff->ppDesc, &ppDesc); 
    }
    if(ppfMag)
    {
        eafDestroy(&buff->ppfMag);
        eafCopy( &buff->ppfMag, &ppfMag);
    }

    if( push )
        eaPush(&e->ppowBuffs, buff);
}

void freePowerBuff( void * buff )
{
    if( buff )
    {
        freeToolTip( &((PowerBuff*)buff)->tip );
        memset(buff, 0, sizeof(PowerBuff));
        eaDestroy(&((PowerBuff*)buff)->ppDesc);
        eafDestroy(&((PowerBuff*)buff)->ppfMag);
        free( ((PowerBuff*)buff) );
        buff = 0;
    }
}

void markPowerBuff(Entity *e)
{
    int i;
    if(!e)
        return;
    for( i=eaSize(&e->ppowBuffs)-1; i>=0; i-- )
        e->ppowBuffs[i]->delete_mark = true;
}

void clearMarkedPowerBuff(Entity *e)
{
    int i;
    if(!e)
        return;
    for( i=eaSize(&e->ppowBuffs)-1; i >= 0; i-- )
    {
        if( e->ppowBuffs[i]->delete_mark )
        {
            freePowerBuff(eaRemove(&e->ppowBuffs, i ));
        }
    }
}



void displayBuffIcon( Entity *e, PowerBuff * buff, float x, float y, float z, float sc, ToolTipParent *parent, int wdw, int count, int blink )
{
    AtlasTex *ptex = atlasLoadTexture(buff->ppow->pchIconName);
    char achShort[1024];
    CBox box;
    int color = 0xffffff88;

    //    if iBlink & 2 ... it means that we need to account for duration so don't default to color = 1/2 white
    if( buff->iBlink )
    {
        //    if iBlink & 1 .. need to blink now
        if ( (buff->iBlink & 1) && blink )
        {
            // blink
            float alpha;
            buff->timer += TIMESTEP*.3f;

            alpha = (cosf(buff->timer)+1)/2;
            color = 0xffffff00 | (int)(255*alpha);
        }
        else
        {
            color = CLR_WHITE;
        }
    }
    else if( buff->ppow->eType == kPowerType_Auto || buff->ppow->eType == kPowerType_Toggle  )
    {
        color = CLR_WHITE; 
    }

    display_sprite(ptex, x, y, z, sc, sc, color);

    if( count > 1 )
    {    
        font_color( 0xffffff88, 0xffffff88 );
        cprntEx( x + ptex->width*sc/2, y + ptex->height*sc/2 + 1*sc,z, 2*sc,  2*sc, CENTER_X|CENTER_Y, "%i", count );
    }

    BuildCBox( &box, x, y, ptex->width*sc, ptex->height*sc);
    strcpy(achShort, buff->ppow->pchDisplayName);
    setToolTip(&buff->tip, &box, achShort, parent, MENU_GAME, wdw);
     addToolTip(&buff->tip);

    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        int xp, yp;
        rightClickCoords( &xp, &yp );

        if( wdw == WDW_STAT_BARS )
            buff->window = kBuff_Status;
        else if( ( wdw == WDW_GROUP ) || ( wdw == WDW_LEAGUE ) )
            buff->window = kBuff_Group;
        else if( wdw == WDW_PET )
            buff->window = kBuff_Pet;

        if( !gPowerBuffcm )
            initPowerBuffContext();

        gPowerBuffContextWindow = buff->window;
        s_ContextBuffID = buff->uid;
        s_ContextBuffPower = buff->ppow;
        s_ContextBuffEnt = e;
        contextMenu_set( gPowerBuffcm, xp, yp, (void*)buff->ppow );
    }
}


#define ICON_SIZE    32.f
//
//
float status_drawBuffs( Entity *e, float x, float y, float z, float wd, float ht, float scale, int flip, ToolTipParent *parent )
{
    int i, iExtraHeight = 0, blink = !(isBuffSetting( kBuff_Status, kBuff_NoBlink ));
    float tx = x+wd-30*scale, ty = flip?y-20*scale:y+ht, sc = .7f;

    for(i=0; i<eaSize(&e->ppowBuffs) && i<MAX_BUFF_ICONS; i++)
    {
        int j, stack_count = 0, skip = 0, offset = 0;
        const BasePower * buff = e->ppowBuffs[i]->ppow;
        const BasePowerSet * buff_set = e->ppowBuffs[i]->ppow->psetParent;

        if( isBuffSetting( kBuff_Status, kBuff_HideAuto ) && buff->eType == kPowerType_Auto )
            continue;

        if( isBuffSetting( kBuff_Status, kBuff_HideToggle ) && buff->eType == kPowerType_Toggle )
            continue;

        // see if we covered this icon already
        if( !isBuffSetting( kBuff_Status, kBuff_NoStack ) )
        {
            for(j=0; j<i && j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * earlier_buff = e->ppowBuffs[j]->ppow;
                const BasePowerSet * earlier_set = e->ppowBuffs[j]->ppow->psetParent;
                if( stricmp( buff->pchName, earlier_buff->pchName ) == 0 &&
                    stricmp( buff_set->pchName, earlier_set->pchName ) == 0 )
                {
                    skip = true;
                    break;
                }
            }

            if( skip )         // skip this one, it was covered earlier
                continue;     // each powers will search for itself

            // now count up the new icons so we know if we need to wrap
            for(j=i; j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * later_buff = e->ppowBuffs[j]->ppow;
                const BasePowerSet * later_set = e->ppowBuffs[j]->ppow->psetParent;
                if( stricmp( buff->pchName, later_buff->pchName ) == 0 && 
                    stricmp( buff_set->pchName, later_set->pchName ) == 0 )
                    stack_count++;
            }
        }

        if( !isBuffSetting( kBuff_Status, kBuff_NumStack ) )
            offset = 7*scale*stack_count;

        // wrap if nessecary
        if( tx - (ICON_SIZE*sc*scale) - offset < x )
        {
            tx = x+wd-30*scale;
            if(flip)
                ty -= (ICON_SIZE*sc*scale);
            else
                ty += (ICON_SIZE*sc*scale);

            iExtraHeight += (ICON_SIZE*sc*scale);
        }

        // now display
        if( isBuffSetting( kBuff_Status, kBuff_NoStack) )
            displayBuffIcon( e, e->ppowBuffs[i], tx, ty, z, sc*scale, parent, WDW_STAT_BARS, 0, blink );
        else if( isBuffSetting( kBuff_Status, kBuff_NumStack) )
            displayBuffIcon( e, e->ppowBuffs[i], tx, ty, z, sc*scale, parent, WDW_STAT_BARS, stack_count, blink );
        else
        {
            for(j=i; j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * later_buff = e->ppowBuffs[j]->ppow;
                const BasePowerSet * later_set = e->ppowBuffs[j]->ppow->psetParent;

                if( stricmp( buff->pchName, later_buff->pchName ) == 0 && 
                    stricmp( buff_set->pchName, later_set->pchName ) == 0 )
                {
                    if( i != j )
                        tx -= 7*scale;
                    displayBuffIcon( e, e->ppowBuffs[j], tx, ty, z, sc*scale, parent, WDW_STAT_BARS, 0, blink );
                }
            }
        }

        tx -= (ICON_SIZE*sc*scale);
    }

    iExtraHeight += (ICON_SIZE*sc*scale); // add at least one row

    if( flip )
        BuildCBox( &parent->box, x, y-iExtraHeight, wd, ht+iExtraHeight );
    else
        BuildCBox( &parent->box, x, y, wd, ht+iExtraHeight );

    return ty + (ICON_SIZE*sc*scale)*(flip?-1:1);
}

void linedrawBuffs( BuffWindow window, Entity *e, float x, float y, float z, float wd, float ht, float scale, int flip, ToolTipParent *parent )
{
    int i,j, iWidth = 0, blink = !(isBuffSetting( window, kBuff_NoBlink ));
    float sc = 0.50f*scale;
    int wdw = (window==kBuff_Group)?WDW_GROUP:WDW_PET;

    for(i=0; i<eaSize(&e->ppowBuffs) && i<50; i++)
    {
         const BasePower * buff = e->ppowBuffs[i]->ppow;
        const BasePowerSet * buff_set = e->ppowBuffs[i]->ppow->psetParent;

        float xorg;
        int skip=0, stack_count = 0, icon_wd = 32.f;

        if( isBuffSetting( window, kBuff_HideAuto ) && buff->eType == kPowerType_Auto )
            continue;

        if( isBuffSetting( window, kBuff_HideToggle ) && buff->eType == kPowerType_Toggle )
            continue;

        // see if we covered this icon already
        if( !isBuffSetting( window, kBuff_NoStack ) )
        {
            for(j=0; j<i && j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * earlier_buff = e->ppowBuffs[j]->ppow;
                const BasePowerSet * earlier_set = e->ppowBuffs[j]->ppow->psetParent;
                if( stricmp( buff->pchName, earlier_buff->pchName ) == 0 &&
                    stricmp( buff_set->pchName, earlier_set->pchName ) == 0 )
                {
                    skip = true;
                    break;
                }
            }

            if( skip )         // skip this one, it was covered earlier
                continue;     // each powers will search for itself

            // now count up the new icons so we know if we need to wrap
            for(j=i; j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * later_buff = e->ppowBuffs[j]->ppow;
                const BasePowerSet * later_set = e->ppowBuffs[j]->ppow->psetParent;
                if( stricmp( buff->pchName, later_buff->pchName ) == 0 &&
                    stricmp( buff_set->pchName, later_set->pchName ) == 0 )
                    stack_count++;
            }
        }

        if( flip )
            xorg = x-PIX3*scale-iWidth-icon_wd*sc+2*scale-15*scale;
        else
            xorg = x+PIX3*scale+wd+iWidth;

        if( isBuffSetting( window, kBuff_NoStack) )
            displayBuffIcon( e, e->ppowBuffs[i], xorg, y+2*scale, z, sc, parent, wdw, 0, blink );
        else if( isBuffSetting( window, kBuff_NumStack) )
            displayBuffIcon( e, e->ppowBuffs[i], xorg, y+2*scale, z, sc, parent, wdw, stack_count, blink );
        else
        {
            for(j=i; j<eaSize(&e->ppowBuffs) && j<MAX_BUFF_ICONS; j++)
            {
                const BasePower * later_buff = e->ppowBuffs[j]->ppow;
                if( stricmp( buff->pchName, later_buff->pchName ) == 0 )
                {
                    if( i != j )
                    {
                        iWidth += 7*sc;
                        if( flip )
                            xorg -= 7*sc;
                        else
                            xorg += 7*sc;
                    }
                    displayBuffIcon( e, e->ppowBuffs[j], xorg, y+2*scale, z, sc, parent, wdw, 0, blink );
                }
            }
        }

        iWidth += icon_wd*sc+2*scale;
    }
}
