
#include "beaconPrivate.h"
#include "beaconGenerate.h"
#include "beaconConnection.h"
#include "beaconClientServerPrivate.h"
#include "comm_game.h"
#include "cmdparse/cmdserver.h"
#include "svr/svr_base.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/crypt.h>
#include "seq/anim.h"
#include <utilitieslib/utils/timing.h>

#define SEND_LINE(x1,y1,z1,x2,y2,z2,color){        \
    if(pak){                                    \
        pktSendBitsPack(pak, 2, 1);                \
        pktSendF32(pak,x1);                        \
        pktSendF32(pak,y1);                        \
        pktSendF32(pak,z1);                        \
        pktSendF32(pak,x2);                        \
        pktSendF32(pak,y2);                        \
        pktSendF32(pak,z2);                        \
        pktSendBits(pak,32,color);                \
    }                                            \
}

#define SEND_BEACON(x,y,z,color){        \
    if(pak){                            \
        pktSendBitsPack(pak, 2, 2);        \
        pktSendF32(pak,x);                \
        pktSendF32(pak,y);                \
        pktSendF32(pak,z);                \
        pktSendIfSetBits(pak,32,color);    \
    }                                    \
}

static Packet* pak;

static S32 min_tri_x;
static S32 max_tri_x;
static S32 min_tri_z;
static S32 max_tri_z;

#if BEACONGEN_CHECK_VERTS
    static BeaconGenerateColumnAreaTriVerts currentTriVerts;
#endif

#define MP_BASIC(type, size)        \
    MP_DEFINE(type);                \
    type* create##type(){            \
        MP_CREATE(type, size);        \
                                    \
        return MP_ALLOC(type);        \
    }                                \
    void destroy##type(type* chunk){\
        MP_FREE(type, chunk);        \
    }

//----- BeaconGenerateChunk ---------------------------------------------------------------------

MP_BASIC(BeaconGenerateChunk, 9);

//----- BeaconGenerateColumnArea ----------------------------------------------------------------

MP_BASIC(BeaconGenerateColumnArea, 10000);

//----- BeaconGenerateFlatArea ------------------------------------------------------------------

MP_DEFINE(BeaconGenerateFlatArea);

BeaconGenerateFlatArea* createBeaconGenerateFlatArea(){
    BeaconGenerateFlatArea* flatArea;
    
    MP_CREATE(BeaconGenerateFlatArea, 1000);
    
    flatArea = MP_ALLOC(BeaconGenerateFlatArea);
    
    flatArea->y_min = FLT_MAX;
    flatArea->y_max = -FLT_MAX;
    
    return flatArea;
}

void destroyBeaconGenerateFlatArea(BeaconGenerateFlatArea* area){
    MP_FREE(BeaconGenerateFlatArea, area);
}

//----- BeaconGenerateCheckList -----------------------------------------------------------------

MP_BASIC(BeaconGenerateCheckList, 1024);

//----- BeaconGenerateBeaconingInfo -------------------------------------------------------------

MP_BASIC(BeaconGenerateBeaconingInfo, 512);

//----- BeaconGenerateEdgeNode -------------------------------------------------------------

MP_BASIC(BeaconGenerateEdgeNode, 512);

//-----------------------------------------------------------------------------------------------

static BeaconGenerateColumn* getColumn(S32 x, S32 z, S32 create){
    BeaconDiskSwapBlock* swapBlock = beaconGetDiskSwapBlock(x, z, create);
    BeaconGenerateColumn* column;
    
    if(!swapBlock){
        return NULL;
    }
    
    assert(!create || x >= min_tri_x && x < max_tri_x && z >= min_tri_z && z < max_tri_z);
    
    if(!swapBlock->chunk){
        S32 i;
        
        swapBlock->chunk = createBeaconGenerateChunk();
        swapBlock->chunk->swapBlock = swapBlock;
        
        for(i = 0; i < ARRAY_SIZE(swapBlock->chunk->columns); i++){
            swapBlock->chunk->columns[i].chunk = swapBlock->chunk;
        }
    }
    
    x -= swapBlock->x;
    z -= swapBlock->z;
    
    column = swapBlock->chunk->columns + BEACON_GEN_COLUMN_INDEX(x, z);
    
    return column;
}

void beaconClearSwapBlockChunk(BeaconDiskSwapBlock* block){
    BeaconGenerateChunk* chunk = block->chunk;
    BeaconGenerateFlatArea* flatArea;
    S32 i;
    
    if(!chunk)
        return;
        
    block->inverted = 0;
        
    for(i = 0; i < ARRAY_SIZE(chunk->columns); i++){
        BeaconGenerateColumn* column = chunk->columns + i;
        BeaconGenerateColumnArea* area;

        for(area = column->areas; area;){
            BeaconGenerateColumnArea* nextArea = area->nextColumnArea;
            
            destroyBeaconGenerateColumnArea(area);
            area = nextArea;
        }
        
        #if BEACONGEN_CHECK_VERTS
            SAFE_FREE(column->tris.tris);
            ZeroStruct(&column->tris);
        #endif
    }
    
    for(flatArea = chunk->flatAreas; flatArea;){
        BeaconGenerateFlatArea* nextFlatArea = flatArea->nextChunkArea;
        
        destroyBeaconGenerateFlatArea(flatArea);
        flatArea = nextFlatArea;
    }

    destroyBeaconGenerateChunk(block->chunk);
    block->chunk = NULL;
}

typedef struct LegalConn {
    struct {
        BeaconDiskSwapBlock* block;
        S32 columnIndex;
        S32 areaIndex;
    } src, dst;
} LegalConn;

static struct {
    LegalConn*    conns;
    S32            count;
    S32            maxCount;
} legals;

static void addLegalConn(    BeaconGenerateColumn* srcColumn, BeaconGenerateColumnArea* srcArea,
                            BeaconGenerateColumn* dstColumn, BeaconGenerateColumnArea* dstArea)
{
    LegalConn* l = dynArrayAddStruct(&legals.conns, &legals.count, &legals.maxCount);
    
    l->src.block = srcColumn->chunk->swapBlock;
    l->src.columnIndex = srcColumn - srcColumn->chunk->columns;
    l->src.areaIndex = srcArea->columnAreaIndex;

    l->dst.block = dstColumn->chunk->swapBlock;
    l->dst.columnIndex = dstColumn - dstColumn->chunk->columns;
    l->dst.areaIndex = dstArea->columnAreaIndex;
}                

S32 beaconMakeLegalColumnArea(BeaconGenerateColumn* column, BeaconGenerateColumnArea* area, S32 doNotMakeBeaconsInVolume){
    BeaconGenerateChunk*    chunk = column->chunk;
    BeaconDiskSwapBlock*    swapBlock = chunk->swapBlock;
    BeaconGenerateSlope        slope = area->slope;
    
    if(doNotMakeBeaconsInVolume){
        area->doNotMakeBeaconsInVolume = 1;
    }

    if(area->isLegal){
        return 0;
    }
    
    if(!area->inCompressedLegalList){
        S32 columnIndex = column - column->chunk->columns;
        S32 x = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
        S32 z = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
        
        if(    x == 0 ||
            x == BEACON_GENERATE_CHUNK_SIZE - 1 ||
            z == 0 ||
            z == BEACON_GENERATE_CHUNK_SIZE - 1)
        {
            void beaconCheckDuplicates(BeaconDiskSwapBlock* block);

            BeaconDiskSwapBlock* block = column->chunk->swapBlock;
            BeaconLegalAreaCompressed* legalArea;
            
            area->inCompressedLegalList = 1;
        
            block->addedLegal = 1;
            
            legalArea = beaconAddLegalAreaCompressed(block);
            
            if(legalArea){
                //legalArea = dynArrayAddStruct(    &block->legalCompressed.areas,
                //                                &block->legalCompressed.count,
                //                                &block->legalCompressed.maxCount);
                                    
                legalArea->isIndex = 1;
                legalArea->x = x;
                legalArea->z = z;
                legalArea->y_index = area->columnAreaIndex;
                
                beaconCheckDuplicates(block);
            }

            assert(    block->chunk->columns[BEACON_GEN_COLUMN_INDEX(legalArea->x, legalArea->z)].isIndexed &&
                    legalArea->y_index >= 0 &&
                    (S32)legalArea->y_index < block->chunk->columns[BEACON_GEN_COLUMN_INDEX(legalArea->x, legalArea->z)].areaCount);
        }
    }
    
    //if(    (area->columnIndex % 256) + chunk->swapBlock->x == 27 &&
    //    (area->columnIndex / 256) + chunk->swapBlock->z == 99)
    //{
    //    printf("");
    //}

    area->isLegal = 1;
    area->inChunkLegalList = 1;

    area->nextChunkLegalArea = chunk->legalAreas[slope].areas;
    chunk->legalAreas[slope].areas = area;
    chunk->legalAreas[slope].count++;
    
    if(!swapBlock->isLegal){
        swapBlock->isLegal = 1;
        swapBlock->legalList.next = bp_blocks.legal.head;
        if(bp_blocks.legal.head){
            bp_blocks.legal.head->legalList.prev = swapBlock;
        }
        assert(!swapBlock->legalList.prev);
        bp_blocks.legal.head = swapBlock;
        
        bp_blocks.legal.count++;
    }
        
    return 1;
}

static void invertColumnAreas(BeaconDiskSwapBlock* block){
    BeaconGenerateChunk* chunk = block->chunk;
    U32 curCRC = 0;
    S32 i;
    
    if(!chunk || chunk->swapBlock->inverted){
        return;
    }

    block->inverted = 1;
    
    cryptAdler32Init();
    
    //if(block->x / 256 == -18 && block->z / 256 == -4){
    //    BeaconGenerateColumnArea* area;
    //    
    //    printf("\n\n(-18,-4) (0,59) = %d\n", block->chunk->columns[59 * 256].areaCount);
    //    
    //    area = block->chunk->columns[59 * 256].areas;
    //    
    //    while(area){
    //        printf("  area: %1.1f - %1.1f\n", area->y_min, area->y_max);
    //        area = area->nextColumnArea;
    //    }
    //    
    //    printf("\n\n");
    //}

    for(i = 0; i < ARRAY_SIZE(chunk->columns); i++){
        BeaconGenerateColumn* column = chunk->columns + i;
        BeaconGenerateColumnArea* prevArea = NULL;
        BeaconGenerateColumnArea* area;
        S32 columnAreaIndex = 0;
        
        for(area = column->areas; area; area = area ? area->nextColumnArea : column->areas){
            area->y_min = area->y_max;
            
            if(area->nextColumnArea){
                area->y_max = area->nextColumnArea->y_min;
            }else{
                //if(    (area->columnIndex % 256) + chunk->swapBlock->x == -1058 &&
                //    (area->columnIndex / 256) + chunk->swapBlock->z == 256)
                //{
                //    printf("");
                //}
                area->y_max = scene_info.maxHeight;
            }

            area->y_min_S16 = floor(area->y_min);
            area->y_max_S16 = ceil(area->y_max);

            if(    area->y_min < min(-2000, scene_info.minHeight) ||
                area->y_min > scene_info.maxHeight - 5)
            {
                // This area is too low or too high.
                
                if(prevArea){
                    prevArea->nextColumnArea = area->nextColumnArea;
                }else{
                    column->areas = area->nextColumnArea;
                }
                
                destroyBeaconGenerateColumnArea(area);
                
                area = prevArea;
                
                column->areaCount--;
                
                assert(!column->areaCount == !column->areas);
            }else{
                area->columnAreaIndex = columnAreaIndex++;
            }
            
            prevArea = area;
        }
        
        assert(columnAreaIndex == column->areaCount);
        
        column->isIndexed = 1;
        
        curCRC = cryptAdler32((U8*)&column->areaCount, sizeof(column->areaCount));
    }
    
    if(    block->x >= min_tri_x && block->x + BEACON_GENERATE_CHUNK_SIZE <= max_tri_x &&
        block->z >= min_tri_z && block->z + BEACON_GENERATE_CHUNK_SIZE <= max_tri_z)
    {
        if(!block->foundCRC){
            block->foundCRC = 1;
            block->surfaceCRC = curCRC;
        }else{
            assert(block->surfaceCRC == curCRC);
        }
    }
    
    //{
    //    S32 findx = -1218;
    //    S32 findz = 3584;
    //    
    //    if(    block->x / 256 == (findx + 256 * 1000) / 256 - 1000 &&
    //        block->z / 256 == (findz + 256 * 1000) / 256 - 1000)
    //    {
    //        BeaconGenerateColumnArea* area;
    //        S32 x = (findx + 256 * 1000) % 256;
    //        S32 z = (findz + 256 * 1000) % 256;
    //        
    //        printf(    "\n\n(%d,%d) (crc=0x%8.8x) @ (%d,%d) = %d\n",
    //                block->x / 256,
    //                block->z / 256,
    //                x,
    //                z,
    //                block->surfaceCRC,
    //                block->chunk->columns[0].areaCount);
    //        
    //        area = block->chunk->columns[x + z * 256].areas;
    //        
    //        while(area){
    //            printf("  area: %1.1f - %1.1f\n", area->y_min, area->y_max);
    //            area = area->nextColumnArea;
    //        }
    //        
    //        printf("\n\n");
    //    }
    //}
}

static void beaconInvertAllChunks(){
    BeaconDiskSwapBlock* cur;
    
    for(cur = bp_blocks.list; cur; cur = cur->nextSwapBlock){
        invertColumnAreas(cur);
    }
}

BeaconGenerateColumnArea* beaconGetColumnAreaFromYPos(BeaconGenerateColumn* column, F32 y){
    BeaconGenerateColumnArea* area;
    
    for(area = column->areas; area; area = area->nextColumnArea){
        if(y >= area->y_min - 2 && y <= area->y_max){
            return area;
        }
    }
    
    return NULL;
}

static void addGeneratePosition(Vec3 pos, S32 doMakeBeacon){
    BeaconGenerateColumn* column = getColumn(pos[0], pos[2], 0);
    BeaconGenerateColumnArea* area;
    
    if(!column){
        return;
    }
    
    area = beaconGetColumnAreaFromYPos(column, pos[1]);
    
    if(area && area->slope != BGSLOPE_VERTICAL){
        beaconMakeLegalColumnArea(column, area, 0);
    
        if(doMakeBeacon){
            area->makeBeacon = 1;
        }
    }
}

static S32 dir_offsets[8][2] = {
    { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 },
    { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }
};

static S32 dir_check_order[8] = { 0, 2, 4, 6, 1, 3, 5, 7 };

#define MAX_Y_DIFF_S16 (2)
#define MAX_Y_DIFF (1.8)
#define MAX_FLAT_Y_DIFF (100000.0)
#define BETTER_SLOPE_Y_DIFF (0.2)

