

#include "UI/uiCostume.h"
#include "UI/uiScrollBar.h"

#include <utilitieslib/components/Earray.h>
#include "cmdparse/cmdcommon.h"
#include "sound/sound.h"
#include <utilitieslib/components/estring.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/file.h>

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "uiPCCRank.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "gameComm/trayCommon.h"

#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/textparser.h>
#include "gameComm/villainDef.h"
#include "storyarc/pnpcCommon.h"
#include "gameComm/npc.h"
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/error.h>
#include "entity/entclient.h"
#include "gameComm/itemselect.h"
#include "entity/entity.h"
#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "entity/CustomVillainGroup_Client.h"
#include "entity/costume_client.h"
#include "player/player.h"
#include "entity/costume.h"
#include "entity/classes.h"
#include "entity/powers.h"
#include "filter/profanity.h"

#include "uiNPCCostume.h"
#include "UI/uiCustomVillainGroupWindow.h"
#include "UI/uiMissionMakerScrollSet.h"
#include "uiScrollSelector.h"
#include "UI/uiComboBox.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uidialog.h"
#include "UI/uiClipper.h"
#include "UI/uiBox.h"
#include "UI/uiMissionMaker.h"
#include "uiHelpButton.h"
#include "uiMissionComment.h"
#include "UI/uiMissionReview.h"

#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"

#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/ttFontUtil.h"

#include "storyarc/missionMapCommon.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>

#include "formatter/smf_main.h"
#define SMFVIEW_PRIVATE
#include "uiSMFView.h"
#include "UI/uiPictureBrowser.h"

#include "AutoGen/playerCreatedStoryarcValidate_h_ast.h"
#include "AutoGen/uiMissionMakerScrollSet_h_ast.h"
#include "AutoGen/uiMissionMakerScrollSet_h_ast.c"
#include "uiMMMapViewer.h"
#include "entity/CustomVillainGroup.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiMissionMakerScrollSet.h"

MMRegion *pStoryArcTemplateRegion;
MMRegion **ppNewMissionTemplates;
MMRegion **ppDetailTemplates;

MMRegionButton ** ppAddButtonTemplate;

#define MMSIZE_FILE        175
#define MMSIZE_REGION    200
#define MMSIZE_ELEMENT      5
#define MMSIZE_MAX        5000
#define MM_IMAGE_SIZE 256
static char *mm_chosen_animation = 0;
static MMMapViewer mv = {0};
static int displayMap = 0;
static int mm_smallWindowWidth = 0;
static int s_architectIgnoreLocalValidation = 0;

static TextAttribs s_taMissionInfo =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
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
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};


void MMScrollSet_mapViewerInit(MMElementList * pList)
{
    MMElement * pElement;
    int i;
    char * currentMapName = mmmapviewer_getMapName(&mv);

    if( pList->ppMultiList && pList->current_list >= 0 )
        MMScrollSet_mapViewerInit(pList);

    if(!pList->ppElement)
        return;

    pElement = pList->ppElement[pList->current_element];
    if(!pElement->pchDisplayName || (currentMapName && !strcmp(pElement->pchDisplayName, currentMapName)))
        return;    //we either don't have a map name to display, or this isn't a new map.

    if(mv.maxHt <= 0 || mv.maxWd)
        mmmapviewer_init(&mv, 0,0,0, MINIMAP_BASE_DIMENSION*1.5,MINIMAP_BASE_DIMENSION, 0);

    if(pElement->mapImageName)
    {
        ArchitectMapHeader *header = 0;
        mmmapviewer_setMapName(&mv, pElement->pchDisplayName);
        header = missionMapHeader_Get( pElement->mapImageName );

        mmmapviewer_clearPictures(&mv);
        mmmapviewer_addPictures( &mv, header);
    }

    for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        MMScrollSet_mapViewerInit(pElement->ppElementList[i]);
}

//------------------------------------------------------------------------------------------------------------------------
// File Loading
//------------------------------------------------------------------------------------------------------------------------

int sortElements(const MMElement **a, const MMElement **b)
{
    if( !(*a)->pchText || !(*a)->pchDisplayName || stricmp( (*a)->pchText, "None" ) == 0 || stricmp( (*a)->pchDisplayName, "MMSameAsBoss" ) == 0 )
        return -1;
    if( !(*b)->pchText || !(*b)->pchDisplayName || stricmp( (*b)->pchText, "None" ) == 0 || stricmp( (*b)->pchDisplayName, "MMSameAsBoss" ) == 0 )
        return 1;

    if( stricmp( (*a)->pchDisplayName, textStd("AllCritterListText") ) == 0 )
        return -1;
    if( stricmp( (*b)->pchDisplayName, textStd("AllCritterListText") ) == 0 )
        return 1;

    if( stricmp( (*a)->pchDisplayName, textStd("MMNoVillains") ) == 0 )
        return -1;
    if( stricmp( (*b)->pchDisplayName, textStd("MMNoVillains") ) == 0 )
        return 1;

    //    if they are equal, don't move them,
    //    otherwise, treat normally
    return ( ( ( stricmp( textStd( (*a)->pchDisplayName ), textStd( (*b)->pchDisplayName ) ) ) >= 0 ) ? 1 : -1 );
}


static void addNoneElement( MMElementList *pList )
{
    MMElement * pElement = StructAllocRaw(sizeof(MMElement));
    pElement->pchText = StructAllocString("None");

    if( pList->pchReplaceNoneWith )
        pElement->pchDisplayName = StructAllocString(pList->pchReplaceNoneWith);
    else
        pElement->pchDisplayName = StructAllocString("NoneString");

    eaPush(&pList->ppElement, pElement);
}
static void addDoppelElement(MMElementList *pList)
{
    MMElement * pElement = StructAllocRaw(sizeof(MMElement));
    MMElementList * pDoppelList = StructAllocRaw(sizeof(MMElementList));
    MMElement * pDoppelElement = StructAllocRaw(sizeof(MMElement));
    pElement->pchText = StructAllocString(DOPPEL_GROUP_NAME);
    pElement->pchDisplayName = StructAllocString("DoppelString");

    pDoppelList->pchName = StructAllocString("DoppelList");
    pDoppelList->pchToolTip = StructAllocString("MMToolTipDoppelGangers");
    pDoppelList->pchField = StructAllocString("Entity");
    pDoppelList->eSpecialAction = kAction_BuildDoppelEntityList;

    pDoppelElement->pchText = StructAllocString(DOPPEL_NAME);
    pDoppelElement->pchDisplayName = StructAllocString("DoppelBossString");
    pDoppelElement->maxLevel = 54;
    pDoppelElement->minLevel = 1;

    eaPush(&pDoppelList->ppElement, pDoppelElement);
    eaPush(&pElement->ppElementList, pDoppelList);
    eaPush(&pList->ppElement, pElement);
}

static void updateNoneElements( MMElementList * pList, MMElementList *pListParent )
{
    int i, j;

    if( !pList->pchReplaceNoneWith )
    {
        if(!pListParent || !pListParent->pchReplaceNoneWith)
            return;
        pList->pchReplaceNoneWith = StructAllocString(pListParent->pchReplaceNoneWith);
    }

    for( i =eaSize(&pList->ppElement)-1; i>=0; i-- )
    {
        MMElement * pElement = pList->ppElement[i];
        if(stricmp("None", pElement->pchText) == 0 )
        {
            if( pElement->pchDisplayName )
                StructFreeString(pElement->pchDisplayName);
            pElement->pchDisplayName = StructAllocString( pList->pchReplaceNoneWith );
        }

        for( j = eaSize(&pElement->ppElementList)-1; j>=0; j-- )
            updateNoneElements( pElement->ppElementList[j], pList );
    }
}

static void addAnimList( MMElementList *pList )
{
    int i, j;
    static MMElementList **ppCache = 0;
    MMElementList * pTemp, **ppTempList=0;

    if( !ppCache )
    {
        pTemp = StructAllocRaw(sizeof(MMElementList));
        if( pList->pchReplaceNoneWith )
            pTemp->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
        addNoneElement( pTemp );
        for( i = 0; i < eaSize(&gMMAnimList.anim); i++ )
        {
            MMElement * pElement = StructAllocRaw(sizeof(MMElement));
            pElement->pchText = StructAllocString(gMMAnimList.anim[i]->pchName );
            pElement->pchDisplayName = StructAllocString(gMMAnimList.anim[i]->pchDisplayName?gMMAnimList.anim[i]->pchDisplayName:gMMAnimList.anim[i]->pchName);
            eaPush(&pTemp->ppElement, pElement);
        }
        eaQSort( pTemp->ppElement, sortElements );
        eaPush( &ppCache, pTemp );

        for( i = 0; i < eaSize(&gMMSeqAnimLists.ppLists); i++ )
        {
            MMSequencerAnimList *pSeqAnim = gMMSeqAnimLists.ppLists[i];
            pTemp = StructAllocRaw(sizeof(MMElementList));
            addNoneElement( pTemp );
            for( j = 0; j < eaSize(&pSeqAnim->ppValidAnimLists); j++ )
            {
                MMElement * pElement;
                MMAnim *pAnim;

                if(!stashFindPointer( gMMAnimList.st_Anim, pSeqAnim->ppValidAnimLists[j], &pAnim ))
                {
                    //Errorf( "Autogenerated an Invalid Animation: %s", pSeqAnim->ppValidAnimLists[j] );
                    continue;
                }
                pElement = StructAllocRaw(sizeof(MMElement));
                pElement->pchText = StructAllocString(pAnim->pchName);
                pElement->pchDisplayName = StructAllocString(pAnim->pchDisplayName?pAnim->pchDisplayName:pAnim->pchName);
                eaPush(&pTemp->ppElement, pElement);
            }
            eaQSort( pTemp->ppElement, sortElements );
            eaPush( &ppCache, pTemp );
        }
    }

    for( i = 0; i < eaSize(&ppCache); i++ )
    {
        pTemp = StructAllocRaw(sizeof(MMElementList));
        StructCopy(parse_MMElementList, pList, pTemp, 0 , 0 );
        for( j = 0; j < eaSize(&ppCache[i]->ppElement); j++ )
        {
            MMElement * pElement = StructAllocRaw(sizeof(MMElement));
            StructCopy( parse_MMElement, ppCache[i]->ppElement[j], pElement, 0, 0 );
            eaPush( &pTemp->ppElement, pElement );
        }
        eaPush( &ppTempList, pTemp );
    }

    for( i = 0; i < eaSize(&ppTempList); i++ )
        eaPush( &pList->ppMultiList, ppTempList[i] );
    eaDestroy(&ppTempList);
}

static void addPowerDescription( MMElement * pElement, const VillainDef * pDef )
{
    int i, j;
    char * pchPowerDesc;
    estrCreate(&pchPowerDesc);

    for( i = 0; i < eaSize(&pDef->powers); i++ )
    {
        const PowerNameRef * pRef = pDef->powers[i];

        if( pRef->power && pRef->power[0] == '*')
        {
            const BasePowerSet * pSetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, pRef->powerCategory, pRef->powerSet);

            if( pSetBase )
            {
                for( j = 0; j < eaSize(&pSetBase->ppPowers); j++ )
                    estrConcatf(&pchPowerDesc, "%s, ", textStd(pSetBase->ppPowers[j]->pchDisplayName) );
            }
        }
        else
        {
            const BasePower * pPowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pRef->powerCategory, pRef->powerSet, pRef->power);
            if(pPowBase)
                estrConcatf(&pchPowerDesc, "%s, ", textStd(pPowBase->pchDisplayName) );
        }
    }
    if( pchPowerDesc && strlen(pchPowerDesc) )
    {
        pchPowerDesc[strlen(pchPowerDesc)-2] = '\0';
        pElement->pchPowerDescription = StructAllocString(pchPowerDesc);
    }
    estrDestroy(&pchPowerDesc);    
}

static void addObjectList( MMElementList *pList, int contact )
{
    int i,j;

    addNoneElement( pList );
    for( i = 0; i < eaSize(&gMMObjectList.placement); i++ )
    {
        MMElement * pElement = StructAllocRaw(sizeof(MMElement));
        MMElementList *pSubList = StructAllocRaw(sizeof(MMElementList));

        pSubList->bRequired |= pList->bRequired;
        if( pList->pchReplaceNoneWith )
            pSubList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);

        if( contact )
        {
            pSubList->pchField = StructAllocString("Contact");
            pSubList->pchName = StructAllocString("NameString");
            pSubList->pchToolTip = StructAllocString("MMToolTipContactCategoryName");
        }
        else
        {
            pSubList->pchField = StructAllocString("Entity");
            pSubList->pchName = StructAllocString("MMCollectionObject" );
            pSubList->pchToolTip = StructAllocString("MMToolTipCollectionObject");
        }

        pElement->pchText = StructAllocString( gMMObjectList.placement[i]->pchName );
        pElement->pchDisplayName = StructAllocString( gMMObjectList.placement[i]->pchDisplayName );
        eaPush(&pList->ppElement, pElement);
        eaPush(&pElement->ppElementList,pSubList);

        addNoneElement( pSubList );
        for( j = 0; j < eaSize(&gMMObjectList.placement[i]->object); j++ )
        {
            MMElement * pEntElement = StructAllocRaw(sizeof(MMElement));
            MMObject *pObject = gMMObjectList.placement[i]->object[j];
            int index;

            npcFindByName(pObject->pchName, &index);
            eaiPushUnique(&pEntElement->npcIndexList, index);
            pEntElement->npcIndex = 0;

            pEntElement->pchText = StructAllocString(pObject->pchName);
            pEntElement->pchDisplayName = StructAllocString(pObject->pchDisplayName?pObject->pchDisplayName:pObject->pchName);
            eaPush(&pSubList->ppElement, pEntElement);
        }
        eaQSort( pSubList->ppElement, sortElements );
    }
    eaQSort( pList->ppElement, sortElements );
}

static void addContactList( MMElementList *pList )
{
    int i, j;

    addNoneElement( pList );
    for( i = 0; i < eaSize(&gMMContactCategoryList.ppContactCategories); i++ )
    {
        MMElement * pElement = StructAllocRaw(sizeof(MMElement));
        MMContactCategory *pContactCat = gMMContactCategoryList.ppContactCategories[i];
        MMElementList *pSubList = StructAllocRaw(sizeof(MMElementList));
        pSubList->pchField = StructAllocString("Contact");
        pSubList->pchName = StructAllocString("NameString");
        pSubList->pchToolTip = StructAllocString("MMToolTipContactCategoryName");

        pElement->pchDisplayName = StructAllocString( pContactCat->pchCategory );
        pElement->pchText = StructAllocString( pContactCat->pchCategory );
        eaPush(&pList->ppElement, pElement);
        eaPush(&pElement->ppElementList,pSubList);

        for( j = 0; j < eaSize(&pContactCat->ppContact); j++ )
        {
            MMContact *pContact = pContactCat->ppContact[j];
            if( pContact->pDef )
            {
                MMElement * pContactElement = StructAllocRaw(sizeof(MMElement));
                int index;
                const NPCDef * pNPC = npcFindByName(pContact->pDef->model, &index);
                eaiPushUnique(&pContactElement->npcIndexList, index);
                pContactElement->npcIndex = 0;
                pContactElement->pchDisplayName = StructAllocString( pContact->pDef->displayname );
                pContactElement->pchText = StructAllocString( pContact->pDef->model ); 
                eaPush(&pSubList->ppElement, pContactElement);
            }
        }
        eaQSort( pSubList->ppElement, sortElements );
    }
    eaQSort( pList->ppElement, sortElements );
}

static void MissionMapStatsToMapLimit( MissionMapStats * pStat, MapLimit * pLimit  )
{
    int i;
    if( pStat )
    {
        for( i = 0; i < eaSize(&pStat->places); i++ )
        {
            MissionPlaceStats * pPlace = pStat->places[i]; 
            int type, x;

            if( pPlace->place == MISSION_ANY )
                type = kMapLimit_All;
            else if ( pPlace->place == MISSION_FRONT )
                type = kMapLimit_Front;
            else if ( pPlace->place == MISSION_MIDDLE )
                type = kMapLimit_Middle;
            else if ( pPlace->place == MISSION_BACK )
                type = kMapLimit_Back;
            else
                continue;

            pLimit->regular_only[type] = pPlace->genericEncounterOnlyCount;
            for( x = 0; x < eaSize(&pPlace->customEncounters); x++ )
            {
                if( stricmp( "Around", pPlace->customEncounters[x]->type ) == 0)
                {
                    pLimit->around_only[type] = pPlace->customEncounters[x]->countOnly;
                    pLimit->around_or_regular[type] = pPlace->customEncounters[x]->countGrouped;
                }
                if( stricmp( "GiantMonster", pPlace->customEncounters[x]->type ) == 0)
                    pLimit->giant_monster = pPlace->customEncounters[x]->countOnly;
            }
            pLimit->wall[type] = pPlace->wallObjectiveCount;
            pLimit->floor[type] = pPlace->floorObjectiveCount;
        }
    }
}


static void addMapDesc( MMElement * pElement, MapLimit * pLimit, int floorCount )
{
    char * str;

    if( pLimit && 
        (pLimit->around_or_regular[kMapLimit_All] || pLimit->regular_only[kMapLimit_All]  || pLimit->around_only[kMapLimit_All]  || 
        pLimit->wall[kMapLimit_All] || pLimit->floor[kMapLimit_All])  )
    {
        estrCreate(&str);
        estrConcatf(&str, "<br>%s<br>", textStd("MMMissionDesc") );
        if( pLimit->around_or_regular[kMapLimit_All] )
        {
            estrConcatf(&str, textStd("MMAnyEncounters", pLimit->around_or_regular[kMapLimit_All]));
            if(  pLimit->around_or_regular[kMapLimit_Front] ||  pLimit->around_or_regular[kMapLimit_Middle] ||  pLimit->around_or_regular[kMapLimit_Back] )
            {    
                estrConcatStaticCharArray(&str, "  (");
                if( pLimit->around_or_regular[kMapLimit_Front] )
                {
                    estrConcatf( &str, textStd("MMFront", pLimit->around_or_regular[kMapLimit_Front] ) );
                    if( pLimit->around_or_regular[kMapLimit_Middle] )
                        estrConcatf( &str, ", %s", textStd("MMMiddle", pLimit->around_or_regular[kMapLimit_Middle] ) );
                    if( pLimit->around_or_regular[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->around_or_regular[kMapLimit_Back] ) );
                }
                else if( pLimit->around_or_regular[kMapLimit_Middle] )
                {
                    estrConcatf( &str, textStd("MMMiddle", pLimit->around_or_regular[kMapLimit_Middle] ) );
                    if( pLimit->around_or_regular[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->around_or_regular[kMapLimit_Back] ) );        
                }
                else
                    estrConcatf( &str, textStd("MMBack", pLimit->around_or_regular[kMapLimit_Back] ) );
                estrConcatChar(&str, ')');
            }
            estrConcatStaticCharArray(&str, "<br>");
        }
        if( pLimit->regular_only[kMapLimit_All] )
        {
            estrConcatf(&str, textStd("MMRegularEncounters", pLimit->regular_only[kMapLimit_All]  ));
            if( pLimit->regular_only[kMapLimit_Front] ||  pLimit->regular_only[kMapLimit_Middle] ||  pLimit->regular_only[kMapLimit_Back] )
            {    
                estrConcatStaticCharArray(&str, "  (");
                if( pLimit->regular_only[kMapLimit_Front] )
                {
                    estrConcatf( &str, textStd("MMFront", pLimit->regular_only[kMapLimit_Front] ) );
                    if( pLimit->regular_only[kMapLimit_Middle] )
                        estrConcatf( &str, ", %s", textStd("MMMiddle", pLimit->regular_only[kMapLimit_Middle] ) );
                    if( pLimit->regular_only[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->regular_only[kMapLimit_Back] ) );
                }
                else if( pLimit->regular_only[kMapLimit_Middle] )
                {
                    estrConcatf( &str, textStd("MMMiddle", pLimit->regular_only[kMapLimit_Middle] ) );
                    if( pLimit->regular_only[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->regular_only[kMapLimit_Back] ) );        
                }
                else
                    estrConcatf( &str, textStd("MMBack", pLimit->regular_only[kMapLimit_Back] ) );
                estrConcatChar(&str, ')');
            }
            estrConcatStaticCharArray(&str, "<br>");
        }
        if( pLimit->around_only[kMapLimit_All] )
        {
            estrConcatf(&str, textStd("MMAroundEncounters", pLimit->around_only[kMapLimit_All] ));
            if( pLimit->around_only[kMapLimit_Front] ||  pLimit->around_only[kMapLimit_Middle] ||  pLimit->around_only[kMapLimit_Back] )
            {    
                estrConcatStaticCharArray(&str, "  (");
                if( pLimit->around_only[kMapLimit_Front] )
                {
                    estrConcatf( &str, textStd("MMFront", pLimit->around_only[kMapLimit_Front] ) );
                    if( pLimit->around_only[kMapLimit_Middle] )
                        estrConcatf( &str, ", %s", textStd("MMMiddle", pLimit->around_only[kMapLimit_Middle] ) );
                    if( pLimit->around_only[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->around_only[kMapLimit_Back] ) );
                }
                else if( pLimit->around_only[kMapLimit_Middle] )
                {
                    estrConcatf( &str, textStd("MMMiddle", pLimit->around_only[kMapLimit_Middle] ) );
                    if( pLimit->around_only[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->around_only[kMapLimit_Back] ) );        
                }
                else
                    estrConcatf( &str, textStd("MMBack", pLimit->around_only[kMapLimit_Back] ) );
                estrConcatChar( &str, ')');
            }
            estrConcatStaticCharArray(&str, "<br>");
        }
        if( pLimit->wall[kMapLimit_All] )
        {
            estrConcatf(&str, textStd("MMWallLocations", pLimit->wall[kMapLimit_All] ));
            if( pLimit->wall[kMapLimit_Front] ||  pLimit->wall[kMapLimit_Middle] ||  pLimit->wall[kMapLimit_Back] )
            {    
                estrConcatStaticCharArray(&str, "  (");
                if( pLimit->wall[kMapLimit_Front] )
                {
                    estrConcatf( &str, textStd("MMFront", pLimit->wall[kMapLimit_Front] ) );
                    if( pLimit->wall[kMapLimit_Middle] )
                        estrConcatf( &str, ", %s", textStd("MMMiddle", pLimit->wall[kMapLimit_Middle] ) );
                    if( pLimit->wall[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->wall[kMapLimit_Back] ) );
                }
                else if( pLimit->wall[kMapLimit_Middle] )
                {
                    estrConcatf( &str, textStd("MMMiddle", pLimit->wall[kMapLimit_Middle] ) );
                    if( pLimit->wall[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->wall[kMapLimit_Back] ) );        
                }
                else
                    estrConcatf( &str, textStd("MMBack", pLimit->wall[kMapLimit_Back] ) );
                estrConcatChar(&str, ')');
            }
            estrConcatStaticCharArray(&str, "<br>");
        }
        if( pLimit->floor[kMapLimit_All] )
        {
            estrConcatf(&str, textStd("MMFloorLocations", pLimit->floor[kMapLimit_All] ));
            if( pLimit->floor[kMapLimit_Front] ||  pLimit->floor[kMapLimit_Middle] ||  pLimit->floor[kMapLimit_Back] )
            {    
                estrConcatStaticCharArray(&str, "  (");
                if( pLimit->floor[kMapLimit_Front] )
                {
                    estrConcatf( &str, textStd("MMFront", pLimit->floor[kMapLimit_Front] ) );
                    if( pLimit->floor[kMapLimit_Middle] )
                        estrConcatf( &str, ", %s", textStd("MMMiddle", pLimit->floor[kMapLimit_Middle] ) );
                    if( pLimit->floor[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->floor[kMapLimit_Back] ) );
                }
                else if( pLimit->floor[kMapLimit_Middle] )
                {
                    estrConcatf( &str, textStd("MMMiddle", pLimit->floor[kMapLimit_Middle] ) );
                    if( pLimit->floor[kMapLimit_Back] )
                        estrConcatf( &str, ", %s", textStd("MMBack", pLimit->floor[kMapLimit_Back] ) );        
                }
                else
                    estrConcatf( &str, textStd("MMBack", pLimit->floor[kMapLimit_Back] ) );
                estrConcatChar(&str, ')');
            }
            estrConcatStaticCharArray(&str, "<br>");
        }
        if( pLimit->giant_monster )
            estrConcatf(&str, textStd("MMGiantMonster", floorCount));
        if( floorCount > 1 )
            estrConcatf(&str, textStd("MMMapSections", floorCount));

        pElement->pchDescription = StructAllocString(str);
        estrDestroy(&str);
    }
}


static void addLevelList(MMElementList *pList)
{
    int i;

    MMElement * pElement = StructAllocRaw(sizeof(MMElement));

    pElement->pchText = StructAllocString("0");
    if( pList->pchReplaceNoneWith )
        pElement->pchDisplayName = StructAllocString(pList->pchReplaceNoneWith);
    else
        pElement->pchDisplayName = StructAllocString("NoneString");
    eaPush(&pList->ppElement, pElement);

    for( i = 1; i <= 54; i++ )
    {
        char buff[5];
        sprintf( buff, "%i", i );
        pElement = StructAllocRaw(sizeof(MMElement));
        pElement->pchText = StructAllocString(buff);
        pElement->pchDisplayName = StructAllocString(buff);
        eaPush(&pList->ppElement, pElement);
    }
}


