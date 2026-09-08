
#include <utilitieslib/stdtypes.h>
#include "UI/uiAuction.h"
#include "UI/uiBox.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiComboBox.h"
#include "UI/uiUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiReticle.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "uiEmote.h"
#include "UI/uiGame.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiEnhancement.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "uiAmountSlider.h"
#include "UI/uiTray.h"
#include "UI/uiDialog.h"
#include "UI/uiOptions.h"
#include "uiSalvage.h"
#include "UI/uiPlaque.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "gameComm/wdwbase.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "entity/salvage.h"
#include "entity/origins.h"
#include "graphics/font.h"        // for xyprintf
#include "graphics/ttFontUtil.h"
#include "language/langClientUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "win/win_init.h"    // for windowClientSize
#include "cmdparse/cmdgame.h"
#include "entity/character_base.h"
#include "sound/sound.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/entity.h" 
#include <utilitieslib/components/EString.h>
#include "win/input.h"
#include <utilitieslib/components/StashTable.h>
#include "graphics/textureatlas.h"
#include "bases/DetailRecipe.h"
#include "gameComm/trayCommon.h"
#include "auction/Auction.h"
#include "entity/boost.h"
#include "entity/attrib_names.h"
#include "auction/AuctionData.h"
#include "entity/entPlayer.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "account/AccountCatalog.h"

#define MAX_AUCTION_STACK_SIZE 10

typedef struct AuctionKeyWord
{
    char * keyword;
    AuctionItem **ppItem;
    StashTable *ppStash;
}AuctionKeyWord;

AuctionKeyWord ** g_ppAuctionDictionary;
StashTable g_stAuctionKeyWord;
StashTable g_stAuctionHouse;

int gAuctionDisabled;

static int selected_item = -1;

static void uiAuctionUpdateBannedStatus(void)
{
    static bannedTimer = 0;

    // if i am currently banned from performing auction actions, bug the 
    // map server every second until it tells us we arent banned anymore
    if (gAuctionDisabled )
    {
        if ( !bannedTimer )
        {
            bannedTimer = timerAlloc();
            timerStart(bannedTimer);
        }
            
        if ( timerElapsed(bannedTimer) > 1.0f )
        {
            auction_updateAuctionBannedStatus();
            timerStart(bannedTimer);
        }
    }
}


typedef struct MultiLevelStashNode
{
    StashTable stStash;
    AuctionItem **ppItems;         //we are making an assumption that all the AuctionRequires statements in
                                //all the ppItems are the same in treeRecurseStashTable --JH 
}MultiLevelStashNode;


void uiAuctionHouseItemInfoReceive(Packet *pak)
{
    AuctionItem * pItem;
    char *pchIdentifier = pktGetString(pak);
    int selling = pktGetBitsAuto(pak);
    int buying = pktGetBitsAuto(pak);

    if ( stashFindPointer(g_AuctionItemDict.stAuctionItems, pchIdentifier, &pItem) )
    {
        pItem->numForBuy = buying;
        pItem->numForSale = selling;
    }
}

void uiAuctionHouseBatchItemInfoReceive(Packet *pak)
{
    int i;
    int size = pktGetBitsAuto(pak);
    int idOffset = pktGetBitsAuto(pak);
    int itmListSize = pktGetBitsAuto(pak);
    static int *forSaleList = NULL;
    static int *forBuyList = NULL;

    ea32SetSize(&forSaleList, 0);
    for ( i = 0; i < itmListSize; ++i )
        ea32Push(&forSaleList, pktGetBitsPack(pak, 1));
    ea32SetSize(&forBuyList, 0);
    for ( i = 0; i < itmListSize; ++i )
        ea32Push(&forBuyList, pktGetBitsPack(pak, 1));

    PERFINFO_AUTO_START("uiAuctionHouseBatchItemInfoReceive", 1);
    for ( i = 0; i < size; ++i )
    {
        int id = idOffset + i;

        if ( EAINRANGE(id, g_AuctionItemDict.ppItems )  )
        {
            AuctionItem *pItem = g_AuctionItemDict.ppItems[id];
            pItem->numForSale = forSaleList[i];
            pItem->numForBuy = forBuyList[i];
        }
    }
    PERFINFO_AUTO_STOP();
}

void uiAuctionHouseListItemInfoReceive(Packet * pak)
{
    int i, size;

    PERFINFO_AUTO_START("uiAuctionHouseListItemInfoReceive", 1);

    size = pktGetBitsAuto(pak);
    for( i = 0; i < size; i++ )
    {
        int id = pktGetBitsAuto(pak);
        int forsale = pktGetBitsAuto(pak);
        int forbuy = pktGetBitsAuto(pak);

        if ( EAINRANGE(id, g_AuctionItemDict.ppItems )  )
        {
            AuctionItem *pItem = g_AuctionItemDict.ppItems[id];
            pItem->numForSale = forsale;
            pItem->numForBuy = forbuy;
        }    
    }
    PERFINFO_AUTO_STOP();
}

AuctionHistoryItem **searchMenuHistory = NULL;
void uiAuctionHouseHistoryReceive(Packet *pak)
{
    AuctionItem *pItem;
    AuctionHistoryItem *pHistory;
    char *pchIdentifier    = pktGetString(pak);
    int i, buying, selling, count;

    stashFindPointer(g_AuctionItemDict.stAuctionItems, pchIdentifier, &pItem );

    buying = pktGetBitsAuto(pak);
    selling = pktGetBitsAuto(pak);
    count = pktGetBitsAuto(pak);

    if( pItem )
    {
        pItem->numForBuy = buying;
        pItem->numForSale = selling;

        for( i = eaSize(&searchMenuHistory)-1; i>=0; i-- )
        {
            if( stricmp(searchMenuHistory[i]->pchIdentifier, pItem->pchIdentifier)==0 )
                freeAuctionHistoryItem(eaRemove(&searchMenuHistory,i));
        }
    }

    while(eaSize(&searchMenuHistory) > 10 )
        freeAuctionHistoryItem(eaRemove(&searchMenuHistory,0));

    pHistory = newAuctionHistoryItem(pchIdentifier);
    eaPush(&searchMenuHistory, pHistory);

    
    for(i = 0; i < count; i++)
    {
        int date = pktGetBitsAuto(pak);
        int price = pktGetBitsAuto(pak);
        eaPush(&pHistory->histories, newAuctionHistoryItemDetail(NULL,NULL,date,price));
    }
}

static int alphabeticalSort(const void* leftData, const void* rightData)
{
    const AuctionKeyWord** left = (const AuctionKeyWord**)leftData;
    const AuctionKeyWord** right = (const AuctionKeyWord**)rightData;
    return stricmp( (*left)->keyword, (*right)->keyword );
}

static bool DoAllAuctionItemsFailAuctionRequiresCheck( AuctionItem** ppItems )
{
    int numItems = eaSize(&ppItems);
    int i;
    Entity *e = playerPtr();
    
    for (i = 0; i < numItems; ++i)
    {
        if ( DoesAuctionItemPassAuctionRequiresCheck(ppItems[i], e->pchar) )
        {
            return false;
        }
    }
    
    return true;
}

static char* auctionFindMatchingKeyword( char * token, int nextword )
{
    AuctionKeyWord *pKey = calloc(1, sizeof(AuctionKeyWord));
    int i;

    pKey->keyword = token;
    i = bfind(&pKey, g_ppAuctionDictionary, eaSize(&g_ppAuctionDictionary), sizeof(AuctionKeyWord*), alphabeticalSort);
    if( i < eaSize(&g_ppAuctionDictionary) )
    {
        AuctionKeyWord* dictionaryItem = NULL;
        if( !nextword && strnicmp( g_ppAuctionDictionary[i]->keyword, token, strlen(token) )==0 )
        {
            dictionaryItem = g_ppAuctionDictionary[i];
        }
        else if( i > 0 && nextword < 0 ) // backwards
        {
            dictionaryItem = g_ppAuctionDictionary[i-1];
        }
        else if( i < eaSize(&g_ppAuctionDictionary)-1 && nextword > 0 )
        {
            dictionaryItem = g_ppAuctionDictionary[i+1];
        }
        //do not return a keyword if all the associated auction items of a keyword fail the AuctionRequires check
        if ( dictionaryItem && (!dictionaryItem->ppItem || !DoAllAuctionItemsFailAuctionRequiresCheck(dictionaryItem->ppItem)) )
        {
            return dictionaryItem->keyword;
        }
    }

    return 0;
}

void auction_addKeyWordsFromString( char * str, AuctionItem *pItem, StashTable stStash )
{
    char * temp, *s = strdup(str);
    AuctionKeyWord *pKeyword;
    int i;

    if(!g_stAuctionKeyWord)
        g_stAuctionKeyWord = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);

    for( i = strlen(s)-2; i >= 0; i-- ) 
    {
        if( s[i] == ' ' && s[i+1] == ' ' ) // remove double spaces, this screws up smf parsing
            strcpy( s+i, s+i+1 );
    }
    temp = s;

    // add the whole string
    if(!stashFindPointer(g_stAuctionKeyWord, s, &pKeyword) )
    {
        pKeyword = calloc(1,sizeof(AuctionKeyWord));
        pKeyword->keyword = strdup(smf_DecodeAllCharactersGet(s));
        if( stashAddPointer(g_stAuctionKeyWord, s, pKeyword, 0) )
            eaPush(&g_ppAuctionDictionary, pKeyword);
    }
    if( pItem )
        eaPushUnique(&pKeyword->ppItem, pItem);
    if( stStash )
        eaPushUnique(&pKeyword->ppStash, stStash);

    // add each word
    s = strtok( temp, " ,");
    s = smf_DecodeAllCharactersGet(s);
    while(s)
    {
        if(!stashFindPointer(g_stAuctionKeyWord, s, &pKeyword) )
        {
            pKeyword = calloc(1, sizeof(AuctionKeyWord));
            pKeyword->keyword = strdup(s);
            if( stashAddPointer(g_stAuctionKeyWord, s, pKeyword, 0) )
                eaPush(&g_ppAuctionDictionary, pKeyword);
        }

        if( pItem )
            eaPushUnique(&pKeyword->ppItem, pItem);
        if( stStash )
            eaPushUnique(&pKeyword->ppStash, stStash);
        
        s = strtok(NULL, " ,");
        s = smf_DecodeAllCharactersGet(s);
    }
    SAFE_FREE(temp);
}

static int colorFromRarity(int rarity)
{
    int color = CLR_WHITE;

    if ( rarity == 0 )
        color = CLR_WHITE;
    else if ( rarity == 1 )
        color = CLR_WHITE;
    else if ( rarity == 2 )
        color = CLR_YELLOW;
    else if ( rarity == 3 )
        color = 0xff5500ff;
    else
        // This colour comes from Ken and is intended to be a more readable
        // and pleasing purple.
        color = 0xa082dcff;
    return color;
}

static int colorFromItem( AuctionItem *pItem, int default_color )
{
    switch( pItem->type )
    {
        xcase kTrayItemType_SpecializationInventory:
        {
            const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(pItem->enhancement->pchFullName, pItem->lvl+1);
            if( pRecipe )
                return colorFromRarity(pRecipe->Rarity);
        }
        xcase kTrayItemType_Recipe:
            return colorFromRarity(pItem->recipe->Rarity);
        xcase kTrayItemType_Salvage:
            return colorFromRarity(pItem->salvage->rarity);
    }

    return default_color;
}

typedef struct SimpleTreeNode SimpleTreeNode;
typedef struct SimpleTreeNode
{
    int open;
    int type;
    int hideThisNode;
    const char * name;
    MultiLevelStashNode *pStashNode;
    SimpleTreeNode ** ppNodes;
}SimpleTreeNode;

static SimpleTreeNode * g_pRoot = 0;

typedef struct AuctionFilter
{
    int auto_complete;

    char **ppKeywords;

    int iMinLevel;
    int iMaxLevel;
    
    int bForsale;
    int bBidding;
    
    int origins;
    int rarities;

    int update;
    int reupdate;
    int lastSort;

    MultiLevelStashNode *pStashNode;
    int item_type; // invisible filter used by search tree
    int *item_ids; // invisible list for making exact matches

}AuctionFilter;

AuctionFilter currentFilter = { 1 };



static SMFBlock *smfSearch = 0;
static SMFBlock *smfLow = 0;
static SMFBlock *smfHigh = 0;
static SMFBlock *smfBid = 0;
static SMFBlock *smfBidCount = 0;
static SMFBlock *smfPrice = 0;
static SMFBlock *smfCurrentItem = 0;
static SMFBlock *smfInfo = 0;
static ComboBox s_rarityCombo = {0};
static ComboBox s_originCombo = {0};
static TextAttribs s_taSearchInput =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0x222222ff,
    /* piColorSelectBG*/ (int *)0x999999ff,
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

static TextAttribs s_taTitle =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_AH_TEXT_SELECTED,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0x222222ff,
    /* piColorSelectBG*/ (int *)0x999999ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
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

static TextAttribs s_taInfo =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_WHITE,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0x222222ff,
    /* piColorSelectBG*/ (int *)0x999999ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
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

void clearAuctionFields(void)
{
    if( smfSearch )
        smf_SetRawText( smfSearch, "", 0 );
    if( smfBid )
        smf_SetRawText( smfBid, "", 0 );
    if( smfBidCount )
        smf_SetRawText( smfBidCount, "", 0 );
    if( smfPrice )
        smf_SetRawText( smfPrice, "", 0 );
}

static void setSearchText( char * text, int item_type )
{

    char *s, *temp;
    
    smf_SetRawText(smfSearch, text, 0 );

    temp = strdup( stripPlayerHTML( text, 0 ) );

    eaDestroyEx(&currentFilter.ppKeywords, NULL);

    s = strtok( temp, "," );
    while( s )
    {
        int i;

        while( *s && (*s == ' ' || *s == '\t') ) // skip leading whitespace
            s++;

        for( i = strlen(s)-2; i >= 0; i-- ) 
        {
            if( s[i] == '%' && s[i+1] == '%' ) // smf escapes percent, but the entry in the dictionary only has one %
                strcpy( s+i, s+i+1 );
        }

        eaPush(&currentFilter.ppKeywords, strdup(s) );
        s = strtok( NULL, "," );
    }
    free(temp);
    currentFilter.item_type = item_type;
    currentFilter.pStashNode = 0;
    currentFilter.update = 1;

}

static closeNavTreeNode( SimpleTreeNode *pNode )
{
    int i;
    pNode->open = 0;
    for( i = 0; i < eaSize(&pNode->ppNodes); i++ )
        closeNavTreeNode(pNode->ppNodes[i]);
}

