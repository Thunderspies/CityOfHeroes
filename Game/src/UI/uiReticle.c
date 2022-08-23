/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "entity/entclient.h"      // for entfromid
#include "entity/entPlayer.h"      // for entfromid
#include "graphics/ttFont.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "win/win_init.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "graphics/gfxwindow.h"  // for gfxScreenPos
#include "player/player.h"
#include "graphics/camera.h"
#include "gameComm/itemselect.h"
#include "cmdparse/cmdgame.h"
#include "edit/edit_cmd.h"
#include "UI/uiGame.h"
#include "entity/teamup.h"
#include "player/playerState.h"
#include "language/langClientUtil.h"
#include "language/commonLangUtil.h"
#include "player/pmotion.h"
#include "entity/character_base.h"
#include "entity/origins.h"
#include "gameComm/Npc.h"            // for npcDefList.npcDefs
#include "UI/uiReticle.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiUtil.h"
#include "graphics/FX/fx.h"
#include "UI/uiInput.h"
#include "UI/uiCompass.h"
#include "sound/sound.h"        // for sndPlay
#include "entity/entVarUpdate.h" // for INF_NPC_SAYS
#include "UI/uiEditText.h"       // for dumpFormattedTextArray
#include "UI/uiCompass.h"
#include "UI/uiChat.h" // addChatMsg
#include "entity/character_target.h" // character_targetMatchesType
#include "player/badges.h"
#include "filter/titles.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/textureatlas.h"
#include "storyarc/missionClient.h"
#include "UI/uiUtilGame.h"
#include "entity/entity.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/timing.h>
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "UI/uiPet.h"
#include "UI/uiOptions.h"
#include "demo.h"
#include "UI/uiCompass.h"
#include "UI/uiAutomap.h"
#include "edit/ClickToSource.h"
#include "UI/uiBuff.h"
#include "bases/bases.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiPlayerNote.h"
#include "group/groupnetrecv.h"
#include "UI/uiStatus.h"
#ifndef TEST_CLIENT
#include "entity/character_animfx_client.h"
#endif

static Entity *s_eAssist = NULL;
static Entity *s_eAssistTarget = NULL;
static Entity *s_eTargetTarget = NULL;
static Entity *s_eTauntTarget = NULL;

static U32 CalcColor(U32 color, U32 alpha)
{
    return (color & 0xffffff00)|(((alpha*(color&0xff))/255)&0xff);
}

//
//
static void drawBarOnly( int x, int y, int z, int curr, int total, AtlasTex* overbar, AtlasTex * icon, float scale, int iAlpha, int iColor)
{
    AtlasTex * barback = atlasLoadTexture("bar_background.tga");

    unsigned int uiColor = (iAlpha&0xff)|(iColor&~0xff);

    display_sprite(  barback, x - ( scale * ( float )( barback->width) / 2.f ),
        y, z, scale, scale, uiColor );
    display_sprite( overbar, x - ( scale * ( float )( overbar->width) / 2.f ),
        y, z, scale * ( float )curr / ( float )total, scale, uiColor );

    if ( icon )
        display_sprite( icon, x - ( scale * ( float )( barback->width) / 2.f ) - icon->width*scale, y, z, scale, scale, uiColor );
}

//
//
static void drawReticle(Entity* e, int color, int glow_color, float masterScale, int over)
{
#define RETICLE_Z       ( 30.f )
#define RETICLE_GLOW_Z  ( 29.9f )
#define DRAW( s, x, y, z, color, angle ) display_rotated_sprite( s, x, y, z, scale, scale, color, angle, 0 );

    Vec3    camToEnt;

    AtlasTex * sul;
    AtlasTex * sul_glow;

    float   scale;
    float   maxScale = 1.5;
    float   minScale = 0.6;

    if(over){
        sul = atlasLoadTexture("targeted_bracket_over.tga");
        sul_glow = atlasLoadTexture("targeted_bracket_over_glow.tga");
    }else{
        sul = atlasLoadTexture("targeted_bracket.tga");
        sul_glow = atlasLoadTexture("targeted_bracket_glow.tga");
    }

    //AtlasTex * sur = atlasLoadTexture("target_bracket_ll.tga");
    //AtlasTex * sll = atlasLoadTexture("target_bracket_ur.tga");
    //AtlasTex * slr = atlasLoadTexture("target_bracket_ul.tga");

    if(!e)
        return;

    scale = distance3(cam_info.cammat[3], ENTPOS(e));

    scale = minScale + (200.0 - scale) * (maxScale - minScale) / 200.0;

    scale *= masterScale;

    if(scale < minScale * masterScale)
        scale = minScale * masterScale;
    else if(scale > maxScale * masterScale)
        scale = maxScale * masterScale;

    subVec3(ENTPOS(e), cam_info.cammat[3], camToEnt);

    // MS: This is backwards because the cammat is backwards, will fix later.

    if(!e->noDrawOnClient && dotVec3(cam_info.cammat[2], camToEnt) <= 0)
    {
        int     w,h;
        Vec2    pix_ul, pix_lr;
        float   width = scale * sul->width;
        float   height = scale * sul->height;
        float   halfx = width / 2.0f;
        float   halfy = height / 2.0f;
        float   offset = -10 * scale;
        int     x1, x2;
        int     y1, y2;
        int        draw_bone_xhair;

        if( !seqGetSides(e->seq, ENTMAT(e), pix_ul, pix_lr, NULL, NULL) )
            return; //Should never happen -- would be malformed ent

        windowSize(&w,&h);

        draw_bone_xhair =    e->seq->type->selection == SEQ_SELECTION_BONECAPSULES &&
                            eaSize(&e->seq->type->boneCapsules) &&
                            (pix_ul[0] < 0 || pix_ul[1] > h || pix_lr[0] > w);


        if(pix_lr[0] < pix_ul[0] + halfx + 2)
        {
            pix_ul[0] = pix_lr[0] = (pix_ul[0] + pix_lr[0]) / 2;
            pix_ul[0] -= (halfx/2) + 1;
            pix_lr[0] += (halfx/2) + 1;
        }

        if(pix_lr[1] > pix_ul[1] + halfy + 2)
        {
            pix_ul[1] = pix_lr[1] = (pix_ul[1] + pix_lr[1]) / 2;
            pix_ul[1] += (halfy/2) + 1;
            pix_lr[1] -= (halfy/2) + 1;
        }

        x1 = (int)pix_ul[0];
        x2 = (int)pix_lr[0];

        y1 = (int)pix_ul[1];
        y2 = (int)pix_lr[1];

        // Upper left.
        DRAW( sul, x1 - width - offset, h - y1 - height - offset, RETICLE_Z, color, RAD(0));
        DRAW( sul_glow, x1 - width - offset, h - y1 - height - offset, RETICLE_GLOW_Z, glow_color, RAD(0));

        // Upper right.
        DRAW( sul, x2 + offset, h - y1 - height - offset, RETICLE_Z, color, RAD(90));
        DRAW( sul_glow, x2 + offset, h - y1 - height - offset, RETICLE_GLOW_Z, glow_color, RAD(90));

        // Lower left.
        DRAW( sul, x1 - width - offset, h - y2 + offset, RETICLE_Z, color, RAD(270));
        DRAW( sul_glow, x1 - width - offset, h - y2 + offset, RETICLE_GLOW_Z, glow_color, RAD(270));

        // Lower right.
        DRAW( sul, x2 + offset, h - y2 + offset, RETICLE_Z, color, RAD(180));
        DRAW( sul_glow, x2 + offset, h - y2 + offset, RETICLE_GLOW_Z, glow_color, RAD(180));

        // draw more for bone capsules
        if(draw_bone_xhair)
        {
            EntityCapsule *srcCap;
            Mat4 srcMat, srcCapMat;
            F32 dist;
            Vec3 vecHitLoc, vecCamLoc;

            srcCap = &playerPtr()->motion->capsule;
            copyMat3(unitmat, srcMat);
            copyVec3(ENTPOS(playerPtr()), srcMat[3]);
            positionCapsule(srcMat, srcCap, srcCapMat);

            dist = seqCapToBoneCap(    srcCapMat[3], srcCapMat[1], srcCap->length, srcCap->radius, e->seq,
                                    NULL, vecHitLoc, NULL);
            assert(dist >= 0.f);

            mulVecMat4(vecHitLoc, cam_info.viewmat, vecCamLoc);
            gfxWindowScreenPos(vecCamLoc, pix_ul);

            x1 = pix_ul[0];
            y1 = h - pix_ul[1];
            offset = 5.5f * scale;
            DRAW( sul, x1 - offset, y1, RETICLE_Z, color, RAD(0));
            DRAW( sul, x1 - width + offset, y1, RETICLE_Z, color, RAD(90));
            DRAW( sul_glow, x1 - offset, y1, RETICLE_GLOW_Z, glow_color, RAD(0));
            DRAW( sul_glow, x1 - width + offset, y1, RETICLE_GLOW_Z, glow_color, RAD(90));
        }
    }
}

