
#include "gameComm/wdwbase.h"

#include <utilitieslib/language/AppLocale.h>

#include "uiHelp.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiEditText.h"
#include "UI/uiScrollBar.h"
#include "UI/uiBox.h"
#include "UI/uiClipper.h"
#include "UI/uiComboBox.h"
#include "UI/uiDialog.h"
#include <utilitieslib/utils/mathutil.h>

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/entity.h"
#include "cmdparse/cmdgame.h"

#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>

#include "uiSMFView.h"
#include "formatter/smf_main.h"
#include <utilitieslib/language/MessageStoreUtil.h>

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

SMFView *gHelpView;
ComboBox comboHelperStatus;

// Data structures
//---------------------------------------------------------------------

typedef enum HelpType
{
    kHelpType_All,
    kHelpType_HeroOnly,
    kHelpType_VillainOnly,
    kHelpType_PraetorianOnly,
}HelpType;


StaticDefineInt HelpTypeEnum[] =
{
    DEFINE_INT
    { "All",                 kHelpType_All               },
    { "HeroOnly",             kHelpType_HeroOnly           },
    { "VillianOnly",         kHelpType_VillainOnly     },
    { "PraetorianOnly",         kHelpType_PraetorianOnly  },
    DEFINE_END
};

typedef struct HelpItem HelpItem;
typedef struct HelpItem
{
    HelpType    type;
    char        *displayName;
    char        *text;
    char        *vtext;
    char        *ptext;
    HelpItem    **sub_item;
} HelpItem;

typedef struct HelpCategory
{
    char        *displayName;
    HelpItem    **items;
    int            open;
    HelpType    type;        

} HelpCategory;

typedef struct HelpDictionary
{
    int init;
    HelpCategory **categories;
} HelpDictionary;

HelpDictionary gHelpDictionary = {0};
HelpDictionary g_v_HelpDictionary = {0};

// Parse structures
//---------------------------------------------------------------------

//TokenizerParseInfo ParseHelpItem[] =
//{
//    { "Name", TOK_STRING(HelpItem, displayName,0)    },
//    { "Text", TOK_STRING(HelpItem, text,0)        },
//    { "End",  TOK_END,        0                            },
//    { "", 0, 0 }
//};
//
//TokenizerParseInfo ParseHelpCategory[] =
//{
//    { "Name", TOK_STRING(HelpCategory, displayName,0)                                },
//    { "Item", TOK_STRUCT(HelpCategory, items, ParseHelpItem)    },
//    { "End",    TOK_END,        0                                                            },
//    { "", 0, 0 }
//};
//
//TokenizerParseInfo ParseHelpDictionary[] =
//{
//    { "Category", TOK_STRUCT(HelpDictionary, categories, ParseHelpCategory) },
//    { "", 0, 0 }
//};

//-----------------

