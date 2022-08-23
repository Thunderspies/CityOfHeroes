

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiNet.h"
#include "UI/uiWindows.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiInput.h"
#include "UI/uiComboBox.h"
#include "win/win_init.h"
#include "UI/uiCostumeSelect.h"
#include "graphics/seqgraphics.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/costume.h"
#include "UI/uiDialog.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include "uiSMFView.h"
#include "formatter/smf_main.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/Hybrid/uiBody.h"
#include "auth/authUserData.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "player/inventory_client.h"

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

static SMFView *gView = 0;

#define PIC_WD                        128
#define PIC_HT                        256
#define BORDER_WD                    10
#define PIC_SPACING                    140
#define BUTTON_HT                    50
#define HORIZONTAL_DISPLAY_SLOTS    5
#define VERTICAL_DISPLAY_SLOTS        2

int gEnterTailor = 0;
static char **ccEmoteNames = NULL;
static char **ccEmoteNamesInternal = NULL;
static char **ccEmoteStoreProducts = NULL;
static ComboBox comboEmoteNames;
static int setUpComboBox = 0;
static int lastCostumeSelect = -1;
static int lastCostumeListSize = -1;

void receiveCostumeChangeEmoteList(Packet *pak)
{
    char *emote;
    char *product;

    for (;;)
    {
        emote = strdup(pktGetString(pak));
        product = strdup(pktGetString(pak));
        if (emote[0] == '@' && emote[1] == 0)
        {
            break;
        }
        if (eaSize(&ccEmoteNames) == 0)
        {
            eaPush(&ccEmoteNames, strdup(textStd("NoneString")));
            eaPush(&ccEmoteNamesInternal, strdup("x"));
            eaPush(&ccEmoteStoreProducts, strdup(""));
        }
        eaPush(&ccEmoteNames, strdup(textStd(emote)));
        eaPush(&ccEmoteNamesInternal, emote);
        eaPush(&ccEmoteStoreProducts, product);
    }

    if (eaSize(&ccEmoteNames) != 0)
    {
        if (lastCostumeListSize != eaSize(&ccEmoteNames))
        {
            lastCostumeListSize = eaSize(&ccEmoteNames);
            lastCostumeSelect = 0;
        }
        setUpComboBox = 1;
    }
}

static void ConfirmTailor(void *data)
{
    Entity *e = playerPtr();
    int index = (int) data;
    char buf[64];

    if( costume_change( e, index ) )
    {                        
        sprintf( buf, "cc %d", index );
        cmdParse( buf );
        e->powerCust = e->pl->powerCustomizationLists[e->pl->current_powerCust];
        window_setMode( WDW_COSTUME_SELECT, WINDOW_SHRINKING );
        if ((gEnterTailor == 2) && (AccountCanAccessGenderChange(&e->pl->account_inventory, e->pl->auth_user_data, e->access_level)) )
        {
            genderChangeMenuStart();
        }
        else
        {
            start_menu( MENU_TAILOR );
        }
        gEnterTailor = 0;
    }
}