int getReticleColor(Entity* e, int glow, int selected, int isName)
{
    Entity *player = playerPtr();
    int alpha = selected ? 0xff : 0xc0;
    int isOpponent;
    Entity *eOwner = erGetEnt(e->erOwner) ? erGetEnt(e->erOwner) : e;
    bool pvpinactive = (ENTTYPE(eOwner) == ENTTYPE_PLAYER 
                        && server_visible_state.isPvP
                        && (player->pvpClientZoneTimer || eOwner->pvpClientZoneTimer));

    if (character_IsInitialized(player->pchar) && character_IsConfused(player->pchar))
    {
        // Gets a random (per confuse) four-bit index
        U32 confusedIdx = (unsigned int)15 & (player->confuseRand ^ (e ? e->svr_idx : 0));
        // Picks team based on index-th bit of a random (per confuse)
        isOpponent = 1 & (player->confuseRand >> confusedIdx);
    }
    else
    {
         isOpponent = (!e->pchar || character_TargetIsFoe(player->pchar,e->pchar));
    }

    switch(e ? ENTTYPE(e) : ENTTYPE_PLAYER)
    {
        case ENTTYPE_CRITTER:
            if(isOpponent)
            {
                if (e->shouldIConYellow)
                    return alpha | (glow ? 0xffff0000 : 0xffff8b00);
                else if (pvpinactive)
                    return alpha | (glow ? 0xaa444400 : 0xbbbb6600);
                else
                    return alpha | (glow ? 0xff000000 : 0xffff0000);
            }
            else if (erGetEnt(e->erOwner) && isEntitySelectable(e))
            {
                if (entIsMyTeamPet(e))
                    return alpha | (glow ? 0x10fa4c00 : 0x10fab500);
                else if (entIsMyLeaguePet(e))
                    return alpha | (glow ? 0x08b6a500 : 0x08f2da00);    //    averaged team and regular
                else
                    return alpha | (glow ? 0x0072ff00 : 0x00ebff00);
            }
            else 
                return alpha | (glow ? 0x0072ff00 : 0x00ebff00);
        case ENTTYPE_MOBILEGEOMETRY:
        case ENTTYPE_MISSION_ITEM:
        case ENTTYPE_DOOR:
        case ENTTYPE_CAR:
            return alpha | (glow ? 0xb3cff200 : 0xffffff00);
        case ENTTYPE_NPC:
            return alpha | (glow ? 0xb3cff200 : 0xffffff00);
        default:
            if (e && e->pl && e->pl->afk)
            {
                // Use gray for AFK
                return alpha | (glow ? 0xaaaaaa00 : 0xdddddd00);
            }
            else if (player != e && character_IsConfused(player->pchar))
            {
                if (isOpponent) 
                    return alpha | (glow ? 0xff000000 : 0xffff0000);
                else 
                    return alpha | (glow ? 0x0072ff00 : 0x00ebff00);
            }
            else if (e && team_IsMember(playerPtr(), e->db_id) 
                       && playerPtr()->teamup->members.count > 1 )
            {
                return alpha | (glow ? 0x10fa4c00 : 0x10fab500);
            }
            else if (e && league_IsMember(playerPtr(), e->db_id)
                       && playerPtr()->league->members.count > 1 )
            {
                return alpha | (glow ? 0x08b6a500 : 0x08f2da00);    // averaged team and regular
            }
            // Only be red if you say you're not a friend, and you wouldn't have been if you were in phase
            else if (player != e && !character_TargetMatchesType(player->pchar, e->pchar, kTargetType_Friend, true))
            {
                if (pvpinactive)
                {
                    return alpha | (glow ? 0xaa444400 : 0xbbbb6600);
                }
                else
                {
                    return alpha | (glow ? 0xff000000 : 0xffff0000);
                }
            }
            else if (isName && e && e->pl && e->pl->helperStatus == 2)
            {
                // only use special helper colors in name
                return alpha | (glow ? 0xffd70000 : 0xffff2200);
            }
            else if (isName && e && e->pl && e->pl->helperStatus == 1)
            {
                // only use special helper colors in name
                return alpha | (glow ? 0xcc33cc00 : 0xdd55dd00);
            }
            else 
            {
                return alpha | (glow ? 0x0072ff00 : 0x00ebff00);
            }
    }
}



void customTarget( bool bCycle, bool bBackwards, char * str )
{
    char *args[10], *name = 0;
    int i, friendorfoe = 0, deadoralive = 0, baseornot = 0, petornot = 0, teammateornot = 0;
        
    int count = tokenize_line_safe( str, args, 10, &str );
    if( count > 10 )
        addSystemChatMsg( textStd("TooManyArguments"), INFO_USER_ERROR, 0 );

    for( i = 0; i < count; i++ )
    {
        if( commandParameterStrcmp( "enemyCommandParameter", args[i] ))
            friendorfoe++;
        else if( commandParameterStrcmp( "friendCommandParameter", args[i] ) )
            friendorfoe--;
        else if( commandParameterStrcmp( "defeatedCommandParameter", args[i] ) )
            deadoralive--;
        else if( commandParameterStrcmp( "aliveCommandParameter", args[i] ) )
            deadoralive++;
        else if( commandParameterStrcmp( "mypetCommandParameter", args[i] ) )
            petornot--;
        else if( commandParameterStrcmp( "notmypetCommandParameter", args[i] ) )
            petornot++;
        else if( commandParameterStrcmp( "baseCommandParameter", args[i] ) )
            baseornot--;
        else if( commandParameterStrcmp( "notbaseCommandParameter", args[i] ) )
            baseornot++;
        else if( commandParameterStrcmp( "teammateCommandParameter", args[i] ) )
            teammateornot--;
        else if( commandParameterStrcmp( "notteammateCommandParameter", args[i] ) )
            teammateornot++;
        else
        {
            name = args[i];
            //addSystemChatMsg( textStd("UnrecognizedCustomTarget", args[i]), INFO_USER_ERROR, 0 );
        }
    }

    selectTarget( bCycle, bBackwards, friendorfoe, deadoralive, baseornot, petornot, teammateornot, name );

}

void selectTarget( bool bCycle, bool bBackwards, int friendorfoe, int deadoralive, int basepassiveornot, int petornot, int teammateornot, char * name )
{
    int enemy_id;

    if (game_state.viewCutScene)
        return;

    if( current_target && bCycle )
        enemy_id = current_target->svr_idx;
    else
        enemy_id = 0;

    current_target = entFromId( selectNextEntity( enemy_id, bBackwards, friendorfoe, deadoralive, basepassiveornot, petornot, teammateornot, name) );

    if (entIsMyHenchman(current_target)) 
        current_pet = erGetRef(current_target);
}

static int drawPointer(Entity *e, int iID, Vec3 vecLoc, char *pchFX)
{
    Mat4 mat;
    Vec3 vec;
    Vec3 pyr;

    if(!fxIsFxAlive(iID))
    {
        FxParams fxp;

        fxInitFxParams(&fxp);
        copyMat4(unitmat, fxp.keys[0].offset);
        fxp.keycount++;
        fxp.power = 10; //# from 1 to 10
        fxp.fxtype = FX_ENTITYFX;

        iID = fxCreate(pchFX, &fxp);
    }

    copyMat4(ENTMAT(e), mat);

    subVec3(vecLoc, mat[3], vec);
    pyr[0] = 0;
    pyr[1] = getVec3Yaw(vec);
    pyr[2] = 0;
    createMat3YPR(mat, pyr);

    fxChangeWorldPosition(iID, mat);

    return iID;
}

static int killPointer(Entity *e, int iID)
{
    fxDelete(iID, SOFT_KILL);
    return 0;
}

typedef struct Jitter
{
    int x;
    int y;
} Jitter;

#define NUM_JITTERS 6
Jitter g_Jitters[NUM_JITTERS] =
{
    {   0,   0 },
    {  20,  20 },
    { -20,  25 },
    {  -5,  30 },
    {  30,   5 },
    {  10,  15 },
};

#define FLOAT_TOP 180


void formattedTextBlockDims(Array* formattedTextBlock, float fXScale, float fYScale, int *piCntLines, int *piMaxLineHeight, int *piTotalWidth)
{
    int formattedTextCursor;
    int iMaxWidth = 0;
    int iMaxHeight = 0;

    *piCntLines = 0;
    *piMaxLineHeight = 0;
    *piTotalWidth = 0;

    for(formattedTextCursor = 0; formattedTextCursor < formattedTextBlock->size; formattedTextCursor++)
    {
        int j;
        FormattedText *text = formattedTextBlock->storage[formattedTextCursor];

        for(j = 0; j<text->lines.size; j++)
        {
            int width, height;
            float fWidth, fHeight;
            FormattedLine *fl = (FormattedLine *)text->lines.storage[j];

              strDimensionsBasicWide(font_grp, fXScale, fYScale, fl->text, fl->characterCount, &width, &height, NULL);
             fWidth = width;
             fHeight = MAX(str_height(font_grp,fXScale, fYScale,0, "W"),height);
            reverseToScreenScalingFactorf(&fHeight, NULL);
            reverseToScreenScalingFactorf(&fWidth, NULL);
            iMaxHeight = iMaxHeight < height ? fHeight : iMaxHeight;
            iMaxWidth = iMaxWidth < fWidth ? fWidth : iMaxWidth;
        }

        *piCntLines += text->lines.size;

    }

    *piTotalWidth = iMaxWidth;
    *piMaxLineHeight = iMaxHeight;
}



///////////////////////////////////

#define BUBBLE_NUM_X_SLOTS   2
#define BUBBLE_NUM_Y_SLOTS  20
static bool s_abUsed[BUBBLE_NUM_X_SLOTS][BUBBLE_NUM_Y_SLOTS];

static void PutInUnobstructedSpot(int *piX, int *piY)
{
    int w;
    int h;
    int idxX;
    int idxXOrig;
    int idxY;
    int idxYOrig;

    // This has some extremely preliminary stuff in it towards some kind of
    // magical non-overlapping bubble alogrithm. That's really just an excuse
    // for odd it looks. Do what thou wilt.

    windowSize(&w, &h); // TODO: Maybe yank out and do only once each frame

    if(*piX>=w) *piX=(w-1);
    if(*piY>=h) *piY=(h-1);

    idxXOrig = idxX = *piX*BUBBLE_NUM_X_SLOTS/w;
    idxYOrig = idxY = *piY*BUBBLE_NUM_Y_SLOTS/h;

    while(s_abUsed[idxX][idxY] && idxY>=0)
    {
        idxY--;
    }
    while(s_abUsed[idxX][idxY] && idxY<BUBBLE_NUM_Y_SLOTS)
    {
       idxY++;
    }

    if(!s_abUsed[idxX][idxY])
    {
        s_abUsed[idxX][idxY]=true;

        if(*piX==0 || *piX==w-1)
        {
            *piY = idxY*h/BUBBLE_NUM_Y_SLOTS+h/BUBBLE_NUM_Y_SLOTS/2;
        }
    }
}

static bool CalcTops(Entity *e, int *xp, int *yp, int *xp_stable, int *yp_stable, float *z, float *scale )
{
    Vec2 pix_ul, pix_lr;
    Vec3 steady_top;
    Vec2 pix_steady_top;
    int  w, h;
    float fDist;
    bool bRet = true;

    if(!e->seq )
        return false;

    // DANGER! All sorts of arbitrary numbers ahead.

    seqGetSteadyTop( e->seq, steady_top );
    gfxWindowScreenPos( steady_top, pix_steady_top );
    //pix_steady_top == screen coords of top of ent's collision box

    if(!seqGetSides( e->seq, ENTMAT(e), pix_ul, pix_lr, NULL, NULL ))
    {
        // The character has no graphics at all -- something is wrong with him
        copyVec2(pix_steady_top, pix_ul);
        copyVec2(pix_steady_top, pix_lr);

        bRet = false;
    }

    windowSize(&w, &h); // TODO: Maybe yank out and do only once each frame

    if(    bRet &&
        e->seq->type->selection == SEQ_SELECTION_BONECAPSULES &&
        eaSize(&e->seq->type->boneCapsules) &&
        (pix_ul[0] < 0 || pix_ul[1] > h || pix_lr[0] > w) )
    {
        EntityCapsule *srcCap;
        Mat4 srcMat, srcCapMat;
        F32 dist;
        Vec3 vecHitLoc, vecCamLoc;

        srcCap = &playerPtr()->motion->capsule;
        copyMat3(unitmat, srcMat);
        copyVec3(ENTPOS(playerPtr()), srcMat[3]);
        positionCapsule(srcMat, srcCap, srcCapMat);

        dist = seqCapToBoneCap(    srcCapMat[3], srcCapMat[1], srcCap->length, srcCap->radius, e->seq,
                                NULL, vecHitLoc, NULL);
        assert(dist >= 0.f);

        mulVecMat4(vecHitLoc, cam_info.viewmat, vecCamLoc);
        gfxWindowScreenPos(vecCamLoc, pix_ul);
        gfxWindowScreenPos(vecCamLoc, pix_lr);
    }


    *xp = ceil(( pix_ul[0] + pix_lr[0] ) / 2);        //center x
    *yp = ceil(( h - pix_ul[1] ) - 20);                //y moved up a little
    *xp_stable = ceil(pix_steady_top[0]);
    *yp_stable = ceil(h - pix_steady_top[1] - 15);  //
    *z = ceil(steady_top[2]);

    fDist = distance3(ENTPOS(e), cam_info.cammat[3] ); //mw changed this for cutscene

    if(fDist > -steady_top[2])
    {
        *scale = 1.7f / (1 + fDist/50.0f);
    }
    else
    {
        *scale = 1.7f / (1 + -steady_top[2]/50.0f);
    }

    if(*scale > 1.7f)
        *scale = 1.7f;
    else if(*scale < 0.8f)
        *scale = 0.8f;

    if( bRet
        && !e->noDrawOnClient
        && !( e->seq->gfx_root->flags & GFXNODE_HIDE )
        //&& *xp_stable>=0 && *xp_stable<=w
        //&& *yp_stable>=0 && *yp_stable<=h
        && *z<=0)
    {
        return true;
    }

    return false;
}

