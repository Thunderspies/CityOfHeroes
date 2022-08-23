
#include "gameComm/wdwbase.h"

#include "UI/uiMissionReview.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/uiBox.h"
#include "UI/uiScrollBar.h"
#include "UIEdit.h"
#include "UI/uiPetition.h"
#include "UI/uiFocus.h"
#include "UI/uiNet.h"
#include "UI/uiEmail.h"
#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "uiHelpButton.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"

#include "entity/entity.h"
#include "entity/teamCommon.h"
#include "player/player.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/file.h>
#include "mission/MissionSearch.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include <utilitieslib/language/AppLocale.h>

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

#include "formatter/smf_main.h"
#define SMFVIEW_PRIVATE
#include "uiSMFView.h"


typedef struct ArchitectMissionReview
{
    int id;
    int flags;

    int rating;
    int votefield;
    int euro;

    int completed;
    int rated;
    int commented;
    int reviewed;
    int reported;

    char * pchName;
    char * pchAuthor;
    char * pchDesc;

}ArchitectMissionReview;

ArchitectMissionReview gMissionReview;


static TextAttribs s_taMissionReview =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)CLR_MM_TEXT,
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

static SMFBlock *s_pMissionReview = 0;

#define ARCHITECT_KEYWORD_COLUMNS 3
#define ARCHITECT_KEYWORD_ROWS 10
static ToolTip s_keywordTips[ARCHITECT_KEYWORD_COLUMNS][ARCHITECT_KEYWORD_ROWS];
char **g_ppKeywords[ARCHITECT_KEYWORD_COLUMNS] ;



