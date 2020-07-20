/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/components/earray.h>
#include "container/containerloadsave.h"
#include "container/dbcontainerpack.h"
#include "costume_db.h"
#include "auth/authUserData.h"
#include "entity/character_level.h"
#include "entity/character_base.h"
#include "entity/entity.h"
#include "gameData/BodyPart.h"
#include "entity/powers.h"

void packageCostumes( Entity * e, StuffBuff *psb, StructDesc *desc)
{
    int i, j;
    DBCostume dbcostumes ={0};

    for( i = 0; i < e->pl->num_costumes_stored; i++ )
    {
        for( j = 0; j < MAX_COSTUME_PARTS; j++ )
        {
             if( j < e->pl->costume[i]->appearance.iNumParts )
            {

                if( (!e->pl->costume[i]->parts[j]->pchGeom || stricmp( e->pl->costume[i]->parts[j]->pchGeom, "none") == 0 ) &&
                    (!e->pl->costume[i]->parts[j]->pchFxName || stricmp( e->pl->costume[i]->parts[j]->pchFxName, "none") == 0 ) &&
                    (!e->pl->costume[i]->parts[j]->pchTex1 || stricmp( e->pl->costume[i]->parts[j]->pchTex1, "none") == 0 ) &&
                    (!e->pl->costume[i]->parts[j]->pchTex2 || stricmp( e->pl->costume[i]->parts[j]->pchTex2, "none") == 0 ) &&
                    (!e->pl->costume[i]->parts[j]->regionName || stricmp( e->pl->costume[i]->parts[j]->regionName, "none") == 0 ) &&
                    (!e->pl->costume[i]->parts[j]->bodySetName || stricmp( e->pl->costume[i]->parts[j]->bodySetName, "none") == 0 ) )
                {
                    continue;
                }

                StructCopy(ParseCostumePart, e->pl->costume[i]->parts[j],&dbcostumes.part[(i*MAX_COSTUME_PARTS)+j],0,0 );
                dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].costumeNum = i;

                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchGeom && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchGeom, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchGeom = NULL;
                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchFxName && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchFxName, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchFxName = NULL;
                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex1 && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex1, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex1 = NULL;
                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex2 && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex2, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].pchTex2 = NULL;
                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].regionName && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].regionName, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].regionName = NULL;
                if( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].bodySetName && stricmp( dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].bodySetName, "none") == 0 )
                    dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].bodySetName = NULL;
                
                dbcostumes.part[(i*MAX_COSTUME_PARTS)+j].displayName = NULL;
            }
        }
    }

    for(j=0;desc->lineDescs[j].type;j++)
    {
        structToLine(psb,(char*)&dbcostumes,0,desc,&desc->lineDescs[j],0);
    }
}


void packageAppearances( Entity *e, StuffBuff *psb, StructDesc *desc )
{
    int i;
    DBAppearance dba ={0};

    for( i = 0; i < e->pl->num_costumes_stored; i++ )
    {
        compressAppearanceScales(&e->pl->costume[i]->appearance);
        memcpy( &dba.appearance[i], &e->pl->costume[i]->appearance, sizeof(Appearance) );
    }

    for(i=0;desc->lineDescs[i].type;i++)
    {
        structToLine(psb,(char*)&dba,0,desc,&desc->lineDescs[i],0);
    }
}

void packagePowerCustomizations( Entity *e, StuffBuff *psb, StructDesc *desc )
{
    int i,j;
    int currentPower = 0;
    DBPowerCustomizationList dbpc ={0};

    for( i = 0; i < e->pl->num_costumes_stored; i++ )
    {
        int numCustomizations = eaSize(&e->pl->powerCustomizationLists[i]->powerCustomizations);
        for (j = 0; j < numCustomizations; ++j)
        {
            const BasePower *power = e->pl->powerCustomizationLists[i]->powerCustomizations[j]->power;
            StructInitFields(ParseDBPowerCustomization, &dbpc.powerCustomization[currentPower]);
            if (power)
            {
                dbpc.powerCustomization[currentPower].token = e->pl->powerCustomizationLists[i]->powerCustomizations[j]->token;
                dbpc.powerCustomization[currentPower].customTint = e->pl->powerCustomizationLists[i]->powerCustomizations[j]->customTint;
                dbpc.powerCustomization[currentPower].powerCatName = power->psetParent->pcatParent->pchName;
                dbpc.powerCustomization[currentPower].powerSetName = power->psetParent->pchName;
                dbpc.powerCustomization[currentPower].powerName = power->pchName;
                dbpc.powerCustomization[currentPower].slotId = i+1;        //    0 is not set
                currentPower++;
                continue;
            }
        }
    }
    for(i=0;desc->lineDescs[i].type;i++)
    {
        structToLine(psb,(char*)&dbpc,0,desc,&desc->lineDescs[i],0);
    }
}