static F32 drawAuctionSearch( F32 x, F32 y, F32 z, F32 wd, F32 sc, int color )
{
    F32 text_ht = 22*sc, ty, tx, twd, tht, arrow_sc = 1.f;
    static int init = 0, last_cursor;
    Entity * e = playerPtr();
    CBox box;
    AtlasTex *up_arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
      AtlasTex *down_arrow = atlasLoadTexture( "chat_separator_arrow_up.tga" );
    char level_buff[4];
    static F32 level_timer[4] = {0};
 
    tx = x + 10*sc;
 
    // init
     if( !smfSearch )
    {
        int i;

        smfSearch = smfBlock_Create();
        smfLow = smfBlock_Create();
        smfHigh = smfBlock_Create();
        smfBid = smfBlock_Create();
        smfBidCount = smfBlock_Create();
        smfPrice = smfBlock_Create();
        smfCurrentItem = smfBlock_Create();
        smfInfo = smfBlock_Create();

        comboboxClear(&s_rarityCombo);
        combocheckbox_init(&s_rarityCombo, 300, 40, 20, 100, 120, 20, 500, "RarityCombo", WDW_AUCTION, 0, 1 );
        s_rarityCombo.elements[0]->id = kSalvageRarity_Count;

        comboboxClear(&s_originCombo);
        combocheckbox_init(&s_originCombo, 400, 40, 20, 100, 140, 20, 500, "OriginCombo", WDW_AUCTION, 0, 1 );
        s_originCombo.elements[0]->id = (eaSize(&g_CharacterOrigins.ppOrigins));

        for( i = 1; i < kSalvageRarity_Count; i++ )
            comboboxTitle_add( &s_rarityCombo, 0, 0, StaticDefineIntRevLookup(RarityEnum, i), NULL, i, colorFromRarity(i), 0, 0  );
        for( i = eaSize(&g_CharacterOrigins.ppOrigins)-1; i >= 0; i-- )
            combocheckbox_add( &s_originCombo, 0, uiGetOriginTypeIconByName( g_CharacterOrigins.ppOrigins[i]->pchName), textStd(g_CharacterOrigins.ppOrigins[i]->pchDisplayName), i );
    }

    // Set default Values
       if( !smfBlock_HasFocus(smfLow) && (!smfLow->rawString || !*smfLow->rawString)  )
        smf_SetRawText(smfLow, "1", 0 );
    if( !smfBlock_HasFocus(smfHigh) && (!smfHigh->rawString || !*smfHigh->rawString)  )
        smf_SetRawText(smfHigh, "54", 0 );

    // Name
       drawMMCheckBox( tx + wd - 350*sc, y + 14*sc , z+1, sc, textStd("AutocompleteText"), &currentFilter.auto_complete, CLR_AH_TEXT, color&0xffffffaa, CLR_WHITE, color, &twd, &tht);

       BuildCBox(&box, tx+PIX3*sc, y + 10*sc, wd-240*sc, text_ht ); 
     if( mouseClickHit(&box, MS_LEFT ))
    {
        smf_SelectAllText(smfSearch);
        collisions_off_for_rest_of_frame = 1;
    }

     smf_SetFlags(smfSearch, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_AnyTextNoTagsOrCodes, 128, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripNoTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
       smf_SetScissorsBox(smfSearch, tx+PIX3*sc, y + 10*sc, wd-355*sc*sc, text_ht );
    s_taSearchInput.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
       smf_Display(smfSearch, tx+PIX3*sc, y + 13*sc, z+2, wd-355*sc*sc, 0, 0, 0, &s_taSearchInput, 0);

     if(smfBlock_HasFocus(smfSearch)) 
    {
         KeyInput* input;
        int index, autocomplete=0, nextword = 0;
        char* token = NULL;
        char* match=0;

        // Handle keyboard input.
         input = inpGetKeyBuf();
         while(input)
        {
            autocomplete = 1; // something changed try autocomplete
            if(input->type == KIT_EditKey)
            {
                switch(input->key)
                {
                    xcase INP_ESCAPE:    smf_ClearSelection();
                    xcase INP_RETURN:
                    case INP_NUMPADENTER:
                        autocomplete=0;
                        setSearchText( smfSearch->outputString, 0 );
                    xcase INP_TAB:
                        {    
                            char * newstr;
                            if( currentFilter.auto_complete )
                            {
                                autocomplete=0;
                                smf_DecodeAllCharactersInPlace(&smfSearch->rawString);
                                newstr = stripPlayerHTML( smfSearch->rawString, 0 );
                                estrPrintCharString(&smfSearch->rawString, newstr);
                                smf_SetCursor( smfSearch, strlen(newstr) );
                            }
                            else
                            {
                                smf_SelectAllText( smfLow );
                                inpClear();
                            }
                        }
                    xcase INP_UPARROW:
                        nextword = -1;
                    xcase INP_DOWNARROW:
                        nextword = 1;
                    xcase INP_LEFTARROW: // nothing for left
                        autocomplete=0;
                    xcase INP_RIGHTARROW:
                        {
                            char * newstr;
                            if( currentFilter.auto_complete )
                            {
                                autocomplete=0;
                                smf_DecodeAllCharactersInPlace(&smfSearch->rawString);
                                newstr = stripPlayerHTML( smfSearch->rawString, 0 );
                                estrPrintCharString(&smfSearch->rawString, newstr);
                            }
                        }
                    default:
                        break;
                }
            }
            else
            {
                //uiFormattedEdit_DefaultKeyboardHandler(edit, input);
            }
            inpGetNextKey(&input);
        }

        // auto-complete
         if( autocomplete && !currentFilter.auto_complete )
            setSearchText( smfSearch->outputString, 0 );
        else if( autocomplete && currentFilter.auto_complete )
        {
            char* token_start = NULL;
            int skip_tokens = 1;
            index = smf_GetCursorIndex();

            smf_DecodeAllCharactersInPlace(&smfSearch->rawString);

            // If this is a completely auto-completed section, use it as-is
            // this step is neseccary because display names have multiple spaces in a row which cause no problems in the UI
            // since the spaces are ignored, however they do cause a problem when trying to match exact text.
            if(strncmp( "<color #ffff88>", smfSearch->rawString, strlen("<color #ffff88>") ) == 0) 
            {
                token_start = malloc(smfBlock_GetDisplayLength(smfSearch) +2);
                token = token_start;

                strncpyt( token, smfSearch->rawString + strlen("<color #ffff88>"), strlen(smfSearch->rawString) - strlen("<color #ffff88></color>")+1);
                skip_tokens = 0;
            }
            
            if( skip_tokens )
            {
                token_start = malloc(sizeof(char)*index+2);
                token = token_start;

                strncpyt( token, stripPlayerHTML(smfSearch->rawString, 0), index+1 );

                while ( strchr( token, ',') ) // skip past other tokens
                    token = strchr( token, ',')+1;
    
                while( *token && (*token == ' ' || *token == '\t') )
                    token++;
            }

            if( token && *token )
            {
                match = auctionFindMatchingKeyword( token, nextword ); 
            }
            if( match )
            {
                if( nextword )
                {
                    estrPrintf(&smfSearch->rawString, "<color #ffff88>%s</color>", match);
                    smf_ParseAndFormat(smfSearch, smfSearch->rawString, 0, 0, 0, 0, 0, 0, 1, 0, &s_taSearchInput, 0 );
                    smf_SetCursor(smfSearch, smfBlock_GetDisplayLength(smfSearch) );
                }
                else
                {
                    estrSetLengthNoMemset( &smfSearch->rawString, index );
                    smf_EncodeAllCharactersInPlace(&smfSearch->rawString);
                    estrConcatf( &smfSearch->rawString, "<color #ffff88>%s</color>", match + strlen(token)  );
                    smfSearch->ulCrc = 0;
                }
            }
            else
            {
                char * str = stripPlayerHTML(smfSearch->rawString, 0);
                estrPrintCharString(&smfSearch->rawString, str);
                estrSetLengthNoMemset( &smfSearch->rawString, index );
                smf_EncodeAllCharactersInPlace(&smfSearch->rawString);
            }

            free(token_start);
        }
        else if( last_cursor != smf_GetCursorIndex() )
        {
            char * newstr = stripPlayerHTML( smfSearch->rawString, 0 );
            estrPrintCharString(&smfSearch->rawString, newstr);
        }
        last_cursor = smf_GetCursorIndex();
    }
    drawTextEntryFrame( tx, y + 10*sc, z, wd-240*sc, text_ht, sc, 2 );

    // Level Range
    // MIN LEVEL
       ty = y + 19*sc + text_ht;
    font( &game_12 );
     font_color( CLR_AH_TEXT, CLR_AH_TEXT );
     prnt( tx, ty + text_ht - 3*sc, z, sc, sc, "LevelsString" );
      tx += str_wd( font_grp, sc, sc, "LevelsString" ) + 5*sc;
      twd = (37 - up_arrow->width)*sc;

    smf_SetFlags(smfLow, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 54, SMFScrollMode_None, 
          SMFOutputMode_None, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
       BuildCBox(&box, tx, ty, twd - 2*sc, text_ht );
    if( mouseClickHit(&box, MS_LEFT ) )
        smf_SelectAllText(smfLow);
    smf_Display(smfLow, tx+PIX3*sc, ty + 4*sc, z+2,  twd, 0, 0, 0, &s_taSearchInput, 0);

    if( smfBlock_HasFocus( smfLow ) )
    {
        KeyInput* input = inpGetKeyBuf();
        while( input )
        {
            if( input->type == KIT_EditKey && input->key == INP_TAB )
            {
                if( input->attrib == KIA_NONE )
                {
                    smf_SelectAllText( smfHigh );
                    break;
                }
                else if( input->attrib == KIA_SHIFT )
                {
                    smf_SelectAllText( smfSearch );
                    break;
                }
            }
            inpGetNextKey( &input );
        }
    }

    if( smfLow->outputString )
        currentFilter.iMinLevel = atoi( smfLow->outputString );
    else
        currentFilter.iMinLevel = 0;

       drawTextEntryFrame( tx, ty, z,  twd + (3+up_arrow->width)*sc, text_ht, sc, 2 );

     if( currentFilter.iMinLevel < 54 )
    {
        BuildCBox(&box, tx + twd, ty, up_arrow->width*sc, text_ht/2 );
         if( mouseCollision(&box) )
        {
            arrow_sc = 1.2f;
            if( isDown(MS_LEFT) )
                level_timer[0] += TIMESTEP;
        }
        if( mouseClickHit(&box, MS_LEFT) || level_timer[0] > 5 )
        {
            arrow_sc = .9f;
            currentFilter.iMinLevel++;
            itoa( currentFilter.iMinLevel, level_buff, 10 );
            smf_SetRawText(smfLow, level_buff, 0 );

            if( currentFilter.iMinLevel > currentFilter.iMaxLevel)
            {
                currentFilter.iMaxLevel = currentFilter.iMinLevel;
                itoa( currentFilter.iMaxLevel, level_buff, 10 );
                smf_SetRawText(smfHigh, level_buff, 0 );
            }
            currentFilter.update = 1;
            level_timer[0] = 0;
        }
    }
    display_sprite(down_arrow, tx + twd + (up_arrow->width*sc - up_arrow->width*sc*arrow_sc)/2, ty + text_ht/4 - up_arrow->height*sc*arrow_sc/2, z, sc*arrow_sc, sc*arrow_sc, currentFilter.iMinLevel < 54 ? color:0xffffff88 );
    
    arrow_sc = 1.f; 
    if( currentFilter.iMinLevel > 1 )
    {
        BuildCBox(&box, tx + twd, ty + text_ht/2, up_arrow->width*sc, text_ht/2 );
        if( mouseCollision(&box) )
        {
            arrow_sc = 1.1f;
            if( isDown(MS_LEFT) )
                level_timer[1] += TIMESTEP;
        }
        if( mouseClickHit(&box, MS_LEFT) || level_timer[1] > 5 )
        {
            arrow_sc = .9f;
            currentFilter.iMinLevel--;
            itoa( currentFilter.iMinLevel, level_buff, 10 );
            smf_SetRawText(smfLow, level_buff, 0 );
            currentFilter.update = 1;
            level_timer[1] = 0;
        }
    }
    display_sprite(up_arrow, tx + twd + (up_arrow->width*sc - up_arrow->width*sc*arrow_sc)/2, ty + (3*text_ht)/4 - up_arrow->height*sc*arrow_sc/2, z, sc*arrow_sc, sc*arrow_sc,  currentFilter.iMinLevel > 1  ? color:0xffffff88);


    // ----
    tx += twd + (up_arrow->width+5)*sc;
      prnt( tx, ty + text_ht - 3*sc, z, sc, sc, "ToString" );
     tx += str_wd(font_grp,sc, sc, "ToString") + 5*sc;
    // ----

    // MAX LEVEL
    smf_SetFlags(smfHigh, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine,SMFInputMode_NonnegativeIntegers, 54, SMFScrollMode_None, 
           SMFOutputMode_None, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    BuildCBox(&box, tx, ty, twd - 2*sc, text_ht );
    if( mouseClickHit(&box, MS_LEFT ) )
        smf_SelectAllText(smfHigh);
    smf_Display(smfHigh, tx+PIX3*sc, ty + 4*sc, z+2, twd, 0, 0, 0, &s_taSearchInput, 0);

    if( smfBlock_HasFocus( smfHigh ) )
    {
        KeyInput* input = inpGetKeyBuf();
        while( input )
        {
            if( input->type == KIT_EditKey && input->key == INP_TAB )
            {
                if( input->attrib == KIA_NONE && selected_item != -1 )
                {
                    smf_SelectAllText( smfBid );
                    inpClear();
                    break;
                }
                else if( input->attrib == KIA_SHIFT )
                {
                    smf_SelectAllText( smfLow );
                    break;
                }
            }
            inpGetNextKey( &input );
        }
    }

    if( smfLow->outputString )
        currentFilter.iMaxLevel = atoi( smfHigh->outputString );
    else
        currentFilter.iMaxLevel = 54;

    if( currentFilter.iMinLevel > currentFilter.iMaxLevel && !smfBlock_HasFocus(smfLow) && !smfBlock_HasFocus(smfHigh) )
    {
        currentFilter.iMinLevel = currentFilter.iMaxLevel;
        itoa( currentFilter.iMinLevel, level_buff, 10 );
        smf_SetRawText(smfLow, level_buff, 0 );
    }

       drawTextEntryFrame( tx, ty, z, twd + (3+up_arrow->width)*sc, text_ht, sc, 2 );

    arrow_sc = 1.f;
    if( currentFilter.iMaxLevel < 54 )
    {
        BuildCBox(&box, tx + twd, ty, up_arrow->width*sc, text_ht/2 );
        if( mouseCollision(&box) )
        {
            arrow_sc = 1.2f;
            if( isDown(MS_LEFT) )
                level_timer[2] += TIMESTEP;
        }
        if( mouseClickHit(&box, MS_LEFT) || level_timer[2] > 5 )
        {
            arrow_sc = .9f;
            currentFilter.iMaxLevel++;
            itoa( currentFilter.iMaxLevel, level_buff, 10 );
            smf_SetRawText(smfHigh, level_buff, 0 );
            currentFilter.update = 1;
            level_timer[2] = 0;
        }
    }
       display_sprite(down_arrow, tx + twd + (up_arrow->width*sc - up_arrow->width*sc*arrow_sc)/2, ty + text_ht/4 - up_arrow->height*sc*arrow_sc/2, z, sc*arrow_sc, sc*arrow_sc, currentFilter.iMaxLevel < 54 ? color:0xffffff88 );
    arrow_sc = 1.f; 

      if( currentFilter.iMaxLevel > 1 )
    {
        BuildCBox(&box, tx + twd, ty + text_ht/2, up_arrow->width*sc, text_ht/2 );
        if( mouseCollision(&box) )
        {
            arrow_sc = 1.1f;
            if( isDown(MS_LEFT) )
                level_timer[3] += TIMESTEP;
        }
        if( mouseClickHit(&box, MS_LEFT) || level_timer[3] > 5 )
        {
            arrow_sc = .9f;
            currentFilter.iMaxLevel--;
            itoa( currentFilter.iMaxLevel, level_buff, 10 );
            smf_SetRawText(smfHigh, level_buff, 0 );

            if( currentFilter.iMaxLevel < currentFilter.iMinLevel )
            {
                currentFilter.iMinLevel = currentFilter.iMaxLevel;
                itoa( currentFilter.iMaxLevel, level_buff, 10 );
                smf_SetRawText(smfLow, level_buff, 0 );
            }
            currentFilter.update = 1;
            level_timer[3] = 0;
        }
    }
    display_sprite(up_arrow, tx + twd + (up_arrow->width*sc - up_arrow->width*sc*arrow_sc)/2, ty + (3*text_ht)/4 - up_arrow->height*sc*arrow_sc/2, z, sc*arrow_sc, sc*arrow_sc,  currentFilter.iMaxLevel > 1  ? color:0xffffff88);

       tx += twd + (up_arrow->width+18)*sc;

    // Rarity Filter
    // Origin Filter
     s_rarityCombo.x = (tx-x)/sc;
    s_originCombo.x = (tx + 105*sc-x)/sc;
    combobox_display(&s_rarityCombo);
    combobox_display(&s_originCombo);

      tx += 225*sc;

    // Sale/Bidding Checkbox
       drawMMCheckBox( tx, ty+5*sc, z+1, sc, textStd("ForSale"), &currentFilter.bForsale, CLR_AH_TEXT, color&0xffffffaa, CLR_WHITE, color, &twd, &tht);
    drawMMCheckBox( tx + twd + 5*sc, ty+5*sc, z+1, sc, textStd("BiddingString"), &currentFilter.bBidding, CLR_AH_TEXT, color&0xffffffaa, CLR_WHITE, color, &twd, &tht);

    // Buttons
       if( drawMMButton( "SearchString", 0, 0, x + wd - 160*sc, y+23*sc, z, 100*sc, sc, MMBUTTON_SMALL|MMBUTTON_COLOR, color ) )
    {
        char * newstr = stripPlayerHTML( smfSearch->rawString, 0 );
        estrPrintCharString(&smfSearch->rawString, newstr);

        currentFilter.update = 1;
        currentFilter.item_type = 0;
        currentFilter.pStashNode = 0;
        setSearchText( smfSearch->outputString, 0 );

    }
       if( drawMMButton( "ResetString", 0, 0, x + wd - 60*sc, y + 23*sc, z, 80*sc, .7*sc, MMBUTTON_SMALL|MMBUTTON_COLOR, color ) )
    {
        int i;

        eaDestroyEx(&currentFilter.ppKeywords, NULL);
        currentFilter.iMinLevel = 0;
        currentFilter.iMaxLevel = 0;

         combobox_setElementsFromBitfield(&s_originCombo,1<<eaSize(&g_CharacterOrigins.ppOrigins));
        combobox_setElementsFromBitfield(&s_rarityCombo,1<<kSalvageRarity_Count);

        currentFilter.item_type = 0;
        currentFilter.pStashNode = 0;
        currentFilter.update = 1;
        currentFilter.bBidding = currentFilter.bForsale = 0;

        eaiDestroy(&currentFilter.item_ids);
        currentFilter.item_ids = 0;

        smf_SetRawText(smfSearch, "", 0);
        smf_SetRawText(smfLow, "", 0);
        smf_SetRawText(smfHigh, "", 0);

        for( i = 0; i < eaSize(&g_pRoot->ppNodes); i++ )
        {
            closeNavTreeNode( g_pRoot->ppNodes[i] );
        }
    }

      return 80*sc;
}




static SimpleTreeNode * addSimpleNode( SimpleTreeNode *pParent, MultiLevelStashNode * pStashNode, const char * pchName, int type )
{
    SimpleTreeNode *pNew = calloc(sizeof(SimpleTreeNode), 1);
    pNew->name = pchName;
    pNew->type = type;
    pNew->pStashNode = pStashNode;
    pNew->hideThisNode = 0;
    if(pParent)
        eaPush(&pParent->ppNodes, pNew);
    return pNew;
}

static int nodeSort(const void* leftData, const void* rightData)
{
    const SimpleTreeNode** left = (const SimpleTreeNode**)leftData;
    const SimpleTreeNode** right = (const SimpleTreeNode**)rightData;
    if( stricmp((*left)->name, "OtherString") == 0 )
        return -1;
    else if( stricmp((*right)->name, "OtherString") == 0 )
        return 1;
    else
        return stricmp( textStd((*left)->name), textStd((*right)->name) );
}


static void simpleTreeSort( SimpleTreeNode * pNode );
static void simpleTreeSort( SimpleTreeNode * pNode )
{
    int i;
    for( i = 0; i < eaSize(&pNode->ppNodes); i++ )
        simpleTreeSort(pNode->ppNodes[i]);
    eaQSort( pNode->ppNodes, nodeSort );
    
}

static void treeRecurseStashtable( SimpleTreeNode * pNode, StashTable stStash, int type )
{
    StashTableIterator iHash;
    StashElement eltHash;
    int i;
    Entity *e = playerPtr();

    stashGetIterator(stStash, &iHash);
    while( stashGetNextElement(&iHash, &eltHash) )
    {
        MultiLevelStashNode *pStashNode = 0;
        SimpleTreeNode * pSub;

        const char * substr = stashElementGetStringKey(eltHash);
        pStashNode = stashFindPointerReturnPointer(stStash, substr);

        //we are making an assumption that all the AuctionRequires statements in
        //all the ppItems are the same --JH
        if( pStashNode && !pStashNode->stStash && eaSize(&pStashNode->ppItems)
            && !DoesAuctionItemPassAuctionRequiresCheck(pStashNode->ppItems[0], e->pchar))
        {
            continue;
        }

        pSub = addSimpleNode( pNode, pStashNode, substr, type );

        if( pStashNode && pStashNode->stStash )
        {
            if( !type )
            {
                if( stricmp( substr, "UiInventoryTypeSalvage") == 0 )
                    pSub->type = kTrayItemType_Salvage;
                else if( stricmp( substr, "UiInventoryTypeInspiration") == 0 )
                    pSub->type = kTrayItemType_Inspiration;
                else if( stricmp( substr, "UiInventoryTypeRecipe") == 0 )
                    pSub->type = kTrayItemType_Recipe;
                else if( stricmp( substr, "UiInventoryTypeEnhancement") == 0 )
                    pSub->type = kTrayItemType_SpecializationInventory;
            }

            treeRecurseStashtable( pSub, pStashNode->stStash , pSub->type );
        }        
    }

    {
        //mark a tree node as hidden if all its child nodes are marked as hidden
        //or if no child nodes were added to the node
        bool markThisNodeAsHidden = true;
        for( i = 0; i < eaSize(&pNode->ppNodes); i++ )
        {
            if (!pNode->ppNodes[i]->hideThisNode)
            {
                markThisNodeAsHidden = false;
                // add keywords associated with node to Auction House keyword dictionary if node was not marked as hidden
                if (pNode->ppNodes[i]->pStashNode && pNode->ppNodes[i]->pStashNode->stStash)
                {
                    auction_addKeyWordsFromString( textStd(pNode->ppNodes[i]->name), 0, pNode->ppNodes[i]->pStashNode->stStash  );
                }
            }
        }
        if (markThisNodeAsHidden)
        {
            pNode->hideThisNode = true;
        }
    }
}




F32 tree_max_wd;
F32 drawNavTreeNode( SimpleTreeNode *pNode, F32 x, F32 y, F32 z, F32 sc )
{
    CBox box;
    F32 start_y = y;
    F32 wd;
     int i, color = CLR_AH_TEXT;

    if (pNode->hideThisNode)
    {
        return 0;
    }

    wd = str_wd( font_grp, sc, sc, "[-] %s", textStd(pNode->name));

      BuildCBox( &box, x, y, wd*sc, 20*sc );
    
       if( pNode->open )
        color = CLR_AH_TEXT_SELECTED;
    if( mouseCollision(&box))
        color = CLR_AH_TEXT_LIT;

    if(  mouseClickHit(&box, MS_LEFT) )
    {
        if( eaSize(&pNode->ppNodes) )
            pNode->open = !pNode->open;

        setSearchText( textStd(pNode->name), pNode->type ); // this clears stash node
        currentFilter.pStashNode = pNode->pStashNode;
    }

     font(&game_12);
    font_color(color, color);
    if(  eaSize(&pNode->ppNodes)  )
    {
        if( pNode->open )
        {
            cprntEx( x + 10*sc, y + 10*sc, z, sc, sc, CENTER_Y, "[-] %s", textStd(pNode->name) );
            wd = str_wd( font_grp, sc, sc, "[-] %s", textStd(pNode->name));
            for( i = 0; i < eaSize(&pNode->ppNodes); i++ )
            {
                y += drawNavTreeNode( pNode->ppNodes[i], x + 15*sc, y + 20*sc, z, sc );
            }
        }
        else
        {
             cprntEx( x + 10*sc, y + 10*sc, z, sc, sc, CENTER_Y, "[+] %s", textStd(pNode->name));
            wd = str_wd( font_grp, sc, sc, "[-] %s", textStd(pNode->name));
        }
    }
    else
    {
        cprntEx( x + 10*sc, y + 10*sc, z, sc, sc, CENTER_Y, "%s", textStd(pNode->name));
        wd = str_wd( font_grp, sc, sc, "[-] %s", textStd(pNode->name));
    }

       if( x + 10*sc + wd > tree_max_wd )
        tree_max_wd = x + 10*sc + wd;    
    return y-start_y + 20*sc; 
}


F32 drawAuctionNavTree( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    static ScrollBar sb = {0};
    F32 doc_ht=0;
    UIBox box;
    int i;
    F32 twd;
      
       uiBoxDefine(&box, x, y, wd - 10*sc, ht );
    clipperPushRestrict( &box );
 
    tree_max_wd = 0;
     if(!g_pRoot)  
    {
        g_pRoot = addSimpleNode( 0, 0, strdup("AllString"), 0);
        g_pRoot->open = 1;
        treeRecurseStashtable( g_pRoot, g_stAuctionHouse, 0 );
        simpleTreeSort(g_pRoot);
        eaQSort( g_ppAuctionDictionary, alphabeticalSort );

    }

    for( i = 0; i < eaSize(&g_pRoot->ppNodes); i++ )
    {
        doc_ht += drawNavTreeNode( g_pRoot->ppNodes[i], x, y + doc_ht + R10*sc - sb.offset, z, sc );
    }

    clipperPop();

    twd = MIN(wd/2 - 20*sc, tree_max_wd-x);
     sb.use_color = 0;
        doScrollBar( &sb, ht-20*sc, doc_ht+20*sc, x + wd - 5*sc, y+9*sc, z, 0, &box );

       return twd + 20*sc;
}

static uiEnhancement *toolTipEnhancement = NULL;
static ToolTip powerTip;

void displayAuctionItemIcon( AuctionItem * pItem, F32 cx, F32 cy, F32 z, F32 sc )
{
    F32 size = 38.f;

     switch( pItem->type )
    {
        xcase kTrayItemType_Recipe:
        {
              uiRecipeDrawIcon( pItem->recipe,  pItem->recipe->level, cx, cy, z, sc, WDW_AUCTION, 1, CLR_WHITE);
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            uiEnhancement *enhancement = uiEnhancementCreate(pItem->enhancement->pchFullName, pItem->lvl);
            uiEnhancementSetColor(enhancement, CLR_WHITE);
               uiEnhancementDrawCentered( enhancement, cx + 10*sc, cy+3*sc, z, sc, 1.5*sc, MENU_GAME, WDW_AUCTION, 1, 0 );
            if (enhancement->isDisplayingToolTip)
            {
                uiEnhancementFree(&toolTipEnhancement);
                toolTipEnhancement = enhancement;
            }
            else
                uiEnhancementFree(&enhancement); 
        }
        xcase kTrayItemType_Salvage:
        {
              AtlasTex * ptex = atlasLoadTexture(pItem->salvage->ui.pchIcon);
            display_sprite( ptex, cx + 10*sc, cy, z, sc, sc, CLR_WHITE);
        }
        xcase kTrayItemType_Inspiration:
        case kTrayItemType_Power:
        {
            CBox box;
             AtlasTex * ptex = atlasLoadTexture(pItem->temp_power->pchIconName);
             display_sprite( ptex, cx + 20*sc, cy + 15*sc, z, sc*1.3, sc*1.3, CLR_WHITE);
    
            BuildCBox(&box, cx + 20*sc, cy + 15*sc, ptex->width*sc*1.3, ptex->height*sc*1.3f );
            if( mouseCollision(&box) )
            {
                char * text = 0;
                estrConcatf(&text,"<color InfoBlue><b>%s</b></color><p>", textStd(pItem->temp_power->pchDisplayName)); 
                estrConcatf(&text,"<color paragon>%s</color><p>", textStd(pItem->temp_power->pchDisplayShortHelp));
                estrConcatf(&text,"<color white>%s</color><p>", textStd(pItem->temp_power->pchDisplayHelp));
                addToolTip( &powerTip );
                powerTip.flags |= TT_NOTRANSLATE;
                forceDisplayToolTip( &powerTip, &box, text, MENU_GAME, WDW_AUCTION );
            }
            
        }

    }
}

static AuctionItem **ppLocalInventory;
static int local_idx = -1;
static int my_category = 0;
void addItemToLocalInventory(char * pchIdent, int id, int status, int amtCancelled, int amtStored, int infStored, int amtOther, int infPrice, bool bMergedBid, int make_current )
{
    AuctionItem *newInvItem = calloc(1, sizeof(AuctionItem));
    AuctionItem *baseItem;
    int idx;

    if( !stashFindPointer( g_AuctionItemDict.stAuctionItems, pchIdent, &baseItem ) )
        return;

    newInvItem->pchIdentifier = pchIdent;
    newInvItem->type = baseItem->type;
    newInvItem->lvl = baseItem->lvl;
    newInvItem->pItem = baseItem->pItem;

    newInvItem->auction_id = id;
    newInvItem->auction_status = status;
    newInvItem->amtCancelled = amtCancelled;
    newInvItem->amtStored = amtStored;
    newInvItem->infStored = infStored;
    newInvItem->amtOther = amtOther;
    newInvItem->infPrice = infPrice;
    newInvItem->bMergedBid = bMergedBid;

    switch ( newInvItem->type )
    {
        xcase kTrayItemType_Salvage:     newInvItem->displayName = newInvItem->salvage->ui.pchDisplayName;
        xcase kTrayItemType_Recipe:         newInvItem->displayName = newInvItem->recipe->ui.pchDisplayName;
        xcase kTrayItemType_Inspiration: newInvItem->displayName = newInvItem->inspiration->pchDisplayName;
        xcase kTrayItemType_SpecializationInventory: newInvItem->displayName = newInvItem->inspiration->pchDisplayName;
    }

    idx = eaPush(&ppLocalInventory, newInvItem);

    if( make_current )
    {
        my_category = status;
         local_idx = idx;
        smf_SetFlags(smfPrice, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 0x7fffffff, SMFScrollMode_None, 
            SMFOutputMode_None, SMFDisplayMode_NumbersWithCommas, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
        smf_SetCursor(smfPrice, 0);
    }

}

static void drawItemInfo( AuctionItem * pItem, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    static AuctionItem *pLastItem=0;
    static char * pchInfo;
    F32 text_ht;
    UIBox box;
    static ScrollBar sb = {0};

      drawFlatFrame(PIX3, R10, x, y, z, wd, ht, sc, 0x00000044, 0x00000044 );

    x += 10*sc;
    y += 5*sc;
    wd -= 20*sc;
    ht -= 10*sc;

    if( pItem!=pLastItem )
    {
        estrClear(&pchInfo);

        switch( pItem->type )
        {
            xcase kTrayItemType_Recipe:
            {
                const DetailRecipe * pRec = pItem->recipe;

                if( pRec->ui.pchDisplayShortHelp )
                    estrConcatf( &pchInfo, "%s<br><br>", textStd(pRec->ui.pchDisplayShortHelp) );

                if (pRec->pchEnhancementReward == NULL)
                {
                    // display help
                    estrConcatCharString(&pchInfo, textStd(pRec->ui.pchDisplayHelp));
                }
                else
                {
                    // display enhancement information
                    uiEnhancement *pEnh = uiEnhancementCreateFromRecipe((DetailRecipe*)pRec, pRec->level, false);
                    char *pHelp = NULL;
                    if (pEnh)
                    {
                        pHelp = uiEnhancementGetToolTipText(pEnh);
                        if (pHelp)
                        {
                            estrConcatCharString(&pchInfo, pHelp);
                        }
                        uiEnhancementFree(&pEnh);
                    }
                }
            }
            xcase kTrayItemType_Salvage:
            {
                SalvageItem * sal = pItem->salvage;
                DetailRecipe **useful_to = NULL;

                if( sal->ui.pchDisplayShortHelp )
                    estrConcatf( &pchInfo, "%s<br>", textStd(sal->ui.pchDisplayShortHelp) );

                // display if it's used in player's recipes
                if (salvageitem_IsInvention(sal))
                {
                    int i;
                    eaSetSize(&useful_to,0);
                    if(character_IsSalvageUseful(playerPtr()->pchar, sal, &useful_to))
                    {
                        estrConcatCharString(&pchInfo, textStd("SalvageDropUseful"));
                        for (i = 0; i < eaSize(&useful_to); i++)
                        {
                            DetailRecipe *pRec = useful_to[i];

                            if (pRec)
                            {
                                estrConcatf(&pchInfo, "<br>&nbsp;&nbsp;&nbsp;%s", textStd(pRec->ui.pchDisplayName));
                            }
                        }
                    } 

                    {
                        StashElement elt;
                         estrConcatf(&pchInfo, "<br>%s", textStd("SalvageDropUseless"));

                        if( stashFindElementConst( g_DetailRecipeDict.stBySalvageComponent, sal->pchName, &elt) )
                        {
                            DetailRecipe **ppRec = stashElementGetPointer(elt);
                            for( i = eaSize(&ppRec)-1; i>=0; i-- )
                            {
                                estrConcatf(&pchInfo, "<br>&nbsp;&nbsp;&nbsp;%s", textStd(ppRec[i]->ui.pchDisplayName));
                            }
                        }
                    }

                }
            }
            xcase kTrayItemType_SpecializationInventory:
            {    
                BasePower * ppow = pItem->temp_power;
                StuffBuff stuffb;
                initStuffBuff(&stuffb, 128);
                uiEnhancementAddHelpText(NULL, ppow, pItem->lvl, &stuffb);
                addStringToStuffBuff(&stuffb, "<br>");
                uiEnhancementGetInfoText(&stuffb, ppow, NULL, pItem->lvl, false);
                estrConcatCharString(&pchInfo, stuffb.buff);
            }
            xcase kTrayItemType_Inspiration:
            case kTrayItemType_Power:
            {
                BasePower * ppow = pItem->temp_power;
                if(ppow->pchDisplayShortHelp )
                    estrConcatf( &pchInfo, "%s<br><br>", textStd(ppow->pchDisplayShortHelp) );
                estrConcatCharString(&pchInfo, textStd(ppow->pchDisplayHelp));
            }
        }
    }

       uiBoxDefine(&box, x, y-5*sc, wd, ht+10*sc );
    clipperPushRestrict(&box);

      s_taInfo.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
    s_taInfo.piColor = (int *)((int)(CLR_AH_TEXT));
    text_ht = smf_ParseAndDisplay( smfInfo, pchInfo, x, y - sb.offset, z, wd, 0, pItem!=pLastItem, pItem!=pLastItem, &s_taInfo, NULL, 0, true);
    pLastItem = pItem;
    clipperPop();

     sb.use_color = 1;
    sb.color = CLR_GREY;
      doScrollBar(&sb, ht-10*sc, text_ht, x+wd+10*sc, y+5*sc, z, 0, &box );
}


static void drawItemHistory( AuctionItem * pItem, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    AuctionHistoryItem *pHistory=0;
    int i;

    font(&game_9);
     font_color( CLR_AH_TEXT, CLR_AH_TEXT );
    drawFlatFrame( PIX3, R10, x, y, z, wd, ht, sc, 0x00000044, 0x00000044 );

    for( i = 0; i < eaSize(&searchMenuHistory); i++ )
    {
        if( stricmp(searchMenuHistory[i]->pchIdentifier, pItem->pchIdentifier)==0 )
            pHistory = searchMenuHistory[i];
    }

    if(pHistory)
    {
        if ( eaSize(&pHistory->histories) <= 0 )
        {
            font_color( 0xffffff77, 0xffffff77 );
            cprnt( x + wd/2, y + 13*sc, z, sc, sc, textStd("NoHistory") );
        }
        else
        {
            int j;
            for ( j = 0; j < eaSize(&pHistory->histories); ++j )
            {
                // remove time
                char buffer[80];
                char *c;
                timerMakeDateStringFromSecondsSince2000(buffer,pHistory->histories[j]->date);
                if( (c = strrchr(buffer,' ')) != NULL )
                    *c = '\0';
                prnt( x + 10*sc, y + 13*sc + j*13*sc, z, sc, sc, textStd("HistoryItemStringSmall", pHistory->histories[j]->price, buffer) );
            }
        }
    }
    else
    {
        font_color( 0xffffff77, 0xffffff77 );
        cprnt( x + wd/2, y + 13*sc , z, sc, sc, textStd("WaitingForHistory") );
    }
}

static F32 drawAuctionItem( AuctionItem * pItem, int i, F32 x, F32 y, F32 z, F32 wd, F32 sc, int color )
{
     F32 ht = 40*sc;
    int clr = CLR_AH_BACKGROUND&0xffffffcc;
    Entity * e = playerPtr();
    UIBox uibox;

     wd -= 10*sc;

      if( wd < 400*sc)
        wd = 400*sc;

       if( i == selected_item )
    {
        int offerFlag = MMBUTTON_SMALL|MMBUTTON_COLOR;
        int amount=0;
        int bid=0;
        F32 title_ht;
        F32 link_ht = 0;
        F32 ty;
          ht = 100*sc;

        if(gAuctionDisabled)
            offerFlag |= (MMBUTTON_LOCKED|MMBUTTON_GRAYEDOUT);

        if( currentFilter.update )
            auction_updateHistory(pItem->pchIdentifier);

        s_taTitle.piColor = (int*)colorFromItem(pItem,CLR_AH_TEXT_SELECTED);

        clr = CLR_AH_BACKGROUND_SELECTED&0xffffffcc;

        // Display Icon Here
        displayAuctionItemIcon( pItem, x, y+2*sc, z, sc );

         smf_SetRawText(smfCurrentItem, textStd(pItem->displayName), 0);
         smf_SetFlags(smfCurrentItem, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, SMFScrollMode_None, 
            SMFOutputMode_None, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
         title_ht = smf_Display(smfCurrentItem,  x + 90*sc, y + 5*sc, z+2, wd - 95*sc, 0, 0, 0, &s_taTitle, 0);

         if( pItem->type == kTrayItemType_Recipe || (pItem->type == kTrayItemType_Salvage && salvageitem_IsInvention(pItem->salvage)) )
            link_ht = 15*sc;

        ht += title_ht + link_ht;
         drawFlatFrame( PIX3, R10, x, y+2*sc, z-1, wd, ht-2*sc, sc, clr, clr );
         uiBoxDefine(&uibox, x+5*sc, y, wd-10*sc, ht);
        clipperPushRestrict(&uibox);

        if( pItem->buyPrice )
        {
            prnt( x + 90*sc, y + title_ht + 20*sc, z, sc, sc, textStd( "ValueInf", pItem->buyPrice) );
            if( drawMMButton( "Buy", 0, 0, x + 160*sc, y + ht - 20*sc, z, 100, .8f*sc, offerFlag, MMBUTTON_COLOR ) )
            {
                auction_addItem(pItem->pchIdentifier, 0, 1, pItem->buyPrice, AuctionInvItemStatus_Bidding, pItem->id );
            }
        }
        else
        {
             font(&game_9);
            font_color( CLR_AH_TEXT, CLR_AH_TEXT );

            ty = y + ht - link_ht;

            // For Sale/Bidding
               prnt( x + 15*sc, ty - 18*sc, z, sc, sc, "%d %s", pItem->numForSale, textStd("ForSale") );
            prnt( x + 15*sc, ty - 5*sc, z, sc, sc, "%d %s", pItem->numForBuy, textStd("BiddingString") );
            // History
             drawItemHistory( pItem,  x + 250*sc, y + title_ht + 10*sc, z, 180*sc, ht-title_ht-link_ht-20*sc, sc );

            // Bid/Offer
               prnt( x + 90*sc, y + ht - link_ht - 60*sc, z, sc, sc, "Bid" );
              if (drawTextEntryFrame( x + 110*sc, ty - 75*sc, z, 85*sc, 22*sc, sc, 2 ) )
                smf_SelectAllText( smfBid );

            smf_SetFlags(smfBid, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 0x7fffffff, SMFScrollMode_None, 
                     SMFOutputMode_None, SMFDisplayMode_NumbersWithCommas, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
            smf_SetScissorsBox(smfBid, x + 110*sc, y + ht - link_ht - 75*sc, 85*sc, 22*sc );
            smf_Display(smfBid,  x + 112*sc, ty - 73*sc, z+2, 80*sc, 0, 1, 0, &s_taSearchInput, 0);

               prnt( x + 202*sc,  y+ ht - link_ht - 58*sc, z, sc*1.3, sc*1.3, "X" );
               if( drawTextEntryFrame( x + 215*sc, ty - 75*sc, z, 20*sc, 22*sc, sc, 2 ) )
                smf_SelectAllText( smfBidCount );

             if( !smfBidCount->rawString || !*smfBidCount->rawString && !smfBlock_HasFocus(smfBidCount) )
                smf_SetRawText(smfBidCount, "1", 0 );

            smf_SetFlags(smfBidCount, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, MAX_AUCTION_STACK_SIZE, SMFScrollMode_None, 
                SMFOutputMode_None, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
            smf_SetScissorsBox(smfBidCount, x + 215*sc, ty - 75*sc, 25*sc, 22*sc );
            smf_Display(smfBidCount,  x + 217*sc, ty - 73*sc, z+2, 25*sc, 0, 1, 0, &s_taSearchInput, 0);


             if( smfBid->outputString && smfBidCount->outputString )
            {
                bid = atoi_ignore_nonnumeric(smfBid->outputString);
                amount = atoi(smfBidCount->outputString);
            }
            if( (INT64)bid*(INT64)amount <= 0 || (INT64)bid*(INT64)amount > (INT64)e->pchar->iInfluencePoints || gAuctionDisabled )
                offerFlag |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

            // if enter is pressed, send bid
            // also deal with tab order - bid amount is after max level and
            // before quantity
            if( smfBlock_HasFocus( smfBid ) ) 
            {
                KeyInput* input = inpGetKeyBuf();
                while( input )
                {
                    if( input->type == KIT_EditKey && ( input->key == INP_RETURN || input->key == INP_NUMPADENTER ) )
                    {
                        auction_addItem( pItem->pchIdentifier, 0, amount, bid, AuctionInvItemStatus_Bidding, pItem->id );
                        addItemToLocalInventory( pItem->pchIdentifier, 0, AuctionInvItemStatus_Bidding, 0, 0, 0, amount, bid, false, 0 );
                        break;
                    }
                    else if( input->type == KIT_EditKey && input->key == INP_TAB )
                    {
                        if( input->attrib == KIA_NONE )
                        {
                            smf_SelectAllText( smfBidCount );
                            break;
                        }
                        else if( input->attrib == KIA_SHIFT )
                        {
                            smf_SelectAllText( smfHigh );
                            break;
                        }
                    }
                    inpGetNextKey( &input );
                }
            }
            // back-tab to bid amount from quantity
            else if( smfBlock_HasFocus( smfBidCount ) )
            {
                KeyInput* input = inpGetKeyBuf();
                while( input )
                {
                    if( input->type == KIT_EditKey && input->key == INP_TAB )
                    {
                        if( input->attrib == KIA_NONE )
                        {
                            smf_SelectAllText( smfSearch );
                            break;
                        }
                        else if( input->attrib == KIA_SHIFT )
                        {
                            smf_SelectAllText( smfBid );
                            break;
                        }
                    }
                    inpGetNextKey( &input );
                }
            }

              if( drawMMButton( "MakeOffer", 0, 0, x + 165*sc, ty - 20*sc, z, 100, .8f*sc, offerFlag, color ) )
            {
                auction_addItem(pItem->pchIdentifier, 0, amount, bid, AuctionInvItemStatus_Bidding, pItem->id );
                addItemToLocalInventory(pItem->pchIdentifier, 0, AuctionInvItemStatus_Bidding, 0, 0, 0, amount, bid, false, 0);
            }

            if( pItem->type == kTrayItemType_Recipe || (pItem->type == kTrayItemType_Salvage && salvageitem_IsInvention(pItem->salvage)) )
            {
                CBox box;
                
                BuildCBox(&box, x, y + ht - link_ht, wd, link_ht );

                font_color(CLR_AH_TEXT,CLR_AH_TEXT);
                if( mouseCollision(&box) )
                    font_color(CLR_AH_TEXT_LIT,CLR_AH_TEXT_LIT);

                if( mouseClickHit(&box, MS_LEFT) )
                {
                    char * str;
                    int j;

                    estrCreate(&str);
                    if( pItem->type == kTrayItemType_Recipe )
                    {
                        for( j = eaSize(&pItem->recipe->ppSalvagesRequired)-1; j>=0; j-- )
                        {
                            const SalvageItem * pSal = pItem->recipe->ppSalvagesRequired[j]->pSalvage;
                            if( j == 0 )
                                estrConcatCharString(&str, textStd(pSal->ui.pchDisplayName));
                            else
                                estrConcatf(&str, "%s, ", textStd(pSal->ui.pchDisplayName));
                             eaiPush(&currentFilter.item_ids, auctionData_GetID(kTrayItemType_Salvage, pSal->pchName, 0));
                        }
                        setSearchText(str, kTrayItemType_Salvage);
                        currentFilter.reupdate = 1;
                    }
                    else
                    {
                        StashElement elt;
                         if( stashFindElementConst( g_DetailRecipeDict.stBySalvageComponent, pItem->salvage->pchName, &elt) )
                        {
                            DetailRecipe **ppRec = stashElementGetPointer(elt);
                            const char ** names = 0;
                            for( j = eaSize(&ppRec)-1; j>=0; j-- )
                            {
                                int id, k;
                                int name_found = 0;

                                id = auctionData_GetID(kTrayItemType_Recipe, ppRec[j]->pchName, ppRec[j]->level );
                                if( id > 0 )
                                    eaiPush(&currentFilter.item_ids, id);
        
                                for( k = 0; k < eaSize(&names); k++ )
                                {
                                    if( stricmp(ppRec[j]->ui.pchDisplayName, names[k]) == 0)
                                        name_found = 1;
                                }

                                if(!name_found)    
                                    eaPushConst(&names, ppRec[j]->ui.pchDisplayName );
                            }

                            for( j = eaSize(&names)-1; j>=0; j-- )
                            {
                                if( j == 0 )
                                    estrConcatCharString(&str, textStd(names[j]));
                                else
                                    estrConcatf(&str, "%s, ", textStd(names[j]));
                            }
                            eaDestroyConst(&names);
                            setSearchText(str, kTrayItemType_Recipe);
                            currentFilter.reupdate = 1;
                        }
                    }
                    estrDestroy(&str);
                }

                 font( &game_12 );
                font_outl(0);
                font_ital(1);
                if( pItem->type == kTrayItemType_Recipe )
                    cprnt(x + wd/2, y + ht - 3*sc, z, sc, sc, "FindMatchingSalvage" );
                else
                    cprnt(x + wd/2, y + ht - 3*sc, z, sc, sc, "FindMatchingRecipes" );
                    
                font_outl(1);
                font_ital(0);
            }
        }
        clipperPop();
    }
    else
    {
        CBox box;
        int tclr;

        BuildCBox(&box, x, y+2*sc, wd, ht-2*sc );
        if( mouseCollision(&box) )
            clr = CLR_AH_BACKGROUND_LIT&0xffffffcc;

        if( mouseClickHit(&box, MS_LEFT) )
        {
            if( selected_item != i )
            {
                smf_SetRawText( smfBid, "", 0 );
                smf_SetRawText( smfBidCount, "", 0 );
            }
            selected_item = i;
            auction_updateHistory(pItem->pchIdentifier);
        }

        drawFlatFrame( PIX3, R10, x, y+2*sc, z, wd, ht-2*sc, sc, clr, clr );
        uiBoxDefine(&uibox, x+5*sc, y, wd-10*sc, ht);
        clipperPushRestrict(&uibox);

           displayAuctionItemIcon( pItem, x + 3*sc, y + 1*sc, z, .6f*sc );

         font( &game_12 );
        tclr = colorFromItem(pItem, CLR_AH_TEXT);

        if( !(pItem->numForBuy || pItem->numForSale) )
             tclr = tclr&0xffffff88;
        
        font_color( tclr, tclr );

         cprntEx( x + 55*sc, y + 20*sc, z, sc, sc, CENTER_Y, "%s", textStd(pItem->displayName) );
        clipperPop();
    }

    return ht;
}

static void resultsRecurseStashtable( AuctionItem *** results, StashTable stStash, int depth );
static void resultsRecurseStashtable( AuctionItem *** results, StashTable stStash, int depth )
{
    StashTableIterator iHash;
    StashElement eltHash;
    int i;
    Entity* e = playerPtr();

    //if(eaSize(results)>1000)
    //    return; 

    if (!e)
    {
        return;
    }

     stashGetIterator(stStash, &iHash);
    while( stashGetNextElement(&iHash, &eltHash) )
    {
        MultiLevelStashNode *pStashNode = 0;
        const char * substr = stashElementGetStringKey(eltHash);
        pStashNode = stashFindPointerReturnPointer(stStash, substr);

        if( pStashNode && pStashNode->stStash )
        {
            resultsRecurseStashtable( results, pStashNode->stStash, (depth+1) );
        }
        else if( pStashNode && eaSize(&pStashNode->ppItems) )
        {
            for( i = 0; i < eaSize(&pStashNode->ppItems); i++ )
            {
                if( (!currentFilter.item_type || pStashNode->ppItems[i]->type == currentFilter.item_type)
                     && DoesAuctionItemPassAuctionRequiresCheck(pStashNode->ppItems[i], e->pchar) )
                {
                    eaPushUnique(results, pStashNode->ppItems[i]);
                }    
            }
        }
        //else
        //{
        //    AuctionKeyWord * pKey;
        //    if( stashFindPointer(g_stAuctionKeyWord, textStd(substr), &pKey ) )
        //    {
        //        for( i = 0; i < eaSize(&pKey->ppItem); i++ )
        //            eaPushUnique(results, pKey->ppItem[i]);
        //    }
        //}
    }
}

#define BATCH_ITEMSTATUS_REQ_SIZE 1000
#define BATCH_ITEMSTATUS_REQ_UPDATE_TIME 0.3333f
#define BATCH_ITEMSTATUS_REQ_WAIT_TIME 5.0f

static void doBatchItemStatusRequests(int lowID, int highID, int newRequest, int *displayedIDs )
{    
    static int requestProcessing; // 0 - no request, 1 - new request, 2 - processing 
    static int curStatusRequestIndex;
    static int requestTimer;

     if(!requestTimer)
    {
        requestTimer = timerAlloc();
        timerStart(requestTimer);
    }
    
    if( newRequest )
        requestProcessing = 1;

    if( requestProcessing==1 && eaiSize(&displayedIDs) < 500 && timerElapsed(requestTimer) >= BATCH_ITEMSTATUS_REQ_WAIT_TIME)
    {
        auction_listRequestItemStatus( displayedIDs );
        timerStart(requestTimer);
        requestProcessing = 0;
        return;
    }

    if( lowID > highID )
        return;

    if ( requestProcessing==1 &&  timerElapsed(requestTimer) >= BATCH_ITEMSTATUS_REQ_WAIT_TIME )
    {
        requestProcessing = 1;
        curStatusRequestIndex = MAX(lowID, 0);
        auction_batchRequestItemStatus(curStatusRequestIndex, MIN( highID-lowID, BATCH_ITEMSTATUS_REQ_SIZE));

        curStatusRequestIndex +=  MIN( highID-lowID, BATCH_ITEMSTATUS_REQ_SIZE);
        if ( curStatusRequestIndex > highID )
        {
            requestProcessing = 0;
            curStatusRequestIndex = 0;
        }
        timerStart(requestTimer);
    }
    else if ( requestProcessing==2 && timerElapsed(requestTimer) >= BATCH_ITEMSTATUS_REQ_UPDATE_TIME )
    {
        auction_batchRequestItemStatus(curStatusRequestIndex, MIN(highID-curStatusRequestIndex, BATCH_ITEMSTATUS_REQ_SIZE));
        curStatusRequestIndex += BATCH_ITEMSTATUS_REQ_SIZE;
        if ( curStatusRequestIndex > highID )
        {
            requestProcessing = 0;
            curStatusRequestIndex = 0;
        }
        timerStart(requestTimer);
    }
}

static int s_sort_idx;

typedef enum AuctionSortType
{
    kAuctionSort_Name = 1,
    kAuctionSort_Level,
    kAuctionSort_Rarity,
    kAuctionSort_NumForSale,
    kAuctionSort_NumBidding,
}AuctionSortType;

static int auction_resultsort(const void* p1Data, const void* p2Data)
{
    const AuctionItem ** p1 = (const AuctionItem **)p1Data;
    const AuctionItem ** p2 = (const AuctionItem **)p2Data;
    int sort_type = ABS(s_sort_idx);

    //if this matches arc id
    switch( sort_type)
    {
        xcase kAuctionSort_Name:    
        {
            int val = strcmp( (*p1)->sortName,  (*p2)->sortName );

            if( val ) // Name
                return val;
            else if( (*p1)->lvl - (*p2)->lvl != 0 ) // Secondary Level
                return  ((*p2)->lvl - (*p1)->lvl);
            else
                return (*p2)->id - (*p1)->id; // Lastly ID to prevent flip-flopping
        }
        xcase kAuctionSort_Level:
        {
            if( (*p1)->lvl - (*p2)->lvl != 0 ) //  Level
                return  ((*p2)->lvl - (*p1)->lvl);
            else
            {
                int val = strcmp( (*p1)->sortName, (*p2)->sortName );
                if( val ) // Name
                    return val;
                else
                    return (*p2)->id - (*p1)->id; 
            }
        }
        xcase kAuctionSort_Rarity:
        {
            int rarity1 = -1, rarity2 = -1;

            if( (*p1)->type == kTrayItemType_Salvage )
                rarity1 = (*p1)->salvage->rarity;
            if( (*p1)->type == kTrayItemType_Recipe )
                rarity1 = (*p1)->recipe->Rarity;
            if( (*p1)->type == kTrayItemType_SpecializationInventory )
            {
                const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel((*p1)->enhancement->pchFullName, (*p1)->lvl+1);
                if( pRecipe )
                    rarity1 = pRecipe->Rarity;
            }

            if( (*p2)->type == kTrayItemType_Salvage )
                rarity2 = (*p2)->salvage->rarity;
            if( (*p2)->type == kTrayItemType_Recipe )
                rarity2 = (*p2)->recipe->Rarity;
            if( (*p1)->type == kTrayItemType_SpecializationInventory )
            {
                const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel((*p2)->enhancement->pchFullName, (*p2)->lvl+1);
                if( pRecipe )
                    rarity2 = pRecipe->Rarity;
            }
    

            if( rarity2 -rarity1 != 0 ) //  Level
                return  (rarity2 -rarity1);
            else
            {
                int val = strcmp( (*p1)->sortName, (*p2)->sortName );
                if( val ) // Name
                    return val;
                else
                    return (*p2)->id - (*p1)->id; 
            }
        }
        xcase kAuctionSort_NumForSale:
        {
            if( (*p1)->numForSale - (*p2)->numForSale != 0 ) 
                return  ((*p2)->numForSale - (*p1)->numForSale);
            else
            {
                int val = strcmp( (*p1)->sortName,  (*p2)->sortName );
                if( val ) // Name
                    return val;
                else
                    return (*p2)->id - (*p1)->id; 
            }
        }
        xcase kAuctionSort_NumBidding:
        {
            if( (*p1)->numForBuy - (*p2)->numForBuy != 0 ) 
                return  ((*p2)->numForBuy - (*p1)->numForBuy);
            else
            {
                int val = strcmp( (*p1)->sortName, (*p2)->sortName );
                if( val ) // Name
                    return val;
                else
                    return (*p2)->id - (*p1)->id; 
            }
        }
    }

    return 0;
}


static F32 aucton_sortButton( F32 x, F32 y, F32 z, F32 sc, int eSort, char * pchText, int active, int *resort)
{
    AtlasTex *arrow = 0;
    F32 ty = y, wd, ht = 26*sc;
    CBox box;
    UIBox uibox;
    int box_clr = 0xfffffff22;
    char *translatedText;

    font(&game_12);
    font_ital(1);
     if(active)
        font_color(CLR_AH_TEXT,CLR_AH_TEXT);
    else
        font_color(CLR_AH_TEXT ,CLR_AH_TEXT);

    translatedText = textStd(pchText);

    wd = str_wd( font_grp, sc, sc, translatedText ) + 2*R10*sc + 10*sc;

    if(s_sort_idx == eSort && !active)
        s_sort_idx = 1;

    if( -s_sort_idx == eSort)
    {
        arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        ty = y - 12*sc;
    }
    else if( s_sort_idx == eSort) 
    {
        arrow = atlasLoadTexture( "chat_separator_arrow_up.tga" );
        ty = y - 14*sc;
    }

    BuildCBox(&box, x - wd, y - ht, wd, ht );
    uiBoxDefine(&uibox, x - wd, y - ht, wd, ht );
    clipperPush(&uibox);

    if(active)
    {
        if( mouseCollision(&box) )
            box_clr = 0xfffffff88;
        if( mouseClickHit(&box, MS_LEFT) )
        {
            box_clr = 0xffffffaa;

            if( ABS(s_sort_idx) == eSort )
                s_sort_idx = -s_sort_idx;
            else
                s_sort_idx = eSort;

            *resort = 1;
        }
    }
    else
    {
        box_clr = CLR_TAB_INACTIVE;
    }

    if(box_clr)
    {
        AtlasTex * end = atlasLoadTexture("Tab_end.tga");
        AtlasTex * mid = atlasLoadTexture("Tab_mid.tga");

        display_sprite( end, x - wd, y - ht + 10*sc, z, sc, sc, box_clr );
        display_spriteFlip( end, x - end->width*sc, y - ht + 10*sc, z, sc, sc, box_clr, FLIP_HORIZONTAL );
        display_sprite( mid, x - wd + end->width*sc, y - ht + 10*sc, z, (wd-2*end->width*sc)/mid->width, sc, box_clr );
    }

    prnt( x-wd+8*sc, y, z+5, sc, sc, "%s", translatedText );

    if(arrow)
        display_sprite(arrow, x - (R10+8)*sc, ty, z+5, 12*sc/arrow->width, 10*sc/arrow->height, CLR_WHITE );

    clipperPop();
    font_ital(0);

    return wd;
}

typedef enum FilteredResultReason
{
    FilteredResultReason_Bidding,
    FilteredResultReason_Selling,
    FilteredResultReason_Level,
    FilteredResultReason_Origin,
    FilteredResultReason_Rarity,
    FilteredResultReason_Count
}FilteredResultReason;

static void drawAuctionResultsPane( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int color )
{
    static AuctionItem **ppResultList;
    static ScrollBar sb = { 0 };
     int i, last_displayed_item = 0, idx, j, filtercount[FilteredResultReason_Count] = {0}, display_count = 0, origins, rarities, count;
    F32 start_y = y;
    UIBox box; 
    static int min_id = 0x7fffffff, max_id = 0;
    Entity *e = playerPtr();
    int *displayedItems = 0;

    if (!e)
    {
        return;
    }

     uiBoxDefine(&box, x, y, wd-PIX3*sc, ht );
    
    if( currentFilter.update )
    {
        min_id = 0x7fffffff;
        max_id = 0;
        selected_item = -1;

        // make a new results list
        eaClear(&ppResultList);  
        ppResultList = 0;

        if( currentFilter.pStashNode )
        {
            if( currentFilter.pStashNode->stStash )
                 resultsRecurseStashtable( &ppResultList, currentFilter.pStashNode->stStash, 0 );
            else
            {
                for( i = 0; i < eaSize(&currentFilter.pStashNode->ppItems); i++ )
                {    
                    if( (!currentFilter.item_type || currentFilter.pStashNode->ppItems[i]->type == currentFilter.item_type)
                         && DoesAuctionItemPassAuctionRequiresCheck(currentFilter.pStashNode->ppItems[i], e->pchar) )
                    {
                        eaPushUnique(&ppResultList, currentFilter.pStashNode->ppItems[i]);
                    }                
                }
            }
        }
        else if( eaiSize(&currentFilter.item_ids) )
        {
            for( i = 0; i < eaiSize(&currentFilter.item_ids); i++ )
            {    
                int idx = currentFilter.item_ids[i];
                if( EAINRANGE( idx,  g_AuctionItemDict.ppItems ) 
                    && DoesAuctionItemPassAuctionRequiresCheck(g_AuctionItemDict.ppItems[idx], e->pchar) )
                {
                    eaPush(&ppResultList, g_AuctionItemDict.ppItems[idx]);
                }
            }
            eaiDestroy(&currentFilter.item_ids);
            currentFilter.item_ids = 0;
        }
        else
        {
            if( currentFilter.auto_complete )
            {
                for( i = 0; i < eaSize(&currentFilter.ppKeywords); i++ )
                {
                    AuctionKeyWord * pKey;
                    if( stashFindPointer(g_stAuctionKeyWord, currentFilter.ppKeywords[i], &pKey ) )
                    {
                        for( j = 0; j < eaSize(&pKey->ppItem); j++ )
                        {
                            if( (!currentFilter.item_type || pKey->ppItem[j]->type == currentFilter.item_type)
                                 && DoesAuctionItemPassAuctionRequiresCheck(pKey->ppItem[j], e->pchar) )
                            {
                                eaPushUnique(&ppResultList, pKey->ppItem[j]);
                            }                        
                        }
                        for( j = 0; j < eaSize(&pKey->ppStash); j++ )
                            resultsRecurseStashtable( &ppResultList, pKey->ppStash[j], 0 );
                    }
                }
            }
            else
            {
                for( i = 0; i < eaSize(&currentFilter.ppKeywords); i++ )
                {
                    for( j = 0; j < eaSize(&g_AuctionItemDict.ppItems); j++ )
                    {
                         if( strstri(textStd(g_AuctionItemDict.ppItems[j]->displayName), currentFilter.ppKeywords[i] ) 
                            && IsAuctionItemValid(g_AuctionItemDict.ppItems[j])
                            && DoesAuctionItemPassAuctionRequiresCheck(g_AuctionItemDict.ppItems[j], e->pchar) )
                        {
                            eaPushUnique(&ppResultList, g_AuctionItemDict.ppItems[j]);
                        }
                    }
                }
            }
        }

        if(eaSize(&ppResultList)==1)
            selected_item = 0;

        for( i = 0; i < eaSize(&ppResultList); i++ )
        {
            if( ppResultList[i]->id < min_id )
                min_id = ppResultList[i]->id;
            if( ppResultList[i]->id > max_id )
                max_id = ppResultList[i]->id;
        }
    }

    count = eaSize(&ppResultList);
       if( count ) // sort options
    {
        F32 twd = wd - PIX3*sc;
        int resort = 0;
        
         twd    -= PIX3*sc + aucton_sortButton( x+twd, y-2*sc, z, sc, kAuctionSort_NumBidding, "BiddingString", 1, &resort);
        twd -= PIX3*sc + aucton_sortButton( x+twd, y-2*sc, z, sc, kAuctionSort_NumForSale, "ForSale", 1, &resort);
         twd -= PIX3*sc + aucton_sortButton( x+twd, y-2*sc, z, sc, kAuctionSort_Rarity, "SortByNameRarity", 1, &resort);
        twd -= PIX3*sc + aucton_sortButton( x+twd, y-2*sc, z, sc, kAuctionSort_Level, "PowerLevel", 1, &resort);
        twd -= PIX3*sc + aucton_sortButton( x+twd, y-2*sc, z, sc, kAuctionSort_Name, "SortByNameString", 1, &resort);

          if( (currentFilter.update) || (resort && currentFilter.lastSort != ABS(s_sort_idx)) )
        {
            currentFilter.lastSort = s_sort_idx;
            eaQSort( ppResultList, auction_resultsort );
        }

        font(&gamebold_12);
        font_color(CLR_AH_TEXT ,CLR_AH_TEXT);
        cprntEx(x + twd - str_wd(font_grp,sc,sc,"MMSortBy") - 2*PIX3*sc, y-2*sc, z+1, sc, sc, 0, "MMSortBy" );    
    }

     clipperPush(&box);

    origins = combobox_getBitfieldFromElements(&s_originCombo);
    rarities = combobox_getBitfieldFromElements(&s_rarityCombo);
    
    if (toolTipEnhancement)
    {
        if(!toolTipEnhancement->isDisplayingToolTip)
            uiEnhancementFree(&toolTipEnhancement);
        else
            toolTipEnhancement->isDisplayingToolTip = 0;
    }

     for( idx = 0; idx < count; idx++ )
    {
        i = idx;
        if( s_sort_idx < 0 ) // rather than reorder entire list for a reverse sort, just go through it backwards
            i = count - idx -1;

        if( ppResultList[i]->playerType )
        {
            if ((ppResultList[i]->playerType == 1 && !ENT_IS_HERO(e)) ||
                (ppResultList[i]->playerType == 2 && !ENT_IS_VILLAIN(e)))
            {
                continue; // no reason needed 
            }
        }

        if( currentFilter.item_type && ppResultList[i]->type != currentFilter.item_type )
        {
            continue; // no reason needed here
        }

        // Level Filter
          if( ppResultList[i]->type == kTrayItemType_Recipe && (ppResultList[i]->lvl < currentFilter.iMinLevel || ppResultList[i]->lvl > currentFilter.iMaxLevel) )
        {
            filtercount[FilteredResultReason_Level]++;
            continue;
        }

         if( ppResultList[i]->type == kTrayItemType_SpecializationInventory && (ppResultList[i]->lvl < currentFilter.iMinLevel-1 || ppResultList[i]->lvl > currentFilter.iMaxLevel-1) )
        {
            filtercount[FilteredResultReason_Level]++;
            continue;
        }

        // Origin Filter
        if( !(origins&(1<<eaSize(&g_CharacterOrigins.ppOrigins))) && ppResultList[i]->type == kTrayItemType_SpecializationInventory )
        {    
            int j, ok = 0;

            for( j = eaSize(&g_CharacterOrigins.ppOrigins)-1; j >= 0; j-- )
            {
                if( (origins&(1<<(j))) && strstriConst(ppResultList[i]->enhancement->pchFullName, textStd(g_CharacterOrigins.ppOrigins[j]->pchDisplayName)) )
                    ok = 1;
            }

            if(!ok)
            {
                filtercount[FilteredResultReason_Origin]++;
                continue;
            }    
        }

        // Rarity Filter
        if( !(rarities&(1<<kSalvageRarity_Count)) && (ppResultList[i]->type == kTrayItemType_Recipe || ppResultList[i]->type == kTrayItemType_Salvage || ppResultList[i]->type == kTrayItemType_SpecializationInventory ) )
        {
            int rarity;

            if(ppResultList[i]->type == kTrayItemType_Salvage && ppResultList[i]->salvage )
                rarity = (1<<ppResultList[i]->salvage->rarity);
            else if ( ppResultList[i]->type == kTrayItemType_Recipe && ppResultList[i]->recipe )
                rarity = (1<<ppResultList[i]->recipe->Rarity);
            else if( ppResultList[i]->type == kTrayItemType_SpecializationInventory && ppResultList[i]->enhancement )
            {
                const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(ppResultList[i]->enhancement->pchFullName, ppResultList[i]->lvl+1);
                if( pRecipe )
                    rarity = (1<<pRecipe->Rarity);
            }

            if( !(rarity&rarities) )
            {
                filtercount[FilteredResultReason_Rarity]++;
                continue;
            }
        }

        eaiPush(&displayedItems, ppResultList[i]->id); // even if we don't display it add it to list, otherwise it won't get updated.

        if( currentFilter.bBidding && !ppResultList[i]->numForBuy )
        {
            filtercount[FilteredResultReason_Bidding]++;
            continue;
        }

        if( currentFilter.bForsale && !(ppResultList[i]->numForSale || ppResultList[i]->buyPrice) )
        {
            filtercount[FilteredResultReason_Selling]++;
            continue;
        }

         display_count++;
         if( y- sb.offset < start_y - 100*sc  || y- sb.offset > start_y + ht ) // pre-cull long lists
            y += selected_item==i?100*sc:40*sc;
        else
            y += drawAuctionItem( ppResultList[i], i, x, y - sb.offset, z+10, wd, sc, color );
        last_displayed_item = i;
        
    }

    if( display_count == 1 )
        selected_item = last_displayed_item;

    doBatchItemStatusRequests( min_id, max_id, currentFilter.update, displayedItems);
    eaiDestroy(&displayedItems);

     if(!eaSize(&ppResultList))
    {
        font( &title_12 );
        font_color(CLR_AH_TEXT, CLR_AH_TEXT);
        cprnt( x+wd/2, y+ht/2, z, sc, sc, "TooManyResults" );
        cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "PleaseRefineSearch" );
    }
    else if( !display_count )
    {
        int reason, top_cnt = 0;
        font( &title_12 );
        font_color(CLR_AH_TEXT, CLR_AH_TEXT);
        cprnt( x+wd/2, y+ht/2, z, sc, sc, "NoResultsFound" );
        for( i = 0; i < 4; i++ )
        {
            if( top_cnt < filtercount[i] )
            {
                top_cnt = filtercount[i];
                reason = i;
            }
        }
        switch( reason )
        {
            xcase FilteredResultReason_Bidding: cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "NoResultsWithBidders" );
            xcase FilteredResultReason_Selling: cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "NoResultsForSale" );
            xcase FilteredResultReason_Level:    cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "NothingInLevelRange" );
            xcase FilteredResultReason_Origin:    cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "NoOriginMatches" );
            xcase FilteredResultReason_Rarity:     cprnt( x+wd/2, y+ht/2 + 20*sc, z, sc, sc, "NoRarityMatches" );
        }
    }

    clipperPop();

    sb.use_color = 0;
       doScrollBar(&sb, ht - 20*sc, y-start_y, x+wd, start_y + 9*sc, z, 0, &box );

    if( !currentFilter.reupdate ) // users can click on items in list which ill invoke a new search.  However items in list also check to see if there was just an update to retrieve their histories
        currentFilter.update = 0; // so we can't clear flag before loop, thus double flag silliness.
    else
        currentFilter.reupdate = 0;
    
}



static void auctionUpdateInventory()
{
    int i;
    int lastSize = 0;
    static int timer = 0;
    Entity * e = playerPtr();

    if ( !timer )
    {
        timer = timerAlloc();
        timerStart(timer);
        auction_updateInventory();
    }

    if ( timerElapsed(timer) >= 30.0f )
    {
        auction_updateInventory();
        timerStart(timer);
    }

    if ( e->pchar->auctionInv && !e->pchar->auctionInvUpdated )
        return;

    if ( !e->pchar->auctionInv )
        return;

    while(eaSize(&ppLocalInventory))
    {
        AuctionItem * pItem = eaRemove(&ppLocalInventory,0);
        free(pItem);
    }
    
    for ( i = 0; i < eaSize(&e->pchar->auctionInv->items); ++i )
    {
        addItemToLocalInventory((char*)e->pchar->auctionInv->items[i]->pchIdentifier, e->pchar->auctionInv->items[i]->id, 
                e->pchar->auctionInv->items[i]->auction_status, e->pchar->auctionInv->items[i]->amtCancelled,
                e->pchar->auctionInv->items[i]->amtStored, e->pchar->auctionInv->items[i]->infStored,
                e->pchar->auctionInv->items[i]->amtOther, e->pchar->auctionInv->items[i]->infPrice,
                e->pchar->auctionInv->items[i]->bMergedBid, 0);
    }
}

static void auctionHouseGetInfStored(AuctionItem* pItem)
{
    if( !ent_canAddInfluence( playerPtr(), pItem->infStored ) )
    {
         plaquePopup(textStd("CannotClaimInf"),0);
    }
    else
    {
        auction_getInfStored(pItem->auction_id);
        pItem->infStored = 0;
        pItem->amtOther = 0;
        if ( !pItem->amtStored )
            eaFindAndRemove(&ppLocalInventory, pItem );
    }
}

static void auctionHouseGetAmtStored(AuctionItem* pItem)
{
    auction_getAmtStored(pItem->auction_id);
    pItem->amtStored = 0;
    if ( !pItem->infStored )
        eaFindAndRemove(&ppLocalInventory, pItem );
}

static void auctionPostItem(AuctionItem * pItem)
{
    int price = atoi_ignore_nonnumeric(smfPrice->outputString);
    auction_changeItemStatus(pItem->auction_id, price, AuctionInvItemStatus_ForSale);
    local_idx = -1;
}

static DialogCheckbox hideFeeDCB[] = { { "HideFeeWarning", 0, kUO_HideFeePrompt}};

static void auctionPostItemAdapter(void * arg0)
{
    auctionPostItem((AuctionItem *)arg0);
}

static F32 drawAuctionInventoryItem( AuctionItem * pItem, int status, int i, F32 x, F32 y, F32 z, F32 wd, F32 sc, int color )
{
    Entity *e = playerPtr();
      F32 ht = local_idx==i?50*sc:40*sc;
     int tclr, clr = local_idx==i?CLR_AH_BACKGROUND_SELECTED&0xffffffcc:CLR_AH_BACKGROUND&0xffffffcc;
    CBox box;
    UIBox uibox;
     F32 reserved_wd = pItem->auction_status==AuctionInvItemStatus_Stored?280*sc:100*sc;
       static int more_showing = 1;
      
    x += 10*sc;
     wd -= 20*sc;

     if( local_idx==i )
        ht = 150*sc;

    BuildCBox( &box, x, y, wd, ht);
       if( mouseCollision(&box) )
        clr = CLR_AH_BACKGROUND_LIT&0xffffffcc;

       drawFlatFrame( PIX3, R10, x, y+2*sc, z, wd, ht-2*sc, sc, clr, clr );
     displayAuctionItemIcon( pItem, x, y, z, .6f*sc );
 
    uiBoxDefine(&uibox, x, y, wd, ht);
    clipperPushRestrict(&uibox);

     font( &game_14 );
    tclr = colorFromItem(pItem, CLR_AH_TEXT);
     font_color( tclr, tclr );

      if (pItem->auction_status == AuctionInvItemStatus_Stored) 
    {
        int postFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
        int fee = 0;
        int price = 0;
        INT64 total;

         uiBoxDefine(&uibox, x, y, wd - 360*sc, ht);
        clipperPushRestrict(&uibox);
        if( pItem->amtStored > 1 )
            prnt( x + 50*sc, y + 27*sc, z, sc, sc, "%i X %s", pItem->amtStored, textStd(pItem->displayName) );
        else
            prnt( x + 50*sc, y + 27*sc, z, sc, sc, "%s", textStd(pItem->displayName) );
        clipperPop();

        font( &title_9 );
        font_color(CLR_AH_TEXT, CLR_AH_TEXT);
        font_outl(0);

          if( drawTextEntryFrame( x + wd - 350*sc, y + 10*sc, z, 100*sc, 22*sc, sc, 0 ) )
        {
            local_idx = i;
        }

        if( pItem->amtStored > 1 )
            prnt( x + wd - 245*sc, y + 27*sc, z+1, sc, sc, "EachString" );

         if( local_idx != i )
            cprnt( x + wd - 300*sc, y + 27*sc, z+1, sc, sc, "EnterPrice" );
        else
        {
            price = atoi_ignore_nonnumeric(smfPrice->outputString);
            total = (INT64)price * pItem->amtStored; // do not allow stack prices to exceed 2 billion
             fee = calcAuctionSellFee(total);

            smf_SetFlags(smfPrice, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 0x7fffffff, SMFScrollMode_None, 
                SMFOutputMode_None, SMFDisplayMode_NumbersWithCommas, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
            //smf_SetScissorsBox(smfPrice, x + wd - 270*sc, y + 10*sc,  100*sc, 22*sc );
            s_taSearchInput.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
            smf_Display(smfPrice, x + wd - 345*sc, y + 12*sc, z+2, 100*sc, 0, 0, 0, &s_taSearchInput, 0);

            if(fee)
            {
                font_outl(1);
                font(&game_9);
                font_outl(0);
                if ( fee > e->pchar->iInfluencePoints )
                    font_color( CLR_RED, CLR_RED );
                cprnt( x + wd - 300, y + 47*sc, z+1, sc, sc, "%s", textStd("TotalFeeColonValue", fee) );
            }
        }
        font_outl(1);


          if( drawMMButton( "Find", 0, 0, x + wd - 165*sc, y + 20*sc, z, 60*sc, sc*.8,postFlags, CLR_GREY )  )
        {
            AuctionItem *dictItem;
            if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
                eaiPush(&currentFilter.item_ids, dictItem->id );

            setSearchText( textStd(pItem->displayName), pItem->type );
        }

        if( local_idx != i || !fee || fee > e->pchar->iInfluencePoints || gAuctionDisabled )
            postFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        
        if ( total > 2000000000 )
            postFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if (!AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch")))
            postFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if( drawMMButton( "PostItem", 0, 0, x + wd - 100*sc, y + 20*sc, z, 60*sc, sc*.8, postFlags, color )  )
        {
            collisions_off_for_rest_of_frame = 1;
            if( fee > 100000 && !optionGet( kUO_HideFeePrompt ) )
                dialog( DIALOG_YES_NO, -1, -1, -1, 300, textStd("LargePostingFee", fee), NULL, auctionPostItemAdapter, NULL, NULL,0,sendOptions,hideFeeDCB,1,0,0,pItem );
            else
            {
                auction_changeItemStatus(pItem->auction_id, price, AuctionInvItemStatus_ForSale);
                local_idx = -1;
            }
        }

         postFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
        if( gAuctionDisabled )
            postFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);
        if( drawMMButton( "GetFromInv", 0, 0, x + wd - 35*sc, y + 20*sc, z, 60*sc, sc*.8, postFlags, color ) )
        {
            AuctionItem *pTest=0;
            collisions_off_for_rest_of_frame = 1;
            stashFindPointer(g_AuctionItemDict.stAuctionItems, pItem->pchIdentifier, &pTest);
            auctionHouseGetAmtStored(pItem);

            local_idx = -1;
        }
    }    
     if (pItem->auction_status == AuctionInvItemStatus_ForSale && (!status || status == AuctionInvItemStatus_ForSale) )
    {
        int buttonFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
        if( gAuctionDisabled )
            buttonFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if( pItem->amtStored > 1 )
            prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%i X %s", pItem->amtStored, textStd(pItem->displayName) );
        else
            prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%s", textStd(pItem->displayName) );

        font_color( CLR_AH_TEXT_LIT, CLR_AH_TEXT_LIT );
        font(&game_12);
        font_outl(0);
        font_ital(1);
        if( pItem->amtStored > 1 )
        {
            INT64 total = (INT64)pItem->infPrice*pItem->amtStored;
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd( "ListedForInfEach", total, pItem->infPrice ));
        }
        else
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd( "ListedForInf", pItem->infPrice ));
        font_ital(0);
        font_outl(1);

        if( drawMMButton( "FindString", 0, 0, x + wd - 125*sc, y + 20*sc, z, 60*sc, sc*.8, buttonFlags, CLR_GREY )  )
        {
            AuctionItem *dictItem;
            if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
                eaiPush(&currentFilter.item_ids, dictItem->id );

            setSearchText( textStd(pItem->displayName), pItem->type );
        }

        if( drawMMButton( "GetFromInv", 0, 0, x + wd - 50*sc, y + 20*sc, z, 80*sc, sc*.8, buttonFlags, color) )
        {
            collisions_off_for_rest_of_frame = 1;
            auctionHouseGetAmtStored(pItem);
        }
    }
     if (pItem->auction_status == AuctionInvItemStatus_Sold || ( status == AuctionInvItemStatus_Sold && pItem->auction_status == AuctionInvItemStatus_ForSale && pItem->amtOther) ) 
    {
        int fee = calcAuctionSoldFee(pItem->infStored, pItem->infPrice, pItem->amtOther, pItem->amtCancelled);

        int buttonFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
        if( gAuctionDisabled )
            buttonFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if( pItem->amtOther > 1 )
            prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%i X %s", pItem->amtOther, textStd(pItem->displayName) );
        else
            prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%s", textStd(pItem->displayName) );

        font_color( CLR_AH_TEXT_LIT, CLR_AH_TEXT_LIT );
        font(&game_12);
        font_outl(0);
        font_ital(1);
        if( pItem->amtCancelled && !pItem->infStored )
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd( "RefundedInf", -fee ));
        else if( pItem->amtCancelled )
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd( "SoldForInfAdjusted", pItem->infStored, fee ));
        else
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd( "SoldForInf", pItem->infStored, fee ));
        font_ital(0);
        font_outl(1);

        if( drawMMButton( "FindString", 0, 0, x + wd - 125*sc, y + 20*sc, z, 60*sc, sc*.8, buttonFlags, CLR_GREY )  )
        {
            AuctionItem *dictItem;
            if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
                eaiPush(&currentFilter.item_ids, dictItem->id );

            setSearchText( textStd(pItem->displayName), pItem->type );
        }

        if( drawMMButton( "GetInf", 0, 0, x + wd - 50*sc, y + 20*sc, z, 80*sc, sc*.8,buttonFlags, color ))
        {
            collisions_off_for_rest_of_frame = 1;
            auctionHouseGetInfStored(pItem);
        }
    }
    if (pItem->auction_status == AuctionInvItemStatus_Bidding  && (!status || status == AuctionInvItemStatus_Bidding)  )
    { 

        int buttonFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
        if( gAuctionDisabled )
            buttonFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if( pItem->amtOther > 1 )
             prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%i X %s", pItem->amtOther, textStd(pItem->displayName) );
        else
            prnt( x + 50*sc, y + 20*sc, z, sc, sc, "%s", textStd(pItem->displayName) );

        font_color( CLR_AH_TEXT_LIT, CLR_AH_TEXT_LIT );
        font(&game_12);
        font_outl(0);
        font_ital(1);
        if( pItem->bMergedBid )
        {
            font_color( CLR_RED, CLR_RED );
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd("NotBiddingForInf", pItem->infStored ));
        }
        else if(  pItem->infPrice != pItem->infStored )
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd("BiddingForInfEach", pItem->infStored, pItem->infPrice ));
        else
            prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd("BiddingForInf", pItem->infStored ));
        font_ital(0);
        font_outl(1);


        if( drawMMButton( "FindString", 0, 0, x + wd - 125*sc, y + 20*sc, z, 60*sc, sc*.8, buttonFlags, CLR_GREY )  )
        {
            AuctionItem *dictItem;
            if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
                eaiPush(&currentFilter.item_ids, dictItem->id );

            setSearchText( textStd(pItem->displayName), pItem->type );
        }

        if( drawMMButton( "CancelBid", 0, 0, x + wd - 50*sc, y + 20*sc, z, 80*sc, sc*.8, buttonFlags, color ) )
        {
            collisions_off_for_rest_of_frame = 1;
            auctionHouseGetInfStored(pItem);
        }
    }
    if (pItem->auction_status == AuctionInvItemStatus_Bought || ( status == AuctionInvItemStatus_Bought && pItem->auction_status == AuctionInvItemStatus_Bidding && pItem->amtStored))
    {

        int buttonFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;
         if( gAuctionDisabled )
            buttonFlags |= (MMBUTTON_GRAYEDOUT|MMBUTTON_LOCKED);

        if( pItem->amtStored > 1 )
            prnt( x + 55*sc, y + 20*sc, z, sc, sc, "%i X %s", pItem->amtStored, textStd(pItem->displayName) );
        else
            prnt( x + 55*sc, y + 20*sc, z, sc, sc, "%s", textStd(pItem->displayName));

        font(&game_12);
        font_color( CLR_AH_TEXT_LIT, CLR_AH_TEXT_LIT );
        font_outl(0);
        font_ital(1);
        prnt( x + 60*sc, y + 35*sc, z+1, sc, sc, textStd("PaidInf", pItem->infPrice ), pItem->infPrice );
        font_ital(0);
        font_outl(1);

        if( drawMMButton( "FindString", 0, 0, x + wd - 125*sc, y + 20*sc, z, 60*sc, sc*.8, buttonFlags, CLR_GREY )  )
        {
            AuctionItem *dictItem;
            if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
                eaiPush(&currentFilter.item_ids, dictItem->id );

            setSearchText( textStd(pItem->displayName), pItem->type );
        }

        if( drawMMButton( "GetFromInv", 0, 0, x + wd - 50*sc, y + 20*sc, z, 80*sc, sc*.8, buttonFlags, color ))
        {
            collisions_off_for_rest_of_frame = 1;
            auctionHouseGetAmtStored(pItem);
        }
    }

    if( local_idx==i )
    {
        AuctionItem *dictItem;
        drawItemInfo( pItem, x +10*sc, y + 50*sc, z, wd-300*sc, ht - 60*sc, sc );
        font(&game_9);

        if( stashFindPointer( auctionData_GetDict()->stAuctionItems, pItem->pchIdentifier, &dictItem) )
        {    
            prnt(  x + wd - 270*sc, y + 85*sc, z, sc, sc, "%d %s", dictItem->numForBuy, textStd("BiddingString") );
            prnt(  x + wd - 270*sc, y + 100*sc, z, sc, sc, "%d %s", dictItem->numForSale, textStd("ForSale") );
        }
         drawItemHistory( pItem,  x + wd - 195*sc, y + 50*sc, z, 185*sc, ht - 60*sc, sc );
     }

    clipperPop();

       BuildCBox( &box, x, y, wd, ht);
    if(mouseClickHit(&box, MS_LEFT))
    {    
        auction_updateHistory(pItem->pchIdentifier);
        local_idx = i;
        collisions_off_for_rest_of_frame = 1;
    }

    return ht;

}


