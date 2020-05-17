#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiInput.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiPictureBrowser.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"
#include "UI/sprite/sprite_text.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/mathutil.h>
#include "graphics/imageCapture.h"
#include "entity/entity.h"
#include <utilitieslib/components/estring.h>
#include "entity/motion.h"
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "win/win_init.h"
#include "seq/seqstate.h"
#include "entity/character_animfx_client.h"
#include "entity/character_base.h"
#include <utilitieslib/components/memorypool.h>
#include "seq/seqsequence.h"
#include "seq/AnimBitList.h"
#include "render/tex.h"
#include "seq/tricks.h"



typedef enum pictBrowseState
{
    PICTBROWSE_IDLE,
    PICTBROWSE_MOVING,
} PictBrowseState;

typedef enum pictBrowseAction
{
    PICTBROWSE_NONE = 0,
    PICTBROWSE_HOVER = -2,
    PICTBROWSE_HIT,
} PictBrowseAction;

typedef enum pictBrowseMotion
{
    PICTBROWSE_ONE_ANIMATION = 0,
    PICTBROWSE_MULTIPLE_ANIMATIONS,
    PICTBROWSE_NO_ANIMATIONS,
} PictBrowseMotion;

static int mmPictureChosenIdx = 0;
static PictBrowseMotion mmPictureAllowAnimations = 0;
static int mmPictureCurIdx = 1;
static int mmPictureAnimBitListLoaded = 0;

#define WINDOW_SIDES_BUFFER 5*PIX2
#define A_BUTTON_HEIGHT PIX3*5


static StashTable s_pictureBrowsers;
MP_DEFINE(PictureBrowser);

void picturebrowser_setAnimation(int toggle)
{
    if(toggle)
        mmPictureAllowAnimations = PICTBROWSE_ONE_ANIMATION;
    else
        mmPictureAllowAnimations = PICTBROWSE_NO_ANIMATIONS;
}

PictureBrowser *picturebrowser_getById(int id)
{
    PictureBrowser *pb = 0; 
    stashIntFindPointer( s_pictureBrowsers, id, &pb);
    if(!pb || pb->maxHt <=0 || pb->maxWd <= 0)
        pb= NULL;
    return pb;
}
PictureBrowser *picturebrowser_create()
{
    PictureBrowser *pb = NULL;

    // create the pool, arbitrary number
    MP_CREATE(PictureBrowser, 64);
    pb = MP_ALLOC( PictureBrowser );

    return pb;
}

StashTable s_pbrowsers = {0};

static Vec3 *s_getBGColor()
{
    int r = 0xFF000000;
    int g = 0x00FF0000;
    int b = 0x0000FF00;
    static Vec3 bg = {0};
    r &=PB_MM_COLOR;
    r = r >> 24;
    g &=PB_MM_COLOR;
    g = g >> 16;
    b &=PB_MM_COLOR;
    b = b >> 8;

    bg[0] = (r/255.0f);
    bg[1] = (g/255.0f);
    bg[2] = (b/255.0f);
    return &bg;
}
void picturebrowser_init( PictureBrowser *pb, float x, float y, float z, float wd, float ht, AtlasTex **elements, int wdw )
{
    assert(pb);
    memset(pb, 0, sizeof(PictureBrowser));
    pb->x = x;
    pb->y = y;
    pb->z = z;
    pb->idx = mmPictureCurIdx;
    mmPictureCurIdx++;

    
    pb->maxWd = pb->wd = wd;
    pb->wdw = wdw;

    pb->maxHt = pb->ht = ht;
    pb->elements = elements;
    if(!elements)
    {
        Vec3 *v = s_getBGColor();
        pb->mmdata = (MMPinPData*)calloc(1, sizeof(MMPinPData));
        copyVec3(*v, pb->mmdata->bgColor);
        pb->mmdata->motionAvailable = 1;
    }
    else
        pb->mmdata = 0;
    pb->state = PICTBROWSE_IDLE;
    pb->changed = TRUE;
    pb->currChoice = 0;
    pb->sc = 1;
    if(!mmPictureAnimBitListLoaded)
    {
        loadAnimLists();
        mmPictureAnimBitListLoaded = 1;
    }
    if(!s_pictureBrowsers)
    {
        s_pictureBrowsers = stashTableCreateInt(100);
    }

    //if we've already recorded the image, return it.
    //if(stillImage)
    //{
    stashIntAddPointer( s_pictureBrowsers, pb->idx, pb, true);
}

// for dynamically populating a browser
void picturebrowser_addPictures( PictureBrowser *pb, AtlasTex **elements)
{
    assert(pb);
    eaPushArray(&pb->elements, &elements);
}