void unpackCostumes( Entity * e, DBCostume *dbcostumes)
{
    int i, j;
    static char none_str[] = "none";
    int numCostumes = MIN(MAX_COSTUMES,costume_get_num_slots(e));

    if (e && e->pl)
        numCostumes = MIN(MAX_COSTUMES, MAX(e->pl->num_costumes_stored, numCostumes));

    for( i = 0; i < numCostumes; i++ )
    { 

        for( j = 0; j < MAX_COSTUME_PARTS; j++ )
        {
             if( dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].pchGeom || dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].pchFxName || 
                dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].pchTex1 || dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].pchTex2 ||
                dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].regionName || dbcostumes->part[(i*MAX_COSTUME_PARTS)+j].bodySetName)
            {    
                StructCopy(ParseCostumePart, &dbcostumes->part[(i*MAX_COSTUME_PARTS)+j],e->pl->costume[i]->parts[j], 0,0);
            }

            if( !e->pl->costume[i]->parts[j]->pchGeom || !e->pl->costume[i]->parts[j]->pchGeom[0] )
                e->pl->costume[i]->parts[j]->pchGeom = none_str;
            if( !e->pl->costume[i]->parts[j]->pchFxName || !e->pl->costume[i]->parts[j]->pchFxName[0] ) 
                e->pl->costume[i]->parts[j]->pchFxName = none_str;
            if( !e->pl->costume[i]->parts[j]->pchTex1 || !e->pl->costume[i]->parts[j]->pchTex1[0] )
                e->pl->costume[i]->parts[j]->pchTex1 = none_str;
            if( !e->pl->costume[i]->parts[j]->pchTex2 || !e->pl->costume[i]->parts[j]->pchTex2[0] )
                e->pl->costume[i]->parts[j]->pchTex2 = none_str;
            //if( !e->pl->costume[i]->parts[j]->regionName || !e->pl->costume[i]->parts[j]->regionName[0] )
            //    e->pl->costume[i]->parts[j]->regionName = none_str;
            //if(! e->pl->costume[i]->parts[j]->bodySetName || !e->pl->costume[i]->parts[j]->bodySetName[0] )
            //    e->pl->costume[i]->parts[j]->bodySetName = none_str;
        }
    }

    if( e->pl && character_IsInitialized(e->pchar) )
        costumeAwardAuthParts( e );

    // while we are at it, might as well convert supergroup to new color structure, fun fun
    if(e->supergroup && (!e->pl->superColorsPrimaryU.SGBitSet || !e->pl->superColorsSecondaryU.SGBitSet || !e->pl->superColorsPrimary2U.SGBitSet || !e->pl->superColorsSecondary2U.SGBitSet))
        costume_SGColorsExtract(e, e->pl->superCostume, &e->pl->superColorsPrimaryU, &e->pl->superColorsSecondaryU, &e->pl->superColorsPrimary2U, &e->pl->superColorsSecondary2U, &e->pl->superColorsTertiaryU, &e->pl->superColorsQuaternaryU);

}
void unpackAppearance( Entity *e, DBAppearance * dba )
{
    int i,k;
    int numCostumes = MIN(MAX_COSTUMES,costume_get_num_slots(e));

    if (e && e->pl)
        numCostumes = MIN(MAX_COSTUMES, MAX(e->pl->num_costumes_stored, numCostumes));
    // If any fields in Appearance are changed make sure that this function gets
    // updated!
#ifndef _M_X64
    STATIC_INFUNC_ASSERT(sizeof(Appearance) == 472)
#endif

    for( i = 0; i < numCostumes; i++ )
    {
        decompressAppearanceScales(&dba->appearance[i]);

        // OLD CHARACTERS FIX
        // appearance[0].bodytype -> BodyType
        // appearance[0].colorSkin -> ColorSkin
        // Don't overwrite these from the Appearances data
        if( !e->pl->costume[i] )
        {
            e->pl->costume[i] = costume_create(GetBodyPartCount());
        }

        if (i > 0)
        {
            e->pl->costume[i]->appearance.bodytype        = dba->appearance[i].bodytype;
            e->pl->costume[i]->appearance.colorSkin        = dba->appearance[i].colorSkin;
        }

        e->pl->costume[i]->appearance.convertedScale    = dba->appearance[i].convertedScale;

        for(k=0;k<MAX_BODY_SCALES;k++)
        {
            // OLD CHARACTERS FIX
            // appearance[0].fScales[0] -> BodyScale
            // appearance[0].fScales[1] -> BoneScale
            // Don't overwrite these from the Appearances data

            if (i > 0 || k > 1)
                e->pl->costume[i]->appearance.fScales[k] = dba->appearance[i].fScales[k];
        }

        for(k=0;k<NUM_SG_COLOR_SLOTS;k++)
        {
            e->pl->costume[i]->appearance.superColorsPrimaryU[k]    = dba->appearance[i].superColorsPrimaryU[k];
            e->pl->costume[i]->appearance.superColorsSecondaryU[k]  = dba->appearance[i].superColorsSecondaryU[k];
            e->pl->costume[i]->appearance.superColorsPrimary2U[k]   = dba->appearance[i].superColorsPrimary2U[k];
            e->pl->costume[i]->appearance.superColorsSecondary2U[k] = dba->appearance[i].superColorsSecondary2U[k];
            e->pl->costume[i]->appearance.superColorsTertiaryU[k]   = dba->appearance[i].superColorsTertiaryU[k];
            e->pl->costume[i]->appearance.superColorsQuaternaryU[k] = dba->appearance[i].superColorsQuaternaryU[k];
        }

        e->pl->costume[i]->appearance.currentSuperColorSet = dba->appearance[i].currentSuperColorSet;


        if(    !e->pl->costume[i]->appearance.convertedScale )
            costume_retrofitBoneScale( e->pl->costume[i] );
    }
}