static void drawAuctionInventory( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int color )
{
     char * myCategories[AuctionInvItemStatus_Count] = { "AllString", "StoredString", "SellingString", "SoldString", "BiddingString", "BoughtString" };
    int iInventoryCount[AuctionInvItemStatus_Count] = {0};
    char **ppNames = 0;
    int i, buttonFlags = MMBUTTON_SMALL|MMBUTTON_COLOR;

    Entity * e = playerPtr();
    UIBox box;
    F32 start_y, inf_wd = 0;
    char *currency = NULL;
    UISkin skin;

    static ScrollBar sb = {0};
    
    if(gAuctionDisabled)
        buttonFlags |= (MMBUTTON_LOCKED|MMBUTTON_GRAYEDOUT);

    auctionUpdateInventory();

    // Update list
    for ( i = 0; i < eaSize(&ppLocalInventory); i++ )
    {
        iInventoryCount[ppLocalInventory[i]->auction_status]++;
        if( ppLocalInventory[i]->auction_status == AuctionInvItemStatus_ForSale && ppLocalInventory[i]->amtOther )
            iInventoryCount[AuctionInvItemStatus_Sold]++;
        if (ppLocalInventory[i]->auction_status == AuctionInvItemStatus_Bidding && ppLocalInventory[i]->amtStored)
            iInventoryCount[AuctionInvItemStatus_Bought]++;
    }

      for(i = 0; i < ARRAY_SIZE(myCategories); i++)
    {
        if( i == 0 )
            eaPush( &ppNames, textStd("CategoryCountTotal", myCategories[i],  eaSize(&ppLocalInventory), e->pchar->auctionInvTotalSlots ) );
        else if( iInventoryCount[i] > 0 )
            eaPush( &ppNames, textStd("CategoryCount", myCategories[i], iInventoryCount[i] ) );
        else
            eaPush( &ppNames, myCategories[i] );
     }
      my_category = drawMMFrameTabs( x, y + 30*sc, z+2, sc, wd, ht-30*sc, 20*sc, kStyle_AuctionHouse, &ppNames, my_category );

    font( &game_14 );
       font_color( CLR_AH_TEXT_SELECTED, CLR_AH_TEXT_SELECTED );
    font_outl(0);
    font_ital(1);

    skin = skinFromMapAndEnt( e );
    if( skin == UISKIN_PRAETORIANS )
        currency = textStdEx(TRANSLATE_NOPREPEND, "InfluencePraetoria");
    else if( skin == UISKIN_VILLAINS )
        currency = textStdEx(TRANSLATE_NOPREPEND, "V_InfluenceString");
    else
        currency = textStdEx(TRANSLATE_NOPREPEND, "InfluenceString");

    inf_wd = str_wd(font_grp, sc, sc, "%s:", currency);
    prnt( x + wd - inf_wd- 15*sc, y+15*sc, z, sc, sc, "%s:", currency );

    inf_wd = str_wd(font_grp, sc, sc, textStd("InfluenceWithCommas",e->pchar->iInfluencePoints));
     prnt( x + wd - inf_wd- 15*sc, y+30*sc, z, sc, sc,  textStd("InfluenceWithCommas",e->pchar->iInfluencePoints) );
    font_outl(1);
    font_ital(0);
    z += 5;
    y += 40*sc;

    if( my_category == AuctionInvItemStatus_Sold )
        uiBoxDefine(&box, x, y, wd, ht-70*sc);
    else
        uiBoxDefine(&box, x, y, wd, ht-40*sc);
    clipperPush(&box);
    start_y = y;
    for ( i = 0; i < eaSize(&ppLocalInventory); i++ )    
    {
        // Items that can be sold in stacks and are only partially sold only occupy one inventory slot, but exist in two tabs: for-sale and sold
        if( my_category && (ppLocalInventory[i]->auction_status != my_category) && 
            !(my_category == AuctionInvItemStatus_Sold && ppLocalInventory[i]->auction_status == AuctionInvItemStatus_ForSale && ppLocalInventory[i]->amtOther) &&
             !(my_category == AuctionInvItemStatus_Bought && ppLocalInventory[i]->auction_status == AuctionInvItemStatus_Bidding && ppLocalInventory[i]->amtStored))
            continue;

        y += drawAuctionInventoryItem( ppLocalInventory[i], my_category ,  i, x, y - sb.offset, z, wd, sc, color );

        if( local_idx == i && ppLocalInventory[i]->auction_status == AuctionInvItemStatus_Stored && smfBlock_HasFocus( smfPrice ) ) 
        {
            KeyInput* input = inpGetKeyBuf();
            while( input )
            {
                if( input->type == KIT_EditKey && ( input->key == INP_RETURN || input->key == INP_NUMPADENTER ) )
                {
                    int price = atoi_ignore_nonnumeric(smfPrice->outputString);
                    INT64 total = (INT64)price * ppLocalInventory[i]->amtStored; // do not allow stack prices to exceed 2 billion
                    int fee = calcAuctionSellFee(total);
                    collisions_off_for_rest_of_frame = 1;
                    if( fee > 100000 && !optionGet( kUO_HideFeePrompt ) )
                        dialog( DIALOG_YES_NO, -1, -1, -1, 300, textStd("LargePostingFee", fee), NULL, auctionPostItemAdapter, NULL, NULL,0,sendOptions,hideFeeDCB,1,0,0,ppLocalInventory[i] );
                    else
                    {
                        auction_changeItemStatus(ppLocalInventory[i]->auction_id, price, AuctionInvItemStatus_ForSale);
                        local_idx = -1;
                    }
                    break;
                }
                else if( input->type == KIT_EditKey && input->key == INP_TAB )
                {
                    int dir = 0;
                    if( input->attrib == KIA_NONE )
                        dir = 1;
                    else if( input->attrib == KIA_SHIFT )
                        dir = -1;

                    if( dir == -1 || dir == 1 )
                    {
                        int idx = i + dir;
                        int max = eaSize( &ppLocalInventory );

                        if( dir == -1 && idx < 0 )
                            idx = max - 1;
                        else if( dir == 1 && idx >= max )
                            idx = 0;

                        // Haven't returned to the starting point and haven't
                        // updated which item is selected
                        while( idx != i && idx != local_idx )
                        {
                            if( ppLocalInventory[idx]->auction_status == AuctionInvItemStatus_Stored )
                            {
                                local_idx = idx;
                                inpClear();
                                smf_SetRawText( smfPrice, "", false );
                                smf_SetCursor( smfPrice, 0 );
                            }
                            else
                            {
                                idx += dir;
                                // Wrap around at the bottom and top
                                if( dir == -1 && idx < 0 )
                                    idx = max - 1;
                                else if( dir == 1 && idx >= max )
                                    idx = 0;
                            }
                        }
                    }
                }
                inpGetNextKey(&input);
            }
        }

        if( !my_category )
        {
            if( ppLocalInventory[i]->auction_status == AuctionInvItemStatus_ForSale && ppLocalInventory[i]->amtOther ) 
                y += drawAuctionInventoryItem( ppLocalInventory[i], AuctionInvItemStatus_Sold,  i, x, y - sb.offset, z, wd, sc, color );
            if (ppLocalInventory[i]->auction_status == AuctionInvItemStatus_Bidding && ppLocalInventory[i]->amtStored)
                y += drawAuctionInventoryItem( ppLocalInventory[i], AuctionInvItemStatus_Bought,  i, x, y - sb.offset, z, wd, sc, color );
        }
    }

     if( y == start_y )
    {
        font(&title_14);
        font_color(CLR_AH_TEXT, CLR_AH_TEXT);
        cprnt( x + wd/2, start_y + ht/2, z, sc, sc, "DragItemsHere" );
    }
    clipperPop();


       if( my_category == AuctionInvItemStatus_Sold && drawMMButton( "GetAllInf", 0, 0, x + wd/2, start_y + ht - 55*sc, z, 120*sc, sc*.8, buttonFlags, color )  )
    {
        for( i = eaSize(&ppLocalInventory)-1; i >=0; i-- )
        {
            if (ppLocalInventory[i]->auction_status == AuctionInvItemStatus_Sold || (ppLocalInventory[i]->auction_status == AuctionInvItemStatus_ForSale && ppLocalInventory[i]->amtOther) ) 
                auctionHouseGetInfStored(ppLocalInventory[i]);
        }
    }

    sb.use_color = 0;
     if( my_category == AuctionInvItemStatus_Sold )
        doScrollBar(&sb, ht - 80*sc, y-start_y, x+wd, start_y, z, 0, &box );
    else
           doScrollBar(&sb, ht - 50*sc, y-start_y, x+wd, start_y, z, 0, &box );
}


