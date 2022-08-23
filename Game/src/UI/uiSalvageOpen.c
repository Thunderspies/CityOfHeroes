

#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "UI/uiInput.h"

#include "formatter/smf_main.h"

#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiCursor.h"
#include "UI/uiOptions.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiChat.h"
#include <utilitieslib/components/EString.h>
#include "uiSalvage.h"
#include "entity/salvage.h"

//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------

static TextAttribs s_taSalvage =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfVerySmall,
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

//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------

typedef struct SuperPackCard
{
    char *productCode;
    char *rarity;
    char *cardTitle;
    char *cardCategory;
    int cardDisplayCount;
    char *cardDescription;
    char *cardBorder;
    char *cardIcon;
    int cardNumber;
    int count;
} SuperPackCard;

typedef struct SuperPackSet 
{
    SuperPackCard **cards;
    char *setName;
    char *setDisplayName;
    char *setIcon;
    char *setBack; 
    char *setSalvage;
} SuperPackSet;

typedef struct SuperPackDescriptions
{
    SuperPackSet **sets;
} SuperPackDescriptions;

static ParseTable parse_SuperPackCard[] =  
{
    { "Number",                    TOK_INT(SuperPackCard, cardNumber, 0)                                        }, 
    { "Rarity",                    TOK_STRING(SuperPackCard, rarity,0)                                            },
    { "Count",                    TOK_INT(SuperPackCard, count, 0)                                            },
    { "DisplayCount",            TOK_INT(SuperPackCard, cardDisplayCount, 0)                                    },
    { "Product",                TOK_STRING(SuperPackCard, productCode,0)                                    },
    { "Title",                    TOK_STRING(SuperPackCard, cardTitle,0)                                        },
    { "Category",                TOK_STRING(SuperPackCard, cardCategory,0)                                    },
    { "Description",            TOK_STRING(SuperPackCard, cardDescription,0)                                },
    { "Border",                    TOK_STRING(SuperPackCard, cardBorder,0)                                        },
    { "Icon",                    TOK_STRING(SuperPackCard, cardIcon,0)                                        },
    { "End",                    TOK_END                                                                        },
    { "",                        0, 0 }
};

static ParseTable parse_SuperPackSet[] =  
{
    { "Name",                    TOK_STRING(SuperPackSet, setName,0)                                            },
    { "DisplayName",            TOK_STRING(SuperPackSet, setDisplayName,0)                                    },
    { "Icon",                    TOK_STRING(SuperPackSet, setIcon,0)                                            },
    { "CardBack",                TOK_STRING(SuperPackSet, setBack,0)                                            },
    { "Salvage",                TOK_STRING(SuperPackSet, setSalvage,0)                                        },
    { "Card",                    TOK_STRUCT(SuperPackSet, cards, parse_SuperPackCard)                        },
    { "End",                    TOK_END                                                                        },
    { "",                        0, 0 }
};

static ParseTable parse_SuperPacks[] =  
{
    { "Set",                    TOK_STRUCT(SuperPackDescriptions, sets, parse_SuperPackSet)                    },
    { "End",                    TOK_END                                                                        },
    { "",                        0, 0 }
};

static SuperPackDescriptions g_superPackDescriptions;
static StashTable g_cardIndex;


int superPackDescriptionsLoad(void)
{
    int retval;
    int i, j;

    StructInit(&g_superPackDescriptions, sizeof(SuperPackDescriptions), parse_SuperPacks);
    retval = ParserLoadFiles(NULL, "defs/Cards/Cards.def", "Cards.bin", 0, parse_SuperPacks, &g_superPackDescriptions, NULL, NULL, NULL);

    // set up lookup cache
    if (g_superPackDescriptions.sets != NULL)
    {
        g_cardIndex = stashTableCreateWithStringKeys(256, StashDefault);
        for (i = eaSize(&g_superPackDescriptions.sets) - 1; i >= 0; i--)
        {
            SuperPackSet *set = g_superPackDescriptions.sets[i];

            if (set->cards != NULL)
            {
                for (j = eaSize(&set->cards) - 1; j >= 0; j--)
                {
                    SuperPackCard *card = set->cards[j];
                    char storeName[256];

                    sprintf_s(storeName, 256, "%s %s %d %s", set->setName, card->productCode, card->count, card->rarity);
                    stashAddPointer(g_cardIndex, strdup(storeName), card, false);
                }
            }
        }
    }
    
    return retval;
}


//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------

// 

