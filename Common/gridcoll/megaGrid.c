
#include <stdio.h>

#include "megaGrid.h"
#include "megaGridPrivate.h"

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/timing.h>

#include "entity/entity.h"
#include "entity/entserver.h"

MegaGrid megaGrid[4];

void mgInitGrid(MegaGrid* grid, int maxSize){
    int realMax = 1;
    
    assert(vec3IsZero(grid->rootCell.mid));

    while(maxSize){
        maxSize >>= 1;
        realMax <<= 1;
    }
    
    grid->rootCell.mid[0] = 0;
    grid->rootCell.mid[1] = 0;
    grid->rootCell.mid[2] = 0;
    
    grid->rootCell.cellSize = realMax * 2;
}

MP_DEFINE(MegaGridCellEntryArray);

static MegaGridCellEntryArray* __fastcall createMegaGridCellEntryArray(){
    MegaGridCellEntryArray* array;
    
    if(!MP_NAME(MegaGridCellEntryArray)){
        MP_CREATE(MegaGridCellEntryArray, 1000);
    
        mpSetMode(MP_NAME(MegaGridCellEntryArray), ZeroMemoryBit);
    }
    
    array = MP_ALLOC(MegaGridCellEntryArray);
    
    // Not necessary because of ZeroMemoryBit above
    //array->count = 0;
    //array->next = NULL;

    return array;
}

MP_DEFINE(MegaGridCell);

static MegaGridCell* __fastcall createMegaGridCell(int cellSize, int midx, int midy, int midz){
    MegaGridCell* cell;
    
    if(!MP_NAME(MegaGridCell)){
        MP_CREATE(MegaGridCell, 1000);

        mpSetMode(MP_NAME(MegaGridCell), TurnOffAllFeatures);
    }
    
    cell = MP_ALLOC(MegaGridCell);
    
    cell->mid[0] = midx;
    cell->mid[1] = midy;
    cell->mid[2] = midz;
    
    cell->cellSize = cellSize;
    cell->entries = NULL;
    
    memset(&cell->children, 0, sizeof(cell->children));

    return cell;
}

MP_DEFINE(MegaGridNode);

MegaGridNode* createMegaGridNode(void* owner, F32 radiusF32, int use_small_cells){
    MegaGridNode* node;
    
    MP_CREATE(MegaGridNode, 10);
    
    node = MP_ALLOC(MegaGridNode);
    
    node->owner = owner;

    mgUpdateNodeRadius(node, radiusF32, use_small_cells);
    
    return node;
}

void destroyMegaGridNode(MegaGridNode* node){
    if(node){
        void mgRemove(MegaGridNode* node);

        mgRemove(node);
        
        MP_FREE(MegaGridNode, node);
    }
}

void mgUpdateNodeRadius(MegaGridNode* node, F32 radiusF32, int use_small_cells){
    if(*(int*)&node->radius != *(int*)&radiusF32){
        int bits = 0;
        int radius = radiusF32;
    
        node->radius = radius;

        while(radius > 1){
            bits++;
            radius >>= 1;
        }
        
        if(node->radius > (1 << bits)){
            bits++;
        }
        
        radius = (1 << bits) - (1 << (bits - 2));

        if(use_small_cells)
        {
            if(node->radius < radius)
            {
                bits--;
                node->radius_lo = (1 << bits) + (1 << (bits - 1));
             }
             else
             {
                node->radius_lo = (1 << bits);
            }
        }
        else
        {
            node->radius_lo = (1 << bits);
            
            use_small_cells = 0;
        }

        node->radius_hi_bits = bits;
        
        node->use_small_cells = use_small_cells;
    }
}

static struct {
    int                    smallCellSize;
    int                    largeCellSize;
    MegaGridNode*        node;
    void*                owner;
    MegaGridNodeEntry*    entries;
    int                    entries_count;
    int                    lo[3];
    int                    hi[3];
} addNode;

