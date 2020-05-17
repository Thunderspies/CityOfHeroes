/* imageCapture.h
* Contains functions used in acquiring and saving 2D images
* of objects in the CoH world.
*/
#include "graphics/imageCapture.h"
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/wininclude.h>
#include <string.h>
#include <time.h>
#include "seq/seq.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "assert.h"
#include "seq/anim.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/HashFunctions.h>
#include "graphics/font.h"
#include "seq/animtrack.h"
#include "seq/seqanimate.h"
#include "seq/animtrackanimate.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/costume.h"
#include "seq/tricks.h"
#include "render/tex.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/entity.h"
#include "entity/motion.h"
#include "graphics/gfxwindow.h"
#include "graphics/FX/fxlists.h"
#include "graphics/camera.h"
#include "render/render.h"
#include "sound/sound.h"
#include "graphics/font.h"
#include "cmdparse/cmdgame.h"
#include "graphics/FX/fx.h"
#include "graphics/light.h"
#include <utilitieslib/utils/timing.h>
#include "render/model_cache.h"
#include "win/win_init.h" //moving
#include "graphics/gfx.h"
#include "render/tex_gen.h"
#include "graphics/textureatlas.h"
#include "player/player.h"
#include "graphics/sun.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "seq/gfxtree.h"
#include "gameComm/npc.h"        // For NPC structure definition
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "seq/seqregistration.h"
#include "render/renderprim.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "group/group.h"
#include "graphics/groupMiniTrackers.h"
#include <utilitieslib/components/estring.h>

#include <utilitieslib/utils/tga.h>
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "graphics/gfxwindow.h"
#include "entity/entrecv.h"
#include "render/pbuffer.h"
#include "render/renderprim.h"
#include "render/renderUtil.h"
#include "UI/uiAutomap.h"
#include "group/groupfileload.h"
#include "group/groupnetrecv.h"
#include "bases/bases.h"
#include "UI/uiBaseInput.h"
#include "render/tex.h"
#include "render/pbuffer.h"
#include "graphics/FX/fxgeo.h"
#include "graphics/gfxLoadScreens.h"
#include "group/groupMetaMinimap.h"

extern bool g_doingHeadShot;
extern PBuffer pbufHeadShot;
static PBuffer debug_pbufOld;
extern FxEngine fx_engine;

#ifndef FRAME_TIMESTEP
#define FRAME_TIMESTEP 0.5
#endif

#ifndef FRAME_TIMESTEP_INIT
#define FRAME_TIMESTEP_INIT 150
#endif

#ifndef FRAMES_BEFORE_CAPTURE
#define FRAMES_BEFORE_CAPTURE (hasFx?10:2)
#endif

#ifndef MAP_IMAGE_SIZE
#define MAP_IMAGE_SIZE 256
#endif

#define MAP_IMAGES_DIRECTORY "texture_library/MAPS/MMMaps"
#define MM_IMAGE_BUFFER_SIZE 15
int getCurrentMapImages(unsigned int, unsigned int, U8 ***);
void loadMapForImages(char *);


//ImageCapture_WriteMapImages: Creates bird's eye images of a map for the Mission Maker
//if the directory is NULL, this will just write the image of the current map
//with the given filename
//textures are produced with the following name template:
//texture_library/MAPS/MMMaps/<name of map>-floor_<2 digit floor #>

void ImageCapture_WriteMapImages(char *filename, char *directory, int checkin)
{
#ifdef CLIENT
//    U8 * * mapsImages = 0;
//    int nImages;
//    TexReadInfo imageInfo;
//    int i;
//    char        fname[1000];
    char *s;
    char mapName[1000];
    
    
    //example: char mapname[] = "C:\\game\\data\\maps\\Missions\\Outdoor_Missions\\V_Outdoor_LongBow_CargoBase\\V_Outdoor_LongBow_CargoBase.txt";

    //FIRST COUNT THE NUMBER OF '.'s IN THE FILENAME!  More than one period will result in everyone's client crashing.  Not good.
    s = strchr(filename, '.');//point to the first period.
    if(s != NULL)
        s = strchr(s+1, '.');//point to a second period?
    if(s != NULL)
        return;


    //get the name of the map file to load
    if(directory)
    {
        //prepare directory name
        sprintf(mapName,"%s/%s",directory,filename);
        s = strrchr(mapName,'.');
        strcpy(s,".txt");

        loadMapForImages(mapName);
    }
    else 
    {
        //if the filename has directory information attached, remove it.
        s = strrchr(filename, '/');
        if(!s)
            s = strrchr(filename, '\\');
        if(s)
            filename = s+1;

        sprintf(mapName,"%s",filename);
    }
    
    minimap_saveHeader(mapName);
#endif
}




//TODO: don't do this on every pixel.
void clearImageBorder(U8 * origBuffer, unsigned int width, unsigned int height, 
                      unsigned int left, unsigned int right, 
                      unsigned int top, unsigned int bottom)
{
    unsigned int i, j;
    U8 * buffer = origBuffer;
    F32 percent;
    U8 luminance;
    U8 high = 0;

    if(bottom < top || left > right)
        return;
    if(left < 0)
        left = 0;
    if(right > width)
        right = width-1;
    if(top < 0)
        top = 0;
    if(bottom > height)
        bottom = height-1;

    for (i = top; i < top+MM_IMAGE_BUFFER_SIZE && i < bottom; i++)
    {
        for(j = left; j < right; j++)
        {
            buffer = origBuffer +(i*width + j) * sizeof(U8)*4;

            percent = (i-top)/(F32)MM_IMAGE_BUFFER_SIZE;
            luminance = (U8)(percent * 255);
            buffer[3] = MIN(buffer[3],luminance);
        }
    }

    for (i = bottom-MM_IMAGE_BUFFER_SIZE-1; i < bottom; i++)
    {
        for(j = left; j < right; j++)
        {
            buffer = origBuffer +(i*width + j) * sizeof(U8)*4;

            percent = (i-(bottom-MM_IMAGE_BUFFER_SIZE-1))/(F32)MM_IMAGE_BUFFER_SIZE;
            percent = 1.0-percent;
            luminance = (U8)(percent * 255);

            buffer[3] = MIN(buffer[3],luminance);
        }
    }

    //top to bottom
    for (i = left; i < left+MM_IMAGE_BUFFER_SIZE; i++)
    {
        for(j = top; j < bottom; j++)
        {
            buffer = origBuffer + (j * width + i)*4;
            percent = (i-left)/(F32)MM_IMAGE_BUFFER_SIZE;
            luminance = (U8)(percent * 255);
            buffer[3] = MIN(buffer[3],luminance);
        }
    }

    for (i = right-MM_IMAGE_BUFFER_SIZE; i < right; i++)
    {
        for(j = top; j < bottom; j++)
        {
            buffer = origBuffer + (j * width + i)*4;
            percent = (i-(right-MM_IMAGE_BUFFER_SIZE))/(F32)MM_IMAGE_BUFFER_SIZE;
            percent = 1.0-percent;
            luminance = (U8)(percent * 255);
            buffer[3] = MIN(buffer[3],luminance);
        }
    }
}