typedef struct BubbleToDraw
{
    //Floater from whence this came
    Floater * floaterInfo;

    //drawChatBubbleParams
    EBubbleType type;
    EBubbleLocation location;
    char * pch;
    int x;
    int y;
    int z;
    float scale;
    int colorFG;
    int colorBG;
    int colorBorder;

    //My Params

} BubbleToDraw;




#define BUBBLE_MAX_WIDTH      250
#define BUBBLE_WIDTH_BORDER    6
#define BUBBLE_HEIGHT_BORDER   6
#define BUBBLE_LEADING        (0.5)

int drawChatBubble(EBubbleType type, EBubbleLocation location, char *pch, int x, int y, int z, float scale, int colorFG, int colorBG, int colorBorder, Vec3 headScreenPos)
{
    unsigned int uiNeededSize;
    float sc = 1.f;
    int lines;
    int width;
    CBox box = { 0 };
    int line_height;
    int iTailOverlap = 3;
    float fHeightBubble, fWidthBubble;
    int iXTail, iYTail;
    float fXBubble, fYBubble;
    float fAngle;
    bool bOrigItal = chatThin.renderParams.italicize;
    F32 fContinuationOffset = 0; 

    static Array *s_text = NULL;
    static char *s_pchBuff = NULL;
    static unsigned int s_uiSize = 0;

    AtlasTex *pTexTail = NULL;

    if(pch==NULL)  
    {
        return y;
    }

    // First figure out how much space the text will take up.
    uiNeededSize = msPrintf(menuMessages, NULL, 0, "%s", pch);
    if(uiNeededSize>s_uiSize)
    {
        s_uiSize = max(uiNeededSize*2, 512);
        s_pchBuff = realloc(s_pchBuff, s_uiSize);
    }
    msPrintf(menuMessages, s_pchBuff, s_uiSize, "%s", pch);

    if(!s_text)
    {
        s_text = createArray();
    }
    clearArrayEx(s_text, psudeoDestroyFormattedText);

    font(&chatThin);
    font_color(colorFG, colorFG);

    //str_dims(font_grp, 1.f, 1.f, FALSE, &box, "%s", s_pchBuff);
    width = str_wd_notranslate(font_grp, 1.f, 1.f, s_pchBuff );//(box.hx - box.lx);
    width = min(BUBBLE_MAX_WIDTH, width);

    formatTextBlocks(s_text, font_grp, 1.0f, 1.0f, width+1, NULL, &s_pchBuff, 1, 0);
    formattedTextBlockDims(s_text, 1.0f, 1.0f, &lines, &line_height, &width);
    width *= scale;
    line_height *= scale;

    line_height *= BUBBLE_LEADING+1; 

    // Calculate the size of the chat bubble
    fHeightBubble = lines*line_height+BUBBLE_HEIGHT_BORDER*2*scale;
    fWidthBubble = width + BUBBLE_WIDTH_BORDER*2*scale;


    // The given x, y value is the origin of the bubble and the tail. This
    // origin may be clamped to any side of the screen or hover over
    // someone's head.
    switch(location)
    {
    case kBubbleLocation_Left:
        pTexTail = atlasLoadTexture("chat_side_tail.tga");
        iXTail = 0;
        iYTail = pTexTail->height*scale/2;
        fXBubble = -(pTexTail->width-iTailOverlap)*scale;
        fYBubble = fHeightBubble/2;
        fAngle = PI;
        break;
    case kBubbleLocation_Right:
        pTexTail = atlasLoadTexture("chat_side_tail.tga");
        iXTail = pTexTail->width*scale;
        iYTail = pTexTail->height*scale/2;
        fXBubble = (pTexTail->width-iTailOverlap)*scale + fWidthBubble;
        fYBubble = fHeightBubble/2;
        fAngle = 0;
        break;
    case kBubbleLocation_Bottom:
        pTexTail = atlasLoadTexture("chat_side_tail.tga");
        iXTail = pTexTail->width*scale/2;
        iYTail = pTexTail->height*scale;
        fXBubble = fWidthBubble/2;
        fYBubble = (pTexTail->height-iTailOverlap)*scale + fHeightBubble;
        fAngle = PI/2;
        break;
    case kBubbleLocation_Top:
        pTexTail = atlasLoadTexture("chat_side_tail.tga");
        iXTail = pTexTail->width*scale/2;
        iYTail = y;
        fXBubble = fWidthBubble/2;
        fYBubble = -(pTexTail->height-iTailOverlap)*scale;
        fAngle = 3*PI/2;
        break;
    case kBubbleLocation_Somewhere:
        pTexTail = NULL;
        iXTail = 0;
        iYTail = 0;
        fXBubble = fWidthBubble/2;
        fYBubble = fHeightBubble;
        fAngle = 0;
        break;

    case kBubbleLocation_Above:
    default: 
        switch(type)
        {
        case kBubbleType_Chat:
            pTexTail = atlasLoadTexture("chat_bubble_tail.tga");
            break;

        case kBubbleType_ChatContinuation: 
            pTexTail = atlasLoadTexture("chat_continuation_tail.tga"); 
            //y+=(iTailOverlap+1)*scale;
            fContinuationOffset = (iTailOverlap+1)*scale;
            break;

        case kBubbleType_EmoteContinuation:
            font_grp->renderParams.italicize = 1;
            pTexTail = atlasLoadTexture("chat_continuation_tail.tga");
            break;

        case kBubbleType_Emote:
            font_grp->renderParams.italicize = 1;
            pTexTail = atlasLoadTexture("chat_emote_tail.tga");
            break;

        case kBubbleType_Private:
            pTexTail = atlasLoadTexture("chat_private_tail.tga");
            break;
        }
 
        fAngle = 0;
        iXTail = pTexTail->width*scale/2;
        iYTail = pTexTail->height*scale - fContinuationOffset;
        fXBubble = fWidthBubble/2;      
        fYBubble = (pTexTail->height-iTailOverlap-1)*scale + fHeightBubble - fContinuationOffset;
        break;
    }

    //This keeps the text from drifting off the top or bottom of the screen (preliminary)
    if( location == kBubbleLocation_Above && 
        type != kBubbleType_ChatContinuation &&
        type != kBubbleType_EmoteContinuation )
    {
        int w,h;
        windowSize(&w, &h); 

        if( y > h + (iYTail) ) 
            y = h + (iYTail) ;
  
        if( y < fHeightBubble + (iYTail) ) 
            y = fHeightBubble + (iYTail); 

        //Remove the tail if the head is not visible
        if( h - headScreenPos[1] < fHeightBubble + (iYTail) )
            pTexTail = 0;

        //if( x < fWidthBubble/2 ) 
        //    x = fWidthBubble/2; 

        //if( x > w - fWidthBubble/2 )  
        //    x = w - fWidthBubble/2; 
    }
    //*/

    iXTail = x - iXTail;
    iYTail = y - iYTail; 

    fXBubble = x - fXBubble; 
    fYBubble = y - fYBubble;

    if (type==kBubbleType_Caption)
    {
        int w,h;
        CBox box;

        windowSize(&w, &h); 

        if (fXBubble < 0.0)
            fXBubble = 0.0;
        else if (fXBubble > w - fWidthBubble - 1)
            fXBubble = w - fWidthBubble - 1;

        if (fYBubble < 0.0)
            fYBubble = 0.0;
        else if (fYBubble > w - fHeightBubble - 1)
            fYBubble = w - fHeightBubble - 1;

        BuildCBox(&box, fXBubble, fYBubble, fWidthBubble, fHeightBubble);
        drawOutlinedBox(&box, z, colorBorder, colorBG, PIX2);
    }
    else
    {
        drawFrame(PIX2, R4, fXBubble, fYBubble, z, fWidthBubble, fHeightBubble,
            1.f, colorBorder, colorBG);
    }

    if(pTexTail)
    {
        display_rotated_sprite(pTexTail,
            iXTail, iYTail, z,
            scale, scale, colorBG,
            fAngle, 0);
    }

    dumpFormattedTextArray(fXBubble+BUBBLE_WIDTH_BORDER*scale, // don't know why -2
        fYBubble+line_height+(BUBBLE_HEIGHT_BORDER-2)*scale,
        z,
        scale, scale,
        1000, line_height/scale,
        s_text, &colorFG, true, 0, 0, 0, 0);

    font_grp->renderParams.italicize = bOrigItal;

    return fYBubble;
}


