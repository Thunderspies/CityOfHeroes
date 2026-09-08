
#include <utilitieslib/stdtypes.h>
#include "entity/character_inventory.h"
#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/utils.h>

#include "UI/uiNet.h"
#include "UI/uiInfo.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/uiWindows.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "uiCombineSpec.h"
#include "UI/uiTabControl.h"
#include "UI/uiBaseRoom.h"
#include "UI/uiBaseProps.h"
#include "UI/uiBaseInventory.h"
#include "UI/uiEnhancement.h"
#include "uiSalvage.h"
#include "UI/uiPowerInfo.h"
#include "UI/uiContact.h"

#include "bases/DetailRecipe.h"
#include "UI/uiRecipeInventory.h"

#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include "player/player.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "win/win_init.h"
#include "entity/attrib_names.h"
#include "entity/character_base.h"
#include "entity/entVarUpdate.h"
#include "graphics/seqgraphics.h"
#include "language/langClientUtil.h"
#include "language/commonLangUtil.h"
#include "entity/character_level.h"
#include "player/badges.h"
#include <utilitieslib/components/bitfield.h>

#include "formatter/smf_main.h"
#define SMFVIEW_PRIVATE
#include "uiSMFView.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdcommon.h"
#include "seq/seq.h"
#include <utilitieslib/utils/mathutil.h>
#include "bases/basedata.h"
#include "bases/baseedit.h"
#include "bases/bases.h"
#include "graphics/groupThumbnail.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/contactClient.h"
#include "auth/authUserData.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/StashTable.h>
#include "clientcomm/clientcomm.h"

#define HEADER_HT   75
#define MAX_TABS 5

typedef struct InfoButton InfoButton;
typedef void (*InfoButtonCb)(InfoButton *);

typedef struct InfoButton
{
    // offsets relative to parent.
    // NOTE: currently anchored to bottom left, refactor if you'd like.
    char *text;
    InfoButtonCb cb;
    void *context;
    int tab; // -1 for all
} InfoButton;

enum 
{
    NOTRANS_TITLE = 1,
    NOTRANS_SHORT = 2,
    NOTRANS_CLASS = 4,
};


typedef struct infoData
{
    float           x;
    float           y;

    AtlasTex        *icon;
    const BasePower    *spec;
    Power            *pow;
    int                pcharPowIdxs[2];
    const DetailRecipe *pRec;

    char              *name;
    char              *shortInfo;
    char              *classSet;
    StuffBuff         stuffbuff;

    int             inventory;
    int             level;
    int             numCombines;
    int             headshot;
    int                notranslate;

    Entity        *ent;
    EntityRef    eRef;

    int            badgeId;
    U32            aiBadges[BADGE_ENT_BITFIELD_SIZE];

    const SalvageItem **ppSalvageDrops;
    int *ppSalvageUseless;
    int *ppSalvageUseful;
    const DetailRecipe **ppSalvageUsefulTo;
    int *ppSalvageUsefulToIndices;

    int             numTabs;
    const RoomTemplate *room;
    const BasePlot     *plot;    

    uiTabControl *tc;

    SMFView     *pview;
    SMFBlock    *shortInfoSMF;
    SMFBlock    *classSetSMF;
    int            reparse;        // do the smfs need to be reparsed?
    float        header_ht;        // because we wrap text in header now, the header ht is variable

    InfoButton **buttons;
    void *context; // for any data you need to assoc with info
} infoData;

static infoData info;

#define INFO_WD 320

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

static TextAttribs s_taTitle =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_PARAGON,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_PARAGON,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)CLR_PARAGON,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)CLR_PARAGON,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taShortLines =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xaadeffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xaadeffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0xaadeffff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0xaadeffff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taClassSet =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x99cdeff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x99cdeff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x99cdeff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x99cdeff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static SMFBlock *s_curAlignmentName = NULL;
static SMFBlock *s_topAlignmentName = NULL;
static SMFBlock *s_midAlignmentName = NULL;
static SMFBlock *s_botAlignmentName = NULL;

// ------------------------------------------------------------
// info buttons

MP_DEFINE(InfoButton);
static InfoButton* infobutton_Create( char *str, InfoButtonCb cb, void *context, int tab )
{
    InfoButton *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(InfoButton, 64);
    res = MP_ALLOC( InfoButton );
    if( verify( res ))
    {
        res->text = str;
        res->cb = cb;
        res->context = context;
        res->tab = tab;
    }
    // --------------------
    // finally, return

    return res;
}

static void infobutton_Destroy( InfoButton *item )
{
    MP_FREE(InfoButton, item);
}



//
//
static InitInfo(void)
{
    int i;
    
    info_clear();

    if (!info.shortInfoSMF)
    {
        info.shortInfoSMF = smfBlock_Create();
    }

    if (!info.classSetSMF)
    {
        info.classSetSMF = smfBlock_Create();
    }

    if(!info.pview)
    {
        info.pview = smfview_Create(0);
        smfview_SetAttribs(info.pview, &s_taDefaults);
    }

    for(i = 0; i < MAX_TABS; i++)
    {
        if(!info.stuffbuff.buff)
            initStuffBuff(&info.stuffbuff, 128);
    }
    
}

//
//
void info_clear()
{
    int i;
    
    SAFE_FREE( info.name );
    SAFE_FREE( info.shortInfo );
    SAFE_FREE( info.classSet );
    if( info.pview)
    {
        smfview_Destroy(info.pview);
        info.pview = NULL;
    }
    if (info.stuffbuff.buff)
        freeStuffBuff(&info.stuffbuff);

    smfBlock_Destroy(info.shortInfoSMF);
    smfBlock_Destroy(info.classSetSMF);
    uiTabControlDestroy(info.tc);
    info.tc = 0;
    info.numTabs = 0;
    info.room = NULL;
    info.plot = NULL;
    info.pow = NULL;
    info.spec = NULL;
    info.pcharPowIdxs[0] = info.pcharPowIdxs[1] = -1;

    eaDestroyConst(&info.ppSalvageDrops);
    eaiDestroy(&info.ppSalvageUseless);
    eaiDestroy(&info.ppSalvageUseful);
    eaDestroyConst(&info.ppSalvageUsefulTo);
    eaiDestroy(&info.ppSalvageUsefulToIndices);

    // --------------------
    // buttons

    for( i = 0; i < eaSize( &info.buttons ); ++i ) 
    {
        infobutton_Destroy(info.buttons[i]);
    }
    eaDestroy(&info.buttons);

    // --------------------
    // finally
    
    ZeroStruct( &info );
}

//
//
static void info_setFields( AtlasTex * icon, const char * name, const char * shortInfo, const char * classSet, int headshot )
{
    info.icon = icon;

    // name
    SAFE_FREE( info.name );

    info.name = strdup( name );

    // first line
    SAFE_FREE( info.shortInfo );

    if (info.notranslate & NOTRANS_SHORT)
    {
        int len = strlen(shortInfo) + 1;
        info.shortInfo = malloc(sizeof(*info.shortInfo) * len);
        strcpy_s(info.shortInfo, len, shortInfo);
    }
    else
    {
        int len = msPrintf( menuMessages, NULL, 0, shortInfo) + 1;
        info.shortInfo = malloc( sizeof(*info.shortInfo) * len );
        msPrintf( menuMessages, info.shortInfo, len, shortInfo );
    }

    // second line
    SAFE_FREE( info.classSet );

    if (info.notranslate & NOTRANS_CLASS)
    {
        int len = strlen(classSet) + 1;
        info.classSet = malloc( sizeof(*info.classSet) * len);
        strcpy_s(info.classSet, len, classSet);
    }
    else
    {    
        int len = msPrintf( menuMessages, NULL, 0, classSet) + 1;
        
        info.classSet = malloc(sizeof(*info.classSet) * len);
        
        msPrintf(menuMessages, info.classSet, len, classSet);
    }

    // these will specifically be recreated
    uiTabControlDestroy(info.tc);
    info.tc = 0;
    info.numTabs = 0;
    info.reparse = true;
    info.header_ht = HEADER_HT;
}

//
//
void info_addBadges( Packet * pak )
{
    int i;
    memset(&info.aiBadges, 0, sizeof(U32)*BADGE_ENT_BITFIELD_SIZE);

    // badge array
    for(i = 0; i < BADGE_ENT_BITFIELD_SIZE; i++)
        info.aiBadges[i] = pktGetBits(pak, 32);

    if(pktGetBits(pak,1)) // selected badge
        pktGetBits(pak, 32); // eat data for now, may use later
}

static int info_addSalvageDrops_cmp(const void* aData, const void* bData)
{
    const SalvageItem ** a = (const SalvageItem **)aData;
    const SalvageItem ** b = (const SalvageItem **)bData;
    if((*a)->rarity != (*b)->rarity)
        return (*a)->rarity - (*b)->rarity;
    else
        return strcmp(textStd((*a)->ui.pchDisplayName),textStd((*b)->ui.pchDisplayName));
}

