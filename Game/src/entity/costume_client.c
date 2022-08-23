/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include "entity/entity.h"                // for Entity
#include "entity/entPlayer.h"
#include "entity/entclient.h"            // for changeColor
#include "UI/uiAvatar.h"            // for scaleHero
#include "player/player.h"                // for isPlayerValid
#include "UI/uiCostume.h"            // for stupid cpicker crap
#include "UI/uiGame.h"            // for shell_menu()
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include "entity/costume_client.h"
#include "gameData/costume_data.h"
#include "gameComm/Npc.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/SimpleParser.h>
#include "cmdparse/cmdgame.h"
#include "render/tex.h"
#include "seq/seq.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/StashTable.h>
#include "gameData/costume_critter.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StringCache.h>
#include "fxinfo.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/error.h>
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/FolderCache.h>

StashTable g_hashCostumeRewards;
StashTable g_hashArchitectCostumeRewards;
void get_color( Entity *e, Color colorSkin, Color color1, Color color2, int *rgba1, int *rgba2, const char *pchTex )
{
    *rgba1 =    color1.integer;
    *rgba2 =    color2.integer;

    if(pchTex && strstriConst( pchTex, "SKIN" ))
    {
         *rgba2 =    *rgba1;
        *rgba1 =    colorSkin.integer;
    }
}

static void doChangeColor(Entity *e, const BodyPart *bone, Color colorSkin, Color color1, Color color2, const char *pchTex)
{
    int rgba1, rgba2;

    if( bone->bone_num[0] != -1 )
    {
        get_color(e, colorSkin, color1, color2, &rgba1, &rgba2, pchTex);
        changeColor(e, bone->bone_num[0], (char *)&rgba1, (char *)&rgba2);

        if( bone->num_parts == 2 )
        {
            changeColor(e, bone->bone_num[1], (char *)&rgba1, (char *)&rgba2);
        }
    }
}


//
//
//
static int gender_prefix_fixup(const Appearance *appearance, char *tex_name)
{
    char    pre[256];

    STR_COMBINE_BEGIN(pre);
    if(appearance->bodytype == kBodyType_Villain)
    {
        //sprintf( pre, "%s_%s.tga", costumePrefixName(appearance), tex_name );
        STR_COMBINE_CAT(costumePrefixName(appearance));
    }
    else
    {
        //sprintf( pre, "%s_%s.tga", g_BodyTypeInfos[appearance->bodytype].pchTexPrefix, tex_name );
        STR_COMBINE_CAT(g_BodyTypeInfos[appearance->bodytype].pchTexPrefix);
    }
    STR_COMBINE_CAT("_");
    STR_COMBINE_CAT(tex_name);
    STR_COMBINE_CAT(".tga");
    STR_COMBINE_END();

    if(texFind( pre ))
    {
        strcpy( tex_name, pre );
        tex_name[ strlen( tex_name ) - 4 ] = 0;
        return TRUE;
    }

    return FALSE;
}

void setGender( Entity *e, const char * gender )
{
    BodyType bodytype = kBodyType_Male;
    BodyTypeInfo* bodyInfo;
    int    hack_for_playerptr = false;

    // JE: Hackish code to have a seperate entity operated on when in SHOW_TEMPLATE mode
    // but sharing the player's costume pointer
    if (game_state.game_mode == SHOW_TEMPLATE && e == playerPtr()) {
        Entity *pe = playerPtrForShell(1);
        entCostumeShare( pe, e ); // pe->costume = e->costume;
        e = pe;
        hack_for_playerptr = true;
    }

    assert(e);
    assert(e->costume);

    // load sequence
    if(!gCostumeCritter)
    {
        doDelAllCostumeFx(e,0); // we don't always delete all costume fx anymore
        entSetSeq(e, seqLoadInst( gender, 0, SEQ_LOAD_FULL, e->randSeed, e ));
        if( hack_for_playerptr )
        {
            doDelAllCostumeFx(playerPtr(),0);
            entSetSeq(playerPtr(), seqLoadInst( gender, 0, SEQ_LOAD_FULL, playerPtr()->randSeed, playerPtr() ));
        }
    }

    bodyInfo = GetBodyTypeInfo(gender);
    costume_SetBodyType(costume_as_mutable_cast(e->costume), bodyInfo->bodytype);
    costume_SetBodyName(costume_as_mutable_cast(e->costume), bodyInfo->pchEntType);
}