// clear and free all of the elements
void picturebrowser_clearPictures( PictureBrowser *pb)
{
    if(pb->elements)
        eaClear(&pb->elements);
    if(pb->mmdata)
    {
        pbufDelete(&pb->mmdata->pbuffer);
        if(pb->mmdata->e)
        {
            pb->mmdata->e->waitingForFadeOut =1;
            pb->mmdata->e->currfade = 0;
            //entity will be freed properly on its update tick.
            //we can get rid of it for now.
            if(pb->mmdata->e->seq && pb->mmdata->e->motion)
                entFree(pb->mmdata->e);
            else
                pb->mmdata->e = 0;
            pb->mmdata->e = 0;
        }
        free(pb->mmdata);
        pb->mmdata = 0;
    }
}

// clear out a pictureBrowser structure to be re-inited
void picturebrowser_destroy(PictureBrowser *pb)
{
    if(!pb)
        return;
    stashIntRemovePointer( s_pictureBrowsers, pb->idx, &pb);
    picturebrowser_clearPictures(pb);
    memset( pb, 0, sizeof(PictureBrowser) );
    MP_FREE(PictureBrowser, pb);
}


int picturebrowser_ClearElement(StashElement element)
{
    PictureBrowser* pb = (PictureBrowser*)stashElementGetPointer(element);

    if(pb)
        picturebrowser_destroy(pb);
    return 1;
}

int picturebrowser_clearEntity(StashElement element)
{
    PictureBrowser* pb = (PictureBrowser*)stashElementGetPointer(element);

    if(pb && pb->mmdata && pb->mmdata->e)
    {
        //the entity is going to be reused, so we should let go of it.
        entFree(pb->mmdata->e);
        pb->mmdata->e = 0;
    }
    return 1;
}

void picturebrowser_clearElements()
{
    if(s_pictureBrowsers && stashGetValidElementCount(s_pictureBrowsers))
        stashForEachElement(s_pictureBrowsers, picturebrowser_ClearElement);
}

void picturebrowser_clearEntities()
{
    if(s_pictureBrowsers && stashGetValidElementCount(s_pictureBrowsers))
        stashForEachElement(s_pictureBrowsers, picturebrowser_clearEntity);
}

// sets a specific element to be selected
void picturebrowser_setId( PictureBrowser *pb, int id)
{
    pb->currChoice = id;
    pb->changed = TRUE;
}

// set the location of a picture browser
void picturebrowser_setLoc( PictureBrowser *pb, float x, float y, float z )
{
    assert(pb);
    pb->x = x;
    pb->y = y;
    pb->z = z;
}
void picturebrowser_setScale(PictureBrowser *pb, float scale)
{
    assert(pb);
    pb->sc = scale;
}

int picurebrowser_drawNavButton(float x, float y, float z, float wd, float ht, AtlasTex * icon)
{
    UIBox uibox;
    float subx = x, suby = y, subht = ht, subwd = wd;
    float subsc = 1;
    CBox cbox = {0};

    uibox.x = subx;
    uibox.y = suby;
    uibox.width = subwd;
    uibox.height = subht;

    uiBoxToCBox(&uibox, &cbox);

    if(icon)
    {
        subwd = MIN(icon->width, subwd - PIX2*2);
        subht = MIN(icon->height, subht - PIX2*2);
        subsc = MIN(subwd/icon->width, subht/icon->height);
        suby = y+(ht - icon->height*subsc)/2.0;
        subx = x+(wd - icon->width*subsc)/2.0;
    }

    if(icon)
        display_sprite(icon, subx, suby, z+2, subsc, subsc, 0xffffffff);

    //if hover
    if(mouseCollision( &cbox ))
    {
        drawFlatFrame(PIX3, PIX2, x, y, z+1, wd, ht, 1, 0x88888888, 0x88888888);
        return PICTBROWSE_HOVER;
    }
    if(mouseDownHit(&cbox, MS_LEFT))
    {
        drawFlatFrame(PIX3, PIX2, x, y, z+1, wd, ht, 1, 0xffffff88, 0xffffff88);
        return PICTBROWSE_HIT;
    }
    else
    {
        return PICTBROWSE_NONE;
    }
}

