/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>
#include "dbcomm/dbcontainer.h"
#include "dbcomm/logcomm.h"

#include <utilitieslib/stdtypes.h>
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StashTable.h>
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "containerInventory.h"
#include "entity/salvage.h"
#include "entity/Concept.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/components/StashTable.h>

#define MAX_INVITEMS_PER_ROW (900) // Upper limit is 1012 and subtract some room for ContainerId, SubId, ShardId, and ShardName columns
#define CONCEPT_COLNAME_DEFIDANDAMT "ConceptDefIdAndAmount"
#define CONCEPT_COLNAME_VARSTEM "Var" 

// 5 fields: id, vars 0-3


#define CONCEPT_DEFIDAMTCOL_DEFID_MASK 0xffff
#define CONCEPT_DEFIDAMTCOL_DEFID_OFFSET 0
#define CONCEPT_DEFIDAMTCOL_AMT_MASK 0xffff
#define CONCEPT_DEFIDAMTCOL_AMT_OFFSET 16

static void s_UnpackDefIdAndAmountCol(int *resDefId, int *resAmt, int val)
{
    if( verify( resDefId && resAmt ))
    {
        *resDefId = (val >> CONCEPT_DEFIDAMTCOL_DEFID_OFFSET) & CONCEPT_DEFIDAMTCOL_DEFID_MASK;
        *resAmt = (val >> CONCEPT_DEFIDAMTCOL_AMT_OFFSET) & CONCEPT_DEFIDAMTCOL_AMT_MASK;
    }
}

static int s_PackDefIdAndAmountCol(int defId, int amt)
{
    int res = 0;
    res = (defId << CONCEPT_DEFIDAMTCOL_DEFID_OFFSET) & (CONCEPT_DEFIDAMTCOL_DEFID_MASK<<CONCEPT_DEFIDAMTCOL_DEFID_OFFSET);
    res |= (amt << CONCEPT_DEFIDAMTCOL_AMT_OFFSET) & (CONCEPT_DEFIDAMTCOL_AMT_MASK<<CONCEPT_DEFIDAMTCOL_AMT_OFFSET);
    return res;
}

static int s_GetConceptInvIdx(int row, int col)
{
    return MIN( row, CONCEPT_MAX_ROWS )*CONCEPT_MAX_PER_ROW + MIN(col,CONCEPT_MAX_PER_ROW);
}

static void s_GetConceptRowCol(int invIdx, int *row, int *col)
{
    if( row )
    {
        *row = invIdx/CONCEPT_MAX_PER_ROW;
        assert(INRANGE0(*row, CONCEPT_MAX_ROWS));
    }
    if( col )
    {
        *col = invIdx%(CONCEPT_MAX_PER_ROW);
    }
}

static void s_ConceptTemplate(StuffBuff *psb)
{
    int i;
    char const *tablename = inventory_GetDbTableName( kInventoryType_Concept );

    for( i = 0; i < CONCEPT_MAX_PER_ROW; ++i ) 
    {
        int row = CONCEPT_MAX_ROWS;
        int col = i;
        int j;

        addStringToStuffBuff( psb, "%s[%d].c%d%s 2147483647\n" ,tablename,row,col,CONCEPT_COLNAME_DEFIDANDAMT);
        
        for( j = 0; j < TYPE_ARRAY_SIZE( ConceptItem, afVars ); ++j ) 
        {
            addStringToStuffBuff( psb, "%s[%d].c%d%s%d 1.000000\n" ,tablename,row,col,CONCEPT_COLNAME_VARSTEM,j);
        }
    }
}

#define LAST_BASERECIPE_ID 450
#define RECIPESPLIT_NUM_COLS (MAX_INVITEMS_PER_ROW/2)

