

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include "formatter/smf_main.h"
#include "formatter/smf_util.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "mission/MissionSearch.h"
#include "gameComm/wdwbase.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "player/player.h"
#include "entity/entity.h"

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "uiMissionComment.h"
#include "UI/uiBox.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "uiHelpButton.h"
#include "AutoGen/uiMissionComment_c_ast.h"

static TextAttribs s_taMissionComplaint =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_MM_TEXT,
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

AUTO_STRUCT;
typedef struct MissionComment
{
    int iArcId;
    PCStoryComplaint eType;
    char * pchName;
    char * pchComment;
    SMFBlock *pBlock; NO_AST
}MissionComment;

AUTO_STRUCT;
typedef struct MissionCommentList
{
    MissionComment **ppComments;
}MissionCommentList;

MissionCommentList s_CommentList;
MissionCommentList s_CommentListAdmin;

void loadMissionComments()
{
    char buff[MAX_PATH];
    sprintf(buff, "%s/Comments.txt", missionMakerPath() );
    ParserLoadFiles(NULL, buff, NULL, 0, parse_MissionCommentList, &s_CommentList, NULL, NULL, NULL );
}

void saveMissionComments()
{
    char buff[MAX_PATH];
    sprintf(buff, "%s/Comments.txt", missionMakerPath() );
    mkdirtree(buff);
    ParserWriteTextFile(buff, parse_MissionCommentList, &s_CommentList, 0, 0 );
}

void missioncomment_ClearAdmin(int arcid)
{
    if(arcid)
    {
        int i;
        for(i = eaSize(&s_CommentListAdmin.ppComments)-1; i >= 0; --i)
            if(s_CommentListAdmin.ppComments[i]->iArcId == arcid)
                StructDestroy(parse_MissionComment, eaRemove(&s_CommentListAdmin.ppComments, i));
    }
    else
    {
        StructClear(parse_MissionCommentList, &s_CommentListAdmin);
    }
}

void missioncomment_Add(int owned, int arcid, int type, char * name, char *comment)
{
    MissionComment *pComment;
    
    if(!arcid || !comment)
        return;

    pComment = StructAllocRaw(sizeof(MissionComment));
    pComment->iArcId =  arcid;
    pComment->eType = type;
    pComment->pchComment = StructAllocString(comment);
    if( name )
        pComment->pchName = StructAllocString(name);

    if(owned)
    {
        eaPush(&s_CommentList.ppComments, pComment);
        saveMissionComments();
    }
    else
    {
        eaPush(&s_CommentListAdmin.ppComments, pComment);
    }
}

int missioncomment_Exists( int arcid )
{
    int i;
    for( i = eaSize(&s_CommentList.ppComments)-1; i>=0; i-- )
        if( s_CommentList.ppComments[i]->iArcId == arcid )
            return 1;
    for( i = eaSize(&s_CommentListAdmin.ppComments)-1; i>=0; i-- )
        if( s_CommentListAdmin.ppComments[i]->iArcId == arcid )
            return 1;
    return 0;
}

static int s_iArcId=0;
void missioncomment_Open( int arcid )
{
    window_openClose(WDW_MISSIONCOMMENT);
    s_iArcId = arcid;
}

static F32 drawCommentLine(MissionComment *pComment, F32 x, F32 y, F32 z, F32 sc, F32 wd, int * deleteme )
{
    F32 retval = 0;

    if(!pComment->pBlock)
        pComment->pBlock = smfBlock_Create();

    if( pComment->eType == kComplaint_Comment )
        smf_SetRawText(pComment->pBlock, textStd("MMComment", pComment->pchName, pComment->pchComment ), 0);
    else if(pComment->eType == kComplaint_BanMessage)
        smf_SetRawText(pComment->pBlock, textStd("MMBanMessage", pComment->pchComment), 0);
    else
        smf_SetRawText(pComment->pBlock, textStd("MMComplaint", pComment->pchComment ), 0);

    retval = smf_Display(pComment->pBlock, x+R10*sc, y+PIX3*sc, z+1, wd - 20*sc, 0, 0, 0, &s_taMissionComplaint, 0 );

    if( deleteme && drawMMButton( "DeleteString", "delete_button_inside", "delete_button_outside", x + wd - 55*sc, y+retval+20*sc, z+1, 100*sc, sc*.8f, MMBUTTON_SMALL|MMBUTTON_ERROR, 0 ) )
        *deleteme = 1;

    drawFlatFrame( PIX3, R10, x, y, z, wd, retval+2*PIX3*sc + 30*sc, sc, CLR_MM_FRAME_BACK, CLR_MM_FRAME_BACK );

    return retval + (PIX3+R10+30)*sc;
}

int missionCommentWindow()
{
    F32 x, y, z, wd, ht, sc, tht = 0;
    static ScrollBar sb = { WDW_MISSIONCOMMENT };
    UIBox box;
    static HelpButton *pHelp;
    int deleteme = 0;
    int i;
    Entity *e = playerPtr();


    if(!window_getDims(WDW_MISSIONCOMMENT, &x, &y, &z, &wd, &ht, &sc, 0, 0 ) )
        return 0;

    if( !s_iArcId || !missioncomment_Exists(s_iArcId) )
        window_setMode(WDW_MISSIONCOMMENT, WINDOW_SHRINKING);

       drawWaterMarkFrame( x, y, z, sc, wd, ht, 1 );

    font( &title_14 ); 
    font_color( CLR_MM_TEXT, CLR_MM_TEXT );
    prnt( x + R10*sc, y + 25*sc, z, sc, sc, "MMCommentsAndComplaints");
     helpButton(&pHelp, x + wd - 30*sc, y +20*sc, z, sc, "MMCommentsAndComplaintsTip", 0);
     tht = 40*sc;

     uiBoxDefine(&box, x+ PIX3*sc, y + tht, wd - 2*PIX3*sc, ht-PIX3*sc - tht);
    sb.architect = 1;
    clipperPush(&box);

    for( i = eaSize(&s_CommentList.ppComments)-1; i>=0; i-- )
    {
        MissionComment *pComment = s_CommentList.ppComments[i];
        if( pComment->iArcId == s_iArcId )
        {
            tht += drawCommentLine( pComment, x + R10*sc, y + tht - sb.offset, z, sc, wd-2*R10*sc, &deleteme );
            
            if( deleteme )
            {
                eaRemove( &s_CommentList.ppComments, i);
                if( pComment->pBlock )
                    smfBlock_Destroy( pComment->pBlock );
                StructDestroy( parse_MissionComment, pComment);
                saveMissionComments();
                deleteme = 0;
            }
        }
    }

    if( e && e->access_level )
    {
        for( i = eaSize(&s_CommentListAdmin.ppComments)-1; i>=0; i-- )
        {
            MissionComment *pComment = s_CommentListAdmin.ppComments[i];
            if( pComment->iArcId == s_iArcId )
                tht += drawCommentLine(pComment, x + R10*sc, y + tht - sb.offset, z, sc, wd-2*R10*sc, NULL );
        }
    }

    clipperPop();

    doScrollBar(&sb, ht-R10*sc*2, tht, x +wd, R10*sc, z, 0, &box );
    return 0;
}

#include "AutoGen/uiMissionComment_c_ast.c"