static void addToChatBubbleBucket( Floater * floater, int iCntChat, int xp_stable, int yp_stable, int z, F32 scale, bool bDraw, U8 currfade, Vec3 headScreenPos )
{
    EBubbleType type;
    U32 clrfg;
    U32 clrbg;
    U32 clrborder;
    int iAlpha = 0xff;
    int bubbleX;
    int bubbleY;
    EBubbleLocation loc;
    static int iYOfPrevBubble;
    static int iTimerOfPrevBubble;

    ///////// Set up: Set Color, Alpha, and BubbleType 
    if(floater->fTimer<10.0f)
    {
        iAlpha = (int)(0xff*floater->fTimer/10.0f);
    }
    else if(floater->fTimer+30.0f > floater->fMaxTimer)
    {
        iAlpha = (int)(0xff*(floater->fMaxTimer-floater->fTimer)/30.0f);
    }
    clrfg     = CalcColor(floater->colorFG, iAlpha);
    clrbg     = CalcColor(floater->colorBG, iAlpha);
    clrborder = CalcColor(floater->colorBorder, iAlpha);

    type = kBubbleType_Chat;

    if (floater->eStyle==kFloaterStyle_Caption)
        type = kBubbleType_Caption;
    else if(iCntChat>0)
    {
        if(floater->eStyle==kFloaterStyle_Emote)
            type = kBubbleType_EmoteContinuation;
        else
            type = kBubbleType_ChatContinuation;
    }
    else
    {
        if(floater->eStyle==kFloaterStyle_Chat_Private)
            type = kBubbleType_Private;
        else if(floater->eStyle==kFloaterStyle_Emote)
            type = kBubbleType_Emote;
    }
    
    ///////////////////////////////////////////////////
    //Figure out where and how to draw it

    //If the speaker is drawn
    if( bDraw )
    {
        int iY;

        if(iCntChat>0)
        {
            // This ugly if stuff is to slide the old bubble up.
            if(iTimerOfPrevBubble<10.0f)
            {
                int ii = yp_stable - 32*scale; // HACK: 32 is the height of the previous tail.
                iY = ii + (iYOfPrevBubble-ii) * iTimerOfPrevBubble/10.0f;
            }
            else
            {
                iY = iYOfPrevBubble;
            }
        }
        else
        {
            iY = yp_stable;
        }

        // If they're faded out (imperceptible)
        if (!currfade)
        {
            // If we haven't already picked a position for this one
            if (floater->iXLast == -1)
            {
                int w,h;
                windowSize(&w, &h);
                xp_stable = (w<BUBBLE_MAX_WIDTH*2)?w/2:randInt(w-BUBBLE_MAX_WIDTH*2) + BUBBLE_MAX_WIDTH;
                iY = randInt(0.8*h);
            }
            else 
            {
                xp_stable = floater->iXLast;
                iY = floater->iYLast;
            }
            // Try to position it
            PutInUnobstructedSpot(&xp_stable,&iY);
            z = -30;
            scale = 1;
        }
        
        loc = (currfade)?kBubbleLocation_Above:kBubbleLocation_Somewhere;

        bubbleX = xp_stable;
        bubbleY = iY;

        floater->iXLast = xp_stable;
        floater->iYLast = iY;
    }
    else  /////////////////If the speaker is not drawn
    {
        int w, h;

        windowSize(&w, &h); // TODO: Maybe yank out and do only once each frame

        // If they're faded out (imperceptible)
        if(!currfade)
        {
            // If we haven't already picked a position for this one
            if (floater->iXLast == -1)
            {
                xp_stable = (w<BUBBLE_MAX_WIDTH*2)?w/2:randInt(w-BUBBLE_MAX_WIDTH*2) + BUBBLE_MAX_WIDTH;
                yp_stable = randInt(0.8*h);
            }
            else 
            {
                xp_stable = floater->iXLast;
                yp_stable = floater->iYLast;
            }
            // Try to position it
            PutInUnobstructedSpot(&xp_stable,&yp_stable);
            z = -30;
            scale = 1;
        }

        if(xp_stable>=0 && xp_stable<w && yp_stable>=0 && yp_stable<h && z<0)
        {
            // It's in the window, but not visible to us for some reason.
            // Get rid of the tail, and leave it in one spot.
            floater->iXLast=xp_stable;
            floater->iYLast=yp_stable;

            loc = kBubbleLocation_Somewhere;
        }
        else
        {
            if(z>0)
            {
                z = -z;
                floater->iYLast = floater->iYLast<=0 || floater->iYLast>=h ? h/2 : floater->iYLast;
                if(xp_stable>w/2)
                {
                    floater->iXLast = 0;
                    loc = kBubbleLocation_Left;
                }
                else if(xp_stable<=w/2)
                {
                    floater->iXLast = w;
                    loc = kBubbleLocation_Right;
                }
                else
                {
                    floater->iXLast = floater->iXLast<=0 || floater->iXLast>=w  ? w/2 : floater->iXLast;
                    if(yp_stable>h/2)
                    {
                        floater->iYLast = 0;
                        loc = kBubbleLocation_Top;
                    }
                    else if(yp_stable<=h/2)
                    {
                        floater->iYLast = h;
                        loc = kBubbleLocation_Bottom;
                    }
                }
            }
            else
            {
                floater->iYLast = floater->iYLast<=0 || floater->iYLast>=h ? h/2 : floater->iYLast;
                if(xp_stable>w/2)
                {
                    floater->iXLast = w;
                    loc = kBubbleLocation_Right;
                }
                else if(xp_stable<=w/2)
                {
                    floater->iXLast = 0;
                    loc = kBubbleLocation_Left;
                }
                else
                {
                    floater->iXLast = floater->iXLast<=0 || floater->iXLast>=w  ? w/2 : floater->iXLast;
                    if(yp_stable>h/2)
                    {
                        floater->iYLast = h;
                        loc = kBubbleLocation_Bottom;
                    }
                    else if(yp_stable<=h/2)
                    {
                        floater->iYLast = 0;
                        loc = kBubbleLocation_Top;
                    }
                }
            }
        }

        PutInUnobstructedSpot(&floater->iXLast, &floater->iYLast);

        bubbleX = floater->iXLast;
        bubbleY = floater->iYLast;
    }

    if(!demoIsPlaying() || !demo_state.demo_hide_chat)
    {
        iYOfPrevBubble = drawChatBubble(type, loc, floater->ach, bubbleX, bubbleY, z, floater->fScale*scale, clrfg, clrbg, clrborder, headScreenPos);
    }

    //MILD HACK iYOfPrevBubble and this Store these two statics for the next float to come through in case it is an older continuation that needs to know where this one is
    iTimerOfPrevBubble = floater->fTimer;
}


void drawFloatersOnEntity(Entity *e, bool bDraw, bool bForceBubbles)
{
    int inPhase = entity_TargetIsInVisionPhase(playerPtr(), e);
    int xp;            //Unused
    int yp;            //Unused
    int e_xp_stable;    //point in screen space just over e's head 
    int e_yp_stable;  //
    float z;        //distance from camera
    float scale;   //Amount to scale bubble based on z distance
    int i;
    float fLastTime = -10.0; //For Damage floaters
    int iDupIdx = 0;   //For Damage floaters
    int iCntChat = 0;  //Chat bubbles over your head

    if(demoIsPlaying() && demo_state.demo_hide_damage){
        return;
    }

    // Always CalcTops since we might be forcing
    bDraw = CalcTops(e, &xp, &yp, &e_xp_stable, &e_yp_stable, &z, &scale) && bDraw;

    bForceBubbles = bForceBubbles || (!e->currfade);

    for(i=e->iNumFloaters-1; i>=0; i--)
    {
         bool bRemoveBecauseDead = 0;
        // entMode(e, ENT_DEAD)
        //    && e->aFloaters[i].eStyle!=kFloaterStyle_Damage
        //    && e->aFloaters[i].eStyle!=kFloaterStyle_DeathRattle;

        int xp_stable,yp_stable;

        if(e->aFloaters[i].bUseLoc)
        {
            int w, h;
            windowSize(&w, &h);

            if (e->aFloaters[i].bScreenLoc)
            {
                xp_stable = e->aFloaters[i].vecLocation[0] * (w - 1);
                yp_stable = e->aFloaters[i].vecLocation[1] * (h - 1);
            }
            else
            {
                Vec3 camloc;
                Vec2 floc;
                mulVecMat4( e->aFloaters[i].vecLocation, cam_info.viewmat, camloc);
                gfxWindowScreenPos( camloc, floc );
                bDraw = 1;
                xp_stable = floc[0];
                yp_stable = h-floc[1];
            }

        }
        else
        {
            xp_stable = e_xp_stable;
            yp_stable = e_yp_stable;
        }

        e->aFloaters[i].fTimer += TIMESTEP;
        if(bRemoveBecauseDead || e->aFloaters[i].fTimer >= e->aFloaters[i].fMaxTimer)
        {
            memmove(&e->aFloaters[i], &e->aFloaters[i+1], sizeof(Floater)*(e->iNumFloaters-i-1));
            e->iNumFloaters--;
        }
        else if(e->aFloaters[i].eStyle==kFloaterStyle_Caption && playerPtr()==e)
        {
            Vec3 headScreenPosDummy;
            addToChatBubbleBucket( &e->aFloaters[i], iCntChat, xp_stable, 
                yp_stable, -1, 1.0, false, e->currfade, headScreenPosDummy ); 
        }
        else if(bDraw)
        {
            if(e->aFloaters[i].fTimer>=0.0f)
            {
                switch(e->aFloaters[i].eStyle)
                {
                    default:
                    case kFloaterStyle_Damage:
                    {
                        if(fLastTime>=0 && fabs(fLastTime-e->aFloaters[i].fTimer)<7 && iDupIdx<NUM_JITTERS)
                        {
                            iDupIdx++;
                        }
                        else
                        {
                            iDupIdx = 0;
                        }

                        if (inPhase)
                        {
                            U32 clr = (e->aFloaters[i].colorFG & 0xffffff00) | (0xff-(int)(e->aFloaters[i].fTimer*0xff/e->aFloaters[i].fMaxTimer));
                            font(&title_9);
                            font_color(clr, clr);

                            cprnt(xp_stable + g_Jitters[iDupIdx].x*scale*.90,
                                yp_stable - (e->aFloaters[i].fTimer*FLOAT_TOP/MAX_FLOAT_DMG_TIME) + g_Jitters[iDupIdx].y*scale*.75,
                                (e->aFloaters[i].colorFG == 0xa0a0a000 ? z-1 : z), // HACK: push grey spew to the back
                                e->aFloaters[i].fScale*scale, e->aFloaters[i].fScale*scale,
                                e->aFloaters[i].ach);
                        }

                        fLastTime = e->aFloaters[i].fTimer;
                    }
                    break;

                    case kFloaterStyle_Chat:
                    case kFloaterStyle_Chat_Private:
                    case kFloaterStyle_Emote:
                    case kFloaterStyle_DeathRattle:
                        if (inPhase)
                        {
                            if(iCntChat>1) 
                            {
                                // Get rid of excess (more than two) chat messages.
                                memmove(&e->aFloaters[i], &e->aFloaters[i+1], sizeof(Floater)*(e->iNumFloaters-i-1));
                                e->iNumFloaters--;
                            }
                            else
                            {
                                //If this is the older of two bubbles on an entity
                                Vec3 headScreenPos;
                                GetHeadScreenPos( e->seq, headScreenPos );
                                addToChatBubbleBucket( &e->aFloaters[i], iCntChat, xp_stable, yp_stable, z, scale, bDraw, e->currfade, headScreenPos ); 
                                iCntChat++;
                            }
                        }
                        break;

                    case kFloaterStyle_Icon:
                        {
                            AtlasTex *pTex = atlasLoadTexture(e->aFloaters[i].ach);
                            if(pTex!=NULL && inPhase)
                            {
                                int iAlpha = 0xff;
                                if(e->aFloaters[i].fTimer<10.0f)
                                {
                                    iAlpha = (int)(0xff*e->aFloaters[i].fTimer/10.0f);
                                }
                                else if(e->aFloaters[i].fTimer+30.0f > e->aFloaters[i].fMaxTimer)
                                {
                                    iAlpha = (int)(0xff*(e->aFloaters[i].fMaxTimer-e->aFloaters[i].fTimer)/30.0f);
                                }
                                display_sprite(pTex,
                                    xp_stable-pTex->width*scale*1.5/2, yp_stable-pTex->height*scale*1.5, z,
                                    scale*1.5, scale*1.5,
                                    0xffffff00 | iAlpha);
                            }
                        }
                        break;
                }
            }
        }
        else if(bForceBubbles && inPhase) 
        {
            if((e->aFloaters[i].eStyle==kFloaterStyle_Chat
                || e->aFloaters[i].eStyle==kFloaterStyle_Chat_Private
                || e->aFloaters[i].eStyle==kFloaterStyle_DeathRattle)
                && e->aFloaters[i].fTimer>=0.0f)
            {
                if(iCntChat>0)
                {
                    // Get rid of excess (more than one) chat messages.
                    memmove(&e->aFloaters[i], &e->aFloaters[i+1], sizeof(Floater)*(e->iNumFloaters-i-1));
                    e->iNumFloaters--;
                }
                else
                {
                    Vec3 headScreenPos;
                    GetHeadScreenPos( e->seq, headScreenPos );
                    addToChatBubbleBucket( &e->aFloaters[i], iCntChat, xp_stable, yp_stable, z, scale, bDraw, e->currfade, headScreenPos ); 
                    iCntChat++;
                }
            }
        }
    }
}