int picturebrowse_drawSpinButtons(Vec2 ul, Vec2 lr, F32 z)
{
    AtlasTex * spinL;
    AtlasTex * spinR;
    AtlasTex * ospinL;
    AtlasTex * ospinR;
    AtlasTex * aspinL;
    AtlasTex * aspinR;
    F32 scale;// = 1;
    F32 x = ul[0],y = lr[1];
    CBox box;
    //int x[2] = {735, 930}, y = 656, z = SPIN_BUTTON_ZPOS_DEFAULT;
    int colorNormal = CLR_WHITE;
    int turn = -1;

    spinL     = atlasLoadTexture( "costume_button_spin_L_tintable.tga" );
    spinR     = atlasLoadTexture( "costume_button_spin_R_tintable.tga" );
    ospinL    = atlasLoadTexture( "costume_button_spin_over_L_tintable.tga" );
    ospinR    = atlasLoadTexture( "costume_button_spin_over_R_tintable.tga" );
    aspinL    = atlasLoadTexture( "costume_button_spin_activate_L_grey.tga" );
    aspinR    = atlasLoadTexture( "costume_button_spin_activate_R_grey.tga" );

    scale = (lr[0]-ul[0])/(spinL->width*5);
    x +=spinL->width*scale;
    y -=spinL->height*scale;
    
    build_cboxxy( spinL, &box, scale, scale, x, y);
    if( mouseCollision(&box) )
    {
        turn = PICTBROWSE_HOVER;
        if( isDown( MS_LEFT ))
        {
            display_sprite( aspinL, x, y, z, scale, scale, CLR_WHITE );
            turn = PICTBROWSE_LEFT;
        }
        else
            display_sprite( ospinL, x, y, z, scale, scale, colorNormal );
    }
    else
        display_sprite( spinL, x, y, z, scale, scale, colorNormal );

    x +=2*spinL->width*scale;
    build_cboxxy( spinR, &box, scale, scale, x, y );

    if( mouseCollision(&box) )
    {
        turn = PICTBROWSE_HOVER;
        if( isDown( MS_LEFT ))
        {
            display_sprite( aspinR, x, y, z, scale, scale, CLR_WHITE );
            turn = PICTBROWSE_RIGHT;
        }
        else
            display_sprite( ospinR, x, y, z, scale, scale, colorNormal );
    }
    else
        display_sprite( spinR, x, y, z, scale, scale, colorNormal );
    return turn;
}


unsigned int NextPow2( unsigned int x ) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

void picturebrowser_updateFrame(PictureBrowser *pb, MMPinPData *mmdata)
{
    Vec3 pos = {0,mmdata->yOffset*2,0};
    Vec2 screenPos = {0,0};
    if(!imageCapture_verifyMMdata(mmdata))    //if we don't have a sequence, we can't do anything.
        return;


    //entUpdatePosInstantaneous(mmdata->e,pos);
    //copyVec3(pos, mmdata->e->motion->last_pos);
    mmdata->e->noDrawOnClient = 0;
    updateMMSeqImage(mmdata, screenPos);

    {
        Vec2 ul={mmdata->x1, mmdata->y1};
        Vec2 lr={mmdata->x2, mmdata->y2};
        entUpdatePosInstantaneous(mmdata->e,pos);
        atlasUpdateTexturePbuffer(ul, lr, mmdata->bind, &mmdata->pbuffer);            
        mmdata->e->noDrawOnClient = 1;
    }
}