static void addMapList( MMElementList *pList)
{
    int i, j, k;

    addNoneElement( pList );

    // First add giant list of generic maps
    for( i = 0; i < eaSize(&g_missionmaplist.missionmapsets); i++ )
    {
        const MissionMapSet *pMapSet = g_missionmaplist.missionmapsets[i];
        char * displayName = playerCreatedStoryarc_getMapSetDisplayName( StaticDefineIntRevLookup(ParseMapSetEnum,pMapSet->mapset) );
        MMElement * pElement;
        MMElementList *pTimeList;

        if( !displayName )
            continue;

        pElement = StructAllocRaw(sizeof(MMElement));
        pTimeList = StructAllocRaw(sizeof(MMElementList));
        pTimeList->bRequired |= pList->bRequired;
        if( pList->pchReplaceNoneWith )
            pTimeList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
        pTimeList->pchField = StructAllocString( "MapLength" );
        pTimeList->pchName = StructAllocString( "MMMapLength" );
        pTimeList->pchToolTip = StructAllocString( "MMToolTipMapLength" );
        pTimeList->eSpecialAction =  kAction_EnumMapLength;
        pElement->pchText = StructAllocString( StaticDefineIntRevLookup(ParseMapSetEnum,pMapSet->mapset) );
        pElement->pchDisplayName = StructAllocString( displayName );
        eaPush(&pList->ppElement, pElement);
        eaPush(&pElement->ppElementList,pTimeList);

        addNoneElement( pTimeList );
        for( j = 0; j < eaSize(&pMapSet->missionmaptimes); j++ )
        {
            const MissionMapTime *pTime = pMapSet->missionmaptimes[j];
            MMElement * pTimeElement = StructAllocRaw(sizeof(MMElement));
            MMElementList *pMapList = StructAllocRaw(sizeof(MMElementList));

            pMapList->bRequired |= pList->bRequired;
            if( pList->pchReplaceNoneWith )
                pMapList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
            pMapList->pchField = StructAllocString( "MapName" );
            pMapList->pchName = StructAllocString( "MMMapName" );
            pMapList->pchToolTip = StructAllocString( "MMToolTipMapMap" );

            pTimeElement->pchText = StructAllocString( MapLengthEnum[(pTime->maptime/15)].key );
            pTimeElement->pchDisplayName = StructAllocString(textStd(MapLengthEnum[(pTime->maptime/15)].display));
            eaPush(&pTimeList->ppElement, pTimeElement);
            eaPush(&pTimeElement->ppElementList,pMapList);

            // first add "random"
            {
                MMElement * pMapElement = StructAllocRaw(sizeof(MMElement));

                pMapElement->pchDisplayName = StructAllocString( "RandomString" );
                pMapElement->pchText = StructAllocString( "Random" );
                eaPush(&pMapList->ppElement, pMapElement);
                memcpy( &pMapElement->maplimit, &pTime->randomLimit, sizeof(MapLimit) );

                addMapDesc( pMapElement, &pMapElement->maplimit, 0 );
            }

            for( k = 0; k < eaSize(&pTime->maps); k++ )
            {
                MissionMap * pMap = pTime->maps[k];
                MMElement * pMapElement = StructAllocRaw(sizeof(MMElement));
                MissionMapStats *pStat = missionMapStat_Get( pMap->mapfile );

                if( pStat )
                {
                    MissionMapStatsToMapLimit( pStat, &pMapElement->maplimit );
                    addMapDesc( pMapElement, &pMapElement->maplimit, pStat->floorCount );

                    {
                        //create the mmmap name
                        //char * nameStart;

                        estrCreate(&pMapElement->mapImageName);
                        estrPrintCharString(&pMapElement->mapImageName, pMap->mapfile);
                    }
                }

                pMapElement->pchDisplayName = StructAllocString( textStd("MMMapNameIndividual", displayName, k+1) );
                pMapElement->pchText = StructAllocString( pMap->mapfile );
                eaPush(&pMapList->ppElement, pMapElement);
            }
        }
    }

    // Now add the unique maps
    {
        MMElement * pElement = StructAllocRaw(sizeof(MMElement));
        MMElementList *pUniqueCatList = StructAllocRaw(sizeof(MMElementList));

        pUniqueCatList->bRequired |= pList->bRequired;
        if( pList->pchReplaceNoneWith )
            pUniqueCatList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
        pUniqueCatList->pchField = StructAllocString( "UniqueMapCategory" );
        pUniqueCatList->pchName = StructAllocString( "CategoryString" );
        pElement->pchText = StructAllocString( "Unique" );
        pElement->pchDisplayName = StructAllocString( "UniqueMapString" );

        eaPush(&pList->ppElement, pElement);
        eaPush(&pElement->ppElementList,pUniqueCatList);

        addNoneElement( pUniqueCatList );
        for( i = 0; i < eaSize(&gMMUniqueMapCategoryList.mapcategorylist); i++ )
        {
            MMElement * pUniqueCatElement = StructAllocRaw(sizeof(MMElement));
            MMElementList *pMapList = StructAllocRaw(sizeof(MMElementList));
            MMUniqueMapCategory * pCat = gMMUniqueMapCategoryList.mapcategorylist[i];

            pMapList->bRequired |= pList->bRequired;
            if( pList->pchReplaceNoneWith )
                pMapList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
            pMapList->pchField = StructAllocString( "MapName" );
            pMapList->pchName = StructAllocString( "MMMapName" );

            pUniqueCatElement->pchText = StructAllocString( textStd(pCat->pchName) );
            pUniqueCatElement->pchDisplayName = StructAllocString( textStd(pCat->pchName)  );
            eaPush(&pUniqueCatList->ppElement, pUniqueCatElement);
            eaPush(&pUniqueCatElement->ppElementList,pMapList);

            for( j = 0; j < eaSize(&pCat->maplist); j++ )
            {
                MMUniqueMap * pMap = pCat->maplist[j];
                MMElement * pMapElement = StructAllocRaw(sizeof(MMElement));
                MissionMapStats *pStat = missionMapStat_Get( pMap->pchMapFile );

                if( pStat )
                {
                    MissionMapStatsToMapLimit( pStat, &pMapElement->maplimit );
                    addMapDesc( pMapElement, &pMapElement->maplimit, pStat->floorCount );

                    {
                        estrCreate(&pMapElement->mapImageName);
                        estrPrintCharString(&pMapElement->mapImageName, pMap->pchMapFile);
                    }
                }

                pMapElement->pchDisplayName = StructAllocString( pMap->pchName );
                pMapElement->pchText = StructAllocString( pMap->pchMapFile );
                eaPush(&pMapList->ppElement, pMapElement);
            }
            eaQSort( pMapList->ppElement, sortElements );
        }
    }
    eaQSort( pList->ppElement, sortElements );
}

typedef enum VGListType
{
    kVGList_VillainGroup,
    kVGList_Entity,
    kVGList_Boss,
    kVGList_Contact,
    kVGList_SupportEntity,
    kVGList_GiantMonster,
    kVGList_Count,
}VGListType;

static void addVillainDef( MMVillainGroup * pGroup, MMElement * pElement, VGListType type, MMElementList *pListParent )
{
    int i, j;
    MMElementList *pList = StructAllocRaw(sizeof(MMElementList));

    if( pListParent->pchReplaceNoneWith )
        pList->pchReplaceNoneWith = StructAllocString(pListParent->pchReplaceNoneWith);

    if( type == kVGList_Contact )
    {
        pList->pchField = StructAllocString("Contact");
        pList->pchName = StructAllocString("MMName");
        pList->pchToolTip = StructAllocString("MMToolTipContactCategoryName");
    }
    else if( type == kVGList_SupportEntity )
    {
        pList->pchField = StructAllocString("SupportEntity");
        pList->pchName = StructAllocString("MMSupportEntity");
        pList->pchToolTip = StructAllocString("MMToolTipMinionOverride");
        addNoneElement( pList );
    }
    else
    {
        if (type != kVGList_GiantMonster) // only works for regular bosses
        {
            MMElement * pRandomElement = StructAllocRaw(sizeof(MMElement));
            pRandomElement->pchDisplayName = StructAllocString( "RandomString" );
            pRandomElement->pchText = StructAllocString( "Random" );
            pRandomElement->minLevel = pGroup->minLevel;
            pRandomElement->maxLevel = pGroup->maxLevel; 
            eaPush(&pList->ppElement, pRandomElement);
        }

        pList->pchField = StructAllocString( "Entity" );
        pList->pchName = StructAllocString( "MMCharacter" );
        if( type == kVGList_Boss )
            pList->pchToolTip = StructAllocString("MMToolTipBossCharacter");
        else if (type == kVGList_GiantMonster)
            pList->pchToolTip = StructAllocString("MMToolTipGiantMonster");        
        else
            //if( type == kVGList_Entity)
            pList->pchToolTip = StructAllocString("MMToolTipEscortRescue");

    }
    pList->bRequired |= pListParent->bRequired;


    for( i = 0; i < eaSize(&villainDefList.villainDefs); i++ )
    {
        const VillainDef *pDef = villainDefList.villainDefs[i];

        if( !playerCreatedStoryarc_isValidVillainDefName( pDef->name, 0 ) )
            continue;
        if( pDef->group != pGroup->vg )
            continue;

        if( type == kVGList_GiantMonster && pDef->rank != VR_BIGMONSTER)
            continue;
        if( type != kVGList_GiantMonster && pDef->rank == VR_BIGMONSTER)
            continue;

        if( type == kVGList_Entity && (pDef->rank == VR_SNIPER || playerCreatedStoryarc_isCantMoveVillainDefName( pDef->name )) )
            continue;
 
        if( pDef->rank == VR_PET && type==kVGList_Boss )
        {
            const CharacterClass *pclass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);

            if( pclass && pclass->pchName  )
            {
                if( !strstriConst(pclass->pchName, "Arch") && !strstriConst(pclass->pchName, "Monster") && !strstriConst( pclass->pchName, "Boss") )
                    continue;
            }
        }

        if( pDef->group == pGroup->vg && ( (type!=kVGList_Boss) || pDef->rank >= VR_SNIPER) )
        {
            int minlevel = 9999, maxlevel = 0;
            MMElement * pNewElement=0;
            char tmpName[256];
            const char * pchRank = "";
            const char **aliases=0;
            char *estrAlias;
            int index = 0;
            const NPCDef *npcDef;
            int npcIndex;

            for(j = 0; j < eaSize(&pDef->levels); j++)
            {
                int k;        
                for(k = 0; k < eaSize(&pDef->levels[j]->displayNames); k++ )
                {
                    int x, found = 0;
                    for( x = 0; x < eaSize(&aliases); x++ )
                    {
                        if( stricmp(textStd(aliases[x]), textStd(pDef->levels[j]->displayNames[k]) ) == 0 )
                        {    
                            found = 1;
                            break;
                        }
                    }
                    if( !found )
                        eaPushConst(&aliases, pDef->levels[j]->displayNames[k] );
                }

                if( villainDefList.villainDefs[i]->levels[j]->level < minlevel )
                    minlevel = villainDefList.villainDefs[i]->levels[j]->level;

                if( villainDefList.villainDefs[i]->levels[j]->level > maxlevel )
                    maxlevel = villainDefList.villainDefs[i]->levels[j]->level;
            }

            if( maxlevel == 0 || minlevel == 9999 )
            {    
                eaDestroyConst(&aliases);
                continue;
            }

            if( pDef->characterClassName)
            {
                const CharacterClass *pclass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
                if( pclass )
                    pchRank = pclass->pchDisplayName;
            }
            else
                pchRank = StaticDefineIntRevLookup(ParseVillainRankEnum, pDef->rank);

            pNewElement = StructAllocRaw(sizeof(MMElement));
            npcIndex = playerCreatedStoryArc_NPCIdByCostumeIdx(villainDefList.villainDefs[i]->name, -1, &pNewElement->npcIndexList );
            npcDef = npcFindByIdx(npcIndex);
            pNewElement->pchSeq =  StructAllocString(npcDef->costumes[0]->appearance.entTypeFile);

            if( type == kVGList_Contact && !eaiSize(&pNewElement->npcIndexList) )
                continue;

            pNewElement->npcIndex = 0;
            pNewElement->pchText = StructAllocString(pDef->name);
            pNewElement->minLevel = minlevel;
            pNewElement->maxLevel = maxlevel;

            if( type == kVGList_Contact )
                sprintf( tmpName, "%s (%s)", textStd(pDef->levels[0]->displayNames[0]), textStd(pchRank) );
            else
                sprintf( tmpName, "%s (%s %i-%i)", textStd(pDef->levels[0]->displayNames[0]), textStd(pchRank), minlevel, maxlevel );
            pNewElement->pchDisplayName = StructAllocString(tmpName);

            if(villainDefList.villainDefs[i]->description)
                pNewElement->pchDescription = StructAllocString(villainDefList.villainDefs[i]->description);
            addPowerDescription( pNewElement, villainDefList.villainDefs[i] );

            estrCreate(&estrAlias);
            for( j = eaSize(&aliases)-1; j>0; j--)
            {
                if( j == 1 )
                    estrConcatCharString(&estrAlias, textStd(aliases[j]));
                else
                    estrConcatf(&estrAlias, "%s, ", textStd(aliases[j]) );
            }
            eaDestroyConst(&aliases);
            if( strlen(estrAlias) )
                pNewElement->pchAlias = StructAllocString(estrAlias);
            estrDestroy(&estrAlias);

            eaPush(&pList->ppElement, pNewElement);
        }
    }

    for( i = eaSize(&pList->ppElement)-1; i>=0; i-- )
    {
        if( pList->ppElement[i]->minLevel > pGroup->maxLevel ||    pList->ppElement[i]->maxLevel < pGroup->minLevel)
        {
            MMElement * pDel = eaRemove(&pList->ppElement, i);
            StructDestroy(parse_MMElement, pDel);
        }
    }


    if(eaSize(&pList->ppElement)>1 || ((type == kVGList_Contact || type == kVGList_GiantMonster) && eaSize(&pList->ppElement)>0)) // first element is none or random
    {
        eaPush(&pElement->ppElementList, pList);
        eaQSort( pList->ppElement, sortElements );
    }
    else
        StructDestroy(parse_MMElementList, pList);

}

static void addDestuctibleObjectList( MMElementList *pList, int contact )
{
    int i, j;
    char tmp[256];
    addNoneElement( pList );
    for( i = 0; i < eaSize(&gMMDestructableObjectList.object); i++ )
    {
        const VillainDef  *pVillainDef = villainFindByName(gMMDestructableObjectList.object[i]->pchName);
        const char * prevName = "";
        int lastLevel = 0;
        MMElement * pLastElement=0;

        for(j = 0; pVillainDef && j < eaSize(&pVillainDef->levels); j++)
        {
            if( stricmp(prevName, pVillainDef->levels[j]->displayNames[0] ) != 0 )
            {
                MMElement * pElement = StructAllocRaw(sizeof(MMElement));
                pElement->pchText = StructAllocString(pVillainDef->name );
                pElement->pchDisplayName = StructAllocString(pVillainDef->levels[j]->displayNames[0]);

                if( pLastElement )
                {
                    pLastElement->maxLevel = lastLevel;

                    if( contact )
                        sprintf( tmp, "%s", textStd(pLastElement->pchDisplayName) );
                    else
                        sprintf( tmp, "%s (%i-%i)", textStd(pLastElement->pchDisplayName), pLastElement->minLevel, pLastElement->maxLevel );
                    pLastElement->minLevel = pLastElement->maxLevel = 0;
                    StructFreeString(pLastElement->pchDisplayName);
                    pElement->pchDisplayName = StructAllocString(tmp);
                }
                pElement->minLevel = pVillainDef->levels[j]->level;

                if(pVillainDef->description)
                    pElement->pchDescription = StructAllocString(pVillainDef->description);

                addPowerDescription(pElement, pVillainDef);

                playerCreatedStoryArc_NPCIdByCostumeIdx(pVillainDef->name, -1, &pElement->npcIndexList );

                pElement->npcIndex = 0;
                eaPush(&pList->ppElement, pElement);
                pLastElement = pElement;
                lastLevel = pVillainDef->levels[j]->level;
            }
            prevName = pVillainDef->levels[j]->displayNames[0];
        }

        if( pLastElement )
        {
            pLastElement->maxLevel = pVillainDef->levels[j-1]->level;
            sprintf( tmp, "%s (%i-%i)", textStd(pLastElement->pchDisplayName), pLastElement->minLevel, pLastElement->maxLevel );
            pLastElement->minLevel = pLastElement->maxLevel = 0;
            StructFreeString(pLastElement->pchDisplayName);
            pLastElement->pchDisplayName = StructAllocString(tmp);
        }
    }

    eaQSort( pList->ppElement, sortElements );
}