void info_addSalvageDrops(Packet *pak)
{
    int index, count = pktGetBitsPack(pak, 5); // probably not more than 30 for a single critter
    int index2, count2;
    const DetailRecipe **useful_to;

    eaSetSizeConst(&info.ppSalvageDrops, 0);
    for (index = 0; index < count; ++index)
    {
        SalvageItem const *salvage = salvage_GetItemById(pktGetBitsPack(pak, 9));
        if (salvage)
            eaPushConst(&info.ppSalvageDrops, salvage);  // last I checked, there's 288 salvage
        else
            Errorf("Could not find salvage by id for SalvageDrops info");
    }
    eaQSortConst(info.ppSalvageDrops, info_addSalvageDrops_cmp);

    eaiSetSize(&info.ppSalvageUseless, 0);
    eaiSetSize(&info.ppSalvageUseful, 0);
    eaSetSizeConst(&info.ppSalvageUsefulTo, 0);
    eaiSetSize(&info.ppSalvageUsefulToIndices, 0);
    eaiPush(&info.ppSalvageUsefulToIndices, 0);
    eaCreateConst(&useful_to);

    count = eaSize(&info.ppSalvageDrops);
    for (index = 0; index < count; ++index)
    {
        const SalvageItem *salvage = info.ppSalvageDrops[index];
        if (salvage)
        {
            eaSetSizeConst(&useful_to, 0);

            if (!character_IsSalvageUseful(playerPtr()->pchar, salvage, &useful_to))
            {
                eaiPush(&info.ppSalvageUseless, index);
            }
            else
            {
                eaiPush(&info.ppSalvageUseful, index);

                count2 = eaSize(&useful_to);
                for (index2 = 0; index2 < count2; index2++)
                {
                    eaPushConst(&info.ppSalvageUsefulTo, useful_to[index2]);
                }

                eaiPush(&info.ppSalvageUsefulToIndices, eaSize(&info.ppSalvageUsefulTo));
            }
        }
    }

    eaDestroyConst(&useful_to);
}

char * HTMLizeEntersAndTabs( const char * text )
{
    char * tmp, *ret;
    tmp = strdup(strchrInsert( text, "<br>", '\n' ));
    ret = strchrInsert( tmp, "&nbsp;&nbsp;&nbsp;&nbsp;", '\t' );
    free(tmp);
    return ret;
}

//
//
void info_replaceTab( const char * name, const char * text, int selected)
{
    int select;
     if( !info.tc )
        info.tc = uiTabControlCreate(TabType_Undraggable, NULL, NULL, uiTabDestroyFreeData, NULL, NULL);

    select = uiTabControlRemoveByName(info.tc,name);
    if( selected < 0 )
        selected = select;

    info_addTab(name,text,selected);
}

void info_addTab( const char * name, const char * text, int selected )
{
    if( !info.tc )
        info.tc = uiTabControlCreate(TabType_Undraggable, NULL, NULL, uiTabDestroyFreeData, NULL, NULL);

    uiTabControlAddCopy( info.tc, name, HTMLizeEntersAndTabs( text ) );

    if( selected )
        uiTabControlSelectByName( info.tc, name );

    info.numTabs++;
}

void info_noResponse(void)
{
    clearStuffBuff(&info.stuffbuff);
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("InfoDataUnavailable") );
    smfview_Reparse(info.pview);
}

typedef enum InfoButtonSalvageRE
{
    kInfoButtonSalvageRE_One,
    kInfoButtonSalvageRE_All,
    kInfoButtonSalvageRE_Count
} InfoButtonSalvageRE;


static void infobutton_SalvageCb(InfoButton *b)
{
    InfoButtonSalvageRE type = kInfoButtonSalvageRE_Count;
    SalvageInventoryItem *inv = (SalvageInventoryItem*)info.context;
    int i;
    
    if( verify( b && inv && inv->salvage && inv->amount > 0 && playerPtr() ))
    {
        int numToRe = -1;
        Character *pchar = playerPtr()->pchar;
        int invId = character_FindSalvageInvIdx( pchar, inv );

        if( verify( invId >= 0 ) )
        {
            type = (InfoButtonSalvageRE)b->context;
            switch ( type )
            {
            case kInfoButtonSalvageRE_One:
                {
                    numToRe = 1;
                }
                break;
            case kInfoButtonSalvageRE_All:
                {
                    numToRe = inv->amount;
                }
                break;
            default:
                assertmsg(0,"invalid.");
            };

            for( i = 0; i < numToRe; ++i ) 
            {
                crafting_reverseengineer_send( invId );
            }

            // if out of inventory, disable buttons
            if( inv->amount - i <= 0 )
            {
                int i;
                for( i = 0; i < eaSize( &info.buttons ); ++i ) 
                {
                    infobutton_Destroy(info.buttons[i]);
                }
                eaDestroy(&info.buttons);
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------- RECIPES

void info_Recipes( const DetailRecipe *pRec )
{
    AtlasTex * icon = NULL;
    DetailRecipe **useful_to = NULL;

    if( !verify( pRec ))
    {
        return;
    }


//    icon = pRec->ui.pchIcon ? atlasLoadTexture( pRec->ui.pchIcon ) : NULL; 

    InitInfo();
    info_setFields( icon, 
        pRec->ui.pchDisplayName , 
        pRec->ui.pchDisplayShortHelp == NULL ? "" : pRec->ui.pchDisplayShortHelp, 
        pRec->flags & RECIPE_NO_TRADE ? "RecipeIsNoTrade": "", //"Class set?", - the text underneath the description
        TRUE );

    info.pRec = (DetailRecipe * ) pRec;

    if (pRec->pchEnhancementReward == NULL)
    {
        // display help    
        addStringToStuffBuff(&info.stuffbuff, "%s", textStd(pRec->ui.pchDisplayHelp));
    } else {
        // display enhancement information
        uiEnhancement *pEnh = uiEnhancementCreateFromRecipe((DetailRecipe*)pRec, pRec->level, false);
        char *pHelp = NULL;
        if (pEnh)
        {
            pHelp = uiEnhancementGetToolTipText(pEnh);
            if (pHelp)
            {
                addStringToStuffBuff(&info.stuffbuff, "%s", pHelp);
            }
            uiEnhancementFree(&pEnh);
        }
    }


    smfview_Reparse(info.pview);

    // add the tabs
    //info_addTab( "InfoSalvage", sal->ui.pchDisplayHelp, TRUE);

    // --------------------
    // show the window

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
    window_setMode( WDW_INFO, WINDOW_GROWING );
}

//------------------------------------------------------------------------------------------------------- SALVAGE

void info_Salvage( const SalvageItem *sal )
{
    AtlasTex * icon;
    DetailRecipe **useful_to = NULL;
    int i;

    if( !verify(sal ))
    {
        return;
    }
     icon = sal->ui.pchIcon ? atlasLoadTexture( sal->ui.pchIcon ) : NULL;

    InitInfo();
    info_setFields( icon, 
        sal->ui.pchDisplayName , 
        sal->ui.pchDisplayShortHelp, 
        (sal->flags & SALVAGE_NOTRADE) ?  "SalvageIsNoTrade": "", //"Class set?", - the text underneath the description
        TRUE );

    // display help    
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd(sal->ui.pchDisplayHelp));

    // display if it's used in player's recipes
    if (salvageitem_IsInvention(sal))
    {
        eaSetSize(&useful_to,0);
        if(character_IsSalvageUseful(playerPtr()->pchar, sal, &useful_to))
        {
            addStringToStuffBuff(&info.stuffbuff, "%s", textStd("<br><br>"));
            addStringToStuffBuff(&info.stuffbuff, "%s", textStd("SalvageDropUseful"));
            for (i = 0; i < eaSize(&useful_to); i++)
            {
                DetailRecipe *pRec = useful_to[i];

                if (pRec)
                {
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("<br>&nbsp;&nbsp;&nbsp;"));
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd(pRec->ui.pchDisplayName));
                }
            }
        } 

        {
            cStashElement elt;
        
            if( stashFindElementConst( g_DetailRecipeDict.stBySalvageComponent, sal->pchName, &elt) )
            {
                const DetailRecipe **ppRec = stashElementGetPointerConst(elt);
                const char ** names=0;
                int j;

                for( i = 0; i < eaSize(&ppRec); i++ )
                {
                    int name_found = 0;

                    for( j = 0; j < eaSize(&names); j++ )
                    {
                        if( stricmp(ppRec[i]->ui.pchDisplayName, names[j]) == 0)
                            name_found = 1;
                    }

                    if(!name_found)    
                        eaPushConst(&names, ppRec[i]->ui.pchDisplayName );
                }

                if (eaSize(&names)) //only print the heading if we found some recipes
                {
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("<br><br>"));
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("SalvageDropUseless"));
                }

                for( i = eaSize(&names)-1; i>=0; i-- )
                {
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("<br>&nbsp;&nbsp;&nbsp;"));
                    addStringToStuffBuff(&info.stuffbuff, "%s", textStd(names[i]));
                }
                eaDestroyConst(&names);
            }
        }

    }

    smfview_Reparse(info.pview);

    // add the tabs
    //info_addTab( "InfoSalvage", sal->ui.pchDisplayHelp, TRUE);

    // --------------------
    // show the window

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
    window_setMode( WDW_INFO, WINDOW_GROWING );
}


void info_SalvageInventory( SalvageInventoryItem *inv )
{
    AtlasTex * icon;
    SalvageItem const *sal;
    U32 all_tab = 1;

    if( !verify( inv && inv->salvage ))
    {
        return;
    }

    sal = inv->salvage;
    icon = sal->ui.pchIcon ? atlasLoadTexture( sal->ui.pchIcon ) : NULL;
    
    InitInfo();
    info_setFields( icon, 
                    sal->ui.pchDisplayName , 
                    sal->ui.pchDisplayShortHelp, 
                    "", //"Class set?", - the text underneath the description
                    TRUE );

    // add the context
    info.context = inv;

    // formatted text stuff
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("RequestingData"));
    smfview_Reparse(info.pview);

    // add the tabs
    info_addTab( "InfoSalvage", sal->ui.pchDisplayHelp, TRUE);
    info_addTab( "InfoSalvageReverseEngineer", "todo: put buttons here", FALSE);
    all_tab = 1;

    // --------------------
    // set the buttons

    if( inv->amount > 0 )
    {
        eaPush(&info.buttons, infobutton_Create("InfoSalvageReverseEngineerOne", infobutton_SalvageCb, (void*)kInfoButtonSalvageRE_One, all_tab ));
        eaPush(&info.buttons, infobutton_Create("InfoSalvageReverseEngineerAll", infobutton_SalvageCb, (void*)kInfoButtonSalvageRE_All, all_tab  ));
    }
    
    // --------------------
    // show the window

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );
}

//------------------------------------------------------------------------------------------------------- POWERS