AtlasTex * picturebrowser_updateImageCaptureData(MMPinPData *mmdata, unsigned int wd, 
                                                 unsigned int ht, int resetImageBoundaries,
                                                 AtlasTex *stillImage)
{
        U8 * pixbuf =0;
        //Settable params
        //    int screenWidth, screenHeight;
        unsigned int sizeOfPictureX = mmdata->wdPow = NextPow2(wd); //Size of shot on screen.
        unsigned int sizeOfPictureY = mmdata->htPow = NextPow2(ht); //Powers of two!!! ARgh.
        F32 headShotFovMagic = 15;//, height;   //This is a weird magic thing
        F32 scale = 1.5;
        int shouldIFlip = 1;
        F32 height, width;
        int tempX1, tempX2, tempY1, tempY2;

        if(!imageCapture_verifyMMdata(mmdata))    //if we don't have a sequence, we can't do anything.
            return white_tex_atlas;

        mmdata->FOV = headShotFovMagic;


        copyMat4( unitmat, mmdata->e->seq->gfx_root->mat );


        //setup lookat position
        if(resetImageBoundaries)
        {
            BoneId headNum;
            GfxNode * headNode;
            Mat4 lookAtMatrix;
            Mat4 collMat;
            Vec3 headPosition;
            EntityCapsule cap;
            
            F32 rotateRadians = 0;
            Vec3 pos = {0,mmdata->yOffset*2,0};
            entUpdatePosInstantaneous(mmdata->e,pos);
            
            mmdata->headshotCameraPos[0] = 0.0;
            mmdata->headshotCameraPos[1] = 0.0;
            mmdata->headshotCameraPos[2] = 0.0;

            mmdata->lookatPos[0] = 0;//(worldMax[0]-worldMin[0])/2.0;
            mmdata->lookatPos[1] = 0;//seq->type->capsuleSize[1] + seq->type->reticleHeightBias;//(worldMax[1]-worldMin[1])/2.0;
            mmdata->lookatPos[2] = 0;//(worldMax[2]-worldMin[2])/2.0;
            gfxTreeFindWorldSpaceMat(lookAtMatrix, mmdata->e->seq->gfx_root);

            if( mmdata->e->seq->type->selection == SEQ_SELECTION_LIBRARYPIECE )
            {
                getCapsule(mmdata->e->seq, &cap ); 
                positionCapsule( mmdata->e->seq->gfx_root->mat, &cap, collMat );

                if(cap.dir == 0)
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.radius*2+ mmdata->e->seq->type->reticleWidthBias)/2.0 + cap.offset[0]);
                else if(cap.dir == 1)
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias)/2.0 + cap.offset[1]);
                else if(cap.dir == 2)
                {
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.radius*2+ mmdata->e->seq->type->reticleBaseBias)/2.0 + cap.offset[2]);

                }
                else
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.radius*2+ mmdata->e->seq->type->reticleBaseBias)/2.0 + cap.offset[2]);

                if(cap.dir == 1)
                {
                    height = mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias));
                    width = mmdata->e->seq->curranimscale*((cap.radius*2+ mmdata->e->seq->type->reticleWidthBias)/2.0 + cap.offset[0]);
                }
                else
                {
                    height = mmdata->e->seq->curranimscale*((cap.radius*2));
                    width = mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias));
                }
                height *= scale;
                width *= scale;

                //mmdata->headshotCameraPos[1] += height*.5;
            }
            else if( mmdata->e->seq->type->selection == SEQ_SELECTION_BONES )
            {
                //HIPS
                headNum = BONEID_HIPS;  
                headNode = gfxTreeFindBoneInAnimation( headNum, mmdata->e->seq->gfx_root->child, mmdata->e->seq->handle, 1 );
                gfxTreeFindWorldSpaceMat(lookAtMatrix, headNode);
                copyVec3(lookAtMatrix[3], mmdata->lookatPos);

                //HEAD
                headNum = BONEID_HEAD;  
                headNode = gfxTreeFindBoneInAnimation( headNum, mmdata->e->seq->gfx_root->child, mmdata->e->seq->handle, 1 );
                gfxTreeFindWorldSpaceMat(lookAtMatrix, headNode);
                copyVec3(lookAtMatrix[3], headPosition);

                height = mmdata->e->seq->curranimscale*(mmdata->e->seq->type->capsuleSize[1]+ mmdata->e->seq->type->reticleHeightBias);
                width = mmdata->e->seq->curranimscale*(mmdata->e->seq->type->capsuleSize[0]+ mmdata->e->seq->type->reticleWidthBias);

                width *= headPosition[1]*scale/height;
                height = headPosition[1]*scale;//seq->curranimscale*seq->type->reticleHeightBias;//headPosition[1]+ seq->type->reticleHeightBias;

            }

            else
            {
                getCapsule(mmdata->e->seq, &cap ); 
                positionCapsule( mmdata->e->seq->gfx_root->mat, &cap, collMat );

                if(cap.dir == 1)
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias)/2.0 + cap.offset[1] );
                else if(cap.dir == 2)
                    mmdata->lookatPos[1] =mmdata->e->seq->curranimscale*((cap.radius*2)/2);

                if(cap.dir == 1)
                {
                    height = mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias));
                    width = mmdata->e->seq->curranimscale*((cap.radius*2+ mmdata->e->seq->type->reticleWidthBias));
                }
                else
                {
                    height = mmdata->e->seq->curranimscale*((cap.radius*2));
                    width = mmdata->e->seq->curranimscale*((cap.length+ cap.radius*2+ mmdata->e->seq->type->reticleHeightBias));
                }

                //these tend to be a little more flush--move them out by adding height
                height *= scale;
                width *= scale;
            }

            //Match so that the full height of the image fits into the image.
            //if the width is larger than the height, change the height so that both fit.
            if(width > height)
                height = width;

            mmdata->lookatPos[2] = 10.0*height/3.0;


            //move the particles forward about 30 seconds.
            if(mmdata->e->seq->type->selection == SEQ_SELECTION_LIBRARYPIECE)
            {
                mmdata->lookatPos[1] = 0;
            }
        }

        //if (getRootHealthFxLibraryPiece( e->seq , (int)e->svr_idx ))
        manageEntityHitPointTriggeredfx(mmdata->e);

        tempX1 = mmdata->x1;
        tempX2 = mmdata->x2;
        tempY1 = mmdata->y1;
        tempY2 = mmdata->y2;


        


        getMMSeqImage(&pixbuf, mmdata->e->seq, mmdata->headshotCameraPos, mmdata->lookatPos, mmdata->yOffset, mmdata->rotation, (F32) headShotFovMagic,
            &sizeOfPictureX, &sizeOfPictureY, &mmdata->x1, &mmdata->x2, &mmdata->y1, &mmdata->y2, resetImageBoundaries, NULL, 0);


        //if the first try was incorrect due to bad model data, get the correct size now.
        if(resetImageBoundaries)
        {

            //if this is ridiculously large, it's because the model information is unhelpful.
            //we need to try again
            float xRatio = sizeOfPictureX/(F32)mmdata->wdPow;
            float yRatio = sizeOfPictureY/(F32)mmdata->htPow;
            float yMovePixels = (mmdata->y1 + mmdata->y2 - mmdata->htPow*2.0)/2.0;
            float percent = yMovePixels/(F32)mmdata->htPow;
            {
                float yAdjust =  mmdata->lookatPos[1]*percent;
                yAdjust = (yAdjust)?yAdjust:height*.75*percent;
                mmdata->lookatPos[1] +=yAdjust;
                mmdata->headshotCameraPos[1] += yAdjust;
            }
            if(sizeOfPictureX/(F32)mmdata->wdPow >= 1.95 &&
                sizeOfPictureY/(F32)mmdata->htPow>= 1.95)
                mmdata->lookatPos[2] *= 2.5;
            else if(mmdata->e->seq->type->selection == SEQ_SELECTION_LIBRARYPIECE && MAX(xRatio, yRatio) < 1.25)
            {
                mmdata->lookatPos[2] *= MAX(xRatio, yRatio) * 1.25;
            }
            else
            {
                mmdata->lookatPos[2] *= MAX(xRatio, yRatio) * 1.1;
            }
            sizeOfPictureX = mmdata->wdPow;
            sizeOfPictureY = mmdata->htPow;
            getMMSeqImage(&pixbuf, mmdata->e->seq, mmdata->headshotCameraPos, mmdata->lookatPos, mmdata->yOffset, mmdata->rotation, (F32) headShotFovMagic,
                &sizeOfPictureX, &sizeOfPictureY, &mmdata->x1, &mmdata->x2, &mmdata->y1, &mmdata->y2, resetImageBoundaries, NULL, 0);
        }

        if(!resetImageBoundaries)
        {
            mmdata->x1 = tempX1;
            mmdata->x2 = tempX2;
            mmdata->y1 = tempY1;
            mmdata->y2 = tempY2;
        }



        //Bind this into a Texture

        
        if(!stillImage)
        {
            stillImage = atlasGenTexture(pixbuf, sizeOfPictureX, sizeOfPictureY, PIX_RGBA|PIX_DONTATLAS|PIX_NO_VFLIP, "mmShot_still");
        }
        else
            atlasUpdateTexture(stillImage, pixbuf, sizeOfPictureX, sizeOfPictureY, PIX_RGBA|PIX_DONTATLAS|PIX_NO_VFLIP);

        if(!mmdata->bind)
        //we need a different texture for the moving image.
            mmdata->bind = atlasGenTexture(pixbuf, sizeOfPictureX, sizeOfPictureY, PIX_RGBA|PIX_DONTATLAS|PIX_NO_VFLIP, "mmShot_motion");
        


        mmdata->wdPow*=2;
        mmdata->htPow*=2;
        estrDestroy(&pixbuf);
        mmdata->e->noDrawOnClient = 1;
        return stillImage;
}