typedef struct ChestGeoLink
{
    const char * bonesetName;
    const char ** geoStrings;
    const char * filename;
}ChestGeoLink;

typedef struct ChestGeoLinkList
{
    ChestGeoLink ** list;

}ChestGeoLinkList;
ChestGeoLinkList gChestGeoLinkList = { 0 };
TokenizerParseInfo ParseChestGeoLink[] =
{
    { "{",                TOK_START, 0 },
    { "BonesetName",    TOK_STRING(ChestGeoLink, bonesetName, 0) },
    { "GeoStrings",        TOK_STRINGARRAY(ChestGeoLink, geoStrings) },
    { "filename",        TOK_CURRENTFILE(ChestGeoLink,filename)    },
    { "}",                TOK_END, 0 },
    { 0 }
};

TokenizerParseInfo ParseChestGeoLinkList[] =
{
    { "ChestGeoLink",    TOK_STRUCT(ChestGeoLinkList, list, ParseChestGeoLink)        },
    { "", 0, 0 }
};

static void RebuildChestGeoLinkList(const char *relpath, int when)
{
    if (strEndsWith(relpath, ".bak"))
        return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    StructClear(ParseChestGeoLinkList, &gChestGeoLinkList);
    if (!ParserReloadFile(relpath, ParseChestGeoLinkList, sizeof(ChestGeoLinkList), &gChestGeoLinkList, NULL, NULL))
    {
        Errorf("Error reloading Chest Geo Link: %s\n", relpath);
    }
}

void load_ChestGeoLinkList(void)
{
    const char *pchFilename = "defs/chestGeoLink.def";
    const char *pchBinFilename = MakeBinFilename(pchFilename);

    ParserLoadFiles(0,pchFilename, pchBinFilename, 0, ParseChestGeoLinkList, &gChestGeoLinkList, NULL, NULL, NULL);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, pchFilename, RebuildChestGeoLinkList);
}
 
const char ** getChestGeoLink( Entity *e )
{
    int i;

    for( i = 0; i < e->costume->appearance.iNumParts; ++i )
    {
        //determine which piece is the chest
        if( stricmp( bodyPartList.bodyParts[i]->name, "chest" ) == 0 )
        {
            const char * bonesetName = e->costume->parts[i]->bodySetName;
            int i, size = eaSize(&gChestGeoLinkList.list);

            if (!bonesetName)
            {
                //not using normal body set parts, get out and return default
                break;
            }

            for (i = 0; i < size; ++i)
            {
                if (stricmp(gChestGeoLinkList.list[i]->bonesetName, bonesetName) == 0)
                {
                    return gChestGeoLinkList.list[i]->geoStrings;
                }
            }
        }
    }

    // return the first entry as default
    devassertmsg(eaSize(&gChestGeoLinkList.list), "Error with chestGeoLink.def");
    return gChestGeoLinkList.list[0]->geoStrings;
}


bool bodyPartIsLinkedToChest( const BodyPart * bp )
{
    if( stricmp( bp->name, "Collar" ) == 0 ||
        stricmp( bp->name, "Capeharness" ) == 0 ||
        stricmp( bp->name, "Broach" ) == 0 ||
        stricmp( bp->name, "Back" ) == 0 ||
        stricmp( bp->name, "Cape" ) == 0)
    {
        return true;
    }

    return false;
}