static void __fastcall mgAddToCellHelper(MegaGridCell* cell){
    MegaGridNodeEntry* entry = addNode.entries + addNode.entries_count;
    MegaGridCellEntryArray* head = cell->entries;
    int count;
    
    addNode.entries_count++;
    
    assert(addNode.entries_count <= ARRAY_SIZE(addNode.node->entries));

    if(!head){
        head = cell->entries = createMegaGridCellEntryArray();
        head->next = NULL;
    }
    else if(head->count == ARRAY_SIZE(head->slot)){
        head = createMegaGridCellEntryArray();

        head->next = cell->entries;
        cell->entries = head;
    }

    // Add to cell's list.
    
    count = head->count++;
    
    head->slot[count].entry = entry;
    head->slot[count].owner = addNode.owner;
    
    #if MEGA_GRID_DEBUG
        head->slot[count].node = addNode.node;
    #endif

    entry->cell = cell;
    entry->slot = head->slot + count;
    //entry->array = head;
    //entry->arrayIndex = count;
}

static void __fastcall mgAddToCell(MegaGridCell* cell, int x0, int x1, int y0, int y1, int z0, int z1){
    int cellSize = cell->cellSize;
    
    if(cellSize == addNode.smallCellSize){
        mgAddToCellHelper(cell);
        return;
    }
    else if(cellSize == addNode.largeCellSize){
        int xs = !x0 && x1;
        int ys = (!y0 && y1) ? 2 : 0;
        int zs = (!z0 && z1) ? 4 : 0;
        int halfIndex = xs | ys | zs;
        MegaGridCell* newCell;
        
        switch(halfIndex){
            case 3:
                newCell = cell->children.xy[z1];
                if(!newCell){
                    newCell = cell->children.xy[z1] = createMegaGridCell(0,0,0,0);
                }
                mgAddToCellHelper(newCell);
                return;
            case 5:
                newCell = cell->children.xz[y1];
                if(!newCell){
                    newCell = cell->children.xz[y1] = createMegaGridCell(0,0,0,0);
                }
                mgAddToCellHelper(newCell);
                return;
            case 6:
                newCell = cell->children.yz[x1];
                if(!newCell){
                    newCell = cell->children.yz[x1] = createMegaGridCell(0,0,0,0);
                }
                mgAddToCellHelper(newCell);
                return;
            case 7:
                mgAddToCellHelper(cell);
                return;
        }
    }
        
    {
        MegaGridCell* (*children)[2][2] = cell->children.xyz;
        MegaGridCell* child;
        int mid[3];
        
        copyVec3(cell->mid, mid);

        // Go into the child cells.
        
        if(!x0){
            if(!y0){
                if(!z0){
                    child = children[0][0][0];
                    
                    if(!child){
                        child = children[0][0][0] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] - cellSize / 4,
                                                                        mid[1] - cellSize / 4,
                                                                        mid[2] - cellSize / 4);
                    }

                    mgAddToCell(child,
                                addNode.lo[0] >= child->mid[0], x1 ? 1 : addNode.hi[0] >= child->mid[0],
                                addNode.lo[1] >= child->mid[1], y1 ? 1 : addNode.hi[1] >= child->mid[1],
                                addNode.lo[2] >= child->mid[2], z1 ? 1 : addNode.hi[2] >= child->mid[2]);
                }

                if(z1){
                    child = children[0][0][1];

                    if(!child){
                        child = children[0][0][1] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] - cellSize / 4,
                                                                        mid[1] - cellSize / 4,
                                                                        mid[2] + cellSize / 4);
                    }

                    mgAddToCell(child,
                                addNode.lo[0] >= child->mid[0], x1 ? 1 : addNode.hi[0] >= child->mid[0],
                                addNode.lo[1] >= child->mid[1], y1 ? 1 : addNode.hi[1] >= child->mid[1],
                                z0 ? addNode.lo[2] >= child->mid[2] : 0, addNode.hi[2] >= child->mid[2]);
                }
            }
            
            if(y1){
                if(!z0){
                    child = children[0][1][0];

                    if(!child){
                        child = children[0][1][0] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] - cellSize / 4,
                                                                        mid[1] + cellSize / 4,
                                                                        mid[2] - cellSize / 4);
                    }

                    mgAddToCell(child,
                                addNode.lo[0] >= child->mid[0], x1 ? 1 : addNode.hi[0] >= child->mid[0],
                                y0 ? addNode.lo[1] >= child->mid[1] : 0, addNode.hi[1] >= child->mid[1],
                                addNode.lo[2] >= child->mid[2], z1 ? 1 : addNode.hi[2] >= child->mid[2]);
                }

                if(z1){
                    child = children[0][1][1];

                    if(!child){
                        child = children[0][1][1] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] - cellSize / 4,
                                                                        mid[1] + cellSize / 4,
                                                                        mid[2] + cellSize / 4);
                    }

                    mgAddToCell(child,
                                addNode.lo[0] >= child->mid[0], x1 ? 1 : addNode.hi[0] >= child->mid[0],
                                y0 ? addNode.lo[1] >= child->mid[1] : 0, addNode.hi[1] >= child->mid[1],
                                z0 ? addNode.lo[2] >= child->mid[2] : 0, addNode.hi[2] >= child->mid[2]);
                }
            }
        }
        
        if(x1){
            if(!y0){
                if(!z0){
                    child = children[1][0][0];

                    if(!child){
                        child = children[1][0][0] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] + cellSize / 4,
                                                                        mid[1] - cellSize / 4,
                                                                        mid[2] - cellSize / 4);
                    }

                    mgAddToCell(child,
                                x0 ? addNode.lo[0] >= child->mid[0] : 0, addNode.hi[0] >= child->mid[0],
                                addNode.lo[1] >= child->mid[1], y1 ? 1 : addNode.hi[1] >= child->mid[1],
                                addNode.lo[2] >= child->mid[2], z1 ? 1 : addNode.hi[2] >= child->mid[2]);
                }

                if(z1){
                    child = children[1][0][1];

                    if(!child){
                        child = children[1][0][1] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] + cellSize / 4,
                                                                        mid[1] - cellSize / 4,
                                                                        mid[2] + cellSize / 4);
                    }

                    mgAddToCell(child,
                                x0 ? addNode.lo[0] >= child->mid[0] : 0, addNode.hi[0] >= child->mid[0],
                                addNode.lo[1] >= child->mid[1], y1 ? 1 : addNode.hi[1] >= child->mid[1],
                                z0 ? addNode.lo[2] >= child->mid[2] : 0, addNode.hi[2] >= child->mid[2]);
                }
            }
            
            if(y1){
                if(!z0){
                    child = children[1][1][0];

                    if(!child){
                        child = children[1][1][0] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] + cellSize / 4,
                                                                        mid[1] + cellSize / 4,
                                                                        mid[2] - cellSize / 4);
                    }

                    mgAddToCell(child,
                                x0 ? addNode.lo[0] >= child->mid[0] : 0, addNode.hi[0] >= child->mid[0],
                                y0 ? addNode.lo[1] >= child->mid[1] : 0, addNode.hi[1] >= child->mid[1],
                                addNode.lo[2] >= child->mid[2], z1 ? 1 : addNode.hi[2] >= child->mid[2]);
                }

                if(z1){
                    child = children[1][1][1];

                    if(!child){
                        child = children[1][1][1] = createMegaGridCell(    cellSize / 2,
                                                                        mid[0] + cellSize / 4,
                                                                        mid[1] + cellSize / 4,
                                                                        mid[2] + cellSize / 4);
                    }

                    mgAddToCell(child,
                                x0 ? addNode.lo[0] >= child->mid[0] : 0, addNode.hi[0] >= child->mid[0],
                                y0 ? addNode.lo[1] >= child->mid[1] : 0, addNode.hi[1] >= child->mid[1],
                                z0 ? addNode.lo[2] >= child->mid[2] : 0, addNode.hi[2] >= child->mid[2]);
                }
            }
        }
    }
}