int picturebrowser_updateCostume( const cCostume * costume, MMPinPData *mmdata, int force)
{
    if(!mmdata || !costume)
        return 0;


    if(!mmdata->e || mmdata->e->costume != costume ||force)
    {
        Vec3 newScale = {1,1,1};
        if(mmdata->e)
        {
            mmdata->e->waitingForFadeOut =1;
            mmdata->e->currfade = 0;
            entFree(mmdata->e);
        }
        mmdata->e = entCreate("");
        mmdata->e->costume = costume;
        costume_Apply( mmdata->e );
        animSetHeader( mmdata->e->seq, 0 );
        manageEntityHitPointTriggeredfx(mmdata->e);
        mmdata->rotation = 0;
        imagecapture_passSeqIntro(mmdata->e->seq);
        copyMat4( unitmat, mmdata->e->seq->gfx_root->mat );
        copyVec3(newScale, mmdata->e->seq->currgeomscale);
        return 1;
    }
    return 0;
}

int updateStillImage(PictureBrowser *pb, AtlasTex * still)
{
    if(!pb || !still)
        return 0;
    if(eaSize(&pb->elements))
    {
        eaDestroy(&pb->elements);
    }
    eaPush(&pb->elements, still);
    //update size of the pbrowser
    {
        F32 floatScale = pb->sc * MIN((pb->maxWd/still->width),(pb->maxHt/still->height));
        //if smaller than the window, don't expand.
        floatScale = MIN(1, floatScale);

        pb->wd = floatScale*still->width+WINDOW_SIDES_BUFFER*2;
        pb->ht = floatScale*still->height+A_BUTTON_HEIGHT + WINDOW_SIDES_BUFFER*2;
    }
    return 1;
}

void picturebrowser_setPlay(PictureBrowser *pb, int play)
{
    if(play && pb->mmdata && pb->mmdata->motionAvailable)
    {    
        mmPictureChosenIdx = pb->idx;
    }
    else
    {
        AtlasTex *update =0;
        if(mmPictureChosenIdx == pb->idx)
            mmPictureChosenIdx = -1;

        if(pb->elements && pb->elements[0])
            update = pb->elements[0];
        
        update = picturebrowser_updateImageCaptureData(pb->mmdata, 256,256, 0, update);//((unsigned int)pb->wd), ((unsigned int)pb->ht));
        if(update !=white_tex_atlas)
            updateStillImage(pb, update);
    }

    pb->type = play;
}