//
//
void info_Power( const BasePower *pow, int forTarget )
{
    const char *pchHelp;
    const char *pchShortHelp;

    if( !pow )
        return;

    InitInfo();
    info.notranslate = 0;
    info.spec = pow;

    if(forTarget && pow->pchDisplayTargetHelp && *pow->pchDisplayTargetHelp)
        pchHelp = pow->pchDisplayTargetHelp;
    else
        pchHelp = pow->pchDisplayHelp;

    if(forTarget && pow->pchDisplayTargetShortHelp && *pow->pchDisplayTargetShortHelp)
        pchShortHelp = pow->pchDisplayTargetShortHelp;
    else
        pchShortHelp = pow->pchDisplayShortHelp;

    info_setFields( basepower_GetIcon( pow ), pow->pchDisplayName, pchShortHelp,
                    g_PowerDictionary.ppPowerCategories[pow->id.icat]->ppPowerSets[pow->id.iset]->pchDisplayShortHelp, 0 );

    info_addTab( "PowerDescription", textStd(pchHelp), 1 );
    info_addTab( "Power_Info", "power_info", 0 );
    if(playerPtr())
    {
        powerInfoSetLevel( playerPtr()->pchar->iCombatLevel );
        powerInfoSetClass( playerPtr()->pchar->pclass );
    }

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );
}


void info_SpecificPower( int setIdx, int powIdx)
{
    const char *pchHelp;
    const char *pchShortHelp;
    char timeString[128];
    const BasePower *base;
    bool addedspace=false;
    Power *pow;

    if(setIdx < 0 || powIdx < 0 || 
        eaSize(&playerPtr()->pchar->ppPowerSets) <=setIdx || 
        eaSize(&playerPtr()->pchar->ppPowerSets[setIdx]->ppPowers)<=powIdx)
        return;
    pow = playerPtr()->pchar->ppPowerSets[setIdx]->ppPowers[powIdx];

    if( !pow || !pow->ppowBase)
        return;
    base = pow->ppowBase;
    pchHelp = base->pchDisplayHelp;
    pchShortHelp = base->pchDisplayShortHelp;

    InitInfo();
    info.notranslate = 0;
    info.pow = pow;
    info.pcharPowIdxs[0] = setIdx;
    info.pcharPowIdxs[1] = powIdx;
    info.spec = base;

    info_setFields( basepower_GetIcon( base ), base->pchDisplayName, pchShortHelp,
        g_PowerDictionary.ppPowerCategories[base->id.icat]->ppPowerSets[base->id.iset]->pchDisplayShortHelp, 0 );
    
    // formatted text stuff
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd(pchHelp) );

    // Add temporary power info

    if (base->bHasUseLimit)
    {
        if (!addedspace)
        {
            addedspace=true;
            addStringToStuffBuff(&info.stuffbuff,"<br>");
        }
        if (base->iNumCharges)
        {
            addStringToStuffBuff(&info.stuffbuff,"<br>%s",textStd("PowerUsesLeft",pow->iNumCharges));            
        }
        if (base->fUsageTime)
        {
            timerMakeOffsetStringFromSeconds(timeString,pow->fUsageTime);
            addStringToStuffBuff(&info.stuffbuff,"<br>%s",textStd("PowerTimeLeft",timeString));            
        }
    }
    if (base->bHasLifetime)
    {
        U32 secleft;
        if (!addedspace)
        {
            addedspace = true;
            addStringToStuffBuff(&info.stuffbuff,"<br>");
        }
        // Total time expire
        if (base->fLifetime)
        {        
            secleft = pow->ulCreationTime+base->fLifetime - timerSecondsSince2000();
            if (secleft > 60*60*48)
            {
                // If we're over 2 das, say things in terms of days.
                sprintf(timeString,textStd("PowerTimeDays",secleft/(60*60*24)));
            }
            else
            {
                timerMakeOffsetStringFromSeconds(timeString,secleft);
            }
            addStringToStuffBuff(&info.stuffbuff,"<br>%s",textStd("PowerExpireLeft",timeString));            
        }
        // In game time expire
        if (base->fLifetimeInGame)
        {        
            secleft = base->fLifetimeInGame-pow->fAvailableTime;
            if (secleft > 60*60*48)
            {
                // If we're over 2 das, say things in terms of days.
                sprintf(timeString,textStd("PowerTimeDays",secleft/(60*60*24)));
            }
            else
            {
                timerMakeOffsetStringFromSeconds(timeString,secleft);
            }
            addStringToStuffBuff(&info.stuffbuff,"<br>%s",textStd("PowerExpireInGameLeft",timeString));            
        }
    }

    info_addTab( "PowerDescription", info.stuffbuff.buff, 1 );
    info_addTab( "Power_Info", "power_info", 0 );

    if(playerPtr())
    {
        powerInfoSetLevel( playerPtr()->pchar->iCombatLevel );
        powerInfoSetClass( playerPtr()->pchar->pclass );
    }

    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
    window_setMode( WDW_INFO, WINDOW_GROWING );
}

//------------------------------------------------------------------------------------------------------- BASE ITEMS

//
////
void info_Item( const Detail *item, AtlasTex * pic )
{
    RoomDetail * room_detail = 0;
    const DetailCategory *pcat;

    if( !item || !item->pchDisplayName || !item->pchDisplayShortHelp )
        return;

    InitInfo();
    info.notranslate = NOTRANS_CLASS;

    pcat = basedata_GetDetailCategoryByName(item->pchCategory);
    info_setFields( pic, item->pchDisplayName, pcat->pchDisplayName, detailGetDescription(item,0), 0 );

    // BASE_TODO: Add requirement string and upgrade string to help
    // formatted text stuff
    addStringToStuffBuff(&info.stuffbuff, "%s", getBaseDetailDescriptionString(item,NULL,0) );
    
    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );
}

void info_Room( const RoomTemplate *room )
{
    if( !room || !room->pchDisplayName || !room->pchDisplayShortHelp )
        return;

    InitInfo();
    info.notranslate = 0;
    info.room = room;

    // BASE_TODO: pchUpgrade should be category name.  Need back-pointer in item stuct though
    info_setFields( NULL, room->pchDisplayName, room->pRoomCat->pchDisplayName, room->pchDisplayShortHelp, 0 );
    addStringToStuffBuff(&info.stuffbuff, "%s", baseRoomGetDescriptionString( room, NULL, 1, 1, 0) );

    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );
}

void info_Plot( const BasePlot * plot )
{
    if( !plot || !plot->pchDisplayName || !plot->pchDisplayShortHelp )
        return;

    InitInfo();
    info.notranslate = 0;
    info.plot = plot;

    // BASE_TODO: pchUpgrade should be category name.  Need back-pointer in item stuct though
    info_setFields( NULL, plot->pchDisplayName, textStd("HiddenBases"), plot->pchDisplayShortHelp, 0 );    
    addStringToStuffBuff(&info.stuffbuff, "%s", basePlotGetDescriptionString( plot ,0));

    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
    window_setMode( WDW_INFO, WINDOW_GROWING );
}


//------------------------------------------------------------------------------------------------------- ENHANCEMNTS
//
//
void info_CombineSpec( const BasePower *pow, int inventory, int level /* zero-based */, int numCombines)
{
    int iCharLevel;

    if( !pow )
        return;

    iCharLevel = character_CalcExperienceLevel(playerPtr()->pchar);

    InitInfo();

    info.spec = pow;
    info.icon = 0;
    info.notranslate = 0;
    info.inventory = inventory;
    info.level = level;
    info.numCombines = numCombines;

    info_setFields( NULL, pow->pchDisplayName, pow->pchDisplayShortHelp, g_PowerDictionary.ppPowerCategories[pow->id.icat]->ppPowerSets[pow->id.iset]->pchDisplayShortHelp, 0 );

    // formatted text stuff    
    uiEnhancementAddHelpText(NULL, pow, level + numCombines, &info.stuffbuff);
    addStringToStuffBuff(&info.stuffbuff, "<br>");

    if (pow->bBoostBoostable)
        uiEnhancementGetInfoText(&info.stuffbuff, pow, NULL, level, false);
    else 
        uiEnhancementGetInfoText(&info.stuffbuff, pow, NULL, level + numCombines, false);

    smfview_Reparse(info.pview);

    if( !shell_menu() && window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );
}