static void auctionStashTableMultiLevelAddVA( StashTable pTable, AuctionItem *pItem, int depth, va_list va )
{

    while( depth > 0 )
    {
        StashElement elt;
        char* param = va_arg(va, char*);

        stashFindElement( pTable, param, &elt);
        if(!elt)
        {
            MultiLevelStashNode *pNode = calloc(1, sizeof(MultiLevelStashNode));
            if( depth > 1 )
                pNode->stStash = stashTableCreateWithStringKeys( 8, StashDeepCopyKeys );
            else
                eaCreate(&pNode->ppItems);

            stashAddPointerAndGetElement(pTable, param, pNode, false, &elt);
        }

        if(verify( elt ))
        {
            MultiLevelStashNode *pNode = stashElementGetPointer(elt);
            if( depth > 1 )
                pTable = pNode->stStash;
            else
                eaPush(&pNode->ppItems, pItem);
        }        
        depth--;
    }
}

static void auctionStashTableMultiLevelAdd( StashTable pTable, AuctionItem *pItem, int depth, ... )
{
    va_list va;
    va_start( va, depth );
    auctionStashTableMultiLevelAddVA(pTable, pItem, depth, va);
    va_end( va );
}

static void AddAuctionItemToAuctionHouseStashTable(AuctionItem *pItem)
{
    switch( pItem->type )
    {
        xcase kTrayItemType_Salvage:
        {
            SalvageItem *salvage = pItem->salvage;

            auction_addKeyWordsFromString( textStd(salvage->ui.pchDisplayName), pItem, 0 );
            auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 3, "UiInventoryTypeSalvage", salvage->pchDisplayTabName, salvage->ui.pchDisplayName );
        }
        xcase kTrayItemType_Recipe:
        {
            const char* catName = NULL;
            const char* setName = NULL;

            auction_addKeyWordsFromString( textStd(pItem->recipe->ui.pchDisplayName), pItem, 0 );

            if (pItem->recipe->pchEnhancementReward)
            {
                catName = boostSetGroupNameFromBasePowerName(pItem->recipe->pchEnhancementReward);
                setName = boostSetNameFromBasePowerName(pItem->recipe->pchEnhancementReward);
            }
            if( !catName )
                auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 3, "UiInventoryTypeRecipe", "OtherString", pItem->recipe->ui.pchDisplayName );
            else
                auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 4, "UiInventoryTypeRecipe", catName, setName, pItem->recipe->ui.pchDisplayName );
        }
        xcase kTrayItemType_Inspiration:
        {
            BasePower *ppow = pItem->inspiration;

            auction_addKeyWordsFromString( textStd(pItem->displayName), pItem, 0 );
            auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 3, "UiInventoryTypeInspiration", pItem->inspiration->psetParent->pchDisplayName, pItem->inspiration->pchDisplayName );
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            int j,k;
            BasePower *ppow =  pItem->enhancement;
            const BasePowerSet *pset = ppow->psetParent;

            auction_addKeyWordsFromString( textStd(ppow->pchDisplayName), pItem, 0 );

            if (!power_GetReplacementPowerName(ppow->pchName))
            {
                char * catNodeName = "NormalEnhancementsString";
                int special = 0, isCustomOrganized = 0;

                if ( strstriConst(pset->pchName, "hamidon") == pset->pchName ||
                    strstriConst(pset->pchName, "hydra") == pset->pchName ||
                    strstriConst(pset->pchName, "synthetic") == pset->pchName ||
                    strstriConst(pset->pchName, "titan") == pset->pchName ||
                    strstriConst(pset->pchName, "yins") == pset->pchName  )
                {
                    catNodeName = "SpecialEnhancementsString";
                    special = 1;
                }
                else if (baseset_IsCrafted(pset))
                {
                    catNodeName = "CraftedEnhancementsString";
                    isCustomOrganized = 1;
                }
                else if (ppow->bBoostUsePlayerLevel && IsAttunedEnhancementAllowedInAuctionHouse(ppow, pItem->lvl))
                {                    
                    catNodeName = "AttunedEnhancementsString";
                    isCustomOrganized = 1;
                }

                if ( isCustomOrganized )
                {
                    const char *catName = ppow->pBoostSetMember ? ppow->pBoostSetMember->pchGroupName : NULL;
                    const char *setName = ppow->pBoostSetMember ? ppow->pBoostSetMember->pchDisplayName : NULL;

                    if ( catName )
                    {
                        if ( !setName )
                            setName = "OtherString";
                        auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 5, "UiInventoryTypeEnhancement", catNodeName, catName, setName, ppow->pchDisplayName );
                    }
                    else
                    {
                        catName = "OtherString";
                        auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 4, "UiInventoryTypeEnhancement", catNodeName, catName, ppow->pchDisplayName );
                    }
                }
                else
                {
                    char **originNodeNames=0;
                    char **effectNodeNames=0;

                    int iSizeOrigins = eaSize(&g_CharacterOrigins.ppOrigins);
                    int iSize = eaiSize(&(int *)ppow->pBoostsAllowed);


                    for(j = 0; j < iSize; j++)
                    {
                        int num = ppow->pBoostsAllowed[j];

                        // an origin
                        if(num < iSizeOrigins)
                        {
                            if ( !special )
                            {
                                const char *originStr = g_CharacterOrigins.ppOrigins[num]->pchName;

                                if( strstriConst( pset->pchName, "generic") != 0)
                                {
                                    //originNodeName = "Generic";
                                    eaPush(&originNodeNames,"Generic");
                                    break;
                                }
                                else if(stricmp(originStr, "Magic") == 0)
                                {
                                    //originNodeName = "Magic";
                                    eaPush(&originNodeNames,"Magic");
                                }
                                else if( stricmp( originStr, "Science") == 0)
                                {
                                    //originNodeName = "Science";
                                    eaPush(&originNodeNames,"Science");
                                }
                                else if( stricmp( originStr, "Mutation") == 0)
                                {
                                    //originNodeName = "Mutation";
                                    eaPush(&originNodeNames,"Mutation");
                                }
                                else if( stricmp( originStr, "Technology") == 0)
                                {
                                    //originNodeName = "Technology";
                                    eaPush(&originNodeNames,"Technology");
                                }
                                else if( stricmp( originStr, "Natural") == 0)
                                {
                                    //originNodeName = "Natural";
                                    eaPush(&originNodeNames,"Natural");
                                }
                            }
                        }
                        // an effect
                        else
                        {
                            eaPush(&effectNodeNames, strdup(textStd(g_AttribNames.ppBoost[num-iSizeOrigins]->pchDisplayName)));
                        }
                    }

                    if ( eaSize(&originNodeNames) == 0 )
                    {
                        for ( j = 0; j < eaSize(&effectNodeNames); ++j )
                            auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 4, "UiInventoryTypeEnhancement", catNodeName, effectNodeNames[j], ppow->pchDisplayName );
                    }
                    else
                    {
                        for ( j = 0; j < eaSize(&originNodeNames); ++j )
                        {
                            for ( k = 0; k < eaSize(&effectNodeNames); ++k )
                                auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 5, "UiInventoryTypeEnhancement", catNodeName, originNodeNames[j], effectNodeNames[k], ppow->pchDisplayName );
                        }
                    }

                    eaDestroyEx(&effectNodeNames, NULL);
                    eaDestroy(&originNodeNames);
                }
            }
        }
        xcase kTrayItemType_Power:
        {
            auction_addKeyWordsFromString(textStd(pItem->displayName), pItem, 0 );
            auctionStashTableMultiLevelAdd( g_stAuctionHouse, pItem, 2, "FixedPriceString", pItem->temp_power->pchDisplayName );
        }

    }
}