static MMElementList **s_ppCustomCritterLists;
extern CustomVG **g_CustomVillainGroups;
static void addCustomCritterList( MMElementList *pList, int includeContacts )
{
    int i, j;
    MMElementList *noneElementList;
    eaPush( &s_ppCustomCritterLists, pList );
    addNoneElement( pList );
    noneElementList = StructAllocRaw(sizeof(MMElementList));
    noneElementList->pchField = StructAllocString( includeContacts ? "CustomCritterAndContactsIdx" : "CustomCritterIdx");
    eaPush(&pList->ppElement[0]->ppElementList, noneElementList);
    for( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
    {
        if (eaSize(&(g_CustomVillainGroups[i]->customVillainList)) > 0)
        {
            MMElement * pElement = StructAllocRaw(sizeof(MMElement));
            MMElementList *pCritterList = StructAllocRaw(sizeof(MMElementList));

            pCritterList->pchName = StructAllocString("MMCritterName");
            pCritterList->bRequired |= pList->bRequired;
            if( pList->pchReplaceNoneWith )
                pCritterList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
            if (includeContacts) 
            {
                pCritterList->pchField = StructAllocString("CustomCritterAndContactsIdx");
            }
            else
            {
                pCritterList->pchField = StructAllocString("CustomCritterIdx");
            }
            pCritterList->pchToolTip = StructAllocString("MMToolTipBossCustomCharacter");

            pElement->pchText = StructAllocString(g_CustomVillainGroups[i]->displayName);
            pElement->pchDisplayName = StructAllocString(g_CustomVillainGroups[i]->displayName);
            pElement->minLevel = 1;
            pElement->maxLevel = 54;
            eaPush(&pList->ppElement, pElement);
            eaPush(&pElement->ppElementList,pCritterList);

            addNoneElement( pCritterList );
            for( j = 0; j < eaSize(&g_CustomVillainGroups[i]->customVillainList); j++ )
            {
                MMElement * pCritterElement = StructAllocRaw(sizeof(MMElement));
                PCC_Critter *pCritter = g_CustomVillainGroups[i]->customVillainList[j];

                pCritterElement->pchText = StructAllocString(pCritter->name);
                pCritterElement->pchDisplayName = StructAllocString(pCritter->name);
                if (pCritter->description)
                {
                    pCritterElement->pchDescription = StructAllocString(smf_DecodeAllCharactersGet(pCritter->description)) ;
                }
                pCritterElement->pCritter = clone_PCC_Critter(pCritter);
                pCritterElement->minLevel = 1;
                pCritterElement->maxLevel = 54;
                eaPush(&pCritterList->ppElement, pCritterElement);
            }

            eaQSort( pCritterList->ppElement, sortElements );
        }
    }

    eaQSort( pList->ppElement, sortElements );
}
static void addCVGList( MMElementList* pList )
{
    int i;
    addNoneElement( pList );
    for( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
    {
        MMElement * pElement = StructAllocRaw(sizeof(MMElement));
        pElement->pchText = StructAllocString(g_CustomVillainGroups[i]->displayName);
        pElement->pchDisplayName = StructAllocString(g_CustomVillainGroups[i]->displayName);
        CVG_getLevelsFromUncompressedVG(g_CustomVillainGroups[i], &pElement->minLevel, &pElement->maxLevel);
        eaPush(&pList->ppElement, pElement);
    }

    eaQSort( pList->ppElement, sortElements    );
}
static void freeListNonStructFields( MMElementList *pList );



// probably need to save state
// this is slow as hell
void updateCustomCrittersElementList( MMElementList *pList )
{
    int i, j, k,n;
    int createdCritterString = 0;
    char *originalCritterString = NULL;
    int createdString = 0;
    char *originalCurrentElementString = NULL;
    if( !pList->eSpecialAction || 
        (pList->eSpecialAction != kAction_BuildCustomCritterList &&
         pList->eSpecialAction != kAction_BuildCustomCritterAndContactList &&
         pList->eSpecialAction != kAction_BuildCustomVillainGroupList))
    {
        for( i = 0; i < eaSize(&pList->ppElement); i++ )
        {
            for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
                updateCustomCrittersElementList(pList->ppElement[i]->ppElementList[j]);
        }
        return;
    }
    if ( (eaSize(&pList->ppElement) > 0 ) && EAINRANGE(pList->current_element, pList->ppElement) &&
        pList->ppElement[pList->current_element]->pchDisplayName )
    {
        createdString = 1;
        estrCreate(&originalCurrentElementString);
        estrConcatCharString(&originalCurrentElementString, pList->ppElement[pList->current_element]->pchDisplayName );
        if ( pList->eSpecialAction == kAction_BuildCustomCritterAndContactList || pList->eSpecialAction == kAction_BuildCustomCritterList )
        {
            MMElementList *pCritterList;
            pCritterList = pList->ppElement[pList->current_element]->ppElementList[0]; 
            if ( (eaSize(&pCritterList->ppElement) > 0 ) && EAINRANGE(pCritterList->current_element, pCritterList->ppElement) &&
                pCritterList->ppElement[pCritterList->current_element]->pCritter && pCritterList->ppElement[pCritterList->current_element]->pCritter->referenceFile )
            {
                createdCritterString = 1;
                estrCreate(&originalCritterString);
                estrConcatCharString(&originalCritterString, pCritterList->ppElement[pCritterList->current_element]->pCritter->referenceFile );
            }
        }
    }
    //    handles removal of empty lists
    for( i = (eaSize(&pList->ppElement)-1); i >= 1; i--)
    {
        //    loop through all the group lists
        for (j = 0; j < eaSize(&g_CustomVillainGroups); j++)
        {
            if (stricmp( g_CustomVillainGroups[j]->displayName, pList->ppElement[i]->pchText ) == 0)
            {
                break;
            }
        }

        //    if the group doesn't exist anymore, delete it
        if (j >= eaSize(&g_CustomVillainGroups))
        {    
            //    Destroy all the critters in this list
            //    Including the none element
            if (pList->ppElement[i]->ppElementList)
            {
                for (k = (eaSize(&pList->ppElement[i]->ppElementList[0]->ppElement)-1); k >= 0; k--)
                {
                    MMElement *pCritterElement = pList->ppElement[i]->ppElementList[0]->ppElement[k];
                    eaRemove(&pList->ppElement[i]->ppElementList[0]->ppElement, k);
                    StructFreeString(pCritterElement->pchText);
                    StructFreeString(pCritterElement->pchDisplayName);
                    if (pCritterElement->pchDescription)
                    {
                        StructFreeString(pCritterElement->pchDescription);
                        pCritterElement->pchDescription = NULL;
                    }
                    if (pCritterElement->pCritter)
                    {
                        StructDestroy(parse_PCC_Critter, pCritterElement->pCritter);
                        pCritterElement->pCritter = NULL;
                    }
                    StructFree(pCritterElement);
                }
                eaDestroy(&pList->ppElement[i]->ppElementList[0]->ppElement);

                //    Destroy the list
                StructFreeString(pList->ppElement[i]->ppElementList[0]->pchName);
                StructFreeString(pList->ppElement[i]->ppElementList[0]->pchField);
                StructFree(pList->ppElement[i]->ppElementList[0]);
                eaDestroy(&pList->ppElement[i]->ppElementList);
            }
            //    Destroy the element (group listing)
            StructFreeString(pList->ppElement[i]->pchDisplayName);
            StructFreeString(pList->ppElement[i]->pchText);
            StructFree(pList->ppElement[i]);
            eaRemove(&pList->ppElement, i);
            if (pList->current_element > i)
            {
                pList->current_element--;
            }
            else if (pList->current_element == i)
            {
                pList->current_element = 0;
            }
        }
        else if ( pList->eSpecialAction == kAction_BuildCustomCritterList || pList->eSpecialAction == kAction_BuildCustomCritterAndContactList )
        {
            for ( k = (eaSize(&pList->ppElement[i]->ppElementList[0]->ppElement)-1); k >= 0; k--)
            {
                if (!pList->ppElement[i]->ppElementList[0]->ppElement[k]->pCritter)
                {
                    continue;
                }
                if (pList->ppElement[i]->ppElementList[0]->ppElement[k]->pCritter->referenceFile)
                {
                    for ( n = 0; n < eaSize(&g_CustomVillainGroups[j]->customVillainList); n++)
                    {
                        if ( stricmp( g_CustomVillainGroups[j]->customVillainList[n]->referenceFile,
                            pList->ppElement[i]->ppElementList[0]->ppElement[k]->pCritter->referenceFile) == 0 )
                        {
                            break;            
                        }
                    }
                }
                else
                {
                    n = eaSize(&g_CustomVillainGroups[j]->customVillainList);
                }
                if ( n >= eaSize(&g_CustomVillainGroups[j]->customVillainList))
                {
                    MMElement *pCritterElement = pList->ppElement[i]->ppElementList[0]->ppElement[k];
                    eaRemove( &pList->ppElement[i]->ppElementList[0]->ppElement, k);
                    StructFreeString(pCritterElement->pchText);
                    StructFreeString(pCritterElement->pchDisplayName);
                    if (pCritterElement->pchDescription)
                    {
                        StructFreeString(pCritterElement->pchDescription);
                        pCritterElement->pchDescription = NULL;
                    }
                    StructDestroy(parse_PCC_Critter, pCritterElement->pCritter);
                    pCritterElement->pCritter = NULL;
                    pList->ppElement[i]->ppElementList[0]->doneCritterValidation = 0;
                    StructFree(pCritterElement);
                    if (pList->ppElement[i]->ppElementList[0]->current_element > k)
                    {
                        pList->ppElement[i]->ppElementList[0]->current_element--;
                    }
                    else if (pList->ppElement[i]->ppElementList[0]->current_element == k)
                    {
                        pList->ppElement[i]->ppElementList[0]->current_element = 0;
                    }
                }
            }
        }
    }

    for( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
    {
        if( eaSize(&g_CustomVillainGroups[i]->customVillainList) > 0 || pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
        {
            MMElementList *pCritterList = 0;
            MMElement * pGroupElement = 0;
            if( pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
            {
                j = 0;
            }
            for( j = 0; j < eaSize(&pList->ppElement); j++ )
            {
                if( stricmp( g_CustomVillainGroups[i]->displayName, pList->ppElement[j]->pchText ) == 0 )
                {    
                    pGroupElement = pList->ppElement[j];
                    if( pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
                    {
                        CVG_getLevelsFromUncompressedVG(g_CustomVillainGroups[i], &pGroupElement->minLevel, &pGroupElement->maxLevel);
                    }
                    break;
                }
            }

            if( !pGroupElement )
            {
                //    Add it to the pList
                pGroupElement = StructAllocRaw(sizeof(MMElement));
                pGroupElement->pchText = StructAllocString(g_CustomVillainGroups[i]->displayName);
                pGroupElement->pchDisplayName = StructAllocString(g_CustomVillainGroups[i]->displayName);

                if( pList->eSpecialAction != kAction_BuildCustomVillainGroupList )
                {
                    MMElementList *pNewList = StructAllocRaw(sizeof(MMElementList));
                    pNewList->pchName = StructAllocString("MMCritterName");
                    pNewList->bRequired |= pList->bRequired;
                    if( pList->pchReplaceNoneWith )
                        pNewList->pchReplaceNoneWith = StructAllocString(pList->pchReplaceNoneWith);
                    if ( pList->eSpecialAction == kAction_BuildCustomCritterList ) 
                    {
                        pNewList->pchField = StructAllocString("CustomCritterIdx");
                    }
                    else if( pList->eSpecialAction == kAction_BuildCustomCritterAndContactList ) 
                    {
                        pNewList->pchField = StructAllocString("CustomCritterAndContactsIdx");
                    }
                    else
                    {
                        pNewList->pchField = StructAllocString("Field is bad. Check in updateCustomCritterList");
                    }
                    pNewList->pNameLinkPtr = pList->pNameLinkPtr;
                    eaPush(&pList->ppElement, pGroupElement);
                    eaPush(&pGroupElement->ppElementList, pNewList);
                    addNoneElement( pNewList );

                }
                else
                {
                    CVG_getLevelsFromUncompressedVG(g_CustomVillainGroups[i], &pGroupElement->minLevel, &pGroupElement->maxLevel);
                    eaPush(&pList->ppElement, pGroupElement);
                }
            }

            if( pList->eSpecialAction == kAction_BuildCustomCritterAndContactList || pList->eSpecialAction == kAction_BuildCustomCritterList )
            {
                pCritterList = pGroupElement->ppElementList[0]; 
                pCritterList->pNameLinkPtr = pList->pNameLinkPtr;
                for(j = 0; j < eaSize(&g_CustomVillainGroups[i]->customVillainList); j++ )
                {
                    MMElement * pCritterElement = 0;
                    PCC_Critter *pCritter = g_CustomVillainGroups[i]->customVillainList[j];

                    if( !pCritter->name || !pCritter->villainGroup )
                        continue;

                    for( k = 0; k < eaSize(&pCritterList->ppElement); k++ )
                    {
                        if( pCritterList->ppElement[k]->pCritter &&
                            ( stricmp( pCritter->referenceFile, pCritterList->ppElement[k]->pCritter->referenceFile) == 0 ) )
                        {
                            pCritterElement = pCritterList->ppElement[k];
                            if ( pCritter->timeAge > pCritterList->ppElement[k]->pCritter->timeAge)
                            {
                                PCC_Critter *newCritter = clone_PCC_Critter( pCritter );
                                if (pCritterElement->pchText)            
                                    StructFreeString(pCritterElement->pchText);
                                if (pCritterElement->pchDisplayName)    
                                    StructFreeString(pCritterElement->pchDisplayName);
                                if (pCritterElement->pchDescription)    
                                    StructFreeString(pCritterElement->pchDescription);
                                destroy_PCC_Critter( pCritterList->ppElement[k]->pCritter);
                                pCritterElement->pchText = StructAllocString( pCritter->name );
                                pCritterElement->pchDisplayName = StructAllocString( pCritter->name );
                                if (pCritter->description)
                                {
                                    pCritterElement->pchDescription = StructAllocString(smf_DecodeAllCharactersGet(pCritter->description)) ;
                                }
                                else
                                {
                                    pCritterElement->pchDescription = NULL;
                                }
                                pCritterElement->pCritter = newCritter;
                            }

                            break;
                        }
                    }

                    if( !pCritterElement )
                    {
                        char *nameWithSource;
                        pCritterElement = StructAllocRaw(sizeof(MMElement));

                        estrCreate(&nameWithSource);
                        if (pCritter->sourceFile)
                        {
                            char *extension;
                            char *source;
                            extension = strchr(pCritter->sourceFile, '.');
                            if (extension && (stricmp(extension, ".storyarc") == 0))
                            {
                                source = "StoryArcString";
                                estrConcatf(&nameWithSource, "%s (from %s)", pCritter->name, textStd(source));
                            }
                            else if (extension && (stricmp(extension, ".cvg") == 0))
                            {
                                source = "CustomVGString";
                                estrConcatf(&nameWithSource, "%s (from %s)", pCritter->name, textStd(source));
                            }
                            else
                            {
                                estrConcatCharString(&nameWithSource, pCritter->name);
                            }
                        }
                        else
                        {
                            estrConcatCharString(&nameWithSource, pCritter->name);
                        }

                        pCritterElement->pchText = StructAllocString(nameWithSource);
                        pCritterElement->pchDisplayName = StructAllocString(nameWithSource);
                        estrDestroy(&nameWithSource);                
                        if (pCritter->description)
                        {
                            pCritterElement->pchDescription = StructAllocString(smf_DecodeAllCharactersGet(pCritter->description)) ;
                        }
                        if (pCritterElement->pCritter)
                        {
                            destroy_PCC_Critter(pCritterElement->pCritter);
                        }
                        pCritterElement->pCritter = clone_PCC_Critter(pCritter);

                        eaPush(&pCritterList->ppElement, pCritterElement);
                    }
                }
                eaQSort( pCritterList->ppElement, sortElements );
                if (createdCritterString)
                {
                    int itr;
                    for (itr = 0; itr < eaSize(&pCritterList->ppElement); itr++)
                    {
                        if (pCritterList->ppElement[itr]->pCritter && pCritterList->ppElement[itr]->pCritter->referenceFile)
                        {
                            if (stricmp(pCritterList->ppElement[itr]->pCritter->referenceFile, originalCritterString) == 0)
                            {
                                pCritterList->current_element = itr;
                                break;
                            }
                        }
                    }
                    if (itr >= eaSize(&pCritterList->ppElement))
                    {
                        pCritterList->current_element = 0;
                        pCritterList->doneCritterValidation = 0;    
                    }
                }
                else
                {
                    pCritterList->doneCritterValidation = 0;
                }
            }
        }
    }
    eaQSort( pList->ppElement, sortElements );
    if (createdString)
    {
        for (i = 0; i < eaSize(&pList->ppElement); i++)
        {
            if (stricmp(pList->ppElement[i]->pchDisplayName, originalCurrentElementString) == 0)
            {
                pList->current_element = i;
                break;
            }
        }
        if (i >= eaSize(&pList->ppElement))
        {
            pList->current_element = 0;
        }
        estrDestroy(&originalCurrentElementString);
        pList->doneCritterValidation = 0;
        createdString = 0;
    }
    if (createdCritterString)
    {
        estrDestroy(&originalCritterString);
        createdCritterString = 0;
    }
}

void updateCustomCritterList(MMScrollSet *pSet)
{
    int i, j, k;

    // update the templates
    for( i = eaSize(&s_ppCustomCritterLists)-1; i>=0; i-- )
        updateCustomCrittersElementList(s_ppCustomCritterLists[i]);

    // now update everything already set up
    for( i = 0; i < eaSize(&pSet->ppStoryRegion); i++ )
    {
        MMRegion *pRegion = pSet->ppStoryRegion[i];
        for( j = 0; j < eaSize(&pRegion->ppElementList); j++ )
            updateCustomCrittersElementList( pRegion->ppElementList[j] );
        for( j = 0; j < eaSize(&pRegion->ppButton); j++ )
        {
            for( k = 0; k < eaSize(&pRegion->ppButton[j]->ppElementList); k++ )
                updateCustomCrittersElementList( pRegion->ppButton[j]->ppElementList[k] );
        }
    }

    for( i = 0; i < eaSize(&pSet->ppMission); i++ )
    {
        for( j = 0; j < eaSize(&pSet->ppMission[i]->ppMissionRegion); j++ )
        {
            MMRegion *pRegion = pSet->ppMission[i]->ppMissionRegion[j];
            for( k = 0; k < eaSize(&pRegion->ppElementList); k++ )
                updateCustomCrittersElementList( pRegion->ppElementList[k] );
            for( k = 0; k < eaSize(&pRegion->ppButton); k++ )
            {
                int x;
                for( x = 0; x < eaSize(&pRegion->ppButton[k]->ppElementList); x++ )
                    updateCustomCrittersElementList( pRegion->ppButton[k]->ppElementList[x] );
            }
        }
        for( j = 0; j < eaSize(&pSet->ppMission[i]->ppDetailRegion); j++ )
        {
            MMRegion *pRegion = pSet->ppMission[i]->ppDetailRegion[j];
            for( k = 0; k < eaSize(&pRegion->ppElementList); k++ )
                updateCustomCrittersElementList( pRegion->ppElementList[k] );
            for( k = 0; k < eaSize(&pRegion->ppButton); k++ )
            {
                int x;
                for( x = 0; x < eaSize(&pRegion->ppButton[k]->ppElementList); x++ )
                    updateCustomCrittersElementList( pRegion->ppButton[k]->ppElementList[x] );
            }
        }
    }
    pSet->dirty = 1;
}

void setSelectionToCritter(MMElementList *pList, PCC_Critter *returnCritter)
{
    int i;
    char * critterIndex = returnCritter->referenceFile;
    MMElement *villainGroup = NULL;
    if (!pList)
    {
        return;
    }
    if (pList->ppElement)
    {
        if (pList->current_element >= eaSize(&pList->ppElement))
        {
            return;
        }
        villainGroup = pList->ppElement[pList->current_element];
    }
    else
    {
        return;
    }

    //    If the list current element is not the all critter group
    //    && the correct villain group
    if ((stricmp(villainGroup->pchDisplayName, textStd("AllCritterListText")) != 0) &&
        (stricmp(villainGroup->pchDisplayName, returnCritter->villainGroup) != 0))
    {
        //    get the right list
        for (i = 0; i < eaSize(&pList->ppElement); i++)
        {
            if (stricmp(pList->ppElement[i]->pchDisplayName, returnCritter->villainGroup) == 0)
            {
                pList->current_element = i;
                villainGroup = pList->ppElement[i];
                break;
            }
        }
        if (i >= eaSize(&pList->ppElement))
        {
            //    couldn't find it.. error
            return;
        }
    }
    //    set the current element to the critterIndex
    if (villainGroup->ppElementList)
    {
        for (i = 0; i < eaSize(&villainGroup->ppElementList[0]->ppElement); i++)
        {
            if (villainGroup->ppElementList[0]->ppElement[i]->pCritter && 
                (stricmp(villainGroup->ppElementList[0]->ppElement[i]->pCritter->referenceFile, critterIndex) == 0) )
            {
                //    set the doneCritterValidation to 1 to make sure that critter validation occurs on the list (so that error messages appear)
                villainGroup->ppElementList[0]->doneCritterValidation = 0;
                villainGroup->ppElementList[0]->current_element = i;
                return;
            }
        }
    }
}

static void addVillainGroup( MMVillainGroup *pGroup, MMElementList *pList, VGListType type, int set )
{
    MMElement * pElement;
    char tmp[256];

    if( pGroup->pchExclude )
    {
        if( type == kVGList_VillainGroup && (stricmp(pGroup->pchExclude, "NotAGroup") == 0 || stricmp( pGroup->pchExclude,  "ContactAndPersonOnly" ) == 0)  )
            return;
        if( type == kVGList_Boss && stricmp( pGroup->pchExclude,  "ContactAndPersonOnly" ) == 0 )
            return;
    }

    pElement = StructAllocRaw(sizeof(MMElement));

    if( type == kVGList_VillainGroup  )
    {
        sprintf( tmp, "%s (%i-%i)", textStd(pGroup->pchDisplayName), pGroup->minGroupLevel, pGroup->maxGroupLevel);
        pElement->minLevel = pGroup->minGroupLevel;
        pElement->maxLevel = pGroup->maxGroupLevel;
    }
    else
    {
        sprintf( tmp, "%s (%i-%i)", textStd(pGroup->pchDisplayName), pGroup->minLevel, pGroup->maxLevel);
        pElement->minLevel = pGroup->minLevel;
        pElement->maxLevel = pGroup->maxLevel;
    }

    pElement->pchText = StructAllocString(pGroup->pchName );
    pElement->pchDisplayName = StructAllocString(tmp);
    if( pGroup->pchDescription )
        pElement->pchVGDescription = StructAllocString(pGroup->pchDescription);


    pElement->setnum = set;

    eaPush(&pList->ppElement, pElement);

    if( type != kVGList_VillainGroup  )
    {
        addVillainDef( pGroup, pElement, type, pList  );
        if( !eaSize(&pElement->ppElementList) )
        {
            pElement = eaPop(&pList->ppElement);
            StructDestroy(parse_MMElement, pElement);
        }
    }
}

static void addVillainGroupList( MMElementList *pListNew, VGListType type )
{
    int i, j;
    static MMElementList *cachedList[kVGList_Count] = {0};

    if( !cachedList[type] )
    {
        MMElementList *pList = StructAllocRaw(sizeof(MMElementList));

        eaPush( &s_ppCustomCritterLists, pList );

        StructCopy( parse_MMElementList, pListNew, pList, 0, 0 );

        addNoneElement( pList );

        if( type == kVGList_VillainGroup )
        {
            MMElement * pElement = StructAllocRaw(sizeof(MMElement));
            pElement->pchText = StructAllocString("Empty");
            pElement->pchDisplayName = StructAllocString("MMNoVillains");
            pElement->minLevel = 1;
            pElement->maxLevel = 54;
            eaPush(&pList->ppElement, pElement);
        }

        for( i = 0; i < eaSize(&gMMVillainGroupList.group); i++ )
        {
            addVillainGroup( gMMVillainGroupList.group[i], pList, type, 0 );
            for(j = 0; j < eaSize(&gMMVillainGroupList.group[i]->ppGroup); j++ )
                addVillainGroup( gMMVillainGroupList.group[i]->ppGroup[j], pList, type, j+1 );
        }

        if(    (type == kVGList_SupportEntity) ||
            (type == kVGList_Entity) ||
            (type == kVGList_Boss) )
        {
            addDoppelElement(pList);
        }

        eaQSort( pList->ppElement, sortElements );
        cachedList[type] = pList;
    }

    for( i = 0; i < eaSize(&cachedList[type]->ppElement); i++ )
    {
        MMElement * pElement = StructAllocRaw(sizeof(MMElement));
        StructCopy( parse_MMElement, cachedList[type]->ppElement[i], pElement, 0, 0 );
        eaPush( &pListNew->ppElement, pElement );
    }
    updateNoneElements( pListNew, 0 );
}

static void setDefaultList( MMElementList *pList )
{
    int i;
    if( pList->pchDefault )
    {
        for( i = 0; i < eaSize(&pList->ppElement); i++ )
        {
            if(stricmp(pList->pchDefault, pList->ppElement[i]->pchText) == 0)
                pList->current_element = i;
        }
    }
}

static void populateList( MMElementList *pList, int bRequired, MMElementList * pListParent )
{
    StaticDefineInt * pSDI;
    MMElement *pElement;
    int i, j;

    pList->bRequired |= bRequired;
    if( pListParent && pListParent->pchReplaceNoneWith )
        pList->pchReplaceNoneWith = StructAllocString(pListParent->pchReplaceNoneWith);
    if( !eaSize(&pList->ppElement) )
    {
        switch( pList->eSpecialAction )
        {
            xcase kAction_BuildVillainGroupList:
                addVillainGroupList( pList, kVGList_VillainGroup );
            xcase kAction_BuildEntityList:
                addVillainGroupList( pList, kVGList_Entity );
            xcase kAction_BuildSupportEntityList:
                addVillainGroupList( pList, kVGList_SupportEntity );
            xcase kAction_BuildBossEntityList:
                addVillainGroupList( pList, kVGList_Boss );
            xcase kAction_BuildGiantMonsterEntityList:
                addVillainGroupList( pList, kVGList_GiantMonster);
            xcase kAction_BuildObjectEntityList:
                addDestuctibleObjectList( pList, 0 );
            xcase kAction_BuildAnimList:
                addAnimList( pList );        
            xcase kAction_BuildModelList:
                addObjectList( pList, 0 );
            xcase kAction_BuildMapList:
                addMapList( pList );
            xcase kAction_BuildContactList:
                addContactList( pList );
            xcase kAction_BuildModelListContact:
                addObjectList( pList, 1 );
            xcase kAction_BuildEntityListContact:
                addVillainGroupList( pList, kVGList_Contact);
            xcase kAction_BuildObjectEntityListContact:
                addDestuctibleObjectList( pList, 1 );
            xcase kAction_BuildLevelList:
                addLevelList( pList );

        //    The name is a bit misleading, but buildCustomCritterList includes both critters and contacts, 
        //    but the contacts will not pass validation
        //    Still load them into the UI because the person may want to edit their contact to make them valid
            xcase kAction_BuildCustomCritterList:
                addCustomCritterList( pList, 0);
            xcase kAction_BuildCustomCritterAndContactList:
                addCustomCritterList( pList, 1);
            xcase kAction_BuildCustomVillainGroupList:
                addCVGList( pList );
        }
    }

    if( !eaSize(&pList->ppElement) )
    {
        pSDI = playerCreatedSDIgetByName(pList->eSpecialAction);
        if(pSDI && pSDI->key == (char*)1 )
        {
            pSDI++;
            while( pSDI->key )
            {

                MMElement * pElement = StructAllocRaw(sizeof(MMElement));
                pElement->pchDisplayName = StructAllocString( pSDI->display?pSDI->display:pSDI->key );
                pElement->pchText = StructAllocString( pSDI->key );
                eaPush(&pList->ppElement, pElement);
                pSDI++;
            }
            //eaQSort( pList->ppElement, sortElements );
        }
    }

    if( eaSize(&pList->ppElement) )
    {
        setDefaultList(pList);
        for( i = 0; i < eaSize(&pList->ppElement); i++  )
        {
            pElement = pList->ppElement[i];
            for( j = 0; j < eaSize(&pElement->ppElementList); j++ )
                populateList( pElement->ppElementList[j], bRequired, pList );
        }
    }
}

static void populateRegion(MMRegion *pRegion)
{
    int i, j;

    for(i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
        MMRegionButton * pButton = pRegion->ppButton[i];
        for(j = 0; j < eaSize(&pButton->ppElementList); j++)
            populateList( pButton->ppElementList[j], 0, 0 );
    }

    for(i = 0; i < eaSize(&pRegion->ppElementList); i++)
        populateList( pRegion->ppElementList[i], 1, 0 );

}

static void linkInName( MMElementList *pList, MMElementList *pName)
{
    int i ,j;
    if( pList->eSpecialAction == kAction_BuildCustomCritterList || pList->eSpecialAction == kAction_BuildCustomCritterAndContactList ||
        pList->eSpecialAction == kAction_BuildBossEntityList || pList->eSpecialAction == kAction_BuildEntityList || pList->eSpecialAction == kAction_BuildGiantMonsterEntityList ||
        pList->eSpecialAction == kAction_BuildEntityListContact || pList->eSpecialAction == kAction_BuildContactList )
    {
        pList->pNameLinkPtr = pName;
    }

    if (pList && pList->pchName && ( stricmp( pList->pchName, "MMCritterName") == 0 ) )
    {
        pList->pNameLinkPtr = pName;
    }

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            linkInName( pList->ppElement[i]->ppElementList[j], pName );
    }
}

static void linkInAnimLists( MMElementList *pList, MMElementList *pEnt)
{
    int i ,j;
    if( pList->eSpecialAction == kAction_BuildAnimList && stricmp( pList->pchField, "DefaultAnimation") != 0 )
    {
        pList->pCrossLinkPointer = pEnt;
    }

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            linkInAnimLists( pList->ppElement[i]->ppElementList[j], pEnt );
    }
}



static void linkInDescription( MMElementList * pList, MMElementList *pDescription)
{
    int i,j;

    if( pList->eSpecialAction == kAction_BuildEntityList || 
        pList->eSpecialAction == kAction_BuildBossEntityList || 
        pList->eSpecialAction == kAction_BuildGiantMonsterEntityList ||
        pList->eSpecialAction == kAction_BuildObjectEntityList||
        pList->eSpecialAction == kAction_BuildContactList ||
        pList->eSpecialAction == kAction_BuildEntityListContact )
    {
        char * pchDesc;
        int x, y;
        pList->pCrossLinkPointer = pDescription;

        for( x = 0; x < eaSize(&pList->ppElement); x++ )
        {
            for( y = 0; y < eaSize(&pList->ppElement[x]->ppElementList); y++ )
                pList->ppElement[x]->ppElementList[y]->pCrossLinkPointer = pDescription;
        }

        // the currently selected thing is the default desc
        if( pList->ppElement[pList->current_element]->ppElementList )
        {
            MMElementList * pDescList = pList->ppElement[pList->current_element]->ppElementList[0];
            pchDesc = pDescList->ppElement[pDescList->current_element]->pchDescription;

            if( pDescription->pchDefault )
                StructFreeString(pDescription->pchDefault);
            if( pchDesc )
            {
                pDescription->pchDefault = StructAllocString(pchDesc);
                if(pDescription->pBlock && (!strlen(pDescription->pBlock->rawString) || stricmp(textStd(pDescription->pchDefault),pDescription->pBlock->rawString) == 0) ) // Only update non-modified text
                    smf_SetRawText(pDescription->pBlock, textStd(pchDesc), false);
            }
            else
            {
                pDescription->pchDefault = NULL;
            }
        }
    }

    if( pList->pchField && stricmp( pList->pchField, "Entity" ) == 0 ) 
        pList->pCrossLinkPointer = pDescription;

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            linkInDescription( pList->ppElement[i]->ppElementList[j], pDescription );
    }
}

static void linkInBossList( MMElementList *pList, MMElementList *pBossList)
{
    int i,j;
    if( pList->ppElement && pList->ppElement[0] && pList->ppElement[0]->pchDisplayName &&
        ( pList->eSpecialAction == kAction_BuildVillainGroupList && (stricmp(pList->ppElement[0]->pchDisplayName, textStd("MMSameAsBoss")) == 0)))
    {
        pList->pCrossLinkPointer = pBossList;
    }
    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            linkInBossList( pList->ppElement[i]->ppElementList[j], pBossList );
    }
}
static void linkInGroupName( MMElementList * pList, MMElementList *pGroupName )
{
    int i,j;

    if( pList->eSpecialAction == kAction_BuildEntityList || 
        pList->eSpecialAction == kAction_BuildBossEntityList || 
        pList->eSpecialAction == kAction_BuildGiantMonsterEntityList ||
        pList->eSpecialAction == kAction_BuildObjectEntityList ||
        pList->eSpecialAction == kAction_BuildContactList ||
        pList->eSpecialAction == kAction_BuildEntityListContact )
    {
        pList->pGroupLinkPtr = pGroupName;

        for( i = 0; i < eaSize(&pList->ppElement); i++ )
        {
            for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
                pList->ppElement[i]->ppElementList[j]->pGroupLinkPtr = pGroupName;
        }
    }

    if( pList->pchField && ((stricmp( pList->pchField, "OverrideGroup" ) == 0) || (stricmp(pList->pchField, "ContactGroup") == 0)) )
        pList->pGroupLinkPtr = pGroupName;

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            linkInGroupName( pList->ppElement[i]->ppElementList[j], pGroupName );
    }
}
void MMScrollSet_SetCrossLinks(MMRegion *pRegion)
{
    MMElementList *pList, *pDescription = 0, *pName = 0, *pEnt = 0, *pGroup = 0, *pBossList = 0;
    int i, j;

    for (i = 0; i < eaSize(&pRegion->ppElementList); i++)
    {
        if ( stricmp(pRegion->ppElementList[i]->pchField,"Name") == 0 )
        {
            pName = pRegion->ppElementList[i];
        }
        if ( stricmp(pRegion->ppElementList[i]->pchField,"EntityType") == 0 )
        {
            pEnt = pRegion->ppElementList[i];
            pBossList = pRegion->ppElementList[i];
        }
        if ( ((stricmp( pRegion->ppElementList[i]->pchField, "OverrideGroup" ) == 0) || (stricmp(pRegion->ppElementList[i]->pchField, "ContactGroup") == 0)))
        {
            pGroup = pRegion->ppElementList[i];
        }
    }

    // scan for optional description
    for(i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
        MMRegionButton * pButton = pRegion->ppButton[i];
        for(j = 0; j < eaSize(&pButton->ppElementList); j++)
        {
            pList = pButton->ppElementList[j];
            if ( stricmp( pList->pchField, "Description") == 0 )
                pDescription = pList;
            if (stricmp(pList->pchField, "AboutContact")==0) 
            {
                int k;
                for (k = 0; k < eaSize(&pButton->ppElementList); ++k)
                {
                    linkInDescription(pButton->ppElementList[k], pList);
                }
            }
            if ( (stricmp(pList->pchField, "ContactDisplayName") == 0 ) )
            {
                int k;
                for (k = 0; k < eaSize(&pButton->ppElementList); ++k)
                {
                    linkInName(pButton->ppElementList[k], pList);
                }
            }
            if ( !pGroup && ((stricmp( pList->pchField, "OverrideGroup" ) == 0) || (stricmp(pList->pchField, "ContactGroup") == 0)))
            {
                int k;
                pGroup = pList;
                for (k = 0; k < eaSize(&pButton->ppElementList); ++k)
                {
                    linkInGroupName(pButton->ppElementList[k], pGroup);
                }
            }
        }
    }

    if( pBossList )
    {
        for(i = 0; i < eaSize(&pRegion->ppButton); i++ )
        {
            MMRegionButton * pButton = pRegion->ppButton[i];
            for(j = 0; j < eaSize(&pButton->ppElementList); j++)
            {
                linkInBossList( pButton->ppElementList[j], pBossList );
            }
        }
    }
    if( pDescription )
    {
        for(i = 0; i < eaSize(&pRegion->ppElementList); i++)
            linkInDescription( pRegion->ppElementList[i], pDescription );
    }
    if (pName)
    {
        for (i = 0; i < eaSize(&pRegion->ppElementList); i++)
            linkInName( pRegion->ppElementList[i], pName );
    }
    if ( pGroup )
    {
        for (i = 0; i < eaSize(&pRegion->ppElementList); i++)
        {
            linkInGroupName( pRegion->ppElementList[i], pGroup );
        }
    }
    if( pEnt ) 
    {
        for(i = 0; i < eaSize(&pRegion->ppButton); i++ )
        {
            MMRegionButton * pButton = pRegion->ppButton[i];
            for(j = 0; j < eaSize(&pButton->ppElementList); j++)
            {
                linkInAnimLists( pButton->ppElementList[j], pEnt );
            }
        }
    }
}

static void regionFixup(MMRegion *pRegion)
{
    int i,j;

    MMElementList *pList = 0;

    // scan for optional description
    for(i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
        MMRegionButton * pButton = pRegion->ppButton[i];
        for(j = 0; j < eaSize(&pButton->ppElementList); j++)
        {
            pList = pButton->ppElementList[j];
            if( !eaSize(&pList->ppElement) && !pList->pBlock )
            {
                pList->pBlock = smfBlock_Create();
                if( pList->pchDefault )
                    smf_SetRawText(pList->pBlock, textStd(pList->pchDefault), false);
            }

            if( stricmp(pList->pchField, "MissionReqCheckBox") == 0 )
                pList->pCrossLinkPointer = pRegion;
        }
    }

    for(i = 0; i < eaSize(&pRegion->ppElementList); i++)
    {
        pList = pRegion->ppElementList[i];
        if(!eaSize(&pList->ppElement) && !pList->pBlock )
        {
            pList->pBlock = smfBlock_Create();
            if( pList->pchDefault )
                smf_SetRawText(pList->pBlock, textStd(pList->pchDefault), false);
        }
    }
}


MMScrollSet_Mission* MMScrollSet_AddMission(MMScrollSet *pSet)
{
    MMScrollSet_Mission *pMission = StructAllocRaw(sizeof(MMScrollSet_Mission)); 
    int i;

    eaPush( &pSet->ppMission, pMission );
    pMission->pSetParent = pSet;

    for( i = 0; i < eaSize(&ppNewMissionTemplates); i++ )
    {
        MMRegion * pRegion = StructAllocRaw(sizeof(MMRegion));
        StructCopy(parse_MMRegion, ppNewMissionTemplates[i], pRegion, 0, 0);
        eaPush(&pMission->ppMissionRegion, pRegion);
        pRegion->bUndeleteable = 1;
        pRegion->isOpen = 1;

        regionFixup(pRegion);
    }

    for( i = 0; i < eaSize(&ppAddButtonTemplate); i++ )
    {
        MMRegionButton *pButton = StructAllocRaw(sizeof(MMRegionButton));
        StructCopy( parse_MMRegionButton, ppAddButtonTemplate[i], pButton, 0, 0 );
        eaPush( &pMission->ppDetailButton, pButton );
        if(i==0)
            pButton->isOpen = 1;
    }
    updateCustomCritterList( &missionMaker );
    return pMission;
}

void MMScrollSet_New(MMScrollSet *pSet)
{
    MMRegion * pRegion = StructAllocRaw(sizeof(MMRegion));
    StructCopy( parse_MMRegion, pStoryArcTemplateRegion, pRegion, 0, 0 );
    eaPush( &pSet->ppStoryRegion, pRegion );
    regionFixup(pRegion);
    pRegion->isOpen = 1;
    pRegion->bUndeleteable = 1;
    MMScrollSet_SetCrossLinks(pRegion);
}

void MMScrollSet_Load(void)
{
    int i;

    writeConsole(OUTPUT_DEBUG, "Loading player created story arc data");
    playerCreatedStoryarc_LoadData(game_state.create_bins);
    writeConsole(OUTPUT_INFO, "Loaded player created story arc data");

    writeConsole(OUTPUT_DEBUG, "Loading custom critters");
    loadPCCFromFiles();
    writeConsole(OUTPUT_INFO, "Loaded custom critters");

    writeConsole(OUTPUT_DEBUG, "Loading custom villain groups");
    loadCVGFromFiles();
    writeConsole(OUTPUT_INFO, "Loaded custom villain groups");

    writeConsole(OUTPUT_DEBUG, "Loading mission comments");
    loadMissionComments();
    writeConsole(OUTPUT_INFO, "Loaded mission comments");

    if(game_state.create_bins)
        return;

    writeConsole(OUTPUT_DEBUG, "Generating mission maker data");

    for( i = 0; i < eaSize(&mmScrollSet_template.ppRegion); i++ )
    {
        populateRegion( mmScrollSet_template.ppRegion[i] );

        if( stricmp( mmScrollSet_template.ppRegion[i]->pchName, "StoryArcTemplate" )==0 )
            pStoryArcTemplateRegion = mmScrollSet_template.ppRegion[i];

        if( mmScrollSet_template.ppRegion[i]->pchStruct )
        {
            if( stricmp( mmScrollSet_template.ppRegion[i]->pchStruct, "Mission" )==0 )
            {
                int j, add = 1;
                for( j = 0; j < eaSize(&ppNewMissionTemplates); j++ )
                {
                    if( strcmp( ppNewMissionTemplates[j]->pchName, mmScrollSet_template.ppRegion[i]->pchName) == 0 )
                        add = 0;
                }

                if( add )
                    eaPush( &ppNewMissionTemplates, mmScrollSet_template.ppRegion[i] ); 
                else
                    Errorf( "Duplicate Mission Maker Data Loaded!" );
            }

            if( stricmp( mmScrollSet_template.ppRegion[i]->pchStruct, "Detail" )==0 )
            {
                int j, add = 1;
                for( j = 0; j < eaSize(&ppNewMissionTemplates); j++ )
                {
                    if( strcmp( ppNewMissionTemplates[j]->pchName, mmScrollSet_template.ppRegion[i]->pchName) == 0 )
                        add = 0;
                }

                if( add )
                    eaPush( &ppDetailTemplates, mmScrollSet_template.ppRegion[i] ); 
                else
                    Errorf( "Duplicate Mission Maker Data Loaded!" );
            }
        }
        writeConsole(OUTPUT_DEBUG, "\tGenerated %s", mmScrollSet_template.ppRegion[i]->pchName);
    }

    for( i = 0; i < eaSize(&mmScrollSet_template.ppButton); i++ )
    {
        if( mmScrollSet_template.ppButton[i]->pchSpecialAction && stricmp( mmScrollSet_template.ppButton[i]->pchSpecialAction, "AddButton" )==0 )
            eaPush(&ppAddButtonTemplate, mmScrollSet_template.ppButton[i]);
    }
    writeConsole(OUTPUT_INFO, "Generated mission maker data");
}


//------------------------------------------------------------------------------------------------------------------------
// Validation
//------------------------------------------------------------------------------------------------------------------------

static void setRestrictedLevel( MMScrollSet_Mission *pMission )
{
    int i;

    pMission->minLevel = 1;
    pMission->maxLevel = 54;

    for( i = 0; i < eaSize(&pMission->ppRestrict); i++ )
    {
        MAX1(pMission->minLevel, pMission->ppRestrict[i]->min);
        MIN1(pMission->maxLevel, pMission->ppRestrict[i]->max);
    }
}

// this will remove one restriction equal to the inmin and inmax, this is used to see if something would be valid if
// it was removed
static void getRestrictedLevelsMinus( MMScrollSet_Mission * pMission, int inmin, int inmax, int *outmin, int *outmax )
{
    int i, found = 0;

    *outmin = 1;
    *outmax = 54;

    for( i = 0; i < eaSize(&pMission->ppRestrict); i++ )
    {
        if( !found && inmin == pMission->ppRestrict[i]->min && inmax == pMission->ppRestrict[i]->max )
        {
            found = true; 
        }
        else
        {
            MAX1(*outmin, pMission->ppRestrict[i]->min);
            MIN1(*outmax, pMission->ppRestrict[i]->max);
        }
    }
}

void MMScrollSet_GetMissionLevelRange(MMScrollSet_Mission * pMission, int *outmin, int *outmax )
{
    int i;

    *outmin = 1;
    *outmax = 54;

    for( i = 0; i < eaSize(&pMission->ppRestrict); i++ )
    {
        MAX1(*outmin, pMission->ppRestrict[i]->min);
        MIN1(*outmax, pMission->ppRestrict[i]->max);
    }

    if( pMission->minRestrictedLevel )
        *outmin = pMission->minRestrictedLevel;
    if( pMission->maxRestrictedLevel )
        *outmax = pMission->maxRestrictedLevel;
}


int pushRestrict(MMScrollSet_Mission *pMission, int min, int max, int update)
{
    if( min && max )
    {
        int size = eaSize(&pMission->ppRestrict);

        if( update && size ) // my parent set a level already, just update it to my more relevent data
        {
            LevelRestrict * pRestrict = pMission->ppRestrict[size-1];
            pRestrict->min = min;
            pRestrict->max = max;
        }
        else
        {
            LevelRestrict * pRestrict = StructAllocRaw(sizeof(LevelRestrict));
            pRestrict->min = min;
            pRestrict->max = max;
            eaPush(&pMission->ppRestrict, pRestrict);
        }
        return 1;
    }

    return 0;
}

void updateLevelFromList(MMScrollSet_Mission *pMission, MMElementList *pList, int parent_restrict )
{
    MMElement * pElement;
    int i, added_restrict = 0;

    if( !eaSize(&pList->ppElement) )
        return;

    pElement = pList->ppElement[pList->current_element];

    if( pushRestrict( pMission, pElement->minLevel, pElement->maxLevel, parent_restrict ) )
        added_restrict = 1;

    for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        updateLevelFromList( pMission, pElement->ppElementList[i], added_restrict );

}


static MMElement * getElementFromList( MMElementList * pList, char * pchKey )
{
    MMElement *pElement = 0;
    int i;

    if( pList->pchName && pList->pchField && stricmp( pchKey, pList->pchField ) == 0 )
    {
        if( eaSize(&pList->ppElement) )
        {
            return pList->ppElement[pList->current_element];
        }
        else if( pList->pBlock )
        {
            return 0;
        }
    }

    if(eaSize(&pList->ppElement))
        pElement = pList->ppElement[pList->current_element];
    if( pElement && eaSize(&pElement->ppElementList) )
    {
        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        {
            MMElement *pTmpElement = getElementFromList( pElement->ppElementList[i], pchKey );
            if( pTmpElement )
                return pTmpElement;
        }
    }

    return 0;
}


static char * getValueFromList( MMElementList * pList, char * pchKey, int displayName )
{
    MMElement *pElement = 0;
    int i;

    if( pList->pchName && pList->pchField && stricmp( pchKey, pList->pchField ) == 0 )
    {
        if( eaSize(&pList->ppElement) )
        {
            if( pList->bCheckBoxList ) // just return first true element for now
            {
                for( i = 0; i < eaSize(&pList->ppElement); i++ )
                {
                    if( pList->ppElement[i]->bActive )
                    {
                        return displayName?textStd(pList->ppElement[i]->pchDisplayName):pList->ppElement[i]->pchText;
                    }                        
                }

                return displayName?textStd("NoneString"):"None";
            }

            return displayName? textStd(pList->ppElement[pList->current_element]->pchDisplayName):pList->ppElement[pList->current_element]->pchText;
        }
        else if( pList->pBlock )
        {
            return pList->pBlock->outputString;
        }
    }

    if(eaSize(&pList->ppElement))
        pElement = pList->ppElement[pList->current_element];
    if( pElement && eaSize(&pElement->ppElementList) )
    {
        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        {
            char * pchTemp = getValueFromList( pElement->ppElementList[i], pchKey, displayName );
            if( pchTemp )
                return pchTemp;
        }
    }

    return 0;
}


char * MMScrollSet_getValueFromRegion( MMRegion * pRegion, char * pchKey, int displayName)
{
    int i, j;

    for( i = 0; i < eaSize(&pRegion->ppElementList); i++)
    {
        char * pchTemp = getValueFromList( pRegion->ppElementList[i], pchKey, displayName );
        if( pchTemp )
            return pchTemp;
    }

    for( i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
        for( j = 0; j < eaSize(&pRegion->ppButton[i]->ppElementList); j++ )
        {
            char * pchTemp = getValueFromList( pRegion->ppButton[i]->ppElementList[j], pchKey, displayName );
            if( pchTemp )
                return pchTemp;
        }
    }

    return 0;
}

static MapLimit * getMapRecurse( MMElementList *pList )
{
    MMElement *pElement = 0;
    int i;

    if( pList->pchField && (stricmp( pList->pchField, "MapName" ) == 0) && EAINRANGE(pList->current_element, pList->ppElement) )
    {
        pElement = pList->ppElement[pList->current_element];
        return &pElement->maplimit;
    }

    if(eaSize(&pList->ppElement))
        pElement = pList->ppElement[pList->current_element];
    if( pElement && eaSize(&pElement->ppElementList) )
    {
        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
            return getMapRecurse( pElement->ppElementList[i] );
    }

    return 0;

}

// This function is solely to find the currently chosen map limit, might be faster to put this in callback when map changes
static MapLimit * getMapLimits( MMScrollSet_Mission *pMission )
{
    int i, j;

    for( i = 0; i < eaSize(&pMission->ppMissionRegion); i++ )
    {
        MMRegion * pRegion = pMission->ppMissionRegion[i];
        for( j = 0; j < eaSize(&pRegion->ppElementList); j++ )
        {
            MMElementList *pList = pRegion->ppElementList[j];
            MapLimit *pLimit = getMapRecurse(pList);
            if(pLimit)
                return pLimit;
        }
    }
    return 0;
}

void MMScrollSet_SetLimits( MMScrollSet * pSet, int *minLevel, int *maxLevel )
{
    int i,j,k,m;
    MapLimit *pMapCount;
    char * pchQuantity;
    int iQty = 1;
    MMScrollSet_Mission *pMission;

    for( m = 0; m < eaSize(&pSet->ppMission); m++ )
    {
        char *tmp;
        pMission = pSet->ppMission[m];

        pMapCount = &pMission->mapCount;
        while(eaSize(&pMission->ppRestrict))
        {
            LevelRestrict *pRestrict = eaPop(&pMission->ppRestrict);
            StructFree(pRestrict);
        }
        eaDestroy(&pMission->ppRestrict);
        pMission->pMapLimit = getMapLimits(pMission);
        memset(pMapCount, 0, sizeof(MapLimit));
        
        tmp = MMScrollSet_getValueFromRegion(pMission->ppMissionRegion[0], "MinLevel", 0);
        pMission->minRestrictedLevel = tmp ? atoi(tmp) : 0;
        tmp = MMScrollSet_getValueFromRegion(pMission->ppMissionRegion[0], "MaxLevel", 0);
        pMission->maxRestrictedLevel = tmp ? atoi(tmp) : 0;

        if( !pMission->minRestrictedLevel && !pMission->maxRestrictedLevel )
        {
            // traverse entire struct looking for mins and maxes
            for( i = 0; i < eaSize(&pMission->ppMissionRegion); i++ )
            {
                MMRegion *pRegion = pMission->ppMissionRegion[i];
                for(j = 0; j < eaSize(&pRegion->ppElementList); j++)
                    updateLevelFromList( pMission,pRegion->ppElementList[j], 0 );

                for(j = 0; j < eaSize(&pRegion->ppButton); j++ )
                {
                    for( k = 0; k < eaSize(&pRegion->ppButton[j]->ppElementList); k++ )
                        updateLevelFromList( pMission, pRegion->ppButton[j]->ppElementList[k], 0 );
                }
            }
        }

        for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
        {
            MMRegion *pRegion = pMission->ppDetailRegion[i];
            int detailType = pRegion->detailType;

            if( !pMission->minRestrictedLevel && !pMission->maxRestrictedLevel )
            {
                for(j = 0; j < eaSize(&pRegion->ppElementList); j++)
                    updateLevelFromList( pMission,pRegion->ppElementList[j], 0 );

                for(j = 0; j < eaSize(&pRegion->ppButton); j++ )
                {
                    for( k = 0; k < eaSize(&pRegion->ppButton[j]->ppElementList); k++ )
                        updateLevelFromList( pMission, pRegion->ppButton[j]->ppElementList[k], 0 );
                }
            }
            pchQuantity = MMScrollSet_getValueFromRegion(pRegion,"Quantity",1);
            if( pchQuantity )
                iQty = MAX(1, atoi(pchQuantity) );

            if( detailType == kDetail_GiantMonster )
            {
                pMapCount->giant_monster += iQty;
            }
            else if( detailType  == kDetail_Collection )
            {
                if( stricmp( "Wall", MMScrollSet_getValueFromRegion(pRegion, "ObjectLocation", 0 ) ) == 0 )
                    pMapCount->wall[0] += iQty;
                else
                    pMapCount->floor[0] += iQty;
            }
            else if( detailType == kDetail_Rescue || detailType == kDetail_Escort || detailType == kDetail_Ally ||
                detailType == kDetail_DefendObject || detailType == kDetail_Rumble  )
            {
                pMapCount->around_only[0] += iQty;
            }
            else
                pMapCount->regular_only[0] += iQty;
        }

        setRestrictedLevel( pMission ); 
    }
}

typedef enum LevelRangeErrors
{
    kLevelRangeError_none,
    kLevelRangeError_error,
    kLevelRangeError_warning_low,
    kLevelRangeError_warning_high,
}LevelRangeErrors;

int MMScrollSet_ValidLevel( MMScrollSet_Mission *pMission, int min, int max, int removemin, int removemax )
{
    int tmin, tmax;

    if(!pMission)
        return kLevelRangeError_none;

    tmin = pMission->minLevel;
    tmax = pMission->maxLevel;

    if( !min && !max ) // 0 - 0 just means no level range
        return kLevelRangeError_none;

    if( removemin || removemax  )
        getRestrictedLevelsMinus( pMission, removemin, removemax, &tmin, &tmax );


    if( pMission->maxRestrictedLevel && pMission->minRestrictedLevel ) // Player has set the level range
    {
        if( pMission->maxRestrictedLevel < pMission->minRestrictedLevel ) // nothing has a valid range
            return kLevelRangeError_error;
        
        //otherwise we cannot fail here, just return warnings
        if( max < pMission->minRestrictedLevel )
            return kLevelRangeError_warning_low;
        if( min > pMission->maxRestrictedLevel )
            return kLevelRangeError_warning_high;
    }
    else if( pMission->minRestrictedLevel ) // player just set a lower bound
    {
        if( tmax < pMission->minRestrictedLevel ) // nothing valid
            return kLevelRangeError_error;
        
        if( min > tmax )
            return kLevelRangeError_error;
        if( max < pMission->minRestrictedLevel )
            return kLevelRangeError_warning_low;
    }
    else if( pMission->maxRestrictedLevel ) // player just set an upper bound
    {
        if( tmin > pMission->maxRestrictedLevel ) // nothing valid
            return kLevelRangeError_error;

        if( max < tmin )
            return kLevelRangeError_error;
        if( min >pMission->maxRestrictedLevel )
            return kLevelRangeError_warning_high;
    }
    else
    {
        if(tmax < tmin)
            return kLevelRangeError_error;
        if( max < tmin || min > tmax )
            return kLevelRangeError_error;
    }

    return kLevelRangeError_none;
}

//------------------------------------------------------------------------------------------------------------------------
// Page Display
//------------------------------------------------------------------------------------------------------------------------
#define MM_MIN_TEX_WIDTH 128
static void MMPage_AddElementListPic( char ** str, MMElementList * pList, PictureBrowser ***ppb, int *idx)
{
    int i;
    if(!idx)
        return;

    if( pList->ppMultiList && EAINRANGE(pList->current_list, pList->ppMultiList) )
    {
        MMPage_AddElementListPic( str, pList->ppMultiList[pList->current_list], ppb, idx);
        return;
    }

    if( eaSize( &pList->ppElement ) )
    {
        MMElement * pElement = pList->ppElement[pList->current_element];
        if(!pElement)
            return;
        if(pElement->npcIndexList && ppb)
        {
            char *temp;
            PictureBrowser *pb;
            estrCreate(&temp);
            if(!*ppb)
                eaCreate(ppb);

            pb = eaGet(ppb, *idx);

            if(!pb)
            {
                pb = picturebrowser_create();
                picturebrowser_init( pb,0, 0, 0, MM_IMAGE_SIZE, MM_IMAGE_SIZE, NULL, 0 );
                eaInsert(ppb, pb, *idx);
            }
            if (pElement->pNPCCostume)
            {
                const NPCDef *npc = NULL;
                npc = npcFindByIdx(pElement->npcIndexList[pElement->npcIndex]);
                if (npc)
                {
                    if (pElement->pNPCCostume != pElement->plastNPCCostume)
                    {
                        Costume *newCostume = costume_clone(npc->costumes[0]);
                        if (pElement->tempCostume)
                        {
                            costume_destroy(pElement->tempCostume);
                        }
                        pElement->tempCostume = newCostume;
                        costumeApplyNPCColorsToCostume(pElement->tempCostume, pElement->pNPCCostume);
                    }
                    picturebrowser_setMMImageCostume(pb, costume_as_const(pElement->tempCostume));
                    pElement->plastNPCCostume = pElement->pNPCCostume;
                }
                else
                    picturebrowser_setMMImage( pb, &pElement->npcIndex, pElement->npcIndexList );
            }
            else
                picturebrowser_setMMImage( pb, &pElement->npcIndex, pElement->npcIndexList );

            //this is a hack, but we've got to get SMF to update, and a change in the text is much easier
            //than tracking the change down to where the text is actually drawn.
            if(pb->changed)
                estrConcatChar(str, ' ');

            if(mm_smallWindowWidth-(pb)->wd < MM_MIN_TEX_WIDTH)
            {
                estrConcatf(str, "<img src=pictureBrowser:%d align=center>", ((pb)->idx) );
            }
            else
            {
                estrConcatf(&temp, "<img src=pictureBrowser:%d align=right>", ((pb)->idx) );
                estrInsert(str, 0, temp, strlen(temp));
            }

            estrDestroy(&temp);
            (*idx)++;
            //estrConcatf(str, "<img src=empty:%d_%d align=right>", (int)((*pb)->x), (int)((*pb)->ht) );
            //estrConcatf(str, "<img src=mmframe:%d align=right>", pElement->npcIndex );
        }
        else if (ppb && pElement->pCritter && pElement->pCritter->pccCostume)
        {
            char *temp;
            PictureBrowser *pb;
            estrCreate(&temp);

            if(!*ppb)
                eaCreate(ppb);

            pb = eaGet(ppb, *idx);
            if(!pb)
            {
                pb = picturebrowser_create();
                picturebrowser_init( pb,0, 0, 0, MM_IMAGE_SIZE, MM_IMAGE_SIZE, NULL, 0 );
                eaInsert(ppb, pb, *idx);
            }
            picturebrowser_setMMImageCostume(pb, costume_as_const(pElement->pCritter->pccCostume));
            if(mm_smallWindowWidth-(pb)->wd < MM_MIN_TEX_WIDTH)
            {
                estrConcatf(str, "<img src=pictureBrowser:%d align=center>", ((pb)->idx) );
            }
            else
            {
                estrConcatf(&temp, "<img src=pictureBrowser:%d align=right>", ((pb)->idx) );
                estrInsert(str, 0, temp, strlen(temp));
            }

            estrDestroy(&temp);
            (*idx)++;
        }

        if(pElement->animationUpdate && eaSize(ppb) )
        {
            int index = (pElement->animationUpdate-1);
            picturebrowser_setEmoteByName((*ppb)[index], mm_chosen_animation);
            SAFE_FREE(mm_chosen_animation);
        }
        pElement->animationUpdate = 0;
        if(pElement->mapImageName)
        {
            displayMap =1;    
            MMScrollSet_mapViewerInit(pList);
        }
        //estrConcatf(str, "<img src=mmframe:%d align=right>", pElement->npcIndex );

        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
            MMPage_AddElementListPic(str, pElement->ppElementList[i], ppb, idx);
    }

}

static int *s_ErrorPath;
static int *s_IndexPath;
char *s_pchErrorLog;

static void MMPage_AddElementList( char ** str, MMScrollSet *pSet, MMElementList * pList, int forceTitle )
{
    int i;
    MMScrollSet_Mission *pMission = pSet->current_mission>=0?pSet->ppMission[pSet->current_mission]:0;

    if( pList->ppMultiList && EAINRANGE(pList->current_list, pList->ppMultiList) )
    {
        MMPage_AddElementList( str, pSet, pList->ppMultiList[pList->current_list], forceTitle );
        return;
    }

    //    If it is just a regular button, return,
    //    But if the button has elements, display those.
    if( pList->bButton && (eaSize(&pList->ppElement)==0))
        return;

    if( !forceTitle )
    {
        if( eaSize( &pList->ppElement ) )
        {
            MMElement * pElement = pList->ppElement[pList->current_element];
            if( stricmp( pElement->pchText, "none" ) == 0 || (pList->pchDefault && stricmp( pElement->pchText, pList->pchDefault  ) == 0)  )
                return;
        }
        else if( !pList->pBlock || !pList->pBlock->rawString[0] || (pList->pchDefault && stricmp( pList->pBlock->rawString, textStd(pList->pchDefault) ) == 0)) 
            return;
    }

    estrConcatf( str, "<linkhover #a7ff6c><link #ffffff><a href='cmd:mmscrollsetViewList "  );
    for( i = 0; i < eaiSize(&s_IndexPath); i++ )
        estrConcatf(str, "%i.", s_IndexPath[i] );
    estrConcatStaticCharArray( str, "'>" );

    if( pList->bCheckBoxList )
    {    
        estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0 color=architect>", textStd(pList->pchName) );

        if( pList->ppElement[0] && pList->ppElement[0]->bActive )
            estrConcatf( str, " %s</font><br>", textStd( "MMTrue" ) );
        else
            estrConcatf( str, " %s</font><br>", textStd( "MMFalse" ));
    }
    else if( eaSize( &pList->ppElement ) )
    {
        MMElement * pElement = pList->ppElement[pList->current_element];

        if( !pElement )
            return;

        if(    pElement->pCritter ) 
        {
            char *color = "ArchitectDark";
            if (!pList->doneCritterValidation)
            {
                pList->reasonForInvalid = isPCCCritterValid( (s_architectIgnoreLocalValidation & MM_IGNORE_ON) ? NULL : playerPtr(), pElement->pCritter, 0, 0);
                if (pList->pNameLinkPtr && pElement->pCritter->name && ((pList->reasonForInvalid & PCC_LE_PROFANITY_NAME) != 1) )
                {
                    MMElementList *nameList;
                    nameList = (MMElementList*)pList->pNameLinkPtr;
                    smf_SetRawText(nameList->pBlock, pElement->pCritter->name, 0);
                    //    parse to get an output string, calling parseAndFormat is a bit heavy, but only done once so it should be okay.
                    smf_ParseAndFormat(nameList->pBlock, nameList->pBlock->rawString, 0,0,0,1,1,0,0,0,0,0);
                }
                pList->doneCritterValidation = 1;
            }
            PCC_generatePCCHTMLSelectionText(str, pElement->pCritter, pList->reasonForInvalid, 
                (stricmp(pList->pchField, "CustomCritterAndContactsIdx") != 0), "ArchitectDark" );
            if ( isDevelopmentMode() )
            {
                estrConcatf(str, "<color %s>%s</color> <font outline=0 color=architect>%s</font><br>", color, textStd("DebugSourceFile"),pElement->pCritter->sourceFile );
                estrConcatf(str, "<color %s>%s</color> <font outline=0 color=architect>%s</font><br>", color, textStd("DebugReferenceFile"),pElement->pCritter->referenceFile );
            }
        }
        else if( pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
        {
            if (pList->ppElement && EAINRANGE(pList->current_element, pList->ppElement))
            {
                int i;
                i = CVG_CVGExists(pList->ppElement[pList->current_element]->pchDisplayName);
                if (i == -1)
                {
                    //    this shouldn't ever happen except in the middle of an update
                }
                else
                {
                    CustomVG *cvg;
                    char *color = "ArchitectDark";
                    cvg = g_CustomVillainGroups[i];
                    if (!pList->doneCritterValidation)
                    {
                        pList->reasonForInvalid = CVG_isValidCVG(cvg,(s_architectIgnoreLocalValidation & MM_IGNORE_ON) ? NULL : playerPtr(),0,0);
                        pList->doneCritterValidation = 1;
                    }
                    if (pList->reasonForInvalid != CVG_LE_VALID)
                    {
                        color = "ArchitectError";
                    }
                    if ( pList->reasonForInvalid & CVG_LE_PROFANITY )
                    {
                        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect> %s</font><br>", 
                            textStd("CVGErrorString"), textStd("CVGProfanityString"));
                    }
                    else
                    {
                        estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", 
                            color, textStd("CVGDisplayNamePrompt"), cvg->displayName );
                    }
                    //    display informative error messages =]]
                    if ( pList->reasonForInvalid & CVG_LE_NO_CRITTERS )
                    {
                        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect> %s</font><br>", 
                            textStd("CVGErrorString"), textStd("CVGNoCritterString", cvg->displayName));                            
                    }
                    if ( pList->reasonForInvalid & CVG_LE_INVALID_CRITTER_RANK )
                    {
                        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect> %s</font><br>", 
                            textStd("CVGErrorString"), textStd("CVGCantIncludesContactsString", cvg->displayName));
                    }
                    if ( pList->reasonForInvalid & CVG_LE_INVALID_CRITTER )
                    {
                        char *critter_list;
                        int itr;
                        estrCreate(&critter_list);
                        estrConcatf(&critter_list, "(%d, %s)", cvg->badCritters[0]+1, 
                            cvg->customVillainList[cvg->badCritters[0]]->name);
                        for (itr = 1; itr < eaiSize(&cvg->badCritters); ++itr)
                        {
                            estrConcatf(&critter_list, ", (%d, %s)", cvg->badCritters[itr]+1, 
                                cvg->customVillainList[cvg->badCritters[itr]]->name);
                        }
                        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect> %s %s</font><br>", 
                            textStd("CVGErrorString"), textStd("CVGInvalidCustomCritterList", cvg->displayName, critter_list ) );
                        estrDestroy(&critter_list);
                    }
                    if ( pList->reasonForInvalid & CVG_LE_INVALID_STANDARD )
                    {
                        char *critter_list;
                        int itr;
                        estrCreate(&critter_list);
                        estrConcatf(&critter_list, "(%d, %s)", cvg->badStandardCritters[0]+1, 
                            cvg->existingVillainList[cvg->badStandardCritters[0]]->pchName );
                        for (itr = 1; itr < eaiSize(&cvg->badStandardCritters); ++itr)
                        {
                            estrConcatf(&critter_list, ", (%d, %s)", cvg->badStandardCritters[itr]+1, 
                                cvg->existingVillainList[cvg->badStandardCritters[itr]]->pchName);
                        }
                        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect> %s %s</font><br>", 
                            textStd("CVGErrorString"), textStd("CVGInvalidStandardCritterList", cvg->displayName, critter_list ) );
                        estrDestroy(&critter_list);
                    }
                }
            }
            else
            {
                //    why would this be happening????
                //    empty .. w/o a none string even.. the heck?
            }
        }
        else
        {
            //    don't want to use textStd on custom critter names
            if( pList->pchName && (!pList->eSpecialAction != kAction_BuildCustomCritterList && pList->eSpecialAction != kAction_BuildCustomCritterAndContactList) )
            {        
                if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0) ) 
                    estrConcatf(str, "<font outline=1 color=ArchitectError>%s: </font><font outline=0 color=architect>%s", textStd(pList->pchName), textStd(pElement->pchDisplayName) );
                else
                    estrConcatf(str, "<font outline=1 color=ArchitectDark>%s: </font><font outline=0 color=architect>%s", textStd(pList->pchName), textStd(pElement->pchDisplayName) );

                if( pElement->pchVGDescription )
                    estrConcatf(str, " -- %s</font><br>", textStd(pElement->pchVGDescription) );
                else
                    estrConcatStaticCharArray(str, "</font><br>" );
            }
            else
                estrConcatf(str, "<font outline=1 color=ArchitectDark>%s: </font><font outline=0 color=architect>%s</font><br>", textStd(pList->pchName), textStd(pElement->pchDisplayName) );

            if(pElement->pchAlias)
                estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</color> <font outline=0 color=architect>%s</font><br>", textStd("MMAlias"), pElement->pchAlias );

        }
        estrConcatStaticCharArray( str, "</linkhoverbg></a>" );
        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        {
            eaiPush(&s_IndexPath, i);
            MMPage_AddElementList(str, pSet, pElement->ppElementList[i], forceTitle);
            eaiPop(&s_IndexPath);
        }

        if(pElement->pchDescription)
        {
            if( pList->pCrossLinkPointer )
            {
                SMFBlock *block = ((MMElementList*)pList->pCrossLinkPointer)->pBlock;
                if (block->rawString && block->outputString && (block->rawString[0] != 0 ) && (block->outputString[0] == 0))
                {
                    //    not parsed yet
                    smf_ParseAndFormat(block, block->rawString, 0,0,0,0,0,0,1,0,0,0);
                }
                estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</color> <font outline=0 color=architect>%s</font><br>", textStd("MMDescription"), block->outputString );
            }
            else
                estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</color> <font outline=0 color=architect>%s</font><br>", textStd("MMDescription"), textStd(pElement->pchDescription) );
        }
        if(pElement->pchPowerDescription)
            estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</color> <font outline=0 color=architect>%s</font><br>", textStd("MMPowerDesc"), textStd(pElement->pchPowerDescription) );

        if( isDevelopmentMode() )
            estrConcatf(str, "<color #aaaa88>Debug Info:</color><color #aa88aa> %s</color><br>", pElement->pchText );

        return;
    }
    else if(pList->pBlock)
    {
        if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0))
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> ", textStd(pList->pchName) );
        else
            estrConcatf(str, "<font outline=1 color=ArchitectDark>%s:</font> ", textStd(pList->pchName) );

        if (pList->pBlock->rawString && pList->pBlock->outputString &&  (pList->pBlock->rawString[0] != 0) && (pList->pBlock->outputString[0] == 0))
        {
            smf_ParseAndFormat(pList->pBlock, pList->pBlock->rawString, 0,0,0,0,0,0,1,0,0,0);
        }
        if( pList->pBlock->outputString[0] )
        {
            if( pList->AllowHTML && !playerCreatedStoryarc_basicSMFValidate(pList->pchName, smf_DecodeAllCharactersGet(pList->pBlock->rawString), pList->CharLimit) )
                estrConcatf(str, "<font outline=0 color=architect>%s</font><br>", smf_DecodeAllCharactersGet(pList->pBlock->rawString) );
            else
                estrConcatf(str, "<font outline=0 color=architect>%s</font><br>", pList->pBlock->outputString );
        }
        else if( pList->pchDefault )
            estrConcatf(str, "<font outline=0 color=architect>%s</font><br>", textStd(pList->pchDefault) );
        else
            estrConcatf(str, "<font outline=0 color=architect>%s</font><br>", textStd("NoneString") );
    }

    estrConcatStaticCharArray( str, "</linkhoverbg></a>" );
}