//
//
void info_CombineSpecAllowed( Power *pow )
{
    int i;
    const BasePower *pBase;

    if( !pow )
        return;

    pBase = pow->ppowBase;

    InitInfo();

    // icon
    info.spec = 0; 
    info.inventory = 0;
    info.notranslate = 0;
    info_setFields( basepower_GetIcon( pBase ), pBase->pchDisplayName, pBase->pchDisplayShortHelp,
        g_PowerDictionary.ppPowerCategories[pBase->id.icat]->ppPowerSets[pBase->id.iset]->pchDisplayName, 0);

    ////////////////////////////////////////////////////////////////////////
    // Formatted text

    // Enhancements
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("AllowedEnhancements"));
    addStringToStuffBuff(&info.stuffbuff, ":<br><color InfoBlue>");
    for(i=0;i<eaiSize(&(int *)pBase->pBoostsAllowed);i++)
    {
        int j;
        if(i>0)
        {
            addStringToStuffBuff(&info.stuffbuff, ", ");
        }

        // TODO: This feels hackish. Maybe load_def should fiddle this
        //       array instead. --poz
        //mw 3.10.06 added guard here because it's everywhere else this calc is done, 
        //and there's reported crash here that I can't repro, so I'm doing this and hoping for the best
        //(subtracting off the number of origins seems insane and neither Jered nor CW can remember why its needed)
        if( pBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
        {
            j = pBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins);
        
            addStringToStuffBuff(&info.stuffbuff, "%s", textStd(g_AttribNames.ppBoost[j]->pchDisplayName));
        }
    }
    addStringToStuffBuff(&info.stuffbuff, "</color>");

    // Slotted Boosts for Boostsets
    if (eaSize(&pow->ppBoosts) > 0)
    {
        int *boostSetCount;
        const BoostSet **boostSet;

        eaCreateConst(&boostSet);
        eaiCreate(&boostSetCount);

        // make a list of which boost sets are in this power and how many of each type
        for (i = 0; i < eaSize(&pow->ppBoosts); i++) 
        {
            Boost *pBoost = pow->ppBoosts[i];

            if (pBoost && pBoost->ppowBase->pBoostSetMember)
            {
                int loc = eaPushUniqueConst(&boostSet, pBoost->ppowBase->pBoostSetMember);
                if (loc + 1 > eaiSize(&boostSetCount))
                    eaiSetSize(&boostSetCount, loc + 1);
                boostSetCount[loc]++;
            }
        }

        // find out what bonuses this makes the player eligible for
        if (eaSize(&boostSet) > 0)  
        { 
            addStringToStuffBuff(&info.stuffbuff, "<br><br>");
            addStringToStuffBuff(&info.stuffbuff, "%s", textStd("ActiveBoostSets"));
            addStringToStuffBuff(&info.stuffbuff, "<br>"); 

            for (i = 0; i < eaSize(&boostSet); i++)
            {    
                BoostSet *pBoostSet = (BoostSet *) boostSet[i];
                int j;

                addStringToStuffBuff(&info.stuffbuff, "<color InfoBlue>%s (%d/%d)</color><br>", textStd(pBoostSet->pchDisplayName), 
                                        boostSetCount[i], eaSize(&pBoostSet->ppBoostLists));

                for (j = 0; j < eaSize(&pBoostSet->ppBonuses); j++)
                {
                    if ((pBoostSet->ppBonuses[j]->iMinBoosts == 0 || boostSetCount[i] >= pBoostSet->ppBonuses[j]->iMinBoosts) &&
                        (pBoostSet->ppBonuses[j]->iMaxBoosts == 0 || boostSetCount[i] <= pBoostSet->ppBonuses[j]->iMaxBoosts) )
                    {
                        // this effect applies

                        if (pBoostSet->ppBonuses[j]->pBonusPower) 
                            addStringToStuffBuff(&info.stuffbuff, "<color SpringGreen>%s</color><br>", textStd(pBoostSet->ppBonuses[j]->pBonusPower->pchDisplayHelp));

                        if (eaSize(&pBoostSet->ppBonuses[j]->ppAutoPowers) > 0) 
                        {
                            int k;
                            for (k = 0; k < eaSize(&pBoostSet->ppBonuses[j]->ppAutoPowers); k++)
                            {
                                const BasePower *pAuto = pBoostSet->ppBonuses[j]->ppAutoPowers[k];
                                addStringToStuffBuff(&info.stuffbuff, "<color SpringGreen>%s</color><br>", textStd(pAuto->pchDisplayHelp));
                            }
                        }
                    }
                }
            }
        }

        eaDestroyConst(&boostSet);
        eaiDestroy(&boostSetCount);
    }

    smfview_Reparse(info.pview);
    info.headshot = 0;
}

//
//
void info_combineMenu( void *pow, int isSpec, int inventory, int level, int numCombines )
//void info_combineMenu( BasePower * pow, int isSpec, int inventory, int level, int numCombines )
{
    if( isSpec )
        info_CombineSpec( (BasePower *) pow, inventory, level, numCombines );
    else
        info_CombineSpecAllowed( (Power *) pow );
}

//------------------------------------------------------------------------------------------------------- ENTITIES
//
//
static void info_Player( Entity *e, int tab )
{
    AtlasTex * icon;
    char tmp[128];

    if( e == NULL || e->pchar == NULL )
        return;

    icon = e->costume ? seqGetHeadshotForce(e->costume, (e->db_id|(e->pl->supergroup_mode<<30))) : 0;

    InitInfo();
    info.notranslate = NOTRANS_TITLE;
    strncpy(tmp, textStd("InfoLevel", e->pchar->iLevel+1), 127 );
    info_setFields( icon, e->name, tmp,    textStd("OriginAndClass", e->pchar->porigin->pchDisplayName, e->pchar->pclass->pchDisplayName), 1 );

    // formatted text stuff
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("RequestingData"));
    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );

    sendEntityInfoRequest( e->svr_idx, tab );
}

static bool shouldShowHeadshot(Entity *e)
{
    if (!e->costume)
        return false;
    if (e->seq) {
        if ( e->seq->worldGroupPtr )
            return false;
        if ( getRootHealthFxLibraryPiece( e->seq , (int)e->svr_idx ) )//HealthFXMultiples
            return false;
    }
    return true;
}

//
//
static void info_Critter( Entity *e )
{
    AtlasTex *icon;
    char tmp[128];

    if( !e )
        return;

    //icon = shouldShowHeadshot(e) ? seqGetHeadshot(e->costume, 0) : 0;
    icon = e->ownedCostume ? seqGetHeadshotForce(e->costume, 0) : seqGetHeadshot(e->costume, 0); 

    InitInfo();
    info.notranslate = 0;

    if (e->pchar) {
        if (e->pchar->bIgnoreCombatMods)
        {
            strncpy(tmp, textStd("InfoLevelNoCombatMods"), 127 );
        } else {
            strncpy(tmp, textStd("LevelLevel", e->pchar->iLevel+1), 127 );
        }
    } else {
        strncpy(tmp, textStd("LevelLevel", 0), 127 ); //This crashed before, this should never happen
    }
    info_setFields( icon, e->name, tmp,    textStd("OriginAndClass", e->pchVillGroupName, e->pchDisplayClassNameOverride?e->pchDisplayClassNameOverride:e->pchar->pclass->pchDisplayName), 1 );

    // formatted text stuff
    addStringToStuffBuff(&info.stuffbuff, "%s", textStd("RequestingData"));
    smfview_Reparse(info.pview);

    if( window_getMode(WDW_INFO) != WINDOW_DISPLAYING );
        window_setMode( WDW_INFO, WINDOW_GROWING );

    sendEntityInfoRequest( e->svr_idx, 0 );
}



void info_Entity( Entity *e, int tab )
{
    if( e )
    {
        if( ENTTYPE(e) == ENTTYPE_PLAYER )
            info_Player( e, tab );
        if( ENTTYPE(e) == ENTTYPE_CRITTER )
        {
            RoomDetail *pDetail = NULL;
            BaseRoom *pRoom = NULL;

            if( g_base.rooms )
            {    
                pRoom = baseRoomLocFromPos(&g_base, ENTPOS(e), NULL);
                if(pRoom)
                    pDetail = roomGetDetailFromWorldPos(pRoom, ENTPOS(e));
            }

            if( pDetail && pDetail->info )
            {    
                const Detail *item = pDetail->info;
                info_Item( item, groupThumbnailGet(detailGetThumbnailName(item), false, item->eSurface==kSurface_Ceiling,0) );
                return;
            }
            else
                info_Critter( e );
        }

        info.ent = e;
        info.eRef = erGetRef(e);
        info.headshot = 1;
    }
}

void info_EntityTarget( void *unused )
{
    if( current_target )
        info_Entity(current_target, 0);
}


//----------------------------------------------------------------------------------------------------- DRAWING


#define ICON_WD     80
#define ICON_MAX    (64.0f)

void DrawTitle(TTDrawContext *pfont, int cx, int cy, int cz, int width, char *pch, float sc)
{
    int rgba[4];
    float tmpWd;
    char *pchTmp;

    if(info.notranslate & NOTRANS_TITLE)
    {
        pchTmp = _alloca(strlen(pch)+1);
        strcpy(pchTmp, pch);
    }
    else
    {
        pchTmp = _alloca(strlen(textStd(pch))+1);
        strcpy(pchTmp, textStd(pch));
    }

    tmpWd = str_wd_notranslate(pfont, sc, sc, pchTmp);

    if( tmpWd > width)
    {
        char *pchMid = pchTmp+strlen(pchTmp)/2;
        while(*pchMid!=' ' && *pchMid!=0)
        {
            pchMid++;
        }
        if(*pchMid)
        {
            *pchMid = 0;
            pchMid++;
        }

        if(str_wd_notranslate(pfont, sc, sc, pchTmp) > width
            || (strlen(pchMid) && str_wd_notranslate(pfont, sc, sc, pchMid) > width))
        {
            float sc2 = str_sc(pfont, width, pchTmp);
            sc = str_sc(pfont, width, pchMid);
            sc = min(sc, sc2);
        }

        // Not cprnt since we've already translated
        determineTextColor(rgba);
        printBasic(font_grp, cx, cy-9*sc, cz, sc, sc, NO_MSPRINT, pchTmp, strlen(pchTmp), rgba);
        printBasic(font_grp, cx, cy+6*sc, cz, sc, sc, NO_MSPRINT, pchMid, strlen(pchMid), rgba);
    }
    else
    {
        cprntEx(cx, cy, cz, sc, sc, NO_MSPRINT, "%s", pchTmp);
    }
}

typedef struct BadgeRow
{
    AtlasTex **spr;
    const BadgeDef **badge;
    float wd;
    float ht;
    int iOffset;
} BadgeRow;

typedef struct BadgeEmbiggen
{
    AtlasTex *spr;
    float idx;
    float x;
    float y;
    float winsc;
    float sc;
}BadgeEmbiggen;

BadgeEmbiggen **badgeEmbiggen = 0;
#define BADGE_SCALE        .5f
#define ICON_SPACE        10

void badge_Embiggen( AtlasTex * spr, float x, float y, float winsc, int idx )
{
    int i;
    BadgeEmbiggen *be;

    for( i=eaSize(&badgeEmbiggen)-1; i >= 0; i-- )
    {
        if(badgeEmbiggen[i]->idx == idx)
            return;
    }

    be = malloc(sizeof(BadgeEmbiggen));
    be->sc = BADGE_SCALE;
    be->idx = idx;
    be->winsc = winsc;
    be->spr = spr;
    be->x = x;
    be->y = y;
    eaPush(&badgeEmbiggen, be );

}