TokenizerParseInfo ParseHelpItem2[] =
{
    { "Name", TOK_STRUCTPARAM|TOK_STRING(HelpItem, displayName,0)    },
    { "Type", TOK_STRUCTPARAM|TOK_INT(HelpItem, type,0), HelpTypeEnum    },
    { "Text", TOK_STRING(HelpItem, text,0)        },
    { "V_Text", TOK_STRING(HelpItem, vtext,0)        },
    { "P_Text", TOK_STRING(HelpItem, ptext,0)        },
    { "{",    TOK_START,        0                            },
    { "}",  TOK_END,        0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseHelpItem[] =
{
    { "Name", TOK_STRUCTPARAM|TOK_STRING(HelpItem, displayName,0)    },
    { "Type", TOK_STRUCTPARAM|TOK_INT(HelpItem, type,0), HelpTypeEnum    },
    { "Text", TOK_STRING(HelpItem, text,0)        },
    { "V_Text", TOK_STRING(HelpItem, vtext,0)        },
    { "P_Text", TOK_STRING(HelpItem, ptext,0)        },
    { "SubItem", TOK_STRUCT(HelpCategory, items, ParseHelpItem2)    },
    { "{",    TOK_START,        0                            },
    { "}",  TOK_END,        0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseHelpCategory[] =
{
    { "Name", TOK_STRUCTPARAM|TOK_STRING(HelpCategory, displayName,0)    },
    { "Type", TOK_STRUCTPARAM|TOK_INT(HelpCategory, type, 0), HelpTypeEnum    },
    { "Item", TOK_STRUCT(HelpCategory, items, ParseHelpItem)    },
    { "{",    TOK_START,        0                            },
    { "}",    TOK_END,        0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseHelpDictionary[] =
{
    { "Category", TOK_STRUCT(HelpDictionary, categories, ParseHelpCategory) },
    { "", 0, 0 }
};


uiTabControl * helpTC = 0;

void loadHelp()
{
    char achPath[MAX_PATH];
    char v_achPath[MAX_PATH];
    
    strcpy(achPath, "/texts/");
    strcat(achPath, locGetName(locGetIDInRegistry()));
    strcpy(v_achPath,achPath);

    strcat(achPath, "/help/help.txt");
    //strcat(v_achPath, "/help/v_help.txt");    

    if (!ParserLoadFiles(NULL, achPath, NULL, 0, ParseHelpDictionary, &gHelpDictionary, NULL, NULL, NULL))
    {
        printf("Couldn't load help!!\n");
    }

    if(0)
    {
        int i,j;

        for( i = 0; i < eaSize(&gHelpDictionary.categories); i++ )
        {
            HelpCategory * hCat = gHelpDictionary.categories[i];
            HelpCategory * vCat = 0;

            for( j = 0; j < eaSize(&g_v_HelpDictionary.categories); j++ )
            {
                if( stricmp( hCat->displayName, g_v_HelpDictionary.categories[j]->displayName) == 0 )
                {
                    vCat = g_v_HelpDictionary.categories[j];
                    eaRemove(&g_v_HelpDictionary.categories, j);
                }
            }

            if(!vCat)
                hCat->type = kHelpType_HeroOnly;
            else
            {
                int k, x;
                for(k = 0; k < eaSize(&hCat->items); k++ )
                {
                    HelpItem * hItem = hCat->items[k];
                    HelpItem * vItem = 0;
                    for( x = 0; x < eaSize(&vCat->items); x++ )
                    {
                        if( stricmp( hItem->displayName, vCat->items[x]->displayName) == 0 )
                        {
                            vItem = vCat->items[x];
                            eaRemove(&vCat->items, x);
                        }
                    }

                    if( !vItem )
                        hItem->type = kHelpType_HeroOnly;
                    else
                    {
                        if( stricmp( hItem->text, vItem->text) != 0 )
                        {
                            hItem->vtext = StructAllocString(vItem->text); 
                        }
                    }
                }
                for(k = 0; k < eaSize(&vCat->items); k++ )
                {
                    vCat->items[k]->type = kHelpType_VillainOnly;
                    eaPush(&hCat->items, vCat->items[k] );
                }
            }
        }


        for( i = 0; i < eaSize(&g_v_HelpDictionary.categories); i++ )
        {
            HelpCategory *vCat = g_v_HelpDictionary.categories[i];
            vCat->type = kHelpType_VillainOnly;
            eaPush(&gHelpDictionary.categories, vCat);
        }

        ParserWriteTextFile( "c:\\FrenchHelp.txt", ParseHelpDictionary, &gHelpDictionary, 0, 0);
    }

    comboboxTitle_init(&comboHelperStatus, 140, 10, 20, 100, 20, 400, WDW_HELP);
    comboboxTitle_add(&comboHelperStatus, 0, NULL, "HelperStatusNone", "HelperStatusNone", 3, CLR_WHITE, 0, 0);
    comboboxTitle_add(&comboHelperStatus, 0, NULL, "HelperStatusHelpMe", "HelperStatusHelpMe", 1, CLR_WHITE, 0, 0);
    comboboxTitle_add(&comboHelperStatus, 0, NULL, "HelperStatusHelper", "HelperStatusHelper", 2, CLR_WHITE, 0, 0);    
}

void helpSet(int categoryIdx)
{
    window_openClose(WDW_HELP);
    gHelpDictionary.categories[categoryIdx]->open = 1;

}

static int statusPopupDone = 0; // mild hack to prevent the popup from repopping

void helperStatusPopupHelpMe(void *blah)
{
    cmdParse("sethelperstatus 1");
    statusPopupDone = playerPtr()->db_id;
}

void helperStatusPopupHelper(void *blah)
{
    cmdParse("sethelperstatus 2");
    statusPopupDone = playerPtr()->db_id;
}

void helperStatusPopupNone(void *blah)
{
    cmdParse("sethelperstatus 3");
    statusPopupDone = playerPtr()->db_id;
}

void displayHelperStatusPopup(void)
{
    dialogStd3(DIALOG_THREE_RESPONSE, textStd("DoYouWantToFlagForHelp"), "HelperStatusHelpMe", "HelperStatusNone", "HelperStatusHelper", helperStatusPopupHelpMe, helperStatusPopupNone, helperStatusPopupHelper, 0);
}

// Parse structures
//---------------------------------------------------------------------

int helpWindow()
{
    float x, y, z, wd, ht, scale;
    int i,j, ty, color, bcolor, reparse = 0, count = 0;
    HelpDictionary *pDict = &gHelpDictionary;
    static HelpItem *current_item, *last_item;
    static HelpCategory *current_cat = 0;
    static int lastItem = 0, last_cov_state = -1;
    CBox box;
    UIBox uibox;
    Entity * e = playerPtr();
    static ScrollBar sb = {0};

    static AtlasTex *front_tab;
    static AtlasTex *back_tab;
    static texBindInit=0;

    int cov_state = ENT_IS_PRAETORIAN(e)?2:ENT_IS_VILLAIN(e);


    if (!texBindInit)
    {
        texBindInit = 1;
        front_tab = atlasLoadTexture("map_tab_active.tga");
        back_tab  = atlasLoadTexture("map_tab_inactive.tga");
    }

    if( !pDict )
         loadHelp();    

    if(!gHelpView)
    {
        gHelpView = smfview_Create(WDW_HELP);
        smfview_SetAttribs(gHelpView, &gTextAttr);
    }

    if (e && e->pl && !e->pl->helperStatus && statusPopupDone != e->db_id
        && game_state.base_map_id != 41 // Neutral Tutorial
        && game_state.base_map_id != 40 // Praetorian Tutorial
        && game_state.base_map_id != 24 // Old Hero Tutorial 
        && game_state.base_map_id != 70 // Old Villain Tutorial
        && !game_state.mission_map)
    {
        displayHelperStatusPopup();
    }

    if( !window_getDims( WDW_HELP, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) || !(pDict) )
        return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );
     drawVerticalLine( x + (PIX3+250)*scale, y + (PIX3)*scale, ht - (2*PIX3)*scale, z, scale, CLR_WHITE );

    if( window_getMode( WDW_HELP ) != WINDOW_DISPLAYING )
        return 0;

    uiBoxDefine(&uibox, x+PIX3*scale, y+PIX3*scale+(PIX3 + 40)*scale, wd - 2* PIX3*scale, ht-2*PIX3*scale-(PIX3 + 40)*scale);
    clipperPush(&uibox);
     ty = y + (PIX3 + 15)*scale - sb.offset;
    font( &game_12 );

     for( i = 0; i < eaSize(&pDict->categories); i++)
    {
        HelpCategory *currentCategory = pDict->categories[i];

        if( !currentCategory->type || 
            (ENT_IS_PRAETORIAN(e) && currentCategory->type == kHelpType_PraetorianOnly) ||
            (ENT_IS_VILLAIN(e) &&currentCategory->type == kHelpType_VillainOnly) ||
            (ENT_IS_HERO(e) && currentCategory->type == kHelpType_HeroOnly ) )
        {        
            box.lx = x + 20*scale;
            box.ly = ty + count*15*scale + (PIX3 + 40)*scale;

            str_dims( &game_12, scale, scale, 0, &box, "[ + ] %s",  textStd(currentCategory->displayName) );

            if( currentCategory->open )
                font_color( CLR_YELLOW, CLR_RED );
            else
                font_color( CLR_WHITE, CLR_WHITE );
            
            if( mouseCollision( &box ) )
            {
                font_color( CLR_WHITE, CLR_GREEN );
                if( mouseClickHit( &box, MS_LEFT) )
                    currentCategory->open = !currentCategory->open;
            }

            if( currentCategory->open )
            {
                prnt( box.lx, ty + count*15*scale + (PIX3 + 40)*scale, z, scale, scale,  "[ - ] %s", currentCategory->displayName );
                count++;
                for( j = 0; j < eaSize(&currentCategory->items); j++)
                {
                    if( !currentCategory->items[j]->type || 
                        (ENT_IS_IN_PRAETORIA(e) && currentCategory->items[j]->type == kHelpType_PraetorianOnly) ||
                        (ENT_IS_VILLAIN(e) && currentCategory->items[j]->type == kHelpType_VillainOnly) ||
                        (ENT_IS_HERO(e) && currentCategory->items[j]->type == kHelpType_HeroOnly ) )
                    {
                        box.lx = x + 40*scale;
                        box.ly = ty + count*15*scale + (PIX3 + 40)*scale;

                        if( box.ly < y || box.ly > y + ht + 30*scale )
                        {
                            count++;
                            continue;
                        }

                        str_dims( &game_12, scale, scale, 0, &box, currentCategory->items[j]->displayName );

                        if( currentCategory->items[j] == current_item )
                            font_color( CLR_YELLOW, CLR_RED );
                        else
                        {
                            if( mouseCollision( &box ) )
                            {
                                font_color( CLR_WHITE, CLR_GREEN );
                                if( mouseClickHit( &box, MS_LEFT) )
                                {
                                    current_item = currentCategory->items[j];
                                    current_cat = currentCategory;
                                }
                            }
                            else
                                font_color( CLR_WHITE, CLR_WHITE );
                        }

                        prnt( box.lx, ty + count*15*scale + (PIX3 + 40)*scale, z, scale, scale, currentCategory->items[j]->displayName );
                        count++;
                    }
                }
            }
            else
            {
                prnt( box.lx, ty + count*15*scale + (PIX3 + 40)*scale, z, scale, scale,  "[ + ] %s", currentCategory->displayName );
                count++;
            }        
        }
    }
     clipperPop();

       doScrollBar( &sb, ht - PIX3*2*scale - (PIX3 + 40)*scale, (count+1)*15*scale, x + 256*scale, y + (PIX3 + 40)*scale, z, 0, &uibox );

     ty = y + (PIX3 + 40)*scale;
    if( last_item != current_item )
        smfview_Reparse(gHelpView);

     gTextAttr.piScale = (int*)((int)(1.1f*SMF_FONT_SCALE*scale));
    smfview_SetLocation(gHelpView, (PIX3+260)*scale,ty-y, 0);
     smfview_SetSize(gHelpView, wd - (260+PIX3*2)*scale, ht - (ty-y) - PIX3*scale);

    if( current_item )
    {
         font(&game_18);
         font_color( CLR_WHITE, CLR_WHITE );
         prnt( x + (PIX3+265)*scale, y + 20*scale, z, scale, scale,  "%s", current_cat->displayName );
        font(&game_12);
        prnt( x + (PIX3+265)*scale, y + 35*scale, z, scale, scale,  "%s", current_item->displayName );
        if( ENT_IS_IN_PRAETORIA(e) && current_item->ptext )
            smfview_SetText(gHelpView, textStd(current_item->ptext));
        else if( ENT_IS_VILLAIN(e) && current_item->vtext )
            smfview_SetText(gHelpView, textStd(current_item->vtext));
        else
            smfview_SetText(gHelpView, textStd(current_item->text));
    }

    smfview_Draw(gHelpView);

    last_item = current_item;

    prnt(x + 25*scale, y + 28*scale, 20000, scale, scale, "CurrentHelperStatusColon");
    if (e && e->pl)
        comboHelperStatus.currChoice = e->pl->helperStatus;
    combobox_display(&comboHelperStatus);
    if (comboHelperStatus.changed 
        && e && e->pl && e->pl->helperStatus != comboHelperStatus.elements[comboHelperStatus.currChoice]->id)
    {
        char temp[50];
        sprintf(temp, "sethelperstatus %i", comboHelperStatus.elements[comboHelperStatus.currChoice]->id);
        cmdParse(temp);
    }

    return 0;
}