#define NUM_CARDS 5
#define CARD_SC 1.375f
#define CARD_FLIP_SPEED 0.1f
#define CARD_FLIP_TIME 2.f
#define CARD_TEXT_FADE_TIME 1.f
#define CARD_COMPLETE_TIME (CARD_FLIP_TIME + CARD_TEXT_FADE_TIME)

typedef struct CardUIData
{
    SuperPackCard *card;
    int messageSent;
    F32 spinAngle;
} CardUIData;

static SuperPackSet *s_cardSet;
static CardUIData s_cardList[NUM_CARDS];
static int s_cardNum;
static int s_newPackJustOpened;

//print to rewards chat
static void printReceivedCard(const SuperPackCard * card)
{
    char * rewardStr;
    estrCreate(&rewardStr);
    estrPrintf(&rewardStr, "%s: %s, ", textStd("ReceivedCardString"), textStd(card->cardTitle));
    if (stricmp(card->rarity, "veryrare") == 0)
    {
        estrConcatf(&rewardStr, "<font color='%s'>", g_salvageColor[kSalvageRarity_VeryRare]);
    }
    else if (stricmp(card->rarity, "rare") == 0)
    {
        estrConcatf(&rewardStr, "<font color='%s'>", g_salvageColor[kSalvageRarity_Rare]);
    }
    else if (stricmp(card->rarity, "uncommon") == 0)
    {
        estrConcatf(&rewardStr, "<font color='%s'>", g_salvageColor[kSalvageRarity_Uncommon]);
    }
    else //common
    {
        estrConcatf(&rewardStr, "<font color='%s'>", g_salvageColor[kSalvageRarity_Common]);
    }

    estrConcatf(&rewardStr, "%s</font>, x%d", textStd(card->rarity), card->cardDisplayCount);
    addSystemChatMsg(rewardStr, INFO_REWARD, 0);
    estrDestroy(&rewardStr);
}

void setSalvageOpenResultsPackage(char *package)
{
    int i;
    SuperPackSet *set = NULL;

    if (g_superPackDescriptions.sets == NULL)
        return;

    // find set
    for (i = eaSize(&g_superPackDescriptions.sets) - 1; i >= 0 && set == NULL; i-- )
    {
        if (stricmp(package, g_superPackDescriptions.sets[i]->setSalvage) == 0)
            set = g_superPackDescriptions.sets[i];
    }

    if (!set)
        return;

    s_cardSet = set;
    s_cardNum = 0;
    s_newPackJustOpened = 1;
    for (i = 0; i < NUM_CARDS; ++i)
    {
        //if not all the cards were opened and new pack is being opened, list the unopened cards
        if (!s_cardList[i].messageSent && s_cardList[i].card)
        {
            printReceivedCard(s_cardList[i].card);
        }
        s_cardList[i].messageSent = 1;
        s_cardList[i].spinAngle = 0.f;
    }
}

void setSalvageOpenResultsDesc(char *product, int quantity, const char *rarity)
{
    SuperPackCard *card;
    char findName[256];

    sprintf_s(findName, 256, "%s %.8s %d %s", s_cardSet->setName, product, quantity, rarity);
    if (stashFindPointer(g_cardIndex, findName, &card))
    {
        if (s_cardNum < NUM_CARDS)
        {
            s_cardList[s_cardNum].card = card;
            s_cardList[s_cardNum].messageSent = 0;
            ++s_cardNum;
        }
    }
}