//------------------------------------------------------------
//  create the template for the inventory
//----------------------------------------------------------
void inventory_Template(StuffBuff *psb)
{
    if( verify( psb ))
    {
        
        int type;
        int count = kInventoryType_Count;

        for( type = 0; type < count; ++type ) 
        {
            // deprecated
//             if( type == kInventoryType_Concept )
//             {
//                 s_ConceptTemplate(psb);
//             }
//            else 
            if( type == kInventoryType_BaseDetail )
            {
                // do nothing. handled int ent2_desc
            }
            else
             {
                char const *name = inventory_GetDbTableName( type );
                int i;
                AttribFileDict dict = inventory_GetMergedAttribs(type);
                int size = eaSize( &dict.ppAttribItems );
                bool end_of_base_recipes_found = false;

                // ----------
                // write out the inventory fields, packing them in each row.

                // Note: there is a limit of columns fields in a table,
                // so we have to make a series of parallel table as
                // the number of ids increase to store the inventory
                // info in. The alternative, storing in multiple rows,
                // would require naming the columns c0,c1,...,cN,
                // which is less useful to someone looking at the
                // db. c'est la vie.

                for( i = 0; i < size; ++i ) 
                {
                    const char *colname = dict.ppAttribItems[i]->name;
                    int tableId = dict.ppAttribItems[i]->id/MAX_INVITEMS_PER_ROW;
                                        
                    addStringToStuffBuff( psb, "%s%d[1].%s \"int4\"\n", name, tableId, colname );
                    if(dict.ppAttribItems[i]->id == LAST_BASERECIPE_ID)
                    {
                        end_of_base_recipes_found = true;
                        break;
                    }
                }

                if(type == kInventoryType_Recipe && end_of_base_recipes_found)
                {
                    // Note: special hack for invention recipes. Unfortunately the default
                    // inventory system was used for invention recipes. The default system
                    // assumes every player will have at least one of many items, but that
                    // there will be few of such types. the invention recipes assume the
                    // opposite: 12k types of recipes of which you'll have few. this is a hack
                    // to support this since they use the same database as base recipes where
                    // this system was acceptable.
                    int maxId = recipe_GetMaxId();
                    int numRows = maxId/RECIPESPLIT_NUM_COLS + 1;
                    
                    for(i = 0; i < RECIPESPLIT_NUM_COLS; ++i ) 
                    {
                        addStringToStuffBuff( psb, "InvRecipeInvention[%i].c%iType \"attribute\"\n", numRows, i);
                        addStringToStuffBuff( psb, "InvRecipeInvention[%i].c%iAmount \"int4\"\n", numRows, i);
                    }
                }
                
                // not allowed to clean up.
                // some of these items come from pooled memory.
                // just a leak we'll have to live with for building templates...
                //attribfiledict_Cleanup(&dict);
            }            
        }
    }
}


//------------------------------------------------------------
//  special case for concepts
//----------------------------------------------------------
static void s_packageConcepts(Entity *e, StuffBuff *psb)
{
    int num = eaSize(&e->pchar->conceptInv);
    int i;
    char const *tablename = inventory_GetDbTableName( kInventoryType_Concept );

    for( i = 0; i < num; ++i ) 
    {
        ConceptItem *c = e->pchar->conceptInv[i]->concept;
        int row;
        int col;
        int j;
        
        s_GetConceptRowCol(i,&row,&col);
        addStringToStuffBuff( psb, "%s[%d].c%d%s %d\n" ,tablename,row,col,CONCEPT_COLNAME_DEFIDANDAMT, s_PackDefIdAndAmountCol( c->def->id, e->pchar->conceptInv[i]->amount ));
        
        for( j = 0; j < ARRAY_SIZE( c->afVars ); ++j ) 
        {
            addStringToStuffBuff( psb, "%s[%d].c%d%s%d %f\n" ,tablename,row,col,CONCEPT_COLNAME_VARSTEM,j, c->afVars[j] );
        }
    }
}