void hasAdditiveParticles(FxObject* fx, int* additive)
{
    int j;
    FxGeo *fxgeo;
    if(!fx || !additive)
        return;

    for( fxgeo = fx->fxgeos; fxgeo ; fxgeo = fxgeo->next) 
    {
        for(j = 0; j < fxgeo->psys_count; j++)
        {
            ParticleSystem *system = fxgeo->psys[j];
            if(system && system->sysInfo)
            {
                if(system->sysInfo->blend_mode == PARTICLE_ADDITIVE)
                {
                    *additive = 1;//PARTICLE_LUMINANCE;
                }
            }
        }
    }
}




//swap R and B
void swizzleImageForCOHColor(U8 * origBuffer, unsigned int width, unsigned int height)
{
    unsigned int i, j;
    U8 * buffer = origBuffer;
    U8 temp;
    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            temp = buffer[0];
            buffer[0] = buffer[2];
            buffer[2] = temp;
            buffer = buffer + sizeof(U8)*4;
        }
    }
}

void clearImageByLuminance(U8 * origBuffer, unsigned int width, unsigned int height, U8 min, U8 max, U8 cutoff)
{
    unsigned int i, j;
    U8 * buffer = origBuffer;
    F32 scale;
    U8 luminance;
    int add;
    U8 high = 0;
    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            luminance = (U8)(.3*buffer[0] + .59*buffer[1] + .11*buffer[2]);
            add = luminance;
            //if there is content above the cutoff, add min to it.
            if(luminance >= cutoff)
                add += min - cutoff;    //anything cutoff or above should be min or bigger
            luminance = (U8)(MIN(255, add));

            //track the largest value.  We'll use this to scale the range of values.
            if(luminance > high)
                high = luminance;

            buffer[3] = luminance;
            buffer = buffer + sizeof(U8)*4;
        }
    }

    //scale so that the largest value = 255.
    if(high == 0)
        return;
    scale = max/(F32)high;
    buffer = origBuffer;

    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(buffer[3] >= cutoff)
                buffer[3] *= scale;
            else
                buffer[3] = 0;
            buffer = buffer + sizeof(U8)*4;
        }
    }
}

void clearImageByBlueness(U8 * origBuffer, unsigned int width, unsigned int height )
{
    unsigned int i, j;
    U8 * buffer = origBuffer;

    for (i = 0; i < height; i++) 
    {
        for(j = 0; j < width; j++)
        {
               if( buffer[0] && !(buffer[0] == buffer[1] && buffer[0] == buffer[2] ) )
            {
                buffer[3] = (-buffer[0]+255);
                buffer[0] = 0;
            }

            buffer = buffer + sizeof(U8)*4;
        }
    }
}

void flipImageVertically(U8 * buffer, unsigned int width, unsigned int height )
{
    unsigned int i;
    U8 temp[256*4];

    if(width > 256)
        return;

    for (i = 0; i < height/2; i++) 
    {
        memcpy( temp, buffer + i*(width*4), (width)*4 );
        memcpy( buffer + i*width*4, buffer + (height-i-1)*width*4, width*4 );
        memcpy( buffer + (height-i-1)*width*4, temp, width*4 );
    }
}

//blends two images with alpha of the given size, placing the combination in dst.
void placeImageOverImage(U8 * src, U8 * dst, unsigned int width, unsigned int height)
{
    unsigned int i, j, k;
    //unsigned int temp;
    F32 percent;
    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            src = src + sizeof(U8)*4;
            dst = dst + sizeof(U8)*4;
            if(dst[3] < 250 && src[3] != 0)
            {
                percent = (dst[3]/255.0);
                for(k = 0; k < 4; k++)
                {
                    dst[k] = (U8)(percent * dst[k] + (1-percent)*src[k]);
                }
            }
        }
    }
}

//search a row in the image for any alpha value.
//returns 1 for true, 0 for false.
#define MM_MIN_ALPHA_THRESHOLD 75
int checkImageRowForContent(U8 * img, unsigned int width, unsigned int height, unsigned int row)
{
    unsigned int i;
    unsigned int rowStartIndex = row*width;

    if(row >= height)
        return -1;

    //go to the first spot.
    img += rowStartIndex *sizeof(U8)*4;

    for (i = 0; i < width; i++)
    {
        if(img[3] > MM_MIN_ALPHA_THRESHOLD)
            return 1;
        img += sizeof(U8)*4; 
    }
    return 0;
}

//search a column in the image for any alpha value.
//returns 1 for true, 0 for false.
int checkImageColumnForContent(U8 * img, unsigned int width, unsigned int height, unsigned int column)
{
    unsigned int i;

    if(column >= width)
        return -1;

    //go to the first spot.
    img += column *sizeof(U8)*4;

    for (i = 0; i < height; i++)
    {
        if(img[3] > MM_MIN_ALPHA_THRESHOLD)
            return 1;
        img += width*sizeof(U8)*4;
    }
    return 0;
}

//returns a number between 0 and height-1 indicating the highest position with content.  
int getTopY(U8 * img, unsigned int width, unsigned int height)
{
    unsigned int i;
    for(i = 0; i < height; i++)
    {
        if(checkImageRowForContent(img, width, height, i))
        {
            return i;
        }
    }
    return 0;
}

//returns a number between 0 and height-1 indicating the lowest position with content.
int getBottomY(U8 * img, unsigned int width, unsigned int height)
{
    unsigned int i;
    for(i = height-1; i > 0; i--)
    {
        if(checkImageRowForContent(img, width, height, i))
        {
            return i;
        }
    }
    return 0;
}

//returns a number between 0 and height-1 indicating the highest position with content.  On error returns -1;
int getLeftX(U8 * img, unsigned int width, unsigned int height)
{
    unsigned int i;
    for(i = 0; i < width; i++)
    {
        if(checkImageColumnForContent(img, width, height, i))
        {
            return i;
        }
    }
    return 0;
}

//returns a number between 0 and height-1 indicating the lowest position with content.  On error returns -1;
int getRightX(U8 * img, unsigned int width, unsigned int height)
{
    int i;
    for(i = width-1; i >= 0; i--)
    {
        if(checkImageColumnForContent(img, width, height, i))
        {
            return i;
        }
    }
    return 0;
}

int hasContent(U8 * img, unsigned int width, unsigned int height)
{
    int i;
    for(i = width-1; i >= 0; i--)
    {
        if(checkImageColumnForContent(img, width, height, i))
        {
            return 1;
        }
    }
    return 0;
}

void copyMMImage(U8 * src, unsigned int srcWidth, unsigned int srcHeight, U8 *dst, unsigned int dstWidth, 
                 unsigned int dstHeight, unsigned int leftX, unsigned int topY, int srcToDst)
{
    U8 * currentIn, *currentOut;
    int rowsWritten = 0;
    int smallerToLargerW = srcWidth<=dstWidth;
    int smallerToLargerH = srcHeight<=dstHeight;
    int rowsToWrite = (smallerToLargerH)?(MIN(srcHeight, dstHeight-topY)): 
        (MIN(dstHeight, srcHeight-topY));
    int colsToWrite = (smallerToLargerW)?(MIN(srcWidth, dstWidth-leftX)): 
        (MIN(dstWidth, srcWidth-leftX));

    while(rowsWritten<rowsToWrite)
    { 
        int position = 4*srcWidth*(rowsWritten + ((smallerToLargerH)?0:topY))+ ((smallerToLargerW)?0:leftX*4);
        currentIn = (src + position);
        position = 4*dstWidth*(rowsWritten + ((smallerToLargerH)?topY:0)) + ((smallerToLargerW)?leftX*4:0);
        currentOut = (dst + position);
        memcpy(currentOut, currentIn, colsToWrite*4);
        rowsWritten++;
    }
}

