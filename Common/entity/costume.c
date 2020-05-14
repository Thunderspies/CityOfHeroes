/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/costume.h"
#include "entity/rewardtoken.h"
#include "entity/Supergroup.h"
#include "entity/entity.h"                // for Entity
#include "entity/EntPlayer.h"
#include "entity/character_base.h"
#include "entity/character_level.h"    // for tailor_setCurrent
#include "entity/character_inventory.h"
#include <utilitieslib/network/net_packet.h>            // for pkt.. functions
#include "gameData/BodyPart.h"  // for BodyPart
#include "gameData/costume_data.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include "entity/entVarUpdate.h"
#include "GameData/costume_data.h"
#include <utilitieslib/utils/mathutil.h>
#include "seq/seq.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "auth/authUserData.h"
#include "entity/powers.h"                // for PowerSet
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StringCache.h>
#include "gameComm/Npc.h"
#include "entity/character_net.h"
#include "entity/power_customization.h"
#include "account/AccountData.h"
#include <utilitieslib/components/estring.h>
#include "entity/character_level.h"

#ifdef SERVER
    #include "dbcomm/dbcomm.h"
    #include "entity/entVarUpdate.h"
    #include "gamecomm/svr_chat.h"
    #include "language/langServerUtil.h"
    #include "entity/entGameActions.h"
    #include "entity/entserver.h"
    #include "player/badges_server.h"
    #include "comm_game.h"
    #include "cmdparse/chat_emote.h"
    #include "TeamReward.h"
    #include "dbcomm/logcomm.h"
    #include "account/AccountData.h"
#elif CLIENT
    #include "UI/uiChat.h"
    #include "UI/sprite/sprite_text.h"
    #include "entity/costume_client.h"
    #include "player/player.h"
    #include "UI/uiTailor.h"
    #include "UI/uiSuperCostume.h"
    #include "clientcomm/dbclient.h"
    #include "player/inventory_client.h"
#endif

STATIC_ASSERT(sizeof(Costume) == sizeof(cCostume));

#define ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER 0
static  char    *s_types[] = {"GEOM","TEXTURE","TEXTURE1","COLOR"};

static const TailorCost * gTailorMaster_curr_cost;
static const TailorCost * gTailorMaster_next_cost;

Costume* costume_create(int numParts)
{
    Costume* costume;

    // "numParts" must be less than MAX_COSTUME_PARTS because that's how many
    // parts containersaveload.c is hardcoded to save/load.
    //
    // This is also the reason why we always allocate MAX_COSTUME_PART parts even
    // though the number of parts in use is likely to be less.
    assert(numParts <= MAX_COSTUME_PARTS);
    costume = StructAllocRaw(sizeof(Costume));
    costume->appearance.iNumParts = numParts;
    costume->appearance.convertedScale = 1;
    costume->containsRestrictedStoreParts = false;
    // Create and fill the CostumeParts array with emtpy parts for this costume.
    // JS:    It is required that these things be filled with emtpty entries because
    //        the contaier loading code does not know how to create this kind of
    //        structure on its own.
    {
        int i;
        // Add each empty CostumePart into the array.
        for(i = 0; i < MAX_COSTUME_PARTS; i++)
            eaPush(&costume->parts, StructAllocRaw(sizeof(CostumePart)));
    }

    return costume;
}

void costume_init(Costume* costume)
{
    int i; //, color;

    assert(costume);
    costume->appearance.convertedScale = 1;
    costume->containsRestrictedStoreParts = false;
     for( i = 0; i < costume->appearance.iNumParts; i++ )
    {
        costume_PartSetGeometry(costume, i, NULL);
        costume_PartSetTexture1(costume, i, NULL);
        costume_PartSetTexture2(costume, i, NULL);
        costume_PartSetFx(costume, i, NULL);
    }
}


int costume_get_max_earned_slots(Entity* e)
{
    int retval = MAX_EARNED_COSTUMES;

    // VEATs get an extra slot because of their missions.
    if( e && e->pchar && e->pchar->pclass && ( class_MatchesSpecialRestriction( e->pchar->pclass, "ArachnosSoldier" ) || class_MatchesSpecialRestriction( e->pchar->pclass, "ArachnosWidow" ) ) )
        retval++;

    return retval;
}

int costume_get_max_slots(Entity* e)
{
    int retval = MAX_COSTUMES;

    return retval;
}

int costume_get_num_slots(Entity *e)
{

    int retval = 1;

    if (e && e->pl)
    {
        retval += e->pl->num_costume_slots;
        retval += AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("upcscs06")) != 0;
        retval += AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("upcscs07")) != 0;
        retval += AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("upcscs08")) != 0;
        retval += AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("upcscs09")) != 0;
        retval += AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("upcscs10")) != 0;
    }

    return MIN(retval, MAX_COSTUMES);
}

const cCostume* costume_as_const(Costume * src)
{
    return cpp_reinterpret_cast(cCostume*)(src);
}

#if CLIENT
Costume* costume_as_mutable_cast(const cCostume* src)    // necessary client hack at the moment
{
    return cpp_reinterpret_cast(Costume*)(src);
}
#endif

// @todo, change these names around to avoid a lot of cast calls I added
Costume* costume_clone_nonconst(Costume* src)
{
    return costume_clone(costume_as_const(src));
}

Costume* costume_clone(const cCostume* src)
{
    Costume* dst;
    int i;
    dst = costume_create(GetBodyPartCount());
    costume_init(dst);
    dst->appearance = src->appearance;
    dst->containsRestrictedStoreParts = src->containsRestrictedStoreParts;
    for (i = 0; i < MIN(eaSize(&dst->parts), eaSize(&src->parts)); ++i)
    {
        int j;
        StructCopy(ParseCostumePart, src->parts[i], dst->parts[i], 0, 0 );
        for (j = 0; j < 4; ++j)
        {
            dst->parts[i]->colorTmp[j].integer = src->parts[i]->colorTmp[j].integer;
        }
    }

    return dst;
}

Costume* costume_new_slot(Entity* e)
{
    int i;
    Costume* dst = NULL;

    if(devassert(e && e->pl && e->pchar))
    {
        int j = 0;
        BodyType eBodyType = kBodyType_Male;
        
        dst = costume_create(GetBodyPartCount());
        costume_init(dst);

        // By default clone from the first slot.
        i = 0;

        if(!stricmp(e->pchar->pclass->pchName, "Class_Arachnos_Soldier") ||
            !stricmp(e->pchar->pclass->pchName, "Class_Arachnos_Widow"))
        {
            // If this is a VEAT, we will attempt to clone the 1st slot, since slot 0
            //    contains the uniform
            i = 1;
            if (e->pl->costume[0] != NULL)
                eBodyType = e->pl->costume[0]->appearance.bodytype;
        }

        if (e->pl->costume[i] != NULL)
        {
            StructCopy(ParseCostume, e->pl->costume[i], dst, 0,0);
            dst->appearance.convertedScale = e->pl->costume[i]->appearance.convertedScale;
        } else {
            costume_SetBodyType(dst, eBodyType);
            e->pl->costume[i] = dst;
            costume_setDefault(e, i);
        }
    }

    return dst;
}

void costume_destroy(Costume* costume)
{
    assert(costume);
    StructDestroy(ParseCostume, costume);
}

void costume_SetSkinColor(Costume* costume, Color color)
{
    assert(costume);
    costume->appearance.colorSkin = color;
}

void costume_SetBodyName(Costume* costume, const char* bodyName)
{
    assert(costume);

    costume->appearance.costumeFilePrefix = allocAddString_checked(bodyName);
}

void costume_SetBodyType(Costume* costume, BodyType bodytype)
{
    assert(costume);
    costume->appearance.bodytype = bodytype;
}

void costume_SetVillainIdx(Costume* costume, int i)
{
    assert(costume);
    costume->appearance.iVillainIdx = i;
}

void costume_SetBodyScale(Costume * costume, BodyScale_Type type, float fScale)
{
    assert(costume);
    assert(type < MAX_BODY_SCALES && type > 0);
    costume->appearance.fScales[type] = fScale;
}

void costume_SetScale(Costume* costume, float fScale)
{
    assert(costume);
    costume->appearance.fScale = fScale;
}

void costume_SetBoneScale(Costume* costume, float fBoneScale)
{
    assert(costume);
    costume->appearance.fBoneScale = fBoneScale;
}

void costume_PartSetName(Costume* costume, int iIdxBone, const char* name)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchName = allocAddString_checked(name);
}

const char * costume_GetCachedStringOrNone(const char *string)
{
    if (!string || string[0] == 0 || stricmp(string, "none") == 0)
        return allocAddString("none");

    return allocAddString(string);
}

void costume_PartSetPath( Costume* costume, int iIdxBone, const char* displayName, const char* region, const char* set )
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->displayName = allocAddString_checked(displayName);
    costume->parts[iIdxBone]->regionName = allocAddString_checked(region);
    costume->parts[iIdxBone]->bodySetName = allocAddString_checked(set);
}

void costume_PartSetGeometry(Costume* costume, int iIdxBone, const char *pchGeom)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchGeom = allocAddString_checked(pchGeom);
}

void costume_PartSetTexture1(Costume* costume, int iIdxBone, const char *pchTex)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchTex1 = costume_GetCachedStringOrNone(pchTex);
}

void costume_PartSetTexture2(Costume* costume, int iIdxBone, const char *pchTex)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchTex2 = costume_GetCachedStringOrNone(pchTex);
}

void costume_PartSetFx(Costume* costume, int iIdxBone, const char *pchFx)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchFxName = allocAddString_checked(pchFx);
}

void costume_PartSetColor(Costume* costume, int iIdxBone, int colornum, const Color color)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);
    assert(colornum >=0 && colornum < ARRAY_SIZE(costume->parts[iIdxBone]->color));

    costume->parts[iIdxBone]->color[colornum] = color;
}

void costume_PartForceTwoColor(Costume* costume, int iIdxBone)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);
    assert(ARRAY_SIZE(costume->parts[iIdxBone]->color) >= 4);

    costume->parts[iIdxBone]->color[2] = costume->parts[iIdxBone]->color[0];
    costume->parts[iIdxBone]->color[3] = costume->parts[iIdxBone]->color[1];
}

void costume_GetColors(Costume* costume, int iIdxBone, Color *pcolorSkin, Color *pcolor1, Color *pcolor2)
{
    assert(costume);
    assert(costume->parts);

    *pcolorSkin = costume->appearance.colorSkin;
    *pcolor1 = costume->parts[iIdxBone]->color[0];
    *pcolor2 = costume->parts[iIdxBone]->color[1];
}

void costume_PartSetAll(Costume* costume, int iIdxBone,
                        const char *pchGeom, const char *pchTex1, const char *pchTex2,
                        Color color1, Color color2)
{
    assert(costume);
    assert(costume->parts);
    assert(iIdxBone < costume->appearance.iNumParts);

    costume->parts[iIdxBone]->pchGeom = allocAddString_checked(pchGeom);
    costume->parts[iIdxBone]->pchTex1 = costume_GetCachedStringOrNone(pchTex1);
    costume->parts[iIdxBone]->pchTex2 = costume_GetCachedStringOrNone(pchTex2);
    costume->parts[iIdxBone]->color[0]  = color1;
    costume->parts[iIdxBone]->color[1]  = color2;
}

bool costume_StripBodyPart(Costume* costume, const char *part)
{
    int i;
    bool retval = false;

    devassert(costume);
    devassert(costume->parts);
    devassert(part);

    if( costume && costume->parts && part )
    {
        i = bpGetIndexFromName( part );

        if( i >= 0 && i < costume->appearance.iNumParts && EAINRANGE(i, costume->parts) )
        {
            devassert(!costume->parts[i]->pchName || allocFindString(costume->parts[i]->pchName));
            devassert(!costume->parts[i]->displayName || allocFindString(costume->parts[i]->displayName));
            devassert(!costume->parts[i]->bodySetName || allocFindString(costume->parts[i]->bodySetName));
            devassert(!costume->parts[i]->pchFxName || allocFindString(costume->parts[i]->pchFxName));
            costume->parts[i]->pchName = NULL;
            costume->parts[i]->displayName = NULL;
            costume->parts[i]->bodySetName = NULL;
            costume->parts[i]->pchFxName = NULL;

            retval = true;
        }
    }

    return retval;
}

static bool costume_AdjustCostumeForPower(Costume *costume, CostumePart *part )
{
    int retval = false;
    int i;

    assert(costume);
    assert(costume->parts);

    i = bpGetIndexFromName( part->pchName );

    if( i >= 0 && i < costume->appearance.iNumParts && EAINRANGE(i, costume->parts))
    {
        StructCopy(ParseCostumePart, part, costume->parts[i],0,0);
        retval = true;
    }

    return retval;
}