// Returns true if anything is no longer the default
static int buttonIsNotDefault( MMRegionButton *pButton )
{
    int i;

    for( i = 0; i < eaSize(&pButton->ppElementList); i++ )
    { 
        MMElementList *pList = pButton->ppElementList[i];

        if( pList->bButton || pList->bCheckBoxList )
            continue;

        if(pList->pBlock && pList->pBlock->rawString[0] && (!pList->pchDefault || stricmp( pList->pBlock->rawString, textStd(pList->pchDefault) ) != 0) )
            return 1;

        if( pList->ppMultiList )
        {
            if(pList->current_element!=0)
                return 1;
        }

        if( eaSize( &pList->ppElement ) )
        {
            MMElement * pElement = pList->ppElement[pList->current_element];
            if( stricmp( pElement->pchText, "any" ) != 0 && stricmp( pElement->pchText, "none" ) != 0 && (!pList->pchDefault || stricmp( pElement->pchText, textStd(pList->pchDefault)  ) != 0)  )
                return 1;
        }
    }

    return 0;
}


static F32 MMPage_RegionSummary( MMScrollSet *pSet, MMRegion *pRegion, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    F32 text_ht=0;
    char * str;
    int i=0,j=0;
    F32 subht, subwd;
    int pbIndex = 0;
    CBox box;

    estrCreate(&str);
    estrConcatStaticCharArray(&str, "<br><b><font outline=1 i=0 color=ArchitectTitle><link ArchitectTitle><scale 1.2><a href='cmd:mmscrollsetToggleRegion ");
    for( i = 0; i < eaiSize(&s_IndexPath); i++ )
        estrConcatf(&str, "%i.", s_IndexPath[i] );
    estrConcatf( &str, "'>%s ", textStd(pRegion->pchDisplayName) );

    if(pRegion->isOpen)
    {
        estrConcatStaticCharArray(&str, "<font outline=0>[-]</a></scale></link></linkhover></font></b><br>");

        eaiPush(&s_IndexPath, 0);
        for( i = 0; i < eaSize(&pRegion->ppElementList); i++ )
        {
            eaiPush(&s_IndexPath, i);
            MMPage_AddElementList(&str, pSet, pRegion->ppElementList[i], 1 );
            eaiPop(&s_IndexPath);
        }
        eaiPop(&s_IndexPath);

        mm_smallWindowWidth = wd;

        for( i = 0; i < eaSize(&pRegion->ppElementList); i++ )
            MMPage_AddElementListPic(&str, pRegion->ppElementList[i], &pRegion->pb, &pbIndex);

        for( i = 0; i < eaSize(&pRegion->ppButton); i++ )
        {
            for( j = 0; j < eaSize(&pRegion->ppButton[i]->ppElementList); j++ )
                MMPage_AddElementListPic(&str, pRegion->ppButton[i]->ppElementList[j], &pRegion->pb, &pbIndex);
        }


        for( i = 0; i < eaSize(&pRegion->ppButton); i++ )
        {
            eaiPush(&s_IndexPath, i+1);
            if( buttonIsNotDefault(pRegion->ppButton[i]) )
            {
                estrConcatf(&str, "<br><b><font outline=1 color=Architect><scale 1.1>%s</scale></color></b><br>", textStd(pRegion->ppButton[i]->pchName) );
                for( j = 0; j < eaSize(&pRegion->ppButton[i]->ppElementList); j++ )
                {
                    eaiPush(&s_IndexPath, j);
                    MMPage_AddElementList(&str, pSet, pRegion->ppButton[i]->ppElementList[j], 0 );
                    eaiPop(&s_IndexPath);
                }
            }
            eaiPop(&s_IndexPath);
        }

        if( i == 0 && j == 0)
            return 0;
    }
    else
    {
        estrConcatStaticCharArray(&str, "<font outline=0>[+]</a></scale></link></linkhover></font></b><br>");
    }

    //    hack: switched the order w/ the creation so that the reparse would happen 1 frame later
    if( pRegion->pBlock && stricmp(str, pRegion->pBlock->rawString ) != 0 )
        smf_SetRawText(pRegion->pBlock, str, false);

    if( !pRegion->pBlock )
    {
        pRegion->pBlock = smfBlock_Create();
        smf_SetFlags(pRegion->pBlock, SMFEditMode_ReadOnly, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0 );
    }

    s_taMissionInfo.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    //draw in the map viewer.
    subwd = wd;
    if(displayMap && wd /2 >= mv.wd + 20)
        subwd = wd/2;
    text_ht = smf_Display( pRegion->pBlock, x+10*sc, y+10*sc, z+2, subwd-20*sc, 0, 0, 0, &s_taMissionInfo, NULL);
    subht = text_ht + 10*sc;

    if(displayMap)
    {
        if(wd /2 >= mv.wd+20)
        {
            mmmapviewer_setLoc(&mv, x+subwd +10 , y+10*sc, z+2 );
            if(mmmapviewer_display(&mv)>=0)
                subht= MAX(subht, mv.ht+10*sc+PIX3+20);
        }
        else
        {
            mmmapviewer_setLoc(&mv, x+wd/2 - mv.wd/2 , y+10*sc+subht, z+2 );
            if(mmmapviewer_display(&mv)>=0)
                subht+= mv.ht+10*sc+PIX3+10;
        }

        displayMap = 0;
    }

    BuildCBox(&box, x, y, wd, subht );
    drawBox(&box, z, 0, CLR_MM_BACK_ALTERNATE );

    estrDestroy(&str);
    return subht+PIX3*sc;
}