float badge_GetEmbiggen( int idx )
{
    int i;
    for( i=eaSize(&badgeEmbiggen)-1; i >= 0; i-- )
    {
        if(badgeEmbiggen[i]->idx == idx)
            return badgeEmbiggen[i]->sc;
    }

    return BADGE_SCALE;
}

void badge_EmbiggenUpdate()
{
    int i;
    float scale_spd = .1f;
    float iconspace = ICON_SPACE;

    for( i=eaSize(&badgeEmbiggen)-1; i >= 0; i-- )
    {
        CBox box;

        BuildCBox( &box, badgeEmbiggen[i]->x - iconspace/2, badgeEmbiggen[i]->y - iconspace/2,
                    (badgeEmbiggen[i]->spr->width*BADGE_SCALE + iconspace)*badgeEmbiggen[i]->winsc-1,
                    (badgeEmbiggen[i]->spr->height*BADGE_SCALE+ iconspace)*badgeEmbiggen[i]->winsc-1 );
        if( mouseCollision(&box) )
        {
            badgeEmbiggen[i]->sc += TIMESTEP*scale_spd;
            if( badgeEmbiggen[i]->sc > 1.f )
                badgeEmbiggen[i]->sc = 1.f;
        }
        else
        {
            badgeEmbiggen[i]->sc -= TIMESTEP*scale_spd;
            if( badgeEmbiggen[i]->sc < BADGE_SCALE )
                eaRemove( &badgeEmbiggen, i );
        }
    }
}

void draw_BadgeRow( float x, float y, float z, float wsc, float wd, BadgeRow * prow)
{
    int i;
    int idxText = ENT_IS_VILLAIN(erGetEnt(info.eRef)) ? 1 : 0;
    float  iconspace = wsc*ICON_SPACE, tx = x + (wd - prow->wd + iconspace)/2;

    for( i = 0; i < eaSize(&prow->spr); i++ )
    {
        CBox box;
        float sc = badge_GetEmbiggen( prow->badge[i]->iIdx );
        float ty = y + (prow->ht - prow->spr[i]->height*sc*wsc)/2;

        BuildCBox( &box, tx-iconspace/2, ty-iconspace/2, prow->spr[i]->width*wsc*BADGE_SCALE+iconspace-1, prow->ht+iconspace-1 );
        if( mouseCollision(&box) )
            badge_Embiggen(prow->spr[i], tx, ty, wsc, prow->badge[i]->iIdx);

        if( sc == BADGE_SCALE )
            display_sprite( prow->spr[i], tx - (prow->spr[i]->width*sc-prow->spr[i]->width*BADGE_SCALE)/2, ty, z, sc*wsc, sc*wsc, CLR_WHITE );
        else
        {
            float fake_sc_max = 5.f*(sc-BADGE_SCALE), fake_sc = fake_sc_max, cx;
            int j, count = 10;

            cx = tx + prow->spr[i]->width*BADGE_SCALE/2;
            if( cx - prow->spr[i]->width*sc/2 < x )
                cx = x + prow->spr[i]->width*sc/2;
            if( cx + prow->spr[i]->width*sc/2 > x + wd)
                cx = x + wd - prow->spr[i]->width*sc/2;

            display_sprite( prow->spr[i], cx - prow->spr[i]->width*sc/2, ty, z+2, sc*wsc, sc*wsc, CLR_WHITE );

            for( j = 0; j < count; j++ )
            {
                display_sprite( prow->spr[i], cx - prow->spr[i]->width*fake_sc/2, ty - (prow->spr[i]->height*fake_sc-prow->spr[i]->height)/2, z+1, sc*wsc*fake_sc, sc*wsc*fake_sc, (int)(255/(count-j)) );
                fake_sc -= (fake_sc_max-sc)/count;
            }


            { // badge title

                float txt_sc = ((sc-BADGE_SCALE)/(1-BADGE_SCALE));
                float txt_wd = str_wd( &game_12, txt_sc, txt_sc, prow->badge[i]->pchDisplayTitle[idxText]);

                int color = (CLR_PARAGON & 0xffffff00)|(int)(txt_sc*255);

                if( txt_wd < 200*wsc ) // don't need to wrap it
                {
                    if( cx - txt_wd/2 < x )
                        cx = x    + txt_wd/2;

                    if( cx + txt_wd/2 > x + wd )
                        cx = x    + wd - txt_wd/2;

                    font(&game_12);
                    font_color( color, color );
                    cprntEx( cx, ty + (prow->spr[i]->height+4)*sc , z+3, txt_sc, txt_sc, (CENTER_X|CENTER_Y),  printLocalizedEnt(prow->badge[i]->pchDisplayTitle[idxText], erGetEnt(info.eRef)));
                }
                else
                {
                    char tmp[1024] = {0};

                    txt_wd = 200*wsc;
                    if( cx - txt_wd/2 < x )
                        cx = x    + txt_wd/2;

                    if( cx + txt_wd/2 > x + wd )
                        cx = x    + wd - txt_wd/2;

                    strcat( tmp, "<table><tr><td align=center>");
                    strcat( tmp, textStd(prow->badge[i]->pchDisplayTitle[idxText]) );

                    s_taTitle.piScale = (int *)((int)(sc*SMF_FONT_SCALE));

                    smf_ParseAndDisplay(NULL, tmp, cx - txt_wd/2, ty + (prow->spr[i]->height+4)*sc, z+1, txt_wd, 0, 1, 1, &s_taTitle, NULL, 0, true);
                }
            }
        }

        tx += prow->spr[i]->width*wsc*BADGE_SCALE+iconspace;
    }
}


void info_displayBadges( float x, float y, float z, float sc, float wd, float ht )
{
    int i,j, iCount = 0, iSize = eaSize(&g_BadgeDefs.ppBadges), iconspace = sc*ICON_SPACE;
    const BadgeDef *badge;
    AtlasTex  *icon;
    static ScrollBar sb = { WDW_INFO, 0 };
    float tx = x, ty = y, iconSc = BADGE_SCALE;
    AtlasTex * left = atlasLoadTexture( "Headerbar_HorizDivider_L.tga" );
    AtlasTex * right = atlasLoadTexture( "Headerbar_HorizDivider_R.tga" );
    static char *pchCount;
    int bShowCategory[kBadgeType_Count] = {0};

    set_scissor(1);
    scissor_dims(x-PIX3*sc, y-PIX3*sc, wd+PIX3*sc*2, ht+PIX3*sc*4);

    if( !sb.grabbed )
        badge_EmbiggenUpdate();

    for(j = 0; j < iSize; ++j)
    {
        badge = g_BadgeDefs.ppBadges[j];
        if(badge && badge->eCategory &&    !badge->bDoNotCount &&
            badge->iIdx >= 0 && badge->iIdx <= g_BadgeDefs.idxMax)
        {
            if(BitFieldGet(info.aiBadges, BADGE_ENT_BITFIELD_SIZE, badge->iIdx))
            {
                bShowCategory[badge->eCategory] = 1;
                iCount++;
            }
        }
    }
    estrPrintCharString(&pchCount,textStd("CountBadgesEarned",iCount));

    ty += 20*sc;
    font(&game_14);
    drawMenuHeader( tx + wd/2, ty - sb.offset, z, CLR_WHITE, 0xffb688ff, pchCount, 1.f);
    ty += 10*sc;

    for( i = kBadgeType_LastBadgeCategory-1; i > 0; i-- )
    {
        BadgeRow brow = {0};

        if(!bShowCategory[i])
            continue;

        ty += 20*sc;
        font(&game_14);
        drawMenuHeader( tx + wd/2, ty - sb.offset, z, CLR_WHITE, 0x88b6ffff, badge_CategoryGetName(playerPtr(), kCollectionType_Badge, i), 1.f );
        ty += 10*sc;

        for(j = 0; j < iSize; j++)
        {
            int iIdx = g_BadgeDefs.ppBadges[j]->iIdx;
            int idxType = ENT_IS_VILLAIN(erGetEnt(info.eRef)) ? 1 : 0;

            if( iIdx<0 || !BitFieldGet(info.aiBadges, BADGE_ENT_BITFIELD_SIZE, iIdx))
                continue;

            badge = g_BadgeDefs.ppBadges[j];

            if(!badge ||  i != badge->eCategory || badge->bDoNotCount)
                continue;

            icon = atlasLoadTexture(badge->pchIcon[idxType]);
            if(!icon)
                continue;

            if( tx + icon->width*sc*iconSc > x+wd )
            {
                brow.wd -= iconspace;
                draw_BadgeRow( x, ty-sb.offset, z, sc, wd, &brow );
                tx = x;
                ty += brow.ht;
                eaDestroy(&brow.spr);
                memset( &brow, 0, sizeof(BadgeRow));
            }

            eaPush( &brow.spr, icon );
            eaPushConst( &brow.badge, badge );

            if(tx == x)
                brow.iOffset = j;
            brow.wd += iconspace+icon->width*sc*iconSc;
            brow.ht = MAX(brow.ht, iconspace+icon->height*sc*iconSc);
            tx += iconspace+icon->width*sc*iconSc;
        }
        brow.wd -= iconspace;
        draw_BadgeRow( x, ty-sb.offset, z, sc, wd, &brow );
        ty += brow.ht;
        tx = x;

    }
    set_scissor(0);

    doScrollBar( &sb, ht, ty + 70 - y, PIX3*sc, (HEADER_HT+PIX3*2+R10)*sc, z+5, 0, 0 );
}

static ToolTip uiInfo_alignmentTooltip;