void picturebrowser_setCostumeVariation(PictureBrowser *pb, int index)
{
    int numChanges = eaiSize(&pb->costumeChanges);
    if (!numChanges)
        return;
    int id = pb->currentCostume = index % numChanges;
    AtlasTex *update = 0;
    const cCostume *c= npcDefsGetCostume(eaiGet(&pb->costumeChanges, id), 0);


    pb->changed = picturebrowser_updateCostume( c, pb->mmdata, 0);
    if(pb->changed)
    {
        if(pb->elements && pb->elements[0])
            update = pb->elements[0];
        pb->mmdata->yOffset = -100;
        update = picturebrowser_updateImageCaptureData(pb->mmdata, 256,256, 1, update);//((unsigned int)pb->wd), ((unsigned int)pb->ht));
        if(update!=white_tex_atlas)
            updateStillImage(pb, update);
    }
}

void picturebrowser_setMMImage( PictureBrowser *pb, int *npcIndex, int *costumeList )
{
    const cCostume *c = 0;
    AtlasTex *update = 0;

    if(!pb || !npcIndex)
        return;
    if(pb->costumeChanges == costumeList && pb->mmdata && pb->mmdata->e)    //just record the current index
    {
        *npcIndex = pb->currentCostume;
        return;
    }
    pb->costumeChanges = costumeList;
    pb->currentCostume = *npcIndex;

    c= npcDefsGetCostume(eaiGet(&pb->costumeChanges, pb->currentCostume), 0);
    

    pb->changed = picturebrowser_updateCostume( c, pb->mmdata, 0);
    if(pb->changed)
    {
        if(pb->elements && pb->elements[0])
            update = pb->elements[0];
        pb->mmdata->yOffset = -100;
        update = picturebrowser_updateImageCaptureData(pb->mmdata, 256,256, 1, update);//((unsigned int)pb->wd), ((unsigned int)pb->ht));
        if(update!=white_tex_atlas)
            updateStillImage(pb, update);
        if(pb->mmdata->e)
            pb->mmdata->e->noDrawOnClient = 1;
    }
}

void picturebrowser_setMMImageCostume( PictureBrowser *pb, const cCostume *c )
{
    AtlasTex *update = 0;
    if(!pb)
        return;

    pb->changed = picturebrowser_updateCostume( c, pb->mmdata, 0);
    if(pb->changed)
    {
        pb->mmdata->yOffset = -100;
        pb->costumeChanges = 0;
        pb->currentCostume = 0;
        if(pb->elements && pb->elements[0])
            update = pb->elements[0];
        update = picturebrowser_updateImageCaptureData(pb->mmdata, 256,256, 1, update);//((unsigned int)pb->wd), ((unsigned int)pb->ht));
        if(update !=white_tex_atlas)
            updateStillImage(pb, update);
    }
}


AtlasTex *getCurrentImage(PictureBrowser *pb)
{
    AtlasTex *display = NULL, *update = NULL;
    if(!pb)
        return NULL;


    if(pb->mmdata && pb->type == PICTBROWSE_MOVIE)
    {
        picturebrowser_updateFrame(pb, pb->mmdata);
        display = pb->mmdata->bind;
        if((mmPictureChosenIdx != pb->idx && mmPictureAllowAnimations == PICTBROWSE_ONE_ANIMATION) ||
            mmPictureAllowAnimations == PICTBROWSE_NO_ANIMATIONS)
        {
            //int element;
            pb->type = PICTBROWSE_IMAGE;    
            picturebrowser_setPlay(pb, 0);
            
        }

    }
    else if(pb->elements && eaSize(&pb->elements))
    {
        int element;    

        element = pb->currChoice%eaSize(&pb->elements);

        display = pb->elements[element];
    }
    return display;
}

const SeqMove * tryFindMoveBySeqName(SeqInst *seq, char *name)
{
    int seqId = seqGetStateNumberFromName(name);
    if(seqId >=0)
    {
        seqSetState( seq->state, TRUE, seqId);
        return seqForceGetMove( seq, seq->state, 0 );
    }
    return NULL;
}

void picturebrowser_setEmoteByName(PictureBrowser *pb, char * emote)
{
    Entity *e;
    const SeqMove * move = 0;
    U16 idx = 0;
    if(!pb || !emote)
        return;
    if(!imageCapture_verifyMMdata(pb->mmdata))    //if we don't have a sequence, we can't do anything.
        return;

    e = pb->mmdata->e;

    //the following is necessary to get the sequencer to consistently 
    move = getFirstSeqMoveByName(e->seq, "READY");
    if(move)
        e->seq->animation.move = move;
    seqClearState(e->aiAnimListState);
    seqClearState(pb->mmdata->e->seq->state);
    alSetAIAnimListOnEntity( e, emote );
    seqSetOutputs( e->seq->state, e->aiAnimListState );
    move = seqProcessInst( e->seq, TIMESTEP );

    if(move)
    {
        picturebrowser_setPlay(pb, 1);
        picturebrowser_updateCostume( e->costume, pb->mmdata, 1);
        //seqSetState( e->seq->state, TRUE, seqId);
        idx = move->raw.idx;
        e->next_move_idx = idx;
        e->move_change_timer = 1;
    }
}