static F32 MMScrollSet_DrawPage( MMScrollSet * pSet,  MMRegion ***ppRegion, F32 x, F32 y, F32 z, F32 sc )
{
    int i;
    F32 start_y = y; 

    font(&game_12);
    font_color(CLR_MM_TEXT, CLR_MM_TEXT);
    s_taMissionInfo.piItalic = (int *)(0);

    y += 20*sc;

    for( i = 0; i < eaSize(ppRegion); i++ )
    { 
        eaiPush(&s_IndexPath,i);
        y += MMPage_RegionSummary( pSet, (*ppRegion)[i], x+10*sc, y, z, pSet->page_wd-20*sc, pSet->ht, sc );
        eaiPop(&s_IndexPath);
    }
    s_taMissionInfo.piItalic = (int *)(1);
    return y-start_y;
}

//------------------------------------------------------------------------------------------------------------------------
// This does the scroll set, which is basically a fancy tab 
//------------------------------------------------------------------------------------------------------------------------

#define LEFT_COLUMN_SQUISH .9f

#define SS_REGION_MIN_HT   (60*LEFT_COLUMN_SQUISH)
#define SS_REGION_MAX_HT   (60*LEFT_COLUMN_SQUISH)
#define SS_REGION_SPACER    20
#define SS_REGION_SPEED  45
#define SS_RSET_SPEED     45

#define SS_SELECTOR_WD  300
#define SS_SELECTOR_Y   (30*LEFT_COLUMN_SQUISH)

#define SS_LIST_HT      (40*LEFT_COLUMN_SQUISH)

#define SS_BUTTON_HT    20
#define SS_BUTTON_SPEED 45


static void freeElementNonStructFields( MMElement *pElement )
{
    int i;
    if( pElement->pBlock )
        smfBlock_Destroy( pElement->pBlock  );
    if ( pElement->tempCostume )
        costume_destroy(pElement->tempCostume);
    if ( pElement->pNPCCostume)
        StructDestroy(ParseCustomNPCCostume, pElement->pNPCCostume);
    for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        freeListNonStructFields(pElement->ppElementList[i]);
}

static void freeListNonStructFields( MMElementList *pList )
{
    int i;

    eaFindAndRemove(&s_ppCustomCritterLists, pList);

    if( pList->pHelp )
        freeHelpButton( pList->pHelp );
    if( pList->pBlock )
        smfBlock_Destroy( pList->pBlock  );

    for(i = 0; i < eaSize(&pList->ppElement); i++ )
        freeElementNonStructFields(pList->ppElement[i]);    
}

static void freeRegionNonStructFields(MMRegion * pRegion)
{
    int j, k;
    if( pRegion->pHelp )
        freeHelpButton( pRegion->pHelp );

    for( j = 0; j <  eaSize(&pRegion->ppElementList); j++ )
        freeListNonStructFields(pRegion->ppElementList[j]);

    for( j = 0; j < eaSize(&pRegion->ppButton); j++)
    {
        MMRegionButton *pButton = pRegion->ppButton[j];
        if( pButton->pHelp )
            freeHelpButton( pButton->pHelp );
        for( k = 0; k < eaSize(&pButton->ppElementList); k++ )
            freeListNonStructFields(pButton->ppElementList[k]);
    }
}

static void regionOpen( MMRegion *pRegion )
{
    if( pRegion->isOpen)
    {
        pRegion->isOpen = 0;
        return;
    }
    pRegion->isOpen = 1;
}

static void addAmbushElement( MMElementList *pAmbushList, char * pchDisplay, char * pchName, char * pchMatch )
{
    MMElement *pElement = StructAllocRaw(sizeof(MMElement));
    int idx;
    char * estrDisplay;

    estrCreate(&estrDisplay);
    estrConcatf(&estrDisplay, "%s %s", textStd(pchDisplay), textStd("MMCompleted") );

    pElement->pchDisplayName = StructAllocString(estrDisplay);
    pElement->pchText = StructAllocString(pchName);

    estrDestroy(&estrDisplay);

    idx = eaPush(&pAmbushList->ppElement, pElement);
    if( pchMatch && stricmp(pchMatch, smf_DecodeAllCharactersGet(pElement->pchText) ) == 0 )
        pAmbushList->current_element = idx;
}

static void updateAmbushTriggerLists( MMElementList * pAmbushList, MMRegion *pRegion, MMRegion **ppDetailRegions )
{
    if( pAmbushList->eSpecialAction == kAction_BuildAmbushTrigger )
    {
        int x;
        char *match_string = 0;

        pAmbushList->pCrossLinkPointer = pRegion; 

        if( eaSize(&pAmbushList->ppElement) > 1 )
            match_string = strdup( smf_DecodeAllCharactersGet(pAmbushList->ppElement[pAmbushList->current_element]->pchText));
        else if( pAmbushList->pchAmbushLoad )
            match_string = strdup( pAmbushList->pchAmbushLoad );

        // clean list
        for( x = eaSize(&pAmbushList->ppElement)-1; x>=0; x-- )
        {
            MMElement *pElement = eaRemove(&pAmbushList->ppElement, x);
            freeElementNonStructFields(pElement);
            StructDestroy(parse_MMElement, pElement);
        }            

        // recreate list
        pAmbushList->current_element = 0;
        addNoneElement( pAmbushList );
        for( x = 0; x < eaSize(&ppDetailRegions); x++ )
        {
             if( ppDetailRegions[x]->detailType == kDetail_Ambush || ppDetailRegions[x]->detailType == kDetail_Patrol ) // ambush can't trigger on self
                continue;
            
            if( ppDetailRegions[x] == pRegion && stricmp(pAmbushList->pchField, "PersonBetrayalTrigger")!=0 )
                continue;

            if( !ppDetailRegions[x]->pchUserName )
                continue;

            addAmbushElement( pAmbushList, ppDetailRegions[x]->pchActualDisplay, ppDetailRegions[x]->pchUserName, match_string );

            if( ppDetailRegions[x]->detailType == kDetail_Boss || ppDetailRegions[x]->detailType == kDetail_GiantMonster || ppDetailRegions[x]->detailType == kDetail_DestructObject )
            {
                char * textEstr, *displayEstr;

                estrCreate(&textEstr);
                estrCreate(&displayEstr);
                estrConcatf(&textEstr, "%s 75_HEALTH", ppDetailRegions[x]->pchUserName );
                estrConcatf(&displayEstr, "%s %s", ppDetailRegions[x]->pchActualDisplay, textStd( "MMAt75Health" ) );
                addAmbushElement( pAmbushList, displayEstr, textEstr, match_string );

                estrClear(&textEstr);
                estrClear(&displayEstr);
                estrConcatf(&textEstr, "%s 50_HEALTH", ppDetailRegions[x]->pchUserName );
                estrConcatf(&displayEstr, "%s %s", ppDetailRegions[x]->pchActualDisplay, textStd( "MMAt50Health" ) );
                addAmbushElement( pAmbushList, displayEstr, textEstr, match_string );

                estrClear(&textEstr);
                estrClear(&displayEstr);
                estrConcatf(&textEstr, "%s 25_HEALTH", ppDetailRegions[x]->pchUserName );
                estrConcatf(&displayEstr, "%s %s", ppDetailRegions[x]->pchActualDisplay, textStd( "MMAt25Health" ) );
                addAmbushElement( pAmbushList, displayEstr, textEstr, match_string );

                estrDestroy(&textEstr);
                estrDestroy(&displayEstr);
            }

            if( ppDetailRegions[x]->detailType == kDetail_Escort )
            {
                char * textEstr, *displayEstr;
                estrCreate(&textEstr);
                estrCreate(&displayEstr);
                estrConcatf(&textEstr, "%s Rescue", ppDetailRegions[x]->pchUserName );
                estrConcatf(&displayEstr, "%s %s", ppDetailRegions[x]->pchActualDisplay, textStd( "MMAfterRescue" ) );
                addAmbushElement( pAmbushList, displayEstr, textEstr, match_string );
                estrDestroy(&textEstr);
                estrDestroy(&displayEstr);
            }
        }

        SAFE_FREE(match_string);
    }
}


static void addDestElement( MMElementList *pDestList, char * pchDisplay, char * pchName, char * pchMatch )
{
    MMElement *pElement = StructAllocRaw(sizeof(MMElement));
    int idx;

    pElement->pchDisplayName = StructAllocString( textStd(pchDisplay));
    pElement->pchText = StructAllocString(pchName);

    idx = eaPush(&pDestList->ppElement, pElement);
    if( pchMatch && stricmp(pchMatch, smf_DecodeAllCharactersGet(pElement->pchText) ) == 0 )
        pDestList->current_element = idx;
}


static void updateDestinationLists( MMElementList * pDestList, MMRegion *pRegion, MMRegion **ppDetailRegions )
{
    if( pDestList->eSpecialAction == kAction_BuildDestinationList )
    {
        int x;
        char *match_string = 0;

        pDestList->pCrossLinkPointer = pRegion; 

        if( eaSize(&pDestList->ppElement) > 1 )
            match_string = strdup( smf_DecodeAllCharactersGet(pDestList->ppElement[pDestList->current_element]->pchText));
        else if( pDestList->pchDestinationLoad )
            match_string = strdup( pDestList->pchDestinationLoad );

        // clean list
        for( x = eaSize(&pDestList->ppElement)-1; x>=0; x-- )
        {
            MMElement *pElement = eaRemove(&pDestList->ppElement, x);
            freeElementNonStructFields(pElement);
            StructDestroy(parse_MMElement, pElement);
        }            

        // recreate list
        pDestList->current_element = 0;
        addNoneElement( pDestList );
        for( x = 0; x < eaSize(&ppDetailRegions); x++ )
        {
            if( ppDetailRegions[x] == pRegion || ppDetailRegions[x]->detailType == kDetail_Ambush || ppDetailRegions[x]->detailType == kDetail_Patrol ) // ambush can't trigger on self
                continue;

            if( !ppDetailRegions[x]->pchUserName )
                continue;

            addDestElement( pDestList, ppDetailRegions[x]->pchActualDisplay, ppDetailRegions[x]->pchUserName, match_string );
        }
        SAFE_FREE(match_string);
    }
}

void MMScrollSet_updateAmbushTrigger( MMScrollSet_Mission *pMission )
{
    MMRegion *pRegion;
    MMRegion **ppDetailRegions=0;
    int i,j,k;

    // Build Detail List
    for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
    {
        pRegion = pMission->ppDetailRegion[i];
        if( stricmp( pRegion->pchName, "AmbushTemplate") != 0 && pRegion->detailType != kDetail_Ambush && pRegion->detailType != kDetail_Patrol )
            eaPush(&ppDetailRegions, pRegion);
    }

    for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
    {
        pRegion = pMission->ppDetailRegion[i];
        for( j = 0;  j < eaSize(&pRegion->ppElementList); j++ )
        {
            updateAmbushTriggerLists( pRegion->ppElementList[j], pRegion, ppDetailRegions );
            updateDestinationLists( pRegion->ppElementList[j], pRegion, ppDetailRegions );
        }

        for( j = 0; j < eaSize(&pRegion->ppButton); j++ )
        {
            for( k = 0; k < eaSize(&pRegion->ppButton[j]->ppElementList); k++ )
            {
                updateAmbushTriggerLists( pRegion->ppButton[j]->ppElementList[k], pRegion, ppDetailRegions );
                updateDestinationLists( pRegion->ppButton[j]->ppElementList[k], pRegion, ppDetailRegions );
            }
        }
    }
    eaDestroy(&ppDetailRegions);

}