void updateDisplayAlignmentStatsToOthers()
{
    Entity *e = playerPtr();

    if (e && e->pl)
    {
        START_INPUT_PACKET(pak, CLIENTINP_SEND_DISPLAYALIGNMENTSTATSTOOTHERS_SETTING);
        pktSendBitsAuto(pak, e->pl->displayAlignmentStatsToOthers);
        END_INPUT_PACKET
    }
}

void info_displayAlignment( Entity *e, float x, float y, float z, float sc, float wd, float ht, int color )
{
    static SMFBlock *statsBlock = 0;
    char *statsString = 0;
    int timeAsCurrentAlignment = 0;
    char tempString[80];
    static ScrollBar sb = { WDW_INFO, 0 };
    int missionsCompleted_Paragon = alignmentMissionsCompleted_Paragon + 
                                    switchMoralityMissionsCompleted_Paragon + 
                                    reinforceMoralityMissionsCompleted_Paragon;
    int missionsCompleted_Hero = alignmentMissionsCompleted_Hero + 
                                    switchMoralityMissionsCompleted_Hero + 
                                    reinforceMoralityMissionsCompleted_Hero;
    int missionsCompleted_Vigilante = alignmentMissionsCompleted_Vigilante + 
                                    switchMoralityMissionsCompleted_Vigilante + 
                                    reinforceMoralityMissionsCompleted_Vigilante;
    int missionsCompleted_Rogue = alignmentMissionsCompleted_Rogue + 
                                    switchMoralityMissionsCompleted_Rogue + 
                                    reinforceMoralityMissionsCompleted_Rogue;
    int missionsCompleted_Villain = alignmentMissionsCompleted_Villain + 
                                    switchMoralityMissionsCompleted_Villain + 
                                    reinforceMoralityMissionsCompleted_Villain;
    int missionsCompleted_Tyrant = alignmentMissionsCompleted_Tyrant + 
                                    switchMoralityMissionsCompleted_Tyrant + 
                                    reinforceMoralityMissionsCompleted_Tyrant;
    int ty = 0;
    
    if (!e || !e->pl)
    {
        return;
    }

    if (!statsBlock)
    {
        statsBlock = smfBlock_Create();
        smf_SetFlags(statsBlock, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, SMFScrollMode_ExternalOnly, 0, SMFDisplayMode_AllCharacters, 0, SMAlignment_Left, 0, 0, 0);
    }

     set_scissor(1);
     scissor_dims(x-PIX3*sc, y-PIX3*sc, wd+PIX3*sc*2, ht+PIX3*sc*4);

    y -= sb.offset;

    if (e->db_id == playerPtr()->db_id)
    {
        drawSmallCheckbox(WDW_INFO, 20, 110 - sb.offset, "DisplayAlignmentStatsToOthersText", &e->pl->displayAlignmentStatsToOthers, updateDisplayAlignmentStatsToOthers, true, 0, 0);
        y += 22;
    }

    ty -= y; // add the y difference added in displayAlignmentStatus to the scroll bar size
    displayAlignmentStatus(e, x, &y, z, wd, sc, color, 0, &uiInfo_alignmentTooltip);
    ty += y;

    //******************************
    // Draw the statistics
    //******************************

    estrPrintCharString(&statsString, "<table>");

    // Time Played as Current Alignment

    estrConcatf(&statsString, "<tr><td>%s</td>", textStd("TimePlayedAsCurrentAlignment"));

    timeAsCurrentAlignment = timePlayedAsCurrentAlignmentAsOfTimeWasLastSaved + timerSecondsSince2000() - lastTimeWasSaved;
    estrConcatCharString(&statsString, "<td>");
    if (timeAsCurrentAlignment >= 60 * 60 * 24)
    {
        estrConcatf(&statsString, "%i ", timeAsCurrentAlignment / (60 * 60 * 24));
        if (timeAsCurrentAlignment >= 60 * 60 * 24 * 2)
        {
            estrConcatCharString(&statsString, textStd("DaysPlural"));
        }
        else
        {
            estrConcatCharString(&statsString, textStd("DaySingular"));
        }
        estrConcatCharString(&statsString, ", ");
    }
    estrConcatf(&statsString, "%i:%02i:%02i</td></tr>", timeAsCurrentAlignment / 3600 % 24, timeAsCurrentAlignment / 60 % 60, timeAsCurrentAlignment % 60);

    // Date of Last Alignment Change

    estrConcatf(&statsString, "<tr><td>%s</td>", textStd("DateOfLastAlignmentChange"));

    if (lastTimeAlignmentChanged)
    {
        timerMakeDateStringFromSecondsSince2000(tempString, lastTimeAlignmentChanged);
    }
    else
    {
        strcpy(tempString, "N/A");
    }
    estrConcatf(&statsString, "<td>%s</td></tr>", tempString);

    // Preferred Alignment

    if (missionsCompleted_Paragon &&
        missionsCompleted_Paragon >= missionsCompleted_Hero &&
        missionsCompleted_Paragon >= missionsCompleted_Vigilante &&
        missionsCompleted_Paragon >= missionsCompleted_Rogue &&
        missionsCompleted_Paragon >= missionsCompleted_Villain &&
        missionsCompleted_Paragon >= missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("ParagonAlignment"));
    }
    else if (missionsCompleted_Hero &&
             missionsCompleted_Hero >= missionsCompleted_Vigilante &&
             missionsCompleted_Hero >= missionsCompleted_Rogue &&
             missionsCompleted_Hero >= missionsCompleted_Villain &&
             missionsCompleted_Hero >= missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("HeroAlignment"));
    }
    else if (missionsCompleted_Vigilante &&
             missionsCompleted_Vigilante >= missionsCompleted_Rogue &&
             missionsCompleted_Vigilante >= missionsCompleted_Villain &&
             missionsCompleted_Vigilante >= missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("VigilanteAlignment"));
    }
    else if (missionsCompleted_Rogue &&
             missionsCompleted_Rogue >= missionsCompleted_Villain &&
             missionsCompleted_Rogue >= missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("RogueAlignment"));
    }
    else if (missionsCompleted_Villain &&
             missionsCompleted_Villain >= missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("VillainAlignment"));
    }
    else if (missionsCompleted_Tyrant)
    {
        strcpy(tempString, textStd("TyrantAlignment"));
    }
    else if (ENT_IS_VILLAIN(e))
    {
        strcpy(tempString, textStd("VillainAlignment"));
    }
    else
    {
        strcpy(tempString, textStd("HeroAlignment"));
    }

    estrConcatf(&statsString, "<tr><td>%s</td><td>%s</td></tr>", textStd("PreferredAlignment"), tempString);

    // Number of Alignment Changes

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("AlignmentChangeCount"), countAlignmentChanged);

    // Total Alignment Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("TotalAlignmentMissionsCompleted"),
                                alignmentMissionsCompleted_Paragon + alignmentMissionsCompleted_Hero +
                                alignmentMissionsCompleted_Vigilante + alignmentMissionsCompleted_Rogue +
                                alignmentMissionsCompleted_Villain + alignmentMissionsCompleted_Tyrant);

    // Don't display Paragon Alignment Missions Completed yet!

    // Hero Alignment Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("HeroAlignmentMissionsCompleted"), alignmentMissionsCompleted_Hero);

    // Vigilante Alignment Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VigilanteAlignmentMissionsCompleted"), alignmentMissionsCompleted_Vigilante);

    // Rogue Alignment Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("RogueAlignmentMissionsCompleted"), alignmentMissionsCompleted_Rogue);

    // Villain Alignment Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VillainAlignmentMissionsCompleted"), alignmentMissionsCompleted_Villain);

    // Don't display Tyrant Alignment Missions Completed yet!

    // Total Alignment Tips Dismissed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("TotalAlignmentTipsDismissed"), alignmentTipsDismissed);

    // Total Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("TotalMoralityMissionsCompleted"),
                                switchMoralityMissionsCompleted_Paragon + switchMoralityMissionsCompleted_Hero +
                                switchMoralityMissionsCompleted_Vigilante + switchMoralityMissionsCompleted_Rogue +
                                switchMoralityMissionsCompleted_Villain + switchMoralityMissionsCompleted_Tyrant +
                                reinforceMoralityMissionsCompleted_Paragon + reinforceMoralityMissionsCompleted_Hero +
                                reinforceMoralityMissionsCompleted_Vigilante + reinforceMoralityMissionsCompleted_Rogue +
                                reinforceMoralityMissionsCompleted_Villain + reinforceMoralityMissionsCompleted_Tyrant);

    // Don't display Paragon Morality Missions Completed yet!

    // Hero Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("HeroMoralityMissionsCompleted"),
                                switchMoralityMissionsCompleted_Hero + reinforceMoralityMissionsCompleted_Hero);

    // Vigilante Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VigilanteMoralityMissionsCompleted"),
                                switchMoralityMissionsCompleted_Vigilante + reinforceMoralityMissionsCompleted_Vigilante);

    // Rogue Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("RogueMoralityMissionsCompleted"),
                                switchMoralityMissionsCompleted_Rogue + reinforceMoralityMissionsCompleted_Rogue);

    // Villain Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VillainMoralityMissionsCompleted"),
                                switchMoralityMissionsCompleted_Villain + reinforceMoralityMissionsCompleted_Villain);

    // Don't display Tyrant Morality Missions Completed yet!

    // Don't display Paragon Reinforce Morality Missions Completed yet!

    // Hero Reinforce Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("HeroReinforceMoralityMissionsCompleted"), reinforceMoralityMissionsCompleted_Hero);

    // Vigilante Reinforce Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VigilanteReinforceMoralityMissionsCompleted"), reinforceMoralityMissionsCompleted_Vigilante);

    // Rogue Reinforce Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("RogueReinforceMoralityMissionsCompleted"), reinforceMoralityMissionsCompleted_Rogue);

    // Villain Reinforce Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VillainReinforceMoralityMissionsCompleted"), reinforceMoralityMissionsCompleted_Villain);

    // Don't display Tyrant Reinforce Morality Missions Completed yet!

    // Don't display Paragon Switch Morality Missions Completed yet!

    // Hero Switch Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("HeroSwitchMoralityMissionsCompleted"), switchMoralityMissionsCompleted_Hero);

    // Vigilante Switch Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VigilanteSwitchMoralityMissionsCompleted"), switchMoralityMissionsCompleted_Vigilante);

    // Rogue Switch Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("RogueSwitchMoralityMissionsCompleted"), switchMoralityMissionsCompleted_Rogue);

    // Villain Switch Morality Missions Completed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("VillainSwitchMoralityMissionsCompleted"), switchMoralityMissionsCompleted_Villain);

    // Don't display Tyrant Switch Morality Missions Completed yet!

    // Total Morality Tips Dismissed

    estrConcatf(&statsString, "<tr><td>%s</td><td>%i</td></tr>", textStd("TotalMoralityTipsDismissed"), moralityTipsDismissed);

    estrConcatStaticCharArray(&statsString, "</table>");

    smf_SetRawText(statsBlock, statsString, false);

    ty += smf_Display(statsBlock, x, y, z, wd, 0, 0, 0, &s_taDefaults, 0);

    set_scissor(0);

    doScrollBar(&sb, ht, ty, PIX3*sc, (HEADER_HT+PIX3*2+R10)*sc, z+5, 0, 0);
}