AtlasTex* uiGetArchetypeIconByName(const char* archetype)
{
    char ach[256];
    char *pch;
    AtlasTex* tex = NULL;

    if(!archetype)
        return NULL;

    strcpy(ach, "v_archetypeicon_");
    pch = strchr(archetype, '_');
    if(pch)
        strcat(ach, pch+1);
    else
        strcat(ach, archetype);

    tex = atlasLoadTexture(ach);

    if( !tex || stricmp(tex->name, "white") == 0 )
    {
        strcpy(ach, "archetypeicon_");
        pch = strchr(archetype, '_');
        if(pch)
            strcat(ach, pch+1);
        else
            strcat(ach, archetype);
        return atlasLoadTexture(ach);
    }

    return tex;
}

AtlasTex* uiGetOriginTypeIconByName(const char* origin)
{
    char ach[256];
    strcpy(ach, "originicon_");
    strcat(ach, origin);
    strcat(ach, ".tga");
    return atlasLoadTexture(ach);
}

AtlasTex* uiGetAlignmentIconByType(int alignmentType)
{
//    char ach[256];
//    strcpy(ach, "originicon_");
//    strcat(ach, origin);
//    strcat(ach, ".tga");
//    return atlasLoadTexture(ach);
    return white_tex_atlas;
}

AtlasTex *GetArchetypeIcon(Entity *e)
{

    if(ENTTYPE(e)==ENTTYPE_PLAYER)
    {
        if (e->pchar && e->pchar->pclass)
            return uiGetArchetypeIconByName(e->pchar->pclass->pchName);
    }

    return white_tex_atlas;
}

AtlasTex *GetOriginIcon(Entity *e)
{
    if(ENTTYPE(e)==ENTTYPE_PLAYER)
    {
        if (e->pchar && e->pchar->porigin)
             return uiGetOriginTypeIconByName(e->pchar->porigin->pchName);
    }

    return white_tex_atlas;
}

AtlasTex *GetAlignmentIcon(Entity *e)
{
    if(ENTTYPE(e)==ENTTYPE_PLAYER)
    {
        if (e->pl)
        {
            return uiGetAlignmentIconByType(getEntAlignment(e));
        }
    }

    return white_tex_atlas;
}

static bool IsShown(Entity *e, Show pshow)
{
    if(e && !e->currfade==0 && pshow
        && ((pshow & kShow_Always)
        || (pshow & kShow_OnMouseOver && e==current_target_under_mouse)
        || (pshow & kShow_OnMouseOver && e->alwaysCon)
        || (pshow & kShow_Selected && e==current_target)
        || (pshow & kShow_Selected && e==s_eAssist)
        || (pshow & kShow_Selected && e==s_eAssistTarget)
        || (pshow & kShow_Selected && e==s_eTargetTarget)))
    {
        return true;
    }

    return false;
}

#define CURRENT_OVERHEAD_ICON_STATE_WILL_HAVE_TASK 1
#define CURRENT_OVERHEAD_ICON_STATE_HAS_TASK 2
#define CURRENT_OVERHEAD_ICON_STATE_HAS_COMPLETED_TASK 3
#define CURRENT_OVERHEAD_ICON_STATE_ON_CURRENT_TASK 4
#define CURRENT_OVERHEAD_ICON_STATE_NONE 0

// copy/pasted from compass_Draw3dBlip().
static void drawContactOverheadIcon(Entity *contact)
{
#ifndef TEST_CLIENT
//    Destination dest = status->location;
    ContactStatus *status = 0;
    static char *hasNewTask = "UI/Icon_MissionAvailable.fx";
    static char *onCurrentTask = "UI/Icon_MissionInProgress.fx";
    static char *hasCompletedTask = "UI/Icon_MissionCompleted.fx";
    int contactIndex, contactCount, displayWillHaveTask = 0;

    contactCount = eaSize(&contacts);
    for (contactIndex = 0; contactIndex < contactCount; contactIndex++)
    {
        if (contacts[contactIndex]->handle == contact->contactHandle)
        {
            status = contacts[contactIndex];
            break;
        }
    }

    if (!status)
    {
        contactCount = eaSize(&contactsAccessibleNow);
        for (contactIndex = 0; contactIndex < contactCount; contactIndex++)
        {
            if (contactsAccessibleNow[contactIndex]->handle == contact->contactHandle)
            {
                status = contactsAccessibleNow[contactIndex];
                break;
            }
        }
    }

    if (!status)
    {
        entity_KillClientsideMaintainedFX(contact, contact, hasNewTask);
        entity_KillClientsideMaintainedFX(contact, contact, hasCompletedTask);
        entity_KillClientsideMaintainedFX(contact, contact, onCurrentTask);
    }
    else if (status->hasTask)
    {
        if (status->currentOverheadIconState != CURRENT_OVERHEAD_ICON_STATE_HAS_TASK)
        {
            entity_KillClientsideMaintainedFX(contact, contact, hasNewTask);
            entity_KillClientsideMaintainedFX(contact, contact, hasCompletedTask);
            entity_KillClientsideMaintainedFX(contact, contact, onCurrentTask);

            entity_PlayClientsideMaintainedFX(contact, contact, hasNewTask, colorPairNone, 0, 0, 0.0f, PLAYFX_NO_TINT);
            status->currentOverheadIconState = CURRENT_OVERHEAD_ICON_STATE_HAS_TASK;
        }
    }
    else if (status->notifyPlayer)
    {
        if (status->currentOverheadIconState != CURRENT_OVERHEAD_ICON_STATE_HAS_COMPLETED_TASK)
        {
            entity_KillClientsideMaintainedFX(contact, contact, hasNewTask);
            entity_KillClientsideMaintainedFX(contact, contact, hasCompletedTask);
            entity_KillClientsideMaintainedFX(contact, contact, onCurrentTask);

            entity_PlayClientsideMaintainedFX(contact, contact, hasCompletedTask, colorPairNone, 0, 0, 0.0f, PLAYFX_NO_TINT);
            status->currentOverheadIconState = CURRENT_OVERHEAD_ICON_STATE_HAS_COMPLETED_TASK;
        }
    }
    else if (status->taskcontext)
    {
        if (status->currentOverheadIconState != CURRENT_OVERHEAD_ICON_STATE_ON_CURRENT_TASK)
        {
            entity_KillClientsideMaintainedFX(contact, contact, hasNewTask);
            entity_KillClientsideMaintainedFX(contact, contact, hasCompletedTask);
            entity_KillClientsideMaintainedFX(contact, contact, onCurrentTask);

            entity_PlayClientsideMaintainedFX(contact, contact, onCurrentTask, colorPairNone, 0, 0, 0.0f, PLAYFX_NO_TINT);
            status->currentOverheadIconState = CURRENT_OVERHEAD_ICON_STATE_ON_CURRENT_TASK;
        }
    }
    else
    {
        // always kill overhead icons, for safety's sake in case the state gets weird
        entity_KillClientsideMaintainedFX(contact, contact, hasNewTask);
        entity_KillClientsideMaintainedFX(contact, contact, hasCompletedTask);
        entity_KillClientsideMaintainedFX(contact, contact, onCurrentTask);

        status->currentOverheadIconState = CURRENT_OVERHEAD_ICON_STATE_NONE;
    }

    if (status)
    {
        assignContactIcon(status);
    }

#endif
}