static void deleteRegion(MMRegion * pDeleteRegion)
{
    if(pDeleteRegion)
    {
        if( pDeleteRegion->pParentSet )
            eaFindAndRemove( &pDeleteRegion->pParentSet->ppStoryRegion, pDeleteRegion);
        if( pDeleteRegion->pParentMission )
        {
            eaFindAndRemove( &pDeleteRegion->pParentMission->ppMissionRegion, pDeleteRegion );
            eaFindAndRemove( &pDeleteRegion->pParentMission->ppDetailRegion, pDeleteRegion );

            MMScrollSet_updateAmbushTrigger(pDeleteRegion->pParentMission);
        }
        freeRegionNonStructFields( pDeleteRegion );
        StructDestroy(parse_MMRegion, pDeleteRegion);
        missionMaker.dirty = 1;
    }
    pDeleteRegion = 0;
}

void MMScrollSet_deleteMission(MMScrollSet_Mission * pMission)
{
    if(pMission)
    {
        int i;
        if( pMission->pSetParent )
        {
            if( pMission->pSetParent->current_mission >= eaFindAndRemove(&pMission->pSetParent->ppMission, pMission) )
                pMission->pSetParent->current_mission--;
        }

        for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
            deleteRegion( pMission->ppDetailRegion[i] );
        eaDestroy(&pMission->ppDetailRegion);
        pMission->ppDetailRegion = 0;

        for( i = 0; i < eaSize(&pMission->ppMissionRegion); i++ )
            deleteRegion( pMission->ppMissionRegion[i] );
        eaDestroy(&pMission->ppMissionRegion);
        pMission->ppMissionRegion = 0;

        StructDestroy(parse_MMScrollSet_Mission, pMission);
        missionMaker.dirty = 1;
    }
    pMission = 0;
}



static void buttonOpen( MMRegion *pRegion, MMRegionButton *pButton )
{
    if(pButton->isOpen)
    {
        pButton->isOpen = 0;
        return;
    }
    else
    {
        if( pRegion )
        {
            int i;
            for( i = eaSize( &pRegion->ppButton )-1; i >= 0; i-- )
                pRegion->ppButton[i]->isOpen = 0;
        }
        pButton->isOpen = 1;
    }
}


static addErrorString( char * pchError )
{
    int i;

    estrConcatStaticCharArray(&s_pchErrorLog, "<linkhover #c74223><linkhoverbg #6c4818><link ArchitectErrorLit><a href='cmd:mmscrollsetViewList ");
    for( i = 0; i < eaiSize(&s_ErrorPath); i++ )
        estrConcatf(&s_pchErrorLog, "%i.", s_ErrorPath[i]);
    estrConcatStaticCharArray(&s_pchErrorLog, "'>");

    if( eaiSize(&s_ErrorPath) )
    {
        if( s_ErrorPath[0] == 0 )
            estrConcatf( &s_pchErrorLog, "%s: ", textStd( "MMStoryArcTitle" ) );
        else
            estrConcatf( &s_pchErrorLog, "%s: ", textStd( "MMMissionString",  s_ErrorPath[0] ) );
    }
    estrConcatf( &s_pchErrorLog, "%s</a><br>", pchError );
}

static int checkRegionsRequired( MMRegion * pRegion, MMRegion * pParent, int log_errs )
{
    char * pchRegionReq = MMScrollSet_getValueFromRegion(pParent, "MissionReqCheckBox", 0);
    if( pchRegionReq && stricmp(pchRegionReq, "Required")==0 )
    {
        char * pchParentReq = MMScrollSet_getValueFromRegion(pRegion, "MissionReqCheckBox", 0);
        if( pchParentReq && stricmp(pchParentReq, "Required")==0 )
            return 1;
        else
        {
            if(log_errs)
                addErrorString( textStd("MMInvalidTriggerParentNotRequired") );
            return 0;
        }
    }
    return 1;
}

int MMScrollSet_isElementListValid( MMScrollSet_Mission * pMission, MMElementList * pList, int recurse, int log_errs, int *only_level, int *sub_has_level )
{
    MMElement * pElement = 0;
    int i, retval = 1;

    if( eaSize(&pList->ppElement) ) 
        pElement = pList->ppElement[pList->current_element];

    if( pList->bRequired ) // make sure its not default or null
    {
        if( pElement )
        {
            if( !pElement->pchText || stricmp(pElement->pchText, "None") ==0 || (pList->pchDefault && stricmp(pElement->pchText, pList->pchDefault )==0) )
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMNotOptionSet", pList->pchName ) );
            }
        }
    }

    if( pList->pBlock ) 
    {
        if( pList->eSpecialAction != kAction_BuildAmbushTrigger && pList->eSpecialAction != kAction_BuildDestinationList )
        {
            int bIsDefault = (pList->pchDefault && pList->pBlock->rawString[0] && stricmp(pList->pBlock->rawString, textStd(pList->pchDefault) )==0);

            if( pList->bRequired && !strlen(pList->pBlock->rawString) ) 
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMRequiredText", pList->pchName) );
            }

            if( bIsDefault )
            {
                if( pList->bRequired )
                {
                    retval = 0;
                    if( log_errs )
                        addErrorString( textStd("MMNonDefaultText", pList->pchName, textStd(pList->pchDefault) ) );
                }
            }
            else
            {
                char * pchProfane=0;
                if( pList->AllowHTML )
                {
                    char * error = playerCreatedStoryarc_basicSMFValidate(pList->pchName, smf_DecodeAllCharactersGet(pList->pBlock->rawString), pList->CharLimit );
                    if( error )
                    {
                        retval = 0;
                        if( log_errs )
                            addErrorString( textStd(error) );
                    }
                }

                if( pList->bCopyrightCheck )
                {
                    pchProfane = IsAnyWordProfaneOrCopyright(smf_DecodeAllCharactersGet(pList->pBlock->outputString));
                    if( pchProfane )
                    {
                        retval = 0;
                        if( log_errs )
                        {
                            addErrorString( textStd("MMProfaneText", pList->pchName, pchProfane ) );
                        }
                    }
                }
                else
                {
                    pchProfane = IsAnyWordProfane(smf_DecodeAllCharactersGet(pList->pBlock->outputString));
                    if( pchProfane )
                    {
                        retval = 0;
                        if( log_errs )
                        {
                            addErrorString( textStd("MMProfaneText", pList->pchName, pchProfane ) );
                        }
                    }
                }

                if( pList->ValMax &&  atoi(pList->pBlock->outputString) > pList->ValMax )
                {
                    retval = 0;
                    if( log_errs )
                        addErrorString( textStd("MMTooLarge", pList->pchName ) );
                }

                if( pList->ValMin &&  atoi(pList->pBlock->outputString) < pList->ValMin )
                {
                    retval = 0;
                    if( log_errs )
                        addErrorString( textStd("MMTooSmall", pList->pchName ) );
                }
            }
        }
    }

    if( pList->pchField && stricmp( pList->pchField, "CompletionTime")== 0 )
    {
        if( stricmp(pList->ppElement[pList->current_element]->pchText, "0") != 0 )
            pMission->bFailable = 1;
    }

    if( pMission && pMission->bFailable && pList->pchField && stricmp( pList->pchField, "ContactReturnFail" ) == 0 && !strlen(pList->pBlock->rawString) )
    {
        retval = 0;
        if( log_errs )
            addErrorString( textStd("MMFailableMission" ) );
    }



    if(!pElement)
        return retval;

    if( !playerCreatedStoryarc_isUnlockedContent(pElement->pchText, (s_architectIgnoreLocalValidation & MM_IGNORE_ON) ? NULL : playerPtr() ) && !pList->bDontUnlockCheck )
    {
        retval = 0;
        if( log_errs )
            addErrorString( textStd("MMUnlockableContent", pElement->pchDisplayName) );
    }

    // Required objective have additional contraints
    if( pList->bCheckBoxList && stricmp(pList->pchField, "MissionReqCheckBox") == 0 && pList->ppElement[0]->bActive  )
    {
        MMRegion *pRegion = (MMRegion*)pList->pCrossLinkPointer;

        if( pRegion && (pRegion->detailType == kDetail_Boss || pRegion->detailType == kDetail_GiantMonster ||  pRegion->detailType == kDetail_Rescue || pRegion->detailType == kDetail_Escort || 
            pRegion->detailType == kDetail_Ally || pRegion->detailType == kDetail_DestructObject || pRegion->detailType == kDetail_DefendObject) )
        {
            char * pchAlign = MMScrollSet_getValueFromRegion( pRegion, "Alignment", 0 );
            if(pchAlign && stricmp(pchAlign, "Ally") == 0)
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMMissionRequirementAlign", pRegion->pchUserName?pRegion->pchUserName:textStd(pRegion->pchDisplayName)) );
            }
        }

        if( pRegion->detailType == kDetail_Escort || pRegion->detailType == kDetail_DefendObject )
            pMission->bFailable = 1;

        if( pRegion->detailType == kDetail_Boss )
        {
            char * pchHP = MMScrollSet_getValueFromRegion( pRegion, "BossRunsWhenHPISBelow", 0 );
            char * pchAlly = MMScrollSet_getValueFromRegion( pRegion, "BossRunsWhenAllyCountIsBelow", 0 );
            if( pchHP && stricmp( pchHP, "0") != 0 )
                pMission->bFailable = 1;
            if( pchAlly && stricmp( pchAlly, "0") != 0 )
                pMission->bFailable = 1;        
        }
    }    

    if( pMission && pList->pchField && stricmp( pList->pchField, "MapName" ) == 0)
    {
        MapLimit *pLimit = &pList->ppElement[pList->current_element]->maplimit;

        if( pMission->mapCount.giant_monster > pLimit->giant_monster )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyGiantMonster" ) );
        }
        if( pMission->mapCount.around_only[0] > pLimit->around_only[0] + pLimit->around_or_regular[0] - MIN(0, pLimit->regular_only[0] - pMission->mapCount.regular_only[0] ) )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyArounds" ) );
        }
        if( pMission->mapCount.regular_only[0] > pLimit->regular_only[0] + pLimit->around_or_regular[0] - MIN(0, pLimit->around_only[0] - pMission->mapCount.around_only[0]) )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyRegulars" ) );
        }
        if( pMission->mapCount.wall[0] > pLimit->wall[0] )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyWallItems" ) );
        }
        if( pMission->mapCount.floor[0] > pLimit->floor[0] )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyFloorItems" ) );
        }
    }

    if( pMission && pList->pchField && stricmp( pList->pchField, "ObjectLocation" ) == 0)
    {        
        if( pMission->pMapLimit && !pMission->pMapLimit->wall[0] && stricmp( pElement->pchText, "Wall")==0 )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyWallItems" ) );
        }
        if( pMission->pMapLimit && !pMission->pMapLimit->floor[0] && stricmp( pElement->pchText, "Floor")==0 )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMTooManyFloorItems" ) );
        }
    }

    if( pList->eSpecialAction == kAction_BuildCustomCritterList || pList->eSpecialAction == kAction_BuildCustomCritterAndContactList )
    {
        if (pList->current_element)
        {
            MMElementList *critterList;
            critterList = pList->ppElement[pList->current_element]->ppElementList[0];
            if ( !critterList->doneCritterValidation  && critterList->current_element )
            {
                critterList->reasonForInvalid = isPCCCritterValid( (s_architectIgnoreLocalValidation & MM_IGNORE_ON) ? NULL : playerPtr(), critterList->ppElement[critterList->current_element]->pCritter,0,0);
                critterList->doneCritterValidation = 1;
            }
            if (critterList->reasonForInvalid != PCC_LE_VALID && (critterList->current_element) )
            {
                retval = 0;
                if ( log_errs )
                {

                    addErrorString( textStd("MMInvalidPCC", critterList->ppElement[critterList->current_element]->pCritter->name) );
                }
            }
            if( pList->eSpecialAction == kAction_BuildCustomCritterList )
            {
                if(critterList->current_element)
                {
                    PCC_Critter *pcc;
                    pcc = critterList->ppElement[critterList->current_element]->pCritter;
                    if (pcc->rankIdx == PCC_RANK_CONTACT)
                    {
                        retval = 0;
                        if ( log_errs )
                        {
                            addErrorString( textStd("ArchitectContactAsBoss", critterList->ppElement[critterList->current_element]->pCritter->name) );
                        }
                    }
                }
            }
        }
    }

    if( pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
    {
        if ( !pList->doneCritterValidation )
        {
            int cvg_idx;
            cvg_idx = CVG_CVGExists(pList->ppElement[pList->current_element]->pchDisplayName);
            if (cvg_idx >= 0)
            {
                pList->reasonForInvalid = CVG_isValidCVG(g_CustomVillainGroups[cvg_idx], (s_architectIgnoreLocalValidation & MM_IGNORE_ON) ? NULL : playerPtr(),0,0 );
                pList->doneCritterValidation = 1;
            }
        }
        if ( pList->reasonForInvalid != CVG_LE_VALID && (pList->current_element) )
        {
            retval = 0;
            if ( log_errs)
            {
                addErrorString( textStd("MMInvalidCVG", pList->ppElement[pList->current_element]->pchText) );
            }
        }
    }

    if (pList->eSpecialAction == kAction_BuildDoppelEntityList)
    {
        MMElement *pchDoppelElement = pList->ppElement[pList->current_element];
        int j = 0;
        int stop = 0;

        while (MMdoppelFlagSets[j] && !stop)
        {
            int count = 0;
            for (i = 1; i < VDF_COUNT; i = i << 1)
            {
                if (i & MMdoppelFlagSets[j])
                {
                    if (pchDoppelElement->doppelFlags & i)
                    {
                        if (count)
                        {
                            if (log_errs)
                            {
                                addErrorString( textStd("MMDoppelHasInvalidFlags") );
                            }
                            retval = 0;
                            stop = 1;
                            break;
                        }
                        else
                            count++;
                    }
                }
            }
            j++;
        }
    }

    if( pMission && (pList->eSpecialAction == kAction_BuildAmbushTrigger || pList->eSpecialAction == kAction_BuildDestinationList) && stricmp(pList->pchField, "PersonBetrayalTrigger")!=0 )
    {
        if( !pList->ppElement )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMNoValidAmbushTrigger") );
        }
        else
        {
            MMElement * pElement = pList->ppElement[pList->current_element];
            int found_match = 0;
            MMRegion *pParentRegion = (MMRegion*)pList->pCrossLinkPointer;

            for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
            {
                MMRegion *pRegion = pMission->ppDetailRegion[i];

                if( pRegion->detailType == kDetail_Ambush && pParentRegion != pRegion)
                {
                    char * trigger = MMScrollSet_getValueFromRegion(pRegion, "Trigger", 0);
                    if( trigger && stricmp(trigger, pElement->pchText) == 0)
                    {
                        retval = 0;
                        if( log_errs )
                            addErrorString( textStd("MMMoreThanOneAmbushOnOneObjective") );
                    }
                }
                if( pRegion->detailType != kDetail_Ambush && pRegion->detailType != kDetail_Patrol && pRegion->pchUserName )
                {
                    if( stricmp( pRegion->pchUserName, pElement->pchText) == 0 )
                    {
                        found_match++;

                        if(log_errs && !playerCreatedStoryArc_addDetailNodeLink( pRegion, pParentRegion, 0 ))
                        {
                            retval = 0;
                            addErrorString( textStd("MMInvalidTriggerCircularDependency") );
                        }

                        if(!checkRegionsRequired( pRegion, pParentRegion, log_errs ))
                            retval = 0;

                        continue;
                    }

                    if( pRegion->detailType == kDetail_Boss || pRegion->detailType == kDetail_GiantMonster || pRegion->detailType == kDetail_DestructObject )
                    {
                        char * estr;
                        estrCreate(&estr);
                        estrConcatf(&estr, "%s 75_HEALTH", pRegion->pchUserName );
                        if( strcmp(pElement->pchText, estr ) == 0 )
                        {
                            found_match++;
                            if(log_errs && !playerCreatedStoryArc_addDetailNodeLink( pRegion, pParentRegion, 0 ))
                            {
                                retval = 0;
                                addErrorString( textStd("MMInvalidTriggerCircularDependency") );
                            }
                            if(!checkRegionsRequired( pRegion, pParentRegion, log_errs ))
                                retval = 0;
                        }

                        estrClear(&estr);
                        estrConcatf(&estr, "%s 50_HEALTH", pRegion->pchUserName );
                        if( strcmp(pElement->pchText, estr ) == 0 )
                        {
                            found_match++;
                            if(log_errs && !playerCreatedStoryArc_addDetailNodeLink( pRegion, pParentRegion, 0 ))
                            {
                                retval = 0;
                                addErrorString( textStd("MMInvalidTriggerCircularDependency") );
                            }
                            if(!checkRegionsRequired( pRegion, pParentRegion, log_errs ))
                                retval = 0;
                        }

                        estrClear(&estr);
                        estrConcatf(&estr, "%s 25_HEALTH", pRegion->pchUserName );
                        if( strcmp(pElement->pchText, estr ) == 0 )
                        {
                            found_match++;
                            if(log_errs && !playerCreatedStoryArc_addDetailNodeLink( pRegion, pParentRegion, 0 ))
                            {
                                retval = 0;
                                addErrorString( textStd("MMInvalidTriggerCircularDependency") );
                            }
                            if(!checkRegionsRequired( pRegion, pParentRegion, log_errs ))
                                retval = 0;
                        }

                        estrDestroy(&estr);
                    }

                    if( pRegion->detailType == kDetail_Escort )
                    {
                        char * estr;
                        estrCreate(&estr);
                        estrConcatf(&estr, "%s Rescue", pRegion->pchUserName );
                        if( strcmp(pElement->pchText, estr ) == 0 )
                        {
                            found_match++;
                            if(log_errs && !playerCreatedStoryArc_addDetailNodeLink( pRegion, pParentRegion, 0 ))
                            {
                                retval = 0;
                                addErrorString( textStd("MMInvalidTriggerCircularDependency") );
                            }
                            if(!checkRegionsRequired( pRegion, pParentRegion, log_errs ))
                                retval = 0;
                        }
                        estrDestroy(&estr);
                    }
                }
            }

            if(!found_match && pParentRegion->detailType == kDetail_Ambush)
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMInvalidAmbushTrigger", pElement->pchDisplayName) );
            }
            if(found_match>1)
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMInvalidAmbushTriggerDuplicate", pElement->pchDisplayName) );
            }
        }
    }

    if( recurse )
    {
        for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
        {
            eaiPush(&s_ErrorPath, i);
            retval &= MMScrollSet_isElementListValid(pMission, pElement->ppElementList[i], 1, log_errs, 0, sub_has_level);
            eaiPop(&s_ErrorPath);
        }
    }


    if( !sub_has_level || !(*sub_has_level) )
    {    
        int level_error = MMScrollSet_ValidLevel( pMission, pElement->minLevel, pElement->maxLevel, 0, 0 );

        if( level_error )
        {
            if(retval && only_level)
                *only_level = 1;

            if( level_error == kLevelRangeError_error )
                retval = 0;
            if( log_errs && !(pMission->bReportedLevelRange & (1<<level_error)) )
            {
                if( level_error == kLevelRangeError_error )
                    addErrorString( textStd("MMLevelRangeNotContiguous") );
                else if( level_error == kLevelRangeError_warning_low )
                {
                    addErrorString( textStd("MMLevelRangeWarningLow") );
                    missionMaker.bWarnings = 1;
                }
                else if( level_error == kLevelRangeError_warning_high )
                {
                    addErrorString( textStd("MMLevelRangeWarningHigh") );
                    missionMaker.bWarnings = 1;
                }

                pMission->bReportedLevelRange &= (1<<level_error);
            }
        }
    }

    if( sub_has_level && pElement->minLevel &&  pElement->maxLevel )
        *sub_has_level = 1;

    return retval;
}

int isButtonValid( MMScrollSet_Mission * pMission, MMRegionButton * pButton, int log_errs )
{
    int i, retval = 1, sub_has_level=0;

    for(i = 0; i < eaSize(&pButton->ppElementList); i++) 
    {
        eaiPush(&s_ErrorPath, i);
        retval &= MMScrollSet_isElementListValid(pMission, pButton->ppElementList[i], 1, log_errs, 0, &sub_has_level);
        eaiPop(&s_ErrorPath);
    }
    return retval;
}

int MMScrollSet_isRegionValid( MMScrollSet_Mission * pMission, MMRegion * pRegion, int log_errs )
{
    int i, retval = 1, sub_has_level=0;

    eaiPush(&s_ErrorPath, 0);
    for(i = 0; i < eaSize(&pRegion->ppElementList); i++ )
    {
        eaiPush(&s_ErrorPath, i);
        retval &= MMScrollSet_isElementListValid(pMission, pRegion->ppElementList[i], 1, log_errs, 0, &sub_has_level);
        eaiPop(&s_ErrorPath);
    }
    eaiPop(&s_ErrorPath);

    for(i = 0; i < eaSize(&pRegion->ppButton); i++)
    {
        eaiPush(&s_ErrorPath, i+1);
        retval &= isButtonValid(pMission, pRegion->ppButton[i],log_errs);
        eaiPop(&s_ErrorPath);
    }

    if( !(eaSize(&pRegion->ppElementList) || eaSize(&pRegion->ppButton) ) )
    {
        retval = 0;
        if( log_errs )
            addErrorString( textStd("MMRegionUnitialized") );
    }

    return retval;
}

int MMScrollSet_isMissionValid( MMScrollSet_Mission *pMission, int page, int log_errs )
{
    int i, j, retval = 1, ambush_cnt = 0, defeat_all = 0;

    playerCreatedStoryarc_clearDetailNodeList();

    eaiPush(&s_ErrorPath, page);
    if( page )
    {
        int objective_cnt = 0;
        for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
        {
            MMRegion * pRegion = pMission->ppDetailRegion[i];

            for( j = i+1; j < eaSize(&pMission->ppDetailRegion); j++ )
            {
                if( pRegion->pchUserName && *pRegion->pchUserName  && pMission->ppDetailRegion[j]->pchUserName && *pMission->ppDetailRegion[j]->pchUserName 
                    && stricmp( pRegion->pchUserName, pMission->ppDetailRegion[j]->pchUserName) == 0 )
                {
                    retval = 0;
                    if( log_errs )
                        addErrorString( textStd("MMDuplicateNames", pRegion->pchUserName ) );
                }    
            }

            eaiPush(&s_ErrorPath, i);
            retval &= MMScrollSet_isRegionValid(pMission, pRegion, log_errs);
            eaiPop(&s_ErrorPath);

            // Detail Parameters
            if( stricmp(pRegion->pchName, "DefeatAllTemplate")==0 )
            {
                defeat_all = 1;
                objective_cnt++;
            }
            else
            {
                char * pchReq = MMScrollSet_getValueFromRegion(pRegion, "MissionReqCheckBox", 0);
                if( pchReq && stricmp(pchReq, "Required")==0 )
                    objective_cnt++;
            }

            if( stricmp( pMission->ppDetailRegion[i]->pchName, "AmbushTemplate" ) == 0 )
                ambush_cnt++;
            if( stricmp( pMission->ppDetailRegion[i]->pchName, "DefendObjectTemplate" ) == 0   )
            {
                char * pchQuantity = MMScrollSet_getValueFromRegion(pRegion, "Quantity", 0);
                int iQty = 1;
                if( pchQuantity )
                    iQty = MAX(1, atoi(pchQuantity) );
                ambush_cnt += iQty;
            }

            if( ambush_cnt > ARCHITECT_AMBUSH_LIMIT )
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMMissionTooManyAmbush", ARCHITECT_AMBUSH_LIMIT) );
            }

        }

        if( !eaSize(&pMission->ppDetailRegion) )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMMissionWithNoDetails") );
        }

        if( eaSize(&pMission->ppDetailRegion) > 25 )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMMissionTooManyDetails") );
        }
        else if( !objective_cnt )
        {
            retval = 0;
            if( log_errs )
                addErrorString( textStd("MMMissionWithNoObjectives") );
        }

        if( objective_cnt == 1 && defeat_all )
        {
            char * pchGroup = MMScrollSet_getValueFromRegion(pMission->ppMissionRegion[0], "VillainGroup", 0);
            if( pchGroup && stricmp(pchGroup, "Empty")==0 )
            {
                retval = 0;
                if( log_errs )
                    addErrorString( textStd("MMMissionWithEmptyDefeatAll") );
            }
        }

    }
    else
    {

        for( i = 0; i < eaSize(&pMission->ppMissionRegion); i++ )
        {
            eaiPush(&s_ErrorPath, i);
            retval &= MMScrollSet_isRegionValid(pMission, pMission->ppMissionRegion[i], log_errs);
            eaiPop(&s_ErrorPath);
        }
    }
    eaiPop(&s_ErrorPath);

    return retval;
}

int MMScrollSet_isStoryArcValid( MMScrollSet *pSet, int log_errs )
{
    int i, retval = 1;
    for( i = 0; i < eaSize(&pSet->ppStoryRegion); i++ )
    {
        eaiPush(&s_ErrorPath, i);
        retval &= MMScrollSet_isRegionValid(0, pSet->ppStoryRegion[i], log_errs);
        eaiPop(&s_ErrorPath);
    }
    return retval;
}