//
//
void doChangeGeo( Entity *e, const BodyPart *bone, const char *name, const char *tex1, const char *tex2 )
{
    const char *s;
    char base_geo_name[ MAX_NAME_LEN ];
    char geo_name[ MAX_NAME_LEN ];
    char tmp[ MAX_NAME_LEN ];

    if( !name || !bone || bone->bone_num[0] == -1 )
        return;

    base_geo_name[0] = geo_name[0] = tmp[0] = 0;

     if(stricmp( name, "NONE" )==0)
    {
        changeGeo(  e, bone->bone_num[0], "NONE", 1 );
        if( bone->num_parts == 2 )
        {
            changeGeo(  e, bone->bone_num[1], "NONE", 1 );
        }
        return;
    }

    if (s=strstriConst(name, ".geo/")) {
        // changeGeo wants name of FILENAME.GEOMETRYNAME
        // We have been given a complete filename + geometry name
        strcpy(geo_name, name);
        geo_name[s-name]='\0';
        strcat(geo_name, ".");
        strcat(geo_name, s+5);
        if ( bodyPartIsLinkedToChest( bone ) )
        {
            const char ** possibleChestNames = getChestGeoLink(e);
            int i, revert = true;;
            for (i = 0; i < eaSize(&possibleChestNames); ++i)
            {
                sprintf(tmp, "%s_%s", geo_name, possibleChestNames[i]);
                if(changeGeo( e, bone->bone_num[0], tmp, 0))
                {
                    revert = false;
                    break;
                }
            }
            if (revert)
            {
                // no special geo found
                changeGeo( e, bone->bone_num[0], geo_name, 1);
            }
        } else {
            // Normal
            if (bone->num_parts==2) {
                char *s2;
                if (s2=strchr(geo_name, '*')) {
                    *s2='R';
                    changeGeo( e, bone->bone_num[0], geo_name, 1);
                    *s2='L';
                    changeGeo( e, bone->bone_num[1], geo_name, 1);
                } else {
                    if (strstri(geo_name, "LArmR")) {
                        changeGeo( e, bone->bone_num[0], geo_name, 1);
                        changeGeo( e, bone->bone_num[1], "NONE", 1);
                    } else if (strstri(geo_name, "LArmL")) {
                        changeGeo( e, bone->bone_num[0], "NONE", 1);
                        changeGeo( e, bone->bone_num[1], geo_name, 1);
                    } else if (strstri(geo_name, "LLegR")) {
                        changeGeo( e, bone->bone_num[0], geo_name, 1);
                        changeGeo( e, bone->bone_num[1], "NONE", 1);
                    } else if (strstri(geo_name, "LLegL")) {
                        changeGeo( e, bone->bone_num[0], "NONE", 1);
                        changeGeo( e, bone->bone_num[1], geo_name, 1);
                    }
                }

            } else {
                changeGeo( e, bone->bone_num[0], geo_name, 1);
            }
        }
    } else {
        // Auto-generate name via madness

        //sprintf( base_geo_name, "%s_%s.GEO_%s",costumePrefixName(&e->costume->appearance), bone->base_var, bone->name_geo);
        STR_COMBINE_BEGIN(base_geo_name);
        STR_COMBINE_CAT(costumePrefixName(&e->costume->appearance));
        STR_COMBINE_CAT("_");
        STR_COMBINE_CAT(bone->base_var);
        STR_COMBINE_CAT(".GEO_");
        STR_COMBINE_CAT(bone->name_geo);
        STR_COMBINE_END();
        
        strcpy( geo_name, base_geo_name );

        if( bone->num_parts == 2 )
        {
            strcat( geo_name, "R" );
        }

        if( bodyPartIsLinkedToChest( bone ) )
        {
            const char ** possibleChestNames = getChestGeoLink(e);
            int i, revert = true;;
            for (i = 0; i < eaSize(&possibleChestNames); ++i)
            {
                sprintf( tmp, "%s_%s_%s", geo_name, possibleChestNames[i], name );
                if(changeGeo( e, bone->bone_num[0], tmp, 0))
                {
                    revert = false;
                    break;
                }
            }
            if (revert)
            {
                // no special geo found
                sprintf( tmp, "%s_%s", geo_name, name );
            }
        }
        else
        {
            //sprintf( tmp, "%s_%s", geo_name, name );
            STR_COMBINE_BEGIN(tmp);
            STR_COMBINE_CAT(geo_name);
            STR_COMBINE_CAT("_");
            STR_COMBINE_CAT(name);
            STR_COMBINE_END();
        }

        changeGeo( e, bone->bone_num[0], tmp, 1);

        if(bone->num_parts == 2)
        {
            strcat( base_geo_name, "L" );
            sprintf( tmp, "%s_%s", base_geo_name, name );
            changeGeo( e, bone->bone_num[1], tmp, 1);
        }
    }
}