static void mgAdd(    MegaGrid* grid,
                    int pos[3],
                    MegaGridNode* node,
                    int radius,
                    int radius_hi_bits,
                    int use_small_cells)
{
    static int max_entries_count = 0;
    int mid[3];
    int x0, x1;
    int y0, y1;
    int z0, z1;

    addNode.largeCellSize = 1 << radius_hi_bits;
    addNode.smallCellSize = 1 << (radius_hi_bits - use_small_cells);
    
    addNode.node = node;
    addNode.owner = node->owner;
    addNode.entries = node->entries;
    addNode.entries_count = 0;

    addNode.lo[0] = pos[0] - radius;
    addNode.lo[1] = pos[1] - radius;
    addNode.lo[2] = pos[2] - radius;
    
    addNode.hi[0] = pos[0] + radius;
    addNode.hi[1] = pos[1] + radius;
    addNode.hi[2] = pos[2] + radius;

    copyVec3(grid->rootCell.mid, mid);

    if(addNode.lo[0] < mid[0]){
        x0 = 0;
        x1 = addNode.hi[0] > mid[0];
    }else{
        x0 = 1;
        x1 = 1;
    }
    
    if(addNode.lo[1] < mid[1]){
        y0 = 0;
        y1 = addNode.hi[1] > mid[1];
    }else{
        y0 = 1;
        y1 = 1;
    }

    if(addNode.lo[2] < mid[2]){
        z0 = 0;
        z1 = addNode.hi[2] > mid[2];
    }else{
        z0 = 1;
        z1 = 1;
    }

    mgAddToCell(&grid->rootCell, x0, x1, y0, y1, z0, z1);
    
    node->entries_count = addNode.entries_count;
    node->grid = grid;
    grid->nodeCount++;
    grid->nodeEntryCount += addNode.entries_count;
    
    //if(addNode.entries_count > max_entries_count){
    //    max_entries_count = addNode.entries_count;
    //    
    //    printf("new max entries: %d\n", max_entries_count);
    //}
}