//blends the src pixel onto the dst pixel using alpha to determine the proportion of each.
void blendPixels(U8 *src, U8 *dst)
{
    int i;
    //(Rs As + Rd (1 - As), Gs As + Gd (1 - As), Bs As + Bd (1 - As), As As + Ad (1 - As))
    F32 percent = (src[3]/255.0);
    for(i = 0; i < 4; i++)
    {
        dst[i] = (U8)(src[i]*percent + dst[i]*(1-percent));
    }
}

//use additive blending with percentSrc of the src pixel on top of dst.
void addPixels(U8 *src, U8 *dst, F32 percentSrc)
{
    int i;
    int temp;
    for(i = 0; i < 4; i++)
    {
        temp = (int)(src[i]*percentSrc);
        temp += dst[i];
        if(temp > 255)
            i += 0;
        dst[i] = (U8)MIN(temp, 255);
    }
}


void imageCapture_copyOver(U8 *src, int srcW, int srcH, U8 *dst, int dstW, int dstH, unsigned int left, unsigned int top)
{
    copyMMImage(src, srcW, srcH, dst, dstW, dstH,left, top, 0);
}


static void imageCaptureUpdateImage(SeqInst *seq)
{
    int hasFx;
    int i;

    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(0);

    // Check to see if we have any FX on us
    hasFx = eaiSize(&seq->seqfx);
    for( i = 0 ; i < MAX_SEQFX ; i++ )
    {
        if( seq->const_seqfx[i] )
            hasFx = 1;
    }
    for( i = 0 ; i < eaiSize(&seq->seqcostumefx) ; i++ )
    {
        if( seq->seqcostumefx[i] )
            hasFx = 1;
    }

    if (rdr_caps.use_pbuffers)
        pbufMakeCurrent(&pbufHeadShot);

    rdrClearScreenEx(CLEAR_DEPTH|CLEAR_STENCIL|CLEAR_COLOR|CLEAR_ALPHA);
    rdrSetAdditiveAlphaWriteMask(1);
    
    gfxWindowReshapeForHeadShot(game_state.fov_custom);

    drawSortedModels_headshot(DSM_OPAQUE);
    drawSortedModels_headshot(DSM_ALPHA);

    //draw particles
    if(hasFx)
    {
        partRunStartup();
        TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP;
        fxDoForEachFx(seq, fxRunParticleSystems, NULL); 
        partRunShutdown();
    }
}

//you must have set up the entity you're taking the image of as well as the camera, lighting, etc.
//this function ONLY returns the pixels from the buffer.
//the buffer pointed to should already be allocated.
static void imageCaptureGetPixels(U8 * * buffer, int bWidth, int bHeight, Vec2 captureBottomLeft, SeqInst *seq)
{
    //Vec3 clear = {0.5,0.5,0.5};
    unsigned int total_size = bWidth* bHeight * 4;

    imageCaptureUpdateImage(seq);

    //Read the pixels

    if (rdr_caps.use_pbuffers) {
        U8 * pixdata =     pbufFrameGrab( &pbufHeadShot, *buffer, total_size );
        if(pixdata != *buffer)
        {
            imageCapture_copyOver(pixdata, pbufHeadShot.width, pbufHeadShot.height, *buffer, bWidth, bHeight, (pbufHeadShot.width-bWidth)/2, (pbufHeadShot.height-bHeight)/2);
            free(pixdata);
        }
    } else {
        rdrGetFrameBuffer(captureBottomLeft[0], captureBottomLeft[1], bWidth, bHeight, GETFRAMEBUF_RGBA, *buffer);
    }

    winMakeCurrent();
}

void getMMBounds(U8 * image, unsigned int width, unsigned int height, unsigned int * left, unsigned int * right, unsigned int * top, unsigned int * bot)
{
    //brute force approach-->creates problems when there are world particle effects we're not hiding
    *left = getLeftX(image, width, height);
    *right = getRightX(image, width, height);
    *top = getTopY(image, width, height);
    *bot = getBottomY(image, width, height);

}

#define FULL_BODY_SHOT_REQUIRED_MULTI 2
#define FULL_BODY_SHOT_DESIRED_MULTI 8

Entity * debugMMEntity;
int debugMMGameState;

void centerMapImage(unsigned int width, unsigned int height, U8 * imageIn, U8 * imageIn2, U8 * imageIn3 )
{
    U8 * tempMap = 0, *tempMap2 = 0, *tempMap3 = 0;
    unsigned int x1,x2,y1,y2;
    int newWidth, newHeight;

    estrCreate(&tempMap);
    estrConcatFixedWidth(&tempMap, imageIn, (width)*(height)*4);

    estrCreate(&tempMap2);
    estrConcatFixedWidth(&tempMap2, imageIn2, (width)*(height)*4);

    estrCreate(&tempMap3);
    estrConcatFixedWidth(&tempMap3, imageIn3, (width)*(height)*4);

    clearImageByLuminance(tempMap, width, height, 255, 255, 10); 
    getMMBounds(tempMap, width, height, &x1, &x2, &y1, &y2);
    newWidth = x2-x1+1;
    newHeight = y2-y1+1;

    estrSetLength(&tempMap, (newWidth)*(newHeight)*4);
    estrSetLength(&tempMap2, (newWidth)*(newHeight)*4);
    estrSetLength(&tempMap3, (newWidth)*(newHeight)*4);

    imageCapture_copyOver(imageIn, width, height, tempMap, newWidth, newHeight, x1, y1);
    memset(imageIn, 0, width*height*4);
    imageCapture_copyOver(tempMap, newWidth, newHeight, imageIn, width, height, (width-newWidth)/2, (height-newHeight)/2);
    estrDestroy(&tempMap);

    imageCapture_copyOver(imageIn2, width, height, tempMap2, newWidth, newHeight, x1, y1);
    memset(imageIn2, 0, width*height*4);
    imageCapture_copyOver(tempMap2, newWidth, newHeight, imageIn2, width, height, (width-newWidth)/2, (height-newHeight)/2);
    estrDestroy(&tempMap2);

    imageCapture_copyOver(imageIn3, width, height, tempMap3, newWidth, newHeight, x1, y1);
    memset(imageIn3, 0, width*height*4);
    imageCapture_copyOver(tempMap3, newWidth, newHeight, imageIn3, width, height, (width-newWidth)/2, (height-newHeight)/2);
    estrDestroy(&tempMap3);
}