// Returns 1 if invalid data/no texture/no bone
int determineTextureNames(const BodyPart *bone, const Appearance *appearance, const char *name1, const char *name2, char *out1, size_t out1size, char *out2, size_t out2size, bool *found1, bool *found2)
{
    int none1, none2;
    bool explicit1=false, explicit2=false;
    int dual_pass;
    int single_texture;

    if (found1)
        *found1 = TRUE;
    if (found2)
        *found2 = TRUE;
    if( !name1 || !bone ) {
        return TRUE;
    }
    if (bone->bone_num[0] == -1) {
        strcpy(out1, name1);
        strcpy(out2, name2);
        return TRUE;
    }

    calc_tex_types1(name1, &single_texture, &dual_pass);

    explicit1 = name1[0]=='!';
    none1 = stricmp( name1+explicit1, "NONE" )==0;

    if( !none1 )
    {
        if (explicit1)
            strcpy(out1, name1+1);
        else
        {
            //sprintf( out1, "%s_%s", bone->name_tex, name1 );
            STR_COMBINE_BEGIN_S(out1, out1size);
            STR_COMBINE_CAT(bone->name_tex);
            STR_COMBINE_CAT("_");
            STR_COMBINE_CAT(name1);
            STR_COMBINE_END();
        }
    }
    else
    {
        strcpy( out1, "none" );
    }

    if( !name2 )
    {
        none2 = TRUE;
        explicit2 = false;
    }
    else
    {
        explicit2 = name2[0]=='!';
        none2 = stricmp( name2+explicit2, "NONE" )==0;
    }

    if( none2 || single_texture )
    {
        strcpy( out2, "none" );
    }
    else if( !dual_pass )
    {
        if (explicit2)
            strcpy(out2, name2+1); // Use explicit texture name
        else
        {
            //sprintf( out2, "%s_%s", bone->name_tex, name2 );
            STR_COMBINE_BEGIN_S(out2, out2size);
            STR_COMBINE_CAT(bone->name_tex);
            STR_COMBINE_CAT("_");
            STR_COMBINE_CAT(name2);
            STR_COMBINE_END();
        }
    }
    else
    {
        int len;

        len = strlen( out1 );
        if (explicit2)
            strcpy(out2, name2+1); // Use explicit texture name
        else if( appearance->costumeFilePrefix && stricmp( &out1[len-3], "01a" ) == 0 ) // Hack still needed for villains
        {
            strcpy( out2, out1 );
            out2[len-1] = 'B';
        }
        else
            sprintf( out2, "%s_%s", bone->name_tex, name2 );
    }

    //check to add MM SM or SF prefix?
    if (!explicit1)
        gender_prefix_fixup( appearance, out1 );      //textures may have mm_, sm_ or sf_ prefix.  Check and add if necc.
    if (!explicit2)
        gender_prefix_fixup( appearance, out2 );

    if (found1 && !none1 && !texFindComposite(out1))
        *found1 = FALSE;
    if (found2 && !none2 && !texFind(out2))
        *found2 = FALSE;

    return FALSE;
}

//
//
//
int doChangeTex( Entity *e, const BodyPart *bone, const char *name1, const char *name2 )
{
    char    tmp[MAX_NAME_LEN], tmp1[ MAX_NAME_LEN ];
    int     fnd = FALSE;
    int        ret, j;

    ret = determineTextureNames(bone, &e->costume->appearance, name1, name2, SAFESTR(tmp), SAFESTR(tmp1), NULL, NULL);
    if (ret)
        return TRUE;

    for( j = 0; j < bone->num_parts; j++ )
    {
        changeTexture(  e, bone->bone_num[j], tmp, tmp1 );
    }

    return fnd;
}


static void doAddCostumeFx( Entity *e, const CostumePart *part, const BodyPart * bp, int index, int numColors, int incompatibleFxFlags)
{
    if (e && e->seq) {
        seqAddCostumeFx(e, part, bp, index, numColors, incompatibleFxFlags);
    }
}

void doDelAllCostumeFx( Entity *e, int firstIndex)
{
    if (e && e->seq) {
        seqDelAllCostumeFx(e->seq, firstIndex);
    }
}

//
//
//

// If the texture name ends in
//     a|A  == dual pass
//     x|X  == single_texture