int picturebrowser_drawNavButton(float x, float y, float z, float wd, float ht, AtlasTex * icon, int *buttonColor)
{
    UIBox uibox;
    float subx = x, suby = y, subht = ht, subwd = wd;
    float subsc = 1;
    CBox cbox = {0};

    uibox.x = subx;
    uibox.y = suby;
    uibox.width = subwd;
    uibox.height = subht;

    uiBoxToCBox(&uibox, &cbox);

    if(icon)
    {
        subwd = MIN(icon->width, subwd - PIX2*2);
        subht = MIN(icon->height, subht - PIX2*2);
        subsc = MIN(subwd/icon->width, subht/icon->height);
        suby = y+(ht - icon->height*subsc)/2.0;
        subx = x+(wd - icon->width*subsc)/2.0;
    }

    if(icon)
        display_sprite(icon, subx, suby, z+2, subsc, subsc, 0xffffffff);

    //if hover
    if(mouseCollision( &cbox ) && buttonColor)
    {
        *buttonColor = 0xffffffaa;
        if(mouseDownHit(&cbox, MS_LEFT))
        {
            if(buttonColor)
                *buttonColor = 0xffffffff;
            return 1;
        }
        collisions_off_for_rest_of_frame = 1;
    }

    return 0;
}

static int s_isPlaying(PictureBrowser *pb)
{
    if(!pb || !pb->mmdata)
        return 0;

    if(pb->type != PICTBROWSE_MOVIE || (mmPictureChosenIdx != pb->idx && mmPictureAllowAnimations == PICTBROWSE_ONE_ANIMATION) || mmPictureAllowAnimations ==PICTBROWSE_NO_ANIMATIONS)
        return 0;

    return 1;
}

void picturebrowser_setAnimationAvailable(PictureBrowser *pb, int allowAnimation)
{
    if(!pb || !pb->mmdata)
        return;
    pb->mmdata->motionAvailable = allowAnimation;
}