static S32 checkSide(    BeaconGenerateColumn* column,
                        BeaconGenerateColumnArea* area,
                        BeaconGenerateFlatArea* flatArea,
                        S32 x,
                        S32 z,
                        S32 requireConn,
                        F32 scale)
{
    if(    x >= 0 && x < BEACON_GENERATE_CHUNK_SIZE &&
        z >= 0 && z < BEACON_GENERATE_CHUNK_SIZE)
    {
        BeaconGenerateColumn* sideColumn = column->chunk->columns + x + z * BEACON_GENERATE_CHUNK_SIZE;
    
        if(sideColumn->propagate.flatArea == flatArea){
            BeaconGenerateColumnArea* sideArea = sideColumn->propagate.curArea;
            
            if(sideArea){
                S16 diff_S16;

                if(sideArea->slope != area->slope){
                    return 0;
                }
                
                diff_S16 = sideArea->y_min_S16 - area->y_min_S16;
                
                switch(area->slope){
                    case BGSLOPE_FLAT:{
                        if( diff_S16 > MAX_Y_DIFF_S16 ||
                            diff_S16 < -MAX_Y_DIFF_S16 ||
                            fabs(sideArea->y_min - area->y_min) > MAX_Y_DIFF * scale ||
                            sideArea->y_min - flatArea->y_min > MAX_FLAT_Y_DIFF ||
                            flatArea->y_max - sideArea->y_min > MAX_FLAT_Y_DIFF)
                        {
                            return 0;
                        }
                        break;
                    }
                    
                    case BGSLOPE_SLIPPERY:{
                        if( diff_S16 > 4 ||
                            diff_S16 < -4 ||
                            fabs(sideArea->y_min - area->y_min) > 3.3 &&
                            sideArea->y_max - area->y_min > 5 &&
                            sideArea->y_min - area->y_max < -5)
                        {
                            return 0;
                        }
                        break;
                    }
                    
                    case BGSLOPE_STEEP:{
                        if( diff_S16 > 10 ||
                            diff_S16 < -10 ||
                            fabs(sideArea->y_min - area->y_min) > 9.2 &&
                            sideArea->y_max - area->y_min > 5 &&
                            sideArea->y_min - area->y_max < -5)
                        {
                            return 0;
                        }
                        break;
                    }
                    
                    default:{
                        assert(0);
                    }
                }
            }else{
                if(requireConn){
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

static S32 checkSides(    BeaconGenerateColumn* column,
                        BeaconGenerateColumnArea* area,
                        BeaconGenerateFlatArea* flatArea)
{
    S32 columnIndex = area->columnIndex;
    S32 x = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
    S32 z = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
    S32 i;
    
    //if(    0 &&
    //    x + area->column->chunk->swapBlock->x == 268 && 
    //    z + area->column->chunk->swapBlock->z == -380)
    //{
    //    printf("");
    //}

    //if(    0 &&
    //    x + area->column->chunk->swapBlock->x == 267 && 
    //    z + area->column->chunk->swapBlock->z == -379)
    //{
    //    printf("");
    //}

    for(i = 0; i < 8; i++){
        S32 dx = dir_offsets[i][0];
        S32 dz = dir_offsets[i][1];
        
        if(!checkSide(column, area, flatArea, x + dx, z + dz, 0, (i & 1) ? 1.414 : 1.0)){
            return 0;
        }
        
        if(i & 1 && area->slope == BGSLOPE_FLAT){
            // If diagonal, check for a perpendicular connection.
            
            if(    !checkSide(column, area, flatArea, x, z + dz, 1, 1.0) &&
                !checkSide(column, area, flatArea, x + dx, z, 1, 1.0))
            {
                return 0;
            }
        }
    }
    
    return 1;
}

void beaconAddToCheckList(BeaconGenerateFlatArea* flatArea, BeaconGenerateColumnArea* columnArea){
    BeaconGenerateCheckList* prev = NULL;
    BeaconGenerateCheckList* cur;
    U32 distance;
    
    columnArea->inCheckList = 1;

    if(flatArea->y_min > flatArea->y_max){
        flatArea->y_min = flatArea->y_max = columnArea->y_min;
    }
    
    distance = fabs((columnArea->y_max + columnArea->y_min) * 0.5 - flatArea->y_min);
    
    for(cur = flatArea->checkList; cur; prev = cur, cur = cur->next){
        if(distance <= cur->distance){
            if(distance < cur->distance){
                BeaconGenerateCheckList* newList = createBeaconGenerateCheckList();
                
                if(prev){
                    prev->next = newList;
                }else{
                    flatArea->checkList = newList;
                }

                newList->next = cur;
                cur = newList;
                
                cur->distance = distance;
            }
            
            columnArea->nextInFlatAreaSubList = cur->areas;
            cur->areas = columnArea;

            break;
        }
    }
    
    if(!cur){
        BeaconGenerateCheckList* newList = createBeaconGenerateCheckList();
        
        if(prev){
            prev->next = newList;
        }else{
            flatArea->checkList = newList;
        }

        newList->next = NULL;
        newList->distance = distance;
        
        columnArea->nextInFlatAreaSubList = newList->areas;
        newList->areas = columnArea;
    }
}

#define MAX_LEGAL_PROP_Y_DIFF (5)

static S32 compareAndMakeLegalColumnArea(    BeaconGenerateColumnArea* center,
                                            BeaconGenerateColumn* sideColumn,
                                            BeaconGenerateColumnArea* sideArea,
                                            S32 dir_index)
{
    S32 count = 0;
    
    if(    ~dir_index & 1 &&
        !sideArea->isLegal &&
        sideArea->y_min_S16 <= center->y_max_S16 - MAX_LEGAL_PROP_Y_DIFF &&
        sideArea->y_max_S16 >= center->y_min_S16 + MAX_LEGAL_PROP_Y_DIFF &&
        sideArea->y_max - sideArea->y_min >= 5 &&
        sideArea->y_min <= center->y_max - MAX_LEGAL_PROP_Y_DIFF &&
        sideArea->y_max >= center->y_min + MAX_LEGAL_PROP_Y_DIFF)
    {
        count += beaconMakeLegalColumnArea(sideColumn, sideArea, center->doNotMakeBeaconsInVolume);
        //addLegalConn(chunk->columns + columnIndex, curArea, sideColumn, sideArea);
    }
    
    return count;
}

static void propagateFlatArea(    BeaconGenerateChunk* chunk,
                                BeaconGenerateColumnArea* startArea)
{
    BeaconGenerateFlatArea* flatArea;
    S32 internalLegalCount = 0;
    
    if(startArea->flatArea){
        return;
    }
    
    if(startArea->slope != BGSLOPE_VERTICAL){
        BeaconGenerateColumn* column = chunk->columns + startArea->columnIndex;
        
        flatArea = startArea->flatArea = createBeaconGenerateFlatArea();
        
        // Add the flatArea to the chunk.

        flatArea->chunk = chunk;
        flatArea->nextChunkArea = chunk->flatAreas;
        chunk->flatAreas = flatArea;
        chunk->flatAreaCount++;
        
        // Set the propagation areas in the column.

        column->propagate.curArea = startArea;
        column->propagate.flatArea = startArea->flatArea;
        
        // Add the start area to the checkList.
        
        assert(!startArea->inCheckList);
        beaconAddToCheckList(flatArea, startArea);
        flatArea->allAreasCount++;
        
        // Make a random color.
        
        flatArea->color =    0xff000000 |
                            //((0x30 + (rand() & (0xff - 0x30))) << 16) |
                            ((0x30 + (rand() & (0xff - 0x30))) << 8) |
                            ((0x30 + (rand() & (0xff - 0x30))) << 0);
                            
        startArea = NULL;
    }else{
        flatArea = NULL;
    }
        
    while(startArea || flatArea->checkList){
        BeaconGenerateColumnArea* curArea = startArea ? startArea : flatArea->checkList->areas;
        S32 columnIndex = curArea->columnIndex;
        S32 x = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
        S32 z = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
        S32 i;
        S32 sideConnCount = 0;
        
        startArea = NULL;
        
        //if(    x + chunk->swapBlock->x == -863 && 
        //    z + chunk->swapBlock->z == -483)
        //{
        //    printf("");
        //}
        
        //if(    x + chunk->swapBlock->x == 267 && 
        //    z + chunk->swapBlock->z == -379)
        //{
        //    printf("");
        //}

        if(flatArea){
            assert(curArea->inCheckList);

            flatArea->checkList->areas = curArea->nextInFlatAreaSubList;
            curArea->inCheckList = 0;
            
            if(!curArea->nextInFlatAreaSubList){
                BeaconGenerateCheckList* oldCheckList = flatArea->checkList;
                
                flatArea->checkList = oldCheckList->next;
                destroyBeaconGenerateCheckList(oldCheckList);
            }
            
            curArea->nextInFlatAreaSubList = NULL;
        }

        for(i = 0; i < 8; i++){
            S32 dir_index = dir_check_order[i];
            BeaconGenerateColumnAreaSide* side = curArea->sides + dir_index;
            
            if(!(curArea->foundSides & (1 << dir_index))){
                S32 side_x = x + dir_offsets[dir_index][0];
                S32 side_z = z + dir_offsets[dir_index][1];
                
                curArea->foundSides |= 1 << dir_index;
                
                if(    side_x >= 0 && side_x < BEACON_GENERATE_CHUNK_SIZE &&
                    side_z >= 0 && side_z < BEACON_GENERATE_CHUNK_SIZE)
                {
                    BeaconGenerateColumn* sideColumn = chunk->columns + side_x + side_z * BEACON_GENERATE_CHUNK_SIZE;
                    
                    if(!flatArea || sideColumn->propagate.flatArea == flatArea){
                        BeaconGenerateColumnAreaSide* sideBackSide;
                        BeaconGenerateColumnArea* sideArea;
                        S32 side_dir_index;

                        for(sideArea = sideColumn->areas; sideArea; sideArea = sideArea->nextColumnArea){
                            internalLegalCount += compareAndMakeLegalColumnArea(curArea, sideColumn, sideArea, dir_index);
                        }

                        if(!flatArea || !sideColumn->propagate.curArea){
                            continue;
                        }
                        
                        //assert(fabs(sideColumn->propagate.curArea->y_min - curArea->y_min) <= MAX_Y_DIFF);
                        
                        side->area = sideColumn->propagate.curArea;
                        
                        // Connect the side back to myself.
                        
                        side_dir_index = (dir_index + 4) % 8;
                        
                        sideBackSide = side->area->sides + side_dir_index;
                        
                        if(sideBackSide->area){
                            assert(side->area->foundSides & (1 << side_dir_index) && sideBackSide->area == curArea);
                        }else{
                            sideBackSide->area = curArea;
                            side->area->foundSides |= 1 << side_dir_index;
                        }

                        sideConnCount++;
                    }else{
                        BeaconGenerateColumnArea* sideArea;
                        BeaconGenerateColumnArea* sideAreaConnect = NULL;
                        F32 scale = (dir_index & 1) ? 1.414 : 1.0;
                        
                        //if(!sideColumn->gotAreas){
                        //    createColumnAreas(    sideColumn,
                        //                        chunk->swapBlock->x + side_x,
                        //                        chunk->swapBlock->z + side_z);
                        //}
                        
                        // Find the column-area that connects to me.
                        
                        for(sideArea = sideColumn->areas; sideArea; sideArea = sideArea->nextColumnArea){
                            internalLegalCount += compareAndMakeLegalColumnArea(curArea, sideColumn, sideArea, dir_index);

                            if(!sideAreaConnect){
                                S16 diff_S16 = sideArea->y_min_S16 - curArea->y_min_S16;
                                
                                if(sideArea->slope == curArea->slope){
                                    switch(curArea->slope){
                                        case BGSLOPE_FLAT:{
                                            if(    diff_S16 <= MAX_Y_DIFF_S16 &&
                                                diff_S16 >= -MAX_Y_DIFF_S16 &&
                                                fabs(sideArea->y_min - curArea->y_min) <= MAX_Y_DIFF * scale &&
                                                sideArea->y_min - flatArea->y_min <= MAX_FLAT_Y_DIFF &&
                                                flatArea->y_max - sideArea->y_min <= MAX_FLAT_Y_DIFF)
                                            {
                                                sideAreaConnect = sideArea;
                                            }
                                            break;
                                        }

                                        case BGSLOPE_SLIPPERY:{
                                            if(    diff_S16 <= 4 &&
                                                diff_S16 >= -4 &&
                                                fabs(sideArea->y_min - curArea->y_min) <= 3.3 &&
                                                sideArea->y_max - curArea->y_min > 5 &&
                                                sideArea->y_min - curArea->y_max < -5)
                                            {
                                                sideAreaConnect = sideArea;
                                            }
                                            break;
                                        }

                                        case BGSLOPE_STEEP:{
                                            if(    diff_S16 <= 10 &&
                                                diff_S16 >= -10 &&
                                                fabs(sideArea->y_min - curArea->y_min) <= 9.2 &&
                                                sideArea->y_max - curArea->y_min > 5 &&
                                                sideArea->y_min - curArea->y_max < -5)
                                            {
                                                sideAreaConnect = sideArea;
                                            }
                                            break;
                                        }
                                        
                                        case BGSLOPE_VERTICAL:{
                                            if(    sideArea->y_max - curArea->y_min > 5 &&
                                                sideArea->y_min - curArea->y_max < -5)
                                            {
                                                sideAreaConnect = sideArea;
                                            }
                                            break;
                                        }

                                        default:{
                                            assert(0);
                                        }
                                    }
                                }
                            }
                        }
                        
                        sideColumn->propagate.flatArea = flatArea;
                        sideColumn->propagate.curArea = NULL;
                        
                        if(sideAreaConnect){
                            if(    (!sideAreaConnect->flatArea || sideAreaConnect->flatArea == flatArea) &&
                                checkSides(sideColumn, sideAreaConnect, flatArea))
                            {
                                BeaconGenerateColumnAreaSide* sideBackSide;
                                S32 side_dir_index;
                                
                                internalLegalCount += beaconMakeLegalColumnArea(sideColumn, sideAreaConnect, curArea->doNotMakeBeaconsInVolume);
                                
                                sideColumn->propagate.curArea = sideAreaConnect;
                                
                                side->area = sideAreaConnect;
                                
                                // Connect the side back to myself.
                                
                                side_dir_index = (dir_index + 4) % 8;
                                
                                sideBackSide = sideAreaConnect->sides + side_dir_index;
                                
                                assert(!sideBackSide->area);
                                sideBackSide->area = curArea;
                                sideAreaConnect->foundSides |= 1 << side_dir_index;
                                
                                sideAreaConnect->flatArea = flatArea;
                                
                                if(sideAreaConnect->y_min < flatArea->y_min)
                                    flatArea->y_min = sideAreaConnect->y_min;
                                if(sideAreaConnect->y_min > flatArea->y_max)
                                    flatArea->y_max = sideAreaConnect->y_min;
                                
                                assert(!sideAreaConnect->inCheckList);
                                beaconAddToCheckList(flatArea, sideAreaConnect);
                                flatArea->allAreasCount++;
                                
                                sideConnCount++;
                            }
                        }
                    }
                }
                else if(!(dir_index & 1)){
                    // Side is in another chunk, so load that up.
                    
                    BeaconGenerateColumnArea* sideArea;
                    BeaconGenerateColumn* sideColumn;
                    S32 column_x = chunk->swapBlock->x + side_x;
                    S32 column_z = chunk->swapBlock->z + side_z;
                    
                    sideColumn = getColumn(column_x, column_z, 0);
                    
                    if(sideColumn){
                        for(sideArea = sideColumn->areas; sideArea; sideArea = sideArea->nextColumnArea){
                            //if(column_x == -1058 && column_z == 255){
                            //    printf("");
                            //}
                            compareAndMakeLegalColumnArea(curArea, sideColumn, sideArea, dir_index);
                        }
                    }
                }
            }else{
                S32 side_x = x + dir_offsets[dir_index][0];
                S32 side_z = z + dir_offsets[dir_index][1];
                
                if(    side_x >= 0 && side_x < BEACON_GENERATE_CHUNK_SIZE &&
                    side_z >= 0 && side_z < BEACON_GENERATE_CHUNK_SIZE)
                {
                    BeaconGenerateColumn* sideColumn = chunk->columns + side_x + side_z * BEACON_GENERATE_CHUNK_SIZE;
                    BeaconGenerateColumnArea* sideArea;

                    for(sideArea = sideColumn->areas; sideArea; sideArea = sideArea->nextColumnArea){
                        internalLegalCount += compareAndMakeLegalColumnArea(curArea, sideColumn, sideArea, dir_index);
                    }
                }

                if(side->area){
                    sideConnCount++;
                }
            }
        }
        
        if(flatArea){
            if(sideConnCount != 8){
                curArea->inEdgeList = 1;
                curArea->nextInFlatAreaSubList = flatArea->edgeList;
                flatArea->edgeList = curArea;
            }else{
                curArea->inNonEdgeList = 1;
                curArea->nextInFlatAreaSubList = flatArea->nonEdgeList;
                flatArea->nonEdgeList = curArea;
            }
        }else{
            break;
        }
    }
    
    //printf("flatArea(%d areas, %d legal)\n", flatArea->allAreasCount, internalLegalCount);
}

typedef struct AreaListGrid {
    BeaconGenerateColumnArea* cell[BEACON_GENERATE_CHUNK_SIZE / 16][BEACON_GENERATE_CHUNK_SIZE / 16];
} AreaListGrid;

static AreaListGrid* areaListGrid;

static void findClosestEdgeInGrid(S32 ex, S32 ez, S32 x, S32 z, BeaconGenerateColumnArea** outBest, U32* distSQR){
    BeaconGenerateColumnArea* bestArea = *outBest;
    U32 bestDistSQR = *distSQR;
    
    if(    ex >= 0 && ex < ARRAY_SIZE(areaListGrid->cell[0]) &&
        ez >= 0 && ez < ARRAY_SIZE(areaListGrid->cell))
    {
        BeaconGenerateColumnArea* edge = areaListGrid->cell[ez][ex];
        
        for(; edge; edge = edge->nextInFlatAreaSubList){
            S32 columnIndex = edge->columnIndex;
            S32 edgex = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
            S32 edgez = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
            U32 edgeDistSQR = SQR(edgex - x) + SQR(edgez - z);
            
            if(edgeDistSQR < bestDistSQR){
                bestArea = edge;
                bestDistSQR = edgeDistSQR;
            }
        }
    }
    
    *outBest = bestArea;
    *distSQR = bestDistSQR;
}

static BeaconGenerateColumnArea* findClosestEdge(S32 x, S32 z, U32* bestDist){
    S32 cx = x / 16;
    S32 cz = z / 16;
    S32 radius = 0;
    S32 stopThen = 0;
    BeaconGenerateColumnArea* bestArea = NULL;
    
    *bestDist = INT_MAX;

    for(;; radius++){
        S32 stopNow = stopThen;
        
        bestArea = NULL;
        
        if(radius){
            S32 i;
            S32 run = radius * 2;
            S32 bx0 = cx - radius;
            S32 bx1 = cx + radius;
            S32 bz0 = cz - radius;
            S32 bz1 = cz + radius;
            S32 curDist = *bestDist;
            S32 use_bz1 = bz1 * 16 - z < curDist;
            S32 use_bx0 = x - (bx0 * 16 + 15) < curDist;
            S32 use_bz0 = z - (bz0 * 16 + 15) < curDist;
            S32 use_bx1 = bx1 * 16 - x < curDist;
            
            for(i = 0; i < run; i++){
                if(use_bz1){
                    findClosestEdgeInGrid(bx1 - i, bz1, x, z, &bestArea, bestDist);
                }
                
                if(use_bx0){
                    findClosestEdgeInGrid(bx0, bz1 - i, x, z, &bestArea, bestDist);
                }

                if(use_bz0){
                    findClosestEdgeInGrid(bx0 + i, bz0, x, z, &bestArea, bestDist);
                }
                
                if(use_bx1){
                    findClosestEdgeInGrid(bx1, bz0 + i, x, z, &bestArea, bestDist);
                }
            }
        }else{
            findClosestEdgeInGrid(cx, cz, x, z, &bestArea, bestDist);
        }
        
        if(stopNow){
            break;
        }
        else if(bestArea){
            stopThen = 1;
        }
    }
    
    return bestArea;
}

static S32 putAreaListIntoGrid(BeaconGenerateColumnArea* area, S32 useBeacon){
    S32 count = 0;
    
    while(area){
        BeaconGenerateColumnArea* next = area->nextInFlatAreaSubList;
        S32 x = (area->columnIndex % BEACON_GENERATE_CHUNK_SIZE) / 16;
        S32 z = (area->columnIndex / BEACON_GENERATE_CHUNK_SIZE) / 16;
        
        //assert(area->flatArea == flatArea);
        
        if(useBeacon){
            area->beacon->edgeCircle.next = areaListGrid->cell[z][x];
            area->beacon->edgeCircle.prev = NULL;
            
            if(area->beacon->edgeCircle.next){
                area->beacon->edgeCircle.next->beacon->edgeCircle.prev = area;
            }
        }else{
            area->nextInFlatAreaSubList = areaListGrid->cell[z][x];
        }
        
        areaListGrid->cell[z][x] = area;

        area = next;
        
        count++;
    }
    
    return count;
}

static S32 restoreListFromGrid(BeaconGenerateColumnArea** listHead, S32 useBeacon){
    S32 x;
    S32 z;
    S32 count = 0;
    
    for(z = 0; z < ARRAY_SIZE(areaListGrid->cell); z++){
        for(x = 0; x < ARRAY_SIZE(areaListGrid->cell[0]); x++){
            BeaconGenerateColumnArea* edge = areaListGrid->cell[z][x];

            while(edge){
                BeaconGenerateColumnArea* next = useBeacon ? edge->beacon->edgeCircle.next : edge->nextInFlatAreaSubList;
            
                if(!useBeacon){
                    edge->nextInFlatAreaSubList = *listHead;
                }else{
                    edge->beacon->edgeCircle.next = edge->beacon->edgeCircle.prev = NULL;
                }
                
                *listHead = edge;
                edge = next;
                
                count++;
            }
            
            areaListGrid->cell[z][x] = NULL;
        }
    }
    
    return count;
}

static void findMaxBeaconDist(    BeaconGenerateColumnArea* placedBeacons,
                                BeaconGenerateColumnArea* areaHead,
                                BeaconGenerateColumnArea** outArea,
                                F32* outMaxDistSQR,
                                S32 isList)
{
    F32 maxDistSQR = *outMaxDistSQR;
    BeaconGenerateColumnArea* maxArea = *outArea;
    BeaconGenerateColumnArea* area;
    
    for(area = areaHead; area; area = area->nextInFlatAreaSubList){
        BeaconGenerateColumnArea* beacon;
        F32 minDistSQR = area->beacon->beaconDistSQR;
        
        if(area->beacon->hasBeacon){
            continue;
        }
        
        for(beacon = placedBeacons; beacon; beacon = beacon->beacon->nextPlacedBeacon){
            F32 distSQR = distance3Squared(area->beacon->beaconPos, beacon->beacon->beaconPos);
            
            if(distSQR < minDistSQR){
                minDistSQR = distSQR;
            }
            
            if(!isList){
                break;
            }
        }
        
        area->beacon->beaconDistSQR = minDistSQR;
        
        if(minDistSQR > maxDistSQR){
            maxDistSQR = minDistSQR;
            maxArea = area;
        }
    }
    
    *outArea = maxArea;
    *outMaxDistSQR = maxDistSQR;
}

#define NEARBY_GRID_MAX_BUFFER 3
#define NEARBY_GRID_RADIUS    ((NEARBY_GRID_MAX_BUFFER) * 2 + 1)
#define NEARBY_GRID_SIDE    ((NEARBY_GRID_RADIUS) * 2 + 1)

typedef struct NearbyAreaGrid {
    struct {
        BeaconGenerateColumnArea*    area;
        S32                            radiusSQR;
        U32                            bad : 1;
    } grid[NEARBY_GRID_SIDE][NEARBY_GRID_SIDE];
    
    struct {
        S32                            radiusSQR;
        S32                            x;
        S32                            z;
        F32                            y;
    } best;
} NearbyAreaGrid;

static void calculateRadiusHelper(NearbyAreaGrid* grid, S32 cx, S32 cz, S32 dx, S32 dz){
    S32 x = cx + dx;
    S32 z = cz + dz;

    if(    x < 0 ||
        x >= ARRAY_SIZE(grid->grid[0]) ||
        z < 0 ||
        z >= ARRAY_SIZE(grid->grid) ||
        !grid->grid[z][x].area)
    {
        S32 radiusSQR = SQR(dx) + SQR(dz);
        
        if(    !grid->grid[cz][cx].radiusSQR ||
            radiusSQR < grid->grid[cz][cx].radiusSQR)
        {
            grid->grid[cz][cx].radiusSQR = radiusSQR;
        }
    }
}

static void calculateRadius(NearbyAreaGrid* grid, S32 cx, S32 cz){
    S32 r;
    S32 radiusToCenterSQR = SQR(cx - ARRAY_SIZE(grid->grid[0]) / 2) + SQR(cz - ARRAY_SIZE(grid->grid) / 2);
    
    if(!grid->grid[cz][cx].area){
        return;
    }
    
    for(r = 1; r < ARRAY_SIZE(grid->grid) / 2 + 2; r++){
        S32 d;
        
        for(d = -r; d < r - 1; d++){
            calculateRadiusHelper(grid, cx, cz, d, r);
            calculateRadiusHelper(grid, cx, cz, -d, -r);
            calculateRadiusHelper(grid, cx, cz, r, -d);
            calculateRadiusHelper(grid, cx, cz, -r, d);
        }
        
        if(grid->grid[cz][cx].radiusSQR){
            break;
        }
    }
    
    if(    grid->grid[cz][cx].radiusSQR > radiusToCenterSQR &&
        grid->grid[cz][cx].radiusSQR > grid->best.radiusSQR)
    {
        grid->best.radiusSQR = grid->grid[cz][cx].radiusSQR;
        grid->best.x = cx;
        grid->best.z = cz;
        grid->best.y = grid->grid[cz][cx].area->y_min;
    }
}

static void calculateNearbyRadiuses(NearbyAreaGrid* grid){
    S32 x;
    S32 z;
    
    for(x = 0; x < ARRAY_SIZE(grid->grid[0]); x++){
        for(z = 0; z < ARRAY_SIZE(grid->grid); z++){
            calculateRadius(grid, x, z);
        }
    }
}

static void createNearbyGridHelper(    NearbyAreaGrid* grid,
                                    BeaconGenerateColumnArea* area,
                                    S32 x,
                                    S32 z)
{
    S32 i;
    
    if(    !area ||
        !grid ||
        x < 0 ||
        x >= ARRAY_SIZE(grid->grid[0]) ||
        z < 0 ||
        z >= ARRAY_SIZE(grid->grid) ||
        grid->grid[z][x].area)
    {
        return;
    }
    
    grid->grid[z][x].area = area;
    
    for(i = 0; i < 8; i++){
        S32 side_x = x + dir_offsets[i][0];
        S32 side_z = z + dir_offsets[i][1];
        BeaconGenerateColumnArea* sideArea = area->sides[i].area;
        
        createNearbyGridHelper(grid, sideArea, side_x, side_z);
    }
}

static void createNearbyGrid(NearbyAreaGrid* grid, BeaconGenerateColumnArea* area){
    ZeroStruct(grid);
    
    createNearbyGridHelper(grid, area, NEARBY_GRID_RADIUS, NEARBY_GRID_RADIUS);
    
    assert(grid->grid[NEARBY_GRID_RADIUS][NEARBY_GRID_RADIUS].area);
    
    calculateNearbyRadiuses(grid);
}

static S32 useNewPositions = 1;

S32 beaconGenerateUseNewPositions(S32 set){
    if(set >= 0){
        useNewPositions = set ? 1 : 0;
    }
    
    return useNewPositions;
}

static void getAreaBeaconPos(BeaconGenerateChunk* chunk, BeaconGenerateColumnArea* area, Vec3 pos){
    S32 columnIndex = area->columnIndex;
    S32 x = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
    S32 z = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
    Vec3 offset = {0,0,0};
    
    if(useNewPositions){
        NearbyAreaGrid grid;
        
        createNearbyGrid(&grid, area);
        
        if(grid.best.radiusSQR){
            offset[0] = (S32)(grid.best.x - ARRAY_SIZE(grid.grid[0]) / 2);
            offset[1] = grid.best.y - area->y_min;
            offset[2] = (S32)(grid.best.z - ARRAY_SIZE(grid.grid) / 2);
        }
    }else{
        S32 offsetCount = 1;
        S32 useEdges = 1;
        S32 j;
        F32 y_min = area->y_min;

        for(j = 0; j < 8; j++){
            BeaconGenerateColumnArea* sideArea = area->sides[j].area;
            
            if(sideArea){
                S32 useOffset = 1;
                
                if(sideArea->inEdgeList){
                    useOffset = useEdges;
                }
                else if(useEdges){
                    zeroVec3(offset);
                    useEdges = 0;
                    offsetCount = 0;
                }

                if(useOffset){
                    offset[0] += dir_offsets[j][0];
                    offset[1] += sideArea->y_min - y_min;
                    offset[2] += dir_offsets[j][1];
                    offsetCount++;
                }
            }
        }

        if(offsetCount > 1){
            scaleVec3(offset, 1.0 / offsetCount, offset);
        }
    }
    
    pos[0] = x + 0.5 + chunk->swapBlock->x;
    pos[1] = area->y_min;
    pos[2] = z + 0.5 + chunk->swapBlock->z;

    addVec3(pos, offset, pos);

    pos[1] += 2;
}

static S32 __cdecl compareEdgeYaws(const void* n1pData, const void* n2pData){
    const BeaconGenerateEdgeNode** n1p = (const BeaconGenerateEdgeNode**)n1pData;
    const BeaconGenerateEdgeNode** n2p = (const BeaconGenerateEdgeNode**)n2pData;
    const BeaconGenerateEdgeNode* n1 = *n1p;
    const BeaconGenerateEdgeNode* n2 = *n2p;
    
    if(n1->yaw < n2->yaw){
        return -1;
    }
    else if(n1->yaw == n2->yaw){
        if(n1->distSQR < n2->distSQR){
            return -1;
        }
        else if(n1->distSQR == n2->distSQR){
            return 0;
        }
    }

    return 1;
}

static BeaconGenerateEdgeNode* sortEdgeNodesClockwise(BeaconGenerateChunk* chunk, BeaconGenerateEdgeNode* startNode){
    static BeaconGenerateEdgeNode** sortArray;

    BeaconGenerateEdgeNode* bestNode = NULL;
    BeaconGenerateEdgeNode* cur = startNode;
    S32 sortCount = 0;
    Vec3 centerPos;
    S32 i;
    
    copyVec3(startNode->columnArea->beacon->beaconPos, centerPos);

    if(!sortArray){
        sortArray = calloc(sizeof(*sortArray), ARRAY_SIZE(chunk->columns));
    }
    
    while(cur){
        Vec3 diff;
        
        diff[0] = cur->columnArea->beacon->beaconPos[0] - centerPos[0];
        diff[2] = cur->columnArea->beacon->beaconPos[2] - centerPos[2];
        
        cur->yaw = getVec3Yaw(diff);
        cur->distSQR = SQR(diff[0]) + SQR(diff[2]);
        
        sortArray[sortCount++] = cur;
        
        if(    !bestNode
            ||
            cur->columnArea->beacon->beaconPos[0] < bestNode->columnArea->beacon->beaconPos[0]
            ||
            cur->columnArea->beacon->beaconPos[0] == bestNode->columnArea->beacon->beaconPos[0] &&
            cur->columnArea->beacon->beaconPos[2] < bestNode->columnArea->beacon->beaconPos[2])
        {
            bestNode = cur;
        }
        
        cur = cur->nextEdgeNode;
    }
    
    qsort(sortArray, sortCount, sizeof(*sortArray), compareEdgeYaws);
    
    for(i = 0; i < sortCount; i++){
        sortArray[i]->nextYawNode = sortArray[(i + 1) % sortCount];
    }

    return bestNode;
}

static void sendBestCorners(BeaconGenerateEdgeNode* farNode1, BeaconGenerateEdgeNode* farNode2){
    addCombatBeacon(farNode1->columnArea->beacon->beaconPos, 1, 1, 1);
    
    if(farNode1 != farNode2){
        addCombatBeacon(farNode2->columnArea->beacon->beaconPos, 1, 1, 1);
    }
    
    SEND_LINE(    farNode1->columnArea->beacon->beaconPos[0],
                farNode1->columnArea->beacon->beaconPos[1],
                farNode1->columnArea->beacon->beaconPos[2],
                farNode2->columnArea->beacon->beaconPos[0],
                farNode2->columnArea->beacon->beaconPos[1],
                farNode2->columnArea->beacon->beaconPos[2],
                0xff0080ff);
}

static void sendEdge(BeaconGenerateChunk* chunk, BeaconGenerateEdgeNode* startNode){
    BeaconGenerateEdgeNode* curNode;
    U32 color = 0xff808080 | ((rand()&127) << 16) | ((rand()&127) << 8) | (rand()&127);
    F32 y_offset = (rand() % 101) * 0.01;

    curNode = startNode->nextYawNode;
    startNode->nextYawNode = NULL;
    startNode = curNode;

    while(curNode){
        BeaconGenerateEdgeNode* next = curNode->nextYawNode ? curNode->nextYawNode : curNode != startNode ? startNode : NULL;
        
        if(next){
            Vec3 center;
            
            addVec3(curNode->columnArea->beacon->beaconPos, next->columnArea->beacon->beaconPos, center);
            scaleVec3(center, 0.5, center);
            center[1] += 0.2;
            
            SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                        curNode->columnArea->beacon->beaconPos[1] + y_offset,
                        curNode->columnArea->beacon->beaconPos[2],
                        center[0],
                        center[1] + y_offset,
                        center[2],
                        0xffffffff);

            SEND_LINE(    center[0],
                        center[1] + y_offset,
                        center[2],
                        next->columnArea->beacon->beaconPos[0],
                        next->columnArea->beacon->beaconPos[1] + y_offset,
                        next->columnArea->beacon->beaconPos[2],
                        0xffff7777);

            //SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
            //            curNode->columnArea->beacon->beaconPos[1] + y_offset,
            //            curNode->columnArea->beacon->beaconPos[2],
            //            next->columnArea->beacon->beaconPos[0],
            //            next->columnArea->beacon->beaconPos[1] + y_offset,
            //            next->columnArea->beacon->beaconPos[2],
            //            color);

            {
                S32 x = curNode->columnArea->columnIndex % BEACON_GENERATE_CHUNK_SIZE + chunk->swapBlock->x;
                S32 z = curNode->columnArea->columnIndex / BEACON_GENERATE_CHUNK_SIZE + chunk->swapBlock->z;
                
                //if(x == -975 && z == 2471){
                //    printf("");
                //}
                
                SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                            curNode->columnArea->beacon->beaconPos[1] + y_offset,
                            curNode->columnArea->beacon->beaconPos[2],
                            x + 0.5,
                            curNode->columnArea->y_min,
                            z + 0.5,
                            0x8000ff00);
            }

            if(curNode == startNode){
                SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                            curNode->columnArea->beacon->beaconPos[1] + y_offset,
                            curNode->columnArea->beacon->beaconPos[2],
                            curNode->columnArea->beacon->beaconPos[0] + 0.5, 
                            curNode->columnArea->beacon->beaconPos[1] + y_offset + 0.5,
                            curNode->columnArea->beacon->beaconPos[2] + 0.5,
                            0x80ff0000);
            }
            
            {
                S32 i;
                for(i = 0; i < 8; i++){
                    if(curNode->columnArea->sides[i].area){
                        SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                                    curNode->columnArea->beacon->beaconPos[1] + 0.1 + y_offset,
                                    curNode->columnArea->beacon->beaconPos[2],
                                    curNode->columnArea->beacon->beaconPos[0] + 0.2 * dir_offsets[i][0],
                                    curNode->columnArea->beacon->beaconPos[1] + 0.1 + y_offset,
                                    curNode->columnArea->beacon->beaconPos[2] + 0.2 * dir_offsets[i][1],
                                    0x80ff00ff | ((i * 31) << 8));
                    }
                }
            }
        }
        
        //addCombatBeacon(curNode->columnArea->beacon->beaconPos, 1, 1);
        curNode = curNode->nextYawNode;
    }
}

static void sendHull(BeaconGenerateEdgeNode* curNode){
    BeaconGenerateEdgeNode* first = curNode;
    U32 color = 0xff000000 | (rand()&255) << 16 | (rand()&255) << 8 | (rand()&255);
    F32 y_offset = (rand() % 101) * 0.01;
    S32 count = 0;
    
    while(curNode){
        BeaconGenerateEdgeNode* next = curNode->hull.next ? curNode->hull.next : curNode != first ? first : NULL;
        
        if(0)switch(count++){
            case 0: color = 0xffff0000;break;
            case 1: color = 0xff00ff00;break;
            case 2: color = 0xff0000ff;break;
            case 3: color = 0xffffffff;count = 0;break;
        }
        
        if(next){
            SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                        curNode->columnArea->beacon->beaconPos[1] + y_offset,
                        curNode->columnArea->beacon->beaconPos[2],
                        next->columnArea->beacon->beaconPos[0],
                        next->columnArea->beacon->beaconPos[1] + y_offset,
                        next->columnArea->beacon->beaconPos[2],
                        color);

            if(0){
                SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
                            curNode->columnArea->beacon->beaconPos[1] + y_offset,
                            curNode->columnArea->beacon->beaconPos[2],
                            curNode->columnArea->beacon->beaconPos[0] + 0.2 * cos(count * PI / 2), 
                            curNode->columnArea->beacon->beaconPos[1] + 0.2 + y_offset,
                            curNode->columnArea->beacon->beaconPos[2] + 0.2 * sin(count * PI / 2),
                            color);
            }
        }
        
        //if(curNode == startNode){
        //    SEND_LINE(    curNode->columnArea->beacon->beaconPos[0], 
        //                curNode->columnArea->beacon->beaconPos[1] + y_offset,
        //                curNode->columnArea->beacon->beaconPos[2],
        //                curNode->columnArea->beacon->beaconPos[0] + 0.5, 
        //                curNode->columnArea->beacon->beaconPos[1] + y_offset + 0.5,
        //                curNode->columnArea->beacon->beaconPos[2] + 0.5,
        //                0x80ff0000);
        //}
        
        //addCombatBeacon(curNode->columnArea->beacon->beaconPos, 1, 1);
        curNode = curNode->hull.next;
    }
}