void costume_AdjustAllCostumesForPowers(Entity *e)
{
    const BasePowerSet *crab_primary = NULL;
    const BasePowerSet *crab_secondary = NULL;
    CostumePart *backpack_uniform_part = NULL;
    CostumePart *backpack_civilian_part = NULL;
    bool has_crab_powers = false;
    bool has_crab_backpack;
    int i;
    CostumePart *backpack;

    assert(e);
    assert(e->pl);

    crab_primary = powerdict_GetBasePowerSetByName( &g_PowerDictionary,
        "Arachnos_Soldiers", "Crab_Spider_Soldier" );
    crab_secondary = powerdict_GetBasePowerSetByName( &g_PowerDictionary,
        "Training_Gadgets", "Crab_Spider_Training" );

    // If you have either crab spider powerset you must have at least one
    // power from them (they're specialization powersets, so they're only
    // added to buy a power). All crab spiders need a backpack on all their
    // costumes because the attack powers don't work properly without it.
    if( ( crab_primary && character_OwnsPowerSetInAnyBuild( e->pchar, crab_primary ) ) ||
        ( crab_secondary && character_OwnsPowerSetInAnyBuild( e->pchar, crab_secondary ) ) )
    {
        has_crab_powers = true;
    }

    // Find the appropriate backpacks based on the character's body type.
    // We'll need the 'uniform' one if they're wearing crab/bane spider armor
    // in a given slot and the 'civilian' one in all other cases.
    switch( e->gender )
    {
        case GENDER_MALE:
        default:
            backpack_uniform_part = costume_getTaggedPart( "Crab_Spider_Backpack_Uniform_Male", kBodyType_Male );
            backpack_civilian_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Male", kBodyType_Male );
            break;
        case GENDER_FEMALE:
            backpack_uniform_part = costume_getTaggedPart( "Crab_Spider_Backpack_Uniform_Female", kBodyType_Female );
            backpack_civilian_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Female", kBodyType_Female );
            break;
        case GENDER_NEUTER:
            backpack_uniform_part = costume_getTaggedPart( "Crab_Spider_Backpack_Uniform_Huge", kBodyType_Huge );
            backpack_civilian_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Huge", kBodyType_Huge );
            break;
    }

    // Can't do anything if we don't have the parts to replace.
    if( backpack_uniform_part && backpack_civilian_part )
    {
        for( i = 0; i < MIN(MAX_COSTUMES, costume_get_num_slots(e)); i++ )
        {
            // Are they already wearing a backpack in this slot?
            has_crab_backpack = costume_PartFxPartialMatch( e->pl->costume[i], "back", "CrabBackpack" );

            // We need to modify the costume if it is lacking a backpack and
            // needs one added, or has one that is now superfluous.
            if( has_crab_powers && !has_crab_backpack )
            {
                costume_StripBodyPart( e->pl->costume[i], "Back" );
                costume_StripBodyPart( e->pl->costume[i], "Broach" );
                costume_StripBodyPart( e->pl->costume[i], "Cape" );
                costume_StripBodyPart( e->pl->costume[i], "Collar" );
#ifdef SERVER
                rewardtoken_Award( &e->pl->rewardTokens, "CrabBackpack", 0 );
#endif
                backpack = backpack_civilian_part;

                if( costume_PartGeomPartialMatch( e->pl->costume[i], "chest", "Chest_Arachnos_Crab" ) )
                {
                    backpack = backpack_uniform_part;
                }

                costume_AdjustCostumeForPower( e->pl->costume[i], backpack );
#ifdef SERVER
                e->send_costume = true;
                e->send_all_costume = true;
#endif
            }
            else if ( !has_crab_powers && has_crab_backpack )
            {
                costume_StripBodyPart( e->pl->costume[i], "Back" );
#ifdef SERVER
                rewardtoken_Unaward( &e->pl->rewardTokens, "CrabBackpack" );

                e->send_costume = true;
                e->send_all_costume = true;
#endif
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------

int    costume_PartIndexFromName( Entity* e, const char* name )
{
    int i;

    assert( e );
    assert( costume_current(e) );
    assert( costume_current(e)->parts );

    for( i = 0; i < GetBodyPartCount(); i++ )
    {
        if( stricmp( bodyPartList.bodyParts[i]->name, name ) == 0 )
            return i;
    }

    return -1;
}

int    costume_GlobalPartIndexFromName( const char* name )
{
    int i;
    if (!name)
        return -1;
    for( i = eaSize(&bodyPartList.bodyParts)-1; i >= 0; i-- )
    {
        if( stricmp( bodyPartList.bodyParts[i]->name, name ) == 0 )
            return i;
    }
    return -1;
}//

bool costume_PartGeomPartialMatch( Costume* costume, char* part, const char* name )
{
    bool retval = false;
    int bp = bpGetIndexFromName( part );

    if( bp >= 0 && bp < costume->appearance.iNumParts && EAINRANGE(bp, costume->parts))
    {
        if( costume->parts[bp]->pchGeom &&
            strstriConst( costume->parts[bp]->pchGeom, name ) )
        {
            retval = true;
        }
    }

    return retval;
}

bool costume_PartFxPartialMatch( Costume* costume, char* part, const char* name )
{
    bool retval = false;
    int bp = bpGetIndexFromName( part );

    if( bp >= 0 && bp < costume->appearance.iNumParts && EAINRANGE(bp, costume->parts) )
    {
        if( costume->parts[bp]->pchFxName &&
            strstriConst( costume->parts[bp]->pchFxName, name ) )
        {
            retval = true;
        }
    }

    return retval;
}

int costume_ValidateScales( SeqInst * seq, float fBoneScale, float fShoulderScale, float fChestScale, float fWaistScale, float fHipScale, float fLegScale, float fArmScale )
{
    float start;
    float fudge = .001; // there seems to be some loss of precision somewhere

    //start = (((fBoneScale+1)*(2-e->seq->type->headScaleRange))/2)-1;
    //if( fHeadScale < start || fHeadScale > start + e->seq->type->headScaleRange )
    //    return false;

    start = (((fBoneScale+1)*(2-seq->type->shoulderScaleRange))/2)-1;
    if( fShoulderScale < start-fudge || fShoulderScale > start + seq->type->shoulderScaleRange + fudge )
        return false;

    start = (((fBoneScale+1)*(2-seq->type->chestScaleRange))/2)-1;
    if( fChestScale < start-fudge || fChestScale > start + seq->type->chestScaleRange + fudge )
        return false;

    start = (((fBoneScale+1)*(2-seq->type->waistScaleRange))/2)-1;
    if( fWaistScale < start-fudge || fWaistScale > start + seq->type->waistScaleRange + fudge )
        return false;

    start = (((fBoneScale+1)*(2-seq->type->hipScaleRange))/2)-1;
    if( fHipScale < start-fudge || fHipScale > start + seq->type->hipScaleRange + fudge )
        return false;

    if( fLegScale < -(ABS(seq->type->legScaleMin)+fudge) || fLegScale > ABS(seq->type->legScaleMax)+fudge)
        return false;

    if( fArmScale < -(ARM_SHRINK_SCALE+fudge) || fArmScale > ARM_GROW_SCALE+fudge)
        return false;

    return true;
}
void getCurrentBodyTypeAndOrigin(Entity *e, Costume *costume, int *costumeBodyType, int *originSet)
{
    //    getCurrentBodyTypeAndOrigin(input, input, output, output)
    //    Function sets the costumeBodyType and originSet using the entity and costume.
    int finalBodyType = 0, finalOriginType = 0;
    char * bodyTypeName[] = { "Male", "Female",  "BasicMale", "BasicFemale", "Huge", "Villain", "Villain2", "Villain3"  };
    int i,j;
    if (costume)
    {
        for( i = eaSize( &gCostumeMaster.bodySet )-1; i >= 0; i-- )
        {
            if (gCostumeMaster.bodySet[i] && gCostumeMaster.bodySet[i]->name)
            {
                if ( stricmp( gCostumeMaster.bodySet[i]->name, bodyTypeName[costume->appearance.bodytype] ) == 0 )
                {
                    // look for arachnos soldier first
                    int found = 0;
                    finalBodyType = i;
                    if (originSet)    //    we may not care what the origin set is
                    {
                        for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0; j-- )
                        {
                            //    If there is no entity, we're going to have to assume all, since we can't assume that it will be an arachnos soldier or widow all the time
                            if( e && e->pchar && e->pchar->pclass)
                            {
                                if ((class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosSoldier")) && stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "ArachnosSoldier" ) == 0)
                                {
                                    finalOriginType = j;
                                    found = 1;
                                }
                            }
                        }

                        // look for arachnos widow next
                        if(!found)
                        {
                            for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0; j-- )
                            {
                                //    If there is no entity, we're going to have to assume all, since we can't assume that it will be an arachnos soldier or widow all the time
                                if(  e && e->pchar && e->pchar->pclass)
                                {
                                    if ((class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosWidow")) && stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "ArachnosWidow" ) == 0)
                                    {
                                        finalOriginType = j;
                                        found = 1;
                                    }
                                }
                            }
                        }

                        // didn't find
                        if(!found)
                        {
                            for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0; j-- )
                            {
                                if( stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "All" ) == 0 )
                                {
                                    finalOriginType = j;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    if (costumeBodyType)
    {
        *costumeBodyType = finalBodyType;
    }
    if (originSet)
    {
        *originSet = finalOriginType;
    }
}
int costumeCorrectColors(Entity *e, Costume *costume, int **badColorList)
{
    //    returns 1 for correct colors
    //    returns 0 for incorrect colors
    Color originalColor;
    int i, j, k, bodyType, finalBodyType, finalOriginType, validColor, isGood = 1;
    
    const ColorPalette **palette;
    originalColor = costume->appearance.colorSkin;    
    bodyType = costume->appearance.bodytype;
    finalBodyType = -1;
    if (bodyType == kBodyType_Enemy3)
    {
        bodyType--;
    }
    getCurrentBodyTypeAndOrigin(e, costume, &finalBodyType, &finalOriginType);
    palette = gCostumeMaster.bodySet[finalBodyType]->originSet[finalOriginType]->skinColors;

    validColor = 0;
    originalColor.a = 0xff;
    for( i = eaSize( (F32***)&palette[0]->color )-1; i >= 0; i-- )
    {
        if (((*palette[0]->color[i])[0] == originalColor.r) && 
            ((*palette[0]->color[i])[1] == originalColor.g) && 
            ((*palette[0]->color[i])[2] == originalColor.b) && 
            originalColor.a == 0xff)
        {
            validColor = 1;
            break;
        }
    }
    if (!validColor)
    {
        costume->appearance.colorSkin.r = (*palette[0]->color[0])[0];
        costume->appearance.colorSkin.g = (*palette[0]->color[0])[1];
        costume->appearance.colorSkin.b = (*palette[0]->color[0])[2];
        costume->appearance.colorSkin.a = 0xff;
        if (badColorList)    
        {
            eaiPush(badColorList, 0);
        }
        isGood = 0;
    }

    palette = gCostumeMaster.bodySet[finalBodyType]->originSet[finalOriginType]->bodyColors;
    for ( j = 0; j < eaSize(&costume->parts); j++)
    {        
        for (k = 0; k < 4; k++)
        {
            validColor = 0;
            originalColor = costume->parts[j]->color[k];
            originalColor.a = 0xff;
            for( i = eaSize( (F32***)&palette[0]->color )-1; i >= 0; i-- )
            {
                if (((*palette[0]->color[i])[0] == originalColor.r) && 
                    ((*palette[0]->color[i])[1] == originalColor.g) && 
                    ((*palette[0]->color[i])[2] == originalColor.b) && 
                    originalColor.a == 0xff)
                {
                    validColor = 1;
                    break;
                }
            }
            if (!validColor)
            {
                costume->parts[j]->color[k].r = (*palette[0]->color[0])[0];
                costume->parts[j]->color[k].g = (*palette[0]->color[0])[1];
                costume->parts[j]->color[k].b = (*palette[0]->color[0])[2];
                costume->parts[j]->color[k].a = 0xff;                
                if (badColorList)    
                {
                    eaiPush(badColorList, j+1);
                }
                isGood = 0;
            }
        }
    }
    return isGood;
}
extern MemoryPool seqMp;


// Returns 0 if the costume is OK, 1 if it is bad
// @todo, but also will modify Costume if colors are not valid, i.e. can't test const costume currently
int costume_Validate( Entity *e, Costume * costume, int slotid, int **badPartList, int **badColorList, int isPCC, int allowStoreParts )
{
    int i;
    int bad = 0;
    SeqInst * tmpSeq = 0;

    if( costume->appearance.iNumParts != eaSize(&bodyPartList.bodyParts) )
    {
        Errorf("Appearance num parts mismatch. Alert a programmer");    //    get Edison
        bad |= COSTUME_ERROR_INVALID_NUM_PARTS;
#if SERVER
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Wrong number of parts( %d ): %s", costume->appearance.iNumParts, e->name );
#endif
        if (costume->appearance.iNumParts > eaSize(&bodyPartList.bodyParts))
        {
            return bad;
        }
    }

    if( ABS(costume->appearance.fScale) > 36 )
    {
        bad |= COSTUME_ERROR_INVALIDSCALES;
#if SERVER
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Bad Scale( %f ): %s", costume->appearance.fScale, e->name );
#endif
    }

    for(i=1;i<MAX_BODY_SCALES;i++)
    {
        if( ABS(costume->appearance.fScales[i]) > 1 )
        {
            bad |= COSTUME_ERROR_INVALIDSCALES;
#if SERVER
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, 
                        "costume_Validate Character Creation Failed: Bad Body Scale(fScales[%d] == %f ): %s", i, costume->appearance.fScales[i], e->name );
#endif
        }
    }
    if (e)
    {
        if (!allowStoreParts && e->pl && (costume->appearance.bodytype == kBodyType_Huge) && !AccountCanCreateHugeBody(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            bad |= COSTUME_ERROR_GENDERCHANGE;
#if SERVER
            dbLog( "costume_Validate", e, "Character Creation Failed: Gender Invalid: %s", e->name );        
#endif
        }

        //    This function appears to validate that the ratios for the scales are okay. 
        //    But if there is no entity, how can we make sure this is okay?
        //    Assume it is okay and table this validation until there is an entity.
        if( !costume_ValidateScales( tmpSeq = seqLoadInst( g_BodyTypeInfos[costume->appearance.bodytype].pchEntType, 0, SEQ_LOAD_HEADER_ONLY, e->randSeed, e ), costume->appearance.fBoneScale, costume->appearance.fShoulderScale,
                                    costume->appearance.fChestScale, costume->appearance.fWaistScale, costume->appearance.fHipScale, costume->appearance.fLegScale, costume->appearance.fArmScale) )
        {
            bad |= COSTUME_ERROR_INVALIDSCALES;
    #if SERVER
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Bad Bone Scale: %s", e->name );
    #endif
        }
    }
    if (!costume->parts)
    {
        bad |= COSTUME_ERROR_INVALID_NUM_PARTS;
#if SERVER
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: No Parts %s", e->name );
#endif
        return bad;
    }
    if (eaSize(&costume->parts) != MAX_COSTUME_PARTS)
    {
        bad |= COSTUME_ERROR_INVALID_NUM_PARTS;
#if SERVER
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Invalid Number of Parts: %s", e->name );
#endif
        return bad;
    }

    if ( tmpSeq )
    {
        //mpFree(seqMp, tmpSeq);
        seqFreeInst(tmpSeq);
    }


    {
        // This should be okay since this is only on initial load.
        for(i=0; i < costume->appearance.iNumParts; i++)
        {
            if( !costume_validatePart( costume->appearance.bodytype, i, costume->parts[i], e, isPCC, allowStoreParts, slotid ) )
            {
                if(costume->parts[i]->pchGeom && strstr(costume->parts[i]->pchGeom, "V_MALE_TOP.GEO/GEO_Chest_Baggy"))
                {
                    costume->parts[i]->pchGeom = allocAddString_checked("Baggy");
                    if( costume_validatePart( costume->appearance.bodytype, i, costume->parts[i], e, isPCC, allowStoreParts, slotid ) )
                        continue;
                }

                bad |= COSTUME_ERROR_NOTAWARDED;
#if SERVER
                if(e)
                    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Bad Part( Part: %s Geo: %s Tex1: %s Tex2: %s FX: %s ): %s", bodyPartList.bodyParts[i]->name, costume->parts[i]->pchGeom, costume->parts[i]->pchTex1, costume->parts[i]->pchTex2, costume->parts[i]->pchFxName, e->name );
#else
                if (badPartList)
                {
                    eaiPush(badPartList,i);
                }                
#endif
            }
        }
    }

    if (!costumeCorrectColors(e,costume, badColorList))
    {
        bad |= COSTUME_ERROR_INVALIDCOLORS;
#if SERVER
        if(e)
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Bad Color: %s", e->name );        
#endif
    }

    if (e && e->pchar && !isPCC)
    {
        for(i = eaSize(&e->pchar->ppPowerSets)-1; i >= 0; --i)
        {
            int j;
            PowerSet *pset = e->pchar->ppPowerSets[i];

            if(!pset || !pset->psetBase)
                continue;

            for(j = eaSize(&pset->psetBase->ppchCostumeParts)-1; j >= 0; --j)
            {
                CostumePart *part = costume_getTaggedPart(pset->psetBase->ppchCostumeParts[j], costume->appearance.bodytype);
                int bpId = bpGetIndexFromName(SAFE_MEMBER(part,pchName));

                if(!part || bpId < 0)
                {
                    continue;
                }
                else
                {
                    CostumePart *oldpart = costume->parts[bpId];
                    if(costume_isPartEmpty(oldpart))
                    {
                        bad |= COSTUME_ERROR_BADPART;
#if SERVER
                        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "No Weapon specified for part", e->name );        
#endif
                    }
                }
            }
        }
    }

    return bad;
}


Costume * costume_current( Entity * e )
{
    if (!e || !e->pl)
        return NULL;

    return e->pl->costume[e->pl->current_costume];
}

const cCostume * costume_current_const( Entity * e )
{
    return costume_as_const(costume_current(e));
}

void costume_SGColorsExtract(Entity* e, Costume* costume, SGColorBits* primU, SGColorBits* secU, SGColorBits* prim2U, SGColorBits* sec2U, SGColorBits* threeU, SGColorBits* fourU)
{
    int i, j;
    SGColorBits primaryU = {0}, secondaryU = {0}, primary2U = {0}, secondary2U = {0}, tertiaryU = {0}, quaternaryU = {0};

    for( i = 0; i < costume->appearance.iNumParts && i < MAX_COSTUME_PARTS/2; i++ )
    {
        CostumePart * part = costume->parts[i];

         Color c1 = colorFlip( COLOR_CAST(e->supergroup->colors[0]) );
        Color c2 = colorFlip( COLOR_CAST(e->supergroup->colors[1]) );

        if( part->color[0].integer == c1.integer )
            primaryU.SGBitSet |= (U64)SGC_PRIMARY << (i*2);
        else if( part->color[0].integer == c2.integer )
            primaryU.SGBitSet |= (U64)SGC_SECONDARY << (i * 2);
        else
            primaryU.SGBitSet |= (U64)SGC_DEFAULT << (i * 2);

        if( part->color[1].integer == c1.integer )
            secondaryU.SGBitSet |= (U64)SGC_PRIMARY << (i * 2);
        else if( part->color[1].integer == c2.integer )
            secondaryU.SGBitSet |= (U64)SGC_SECONDARY << (i * 2);
        else
            secondaryU.SGBitSet |= (U64)SGC_DEFAULT << (i * 2);
    }

    for( j = 0; i < costume->appearance.iNumParts; j++, i++ )
    {
        CostumePart * part = costume->parts[i];

        Color c1 = colorFlip( COLOR_CAST(e->supergroup->colors[0]) );
        Color c2 = colorFlip( COLOR_CAST(e->supergroup->colors[1]) );

        if( part->color[0].integer == c1.integer )
            primary2U.SGBitSet |= (U64)SGC_PRIMARY << (j * 2);
        else if( part->color[0].integer == c2.integer )
            primary2U.SGBitSet |= (U64)SGC_SECONDARY << (j * 2);
        else
            primary2U.SGBitSet |= (U64)SGC_DEFAULT << (j * 2);

        if( part->color[1].integer == c1.integer )
            secondary2U.SGBitSet |= (U64)SGC_PRIMARY << (j * 2);
        else if( part->color[1].integer == c2.integer )
            secondary2U.SGBitSet |= (U64)SGC_SECONDARY << (j * 2);
        else
            secondary2U.SGBitSet |= (U64)SGC_DEFAULT << (j * 2);

        if( part->color[2].integer == c1.integer )
            tertiaryU.SGBitSet |= (U64)SGC_PRIMARY << (j * 2);
        else if( part->color[2].integer == c2.integer )
            tertiaryU.SGBitSet |= (U64)SGC_SECONDARY << (j * 2);
        else
            tertiaryU.SGBitSet |= (U64)SGC_DEFAULT << (j * 2);

        if( part->color[3].integer == c1.integer )
            quaternaryU.SGBitSet |= (U64)SGC_PRIMARY << (j * 2);
        else if( part->color[3].integer == c2.integer )
            quaternaryU.SGBitSet |= (U64)SGC_SECONDARY << (j * 2);
        else
            quaternaryU.SGBitSet |= (U64)SGC_DEFAULT << (j * 2);
    }

    *primU = primaryU;
    *secU = secondaryU;
    *prim2U = primary2U;
    *sec2U = secondary2U;
    *threeU = tertiaryU;
    *fourU = quaternaryU;
}

// Extract colors from costume.  And, yes, that declaration of colorArray is correct.  Don't touch it.
void costume_baseColorsExtract( const cCostume *costume, unsigned int colorArray[][4], unsigned int *pSkinColor )
{
    int i;

    for( i = 0; i < MAX_COSTUME_PARTS; ++i )
    {
        const CostumePart *part = costume->parts[i];

        if (i < costume->appearance.iNumParts && !costume_isPartEmpty(part)) //if costume part exists
        {
            colorArray[i][0] = part->color[0].integer;
            colorArray[i][1] = part->color[1].integer;
            colorArray[i][2] = part->color[2].integer;
            colorArray[i][3] = part->color[3].integer;
        }
        else
        {
            //set a default color for unused pieces
            colorArray[i][0] = 0xff1f1f1f;
            colorArray[i][1] = 0xffe3e3e3;
            colorArray[i][2] = 0xff1f1f1f;
            colorArray[i][3] = 0xffe3e3e3;
        }
    }
    if (pSkinColor)
    {
        *pSkinColor = costume->appearance.colorSkin.integer;
    }
}

//modified version of costume_baseColorsExtract to pull out only 2 colors of costume and removes the alpha.
//It also sets the colors of non-existing parts to 0 in colorArray to better pass the did-costume-colors-change
//check (addresses a bug where default costume and costume switched to default from "Only show free costumes"
//selection was failing this check)
void ExtractCostumeColorsForCompareinTailor( const cCostume *costume, unsigned int colorArray[][2], unsigned int *pSkinColor )
{
    int i;

    for( i = 0; i < MAX_COSTUME_PARTS; ++i )
    {
        const CostumePart *part = costume->parts[i];

        if (i < costume->appearance.iNumParts && !costume_isPartEmpty(part)) //if costume part exists
        {
            colorArray[i][0] = part->color[0].integer & 0x00ffffff; //remove alpha
            colorArray[i][1] = part->color[1].integer & 0x00ffffff; //remove alpha
        }
        else
        {
            //default colors set in extract
            colorArray[i][0] = 0x001f1f1f;
            colorArray[i][1] = 0x00e3e3e3;
        }

    }
    if (pSkinColor)
    {
        *pSkinColor = costume->appearance.colorSkin.integer;
    }
}

void costume_SGColorsApplyToCostume(Entity* e, Costume* costume, SGColorBits primU, SGColorBits secU, SGColorBits prim2U, SGColorBits sec2U, SGColorBits threeU, SGColorBits fourU)
{
    int i;

    if( !e->supergroup )
        return;

    for( i = 0; i < costume->appearance.iNumParts; i++ )
    {
        int primary, secondary, tertiary, quaternary;

        if( i < MAX_COSTUME_PARTS/2 )
        {
            primary    = (primU.SGBitSet >> i*2) & 3;
            secondary  = (secU.SGBitSet >> i*2 ) & 3;
            tertiary   = SGC_DEFAULT;
            quaternary = SGC_DEFAULT;
        }
        else
        {
            primary       = (prim2U.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            secondary  = (sec2U.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            tertiary   = (threeU.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            quaternary = (fourU.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
        }

        if( primary == SGC_PRIMARY )
            costume->parts[i]->color[0] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( primary == SGC_SECONDARY )
            costume->parts[i]->color[0] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( secondary == SGC_PRIMARY )
            costume->parts[i]->color[1] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( secondary == SGC_SECONDARY )
            costume->parts[i]->color[1] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( tertiary == SGC_PRIMARY )
            costume->parts[i]->color[2] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( tertiary == SGC_SECONDARY )
            costume->parts[i]->color[2] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( quaternary == SGC_PRIMARY )
            costume->parts[i]->color[3] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( quaternary == SGC_SECONDARY )
            costume->parts[i]->color[3] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));
    }
}

void costume_baseColorsApplyToCostume( Costume *costume, unsigned int colorArray[][4] )
{
    int i;

    for( i = 0; i < costume->appearance.iNumParts && i < MAX_COSTUME_PARTS; i++ )
    {
        CostumePart *part = costume->parts[i];

        part->color[0].integer = colorArray[i][0];
        part->color[1].integer = colorArray[i][1];
        part->color[2].integer = colorArray[i][2];
        part->color[3].integer = colorArray[i][3];
    }
}
void costume_SGColorsApply( Entity *e )
{
    int i;

    if( !e->supergroup )
        return;

    for( i = 0; i < e->pl->superCostume->appearance.iNumParts; i++ )
    {
        int primary, secondary, tertiary, quaternary;

        if( i < MAX_COSTUME_PARTS/2 )
        {
            primary    = (e->pl->superColorsPrimaryU.SGBitSet >> i*2) & 3;
            secondary  = (e->pl->superColorsSecondaryU.SGBitSet >> i*2 ) & 3;
            tertiary   = SGC_DEFAULT;
            quaternary = SGC_DEFAULT;
        }
        else
        {
            primary    = (e->pl->superColorsPrimary2U.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            secondary  = (e->pl->superColorsSecondary2U.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            tertiary   = (e->pl->superColorsTertiaryU.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
            quaternary = (e->pl->superColorsQuaternaryU.SGBitSet >> (i-MAX_COSTUME_PARTS/2)*2 ) & 3;
        }

        if( primary == SGC_PRIMARY )
            e->pl->superCostume->parts[i]->color[0] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( primary == SGC_SECONDARY )
            e->pl->superCostume->parts[i]->color[0] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( secondary == SGC_PRIMARY )
            e->pl->superCostume->parts[i]->color[1] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( secondary == SGC_SECONDARY )
            e->pl->superCostume->parts[i]->color[1] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( tertiary == SGC_PRIMARY )
            e->pl->superCostume->parts[i]->color[2] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( tertiary == SGC_SECONDARY )
            e->pl->superCostume->parts[i]->color[2] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));

        if( quaternary == SGC_PRIMARY )
            e->pl->superCostume->parts[i]->color[3] = colorFlip(COLOR_CAST( e->supergroup->colors[0] ));
        else if( quaternary == SGC_SECONDARY )
            e->pl->superCostume->parts[i]->color[3] = colorFlip(COLOR_CAST( e->supergroup->colors[1] ));
    }
}

Costume * costume_makeSupergroup( Entity *e )
{
    if( e->pl->superCostume )
        costume_destroy( e->pl->superCostume );

    e->pl->superCostume = costume_clone(costume_as_const(e->pl->costume[e->pl->current_costume]));

    if( !e->pl->hide_supergroup_emblem && e->supergroup->emblem[0] )
    {
        U32 index;
        const char* pcKey;

        if( stashFindIndexByKey(costume_HashTable, e->supergroup->emblem, &index) )
        {
            int chest_id = costume_PartIndexFromName( e, "Chest" );
            int detail_id = costume_PartIndexFromName( e, "ChestDetail" );

            // if the chest geometry is not one of the supported ones, use tight
            assert(e->pl->superCostume->parts[chest_id]); // temp asserts for tracking down issue
            assert(e->pl->superCostume->parts[chest_id]->pchGeom);
            if(    stricmp( "jackets", e->pl->superCostume->parts[chest_id]->pchGeom) == 0 )
            {
                if( e->pl->superCostume->parts[detail_id]->pchGeom && stricmp( "none", e->pl->superCostume->parts[detail_id]->pchGeom) != 0 )
                    costume_PartSetGeometry( e->pl->superCostume, detail_id, e->pl->superCostume->parts[detail_id]->pchGeom);
                else
                    costume_PartSetGeometry( e->pl->superCostume, detail_id, "top" );
            }
            else if( stricmp( "tight",    e->pl->superCostume->parts[chest_id]->pchGeom) != 0 &&
                    stricmp( "jacket",    e->pl->superCostume->parts[chest_id]->pchGeom) != 0 &&
                    stricmp( "baggy",    e->pl->superCostume->parts[chest_id]->pchGeom) != 0 &&
                    stricmp( "armored", e->pl->superCostume->parts[chest_id]->pchGeom) != 0 &&
                    stricmp( "robe",    e->pl->superCostume->parts[chest_id]->pchGeom) != 0 &&
                    stricmp( "shirt",    e->pl->superCostume->parts[chest_id]->pchGeom) != 0 )
            {
                costume_PartSetGeometry( e->pl->superCostume, detail_id, "tight");
            }
            else
                costume_PartSetGeometry( e->pl->superCostume, detail_id, e->pl->superCostume->parts[chest_id]->pchGeom);

             costume_PartSetTexture1( e->pl->superCostume, detail_id, "base" );
            stashFindKeyByIndex(costume_HashTable, index, &pcKey);
            costume_PartSetTexture2( e->pl->superCostume, detail_id, pcKey );
            if( !e->pl->superCostume->parts[detail_id]->regionName )
                costume_PartSetPath( e->pl->superCostume, detail_id, bodyPartList.bodyParts[detail_id]->name, e->pl->superCostume->parts[chest_id]->regionName, e->pl->superCostume->parts[chest_id]->bodySetName );
        }
    }

    costume_SGColorsApply( e );
    return e->pl->superCostume;
}

//-------------------------------------------------------------------------------------
// Costume transmission
//-------------------------------------------------------------------------------------
int num_colors_lookup[] = {2,4}; // Sorted by most commonly sent value
int num_colors_inv_lookup[] = {-1,-1,0,-1,1}; // Inverse of above array

static const char* stringcache[30*4];
static int stringcachecount;
static int stringcacheorder;
static const char *getCachedIndexedString(Packet *pak)
{
    int cached = pktGetBits(pak, 1);
    const char *val;
    if (!cached) {
        val = pktGetIndexedString(pak, costume_HashTable);
        stringcache[stringcachecount] = val;
        if (stringcachecount > (1 << stringcacheorder) - 1) {
            stringcacheorder++;
        }
        stringcachecount++;
        assert(stringcachecount < ARRAY_SIZE(stringcache));
    } else {
        if (stringcachecount>1) {
            int index = pktGetBits(pak, stringcacheorder);
            val = stringcache[index];
        } else {
            val = stringcache[0];
        }
    }
    return val;
}

static void sendCachedIndexedString(Packet *pak, const char *val)
{
    int k;
    int cached=-1;
    if (!val)
        val = "None";
    for (k=0; k<stringcachecount; k++) {
        if (stricmp(stringcache[k], val)==0) {
            cached = k;
            break;
        }
    }
    if (cached==-1) {
        pktSendBits(pak, 1, 0);
        pktSendIndexedString(pak, val, costume_HashTable);
        stringcache[stringcachecount] = val;
        if (stringcachecount > (1 << stringcacheorder) - 1) {
            stringcacheorder++;
        }
        stringcachecount++;
        assert(stringcachecount < ARRAY_SIZE(stringcache));
    } else {
        pktSendBits(pak, 1, 1);
        if (stringcachecount>1) {
            pktSendBits(pak, stringcacheorder, k);
        } else {
            // Don't need to send anything, there's only 1 thing in the stringcache!
        }
    }
}

static int colorcache[30*4];
static int colorcachecount;
static int colorcacheorder;
static int getCachedIndexedColor(Packet *pak)
{
    int cached = pktGetBits(pak, 1);
    int val;
    if (!cached) {
        val = pktGetIndexedGeneric(pak, costume_PaletteTable);
        colorcache[colorcachecount] = val;
        if (colorcachecount > (1 << colorcacheorder) - 1) {
            colorcacheorder++;
        }
        colorcachecount++;
        assert(colorcachecount < ARRAY_SIZE(colorcache));
    } else {
        if (colorcachecount>1) {
            int index = pktGetBits(pak, colorcacheorder);
            val = colorcache[index];
        } else {
            val = colorcache[0];
        }
    }
    return val;
}

static void sendCachedIndexedColor(Packet *pak, int val)
{
    int k;
    int cached=-1;
    for (k=0; k<colorcachecount; k++) {
        if (colorcache[k] == val) {
            cached = k;
            break;
        }
    }
    if (cached==-1) {
        pktSendBits(pak, 1, 0);
        pktSendIndexedGeneric(pak, val, costume_PaletteTable);
        colorcache[colorcachecount] = val;
        if (colorcachecount > (1 << colorcacheorder) - 1) {
            colorcacheorder++;
        }
        colorcachecount++;
        assert(colorcachecount < ARRAY_SIZE(colorcache));
    } else {
        pktSendBits(pak, 1, 1);
        if (colorcachecount>1) {
            pktSendBits(pak, colorcacheorder, k);
        } else {
            // Don't need to send anything, there's only 1 thing in the colorcache!
        }
    }
}
int NPCcostume_receive(Packet *pak, CustomNPCCostume* costume)
{
    int i, j, count;

    StructInitFields(ParseCustomNPCCostume, costume);

    costume->skinColor.integer    = pktGetBits(pak, 32);
    count = pktGetBits(pak, 6);
    for(i=0; i < count; i++)
    {
        eaPush(&costume->parts, StructAllocRaw(sizeof(CustomNPCCostumePart)));
        for (j=0; j<2; j++) {
                costume->parts[i]->color[j].integer = pktGetBits(pak, 32);
        }
        costume->parts[i]->regionName = allocAddString(pktGetString(pak));
    }

    return 1;
}



void NPCcostume_send(Packet* pak, CustomNPCCostume * costume)
{
    int i, j, count;
    count = eaSize(&costume->parts);
    pktSendBits(pak, 32, costume->skinColor.integer);
    pktSendBits(pak, 6, count);

    for(i=0; i<count; i++)
    {
        for (j=0; j<2; j++) {
            pktSendBits(pak, 32, costume->parts[i]->color[j].integer);
        }
        pktSendString(pak, costume->parts[i]->regionName);
    }
}

int costume_receive(Packet *pak, Costume* costume)
{
    int i, j, count;
    bool hasExtraInfo;

    // Reset the caches
    colorcachecount=0;
    colorcacheorder=0;
    stringcachecount=0;
    stringcacheorder=0;

    StructInitFields(ParseCostume, costume);

    if (pktGetBits(pak, 1))
        costume->appearance.entTypeFile = getCachedIndexedString(pak);

    if (pktGetBits(pak, 1))
        costume->appearance.costumeFilePrefix = getCachedIndexedString(pak);

    costume->appearance.bodytype            = pktGetBitsPack(pak, 6);
    costume->appearance.colorSkin.integer    = pktGetBits(pak, 32);
    costume->appearance.convertedScale        = pktGetBits(pak,1);
    costume->appearance.currentSuperColorSet = pktGetBits(pak, 3);
    hasExtraInfo                            = pktGetBits( pak, 1 );

    for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
    {
        costume->appearance.superColorsPrimaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsPrimaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume->appearance.superColorsSecondaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsSecondaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume->appearance.superColorsPrimary2U[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsPrimary2U[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume->appearance.superColorsSecondary2U[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsSecondary2U[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume->appearance.superColorsTertiaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsTertiaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume->appearance.superColorsQuaternaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume->appearance.superColorsQuaternaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
    }


    for(i=0;i<MAX_BODY_SCALES;i++) {
        if (i==0 || hasExtraInfo) {
            costume->appearance.fScales[i]    = pktGetF32(pak);
        } else {
            costume->appearance.fScales[i]    = pktGetF(pak, 6, -1, 1);
        }
    }

    if (pktGetBits(pak, 1)) {
        costume->appearance.iNumParts        = pktGetBitsPack(pak, 4);
    } else {
        costume->appearance.iNumParts        = GetBodyPartCount();
    }

    if (costume->appearance.bodytype >= getBodyTypeCount())
    {
#if SERVER
        LOG_ENT(NULL, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Invalid body type( %d )", costume->appearance.bodytype );
#endif
        return 0;
    }

    if( costume->appearance.iNumParts > GetBodyPartCount() )
    {
#if SERVER
        LOG_ENT(NULL, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "costume_Validate Character Creation Failed: Wrong too many parts( %d )", costume->appearance.iNumParts );
#endif
        return 0;
    }

     for(i=0; i < costume->appearance.iNumParts; i++)
    {
        costume->parts[i]->pchGeom = getCachedIndexedString(pak);
        costume->parts[i]->pchTex1 = getCachedIndexedString(pak);
        costume->parts[i]->pchTex2 = getCachedIndexedString(pak);
        costume->parts[i]->pchFxName = getCachedIndexedString(pak);

        count = num_colors_lookup[pktGetBits(pak, 1)];
        for (j=0; j<ARRAY_SIZE(costume->parts[i]->color); j++) {
            if (j < count)
                costume->parts[i]->color[j].integer = getCachedIndexedColor(pak);
            else
                costume->parts[i]->color[j].integer = 0;
        }

        if( hasExtraInfo )
        {
            costume->parts[i]->displayName = "None";
//            costume->parts[i]->displayName = (char*)pktGetIndexedString(pak, costume_HashTableExtraInfo);
            costume->parts[i]->regionName = pktGetIndexedString(pak, costume_HashTableExtraInfo);
            costume->parts[i]->bodySetName = pktGetIndexedString(pak, costume_HashTableExtraInfo);
        }
    }

    return 1;
}



void costume_send(Packet* pak, Costume * costume, int send_names)
{
    int i, j, count;
    bool sendExtraInfo;

    // Reset the caches
    colorcachecount=0;
    colorcacheorder=0;
    stringcachecount=0;
    stringcacheorder=0;

    pktSendBits(pak, 1, costume->appearance.entTypeFile != NULL);

    if (costume->appearance.entTypeFile)
        sendCachedIndexedString(pak, costume->appearance.entTypeFile);

    pktSendBits(pak, 1, costume->appearance.costumeFilePrefix != NULL);

    if (costume->appearance.costumeFilePrefix)
        sendCachedIndexedString(pak, costume->appearance.costumeFilePrefix);

    pktSendBitsPack(pak, 6, costume->appearance.bodytype);
    pktSendBits(pak, 32, costume->appearance.colorSkin.integer);
    pktSendBits(pak, 1, costume->appearance.convertedScale );
    pktSendBits(pak, 3, costume->appearance.currentSuperColorSet);

    sendExtraInfo = send_names;
    pktSendBits(pak, 1, sendExtraInfo);

    for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
    {
        pktSendBits(pak, 32, costume->appearance.superColorsPrimaryU[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsPrimaryU[i].SGBitSetHigh);
        pktSendBits(pak, 32, costume->appearance.superColorsSecondaryU[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsSecondaryU[i].SGBitSetHigh);
        pktSendBits(pak, 32, costume->appearance.superColorsPrimary2U[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsPrimary2U[i].SGBitSetHigh);
        pktSendBits(pak, 32, costume->appearance.superColorsSecondary2U[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsSecondary2U[i].SGBitSetHigh);
        pktSendBits(pak, 32, costume->appearance.superColorsTertiaryU[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsTertiaryU[i].SGBitSetHigh);
        pktSendBits(pak, 32, costume->appearance.superColorsQuaternaryU[i].SGBitSetLow);
        pktSendBits(pak, 32, costume->appearance.superColorsQuaternaryU[i].SGBitSetHigh);
    }

    for(i=0;i<MAX_BODY_SCALES;i++) {
        if (i==0 || sendExtraInfo) {
            pktSendF32(pak, costume->appearance.fScales[i]);
        } else {
            pktSendF(pak, 6, -1, 1, costume->appearance.fScales[i]);
        }
    }

    if (costume->appearance.iNumParts == GetBodyPartCount()) {
        pktSendBits(pak, 1,0 );
    } else {
        pktSendBits(pak, 1,1 );
        pktSendBitsPack(pak, 4, costume->appearance.iNumParts);
    }

    for(i=0; i<costume->appearance.iNumParts; i++)
    {
        sendCachedIndexedString(pak, costume->parts[i]->pchGeom);
        sendCachedIndexedString(pak, costume->parts[i]->pchTex1);
        sendCachedIndexedString(pak, costume->parts[i]->pchTex2);
        sendCachedIndexedString(pak, costume->parts[i]->pchFxName);

        if (isNullOrNone(costume->parts[i]->pchFxName))
            count = 2;
        else
            count = 4;
        pktSendBits(pak, 1, num_colors_inv_lookup[count]); // Send 2 and 0, 4 as 1
        for (j=0; j<count; j++) {
            sendCachedIndexedColor(pak, costume->parts[i]->color[j].integer);
        }

        if( sendExtraInfo )
        {
//            pktSendIndexedString( pak, costume->parts[i]->displayName, costume_HashTableExtraInfo);
            pktSendIndexedString( pak, costume->parts[i]->regionName, costume_HashTableExtraInfo);
            pktSendIndexedString( pak, costume->parts[i]->bodySetName, costume_HashTableExtraInfo);
        }
    }
}

int costume_change( Entity *e, int num )
{
    int max_costumes = 0;

    if(e->pl->npc_costume != 0)
    {
#if SERVER
        chatSendToPlayer(e->db_id, localizedPrintf(e, "CostumeCannotChange" ), INFO_USER_ERROR, 0);
#elif CLIENT
        addSystemChatMsg( textStd( "CostumeCannotChange" ), INFO_USER_ERROR, 0);
#endif
        return 0;
    }

    if( e->pl->current_costume == num )
        return 1;

    max_costumes = costume_get_max_slots( e );

    if( num >= max_costumes || num < 0 )
    {
        // send error message
        return 0;
    }

    if( num >= MAX(costume_get_num_slots(e), e->pl->num_costumes_stored) )
    {
        // send error message
        return 0;
    }

#if SERVER // note wait on server is significantly less than client wait

    if( e->access_level < 9 && (e->pl->last_costume_change && (dbSecondsSince2000() - e->pl->last_costume_change) < CC_TIMEOUT_SERVER ))
    {
        // send error message

        chatSendToPlayer(e->db_id, localizedPrintf( e,"CostumeWait", 30-(dbSecondsSince2000() - e->pl->last_costume_change) ), INFO_USER_ERROR, 0);
        return 0;
    }

    e->pl->last_costume_change = dbSecondsSince2000();

#elif CLIENT

    if( e->access_level < 9 && (e->pl->last_costume_change && (timerCpuSeconds() - e->pl->last_costume_change) < CC_TIMEOUT_CLIENT ))
    {
        // send error message

        addSystemChatMsg( textStd( "CostumeWait", 30-(timerCpuSeconds() - e->pl->last_costume_change) ), INFO_USER_ERROR, 0);
        return 0;
    }

    e->pl->last_costume_change = timerCpuSeconds();

#endif


    e->pl->current_costume = num;
    e->pl->current_powerCust = num;
    if( e->pl->costume[num]->appearance.bodytype == kBodyType_Female || e->pl->costume[num]->appearance.bodytype == kBodyType_BasicFemale )
        e->name_gender = e->gender = GENDER_FEMALE;
    else
        e->name_gender = e->gender = GENDER_MALE;

#if SERVER
    e->send_all_costume = true;
    e->send_costume = true;
    e->updated_powerCust = true;
    e->send_all_powerCust = true;
    // send message
#endif

    return 1;
}

#if CLIENT
int costume_change_emote_check( Entity *e, int num )
{
    if(e->pl->npc_costume != 0)
    {
        addSystemChatMsg( textStd( "CostumeCannotChange" ), INFO_USER_ERROR, 0);
        return 0;
    }

    if( e->pl->current_costume == num )
        return 1;

    if( num >= MAX_COSTUMES || num < 0 )
    {
        // send error message
        return 0;
    }

    if( num > costume_get_num_slots(e) )
    {
        // send error message
        return 0;
    }

    if( e->access_level < 9 && (e->pl->last_costume_change && (timerCpuSeconds() - e->pl->last_costume_change) < CC_TIMEOUT_CLIENT ))
    {
        // send error message
        addSystemChatMsg( textStd( "CostumeWait", 30-(timerCpuSeconds() - e->pl->last_costume_change) ), INFO_USER_ERROR, 0);
        return 0;
    }

    e->pl->last_costume_change = timerCpuSeconds();
    return 1;
}
#endif

//-------------------------------------------------------------------------------------
// Costume color pallette
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Costume definiton loading
//-------------------------------------------------------------------------------------

// Costume list for NPCs and villains
typedef struct{
    Costume** costumes;
} CostumeList;
CostumeList costumeList;
Costume** costumes = NULL;


TokenizerParseInfo ParseCostumePart[] =
{
    {    "Name",            TOK_STRUCTPARAM|TOK_POOL_STRING|TOK_STRING(CostumePart, pchName, 0)        },
    {    "SourceFile",    TOK_CURRENTFILE(CostumePart,sourceFile)                                    },

    {    "Fx",            TOK_POOL_STRING|TOK_STRING(CostumePart, pchFxName, 0 )                    },
    {    "FxName",        TOK_POOL_STRING|TOK_REDUNDANTNAME|TOK_STRING(CostumePart, pchFxName, 0)    },
    {    "Geometry",        TOK_POOL_STRING|TOK_STRING(CostumePart, pchGeom, 0)                        },
    {    "Texture1",        TOK_POOL_STRING|TOK_STRING(CostumePart, pchTex1, 0)                        },
    {    "Texture2",        TOK_POOL_STRING|TOK_STRING(CostumePart, pchTex2, 0)                        },

    {    "DisplayName",    TOK_POOL_STRING|TOK_STRING(CostumePart, displayName,0)                    },
    {    "RegionName",    TOK_POOL_STRING|TOK_STRING(CostumePart, regionName,0 )                    },
    {    "BodySetName",    TOK_POOL_STRING|TOK_STRING(CostumePart, bodySetName,0)                    },

    {    "CostumeNum",    TOK_INT(CostumePart,costumeNum, 0)                                        },
    {    "Color1",        TOK_RGB(CostumePart, color[0].rgb)                                        },
    {    "Color2",        TOK_RGB(CostumePart, color[1].rgb)                                        },
    {    "Color3",        TOK_RGB(CostumePart, color[2].rgb)                                        },
    {    "Color4",        TOK_RGB(CostumePart, color[3].rgb)                                        },

    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseCostumePartDiff[] = 
{
    {    "PartIndex",    TOK_INT(CostumePartDiff, index, 0)                                        },
    {    "CostumePart",    TOK_OPTIONALSTRUCT(CostumePartDiff, part, ParseCostumePart)                },
    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseCostume[] =
{
    {    "EntTypeFile",            TOK_POOL_STRING|TOK_STRING(Costume, appearance.entTypeFile, 0)            },
    {    "CostumeFilePrefix",    TOK_POOL_STRING|TOK_STRING(Costume, appearance.costumeFilePrefix, 0)    },

    {    "Scale",                TOK_F32(Costume, appearance.fScale, 0)                                    },
    {    "BoneScale",            TOK_F32(Costume, appearance.fBoneScale, 0)                                },
    {    "HeadScale",            TOK_F32(Costume, appearance.fHeadScale, 0)                                },
    {    "ShoulderScale",        TOK_F32(Costume, appearance.fShoulderScale, 0)                            },
    {    "ChestScale",            TOK_F32(Costume, appearance.fChestScale, 0)                                },
    {    "WaistScale",            TOK_F32(Costume, appearance.fWaistScale, 0)                                },
    {    "HipScale",                TOK_F32(Costume, appearance.fHipScale, 0)                                },
    {    "LegScale",                TOK_F32(Costume, appearance.fLegScale, 0)                                },
    {    "ArmScale",                TOK_F32(Costume, appearance.fArmScale, 0)                                },

    {    "HeadScales",            TOK_VEC3(Costume, appearance.fHeadScales)                                },
    {    "BrowScales",            TOK_VEC3(Costume, appearance.fBrowScales)                                },
    {    "CheekScales",            TOK_VEC3(Costume, appearance.fCheekScales)                                },
    {    "ChinScales",            TOK_VEC3(Costume, appearance.fChinScales)                                },
    {    "CraniumScales",        TOK_VEC3(Costume, appearance.fCraniumScales)                            },
    {    "JawScales",            TOK_VEC3(Costume, appearance.fJawScales)                                },
    {    "NoseScales",            TOK_VEC3(Costume, appearance.fNoseScales)                                },

    {    "SkinColor",            TOK_RGB(Costume, appearance.colorSkin.rgb)                                },
    {    "NumParts",                TOK_INT(Costume, appearance.iNumParts, 0)                                },
    {    "BodyType",                TOK_INT(Costume, appearance.bodytype, 0)                                },
    {    "CostumePart",            TOK_STRUCT(Costume, parts, ParseCostumePart)                            },
    {    "{",                    TOK_START                                                                },
    {    "}",                    TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseCostumeDiff[] =
{
    {    "EntTypeFile",            TOK_POOL_STRING|TOK_STRING(CostumeDiff, appearance.entTypeFile, 0)            },
    {    "CostumeFilePrefix",    TOK_POOL_STRING|TOK_STRING(CostumeDiff, appearance.costumeFilePrefix, 0)    },

    {    "Scale",                TOK_F32(CostumeDiff, appearance.fScale, 0)                                    },
    {    "BoneScale",            TOK_F32(CostumeDiff, appearance.fBoneScale, 0)                                },
    {    "HeadScale",            TOK_F32(CostumeDiff, appearance.fHeadScale, 0)                                },
    {    "ShoulderScale",        TOK_F32(CostumeDiff, appearance.fShoulderScale, 0)                            },
    {    "ChestScale",            TOK_F32(CostumeDiff, appearance.fChestScale, 0)                                },
    {    "WaistScale",            TOK_F32(CostumeDiff, appearance.fWaistScale, 0)                                },
    {    "HipScale",                TOK_F32(CostumeDiff, appearance.fHipScale, 0)                                },
    {    "LegScale",                TOK_F32(CostumeDiff, appearance.fLegScale, 0)                                },
    {    "ArmScale",                TOK_F32(CostumeDiff, appearance.fArmScale, 0)                                },

    {    "HeadScales",            TOK_VEC3(CostumeDiff, appearance.fHeadScales)                                },
    {    "BrowScales",            TOK_VEC3(CostumeDiff, appearance.fBrowScales)                                },
    {    "CheekScales",            TOK_VEC3(CostumeDiff, appearance.fCheekScales)                                },
    {    "ChinScales",            TOK_VEC3(CostumeDiff, appearance.fChinScales)                                },
    {    "CraniumScales",        TOK_VEC3(CostumeDiff, appearance.fCraniumScales)                            },
    {    "JawScales",            TOK_VEC3(CostumeDiff, appearance.fJawScales)                                },
    {    "NoseScales",            TOK_VEC3(CostumeDiff, appearance.fNoseScales)                                },

    {    "SkinColor",            TOK_RGB(CostumeDiff, appearance.colorSkin.rgb)                                },
    {    "NumParts",                TOK_INT(CostumeDiff, appearance.iNumParts, 0)                                },
    {    "BodyType",                TOK_INT(CostumeDiff, appearance.bodytype, 0)                                },
    {    "BaseCostumeNum",        TOK_INT(CostumeDiff, costumeBaseNum, 0)                                        },
    {    "DiffCostumePart",        TOK_STRUCT(CostumeDiff, differentParts, ParseCostumePartDiff)                    },
    {    "{",                    TOK_START                                                                },
    {    "}",                    TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseCostumeBegin[] =
{
    {    "Costume",        TOK_STRUCT(CostumeList, costumes, ParseCostume)    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseCustomNPCCostumePart[] =
{
    {    "Region",        TOK_POOL_STRING|TOK_STRING(CustomNPCCostumePart, regionName, 0)        },
    {    "Color1",        TOK_RGB(CustomNPCCostumePart, color[0].rgb)                                        },
    {    "Color2",        TOK_RGB(CustomNPCCostumePart, color[1].rgb)                                        },
    {    "{",            TOK_START                                                },
    {    "}",            TOK_END                                                    },
    {    "", 0, 0    }
};

TokenizerParseInfo ParseCustomNPCCostume[] =
{
    {    "OriginalNPCName",    TOK_STRING(CustomNPCCostume, originalNPCName, 0)    },
    {    "SkinColor",    TOK_RGB(CustomNPCCostume, skinColor.rgb)                },
    {    "CostumeParts",    TOK_STRUCT(CustomNPCCostume, parts, ParseCustomNPCCostumePart)    },
    {    "{",            TOK_START                                                },
    {    "}",            TOK_END                                                    },
    {    "", 0, 0}
};

BodyType GetBodyTypeForEntType(const char *pch)
{
    int j;
    int prefix_len = (int)strcspn(pch,"_");

    for(j=0; j<getBodyTypeCount(); j++)
    {
        if(strnicmp(pch, g_BodyTypeInfos[j].pchEntType, prefix_len) == 0)
        {
            return g_BodyTypeInfos[j].bodytype;
        }
    }

    return kBodyType_Male;
}

BodyTypeInfo* GetBodyTypeInfo(const char* type)
{
    return &g_BodyTypeInfos[GetBodyTypeForEntType(type)];
}

void costume_WriteTextFile(Costume* costume, char* filename)
{
    CostumeList tempList;
    eaCreate(&tempList.costumes);
    eaPush(&tempList.costumes, costume);

    ParserWriteTextFile(filename, ParseCostumeBegin, &tempList, 0, 0);

    eaDestroy(&tempList.costumes);
}



//-------------------------------------------------------------------------------------
// Conversion utilities for compressing 3 floating point numbers to a 32 bit integer (only uses 27 bits)
//    Compression assumes floating point values are from -1 to 1, and rounds to nearest hundredth
//    The compression is setup so that a zero value will decompress to no scaling
//-------------------------------------------------------------------------------------




static int compressScale(Vec3 scale)
{
    int i,val=0;
    int s[3];

    for(i=0;i<3;i++)
    {
        F32 t = MINMAX(scale[i], -1, 1);
        s[i] = (int)(t * 100);
        val |= ((ABS(s[i]) & 0x7F) << (i * 8));
        if(s[i] < 0)
            val |= ((1 << 7) << (i * 8));        // 8th bit stores sign
    }

    return val;
}



static void decompressScale(int val, Vec3 scale)
{
    int i;
    for(i=0;i<3;i++)
    {
        int temp = (val >> (i * 8));
        scale[i] = (F32)(temp & 0x7F);
        if(temp & (1 << 7))
            scale[i] = - scale[i];
        scale[i] /= 100.0f;
        scale[i] = MINMAX(scale[i], -1, 1);
    }
}

static void compressionTest()
{
    Vec3 vec;
    int val;

    setVec3(vec, 0, 0 ,0);
    val = compressScale(vec);
    decompressScale(val, vec);

    setVec3(vec, 0.5, 0.5 ,0.5);
    val = compressScale(vec);
    decompressScale(val, vec);

    setVec3(vec, 1, 1 , 1);
    val = compressScale(vec);
    decompressScale(val, vec);

    setVec3(vec, -1, -1 ,-1);
    val = compressScale(vec);
    decompressScale(val, vec);

    setVec3(vec, -.75, 0, .05);
    val = compressScale(vec);
    decompressScale(val, vec);
}

void compressAppearanceScales(Appearance * a)
{
    int i;
    for(i=0;i<NUM_3D_BODY_SCALES;i++)
        a->compressedScales[i] = compressScale(a->f3DScales[i]);
}

void decompressAppearanceScales(Appearance * a)
{
    int i;
    for(i=0;i<NUM_3D_BODY_SCALES;i++)
        decompressScale(a->compressedScales[i], a->f3DScales[i]);
}





//-------------------------------------------------------------------------------------
// Temp home for bodyName()
//-------------------------------------------------------------------------------------

BodyTypeInfo g_BodyTypeInfos[] =
{
    // Make sure the kBodyType_... enums match the index in this list
    { kBodyType_Male         /* = 0 */,      "male",    "SM" },    // Default body type
    { kBodyType_Female       /* = 1 */,      "fem",     "SF" },
    { kBodyType_BasicMale    /* = 2 */,      "bm",      "BM" },
    { kBodyType_BasicFemale  /* = 3 */,      "bf",      "BF" },
    { kBodyType_Huge         /* = 4 */,      "huge",    "SH" },
    { kBodyType_Enemy        /* = 5 */,      "enemy",   "EY" },
    { kBodyType_Villain      /* = 6 */,      "enemy",    "EY" },
    { kBodyType_Enemy2       /* = 6 */,     "enemy2",    "EY" },
    { kBodyType_Enemy3       /* = 6 */,     "enemy3",    "EY" },
};

//
//
int getBodyTypeCount()
{
    return sizeof(g_BodyTypeInfos)/sizeof(BodyTypeInfo);
}


const char *entTypeFileName( const Entity *pe )
{
    char *pchRet = NULL;

    if(pe->costume->appearance.entTypeFile)
    {
        // This is done in postprocessing when loading the data!
        assert(pe->costume->appearance.bodytype == GetBodyTypeForEntType(pe->costume->appearance.entTypeFile));
        return pe->costume->appearance.entTypeFile;
    }

    if( pe->costume->appearance.bodytype != kBodyType_Villain )
        pchRet = g_BodyTypeInfos[pe->costume->appearance.bodytype].pchEntType;

    return pchRet;
}


const char *costumePrefixName( const Appearance *appearance)
{
    char *pchRet = NULL;

    if(appearance->costumeFilePrefix)
        return appearance->costumeFilePrefix;

    //This is very strange.  Why is it pchEntType not texname?
    //This function is used for texture and geometry prefixes, but villains seem to use
    //different texture prefixes and geometry prefixes
    if( appearance->bodytype != kBodyType_Villain )
        pchRet = g_BodyTypeInfos[appearance->bodytype].pchEntType;

    return pchRet;
}

// Tailored costume stuff - needed for data validation
//-----------------------------------------------------------------------------------

bool costume_changedScale( const cCostume * c1, const cCostume * c2 )
{
    int i;
    for( i = 1; i < MAX_BODY_SCALES; i++ )
    {
        if( ABS(c1->appearance.fScales[i]-c2->appearance.fScales[i]) > .001f )
            return true;
    }
    return false;
}

int costume_isDifference( const cCostume * c1, const cCostume * c2 )
{
    int i,j;

    if( !c1 || !c2 )
        return 0;

     if( c1->appearance.colorSkin.integer != c2->appearance.colorSkin.integer )
        return 1;
    
    for( i = 0; i < c1->appearance.iNumParts; i++ )
    {
        if( stricmp( c1->parts[i]->pchGeom, c2->parts[i]->pchGeom ) )
            return 1;
        if( stricmp( c1->parts[i]->pchTex1, c2->parts[i]->pchTex1 ) )
            return 1;
        if( strnicmp( c1->parts[i]->pchTex2, c2->parts[i]->pchTex2, strlen(c1->parts[i]->pchTex2) ) )
            return 1;
         if( isNullOrNone(c1->parts[i]->pchFxName) != isNullOrNone(c2->parts[i]->pchFxName) ||
            !isNullOrNone(c1->parts[i]->pchFxName) && stricmp( c1->parts[i]->pchFxName, c2->parts[i]->pchFxName )!=0 )
            return 1;

         if( stricmp(c1->parts[i]->pchGeom, "None") != 0  || !isNullOrNone(c1->parts[i]->pchFxName) )
        {
            for( j = 0; j < 4; j++)
            {
                if( j >= 2 && strnicmp( c1->parts[i]->pchTex2, "miniskirt", 9 ) == 0 )
                    continue;
                if( !colorEqualsIgnoreAlpha( c1->parts[i]->color[j], c2->parts[i]->color[j] ) )
                    return 1;
                if( !colorEqualsIgnoreAlpha( c1->parts[i]->color[j], c2->parts[i]->color[j] ) )
                    return 1;
            }
        }
    }

    if( costume_changedScale(c1,c2) )
        return 1;

    return 0;
}

const TailorCost * tailor_setCurrent( Entity *e )
{
    //obtain player's maximum potential level regardless of build
    //player is currently using to properly set tailor's prices
    int playerMaxLevel = character_GetExperienceLevelExtern(e->pchar);
    // if current tailor isn't set or not right level
    if( !gTailorMaster_curr_cost ||
        playerMaxLevel > gTailorMaster_curr_cost->max_level ||
         playerMaxLevel < gTailorMaster_curr_cost->min_level )
    {
        int i;
        for( i = eaSize(&gTailorMaster.tailor_cost)-1; i >= 0; i-- )
        {
            // find a new tailor cost
            if(    playerMaxLevel >= gTailorMaster.tailor_cost[i]->min_level &&
                playerMaxLevel <= gTailorMaster.tailor_cost[i]->max_level )
            {
                gTailorMaster_curr_cost = gTailorMaster.tailor_cost[i];
                return gTailorMaster_curr_cost;
            }
        }

        // no tailor found
        return NULL;
    }
    else
        return gTailorMaster_curr_cost; //current tailor is fine
}

const TailorCost * tailor_setNext( Entity *e )
{
    // if next tailor isn't set or not right level
    int i, bestMatch = -1, bestDiff = 100000;

    if( !tailor_setCurrent(e) )
        return NULL;

    for( i = eaSize(&gTailorMaster.tailor_cost)-1; i >= 0; i-- )
    {
        // find a new tailor cost
        if(    gTailorMaster_curr_cost->max_level < gTailorMaster.tailor_cost[i]->min_level )
        {
            if( bestDiff > gTailorMaster.tailor_cost[i]->min_level-gTailorMaster_curr_cost->max_level )
            {
                bestDiff = gTailorMaster.tailor_cost[i]->min_level-gTailorMaster_curr_cost->max_level;
                bestMatch = i;
            }
        }
    }

    if( bestDiff == 100000 || bestMatch == -1  )
        gTailorMaster_next_cost = gTailorMaster_curr_cost;
    else
        gTailorMaster_next_cost = gTailorMaster.tailor_cost[bestMatch];

    return gTailorMaster_next_cost;
}


int tailor_getRegionCost( Entity *e, const char *regionName )
{
    const TailorCost *cur = tailor_setCurrent(e);
    const TailorCost *next = tailor_setNext(e);
    float lvlScale;
    if( !cur || !next )
        return 0;

    if( next == cur )
        lvlScale = 0;
    else
        lvlScale = (float)(character_GetExperienceLevelExtern(e->pchar)-cur->min_level)/(next->min_level-cur->min_level);

    if( stricmp( regionName, "Head" ) == 0 )
    {
        return cur->head + (next->head-cur->head)*lvlScale;
    }
    else if ( stricmp( regionName, "Upper Body" ) == 0 ||
              stricmp( regionName, "Capes") == 0 ||
              stricmp( regionName, "Special" ) == 0 ||
              stricmp( regionName, "Weapons" ) == 0 ||
              stricmp( regionName, "Epic Archetype" ) == 0 ||
              stricmp( regionName, "Shields" ) == 0 )
    {
        return cur->upper + (next->upper-cur->upper)*lvlScale;
    }
    else if ( stricmp( regionName, "Lower Body" ) == 0 )
    {
        return cur->lower + (next->lower-cur->lower)*lvlScale;
    }
    else
    {
        return 0;
    }
}

int tailor_getSubItemCost( Entity *e, const CostumePart *cpart )
{
    const TailorCost *cur = tailor_setCurrent(e);
    const TailorCost *next = tailor_setNext(e);
    float lvlScale;

    if( !cur || !next )
        return 0;

    if( next == cur )
        lvlScale = 0;
    else
        lvlScale = (float)(character_GetExperienceLevelExtern(e->pchar)-cur->min_level)/(next->min_level-cur->min_level);

    if( stricmp( cpart->regionName, "Head" ) == 0 )
    {
        return cur->head_subitem + (next->head_subitem-cur->head_subitem)*lvlScale;
    }
    else if ( stricmp( cpart->regionName, "Upper Body" ) == 0 ||
              stricmp( cpart->regionName, "Capes" ) == 0 ||
              stricmp( cpart->regionName, "Special" ) == 0 ||
              stricmp( cpart->regionName, "Weapons" ) == 0 ||
              stricmp( cpart->regionName, "Epic Archetype")  == 0 ||
              stricmp( cpart->regionName, "Shields" ) == 0 )
    {
        return cur->upper_subitem + (next->upper_subitem-cur->upper_subitem)*lvlScale;
    }
    else if ( stricmp( cpart->regionName, "Lower Body" ) == 0 )
    {
        return cur->lower_subitem + (next->lower_subitem-cur->lower_subitem)*lvlScale;
    }
    else
    {
        return 0;
    }
}

int tailor_getFee( Entity *e )
{
    const TailorCost *cur = tailor_setCurrent(e);
    const TailorCost *next = tailor_setNext(e);
    float lvlScale;

    if( !cur || !next )
        return 0;

    if( next == cur )
        lvlScale = 0;
    else
        lvlScale = (float)(character_GetExperienceLevelExtern(e->pchar)-cur->min_level)/(next->min_level-cur->min_level);

      return cur->entryfee + (next->entryfee-cur->entryfee)*lvlScale;
}

bool isNullOrNone(const char *str)
{
    return !str || !str[0] || stricmp(str,"None")==0;
}

bool costume_isPartEmpty(const CostumePart *part)
{
    return    !part ||
            ( isNullOrNone(part->pchGeom) && isNullOrNone(part->pchTex1) &&
              isNullOrNone(part->pchTex2) && isNullOrNone(part->pchFxName) );
}

// Looks at one costume part and figures out what has changed, returns total cost of change for that part
//
int tailor_CalcPartCost( Entity *e, const CostumePart *cpart1, const CostumePart *cpart2 )
{
    int cost = 0;
     int baseCost = tailor_getSubItemCost( e, cpart1 );

    // geometry
      if( !cpart1->pchGeom || !cpart2->pchGeom || stricmp( cpart1->pchGeom, cpart2->pchGeom ) != 0 )
        cost = baseCost;

    // texture change
    if( !cpart1->pchTex1 || !cpart2->pchTex1 || stricmp( cpart1->pchTex1, cpart2->pchTex1 ) != 0 )
        cost = baseCost;

    if( !cpart1->pchTex2 || !cpart2->pchTex2 || strnicmp( cpart1->pchTex2, "miniskirt", 9) == 0 )
    {
        if( !cpart1->pchTex2 || !cpart2->pchTex2 || strnicmp( cpart1->pchTex2, cpart2->pchTex2, MIN(strlen(cpart1->pchTex2),strlen(cpart2->pchTex2)) ) != 0 )
            cost = baseCost;
    }
    else
    {
        // texture 2 changed (the mask), stupid hack for parts that don't have a Tex2
        if( /*cpart2->pchTex2[strlen(cpart1->pchTex2)-1] != 'b' &&*/ !cpart2->pchTex2 || stricmp( cpart1->pchTex2, cpart2->pchTex2 ) != 0 )
            cost = baseCost;
    }

    if( (isNullOrNone(cpart1->pchFxName) != isNullOrNone(cpart2->pchFxName)) ||  // One is null and the other isn't
        ( !isNullOrNone(cpart1->pchFxName) && stricmp(cpart1->pchFxName, cpart2->pchFxName)) )
        return baseCost;

    return cost;
}

int tailor_RegionCost( Entity *e, const char *regionName, const cCostume* aCostume, int reverse )
{
    const cCostume *costume_current = costume_current_const(e);
    const cCostume *newCostume = aCostume;
    int i, totalCost = 0;

#if CLIENT
    if (newCostume == costume_as_const(gSuperCostume))
    {
        newCostume = costume_as_const(gTailoredCostume);
    }
#endif

    if (reverse)
    {
        const cCostume *temp = costume_current;
        costume_current = newCostume;
        newCostume = temp;
    }

     for( i = 0; i < newCostume->appearance.iNumParts; i++ )
    {
        if( newCostume->parts[i]->regionName && stricmp(newCostume->parts[i]->regionName, regionName ) == 0 )
        {
            // first check to see if bodyset changed, if so use Region change number and leave
            if( costume_current->parts[i]->bodySetName && newCostume->parts[i]->bodySetName &&
                 (stricmp(newCostume->parts[i]->bodySetName, costume_current->parts[i]->bodySetName ) != 0 &&
                (stricmp(newCostume->parts[i]->bodySetName, "weapons") != 0 || stricmp(costume_current->parts[i]->bodySetName, "arachnos weapons") != 0))) //HACK Previously we had a seperate body part name for ararchnos
             {
                return tailor_getRegionCost( e, regionName );
            }

            // drat, now we gotta chack each item
            totalCost += tailor_CalcPartCost( e, newCostume->parts[i], costume_current->parts[i] );
        }
    }

    return totalCost;
}

int tailor_CalcTotalCost( Entity *e, const cCostume* costume, int genderChange )
{
    int total = 0;

    if (genderChange == 2)
    {
//        if( costume_current(e)->appearance.bodytype != e->costume->appearance.bodytype )
        total += 4*tailor_getRegionCost(e, "Upper Body");    //    gender change costs 4?    for now, costing less because this causes a costume reset which would cost quite a bit of change
    }
    else
    {
        if (genderChange)
        {
            if( ABS(costume_current(e)->appearance.fScales[0]- e->costume->appearance.fScales[0]) > .001f )
                total += 3*tailor_getRegionCost(e, "Upper Body");    //    height change costs 3?
        }
        total += tailor_RegionCost( e, "Head", costume, 0 );
        total += tailor_RegionCost( e, "Upper Body", costume,0 );
        total += tailor_RegionCost( e, "Lower Body", costume,0 );
        total += tailor_RegionCost( e, "Weapons", costume,0 );
        total += tailor_RegionCost( e, "Capes", costume,0 );
        total += tailor_RegionCost( e, "Special", costume,0 );
        total += tailor_RegionCost( e, "Shields", costume,0 );
        total += tailor_RegionCost( e, "Epic ArcheType", costume,0 );

        if( costume_changedScale( costume_current_const(e), costume ) )
            total += 2*tailor_getRegionCost( e, "Upper Body" );

    }

    if (total)
    {
        total += tailor_getFee( e );
    }
    else
    {
        int capeOrAura = tailor_RegionCost(e, "Capes", costume, 1) + tailor_RegionCost(e, "Special", costume, 1);
        unsigned int originalCostumeColors[MAX_COSTUME_PARTS][2];
        unsigned int currentCostumeColors[MAX_COSTUME_PARTS][2];
        unsigned int origSkinColor;
        unsigned int newSkinColor;

        //We're only checking for 2 colors, since the last 2 aren't used
        //NOTE: These will need to be changed if costumes start using all four colors
        ExtractCostumeColorsForCompareinTailor(costume, currentCostumeColors, &newSkinColor );
        ExtractCostumeColorsForCompareinTailor(costume_as_const(costume_current(e)), originalCostumeColors, &origSkinColor );

        if (memcmp(currentCostumeColors, originalCostumeColors, sizeof(originalCostumeColors))
            || capeOrAura
            || ((origSkinColor & 0x00ffffff) != (newSkinColor & 0x00ffffff)))
        {
            total += tailor_getFee( e );
        }
    }
    return total;
}

// on the client, these always refer to the player's character
#ifdef SERVER
#define COSTUME_GIVE_TOKEN(tok, notifyPlayer, isPCC) costume_Award(e,tok, notifyPlayer, isPCC)
#define COSTUME_TAKE_TOKEN(tok, isPCC) costume_Unaward(e,tok, isPCC)
#elif CLIENT
#define COSTUME_GIVE_TOKEN(tok, notifyPlayer, isPCC) costumereward_add(tok, isPCC)
#define COSTUME_TAKE_TOKEN(tok, isPCC) costumereward_remove(tok, isPCC)
#else
#define COSTUME_GIVE_TOKEN(tok)
#define COSTUME_TAKE_TOKEN(tok)
#endif

void costumeAwardAuthParts( Entity * e )
{
#ifdef CLIENT
    AccountInventorySet *invSet = inventoryClient_GetAcctInventorySet();
#else
    AccountInventorySet *invSet = ent_GetProductInventory( e );
#endif

    if(authUserHasDVDSpecialEdition(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("DVD_Edition_Cape", 1, 0);
        COSTUME_GIVE_TOKEN("DVD_Edition_Cape", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCovPreorder1(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("PreOrder_CrabSpider_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_CrabSpider_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCovPreorder2(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("PreOrder_BloodWidow_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_BloodWidow_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpiderRed_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpiderRed_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCovPreorder3(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("PreOrder_Mystic_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_Mystic_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCovPreorder4(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpider_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpider_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCovSpecialEdition(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("COV_DVD_Edition_Cape", 1, 0); 
        COSTUME_GIVE_TOKEN("COV_DVD_Edition_Cape", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1); 
        COSTUME_GIVE_TOKEN("COV_DVD_Edition_Emblem", 1, 0);
        COSTUME_GIVE_TOKEN("COV_DVD_Edition_Emblem", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasValentine2006(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Event_Valentine_Accessories", 1, 0);
        COSTUME_GIVE_TOKEN("Event_Valentine_Accessories", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasJumpPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Gold_Combo_Box", 1, 0);
        COSTUME_GIVE_TOKEN("Gold_Combo_Box", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasWeddingPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Wedding_2008", 1, 0);
        COSTUME_GIVE_TOKEN("Wedding_2008", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasCyborgPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("CyborgPack", 1, 0);
        COSTUME_GIVE_TOKEN("CyborgPack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasRogueCompleteBox(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Complete_set", 1, 0);
        COSTUME_GIVE_TOKEN("Complete_set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasMacPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Valkyrie_Set", 1, 0);
        COSTUME_GIVE_TOKEN("Valkyrie_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if(authUserHasMagicPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("MagicPack", 1, 0);
        COSTUME_GIVE_TOKEN("MagicPack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasSupersciencePack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Science_Pack", 1, 0);
        COSTUME_GIVE_TOKEN("Science_Pack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasMartialArtsPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("MartialArts_Set", 1, 0);
        COSTUME_GIVE_TOKEN("MartialArts_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasMutantPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Mutant_Set", 1, 0);
        COSTUME_GIVE_TOKEN("Mutant_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    
    if(authUserHasProvisionalRogueAccess(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("GoingRogue_Set", 1, 0);
        COSTUME_GIVE_TOKEN("GoingRogue_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    else
    {
        if (!e->access_level)
        {
            // This happens when a trial account upgrades to a retail account without buying GR.
            COSTUME_TAKE_TOKEN("GoingRogue_Set", 0);
            COSTUME_TAKE_TOKEN("GoingRogue_Set", 1);
        }
    }

    if(authUserHasAnimalPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Animal_Pack", 1, 0);
        COSTUME_GIVE_TOKEN("Animal_Pack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasFacepalm(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Facepalm", 1, 0);
        COSTUME_GIVE_TOKEN("Facepalm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasRecreationPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("RecreationPack", 1, 0);
        COSTUME_GIVE_TOKEN("RecreationPack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasHolidayPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("HolidayPack", 1, 0);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Boots", 1, 0);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Ear_Muffs", 1, 0);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Gloves", 1, 0);
        COSTUME_GIVE_TOKEN("Winter_Event_2005_Hat", 1, 0);
        COSTUME_GIVE_TOKEN("Aura_NaughtyHalo", 1, 0);
        COSTUME_GIVE_TOKEN("Aura_NiceHalo", 1, 0);
        COSTUME_GIVE_TOKEN("HolidayPack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Boots", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Ear_Muffs", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Event_Xmas_2006_Gloves", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Winter_Event_2005_Hat", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Aura_NaughtyHalo", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Aura_NiceHalo", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasAuraPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("AuraPack", 1, 0);
        COSTUME_GIVE_TOKEN("AuraPack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Origin_Capes", 1, 0);
        COSTUME_GIVE_TOKEN("Origin_Capes", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("whispaur")))
    {
        COSTUME_GIVE_TOKEN("WhispAuraIncentive", 1, 0);
        COSTUME_GIVE_TOKEN("WhispAuraIncentive", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasSteamPunk(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Steam_Punk_Pack", 1, 0);
        COSTUME_GIVE_TOKEN("Steam_Punk_Pack", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    //2011 Promos
    if(authUserHasPromo2011A(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2011_A", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2011_A", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2011B(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2011_B", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2011_B", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2011C(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2011_C", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2011_C", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2011D(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2011_D", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2011_D", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2011E(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2011_E", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2011_E", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }



    //2012 Promos
    if(authUserHasPromo2012A(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2012_A", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2012_A", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2012B(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2012_B", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2012_B", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2012C(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("LoyaltyReward2012", 1, 0);
        COSTUME_GIVE_TOKEN("LoyaltyReward2012", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2012D(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2012_D", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2012_D", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasPromo2012E(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("Promo_2012_E", 1, 0);
        COSTUME_GIVE_TOKEN("Promo_2012_E", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if(authUserHasVanguardPack(e->pl->auth_user_data))
    {
        COSTUME_GIVE_TOKEN("CustomWeapon_Vanguard_01", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_Vanguard_02", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_VanguardKatana_01", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Belt", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Boots", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Chest_Detail", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Chest_Detail_Detail", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Detail_1", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Detail_2", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Glove", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Hair", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_Hips", 1, 0);
        COSTUME_GIVE_TOKEN("Vanguard_SpadR", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianAxe_01", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianBow_01", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianDualBlades_01", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianBroadsword_01", 1, 0);
        COSTUME_GIVE_TOKEN("CustomShield_Talsorian01", 1, 0);
        COSTUME_GIVE_TOKEN("Loyalty2011Emblem", 1, 0);
        COSTUME_GIVE_TOKEN("CustomWeapon_Vanguard_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_Vanguard_02", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_VanguardKatana_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Belt", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Boots", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Chest_Detail", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Chest_Detail_Detail", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Detail_1", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Detail_2", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Glove", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Hair", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_Hips", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Vanguard_SpadR", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianAxe_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianBow_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianDualBlades_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomWeapon_TalsorianBroadsword_01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("CustomShield_Talsorian01", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Loyalty2011Emblem", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    // HYBRID loyalty costume codes
    if (AccountHasStoreProduct(invSet, SKU("ltlbtren")))
    {
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_003", 1, 0);
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_003", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("ltlbgrek")))
    {
        COSTUME_GIVE_TOKEN("veteran_emblem_greek", 1, 0);
        COSTUME_GIVE_TOKEN("veteran_emblem_greek", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbwing")))
    {
        COSTUME_GIVE_TOKEN("Invention_Fly_AngelWings", 1, 0);
        COSTUME_GIVE_TOKEN("Invention_Fly_AngelWings", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Invention_Fly_DemonWings", 1, 0);
        COSTUME_GIVE_TOKEN("Invention_Fly_DemonWings", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Invention_Fly_Wings", 1, 0);
        COSTUME_GIVE_TOKEN("Invention_Fly_Wings", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbkilt")))
    {
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_009", 1, 0);
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_009", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbsrai")))
    {
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_018", 1, 0);
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_018", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbcape")))
    {
        COSTUME_GIVE_TOKEN("Veteran_Shouldercape", 1, 0);
        COSTUME_GIVE_TOKEN("Veteran_Shouldercape", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbtesl")))
    {
        COSTUME_GIVE_TOKEN("veteran_fullcostume_techsleek", 1, 0);
        COSTUME_GIVE_TOKEN("veteran_fullcostume_techsleek", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbsgce")))
    {
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_030", 1, 0);
        COSTUME_GIVE_TOKEN("Veteran_Reward_Month_030", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbboxi")))
    {
        COSTUME_GIVE_TOKEN("Boxing_Set", 1, 0);
        COSTUME_GIVE_TOKEN("Boxing_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbcovh")))
    {
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpider_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpider_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpiderRed_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_WolfSpiderRed_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("PreOrder_Mystic_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_Mystic_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("PreOrder_CrabSpider_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_CrabSpider_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("PreOrder_BloodWidow_Helm", 1, 0);
        COSTUME_GIVE_TOKEN("PreOrder_BloodWidow_Helm", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbboxi")))
    {
        COSTUME_GIVE_TOKEN("Boxing_Set", 1, 0);
        COSTUME_GIVE_TOKEN("Boxing_Set", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbraau")))
    {
        COSTUME_GIVE_TOKEN("LT_radiant_trail", 1, 0);
        COSTUME_GIVE_TOKEN("LT_radiant_trail", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Trail_Auras", 1, 0);
        COSTUME_GIVE_TOKEN("Trail_Auras", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbstau")))
    {
        COSTUME_GIVE_TOKEN("LT_Streak_aura", 1, 0);
        COSTUME_GIVE_TOKEN("LT_Streak_aura", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Trail_Auras", 1, 0);
        COSTUME_GIVE_TOKEN("Trail_Auras", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbblau")))
    {
        COSTUME_GIVE_TOKEN("LT_blazing_aura", 1, 0);
        COSTUME_GIVE_TOKEN("LT_blazing_aura", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Trail_Auras", 1, 0);
        COSTUME_GIVE_TOKEN("Trail_Auras", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbpoau")))
    {
        COSTUME_GIVE_TOKEN("LT_powerful_aura", 1, 0);
        COSTUME_GIVE_TOKEN("LT_powerful_aura", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
        COSTUME_GIVE_TOKEN("Trail_Auras", 1, 0);
        COSTUME_GIVE_TOKEN("Trail_Auras", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("ltlbcl01")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl02")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl03")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl04")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl05")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl06")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl07")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl08")))
    {
        COSTUME_GIVE_TOKEN("CelestialArmor1", 1, 0);
        COSTUME_GIVE_TOKEN("CelestialArmor1", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbcl09")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl11")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl12")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl13")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl14")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl15")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl16")))
    {
        COSTUME_GIVE_TOKEN("CelestialArmor2", 1, 0);
        COSTUME_GIVE_TOKEN("CelestialArmor2", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbcl17")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl18")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl19")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl20")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl21")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl22")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbcl23")))
    {
        COSTUME_GIVE_TOKEN("CelestialArmor3", 1, 0);
        COSTUME_GIVE_TOKEN("CelestialArmor3", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("ltlbfi01")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi02")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi03")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi04")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi05")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi06")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi07")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi08")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi09")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi10")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi11")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi12")))
    {
        COSTUME_GIVE_TOKEN("FireandIce1", 1, 0);
        COSTUME_GIVE_TOKEN("FireandIce1", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbfi13")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi14")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi15")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi16")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi17")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi18")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi19")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi20")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi21")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi22")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi23")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi24")))
    {
        COSTUME_GIVE_TOKEN("FireandIce2", 1, 0);
        COSTUME_GIVE_TOKEN("FireandIce2", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbfi25")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi26")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi27")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi28")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi29")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi30")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi31")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi32")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi33")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi34")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi35")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi36")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi37")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbfi38")))
    {
        COSTUME_GIVE_TOKEN("FireandIce3", 1, 0);
        COSTUME_GIVE_TOKEN("FireandIce3", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("ltlbma01")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma18")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma22")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma23")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma24")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma25")))
    {
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_1", 1, 0);
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_1", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbma02")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma03")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma04")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma05")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma06")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma07")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma08")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma09")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma10")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma11")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma12")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma13")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma14")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma15")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma16")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma17")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma19")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma20")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma21")))
    {
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_2", 1, 0);
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_2", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbma26")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma27")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma28")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma29")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma30")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma31")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma32")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbma33")))
    {
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_3", 1, 0);
        COSTUME_GIVE_TOKEN("Mecha_Armor_T9_3", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }

    if (AccountHasStoreProduct(invSet, SKU("ltlbtk14")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk15")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk17")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk18")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk19")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk20")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk21")))
    {
        COSTUME_GIVE_TOKEN("Magitech_1", 1, 0);
        COSTUME_GIVE_TOKEN("Magitech_1", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbtk01")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk02")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk03")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk04")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk05")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk06")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk08")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk09")))
    {
        COSTUME_GIVE_TOKEN("Magitech_2", 1, 0);
        COSTUME_GIVE_TOKEN("Magitech_2", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
    if (AccountHasStoreProduct(invSet, SKU("ltlbtk07")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk10")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk11")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk12")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk13")) ||
        AccountHasStoreProduct(invSet, SKU("ltlbtk16")))
    {
        COSTUME_GIVE_TOKEN("Magitech_3", 1, 0);
        COSTUME_GIVE_TOKEN("Magitech_3", ARCHITECT_COSTUME_PIECE_NOTIFY_PLAYER, 1);
    }
}

#ifdef SERVER
void costumeValidateCostumes(Entity *e)
{
    int i, j;
    int costume_slots = costume_get_num_slots(e);


    if (e && e->pl && e->pchar && e->pchar->pclass)
    {
        if (e->pl->num_costumes_stored == 0)
        { 
            e->pl->num_costumes_stored = costume_slots;
        }
        else if (costume_slots > e->pl->num_costumes_stored)
        {
            for (i = e->pl->num_costumes_stored; i < costume_slots; i++)
            {
                costume_destroy( e->pl->costume[i]  );
                e->pl->costume[i] = NULL; // destroy does not set to NULL.  NULL is checked in costume_new_slot()
                e->pl->costume[i] = costume_new_slot( e );
                powerCustList_destroy( e->pl->powerCustomizationLists[i] );
                e->pl->powerCustomizationLists[i] = powerCustList_new_slot( e );
            }
            e->pl->num_costumes_stored = costume_slots;
            e->send_costume = true;
            e->send_all_costume = true;
            e->updated_powerCust = true;
            e->send_all_powerCust = true;
        }

        for( i = 0; i < MAX_COSTUMES && i < e->pl->num_costumes_stored; i++ )
        { 
            int valid = false;

            for( j = 0; j < MAX_COSTUME_PARTS && !valid; j++ )
            {
                if (!isNullOrNone(e->pl->costume[i]->parts[j]->pchGeom))
                {
                    valid = true; 
                }
            }

            if (!valid)
            {
                // new costume slot?
                costume_destroy( e->pl->costume[i]  );
                e->pl->costume[i] = NULL; // destroy does not set to NULL.  NULL is checked in costume_new_slot()
                e->pl->costume[i] = costume_new_slot( e );
                powerCustList_destroy( e->pl->powerCustomizationLists[i] );
                e->pl->powerCustomizationLists[i] = powerCustList_new_slot( e );

                e->send_costume = true;
                e->send_all_costume = true;
                e->updated_powerCust = true;
                e->send_all_powerCust = true;
            }
        }

        // adjust for crabbiness
        costume_AdjustAllCostumesForPowers(e);

        // update all weapons in case some haven't been chosen
        costumeFillPowersetDefaults(e);
    }
}

void PrintCostumeToLog(Entity* e)
{
    char * str = estrTemp();
    int i;
    int parts_max = e->costume->appearance.iNumParts;
    estrConcatf(&str, "Gender: %d ~ ", e->costume->appearance.bodytype);
    for(i = 0; i < parts_max; ++i)
    {
        if (e->costume->parts != NULL)
        {
            const CostumePart *costumePart = e->costume->parts[i];

            if(costumePart->regionName && costumePart->bodySetName && !costume_isPartEmpty(costumePart))
            {
                estrConcatf(&str, "RegionName: %s, BodySetName: %s, ", costumePart->regionName, costumePart->bodySetName);
                if (!isNullOrNone(costumePart->pchGeom)) { estrConcatf(&str, "Geo: %s, ", costumePart->pchGeom); }
                if (!isNullOrNone(costumePart->pchTex1)) { estrConcatf(&str, "Texture1: %s, ", costumePart->pchTex1); }
                if (!isNullOrNone(costumePart->pchTex2)) { estrConcatf(&str, "Texture2: %s, ", costumePart->pchTex2); }
                if (!isNullOrNone(costumePart->pchFxName)) { estrConcatf(&str, "FXName: %s, ", costumePart->pchFxName); }
                estrConcatStaticCharArray(&str, "|");
            }
        }
    }
    LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Costume:Log %s", str);
    estrDestroy(&str);
}

#endif

void costumeAwardPowerSetPartsFromPower(Entity *e, const BasePowerSet* psetBase, int notifyPlayer, int isPCC)
{
    if (e && e->pchar)
    {
        if (psetBase)
        {
            int j;
            for (j = (eaSize(&psetBase->ppchCostumeKeys)-1); j >= 0; j--)
            {
                const char * key = psetBase->ppchCostumeKeys[j];
                if (key)
                {
                    COSTUME_GIVE_TOKEN(key, notifyPlayer, isPCC);
                }
            }
        }
    }
}

void costumeAwardPowersetParts(Entity *e, int notifyPlayers, int isPCC)
{
    if(e && e->pchar)
    {
        int i;
        for(i = eaSize(&e->pchar->ppPowerSets)-1; i >= 0; --i)
        {
            PowerSet *pset = e->pchar->ppPowerSets[i];
            if(pset && pset->psetBase)
            {
                costumeAwardPowerSetPartsFromPower(e, pset->psetBase, notifyPlayers, isPCC);
            }
        }
    }
}


void costumeUnawardPowerSetPartsFromPower(Entity *e, const BasePowerSet *psetBase, int isPCC)
{
    if (e && e->pchar)
    {
        if(psetBase)
        {
            int j;
            for(j = eaSize(&psetBase->ppchCostumeKeys)-1; j >= 0; --j)
            {
                const char *key = psetBase->ppchCostumeKeys[j];
                if(key)
                    COSTUME_TAKE_TOKEN(key, isPCC);
            }
        }
    }
}
void costumeUnawardPowersetParts(Entity *e, int isPCC)
{
    if(e && e->pchar)
    {
        int i;
        for(i = eaSize(&e->pchar->ppPowerSets)-1; i >= 0; --i)
        {
            PowerSet *pset = e->pchar->ppPowerSets[i];
            if(pset && pset->psetBase)
            {
                costumeUnawardPowerSetPartsFromPower(e, pset->psetBase, isPCC);
            }
        }
    }
}

void costumeFillPowersetDefaults(Entity *e)
{
    int i;
    for(i = eaSize(&e->pchar->ppPowerSets)-1; i >= 0; --i)
    {
        int j;
        PowerSet *pset = e->pchar->ppPowerSets[i];

        if(!pset || !pset->psetBase)
            continue;

        for(j = eaSize(&pset->psetBase->ppchCostumeParts)-1; j >= 0; --j)
        {
            int k;
            CostumePart *part = costume_getTaggedPart(pset->psetBase->ppchCostumeParts[j], e->costume->appearance.bodytype);
            int bpId = bpGetIndexFromName(SAFE_MEMBER(part,pchName));

            if(!part || bpId < 0)
                continue;

            for(k = 0; k < e->pl->num_costumes_stored; ++k)
            {
                CostumePart *oldpart = e->pl->costume[k]->parts[bpId];
                if(costume_isPartEmpty(oldpart))
                {
                    *oldpart = *part;
                    oldpart->costumeNum = k;
#if SERVER
                    e->send_costume = 1;
                    e->send_all_costume = 1;
#endif
                }
            }
        }
    }
}

#undef COSTUME_GIVE_TOKEN
#undef COSTUME_TAKE_TOKEN


#if SERVER

Costume * costume_recieveTailor( Packet *pak, Entity *e, int genderChange )
{
    int i;
    Costume * costume = costume_create(GetBodyPartCount());

    costume_init(costume);
    costume_receive( pak, costume );

    // Annoyingly authcodes can get added after character creation, so make sue we add them here
    costumeAwardAuthParts( e );
    costumeAwardPowersetParts(e, 1, 0); // in case these change

    // are they cheating?
    if( e->pl->npc_costume || costume_Validate( e, costume, e->pl->current_costume, NULL, NULL, 0, false )  )
    {
        costume_destroy(costume);
        return NULL;
    }

    // check appearance
    if( !e->pl->ultraTailor && !( AccountCanAccessGenderChange(&e->pl->account_inventory, e->pl->auth_user_data, e->access_level) && genderChange) )
    {
        if (    !(AccountCanAccessGenderChange(&e->pl->account_inventory, e->pl->auth_user_data, e->access_level) && 
                    (genderChange == GENDER_CHANGE_MODE_DIFFERENT_GENDER) ) )
        {
            if(costume->appearance.bodytype != e->pl->costume[e->pl->current_costume]->appearance.bodytype )
            {
                costume_destroy(costume);
                return NULL;
            }

        }

        for(i=0;i<1;i++)
        {
            if(costume->appearance.fScales[i]!=e->pl->costume[e->pl->current_costume]->appearance.fScales[i])
            {
                costume_destroy(costume);
                return NULL;
            }
        }
    }    

    return costume;
}

void costume_applyTailorChanges(Entity *e, int genderChange, Costume *costume)
{
    int i;
    // free old costume and assign new costume
    if( e->pl->ultraTailor )
    {
        for(i = 0; i < MAX_COSTUMES && i < costume_get_num_slots(e); i++)
        {
            costume_destroy( e->pl->costume[i] );
            e->pl->costume[i] = costume_clone( costume_as_const(costume) );
        }
        entSetMutableCostumeUnowned( e, costume_current(e) ); // e->costume = costume_as_const(e->pl->costume[e->pl->current_costume]);
        e->pl->ultraTailor = 0;
        costume_destroy(costume);
    }
    else
    {
        for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
        {
            costume->appearance.superColorsPrimaryU[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsPrimaryU[i];
            costume->appearance.superColorsSecondaryU[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsSecondaryU[i];
            costume->appearance.superColorsPrimary2U[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsPrimary2U[i];
            costume->appearance.superColorsSecondary2U[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsSecondary2U[i];
            costume->appearance.superColorsTertiaryU[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsTertiaryU[i];
            costume->appearance.superColorsQuaternaryU[i] = e->pl->costume[e->pl->current_costume]->appearance.superColorsQuaternaryU[i];
        }
        costume->appearance.currentSuperColorSet = e->pl->costume[e->pl->current_costume]->appearance.currentSuperColorSet;

        costume_destroy( e->pl->costume[e->pl->current_costume] );
        e->pl->costume[e->pl->current_costume] = costume;
        entSetMutableCostumeUnowned( e, costume_current(e) ); // e->costume = costume_as_const(e->pl->costume[e->pl->current_costume]);
    }
    if (genderChange == GENDER_CHANGE_MODE_DIFFERENT_GENDER)
    {
        if( e->costume->appearance.bodytype == kBodyType_Female || e->costume->appearance.bodytype == kBodyType_BasicFemale )
            e->name_gender = e->gender = GENDER_FEMALE;
        else
            e->name_gender = e->gender = GENDER_MALE;
    }
    svrChangeBody(e, entTypeFileName(e));
    costume_AdjustAllCostumesForPowers(e);
    e->send_costume = 1;
    e->send_all_costume = true;

}


//-------------------------------------------------------------------------------------
// costume piece award
//-------------------------------------------------------------------------------------


void costume_Award( Entity *e, const char * costumePiece, int notifyPlayer, int isPCC)
{
    if( e && e->pl && costumePiece && costume_isReward(costumePiece) )
    {
        if( rewardtoken_IdxFromName( isPCC ? &e->pl->ArchitectRewardTokens : &e->pl->rewardTokens, costumePiece ) == -1 )
        {
            rewardtoken_Award( isPCC ? &e->pl->ArchitectRewardTokens : &e->pl->rewardTokens, costumePiece, 0 );
            e->send_costume = true;
            if (notifyPlayer)
            {
                //    So far, the only reason not to notify the player is for critter powerset costume pieces
                //    They shouldn't be told that they got some costume piece they don't really have. -EL
                sendInfoBox(e, INFO_REWARD, isPCC ? "ArchitectCostumePieceYouReceived" : "CostumePieceYouReceived", costumePiece);
                serveFloater(e, e, "FloatCostume", 0.0f, kFloaterStyle_Attention, 0);
            }
        }
    }
}

void costume_Unaward( Entity *e, const char * costumePiece, int isPCC)
{
    if( e && costumePiece )
    {
        rewardtoken_Unaward(isPCC ? &e->pl->ArchitectRewardTokens : &e->pl->rewardTokens, costumePiece);
        e->send_costume = true;
        e->send_all_costume = true;
    }
}

void ent_SendCostumeRewards(Packet *pak, Entity *e)
{
    int i, count;

    if( !e || !e->pl )
        return;

    count = eaSize(&e->pl->rewardTokens);
    pktSendBits(pak, 32, count);
    for(i=0; i < count; i++)
    {
        if(costume_isReward(e->pl->rewardTokens[i]->reward) )
        {
            pktSendBits(pak, 1, 1);
            pktSendIndexedString( pak, e->pl->rewardTokens[i]->reward, costume_HashTable );
        }
        else
            pktSendBits(pak, 1, 0);
    }
    count = eaSize(&e->pl->ArchitectRewardTokens);
    pktSendBits(pak, 32, count);
    for(i=0; i < count; i++)
    {
        if(costume_isReward(e->pl->ArchitectRewardTokens[i]->reward) )
        {
            pktSendBits(pak, 1, 1);
            pktSendIndexedString( pak, e->pl->ArchitectRewardTokens[i]->reward, costume_HashTable );
        }
        else
            pktSendBits(pak, 1, 0);
    }

}

void costume_sendSGColors(Entity *e)
{
    int i;
    Costume *costume = costume_current(e);

    if (costume)
    {
        START_PACKET(pak, e, SERVER_SG_COLOR_DATA);
        pktSendBits(pak, 5, costume->appearance.currentSuperColorSet);
        for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
        {
            pktSendBits(pak, 32, costume->appearance.superColorsPrimaryU[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsPrimaryU[i].SGBitSetHigh);
            pktSendBits(pak, 32, costume->appearance.superColorsSecondaryU[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsSecondaryU[i].SGBitSetHigh);
            pktSendBits(pak, 32, costume->appearance.superColorsPrimary2U[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsPrimary2U[i].SGBitSetHigh);
            pktSendBits(pak, 32, costume->appearance.superColorsSecondary2U[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsSecondary2U[i].SGBitSetHigh);
            pktSendBits(pak, 32, costume->appearance.superColorsTertiaryU[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsTertiaryU[i].SGBitSetHigh);
            pktSendBits(pak, 32, costume->appearance.superColorsQuaternaryU[i].SGBitSetLow);
            pktSendBits(pak, 32, costume->appearance.superColorsQuaternaryU[i].SGBitSetHigh);
        }
        END_PACKET
    }
}

#elif CLIENT
void ent_ReceiveCostumeRewards(Packet *pak, Entity *e)
{
    int i, count;
    if( !e || !e->pl )
        return;

    costumereward_clear( 0 );

    count = pktGetBits(pak, 32);

    for(i=0; i < count; i++)
    {
        if( pktGetBits(pak,1) )
            costumereward_add( (char *)pktGetIndexedString( pak, costume_HashTable ), 0);
    }
    count = pktGetBits(pak, 32);
    for(i=0; i < count; i++)
    {
        if( pktGetBits(pak,1) )
            costumereward_add( (char *)pktGetIndexedString( pak, costume_HashTable ), 1);
    }
}

void receiveSGColorData(Packet *pak)
{
    int i;
    Entity *e = playerPtr();
    // These two may, or may not, be different.  In particular, they will be different if the character is in SG mode.  In that case, we need
    // to drop these colors into both costumes.  If the character is not in SG mode, then these two pointers should be the same, in which case we
    // vestigially assign them twice.
    Costume *costume = e->pl->costume[e->pl->current_costume];
    Costume *costume2 = entGetMutableCostume(e);

    costume2->appearance.currentSuperColorSet = costume->appearance.currentSuperColorSet = pktGetBits(pak, 5);
    for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
    {
        costume2->appearance.superColorsPrimaryU[i].SGBitSetLow = costume->appearance.superColorsPrimaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsPrimaryU[i].SGBitSetHigh = costume->appearance.superColorsPrimaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume2->appearance.superColorsSecondaryU[i].SGBitSetLow = costume->appearance.superColorsSecondaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsSecondaryU[i].SGBitSetHigh = costume->appearance.superColorsSecondaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume2->appearance.superColorsPrimary2U[i].SGBitSetLow = costume->appearance.superColorsPrimary2U[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsPrimary2U[i].SGBitSetHigh = costume->appearance.superColorsPrimary2U[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume2->appearance.superColorsSecondary2U[i].SGBitSetLow = costume->appearance.superColorsSecondary2U[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsSecondary2U[i].SGBitSetHigh = costume->appearance.superColorsSecondary2U[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume2->appearance.superColorsTertiaryU[i].SGBitSetLow = costume->appearance.superColorsTertiaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsTertiaryU[i].SGBitSetHigh = costume->appearance.superColorsTertiaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        costume2->appearance.superColorsQuaternaryU[i].SGBitSetLow = costume->appearance.superColorsQuaternaryU[i].SGBitSetLow = pktGetBits(pak, 32);
        costume2->appearance.superColorsQuaternaryU[i].SGBitSetHigh = costume->appearance.superColorsQuaternaryU[i].SGBitSetHigh = pktGetBits(pak, 32);
        if ((costume->appearance.superColorsPrimaryU[i].SGBitSet | costume->appearance.superColorsSecondaryU[i].SGBitSet |
            costume->appearance.superColorsPrimary2U[i].SGBitSet | costume->appearance.superColorsSecondary2U[i].SGBitSet |
            costume->appearance.superColorsTertiaryU[i].SGBitSet | costume->appearance.superColorsQuaternaryU[i].SGBitSet) == 0)
        {
            // 0x55555555 is a sane default initialization value.  Don't change unless you really know what you're doing
            costume2->appearance.superColorsPrimaryU[i].SGBitSet = costume->appearance.superColorsPrimaryU[i].SGBitSet = 0x5555555555555555uLL;
            costume2->appearance.superColorsSecondaryU[i].SGBitSet = costume->appearance.superColorsSecondaryU[i].SGBitSet = 0x5555555555555555uLL;
            costume2->appearance.superColorsPrimary2U[i].SGBitSet = costume->appearance.superColorsPrimary2U[i].SGBitSet = 0x5555555555555555uLL;
            costume2->appearance.superColorsSecondary2U[i].SGBitSet = costume->appearance.superColorsSecondary2U[i].SGBitSet = 0x5555555555555555uLL;
            costume2->appearance.superColorsTertiaryU[i].SGBitSet = costume->appearance.superColorsTertiaryU[i].SGBitSet = 0x5555555555555555uLL;
            costume2->appearance.superColorsQuaternaryU[i].SGBitSet = costume->appearance.superColorsQuaternaryU[i].SGBitSet = 0x5555555555555555uLL;
        }
    }
#ifndef TEST_CLIENT
    tailor_pushSGColors(costume);
#endif
}

#endif

int costume_getNumSGColorSlot(Entity *e)
{
    return 1;
}

#if SERVER
// Kinda the wrong place for this, but I really don't have a good idea where this ought to land.
// Send the list of costume change emotes that the given player entity is capable of
void sendCostumeChangeEmoteList(Entity *e)
{
    int i;
    START_PACKET(pak, e, SERVER_COSTUME_CHANGE_EMOTE_LIST);

    gIsCCemote = 1;

    // Let's see if this is one of the animated emotes
    for (i = eaSize(&g_EmoteAnims.ppAnims) - 1; i >= 0; i--)
    {
        const EmoteAnim *anim = g_EmoteAnims.ppAnims[i];
        bool canDo = chatemote_CanDoEmote(anim, e);
        if ( canDo || (anim->pchStoreProduct != NULL && AccountHasStoreProductOrIsPublished(ent_GetProductInventory(e), skuIdFromString(anim->pchStoreProduct))))
        {
            pktSendString(pak, anim->pchDisplayName);
            if (canDo)
                pktSendString(pak, "");
            else 
                pktSendString(pak, anim->pchStoreProduct);

        }
    }
    gIsCCemote = 0;
    pktSendString(pak, "@");
    pktSendString(pak, "@");

    END_PACKET
}
#endif

//------------------------------------------------------------------------------------------------------
// Costume CSR /////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
void costume_setDefault(Entity * e, int number)
{
    int i, j, bodytype, bodytypeIdx, originset, boneId[MAX_BODYPARTS] = {0}, region_total;
    char * genderName[64] = { "Male", "Female", "BasicMale", "BasicFemale", "Huge", "Villain", "Villain2", "Villain3" };
    const CostumeOriginSet    *cset;
    const CostumeBoneSet      *bset;
    const CostumeGeoSet       *gset;
    Costume                *costume = e->pl->costume[number];

    // first figure out gender
    bodytypeIdx = costume->appearance.bodytype;

    // set the bodyType index and origin index
    for( i = eaSize( &gCostumeMaster.bodySet )-1; i >= 0; i-- )
    {
        if ( stricmp( gCostumeMaster.bodySet[i]->name, genderName[bodytypeIdx] ) == 0 )
        {
            bodytype = i;

            for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0; j-- )
            {
                if ( costume_checkOriginForClassAndSlot( gCostumeMaster.bodySet[i]->originSet[j]->name, e->pchar->pclass, number ) )
                {
                    originset = j;
                    break;
                }
            }
            break;
        }
    }

    cset = gCostumeMaster.bodySet[bodytype]->originSet[originset];
    region_total = eaSize(&cset->regionSet);

    // fist mark all the parts the player has
    assert(AINRANGE(costume->appearance.iNumParts, boneId));
    for( i = 0; i < costume->appearance.iNumParts; i++ )
        boneId[i] = TRUE;

    // now for each region, see what is using those parts
    for( j = 0; j < region_total; j++ )
    {
        bset = cset->regionSet[j]->boneSet[cset->regionSet[j]->currentBoneSet];

        for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
        {
            int bpID = -1;
            gset = bset->geo[i];
            if( gset->bodyPartName )
            {
                bpID = costume_PartIndexFromName( e, gset->bodyPartName );
            }
            if( bpID >= 0 )
            {
                int current = gset->currentInfo;
                boneId[bpID] = FALSE;

                costume_PartSetPath( costume, bpID, gset->displayName, cset->regionSet[j]->name, bset->name );
                costume_PartSetGeometry( costume, bpID, gset->info[current]->geoName);
                costume_PartSetTexture1( costume, bpID, gset->info[current]->texName1);
                costume_PartSetFx( costume, bpID, gset->info[current]->fxName);

                if( !gset->info[current]->texName2 || stricmp( gset->info[current]->texName2, "Mask" ) == 0 )
                {
                    if( eaSize( &gset->mask ) )
                    {
                        int mask = gset->currentMask;
                        costume_PartSetTexture2( costume, bpID, gset->mask[mask]->name );
                    }
                    else if( eaSize( &gset->masks ) )
                    {
                        int mask = gset->currentMask;
                        costume_PartSetTexture2( costume, bpID, gset->masks[mask] );
                    }
                }
                else
                    costume_PartSetTexture2( costume, bpID, gset->info[current]->texName2 );
            }
        }
    }

    // ok now go clear anything that is not being used
    for( i = 0; i < ARRAY_SIZE(boneId); i++ )
    {
        if( boneId[i] )
        {
            costume_PartSetPath( costume, i, NULL, NULL, NULL );
            costume_PartSetGeometry( costume, i, "None" );
            costume_PartSetTexture1( costume, i, "None" );
            costume_PartSetTexture2( costume, i, "None" );
            costume_PartSetFx( costume, i, "None" );
        }
    }

#if CLIENT
    if( costumereward_find( "CrabBackpack", 0 ) )
#elif SERVER
    if( rewardtoken_IdxFromName( &e->pl->rewardTokens, "CrabBackpack" ) >= 0 )
#endif
    {
        CostumePart* backpack_part;

        switch( e->gender )
        {
            case GENDER_MALE:
            default:
                backpack_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Male", kBodyType_Male );
                break;
            case GENDER_FEMALE:
                backpack_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Female", kBodyType_Female );
                break;
            case GENDER_NEUTER:
                backpack_part = costume_getTaggedPart( "Crab_Spider_Backpack_Civilian_Huge", kBodyType_Huge );
                break;
        }

        costume_AdjustCostumeForPower( costume, backpack_part );
    }


    // add default weapons
    costumeFillPowersetDefaults(e);

#ifdef SERVER
    e->send_costume = true;
#endif
}




float maleScale[2*NUM_2D_BODY_SCALES] = {
        //ht    //bone    //head    //shoulder    //chest    //waist    //hips    //legs
        0.00,    -0.65,     0.00,     0.00,        -0.68,    -0.59,     0.00,    0.00,     // min
        0.00,     1.00,     0.00,     0.00,         0.35,     0.14,     0.00,    0.00,     // max
};

float femScale[2*NUM_2D_BODY_SCALES] = {
        //ht    //bone    //head    //shoulder    //chest    //waist    //hips    //legs
        0.00,    -0.85,     0.00,     0.00,        -0.53,    -0.78,     0.00,    0.00,     // min
        0.00,     1.00,     0.00,     0.00,         0.41,     0.22,     0.00,    0.00,     // max
};

float hugeScale[2*NUM_2D_BODY_SCALES] = {
        //ht    //bone    //head    //shoulder    //chest    //waist    //hips    //legs
        0.00,    -0.72,     0.00,     0.00,        -0.75,    -0.74,     0.00,    0.00,     // min
        0.00,     0.86,     0.00,     0.00,         0.52,     0.37,     0.00,    0.00,     // max
};



void costume_retrofitBoneScale( Costume * costume )
{
    Appearance *app = &costume->appearance;
    float *scale, fBoneScale;
    int i;

    if( app->bodytype == kBodyType_Male )
        scale = maleScale;
    else if( app->bodytype == kBodyType_Female )
        scale = femScale;
    else
        scale = hugeScale;

    // pull this out of the union so it doesn't get overwritten
    fBoneScale = app->fBoneScale;

    app->convertedScale = true;

    if( fBoneScale == 0 )
        return;

    if( fBoneScale < 0 )
    {
        for( i = NUM_2D_BODY_SCALES-1; i >= 1; i-- )
            app->fScales[i] = -fBoneScale*(scale[i]);
    }
    else
    {
        for( i = NUM_2D_BODY_SCALES-1; i >= 1; i-- )
            app->fScales[i] = fBoneScale*(scale[i+NUM_2D_BODY_SCALES]);
    }
}

void costume_sendEmblem(Packet * pak, Entity *e )
{
    Costume * costume = costume_makeSupergroup( e );
    int detail_id = costume_PartIndexFromName( e, "ChestDetail" );
    pktSendIndexedString(pak, costume->parts[detail_id]->pchGeom, costume_HashTable);
    pktSendIndexedString(pak, costume->parts[detail_id]->pchTex1, costume_HashTable);
    pktSendIndexedString(pak, costume->parts[detail_id]->pchTex2, costume_HashTable);
}

void costume_recieveEmblem(Packet * pak, Entity *e, Costume * costume )
{
    int detail_id = costume_PartIndexFromName( e, "ChestDetail" );
    CostumePart dummy;
    CostumePart *part = costume ? costume->parts[detail_id] : &dummy;

    part->pchGeom = pktGetIndexedString(pak, costume_HashTable);
    part->pchTex1 = pktGetIndexedString(pak, costume_HashTable);
    part->pchTex2 = pktGetIndexedString(pak, costume_HashTable);
}

static int compareAttr(const char *a1, const char *a2)
{
    if (isNullOrNone(a1)&& isNullOrNone(a2))            return 0;
    if ( (!isNullOrNone(a1) && isNullOrNone(a2) ) || (isNullOrNone(a1) && !isNullOrNone(a2)))    return 1;
    return (stricmp(a1, a2));
}
static int isPartDifferent(const CostumePart *p1, const CostumePart *p2)
{
    int i;
    int isDiff = 0;
    if (!p1 && !p2)                            return 0;
    if ((!p1 && p2) || (p1 && !p2) )        return 1;
    isDiff |= compareAttr(p1->pchName, p2->pchName);
    isDiff |= compareAttr(p1->pchGeom, p2->pchGeom);
    isDiff |= compareAttr(p1->pchTex1, p2->pchTex1);
    isDiff |= compareAttr(p1->pchTex2, p2->pchTex2);
    isDiff |= compareAttr(p1->pchFxName, p2->pchFxName);
    for ( i = 0; i < 4; i++)
    {
        isDiff |= ( p1->color[i].integer != p2->color[i].integer );
    }
    return isDiff;
}
void costume_getDiffParts(const cCostume *c1, const cCostume *c2, int **diffParts)
{
    //    this function is pretty heavy... don't call it too often.
    int i;
    if (c1 && c2 && diffParts)
    {
        for (i = 0; i < MAX_COSTUME_PARTS; ++i)
        {
            if ( isPartDifferent(c1->parts[i], c2->parts[i]) )
            {
                eaiPushUnique(diffParts, i);
            }
        }
    }
}

void costume_createDiffCostume( const cCostume *baseCostume, const cCostume *compareCostume, CostumeDiff *resultingCostume)
{
    if (baseCostume && compareCostume && resultingCostume)
    {
        int *diffPartIdxs;
        int i;
        if ( resultingCostume->differentParts )
        {
            for (i = (eaSize(&resultingCostume->differentParts)-1); i >= 0; --i)
            {
                StructDestroy(ParseCostumeDiff, &resultingCostume->differentParts[i]);
            }
            eaDestroy(&resultingCostume->differentParts);
        }
        eaiCreate(&diffPartIdxs);
        costume_getDiffParts(baseCostume, compareCostume, &diffPartIdxs);
        for (i = 0; i < eaiSize(&diffPartIdxs); ++i)
        {
            CostumePartDiff *differentPart;
            differentPart = StructAllocRaw(sizeof(CostumePartDiff));
            differentPart->index = diffPartIdxs[i];
            differentPart->part = StructAllocRaw(sizeof(CostumePart));
            StructCopy(ParseCostumePart, compareCostume->parts[differentPart->index], differentPart->part, 0,0);
            eaPush(&resultingCostume->differentParts, differentPart);
        }
        resultingCostume->appearance = compareCostume->appearance;
        eaiDestroy(&diffPartIdxs);
    }
}
void costumeApplyNPCColorsToCostume(Costume *costume, CustomNPCCostume *npcCostume)
{
    int i, j;
    if (!costume || !npcCostume)
    {
        return;
    }
    costume->appearance.colorSkin.integer = npcCostume->skinColor.integer;
    for (i = 0; i < eaSize(&npcCostume->parts); ++i)
    {
        for (j = 0; j < costume->appearance.iNumParts; ++j)
        {
            const char *partName = NULL;
            if (costume->parts[j]->pchName)
            {
                partName = costume->parts[j]->pchName;
            }
            else
            {
                partName = bodyPartList.bodyParts[j]->name;
                Errorf("Part Name is missing. Is that right? Using bodyPart (%d, %s) ",j, partName );
            }
            if (stricmp(partName, npcCostume->parts[i]->regionName) == 0)
            {
                costume->parts[j]->color[2].integer = costume->parts[j]->color[0].integer = npcCostume->parts[i]->color[0].integer;
                costume->parts[j]->color[3].integer = costume->parts[j]->color[1].integer = npcCostume->parts[i]->color[1].integer;
            }
        }
    }
}
int validateCustomNPCCostume(CustomNPCCostume *npcCostume)
{
    int i;
    int isBad = 0;
    const ColorPalette **palette;
    const cCostume *originalCostume = NULL;
    const NPCDef *npc = NULL;
    //    sanity check
    if (!npcCostume)
    {
        return 0;
    }
    npc = npcFindByName(npcCostume->originalNPCName, 0);
    if (!npc)
    {
        return 0;
    }
    originalCostume = npc->costumes[0];
    palette = gCostumeMaster.bodySet[0]->originSet[0]->skinColors;

    //    if not the original color, check it against available skin palette colors
    if (npcCostume->skinColor.integer != originalCostume->appearance.colorSkin.integer)
    {
        for (i = 0;(i < eaSize((F32***)&palette[0]->color)); ++i)
        {
            if (((*palette[0]->color[i])[0] == npcCostume->skinColor.r) && 
                ((*palette[0]->color[i])[1] == npcCostume->skinColor.g) && 
                ((*palette[0]->color[i])[2] == npcCostume->skinColor.b) )
            {
                break;
            }
        }
        if ( i >= eaSize((F32***)&palette[0]->color))
        {
            isBad = 1;
        }
    }
    palette = gCostumeMaster.bodySet[0]->originSet[0]->bodyColors;
    for (i = 0; i < eaSize(&npcCostume->parts); ++i)
    {
        //    validate all the npc Costume part attempts
        if (!npcCostume->parts[i]->regionName)
        {
            isBad = 1;
        }
        else
        {
            int j;
            npcCostume->parts[i]->color[0].a = npcCostume->parts[i]->color[1].a = 0xff;
            for (j = 0; j < eaSize(&originalCostume->parts); j++)
            {
                if (stricmp(originalCostume->parts[j]->pchName, npcCostume->parts[i]->regionName) == 0)
                {
                    int m;
                    for (m = 0; m < 2; ++m)
                    {
                        //    if not the original color, check it against available costume part palette colors
                        if (originalCostume->parts[j]->color[m].integer != npcCostume->parts[i]->color[m].integer)
                        {
                            int k;
                            int found = 0;
                            for (k = 0;(k < eaSize((F32***)&palette[0]->color)) && !found; ++k)
                            {
                                if (((*palette[0]->color[k])[0] == npcCostume->parts[i]->color[m].r) && 
                                    ((*palette[0]->color[k])[1] == npcCostume->parts[i]->color[m].g) && 
                                    ((*palette[0]->color[k])[2] == npcCostume->parts[i]->color[m].b) && 
                                    npcCostume->parts[i]->color[m].a == 0xff)
                                {
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                isBad = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return !isBad;
}