int costumeSelectWindow(void)
{
    float x, y, z, wd, ht, sc;
    int i, color, bcolor, w, h, viewht = 0, cceselht = 0;
    Entity *e = playerPtr();

    static int new_open = 0;
    static int cant_drag = 0;
    static int unique_costume_counter;
    int windowWidth;
    SMFBlock *SMFblock;
    static validCostume[MAX_SELECTABLE_COSTUMES];

    windowClientSizeThisFrame( &w, &h );
       if( !window_getDims( WDW_COSTUME_SELECT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
    {
        new_open = 1;
        if( window_getMode( WDW_COSTUME_SELECT ) == WINDOW_DOCKED )
            gEnterTailor = 0;
        cant_drag = 3;
        return 0;
    }

    if(window_getMode(WDW_CONTACT_DIALOG)==WINDOW_DISPLAYING)
        window_setMode(WDW_CONTACT_DIALOG, WINDOW_SHRINKING);

    windowWidth = (BORDER_WD - PIX3 + PIC_SPACING*HORIZONTAL_DISPLAY_SLOTS)*sc;

    if( new_open )
    {
        unique_costume_counter += HORIZONTAL_DISPLAY_SLOTS*VERTICAL_DISPLAY_SLOTS;
        new_open = 0;
        
        // If we get complaints about the delay before the combobox appears, shift this elsewhere.
        // Maybe in the code that initially loads into the mapserver.  More accurately, put the
        // init code (if and for statements) into receiveCostumeChangeEmoteList(...), and the 
        // call to sendCostumeChangeEmoteListRequest() in the mapserver kickoff code.  What I
        // really mean is that the code in parseClientInput.c that fields the request from
        // sendCostumeChangeEmoteListRequest() needs to go in the mapserver code that handles a
        // new player entity.  Either that or the call mentioned goes in the game, somewhere where
        // we get a full update after a zone transfer.
        if (ccEmoteNames == NULL)
        {
            eaCreate(&ccEmoteNames);
            eaCreate(&ccEmoteNamesInternal);
            eaCreate(&ccEmoteStoreProducts);
        }

        for (i = eaSize(&ccEmoteNames) - 1; i >= 0; i--)
        {
            free(ccEmoteNames[i]);
            free(ccEmoteNamesInternal[i]);
            free(ccEmoteStoreProducts[i]);
        }
        eaSetSize(&ccEmoteNames, 0);
        eaSetSize(&ccEmoteNamesInternal, 0);
        eaSetSize(&ccEmoteStoreProducts, 0);
        sendCostumeChangeEmoteListRequest();
        memset(&comboEmoteNames, 0, sizeof(ComboBox));

        for (i = 0; i < MAX_SELECTABLE_COSTUMES; i++)
            validCostume[i] = -1;
    }

    if (setUpComboBox && window_getMode(WDW_COSTUME_SELECT) == WINDOW_DISPLAYING)
    {
        AtlasTex *tex = atlasLoadTexture("CreateSymbol_Lock.tga");

        // Talk to Andy as to how we get the width of the string, so we can replace the 400 with
        // the correct value.
        comboboxTitle_init(&comboEmoteNames, 400.0f, ((PIC_HT + 2*BORDER_WD - PIX3)*VERTICAL_DISPLAY_SLOTS), 5, 175.0f, 
                            PIX3 + R10, 24 +(14* (eaSize(&ccEmoteNames) + 1)), WDW_COSTUME_SELECT);
        comboEmoteNames.reverse = true;
        for (i = 0; i < eaSize(&ccEmoteNames); i++)
        {
            if (strlen(ccEmoteStoreProducts[i]) > 0) 
            {
                comboboxTitle_add(&comboEmoteNames, false, tex, ccEmoteNames[i], ccEmoteNames[i], i, 0xff5555ff, 0xffffffff, NULL);
            } else {
                comboboxTitle_add(&comboEmoteNames, false, NULL, ccEmoteNames[i], ccEmoteNames[i], i, 0xffffffff, 0xffffffff, NULL);
            }
        }
        setUpComboBox = 0;
        comboEmoteNames.currChoice = lastCostumeSelect;
    }

       if( gEnterTailor )
    {
        if( !gView )
        {
            gView = smfview_Create(WDW_COSTUME_SELECT);
            smfview_SetAttribs(gView, &gTextAttr);
        }

        gTextAttr.piScale = (int*)((int)(sc*SMF_FONT_SCALE));
        smfview_SetLocation(gView, PIX3*sc, 2*PIX3*sc, 5);
        smfview_SetSize(gView, windowWidth - PIX3*2*sc, 500*sc);
        smfview_SetText(gView, (gEnterTailor == 2) ? textStd("GenderSelectionMessage") : textStd("CostumeSelectionMessage", tailor_getFee(e)) );
        SMFblock = smfview_GetBlock(gView);
        smf_SetFlags(SMFblock, SMFEditMode_Unselectable, 0, 
                  0, 0, 0, 
                  0, 0, 0,
                  0, NULL,
                  NULL, 0);
        smfview_Draw(gView);
        viewht += smfview_GetHeight(gView);
        viewht += PIX3*sc;
    }
    else if (eaSize(&ccEmoteNames))
    {
        if( !gView )
        {
            gView = smfview_Create(WDW_COSTUME_SELECT);
            smfview_SetAttribs(gView, &gTextAttr);
        }

        gTextAttr.piScale = (int*)((int)(sc * SMF_FONT_SCALE * 1.3f));
        smfview_SetLocation(gView, 150 * sc, ((PIC_HT + 2*BORDER_WD - PIX3)*VERTICAL_DISPLAY_SLOTS) * sc, 5);
        smfview_SetSize(gView, ((windowWidth * 2) / 3 - PIX3 * 2) * sc, 100 * sc);
         smfview_SetText(gView, textStd("CostumeSelectEmote"));
        SMFblock = smfview_GetBlock(gView);
        smf_SetFlags(SMFblock, SMFEditMode_Unselectable, 0, 
                  0, 0, 0, 
                  0, 0, 0,
                  0, NULL,
                  NULL, 0);

        smfview_Draw(gView);
        cceselht = smfview_GetHeight(gView);
        cceselht += PIX3*sc;
        if (comboEmoteNames.elements)
        {
            combobox_display(&comboEmoteNames);
            lastCostumeSelect = comboEmoteNames.currChoice;

            // check to see if we need to display costume change emote purchase button
            if (strlen(ccEmoteStoreProducts[lastCostumeSelect]) > 0)
            {
                drawHybridStoreLargeButton(ccEmoteStoreProducts[lastCostumeSelect], x + 595.0f*sc, y + ((PIC_HT + 2*BORDER_WD - PIX3)*VERTICAL_DISPLAY_SLOTS + 10.0f)*sc, 
                                            z + 1, sc, 0xffffffff, HB_STORE_PRODUCT, WDW_COSTUME_SELECT);
            }
        }
    }

    drawFrame( PIX3, R10, x, y, z-2, wd, ht, sc, color, bcolor );

    set_scissor(1);
    scissor_dims( x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht-2*PIX3*sc );
 
       for( i = 0; i <MAX_SELECTABLE_COSTUMES && i < costume_get_num_slots(e) && window_getMode(WDW_COSTUME_SELECT) == WINDOW_DISPLAYING; i++ )
    {
           AtlasTex *costume = seqGetBodyshot( costume_as_const(e->pl->costume[i]), i+unique_costume_counter );
        CBox box;
         int frame_color = color, frame_back = 0x00000088;

        float tx = x + (BORDER_WD - PIX3 + i*PIC_SPACING)*sc;
        float ty = viewht + y + (BORDER_WD - PIX3)*sc;
        float twd = (PIC_WD+2*PIX3)*sc;
        float tht = (PIC_HT+2*PIX3)*sc;
        int currentCostume = e->pl->current_costume;
        AtlasTex * lockIcon = atlasLoadTexture("League_Icon_Lock_Closed.tga");

        if (i >= HORIZONTAL_DISPLAY_SLOTS)
        {
            int row = i/HORIZONTAL_DISPLAY_SLOTS;
            tx = x + (BORDER_WD - PIX3 + (i%HORIZONTAL_DISPLAY_SLOTS)*PIC_SPACING)*sc;
            ty = viewht + y + (BORDER_WD - PIX3)*sc + (tht + BORDER_WD)*row;
        }
        // HYBRID : Validate costume vs. store products - Add correct display icon
        if (validCostume[i] < 0)
        {
            e->pl->current_costume = i;
            validCostume[i] = costume_Validate(e, e->pl->costume[i], i, NULL, NULL, false, false);
            e->pl->current_costume = currentCostume;
        }

        if (validCostume[i] > 0)
            display_sprite( lockIcon, tx, ty, z+1, ((float) lockIcon->width*sc*0.8)/((float)lockIcon->width), ((float) lockIcon->height*sc*0.8)/((float)lockIcon->height), 0xffffffff );


          BuildCBox( &box, tx, ty, twd, tht );

         if( i == e->pl->current_costume )
        {
            frame_color = CLR_BASE_GREEN;
            frame_back = 0;
        }

         if( mouseCollision(&box) )
        {
            frame_back = 0;

              if( isDown( MS_LEFT ) )
            {
                frame_color = CLR_HIGH_SELECT;
                frame_back = 0xffffff88;
            }

               if( mouseClickHit( &box, MS_LEFT) )
            {
                char buf[64];

                collisions_off_for_rest_of_frame = 1;

                if( e->npcIndex )
                {
                    dialogStd( DIALOG_OK, "CantChangeShapshifted", NULL, NULL, NULL, NULL, 0 );
                }
                else
                {
                     if( gEnterTailor )
                    {                    
                        if (validCostume[i] > 0)
                        {
                            dialog(DIALOG_ACCEPT_CANCEL, -1, -1, -1, -1, "CostumeHasInvalidParts", "OK", ConfirmTailor, "Cancel", NULL, 0, 0, 0, 0, 0, 0, (void *) i );
                        } else {
                            ConfirmTailor((void *) i);
                        }
                    }
                    else if (eaSize(&ccEmoteNames) == 0 || comboEmoteNames.currChoice == 0)
                    {
                        sprintf( buf, "cc %d", i );
                        cmdParse( buf );
                        lastCostumeSelect = comboEmoteNames.currChoice;
                    }
                    else
                    {
                        sprintf( buf, "cce %d %s", i, ccEmoteNamesInternal[comboEmoteNames.currChoice] );
                        cmdParse( buf );
                        lastCostumeSelect = comboEmoteNames.currChoice;
                    }
                }
            }
        }

         display_sprite( costume, tx + PIX3*sc, ty + PIX3*sc, z, sc, sc, CLR_WHITE );
           drawFrame( PIX3, R6, tx, ty, z+1, twd, tht, sc, frame_color, frame_back );
    }

    for( ; i < HORIZONTAL_DISPLAY_SLOTS*VERTICAL_DISPLAY_SLOTS; i++ )
    {
        float tx = x + (BORDER_WD - PIX3 + i*PIC_SPACING)*sc;
        float ty = viewht + y + (BORDER_WD - PIX3)*sc;
        float twd = (PIC_WD+2*PIX3)*sc;
        float tht = (PIC_HT+2*PIX3)*sc;

        if (i >= HORIZONTAL_DISPLAY_SLOTS)
        {
            int row = i/HORIZONTAL_DISPLAY_SLOTS;
            tx = x + (BORDER_WD - PIX3 + (i%HORIZONTAL_DISPLAY_SLOTS)*PIC_SPACING)*sc;
            ty = viewht + y + (BORDER_WD - PIX3)*sc + (tht + BORDER_WD)*row;
        }

          drawFrame( PIX3, R6, tx, ty, z+1, twd, tht, sc, color, CLR_BLACK );

          font( &game_18 );
         font_color( 0xffffff88, 0xffffff88 );
        cprntEx( tx + twd/2, ty + tht/2, z+2, 4.f*sc, 4.f*sc, (CENTER_X|CENTER_Y), "?" );

        if (i - costume_get_num_slots(e) < costume_get_max_earned_slots(e) - e->pl->num_costume_slots)
        {
            // earned slot
            font( &game_18 );
            font_color( 0xffffffff, 0xffffff88 );
            cprntEx( tx + twd/2, ty + tht - 20*sc, z+2, 0.8f*sc, 0.8f*sc, (CENTER_X|CENTER_Y), "EarnInGame" );
        } else { 
            // purchased slot
 
            font( &game_18 );
            font_color( 0xffff55ff, 0xffffff88 );
            cprntEx( tx + twd/2, ty + tht - 20*sc, z+2, 0.8f*sc, 0.8f*sc, (CENTER_X|CENTER_Y), "BuyAtStore" );

//            if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
            {
                char productSKU[12];
                int codeNumber = 6;        // costume slot products are upcscs06 - upcscs10                
        
                while (codeNumber <= 10)
                {
                    sprintf_s(productSKU, 12, "upcscs%02d", codeNumber);
                    if (!AccountHasStoreProduct(ent_GetProductInventory( e ), skuIdFromString(productSKU) ))
                        break;
                    codeNumber++;
                }

                if (codeNumber <= 10)
                    drawHybridStoreLargeButton(productSKU, tx + twd/2, ty + tht*3/4, z+1,sc, CLR_WHITE, HB_STORE_PRODUCT, WDW_COSTUME_SELECT);
            }
        }
    }
        
     set_scissor(0);

    if( gEnterTailor )
    {
//        if( ht != (276*VERTICAL_DISPLAY_SLOTS + viewht)*sc )
        if ((int) ht != (int) (276*sc*VERTICAL_DISPLAY_SLOTS + viewht))
            window_setDims( WDW_COSTUME_SELECT, w/2 - windowWidth/2,(h - 276*sc*VERTICAL_DISPLAY_SLOTS + viewht)/2, windowWidth, 276*sc*VERTICAL_DISPLAY_SLOTS + viewht );
    }
    else if ( ht != (276*VERTICAL_DISPLAY_SLOTS + cceselht)*sc )
    {
        float x = cant_drag ? w / 2 - windowWidth / 2 : -1;
        float y = cant_drag ? (h - 276 * sc + cceselht) / 2 : -1;
//        int width = (BORDER_WD - PIX3 + PIC_SPACING*MAX_SLOTS)*sc;
        window_setDims( WDW_COSTUME_SELECT, x, y, windowWidth, 276*sc*VERTICAL_DISPLAY_SLOTS + cceselht);
        if (window_getMode(WDW_COSTUME_SELECT) == WINDOW_DISPLAYING && cant_drag)
        {
            cant_drag--;
        }
    }

    return 0;
}