#define SAL_ICON_WD 48
#define SAL_COL_WD 96

static void info_displaySalvageDrops(float x, float y, float z, float sc, float wd, float ht)
{
    int i, j, count = eaiSize(&info.ppSalvageUseful);
    static ScrollBar sb = { WDW_INFO, 0 };
    float tx = x, ty = y;

    set_scissor(1);
    scissor_dims(x-PIX3*sc, y-PIX3*sc, wd+PIX3*sc*2, ht+PIX3*sc*4);

    for(i = 0; i < count; ++i)
    {
        const SalvageItem *salvage = info.ppSalvageDrops[info.ppSalvageUseful[i]];
        if(salvage)
        {
            AtlasTex *icon;
            float right_y = ty;

            icon = atlasLoadTexture(salvage->ui.pchIcon);
            if(icon)
            {
                F32 xsc = ((F32)SAL_ICON_WD)/icon->width;
                display_sprite( icon, tx+(SAL_COL_WD-SAL_ICON_WD)*sc/2.0, ty-sb.offset, z+10, sc*xsc, sc*xsc, CLR_WHITE );
            }

            right_y += 15*sc;
            prnt(tx+SAL_COL_WD*sc,right_y-sb.offset,z,sc,sc,textStd("SalvageDropUseful"));
            right_y += 10*sc;

            for(j = info.ppSalvageUsefulToIndices[i]; j < info.ppSalvageUsefulToIndices[i + 1]; j++)
            {
                if (info.ppSalvageUsefulTo[j]->level)
                    prnt(tx+SAL_COL_WD*sc,right_y-sb.offset,z,sc,sc,"%s (%i)",textStd(info.ppSalvageUsefulTo[j]->ui.pchDisplayName),info.ppSalvageUsefulTo[j]->level);
                else
                    prnt(tx+SAL_COL_WD*sc,right_y-sb.offset,z,sc,sc,"%s",textStd(info.ppSalvageUsefulTo[j]->ui.pchDisplayName));
                right_y += 10*sc;
            }

            ty += SAL_ICON_WD*sc + salvagePrintName(salvage, tx, ty-sb.offset+SAL_ICON_WD*sc, z, SAL_COL_WD*sc, sc);
            ty = MAX(ty,right_y);
            ty += PIX4*sc;
        }
    }

    count = eaiSize(&info.ppSalvageUseless);
    for(i = 0; i < count; ++i)
    {
        const SalvageItem *salvage = info.ppSalvageDrops[info.ppSalvageUseless[i]];
        AtlasTex *icon = atlasLoadTexture(salvage->ui.pchIcon);
        if(icon)
        {
            F32 xsc = ((F32)SAL_ICON_WD)/icon->width;
            display_sprite( icon, tx+(SAL_COL_WD-SAL_ICON_WD)*sc/2.0, ty-sb.offset, z+10, sc*xsc, sc*xsc, CLR_WHITE );
        }

        prnt(tx+SAL_COL_WD*sc,ty+20*sc-sb.offset,z,sc,sc,textStd("SalvageDropUseless"));

        ty += SAL_ICON_WD*sc + salvagePrintName(salvage, tx, ty-sb.offset+SAL_ICON_WD*sc, z, SAL_COL_WD*sc, sc);
        ty += PIX4*sc;
    }

    set_scissor(0);
    doScrollBar( &sb, ht, ty + 70 - y, PIX3*sc, (HEADER_HT+PIX3*2+R10)*sc, z+5, 0, 0 );
}

#define BUTTON_HEIGHT 20
#define BUTTON_MAX_WIDTH 200

int infoWindow()
{
    int i;
    float x, y, z, wd, ht, scale;
    int color, bcolor, mode = window_getMode( WDW_INFO );

    static AtlasTex * left;
    static AtlasTex * right;
    static int texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        left = atlasLoadTexture( "Headerbar_HorizDivider_L.tga" );
        right = atlasLoadTexture( "Headerbar_HorizDivider_R.tga" );
    }

    //if this was a power, but now that power is gone, leave the window
    if(info.pow && (!playerPtr() || 
        info.pcharPowIdxs[0] < 0 || info.pcharPowIdxs[1] < 0 || 
        eaSize(&playerPtr()->pchar->ppPowerSets) <=info.pcharPowIdxs[0] || 
        eaSize(&playerPtr()->pchar->ppPowerSets[info.pcharPowIdxs[0]]->ppPowers)<=info.pcharPowIdxs[1] ||
        playerPtr()->pchar->ppPowerSets[info.pcharPowIdxs[0]]->ppPowers[info.pcharPowIdxs[1]] != info.pow))
        info_clear();

    // if the name is gone there can be no info
    if( !info.name && window_getMode(WDW_INFO) != WINDOW_DOCKED )
        window_setMode( WDW_INFO, WINDOW_DOCKED );

    // clear it out while its closed
    if( !window_getDims( WDW_INFO, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
    {
        if( info.name && window_getMode(WDW_INFO) != WINDOW_GROWING )
            info_clear();

        return 0;
    }

    drawJoinedFrame2Panel( PIX3, R10, x, y, z, scale, wd, info.header_ht*scale, ht-info.header_ht*scale, color, bcolor, NULL, NULL );

    // draw the icon
    if( info.icon )
    {
        int longestSide = (info.icon->height > info.icon->width)? info.icon->height : info.icon->width;
        float iconscale = info.headshot?((ICON_MAX*scale)/longestSide):(ICON_MAX-15)*scale/longestSide;
        int centerY = y + (info.header_ht*scale - info.icon->height*iconscale)/2+1*scale;
        //y + (info.header_ht*scale - info.icon->height*iconscale)/2+(1-PIX2)*scale
        if( info.headshot )
            drawFlatFrame(PIX2, R4,  x + (ICON_WD*scale - info.icon->width*iconscale)/2-PIX2*scale, centerY-PIX2*scale, z, info.icon->width*iconscale+(PIX2*2)*scale, info.icon->height*iconscale+(PIX2*2)*scale, scale, 0x000000ff, 0xffffff0f);

        display_sprite( info.icon, x + (ICON_WD*scale - info.icon->width*iconscale)/2, centerY, z,
            iconscale, iconscale, CLR_WHITE );

    }
    else if ( info.spec )
        drawEnhancementOriginLevel(info.spec, info.level+1, info.numCombines, true, x + info.header_ht*scale/2, y + info.header_ht*scale/2, z, scale, scale, CLR_WHITE, false);
    else if ( info.room )
        drawRoomBoxes( info.room, x + PIX3*2*scale, y+PIX3*2*scale, z, (ICON_WD-PIX3*4)*scale, (info.header_ht-R10-PIX3)*scale );
    else if ( info.plot )
        drawPlotBox( info.plot, x + PIX3*2*scale, y+PIX3*2*scale, z, (ICON_WD-PIX3*4)*scale, (info.header_ht-R10-PIX3)*scale );
    else if ( info.pRec )
        uiRecipeDrawIcon( info.pRec, info.pRec->level, x + PIX3*scale, y+PIX3*scale, z, scale*0.9f, WDW_INFO, false, CLR_WHITE );

    if( mode == WINDOW_DISPLAYING ) // hide text until window is open
    {
        float width = wd - (2*PIX3 + R10 + ICON_WD)*scale;
        float textht;

        font( &title_12 );
        font_color( CLR_WHITE, CLR_WHITE );

        // Name
        DrawTitle(font_grp, x + ICON_WD*scale, y + FONT_HEIGHT*2*scale, z, width, info.name , scale);
        s_taShortLines.piScale = (int*)((int)(SMF_FONT_SCALE*scale));
        s_taClassSet.piScale = (int*)((int)(SMF_FONT_SCALE*scale));

        // shortInfo
        textht = 30*scale + smf_ParseAndDisplay( info.shortInfoSMF, info.shortInfo, x + ICON_WD*scale, y + 30*scale, z, width, 0, info.reparse, info.reparse, &s_taShortLines, NULL, 0, true); 
        // classSet
        textht += smf_ParseAndDisplay( info.classSetSMF, info.classSet, x + ICON_WD*scale, y + textht, z, width, 0, info.reparse, info.reparse, &s_taClassSet, NULL, 0, true); 

        info.header_ht = MAX( HEADER_HT, (textht + R10*scale)/scale);
        info.reparse = false;
    }

    // internal window text
    {
        int tabht = 15;
        int ty = (info.header_ht+4)*scale;
        int tht = ht-(info.header_ht+PIX3*3)*scale;
        bool bBadge = false;
        bool bAlignment = false;
        bool bSalvageDrops = false;
        bool bPowerInfo = false;
        s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*scale));
        smfview_SetBaseLocation( info.pview, x, y, z, wd, ht );

        if( info.numTabs > 1) // more than one tab, display tab bar and use current text
        {
            char * text = drawTabControl(info.tc, x+(PIX3+R10)*scale, y + info.header_ht*scale-PIX3*scale, z, wd-(PIX3+R10*2)*scale, tabht, scale, color, color, TabDirection_Horizontal);

            ty  += tabht*scale;
            tht -= tabht*scale;

            if(stricmp(text,"badge")==0)
                bBadge = true;
            else if(stricmp(text,"alignment")==0)
                bAlignment = true;
            else if(stricmp(text,"salvage_drops")==0)
                bSalvageDrops = true;
            else if(stricmp(text,"power_info")==0)
                bPowerInfo = true;
            else
                smfview_SetText(info.pview, text);
        }
        else if( info.numTabs ) // one tab, use its data, but don't draw tab bar
        {
            char * text = uiTabControlGetFirst(info.tc);

            if(stricmp(text,"badge")==0)
                bBadge = true;
            else if(stricmp(text,"alignment")==0)
                bAlignment = true;
            else if(stricmp(text,"salvage_drops")==0)
                bSalvageDrops = true;
            else if(stricmp(text,"Power_Info")==0)
                bPowerInfo = true;
            else
                smfview_SetText(info.pview, text);
        }
        else // no tabs, use stuffbuff
            smfview_SetText(info.pview, info.stuffbuff.buff);

        if(bBadge)
            info_displayBadges( x+(PIX3+4)*scale, y+ty, z, scale, wd-(PIX3*2+9)*scale, tht-R10*scale );
        else if(bAlignment)
            info_displayAlignment( info.ent, x+(PIX3+4)*scale, y+ty, z, scale, wd-(PIX3*2+9)*scale, tht-R10*scale, color );
        else if(bSalvageDrops)
            info_displaySalvageDrops(x+(PIX3+4)*scale, y+ty, z, scale, wd-(PIX3*2+9)*scale, tht-R10*scale);
        else if(bPowerInfo)
            displayPowerInfo(x+(10)*scale, y+ty, z, wd-(20)*scale, tht-1*scale, info.spec, info.pow, WDW_INFO, POWERINFO_ENHANCED, 0);
        else
        {
            smfview_SetLocation(info.pview, (PIX3+4)*scale, ty-PIX3*scale, 0);
            smfview_SetSize(info.pview, wd-(PIX3*2+9)*scale, tht+PIX3*scale);
            smfview_Draw(info.pview);
        }
    }

    // --------------------
    // draw the buttons
    {
        F32 y_btn = y + ht - (BUTTON_HEIGHT/2 + PIX3+4)*scale;
        F32 z_btn = z + 1;
        int num_btn = eaSize(&info.buttons);
        F32 wd_btn = MIN(((wd - PIX3*3*scale) / num_btn) - PIX3*scale,BUTTON_MAX_WIDTH);
        F32 x_base = x + (wd/2 - (num_btn*(wd_btn+PIX3*scale)/2))+wd_btn/2;
        for( i = 0; i < num_btn; ++i ) 
        {
            InfoButton *b = info.buttons[i];
            if( verify( b ) && (b->tab < 0 
                                || (info.tc && b->tab == uiTabControlGetSelectedIdx( info.tc ))))
            {
                int color = CLR_GREYED;
                int txtColor = CLR_WHITE;
                F32 x_btn = x_base + i * (wd_btn + PIX3*scale);
                if( D_MOUSEHIT == drawStdButton(x_btn,y_btn,z_btn,wd_btn,BUTTON_HEIGHT*scale,color,b->text,1.0,0))                
                {
                    if( b->cb )
                    {
                        b->cb(b);
                    }
                }
            }
        }
    }
    

    return 0;
}