int calc_tex_types1( const char *name, int *single_texture, int *dual_pass )
{
    char c;

    *single_texture = *dual_pass = 0;

    if( name )
    {
        int len;

        len = strlen( name );
        c = name[ len - 1 ];

        if( c == 'a' || c == 'A' )
            *dual_pass = TRUE;
        else if( c == 'x' || c == 'X' )
            *single_texture = TRUE;
    }

    return TRUE;
}

//
//
void scaleHero( Entity *e, float val )
{
    Vec3    new_scale;

    // JE: Hackish code to have a separate entity operated on when in SHOW_TEMPLATE mode
    // but sharing the player's costume pointer
    if (game_state.game_mode == SHOW_TEMPLATE && e == playerPtr()) {
        Entity *pe = playerPtrForShell(1);
        entCostumeShare( pe, e ); // pe->costume = e->costume;
        e = pe;
    }

    if( e )
    {
        new_scale[0] = new_scale[1] = new_scale[2] = ( ( float )val / 100.f ) + 1.0f;
        changeScale( e, new_scale );
    }
}



///////////////////////////////////////////////////////////////////////
//
// Costume stuff
//
int g_costume_apply_hack=0; // In case you're in the shell when you get a costume update
void costume_Apply(Entity *pe)
{
    int i;
    char achDebug[1024] = "";
    int parts_max, parts_count = eaSize(&bodyPartList.bodyParts);
    Vec3 shieldpos, shieldpyr;
    int killfx = 0;
    FxInfoFlags incompatibleFlags = 0;
    bool *parts_seen = _alloca(sizeof(bool)*parts_count);
    memset(parts_seen,0,sizeof(bool)*parts_count);

    assert(pe);
    if( !pe->costume )
        return;
    
    PERFINFO_AUTO_START("costume_Apply", 1);

    // JE: Hackish code to have a separate entity operated on when in SHOW_TEMPLATE mode
    // but sharing the player's costume pointer
    if (game_state.game_mode == SHOW_TEMPLATE && pe == playerPtr() && !g_costume_apply_hack) {
        Entity *e = playerPtrForShell(1);
        entCostumeShare( e, pe ); // e->costume = pe->costume;
        e->npcIndex = pe->npcIndex;
        pe = e;
    }

    // DEBUG
    STR_COMBINE_BEGIN(achDebug);
    //sprintf(achDebug, "%s name:%s type:%d", achDebug, pe->name, ENTTYPE(pe));
    STR_COMBINE_CAT(" name:");
    STR_COMBINE_CAT(pe->name);
    STR_COMBINE_CAT("type:");
    STR_COMBINE_CAT_D(ENTTYPE(pe));
    
    if(pe->pl)
    {
        //sprintf(achDebug, "%s numparts:%d", achDebug, pe->costume->appearance.iNumParts);
        STR_COMBINE_CAT(" numparts:");
        STR_COMBINE_CAT_D(pe->costume->appearance.iNumParts);
    }

    if(pe->costume->appearance.entTypeFile)
    {
        //sprintf(achDebug, "%s enttypefile:%s", achDebug, pe->costume->appearance.entTypeFile);
        STR_COMBINE_CAT(" enttypefile:");
        STR_COMBINE_CAT(pe->costume->appearance.entTypeFile);
    }

    if(pe->costume->appearance.bodytype != kBodyType_Villain)
    {
        //sprintf(achDebug, "%s bodytype:%d enttype:%s", achDebug, pe->costume->appearance.bodytype, g_BodyTypeInfos[pe->costume->appearance.bodytype].pchEntType);
        STR_COMBINE_CAT(" bodytype:");
        STR_COMBINE_CAT_D(pe->costume->appearance.bodytype);
        STR_COMBINE_CAT(" enttype:");
        STR_COMBINE_CAT(g_BodyTypeInfos[pe->costume->appearance.bodytype].pchEntType);
    }
    STR_COMBINE_END();
    // END DEBUG

    if( !gCostumeCritter || pe != playerPtrForShell(0) )
    {
        //    Handling height changes -> sequencer updating
        if ( pe->costume && pe->seq && (ENTTYPE(pe) == ENTTYPE_PLAYER) && !pe->npcIndex && isMenu(MENU_GAME) && (game_state.game_mode == SHOW_GAME) )    
                                                                                                                                //    extra sanity check. Dont do this check if you aren't in game, and are a applicable entity
        {
            float newHeight = (100.0f + pe->costume->appearance.fScale) * 0.01;    //    this formula is in the NwRagdoll.cpp
            float oldHeight = pe->seq->currgeomscale[1];
            if (oldHeight && newHeight && (ABS(newHeight - oldHeight) > 0.01f) )
            {
                //    If the height changes, redo the sequencers
                //    we could just free the sequencers but there is some extra gfx information linked to it
                //    To be sure that stuff gets updated, changing the body to the opposite gender prior to the real change
                //    so that the sequencers are guaranteed to be updated. -EL
                changeBody( pe, g_BodyTypeInfos[!pe->costume->appearance.bodytype].pchEntType);
            }
        }
        changeBody( pe, entTypeFileName(pe) );
    }

#ifndef TEST_CLIENT // TestClients don't care about sequencers
    assertmsg(pe->seq, achDebug);
#else 
    return;
#endif

    if(pe->costume->appearance.fScale != 0.0 || ENTTYPE(pe)==ENTTYPE_PLAYER)
        scaleHero( pe, pe->costume->appearance.fScale );

//    if(pe->costume->appearance.fBoneScale != 0.0 || ENTTYPE(pe)==ENTTYPE_PLAYER)
//        changeBoneScale( pe->seq, pe->costume->appearance.fBoneScale );
    changeBoneScaleEx(pe->seq, &pe->costume->appearance);

    //assert(pe->costume->appearance.iNumParts <= eaSize(&pe->costume->parts));
    //assert(pe->costume->appearance.iNumParts);
    parts_max = MIN(pe->costume->appearance.iNumParts,parts_count);

    // First load geometry
    if(pe->seq)
    {
        copyVec3(pe->seq->seqGfxData.shieldpos, shieldpos);
        zeroVec3(pe->seq->seqGfxData.shieldpos);
        copyVec3(pe->seq->seqGfxData.shieldpyr, shieldpyr);
        zeroVec3(pe->seq->seqGfxData.shieldpyr);
    }
    for(i = 0; i < parts_max; i++)
    {
        int bpidx = i;
        const BodyPart* bodyPart;
        const CostumePart *costumePart = pe->costume->parts[i];

        if( !costumePart )  // what costume has no parts?
            continue;

        // On which BodyPart does this piece of costume go?
        if( (ENTTYPE(pe) != ENTTYPE_PLAYER || pe->npcIndex)
            && costumePart->pchName && costumePart->pchName[0] )
        {
            bpidx = bpGetIndexFromName(costumePart->pchName);
        }

        if(bpidx < 0 || bodyPartList.bodyParts[bpidx]->num_parts <= 0) // Unknown bone name - can happen in image server mode if it has too new of data
            continue;
        bodyPart = bodyPartList.bodyParts[bpidx];

        // check to see if using animal head - HACK
        if(bodyPart->name && 0==stricmp(bodyPart->name, "head") && costumePart->bodySetName && 0==strnicmp(costumePart->bodySetName, "animal ", 7))
            incompatibleFlags |= FX_NOT_COMPATIBLE_WITH_ANIMAL_HEAD;

        parts_seen[bpidx] = true;
        if(bodyPart->dont_clear && costume_isPartEmpty(costumePart))
            continue;

        // Change the geometry on that BodyPart.
        doChangeGeo(pe, bodyPart, costumePart->pchGeom, costumePart->pchTex1, costumePart->pchTex2);
    }
    if(pe->seq)
    {
        if(    !sameVec3(pe->seq->seqGfxData.shieldpos, shieldpos) ||
            !sameVec3(pe->seq->seqGfxData.shieldpyr, shieldpyr) )
        {
            killfx = 1;
        }
    }
    for(i = 0; i < parts_max; i++)
    {
        int bpidx = i;
        const BodyPart* bodyPart;
        const CostumePart *costumePart = pe->costume->parts[i];

        // On which BodyPart does this piece of costume go?
        if( (ENTTYPE(pe) != ENTTYPE_PLAYER || pe->npcIndex)
            && costumePart->pchName && costumePart->pchName[0] )
        {
            bpidx = bpGetIndexFromName(costumePart->pchName);
        }

        if(bpidx < 0 || bodyPartList.bodyParts[bpidx]->num_parts <= 0) // Unknown bone name - can happen in image server mode if it has too new of data
            continue;
        bodyPart = bodyPartList.bodyParts[bpidx];
        if(bodyPart->dont_clear && costume_isPartEmpty(costumePart))
            continue;

        if(killfx)
        {
            CostumePart cfx={0};
            doAddCostumeFx(pe, &cfx, bodyPart, i, 0, 0);
        }

        doAddCostumeFx(pe, costumePart, bodyPart, i, 4, incompatibleFlags);
    }

    // Then textures, etc
    for(i = 0; i < parts_max; i++)
    {
        int bpidx = i;
        const BodyPart* bodyPart;
        const CostumePart *costumePart = pe->costume->parts[i];

        // On which BodyPart does this piece of costume go?
        if(!isNullOrNone(costumePart->pchName))
            bpidx = bpGetIndexFromName(costumePart->pchName);

        if(bpidx < 0 || bodyPartList.bodyParts[bpidx]->num_parts <= 0) // Unknown bone name - can happen in image server mode if it has too new of data
            continue;
        bodyPart = bodyPartList.bodyParts[bpidx];
        parts_seen[bpidx] = true;
        if(bodyPart->dont_clear && costume_isPartEmpty(costumePart))
            continue;

        // Change the textures on that BodyPart.
        doChangeTex(pe,bodyPart, costumePart->pchTex1, costumePart->pchTex2);

        // Change the colors on that BodyPart.
        doChangeColor(pe, bodyPart, pe->costume->appearance.colorSkin,
            costumePart->color[0], costumePart->color[1], costumePart->pchTex1);
    }

// it's difficult to tell when to use this code, so just make sure that npc costumes for costume-changing powers
// define every bodypart
//
//     // Clear other parts
//     if(pe->costume->appearance.iNumParts)
//     {
//         parts_max = parts_count; // used below for more cleanup, we're going to handle up to [parts_count-1]
//         for(i = 0; i < parts_count; ++i)
//         {
//             BodyPart *bodyPart = bodyPartList.bodyParts[i];
//             if(bodyPart->dont_clear)
//             {
//                 // MAX1(parts_max,i+1); // just clear everything up to parts_count instead (parts_max = parts_count above)
//                 if(pe->pl && pe->pl->current_costume >= 0 && pe->pl->current_costume < pe->pl->num_costume_slots)
//                 {
//                     Costume *costume = pe->pl->costume[pe->pl->current_costume];
//                     if( costume && i < costume->appearance.iNumParts &&
//                         !costume_isPartEmpty(costume->parts[i]) ) // hack: costume->parts[i] isn't necessarily for bodyParts[i], a for loop would be better
//                     {
//                         CostumePart *costumePart = costume->parts[i];
//                         doChangeGeo(pe, bodyPart, costumePart->pchGeom, costumePart->pchTex1, costumePart->pchTex2);
//                         doAddCostumeFx(pe, costumePart, bodyPart, i, 4);
//                     }
//                 }
//             }
//             else if(!parts_seen[i]) // && bodyPart->num_parts > 0) // just clear everything up to parts_count
//             {
//                 CostumePart cfx={0};
//                 doChangeGeo(pe,bodyPart,"none","none","none");
//                 doAddCostumeFx(pe, &cfx, NULL, i, 0);
//             }
//         }
//     }

    if (pe->pl) {
        if (pe->pl->costumeFxSpecial[0])
        {
            CostumePart cfx={0};
            cfx.pchFxName = pe->pl->costumeFxSpecial;
            doAddCostumeFx(pe, &cfx, NULL, parts_max, 0, incompatibleFlags);
        }
        else if (!pe->npcIndex)
        {
            doDelAllCostumeFx(pe, parts_max);
        }
    }
    
    PERFINFO_AUTO_STOP();
}

