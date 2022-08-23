#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H
/* imageCapture.h
 * Contains functions used in acquiring and saving 2D images
 * of objects in the CoH world.
 */
#include <utilitieslib/stdtypes.h>
#include "seq/seq.h"
#include "gameComm/Npc.h"
#include "seq/seqskeleton.h"
#include "graphics/textureatlas.h"
#include "render/pbuffer.h"

typedef struct MMPinPData
{
    Entity * e;
    Costume *c;
    AtlasTex * bind;
    //set once for every entity.  We must save if this is the same entity
    int x1;    //left hand position of image in the screen's buffer
    int x2;    //right "
    int y1;
    int y2;
    int motionAvailable;
    Vec3 headshotCameraPos; 
    Vec3 lookatPos;
    unsigned int wdPow, htPow;
    float yOffset;
    float rotation;
    float FOV;
    Vec3 bgColor;
    PBuffer pbuffer;
}MMPinPData;


/* Returns a cropped image of the seq entity at approximately desired
 * width by desired height.  These dimensions will only even be close
 * if the capsule and/or bones accurately reflect the entity's height
 * and width.
 */
U8 * getMMSeqImage(U8 ** buf, SeqInst * seq, Vec3 cameraPos, Vec3 lookat, float yOffset, float rot, F32 FOV,
              unsigned int * desiredWidth, unsigned int *desiredHeight,
              int *x1, int *x2, int *y1, int *y2, int newImage, AtlasTex *bgimage, int square);
void updateMMSeqImage(MMPinPData * data, Vec2 screenPosition);

AtlasTex* imageCapture_getMap(char * mapName);

void imageCapture_copyOver(U8 *src, int srcW, int srcH, U8 *dst, int dstW, int dstH, unsigned int left, unsigned int top);
void imageCapture_apply2DMatrix(U8 *src, Vec2 srcDim, U8 *dst, Vec2 dstDim, Mat3 transform);
void imageCapture_SimpleRotate(U8 *src, Vec2 *dimensions, F32 rotateAmount);
void imageCapture_flip(U8 *src, Vec2 *dimensions);
void ImageCapture_WriteMapImages(char *filename, char *directory, int checkin);
int imageCapture_verifyMMdata(MMPinPData *mmdata);
void imagecapture_passSeqIntro(SeqInst *seq);
#endif
