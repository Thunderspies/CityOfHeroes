


#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>
#include "player/player.h"
#include "entity/entity.h"
#include "entity/EntPlayer.h"

#include "graphics/textureatlas.h"
#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiWindows.h"
#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiFocus.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTabControl.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "uiEdit.h"
#include "UI/uiPetition.h"
#include "UI/uiClipper.h"
#include "UI/uiOptions.h"
#include "UI/uiChat.h"
#include "UI/uiNet.h"
#include "UI/uiTarget.h"
#include "UI/uiLogin.h"
#include "UI/uidialog.h"
#include "uiScrollSelector.h"
#include "UI/uiLogin.h"
#include "UI/uiNet.h"
#include "UI/uiComboBox.h"
#include "UI/uiMissionMaker.h"
#include "UI/uiToolTip.h"
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include "UI/uiMissionSearch.h"

#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/textparserUtils.h>
#include <utilitieslib/utils/tokenstore.h>
#include "gameComm/npc.h"
#include "storyarc/pnpcCommon.h"
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/SimpleParser.h>
#include <utilitieslib/utils/structDefines.h>

#include "formatter/smf_main.h"
#include "entity/entclient.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiTeam.h"

#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"

#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"

#include "entity/CustomVillainGroup.h"
#include "entity/CustomVillainGroup_Client.h"
#include "filter/profanity.h"

#include "UI/uiBox.h"
#include "UI/uiClipper.h"
#include "UI/uiMissionMakerScrollSet.h"
#include "UI/uiPictureBrowser.h"
#include "uiHelpButton.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/language/AppLocale.h>
#include <zlib/zlib.h>

#include "AutoGen/playerCreatedStoryArcValidate_h_ast.h"
#include "AutoGen/uiMissionMakerScrollSet_h_ast.h"

static int s_CompressCrittersForTextFile = 0;
static TextAttribs s_taMissionTitle =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_MM_TITLE_OPEN,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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

static TextAttribs s_taMissionDisabled =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_TAB_INACTIVE,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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

HelpButton *mmTipStoryArc,  *mmTipMission1[5],  *mmTipMission2[5], * mmTipAddMission, *mmTipFileSize;
MMScrollSet missionMaker;

extern CustomVG **g_CustomVillainGroups;

void missionMakerSaveAsDialog(void *userdata);
void missionMakerTestStoryArc(PlayerCreatedStoryArc *pArc);

void missionMakerClear()
{
    int i;

    removeStoryArcCritters();
    updateCustomCritterList(&missionMaker);
    for( i = eaSize(&missionMaker.ppMission)-1; i>=0; i-- )
    {
        MMScrollSet_Mission *pMission = eaRemove(&missionMaker.ppMission, i);
        StructDestroy(parse_MMScrollSet_Mission, pMission);
    }

    for(i = eaSize(&missionMaker.ppStoryRegion)-1; i>= 0; i-- )
    {
        MMRegion *pRegion = eaRemove(&missionMaker.ppStoryRegion, i);
        if(pRegion->pb)
        {
            int j;
            for(j = 0; j < eaSize(&pRegion->pb); j++)
            {
                picturebrowser_destroy(pRegion->pb[j]);
            }
        }
        StructDestroy(parse_MMRegion, pRegion);
    }

    SAFE_FREE(missionMaker.pchFilename);
    memset( &missionMaker, 0, sizeof(MMScrollSet) );
}

void missionMakerNew(MMScrollSet *pSet)
{
    missionMakerClear();
    MMScrollSet_New(&missionMaker);
    missionMaker.current_mission = -1;
}

//------------------------------------------------------------------------------------------------------------------------
// Converstion Process - Converting from UI structs to PlayerCreatedStoryArc and back again
//------------------------------------------------------------------------------------------------------------------------

static int addCustomCritterToArc( PlayerCreatedStoryArc *pArc, PCC_Critter *pCritter )
{
    int i;

    for( i = 0; i < eaSize(&pArc->ppCustomCritters); i++ )
    {
        if (stricmp( pCritter->referenceFile, pArc->ppCustomCritters[i]->referenceFile ) == 0 )
            return i;
    }

    return eaPush(&pArc->ppCustomCritters,clone_PCC_Critter(pCritter));
}

static CompressedCVG * createCompressedVG(CustomVG *cvg, PlayerCreatedStoryArc *pArc)
{
    int i;
    CompressedCVG *returnCVG;
    returnCVG = StructAllocRaw(sizeof(CompressedCVG));
    returnCVG->displayName = StructAllocString(cvg->displayName);
    for (i = 0; i < eaSize(&cvg->dontAutoSpawnEVs); ++i)
    {
        eaPush(&returnCVG->dontAutoSpawnEVs, StructAllocString(cvg->dontAutoSpawnEVs[i]));
    }
    for (i = 0; i < eaSize(&cvg->dontAutoSpawnPCCs); ++i)
    {
        eaPush(&returnCVG->dontAutoSpawnPCCs, StructAllocString(cvg->dontAutoSpawnPCCs[i]));
    }
    for (i = 0; i < eaSize(&cvg->existingVillainList); ++i)
    {
        CVG_ExistingVillain *ev;
        ev = StructAllocRaw(sizeof(CVG_ExistingVillain));
        StructCopy(parse_ExistingVillain, cvg->existingVillainList[i], ev, 0, 0);
        eaPush(&returnCVG->existingVillainList, ev );
    }
    for (i = 0; i < eaSize(&cvg->customVillainList); ++i)
    {
        eaiPush(&returnCVG->customCritterIdx, addCustomCritterToArc(pArc, cvg->customVillainList[i])+1);
    }
    return returnCVG;
}
static void selectBaseCostumes(PlayerCreatedStoryArc *pArc, int **differenceChart, int **selectedCostumes)
{
    int i,j, min, minIndex;
    PCC_Critter *pcc;
    for (i = 0; i < eaSize(&pArc->ppCustomCritters); ++i)
    {
        for (j = (i+1); j < eaSize(&pArc->ppCustomCritters); ++j)
        {
            PCC_Critter * pcc2;
            int *diffParts;
            eaiCreate(&diffParts);
            pcc = getCustomCritterByReferenceFile(pArc->ppCustomCritters[i]->referenceFile);
            pcc2 = getCustomCritterByReferenceFile(pArc->ppCustomCritters[j]->referenceFile);
            if (pcc && pcc2)
            {
                costume_getDiffParts(costume_as_const(pcc->pccCostume), costume_as_const(pcc2->pccCostume), &diffParts);
                differenceChart[i][j] = differenceChart[j][i] = eaiSize(&diffParts);
            }
            else
            {
                //    this can happen on the first time the arc is loaded. The critters haven't been added to our critter list
                differenceChart[i][j] = 31;    //    MAX_BODY_PARTS + 1
            }
            eaiDestroy(&diffParts);
        }
        differenceChart[i][i] = 0;
    }

    //    for now, select the one with the least differences from the rest
    min = -1;
    minIndex = 0;
    for (i = 0; i < eaSize(&pArc->ppCustomCritters); ++i)
    {
        int currentDiffCount = 0;
        for (j = 0; j < eaSize(&pArc->ppCustomCritters); ++j)
        {
            currentDiffCount += differenceChart[i][j];
        }
        if ((currentDiffCount < min) || (min == -1))
        {
            min = currentDiffCount;
            minIndex = i;
        }
    }
    

    pcc = getCustomCritterByReferenceFile(pArc->ppCustomCritters[minIndex]->referenceFile);
    if (pcc)
    {
        eaiPushUnique(selectedCostumes, minIndex);
        eaPushConst(&pArc->ppBaseCostumes, costume_as_const( costume_clone(costume_as_const(pcc->pccCostume)) ));
    }
    else
    {
        //    Again, this can occur when arcs are loaded and the critters aren't in the local critter list yet.
        //    If that is the case, they just won't get any compression done
    }
}
static void compressPCCCostumes(PlayerCreatedStoryArc *pArc, int **differenceChart, int **selectedCostumes)
{
    int i;
    for ( i=0; i < eaSize(&pArc->ppCustomCritters); ++i)
    {
        int j;
        int currentMinDiff = -1;
        int bestIndex = 0;
        PCC_Critter *pcc;
        pcc = getCustomCritterByReferenceFile(pArc->ppCustomCritters[i]->referenceFile);
        if (!pcc)
        {
            continue;
        }
        for (j = 0; j < eaiSize(selectedCostumes); ++j)
        {
            if ((currentMinDiff == -1) || (differenceChart[(*selectedCostumes)[j]][i]))
            {
                currentMinDiff = differenceChart[(*selectedCostumes)[j]][i];
                bestIndex = j;
            }
        }
        if (pArc->ppBaseCostumes && EAINRANGE(bestIndex, pArc->ppBaseCostumes))
        {
            if (pArc->ppCustomCritters[i]->compressedCostume)
            {
                StructDestroy(ParseCostumeDiff, pArc->ppCustomCritters[i]->compressedCostume);
            }
            pArc->ppCustomCritters[i]->compressedCostume = StructAllocRaw(sizeof(CostumeDiff));
            costume_createDiffCostume(pArc->ppBaseCostumes[bestIndex], costume_as_const(pcc->pccCostume), pArc->ppCustomCritters[i]->compressedCostume);    
            pArc->ppCustomCritters[i]->compressedCostume->costumeBaseNum = bestIndex;
            if (pArc->ppCustomCritters[i]->pccCostume)
            {
                costume_destroy(pArc->ppCustomCritters[i]->pccCostume);
                pArc->ppCustomCritters[i]->pccCostume = NULL;
            }
        }
    }
}
void compressArcPCCCostumes(PlayerCreatedStoryArc *pArc)
{
    int i;
    int needsCompressing = 0;
    //    destroy old base costume
    if (!pArc)
    {
        return;
    }
    for (i = 0; i < eaSize(&pArc->ppCustomCritters); ++i)
    {
        if (pArc->ppCustomCritters[i]->pccCostume)                //    after files are compressed. pcc lose pccCostume and instead have compressedCostumes
        {                                                    //    if pcc still has its costume, it hasn't been compressed. recompress
            needsCompressing = 1;
            break;
        }
    }
    if (needsCompressing)
    {
        for (i = (eaSize(&pArc->ppBaseCostumes)-1); i >= 0; --i)
        {
            costume_destroy(costume_as_mutable_cast(pArc->ppBaseCostumes[i]));
            eaRemoveConst(&pArc->ppBaseCostumes, i);
        }
        for (i = ((eaSize(&pArc->ppCustomCritters))-1); i>=0; --i)
        {
            if (pArc->ppCustomCritters[i]->compressedCostume)
            {
                StructDestroy(ParseCostumeDiff, pArc->ppCustomCritters[i]->compressedCostume);
                pArc->ppCustomCritters[i]->compressedCostume = NULL;
            }
        }
        eaDestroyConst(&pArc->ppBaseCostumes);

        //    create compressed costumes
        if (eaSize(&pArc->ppCustomCritters))
        {
            int **differenceChart;
            int *selectedCostumes;
            differenceChart = malloc(sizeof(int*)*eaSize(&pArc->ppCustomCritters));
            for (i =0; i < eaSize(&pArc->ppCustomCritters); ++i)
            {
                differenceChart[i] = calloc(eaSize(&pArc->ppCustomCritters), sizeof(int));
            }
            eaiCreate(&selectedCostumes);
            selectBaseCostumes(pArc, differenceChart, &selectedCostumes);
            compressPCCCostumes(pArc, differenceChart, &selectedCostumes);
            eaiDestroy(&selectedCostumes);
            for (i = 0; i < eaSize(&pArc->ppCustomCritters); ++i)
            {
                free(differenceChart[i]);
            }
            free(differenceChart);
        }
    }
}
static int addCVGToArc(CustomVG *cvg, PlayerCreatedStoryArc *pArc)
{
    int i;
    if (!cvg->displayName)
    {
        return 0;
    }
    for (i = 0; i < eaSize(&pArc->ppCustomVGs); ++i)
    {
        if (stricmp(cvg->displayName, pArc->ppCustomVGs[i]->displayName) == 0)
        {
            StructDestroy(parse_CompressedCVG, pArc->ppCustomVGs[i]);
            eaRemove(&pArc->ppCustomVGs, i);
            eaInsert(&pArc->ppCustomVGs, createCompressedVG(cvg, pArc), i);
            return i+1;        //    add one because the CVG index is base 1 not base 0
        }
    }
    if (i >= eaSize(&pArc->ppCustomVGs))
    {
        return (eaPush(&pArc->ppCustomVGs, createCompressedVG(cvg,pArc)) + 1);        //    add one because the CVG index is base 1 not base 0
    }
    return 0;
}