void auction_Init(void)
{
    int i;

    auctiondata_Init();

    g_stAuctionHouse = stashTableCreateWithStringKeys( 8, StashDefault );
    
    // the first dictionary pass just puts everything in one big list and stash table
    // this pass will make keyword dictionary and set up stash table hierarchy for the browser tree
    for( i = 0; i < eaSize(&g_AuctionItemDict.ppItems); i++ )
    {
        AuctionItem *pItem = g_AuctionItemDict.ppItems[i];

        if (IsAuctionItemValid(pItem))
        {
            AddAuctionItemToAuctionHouseStashTable(pItem);
        }
    }
}

static void amountSliderDrawCallback(void *userData, float x, float y, float z, float sc, int clr)
{
    AuctionItem *itm = (AuctionItem*)userData;
    Entity *e = playerPtr();

    if ( itm )
    {
        switch( itm->type )
        {
        case kTrayItemType_Recipe:
            if ( itm->recipe )
                uiRecipeDrawIcon(itm->recipe, itm->recipe->level, x-8*sc, y-5*sc, z, sc, 0, true, CLR_WHITE);

        xcase kTrayItemType_Salvage:
            if ( e && e->pchar && e->pchar->salvageInv && e->pchar->salvageInv[itm->auction_id]->salvage )
            {
                AtlasTex *icon = atlasLoadTexture(e->pchar->salvageInv[itm->auction_id]->salvage->ui.pchIcon);
                if ( icon )
                    display_sprite(icon, x, y, z, sc, sc, CLR_WHITE);
            }
        xdefault:
            Errorf("Unsupported type in amountSliderDrawCallback: %s", itm->pchIdentifier);
        }
    }
}