#define AICON_SIZE (15.0f)
static void drawStuffOnEntity(Entity *e, float fScale, int iAlpha, bool bDrawBars)
{
    int xp; //Unused
    int yp;    //Unused
    int xp_stable; //
    int yp_stable;
    float z;
    float scale;
    Entity *pentOwner = erGetEnt(e->erOwner) ? erGetEnt(e->erOwner) : e;
    int pvpTimer = server_visible_state.isPvP ? MAX(0,1+pentOwner->pvpClientZoneTimer - timerSeconds(timerCpuTicks() - pentOwner->pvpUpdateTime)) : 0;
    int pulsecolor = 0;
    static float pulsefloat = 0;
    float pulseper;

    pulsefloat += 0.07*TIMESTEP;
    pulseper = (sinf(pulsefloat)+1)/2;

     interp_rgba(pulseper,0xff4444ff,0x88222222,&pulsecolor);

    if (e == playerPtr()) pvpTimer = 0;

    // Fade out the entity's attached UI parts if they are faded.
    if (e && e->currfade != 255)
        iAlpha = e->currfade;

    iAlpha = 0xff & iAlpha;
    
    if((demoIsPlaying() && demo_state.demo_hide_all_entity_ui) || game_state.viewCutScene)
        return;
    
    if(CalcTops(e, &xp, &yp, &xp_stable, &yp_stable, &z, &scale))
    {
        static AtlasTex * barH;
        static AtlasTex * barE;
        static AtlasTex * barI;

        if (!barH)
        {
            barH = atlasLoadTexture( "BAR_HEALTH.tga" );
            barE = atlasLoadTexture( "BAR_ENDURANCE.tga" );
            barI = atlasLoadTexture( "BAR_GRAY.tga" );
        }
        fScale *= scale;

        font(&entityNameText);

        if( e->contactOrPnpc )
             font_color( 0xccff4400|iAlpha, 0x44ff4400|iAlpha );
        else
            font_color(getReticleColor(e, 0, 1, 1) & 0xffffff00 | iAlpha,
                        getReticleColor(e, 1, 1, 1) & 0xffffff00 | iAlpha);

         z += 1;

        switch(ENTTYPE(e))
        {
            case ENTTYPE_CRITTER:
                {
                    if(IsShown(e, optionGet(kUO_ShowVillainName)) || isPetDisplayed( e ) || (IsShown(e, optionGet(kUO_ShowPlayerName)) && entIsMyPet(e)))
                    {
                        int yoff = 0;
                        char buf[256];
                        // show display_name if we have one for this entity (server set non-default name)
                        char * crittername = (e->name[0] ? e->name : npcDefList.npcDefs[e->npcIndex]->displayName);
                        int color1, color2;

                        if (entIsPlayerPet(e))
                        {
                            int yy = yp-5*fScale*.75;
                            yoff = -8 * fScale;
                            color1 = getReticleColor(e, 0, 1, 1) & 0xffffff00 | iAlpha;
                            color2 = getReticleColor(e, 1, 1, 1) & 0xffffff00 | iAlpha;                    
                            strncpyt(buf, pentOwner->name,255);
                            if ((!optionGet(kUO_DisablePetNames) || entIsMyPet(e)) && e->petName && e->petName[0] != '\0' )
                            {
                                crittername = e->petName;
                            }
                            if(buf[0]!='\0')
                            {
                                int width = str_wd(font_grp, fScale*.75, fScale*.75, buf);
                                if(!demoIsPlaying() || !demo_state.demo_hide_names)
                                {
                                    cprntEx(xp, yy, z, fScale*.75, fScale*.75, NO_MSPRINT|CENTER_X,buf);
                                }
                            }
                        }
                        else
                        {
                            color1 = color2 = getConningColor(playerPtr(), e) & 0xffffff00 | iAlpha;
                             drawConningArrows(playerPtr(), e, xp, yp-5*scale, z, scale*0.75f, color1);
                        }

                        font_color(color1, color2);

                        if (pvpTimer)
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "NameDisplayWithStatus", crittername, "StatusPvPActive",pvpTimer);
                        }
                        else
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "NameDisplay", crittername);
                        }
 
                        if(!demoIsPlaying() || !demo_state.demo_hide_names)
                        {
                            cprntEx( xp, yp+yoff-5*fScale, z, fScale, fScale, NO_MSPRINT|CENTER_X,buf );
                        }
                    }                    
                    bDrawBars = bDrawBars && IsShown(e, optionGet(kUO_ShowVillainBars));
                }
                break;

            case ENTTYPE_PLAYER:
                // Never display the player's name...unless they want to see it
                {
                    char buf[128];
                    char* buf2 = 0; // for directly inserted articles
                    int yoff = 0;
                    int iTextWidth = 0;

                      if( ( e!=playerPtr() || IsShown(e, optionGet(kUO_ShowOwnerName)) ) && IsShown(e, optionGet(kUO_ShowSupergroup)) )
                    {
                        if(e->supergroup_name[0] != '\0')
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "<%s>", e->supergroup_name);
                            iTextWidth = str_wd(font_grp, fScale*.65, fScale*.65, buf);

                            font_color(0xccddffff, 0xccddffff);
                            if(!demoIsPlaying() || !demo_state.demo_hide_names)
                            {
                                cprntEx( xp, yp-5, z, fScale*.65, fScale*.65, CENTER_X | NO_MSPRINT, buf);
                            }
                            font_color(getReticleColor(e, 0, 1, 1) & 0xffffff00 | iAlpha,
                                        getReticleColor(e, 1, 1, 1) & 0xffffff00 | iAlpha);

                            yoff = -8*fScale;
                        }
                    }

                    font_color(getReticleColor(e, 0, 1, 1) & 0xffffff00 | iAlpha,
                                getReticleColor(e, 1, 1, 1) & 0xffffff00 | iAlpha);

                      if( (e!=playerPtr() && IsShown(e, optionGet(kUO_ShowPlayerName))) || (e==playerPtr() &&IsShown(e,optionGet(kUO_ShowOwnerName))) )
                    {
                        int width;

                        if( e->pl->titleBadge )
                            yoff += -8*fScale;

                        if (e->logout_timer)
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "NameDisplayWithStatus", e->name, e->logout_bad_connection ? "StatusDisconnected" : "StatusQuit",(int)e->logout_timer/30);
                        }
                        else if (pvpTimer)
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "NameDisplayWithStatus", e->name, "StatusPvPActive", pvpTimer);
                        }
                        else
                        {
                            msPrintf(menuMessages, SAFESTR(buf), "NameDisplay", e->name);
                        }

                        if(e->pl->attachArticle && !(e->pl->titleCommon[0] || e->pl->titleOrigin[0]))
                        {
                            buf2 = strInsert(buf, buf, e->pl->titleTheText);
                        }

                        if(!demoIsPlaying() || !demo_state.demo_hide_names)
                        {
                            cprntEx( xp, yp+yoff-5*fScale, z, fScale, fScale, CENTER_X|NO_MSPRINT, buf2 ? buf2 : buf );
                        }

                        width = str_wd_notranslate(font_grp, fScale, fScale, buf);
                        if(width>iTextWidth) iTextWidth=width;

                        // Only show titles when showing player name
                        if(1)
                        {
                             int yy;
                            bool nospace = FALSE;

                            if(e->pl->titleBadge)
                            {
                                const BadgeDef *badge = badge_GetBadgeByIdx(e->pl->titleBadge);
                                if( badge )
                                       cprnt(xp, yp + yoff + 3*fScale, z, fScale*.65, fScale*.65, printLocalizedEnt(badge->pchDisplayTitle[ENT_IS_VILLAIN(e)?1:0], e));
                            }


                            buf[0]='\0';
                            if (e->pl->bIsGanker)
                            {
                                strcat(buf,printLocalizedEnt("PvPGankerTitle",e));
                            }
                            else
                            {
                                if((!e->pl->attachArticle || e->pl->titleCommon[0] || e->pl->titleOrigin[0]) && e->pl->titleTheText[0])
                                {
                                    strcat(buf, e->pl->titleTheText);
                                }

                                if(e->pl->titleCommon[0]!='\0')
                                {
                                    if(buf[0]!='\0' && !e->pl->attachArticle) strcat(buf, " ");
                                    strcat(buf, e->pl->titleCommon);
                                }

                                if(e->pl->titleOrigin[0]!='\0')
                                {
                                    if(buf[0]!='\0' && (e->pl->titleCommon[0] || !e->pl->attachArticle)) strcat(buf, " ");
                                    strcat(buf, e->pl->titleOrigin);
                                }
                            }

                            yy = yp+yoff-5-16*fScale*.75;

                            if(buf[0]!='\0')
                            {
                                int width = str_wd(font_grp, fScale*.75, fScale*.75, buf);
                                if( e->pl->titleColors[0] && e->pl->titleColors[1] )
                                    font_color(e->pl->titleColors[0], e->pl->titleColors[1]);
                                cprnt(xp, yy, z, fScale*.75, fScale*.75, buf);
                                if(width>iTextWidth) iTextWidth=width;
                                yy -= 12*fScale*.75;
                            }

                            if(e->pl->titleSpecial[0]!='\0')
                            {
                                int width = str_wd(font_grp, fScale*.75, fScale*.75, e->pl->titleSpecial);
                                font_color(CLR_WHITE & 0xffffff00 | iAlpha,
                                    CLR_YELLOW & 0xffffff00 | iAlpha);
                                cprnt(xp, yy, z, fScale*.75, fScale*.75, e->pl->titleSpecial);
                                if(width>iTextWidth) iTextWidth=width;
                            }
                            else if(e->pl->helperStatus == 1)
                            {
                                char *text = textStd("HelperStatusHelpMe");
                                int width = str_wd(font_grp, fScale*.75, fScale*.75, text);
                                font_color(0x00ff0000 | iAlpha,
                                    0x00ff0000 | iAlpha);
                                cprnt(xp, yy, z, fScale*.75, fScale*.75, text);
                                if(width>iTextWidth) iTextWidth=width;
                            }
                            else if(e->pl->helperStatus == 2)
                            {
                                char *text = textStd("HelperStatusHelper");
                                int width = str_wd(font_grp, fScale*.75, fScale*.75, text);
                                font_color(0x00ff0000 | iAlpha,
                                    0x00ff0000 | iAlpha);
                                cprnt(xp, yy, z, fScale*.75, fScale*.75, text);
                                if(width>iTextWidth) iTextWidth=width;
                            }
                        }
                    }

                     if( ( e!=playerPtr() || IsShown(e, optionGet(kUO_ShowOwnerName)) ) && IsShown(e, optionGet(kUO_ShowArchetype)) )
                    {
                        AtlasTex *ptex = GetOriginIcon(e);
                        display_sprite(ptex,
                            xp-iTextWidth/2-fScale*AICON_SIZE-3, yp+yoff/2-5-fScale*(AICON_SIZE-2), z,
                            fScale*AICON_SIZE/ptex->width, fScale*AICON_SIZE/ptex->height,
                            iAlpha|0xffffff00);
                    }

                     if( ( e!=playerPtr() || IsShown(e, optionGet(kUO_ShowOwnerName)) ) && IsShown(e, optionGet(kUO_ShowArchetype)) )
                    {
                        AtlasTex *ptex = GetArchetypeIcon(e);
                        display_sprite(ptex,
                            xp+iTextWidth/2+3, yp+yoff/2-5-fScale*(AICON_SIZE-2), z,
                            fScale*AICON_SIZE/ptex->width, fScale*AICON_SIZE/ptex->height,
                            iAlpha|0xffffff00);
                    }

                      if(e->pl && e->pl->afk && e->pl->afk_string[0]!='\0')
                    {
                         int y;
    
                        if( e->pl->titleTheText[0] || e->pl->titleCommon[0] || e->pl->titleOrigin[0] )
                              yoff += -8*fScale;
                        y = yp+yoff;
                        if( (e!=playerPtr() && IsShown(e, optionGet(kUO_ShowPlayerName))) || (e==playerPtr() &&IsShown(e, optionGet(kUO_ShowOwnerName))) )
                              y -= AICON_SIZE*fScale;    
                        
                        if(!demoIsPlaying() || !demo_state.demo_hide_chat)
                        {
                            drawChatBubble(kBubbleType_Emote, kBubbleLocation_Somewhere,
                                e->pl->afk_string,
                                xp, y, z,
                                fScale*0.75f, 0x000000d0 & iAlpha, 0xffffff60 & (~0xff|iAlpha), 0x00000060 & iAlpha, 0);
                        }
                    }
                    else if( IsShown(e, optionGet(kUO_ShowPlayerRating))  )
                    {
                        AtlasTex * star = atlasLoadTexture( "MissionPicker_icon_star_16x16.tga");
                        int rating = playerNote_GetRating(e->name);
                    
                        if( rating )
                        {
                            int i;
                            F32 y = yp+yoff;
                            F32 x = xp - (fScale*star->width*rating)/2;
                            if( e->pl->titleTheText[0] || e->pl->titleCommon[0] || e->pl->titleOrigin[0] )
                                yoff += -8*fScale;
                            y = yp+yoff;
                            if( (e!=playerPtr() && IsShown(e, optionGet(kUO_ShowPlayerName))) || (e==playerPtr() &&IsShown(e, optionGet(kUO_ShowOwnerName))) )
                                y -= AICON_SIZE*fScale;    

                            for( i = 0; i < rating; i++ )
                                display_sprite(star, x + (i*fScale*star->width), y-(5+star->height)*fScale, z, fScale, fScale, CLR_WHITE );
                        }
                    }
                }

                bDrawBars = bDrawBars && IsShown(e,optionGet(kUO_ShowPlayerBars));
                break;

            case ENTTYPE_NPC:
                // show display_name if we have one for this entity (server set non-default name)
                bDrawBars = false;
                if (e->name[0]) 
                {
                     if(!demoIsPlaying() || !demo_state.demo_hide_names)
                    {
                        cprntEx( xp, yp-8*fScale, z, fScale, fScale,CENTER_X | NO_MSPRINT, e->name);
                    }
                 }
                break;

            default:
                break;
        }

        if(bDrawBars)
        {
            if ( e->state.mode & ( 1 << ENT_DEAD ) )
            {
                drawBarOnly( xp, yp, z, 0, 100, barH, NULL, fScale, iAlpha, CLR_WHITE);
            }
            else
            {
                if(e->pchar)
                {
                    float hitpointDisplay = e->pchar->attrCur.fHitPoints + e->pchar->attrMax.fAbsorb;
                    bool specialbaseend = false;

                    // If the entity is not dead yet, always draw *something* in the health bar so the players know it is not dead.
                    if(e->pchar->attrCur.fHitPoints < 1 && !entMode( e, ENT_DEAD ))
                    {
                        hitpointDisplay = 1;
                    }
                    drawBarOnly( xp, yp, z, hitpointDisplay, e->pchar->attrMax.fHitPoints + e->pchar->attrMax.fAbsorb, barH, NULL, fScale, iAlpha, CLR_WHITE);
                    if (e->pchar->attrMax.fAbsorb > 0.005f)
                    {
                        display_sprite(barI, xp - (fScale * barI->width) / 2.f,
                            yp, z + 1, fScale * (float) e->pchar->attrMax.fAbsorb / (e->pchar->attrMax.fHitPoints + e->pchar->attrMax.fAbsorb), fScale, CLR_WHITE);
                    }

                    if(e->idDetail)
                    {
                        RoomDetail *detail = baseGetDetail(&g_base,e->idDetail,NULL);
                        if(detail && !(detail->bControlled && detail->bPowered))
                        {
                            int yip = yp + (barH->height-2)*fScale;
                            AtlasTex *engIcon = atlasLoadTexture("base_icon_power.tga");
                            AtlasTex *conIcon = atlasLoadTexture("base_icon_control.tga");

                            specialbaseend = true;
                            drawBarOnly( xp, yip, z, e->pchar->attrCur.fEndurance, e->pchar->attrMax.fEndurance, barI, NULL, fScale, iAlpha, 0x444444ff);

                            yip -= (barI->height-2)*fScale;
                            font_color(pulsecolor,pulsecolor);

                            if(!detail->bControlled && !detail->bPowered)
                            {
                                display_sprite( engIcon, xp-((engIcon->width*fScale)+1), yip, z, fScale, fScale, pulsecolor );
                                display_sprite( conIcon, xp+1, yip+0.5, z, fScale, fScale, pulsecolor );
                            }
                            else
                            {
                                display_sprite( detail->bControlled?engIcon:conIcon, xp-((engIcon->width * fScale)/2), yip+(detail->bControlled?0:0.5), z, fScale, fScale, pulsecolor );
                            }
                        }
                    }
    
                    if(!specialbaseend)
                        drawBarOnly( xp, yp + (barH->height-2)*fScale, z, e->pchar->attrCur.fEndurance, e->pchar->attrMax.fEndurance, barE, NULL, fScale, iAlpha, CLR_WHITE);
                }
            }
        }
    }
}