void missionReview_UpdateTexts(char * pchName,  char * pchAuthor, char * pchDesc, int id, int rated)
{
    char *estr;
    int i;

    if (!s_pMissionReview)
        s_pMissionReview = smfBlock_Create();

    estrCreate(&estr);
      estrConcatf(&estr, "<scale 1.5><i><font face=title outline=1 color=ArchitectTitle>%s</font></i></scale><br>", pchName?smf_EncodeAllCharactersGet(pchName):textStd("MMNoTitle") );
    if( pchAuthor && strlen(pchAuthor) )
        estrConcatf(&estr, "<font outline=0 color=ArchitectTitle><i>%s</font></i><br>", textStd("MissionSearchByLine", pchAuthor) );
    if( id )
        estrConcatf(&estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%i</font><br>", textStd("MissionSearchArcId"), id );
    if( rated )
    {
        static char * ratings[] = { "RatedPoor", "RatedOk", "RatedGood", "RatedExcellent", "RatedAwesome", "Rated?" };
        char * ratingStr = "Rated?";
        if (rated >=0 && rated < ARRAY_SIZE(ratings))
            ratingStr = ratings[rated];

        estrConcatf( &estr, "<font outline=1 color=ArchitectTitle>%s</font>", textStd("MissionSearchRated"));
        for( i = 0; i < 5; i++ )
        {
            if( i >= (int)rated )
                estrConcatStaticCharArray(&estr, "<img src=Star_Selected_small.tga border=1 color=#aaaaaa>");
            else
                estrConcatStaticCharArray(&estr, "<img src=Star_Selected_small.tga border=1>");
        }
        estrConcatf( &estr, "<font outline=0 color=Architect>%s</font><br>", textStd(ratingStr));
    }

     estrConcatf( &estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font>", textStd("MissionSearchSummary"), smf_EncodeAllCharactersGet(pchDesc) );
        
    smf_SetRawText(s_pMissionReview, estr, false);
    estrDestroy(&estr);
}

void missionReview_Set( int id, int flags, char * pchName,  char * pchAuthor, char * pchDesc, int rated, int euro )
{
    // You can rate arcs as often as you like. The vote just changes on the
    // server.
     if( id != gMissionReview.id )
    {
        gMissionReview.reported = 0;
        gMissionReview.completed = 0;
        gMissionReview.rating = 0;
        gMissionReview.commented = 0;
    }

    gMissionReview.id = id;
    gMissionReview.flags = flags;
    gMissionReview.euro = euro;

    SAFE_FREE(gMissionReview.pchName);
    gMissionReview.pchName = strdup(pchName);

    SAFE_FREE(gMissionReview.pchAuthor);
    if( strlen(pchAuthor) )
        gMissionReview.pchAuthor = strdup(pchAuthor);

    SAFE_FREE(gMissionReview.pchDesc);
    gMissionReview.pchDesc = strdup(pchDesc);

    gMissionReview.rated = rated;

    missionReview_UpdateTexts(pchName, pchAuthor, pchDesc, id, rated);
}

void missionReview_Complete( int id )
{
    if( id != gMissionReview.id )
    {
        return; // our information is out of synch somehow, ignore request
    }

    gMissionReview.completed = 1;
    window_setMode(WDW_MISSIONREVIEW, WINDOW_GROWING);
}

int onArchitectTestMap()
{
    int onArchitect = playerPtr() && playerPtr()->onArchitect;
    int onArchitectMap = onArchitect && gMissionReview.flags&ARCHITECT_TEST && game_state.mission_map;
    if(!onArchitect && gMissionReview.flags&ARCHITECT_TEST)
        gMissionReview.flags&= ~ARCHITECT_TEST;
    return onArchitectMap;
}

static int drawRatingStar( int idx, F32 x, F32 y, F32 z, F32 sc )
{
    AtlasTex * star = atlasLoadTexture( "Star_Selected.tga");
    AtlasTex * star_in = atlasLoadTexture( "Star_Unselected_inside.tga");
    AtlasTex * star_out = atlasLoadTexture( "Star_Unselected_outside.tga");
    int clr = CLR_MM_TITLE_OPEN;
    CBox box;
    F32 spr_sc = 1.f;

      BuildCBox( &box, x+star->width*sc*idx, y, star->width*sc, star->height*sc );
 
       if( mouseCollision(&box)  )
    {
        clr = CLR_MM_TITLE_LIT;
        if(isDown(MS_LEFT))
        {
            spr_sc *= .95f;
            clr = 0xc6ce9aff;
        }
        display_sprite(star_in, x+star->width*sc*idx + (sc-spr_sc)*(star_in->width/2), y + (sc-spr_sc)*(star_in->height/2), z, sc, sc, CLR_MM_TITLE_OPEN );
    }

       if( gMissionReview.rating >= idx+1 )
        display_sprite(star, x+star->width*sc*idx + (sc-spr_sc)*(star->width/2), y + (sc-spr_sc)*(star->height/2), z, sc, sc, clr );
    else
        display_sprite(star_out, x+star->width*sc*idx + (sc-spr_sc)*(star_out->width/2), y + (sc-spr_sc)*(star_out->height/2), z, sc, sc, clr );

    if( mouseClickHit(&box, MS_LEFT) )
    {
        if( idx == 0 && gMissionReview.rating == 1 ) // special case to zero rating
            gMissionReview.rating = 0;
        else
            gMissionReview.rating = idx+1;

        return 1;
    }

    return 0;
}

void missionReviewLoadKeywordList()
{
    int i, j;
    char keywordBuff[64];

    if( !g_ppKeywords[0] )
    {
        for( i = 0; i < ARCHITECT_KEYWORD_COLUMNS; i++ )
        {
            for( j = 0; j < ARCHITECT_KEYWORD_ROWS; j++ )
            {
                sprintf( keywordBuff, "MMColumn%iKeyword%i", i+1, j+1 );
                if( stricmp( keywordBuff, textStd(keywordBuff) ) != 0  )
                    eaPush( &g_ppKeywords[i], strdup(textStd(keywordBuff)) );
                else
                    eaPush( &g_ppKeywords[i], "" );
                addToolTip( &s_keywordTips[i][j] );
            }
        }
    }
}

char * missionReviewGetKeywordKeyFromNumber( int i )
{
    int col = floor(i/ARCHITECT_KEYWORD_ROWS); 
    int row = i - ARCHITECT_KEYWORD_ROWS*col;

    if( col >= 0 && col < ARCHITECT_KEYWORD_COLUMNS && EAINRANGE(row, g_ppKeywords[col]) && g_ppKeywords[col][row] && *g_ppKeywords[col][row]  )
        return g_ppKeywords[col][row];

    return 0;
}

void missionReviewResetKeywordsTooltips()
{
    int i,j;
    char keywordBuff[64];
    if( g_ppKeywords[0] )
    {
        for( i = 0; i < ARCHITECT_KEYWORD_COLUMNS; i++ )
        {
            for( j = 0; j < eaSize(&g_ppKeywords[i]); j++ )
            {

                if( !g_ppKeywords[i][j] || !*g_ppKeywords[i][j] )
                    continue;

                sprintf( keywordBuff, "%sTip", g_ppKeywords[i][j] );
                clearToolTip(&s_keywordTips[i][j]);
            }
        }
    }
}

F32 missionReviewDrawKeywords(F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, int wdw, U32 *bitfield, int limitSelection, int backcolor)
{
    int i,j;
    F32 startY = y;
    char keywordBuff[64];
    int selections = 0;
    if( g_ppKeywords[0] )
    {
        F32 twd = wd-4*10*sc;
        F32 max_ht = 0;

        y += 20*sc;

        for( i = 0; i < ARCHITECT_KEYWORD_COLUMNS; i++ )
        {
            F32 ty = y, tht = 0;
            F32 tx = x + ((float)i+.5)*((wd/ARCHITECT_KEYWORD_COLUMNS))-MIN(wd/6 - 20*sc, 75*sc);

            for( j = 0; j < eaSize(&g_ppKeywords[i]); j++ )
            {
                int val = !!((*bitfield)&(1<<(i*ARCHITECT_KEYWORD_ROWS+j))); 
                F32 rwd, rht;
                UIBox clipbox;
                CBox cbox;

                if( !g_ppKeywords[i][j] || !*g_ppKeywords[i][j] )
                    continue;

                sprintf( keywordBuff, "%sTip", g_ppKeywords[i][j] );
                uiBoxDefine(&clipbox, tx, ty + tht, wd/ARCHITECT_KEYWORD_COLUMNS-20, 20*sc );
                uiBoxToCBox(&clipbox, &cbox);
                clipperPushRestrict(&clipbox);
                setToolTip(&s_keywordTips[i][j], &cbox, textStd(keywordBuff), 0, MENU_GAME, wdw );

                // g_ppKeywords are added to array in textStd formatting already
                if (drawMMCheckBox( tx, ty + tht, z+1, sc, g_ppKeywords[i][j], &val, CLR_MM_TEXT, CLR_MM_BUTTON, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON_LIT, &rwd, &rht ) )
                {
                    if(val)
                        (*bitfield) |= (1<<(i*ARCHITECT_KEYWORD_ROWS+j));
                    else
                        (*bitfield) &= ~(1<<(i*ARCHITECT_KEYWORD_ROWS+j));
                    if(!limitSelection)
                        missionserver_game_setKeywordsForArc(gMissionReview.id, (*bitfield));
                }
                if(val)
                    selections++;
                clipperPop();

                tht += rht+2*sc;
                //tht += 15*sc;
            }

            max_ht = MAX( max_ht, tht );
        }
        if(selections >limitSelection)
            *bitfield = 0;

         font_color( CLR_MM_TEXT, CLR_MM_TEXT );
        font(&game_14);
        if(!limitSelection)
            prnt( x + 20*sc, startY, z+1, sc, sc, "MMKeywordDesc" );
        else if(selections ==limitSelection)
            prnt( x + 20*sc, startY, z+1, sc, sc, "MMKeywordLimit" );
        else
        {
            if( wdw == WDW_MISSIONMAKER )
                prnt( x + 20*sc, startY, z+1, sc, sc, "MMKeywordPick" );
            else
                prnt( x + 20*sc, startY, z+1, sc, sc, "MMKeywordSelect" );
        }

        drawFlatFrame( PIX3, R10, x + 10*sc, y-40*sc, z, wd-20*sc, max_ht+50*sc, sc, backcolor, backcolor );
        y += max_ht;
    }
    return y-startY;
}



int missionReviewWindow(void)
{
    static int report_open;
    static ScrollBar sb = {WDW_MISSIONREVIEW};
    static SMFBlock *dialogEdit = 0, *reportEdit=0;

    F32 x, y, z, wd, ht, startY, sc, txt_ht;
    int i=1,j, color, bcolor;
    Entity *e = playerPtr();
    UIBox box;

       if(!window_getDims(WDW_MISSIONREVIEW, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    if(!gMissionReview.id && !(gMissionReview.flags&ARCHITECT_TEST))
    {
        window_setMode(WDW_MISSIONREVIEW, WINDOW_SHRINKING);
        return 0;
    }
    
    missionReviewLoadKeywordList();

    //drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
    drawWaterMarkFrame( x, y, z, sc, wd, ht, 1 );

     startY = y;
     uiBoxDefine(&box, x, y+PIX3*sc, wd, ht-PIX3*sc*2 );
    clipperPush(&box);

    // Mission Title
    // By Author
    // Desc
       y += smf_Display(s_pMissionReview, x+2*R10*sc, y-sb.offset+2*R10*sc, z, wd-4*R10*sc, 0, 0, 0, &s_taMissionReview, 0);
    y += 20*sc;

       if( !(gMissionReview.flags&ARCHITECT_TEST) )  
    {
        // Submit a comment
        if( !gMissionReview.commented && gMissionReview.pchAuthor )
        {
            if( !dialogEdit )
            {
                dialogEdit = smfBlock_Create();
                smf_SetRawText( dialogEdit, textStd("MissionReviewComment"),0);
                smf_SetFlags(dialogEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_MultiLineBreakOnWhitespace, SMFInputMode_AnyTextNoTagsOrCodes, 512, 
                    SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
                smf_SetCharacterInsertSounds(dialogEdit, "type2", "mouseover");
            }

            y += 20*sc;

               txt_ht = smf_Display(dialogEdit,  x + 2.5*R10*sc, y + PIX3*sc -sb.offset, z+2, wd - 5*R10*sc, 0, 0, 0, &s_taMissionReview, 0);
            drawTextEntryFrame( x + 2*R10*sc, y-sb.offset, z, wd - 4*(R10)*sc, txt_ht+6*sc, sc, smfBlock_HasFocus(dialogEdit) );
            
            y += txt_ht + 20*sc;

                if( drawMMButton( "MissionReviewSubmitComment", 0, 0, x + wd/2, y-sb.offset + 15*sc, z, wd - 5*R10*sc, sc, MMBUTTON_SMALL|(stricmp(dialogEdit->rawString,  textStd("MissionReviewComment") ) == 0), 0 ) )
            {
                if( *gMissionReview.pchAuthor == '@' )
                {
                    if( gMissionReview.euro == locIsEuropean(getCurrentLocale()) )
                        cmdParsef("tell %s, %s %s", gMissionReview.pchAuthor, textStd("MMFeedback", gMissionReview.pchName), smf_DecodeAllCharactersGet(dialogEdit->outputString)  );
                    missionserver_game_reportArc(gMissionReview.id, kComplaint_Comment, dialogEdit->outputString);
                }
                else
                {
                    missionserver_game_comment( gMissionReview.id, smf_DecodeAllCharactersGet(dialogEdit->outputString) );
                    missionserver_game_reportArc(gMissionReview.id, kComplaint_Comment, dialogEdit->outputString);
                }
                smf_SetRawText(dialogEdit, textStd("MissionReviewComment"), 0);
                smf_ClearSelection();
                gMissionReview.commented = 1;
                smf_SetCursor(0,0);
            }

             y += 50*sc;
        }

        // Rate and Petition
           for(i=0; i<5; i++)
        {
             if( drawRatingStar( i, x + wd/2 - 70*sc, y-sb.offset, z, sc ) )
            {
                gMissionReview.reviewed = 1;
                missionserver_game_voteForArc(gMissionReview.id, gMissionReview.rating);
            }
        }

         y += 45*sc;

        if( gMissionReview.reviewed )
        {
            font_color(CLR_MM_TEXT, CLR_MM_TEXT);
            font(&game_12);
            font_outl(0);
            font_ital(1);
            if( gMissionReview.rating )
                cprnt( x + wd/2, y-sb.offset, z, sc, sc, "MMRated", gMissionReview.rating );
            font_ital(0);
            font_outl(1);
        }
         y += 25*sc;

         y += missionReviewDrawKeywords(x, y-sb.offset, z, sc, wd, ht, WDW_MISSIONREVIEW, &gMissionReview.votefield, 3, CLR_MM_FRAME_BACK);

         if( report_open )
          { 
            static int iReportCategory = 0;
            static char * reportNames[] = {"MMInappropriateContent", "MMCopyrightInfringement", "MMBrokenOrBuggedMission", "MMOther"};

            y += 25*sc;
            if( !gMissionReview.reported )
            {
                if( drawMMButton(report_open?"MissionReviewCancelReport":"MissionReviewReportForContent", "cancel", "cancel_outline", x + wd - 120*sc, y-sb.offset + 25*sc, z, 200*sc, sc, MMBUTTON_ERROR, 0)  )
                {
                    smf_SetCursor(0,0);
                    report_open = !report_open;
                }
            }
            y += 50*sc;
 
            font(&title_12);
            font_color(CLR_MM_BUTTON_ERROR_TEXT, CLR_MM_BUTTON_ERROR_TEXT);
            prnt(x+ R10*sc*2, y-sb.offset, z, sc, sc, "MMReportType");
            y+=sc*15;

             for( i = 0; i < ARRAY_SIZE(reportNames); i++ )
            {
                int iOn = (iReportCategory == i);        
                 drawMMCheckBox( x + wd/2 - 100*sc, y-sb.offset, z, sc,  textStd(reportNames[i]), &iOn, CLR_MM_BUTTON_ERROR_TEXT, CLR_MM_BUTTON_ERROR_TEXT,
                                    CLR_MM_BUTTON_ERROR_TEXT_LIT, CLR_MM_BUTTON_ERROR_LIT,0,0);
                y += 20*sc;
                if( iOn )
                    iReportCategory = i;
            }
            
             y += 20*sc;
  
            if( !reportEdit )
            {
                reportEdit = smfBlock_Create();
                smf_SetRawText( reportEdit, textStd("MMEnterReport"),0);
                smf_SetFlags(reportEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_MultiLineBreakOnWhitespace, SMFInputMode_AnyTextNoTagsOrCodes, 512, 
                    SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
                smf_SetCharacterInsertSounds(reportEdit, "type2", "mouseover");
            }
            
             txt_ht = smf_Display(reportEdit,  x + 2.5*R10*sc, y+PIX3*sc-sb.offset, z+2, wd - 5*R10*sc, 0, 0, 0, &s_taMissionReview, 0);
            drawTextEntryFrame( x + 2*R10*sc, y-sb.offset, z, wd - 4*(R10)*sc, txt_ht + 6*sc, sc,  smfBlock_HasFocus(reportEdit)  );
            y += txt_ht + 20*sc;

               if( drawMMButton("MMSendReport", 0, 0, x + wd/2, y + 10*sc-sb.offset, z, wd - 5*R10*sc, sc, MMBUTTON_ERROR|MMBUTTON_SMALL, 0 ) )
            {
                report_open = 0;
                gMissionReview.reported = 1;
                missionserver_game_reportArc(gMissionReview.id, kComplaint_Complaint, reportEdit->outputString);
                smf_SetCursor(0,0);
            }
             y += 50*sc;

             if( drawMMButton( gMissionReview.completed?"FinishedString":"MissionReviewExitStoryArc", 0, 0, x + wd/2, y-sb.offset + 25*sc, z, 200*sc, sc, 0, 0 ) )
            {
                if(!gMissionReview.completed)
                {
                    // quit the taskforce
                    cmdParse( "leaveTeam" );
                }
                smf_SetCursor(0,0);
                window_setMode(WDW_MISSIONREVIEW, WINDOW_DOCKED);
            }
            y += 50*sc;
        }
        else
        {
               y += 25*sc;
            if( drawMMButton( gMissionReview.completed?"FinishedString":"MissionReviewExitStoryArc", 0, 0, x + wd/2, y-sb.offset + 25*sc, z, 200*sc, sc, 0, 0 ) )
            {
                if(!gMissionReview.completed)
                {
                    // quit the taskforce
                    cmdParse( "leaveTeam" );
                }
                smf_SetCursor(0,0);
                window_setMode(WDW_MISSIONREVIEW, WINDOW_DOCKED);
            }
            y += 100*sc;

             if( !gMissionReview.reported && gMissionReview.pchAuthor )
            {
                 F32 buttony = y-sb.offset + 25*sc;
                if( y-startY < ht - 50*sc )
                    buttony = startY + ht - 30*sc;
                   if( drawMMButton(report_open?"MissionReviewCancelReport":"MissionReviewReportForContent", "cancel", "cancel_outline", x + wd - 145*sc, buttony, z, 250*sc, sc, MMBUTTON_ERROR, 0 ) )
                {
                    report_open = !report_open;
                    smf_SetCursor(0,0);
                }
            }
            y += 30*sc;
        }
        // Status -- Time Spent, TF Params, Tickets Earned?
    }
    else
    {
         static HelpButton *pHelp;
        y += 30*sc;  
        if( drawMMButton( gMissionReview.completed?"FinishedString":"MissionReviewExitStoryArc", 0, 0, x + wd/2, y-sb.offset + 25*sc, z, 200*sc, sc, 0, 0 ) )
        {
            if(!gMissionReview.completed)
            {
                // quit the taskforce
                cmdParse( "leaveTeam" );
            }
            window_setMode(WDW_MISSIONREVIEW, WINDOW_DOCKED);
            smf_SetCursor(0,0);
        }

        for( i = 0; i < ARCHITECT_KEYWORD_COLUMNS; i++ )
        {
            for( j = 0; j < ARCHITECT_KEYWORD_ROWS; j++ )
                clearToolTip(&s_keywordTips[i][j]);
        }

           y += 120*sc;

          drawFlatFrame( PIX3, R10, x+10*sc, y - 10*sc, z, wd-20*sc, 190*sc, sc, CLR_MM_FRAME_BACK, CLR_MM_FRAME_BACK );
         drawMMBar( "MMTestModeCommands", x+wd/2, y-10*sc, z+5, wd-80*sc, sc, 1, 0 );
          
          helpButton( &pHelp, x + wd - 30*sc, y-10*sc, z, sc, "MMTestCommandHelp", 0  )
        
         y += 40*sc;

          if( drawMMButton( e->architect_untargetable?"MMInvisibleOFF":"MMInvisibleON", 0, 0, x + wd/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL|!game_state.mission_map, 0  ))
            cmdParse( "architect_invisible" );

        if( drawMMButton( e->architect_invincible?"MMInvincibleOFF":"MMInvincibleON", 0, 0, x + (3*wd)/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL|!game_state.mission_map, 0  ))
            cmdParse( "architect_invincible" );

        y += 50*sc;
        if( drawMMButton( "MMCompleteMission", 0, 0, x + wd/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL, 0  ))
            cmdParse( "architect_completemisison" );

        if( drawMMButton( "MMNextObjective", 0, 0, x + (3*wd)/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL|!game_state.mission_map, 0  ))
            cmdParse( "architect_nextobjective" );

        y += 50*sc;
        if( drawMMButton( "MMKillTarget", 0, 0, x + wd/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL|!game_state.mission_map, 0  ))
            cmdParse( "architect_killtarget" );

        if( drawMMButton( "MMNextCritter", 0, 0, x + (3*wd)/4, y, z, wd/2 - 30*sc, sc, MMBUTTON_SMALL|!game_state.mission_map, 0  ))
            cmdParse( "architect_nextcritter" );
    }

    clipperPop();
     doScrollBar( &sb, ht-30*sc, y-startY, wd, R10*sc, z, 0, &box );

    return 0;
}