static void auctionAmountSliderCallback(int amount, void *userData)
{
    AuctionItem *itm = (AuctionItem*)userData;
    if ( itm )
    {
        auction_addItem(itm->pchIdentifier, itm->auction_id, amount, itm->amtStored, itm->auction_status, itm->id);
        addItemToLocalInventory( itm->pchIdentifier, itm->auction_id, itm->auction_status, 0, amount, 0, 0, itm->infPrice, false, 1 );
    }
}

static DialogCheckbox deleteDCB[] = { { "HideUsefulSalvagePrompt", 0, kUO_HideUsefulSalvagePrompt} };


static void auction_SalvageSell( const SalvageItem * pSalvage )
{
    static AuctionItem addInfo;
    Entity * e = playerPtr();
    int stackSize = MIN(MAX_AUCTION_STACK_SIZE,character_SalvageCount(e->pchar, pSalvage));
    SalvageInventoryItem *pInv = character_FindSalvageInv(e->pchar, pSalvage->pchName);

    addInfo.type = kTrayItemType_Salvage;
    addInfo.pchIdentifier = auction_identifier(kTrayItemType_Salvage, pSalvage->pchName, 0);
    addInfo.lvl = 0;
    addInfo.auction_id = character_FindSalvageInvIdx(e->pchar, pInv);
    addInfo.id = auctionData_GetID(kTrayItemType_Salvage, pSalvage->pchName, 0);
    addInfo.infPrice = 0;
    addInfo.auction_status = AuctionInvItemStatus_Stored;
    amountSliderWindowSetupAndDisplay(1, stackSize, NULL, amountSliderDrawCallback, auctionAmountSliderCallback, &addInfo, 0);
}