static void findFarthestHullNodes(    BeaconGenerateEdgeNode* headNode,
                                    BeaconGenerateEdgeNode** farNode1Out,
                                    BeaconGenerateEdgeNode** farNode2Out)
{
    BeaconGenerateEdgeNode* cur = headNode;
    BeaconGenerateEdgeNode* farNode1;
    BeaconGenerateEdgeNode* farNode2;
    F32 maxDistSQR = -1;
    
    farNode1 = farNode2 = cur;

    while(cur){
        F32* curPos = cur->columnArea->beacon->beaconPos;
        BeaconGenerateEdgeNode* otherNode;
        
        for(otherNode = cur->hull.next; otherNode; otherNode = otherNode->hull.next){
            F32 distSQR = distance3SquaredXZ(otherNode->columnArea->beacon->beaconPos, curPos);
            
            if(distSQR > maxDistSQR){
                farNode1 = cur;
                farNode2 = otherNode;
                maxDistSQR = distSQR;
            }
        }
        
        cur = cur->hull.next;
    }
    
    *farNode1Out = farNode1;
    *farNode2Out = farNode2;
}

static void traceEdge(    BeaconGenerateColumnArea* curArea,
                        BeaconGenerateEdgeNode** startNodeOut,
                        BeaconGenerateEdgeNode** tailNodeOut,
                        S32* nodeCountOut,
                        S32 quiet)
{
    BeaconGenerateEdgeNode* curNode = NULL;
    BeaconGenerateEdgeNode* startNode = NULL;
    BeaconGenerateColumnArea* startArea = NULL;
    S32 nodeCount = 0;
    S32 curDir;
    
    while(1){
        if(!startArea){
            S32 j;
            
            startArea = curArea;

            for(j = 0; j < 8; j++){
                assert(curArea->foundSides & (1 << j));
                
                if(!(curArea->usedSides & (1 << j))){
                    if(!curArea->sides[j].area){
                        curDir = j;
                        break;
                    }else{
                        //curArea->usedSides |= (1 << j);
                    }
                }
            }
            
            if(j == 8){
                curArea->usedSides = 0xff;
                //assert(area->usedSides == 0xff);
                break;
            }
            
            if(!quiet){
                printf("t");
            }
        }
        
        if(curArea == startArea && curArea->usedSides & (1 << curDir)){
            break;
        }
        
        curArea->isEdge = 1;
        
        nodeCount++;
        
        if(curNode){
            curNode->nextEdgeNode = createBeaconGenerateEdgeNode();
            curNode = curNode->nextEdgeNode;
        }else{
            curNode = startNode = createBeaconGenerateEdgeNode();
        }
        
        curNode->columnArea = curArea;
        
        // Get the min/max nodes for the hull.

        //if(!nodes.minX || curArea->beacon->beaconPos[0] < nodes.minX->columnArea->beacon->beaconPos[0]){
        //    nodes.minX = curNode;
        //}
        //if(!nodes.maxX || curArea->beacon->beaconPos[0] > nodes.maxX->columnArea->beacon->beaconPos[0]){
        //    nodes.maxX = curNode;
        //}
        //if(!nodes.minX || curArea->beacon->beaconPos[0] < nodes.minX->columnArea->beacon->beaconPos[0]){
        //    nodes.minX = curNode;
        //}
        //if(!nodes.minX || curArea->beacon->beaconPos[0] < nodes.minX->columnArea->beacon->beaconPos[0]){
        //    nodes.minX = curNode;
        //}
        
        {
            S32 backupCount = 0;
            while(!curArea->sides[curDir].area){
                curDir = (curDir - 1 + 8) % 8;
                if(++backupCount == 8)
                    break;
            }
        }
        
        curDir = (curDir + 1) % 8;

        while(!curArea->sides[curDir].area){
            if(!(curArea->usedSides & (1 << curDir))){
                curArea->usedSides |= (1 << curDir);
            }else{
                break;
            }
            
            curDir = (curDir + 1) % 8;
        }
        
        if(curArea->sides[curDir].area){
            // Find the next edge.
            
            BeaconGenerateColumnArea* newArea;
            S32 newDir;
            
            if(curDir & 1){
                // For a diagonal conn, check if I have the next perpendicular conn, and then jackknife it.
                
                S32 nextDir = (curDir + 1) % 8;
                
                if(curArea->sides[nextDir].area){
                    newArea = curArea->sides[nextDir].area;
                    newDir = (curDir - 2 + 8) % 8;
                }else{
                    newArea = curArea->sides[curDir].area;
                    newDir = (curDir - 3 + 8) % 8;
                }
            }else{
                newArea = curArea->sides[curDir].area;
                newDir = (curDir - 2 + 8) % 8;
            }

            assert(!newArea->sides[newDir].area);
            
            curArea = newArea;
            curDir = newDir;
        }else{
            assert(curArea == startArea);
        }
    }
    
    *startNodeOut = startNode;
    *tailNodeOut = curNode;
    *nodeCountOut = nodeCount;
}