int MMScrollSet_IsReady( MMScrollSet *pSet )
{
    int i, retval=1;

    // zero list
    while(eaiSize(&s_ErrorPath))
        eaiPop(&s_ErrorPath);

    if( !s_pchErrorLog )
        estrCreate(&s_pchErrorLog);
    estrClear(&s_pchErrorLog);

    if( pSet->filesize >= MAX_ARC_FILESIZE )
    {
        addErrorString( textStd("MMArcTooBig") );
        retval = 0;
    }

    pSet->bWarnings = 0;

    eaiPush(&s_ErrorPath, 0);  
    retval &= MMScrollSet_isStoryArcValid(&missionMaker, 1);
    eaiPop(&s_ErrorPath);

    for( i = 0; i < eaSize(&missionMaker.ppMission); i++ )
    {
        MMScrollSet_Mission *pMission = missionMaker.ppMission[i];
        pMission->pMapLimit = getMapLimits(pMission);
        pMission->bReportedLevelRange = 0;
        eaiPush(&s_ErrorPath, i+1);
        retval &= MMScrollSet_isMissionValid(pMission,0, 1);
        pMission->bFailable = 0; // this will lag one frame behind
        retval &= MMScrollSet_isMissionValid(pMission,1, 1);
        eaiPop(&s_ErrorPath);
    }

    return retval;
}

extern int MMdoppelFlagSets[];
F32 drawDoppelFlags( MMElement *pElement, F32 x, F32 y, F32 z, F32 wd, F32 sc)
{
    int i, j = 0;
    int ht = 0;
    float maxWd = 0;
    float startY= y;
    float startX = x = x-wd/2;
    while (MMdoppelFlagSets[j])
    {
        float boxht = 0.0f;
        for (i = 1; i < VDF_COUNT; i = i << 1)
        {
            if (i & MMdoppelFlagSets[j])
            {
                int status = pElement->doppelFlags & i;
                float boxwd;
                if (drawMMCheckBox(x, y, z+10, sc, textStd(StaticDefineIntRevLookup(ParseVillainDoppelFlags, i)), &status, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
                    CLR_MM_TITLE_LIT, CLR_MM_BUTTON_LIT, &boxwd, &boxht))
                {
                    //    clicked on, change the status
                    pElement->doppelFlags &= (~MMdoppelFlagSets[j]) & (VDF_COUNT-1);
                    pElement->doppelFlags |= status ? i : 0;
                }
                x+= boxwd;
            }
        }
        x = startX;
        if (i < (VDF_COUNT << 1))
        {
            y += boxht + (5*sc);
            ht += boxht + (5*sc);
        }
        j++;
    }

    drawFlatFrame( PIX2, R10, startX-sc*PIX3, startY-sc*PIX2, z, wd+sc*PIX2, ht+sc*PIX2, sc, CLR_MM_BACK_ALTERNATE, CLR_MM_BACK_ALTERNATE );
    return ht+20*sc;
}

F32 doElementList( MMScrollSet *pSet, MMScrollSet_Mission * pMission, MMElementList *pList, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc)
{
    F32 tht = 0;
    F32 scale = 1;
    MMElement *pElement;
    int i;

    if( pSet->pTargetList == pList && pSet->fBlinkTimer)
    {
        F32 fStr = (sinf( pSet->fBlinkTimer )+1)/2;
        int blink_color = 0xc7422300;
        interp_rgba( fStr, 0xc7422300, 0xc7422388, &blink_color );
        pSet->fBlinkTimer -= TIMESTEP/4;
        pSet->fTargetY = y;

        if( pSet->fBlinkTimer < 0 )
        {
            pSet->pTargetList = 0;
            pSet->fBlinkTimer = 0;
            pSet->fTargetY = 0;
        }
        drawFlatFrame( PIX3, R10, (x - wd/2), y - 20*sc, z-2, wd, SS_LIST_HT*sc + 20, sc, blink_color, blink_color );
    } 

    scale = MINMAX( ht/SS_LIST_HT*sc, 0, sc);
    if( pList->pchName && ( (!pList->bButton) || (pList->bButton && pList->eSpecialAction) ) )
    {
        int color = CLR_MM_TEXT;

        if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0) )
            color = CLR_MM_TEXT_ERROR;

        font(&game_12);
        font_color( color, color );

        prnt(x - (wd-60*sc)/2, y, z, scale, scale, pList->pchName);
        y += 20*sc;
        tht += 20*sc;
    }
    tht += SS_LIST_HT*sc;


    if(pList->ppElement)
    {
        scrollSet_ElementListSelector( pMission, pList, x, y, z, scale, wd );
        if( pList->updated )
        {
            MMElementList *pDesc = (MMElementList *)pList->pCrossLinkPointer;

            //Hack: if this is a new animation, update the last chosen animation field.
            if (pList->eSpecialAction == kAction_BuildAnimList)
            {
                mm_chosen_animation = strdup(pList->ppElement[pList->current_element]->pchText);
                pList->ppElement[pList->current_element]->animationUpdate = 1;
            }

            //another hack for maps
            MMScrollSet_mapViewerInit( pList );

            // if a new entity was selected, update the description
            if(pDesc && eaSize(&pList->ppElement) && pList->ppElement[pList->current_element]->pchDescription)
            {
                if( (!pDesc->pchDefault && strlen(pDesc->pBlock->rawString) == 0) || (pDesc->pchDefault && stricmp(textStd(pDesc->pchDefault),pDesc->pBlock->rawString) == 0) ) // Only update non-modified text
                {
                    smf_SetRawText(pDesc->pBlock, textStd(pList->ppElement[pList->current_element]->pchDescription), false);
                    estrClear(&pDesc->pBlock->outputString);
                }

                if( pDesc->pchDefault )
                    StructFreeString(pDesc->pchDefault);
                pDesc->pchDefault = StructAllocString(pList->ppElement[pList->current_element]->pchDescription);
            }
            // if this was the boss villain group updated the other villain groups
            pSet->dirty = 1;
        }
        pElement = pList->ppElement[pList->current_element];

        if ( pList->current_element )    //    don't need to do this on none either
        {
            for(i=0; scale >= sc && i < eaSize(&pElement->ppElementList); i++ ) 
            {
                F32 t;
                int prevElement;
                if( scale > 0 && pElement->ppElementList[i]->pchName ) 
                {
                    F32 strwd = str_wd(&game_12,sc,sc,pElement->ppElementList[i]->pchName);
                    helpButton( &pElement->ppElementList[i]->pHelp, x - (wd-50*sc)/2 + strwd + 15*sc,  y + 28*sc, z+10, sc, pElement->ppElementList[i]->pchToolTip, &pSet->bounds );
                }


                pElement->ppElementList[i]->updated = pList->updated; // if a parent is updated, so are its children
                y += SS_LIST_HT*sc;
                prevElement = pElement->ppElementList[i]->current_element;
                t = doElementList( pSet, pMission, pElement->ppElementList[i], x, y, z, wd, ht - tht, sc );
                if (pElement->ppElementList[i]->current_element != prevElement)
                {
                    pSet->dirty = 1;
                }
                y += t;
                tht += t;
            }
        }
        if (pList->eSpecialAction == kAction_BuildDoppelEntityList)
        {
            float t;
            y += SS_LIST_HT*sc;
            t = drawDoppelFlags(pElement, x, y, z, wd*0.9f, sc);
            y += t;
            tht += t;
        }
        pList->updated = 0;
    }

    if( pList->bButton )
    {
        scale = MINMAX( ht/SS_LIST_HT*sc, 0, sc);
        if( pList->eSpecialAction == kAction_BuildCustomCritterList || pList->eSpecialAction == kAction_BuildCustomCritterAndContactList )
        {
            if (pList->ppElement)
            {
                F32 offset = 0;
                F32 width = (wd-80*sc)*scale;
                F32 yOffset = SS_LIST_HT*sc;
                if (pList->current_element != 0)
                {
                    MMElementList *villainGroupList;
                    villainGroupList = *(pList->ppElement[pList->current_element]->ppElementList);
                    yOffset = 0;
                    if (villainGroupList && villainGroupList->ppElement && (villainGroupList->current_element!= 0) &&
                        villainGroupList->ppElement[villainGroupList->current_element]->pCritter)
                    {
                        width /=2;
                        offset = (-width/2)-10;
                        if (drawMMButton("MMEditCustomCritter", "Character_Icon", 0, x-offset, y, z, width-10, scale, MMBUTTON_ICONALIGNTOTEXT | MMBUTTON_USEOVERRIDE_COLOR, 0 ))
                        {
                            editPCC_setup(pList, PCC_EDITING_MODE_EDIT, villainGroupList->ppElement[villainGroupList->current_element]->pCritter, 0, NULL);
                        }
                    }
                }
                if (drawMMButton("MMCreateCustomCritter", "Character_Icon", 0, x+offset, y+yOffset, z, width, scale, MMBUTTON_ICONALIGNTOTEXT | MMBUTTON_USEOVERRIDE_COLOR, 0 ))
                {
                    int flags = 0;
                    char *overrideName = NULL;
                    //    if it is a contact
                    flags |= (stricmp(pList->pchField, "ContactCategory") == 0) ? PCC_SETUP_FLAGS_ISCONTACT : 0;
                    if ( pList->pNameLinkPtr )
                    {
                        MMElementList *nameList;
                        nameList = (MMElementList*)pList->pNameLinkPtr;
                        if ( nameList->pBlock && nameList->pBlock->outputString )
                        {
                            overrideName = nameList->pBlock->outputString;
                        }
                    }
                    editPCC_setup(pList, PCC_EDITING_MODE_CREATION, NULL, flags, overrideName);
                }
            }
            tht += SS_LIST_HT*sc;
        }
        else if( pList->eSpecialAction == kAction_BuildCustomVillainGroupList )
        {
            if (drawMMButton( "EditCVGString", 0, 0, x, y+ SS_LIST_HT*sc, z, (wd-80*sc)*scale, scale, 0, 0 ))
            {
                editCVG_setup(pList->ppElement[pList->current_element]->pchText);
            }
            tht += SS_LIST_HT*sc;
        }
        else if( pList->eSpecialAction == kAction_BuildBossEntityList || pList->eSpecialAction == kAction_BuildEntityList || pList->eSpecialAction == kAction_BuildGiantMonsterEntityList ||
                 pList->eSpecialAction == kAction_BuildEntityListContact ||    pList->eSpecialAction == kAction_BuildContactList )
        {
            if (pList->ppElement && EAINRANGE(pList->current_element, pList->ppElement) && pList->ppElement[pList->current_element]->pchText && (stricmp(pList->ppElement[pList->current_element]->pchText, "None") != 0) )
            {
                MMElementList *villainGroupList;
                villainGroupList = *(pList->ppElement[pList->current_element]->ppElementList);
                if (villainGroupList && villainGroupList->ppElement && EAINRANGE(villainGroupList->current_element, villainGroupList->ppElement) )
                {
                    MMElement *villainElement;
                    villainElement = villainGroupList->ppElement[villainGroupList->current_element];
                    if (villainElement->pchText &&
                        ( stricmp(villainElement->pchText, "None") != 0 ) &&
                        ( stricmp(villainElement->pchText, "Random") != 0 ) )
                    {
                        const char *npc_name = NULL;
                        if( pList->eSpecialAction == kAction_BuildContactList )
                        {
                            const NPCDef *npc = NULL;
                            int index;
                            npc = npcFindByName(villainElement->pchText, &index);
                            if (npc)
                            {
                                if (drawMMButton( "MMEditNPC", 0, 0, x, y, z, (wd-80*sc)*scale, scale, 0, 0 ))
                                {
                                    MMElementList *nameList;
                                    MMElementList *descList;
                                    MMElementList *groupList;
                                    nameList = (MMElementList*)pList->pNameLinkPtr;
                                    descList = (MMElementList*)pList->pCrossLinkPointer;
                                    groupList = (MMElementList*)pList->pGroupLinkPtr;
                                    CustomNPCEdit_setup(npc,0, 0, &villainGroupList->ppElement[villainGroupList->current_element]->pNPCCostume, 
                                        (npc && NPCCostume_canEditCostume(npc->costumes[0])) ? 1 : 0, nameList ? &nameList->pBlock->rawString : 0, descList ? &descList->pBlock->rawString : 0, groupList ? &groupList->pBlock->rawString : 0, 0 );
                                }
                                tht += SS_LIST_HT*sc;
                            }
                        }
                        else
                        {
                            const VillainDef *vDef = villainFindByName(villainElement->pchText);
                            if (vDef && vDef->levels && vDef->levels[0] && vDef->levels[0]->costumes[eaSize(&vDef->levels[0]->costumes) -(villainElement->npcIndex+1)])
                            {
                                const NPCDef *npc = NULL;
                                npc = npcFindByIdx(villainElement->npcIndexList[eaSize(&vDef->levels[0]->costumes) -(villainElement->npcIndex+1)]);
                                if (drawMMButton( "MMEditNPC", 0, 0, x, y, z, (wd-80*sc)*scale, scale, 0, 0 ))
                                {
                                    MMElementList *nameList;
                                    MMElementList *descList;
                                    MMElementList *groupList;
                                    nameList = (MMElementList*)pList->pNameLinkPtr;
                                    descList = (MMElementList*)pList->pCrossLinkPointer;
                                    groupList = (MMElementList*)pList->pGroupLinkPtr;
                                    //    NPCCostume_canEditCostume must be done to setup some state information
                                    CustomNPCEdit_setup(0,vDef, eaSize(&vDef->levels[0]->costumes) -(villainElement->npcIndex+1), &villainGroupList->ppElement[villainGroupList->current_element]->pNPCCostume, 
                                        (npc && NPCCostume_canEditCostume(npc->costumes[0])) ? 1 : 0, nameList ? &nameList->pBlock->rawString : 0, descList ? &descList->pBlock->rawString : 0, groupList ? &groupList->pBlock->rawString : 0, 0 );
                                }
                                tht += SS_LIST_HT*sc;
                            }
                        }
                    }
                }
            }
        }
        else if ( drawMMButton(pList->pchName, 0, 0, x, y - SS_LIST_HT*sc, z, (wd-80*sc)*scale, scale, MMBUTTON_SMALL, 0 ))
        {

        }
    }

    return tht;
}



MMRegion* MMScrollSet_AddDetailRegion( MMScrollSet_Mission *pMission, int type )
{
    MMRegion *pRegion=0, *pNewRegion = 0;
    int i;

    for( i = 0; i < eaSize(&ppDetailTemplates); i++ )
    {
        if( type == ppDetailTemplates[i]->detailType )
            pRegion = ppDetailTemplates[i];
    }
    if(!pRegion)
        return 0;

    pNewRegion = StructAllocRaw(sizeof(MMRegion));
    StructCopy(parse_MMRegion, pRegion, pNewRegion, 0, 0);
    pNewRegion->detailType = type;

    eaPush(&pMission->ppDetailRegion, pNewRegion);
    pNewRegion->isOpen = 1;
    pNewRegion->pParentMission = pMission;

    regionFixup( pNewRegion );
    MMScrollSet_updateAmbushTrigger(pMission);  
    return pNewRegion;
}

static int canAddRegion( MMScrollSet_Mission *pMission, MMRegion *pRegion )
{
    MapLimit * pLimit = pMission->pMapLimit;
    MapLimit * pCount = &pMission->mapCount;
    int matchCount = 0, nonAmbushCount = 0, ambushCount = 0, defeatAllCount = 0;
    int i;

    if(!pLimit)
        return 0;

    if( eaSize(&pMission->ppDetailRegion) >= 25 )
        return 0;

    for( i = 0; i < eaSize(&pMission->ppDetailRegion); i++ )
    {
        if( pMission->ppDetailRegion[i]->detailType == pRegion->detailType )
        {
            matchCount++;
            if( pRegion->limit && matchCount >= pRegion->limit )
                return 0;
        }

        if( pMission->ppDetailRegion[i]->detailType == kDetail_DefeatAll )
            defeatAllCount++;

        // There must be one non-ambush per ambush, so for counting purposes they cancel out
        if( pMission->ppDetailRegion[i]->detailType == kDetail_Ambush || pMission->ppDetailRegion[i]->detailType == kDetail_DefendObject )
        {
            nonAmbushCount--;
            ambushCount++;
        }
        else
            nonAmbushCount++;
    }


    if( pRegion->detailType == kDetail_GiantMonster )
        return MAX(0, pLimit->giant_monster-pCount->giant_monster );
    else if( pRegion->detailType == kDetail_DefeatAll )
    {
        return MAX(0, 1-defeatAllCount); 
    }
    else if( pRegion->detailType == kDetail_Collection )
    {
        return MAX(0, pLimit->wall[kMapLimit_All] + pLimit->floor[kMapLimit_All] - pCount->wall[kMapLimit_All] - pCount->floor[kMapLimit_All]);
    }
    else if( pRegion->detailType == kDetail_Ambush || pRegion->detailType == kDetail_DefendObject ) 
    {
        if( ambushCount > ARCHITECT_AMBUSH_LIMIT )
            return 0;
        else
            return MAX( ARCHITECT_AMBUSH_LIMIT-ambushCount,  pLimit->around_or_regular[kMapLimit_All] + pLimit->regular_only[kMapLimit_All] - pCount->around_or_regular[kMapLimit_All] - pCount->regular_only[kMapLimit_All] );
    }
    else if( pRegion->detailType == kDetail_Escort || pRegion->detailType == kDetail_Rescue || pRegion->detailType == kDetail_Ally || 
        pRegion->detailType == kDetail_DefendObject || pRegion->detailType == kDetail_Rumble )
        return MAX(0, pLimit->around_or_regular[kMapLimit_All] + pLimit->around_only[kMapLimit_All] - pCount->around_or_regular[kMapLimit_All] - pCount->around_only[kMapLimit_All]);
    else
        return MAX(0, pLimit->around_or_regular[kMapLimit_All] + pLimit->regular_only[kMapLimit_All] - pCount->around_or_regular[kMapLimit_All] - pCount->regular_only[kMapLimit_All]);

}



F32 MMScrollSet_DrawElementList( MMScrollSet *pSet, MMScrollSet_Mission *pMission, MMRegion * pRegion, int parentOpen, MMElementList * pList, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    F32 tht = 0;
    F32 ystart = y, scale;
    int i, cancel_blink = 0; 
    CBox box;
    SMFContextMenuMode contextmode = SMFContextMenuMode_CutCopyPaste;

    if( pList->ppMultiList ) 
    {
        // pick current sublist -- currently only used for animations
        if( !pList->pCrossLinkPointer )
            pList->current_list = 0;
        else
        {
            MMElementList *pEntType = (MMElementList*)pList->pCrossLinkPointer;
            MMElement *pElement = getElementFromList( pEntType, "Entity" );
            MMSequencerAnimList * pSeqAnim = 0;

            if( !pElement || !pElement->pchSeq )
                pList->current_list = 0;
            else if ( !stashFindPointer(gMMSeqAnimLists.st_SeqAnimLists, pElement->pchSeq, &pSeqAnim) || !EAINRANGE( pSeqAnim->index+1, pList->ppMultiList ) )
            {
                pList->current_list = -1;
                return 0;
            }
            else
                pList->current_list = pSeqAnim->index+1;

            if(pList->previous_list != pList->current_list) // we switched anim lists, look for our previous selection in the new list
            {
                MMElementList * priorList = pList->ppMultiList[pList->previous_list];
                MMElementList * currentList = pList->ppMultiList[pList->current_list];
                char * priorAnim = priorList->ppElement[priorList->current_element]->pchText;
                pSet->dirty = 1;
                for( i = 0; i < eaSize(&currentList->ppElement); i++ )
                {
                    if( stricmp(priorAnim, currentList->ppElement[i]->pchText)==0 )
                    {
                        currentList->current_element = i;
                        break;
                    }
                }
            }

            pList->previous_list = pList->current_list;
        }
        return MMScrollSet_DrawElementList( pSet, pMission, pRegion, parentOpen, pList->ppMultiList[pList->current_list], x, y, z, wd, ht, sc ); 
    }

    if( parentOpen && pList->pchName ) 
    {
        F32 strwd = str_wd(&game_12,sc,sc,pList->pchName);
        F32 ty = y - 7*sc;
        F32 tx = x;
        if( pList->bButton )
            ty = y+5*sc;
        if( pList->bCheckBoxList )
        {
            tx += 20*sc;
            ty = y;
        }

        if( sc > 0 && ht > 0 )
        {
            if( pList->bCheckBoxList )
            {
                helpButton( &pList->pHelp, x + strwd/2 + 30*sc,  ty, z+10, sc, pList->pchToolTip, &pSet->bounds );
            }
            else if( pList->bButton )    
            {
                strwd = str_wd(&title_12,sc,sc,pList->pchName);
                helpButton( &pList->pHelp, x+(wd-150*sc)/2,  ty, z+10, sc, pList->pchToolTip, &pSet->bounds );
            }
             else if ( pList->bKeywordSelector )
            {
                  helpButton( &pList->pHelp, x + (wd-50*sc)/2, ty + 20*sc, z+10, sc, pList->pchToolTip, &pSet->bounds );
            }
            else
                helpButton( &pList->pHelp, x-(wd-50*sc)/2 + strwd + 15*sc,  ty, z+10, sc, pList->pchToolTip, &pSet->bounds );
        }
    }

    if( pList->bKeywordSelector && parentOpen && sc > 0 && ht > 0 )
    {
           tht += 40*sc + missionReviewDrawKeywords( x - (wd)/2, y + 20*sc, z, sc, wd, 200*sc, WDW_MISSIONMAKER, &pSet->keywordpicks, 3, CLR_MM_FRAME_BACK); 
    }
    else if( pList->bTabbedSelection && parentOpen && sc > 0 && ht > 0 )
    {
        int prevElement;
        char ** ppNames = 0;
        int c = CLR_MM_TEXT;
        if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0) )
            c = CLR_MM_TEXT_ERROR;

        font(&game_12);
        font_color( c, c );
        prnt( x-(wd-50*sc)/2, y, z+1, sc, sc, pList->pchName );
        tht += 25*sc;

        for( i = 0; i < eaSize(&pList->ppElement); i++ )
            eaPush(&ppNames, pList->ppElement[i]->pchDisplayName);

        tht += 30*sc;
        prevElement = pList->current_element;
        tht += doElementList( pSet, pMission, pList->ppElement[pList->current_element]->ppElementList[0], x, y+60*sc, z+20, wd, ht - tht, sc );
        if (pList->current_element != prevElement)
            pSet->dirty = 1;

        pList->current_element = drawMMFrameTabs( x - wd/2, y + 30*sc, z, sc, wd, tht - 30*sc, 20*sc, kStyle_Architect, &ppNames, pList->current_element );
        tht += 20*sc;
        eaDestroy(&ppNames);
    }
    else if( pList->bCheckBoxList )
    {    
        int iSize = eaSize(&pList->ppElement);
        int c = CLR_MM_TEXT;
        AtlasTex * star = atlasLoadTexture("SGroup_icon_Rank02.tga");

        if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0) )
            c = CLR_MM_TEXT_ERROR;

        tht = (iSize+1)*20*sc;
        scale = MINMAX( ht/tht, 0, sc );

        for( i = 0; i < iSize; i++ )
        {
            int prevSetting;
            prevSetting = pList->ppElement[i]->bActive;
            drawSmallCheckboxEx(0, x - 100*sc, y, z+20, scale, pList->ppElement[i]->pchDisplayName, &pList->ppElement[i]->bActive, 0, 1, pList->ppElement[i]->pchToolTip, 0, 1, c );
            if (prevSetting != pList->ppElement[i]->bActive)
            {
                pSet->dirty = 1;
            }
            if( pList->ppElement[i]->bActive )
                display_sprite( star, x - 130*sc, y - 10*sc, z+5, sc, sc, CLR_WHITE );
            else
                display_sprite( star, x - 130*sc, y - 10*sc, z+5, sc, sc, CLR_BLACK );
            y += 20*scale;
        }
        y+=20*sc;
    }
    else if( pList->bButton )
    {
        int unavailable = 0, equal_count = 0, type_count = 0;
        MMRegion * pAddRegion = 0;

        tht = 51*sc;
        scale = MINMAX( ht/tht, 0, sc );

        for( i = 0; i < eaSize(&ppDetailTemplates); i++ )
        {
            if( pList->pchTemplate && stricmp(ppDetailTemplates[i]->pchName, pList->pchTemplate ) == 0 )
                pAddRegion = ppDetailTemplates[i];
        }

          if( pAddRegion )
        {
            int regionsLeft = canAddRegion(pMission, pAddRegion);
            char * estr;

            estrCreate(&estr);
            estrConcatf(&estr, "%s (%i)", textStd(pList->pchName), regionsLeft );

             if( drawMMBulbButton( estr, pAddRegion->pchIcon, pAddRegion->pchIconGlow, x, y+5*scale, (wd-80*sc), z+5, sc, !regionsLeft) )
            {
                MMRegion *pNew = MMScrollSet_AddDetailRegion(pMission, pAddRegion->detailType);
                MMScrollSet_SetCrossLinks(pNew);
                for( i = 0; i < eaSize(&pMission->ppDetailButton); i++ )
                    pMission->ppDetailButton[i]->isOpen = 0;
            }

            estrDestroy(&estr);
        }
        else // default - show something 
        {
            if ( drawMMButton(pList->pchName, 0, 0, x, y, z+5, (wd-80*sc)*scale, scale, MMBUTTON_SMALL, 0 ))
            {

            }
        }

    }
    else if( eaSize( &pList->ppElement ) )
    {
        int prev_element;
        prev_element = pList->current_element;
        tht = doElementList( pSet, pMission, pList, x, y, z+20, wd, ht, sc );
        if (pList->current_element != prev_element)
        {
            pSet->dirty = 1;
        }
        scale = MINMAX( ht/tht, 0, sc );
        cancel_blink = 1; // blink handled in function
    }
    else if(pList->pBlock)
    {
        int bShowDefault=0;
        tht = pList->pBlock->pBlock->pos.iHeight + SS_LIST_HT*sc;
          scale = MINMAX( ht/tht, 0, sc );

        if( scale < .05 )
            return tht;

        if (pList->pBlock->editedThisFrame)
            pSet->dirty = 1;

        if( pList->pchName )
        {
            int c = CLR_MM_TEXT;

            if( pSet->bShowWrongness && !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0) )
                c = CLR_MM_TEXT_ERROR;

            font(&game_12);
            font_color( c, c );
            prnt( x-(wd-50*sc)/2, y, z+1, scale, scale, pList->pchName );

            if( ( ( stricmp( pList->pchField, "Description" ) == 0) || (stricmp(pList->pchField, "AboutContact") == 0 ) )&& pList->pchDefault )
                bShowDefault = (!pList->pchDefault || stricmp(textStd(pList->pchDefault), pList->pBlock->rawString)!=0);
        }

        if( pRegion && pMission && pList->pBlock->editedThisFrame && stricmp( pList->pchField, "Name" ) == 0 )
        {
            char * newName;

            if( pRegion->pchUserName )
                StructFreeString(pRegion->pchUserName);

            if( pList->pBlock->outputString && *pList->pBlock->outputString)
                pRegion->pchUserName = StructAllocString(smf_DecodeAllCharactersGet(pList->pBlock->outputString)); 
            else
                pRegion->pchUserName = StructAllocString(smf_DecodeAllCharactersGet(pList->pBlock->rawString)); 
            if( pRegion->pchActualDisplay )
                StructFreeString(pRegion->pchActualDisplay);

            estrCreate(&newName);
            estrConcatf(&newName, "%s: %s", textStd(pRegion->pchDisplayName), pRegion->pchUserName );
            pRegion->pchActualDisplay = StructAllocString(newName);
            estrDestroy(&newName);

            MMScrollSet_updateAmbushTrigger(pMission);
        }

         contextmode = SMFContextMenuMode_CutCopyPaste;

         if( pList->AllowHTML == 1)
        {
            contextmode = SMFContextMenuMode_CutCopyPasteFormatColorText;
        }
        else if (pList->AllowHTML == 2)
        {
            contextmode = SMFContextMenuMode_CutCopyPasteFormatColor;
        }

        if( pList->ValMin||pList->ValMax )
        {
            smf_SetFlags(pList->pBlock, SMFEditMode_ReadWrite, 0, SMFInputMode_NonnegativeIntegers, pList->ValMax, SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, contextmode, SMAlignment_Left, 0, "MissionMakerTabNavigationSet", -1); 
        }
        else
        {
            smf_SetFlags(pList->pBlock, SMFEditMode_ReadWrite, 0, 0, pList->CharLimit, SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, contextmode, SMAlignment_Left, 0, "MissionMakerTabNavigationSet", -1); 
        }

         s_taMissionInfo.piScale = (int *)((int)(1.1f*sc*SMF_FONT_SCALE));
        tht = SS_LIST_HT*sc + smf_Display(pList->pBlock, x-(wd-40*sc)/2, y+5*sc, z+2, wd-40*sc, 0, 0, 0, &s_taMissionInfo, 0);

        if(bShowDefault)
        {    
            if(drawListButton("DefaultString", x + wd/2 - 100*sc, y + tht - 30*sc, z+300, 70*sc, 15*sc, sc))
            {
                smf_SetRawText( pList->pBlock, textStd(pList->pchDefault), false );
                collisions_off_for_rest_of_frame = 1;
            }
            tht += 25*sc;
        }

        if( ht > 0 )
        {
            int txt_color = 0xffffff22;
            BuildCBox(&box, x-(wd-40*sc)/2, y+5*sc, wd-40*sc, tht - SS_LIST_HT*sc );

            if( mouseCollision(&box) )
            {
                txt_color = 0xffffff44;
            }
            if( smfBlock_HasFocus(pList->pBlock) )
            {
                F32 char_wd;
                char char_buff[128], *char_txt;

                txt_color = 0x88ff88aa;

                font( &game_12 );
                font_color(CLR_MM_TEXT, CLR_MM_TEXT);

                if( pList->CharLimit )
                {
                    sprintf(char_buff, "(%i/%i)", smfBlock_GetDisplayLength(pList->pBlock), pList->CharLimit);
                    char_wd = str_wd( font_grp, sc, sc, char_buff);
                    prnt( x + (wd-40*sc)/2 - char_wd, y, z+4, sc, sc, char_buff );
                }
                else if( pList->ValMax && pList->ValMin )
                {
                    if( atoi(pList->pBlock->outputString) < pList->ValMin ||
                        atoi(pList->pBlock->outputString) > pList->ValMax )
                        font_color( CLR_RED, CLR_RED );

                    char_txt = textStd( "MMValueRange", pList->ValMin, pList->ValMax);
                    char_wd = str_wd( font_grp, sc, sc, char_txt);
                    prnt( x + (wd-40*sc)/2 - char_wd, y, z+4, sc, sc, char_txt );
                }
                else if( pList->ValMax )
                {
                    if( atoi(pList->pBlock->outputString) > pList->ValMax )
                        font_color( CLR_RED, CLR_RED );

                    char_txt = textStd( "MMValueMax", pList->ValMax );
                    char_wd = str_wd( font_grp, sc, sc, char_txt);
                    prnt( x + (wd-40*sc)/2 - char_wd, y, z+4, sc, sc, char_txt );
                }
                else if( pList->ValMin )
                {
                    if( atoi(pList->pBlock->outputString) < pList->ValMin )
                        font_color( CLR_RED, CLR_RED );

                    char_txt = textStd( "MMValueMin", pList->ValMin );
                    char_wd = str_wd( font_grp, sc, sc, char_txt);
                    prnt( x + (wd-40*sc)/2 - char_wd, y, z+4, sc, sc, char_txt );
                }
            }

            drawTextEntryFrame( x-(wd-30*sc)/2, y+3*sc, z, wd-30*sc, tht - SS_LIST_HT*sc + 4*sc, sc, smfBlock_HasFocus(pList->pBlock) );
        }
    }
    else
    {
        scale = 0;
    }

    if( pSet->pTargetList == pList && pSet->fBlinkTimer && !cancel_blink)
    {
        F32 fStr = (sinf( pSet->fBlinkTimer )+1)/2;
        int blink_color = 0xc7422300;
        interp_rgba( fStr, 0xc7422300, 0xc7422388, &blink_color );
        pSet->fBlinkTimer -= TIMESTEP/4;
        pSet->fTargetY = y;

        if( pSet->fBlinkTimer < 0 )
        {
            pSet->pTargetList = 0;
            pSet->fBlinkTimer = 0;
            pSet->fTargetY = 0;
            //pSet->converge = 0;
        }
        drawFlatFrame( PIX3, R10, (x - wd/2), ystart - 20*sc, z, wd, tht, sc, blink_color, blink_color );
    } 

    return tht;
}