//------------------------------------------------------------
//  pack the inventory in the buff
//----------------------------------------------------------
void entity_packageInventory(Entity *e, StuffBuff *psb)
{
    if( verify( e && psb ))
    {
        int type;
        int count = kInventoryType_Count;
        
        for( type = 0; type < count; ++type )
        {
            if( type == kInventoryType_Concept )
            {
                s_packageConcepts(e, psb);
            }
            else if( type == kInventoryType_BaseDetail )
            {
                // do nothing. handled in ent2_desc
            }
            else
            {
                
                char const *tablename = inventory_GetDbTableName( type );
                int num = character_GetInvSize( e->pchar, type );
                int i;
                int split_inv_start = -1; 
                
                // get dict so we don't write ent items that don't exist yet 
                // this is to help the designers by not writing things to the entity
                // until the new templates have been made
                // the side effect of this is that inventory not in the db
                // will be lost silently.
                AttribFileDict const *attribDict = inventorytype_GetAttribs( type );

                // ----------
                // bug track: find any duplicate entries
                for( i = 0; i < num; ++i ) 
                {
                    int id;
                    int amnt;
                    char *columnName = NULL;
                    int j;
                    character_GetInvInfo(e->pchar,&id,&amnt,&columnName,type,i); 

                    if( id )
                    {
                        for( j = i+1; j < num; ++j ) 
                        {
                            int id2;
                            int amnt2;
                            char *columnName2 = NULL;
                            character_GetInvInfo(e->pchar,&id2,&amnt2,&columnName2,type,j); 
                            
                            assert(id2 != id && (!columnName || !columnName2 || 0 != stricmp( columnName2, columnName )));
                        }
                    }
                }
                
                // ----------
                // write out the data

                // for each item in inventory.
                for( i = 0; i < num; ++i ) 
                {
                    int id;
                    int amnt;
                    char *columnName = NULL;
                    character_GetInvInfo(e->pchar,&id,&amnt,&columnName,type,i); 

                    // ----------
                    // if the id is valid, and the character has any
                    // Note: see inventory_Template for how inventory fields work

                    if( type == kInventoryType_Recipe && id > LAST_BASERECIPE_ID )
                    {
                        int cid;
                        int rid;
                        
                        if( split_inv_start < 0 )
                            split_inv_start = i;

                        rid = (i - split_inv_start)/RECIPESPLIT_NUM_COLS;
                        cid = (i - split_inv_start)%RECIPESPLIT_NUM_COLS;
                        
                        addStringToStuffBuff( psb, "InvRecipeInvention[%i].c%iType \"%s\"\n", rid, cid, columnName);
                        addStringToStuffBuff( psb, "InvRecipeInvention[%i].c%iAmount %i\n", rid, cid, amnt);
                        
                    }
                    else if( id > 0 && amnt > 0 && id <= attribDict->maxId )
                    {
                        int tableId = id/MAX_INVITEMS_PER_ROW;
                        addStringToStuffBuff( psb, "%s%d[0].%s %d\n", tablename, tableId, columnName, amnt );
                    }
                    else if( id > attribDict->maxId )
                    {
                        printf("skipping packing inventory with id %d (max %d), may need to build templates\n", id, attribDict->maxId );
                    }
                }                
            }
        }
    }
}


MP_DEFINE(DBGenericInventoryItem);
static DBGenericInventoryItem* dbgenericinventoryitem_Create( InventoryType type, int id, int amt )
{
    DBGenericInventoryItem *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(DBGenericInventoryItem, 64);
    res = MP_ALLOC( DBGenericInventoryItem );
    if( verify( res ))
    {
        res->type = type;
        res->id = id;
        res->amount = amt;
    }
    // --------------------
    // finally, return

    return res;
}


void dbgenericinventoryitem_Destroy( DBGenericInventoryItem *item )
{
    MP_FREE(DBGenericInventoryItem, item);
}


MP_DEFINE(DBConceptInventoryItem);
static DBConceptInventoryItem* dbconceptinventoryitem_Create()
{
    DBConceptInventoryItem *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(DBConceptInventoryItem, 64);
    res = MP_ALLOC( DBConceptInventoryItem );
    if( verify( res ))
    {
        res->defId = 0;
    }

    // --------------------
    // finally, return

    return res;
}

void dbconceptinventoryitem_Destroy( DBConceptInventoryItem *item )
{
    MP_FREE(DBConceptInventoryItem, item);
}