U8 * getMapImage(unsigned int bufferWidth, unsigned int bufferHeight, unsigned int floorNumber, int flags)
{
    Vec3 clear = {0.0,1.0,0.0};
    Vec3 bg_color = {1.0, 1.0, 1.0};
    U8 * pixbuf = 0;    //hold the pixel buffer that will be returned
    
    int screenWidth, screenHeight;
    Vec2 captureBottomLeft;    //the position of the bottom left corner of the capture window in screen coordinates.

    int oldGameMode;
    unsigned int totalSize;

    //save old game mode, then set to game mode.
    oldGameMode = game_state.game_mode;
    toggle_3d_game_modes(SHOW_GAME);

    windowSize( &screenWidth, &screenHeight );

    totalSize = bufferWidth*bufferHeight*4;
    rdrSetBgColor(clear);
    rdrClearScreenEx(CLEAR_DEPTH|CLEAR_STENCIL|CLEAR_COLOR|CLEAR_ALPHA);
    
    //Screen Size
    if (rdr_caps.use_pbuffers)
    {    //taken directly from getThePixelBuffer
        initHeadShotPbuffer(bufferWidth, bufferHeight, FULL_BODY_SHOT_DESIRED_MULTI, FULL_BODY_SHOT_REQUIRED_MULTI);
        bufferWidth *= pbufHeadShot.software_multisample_level;
        bufferHeight *= pbufHeadShot.software_multisample_level; 
    }
    captureBottomLeft[0] = 0;//((F32)screenWidth)/2.0 - ((F32)bufferWidth)/2.0;
    captureBottomLeft[1] = 0;//((F32)screenHeight)/2.0 - ((F32)bufferHeight)/2.0;

        //hide everything but the sequence we're drawing.
    {
        GfxNode * node;

        for( node = gfx_tree_root ; node ; node = node->next )
        {
            if( node->child)
                node->flags |= GFXNODE_HIDE;
            if(!node->child && node->model)
                node->model->flags |= GFXNODE_HIDE;
        }
    }

    //Hide the player
    {
        Entity * player = playerPtr();
        if( player && player->seq && player->seq->gfx_root )
        {
            player->seq->gfx_root->flags |= GFXNODE_HIDE;
        }
    }

    //directly from getThePixelBuffer
    if (game_state.imageServerDebug) 
    {
        extern int glob_force_buffer_flip;
        if (rdr_caps.use_pbuffers)
            glob_force_buffer_flip = 0;
        else
            glob_force_buffer_flip = 1;
    }


    //prepare time for FX  --still experimental
    //gfxUpdateFrame( 0, 1, 0, 0); //need this one to set up loader
    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(0);

    //make sure we're ready to display
    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(1);
    Sleep(5);

    {
        Vec3 clear = {0.0,0.0,0.0};
        unsigned int total_size = bufferWidth* bufferHeight * 4;

        rdrSetBgColor(clear); 
        rdrClearScreenEx(CLEAR_DEPTH|CLEAR_STENCIL|CLEAR_COLOR|CLEAR_ALPHA);
        gfxUpdateFrame( 0, 3, 0); //3 -->avoid the FX and related world geometry loading problems
        pixbuf = malloc(total_size);
        
        automap_drawMap(bufferWidth, bufferHeight, floorNumber, flags);
        gfxUpdateFrame( 0, 3, 0); //3 -->avoid the FX and related world geometry loading problems

        if (rdr_caps.use_pbuffers)
        {
            pbufFrameGrab( &pbufHeadShot, pixbuf, total_size );
        } 
        else 
        {
            rdrGetFrameBuffer(captureBottomLeft[0], captureBottomLeft[1], bufferWidth, bufferHeight, GETFRAMEBUF_RGBA, pixbuf);
        }
    }

    gfxWindowReshape(); //Restore
    toggle_3d_game_modes(oldGameMode);

    if (rdr_caps.use_pbuffers) 
    {
        winMakeCurrent();
    }

    rdrNeedGammaReset(); // Because people were reporting gamma going screwy after headshots taken.

    PERFINFO_AUTO_STOP();

    return pixbuf;
}


//get an array of images representing each floor in the map
int getCurrentMapImages(unsigned int imageWidth, unsigned int imageHeight, U8 ***images)
{
    int num_images = 3*automap_getSectionCount();
    U8 **floorImages = *images;
    int i;

    if(num_images)
        floorImages = (U8 * *)malloc(sizeof(U8 *) * num_images);
    else
        floorImages = 0;

       for(i = 0; i < num_images; i+=3)     
    {
        int floor = i/3;
        floorImages[i] = getMapImage(imageWidth, imageHeight, floor, 0);
        //only do this if we're not an outdoor map.
        flipImageVertically( floorImages[i], imageWidth, imageHeight );
        clearImageByLuminance(floorImages[i], imageWidth, imageHeight, 255, 255, 10);
        swizzleImageForCOHColor(floorImages[i], imageWidth, imageHeight);

        floorImages[i+1] = getMapImage(imageWidth, imageHeight, floor, 1);
        flipImageVertically( floorImages[i+1], imageWidth, imageHeight );
        clearImageByBlueness(floorImages[i+1], imageWidth, imageHeight ); 

        floorImages[i+2] = getMapImage(imageWidth, imageHeight, floor, 2);
        flipImageVertically( floorImages[i+2], imageWidth, imageHeight );
        clearImageByBlueness(floorImages[i+2], imageWidth, imageHeight );

        centerMapImage(imageWidth, imageHeight, floorImages[i], floorImages[i+1], floorImages[i+2]);
    }
    if(!num_images && mapState.mission_type == 3)    //3 = MAP_OUTDOOR_MISSION
    {
        floorImages = (U8 **)malloc(sizeof(U8 *)*3);
        floorImages[0] = getMapImage(imageWidth, imageHeight, 0, 0);
        flipImageVertically( floorImages[0], imageWidth, imageHeight );
        swizzleImageForCOHColor(floorImages[0], imageWidth, imageHeight);
        num_images = 3;

        floorImages[1] = getMapImage(imageWidth, imageHeight, 0, 1);
        flipImageVertically( floorImages[1], imageWidth, imageHeight );
        clearImageByBlueness(floorImages[1], imageWidth, imageHeight );

        floorImages[2] = getMapImage(imageWidth, imageHeight, 0, 2);
        flipImageVertically( floorImages[2], imageWidth, imageHeight );
        clearImageByBlueness(floorImages[2], imageWidth, imageHeight );

    }
    *images = floorImages;
    return num_images;
}


void loadMapForImages(char * filename)
{
    
    //groupReset();
    //groupLoadTo(filename,NULL);
    char    *fname = filename;
    GroupFile    *grp_file=0;
    game_state.mission_map = 1;
    game_state.map_instance_id = 0;
    game_state.doNotAutoGroup = 1; //MW added
    game_state.iAmAnArtist = 1;

    groupReset();

    if (!group_info.file_count)
        grp_file = groupFileNew();
    else
        grp_file = group_info.files[0];
    groupFileSetName(grp_file,fname);
    clientLoadMapStart(fname);
    groupLoadTo(fname,grp_file);
    groupRecalcAllBounds();
    finishLoadMap(true, true, false);
}

int imageCapture_verifyMMdata(MMPinPData *mmdata)
{
    int ret = 1;
    if(!mmdata)
        ret = 0;
    if(ret && (!mmdata->e || !mmdata->e->seq ))
        ret = 0;
    //if(ret && (!mmdata->bind))
    //    ret = 0;
    return ret;
}