F32 MMScrollSet_DrawRegionButton( MMScrollSet *pSet, MMScrollSet_Mission *pMission, MMRegion *pRegion, MMRegionButton *pButton, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    int i,list_count = eaSize(&pButton->ppElementList);
    F32 start_y = y;
    F32 tht, subht = 0;
    F32 scale;
    UIBox uibox;

    scale = MINMAX( ht/SS_LIST_HT*sc, 0, sc );
    tht = MIN(pButton->ht, ht);

    if( scale > 0 )
        helpButton( &pButton->pHelp, x + (wd-60*sc)/2 + 5*sc, y, z+10, sc, pButton->pchToolTip, &pSet->bounds );

    if( drawMMBar( pButton->pchName, x, y, z, wd-80*sc, scale, pButton->isOpen, 1 ) ) 
    {
        buttonOpen( pRegion, pButton );
    }

    if( scale > 0 && pButton->bOptional )
    {
        font(&title_9);
        font_outl(0); 
        font_color( CLR_MM_TEXT_BACK, CLR_MM_TEXT_BACK );
        prnt( x - (wd-80*sc)/2 + 2*R10*sc, y+4*sc, z+10, sc, sc, "MMOptional" );
        font_outl(1);
    }

    uiBoxDefine(&uibox, x - pButton->wd/2, y+SS_LIST_HT*sc/2, pButton->wd, tht);
    clipperPushRestrict(&uibox);

    subht += SS_LIST_HT*sc;
    for( i = 0; i < list_count; i ++ )
    {
        subht += MMScrollSet_DrawElementList( pSet, pMission, pRegion, pButton->isOpen, pButton->ppElementList[i], x, y+subht, z+1, pButton->wd, MIN(ht,pButton->ht-subht), sc );
    }

    if( pButton->isOpen && sc > 0 )
    {
        pButton->ht += TIMESTEP*SS_BUTTON_SPEED;
        if ( pButton->ht > subht )
            pButton->ht = subht;

        pButton->wd += TIMESTEP*SS_BUTTON_SPEED;
        if( pButton->wd > wd-40*sc )
            pButton->wd = wd-40*sc;
    }
    else
    {
        pButton->ht -= TIMESTEP*SS_BUTTON_SPEED;
        if ( pButton->ht < 0 )
            pButton->ht = 0;

        pButton->wd -= TIMESTEP*SS_BUTTON_SPEED;
        if( pButton->wd <  wd-60*sc )
            pButton->wd =  wd-60*sc;
    }

    clipperPop();

    if( scale > 0 && pRegion )
        drawFlatFrame( PIX3, R10, x - (pButton->wd)/2, y, z-1, pButton->wd, pButton->ht, 1.f, 0x5bbabf33, 0x5bbabf33 );

    return SS_LIST_HT*sc + pButton->ht;
}

static float MMScrollSet_drawRegion( MMScrollSet * pSet, MMScrollSet_Mission *pMission, MMRegion * pRegion, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    F32 startY = y, scale=1.f;
    F32 center = x+wd/2;  
    F32 subht, tht;
    int i;
    int iSize;
    char * pchReq;

    if( pRegion->isOpen )
    {
        pRegion->inner_ht += TIMESTEP*SS_RSET_SPEED;
        pRegion->inner_wd += TIMESTEP*SS_RSET_SPEED;
        if ( pRegion->inner_wd > SS_SELECTOR_WD*sc )
            pRegion->inner_wd = SS_SELECTOR_WD*sc;
    }
    else
    {
        if( pRegion->inner_ht> 0 )
            pRegion->inner_ht -= TIMESTEP*SS_RSET_SPEED;
        else
            pRegion->inner_ht= 0;

        pRegion->inner_ht = MAX( 0, pRegion->inner_ht );
    }

    tht = MIN( ht, pRegion->inner_ht );
    scale = sc;

    if( sc > 0 )
        helpButton( &pRegion->pHelp, x + wd - 25*sc, y - pRegion->outer_ht/2, z+10, sc, pRegion->pchToolTip, &pSet->bounds );

    if(!pRegion->bUndeleteable)
    {
        F32 button_wd = pRegion->isOpen?(wd-40*sc):(wd - 80*scale);
        F32 tx = pRegion->isOpen?(x + button_wd/2):(x + wd/2);
        char * name;

        if( pRegion->pchActualDisplay ) // Big hack to make sure the text fits on the button
        {
            int length = strlen(pRegion->pchActualDisplay)-1;
            strdup_alloca(name, pRegion->pchActualDisplay );
            while( length && str_wd( &title_12, scale*.9f, scale*.9f, "%s", name) > button_wd - 160*sc )
            {
                name[length] = 0;
                length--;
            }
        }
        else
            name = textStd(pRegion->pchDisplayName);
        if( scale > 0 )
        {
            int ret = drawMMBulbBar( name, pRegion->pchIcon, pRegion->pchIconGlow, tx, y, button_wd, z+5, scale, pRegion->isOpen, 0, &pRegion->fOffset );
            if( ret == 1 )
            {
                regionOpen( pRegion );
            }
            else if ( ret == 2 )
            {
                dialogStdCB(DIALOG_ACCEPT_CANCEL, textStd("MMReallyDeleteDetail", pRegion->pchActualDisplay?pRegion->pchActualDisplay:pRegion->pchDisplayName ), 0, 0, deleteRegion, 0, DLGFLAG_ARCHITECT|DLGFLAG_NO_TRANSLATE, pRegion );
                collisions_off_for_rest_of_frame = 1; 
            }
            else if( pSet->current_mission >= 0 && pSet->current_page == 1 && ret == 3 ) // started a drag
            {
                TrayObj to = {0};
                AtlasTex *icon = atlasLoadTexture("Macro_Button_01.tga");
                to.iset = pSet->current_mission;
                to.islot = eaFind(&pMission->ppDetailRegion, pRegion);
                to.type = kTrayItemType_PlayerCreatedDetail;
                to.scale = sc;
                trayobj_startDragging(&to,icon,NULL);
            }    
        }
        pchReq = MMScrollSet_getValueFromRegion(pRegion, "MissionReqCheckBox", 0);
        if( pchReq && stricmp(pchReq, "Required")==0 )
        {
            AtlasTex * star = atlasLoadTexture("SGroup_icon_Rank02.tga");
            display_sprite( star, tx - button_wd/2 + 50*sc, y - star->height*sc/2, z+15, sc, sc, CLR_WHITE );
        }
    }
    else
    {
        if( scale > 0 && drawMMBar( pRegion->pchActualDisplay?pRegion->pchActualDisplay:pRegion->pchDisplayName, x+wd/2, y, z+5, wd-8*R10*scale, scale, pRegion->isOpen, 1 ) ) 
            regionOpen( pRegion );
    }

    subht = 0;
    y += 38*sc;

    // First the element lists
    //-----------------------------------------------------------------------------------------------------
    for( i = 0; i < eaSize(&pRegion->ppElementList); i++ )
    {
        subht += MMScrollSet_DrawElementList( pSet, pMission, pRegion, pRegion->isOpen, pRegion->ppElementList[i], center, y+subht, z+10, wd - 40*sc, tht - subht, sc );
    }

    // now the Buttons
    //-----------------------------------------------------------------------------------------------------
    iSize = eaSize( &pRegion->ppButton );
    for( i = 0; i < iSize; i++ )
    {
        MMRegionButton *pButton = pRegion->ppButton[i];
        subht += MMScrollSet_DrawRegionButton( pSet, pMission, pRegion, pRegion->ppButton[i], center, y + subht, z+5,  wd, tht - subht, sc );
    }

    if ( pRegion->inner_ht > subht  )
        pRegion->inner_ht = subht;

    if( tht > 0 )  
         drawFlatFrame( PIX3, R10, x + R10*sc, startY, z, wd-2*R10*sc, MIN(pRegion->inner_ht, tht)+20*sc, 1, CLR_MM_FRAME_BACK, CLR_MM_FRAME_BACK );

    
     if( !isDown(MS_LEFT) && cursor.dragging )
    {
        CBox box;
        int old_col = collisions_off_for_rest_of_frame; // make sure we can detect mouse release
        if( collisions_off_for_rest_of_frame )
            collisions_off_for_rest_of_frame = 0;

        BuildCBox(&box, x, startY - 25*sc, wd, pRegion->inner_ht + 45*sc );
        if( mouseCollision(&box) )
        {
            if(    cursor.drag_obj.type == kTrayItemType_PlayerCreatedDetail && 
                pSet->current_mission == cursor.drag_obj.iset  && 
                pSet->current_page == 1 && 
                EAINRANGE(cursor.drag_obj.islot, pMission->ppDetailRegion)   )
            {
                eaSwap( &pMission->ppDetailRegion, eaFind(&pMission->ppDetailRegion, pRegion), cursor.drag_obj.islot);
            }
            trayobj_stopDragging();
        }
        collisions_off_for_rest_of_frame = old_col;
    }


    return pRegion->inner_ht;
}

F32 MMScrollSet_DrawErrors( MMScrollSet *pSet, F32 x, F32 y, F32 z, F32 wd, F32 max_ht, F32 sc, int clr )
{
    static SMFBlock *error_block;
    static F32 text_ht=0;
    F32 box_ht;
    UIBox box;
    int blink_color = CLR_MM_ERROR_DARK;
    int blink_color2 = CLR_MM_ERROR_MID;

    x += 15*sc;
    wd -= 19*sc;

    if( pSet->fErrorBlinkTimer > 0  ) 
    {
        F32 fStr = (sinf( pSet->fErrorBlinkTimer )+1)/2;

        interp_rgba( fStr, CLR_MM_ERROR_DARK, CLR_MM_ERROR_MID, &blink_color );
        fStr = (sinf( pSet->fErrorBlinkTimer + PI/2 )+1)/2;
        interp_rgba( fStr, CLR_MM_ERROR_DARK, CLR_MM_ERROR_MID, &blink_color2 );
        pSet->fErrorBlinkTimer -= TIMESTEP/4;

        if( pSet->fErrorBlinkTimer < 0 )
        {
            pSet->fErrorBlinkTimer = 0;
            blink_color = CLR_MM_ERROR_DARK;
        }
    }

    if(!strlen(s_pchErrorLog))
        return 0;

    if( !error_block)
    {
        error_block = smfBlock_Create();
        smf_SetFlags(error_block, SMFEditMode_ReadOnly, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0 );
    }

    s_taMissionInfo.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    s_taMissionInfo.piColor = (int *)CLR_MM_BUTTON_ERROR_TEXT_LIT;
    //text_ht = smf_ParseAndFormat(error_block, s_pchErrorLog, x+25*sc, y, z+2, wd-50*sc, -1, false, 0, 0, &s_taMissionInfo, 0 );

    box_ht = MAX(74*sc, MIN( 40*sc + text_ht, max_ht ));

    helpButtonFrame( x, y, z, wd - 10*sc, box_ht, sc, CLR_MM_BUTTON_ERROR_TEXT_LIT, blink_color2, blink_color );
    uiBoxDefine(&box, x+20*sc, y + 20*sc, wd-40*sc, box_ht-39*sc );
    clipperPush(&box);

    //s_taMissionInfo.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    text_ht = smf_ParseAndDisplay( error_block, s_pchErrorLog, x+25*sc, y+20*sc - pSet->errorSb.offset, z+1, wd-95*sc, 0, 0, 0, &s_taMissionInfo, NULL, 0, 1);
    s_taMissionInfo.piColor = (int *)CLR_WHITE;
    clipperPop();
    pSet->errorSb.architect = 2;
    doScrollBar( &pSet->errorSb, box_ht - 60*sc, text_ht-20*sc, x+pSet->wd-35*sc, y + 3*R10*sc, z, 0, &box );

    return box_ht;
}

F32 MMScrollSet_DrawCurrentRegions( MMScrollSet * pSet, MMScrollSet_Mission *pMission, MMRegion *** ppRegion, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    MMRegion * pRegion;
    int i;

    y += SS_REGION_SPACER;
    // manage the button expanding
    for( i = 0; i < eaSize(ppRegion); i++)
    {
        pRegion = (*ppRegion)[i];

        y += MMScrollSet_drawRegion( pSet, pMission, pRegion, x, y, z, wd, ht, sc );
        y += 45*sc; 
    }

    return y;
}




F32 MMScrollSet_Draw( MMScrollSet * pSet, F32 x, F32 y, F32 z, F32 ht, F32 sc, int clr )
{
    int i, draw_right_frame = 0; 
    F32 start_y = y;
    F32 start_ht = ht;
    F32 error_ht = 0;
    UIBox box;
    MMScrollSet_Mission *pMission=0;

    pSet->errorSb.architect = pSet->leftSb.architect = pSet->rightSb.architect = 1;

    MMScrollSet_SetLimits(pSet, 0, 0);

    uiBoxDefine(&box, x, y, pSet->wd, ht-26*sc );
    BuildCBox(&pSet->bounds, x, y, pSet->wd*2, ht-26*sc);

    clipperPush( &box );
    if( pSet->current_mission < 0 )
        y = MMScrollSet_DrawCurrentRegions(pSet, 0, &pSet->ppStoryRegion, x, y-pSet->leftSb.offset, z, pSet->wd, 10000, sc ) + pSet->leftSb.offset;
    else
    {
        if(!pSet->ppMission)
            return 0;

        pMission = pSet->ppMission[pSet->current_mission];
        if(!pMission )
            return 0; // wth

        if( pSet->current_page == 0)
            y = MMScrollSet_DrawCurrentRegions(pSet, pMission, &pMission->ppMissionRegion, x, y-pSet->leftSb.offset, z, pSet->wd, 10000, sc ) + pSet->leftSb.offset;
        else
        {
            F32 starty = y;
            for( i = 0; i < eaSize( &pMission->ppDetailButton ); i++ )
            {
                F32 suby = y;
                y += MMScrollSet_DrawRegionButton( pSet, pMission, 0, pMission->ppDetailButton[i], x+pSet->wd/2, y + 20*sc - pSet->leftSb.offset, z+1, pSet->wd, 10000, sc );
                if( pMission->ppDetailButton[i]->isOpen )
                    drawFlatFrame( PIX3, R10, x+10*sc, suby + 20*sc - pSet->leftSb.offset, z, pSet->wd-20*sc, y-suby-30*sc, sc, 0x315d5cff, 0x315d5cff );
            }
            if( eaSize( &pMission->ppDetailButton ) )
                drawFlatFrame( PIX3, R10, x+5*sc, starty - pSet->leftSb.offset + 2*sc, z, pSet->wd-10*sc, y-starty, sc, CLR_MM_FRAME_BACK, CLR_MM_FRAME_BACK );
            y += 10*sc;
            y = MMScrollSet_DrawCurrentRegions(pSet, pMission, &pMission->ppDetailRegion, x, y-pSet->leftSb.offset, z, pSet->wd, 10000, sc ) + pSet->leftSb.offset;
        }
    }
    clipperPop();

    if( pSet->converge && pSet->fBlinkTimer )
    {
        if( pSet->fTargetY > start_y + ht/2 )
        {
            pSet->leftSb.offset += TIMESTEP*20;
            if( pSet->fTargetY-TIMESTEP*20 <= start_y + ht/2)
                pSet->converge = pSet->fTargetY = 0;
        }
        else
        {
            pSet->leftSb.offset -= TIMESTEP*20;
            if(pSet->leftSb.offset < 0 || pSet->fTargetY+TIMESTEP*20 >= start_y + ht/2)
                pSet->converge = pSet->fTargetY = 0;
        }
    }
    doScrollBar( &pSet->leftSb, ht-50*sc, y-start_y, x+pSet->wd+(PIX3-1)*sc, start_y + R10*sc, z, 0, &box );


    if( pSet->bShowErrors && strlen(s_pchErrorLog) ) 
    {
        clipperPush(0);
        error_ht = MMScrollSet_DrawErrors( pSet, x+pSet->wd, start_y -74*sc, z, pSet->wd, (ht-26*sc)/2, sc, clr );
        clipperPop();

        start_y += (error_ht- 74*sc);
        ht -= (error_ht- 74*sc);
    }

    y = start_y;
    uiBoxDefine(&box, x+pSet->wd, start_y, pSet->wd, ht-26*sc );
    clipperPush( &box );

    while(eaiSize(&s_IndexPath))
        eaiPop(&s_IndexPath);

    start_y += 20*sc; 

    if( pSet->current_mission < 0 )
    {
        eaiPush(&s_IndexPath, 0 );
        drawMMBar( "MMStoryArcSummary",  x + pSet->wd + pSet->wd/2, start_y - pSet->rightSb.offset, z+5, pSet->wd - 4*R10*sc, sc, 1, 0 );
        y += MMScrollSet_DrawPage(pSet, &pSet->ppStoryRegion, x+pSet->wd, start_y - pSet->rightSb.offset, z+5, sc);
        eaiPop(&s_IndexPath);
        draw_right_frame = 1;

    }
    else if( pSet->current_page == 0)
    {
        eaiPush(&s_IndexPath, pSet->current_mission+1 );
        eaiPush(&s_IndexPath, pSet->current_page);
        drawMMBar( textStd("MMMissionSettingSummary", pSet->current_mission+1) ,  x + pSet->wd + pSet->wd/2, start_y - pSet->rightSb.offset, z+5, pSet->wd - 4*R10*sc, sc, 1, 0 );
        if(pMission )
            y += MMScrollSet_DrawPage(pSet, &pMission->ppMissionRegion,  x+pSet->wd, start_y - pSet->rightSb.offset, z+5, sc);
        eaiPop(&s_IndexPath);
        eaiPop(&s_IndexPath);
        draw_right_frame = (pMission && eaSize(&pMission->ppMissionRegion));
    }
    else
    {
        eaiPush(&s_IndexPath, pSet->current_mission+1 );
        eaiPush(&s_IndexPath, pSet->current_page);

        drawMMBar( textStd("MMMissionDetailSummary", pSet->current_mission+1) ,  x + pSet->wd + pSet->wd/2, start_y - pSet->rightSb.offset, z+5, pSet->wd - 4*R10*sc, sc, 1, 0 );
        if(pMission )
            y += MMScrollSet_DrawPage(pSet, &pMission->ppDetailRegion,  x+pSet->wd, start_y - pSet->rightSb.offset, z+5, sc);
        eaiPop(&s_IndexPath);
        eaiPop(&s_IndexPath);
        draw_right_frame = (pMission &&eaSize(&pMission->ppDetailRegion));
    }

    if( draw_right_frame )
        drawFlatFrame(PIX3, R10, x+pSet->wd+R10*sc, start_y - pSet->rightSb.offset, z, pSet->wd - 2*R10*sc, y-start_y + 30*sc, sc, CLR_MM_FRAME_BACK, CLR_MM_FRAME_BACK );

    clipperPop();
    doScrollBar( &pSet->rightSb, ht-50*sc, y-start_y + 40*sc, x+pSet->wd*2, start_y + R10*sc, z, 0, &box );


    return MAX( y-start_y, ht );
}
void MMtoggleArchitectLocalValidation(int mode)
{
    if (mode & MM_IGNORE_FROM_CONSOLE)
    {
        s_architectIgnoreLocalValidation = (mode & MM_IGNORE_ON) ? mode : 0;
    }
    else
    {
        if (!(s_architectIgnoreLocalValidation & MM_IGNORE_FROM_CONSOLE))    s_architectIgnoreLocalValidation = mode;
        if (mode & MM_IGNORE_ON)    dialogStd(DIALOG_OK, "Local Unlockable content validation temporarily ignored for Mission Maker. \"architect_ignore_unlockable_validation\" can also be used to disable for an entire session.", 0, 0, 0, 0, DLGFLAG_ARCHITECT );
    }
}