int picturebrowser_display( PictureBrowser *pb )
{
    int count, color = CLR_WHITE;
    //CBox box;//, box2;
    //AtlasTex *arrowLeft, *arrowRight;
    AtlasTex *display = 0;
    float tx, ty, tz, tht, twd, sc = 1.f;
    float subwd, subht;
    int col, back_color;
    UIBox uibox;
    CBox cbox;
    float floatScale;
    int turning = 0;    //using in place of collisions_off_for_rest_of_frame so that the box will still show up
    int turnHover = 0;    //are we hovering over the turn buttons?
    static int isClicking = 0;//are we still mid click?
    int textColor = 0xFFFFFFFF;
    AtlasTex *arrowLeft = atlasLoadTexture( "chat_separator_arrow_left.tga" );
    AtlasTex *arrowRight = atlasLoadTexture( "chat_separator_arrow_right.tga" );
    F32 buttonWidth = arrowLeft->width+2*PIX2;



    if (!pb || pb->maxHt <= 0 || pb->maxWd <=0)
    { //We're trying to display an uninitialized picture browser
        return -1;
    }

    display = getCurrentImage(pb);
    if(display)
    {
        floatScale = pb->sc * MIN((pb->maxWd/display->width),(pb->maxHt/display->height));
        //if smaller than the window, don't expand.
        floatScale = MIN(1, floatScale);
        subwd = floatScale*display->width;
        subht = floatScale*display->height;

        pb->wd = subwd+floatScale *WINDOW_SIDES_BUFFER*2;
        pb->ht = subht+floatScale *WINDOW_SIDES_BUFFER*2;
        if(pb->mmdata && pb->mmdata->motionAvailable)
            pb->ht +=floatScale*A_BUTTON_HEIGHT;
    }
    else return 0;


    // if its linked to a get offset coordinates
    if( pb->wdw )
    {
        float wx, wy, wz, wwd,wht;

        if( window_getMode( pb->wdw ) != WINDOW_DISPLAYING )
            return -1;

        window_getDims( pb->wdw, &wx, &wy, &wz, &wwd, &wht, &sc, &col, &back_color );
        tx = pb->x*sc + wx;
        ty = pb->y*sc + wy;
        tz = pb->z + wz;
        tht = pb->ht;
        twd = pb->wd;
    }
    else
    {
        tx = pb->x;
        ty = pb->y;
        tz = pb->z;
        tht = pb->ht;
        twd = pb->wd;
        col = back_color = -1;
        if( pb->sc )
            sc = pb->sc;
    }
    tx += pb->sc*WINDOW_SIDES_BUFFER;
    ty += pb->sc*WINDOW_SIDES_BUFFER;

    // load the arrow

    if(pb->type == PICTBROWSE_MOVIE || eaSize(&pb->elements)>1)
    {
        Vec2 ul = {tx, ty};
        Vec2 lr = {tx+display->width*floatScale, ty+display->height*floatScale};
        int turn = picturebrowse_drawSpinButtons(ul, lr, tz+3);
        if(turn >=0)
        {
            picturebrowser_pan( pb, turn);
            turning = 1;
        }
        if(turn >=0 || turn == PICTBROWSE_HOVER)
            turnHover = 1;
    }

    // clear change notification
    pb->changed = false;

    font_color( CLR_WHITE, CLR_WHITE );
    font( &game_12 );


    //setup the uibox.
    {
        uibox.x = pb->x;
        uibox.y = pb->y;
        uibox.width = pb->wd;
        uibox.height = pb->ht+sc*A_BUTTON_HEIGHT;
        clipperPushRestrict(&uibox);
        if(display)
        {
            /*tx += (subwd-display->width*floatScale)*.5;
            ty += (subht-display->height*floatScale)*.5;*/
            display_sprite(display, tx, ty, tz+2, floatScale, floatScale, 0xffffffff);
            ty += display->height*floatScale;
        }


        BuildCBox(&cbox, pb->x, pb->y, pb->wd, pb->ht);
        if(eaiSize(&pb->costumeChanges)>1)
        {
            //draw the costume selector buttons.
            int leftColor, rightColor, centerColor = PB_MM_COLOR;
            leftColor= rightColor = PB_MM_COLOR;
            if(pb->currentCostume >0 && picturebrowser_drawNavButton(pb->x, pb->y, tz+12, pb->sc*buttonWidth, pb->ht, arrowLeft,&leftColor))
            {
                picturebrowser_setCostumeVariation(pb, pb->currentCostume-1);
                
            }
            if(pb->currentCostume < eaiSize(&pb->costumeChanges)-1 && picturebrowser_drawNavButton(pb->x+pb->wd- pb->sc*buttonWidth, pb->y, tz+12, pb->sc*buttonWidth, pb->ht, arrowRight, &rightColor))
            {
                picturebrowser_setCostumeVariation(pb, pb->currentCostume+1);
                collisions_off_for_rest_of_frame =1;
            }
            if((mouseCollision(&cbox) ||  s_isPlaying(pb))&& pb->mmdata && pb->mmdata->motionAvailable) 
            {
                centerColor = PB_MM_COLOR;    //eventually switch to a selected color.
            }
            drawFlatThreeToneFrame(PIX3, R10, pb->x, pb->y,  pb->z, pb->wd, pb->ht, pb->sc, leftColor, centerColor,rightColor);
        }

        if((mouseCollision(&cbox) || s_isPlaying(pb))&& eaiSize(&pb->costumeChanges) <=1 && pb->mmdata && pb->mmdata->motionAvailable)
            drawFlatFrame(PIX3, R10, pb->x, pb->y,  pb->z, pb->wd, pb->ht, pb->sc, PB_MM_COLOR, PB_MM_COLOR);
        if(mouseCollision(&cbox) && pb->mmdata && pb->mmdata->motionAvailable)
        {
            if(!turnHover)
                textColor = 0x55FF55FF;

            if(isDown( MS_LEFT ) && !turning && !isClicking)
            {
                isClicking = 1;
                drawFlatFrame(PIX3, R10, pb->x, pb->y,  pb->z, pb->wd, pb->ht, pb->sc, PB_MM_COLOR, PB_MM_COLOR);
                picturebrowser_setPlay(pb, !pb->type);
            }
            else if (!isDown( MS_LEFT ))
                isClicking = 0;
        }

        if(pb->mmdata && pb->mmdata->motionAvailable)
        {
            char *status;
            


            if(pb->type)
                status = "AnimStop";
            else
                status = "AnimStart";

            font_set(&game_12, 0, 0, 1, 1, textColor, textColor);

            cprntEx(tx+subwd/2, pb->y+pb->ht-pb->sc*WINDOW_SIDES_BUFFER, tz+3, sc, sc, CENTER_X, status );
        }

        
        count = eaSize(&pb->elements);

        clipperPop();
    }
    return pb->y + subht;
}

// Moves the picture browser's selection left or right
void picturebrowser_pan( PictureBrowser *pb, int pan)
{
    int id;
    if(pb->state== PICTBROWSE_MOVING)    //don't change if we're mid move
        return;

    id = pb->currChoice;
    if(pan == PICTBROWSE_LEFT)
    {
        if(pb->mmdata)
            pb->mmdata->rotation-= (TIMESTEP)*.1;
        id--;
    }
    else if(pan == PICTBROWSE_RIGHT)
    {
        if(pb->mmdata)
            pb->mmdata->rotation+= (TIMESTEP)*.1;
        id++;
    }
    //else
    //    id = id;    //do nothing.

    if(pb->elements)
    {
        id %= eaSize(&pb->elements);
        picturebrowser_setId(pb, id);
    }
}