static S32 nodeCanSeeNode(BeaconGenerateChunk* chunk, BeaconGenerateEdgeNode* startNode, BeaconGenerateEdgeNode* endNode){
    BeaconGenerateFlatArea* flatArea = startNode->columnArea->beacon->flatArea;
    S32 block_x = chunk->swapBlock->x;
    S32 block_z = chunk->swapBlock->z;
    S32 x0 = 256 * (startNode->columnArea->beacon->beaconPos[0] - block_x);
    S32 z0 = 256 * (startNode->columnArea->beacon->beaconPos[2] - block_z);
    S32 x1 = 256 * (endNode->columnArea->beacon->beaconPos[0] - block_x);
    S32 z1 = 256 * (endNode->columnArea->beacon->beaconPos[2] - block_z);
    S32 dx = x1 - x0;
    S32 dz = z1 - z0;
    S32 x_major;
    S32 acc;
    S32 x_add;
    S32 z_add;
    S32 acc_add;
    S32 acc_max;
    S32 count;
    S32 x = x0;
    S32 z = z0;
    S32 cur_block_x = x & ~255;
    S32 cur_block_z = z & ~255;
    
    if(abs(dx) >= abs(dz)){
        x_major = 1;
        acc_max = abs(dx);
        acc_add = abs(dz);
    }else{
        x_major = 0;
        acc_max = abs(dz);
        acc_add = abs(dx);
    }
    
    count = acc_max;
    acc = acc_max / 2;
    x_add = dx < 0 ? -1 : dx ? 1 : 0;
    z_add = dz < 0 ? -1 : dz ? 1 : 0;
    
    for(; count; count--){
        S32 block_x;
        S32 block_z;
        
        if(x_major){
            x += x_add;
        }else{
            z += z_add;
        }
        
        acc += acc_add;
        
        if(acc > acc_max){
            acc -= acc_max;
            
            if(x_major){
                z += z_add;
            }else{
                x += x_add;
            }
        }
        
        block_x = x & ~255;
        block_z = z & ~255;
        
        if(block_x != cur_block_x || block_z != cur_block_z){
            cur_block_x = block_x;
            cur_block_z = block_z;
            
            block_x >>= 8;
            block_z >>= 8;
            
            if(chunk->columns[block_x + block_z * BEACON_GENERATE_CHUNK_SIZE].propagate.flatArea != flatArea){
                S32 check;
                S32 found = 0;
                
                if((x & 255) < 64){
                    block_x = ((x - 64) & ~255) >> 8;
                    check = block_x >= 0;
                }
                else if((x & 255) >= 256 - 64){
                    block_x = ((x - 64) & ~255) >> 8;
                    check = block_x < BEACON_GENERATE_CHUNK_SIZE;
                }
                else{
                    check = 0;
                }
                
                if(check){
                    if(chunk->columns[block_x + block_z * BEACON_GENERATE_CHUNK_SIZE].propagate.flatArea == flatArea){
                        found = 1;
                    }
                }
                
                if(!found){
                    block_x = (x & ~255) >> 8;
                    
                    if((z & 255) < 64){
                        block_z = ((z - 64) & ~255) >> 8;
                        check = block_z >= 0;
                    }
                    else if((z & 255) >= 256 - 64){
                        block_z = ((z - 64) & ~255) >> 8;
                        check = block_z < BEACON_GENERATE_CHUNK_SIZE;
                    }
                    else{
                        check = 0;
                    }
                    
                    if(check){
                        if(chunk->columns[block_x + block_z * BEACON_GENERATE_CHUNK_SIZE].propagate.flatArea == flatArea){
                            found = 1;
                        }
                    }
                }
                
                if(!found){
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

static void addGeneratedBeacon(Vec3 pos, S32 noGroundConnections){
    BeaconGeneratedBeacon* genBeacon;
    S32 i = bp_blocks.generatedBeacons.count;
    
    dynArrayAddStruct(    &bp_blocks.generatedBeacons.beacons,
                        &bp_blocks.generatedBeacons.count,
                        &bp_blocks.generatedBeacons.maxCount);
                
    genBeacon = bp_blocks.generatedBeacons.beacons + i;
    copyVec3(pos, genBeacon->pos);
    genBeacon->noGroundConnections = noGroundConnections;
}
            
static void makeBeaconsAlongEdge(BeaconGenerateColumnArea** placedBeacons, BeaconGenerateChunk* chunk, BeaconGenerateEdgeNode* startNode){
    BeaconGenerateEdgeNode* curStartNode = startNode;
    
    while(curStartNode){
        BeaconGenerateEdgeNode* curNode;
        BeaconGenerateEdgeNode* prevNode = curStartNode;
        
        if(!curStartNode->columnArea->makeBeacon){
            BeaconGenerateColumnArea* area = curStartNode->columnArea;
            
            area->beacon->nextPlacedBeacon = *placedBeacons;
            area->beacon->hasBeacon = 1;
            area->makeBeacon = 1;
            *placedBeacons = area;
            
            addGeneratedBeacon(area->beacon->beaconPos, area->noGroundConnections);
        
            //addCombatBeacon(area->beacon->beaconPos, 1, 1);
        }
        
        for(curNode = curStartNode->nextEdgeNode; curNode; curNode = curNode->nextEdgeNode){
            if(!nodeCanSeeNode(chunk, curStartNode, curNode)){
                curNode = prevNode;

                if(curNode == curStartNode){
                    curNode = curNode->nextEdgeNode;
                }
                
                break;
            }
            
            if(curNode == startNode){
                break;
            }

            prevNode = curNode;
        }

        if(curNode == startNode){
            break;
        }

        curStartNode = curNode;
    }
}

static void addFlatAreasToChunkColumns(BeaconGenerateChunk* chunk, BeaconGenerateColumnArea* areas){
    BeaconGenerateFlatArea* flatArea;
    
    if(!areas)
        return;
        
    flatArea = areas->beacon->flatArea;
    
    for(; areas; areas = areas->nextInFlatAreaSubList){
        S32 index = areas->columnIndex;
        chunk->columns[index].propagate.curArea = areas;
        chunk->columns[index].propagate.flatArea = flatArea;
    }
}

static S32 __cdecl compareEdgeDistance(const void* a1pData, const void* a2pData){
    const BeaconGenerateColumnArea** a1p = (const BeaconGenerateColumnArea**)a1pData;
    const BeaconGenerateColumnArea** a2p = (const BeaconGenerateColumnArea**)a2pData;
    const BeaconGenerateBeaconingInfo* b1 = (*a1p)->beacon;
    const BeaconGenerateBeaconingInfo* b2 = (*a2p)->beacon;
    
    if(b1->edgeDistSQR > b2->edgeDistSQR){
        return -1;
    }
    else if(b1->edgeDistSQR < b2->edgeDistSQR){
        return 1;
    }
    else{    
        return 0;
    }
}

static S32 sortAreasByEdgeDistance(BeaconGenerateFlatArea* flatArea, BeaconGenerateColumnArea*** sortArrayOut){
    static BeaconGenerateColumnArea**    sortArray;
    static S32                            sortMaxCount;
    
    BeaconGenerateColumnArea*            area;
    S32                                    sortCount = 0;
    
    for(area = flatArea->nonEdgeList; area; area = area->nextInFlatAreaSubList){
        BeaconGenerateColumnArea** newSlot = dynArrayAddStruct(&sortArray, &sortCount, &sortMaxCount);
        *newSlot = area;
    }
    
    qsort(sortArray, sortCount, sizeof(sortArray[0]), compareEdgeDistance);
    
    *sortArrayOut = sortArray;
    
    return sortCount;
}

static void beaconCreateColumnAreaBeaconingInfo(BeaconGenerateFlatArea* flatArea,
                                                U32* maxEdgeDistSQR)
{
    BeaconGenerateChunk* chunk = flatArea->chunk;
    BeaconGenerateColumnArea* area;

    *maxEdgeDistSQR = 0;

    for(area = flatArea->nonEdgeList; area; area = area->nextInFlatAreaSubList){
        S32 x = area->columnIndex % BEACON_GENERATE_CHUNK_SIZE;
        S32 z = area->columnIndex / BEACON_GENERATE_CHUNK_SIZE;
        BeaconGenerateColumnArea* edgeArea;
        BeaconGenerateBeaconingInfo* info;
        
        assert(area->flatArea == flatArea);
        
        area->beacon = info = createBeaconGenerateBeaconingInfo();
        area->beacon->flatArea = flatArea;
        
        area->beacon->beaconPos[0] = x + 0.5 + chunk->swapBlock->x;
        area->beacon->beaconPos[1] = area->y_min;
        area->beacon->beaconPos[2] = z + 0.5 + chunk->swapBlock->z;
        area->beacon->beaconDistSQR = FLT_MAX;
        
        edgeArea = findClosestEdge(x, z, &info->edgeDistSQR);
        
        if(info->edgeDistSQR > *maxEdgeDistSQR){
            *maxEdgeDistSQR = info->edgeDistSQR;
        }
    }
}

static void makeBeaconsInFlatArea(    BeaconGenerateChunk* chunk,
                                    BeaconGenerateFlatArea* flatArea,
                                    S32 quiet)
{
    BeaconGenerateColumnArea* placedBeacons = NULL;
    S32 oldEdgeCount = 0;
    S32 edgeCount = 0;
    BeaconGenerateColumnArea* area;
    U32 maxEdgeDistSQR;
    
    if(flatArea->foundBeaconPositions){
        return;
    }
    
    flatArea->foundBeaconPositions = 1;
    
    if(    flatArea->edgeList &&
        flatArea->edgeList->slope != BGSLOPE_FLAT &&
        flatArea->edgeList->slope != BGSLOPE_SLIPPERY)
    {
        return;
    }
        
    assert(!flatArea->checkList);
    
    if(!quiet){
        printf("\n  [f:");
    }

    // Put the edges into grid edge lists.
    
    if(!quiet){
        printf("e");
    }
    
    oldEdgeCount = putAreaListIntoGrid(flatArea->edgeList, 0);
    
    if(!quiet){
        printf("(%d)", oldEdgeCount);
    }
    
    flatArea->edgeList = NULL;

    // Create the beaconing info.  "beacon" is unioned with the "flatArea" pointer, so have to restore later.
    
    if(!quiet){
        printf("b");
    }
    
    beaconCreateColumnAreaBeaconingInfo(flatArea, &maxEdgeDistSQR);

    //printf("maxEdgeDist: %1.2f\n", sqrt(maxEdgeDistSQR));
    
    {
        S32 newEdgeCount = restoreListFromGrid(&flatArea->edgeList, 0);
        assert(newEdgeCount == oldEdgeCount);
    }
    
    if(!quiet){
        printf("p");
    }

    for(area = flatArea->edgeList; area; area = area->nextInFlatAreaSubList){
        S32 x = area->columnIndex % BEACON_GENERATE_CHUNK_SIZE;
        S32 z = area->columnIndex / BEACON_GENERATE_CHUNK_SIZE;

        area->usedSides = 0;
        area->beacon = createBeaconGenerateBeaconingInfo();
        area->beacon->flatArea = flatArea;
        area->beacon->beaconDistSQR = FLT_MAX;
        
        getAreaBeaconPos(chunk, area, area->beacon->beaconPos);
    }
    
    if(!quiet){
        printf("i");
    }
    
    addFlatAreasToChunkColumns(chunk, flatArea->edgeList);
    addFlatAreasToChunkColumns(chunk, flatArea->nonEdgeList);

    //printf("\n");
    
    for(area = flatArea->edgeList; area; area = area->nextInFlatAreaSubList){
        struct {
            BeaconGenerateEdgeNode* minX;
            BeaconGenerateEdgeNode* maxX;
            BeaconGenerateEdgeNode* minZ;
            BeaconGenerateEdgeNode* maxZ;
        } nodes = {0};

        assert(area->inEdgeList);
        
        while(area->usedSides != 0xff){
            BeaconGenerateEdgeNode* startNode;
            BeaconGenerateEdgeNode* tailNode;
            S32 nodeCount = 0;
            
            traceEdge(area, &startNode, &tailNode, &nodeCount, quiet);
                            
            // Create the convex hull (there's a tiny bug in this for co-linear points, but it doesn't matter).
            
            if(tailNode && nodeCount > 1){
                BeaconGenerateEdgeNode* curNode = tailNode;
                BeaconGenerateEdgeNode* lastNode = startNode;
                BeaconGenerateEdgeNode* farNode1;
                BeaconGenerateEdgeNode* farNode2;
                
                if(!quiet){
                    printf("h");
                }

                // Reorder to be clockwise around first point.
                
                lastNode = sortEdgeNodesClockwise(chunk, startNode);
                
                curNode = lastNode->nextYawNode;
                
                lastNode->hull.next = curNode;
                curNode->hull.prev = lastNode;
            
                // Create the convex hull now.
                
                while(1){
                    BeaconGenerateEdgeNode* curLastNode;
                    S32 lastDot;
                    
                    lastNode = curNode;
                    curNode = curNode->hull.next ? curNode->hull.next : curNode->nextYawNode;
                    
                    curLastNode = lastNode;
                    
                    while(lastNode->hull.prev){
                        F32 dot;
                        Vec3 prevDiff;
                        Vec3 prevDiffNormal;
                        Vec3 diff;
                        
                        prevDiff[0] = lastNode->columnArea->beacon->beaconPos[0] - lastNode->hull.prev->columnArea->beacon->beaconPos[0];
                        prevDiff[2] = lastNode->columnArea->beacon->beaconPos[2] - lastNode->hull.prev->columnArea->beacon->beaconPos[2];
                        
                        prevDiffNormal[0] = prevDiff[2];
                        prevDiffNormal[2] = -prevDiff[0];

                        diff[0] = curNode->columnArea->beacon->beaconPos[0] - lastNode->columnArea->beacon->beaconPos[0];
                        diff[2] = curNode->columnArea->beacon->beaconPos[2] - lastNode->columnArea->beacon->beaconPos[2];
                        
                        dot = prevDiffNormal[0] * diff[0] + prevDiffNormal[2] * diff[2];
                        
                        lastDot = 0;
                        
                        if(dot > 0){
                            break;
                        }
                        else{
                            lastDot = dot == 0;
                            
                            if(dot == 0){
                                dot = prevDiff[0] * diff[0] + prevDiff[2] * diff[2];
                                
                                if(dot < 0){
                                    break;
                                }
                            }
                            
                            if(lastNode == lastNode->hull.prev){
                                break;
                            }
                        }

                        lastNode = lastNode->hull.prev;
                    }
                    
                    if(lastNode->hull.next == curNode){
                        curNode = lastNode->hull.next;
                        lastNode->hull.next = NULL;
                        curNode->hull.prev = NULL;
                        break;
                    }

                    lastNode->hull.next = curNode;
                    curNode->hull.prev = lastNode;
                }
                
                // Now curNode is the head of the hull list.
                
                assert(curNode);

                // Find the two farthest points in the hull.
                
                findFarthestHullNodes(curNode, &farNode1, &farNode2);
                
                // Send debug stuff.
                
                if(beaconGetDebugVar("showedge")){
                    sendEdge(chunk, startNode);
                }
                else if(beaconGetDebugVar("showhull")){
                    sendHull(curNode);
                }

                if(beaconGetDebugVar("showbestcorners")){
                    sendBestCorners(farNode1, farNode2);
                }
                
                // Calculate the minimal edges.
                
                tailNode->nextEdgeNode = startNode;
                
                if(!quiet){
                    printf("m");
                }
                
                makeBeaconsAlongEdge(&placedBeacons, chunk, farNode1);

                tailNode->nextEdgeNode = NULL;
            }
                                            
            // Free the EdgeNode list.
            
            while(startNode){
                BeaconGenerateEdgeNode* next = startNode->nextEdgeNode;
                destroyBeaconGenerateEdgeNode(startNode);
                startNode = next;
            }
        }
    }

    //printf("\n...End FlatArea\n");
    
    // Place beacons in areas that are too far from a beacon.
    
    if(!beaconGetDebugVar("nocenterfill")){
        S32 sortCount;
        BeaconGenerateColumnArea** sortArray;
        S32 i;
        S32 remaining;
        
        // Sort the beacons by distance from an edge, descending.
        
        if(!quiet){
            printf("s");
        }
        
        sortCount = sortAreasByEdgeDistance(flatArea, &sortArray);
        
        remaining = putAreaListIntoGrid(flatArea->nonEdgeList, 1);
        
        if(!quiet){
            printf("(%d)", sortCount);
        }
        
        // Find the biggest circle that each full point is inside.
        
        {
            U64 ticksFound = 0;
            U64 ticksNotFound = 0;
            S32 foundCount = 0;
            S32 notFoundCount = 0;
        
            for(i = 0; i < sortCount; i++){
                BeaconGenerateColumnArea* centerArea = sortArray[i];
                U32 edgeDistSQR;
                F32 edgeDist;
                S32 center_x;
                S32 center_z;
                S32 grid_min_x;
                S32 grid_max_x;
                S32 grid_min_z;
                S32 grid_max_z;
                S32 gridx;
                S32 gridz;
                S32 foundCount = 0;
                U64 ticksStart;
                
                if(centerArea->beacon->doNotCheckMyCircle)
                    continue;
                    
                GET_CPU_TICKS_64(ticksStart);
                
                edgeDistSQR = centerArea->beacon->edgeDistSQR;
                edgeDist = ceil(sqrt(edgeDistSQR));
                center_x = centerArea->columnIndex % BEACON_GENERATE_CHUNK_SIZE;
                center_z = centerArea->columnIndex / BEACON_GENERATE_CHUNK_SIZE;
                grid_min_x = (center_x - edgeDist) / 16;
                grid_max_x = (center_x + edgeDist) / 16;
                grid_min_z = (center_z - edgeDist) / 16;
                grid_max_z = (center_z + edgeDist) / 16;

                if(grid_min_x < 0)
                    grid_min_x = 0;
                if(grid_max_x > ARRAY_SIZE(areaListGrid->cell[0]) - 1)
                    grid_max_x = ARRAY_SIZE(areaListGrid->cell[0]) - 1;
                if(grid_min_z < 0)
                    grid_min_z = 0;
                if(grid_max_z > ARRAY_SIZE(areaListGrid->cell) - 1)
                    grid_max_z = ARRAY_SIZE(areaListGrid->cell) - 1;
                
                //printf("\n%5d, %5d, %5d, %5d", remaining, center_x, center_z, edgeDistSQR);
                
                for(gridx = grid_min_x; gridx <= grid_max_x; gridx++){
                    for(gridz = grid_min_z; gridz <= grid_max_z; gridz++){
                        BeaconGenerateColumnArea** listHead = &areaListGrid->cell[gridz][gridx];
                        BeaconGenerateColumnArea* area;
                        
                        for(area = *listHead; area; area = area ? area->beacon->edgeCircle.next : *listHead){
                            S32 x = area->columnIndex % BEACON_GENERATE_CHUNK_SIZE;
                            S32 z = area->columnIndex / BEACON_GENERATE_CHUNK_SIZE;
                            U32 distSQR = SQR(x - center_x) + SQR(z - center_z);
                            BeaconGenerateColumnArea* prev;
                            
                            if(distSQR > edgeDistSQR){
                                continue;
                            }
                            
                            if(sqrt(distSQR) + sqrt(area->beacon->edgeDistSQR) <= edgeDist){
                                area->beacon->doNotCheckMyCircle = 1;
                            }
                                
                            prev = area->beacon->edgeCircle.prev;
                            
                            area->beacon->edgeCircleRadius = edgeDist;
                            
                            foundCount++;
                            remaining--;
                            
                            // Found one in my circle, so remove it from the unplaced list.
                            
                            if(prev){
                                prev->beacon->edgeCircle.next = area->beacon->edgeCircle.next;
                            }else{
                                *listHead = area->beacon->edgeCircle.next;
                            }
                            
                            if(area->beacon->edgeCircle.next){
                                area->beacon->edgeCircle.next->beacon->edgeCircle.prev = prev;
                            }
                            
                            area->beacon->edgeCircle.next = area->beacon->edgeCircle.prev = NULL;
                            
                            area = prev;
                        }                        
                    }
                }
                
                if(foundCount){
                    U64 endTime;
                    GET_CPU_TICKS_64(endTime);
                    ticksFound += endTime - ticksStart;
                    //printf("\n%5d, %5d, %5d, %5d, %5d", remaining, center_x, center_z, edgeDistSQR, foundCount);
                    foundCount++;
                }else{
                    U64 endTime;
                    GET_CPU_TICKS_64(endTime);
                    ticksNotFound += endTime - ticksStart;
                    notFoundCount++;
                }

                if(!remaining){
                    break;
                }
            }

            if(!quiet){
                printf(    "\n"
                        "Found:    %I64u ticks for %d found (%I64u ticks/check)\n"
                        "NotFound: %I64u ticks for %d not found (%I64u ticks/check)\n",
                        ticksFound,
                        foundCount,
                        (U64)(foundCount ? ticksFound / foundCount : 0),
                        ticksNotFound,
                        notFoundCount,
                        (U64)(notFoundCount ? ticksNotFound / notFoundCount : 0));
            }
        }

        //printf("]");

        restoreListFromGrid(&flatArea->nonEdgeList, 1);
        
        {
            S32 useList = 1;
            F32 cutOffDistSQR = maxEdgeDistSQR;
            
            if(!quiet){
                printf("f");
            }

            if(cutOffDistSQR < SQR(20)){
                cutOffDistSQR = SQR(20);
            }
            else if(cutOffDistSQR > SQR(64)){
                cutOffDistSQR = SQR(64);
            }
            
            cutOffDistSQR /= 4.0;
        
            while(1){
                F32 maxDistSQR = -FLT_MAX;
                
                area = NULL;

                findMaxBeaconDist(placedBeacons, flatArea->edgeList, &area, &maxDistSQR, useList);
                findMaxBeaconDist(placedBeacons, flatArea->nonEdgeList, &area, &maxDistSQR, useList);
                
                if(area && maxDistSQR > cutOffDistSQR){
                    S32 columnIndex = area->columnIndex;
                    S32 x = columnIndex % BEACON_GENERATE_CHUNK_SIZE;
                    S32 z = columnIndex / BEACON_GENERATE_CHUNK_SIZE;
                    Vec3 pos;
                    
                    getAreaBeaconPos(chunk, area, pos);
                    
                    addGeneratedBeacon(pos, area->noGroundConnections);

                    //newBeacon = addCombatBeacon(pos, 1, 1);
                    //if(newBeacon){
                    //    newBeacon->noGroundConnections = area->noGroundConnections;
                    //}
                    
                    assert(!area->beacon->hasBeacon);
                    
                    copyVec3(pos, area->beacon->beaconPos);
                    area->beacon->nextPlacedBeacon = placedBeacons;
                    area->beacon->hasBeacon = 1;
                    area->makeBeacon = 1;
                    placedBeacons = area;
                    
                    if(!quiet){
                        printf(".");
                    }
                }else{
                    break;
                }
                
                useList = 0;
            }
        }
    }

    if(!quiet){
        printf("r");
    }
    
    // Reset the flatArea pointers.
    
    for(area = flatArea->edgeList; area; area = area->nextInFlatAreaSubList){
        destroyBeaconGenerateBeaconingInfo(area->beacon);
        area->flatArea = flatArea;
    }

    for(area = flatArea->nonEdgeList; area; area = area->nextInFlatAreaSubList){
        destroyBeaconGenerateBeaconingInfo(area->beacon);
        area->flatArea = flatArea;
    }
    
    if(!quiet){
        printf("]");
    }
}

void beaconMakeBeaconsInChunk(BeaconGenerateChunk* chunk, S32 quiet){
    AreaListGrid localEdgeLists;
    BeaconGenerateFlatArea* flatArea;

    if(!chunk){
        return;
    }
    
    areaListGrid = &localEdgeLists;
    
    ZeroStruct(areaListGrid);
    
    bp_blocks.generatedBeacons.count = 0;
    
    for(flatArea = chunk->flatAreas; flatArea; flatArea = flatArea->nextChunkArea){
        if(!flatArea->edgeList->doNotMakeBeaconsInVolume){
            makeBeaconsInFlatArea(chunk, flatArea, quiet);
        }else{
            //printf("skipping flatarea!\n");
        }
    }
}

static void makeBeaconsInChunks(){
    BeaconDiskSwapBlock* swapBlock;
    S32 doneCount = 0;
    
    beaconProcessSetTitle(0, "Beacon");
    
    printf("\n\nBeaconing: ");
    
    for(swapBlock = bp_blocks.list; swapBlock; swapBlock = swapBlock->nextSwapBlock){
        beaconProcessSetTitle(100.0f * doneCount++ / bp_blocks.count, NULL);
        
        printf("\n\nCHUNK(%s): ", beaconCurTimeString(0));

        beaconMakeBeaconsInChunk(swapBlock->chunk, 0);
    }
    
    printf("\nDone!\n");

    beaconProcessSetTitle(100, NULL);
}    

static void sendLinesToClient(){
    BeaconDiskSwapBlock* swapBlock;
    S32 i;
    
    // Send the legal connections.
    
    printf("Sending lines: ");
    
    if(legals.count){
        for(i = 0; i < legals.count; i++){
            LegalConn* l = legals.conns + i;
            BeaconGenerateColumn* srcColumn = l->src.block->chunk->columns + l->src.columnIndex;
            BeaconGenerateColumn* dstColumn = l->dst.block->chunk->columns + l->dst.columnIndex;
            BeaconGenerateColumnArea* srcArea = srcColumn->areas;
            BeaconGenerateColumnArea* dstArea = dstColumn->areas;
            
            while(srcArea && srcArea->columnAreaIndex != l->src.areaIndex)
                srcArea = srcArea->nextColumnArea;

            while(dstArea && dstArea->columnAreaIndex != l->dst.areaIndex)
                dstArea = dstArea->nextColumnArea;
                
            if(srcArea && dstArea && srcArea->flatArea != dstArea->flatArea && !dstArea->flatArea->inBoundConn){
                Vec3 srcPos = { l->src.block->x + 0.5 + (srcColumn - srcColumn->chunk->columns) % BEACON_GENERATE_CHUNK_SIZE,
                                srcArea->y_min + 0.1,
                                l->src.block->z + 0.5 + (srcColumn - srcColumn->chunk->columns) / BEACON_GENERATE_CHUNK_SIZE};
                Vec3 dstPos = { l->dst.block->x + 0.5 + (dstColumn - dstColumn->chunk->columns) % BEACON_GENERATE_CHUNK_SIZE,
                                dstArea->y_min + 0.1,
                                l->dst.block->z + 0.5 + (dstColumn - dstColumn->chunk->columns) / BEACON_GENERATE_CHUNK_SIZE};
                Vec3 halfWay;
                
                addVec3(srcPos, dstPos, halfWay);
                scaleVec3(halfWay, 0.5, halfWay);
                                
                SEND_LINE(    srcPos[0], srcPos[1], srcPos[2],
                            halfWay[0], halfWay[1], halfWay[2],
                            0xff00ff00);

                SEND_LINE(    halfWay[0], halfWay[1], halfWay[2],
                            dstPos[0], dstPos[1], dstPos[2],
                            0xffff0000);

                dstArea->flatArea->inBoundConn = l;

                if(srcArea->flatArea->inBoundConn){
                    LegalConn* l = srcArea->flatArea->inBoundConn;
                    BeaconGenerateColumn* dstColumn = l->dst.block->chunk->columns + l->dst.columnIndex;
                    BeaconGenerateColumnArea* dstArea = dstColumn->areas;
                    
                    while(dstArea && dstArea->columnAreaIndex != l->dst.areaIndex)
                        dstArea = dstArea->nextColumnArea;
                        
                    if(dstArea){
                        Vec3 dstPos = { l->dst.block->x + 0.5 + (dstColumn - dstColumn->chunk->columns) % BEACON_GENERATE_CHUNK_SIZE,
                                        dstArea->y_min + 0.1,
                                        l->dst.block->z + 0.5 + (dstColumn - dstColumn->chunk->columns) / BEACON_GENERATE_CHUNK_SIZE};
                                        
                        SEND_LINE(    srcPos[0], srcPos[1], srcPos[2],
                                    dstPos[0], dstPos[1], dstPos[2],
                                    0x80ff8000);
                    }
                }
            }
        }
    }
    
    if(beaconGetDebugVar("sendbeacons")){
        for(i = 0; i < combatBeaconArray.size; i++){
            Beacon* b = combatBeaconArray.storage[i];
            SEND_BEACON(posX(b), posY(b) - 2, posZ(b), 0xffff8000);
        }
    }
            
    // Send the other stuff.
    
    for(swapBlock = bp_blocks.list; swapBlock; swapBlock = swapBlock->nextSwapBlock){
        BeaconGenerateChunk* chunk;
        
        if(swapBlock->sent){
            continue;
        }
        
        swapBlock->sent = 1;
        
        chunk = swapBlock->chunk;

        if(!chunk){
            continue;
        }
        
        for(i = 0; i < ARRAY_SIZE(chunk->columns); i++){
            BeaconGenerateColumnArea* columnArea;
            S32 x = i % BEACON_GENERATE_CHUNK_SIZE;
            S32 z = i / BEACON_GENERATE_CHUNK_SIZE;
            
            x += chunk->swapBlock->x;
            z += chunk->swapBlock->z;
            
            for(columnArea = chunk->columns[i].areas; columnArea; columnArea = columnArea->nextColumnArea){
                if(!columnArea->flatArea){
                    //if(columnArea->y_min > 100 && columnArea->y_min < 130){
                    //    SEND_BEACON(x, columnArea->y_min, z, columnArea->isLegal ? 0xff00ff00 : (columnArea->slope == BGSLOPE_VERTICAL) ? 0xffff00ff : 0xffff0000);
                    //    //SEND_LINE(x, columnArea->y_min, z, x, columnArea->y_max, z, columnArea->isLegal ? 0xff00ff00 : (columnArea->slope == BGSLOPE_VERTICAL) ? 0xffff00ff : 0xffff0000);
                    //}
                }else{
                    S32 j;
                    U32 color = (columnArea->isEdge ? 0x000000 : 0) | (columnArea->flatArea ? columnArea->flatArea->color : 0xff00ff88);
                    F32 y_min = columnArea->y_min;
                    F32 y_max = columnArea->y_max;
                    
                    if(0)if(columnArea->isEdge){
                        SEND_BEACON(x + 0.5, columnArea->y_min, z + 0.5, (columnArea->isEdge ? 0x000000 : 0) | color);
                    }

                    //if(0)if(x == -1058 && z == 256){
                    //    F32 raise = min(5, columnArea->y_min);
                    //
                    //    printf("ymaxmin: %f, %f\n", columnArea->y_max, columnArea->y_min);
                    //
                    //    raise = columnArea->y_max;
                    //
                    //    SEND_LINE(    x, y_min, z,
                    //                x + 1, y_min, z,
                    //                color);
                    //    SEND_LINE(    x, y_min, z,
                    //                x, y_max, z,
                    //                color);
                    //    SEND_LINE(    x, y_max, z,
                    //                x + 1, y_max, z,
                    //                color);
                    //
                    //    SEND_LINE(    x + 1, y_min, z,
                    //                x + 1, y_min, z + 1,
                    //                color);
                    //    SEND_LINE(    x + 1, y_min, z,
                    //                x + 1, y_max, z,
                    //                color);
                    //    SEND_LINE(    x + 1, y_max, z,
                    //                x + 1, y_max, z + 1,
                    //                color);
                    //
                    //    SEND_LINE(    x + 1, y_min, z + 1,
                    //                x, y_min, z + 1,
                    //                color);
                    //    SEND_LINE(    x + 1, y_min, z + 1,
                    //                x + 1, y_max, z + 1,
                    //                color);
                    //    SEND_LINE(    x + 1, y_max, z + 1,
                    //                x, y_max, z + 1,
                    //                color);
                    //
                    //    SEND_LINE(    x, y_min, z + 1,
                    //                x, y_min, z,
                    //                color);
                    //    SEND_LINE(    x, y_min, z + 1,
                    //                x, y_max, z + 1,
                    //                color);
                    //    SEND_LINE(    x, y_max, z + 1,
                    //                x, y_max, z,
                    //                color);
                    //}
                    
                    if(columnArea->isEdge){
                        S32 edgeCount = 0;
                        S32 sentEdgeCount = 0;
                        U32 color2 = 0xc0000000;
                        
                        switch(columnArea->slope){
                            case BGSLOPE_FLAT:
                                color2 |= 0x00ff8080;
                                break;
                            case BGSLOPE_SLIPPERY:
                                color2 |= 0x00ffff00;
                                break;
                            case BGSLOPE_STEEP:
                                color2 |= 0x0000ff00;
                                break;
                            default:
                                assert(0);
                        }
                        
                        for(j = 0; j < 8; j++){
                            if(columnArea->sides[j].area && columnArea->sides[j].area->isEdge){
                                S32 offsets[2][2] = {{-1 + 8, -2 + 8}, {1, 2}};
                                S32 offset_offsets[2][2][2] = {
                                    {{-1 + 8 + 3, -2 + 8 + 3}, {1 + 8 - 3, 2 + 8 - 3}},
                                    {{-1 + 8 + 2, -2 + 8 + 2}, {1 + 8 - 2, 2 + 8 - 2}},
                                };
                                S32 good[2] = {0, 0};
                                S32 k;
                                S32 b = j & 1;
                                
                                for(k = 0; k < 2; k++){
                                    S32 n;
                                    for(n = 0; n < 2; n++){
                                        S32 index = (j + offsets[k][n]) % 8;
                                        if(columnArea->sides[index].area){
                                            S32 index2;

                                            index2 = (j + offset_offsets[b][k][n]) % 8;
                                            
                                            if(columnArea->sides[index].area->sides[index2].area == columnArea->sides[j].area){
                                                break;
                                            }
                                        }
                                    }
                                    if(n == 2){
                                        good[k] = 1;
                                    }
                                }
                                
                                edgeCount++;

                                if(good[0] + good[1] >= 1){
                                    sentEdgeCount++;
                                
                                    printf(".");
                                    
                                    if((ptrdiff_t)columnArea < (ptrdiff_t)columnArea->sides[j].area){
                                        SEND_LINE(    x + 0.5, columnArea->y_min + 0.1, z + 0.5,
                                                    x + 0.5 + dir_offsets[j][0], columnArea->sides[j].area->y_min + 0.1, z + 0.5 + dir_offsets[j][1],
                                                    color2);//(columnArea->flatArea->color & 0xffffff));
                                    }
                                }
                            }
                        }

                        //if(!edgeCount){
                        //    printf(".");
                        //    SEND_LINE(    x + 0.5, columnArea->y_min + 0.8, z + 0.5,
                        //                x + 0.7, columnArea->y_min + 0.8, z + 0.7,
                        //                0xff000000|color2);//(columnArea->flatArea->color & 0xffffff));
                        //}
                        
                        //if(columnArea->makeBeacon){
                        //    SEND_BEACON(x + 0.5, columnArea->y_min, z + 0.5, (columnArea->isEdge ? 0x000000 : 0) | color);
                        //}

                        if(sentEdgeCount){
                            //SEND_BEACON(x + 0.5, columnArea->y_min, z + 0.5, (columnArea->isEdge ? 0x000000 : 0) | color);
                        }
                    }
                    
                    //for(j = 0; j < 8; j++){
                    //    if(columnArea->sides[j].area && (S32)columnArea->sides[j].area < (S32)columnArea){
                    //        SEND_LINE(    x, columnArea->y_min - 1.9, z,
                    //                    x + dir_offsets[j][0], columnArea->sides[j].area->y_min - 1.9, z + dir_offsets[j][1],
                    //                    0x40000000 | 0x00ffffff);//(columnArea->flatArea->color & 0xffffff));
                    //    }
                    //}
                }
                    
                //    BeaconGenerateColumnArea* area = startArea->column->chunk->columns[i].areas;
                //    
                //    if(startArea->column->chunk->columns[i].gotAreas){
                //        if(area){
                //            for(; area; area = area->nextColumnArea){
                //                //SEND_BEACON(x, area->y_min, z, flatArea->color);
                //            }
                //        }else{     D_BEACON(x, 520, z, 0xffff00ff);
                //        }
                //    }else{
                //        //SEND_BEACON(x, 520, z, 0
                //            //SENxff00ff44);
                //    }
                //}
            }
        }
    }
    
    printf("Done!\n");
}

static S32 getAdjacentInMemoryCount(BeaconDiskSwapBlock* block){
    S32 count = 0;
    S32 x = block->x / BEACON_GENERATE_CHUNK_SIZE;
    S32 z = block->z / BEACON_GENERATE_CHUNK_SIZE;
    S32 i;
    
    for(i = 0; i < 8; i++){
        block = beaconGetDiskSwapBlockByGrid(x + dir_offsets[i][0], z + dir_offsets[i][1]);
        
        if(block){// && !block->fileName){
            count++;
        }
    }

    return count;
}

static BeaconDiskSwapBlock* getBlockWithMostInMemory(S32* outCount){
    BeaconDiskSwapBlock* best = NULL;
    S32 bestCount = -1;
    BeaconDiskSwapBlock* block;
    
    for(block = bp_blocks.inMemory.head; block; block = block->inMemory.next){
        if(block->isLegal){
            S32 count = getAdjacentInMemoryCount(block);
            
            if(count > bestCount){
                best = block;
                bestCount = count;
            }
        }
    }
    
    if(outCount){
        *outCount = bestCount;
    }
    
    return best;
}

static S32 __cdecl compareAreaYMin(const void* a1pData, const void* a2pData){
    const BeaconGenerateColumnArea** a1p = (const BeaconGenerateColumnArea**)a1pData;
    const BeaconGenerateColumnArea** a2p = (const BeaconGenerateColumnArea**)a2pData;
    const BeaconGenerateColumnArea* a1 = *a1p;
    const BeaconGenerateColumnArea* a2 = *a2p;
    
    if(a1->slope < a2->slope){
        return -1;
    }
    else if(a1->slope > a2->slope){
        return 1;
    }
    else if(a1->y_min < a2->y_min){
        return -1;
    }
    else if(a1->y_min > a2->y_min){
        return 1;
    }
    else{
        return 0;
    }
}

void beaconPropagateLegalColumnAreas(BeaconDiskSwapBlock* swapBlock){
    BeaconGenerateChunk* chunk = swapBlock->chunk;
    BeaconGenerateColumnArea* removeNextPass = NULL;
    S32 i;
    
    while(1){
        BeaconGenerateLegalAreaList*    legalAreas = NULL;
        BeaconGenerateSlope                curSlope;
        BeaconGenerateColumnArea*        curArea;
        
        for(i = 0; i < ARRAY_SIZE(chunk->legalAreas); i++){
            if(chunk->legalAreas[i].areas){
                curSlope = i;
                legalAreas = chunk->legalAreas + i;
                assert(legalAreas->count);
                break;
            }else{
                assert(!chunk->legalAreas[i].count);
            }
        }
        
        if(!legalAreas){
            break;
        }
        
        if(curSlope < BGSLOPE_STEEP){
            // Do flat 
            
            static BeaconGenerateColumnArea**    sortArray;
            static S32                            sortMaxCount;

            BeaconGenerateColumnArea*            prevArea;
            S32                                    sortCount = 0;
            
            for(curArea = legalAreas->areas, prevArea = NULL;
                curArea;
                prevArea = curArea, curArea = curArea ? curArea->nextChunkLegalArea : legalAreas->areas)
            {
                assert(curArea->inChunkLegalList);

                if(curArea->flatArea || curArea == removeNextPass){
                    // Remove already-placed areas from the legal list.
                    
                    if(prevArea){
                        prevArea->nextChunkLegalArea = curArea->nextChunkLegalArea;
                    }else{
                        legalAreas->areas = curArea->nextChunkLegalArea;
                    }
                    
                    legalAreas->count--;
                    curArea->inChunkLegalList = 0;
                    curArea = prevArea;
                }else{
                    BeaconGenerateColumnArea** newSlot = dynArrayAddStruct(&sortArray, &sortCount, &sortMaxCount);
                    
                    *newSlot = curArea;
                }
            }
                    
            if(sortCount){
                if(sortCount > 1){
                    qsort(sortArray, sortCount, sizeof(sortArray[0]), compareAreaYMin);
                }
                
                curArea = sortArray[0];
                
                assert(!curArea->flatArea);
                
                propagateFlatArea(chunk, curArea);
                
                removeNextPass = curArea;
            }else{
                assert(!legalAreas->areas && !legalAreas->count);
            }
        }else{
            // Do steep and vertical slopes in whatever order.
        
            curArea = legalAreas->areas;
            legalAreas->areas = curArea->nextChunkLegalArea;
            legalAreas->count--;

            propagateFlatArea(chunk, curArea);
            
            removeNextPass = NULL;
        }
    }
}

static void beaconPropagateLegalBlocks(){
    while(bp_blocks.legal.head){
        S32 count;
        BeaconDiskSwapBlock* swapBlock = getBlockWithMostInMemory(&count);
        BeaconGenerateChunk* chunk;
        S32 i;
        S32 legalCount = 0;
        
        if(!swapBlock){
            swapBlock = bp_blocks.legal.head;
            count = 0;
        }
        
        chunk = swapBlock->chunk;
        
        for(i = 0; i < ARRAY_SIZE(chunk->legalAreas); i++){
            legalCount += chunk->legalAreas[i].count;
        }

        printf(    "\nchunk(%4d,%4d), %5d, %4d, %d adjacent, %d visits, %s: ",
                swapBlock->x / BEACON_GENERATE_CHUNK_SIZE,
                swapBlock->z / BEACON_GENERATE_CHUNK_SIZE,
                legalCount,
                bp_blocks.legal.count,
                count,
                swapBlock->visitCount,
                beaconCurTimeString(0));
                
        swapBlock->visitCount++;

        beaconPropagateLegalColumnAreas(swapBlock);
        
        printf("\n");

        // Do this down here so the propagator doesn't add myself back to the legal list.

        if(swapBlock->legalList.prev){
            swapBlock->legalList.prev->legalList.next = swapBlock->legalList.next;
        }else{
            assert(bp_blocks.legal.head == swapBlock);
            bp_blocks.legal.head = swapBlock->legalList.next;
        }
        
        if(swapBlock->legalList.next){
            swapBlock->legalList.next->legalList.prev = swapBlock->legalList.prev;
        }
        
        swapBlock->legalList.next = swapBlock->legalList.prev = NULL;
        
        swapBlock->isLegal = 0;
        bp_blocks.legal.count--;
        
        for(i = 0; i < ARRAY_SIZE(chunk->legalAreas); i++){
            assert(!chunk->legalAreas[i].count && !chunk->legalAreas[i].areas);
        }
    }
}


static S32 __cdecl compareVec3Z(const void* v1Data, const void* v2Data){
    const F32* v1 = (const F32*)v1Data;
    const F32* v2 = (const F32*)v2Data;
    if(vecZ(v1) < vecZ(v2))
        return -1;
    else if(vecZ(v1) > vecZ(v2))
        return 1;
    else
        return 0;
}

//typedef enum BoxSide {
//    SIDE_x0        = 0x01,
//    SIDE_x1        = 0x02,
//    SIDE_z0        = 0x04,
//    SIDE_z1        = 0x08,
//    ONSIDE_x0    = 0x10,
//    ONSIDE_x1    = 0x20,
//    ONSIDE_z0    = 0x40,
//    ONSIDE_z1    = 0x80,
//} BoxSide;

//static void interpPoint_x(Vec3 p1, Vec3 p2, F32 x, Vec3 pout){
//    F32 ratio = (x - p1[0]) / (p2[0] - p1[0]);
//
//    pout[0] = x;
//    pout[1] = p1[1] + ratio * (p2[1] - p1[1]);
//    pout[2] = p1[2] + ratio * (p2[2] - p1[2]);
//}

//static F32 interpY_x(Vec3 p1, Vec3 p2, F32 x){
//    return p1[1] + (x - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]);
//}

static void interpPoint_z(Vec3 p1, Vec3 p2, F32 z, Vec3 pout){
    F32 ratio = (z - p1[2]) / (p2[2] - p1[2]);
    
    assert(p2[2] != p1[2]);

    if(ratio == 0.0f){
        pout[0] = p1[0];
        pout[1] = p1[1];
    }
    else if(ratio == 1.0f){
        pout[0] = p2[0];
        pout[1] = p2[1];
    }
    else{
        Vec3 pmax = {max(p1[0], p2[0]), max(p1[1], p2[1]), 0};
        Vec3 pmin = {min(p1[0], p2[0]), min(p1[1], p2[1]), 0};
        S32 i;
        
        pout[0] = p1[0] + ratio * (p2[0] - p1[0]);
        pout[1] = p1[1] + ratio * (p2[1] - p1[1]);
        
        for(i = 0; i < 2; i++){
            if(pout[i] > pmax[i]){
                pout[i] = pmax[i];
            }
            else if(pout[i] < pmin[i]){
                pout[i] = pmin[i];
            }
        }
    }

    pout[2] = z;
}

//static F32 getMaxYInSquare(Vec3 verts[3], S32 x, S32 z){
//    U32        outside[7];
//    Vec3    points[7];
//    S32        count = 3;
//    S32        i;
//    S32        outside_count = 0;
//    F32        min_y = FLT_MAX;
//    F32        max_y = -FLT_MAX;
//    
//    for(i = 0; i < 3; i++){
//        copyVec3(verts[i], points[i]);
//        
//        outside[i] = 0;
//
//        if(points[i][0] < x){
//            outside[i] |= SIDE_x0;
//        }
//        else if(points[i][0] > x + 1){
//            outside[i] |= SIDE_x1;
//        }
//        else if(points[i][0] == x){
//            outside[i] |= ONSIDE_x0;
//        }
//        else if(points[i][0] == x + 1){
//            outside[i] |= ONSIDE_x1;
//        }
//        
//        if(points[i][2] < z){
//            outside[i] |= SIDE_z0;
//        }
//        else if(points[i][2] > z + 1){
//            outside[i] |= SIDE_z1;
//        }
//        else if(points[i][2] == z){
//            outside[i] |= ONSIDE_z0;
//        }
//        else if(points[i][2] == z + 1){
//            outside[i] |= ONSIDE_z1;
//        }
//        
//        if(outside[i] & 0xf){
//            outside_count++;
//        }
//    }
//    
//    i = 0;
//    
//    #define PUSH() {                                                                        \
//        memmove(points + i + 2, points + i + 1, sizeof(points[0]) * (count - i - 1));        \
//        memmove(outside + i + 2, outside + i + 1, sizeof(outside[0]) * (count - i - 1));    \
//        copyVec3(temp2, points[i+1]);                                                        \
//        count++;                                                                            \
//        assert(count <= 7);                                                                    \
//    }
//    
//    #define UPDATE_FOR_DEST(in, out, pos){        \
//        if(in){                                    \
//            if(in & SIDE_x0){                    \
//                if(pos[0] < x){                    \
//                    out |= SIDE_x0;                \
//                }                                \
//                else if(pos[0] == x){            \
//                    out |= ONSIDE_x0;            \
//                }                                \
//            }                                    \
//            else if(in & SIDE_x1){                \
//                if(pos[0] > x + 1){                \
//                    out |= SIDE_x1;                \
//                }                                \
//                else if(pos[0] == x + 1){        \
//                    out |= ONSIDE_x1;            \
//                }                                \
//            }                                    \
//            if(in & SIDE_z0){                    \
//                if(pos[2] < z){                    \
//                    out |= SIDE_z0;                \
//                }                                \
//                else if(pos[2] == z){            \
//                    out |= ONSIDE_z0;            \
//                }                                \
//            }                                    \
//            else if(in & SIDE_z1){                \
//                if(pos[2] > z + 1){                \
//                    out |= SIDE_z1;                \
//                }                                \
//                else if(pos[2] == z + 1){        \
//                    out |= ONSIDE_z1;            \
//                }                                \
//            }                                    \
//        }                                        \
//    }
//    
//    #define UPDATE_OTHER(out, pos)                \
//        if(out & (SIDE_z0|SIDE_z1)){            \
//            if(out & SIDE_z0){                    \
//                if(pos[2] >= z){                \
//                    out &= ~SIDE_z0;            \
//                                                \
//                    if(pos[2] == z){            \
//                        out |= ONSIDE_z0;        \
//                    }                            \
//                }                                \
//            }                                    \
//            else if(out & SIDE_z1){                \
//                if(pos[2] <= z + 1){            \
//                    out &= ~SIDE_z1;            \
//                                                \
//                    if(pos[2] == z){            \
//                        out |= ONSIDE_z1;        \
//                    }                            \
//                }                                \
//            }                                    \
//        }
//
//    #define CHECK(coord, side, update_other){                                            \
//        if(outside[i] & SIDE_##coord##side){                                            \
//            outside[i] &= ~SIDE_##coord##side;                                            \
//            outside[i] |= ONSIDE_##coord##side;                                            \
//            if(!(outside[prev] & (SIDE_##coord##side|ONSIDE_##coord##side))){            \
//                interpPoint_##coord(points[i], points[prev], coord + side, temp);        \
//                UPDATE_FOR_DEST(outside[prev], outside[i], temp);                        \
//                if(update_other){                                                        \
//                    UPDATE_OTHER(outside[i], temp);                                        \
//                }                                                                        \
//                if(!(outside[next] & (SIDE_##coord##side|ONSIDE_##coord##side))){        \
//                    interpPoint_##coord(points[i], points[next], coord + side, temp2);    \
//                    PUSH();                                                                \
//                    if(next > i){                                                        \
//                        next++;                                                            \
//                    }                                                                    \
//                    outside[i+1] = outside[i];                                            \
//                    UPDATE_FOR_DEST(outside[next], outside[i+1], temp2);                \
//                    if(update_other){                                                    \
//                        UPDATE_OTHER(outside[i+1], temp2);                                \
//                    }                                                                    \
//                    if(next > i){                                                        \
//                        next--;                                                            \
//                    }                                                                    \
//                    if(outside[i+1] & 0xf){                                                \
//                        outside_count++;                                                \
//                    }                                                                    \
//                }                                                                        \
//                copyVec3(temp, points[i]);                                                \
//            }                                                                            \
//            else if(!(outside[next] & (SIDE_##coord##side|ONSIDE_##coord##side))){        \
//                interpPoint_##coord(points[i], points[next], coord + side, points[i]);    \
//                UPDATE_FOR_DEST(outside[next], outside[i], points[i]);                    \
//                if(update_other){                                                        \
//                    UPDATE_OTHER(outside[i], points[i]);                                \
//                }                                                                        \
//            }                                                                            \
//        }                                                                                \
//    }
//
//    while(outside_count){
//        if(outside[i] & 0xf){
//            Vec3 temp, temp2;
//            S32 prev = (i ? i : count) - 1;
//            S32 next = i == count - 1 ? 0 : i + 1;
//            U32 old_outside = outside[i] & 0xf;
//            
//            CHECK(x, 0, 1);
//            CHECK(x, 1, 1);
//            CHECK(z, 0, 0);
//            CHECK(z, 1, 0);
//            
//            if(!(outside[i] & 0xf)){
//                outside_count--;
//            }
//            else if(outside[i] & old_outside){
//                break;
//            }
//        }else{
//            if(++i >= count){
//                i = 0;
//            }
//        }
//    }
//    
//    for(i = 0; i < count; i++){
//        if(points[i][1] > max_y){
//            max_y = points[i][1];
//        }
//        if(points[i][1] < min_y){
//            min_y = points[i][1];
//        }
//    }
//    
//    return max_y;
//}

static struct {
    BeaconGenerateSlope    slope;
    U32                    noGroundConnections;
} curRender;

static void addNewArea(S32 x, S32 z, F32 y_min, F32 y_max){
    BeaconGenerateColumn* column = getColumn(x, z, 1);
    BeaconGenerateChunk* chunk = column->chunk;
    BeaconGenerateColumnArea* prevArea = NULL;
    BeaconGenerateColumnArea* area;
    BeaconGenerateColumnArea* newArea = NULL;
    S32 used = 0;
    S32 columnIndex = column - chunk->columns;
    
    if(chunk->swapBlock->inverted){
        // This block has already been surfacilized.
        
        return;
    }
    
    #if BEACONGEN_CHECK_VERTS
    {
        S32 i = column->tris.count;
        dynArrayAddStruct(&column->tris.tris, &column->tris.count, &column->tris.maxCount);
        column->tris.tris[i].def = currentTriVerts.def;
        column->tris.tris[i].y_min = y_min;
        column->tris.tris[i].y_max = y_max;
        copyVec3(currentTriVerts.verts[0], column->tris.tris[i].verts[0]);
        copyVec3(currentTriVerts.verts[1], column->tris.tris[i].verts[1]);
        copyVec3(currentTriVerts.verts[2], column->tris.tris[i].verts[2]);
    }
    #endif
    
    //if(x == -4097 && z == -2253){
    //    assert(0);
    //    
    //    //if((S32)y_min == 118){
    //    //    assert(0);
    //    //}
    //    
    //    //printf("\nymaxmin: %f, %f\n", y_max, y_min);
    //}
    
    for(area = column->areas; area; area = area->nextColumnArea){
        assert(area->columnIndex == columnIndex);
        
        if(!used && y_max + 6 <= area->y_min){
            // Create a new area underneath all the current areas.
            
            newArea = createBeaconGenerateColumnArea();
            
            newArea->columnIndex = columnIndex;
            newArea->y_min = y_min;
            newArea->y_max = y_max;
            newArea->slope = curRender.slope;
            newArea->noGroundConnections = curRender.noGroundConnections;
            
            if(prevArea){
                newArea->nextColumnArea = area;
                prevArea->nextColumnArea = newArea;
            }else{
                newArea->nextColumnArea = column->areas;
                column->areas = newArea;
            }
            
            if(column->isIndexed){
                if(newArea->nextColumnArea){
                    column->isIndexed = 0;
                }else{
                    assert(prevArea && prevArea->columnAreaIndex == column->areaCount);
                    newArea->columnAreaIndex = prevArea->columnAreaIndex + 1;
                }
            }
            else if(!column->areaCount){
                column->isIndexed = 1;
            }
            
            column->areaCount++;
            
            used = 1;
            
            #if BEACONGEN_CHECK_VERTS
                newArea->triVerts = currentTriVerts;
            #endif
                                    
            break;
        }
        else if(!used && area->y_max + 6 > y_min){
            // Merge the two together.
            
            if(y_min < area->y_min){
                area->y_min = y_min;
            }
            
            if(y_max > area->y_max){
                if((U32)curRender.slope < area->slope || y_max - area->y_max > BETTER_SLOPE_Y_DIFF){
                    area->slope = curRender.slope;
                }

                area->noGroundConnections = curRender.noGroundConnections;
                area->y_max = y_max;
                
                #if BEACONGEN_CHECK_VERTS
                    area->triVerts = currentTriVerts;
                #endif
            }
            else if((U32)curRender.slope < area->slope && area->y_max - y_max < BETTER_SLOPE_Y_DIFF){
                area->slope = curRender.slope;
            }

            y_min = FLT_MAX;
            y_max = FLT_MAX;
        
            used = 1;
        }
        else if(prevArea && prevArea->y_max + 6 > area->y_min && area->y_max + 6 > prevArea->y_min){
            prevArea->nextColumnArea = area->nextColumnArea;
            
            if(area->y_max > prevArea->y_max){
                if(area->slope < prevArea->slope || area->y_max - prevArea->y_max > BETTER_SLOPE_Y_DIFF){
                    prevArea->slope = area->slope;
                }

                prevArea->noGroundConnections = area->noGroundConnections;
                prevArea->y_max = area->y_max;
                
                #if BEACONGEN_CHECK_VERTS
                    prevArea->triVerts = area->triVerts;
                #endif
            }
            else if(area->slope < prevArea->slope && prevArea->y_max - area->y_max < BETTER_SLOPE_Y_DIFF){
                prevArea->slope = area->slope;
            }
                
            if(area->y_min < prevArea->y_min){
                prevArea->y_min = area->y_min;
            }
            
            if(prevArea->nextColumnArea){
                column->isIndexed = 0;
            }
            
            assert(    !area->inCheckList &&
                    !area->inChunkLegalList &&
                    !area->inEdgeList &&
                    !area->inNonEdgeList &&
                    !area->isLegal &&
                    !area->nextChunkLegalArea &&
                    !area->nextInFlatAreaSubList &&
                    !area->flatArea);
                    
            assert(column->areaCount > 0);
            
            column->areaCount--;
            
            assert(!column->isIndexed || prevArea->nextColumnArea || prevArea->columnAreaIndex == column->areaCount - 1);
            
            destroyBeaconGenerateColumnArea(area);

            area = prevArea;

            used = 1;              
        }

        prevArea = area;
    }
    
    if(!used){
        // New area is above everything else, so make new area for it.
        
        newArea = createBeaconGenerateColumnArea();
        
        newArea->columnIndex = columnIndex;
        newArea->y_min = y_min;
        newArea->y_max = y_max;
        newArea->slope = curRender.slope;
        newArea->noGroundConnections = curRender.noGroundConnections;
        
        #if BEACONGEN_CHECK_VERTS
            newArea->triVerts = currentTriVerts;
        #endif
        
        if(prevArea){
            newArea->nextColumnArea = prevArea->nextColumnArea;
            prevArea->nextColumnArea = newArea;
        }else{
            assert(!column->areas);
            newArea->nextColumnArea = column->areas;
            column->areas = newArea;
        }
        
        if(column->isIndexed){
            if(newArea->nextColumnArea){
                column->isIndexed = 0;
            }else{
                assert(prevArea);
                newArea->columnAreaIndex = prevArea->columnAreaIndex + 1;
            }
        }
        else if(!column->areaCount){
            column->isIndexed = 1;
        }
        
        column->areaCount++;
    }
}

static S32 floatHasFraction(F32 f){
    S32 i = *(S32*)&f;
    S32 exp = ((i & 0x7f800000) >> 23) - 127;

    if(    exp < 0
        ||
        exp < 23 &&
        i & BIT_RANGE(0, 22 - exp))
    {
        return 1;
    }
    
    return 0;
}

static S32 renderXLinesToChunks(Vec3 p[4], S32 z){
    static S32 checkSets[4][2] = {{0, 2}, {1, 3}, {0, 1}, {2, 3}};
    
    S32        process[4];
    S32        process_count = 0;
    S32        lowleft[4];
    S32        x0[4];
    S32        x1[4];
    S32        x0_orig[4];
    S32        x1_orig[4];
    F32        dy_dx[4];
    Vec3    l[4];
    Vec3    r[4];
    S32        min_x = INT_MAX;
    S32        max_x = INT_MIN;
    S32        i;
    S32        x;
    
    for(i = 0; i < 4; i++){
        copyVec3((S32*)p[checkSets[i][0]], (S32*)l[i]);
        copyVec3((S32*)p[checkSets[i][1]], (S32*)r[i]);

        if(l[i][0] > r[i][0]){
            Vec3 ptemp;
            copyVec3((S32*)l[i],    (S32*)ptemp);
            copyVec3((S32*)r[i],    (S32*)l[i]);
            copyVec3((S32*)ptemp,    (S32*)r[i]);
        }
        
        x0_orig[i] = floor(l[i][0]);
        x1_orig[i] = floor(r[i][0]);
        
        //if((F32)x0[i] == l[i][0]){
        //    if(l[i][1] < r[i][1]){
        //        x0[i]--;
        //    }
        //}
        
        if(    x0_orig[i] >= max_tri_x ||
            x1_orig[i] < min_tri_x)
        {
            continue;
        }
            
        if(x0_orig[i] < min_tri_x){
            x0[i] = min_tri_x;
        }else{
            x0[i] = x0_orig[i];
        }
        
        if(x1_orig[i] >= max_tri_x){
            x1[i] = max_tri_x - 1;
        }else{
            x1[i] = x1_orig[i];
        }
        
        if(x0[i] < min_x){
            min_x = x0[i];
        }
        
        if(x1[i] > max_x){
            max_x = x1[i];
        }

        if(l[i][1] < r[i][1]){
            lowleft[i] = 1;
        }else{
            lowleft[i] = 0;
        }
        
        if(x0_orig[i] != x1_orig[i]){
            dy_dx[i] = (r[i][1] - l[i][1]) / (r[i][0] - l[i][0]);
        }
        
        process[process_count++] = i;
    }
        
    #define CHECK_Y_LO(y_eq){        \
        F32 y = y_eq;                \
        if(y < y_min){                \
            y_min = y;                \
        }                            \
    }
    
    #define CHECK_Y_HI(y_eq){        \
        F32 y = y_eq;                \
        if(y > y_max){                \
            y_max = y;                \
        }                            \
    }

    for(x = min_x; x <= max_x; x++){
        S32    first = 1;
        F32    y_min = FLT_MAX;
        F32    y_max = -FLT_MAX;
        S32 j;
        
        for(j = 0; j < process_count;){
            i = process[j];
            
            if(x >= x0[i]){
                if(lowleft[i]){
                    if(x == x0_orig[i]){
                        CHECK_Y_LO(l[i][1]);
                    }else{
                        CHECK_Y_LO(l[i][1] + dy_dx[i] * (x - l[i][0]));
                    }

                    if(x == x1_orig[i]){
                        CHECK_Y_HI(r[i][1]);
                        process[j] = process[--process_count];
                    }
                    else if(x < x1_orig[i]){
                        CHECK_Y_HI(l[i][1] + dy_dx[i] * (x + 1 - l[i][0]));
                        j++;
                    }
                }else{
                    if(x == x0_orig[i]){
                        CHECK_Y_HI(l[i][1]);
                    }else{
                        CHECK_Y_HI(l[i][1] + dy_dx[i] * (x - l[i][0]));
                    }

                    if(x == x1_orig[i]){
                        CHECK_Y_LO(r[i][1]);
                        process[j] = process[--process_count];
                    }
                    else if(x < x1_orig[i]){
                        CHECK_Y_LO(l[i][1] + dy_dx[i] * (x + 1 - l[i][0]));
                        j++;
                    }
                }
            }else{
                j++;
            }
        }
        
        // Got the min and max y, so put them into the column.
        
        addNewArea(x, z, y_min, y_max);
    }
    
    return max_x - min_x + 1;
}

static S32 renderTrapezoidToChunks(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 v3){
    F32     top_z = vecZ(v2);
    F32     end_z = top_z;
    F32     bottom_z = vecZ(v0);
    F32     lo_z = bottom_z;
    S32     pointCount = 0;
    S32     first = 1;
    Vec3    p[4];
    S32        exact = 1;
    
    if(end_z >= max_tri_z){
        end_z = max_tri_z;
        exact = 0;
    }
    
    if(lo_z < min_tri_z){
        lo_z = min_tri_z;
    }
    
    if(top_z == bottom_z){
        copyVec3(v0, p[0]);
        copyVec3(v1, p[1]);
        copyVec3(v2, p[2]);
        copyVec3(v3, p[3]);

        pointCount += renderXLinesToChunks(p, floor(lo_z));
    }else{
        bottom_z = lo_z;
    
        while(    lo_z < end_z
                ||
                exact &&
                lo_z == end_z)
        {
            F32 hi_z;
            
            hi_z = lo_z + 1;
            
            if(hi_z > top_z){
                hi_z = top_z;
            }
            else if(first){
                hi_z = floor(hi_z);
                
                if(hi_z - lo_z > 1){
                    hi_z = hi_z - 1;
                }
            }
            
            interpPoint_z(v0, v2, lo_z, p[0]);
            interpPoint_z(v1, v3, lo_z, p[1]);
            interpPoint_z(v0, v2, hi_z, p[2]);
            interpPoint_z(v1, v3, hi_z, p[3]);
            
            pointCount += renderXLinesToChunks(p, floor(lo_z));
            
            lo_z = lo_z + 1;

            if(first){
                lo_z = floor(lo_z);
                
                if(lo_z - bottom_z > 1){
                    lo_z = lo_z - 1;
                }
                
                first = 0;
            }
        }
    }
        
    return pointCount;
}

static S32 renderTriangleToChunks(Vec3 verts[3], Vec3 normal){
    F32 dz;
    S32 pointCount;
    F32 min_x = verts[0][0];
    F32 max_x = min_x;
    S32 i;
    F32 normal_y = normal[1];
    
    #if BEACONGEN_CHECK_VERTS
        {
            S32 i;
            
            for(i = 0; i < 3; i++){
                copyVec3(verts[i], currentTriVerts.verts[i]);
            }
        }
    #endif
        
    for(i = 1; i < 3; i++){
        if(verts[i][0] < min_x)
            min_x = verts[i][0];
        if(verts[i][0] > max_x)
            max_x = verts[i][0];
    }
    
    if(    min_x >= max_tri_x ||
        max_x < min_tri_x)
    {
        return 1;
    }
        
    qsort(verts, 3, sizeof(verts[0]), compareVec3Z);
    
    if(    verts[0][2] >= max_tri_z ||
        verts[2][2] < min_tri_z)
    {
        return 1;
    }

    dz = vecZ(verts[2]) - vecZ(verts[0]);
    
    if(normal_y > 0.7){
        curRender.slope = BGSLOPE_FLAT;
    }
    else if(normal_y > 0.495){
        curRender.slope = BGSLOPE_SLIPPERY;
    }
    else if(normal_y > 0.2){
        curRender.slope = BGSLOPE_STEEP;
    }
    else{
        curRender.slope = BGSLOPE_VERTICAL;
    }
    
    if(dz == 0){
        pointCount = renderTrapezoidToChunks(verts[1], verts[1], verts[0], verts[2]);
    }
    else if(vecZ(verts[0]) == vecZ(verts[1])){
        if(vecX(verts[0]) < vecX(verts[1])){
            pointCount = renderTrapezoidToChunks(verts[0], verts[1], verts[2], verts[2]);
        }else{
            pointCount = renderTrapezoidToChunks(verts[1], verts[0], verts[2], verts[2]);
        }        
    }
    else if(vecZ(verts[2]) == vecZ(verts[1])){
        if(vecX(verts[2]) < vecX(verts[1])){
            pointCount = renderTrapezoidToChunks(verts[0], verts[0], verts[2], verts[1]);
        }else{
            pointCount = renderTrapezoidToChunks(verts[0], verts[0], verts[1], verts[2]);
        }        
    }
    else{
        Vec3 mid_vert;
        
        interpPoint_z(verts[0], verts[2], vecZ(verts[1]), mid_vert);
        
        //vecZ(mid_vert) = vecZ(verts[1]);
        //vecX(mid_vert) = vecX(verts[0]) + (vecX(verts[2]) - vecX(verts[0])) * (vecZ(mid_vert) - vecZ(verts[0])) / dz;
        //vecY(mid_vert) = vecY(verts[0]) + (vecY(verts[2]) - vecY(verts[0])) * (vecZ(mid_vert) - vecZ(verts[0])) / dz;
        
        if(vecX(mid_vert) < vecX(verts[1])){
            pointCount = renderTrapezoidToChunks(verts[0], verts[0], mid_vert, verts[1]);
            pointCount += renderTrapezoidToChunks(mid_vert, verts[1], verts[2], verts[2]);
        }else{
            pointCount = renderTrapezoidToChunks(verts[0], verts[0], verts[1], mid_vert);
            pointCount += renderTrapezoidToChunks(verts[1], mid_vert, verts[2], verts[2]);
        }
    }
    
    return pointCount;
}

static S32 curGroupDef;
static U64 totalPointsFound;

static void extractTrianglesFromModel(GroupDef* def, Model* model, Mat4 parent_mat){
    S32 pointCount = 0;
    S32 i;

    // Calc the max sizes.
    
    if(!model->ctris){
        modelCreateCtris(model);
        assert(model->ctris);
    }
    
    curRender.noGroundConnections = def->no_beacon_ground_connections;

    //printf("model: %s (%d tris, ", def->name, model->tri_count);
    
    for(i = 0; i < model->tri_count; i++){
        CTri* tri;
        Vec3 verts_orig[3];
        Vec3 verts[3];
        Vec3 normal;
        
        tri = model->ctris + i;
        
        mulVecMat3(tri->norm, parent_mat, normal);
        
        expandCtriVerts(tri, verts_orig);
        
        if(tri->flags & (COLL_NOTSELECTABLE|COLL_NORMALTRI)){
            S32 j;
            
            for(j = 0; j < 3; j++){
                //S32 k;
                
                mulVecMat4(verts_orig[j], parent_mat, verts[j]);
            }
            
            //for(j = 0; j < 3; j++){
            //    if(    fabs(verts[j][0]+1056) < 1 && 
            //        fabs(verts[j][1]-564) < 1 &&
            //        fabs(verts[j][2]-256) < 1)
            //    {
            //        printf("");
            //    }
            //}

            #if BEACONGEN_CHECK_VERTS
                currentTriVerts.def = def;
            #endif

            pointCount += renderTriangleToChunks(verts, normal);
        }
    }
    
    totalPointsFound += pointCount;
}

static S32 extractTrianglesCallback(GroupDef* def, Mat4 parent_mat){
    Model* model = def->model;
    GroupEnt* ent;
    
    //if(curGroupDef && !(curGroupDef % 100)){
    //    printf(".");
    //    if(!(curGroupDef % SQR(100))){
    //        printf("]\n[");
    //    }
    //}
    
    for(ent = def->entries; ent - def->entries < def->count; ent++){
        if(ent->def){
            Mat4 child_mat;
        
            mulMat4(parent_mat, ent->mat, child_mat);
            extractTrianglesCallback(ent->def, child_mat);
        }
    }
    
    //beaconProcessSetTitle(100.0f * curGroupDef++ / beacon_process.groupDefCount, NULL);

    if(!model){
        return 1;
    }
    
    if(!*(S32*)&model->grid.size){
        return 1;
    }
    
    extractTrianglesFromModel(def, model, parent_mat);
    
    //printf("%d points)\n", pointCount);
    
    return 1;
}

void beaconExtractSurfaces(S32 grid_x, S32 grid_z, S32 dx, S32 dz, S32 justEdges){
    S32 x;
    S32 z;
    S32 i;
    S32 remaining = dx * dz;

    if(justEdges){
        assert(dx > 1 && dz > 1);
    
        min_tri_x = (grid_x + 1) * BEACON_GENERATE_CHUNK_SIZE - 1;
        max_tri_x = (grid_x + dx - 1) * BEACON_GENERATE_CHUNK_SIZE + 1;
        
        min_tri_z = (grid_z + 1) * BEACON_GENERATE_CHUNK_SIZE - 1;
        max_tri_z = (grid_z + dz - 1) * BEACON_GENERATE_CHUNK_SIZE + 1;
    }else{
        min_tri_x = grid_x * BEACON_GENERATE_CHUNK_SIZE;
        max_tri_x = min_tri_x + dx * BEACON_GENERATE_CHUNK_SIZE;
        
        min_tri_z = grid_z * BEACON_GENERATE_CHUNK_SIZE;
        max_tri_z = min_tri_z + dz * BEACON_GENERATE_CHUNK_SIZE;
    }
    
    //curGroupDef = 0;

    //groupProcessDef(&group_info, extractTrianglesCallback);
    
    printf("(");

    // Mark everything in range as unused.
    
    for(x = grid_x; x < grid_x + dx; x++){
        for(z = grid_z; z < grid_z + dz; z++){
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(x, z);
            
            if(block){
                for(i = 0; i < block->geoRefs.count; i++){
                    S32 index = block->geoRefs.refs[i];
                    BeaconDiskSwapBlockGeoRef* ref = bp_blocks.geoRefs.refs + index;
                    
                    ref->used = 0;
                }
            }
        }
    }

    // Extract surfaces in range.

    for(x = grid_x; x < grid_x + dx; x++){
        for(z = grid_z; z < grid_z + dz; z++){
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(x, z);
            
            if(block){
                printf(".");
                
                remaining--;
                    
                for(i = 0; i < block->geoRefs.count; i++){
                    S32 index = block->geoRefs.refs[i];
                    BeaconDiskSwapBlockGeoRef* ref = bp_blocks.geoRefs.refs + index;
                    
                    if(!ref->used){
                        ref->used = 1;
                        
                        if(ref->model){
                            extractTrianglesFromModel(ref->groupDef, ref->model, ref->mat);
                        }else{
                            extractTrianglesCallback(ref->groupDef, ref->mat);
                        }
                    }
                }
            }
        }
    }
    
    assert(remaining >= 0);
    
    while(remaining--){
        printf(".");
    }
    
    printf(")");
    
    beaconInvertAllChunks();
}

static void disableGroundConnections(GroupDef* def){
    S32            i;
    GroupEnt*    ent;
    
    def->no_beacon_ground_connections = 1;
    
    for(ent = def->entries, i=0; i < def->count; i++, ent++)
    {
        disableGroundConnections(ent->def);
    }
}

static void beaconGetTriangleBoundingBox(Vec3 verts[3], Vec3 min_xyz, Vec3 max_xyz){
    S32 min_xyz_int[3];
    S32 max_xyz_int[3];
    S32 x;
    S32 z;
    S32 i;
    
    setVec3same(min_xyz, FLT_MAX);
    setVec3same(max_xyz, -FLT_MAX);
    
    for(i = 0; i < 3; i++){
        S32 j;
        
        for(j = 0; j < 3; j++){
            if(verts[i][j] < min_xyz[j])
                min_xyz[j] = verts[i][j];
            if(verts[i][j] > max_xyz[j])
                max_xyz[j] = verts[i][j];
        }
    }
    
    for(i = 0; i < 3; i += 2){
        min_xyz_int[i] = floor((F32)min_xyz[i] / BEACON_GENERATE_CHUNK_SIZE);
        max_xyz_int[i] = floor((F32)max_xyz[i] / BEACON_GENERATE_CHUNK_SIZE);
    }    
    
    for(x = min_xyz_int[0]; x <= max_xyz_int[0]; x++){
        for(z = min_xyz_int[2]; z <= max_xyz_int[2]; z++){
            beaconGetDiskSwapBlock(x * BEACON_GENERATE_CHUNK_SIZE, z * BEACON_GENERATE_CHUNK_SIZE, 1);
        }
    }
}

S32 beaconGetValidStartingPointLevel(GroupDef* def){
    const char* validBeaconString;
    S32 level = 0;
    
    if(!def->properties){
        return 0;
    }
    
    validBeaconString = groupDefFindPropertyValue(def, "ValidBeacon");
    
    if(validBeaconString){
        level = atoi(validBeaconString);
        level = max(1, level);
    }
    else if(groupDefFindPropertyValue(def, "ExitPoint")){
        level = 1;
    }
    
    return level;
}

static S32 totalDefRefs;
static S32 totalModelRefs;

static void beaconPlaceRefInBlocks(GroupDef* def, Model* model, Mat4 mat, Vec3 min_xyz, Vec3 max_xyz){
    BeaconDiskSwapBlockGeoRef* ref;
    S32 x, z;
    S32 min_xyz_int[3];
    S32 max_xyz_int[3];
    S32 i;
    
    if(max_xyz[0] < min_xyz[0]){
        return;
    }
    
    for(i = 0; i < 3; i += 2){
        min_xyz_int[i] = floor((F32)floor(min_xyz[i]) / BEACON_GENERATE_CHUNK_SIZE);
        max_xyz_int[i] = floor((F32)floor(max_xyz[i]) / BEACON_GENERATE_CHUNK_SIZE);
    }    
    
    ref = dynArrayAddStruct(&bp_blocks.geoRefs.refs,
                            &bp_blocks.geoRefs.count,
                            &bp_blocks.geoRefs.maxCount);
    ref->groupDef = def;
    ref->model = model;
    copyMat4(mat, ref->mat);
    
    for(x = min_xyz_int[0]; x <= max_xyz_int[0]; x++){
        for(z = min_xyz_int[2]; z <= max_xyz_int[2]; z++){
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(x, z);
            S32* index;
            
            if(!block){
                continue;
            }
            
            index = dynArrayAddStruct(    &block->geoRefs.refs,
                                        &block->geoRefs.count,
                                        &block->geoRefs.maxCount);
            
            *index = bp_blocks.geoRefs.count - 1;

            if(model){
                totalModelRefs++;
            }else{
                totalDefRefs++;
            }
        }
    }
}

static S32 beaconGenerateMeasureGroupDef(GroupDef* def, Mat4 parent_mat_param, Vec3 out_min_xyz, Vec3 out_max_xyz){
    static struct {
        struct {
            S32        childUsed;
            Vec3    min_xyz;
            Vec3    max_xyz;
            Mat4    mat;
        }*            info;

        S32         count;
        S32         maxCount;
    } children;
    
    Model*        model = def->model;
    GroupEnt*    ent;
    S32            i;
    S32            placeChildren = 0;
    Vec3        model_min_xyz;
    Vec3        model_max_xyz;
    Mat4        parent_mat;
    
    copyMat4(parent_mat_param, parent_mat);
    
    beaconProcessSetTitle(100.0f * curGroupDef++ / beacon_process.groupDefCount, NULL);

    if(groupDefFindProperty(def, "NoGroundConnections")){
        disableGroundConnections(def);
    }
    
    // Check sizes on the children first.
    
    if(out_min_xyz){
        setVec3same(out_min_xyz, FLT_MAX);
        setVec3same(out_max_xyz, -FLT_MAX);
    }else{
        placeChildren = 1;
    }

    dynArrayAddStructs(    &children.info,
                        &children.count,
                        &children.maxCount,
                        def->count);
    
    for(ent = def->entries; ent - def->entries < def->count; ent++){
        Vec3 min_xyz;
        Vec3 max_xyz;
        S32 index = children.count - def->count + (ent - def->entries);
        S32 childUsed;
        
        mulMat4(parent_mat, ent->mat, children.info[index].mat);
        
        childUsed = beaconGenerateMeasureGroupDef(ent->def, children.info[index].mat, min_xyz, max_xyz);
        
        copyVec3(min_xyz, children.info[index].min_xyz);
        copyVec3(max_xyz, children.info[index].max_xyz);
        
        if(childUsed){
            children.info[index].childUsed = placeChildren = 1;
        }else{
            Vec3 span;
            
            children.info[index].childUsed = 0;
            
            subVec3(max_xyz, min_xyz, span);
            
            if(    span[0] >= 256 ||
                span[1] >= 256 ||
                span[2] >= 256)
            {
                placeChildren = 1;
            }
        }
        
        if(out_min_xyz){
            for(i = 0; i < 3; i++){
                out_min_xyz[i] = min(out_min_xyz[i], min_xyz[i]);
                out_max_xyz[i] = max(out_max_xyz[i], max_xyz[i]);
            }
        }
    }
    
    if(placeChildren){
        for(ent = def->entries; ent - def->entries < def->count; ent++){
            S32 index = children.count - def->count + (ent - def->entries);
            
            if(!children.info[index].childUsed){
                children.info[index].childUsed = 1;
                
                beaconPlaceRefInBlocks(ent->def, NULL, children.info[index].mat, children.info[index].min_xyz, children.info[index].max_xyz);
            }
        }
    }
    
    children.count -= def->count;
    
    assert(children.count >= 0);
    
    if(def->properties){
        // Find starting points.
        
        S32 validStartingPointLevel = beaconGetValidStartingPointLevel(def);
        S32 isValidLevel = 1;
        
        if(validStartingPointLevel > beacon_process.validStartingPointMaxLevel){
            S32 i;
            
            beacon_process.validStartingPointMaxLevel = validStartingPointLevel;
            
            //beaconPrintf(COLOR_GREEN, "New ValidBeacon version: %d\n", beacon_process.validStartingPointMaxLevel);
                        
            // Invalidate all previous beacons.
            
            for(i = 0; i < combatBeaconArray.size; i++){
                Beacon* b = combatBeaconArray.storage[i];
                
                b->isValidStartingPoint = 0;
            }
        }
        else if(validStartingPointLevel < beacon_process.validStartingPointMaxLevel){
            isValidLevel = 0;
        }
        
        if(    validStartingPointLevel ||
            groupDefFindPropertyValue(def, "EntryPoint") ||
            groupDefFindPropertyValue(def, "PersistentNPC"))
        {
            Beacon* beacon = addCombatBeacon(parent_mat[3], 0, 1, 0);
            
            if(beacon){
                if(0)if(validStartingPointLevel){
                    printf(    "Found valid starting point (lvl %d): %f, %f, %f\n",
                            validStartingPointLevel,
                            posParamsXYZ(beacon));
                }
                
                beacon->isValidStartingPoint = isValidLevel ? 1 : 0;
            }
        }
    }

    if(!model || model->grid.size == 0){
        return placeChildren;
    }
    
    // Calc the max sizes.
    
    setVec3same(model_min_xyz, FLT_MAX);
    setVec3same(model_max_xyz, -FLT_MAX);

    if(!model->ctris){
        modelCreateCtris(model);
        assert(model->ctris);
    }

    for(i = 0; i < model->tri_count; i++){
        CTri* tri;
        Vec3 tri_verts[3];
        Vec3 world_verts[3];
        Vec3 tri_min_xyz;
        Vec3 tri_max_xyz;
        Vec3 normal;
        S32 j;
        
        tri = model->ctris + i;
        
        mulVecMat3(tri->norm, parent_mat, normal);
        
        expandCtriVerts(tri, tri_verts);
        
        for(j = 0; j < 3; j++){
            S32 k;
            
            mulVecMat4(tri_verts[j], parent_mat, world_verts[j]);
            
            for(k = 0; k < 3; k++){
                beacon_process.world_min_xyz[k] = min(beacon_process.world_min_xyz[k], world_verts[j][k]);
                beacon_process.world_max_xyz[k] = max(beacon_process.world_max_xyz[k], world_verts[j][k]);
            }
        }
        
        beaconGetTriangleBoundingBox(world_verts, tri_min_xyz, tri_max_xyz);
        
        for(j = 0; j < 3; j++){
            model_min_xyz[j] = min(model_min_xyz[j], tri_min_xyz[j]);
            model_max_xyz[j] = max(model_max_xyz[j], tri_max_xyz[j]);
        }
    }
    
    if(out_min_xyz){
        for(i = 0; i < 3; i++){
            out_min_xyz[i] = min(out_min_xyz[i], model_min_xyz[i]);
            out_max_xyz[i] = max(out_max_xyz[i], model_max_xyz[i]);
        }
    }

    if(placeChildren){
        beaconPlaceRefInBlocks(def, model, parent_mat, model_min_xyz, model_max_xyz);
    }
    
    return placeChildren;
}

void beaconGenerateMeasureWorld(S32 quiet){    
    S32 i;
    S32 validCount = 0;

    setVec3same(beacon_process.world_min_xyz, FLT_MAX);
    setVec3same(beacon_process.world_max_xyz, -FLT_MAX);
    
    setVec3same(bp_blocks.grid_min_xyz, INT_MAX);
    setVec3same(bp_blocks.grid_max_xyz, INT_MIN);

    assert(!combatBeaconArray.size);
    
    if(!quiet){
        printf("Measuring world: ");
    }
    
    beaconProcessSetTitle(0, "Measure");
        
    curGroupDef = 0;
    totalDefRefs = 0;
    totalModelRefs = 0;
    
    bp_blocks.geoRefs.count = 0;
    beacon_process.validStartingPointMaxLevel = 0;
    
    for(i = 0; i < group_info.ref_count; i++){
        DefTracker* ref = group_info.refs[i];

        if(ref->def){
            beaconGenerateMeasureGroupDef(ref->def, ref->mat, NULL, NULL);
        }
    }
    
    beaconProcessSetTitle(100, NULL);
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        if(b->isValidStartingPoint){
            validCount++;
        }
    }
    
    if(!quiet){
        printf("Done! (Def refs: %d, Model refs: %d, Valid Starting Points: %d)\n", totalDefRefs, totalModelRefs, validCount);
    }
}

void beaconResetGenerator(){
    beaconDestroyDiskSwapInfo(1);

    ZeroStruct(&bp_blocks.legal);

    legals.count = 0;
    
    bp_blocks.count = 0;

    assert(!bp_blocks.inMemory.count);
    assert(!bp_blocks.inMemory.head);
    assert(!bp_blocks.inMemory.tail);
}

static void beaconAddLegalPositions(){
    S32 i;
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        
        printf("Adding Legal Pos: %1.1f, %1.1f, %1.1f\n", posParamsXYZ(b));
        
        addGeneratePosition(posPoint(b), 1);
    }
}

static void beaconCheckAndFreeMemPools(){
    assert(!mpGetAllocatedCount(MP_NAME(BeaconGenerateChunk)));
    assert(!mpGetAllocatedCount(MP_NAME(BeaconGenerateColumnArea)));
    assert(!mpGetAllocatedCount(MP_NAME(BeaconGenerateFlatArea)));
    assert(!mpGetAllocatedCount(MP_NAME(BeaconGenerateBeaconingInfo)));
    MP_DESTROY(BeaconGenerateChunk);
    MP_DESTROY(BeaconGenerateColumnArea);
    MP_DESTROY(BeaconGenerateFlatArea);
    MP_DESTROY(BeaconGenerateBeaconingInfo);
}

void beaconGenerateCombatBeacons(){
    // Check for debug packet creation.

    if(beaconGetDebugVar("sendlines")){
        pak = pktCreate();
        pktSendBitsPack(pak,1,SERVER_DEBUGCMD);
        pktSendString(pak, "ShowBeaconDebugInfo");
    }else{
        pak = NULL;
    }

    // Initialize generator.
    
    beaconResetGenerator();

    // Measure everything.

    beaconGenerateMeasureWorld(0);

    // Get surfaces for all sections of the world.
    
    //beaconCreateSurfaceSets();
    
    // Process the surface sets.
    
    //beaconProcessSurfaceSets();

    // Add the legal positions.
    
    beaconAddLegalPositions();

    // Propagate the legal blocks.
    
    beaconPropagateLegalBlocks();

    // Check and free the memory pools.
    
    beaconCheckAndFreeMemPools();

    // Create beacons from the 
    
    makeBeaconsInChunks();
    
    // Send debug lines.
    
    if(beaconGetDebugVar("sendlines")){
        sendLinesToClient();

        pktSend(&pak, server_state.curClient->link);

        pak = NULL;
    }
    
    // Destroy all the disk swap blocks.
    
    beaconDestroyDiskSwapInfo(1);
    
    // Check and free the memory pools.
    
    beaconCheckAndFreeMemPools();
}