//Matt: rewriting get pixel buffer
U8 * getMMSeqImage(U8** pixbuf, SeqInst * seq, Vec3 cameraPos, Vec3 lookat, float yOffset, float rot, F32 FOV,
                   unsigned int * desiredWidth, unsigned int *desiredHeight,
                   int * px1, int * px2, int * py1, int * py2, int newImage, AtlasTex *bgimage, int square)
{
    //Vec3 bg_color = {1.0, 1.0, 1.0};
    unsigned int bufferWidth = *desiredWidth;
    unsigned int bufferHeight = *desiredHeight;
    U8 * additiveAlphaBuf = 0, * opaqueBuf = 0;    //hold the pixel buffer that will be returned
    //hold the old state
    Mat4 oldEntityPosition;
    Vec3 oldCameraPosition, oldLookatPosition;
    int oldWorldHide;
    int oldBackgroundLoaderState, oldDisable2D, oldScissor, oldFOV, oldGameMode, oldHidePlayer;
    F32 oldLegScale, oldTimeStep, oldTimeStepNoScale;//, height;
    CameraInfo oldCameraInfo;
    //screen info
    int screenWidth, screenHeight;
    Vec2 captureBottomLeft;    //the position of the bottom left corner of the capture window in screen coordinates.
    int hasFx = 0;
    //first make the image at twice the size
    unsigned int doubleWidth = bufferWidth*2; 
    unsigned int doubleHeight = bufferHeight*2;
    unsigned int totalSize;
    Vec3 bgColor = {0};
    int i;

    F32 oldTimeScale = server_visible_state.timestepscale;
    F32 oldTimeOffset = server_visible_state.time_offset;

    PERFINFO_AUTO_START("getMMSeqImage-Function", 1);
    PERFINFO_AUTO_START("getMMSeqImage-setupState", 1);

    g_doingHeadShot = true;

    server_visible_state.timestepscale = 0.0;
    server_visible_state.time_offset = 12.0 - server_visible_state.time;


    windowSize( &screenWidth, &screenHeight );
    if(screenHeight < (int)doubleHeight || screenWidth < (int)doubleWidth)
    {
        doubleHeight = (unsigned int)bufferHeight;
        doubleWidth = (unsigned int)bufferWidth;
    }
    

    //Screen Size
    if (rdr_caps.use_pbuffers) {    //taken directly from getThePixelBuffer
        initHeadShotPbuffer(doubleWidth, doubleHeight, FULL_BODY_SHOT_DESIRED_MULTI, FULL_BODY_SHOT_REQUIRED_MULTI);
        bufferWidth *= pbufHeadShot.software_multisample_level;
        bufferHeight *= pbufHeadShot.software_multisample_level; 
        // Move back camera so that the same scene is rendered twice as big
        //  we should really do this via fov or something less tweaky... as it changes the aspect of things...
        cameraPos[2] = cameraPos[2] / (pow(pbufHeadShot.software_multisample_level,0.95));
        /*if(bgimage && atlasUpdateTexturePbuffer(bgimage, &pbufHeadShot))
        {
            pbufBind(&pbufHeadShot);
        }*/
//        bgimage->data->basic_texture->id = pbufHeadShot.handle;
        doubleHeight = pbufHeadShot.height;
        doubleWidth = pbufHeadShot.width; 
    }
    totalSize = doubleHeight*doubleWidth*4;
    additiveAlphaBuf = opaqueBuf = NULL;
    //get temporary storage for the images
    estrStackCreate(&additiveAlphaBuf, totalSize);
    estrSetLengthNoMemset(&additiveAlphaBuf, totalSize);
    estrStackCreate(&opaqueBuf, totalSize);
    estrSetLengthNoMemset(&opaqueBuf, totalSize);


    captureBottomLeft[0] = ((F32)screenWidth)/2.0 - ((F32)doubleWidth)/2.0;
    captureBottomLeft[1] = ((F32)screenHeight)/2.0 - ((F32)doubleHeight)/2.0;

    //save the state before setting up the screenshot.
    copyMat4( seq->gfx_root->mat, oldEntityPosition );
    copyVec3(cameraPos, oldCameraPosition);
    copyVec3(lookat, oldLookatPosition);
    oldBackgroundLoaderState = game_state.disableGeometryBackgroundLoader;
    oldLegScale = seq->legScale;
    oldDisable2D = game_state.disable2Dgraphics;
    oldScissor = do_scissor;
    if(seq->worldGroupPtr)
    {
        oldWorldHide = seq->worldGroupPtr->hideForHeadshot;
        seq->worldGroupPtr->hideForHeadshot = 0;
    }
    oldFOV = game_state.fov_custom;
    oldGameMode = game_state.game_mode;
    oldCameraInfo = cam_info;
    oldTimeStep = TIMESTEP;
    oldTimeStepNoScale = TIMESTEP_NOSCALE;


    //set the state that will exist during the screenshot.
    copyMat4( unitmat, seq->gfx_root->mat );
    game_state.disableGeometryBackgroundLoader = 1;
    seq->legScale = 0.0;
    game_state.disable2Dgraphics = 1;
    if(oldScissor) 
        set_scissor(0);
    toggle_3d_game_modes(SHOW_GAME);    //do not draw world or baddies
    seq->gfx_root->flags &= ~GFXNODE_HIDE;
    game_state.fov_custom = FOV;
    gfx_state.renderscale = 0;
    gfx_state.renderToPBuffer = 0;
    fx_engine.no_expire_worldfx = 1;  // dont allow fx to expire, or will crash when gfxnode->models are deleted since we hold on to them for part of this update
    cameraPos[1] += yOffset;
    lookat[1] += yOffset;
    seq->gfx_root->mat[3][1] += yOffset*2;

    {
        cameraPos[0] = oldLookatPosition[0]* cos(rot) - oldLookatPosition[2]*sin(rot);
        cameraPos[2] = oldLookatPosition[2]* cos(rot) + oldLookatPosition[0]*sin(rot);
        lookat[0] = 0;//-data->headshotCameraPos[0];
        lookat[2] = 0;//-data->headshotCameraPos[2];
    }
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("getMMSeqImage-setupView", 1);



    //take image

    lightEnt( &seq->seqGfxData.light, seq->gfx_root->mat[3] , seq->type->minimumambient, 0.2 );

    //verify LOD, loaded textures and geometry (directly from getThePixelBuffer)
    seqSetLod( seq, 1.0 );  //?? nEEDED?
    seq->seqGfxData.alpha = 255;
    if(newImage)
    {
        animSetHeader( seq, 0 );
        seqProcessClientInst( seq, 30.0, 1, 1 ) ; //Ready move

        //seq->animation.frame = seq->animation.prev_frame = 30.0;

        //the following is necessary so that destructible objects like the Rikti bomb aren't in their startup position.
        seq->curr_interp_frame = 30;
        seqSetMove(seq, seq->info->moves[seq->animation.move->raw.nextMove[0]], 1);
    }
    animPlayInst( seq );
    //finally, move the particles forward about 30 seconds.

    
    //imagecapture_passSeqIntro(seq);



    
    copyMat3( unitmat, cam_info.mat ); 
    copyVec3( lookat,cam_info.mat[3] );

    copyMat3( unitmat, cam_info.cammat );
    copyVec3(lookat, cam_info.cammat[3] );
    //data->headshotCameraPos[1] *=2;
    cam_info.cammat[0][0] = cos(rot);
    cam_info.cammat[0][2] = sin(rot);
    cam_info.cammat[2][0] = -sin(rot);
    cam_info.cammat[2][2] = cos(rot);
    //data->headshotCameraPos[2] = oldLookatPosition[2]* cos(data->rotation) + oldLookatPosition[0]*sin(data->rotation);;


    addVec3( cam_info.cammat[3], cameraPos, cam_info.cammat[3] );
    gfxSetViewMat(cam_info.cammat, cam_info.viewmat, NULL);

    camLockCamera( true );



    //hide everything but the sequence we're drawing.
    {
        GfxNode * node;

        for( node = gfx_tree_root ; node ; node = node->next )
        {
            if (node->child)
            {
                if (node->child->seqHandle == seq->handle)
                    node->flags &= ~GFXNODE_HIDE;
                else
                    node->flags |= GFXNODE_HIDE;
            }
        }
    }

    //Hide the player
    {
        Entity * player = playerPtr();
        oldHidePlayer = 0;
        if( player && player->seq && player->seq->gfx_root )
        {
            oldHidePlayer = (player->seq->gfx_root->flags & GFXNODE_HIDE) ? 1 : 0;
            player->seq->gfx_root->flags |= GFXNODE_HIDE;
        }
    }

    //move the character around a little for a more dramatic pose
    yawMat3( -0.2, seq->gfx_root->mat );

    sunPush();
    sunGlareDisable();
    setLightForCharacterEditor();

    setVec3same(g_sun.bg_color, 0);

    //directly from getThePixelBuffer
    if (game_state.imageServerDebug) {
        extern int glob_force_buffer_flip;
        if (rdr_caps.use_pbuffers)
            glob_force_buffer_flip = 0;
        else
            glob_force_buffer_flip = 1;
    }



    scaleVec3(g_sun.ambient, 0.5, g_sun.ambient);
    scaleVec3(g_sun.diffuse, 1.0, g_sun.diffuse);
    mulVecMat3(g_sun.direction, unitmat, g_sun.direction_in_viewspace); 

    // Run the particle systems for a bit
    TIMESTEP_NOSCALE = TIMESTEP = FRAME_TIMESTEP;
    for (i=0; i<FRAMES_BEFORE_CAPTURE; i++) {
        //Fill the buffer
        fxRunEngine();
    }

    gfxUpdateFrame( 0, 2, 0);

    //Sleep(5);
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("getMMSeqImage-getPixels", 1);


    //retrieve the image pieces from the buffer-- first get the image on a transparent background.
    //this will break any additive alpha particle effects or (sometimes) geometry.
    //next get the additive particle effects and geometry by getting the image without alpha
    imageCaptureGetPixels(&opaqueBuf, doubleWidth, doubleHeight, captureBottomLeft, seq);
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("getMMSeqImage-getPixelInfoAndClear", 1);
    memcpy(additiveAlphaBuf, opaqueBuf, doubleWidth*doubleHeight*4);


    {
        unsigned int width, height, x1, x2, y1, y2;
        unsigned int tx1, tx2, ty1, ty2;

        if(newImage)
        {
            getMMBounds(opaqueBuf, doubleWidth, doubleHeight, &x1, &x2, &y1, &y2);
        }
        else
        {
            x1 = *px1+MM_IMAGE_BUFFER_SIZE;
            x2 = *px2-MM_IMAGE_BUFFER_SIZE;
            y1 = *py1+MM_IMAGE_BUFFER_SIZE;
            y2 = *py2-MM_IMAGE_BUFFER_SIZE;
        }
        width = x2-x1;
        height = y2-y1;

        //clearImageByLuminance(additiveAlphaBuf, doubleWidth, doubleHeight, 32, 255, 52);


            if(height<30 || width < 30)
                clearImageByLuminance(additiveAlphaBuf, doubleWidth, doubleHeight, 32, 255, 1);
            else
                clearImageByLuminance(additiveAlphaBuf, doubleWidth, doubleHeight, 32, 130, 8);

            placeImageOverImage(additiveAlphaBuf, opaqueBuf, doubleWidth, doubleHeight);

        if(newImage && height * width>=30)
        {
            int x1D, x2D, y1D, y2D;
            getMMBounds(opaqueBuf, doubleWidth, doubleHeight, &tx1, &tx2, &ty1, &ty2);
            x1D = MAX((int)(x1-tx1),0);
            x2D = MAX((int)(tx2-x2),0);
            y1D = MAX((int)(y1-ty1),0);
            y2D = MAX((int)(ty2-y2),0);
            //we want to center on the solid object(if it's there) by finding the greatest offset
            //on either side and applying it to both.
            x1 -= MAX(x1D,x2D);
            x2 += MAX(x1D,x2D);
            y1 -= y1D;//MAX(y1D,y2D);
            y2 += y2D;//MAX(y1D,y2D);
            width = x2-x1;
            height = y2-y1;
        }
        else if(newImage)
        {
            getMMBounds(opaqueBuf, doubleWidth, doubleHeight, &x1, &x2, &y1, &y2);
            width = x2-x1;
            height = y2-y1;
        }
        
        
        
        if(newImage && width * height < 30)
        {
                /*getMMBounds(opaqueBuf, doubleWidth, doubleHeight, &x1, &x2, &y1, &y2);*/

                width = *desiredWidth;
                height = *desiredHeight;
                x1 = (doubleWidth-width)/2;
                x2 = doubleWidth - x1;
                y1 = (doubleHeight-height)/2;
                y2 = doubleHeight - y1;
            
        }

        *px1 = x1 = MAX((int)(x1-MM_IMAGE_BUFFER_SIZE),0);
        *px2 = x2 = MIN((int)(x2+MM_IMAGE_BUFFER_SIZE), doubleWidth-1);
        *py1 = y1 = MAX((int)(y1-MM_IMAGE_BUFFER_SIZE), 0);
        *py2 = y2 = MIN((int)(y2+MM_IMAGE_BUFFER_SIZE), doubleHeight-1);
        width = x2-x1;
        height = y2-y1;
        if(width > height*1.5)
        {
            int moveIn = (width-(height*1.5))/2;
            x1+=moveIn;
            x2-=moveIn;
            width -= moveIn*2;
        }
        
        
        //width = min(width,doubleWidth);
        //height = min(height, doubleHeight);

        *desiredWidth = width;
        *desiredHeight = height;

        captureBottomLeft[0] = x1;//-MM_IMAGE_BUFFER_SIZE;//width/2 + x1 - bufferWidth/2;
        captureBottomLeft[1] = y1;//-MM_IMAGE_BUFFER_SIZE;//height/2 + y1 - bufferHeight/2;
        //pixbuf = malloc(width*height*4*sizeof(U8));
        if(*pixbuf)
        {
            estrDestroy(pixbuf);
        }
        estrCreateEx(pixbuf, width*height*4*sizeof(U8)); //Since pixbuf is returned, we can't alloc from the stack.
        estrSetLengthNoMemset(pixbuf, width*height*4*sizeof(U8));
        estrClear(pixbuf);

        if(newImage)
        {
            clearImageBorder(opaqueBuf, doubleWidth, doubleHeight, 
            x1, x2, 
            y1, y2);
        }
        copyMMImage(opaqueBuf, doubleWidth, doubleHeight, *pixbuf, width, height, captureBottomLeft[0], captureBottomLeft[1], 1);
    }



    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("getMMSeqImage-resetState", 1);



    //Restore the old gfx settings
    toggle_3d_game_modes(oldGameMode);
    cameraPos[1] -= yOffset;
    lookat[1] -= yOffset;
    seq->gfx_root->mat[3][1] -= yOffset*.5;
    copyMat4( oldEntityPosition, seq->gfx_root->mat );
    copyVec3(oldCameraPosition, cameraPos);
    copyVec3(oldLookatPosition,lookat);
    game_state.disableGeometryBackgroundLoader = oldBackgroundLoaderState;
    seq->legScale = oldLegScale;
    game_state.fov_custom = oldFOV;
    cam_info = oldCameraInfo;
    if(seq->worldGroupPtr)
    {
        seq->worldGroupPtr->hideForHeadshot = oldWorldHide;
    }
    TIMESTEP = oldTimeStep;
    TIMESTEP_NOSCALE = oldTimeStepNoScale;
    game_state.disable2Dgraphics = oldDisable2D;
    seq->gfx_root->flags &= GFXNODE_HIDE;
    if (oldScissor) 
        set_scissor(1);
    game_state.game_mode = oldGameMode;
    toggle_3d_game_modes(oldGameMode);
    sunPop();
    //Un-Hide the player
    if( !oldHidePlayer )
    {
        Entity * player = playerPtr();
        if( player && player->seq && player->seq->gfx_root )
            player->seq->gfx_root->flags &= ~GFXNODE_HIDE;
    }
    fx_engine.no_expire_worldfx = 0; // restore expiration of fx

    gfxWindowReshape(); //Restore

    if (rdr_caps.use_pbuffers) {
        winMakeCurrent();
    }

    rdrNeedGammaReset(); // Because people were reporting gamma going screwy after headshots taken.
    PERFINFO_AUTO_STOP();
    estrDestroy(&opaqueBuf);
    estrDestroy(&additiveAlphaBuf);
    //pbufRelease(&pbufHeadShot);
    //automap_drawMap(pixbuf, 512, 512);

    server_visible_state.timestepscale = oldTimeScale;
    server_visible_state.time_offset = oldTimeOffset;

    g_doingHeadShot = false;

    PERFINFO_AUTO_STOP();
    return *pixbuf;

}

