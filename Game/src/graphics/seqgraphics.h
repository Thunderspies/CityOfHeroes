#ifndef _SEQGRAPHICS_H
#define _SEQGRAPHICS_H

#include <utilitieslib/stdtypes.h>
#include "gameComm/Npc.h"
#include "seq/seqskeleton.h"


typedef struct SeqInst SeqInst;
typedef struct AtlasTex AtlasTex;
typedef struct GfxNode GfxNode;

typedef enum seqgraphics_pictureType
{
    PICTURETYPE_BODYSHOT = 0,
    PICTURETYPE_HEADSHOT,
} Seqgraphics_PictureType;

void seqRequestHeadshot( const cCostume * costume, int uniqueId ); // defer creating the headshot until seqProcessRequestedHeadshots is called
void seqProcessRequestedHeadshots(void);
AtlasTex* seqGetHeadshotCached( const cCostume * costume, int uniqueId ); // only return a headshot if it is already cached
AtlasTex* seqGetHeadshot( const cCostume * costume, int uniqueId );
AtlasTex* seqGetHeadshotForce( const cCostume * costume, int uniqueId );
AtlasTex* seqGetMMShot( const cCostume * costume, int stillImage, Seqgraphics_PictureType  type );
AtlasTex* seqGetBodyshot( const cCostume * costume, int uniqueId );
void updateSeqHeadshotUniqueID();
int getSeqHeadshotUniqueID();
bool doingHeadShot(void);
void initHeadShotPbuffer(int xres, int yres, int desiredMultisample, int requiredMultisample);
void animCalculateSortFlags( GfxNode * gfxnode );
U8 * getThePixelBuffer( SeqInst * seq, Vec3 headshotCameraPos, F32 headShotFovMagic, int sizeOfPictureX,
                       int sizeOfPictureY, BoneId centerBone, int shouldIFlip, int bodyshot,
                       AtlasTex *bgimage, int bgcolor, int isImageServer, char *caption);
void initHeadShotPbuffer(int xres, int yres, int desiredMultisample, int requiredMultisample);
int seqRemoveHeadshotCached( const cCostume * costume, int uniqueId );

void seqClearMMCache();
void seqClearHeadshotCache(void);
#endif