char *encodeDangerousChars(char *str)
{
    char * pCursor = str, *pStart = str;
    static char * estr;

    if(!str)
        return 0;

    if(!estr)
        estrCreate(&estr);
    estrClear(&estr);

    while( *pCursor )
    {
        if (*pCursor == '<')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&lt;");
            pStart = pCursor+1;
        }
        else if (*pCursor == '>')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&gt;");
            pStart = pCursor+1;
        }
        else if (*pCursor == '%')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&#37;");
            pStart = pCursor+1;
        }
        pCursor++;
    }
    estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
    return estr;
}


static void ListToStruct( MMElementList *pList, void * pStruct, TokenizerParseInfo *tpi, PlayerCreatedStoryArc *pArc  ) 
{
    int i;

    if( pList->ppMultiList  && pList->current_list >= 0)
    {
        if(EAINRANGE( pList->current_list, pList->ppMultiList))
            ListToStruct( pList->ppMultiList[pList->current_list], pStruct, tpi, pArc );
    }
    else if( pList->pchField ) 
    {
        if( eaSize(&pList->ppElement) )
        {
            if( pList->bCheckBoxList )
            {
                for( i = 0; i < eaSize(&pList->ppElement); i++ )
                {
                    if( pList->ppElement[i]->bActive && pList->ppElement[i]->pchText )
                        SimpleTokenStore( pList->pchField, pList->ppElement[i]->pchText, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                }
            }
            else
            {    
                MMElement * pElement = pList->ppElement[pList->current_element];

                if ( ( stricmp( "CustomCritterIdx", pList->pchField ) == 0 ) ||    (stricmp("CustomCritterAndContactsIdx", pList->pchField) == 0) )
                {
                    if( pElement->pCritter )
                    {
                        int id = addCustomCritterToArc( pArc, pElement->pCritter );
                        char id_text[128];
                        itoa( id+1, id_text, 10 );
                        SimpleTokenStore( "CustomCritterIdx", id_text, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                    }
                }
                 else if (pList->eSpecialAction == kAction_BuildCustomVillainGroupList || (pList->eSpecialAction == kAction_BuildVillainGroupList && 
                        pList->ppElement[pList->current_element]->pchDisplayName && stricmp(textStd("MMSameAsBoss"), pList->ppElement[pList->current_element]->pchDisplayName)== 0))
                {
                    char *cvg_displayName = "None"; 
                    if (pList->eSpecialAction == kAction_BuildVillainGroupList && pList->pCrossLinkPointer)
                    {
                        MMElementList *pBossList = ((MMElementList*)pList->pCrossLinkPointer);
                        //    using the same as boss option and gotta add the cvg
                        if (EAINRANGE(pBossList->current_element, pBossList->ppElement) && pBossList->ppElement[pBossList->current_element] && pBossList->ppElement[pBossList->current_element]->pchText &&
                            (stricmp(pBossList->ppElement[pBossList->current_element]->pchText, "Custom") == 0 ) )
                        {
                            MMElementList *pBossGroupList = pBossList->ppElement[pBossList->current_element]->ppElementList[0];
                            if (pBossGroupList->current_element)
                            {
                                cvg_displayName = pBossGroupList->ppElement[pBossGroupList->current_element]->pchDisplayName;
                            }
                        }
                    }
                    else
                    {
                        cvg_displayName = pElement->pchDisplayName;
                    }
                    if (stricmp(cvg_displayName, "None") != 0)
                    {
                        int cvg_id, arc_id;
                        char id_text[128];
                        cvg_id = CVG_CVGExists(cvg_displayName);
                        if ( EAINRANGE( cvg_id, g_CustomVillainGroups ) )
                        {    
                            arc_id = addCVGToArc(g_CustomVillainGroups[cvg_id], pArc);
                            if (arc_id)
                            {
                                itoa( arc_id, id_text, 10);
                                if ( pList->pchField && stricmp(pList->pchField, "VillainGroup2") == 0 )
                                {
                                    SimpleTokenStore( "CustomVillainGroupIdx2", id_text, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                                }
                                else
                                {
                                    SimpleTokenStore( "CustomVillainGroupIdx", id_text, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                                }
                                SimpleTokenStore( pList->pchField, pElement->pchDisplayName, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                            }
                            else
                            {
                                char *errorCVGName;
                                estrCreate(&errorCVGName);
                                estrConcatf(&errorCVGName, "Custom Villain Group: \"%s\" ", pElement->pchDisplayName);
                                SimpleTokenStore( pList->pchField, errorCVGName, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                                estrDestroy(&errorCVGName);
                            }
                        }
                        else
                        {
                            SimpleTokenStore( pList->pchField, pElement->pchDisplayName, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                        }
                    }
                }
                else if ( pList->eSpecialAction == kAction_BuildDoppelEntityList )
                {
                    char *doppelString = estrTemp();
                    estrConcatStaticCharArray(&doppelString, DOPPEL_NAME);
                    if ( pElement->doppelFlags )
                    {
                        for (i = 1; i < VDF_COUNT; i = i << 1)
                        {
                            if (pElement->doppelFlags & i)
                                estrConcatCharString(&doppelString, StaticDefineIntRevLookup(ParseVillainDoppelFlags, i));
                        }
                    }
                    SimpleTokenStore( pList->pchField, doppelString, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                    estrDestroy(&doppelString);
                }
                else if ( stricmp( pElement->pchText, "None" ) != 0 )
                {
                    if( pList->eSpecialAction == kAction_BuildAmbushTrigger || pList->eSpecialAction == kAction_BuildDestinationList )
                        SimpleTokenStore( pList->pchField, smf_DecodeAllCharactersGet(pElement->pchText), tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                    else if( pElement->pchText  )
                        SimpleTokenStore( pList->pchField, pElement->pchText, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                }
                if ( pElement->pNPCCostume && validateCustomNPCCostume(pElement->pNPCCostume) )
                {
                    int id;
                    char id_text[128];
                    CustomNPCCostume *npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
                    StructCopy(ParseCustomNPCCostume,pElement->pNPCCostume, npcCostume, 0, 0);
                    id = eaPush(&pArc->ppNPCCostumes, npcCostume);
                    itoa( id+1, id_text, 10 );
                    SimpleTokenStore( "CustomNPCCostumeIdx", id_text, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                }
                if( pElement->npcIndexList && 
                    (stricmp( pList->pchField, "Contact" ) == 0 ||
                    stricmp( pList->pchField, "Entity" ) == 0))
                {
                    char pchSet[32];
                    int temp;
                    if(pElement->npcIndexList && pElement->npcIndex >=0 && pElement->npcIndex <eaiSize(&pElement->npcIndexList))
                    {
                        temp = pElement->npcIndex;
                    }
                    else 
                    {
                        temp = -1;
                    }
                    itoa( temp, pchSet, 10 );
                    if(stricmp( "Contact", pList->pchField ) == 0)
                        SimpleTokenStore("ContactCostumeIdx", pchSet, tpi, pStruct, 0 );
                    else
                        SimpleTokenStore("CostumeIdx", pchSet, tpi, pStruct, 0 );
                }

                if( pElement->setnum )
                {
                    char tmp[256], pchSet[32];
                    sprintf( tmp, "%sSet", pList->pchField );
                    itoa( pElement->setnum, pchSet, 10 );
                    SimpleTokenStore( tmp, pchSet, tpi, pStruct, 0 );
                }
            
                for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
                    ListToStruct( pElement->ppElementList[i], pStruct, tpi, pArc );
            }
        }
         else if ( pList->pBlock )
        {
            if( !( pList->pchDefault && ((stricmp( "Description", pList->pchField ) == 0) || (stricmp("AboutContact", pList->pchField) == 0)) && stricmp(textStd(pList->pchDefault),pList->pBlock->rawString) == 0) )    
            {
                if( pList->AllowHTML )
                    SimpleTokenStore( pList->pchField, (char*)removeLeadingWhiteSpaces(smf_DecodeAllCharactersGet(pList->pBlock->rawString)), tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                else
                    SimpleTokenStore( pList->pchField, (char*)removeLeadingWhiteSpaces(encodeDangerousChars(pList->pBlock->rawString)), tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );                
            }
        }
        else if( pList->pchDefault )
            SimpleTokenStore( pList->pchField, textStd(pList->pchDefault), tpi, pStruct, 0 );
    }
}
static void extractCritterFromArc(int critter_id, PlayerCreatedStoryArc *pArc, MMElementList *pList)
{
    if( EAINRANGE( critter_id, pArc->ppCustomCritters) )
    {
        if (pArc->ppCustomCritters[critter_id]->referenceFile)
        {
            PCC_Critter *pCritter;
            pCritter = getCustomCritterByReferenceFile(pArc->ppCustomCritters[critter_id]->referenceFile);
            if (pCritter == NULL)
            {
                int retVal;
                playerCreatedStoryArc_PCCSetCritterCostume( pArc->ppCustomCritters[critter_id], pArc);
                if (pArc->ppCustomCritters[critter_id]->compressedCostume)
                {
                    StructDestroy(ParseCostumeDiff, pArc->ppCustomCritters[critter_id]->compressedCostume);
                    pArc->ppCustomCritters[critter_id]->compressedCostume = NULL;
                }
                retVal = isPCCCritterValid(NULL, pArc->ppCustomCritters[critter_id],0,0);
                if ( (retVal == PCC_LE_VALID) || ( retVal > PCC_LE_MUST_EXIT ) )
                {
                    char fileName[MAX_PATH];
                    char *oldSource;
                    int i;
                    PCC_Critter *addReturnCritter = NULL;
                    //    to prevent double insertion of story arc critters
                    //    set the index to the negative critter_id (+1 for base 0 offset)
                    //    and use that ID to identify them
                    sprintf(fileName, "%s/%s", getCustomCritterDir(), pArc->ppCustomCritters[critter_id]->referenceFile);
                    oldSource = pArc->ppCustomCritters[critter_id]->sourceFile;
                    pArc->ppCustomCritters[critter_id]->sourceFile = fileName;
                    //    ParserWriteTextFile(fileName, parse_PCC_Critter, pArc->ppCustomCritters[critter_id], 0,0);
                    addReturnCritter = addCustomCritter(pArc->ppCustomCritters[critter_id], NULL);
                    pArc->ppCustomCritters[critter_id]->sourceFile = oldSource;
                    if (!addReturnCritter)
                    {
                        //    the addCustomCritter failed
                        return;
                    }
                    if ( pList )
                    {
                        updateCustomCritterList(&missionMaker);
                        for( i = 0; i < eaSize(&pList->ppElement); i++ )
                        {
                            MMElement * pElement = pList->ppElement[i];
                            if( pElement->pCritter && 
                                (stricmp( addReturnCritter->referenceFile, pElement->pCritter->referenceFile  )  == 0 ) )
                            {
                                pList->current_element = i;
                                pList->doneCritterValidation = 0;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                if (pList)
                {
                    int i;
                    updateCustomCritterList(&missionMaker);
                    //    there is already a critter w/ this reference(source file)
                    //    so assume they are referencing that one
                    for( i = 0; i < eaSize(&pList->ppElement); i++ )
                    {
                        MMElement * pElement = pList->ppElement[i];
                        if( pElement->pCritter && pElement->pCritter->referenceFile && 
                            (stricmp( pArc->ppCustomCritters[critter_id]->referenceFile, pElement->pCritter->referenceFile  ) == 0 ) )
                        {
                            pList->current_element = i;
                            pList->doneCritterValidation = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
}
static void StructToList( MMElementList *pList, void * pStruct, TokenizerParseInfo *tpi, PlayerCreatedStoryArc *pArc )
{
    int i;

    if( pList->ppMultiList ) 
    {
        for( i = 0; i < eaSize(&pList->ppMultiList); i++)
            StructToList( pList->ppMultiList[i], pStruct, tpi, pArc );
    }
     else if( pList->pchField )
    {
        if ( ( stricmp(pList->pchField, "CustomCritterIdx") == 0) || 
            ( stricmp(pList->pchField, "CustomCritterAndContactsIdx") == 0) )
        {
            int critter_id = -1;
            const char *str = SimpleTokenGet( "CustomCritterIdx", tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );

            if( str )
                critter_id = atoi(str)-1;
            extractCritterFromArc(critter_id, pArc, pList);
        }
        else if (pList->eSpecialAction== kAction_BuildCustomVillainGroupList)
        {
            int cvg_arc_id = -1;
            const char *str;
            if (pList->pchField && stricmp(pList->pchField, "VillainGroup2") == 0 )
            {
                str = SimpleTokenGet( "CustomVillainGroupIdx2", tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
            }
            else
            {
                str = SimpleTokenGet( "CustomVillainGroupIdx", tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
            }
            if (str)
            {
                cvg_arc_id = atoi(str)-1;
            }
            if ( EAINRANGE(cvg_arc_id, pArc->ppCustomVGs) && pArc->ppCustomVGs[cvg_arc_id]->displayName )
            {
                int cvg_list_idx;
                cvg_list_idx = CVG_CVGExists(pArc->ppCustomVGs[cvg_arc_id]->displayName);
                if (cvg_list_idx == -1)
                {
                    if (IsAnyWordProfaneOrCopyright(pArc->ppCustomVGs[cvg_arc_id]->displayName) == 0)
                    {
                        int itr;
                        CustomVG * newCVG;
                        newCVG = StructAllocRaw(sizeof(CustomVG));
                        newCVG->displayName = StructAllocString(pArc->ppCustomVGs[cvg_arc_id]->displayName);
                        for (itr = 0; itr < eaiSize(&pArc->ppCustomVGs[cvg_arc_id]->customCritterIdx); ++itr)
                        {
                            int critter_id;
                            PCC_Critter *pcc;
                            critter_id = pArc->ppCustomVGs[cvg_arc_id]->customCritterIdx[itr]-1;
                            extractCritterFromArc(critter_id, pArc, NULL);
                            pcc = getCustomCritterByReferenceFile(pArc->ppCustomCritters[critter_id]->referenceFile);
                            if (pcc)
                            {
                                if (stricmp(pcc->villainGroup, newCVG->displayName) != 0)
                                {
                                    eaPush(&newCVG->customVillainList, pcc);
                                }
                            }
                        }
                        for (itr = 0; itr < eaSize(&pArc->ppCustomVGs[cvg_arc_id]->existingVillainList); ++itr)
                        {
                            CVG_ExistingVillain *newVillain;
                            newVillain = StructAllocRaw(sizeof(CVG_ExistingVillain)); 
                            StructCopy(parse_ExistingVillain, pArc->ppCustomVGs[cvg_arc_id]->existingVillainList[itr], newVillain, 0,0);
                            eaPush(&newCVG->existingVillainList, newVillain);
                        }
                        eaPush(&g_CustomVillainGroups, newCVG);
                        updateCustomCritterList(&missionMaker);
                        for (itr = 0; itr < (eaSize(&pList->ppElement)); ++itr)
                        {
                            if (stricmp(newCVG->displayName,pList->ppElement[itr]->pchDisplayName) == 0 )
                            {
                                pList->current_element = itr;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    int list_itr;
                    updateCustomCritterList(&missionMaker);
                    for (list_itr = 0; list_itr < (eaSize(&pList->ppElement)); ++list_itr)
                    {
                        if (stricmp(g_CustomVillainGroups[cvg_list_idx]->displayName,pList->ppElement[list_itr]->pchDisplayName) == 0 )
                        {
                            pList->current_element = list_itr;
                            break;
                        }
                    }
                }
            }
            
        }
        else if ( pList->eSpecialAction == kAction_BuildDoppelEntityList )
        {
            const char *str = SimpleTokenGet( pList->pchField, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
            for( i = 0; i < eaSize(&pList->ppElement); i++ )
            {
                if( stricmp( pList->ppElement[i]->pchText, DOPPEL_NAME ) == 0)
                {
                    int j;
                    pList->current_element = i;
                    for (j = 1; j < VDF_COUNT; j = j << 1)
                    {
                        if (strstriConst(str, StaticDefineIntRevLookup(ParseVillainDoppelFlags, j)))
                        {
                            pList->ppElement[i]->doppelFlags |= j;
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            char *str = strdup(SimpleTokenGet( pList->pchField, tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) ));
            if( str )
            {
                char tmp[256];
                const char *pchSet;
                int setnum = 0;

                sprintf( tmp, "%sSet", pList->pchField );
                pchSet = SimpleTokenGet( tmp, tpi, pStruct, 0);
                if( pchSet )
                    setnum = atoi(pchSet);

                if ( pList->eSpecialAction == kAction_BuildCustomCritterList || pList->eSpecialAction == kAction_BuildCustomCritterAndContactList )
                {
                    for( i = 0; i < eaSize(&pList->ppElement); i++ )
                    {
                        if( stricmp( str, pList->ppElement[i]->pchText ) == 0 && pList->ppElement[i]->setnum == setnum )
                        {
                            pList->current_element = i;
                            break;
                        }
                    }
                    if (i >= eaSize(&pList->ppElement))
                    {
                        for (i = 0; i < eaSize(&pList->ppElement); i++)
                        {
                            if( stricmp( textStd("AllCritterListText"), pList->ppElement[i]->pchText ) == 0 && pList->ppElement[i]->setnum == setnum )
                            {
                                pList->current_element = i;
                                break;
                            }
                        }
                    }
                    for( i = 0; i < eaSize(&pList->ppElement[pList->current_element]->ppElementList); i++ )
                        StructToList( pList->ppElement[pList->current_element]->ppElementList[i], pStruct, tpi, pArc );
                }
                else if( eaSize(&pList->ppElement) && pList->eSpecialAction != kAction_BuildAmbushTrigger && pList->eSpecialAction != kAction_BuildDestinationList )
                {
                    MMElement * pElement;
                    const char * npcIndexC = SimpleTokenGet( "CostumeIdx", tpi, pStruct, 0);
                    int npcIndex = (npcIndexC)?atoi(npcIndexC):0;
                    
                    if(stricmp("Contact", pList->pchField) == 0 && !npcIndex)
                    {
                        npcIndexC = SimpleTokenGet( "ContactCostumeIdx", tpi, pStruct, 0);
                        npcIndex = (npcIndexC)?atoi(npcIndexC):0;
                    }
                    for( i = 0; i < eaSize(&pList->ppElement); i++ )
                    {
                        if( str && smf_DecodeAllCharactersGet(pList->ppElement[i]->pchText) && 
                            (stricmp( str, smf_DecodeAllCharactersGet(pList->ppElement[i]->pchText) ) == 0) 
                            && pList->ppElement[i]->setnum == setnum )
                        {
                            pList->current_element = i;
                            break;
                        }
                    }

                    pElement = pList->ppElement[pList->current_element];
                    if(npcIndex >=0 && npcIndex < eaiSize(&pElement->npcIndexList))
                    {
                        pElement->npcIndex =npcIndex;
                    }
                    else if(pElement->npcIndexList)
                        pElement->npcIndex = 0;

                    for( i = 0; i < eaSize(&pElement->ppElementList); i++ )
                        StructToList( pElement->ppElementList[i], pStruct, tpi, pArc );
                }
                else if ( pList->pBlock )
                {
                    if( pList->eSpecialAction == kAction_BuildAmbushTrigger )
                        pList->pchAmbushLoad = StructAllocString(str);
                    else if( pList->eSpecialAction == kAction_BuildDestinationList )
                        pList->pchDestinationLoad = StructAllocString(str);
                    else
                          smf_SetRawText(pList->pBlock, (char*)removeLeadingWhiteSpaces(encodeDangerousChars(str)), false);
                }
                if ( pList->eSpecialAction == kAction_BuildContactList || 
                     pList->eSpecialAction == kAction_BuildEntityListContact ||
                     pList->eSpecialAction == kAction_BuildBossEntityList ||
                     pList->eSpecialAction == kAction_BuildEntityList ||
                     pList->eSpecialAction == kAction_BuildGiantMonsterEntityList)
                {
                    int customNPCCostumeIdx = -1;
                    const char *str = SimpleTokenGet("CustomNPCCostumeIdx", tpi, pStruct, playerCreatedSDIgetByName(pList->eSpecialAction) );
                    if (str)
                        customNPCCostumeIdx = atoi(str)-1;
                    if (EAINRANGE(customNPCCostumeIdx, pArc->ppNPCCostumes) && validateCustomNPCCostume(pArc->ppNPCCostumes[customNPCCostumeIdx]) )
                    {
                        MMElementList *villainGroupList = NULL;
                        if (pList && pList->ppElement && pList->ppElement[pList->current_element] && 
                            pList->ppElement[pList->current_element]->ppElementList && pList->ppElement[pList->current_element]->ppElementList[0] )
                        {
                            villainGroupList = pList->ppElement[pList->current_element]->ppElementList[0];
                        }
                        if (villainGroupList && villainGroupList->ppElement && villainGroupList->ppElement[villainGroupList->current_element])
                        {
                            if (!villainGroupList->ppElement[villainGroupList->current_element]->pNPCCostume)
                            {
                                villainGroupList->ppElement[villainGroupList->current_element]->pNPCCostume = StructAllocRaw(sizeof(CustomNPCCostume));
                            }
                            StructCopy(ParseCustomNPCCostume, pArc->ppNPCCostumes[customNPCCostumeIdx], villainGroupList->ppElement[villainGroupList->current_element]->pNPCCostume, 0, 0);
                        }
                    }
                }
            }
            SAFE_FREE(str);
        }
    }
}

static void RegionToStruct( MMRegion *pRegion, void * pStruct, TokenizerParseInfo *tpi, PlayerCreatedStoryArc *pArc  )
{
    int i, j; 
    
    for( i = 0; i < eaSize(&pRegion->ppElementList); i++ )
        ListToStruct( pRegion->ppElementList[i], pStruct, tpi, pArc );

    for( i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
         MMRegionButton *pButton = pRegion->ppButton[i];
        for( j = 0; j < eaSize(&pButton->ppElementList); j++)
        {
            ListToStruct( pButton->ppElementList[j], pStruct, tpi, pArc );
        }
    }
}

static void StructToRegion( MMRegion *pRegion, void * pStruct, TokenizerParseInfo *tpi, PlayerCreatedStoryArc *pArc  )
{
    int i, j;

    for( i = 0; i < eaSize(&pRegion->ppElementList); i++ )
        StructToList( pRegion->ppElementList[i], pStruct, tpi, pArc );

     for( i = 0; i < eaSize(&pRegion->ppButton); i++ )
    {
        MMRegionButton *pButton = pRegion->ppButton[i];
        for( j = 0; j < eaSize(&pButton->ppElementList); j++)
        {
            StructToList(pButton->ppElementList[j], pStruct, tpi, pArc );
        }
    }
}


PlayerCreatedStoryArc * ConvertScrollSetToPlayerCreatedMission( MMScrollSet * pSet, int compressArcCritters )
{
    PlayerCreatedStoryArc * pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));
    int i, j;

    pArc->locale_id = getCurrentLocale();

    pArc->keywords = pSet->keywordpicks;

    for( i = 0; i < eaSize(&pSet->ppStoryRegion); i++ )
        RegionToStruct( pSet->ppStoryRegion[i], pArc, ParsePlayerStoryArc, pArc );

    for( i = 0; i < eaSize(&pSet->ppMission); i++ )
    {
        PlayerCreatedMission *pMission = StructAllocRaw(sizeof(PlayerCreatedMission));
        MMScrollSet_Mission *pScrollSetMission = pSet->ppMission[i];

        for( j = 0; j < eaSize(&pScrollSetMission->ppMissionRegion); j++ )
        {
            MMRegion *pRegion = pScrollSetMission->ppMissionRegion[j];
            RegionToStruct( pRegion, pMission, ParsePlayerCreatedMission, pArc );
        }
        for( j = 0; j < eaSize(&pScrollSetMission->ppDetailRegion); j++ )
        {
            MMRegion *pRegion = pScrollSetMission->ppDetailRegion[j];

            // Detail Parameters
             if( stricmp(pRegion->pchName, "DefeatAllTemplate")==0 )  
            {
                RegionToStruct( pRegion, pMission, ParsePlayerCreatedMission, pArc );
                //eaPush(&pMission->ppMissionCompleteDetails, StructAllocString(pRegion->ppElementList[0]->ppElement[pRegion->ppElementList[0]->current_element]->pchText) );
            }
            else
            {
                PlayerCreatedDetail *pDetail = StructAllocRaw(sizeof(PlayerCreatedDetail));
                char * pchReq = MMScrollSet_getValueFromRegion(pRegion, "MissionReqCheckBox", 0);
                eaPush(&pMission->ppDetail, pDetail);
                pDetail->eDetailType = pRegion->detailType;
                RegionToStruct( pRegion, pDetail, ParsePlayerCreatedDetail, pArc );

                if( pchReq && stricmp(pchReq, "Required")==0 && pDetail->pchName )
                    eaPush(&pMission->ppMissionCompleteDetails, StructAllocString(smf_DecodeAllCharactersGet(pDetail->pchName)));
            }
        }

        eaPush(&pArc->ppMissions, pMission);
    }

    if (compressArcCritters)
    {
        compressArcPCCCostumes(pArc);
    }

    return pArc;

};

void ConvertPlayerCreatedMissionToScrollSet(PlayerCreatedStoryArc *pArc, MMScrollSet *pSet)
{
    MMRegion * pRegion;
    int i, j, k;

    pSet->keywordpicks = pArc->keywords;

    for( i = 0; i < eaSize(&pSet->ppStoryRegion); i++ )
    {
        StructToRegion( pSet->ppStoryRegion[i], pArc, ParsePlayerStoryArc, pArc );
        MMScrollSet_SetCrossLinks(pSet->ppStoryRegion[i]);
    }
    
    for( i = 0; i < eaSize(&pArc->ppMissions); i++ )
    {
        PlayerCreatedMission *pMission = pArc->ppMissions[i];
        MMScrollSet_Mission *pScrollSetMission = MMScrollSet_AddMission(pSet);

          for( j = 0; j < eaSize(&pScrollSetMission->ppMissionRegion); j++ )
        {
            pRegion = pScrollSetMission->ppMissionRegion[j];
            StructToRegion( pRegion, pMission, ParsePlayerCreatedMission, pArc );

            //another hack for maps
            for( k = 0; k < eaSize(&pRegion->ppElementList); k++ )
                MMScrollSet_mapViewerInit( pRegion->ppElementList[k] );
        }

         for( j = 0; j < eaSize(&pMission->ppDetail); j++ )
        {
            pRegion = MMScrollSet_AddDetailRegion( pScrollSetMission, pMission->ppDetail[j]->eDetailType );
            pRegion->isOpen = 0;
            StructToRegion( pRegion, pMission->ppDetail[j], ParsePlayerCreatedDetail, pArc );
        }

        for( j = 0; j < eaSize(&pScrollSetMission->ppDetailRegion); j++ )
        {
            int required = 0;
            pRegion = pScrollSetMission->ppDetailRegion[j];
            MMScrollSet_SetCrossLinks(pRegion);

            // Update the the region name based off the detail name
            for( k = 0; k < eaSize(&pRegion->ppElementList); k++ )
            {
                MMElementList *pList = pRegion->ppElementList[k];
                if(pList->pchField && stricmp(pList->pchField, "Name") == 0)
                {
                    char *newName;
                    if( pRegion->pchUserName ) 
                        StructFreeString(pRegion->pchUserName);
                    if( pList->pBlock->outputString && *pList->pBlock->outputString )
                        pRegion->pchUserName = StructAllocString(pList->pBlock->outputString);
                    else
                        pRegion->pchUserName = StructAllocString(pList->pBlock->rawString);

                    if( pRegion->pchActualDisplay )
                        StructFreeString(pRegion->pchActualDisplay);

                    estrCreate(&newName);
                    estrConcatf(&newName, "%s: %s", textStd(pRegion->pchDisplayName), smf_DecodeAllCharactersGet(pRegion->pchUserName) );
                    pRegion->pchActualDisplay = StructAllocString(newName);
                    estrDestroy(&newName);
                }
            }

              for( k = 0; k < eaSize(&pMission->ppMissionCompleteDetails); k++ )
            {
                if( pMission->ppDetail[j]->pchName && stricmp( pMission->ppDetail[j]->pchName, pMission->ppMissionCompleteDetails[k] ) == 0)
                    required = 1;
            }

            // Unset Mission Requirement Checkboxes
            if(!required)
            {
                int x, y;
                for( k = 0; k < eaSize(&pRegion->ppButton); k++ )
                {
                    MMRegionButton *pButton = pRegion->ppButton[k];
                    for( x = 0; x < eaSize(&pButton->ppElementList); x++ )
                    {
                        MMElementList *pList = pButton->ppElementList[x];
                        if( pList->bCheckBoxList && pList->pchField && stricmp( pList->pchField, "MissionReqCheckBox") == 0 )
                        {
                            pList->pCrossLinkPointer = pRegion;
                            for( y = 0; y < eaSize(&pList->ppElement); y++)
                                pList->ppElement[y]->bActive = 0;
                        }
                    }
                }
            }
        }

         for( k = 0; k < eaSize(&pMission->ppMissionCompleteDetails); k++ )
        {
            if( stricmp( pMission->ppMissionCompleteDetails[k], "DefeatAllVillains") == 0 ) 
            {
                pRegion = MMScrollSet_AddDetailRegion( pScrollSetMission, kDetail_DefeatAll );
                pRegion->ppElementList[0]->current_element = 0;
                pRegion->isOpen = 0;
                if( pMission->pchDefeatAll && pRegion->ppButton[0]->ppElementList[0]->pBlock )
                    smf_SetRawText(pRegion->ppButton[0]->ppElementList[0]->pBlock, (char*)removeLeadingWhiteSpaces(encodeDangerousChars(pMission->pchDefeatAll)), false);
                
            }

            if( stricmp( pMission->ppMissionCompleteDetails[k], "DefeatAllVillainsInEndRoom") == 0 )
            {
                pRegion = MMScrollSet_AddDetailRegion( pScrollSetMission, kDetail_DefeatAll );
                pRegion->ppElementList[0]->current_element = 1;
                pRegion->isOpen = 0;
                if( pMission->pchDefeatAll && pRegion->ppButton[0]->ppElementList[0]->pBlock )
                    smf_SetRawText(pRegion->ppButton[0]->ppElementList[0]->pBlock, (char*)removeLeadingWhiteSpaces(encodeDangerousChars(pMission->pchDefeatAll)), false);
            }
        }

        MMScrollSet_updateAmbushTrigger(pScrollSetMission);
    }

    updateCustomCritterList(pSet);
}


void missionMakerSetFromPlayerCreatedStoryArc(PlayerCreatedStoryArc* pArc, char * pchFile)
{
    missionMakerNew(&missionMaker);
    SAFE_FREE(missionMaker.pchFilename);
    if(pchFile)
        missionMaker.pchFilename = strdup(pchFile);

    ConvertPlayerCreatedMissionToScrollSet(pArc, &missionMaker);
}

void missionMakerSaveAs( char * pchFileName )
{
    PlayerCreatedStoryArc *pArc;

    pArc = ConvertScrollSetToPlayerCreatedMission(&missionMaker, s_CompressCrittersForTextFile);
    if( playerCreatedStoryArc_Save( pArc, getMissionPath(pchFileName)) )
        missionMaker.fSaveMessageTimer = 30;
    else
        missionMaker.fSaveMessageTimer = -30;
    missionsearch_myArcsChanged(0,0);
    StructDestroy(ParsePlayerStoryArc, pArc);
}

void missionMakerSave(int flags)
{
    dialogRemove( 0, missionMakerDialogDummyFunction, 0 );

     if( missionMaker.pchFilename && !(flags&MM_SAVEAS) ) 
    {
        missionMakerSaveAs(missionMaker.pchFilename);
        if(flags&MM_TEST)  
            missionMakerTestStoryArc(0);
        if(flags&MM_EXIT)
            missionMakerExitDialog(0);
    }
    else
    {
        if( missionMaker.pchFilename )
            dialogSetTextEntry(missionMaker.pchFilename);
        dialogStdCB(DIALOG_OK_CANCEL_TEXT_ENTRY, "MMEnterFileName", NULL, NULL, missionMakerSaveAsDialog, NULL, DLGFLAG_ARCHITECT, S32_TO_PTR(flags));
    }
}

void missionMakerFixErrors(int unused)
{
    dialogRemove( 0, missionMakerDialogDummyFunction, 0 );
    missionMaker.bShowErrors = 1;
    missionMaker.bShowWrongness = 1;
    missionMaker.fErrorBlinkTimer = 18;
}

void missionMakerRepublish(void)
{
    Entity *e = playerPtr();
    if(e && e->pl && AccountCanPublishOnMissionArchitect(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        PlayerCreatedStoryArc *pArc = ConvertScrollSetToPlayerCreatedMission(&missionMaker, 1);
        if( pArc )
        {
            char *storystr = playerCreatedStoryArc_ToString(pArc);
            //missionserver_game_publishArc(missionMaker.arc_id, storystr);
            MissionSearch_requestPublish(missionMaker.arc_id, storystr);
            MissionSearch_publishRequestUpdate();
            missionMakerExitDialog(0);
        }
    }
    else
    {
        dialogStd(DIALOG_OK, textStd("InvalidPermissionTier7"), NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
    }
}

void missionMakerOpenWithArc(PlayerCreatedStoryArc *pArc, char *pchFileName, int arc_id, int owned)
{
    if(pArc)
    {
        missionMakerSetFromPlayerCreatedStoryArc(pArc, pchFileName);
        missionMaker.arc_id = arc_id;
        missionMaker.owned = owned;
    }
    else
    {
        missionMakerNew(&missionMaker);
        MMScrollSet_AddMission(&missionMaker);
    }

    if(window_getMode(WDW_MISSIONMAKER) != WINDOW_DISPLAYING)
        window_setMode(WDW_MISSIONMAKER, WINDOW_GROWING);
}




//------------------------------------------------------------------------------------------------------------------------
// Dialog Callbacks
//------------------------------------------------------------------------------------------------------------------------

void missionMakerOverwrite( char * pchFile )
{
    SAFE_FREE(missionMaker.pchFilename);
    missionMaker.pchFilename = pchFile;
    missionMaker.arc_id = 0; // if you were working on a published arc, you are no longer
    missionMaker.owned = 0;
    missionMakerSave( 0 );
}

void missionMakerDontOverwrite( char * pchFile )
{
    SAFE_FREE(pchFile);
}

void missionMakerSaveAsDialog(void *userdata)
{
    int flags = PTR_TO_S32(userdata);
    char *filename = dialogGetTextEntry();
    if(filename && filename[0])
    {
        FILE * open_test = fopen( getMissionPath(filename), "rb" );
        if ( open_test )
        {
            dialogStdCB( DIALOG_YES_NO, "FileExists", NULL, NULL, missionMakerOverwrite, missionMakerDontOverwrite, DLGFLAG_ARCHITECT, strdup(filename) );
            fclose(open_test);
            return;
        }

        SAFE_FREE(missionMaker.pchFilename);
        missionMaker.pchFilename = strdup(filename);
        missionMaker.arc_id = 0; // if you were working on a published arc, you are no longer
        missionMaker.owned = 0;
        missionMakerSave( flags & (~MM_SAVEAS) );
    }
}

static void missionMakerTestStoryArcAccept(PlayerCreatedStoryArc *pArc)
{
    char * error;
    char *errors2;
    estrCreate(&error);
    estrCreate(&errors2);

    if( pArc && playerCreatedStoryarc_GetValid(pArc, 0, &error, &errors2, playerPtr(), 0) !=kPCS_Validation_FAIL )
    {
        sendTestStoryArc(pArc);
        if( isDevelopmentMode() )
            cmdParse( "ContactDialog Player_Created/MissionArchitectContact.contact" );
        StructDestroy(ParsePlayerStoryArc, pArc);
        window_setMode(WDW_MISSIONMAKER, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING);
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
    }
    else if( strlen(error) )
        dialogStd(DIALOG_OK, error, 0, 0, 0, 0, DLGFLAG_ARCHITECT );

    estrDestroy(&error);
    estrDestroy(&errors2);
}

static void missionMakerTestStoryArcCancel(PlayerCreatedStoryArc *pArc)
{
    if(pArc)
        StructDestroy(ParsePlayerStoryArc, pArc);
}

static void quitTFAndContinue(PlayerCreatedStoryArc *pArc)
{
    Entity *e = playerPtr();
    cmdParse("team_quit_internal");
    e->pl->taskforce_mode = 0;
    missionMakerTestStoryArc(pArc);
}

void missionMakerTestStoryArc(PlayerCreatedStoryArc *pArc)
{
    Entity *e = playerPtr();
    char * error;
    char * errors2;
    
    if(!pArc)
        pArc = ConvertScrollSetToPlayerCreatedMission(&missionMaker, 1);
    estrCreate(&error);
    estrCreate(&errors2);
    if( playerCreatedStoryarc_GetValid(pArc, 0, &error, &errors2, e, 0) != kPCS_Validation_OK )
    {
        dialogStd(DIALOG_OK, error, 0, 0, 0, 0, DLGFLAG_ARCHITECT );
    }
    else
    {
        if( e->pl->taskforce_mode )
        {
             dialog(DIALOG_TWO_RESPONSE, -1, -1, 600, -1,"MustLeaveTaskForce", "CancelString", 0, "QuitTFAndContinue", quitTFAndContinue, DLGFLAG_ARCHITECT, 0, 0, 0, 0, 0, pArc );
        }
        else
        {
            dialogStdCB(DIALOG_ACCEPT_CANCEL, "EnteringArchitectTaskForceTestWarning", 0, 0,
                        missionMakerTestStoryArcAccept, missionMakerTestStoryArcCancel,    DLGFLAG_ARCHITECT, pArc );
            
        }
    }
    estrDestroy(&error);
    estrDestroy(&errors2);
}

void missionMakerExitDialog(void *userdata)
{
    window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
    window_setMode(WDW_MISSIONMAKER, WINDOW_SHRINKING);
    window_setMode(WDW_MISSIONSEARCH, WINDOW_GROWING);
}

void missionMakerDialogDummyFunction(void *unused){ return; }
//------------------------------------------------------------------------------------------------------------------------
// UI Functions
//------------------------------------------------------------------------------------------------------------------------

void mmscrollsetViewList( char * indexPath, int toggle_region )
{
    char *index; 
    int idx;
    MMScrollSet *pSet = &missionMaker;
    MMRegion ** ppRegion;

#define NEXT_IDX    index = strtok(NULL, ".");    \
    if( !index )                \
    return;                     \
    idx = atoi(index);            \

    index = strtok(indexPath, ".");    
    if( !index )                    
        return;                        
    idx = atoi(index);    

    if( idx == 0 ) // StoryArc
    {
        pSet->current_mission = -1;
        ppRegion = pSet->ppStoryRegion;
    }
    else if (EAINRANGE(idx-1, pSet->ppMission))
    {
        pSet->current_mission = idx-1;

        NEXT_IDX

        if( idx == 0 || idx == 1 )
            pSet->current_page = idx;

        if( pSet->current_page  )
            ppRegion = pSet->ppMission[pSet->current_mission]->ppDetailRegion;
        else
            ppRegion = pSet->ppMission[pSet->current_mission]->ppMissionRegion;
    }

    NEXT_IDX

    if(EAINRANGE(idx,ppRegion))
    {
        MMRegion * pRegion = ppRegion[idx];
        MMElementList **ppList = NULL;

        if( toggle_region )
        {
            pRegion->isOpen = !pRegion->isOpen;
            return;
        }
        else
            pRegion->isOpen = 1;

        NEXT_IDX

        if( idx == 0 )
            ppList = pRegion->ppElementList;
        else if(EAINRANGE(idx-1,pRegion->ppButton))
        {
            ppList = pRegion->ppButton[idx-1]->ppElementList;
            pRegion->ppButton[idx-1]->isOpen = 1;
        }

        if(!ppList)
            return;

        NEXT_IDX

        if(EAINRANGE(idx, ppList))  
        {
            MMElementList *pList = ppList[idx];

            // recurse the list now
            index = strtok(NULL, ".");
            while(index)
            {
                idx = atoi(index);    
                if( pList->ppElement && pList->ppElement[pList->current_element]->ppElementList && EAINRANGE(idx, pList->ppElement[pList->current_element]->ppElementList) )
                    pList = pList->ppElement[pList->current_element]->ppElementList[idx];
                index = strtok(NULL, ".");
            }

            if( pList->pBlock )
                smf_SelectAllText( pList->pBlock );

            pSet->pTargetList = pList;
               pSet->fBlinkTimer = 18;
            pSet->converge = 1;
        }
    }
}


//------------------------------------------------------------------------------------------------------------------------
// UI Functions
//------------------------------------------------------------------------------------------------------------------------
typedef enum MMControlType
{
    kMMControl_Save        = (1<<0),
    kMMControl_SaveAs      = (1<<1),
    kMMControl_SaveTest    = (1<<2),
    kMMControl_SaveExit    = (1<<3),
    kMMControl_ExitAbandon = (1<<4),
    kMMControl_FixErrors   = (1<<5),
    kMMControl_Ready       = (1<<6),
}MMControlType;

typedef struct MMControl
{
    MMControlType eType;
    void (*code)(int);
    char * pchText;
    void (*republishCode)(int);
    char * pchRepublishText;
    char * pchIcon;
    char * pchRollover;
    char * pchOutline;
    AtlasTex *icon;
    AtlasTex *rollover;
    int (*republishWait)();
}MMControl;

static void mmSave(int unused){  missionMakerSave(0); }
static void mmSaveAs(int unused){  missionMakerSave(MM_SAVEAS); }
static void mmExitAbandon(int unused){ missionMakerExitDialog(0); }

static void mmSaveAndTest(int ready)
{          
    if(!ready)
        missionMakerFixErrors(0);
    else
        missionMakerSave(MM_TEST);
}

static void mmSaveAndExit(int unused)
{
    missionMakerSave(0);
    missionMakerExitDialog(0);
}

static void mmRepublish(int ready)
{
    if( !ready )
        missionMakerFixErrors(0);
    else
        missionMakerRepublish();
}

static void mmRepublishAndPlay(int ready)
{
    if( !ready )
        missionMakerFixErrors(0);
    else
        missionMakerRepublish();
    missionsearch_playPublishedArc(missionMaker.arc_id, 0);
}

static void mmRepublishAndExit(int ready)
{
    if( !ready )
        missionMakerFixErrors(0);
    else
        missionMakerRepublish();
    missionMakerExitDialog(0);
}

MMControl s_mmControls[] = 
{    
    { kMMControl_Save,        mmSave,            "MMSave",            mmRepublish,           "MMRepublish",            "SaveCheck",    "SaveCheck__rollover_glow", 0,0,0,MissionSearch_publishWait},
    { kMMControl_SaveAs,      mmSaveAs,            "MMSaveAs",            mmSaveAs,               "MMSaveLocally",            "SaveAsCheck",    "SaveAsCheck_rollover_glow"},
    { kMMControl_SaveTest,    mmSaveAndTest,    "MMSaveAndTest",    mmRepublishAndPlay,       "MMRepublishAndTest",    "SaveTest",        "SaveTest_glow", 0,0,0,MissionSearch_publishWait},
    { kMMControl_SaveExit,    mmSaveAndExit,    "MMSaveAndExit",    mmRepublishAndExit,       "MMRepublishAndExit",    "SaveExit",        "SaveExit_glow", 0,0,0,MissionSearch_publishWait},
    { kMMControl_ExitAbandon, mmExitAbandon,    "MMExitAndAbandon",    mmExitAbandon,           "MMExitAndAbandon",        "SaveAbandon",    "SaveAbandon_glow"},
    { kMMControl_FixErrors,   missionMakerFixErrors, "MMFixErrors",    missionMakerFixErrors, "MMFixErrors",            "ShowErrors",    "ShowErrors_glow"},
};

#define CONTROL_WD 50

static int drawControlIcon(F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, MMControl * pControl, int storyarc_ready, int dialog )
{
    CBox box;
    static SMFBlock smfBlock={0};
    int rgbaHit, rgbaOpen;
    int locked = 0;
    int secondsTillUnlocked = (pControl->republishWait)?pControl->republishWait():0;
    char * btnText = 0;
    char * estr;

    btnText = textStd(missionMaker.arc_id?pControl->pchRepublishText:pControl->pchText);

    if( !missionMaker.arc_id ||    (!secondsTillUnlocked && (missionMaker.owned || !pControl->republishWait || playerPtr()->access_level >= 9)) ) 
    {
        rgbaHit = CLR_MM_TITLE_LIT;
        rgbaOpen = CLR_MM_TITLE_OPEN;

        if( pControl->eType == kMMControl_FixErrors )
        {
            rgbaHit = CLR_MM_BUTTON_ERROR_TEXT_LIT;
            rgbaOpen = CLR_MM_TEXT_ERROR;
        }
    }
    else
    {
        rgbaOpen = rgbaHit = CLR_TAB_INACTIVE;
        locked = 1;
    }

    if( dialog )
    {    
        F32 text_ht = 0;
        estrCreate(&estr);
        BuildCBox(&box, x, y, wd, ht); 
           if( mouseCollision(&box) )
        {
            display_sprite( pControl->rollover, x + 10*sc, y, z, sc, sc, rgbaHit );
            if( pControl->eType == kMMControl_FixErrors )
                estrConcatStaticCharArray(&estr, "<font face=computer outline=0 color=#ffff88><b><scale 1.8>" );
            else
                estrConcatf(&estr, "<font face=computer outline=0 color=#%06x><b><scale 1.8>", (CLR_MM_TITLE_LIT>>8) );
        }
        else
        {
            display_sprite( pControl->icon, x + 10*sc, y, z, sc, sc, rgbaOpen );
            if( pControl->eType == kMMControl_FixErrors )
                estrConcatStaticCharArray(&estr, "<font face=computer outline=0 color=ArchitectError><b><scale 1.8>" );
            else
                estrConcatStaticCharArray(&estr, "<font face=computer outline=0 color=ArchitectTitle><b><scale 1.8>" );
        }

         s_taMissionTitle.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
        estrConcatf(&estr, "<span align=center>%s</span>", btnText );
        text_ht = smf_ParseAndFormat(&smfBlock, estr, 0, 0, 0, wd - pControl->icon->width*sc - 20*sc, 0, 0, 1, 1, &s_taMissionTitle, 0);
         smf_ParseAndDisplay(&smfBlock, estr, x + pControl->icon->width*sc + 10*sc, y + 20*sc - text_ht/2, z, wd - pControl->icon->width*sc - 20*sc , 0, 0, 0, &s_taMissionTitle, 0, 0, 0);
        estrDestroy(&estr);
    }
    else
    {
           BuildCBox(&box, x, y, CONTROL_WD*sc, ht); 
        if( mouseCollision(&box) && !locked)
        {
            estrCreate(&estr);
        
            display_sprite( pControl->rollover, x + (CONTROL_WD - pControl->rollover->width)*sc/2, y, z, sc, sc, rgbaHit );
            s_taMissionTitle.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
            estrConcatf(&estr, "<span align=center>%s</span>", btnText );
            smf_SetRawText(&smfBlock, estr, 0);
            smf_Display(&smfBlock, x - CONTROL_WD*sc , y + pControl->icon->height*sc, z, 3*CONTROL_WD*sc, 0, 0, 0, &s_taMissionTitle, 0);
            estrDestroy(&estr);
        }
        else
        {
            display_sprite( pControl->icon, x + (CONTROL_WD - pControl->icon->width)*sc/2, y, z, sc, sc, rgbaOpen );
            if(mouseCollision(&box))
            {
                char * estr = estrTemp();
                estrConcatf(&estr, "<span align=center>%s</span>", btnText);
                smf_SetRawText(&smfBlock, estr, 0);
                smf_Display(&smfBlock, x - CONTROL_WD*sc , y + pControl->icon->height*sc, z, 3*CONTROL_WD*sc, 0, 0, 0, &s_taMissionDisabled, 0);
                estrDestroy(&estr);
            }
        }
    }

    if( mouseClickHit(&box, MS_LEFT) && !locked)
    {
        if( missionMaker.arc_id )
            pControl->republishCode(storyarc_ready);
        else
            pControl->code(storyarc_ready);
        return 1;
    }
    else if(mouseClickHit(&box, MS_LEFT))
    {
        if(secondsTillUnlocked)
            dialogStd(DIALOG_OK, textStd("MissionSearchCantPublish"), NULL, NULL, NULL, NULL, 0);
        else
            dialogStd(DIALOG_OK, textStd("MissionSearchCantPublishDisabled"), NULL, NULL, NULL, NULL, 0);
    }

    return 0;
}


int missionMaker_drawDialogOptions( F32 x, F32 y, F32 z, F32 sc, F32 wd, int options )
{
    int i, count = 0, retval = 0;
     for( i = 0; i < ARRAY_SIZE(s_mmControls); i++ ) 
    {
        if( options & s_mmControls[i].eType )
        {
            retval |= drawControlIcon( x, y + 50*sc*count, z, sc, wd, 50*sc, &s_mmControls[i], options&kMMControl_Ready, 1 );
            count++;
        }
    }
    return retval;
}

static void missionMakerControls( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, int storyarc_ready ) 
{
    static int texInit;
    static AtlasTex *error, *error_glow, *error_out;
    int i;
    CBox box;
    char * estr;
    static SMFBlock smfBlock={0};
     AtlasTex *white = atlasLoadTexture("white");
    F32 per_space = (wd-2*R10*sc)/6.f-4*sc;


    if(!texInit)
    {
        for( i = ARRAY_SIZE(s_mmControls)-1; i>=0; i-- ) 
        {
            s_mmControls[i].icon = atlasLoadTexture( s_mmControls[i].pchIcon );
            s_mmControls[i].rollover = atlasLoadTexture( s_mmControls[i].pchRollover );
        }
        error = atlasLoadTexture( "ShowErrors" );
        error_glow = atlasLoadTexture( "ShowErrors_glow" );
        error_out = atlasLoadTexture( "ShowErrors_outline" );
    }

     for( i = 0; i < ARRAY_SIZE(s_mmControls)-1; i++ ) // -1 is because we don't want to display the errror button here
        drawControlIcon( x + per_space/2 - 20*sc + i*CONTROL_WD*sc, y, z, sc, 0, ht, &s_mmControls[i], storyarc_ready, 0 );

    // error button
       BuildCBox(&box, x + wd - 170*sc, y, CONTROL_WD+100*sc, CONTROL_WD );
    estrCreate(&estr);

    z+= 10;

      if( storyarc_ready && !missionMaker.bWarnings  )
    {
        estrConcatf(&estr, "<span align=right><font outline=0 color=#aaaaaa><b>%s</b></color></span>", textStd("MMNoErrors"));
        display_sprite( error, x+ wd - 70*sc, y, z, sc, sc, CLR_GREY );
    }
    else
    {

        if( missionMaker.bShowErrors )
        {
            if( storyarc_ready )
                estrConcatf(&estr, "<span align=right><color ArchitectErrorLit>%s</color>", textStd("HideWarningsString"));
            else
                estrConcatf(&estr, "<span align=right><color ArchitectErrorLit>%s</color>", textStd("HideErrorsString"));
        }
        else
        {
            if( storyarc_ready )
                estrConcatf(&estr, "<span align=right><color ArchitectErrorLit>%s</color>", textStd("ShowWarningsString"));
            else
                estrConcatf(&estr, "<span align=right><color ArchitectError>%s</color>", textStd("ShowErrorsString"));
        }

        if( mouseCollision(&box) )
        {
            display_sprite( error_glow, x+ wd - 70*sc, y, z, sc, sc, CLR_MM_BUTTON_ERROR_TEXT_LIT );
            display_sprite( error_out, x+ wd - 70*sc, y, z, sc, sc, CLR_MM_BUTTON_ERROR_LIT );
        }
        else
        {
              if( missionMaker.bShowErrors )
            {
                // error has a hole in the middle we don't want to show up, this white texture is essentially a patch
                   display_sprite( white, x+ wd - 61*sc, y+8*sc, z, 3*sc, 2.5*sc,  CLR_MM_ERROR_MID);
                display_sprite( error, x+ wd - 70*sc, y, z, sc, sc,  CLR_MM_BUTTON_ERROR_TEXT_LIT);
            }
            else
                display_sprite( error, x+ wd - 70*sc, y, z, sc, sc,  storyarc_ready?CLR_MM_BUTTON_ERROR_TEXT_LIT:CLR_MM_TEXT_ERROR);

            display_sprite( error_out, x+ wd - 70*sc, y, z, sc, sc,  CLR_MM_BUTTON_ERROR);
        }
    }

       s_taMissionTitle.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    s_taMissionDisabled.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    smf_SetRawText(&smfBlock, estr, 0);
       smf_Display(&smfBlock, x + wd - 170*sc, y + 10*sc - (missionMaker.bShowErrors?5*sc:5*sc), z, 100*sc, 0, 0, 0, &s_taMissionTitle, 0);
     estrDestroy(&estr);

    if( mouseClickHit(&box, MS_LEFT) )
    {
        missionMaker.bShowErrors = !missionMaker.bShowErrors;
        
    }
}

static int drawTrackIcon( CBox *box, F32 x, F32 y, F32 z, F32 sc, AtlasTex *icon, AtlasTex *rollover, AtlasTex *glow, int flip, int selected )
{
    static F32 timer;

    y -= 2*sc; 
    if(mouseCollision(box))  
    {
        display_spriteFlip( rollover, x - rollover->width*sc/2, y - rollover->height*sc/2, z+1, sc, sc, CLR_WHITE, flip );
        display_spriteFlip( glow, x - glow->width*sc/2, y - glow->height*sc/2, z, sc, sc, CLR_MM_TITLE_LIT, flip );
    }
    else
           display_spriteFlip( icon, x - icon->width*sc/2, y - icon->height*sc/2, z+1, sc, sc, CLR_WHITE, flip );

     if( selected )
    {
          timer += TIMESTEP/8;
        sc *= (1+(sinf(timer)*.05f));
        display_spriteFlip( glow, x - glow->width*sc/2, y - glow->height*sc/2, z, sc, sc, CLR_MM_TITLE_OPEN, flip );
    }
    return mouseClickHit(box, MS_LEFT);
}

void missionMakerNav( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht )
{
    F32 per_space = (wd-2*R10*sc)/6.f-4*sc, sin_val;
    static AtlasTex *track_arrow, *track_mid, *track_end, *storyarc, *storyarc_roll, *storyarc_glow, *mission;
    static AtlasTex    *mission_roll, *mission_glow, *add_mission, *add_mission_roll, *add_mission_glow;
    static int texinit;
    int i, color_left, color_right;
    static F32 timer;
    CBox cbox;

     drawHorizontalLine(x+2*R10*sc,y+ht, wd-4*R10*sc, z, sc, 0xffffff44);

    if( !texinit )
    {
        track_arrow = atlasLoadTexture("RaceTrackBar_finish_end");
        track_mid = atlasLoadTexture("RaceTrackBar_mid");
        track_end = atlasLoadTexture("RaceTrackBar_start_end");

        storyarc = atlasLoadTexture("MissionPen");
        storyarc_roll = atlasLoadTexture("MissionPen_pen_rollever");
        storyarc_glow = atlasLoadTexture("MissionPen_rollover_glow");
          
        mission = atlasLoadTexture("OpenMissionBook_half");
        mission_roll = atlasLoadTexture("OpenMissionBook_half_rollover");
        mission_glow = atlasLoadTexture("OpenMissionBook_half_rollover_glow");

        add_mission = atlasLoadTexture("ClosedMissionBook");
        add_mission_roll = atlasLoadTexture("ClosedMissionBook_rollover");
        add_mission_glow = atlasLoadTexture("ClosedMissionBook_rollover_glow");
        texinit = 1;
    }

    font( &title_12 );

      timer += TIMESTEP/32; 
    sin_val = (sinf(timer)+1)/2;
      interp_rgba( sin_val, CLR_MM_BACK_DARK&0xffffff44, 0xffffff44, &color_left );
    sin_val = (sinf(timer - PI/2)+1)/2; 
    interp_rgba( sin_val, CLR_MM_BACK_DARK&0xffffff44, 0xffffff44, &color_right );
 
       display_sprite(track_end, x + per_space/2 - 20*sc, y + 13*sc, z, sc, sc, color_left );
      if(eaSize(&missionMaker.ppMission)<5) 
    {
           display_sprite_blend(track_mid, x + per_space/2 - 20*sc + track_end->width*sc, y + 13*sc, z, ((per_space*(2+eaSize(&missionMaker.ppMission)))-track_end->width*sc + 20*sc - per_space/2)/track_mid->width, 
                sc, color_left, color_right, color_right, color_left );
          display_sprite(track_arrow, x + per_space*(2+eaSize(&missionMaker.ppMission)), y + 13*sc, z, sc, sc, color_right );
    }
    else
    {
        display_sprite_blend(track_mid, x + per_space/2 - 20*sc + track_end->width*sc, y + 13*sc, z, (wd - (per_space/2 - 20*sc + track_end->width*sc+2*R10*sc))/track_mid->width, 
                 sc, color_left, color_right, color_right, color_left );
    }

    for( i = 0; i < 6; i++ )
    {
        CBox nav;
        int nc = 0xffffff22, text_clr = CLR_MM_TITLE_OPEN;
        F32 tx = x+i*(per_space+4*sc)+R10*sc+2*sc;

         BuildCBox(&nav, tx, y+R10*sc, per_space, ht-2*R10*sc );
        if( mouseCollision(&nav) )
            text_clr = CLR_MM_TITLE_LIT;
        if( i == 0 )
        {
            if( missionMaker.bShowWrongness && !MMScrollSet_isStoryArcValid(&missionMaker, 0) )
                text_clr = CLR_MM_TEXT_ERROR; 

            //helpButton( &mmTipStoryArc, tx + per_space - 15*sc, y + ht/2, z+15, sc, "MMTipStoryArc", &cbox );

            font(&title_12);
            font_color(text_clr,text_clr);
             cprnt( tx + per_space/2, y + ht, z+1, sc, sc, "MMStoryArcTitle", &cbox );

             if( drawTrackIcon(&nav, tx + per_space/2, y + ht/2, z+1, sc, storyarc, storyarc_roll, storyarc_glow, 0, missionMaker.current_mission == -1 ) )
            {
                missionMaker.current_mission = -1;
                missionMaker.current_page = 0;
            }

            if( !isDown(MS_LEFT) && mouseCollision(&nav) && cursor.dragging )
            {
                if( cursor.drag_obj.type == kTrayItemType_PlayerCreatedMission && EAINRANGE(cursor.drag_obj.iset, missionMaker.ppMission) )
                    eaSwap( &missionMaker.ppMission, 0, cursor.drag_obj.iset);
                trayobj_stopDragging();
            }
        }
        else
        {
            font(&title_12);
              font_color(text_clr,text_clr);
            if( eaSize(&missionMaker.ppMission) < i )
            {
                 if( i == eaSize(&missionMaker.ppMission)+1 )
                {
                    //if( sc > 0 )
                    //    helpButton( &mmTipAddMission, tx + per_space - 15*sc, y + ht/2, z+1, sc, "MMTipAddMission", &cbox );
                     if( drawTrackIcon(&nav, tx + per_space/2, y + ht/2, z+1, sc, add_mission, add_mission_roll, add_mission_glow, 0, 0) )
                    {    
                        MMScrollSet_AddMission(&missionMaker);
                        missionMaker.current_mission = eaSize(&missionMaker.ppMission)-1;
                        missionMaker.current_page = 0;
                    }

                    cprnt( tx + per_space/2, y + ht, z+1, sc, sc, "MMAddMission", 0, &cbox );
                     if( mouseCollision(&nav) )
                        font_color(CLR_MM_TITLE_OPEN, CLR_MM_TITLE_OPEN);
                    else
                         font_color(CLR_MM_TEXT_DARK,CLR_MM_TEXT_DARK);
                        
                      font_ital(0);
                      cprnt( tx + per_space/2-2*sc, y + 52*sc, z+5, sc*2.f, sc*2.f, "+" );
                    font_ital(1);
                     
                }

                if( !isDown(MS_LEFT) && mouseCollision(&nav) && cursor.dragging )
                {
                    if( cursor.drag_obj.type == kTrayItemType_PlayerCreatedMission && EAINRANGE(cursor.drag_obj.iset, missionMaker.ppMission) )
                        eaSwap( &missionMaker.ppMission, eaSize(&missionMaker.ppMission)-1, cursor.drag_obj.iset);
                    trayobj_stopDragging();
                }
            }
            else
            {
                CBox sub1, sub2;
                 AtlasTex * icon = atlasLoadTexture("salvage_treatise.tga");
                F32 icon_sc =1.f;
                 int c1 = 0x1e3137ff, c2 = 0x1e3137ff, c1out = 0, c2out = 0;
                int min, max;
                int text_clr1 = CLR_MM_TITLE_OPEN, text_clr2 = CLR_MM_TITLE_OPEN;

                MMScrollSet_GetMissionLevelRange(missionMaker.ppMission[i-1], &min, &max);

                if( missionMaker.bShowWrongness && !MMScrollSet_isMissionValid(missionMaker.ppMission[i-1], 0, 0) )
                    text_clr = text_clr1 = CLR_MM_TEXT_ERROR; 
                if( missionMaker.bShowWrongness && !MMScrollSet_isMissionValid(missionMaker.ppMission[i-1], 1, 0) )
                    text_clr = text_clr2 = CLR_MM_TEXT_ERROR; 

                   if( drawCloseButton( tx + per_space/2 + 50*sc, y + 30*sc, z, sc, nc  ) ) 
                {
                    collisions_off_for_rest_of_frame = 1;
                    dialogStdCB(DIALOG_ACCEPT_CANCEL, textStd("MMReallyDeleteMission", i), 0, 0, MMScrollSet_deleteMission, 0, DLGFLAG_ARCHITECT, missionMaker.ppMission[i-1] );
                }
                if( missionMaker.current_mission == i-1 )
                {
                    if(missionMaker.current_page == 0 )
                    {
                        c1 = 0x2dafbdff;
                        c1out = 1;
                    }
                    else
                    {
                        c2 = 0x2dafbdff;    
                        c2out = 1;
                    }
                }

                 font(&title_12);
                font_color(text_clr,text_clr);
                  cprnt( tx + per_space/2, y + ht, z+1, sc, sc, textStd("MMMissionMinMax", i, min, max) );

                  BuildCBox(&sub1, tx, y + R10*sc, per_space/2, ht-2*R10*sc );
                BuildCBox(&sub2, tx+per_space/2, y + R10*sc, per_space/2, ht-2*R10*sc );
                
                //helpButton( &mmTipMission1[i-1], tx + per_space/4 + 22*sc, y + ht - 22*sc, z+1, sc, "MMTipMission1", &cbox );
                //helpButton( &mmTipMission2[i-1], tx + per_space/2 + per_space/4 + 22*sc, y + ht - 22*sc, z+1, sc, "MMTipMission2", &cbox );

                if( !cursor.dragging && mouseLeftDrag(&nav) )
                {
                    TrayObj to = {0};
                    AtlasTex *icon = atlasLoadTexture("salvage_treatise.tga");
                    to.type = kTrayItemType_PlayerCreatedMission;
                    to.iset = i-1;
                    to.scale = sc;
                    trayobj_startDragging(&to,icon,NULL);
                }
                if( !isDown(MS_LEFT) && mouseCollision(&nav) && cursor.dragging )
                {
                    if( cursor.drag_obj.type == kTrayItemType_PlayerCreatedMission && EAINRANGE(cursor.drag_obj.iset, missionMaker.ppMission) )
                        eaSwap( &missionMaker.ppMission, i-1, cursor.drag_obj.iset);
                    trayobj_stopDragging();
                }

                if( mouseCollision(&sub1) )
                {
                    c1out = 0;
                    c1 = 0x2dafbdff;
                }
                font_color(c1,c1);
                font(&title_14);
                font_outl(c1out); 
                   cprntEx( tx + per_space/2 - 15*sc, y + 36*sc, z+5, sc, sc, CENTER_X|CENTER_Y, "1" );
                font_outl(1);
                    
                if( mouseCollision(&sub2) )
                {
                    font_outl(c2out);
                    c2 = 0x2dafbdff;
                }
                font_color(c2,c2);
                font_outl(c2out);
                cprntEx( tx + per_space/2 + 15*sc, y + 36*sc, z+5, sc, sc, CENTER_X|CENTER_Y, "2" );
                font_outl(1);

                   if( drawTrackIcon(&sub1, tx + per_space/2 - 17*sc, y + ht/2, z+1, sc, mission, mission_roll, mission_glow, 0, missionMaker.current_mission == i-1 && !missionMaker.current_page )  )
                {
                    missionMaker.current_mission = i-1;
                    missionMaker.current_page = 0;
                }
                   if( drawTrackIcon(&sub2, tx + per_space/2 + 18*sc, y + ht/2, z+1, sc, mission, mission_roll, mission_glow, FLIP_HORIZONTAL, missionMaker.current_mission == i-1 && missionMaker.current_page )  )
                {
                    missionMaker.current_mission = i-1;
                    missionMaker.current_page = 1;
                }
            }
        }
    }
}


int missionMakerWindow()
{
    static F32 autosave_timer;

    F32 x, y, z, wd, ht, sc, title_ht, control_ht;
    int color, bcolor;
    UIBox box;
    CBox cbox;
    char * pchNextButtonMsg[] = { "MMExit", "MMStoryArcSettings", "MMMissionSettings", "MMMissionDetails", "MMSaveAndTest" };
    char * pchNext, *pchPrev;
    int iNextIdx, iPrevIdx;

    int auto_save = optionGet(kUO_ArchitectAutoSave);
    int barclr, storyarc_ready;
    F32 progressBarWd, progressY;

    AtlasTex * white = atlasLoadTexture( "white" );

    winDefs[WDW_MISSIONMAKER].maximizeable = 1;
       if(!window_getDims(WDW_MISSIONMAKER, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
    {
        //if (window_getMode(WDW_CUSTOMVILLAINGROUP) == WINDOW_DISPLAYING)
        //{
            //window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
        //}
        return 0;    //clear picturebrowser here.
    }

    drawWaterMarkFrame( x, y, z, sc, wd, ht, 1 );

    if( !ArchitectCanEdit( playerPtr() ) )
    {
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONMAKER,WINDOW_SHRINKING);
        return 0;
    }


    uiBoxDefine(&box, x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht - 2*PIX3*sc );
    BuildCBox(&cbox, x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht - 2*PIX3*sc );

    // Navigation Section
    //-------------------------------------------------------------------------------------
    title_ht = 80*sc;
     control_ht = 50*sc;
    progressBarWd = 473*sc;
    storyarc_ready = MMScrollSet_IsReady(&missionMaker);
     missionMakerNav( x, y, z, sc, wd, title_ht ); 
    missionMakerControls( x, y+title_ht, z, sc, wd, control_ht, storyarc_ready ); 
    
    // File Size Footer
    //-------------------------------------------------------------------------------------
    // 
    if (missionMaker.dirty)
    {
        PlayerCreatedStoryArc *pArc;
        char * src;
        int size, zipped_size, ret;
        U8 *zip_data;

        pArc = ConvertScrollSetToPlayerCreatedMission(&missionMaker, 1);
        src = playerCreatedStoryArc_ToString(pArc);

        size = strlen(src); 
        zipped_size = size*1.0125+12;
        zip_data = malloc(zipped_size);
        ret = compress2(zip_data,&zipped_size,src,size,5);
        if(ret == Z_OK)
            missionMaker.filesize = zipped_size;
    
        free(zip_data);
        StructDestroy(ParsePlayerStoryArc, pArc);
        missionMaker.dirty = 0;
    }
    progressY = y+ht-53*sc;

    if( missionMaker.filesize >= MAX_ARC_FILESIZE )
    {
        barclr = CLR_MM_TEXT_ERROR;
    }
    else
        barclr = CLR_MM_TITLE_CLOSED;

     font( &title_9 );
    title_9.renderParams.outline = 0;
    
       if( missionMaker.fSaveMessageTimer > 0 )
    {
        F32 fStr = (sinf( missionMaker.fSaveMessageTimer )+1)/2;
        int blink_color = CLR_MM_TITLE_CLOSED;
        char * text = textStd("MMFileSaved", missionMaker.pchFilename );

        interp_rgba( fStr, CLR_MM_TITLE_CLOSED, CLR_MM_TEXT_ERROR, &blink_color );
        font_color( blink_color, blink_color );

        missionMaker.fSaveMessageTimer -= TIMESTEP/4;
        if( missionMaker.fSaveMessageTimer < 0 )
            missionMaker.fSaveMessageTimer = 0;
        cprnt( x + wd/2, progressY, z+5, sc, sc, text);
    }
     else if( missionMaker.fSaveMessageTimer < 0 )
    {
        F32 fStr = (sinf( missionMaker.fSaveMessageTimer )+1)/2;
        int blink_color = 0xffffffff;
        char * text = textStd("MMFileSaveFailed");
        interp_rgba( fStr, CLR_RED, CLR_MM_TEXT_ERROR, &blink_color );
        font_color( blink_color, blink_color );

        missionMaker.fSaveMessageTimer += TIMESTEP/4;
        if( missionMaker.fSaveMessageTimer > 0 )
            missionMaker.fSaveMessageTimer = 0;
        prnt( x + wd/2, progressY, z+5, sc, sc, text);
    }

    {
        char * file;
        
           if( missionMaker.pchFilename && missionMaker.pchFilename[0] )
        {
             file = textStd("FileNameString", missionMaker.pchFilename);
              font_color( barclr, barclr );
             cprnt( x + wd/2, progressY-10*sc, z+5, sc, sc, file  );
        }

        font_color( barclr, barclr );
        file = textStd("FileSizeString" );
           prnt( x + wd/2 - progressBarWd/2 - str_wd(font_grp, sc, sc, file) - 10*sc, progressY, z, sc, sc, file  );
        prnt( x + wd/2 + progressBarWd/2 + 10*sc, progressY, z, sc, sc, "%.2f%%", (F32)missionMaker.filesize*100/MAX_ARC_FILESIZE );
           helpButton( &mmTipFileSize, x + wd/2 + progressBarWd/2 + 28*sc + 35*sc, progressY-3*sc, z, sc, "MMTipFileSize", &cbox );
    }

    title_9.renderParams.outline = 1;
      drawMMProgressBar( x +wd/2,  progressY-10*sc, z, sc, 473*sc, MIN( 1.f, ((float)missionMaker.filesize/MAX_ARC_FILESIZE)) );

    // Button Footer
     //-------------------------------------------------------------------------------------
    if( missionMaker.current_mission < 0 ) 
    {
        pchPrev = pchNextButtonMsg[0]; // Exit
        pchNext = pchNextButtonMsg[2]; // MissionSettings
        iPrevIdx = 0;
        iNextIdx = 1;
        
    }
     else if( missionMaker.current_mission == 0 && !missionMaker.current_page )
    {
        pchPrev = pchNextButtonMsg[1]; // StoryArc
        pchNext = pchNextButtonMsg[3]; // MMMissionDetails
        iPrevIdx = 0;
        iNextIdx = 1;
    }
    else if( missionMaker.current_mission == eaSize(&missionMaker.ppMission)-1 && missionMaker.current_page )
    {
        pchPrev = pchNextButtonMsg[2]; // StoryArc
        pchNext = pchNextButtonMsg[4]; // SaveandTest
        iPrevIdx = missionMaker.current_mission+1;
        iNextIdx = 0;
    }
    else if( missionMaker.current_page )
    {
        pchPrev = pchNextButtonMsg[2]; // MMMissionSettings
        pchNext = pchNextButtonMsg[2]; // MMMissionSettings
        iPrevIdx = missionMaker.current_mission+1;
        iNextIdx = missionMaker.current_mission+2;
    }
    else
    {
        pchPrev = pchNextButtonMsg[3]; // MMMissionDetails
        pchNext = pchNextButtonMsg[3]; // MMMissionDetails
        iPrevIdx = missionMaker.current_mission;
        iNextIdx = missionMaker.current_mission+1;
    }

      drawHorizontalLine( x + 3*R10*sc, y + ht - 40*sc, wd-6*R10*sc, z, sc, 0xffffff44 );

       if( drawMMArrowButton( textStd( pchPrev, iPrevIdx ), x + R10*sc, y + ht - 44*sc, z, sc, 200*sc, 0 ) )
    {
        if(missionMaker.current_mission < 0)
        {
            dialogStd(DIALOG_TWO_RESPONSE, "ArchitectExit", "ExitString", "CancelString", missionMakerExitDialog, NULL, DLGFLAG_ARCHITECT);
        }
        else if( missionMaker.current_page )
        {
            missionMaker.current_page--;
        }
        else
        {
            missionMaker.current_page = 1;
            missionMaker.current_mission--; 
        }
    }

      if( drawMMArrowButton( textStd( pchNext, iNextIdx ), x + wd - 210*sc, y + ht - 44*sc, z, sc, 200*sc, 1 ) )
    {
        if( missionMaker.current_mission == eaSize(&missionMaker.ppMission)-1 && missionMaker.current_page )
        {
            if( !storyarc_ready )
                dialogArchitect( kMMControl_FixErrors|kMMControl_SaveExit|kMMControl_ExitAbandon, 3 );
            else
                dialogArchitect( kMMControl_SaveTest|kMMControl_SaveExit|kMMControl_ExitAbandon|kMMControl_Ready, 3 );
        }
        else if( !missionMaker.current_page )
        {
            missionMaker.current_page++;
        }
        else
        {
            missionMaker.current_page = 0;
            missionMaker.current_mission++; 
        }
    }

    
     uiBoxDefine(&box, x+100*sc, 0, wd-200*sc, 1000000 );
    clipperPush(&box);

    // Auto-Save
       if( optionGet(kUO_ArchitectAutoSave) )
    {
        if( drawMMButton( "AutoSaveOnString", 0, 0, x + wd/2, y+ht - 20*sc, z, 150*sc, sc, MMBUTTON_DEPRESSED|MMBUTTON_SMALL, 0  ) )
            optionSet(kUO_ArchitectAutoSave, 0, 1);
    }
    else
    {
        if( drawMMButton( "AutoSaveOffString", 0, 0, x + wd/2, y+ht - 20*sc, z, 150*sc, sc, MMBUTTON_SMALL, 0  ) )
            optionSet(kUO_ArchitectAutoSave, 1, 1);
    }

    clipperPop();
    
    // Interior
    //-------------------------------------------------------------------------------------
     if (!window_isFront(WDW_CUSTOMVILLAINGROUP))
    {
           missionMaker.scrollsetOpen = updateScrollSelectors(y+title_ht+control_ht, y+ht-(80+PIX3)*sc ); // do this first so it can intercept all clicks
    }

       missionMaker.wd = missionMaker.page_wd = wd/2;
     drawHorizontalLine( x + R10*sc, y + title_ht + control_ht + 29*sc, wd-2*R10*sc, z, sc, 0xffffff44 );
      drawHorizontalLine( x + R10*sc, y + ht - 76*sc, wd-2*R10*sc, z, sc, 0xffffff44 );
    MMScrollSet_Draw(&missionMaker, x, y + title_ht + control_ht + 30*sc, z, ht - title_ht - control_ht - 80*sc, sc, color);

      autosave_timer += TIMESTEP;
    if( auto_save && autosave_timer > 60*30 )  
    {
        char * tmp;
         estrCreate(&tmp);
        estrConcatCharString(&tmp, missionMaker.pchFilename ? missionMaker.pchFilename : "autosave" );
        missionMakerSaveAs(tmp);
        estrDestroy(&tmp);
        autosave_timer = 0;
    }

     return 0;
}

void toggleSaveCompressedCostumeArc(int mode)
{
    s_CompressCrittersForTextFile = mode;
}