void initMMPbuffer(PBuffer *pbuffer, int xres, int yres, int desiredMultisample, int requiredMultisample)
{
    // This pbuffer is now shared by the texWords system
    static int numCalls=0;
    static int numRecreates=0;
    numCalls++;
    if (pbuffer->software_multisample_level) {
        xres *= pbuffer->software_multisample_level;
        yres *= pbuffer->software_multisample_level;
    }
    if (xres > pbuffer->virtual_width || yres > pbuffer->virtual_height ||
        desiredMultisample != pbuffer->desired_multisample_level || requiredMultisample != pbuffer->required_multisample_level)
    {
        PBufferFlags flags = PB_COLOR_TEXTURE|PB_ALPHA|PB_RENDER_TEXTURE;
        if(rdr_caps.supports_fbos)
            flags |= PB_FBO;
        numRecreates++;

        pbufInit(pbuffer, xres, yres, flags, desiredMultisample,requiredMultisample,0,24,0,0);
        pbufMakeCurrent(pbuffer);
        rdrInit();
        rdrSetFog(2000000,2000000,0);
    } else {
        rdrQueueFlush();
        pbuffer->width = xres;
        pbuffer->height = yres;
    }
}

void updateMMSeqImage(MMPinPData * data, Vec2 screenPosition)
{
    //Vec3 bg_color = {1.0, 1.0, 1.0};
    //hold the old state
    Mat4 oldEntityPosition;
    Vec3 oldCameraPosition, oldLookatPosition;
    int oldBackgroundLoaderState, oldDisable2D, oldScissor, oldFOV, oldGameMode, oldHidePlayer;
    F32 oldLegScale, oldTimeStep, oldTimeStepNoScale;//, height;
    int oldWorldHide = 0;
    CameraInfo oldCameraInfo;
    //screen info
    int screenWidth, screenHeight;
    //Vec2 captureBottomLeft;    //the position of the bottom left corner of the capture window in screen coordinates.
    //first make the image at twice the size
    unsigned int totalSize;
    F32 oldTimeScale = server_visible_state.timestepscale;

    if (!rdr_caps.use_pbuffers)
    {
        return;    //if we're not using pbuffers, we need to use a still image.
    }
    if(!data || !data->e || !data->e->seq || !data->bind)
        return;

    data->e->noDrawOnClient = 0;


    PERFINFO_AUTO_START("updateMMSeqImage-Function", 1);

    server_visible_state.timestepscale = 0.0;
    server_visible_state.time_offset = 0;
    server_visible_state.time_offset = 12.0 - server_visible_state.time;
    //animPlayInst( data->e->seq );






    PERFINFO_AUTO_START("updateMMSeqImage-SetupPBuffer", 1);
    windowSize( &screenWidth, &screenHeight );
    totalSize = data->wdPow*data->htPow*4;

    //Screen Size
    
    if (rdr_caps.use_pbuffers) {    //taken directly from getThePixelBuffer
        initMMPbuffer(&data->pbuffer, data->wdPow, data->htPow, FULL_BODY_SHOT_DESIRED_MULTI, FULL_BODY_SHOT_REQUIRED_MULTI);
        pbufMakeCurrent(&data->pbuffer);
        // Move back camera so that the same scene is rendered twice as big
        //  we should really do this via fov or something less tweaky... as it changes the aspect of things...
        data->headshotCameraPos[2] = data->headshotCameraPos[2] / (pow(pbufHeadShot.software_multisample_level,0.95));
        //if(data->bind)
        //{
            pbufBind(&data->pbuffer);
        //}
        //        bgimage->data->basic_texture->id = pbufHeadShot.handle;

    }
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("updateMMSeqImage-SetupState", 1);


    //save the state before setting up the screenshot.
    copyMat4( data->e->seq->gfx_root->mat, oldEntityPosition );
    copyVec3(data->lookatPos, oldLookatPosition);
    copyVec3(data->headshotCameraPos, oldCameraPosition);
    oldBackgroundLoaderState = game_state.disableGeometryBackgroundLoader;
    oldLegScale = data->e->seq->legScale;
    oldDisable2D = game_state.disable2Dgraphics;
    oldScissor = do_scissor;
    oldFOV = game_state.fov_custom;
    oldGameMode = game_state.game_mode;
    oldCameraInfo = cam_info;
    oldTimeStep = TIMESTEP;
    oldTimeStepNoScale = TIMESTEP_NOSCALE;
    if(data->e->seq->worldGroupPtr)
    {
        oldWorldHide = data->e->seq->worldGroupPtr->hideForHeadshot;
        data->e->seq->worldGroupPtr->hideForHeadshot = 0;
    }


    //set the state that will exist during the screenshot.
    copyMat4( unitmat, data->e->seq->gfx_root->mat );
    game_state.disableGeometryBackgroundLoader = 1;
    data->e->seq->legScale = 0.0;
    game_state.disable2Dgraphics = 1;
    //if(oldScissor) //doesn't appear to do anything.
    //    set_scissor(0);
    toggle_3d_game_modes(SHOW_GAME);    //do not draw world or baddies
    data->e->seq->gfx_root->flags &= ~GFXNODE_HIDE;
    game_state.fov_custom = data->FOV;
    gfx_state.renderscale = 0;
    gfx_state.renderToPBuffer = 0;
    fx_engine.no_expire_worldfx = 1; // dont allow fx to expire, or will crash when gfxnode->models are deleted since we hold on to them for part of this update
    data->headshotCameraPos[1] += data->yOffset;
    data->lookatPos[1] += data->yOffset;
    data->e->seq->gfx_root->mat[3][1] += data->yOffset*2;
    {
        data->headshotCameraPos[0] = oldLookatPosition[0]* cos(data->rotation) - oldLookatPosition[2]*sin(data->rotation);
        data->headshotCameraPos[2] = oldLookatPosition[2]* cos(data->rotation) + oldLookatPosition[0]*sin(data->rotation);;
        data->lookatPos[0] = 0;//-data->headshotCameraPos[0];
        data->lookatPos[2] = 0;//-data->headshotCameraPos[2];
    }
    TIMESTEP_NOSCALE = TIMESTEP = .0001;
//    copyMat4( data->e->seq->gfx_root->mat, data->e->fork );
//    copyVec3( data->e->seq->gfx_root->mat[3], data->e->motion->last_pos );
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("updateMMSeqImage-SetupView", 1);
    sunPush();
    sunGlareDisable();



    //take image

    lightEnt( &data->e->seq->seqGfxData.light, data->e->seq->gfx_root->mat[3] , data->e->seq->type->minimumambient, 0.2 );

    //verify LOD, loaded textures and geometry (directly from getThePixelBuffer)
    //seqProcessClientInst( data->e->seq, 1.0, 0, 1 ) ; //Ready move
    seqSetLod( data->e->seq, 1.0 );  //?? nEEDED?
    data->e->seq->seqGfxData.alpha = 255;
    animSetHeader( data->e->seq, 0 );
    animPlayInst( data->e->seq );



    copyMat3( unitmat, cam_info.mat ); 
    copyVec3( data->lookatPos,cam_info.mat[3] );

    copyMat3( unitmat, cam_info.cammat );
    copyVec3(data->lookatPos, cam_info.cammat[3] );
    //data->headshotCameraPos[1] *=2;
    cam_info.cammat[0][0] = cos(data->rotation);
    cam_info.cammat[0][2] = sin(data->rotation);
    cam_info.cammat[2][0] = -sin(data->rotation);
    cam_info.cammat[2][2] = cos(data->rotation);
    //data->headshotCameraPos[2] = oldLookatPosition[2]* cos(data->rotation) + oldLookatPosition[0]*sin(data->rotation);;


    addVec3( cam_info.cammat[3], data->headshotCameraPos, cam_info.cammat[3] );
    gfxSetViewMat(cam_info.cammat, cam_info.viewmat, NULL);

    camLockCamera( true );



    //directly from getThePixelBuffer
#if RDRFIX
    //TO DO why do I need to do this when contact is created on load?  Generally Z test is a bit messy
    glEnable( GL_DEPTH_TEST ); CHECKGL;
    WCW_DepthMask(1);
    glDepthMask( 1 ); CHECKGL;
#endif



    //hide everything but the sequence we're drawing.
    {
        GfxNode * node;

        for( node = gfx_tree_root ; node ; node = node->next )
        {
            if( node->child && node->child->seqHandle != data->e->seq->handle  )
                node->flags |= GFXNODE_HIDE;
            if(!node->child && node->model)
                node->model->flags |= GFXNODE_HIDE;
        }
    }

    //Hide the player
    {
        Entity * player = playerPtr();
        oldHidePlayer = 0;
        if( player && player->seq && player->seq->gfx_root )
        {
            oldHidePlayer = (player->seq->gfx_root->flags & GFXNODE_HIDE) ? 1 : 0;
            player->seq->gfx_root->flags |= GFXNODE_HIDE;
        }
    }

    yawMat3( -0.2, data->e->seq->gfx_root->mat );

    //directly from getThePixelBuffer
    if (game_state.imageServerDebug) {
        extern int glob_force_buffer_flip;
        if (rdr_caps.use_pbuffers)
            glob_force_buffer_flip = 0;
        else
            glob_force_buffer_flip = 1;
    }







    setLightForCharacterEditor();
    scaleVec3(g_sun.ambient, 0.5, g_sun.ambient);
    scaleVec3(g_sun.diffuse, 1.0, g_sun.diffuse);
    mulVecMat3(g_sun.direction, unitmat, g_sun.direction_in_viewspace); 


    gfxUpdateFrame( 0, 2, 0);


    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("updateMMSeqImage-GetImage", 1);

    imageCaptureUpdateImage(data->e->seq);

    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("updateMMSeqImage-RestoreState", 1);
    //Restore the old gfx settings
    data->headshotCameraPos[1] -= data->yOffset;
    data->lookatPos[1] -= data->yOffset;
    data->e->seq->gfx_root->mat[3][1] -= data->yOffset*.5;
    copyVec3(oldLookatPosition,data->lookatPos);
    copyVec3(oldCameraPosition,data->headshotCameraPos);
    copyMat4( oldEntityPosition, data->e->seq->gfx_root->mat );
    game_state.disableGeometryBackgroundLoader = oldBackgroundLoaderState;
    data->e->seq->legScale = oldLegScale;
    game_state.fov_custom = oldFOV;
    cam_info = oldCameraInfo;
    if(data->e->seq->worldGroupPtr)
    {
        data->e->seq->worldGroupPtr->hideForHeadshot = oldWorldHide;
    }
    TIMESTEP = oldTimeStep;
    TIMESTEP_NOSCALE = oldTimeStepNoScale;
    game_state.disable2Dgraphics = oldDisable2D;
    data->e->seq->gfx_root->flags &= GFXNODE_HIDE;
    if (oldScissor) 
        set_scissor(1);
    game_state.game_mode = oldGameMode;
    toggle_3d_game_modes(oldGameMode);
    sunPop();
    //Un-Hide the player
    if( !oldHidePlayer )
    {
        Entity * player = playerPtr();
        if( player && player->seq && player->seq->gfx_root )
            player->seq->gfx_root->flags &= ~GFXNODE_HIDE;
    }
    fx_engine.no_expire_worldfx = 0; // restore expiration of fx

    gfxWindowReshape(); //Restore

    if (rdr_caps.use_pbuffers) {
        winMakeCurrent();
    }

    pbufRelease(&data->pbuffer);
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

AtlasTex* imageCapture_getMap(char * mapName)
{
    //return an image of the map with this name.
    //U8 pixbuf[(sizeof(U8) * 4 * 512 * 512)];
    //automap_drawMap(pixbuf, 512, 512);
    return NULL;
}

void imagecapture_passSeqIntro(SeqInst *seq)
{
    int z;
    F32 realNoScale = TIMESTEP_NOSCALE;
    F32 realTimeStep = TIMESTEP;
    TIMESTEP_NOSCALE = TIMESTEP = 60;
    partRunStartup();
    fxDoForEachFx(seq, fxAdvanceEffect, NULL);
    fxDoForEachFx(seq, fxRunParticleSystems, NULL);

    //fxRunEngine();
    //partRunEngine();
    TIMESTEP_NOSCALE = TIMESTEP = .001;
    for(z = 0; z < 6; z++)
    {
        fxDoForEachFx(seq, fxAdvanceEffect, NULL);
        fxDoForEachFx(seq, fxRunParticleSystems, NULL);
    }
    TIMESTEP_NOSCALE = realNoScale;
    TIMESTEP = realTimeStep;
    partRunShutdown();
}