//
// costume rewards
//-----------------------------------------------------------------------------------------------------------------

void costumereward_add( const char * name, int isPCC )
{
    StashTable *rewardTable;
    rewardTable = isPCC ? &g_hashArchitectCostumeRewards : &g_hashCostumeRewards;
    if( !(*rewardTable) )
        (*rewardTable) = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);

    stashAddPointer((*rewardTable), name, 0, false);
}

void costumereward_remove( const char * name, int isPCC )
{
    StashTable *rewardTable;
    rewardTable = isPCC ? &g_hashArchitectCostumeRewards : &g_hashCostumeRewards;

    if((*rewardTable))
        stashRemovePointer((*rewardTable), name, NULL);
}

void costumereward_clear( int isPCC )
{
    StashTable *rewardTable;
    rewardTable = isPCC ? &g_hashArchitectCostumeRewards : &g_hashCostumeRewards;

    if ((*rewardTable)) stashTableClear((*rewardTable));
}

bool costumereward_find( const char * name, int isPCC )
{
    StashTable *rewardTable;
    rewardTable = isPCC ? &g_hashArchitectCostumeRewards : &g_hashCostumeRewards;

    if( game_state.editnpc ) // show everything in editnpc mode
        return true;

    if(!name) // unkeyed
        return true;

    if( !(*rewardTable) )
        return false;

    return stashFindElement((*rewardTable), name, NULL);
}