void drawMissionInteractTimerProgress()
{
    static CapsuleData capsuleState = {0};
    char* actionStr;
    float timeRemaining;
    float totalTime;
    
    // Draw the interaction progress bar if the timer is running.
    if(MissionGetObjectiveTimer(&timeRemaining, &totalTime))
    {
        capsuleState.showText = 0;
        capsuleState.instant = 1;

        // Add the action string to the progress bar if present.
        actionStr = MissionGetObjectiveInteractString();
        if(actionStr && actionStr[0])
        {
            capsuleState.showText = 1;
            capsuleState.txt = actionStr;
            capsuleState.txt_clr1 = CLR_WHITE;
            capsuleState.txt_clr2 = CLR_WHITE;
        }

        // Draw a progress bar at screen center and 1/3 screen long.
        {
            float xCenter = game_state.screen_x/2;
            float yCenter = game_state.screen_y/2;
            float barWidth = game_state.screen_x/3;

            // JS: FIXME!!! Don't know how tall the bar is.  Can't adjust progressbar height.
            drawCapsule( xCenter  - barWidth/2, yCenter, 1000.0, barWidth, 1.0, CLR_WHITE, 0x00000088, CLR_RED,
                timeRemaining, totalTime, &capsuleState);
        }
    }
    else
    {
        // Otherwise, reset progressbar state.
        memset(&capsuleState, 0, sizeof(CapsuleData));
    }
}

bool EntityIsVisible(Entity *eTarget)
{
    DefTracker *tracker;
    Vec3 vecStart;
    Vec3 vecEnd;
    bool bRet;
    Entity *player = playerPtr();

    if (eTarget->seeThroughWalls)
        return 1;

    copyVec3(cam_info.cammat[3], vecStart);
    copyVec3(ENTPOS(eTarget), vecEnd);
    vecEnd[1]+=6;

    tracker = groupFindOnLine(vecStart, vecEnd, NULL, &bRet, 1);

    return !bRet;
}