#define INFO_BOX_WD     330
#define SC_SPEED        .3f
#define MOVE_SPEED      20
#define RAMP            .2f

static void convergePoint( float target, float * actual, float speed )
{
    if( *actual < target )
    {
        *actual += TIMESTEP*(speed+(ABS(*actual-target)*RAMP));
        if( *actual > target )
            *actual = target;
    }
    else if( *actual > target )
    {
        *actual -= TIMESTEP*(speed+(ABS(*actual-target)*RAMP));
        if( *actual < target )
            *actual = target;
    }
}

void info_PowManagement( EPowInfo mode, float screenScaleX, float screenScaleY )
{
    int z = 100, color = CLR_WHITE;
    float tx, ty, twd, tht, tsc;
    int textht = 0;
    float width;
    AtlasTex * left = atlasLoadTexture( "Headerbar_HorizDivider_L.tga" );
    AtlasTex * right = atlasLoadTexture( "Headerbar_HorizDivider_R.tga" );
    static float x = 0, y = 0, wd = 0, ht = 0, sc = 1.f;
    float screenScale = MIN(screenScaleX, screenScaleY);
    if(!info.pview)
    {
        info.pview = smfview_Create(0);
        smfview_SetAttribs(info.pview, &s_taDefaults);
    }

    twd = INFO_BOX_WD;
    tht = 90 + 2*FONT_HEIGHT + HEADER_HT;
    tsc = 1.f;

    if(mode == kPowInfo_Normal)
    {
        tx = 850;
        ty = 558;
    }
    else if(mode == kPowInfo_Combine)
    {
        tx = DEFAULT_SCRN_WD/2;
        ty = 650;
    }
    else if(mode == kPowInfo_Skill)
    {
        tx = FRAME_X(3)+SKILL_FRAME_WIDTH/2;
        twd = SKILL_FRAME_WIDTH;

        ty = 590;
        tht = 140;
        tsc = .71f;
    }
    else if (mode == kPowInfo_SpecLevel)
    {
        tx = 850;
        ty = 617;
    }
    else    //kPowInfo_Respec
    {
        tx = 822;
        ty = 600-7;
        tht += 20;
    }

    convergePoint( tx, &x, MOVE_SPEED );
    convergePoint( ty, &y, MOVE_SPEED  );
    convergePoint( twd, &wd, MOVE_SPEED  );
    convergePoint( tht, &ht, MOVE_SPEED  );
    convergePoint( tsc, &sc, MOVE_SPEED/10 );

    tx = x - wd/2;
    ty = y - ht/2;

    if( wd > 0 && ht > 0 )
    {
        drawMenuFrame( R12, (tx-4)*screenScaleX, ty*screenScaleY, z, (wd+4)*screenScaleX, ht*screenScaleY, CLR_WHITE, 0x000000ff, 0 );
    }

    if( !info.name )
        return;


    font( &title_14 );
    font_color( CLR_WHITE, CLR_WHITE );
    width = wd - (2*PIX3 + R10 + ICON_WD)*sc;

    DrawTitle(font_grp, (tx*screenScaleX) + (ICON_WD*sc*screenScale), (ty*screenScaleY) + FONT_HEIGHT*2*sc*screenScale, z, width*screenScaleX, info.name, sc );

    s_taShortLines.piScale = (int*)((int)(SMF_FONT_SCALE*1.f*screenScale));
    s_taClassSet.piScale = (int*)((int)(SMF_FONT_SCALE*1.f*screenScale));

    // shortInfo
    textht = 30 + smf_ParseAndDisplay( info.shortInfoSMF, info.shortInfo, (tx*screenScaleX) + (ICON_WD*screenScale), (ty + 30)*screenScaleY, z, width*screenScaleX, 0, info.reparse, info.reparse, &s_taShortLines, NULL, 0, true); 
 
    // classSet
    textht += smf_ParseAndDisplay( info.classSetSMF, info.classSet, (tx*screenScaleX) + (ICON_WD*screenScale), (ty + textht)*screenScaleY, z, width, 0, info.reparse, info.reparse, &s_taClassSet, NULL, 0, true); 

    info.header_ht = MAX( HEADER_HT, (textht + 5));
    info.reparse = false;

    if( info.icon )
        display_sprite( info.icon, (tx*screenScaleX) + (ICON_WD - info.icon->width)*screenScale/2, (ty*screenScaleY) + (info.header_ht - info.icon->height)*screenScale/2, z, screenScale, screenScale, color );
    else if ( info.spec )
        drawEnhancementOriginLevel( info.spec, info.level+1, info.numCombines, true, tx*screenScaleX + ICON_WD*screenScale/2, ty*screenScaleY + info.header_ht*screenScale/2, z, sc*screenScale, sc*screenScale, CLR_WHITE, false );


    display_sprite( left,  (tx + PIX3*sc)*screenScaleX, ty*screenScaleY + info.header_ht*screenScale*sc, z, sc*screenScale, sc*screenScale, CLR_WHITE );
    display_sprite( right, ((tx+wd)*screenScaleX) + (-PIX3*2 -right->width)*sc*screenScale, ty*screenScaleY + info.header_ht*sc*screenScale, z, sc*screenScale, sc*screenScale, CLR_WHITE);
    drawHorizontalLine( tx*screenScaleX + (PIX3 + left->width)*sc*screenScale, ty*screenScaleY+ (info.header_ht)*sc*screenScale, wd*screenScaleX - (PIX3*3 + left->width+right->width)*sc*screenScale, z, sc*screenScale, CLR_WHITE );

    s_taDefaults.piScale = (int *)((int)(sc*SMF_FONT_SCALE)); 
    smfview_SetBaseLocation( info.pview, tx*screenScaleX, ty*screenScaleY, z, wd*screenScaleX, ht*screenScaleY );
    smfview_SetLocation(info.pview, (PIX3+4)*sc*screenScale, (info.header_ht+4)*sc*screenScale, 0);
    smfview_SetSize(info.pview, wd*screenScaleX-(PIX3*2+9)*sc*screenScale, ht*screenScaleY-(info.header_ht+PIX3+R16)*sc*screenScale);
    smfview_SetText(info.pview, info.stuffbuff.buff);
    smfview_Draw(info.pview);
}