bool costumereward_findall(const char **keys, int isPCC)
{
    int i;
    for(i = eaSize(&keys)-1; i >= 0; --i)
        if(!costumereward_find(keys[i], isPCC))
            return false;
    return true;
}

//
// development functions
//-----------------------------------------------------------------------------------------------------------------

void saveVisualTextFile(void)
{
    Entity  *pe = playerPtr();
    Costume* costume;
    int i;

    // The player costume may be missing some important data fields.  (This will be fixed later).
    // Make a seperate copy of the player costume to be fixed up and saved to file.
    costume = costume_clone(pe->costume);

    if(!costume->appearance.entTypeFile)
        costume->appearance.entTypeFile = allocAddString(pe->seq->type->name);

    // Make sure each costume part has a proper name.
    //    Players costumes may not have this information yet.
    //    Fill them in if they are not there.
    for(i = 0; i < costume->appearance.iNumParts; i++)
    {
        char buf1[256],buf2[256];
        bool found1, found2;
        CostumePart* part = eaGet(&costume->parts, i);
        const BodyPart *bone = bodyPartList.bodyParts[i];
        if(!part->pchName)
            part->pchName = bodyPartList.bodyParts[i]->name;
        if (0==stricmp(part->pchName, bone->name)) {
            // We're on the bone I think we are
            if (0==determineTextureNames(bone, &costume->appearance, part->pchTex1, part->pchTex2, buf1+1, ARRAY_SIZE_CHECKED(buf1)-1, buf2+1, ARRAY_SIZE_CHECKED(buf2)-1, &found1, &found2)) {
                // Put in absolute names instead
                if (found1 && stricmp(buf1+1, "none")!=0)
                    buf1[0] = '!';
                else
                    strcpy(buf1, buf1+1);
                if (found2 && stricmp(buf2+1, "none")!=0)
                    buf2[0] = '!';
                else
                    strcpy(buf2, buf2+1);
                part->pchTex1 = allocAddString(buf1);
                part->pchTex2 = allocAddString(buf2);
            }
        }
    }

    while( eaSize( &costume->parts ) > costume->appearance.iNumParts )
    {
        CostumePart *part = eaRemove( &costume->parts, costume->appearance.iNumParts );
        part = 0;
    }

    costume_WriteTextFile(costume, "c:\\visuals.tmp");

    // Add a tab to all lines in front of the output file.
    {
        FILE* src;
        FILE* dest;
        char buffer[1024];
        int firstLine = 1;

        src = fopen("c:\\visuals.tmp", "rt");
        dest = fopen("c:\\visuals.txt", "wt");

        while(fgets(buffer, 1024, src))
        {
            char* b;
            if(isEmptyStr(buffer))
                continue;

            b = buffer;
            removeTrailingWhiteSpaces(&b);
            if(firstLine)
            {
                fprintf(dest, "\t");
                firstLine = 0;
            }
            else
            {
                fprintf(dest, "\n\t");
            }

            fwrite(buffer, 1, strlen(buffer), dest);
        }

        fclose(src);
        fclose(dest);
        remove("c:\\visuals.tmp");
    }
}

/* End of File */