#define MAX_I16_POWER_CUST_POWERS 300
static void correctCostumeIndexFromLegacyDBCust(DBPowerCustomizationList *dbpowers, int numCostumes)
{
    int currentStatus = 0;
    int i;
    for (i = 0; i < numCostumes*MAX_POWERS; ++i)
    {
        if (dbpowers->powerCustomization[i].slotId)
        {
            return;    //    we are up to date with new packing method.    early exit
        }
    }
    for (i = 1; i < numCostumes; ++i)
    {
        //    check the first power at one of these offset to try to find what state we are in
        //    if the costume has added power customization, it will always have the full complement of powers stored, regardless if they are default or not
        if (dbpowers->powerCustomization[i*MAX_I16_POWER_CUST_POWERS].powerName)
        {
            currentStatus = 0;    //    prei16
            break;
        }
        if (dbpowers->powerCustomization[i*MAX_POWERS].powerName)
        {
            currentStatus = 1;    //    posti16
            break;
        }
    }
    for (i = 0; i < numCostumes; ++i)
    {
        int j;
        int maxpowers = currentStatus ? MAX_POWERS : MAX_I16_POWER_CUST_POWERS;
        for (j = 0; j < maxpowers; ++j)
        {
            if (dbpowers->powerCustomization[(i*maxpowers)+j].powerName)
            {
                dbpowers->powerCustomization[(i*maxpowers)+j].slotId = i+1;
            }
        }
    }
}

void unpackPowerCustomizations( Entity * e, DBPowerCustomizationList *dbpowers)
{
    int i;
    int currentPowerIndex[MAX_COSTUMES];
    int numCostumes = MIN(MAX_COSTUMES,costume_get_num_slots(e));

    if (e && e->pl)
        numCostumes = MIN(MAX_COSTUMES, MAX(e->pl->num_costumes_stored, numCostumes));

    for (i = 0; i < numCostumes; ++i)
    {
        currentPowerIndex[i] = 0;
    }
    // If any fields in Appearance are changed make sure that this function gets
    // updated!
#ifndef _M_X64
    STATIC_INFUNC_ASSERT(sizeof(PowerCustomization) == 20)
#endif
    correctCostumeIndexFromLegacyDBCust(dbpowers, numCostumes);
    for( i = 0; i < numCostumes; i++ )
    {
        if (e->pl->powerCustomizationLists[i] == NULL)        e->pl->powerCustomizationLists[i] = StructAllocRaw(sizeof(PowerCustomizationList));
        powerCustList_allocPowerCustomizationList(e->pl->powerCustomizationLists[i], MAX_POWERS);
    }
    for( i = 0; i < MAX_POWERS*numCostumes; i++ )
    {
        if (&dbpowers->powerCustomization[i] && dbpowers->powerCustomization[i].powerName)
        {
            devassert(dbpowers->powerCustomization[i].slotId);    //    power didn't go into a slot. how did this happen?
            if (dbpowers->powerCustomization[i].slotId && dbpowers->powerCustomization[i].slotId <= numCostumes)
            {
                int powerIndex;
                int costumeIndex = dbpowers->powerCustomization[i].slotId-1;
                powerIndex = currentPowerIndex[costumeIndex];
                initializePowerCustomizationPower(&dbpowers->powerCustomization[i], e->pl->powerCustomizationLists[costumeIndex]->powerCustomizations[powerIndex]);
                if (!e->pl->powerCustomizationLists[costumeIndex]->powerCustomizations[powerIndex]->power)
                {
                    StructInitFields(ParsePowerCustomization, e->pl->powerCustomizationLists[costumeIndex]->powerCustomizations[powerIndex]);
                    break;
                }
                else
                {
                    e->pl->powerCustomizationLists[costumeIndex]->powerCustomizations[powerIndex]->token = dbpowers->powerCustomization[i].token;
                    e->pl->powerCustomizationLists[costumeIndex]->powerCustomizations[powerIndex]->customTint = dbpowers->powerCustomization[i].customTint;
                }
                currentPowerIndex[costumeIndex] = currentPowerIndex[costumeIndex]+1;
            }
        }
    }
    for( i = 0; i < numCostumes; i++ )
    {
        int rem;
        for (rem = (MAX_POWERS-1); rem >= currentPowerIndex[i]; --rem)
        {
            StructDestroy(ParsePowerCustomization, e->pl->powerCustomizationLists[i]->powerCustomizations[rem]);
            eaRemove(&e->pl->powerCustomizationLists[i]->powerCustomizations, rem);
        }
        eaQSort(e->pl->powerCustomizationLists[i]->powerCustomizations, comparePowerCustomizations);
    }

    powerCust_handleAllEmptyPowerCust(e);
}
/* End of File */