static void auction_SalvageSellAdapter(void * arg0)
{
    auction_SalvageSell((const SalvageItem *)arg0);
}

static void auctionDragRecieve(CBox *box)
{
    Entity * e = playerPtr();

    if ( cursor.dragging  )
    {
        static AuctionItem addInfo;

        if( !isDown( MS_LEFT ) && mouseCollision(box) )
        {
            if (!AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch")))
            {
                addSystemChatMsg( textStd("AuctionLicenseRequired"), INFO_USER_ERROR, 0 );
            } else {

                switch(cursor.drag_obj.type)
                {
                    xcase kTrayItemType_Recipe:
                    {
                        const DetailRecipe *recipe = detailrecipedict_RecipeFromId(cursor.drag_obj.invIdx);
                        if ( recipe )
                        {
                            if (recipe->flags & RECIPE_NO_TRADE)
                                addSystemChatMsg( textStd("CannotTradeRecipe"), INFO_USER_ERROR, 0 );
                            else
                            {
                                int stackSize = MIN(MAX_AUCTION_STACK_SIZE,character_RecipeCount(e->pchar, recipe));
                                addInfo.type = kTrayItemType_Recipe;
                                addInfo.pchIdentifier = auction_identifier(kTrayItemType_Recipe, recipe->pchName, recipe->level);
                                addInfo.lvl = recipe->level;
                                addInfo.auction_id = cursor.drag_obj.invIdx;
                                addInfo.id = auctionData_GetID(kTrayItemType_Recipe, recipe->pchName, recipe->level);
                                addInfo.infPrice = 0;
                                addInfo.auction_status = AuctionInvItemStatus_Stored;
                                addInfo.recipe = recipe;
                                amountSliderWindowSetupAndDisplay(1, stackSize, NULL, amountSliderDrawCallback, auctionAmountSliderCallback, &addInfo, 0);
                            }
                        }
                    }
                    xcase kTrayItemType_Salvage:
                    {
                        if ( e->pchar->salvageInv[cursor.drag_obj.invIdx] )
                        {
                            const SalvageItem *salvage = (SalvageItem *)e->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage;
                            if ( salvage )
                            {
                                DetailRecipe **ppRecipes=0;

                                if (salvage->flags & SALVAGE_NOAUCTION)
                                {
                                    addSystemChatMsg( textStd("CannotTradeSalvage"), INFO_USER_ERROR, 0 );
                                }
                                else if(character_IsSalvageUseful(e->pchar,salvage,&ppRecipes) && !optionGet(kUO_HideUsefulSalvagePrompt) )
                                {
                                    int i;
                                    char * sal_string=0;
                                    estrConcatCharString(&sal_string, textStd("SellUsefulSalvage",textStd(salvage->ui.pchDisplayName)));
                                    estrConcatf(&sal_string, "<br><br>%s<br>", textStd("SalvageDropUseful") );
                                    for( i = 0; i < eaSize(&ppRecipes); i++ )
                                        estrConcatf(&sal_string, "%s<br>", textStd(ppRecipes[i]->ui.pchDisplayName) );

                                    dialogRemove(0, auction_SalvageSellAdapter,0);
                                    dialog( DIALOG_YES_NO, -1, -1, -1, -1, sal_string, NULL, auction_SalvageSellAdapter, NULL, NULL,0,sendOptions,deleteUsefulDCB,1,0,0,(void*)salvage);
                                    estrDestroy(&sal_string);
                                    eaDestroy(&ppRecipes);
                                }
                                else
                                    auction_SalvageSell( salvage );
                            }
                        }
                    }
                    xcase kTrayItemType_SpecializationInventory:
                    {
                        if ( e->pchar->aBoosts[cursor.drag_obj.ispec] )
                        {
                            const BasePower *ppow = e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase;
                            int level = e->pchar->aBoosts[cursor.drag_obj.ispec]->iLevel;

                            if (!detailrecipedict_IsBoostTradeable(ppow, level, e->auth_name, ""))
                            {
                                addSystemChatMsg( textStd("CannotTradeEnhancement"), INFO_USER_ERROR, 0 );
                            } 
                            else if (e->pchar->aBoosts[cursor.drag_obj.ispec]->iNumCombines > 0) 
                            {
                                addSystemChatMsg( textStd("CantAuctionCombined"), INFO_USER_ERROR, 0 );
                            } 
                            else 
                            {
                                char *name = basepower_ToPath(ppow);
                                char * pchIdent = auction_identifier( kTrayItemType_SpecializationInventory, name,    level);
                                int id = auctionData_GetID(kTrayItemType_SpecializationInventory, name, level);

                                auction_addItem(pchIdent, cursor.drag_obj.ispec, 1, 0, AuctionInvItemStatus_Stored, id);
                                addItemToLocalInventory( pchIdent, cursor.drag_obj.ispec, AuctionInvItemStatus_Stored, 0, 1, 0, 0, 0, false, 1 );
                            }
                        }
                    }
                    xcase kTrayItemType_Inspiration:
                    {
                        const BasePower *ppow = e->pchar->aInspirations[cursor.drag_obj.iset][cursor.drag_obj.ipow];
                        if (ppow)
                        {
                            if (!inspiration_IsTradeable(ppow, e->auth_name, ""))
                            {
                                addSystemChatMsg( textStd("CannotTradeInspiration"), INFO_USER_ERROR, 0 );
                            } 
                            else
                            {
                                char *name = basepower_ToPath(e->pchar->aInspirations[cursor.drag_obj.iset][cursor.drag_obj.ipow]);

                                char * pchIdent = auction_identifier( kTrayItemType_Inspiration, name, 0);
                                int id = auctionData_GetID(kTrayItemType_Inspiration, name, 0);

                                auction_addItem(pchIdent, (cursor.drag_obj.iset << 16 | (cursor.drag_obj.ipow & 0x0000FFFF)), 1, 0, AuctionInvItemStatus_Stored, id);
                                addItemToLocalInventory( pchIdent, (cursor.drag_obj.iset << 16 | (cursor.drag_obj.ipow & 0x0000FFFF)), AuctionInvItemStatus_Stored, 0, 1, 0, 0, 0, false, 1 );
                            }
                        }
                    }
                }
            }
            trayobj_stopDragging();
        }
    }
}

enum
{
    MIDDLE_BAR_CLICKED,
    MIDDLE_BAR_EXPANDING,
    MIDDLE_BAR_OPEN,
    MIDDLE_BAR_CLOSING,
    MIDDLE_BAR_CLOSED,
};

#define MIDDLE_BAR_SPEED    (1.f)
#define MIDDLE_BAR_MIN        (R10+PIX3)*2

static int auction_middleBarState( float x, float y, float z, float * middleBarHt, float top, float max_ht, float cx, float wd, float scale, int color, int id, float min, float max )
{
    CBox box;
    AtlasTex * mid   = atlasLoadTexture( "Chat_separator_handle.tga" );
    AtlasTex * down  = atlasLoadTexture( "Chat_separator_arrow_down.tga" );
    AtlasTex * up    = atlasLoadTexture( "Chat_separator_arrow_up.tga" );

    static int state=MIDDLE_BAR_CLOSED;
    static int win = 0;
    static float clickLoc;

    // Per CW, this is a cut and paste job from the chat window code.  In that instance, id can take multiple values - here it is always 0.  Since the sc
    // array is only ever indexed by id, and id is always zero, we can trim the size of that array.
    static float sc[1] = {0.0f};

    // However, just in case the caller of this ever changes ...
    // If this trips, you changed the id parameter to this, in which case you need to change the size of the sc array.
    assert(id == 0);

    if(*middleBarHt == 0)
        return 0;

    y -= mid->height/2;

      BuildCBox( &box, cx + R10*scale, y - mid->width*scale, wd - R10*2*scale, mid->width*2*scale );

    if( !win || win == id )
    {
        if ( state != MIDDLE_BAR_CLICKED && (*middleBarHt >= min && *middleBarHt <= max ) )
        {
            if( mouseCollision(&box ) )
            {
                state = MIDDLE_BAR_EXPANDING;
                setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
            }
            else
                state = MIDDLE_BAR_CLOSING;
        }

          if( !isDown( MS_LEFT ) && state == MIDDLE_BAR_CLICKED )
        {
            state = MIDDLE_BAR_OPEN;

            if( *middleBarHt >= min && *middleBarHt <= max )
            {
                if ( (*middleBarHt*max_ht) < MIDDLE_BAR_MIN )
                    *middleBarHt = ((float)MIDDLE_BAR_MIN/max_ht);
                if ( (*middleBarHt*max_ht) > max_ht-MIDDLE_BAR_MIN )
                    *middleBarHt = (max_ht-MIDDLE_BAR_MIN)/max_ht;

                if( *middleBarHt < min )
                    *middleBarHt = min;
                if( *middleBarHt > max )
                    *middleBarHt = max;
            }
            win = 0;
        }
        else if ( mouseDownHit( &box, MS_LEFT ) && (*middleBarHt >= min && *middleBarHt <= max ) )
        {
            state = MIDDLE_BAR_CLICKED;
            win = id;
            clickLoc = gMouseInpCur.y - max_ht + (*middleBarHt*max_ht);
        }

        if ( state == MIDDLE_BAR_EXPANDING )
        {
            sc[id] += TIMESTEP*MIDDLE_BAR_SPEED;
            if( sc[id] > 1.0f )
            {
                sc[id] = 1.0f;
                state = MIDDLE_BAR_OPEN;
            }
        }
        if( state == MIDDLE_BAR_CLOSING )
        {
            sc[id] -= TIMESTEP*MIDDLE_BAR_SPEED;
            if( sc[id] < 0.0f )
            {
                sc[id] = 0.0f;
                state = MIDDLE_BAR_CLOSED;
            }
        }

        if( state == MIDDLE_BAR_CLICKED )
        {
            sc[id] = 1.f;
            *middleBarHt = (clickLoc+max_ht-gMouseInpCur.y)/max_ht;
            setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
        }
    }

    // bounds check
    if (window_getMode(WDW_AUCTION) == WINDOW_DISPLAYING)
    {
         if ( (*middleBarHt*max_ht) < MIDDLE_BAR_MIN )
            *middleBarHt = ((float)MIDDLE_BAR_MIN/max_ht);
        if ( (*middleBarHt*max_ht) > max_ht-MIDDLE_BAR_MIN )
            *middleBarHt = (max_ht-MIDDLE_BAR_MIN)/max_ht;

        if( *middleBarHt < min )
            *middleBarHt = min;
        if( *middleBarHt > max )
            *middleBarHt = max;
    }

    y = top+max_ht - *middleBarHt*max_ht - mid->height*scale/2;

      if( *middleBarHt >= min && *middleBarHt <= max )
    {
        if( state != MIDDLE_BAR_CLOSED  && (!win || win == id) )
        {
            display_sprite( up, x - sc[id]*up->width*scale/2, y - (mid->height/2 + sc[id]*up->height + 5 - PIX3)*scale, z+2, sc[id]*scale, sc[id]*scale, color );
            display_sprite( down, x - sc[id]*down->width*scale/2, y + (mid->height/2 + 5 + PIX3)*scale, z+2, sc[id]*scale, sc[id]*scale, color );
        }

        display_sprite( mid, x - mid->width*scale/2, y - (mid->height/2 - PIX3)*scale, z+5, scale, scale, color );
    }

    return state == MIDDLE_BAR_CLICKED;
}

static F32 auction_divider = .5;

int auctionWindow()
{
    F32 x, y, z, wd, ht, sc, search_ht, nav_wd;
    F32 inventory_ht; 
    CBox box;
    AtlasTex *back = atlasLoadTexture("white");
    int color, bcolor;
    int dividerGrabbed;
    Entity * e = playerPtr();

    clearToolTip(&recipeTip);
    clearToolTip(&powerTip);
      if(!window_getDims(WDW_AUCTION, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor)) 
    {
        clearAuctionFields();
        return 0;
    }

    // See if we don't have access to the auction house
    if (!AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch")))
    {
        // Make sure we only see the messages once while closing
        if (window_getMode(WDW_AUCTION) != WINDOW_SHRINKING)
        {
            // Shows up in chat
            addSystemChatMsg( textStd("AuctionLicenseRequired"), INFO_USER_ERROR, 0 );

            // Puts up a dialog
            dialogStd( DIALOG_OK, "AuctionLicenseRequired", NULL, NULL, NULL, NULL, 0 );

            // Closes the window so we don't continue to see the messages
            window_setMode(WDW_AUCTION, WINDOW_SHRINKING);
        }

        return 0;
    }

    uiAuctionUpdateBannedStatus();

    bcolor |= 0xffffff00;
       drawWaterMarkFrame_ex( x, y, z, sc, wd, ht,  0x111111ff&bcolor, 0x333333ff&bcolor, color, 0 );

    BuildCBox(&box, x, y, wd, ht);
    auctionDragRecieve(&box);

     // Search Pane
     search_ht = drawAuctionSearch( x, y, z+1, wd, sc, color );
       drawFlatFrame(PIX3, R10, x + PIX3, y + PIX3, z, wd -2*PIX3, search_ht-4*PIX3, sc, 0xffffff33, 0xffffff33 );

    // Middle divider bar
       inventory_ht = (ht - search_ht)*auction_divider;
       dividerGrabbed = auction_middleBarState( x + wd/2, y + ht - inventory_ht, z+5, &auction_divider, y + search_ht, ht-search_ht, x, wd, sc, color, 0, .2, .8 );
     if (dividerGrabbed && !gWindowDragging)
        gWindowDragging = true;
    inventory_ht = (ht - search_ht)*auction_divider;

    // Nav Tree Pane - Left Side
       nav_wd = drawAuctionNavTree( x, y + search_ht+20*sc, z+1, wd -  445*sc, ht - search_ht - inventory_ht - 23*sc, sc );

    // Background behind tree and results
       display_sprite_blend( back, x+ PIX3*sc, y + search_ht+20*sc, z, (nav_wd)/back->width, (ht - search_ht - inventory_ht - 23*sc)/back->height, 0x33333388&bcolor, 0x444444ff&bcolor, 0x444444ff&bcolor, 0x33333388&bcolor );
       display_sprite_blend( back, x + nav_wd + PIX3*sc, y + search_ht+20*sc, z, (wd-2*PIX3-nav_wd)/back->width, (ht - search_ht - inventory_ht - 23*sc)/back->height, 0x444444ff&bcolor, 0x444444ff&bcolor, 0x444444ff&bcolor, 0x444444ff&bcolor );
       drawHorizontalLine( x + PIX3, y + search_ht+21*sc + ht - search_ht - inventory_ht - 23*sc, wd - 2*PIX3*sc, z, sc, color&0xffffff44);
    drawHorizontalLine( x + PIX3, y + search_ht+18*sc, wd - 2*PIX3*sc, z, sc, color&0xffffff44);

    // Results Pane
       drawAuctionResultsPane( x + wd -  445*sc, y +search_ht+20*sc, z, 445*sc, ht - search_ht - inventory_ht - 23*sc, sc, color );

    //Inventory Pane - Bottom Aligned
    drawAuctionInventory( x, y + ht - inventory_ht, z, wd, inventory_ht, sc, color );

    return 0;
}