static void salvageOpenWindowDrawCard(SuperPackCard *pCard, F32 cx, F32 cy, F32 z, F32 sc, F32 spinAngle)
{ 
    AtlasTex *border;
    AtlasTex *icon = atlasLoadTexture(pCard->cardIcon);
    AtlasTex *setIcon = atlasLoadTexture(s_cardSet->setIcon);
    AtlasTex *text = atlasLoadTexture("SuperPackTextBox.tga");
    static SMFBlock * smfTitle, * smfDesc;
    F32 xsc, ysc, textXSc;

    if (!smfDesc)
    {
        smfDesc = smfBlock_Create();
        smf_SetFlags(smfDesc, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);
        smfTitle = smfBlock_Create();
        smf_SetFlags(smfTitle, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);
    }

    xsc = ysc = sc*CARD_SC;
    //spin
    if (spinAngle < CARD_FLIP_TIME/2.f)
    {
        textXSc = (CARD_FLIP_TIME/2.f-spinAngle);
        xsc *= textXSc;
        border = atlasLoadTexture(s_cardSet->setBack);
        display_sprite_positional(border, cx, cy, z, xsc, ysc, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    else 
    {
        textXSc = MIN(spinAngle-CARD_FLIP_TIME/2.f, CARD_FLIP_TIME/2.f);
        xsc *= textXSc;
        border = atlasLoadTexture(pCard->cardBorder);
        display_sprite_positional(border, cx, cy, z, xsc, ysc, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );
        display_sprite_positional(icon, cx, cy - 45.f*sc, z + 1, xsc, ysc, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );
        display_sprite_positional(setIcon, cx + 49.f*xsc, cy + 6.5f*ysc, z + 2, xsc, ysc, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );
        display_sprite_positional(text, cx , cy + 47.5f*ysc, z + 1, xsc, ysc, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );

        if (pCard->cardDisplayCount > 1)
        {
            font( &game_18 );
            font_color(CLR_WHITE, CLR_WHITE); 
            cprnt( cx + 37.f*xsc, cy - 12.f*ysc, z+3, 0.75f*textXSc*sc, 0.75f*sc, "x%d", pCard->cardDisplayCount);
        }

        if (spinAngle >= CARD_FLIP_TIME)
        {
            F32 alpha = (spinAngle-CARD_FLIP_TIME)/CARD_TEXT_FADE_TIME;
            s_taSalvage.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
            s_taSalvage.piColor = (int *)(CLR_BLACK & (0xffffff00 | (int)(0xff*alpha)));
            s_taSalvage.piOutline = (int *)0;
            s_taSalvage.piFace = (int *)&game_9;

            smf_SetScissorsBox(smfDesc, cx - 57.f*xsc, cy + 17.f*ysc, 110*xsc, 110*ysc);
            smf_SetRawText(smfDesc, textStd(pCard->cardDescription), false);
            smf_Display(smfDesc, cx - 57.f*xsc, cy + 17.f*ysc, z+3, 110*ysc, 0, false, false, &s_taSalvage, 0);
            font( &game_12 );

            s_taSalvage.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
            s_taSalvage.piColor = (int *)(CLR_WHITE & (0xffffff00 | (int)(0xff*alpha)));
            s_taSalvage.piOutline = (int *)1;
            s_taSalvage.piFace = (int *)&game_10;

            smf_SetScissorsBox(smfTitle, cx - 58.f*xsc, cy - 90.f*ysc, 110*xsc, 21*ysc);
            smf_SetRawText(smfTitle, textStd(pCard->cardTitle), false);
            smf_Display(smfTitle, cx - 58.f*xsc,  cy - 90.f*ysc, z+2, 110*ysc, 0, false, false, &s_taSalvage, 0);

            font( &game_10 );
        }
        else
        {
            font( &game_10_dynamic ); //need the scaling font while spinning
        }
        font_color(CLR_WHITE, CLR_WHITE);
        prnt( cx - 58.f*xsc, cy + 13.f*ysc, z+2, textXSc*sc, sc, "%s", textStd(pCard->cardCategory));
        cprnt( cx + 45.f*xsc, cy + 92.f*ysc, z+2, 0.75*textXSc*sc, 0.75f*sc, "%d/%d", pCard->cardNumber, eaSize(&s_cardSet->cards));
    }
}



//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------
#define BUTTON_HT 70
#define CHOICE_SPACE 9.5f

int salvageOpenWindow(void)
{
    F32 x, y, startx, z, wd, ht, sc;
    int i, autoflip;
    int color, bcolor;
    static bool init;
    static HybridElement sButtonClose = {0, NULL, NULL, "icon_lt_closewindow_0"};
    F32 spin = TIMESTEP * CARD_FLIP_SPEED;

    if(!window_getDims(WDW_SALVAGE_OPEN, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) )
    {
        init = false;
        if (!s_newPackJustOpened)  //don't open the cards before the window has opened
        {
            int i;
            for (i = 0; i < NUM_CARDS; ++i)
            {
                //if not all the cards were opened and the window is being closed, list the unopened cards
                if (!s_cardList[i].messageSent && s_cardList[i].card)
                {
                    printReceivedCard(s_cardList[i].card);
                    s_cardList[i].messageSent = 1;
                }
            }
        }
        return 0;
    }
    s_newPackJustOpened = 0;

    //draw frame
    drawHybridPopupFrame(x, y, z, wd, ht);

    if(!init)
    {
        init = true;
    }

    set_scissor(true);
    scissor_dims(x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht-2*PIX3*sc);

    //draw autoflip option
    {
        AtlasTex * mark;
        CBox box;

        mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( mark, x+wd/2.f+180.f*sc, y+ht-40.f*sc, z, sc, sc, (color & 0xffffff00)|0xff  );

        if(optionGet(kUO_AutoFlipSuperPackCards))
        {
            mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
            display_sprite( mark, x+wd/2.f+180.f*sc, y+ht-40.f*sc, z, sc, sc, (color & 0xffffff00)|0xff  );
        }

        font(&hybridbold_12);
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(x+wd/2.f+200.f*sc, y+ht-33.f*sc, z, sc, sc, CENTER_Y, "AutoFlipSuperPackCards" );

        BuildCBox( &box, x+wd/2.f+175.f*sc, y+ht-45.f*sc, 300.f*sc, 25.f*sc );
        //drawBox(&box, 13000.f, CLR_RED, 0);
        if( mouseDownHit( &box, MS_LEFT ) )
        {
            optionToggle(kUO_AutoFlipSuperPackCards, 0 );
        }
    }

    devassert(s_cardNum <= NUM_CARDS);
    autoflip = optionGet(kUO_AutoFlipSuperPackCards);
    if (autoflip)
    {
        for (i = 0; i < s_cardNum; i++)
        {
            s_cardList[i].spinAngle += spin;
            if (s_cardList[i].spinAngle > CARD_COMPLETE_TIME)
            {
                // completely flipped card
                spin = s_cardList[i].spinAngle - CARD_COMPLETE_TIME;
                s_cardList[i].spinAngle = CARD_COMPLETE_TIME;
                if (!s_cardList[i].messageSent)
                {
                    printReceivedCard(s_cardList[i].card);
                    s_cardList[i].messageSent = 1;
                }
            }
            else
            {
                spin = 0;
            }
        }                
    }
    else
    {
        for (i = 0; i < s_cardNum; ++i)
        {
            if (s_cardList[i].spinAngle > 0)
            {
                s_cardList[i].spinAngle += spin;
                if (s_cardList[i].spinAngle > CARD_COMPLETE_TIME)
                {
                    s_cardList[i].spinAngle = CARD_COMPLETE_TIME;
                    if (!s_cardList[i].messageSent)
                    {
                        printReceivedCard(s_cardList[i].card);
                        s_cardList[i].messageSent = 1;
                    }
                }
            }
        }

        font(&hybridbold_12);
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(x+wd/2.f, y+HBPOPUPFRAME_TOP_OFFSET+20.f*sc, z, sc, sc, CENTER_X | CENTER_Y, "ClickToFlipString" );
    }

    // draw the cards!
    startx = x+(4*CHOICE_SPACE*sc);
    for (i = 0; i < s_cardNum; ++i)
    {
        if (!autoflip && s_cardList[i].spinAngle == 0.f)
        {
            CBox box;
            BuildCBox(&box, startx, y+(HBPOPUPFRAME_TOP_OFFSET+ht-10.f)/2.f - 95.f*sc*CARD_SC, 126.f*sc*CARD_SC, 190.f*sc*CARD_SC);
            //drawBox(&box, 13000.f, CLR_RED, 0);
            if (mouseCollision(&box))
            {
                setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2 );
                if (mouseClickHitNoCol(MS_LEFT))
                {
                    s_cardList[i].spinAngle += 0.001f;
                }
            }
        }
        salvageOpenWindowDrawCard(s_cardList[i].card, startx+63.f*sc*CARD_SC, y+(HBPOPUPFRAME_TOP_OFFSET+ht-10.f)/2.f, z+15, sc, s_cardList[i].spinAngle);
        startx += (126 + CHOICE_SPACE)*sc*CARD_SC;
    }

    set_scissor(false);

    //draw label
    font(&title_18);
    font_outl(0);
    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
    cprnt(x+wd/2.f+2.f, y+23.f+2.f, z+3.f, 1.f, 1.f, s_cardSet->setDisplayName);
    font_color(CLR_WHITE, CLR_WHITE);
    cprnt(x+wd/2.f, y+23.f, z+3.f, 1.f, 1.f, s_cardSet->setDisplayName);
    font_outl(1);

    //draw close button
    if (D_MOUSEHIT == drawHybridButton(&sButtonClose, x + wd - 30.f, y + 21.f, z+20.f, 0.75f, CLR_WHITE, HB_DRAW_BACKING))
    {
        window_setMode(WDW_SALVAGE_OPEN, WINDOW_SHRINKING);
    }

    return 0;
}