void drawStuffOnEntities(void)
{
    int         enemy_id;
    Entity*     player = playerPtr();
    int         i;
    Vec3        pos = {0,0,0};

    s_eAssist = NULL;
    s_eAssistTarget = NULL;
    s_eTargetTarget = NULL;
    s_eTauntTarget = NULL; 

    if( (!game_state.viewCutScene && !isMenu(MENU_GAME)) || !player || editMode())
        return;
    
    memset(s_abUsed, 0, sizeof(bool)*BUBBLE_NUM_X_SLOTS*BUBBLE_NUM_Y_SLOTS);

    if (activeTaskDest.navOn && activeTaskDest.showWaypoint)
        compass_Draw3dBlip(&activeTaskDest, 0xff000000);
    if (waypointDest.navOn && waypointDest.showWaypoint && (!activeTaskDest.navOn || !destinationEquiv(&waypointDest, &activeTaskDest)))
        compass_Draw3dBlip(&waypointDest, (waypointDest.type == ICON_MISSION_WAYPOINT)?0xff000000:0xffff0000);

    // Mission Keydoors
    for (i = eaSize(&keydoorDest)-1; i >= 0; i--)
        if (keydoorDest[i]->navOn)
            compass_Draw3dBlip(keydoorDest[i], 0xffff0000);

    // Set new target and/or show mouse menu
    if(mouseRightClick() && current_target_under_mouse)
    {
        //PETUI
        if( entIsMyPet(current_target_under_mouse) && playerIsMasterMind())
            current_pet = erGetRef( current_target_under_mouse );
        else
            current_target = current_target_under_mouse;
        if (!entMode(player, ENT_DEAD) || current_target_under_mouse->pl)
            contextMenuForTarget(current_target_under_mouse);
    }

    if (game_state.edit_base)
    {
        current_target = NULL;
        gSelectedDBID = 0;
    }

    if(player->pchar)
    {
        s_eAssist = erGetEnt(player->pchar->erAssist);
        if(s_eAssist && s_eAssist->pchar)
            s_eAssistTarget = erGetEnt(s_eAssist->pchar->erTargetInstantaneous);

        s_eTauntTarget = erGetEnt(g_erTaunter);
    }
    if(current_target && current_target->pchar && team_IsMember(player, current_target->db_id))
    {
        s_eTargetTarget = erGetEnt(current_target->pchar->erTargetInstantaneous);
    }

    // JS:  Mission objective interact progress bar.
    //      Don't know where this belongs.  I'll cram it in here until someone
    //      screams bloody murder.
    drawMissionInteractTimerProgress();

    // Dismiss dead and invalid entities from current_target
    if( current_target && !( entity_state[ current_target->owner ] & ENTITY_IN_USE ) )
        current_target = ( Entity * )NULL;  //enemy has died.

    if( current_target )
        enemy_id = current_target->svr_idx;
    else
        enemy_id = 0;

    if( current_target && !isEntitySelectable(current_target))
        current_target = NULL;
    if( current_target_under_mouse && !isEntitySelectable(current_target_under_mouse))
        current_target_under_mouse = NULL;

    // First show the current target's info
    if( current_target && current_target->seq &&
        ( ENTTYPE(current_target) == ENTTYPE_CRITTER
            || ENTTYPE(current_target) == ENTTYPE_PLAYER
            || ENTTYPE(current_target) == ENTTYPE_NPC ) )
    {
        drawStuffOnEntity(current_target, 1.0f, 0xff, true);
    }

    // If the cursor is hovering over something else, show that too
    if( current_target_under_mouse
        && current_target_under_mouse != current_target
        && current_target_under_mouse->seq
        && ( ENTTYPE(current_target_under_mouse) == ENTTYPE_CRITTER
            || ENTTYPE(current_target_under_mouse) == ENTTYPE_PLAYER
            || ENTTYPE(current_target_under_mouse) == ENTTYPE_NPC ) )
    {
        drawStuffOnEntity(current_target_under_mouse, 0.8f, 0xc0, true);
    }

    // If the player is assiting someone, show that too
    if( s_eAssistTarget
        && s_eAssistTarget != current_target
        && ( ENTTYPE(s_eAssistTarget) == ENTTYPE_CRITTER
            || ENTTYPE(s_eAssistTarget) == ENTTYPE_PLAYER
            || ENTTYPE(s_eAssistTarget) == ENTTYPE_NPC ) )
    {
        drawStuffOnEntity(s_eAssistTarget, 0.8f, 0xc0, true);
    }

    // Show the auto-target (the current target's target)
    if( s_eTargetTarget
        && s_eTargetTarget != player
        && s_eTargetTarget != current_target
        && ( ENTTYPE(s_eTargetTarget) == ENTTYPE_CRITTER
            || ENTTYPE(s_eTargetTarget) == ENTTYPE_PLAYER
            || ENTTYPE(s_eTargetTarget) == ENTTYPE_NPC ) )
    {
        drawStuffOnEntity(s_eTargetTarget, 0.8f, 0xc0, true);
    }

    for(i = 1; i < entities_max; i++)
    {
        bool bDraw = false;
        Entity* e = entities[i];
        int ctsHeight = 0;

        if(e && ((entInUse(i) && !ENTHIDE_BY_ID(i)) || e==playerPtr()))
        {
            //Am I drawn?  
            if( ENTTYPE(e)==ENTTYPE_PLAYER || 
                e->iNumFloaters>0 ||
                ( ENTTYPE(e)==ENTTYPE_CRITTER && 
                  ( optionGet(kUO_ShowVillainName) & kShow_Always || 
                    e->alwaysCon ||
                    isPetDisplayed( e ) ||
                    ( IsShown(e, optionGet(kUO_ShowPlayerName)) && entIsMyPet(e) ) ) ) )
            {
                bDraw = EntityIsVisible(e);
            }

            // Click to Source Displays
            if (CTS_SHOW_NPC && ENTTYPE(e)!=ENTTYPE_DOOR && e!=playerPtr())
                ClickToSourceDisplay(ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2], ctsHeight-=10, CLR_WHITE, npcDefList.npcDefs[e->npcIndex]->fileName, NULL, CTS_TEXT_DEBUG_3D);
            if (CTS_SHOW_VILLAINDEF)
                ClickToSourceDisplay(ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2], ctsHeight-=10, CLR_WHITE, e->villainDefFilename, e->villainInternalName, CTS_TEXT_DEBUG_3D);
            if (CTS_SHOW_SEQUENCER && ENTTYPE(e)!=ENTTYPE_DOOR && e->seq && e->seq->info)
            {
                char seqPath[MAX_PATH];
                sprintf(seqPath, "sequencers/%s", e->seq->info->name);
                ClickToSourceDisplay(ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2], ctsHeight-=10, CLR_WHITE, seqPath, NULL, CTS_TEXT_DEBUG_3D);
            }
            if (CTS_SHOW_ENTTYPE && ENTTYPE(e)!=ENTTYPE_DOOR && e->seq && e->seq->type)
                ClickToSourceDisplay(ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2], ctsHeight-=10, CLR_WHITE, e->seq->type->filename, NULL, CTS_TEXT_DEBUG_3D);
            if (CTS_SHOW_POWERS)
            {
                const BasePower** activePowers = basePowersFromPowerBuffs(e->ppowBuffs);
                int powCounter;
                for (powCounter = 0; powCounter < eaSize(&activePowers); powCounter++)
                {
                    const char* powerFilename = activePowers[powCounter]->pchSourceFile;
                    if (!powerFilename && activePowers[powCounter]->psetParent)
                        powerFilename = activePowers[powCounter]->psetParent->pchSourceFile;
                    ClickToSourceDisplay(ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2], ctsHeight-=8, CLR_WHITE, powerFilename, activePowers[powCounter]->pchName, CTS_TEXT_DEBUG_3D);
                }
            }

            if(e!=s_eAssist 
                && e!=s_eAssistTarget
                && (e!=s_eTargetTarget || ENTTYPE(e)==ENTTYPE_PLAYER) 
                && e!=current_target
                && e!=current_target_under_mouse
                && bDraw)
            {
                //if I am drawn, do I fulfill all the other strange requirements to have stuff drawn on me? If so, do it
                if( ENTTYPE(e)==ENTTYPE_PLAYER || 
                    ( isEntitySelectable(e) && 
                      ENTTYPE(e)==ENTTYPE_CRITTER && 
                      ( ( optionGet(kUO_ShowVillainName) & kShow_Always || isPetDisplayed( e ) ) || e->alwaysCon ||
                        ( IsShown(e, optionGet(kUO_ShowPlayerName)) && entIsMyPet(e) ) ) ) )
                {
                    drawStuffOnEntity(e, 0.8f, 0xff, true);
                }
            }

            //Do I have any floaters on me? If so, manage and draw them.
            if( e->iNumFloaters > 0 )
            {
                bool bForceBubbles = true;
                if( ENTTYPE(e)==ENTTYPE_PLAYER && !team_IsMember(player, e->db_id) )
                    bForceBubbles = false;
                drawFloatersOnEntity(e, bDraw, bForceBubbles);
            }

            if (e->contactHandle)
            {
                drawContactOverheadIcon(e);
            }
        }
    }

    // Draw reticles
    if( !game_state.viewCutScene )
    {
        int w, h;
        float reticleScale;
        static Entity* last_target = NULL;
        static float shrink_time;
        int selected = shrink_time > 0;

        // If we have a target enemy then draw brackets on the Booger.
        if(current_target != last_target)
        {
            last_target = current_target;
            shrink_time = 30 * 0.2f;
        }
        else
        {
            shrink_time -= TIMESTEP;
        }

        windowSize(&w, &h);
        reticleScale = (55.0 / game_state.fov) * h / 1024.0;

        if(current_target && current_target->seq)
        {
            int color1 = getReticleColor(current_target, 0, 1, 0);
            int color2 = getReticleColor(current_target, 1, 1, 0);

            if(current_target != s_eAssist)
            {
                if(IsShown(s_eAssist, optionGet(kUO_ShowAssistReticles)))
                    drawReticle(s_eAssist, 0x00ffebc0, 0x008800c0, reticleScale, 0);
            }
            else
            {
                color1 = 0x00ffebff;
                color2 = 0x008800ff;
            }

            if((ENTTYPE(current_target)==ENTTYPE_CRITTER && IsShown(current_target, optionGet(kUO_ShowVillainReticles)))
                || (ENTTYPE(current_target)==ENTTYPE_PLAYER && IsShown(current_target, optionGet(kUO_ShowPlayerReticles)))
                || (ENTTYPE(current_target)==ENTTYPE_NPC) || game_state.select_any_entity)
            {
                drawReticle(current_target,    color1,    color2,    reticleScale * (shrink_time > 0 ? 1.0 + (shrink_time / 30.0) * 4.0 : 1.0),    1);
            }
        }

        // If we have a target under the mouse, draw a reticle on it.
        if(current_target_under_mouse
            && current_target_under_mouse->seq
            && current_target!=current_target_under_mouse
            && s_eAssist!=current_target_under_mouse)
        {
            if((ENTTYPE(current_target_under_mouse)==ENTTYPE_CRITTER && IsShown(current_target_under_mouse, optionGet(kUO_ShowVillainReticles)))
                || (ENTTYPE(current_target_under_mouse)==ENTTYPE_PLAYER && IsShown(current_target_under_mouse, optionGet(kUO_ShowPlayerReticles)))
                || (ENTTYPE(current_target_under_mouse)==ENTTYPE_NPC))
            {
                drawReticle(current_target_under_mouse,    getReticleColor(current_target_under_mouse, 0, 0, 0),    getReticleColor(current_target_under_mouse, 1, 0, 0),    reticleScale, 0);
            }
        }

        if(s_eAssistTarget && s_eAssistTarget->seq && IsShown(s_eAssistTarget,optionGet(kUO_ShowAssistReticles)))
        {
            drawReticle(s_eAssistTarget,getReticleColor(s_eAssistTarget, 0, 0, 0),    getReticleColor(s_eAssistTarget, 1, 0, 0),    reticleScale,  0);
        }

        if(s_eTargetTarget && s_eTargetTarget->seq    && IsShown(s_eAssistTarget,optionGet(kUO_ShowAssistReticles)))
        {
            drawReticle(s_eTargetTarget, getReticleColor(s_eTargetTarget, 0, 0, 0),    getReticleColor(s_eTargetTarget, 1, 0, 0),    reticleScale, 0);
        }

        // If we've been taunted, but the taunting enemy is not the current target or
        //  the implied target
        if (s_eTauntTarget && s_eTauntTarget->seq && s_eTauntTarget != current_target && s_eTauntTarget != s_eTargetTarget)
        {
            drawReticle(s_eTauntTarget,    0xff0000c0,    0xff0000c0,    reticleScale,1);
        }
    }
}

#include <utilitieslib/utils/timing.h>
#include "sound/sound.h"
#include "win/input.h"
#include <utilitieslib/utils/utils.h>
void neighborhoodCheck()
{
    char        *name,*music;
    static char last_name[100];
    static char display_name[100];
    static float drawTimer;
    float nameDisplayInterval = 10.0;
    float nameFadeTime = 1.0;

    //if(inpEdge(INP_R))
    //    last_name[0] = 0;
    // Entered new neighborhood?
    if (pmotionGetNeighborhoodProperties(playerPtr(), &name, &music))
    {
        if (name && name[0] && stricmp(last_name, name))
        {
            if (stricmp(name, " ") && strnicmp(name, "NoShow_", 7 /* strlen("NoShow_") */ ))
            {
                drawTimer = nameDisplayInterval;
                strcpy(display_name, name);
                addSystemChatMsg(textStd("EnteringNeighborhood", name), INFO_SVR_COM, 0);
            }
            else if (drawTimer > nameFadeTime)
            {
                drawTimer = nameFadeTime;
            }
        }

        if (music && (strstri(music, "_loop") || (name && stricmp(last_name, name)!=0)))
            sndPlayNoInterrupt( music, SOUND_MUSIC );
        if (name)
            strcpy(last_name, name);
    }

    // Draw neighborhood name?
    if( !shell_menu() && drawTimer > 0)
    {
        int h, w;
        float opacity;
        int oldDynamicSetting;
        Vec3 opacityInterpPoints[] = {    {1000, 0, 0},
                                        {nameDisplayInterval, 0.0, 0},
                                        {nameDisplayInterval-nameFadeTime, 1, 0},
                                        {nameFadeTime, 1, 0},
                                        {0, 0, 0},
                                        {-1000, 0, 0} };

        windowSize(&w,&h);
        opacity = graphInterp(drawTimer, opacityInterpPoints);
        font_color( CLR_CONSTRUCT_EX(255, 255, 255, opacity), CLR_CONSTRUCT_EX(80, 80, 255, opacity) );

        font( &game_18 );
        oldDynamicSetting = font_grp->dynamic;
        cprnt( w / 2, 150, 90, 1.5, 1.5, display_name);
        drawTimer -= TIMESTEP/30;
        font_grp->dynamic = oldDynamicSetting;
    }
}


/* End of File */