InventoryType inventory_typeFromTable(char *table)
{
    static StashTable s_invTypeFromTableName = NULL;
    StashElement res = NULL;
    char tableStem[128];

    if( !s_invTypeFromTableName )
    {
        int i;
        s_invTypeFromTableName = stashTableCreateWithStringKeys( kInventoryType_Count*2, StashDeepCopyKeys );
        for( i = 0; i < kInventoryType_Count; ++i ) 
        {
            stashAddInt(s_invTypeFromTableName, inventory_GetDbTableName(i), i, false);
        }
    }

    // ----------
    // find the passed table in the list

    // get the type name from the table name.
    //see inventory_Template for explaination of table naming scheme
    {
        int i;
        strcpy(tableStem,table);

        for(i = (int)strlen(table) - 1; i >= 0; --i)
        {
            if(tableStem[i] >= '0' && tableStem[i] <= '9')
            {
                tableStem[i] = 0;
            }
            else
            {
                break;
            }
        }

        stashFindElement( s_invTypeFromTableName, tableStem , &res);
    }
    if (res)
    {
        return (int)(intptr_t)stashElementGetPointer(res);        
    }
    else if( 0 == stricmp(tableStem,"InvRecipeInvention")) // special hack for dual inventory
    {
        return kInventoryType_Recipe;
    }
    else
    {
        return kInventoryType_Count;
    }
}

//------------------------------------------------------------
// try to unpack the line into character's inventory
//----------------------------------------------------------
bool entity_unpackInventory(DBGenericInventoryItem ***hGenItems, DBConceptInventoryItem ***hConceptItems, Entity *e, char *table, int row, char *col, char *val)
{
    InventoryType type = inventory_typeFromTable(table);

    if( type != kInventoryType_Count && verify(e) )
    {        
        switch ( type )
        {
        case kInventoryType_Concept:
            if( verify( col[0] == 'c' && strlen(col) >= 2 ))
            {
                int colId = -1;
                char colField[512];
                
                if( 2 == sscanf( col, "c%d%s", &colId, colField ))
                {
                    int n = s_GetConceptInvIdx( row, colId);
                    int defId = -1; 
                    int varid = -1;
                    F32 fVar = 0.f;

                    if( !(*hConceptItems) )
                        eaCreate(hConceptItems);

                    // ensure capacity for this item
                    eaSetSize(hConceptItems, MAX(n + 1,eaSize( hConceptItems )));

                    if( !(*hConceptItems)[n] )
                    {
                        (*hConceptItems)[n] = dbconceptinventoryitem_Create();
                    }

                    if( 0 == strcmp(colField, CONCEPT_COLNAME_DEFIDANDAMT ))
                    {
                        // get the def id
                        int tmp = atoi(val);
                        s_UnpackDefIdAndAmountCol(&(*hConceptItems)[n]->defId, &(*hConceptItems)[n]->amount, tmp );
                    }
                    else if( 1 == sscanf( colField, CONCEPT_COLNAME_VARSTEM "%d", &varid ))
                    {
                        // set the fvar
                        (*hConceptItems)[n]->afVars[varid] = atof(val);
                    }
                    else
                    {
                        assertmsg(0,"unknown column");
                        dbLog("UnpackPowersFatal", e, "unknown column %s", col);
                    }
                }
                else
                {
                    assertmsg(0,"unknown column");
                    dbLog("UnpackPowersFatal", e, "unknown column %s", col);
                }
            }
            break;
        case kInventoryType_BaseDetail:
            // do nothing. handled in ent2_desc
            type = kInventoryType_Count;
            break;
        default:
        {
            // handle the generic inventory types
            
            // check that this id has room for it in the ent's template
            AttribFileDict const *dict = inventorytype_GetAttribs( type );
            int id = -1;

            // check if its a special column in the split inventory c[0-9]+
            if( type == kInventoryType_Recipe 
                && col[0] == 'c' 
                && INRANGE(col[1],'0','9'+1) 
                )
            {
                if( strstr(col,"Type") )
                {
                    if( dict && stashFindInt( dict->idHash, val, &id ) )
                    {
                        eaPush(hGenItems, dbgenericinventoryitem_Create(type,id,0));
                    } else {
                        eaPush(hGenItems, NULL);
                    }
                }
                else 
                {
                    // assumes last thing on earray is the type associated with this
                    int n = eaSize(hGenItems);
                    if(n > 0 && (*hGenItems)[n-1])
                    {
                        DBGenericInventoryItem *last = (*hGenItems)[n-1];
                        last->amount = atoi(val);
                    }
                }
            }
            else if( dict && stashFindInt( dict->idHash, col, &id ) )
            {
                int amount = atoi(val);
                eaPush(hGenItems, dbgenericinventoryitem_Create(type,id,amount));
            }
        }
        };
    }

    // --------------------
    // finally, return

    return type != kInventoryType_Count; 
}