void mgRemove(MegaGridNode* node)
{
    int count = node->entries_count;
    MegaGridNodeEntry* curEntry;
    
    if(!count)
        return;

    assert(node->grid->nodeEntryCount >= count);
    assert(node->grid->nodeCount > 0);
    
    node->grid->nodeCount--;
    node->grid->nodeEntryCount -= count;
    
    assert(node->grid->nodeCount || !node->grid->nodeEntryCount);

    for(curEntry = node->entries; count; curEntry++, count--)
    {
        MegaGridNodeEntry nodeEntry = *curEntry;
        MegaGridCellEntryArray* cellHeadArray = nodeEntry.cell->entries;
        int lastIndex = cellHeadArray->count - 1;

        if(lastIndex < 0 || lastIndex >= ARRAY_SIZE(cellHeadArray->slot))
        {
            mgVerifyAllGrids(node);
            assertmsg(0, "Bad grid entry count.");
        }

        // Copy the last slot of the head entry array into the newly vacant slot.
        *nodeEntry.slot = cellHeadArray->slot[lastIndex];

        // The node entry that was moved into this slot is now made to point to its new slot.
        nodeEntry.slot->entry->slot = nodeEntry.slot;

        if(nodeEntry.slot->entry->cell != nodeEntry.cell)
        {
            mgVerifyAllGrids(node);
            assertmsg(0, "Old cell doesn't match new cell.");
        }

        if(lastIndex == 0)
        {
            nodeEntry.cell->entries = cellHeadArray->next;
            MP_FREE(MegaGridCellEntryArray, cellHeadArray);
        }
        else
        {
            cellHeadArray->count--;
        }
    }
    
    node->entries_count = 0;
}

void mgUpdate(int grid, MegaGridNode* node, const Vec3 posF32){
    static int init = 0;
    int pos[3];
    int grid_pos[3];
    int radius_bits;
    int use_small_cells;
    
    if(node->entries_count && sameVec3((int*)node->pos, (int*)posF32)){
        return;
    }
    
    copyVec3(posF32, node->pos);

    use_small_cells = node->use_small_cells;
    
    radius_bits = node->radius_hi_bits - use_small_cells;

    qtruncVec3(posF32, pos);
    
    grid_pos[0] = pos[0] >> radius_bits;
    grid_pos[1] = pos[1] >> radius_bits;
    grid_pos[2] = pos[2] >> radius_bits;
    
    if(node->entries_count && sameVec3(grid_pos, node->grid_pos)){
        return;
    }
    
    radius_bits += use_small_cells;
    
    copyVec3(grid_pos, node->grid_pos);

    if(!init){
        init = 1;
        mgInitGrid(&megaGrid[0], 32 * 1024 - 1);
        mgInitGrid(&megaGrid[1], 32 * 1024 - 1);
        mgInitGrid(&megaGrid[2], 32 * 1024 - 1);
        mgInitGrid(&megaGrid[3], 32 * 1024 - 1);
    }
    
    PERFINFO_AUTO_START("mgRemove", 1);
        mgRemove(node);
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("mgAdd", 1);
        mgAdd(&megaGrid[grid], pos, node, node->radius_lo, radius_bits, use_small_cells);
    PERFINFO_AUTO_STOP();
}

int mgGetNodesInRange(int grid, const Vec3 posF32, void** nodeArray, int maxCount){
    MegaGridCell* cell = &megaGrid[grid].rootCell;
    int pos[3];
    int count = 0;
    void** arrayCur = nodeArray;

    PERFINFO_AUTO_START("mgGetNodesInRange", 1);
    
    pos[0] = posF32[0];
    pos[1] = posF32[1];
    pos[2] = posF32[2];

    while(cell){
        int x, y, z;
        MegaGridCellEntryArray* entry = cell->entries;
        MegaGridCell* halfCell;
        int count;
        MegaGridCellEntryArraySlot* slot;
        
        while(entry){
            count = entry->count;
            slot = entry->slot;

            while(count){
                count--;
                *arrayCur++ = slot->owner;
                slot++;
            }
            
            entry = entry->next;
        }
        
        x = pos[0] < cell->mid[0] ? 0 : 1;     
        y = pos[1] < cell->mid[1] ? 0 : 1;
        z = pos[2] < cell->mid[2] ? 0 : 1;
        
        // Check the half-cells.
        
        halfCell = cell->children.xy[z];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *arrayCur++ = slot->owner;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        halfCell = cell->children.yz[x];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *arrayCur++ = slot->owner;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        halfCell = cell->children.xz[y];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *arrayCur++ = slot->owner;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        cell = cell->children.xyz[x][y][z];
    }
    PERFINFO_AUTO_STOP();
    return arrayCur - nodeArray;
}

int mgGetNodesInRangeWithSize(int grid, Vec3 posF32, void** nodeArray, int size){
    MegaGridCell* cell = &megaGrid[grid].rootCell;
    int pos[3];
    int count = 0;
    char* arrayCur = (char*)nodeArray;
    
    pos[0] = posF32[0];
    pos[1] = posF32[1];
    pos[2] = posF32[2];

    while(cell){
        int x, y, z;
        MegaGridCellEntryArray* entry = cell->entries;
        MegaGridCell* halfCell;
        int count;
        MegaGridCellEntryArraySlot* slot;
        
        while(entry){
            count = entry->count;
            slot = entry->slot;

            while(count){
                count--;
                *(void**)arrayCur = slot->owner;
                arrayCur += size;
                slot++;
            }
            
            entry = entry->next;
        }
        
        x = pos[0] < cell->mid[0] ? 0 : 1;     
        y = pos[1] < cell->mid[1] ? 0 : 1;
        z = pos[2] < cell->mid[2] ? 0 : 1;
        
        // Check the half-cells.
        
        halfCell = cell->children.xy[z];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *(void**)arrayCur = slot->owner;
                    arrayCur += size;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        halfCell = cell->children.yz[x];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *(void**)arrayCur = slot->owner;
                    arrayCur += size;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        halfCell = cell->children.xz[y];
        
        if(halfCell){
            entry = halfCell->entries;
            
            while(entry){
                count = entry->count;
                slot = entry->slot;

                while(count){
                    count--;
                    *(void**)arrayCur = slot->owner;
                    arrayCur += size;
                    slot++;
                }
                
                entry = entry->next;
            }
        }

        cell = cell->children.xyz[x][y][z];
    }
    
    return (arrayCur - (char*)nodeArray) / size;
}

