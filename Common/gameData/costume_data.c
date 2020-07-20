
#include "entity/character_base.h"
#include "gameData/costume_data.h"
#include "entity/entity.h"
#include "varutils.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/costume.h"
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "time.h"
#include <utilitieslib/utils/error.h>
#include "gameData/BodyPart.h"
#include <utilitieslib/components/SharedMemory.h>
#include "string.h"
#include "entity/entPlayer.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "entity/RewardToken.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/file.h> // isDevelopmentMode()
#include "entity/classes.h"
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/StringCache.h>
#include "account/AccountCatalog.h"

#if CLIENT
    #include "cmdparse/cmdgame.h"
    #include "UI/uiCostume.h"
    #include "UI/uiTailor.h"
    #include "UI/uidialog.h"
    #include "UI/uiSupercostume.h"
    #include "imageServer.h"
    #include "entity/costume_client.h"
    #include <utilitieslib/utils/FolderCache.h>
    #include <utilitieslib/utils/fileutil.h>
    #include "UI/uiGame.h"
    #include "seq/tricks.h"
    #include <utilitieslib/utils/strings_opt.h>
    #include "entity/entclient.h"
    #include <utilitieslib/utils/utils.h>
    #include "UI/Hybrid/uiHybridMenu.h"
    #include "player/inventory_client.h"
#elif SERVER
    #include "Reward.h"
    #include "cmdparse/cmdserver.h"
#endif

// Tailor Parsing definitions
//------------------------------------------------------------------------------------

TokenizerParseInfo ParseTailorCost[] =
{
    { "MinLevel",        TOK_INT(TailorCost, min_level, 0)        },
    { "MaxLevel",        TOK_INT(TailorCost, max_level, 0)        },
    { "EntryFee",        TOK_INT(TailorCost, entryfee, 0)        },
    { "Global",            TOK_INT(TailorCost, global, 0)            },
    { "HeadCost",        TOK_INT(TailorCost, head, 0)            },
    { "HeadSubCost",    TOK_INT(TailorCost, head_subitem, 0)    },
    { "UpperCost",        TOK_INT(TailorCost, upper, 0)            },
    { "UpperSubCost",    TOK_INT(TailorCost, upper_subitem, 0)    },
    { "LowerCost",        TOK_INT(TailorCost, lower, 0)            },
    { "LoserSubCost",    TOK_INT(TailorCost, lower_subitem, 0)    },
    { "NumCostumes",    TOK_INT(TailorCost, num_costumes, 0)    },
    { "End",            TOK_END                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTailorCostList[] =
{
    { "TailorCostSet", TOK_STRUCT(TailorCostList, tailor_cost, ParseTailorCost) },
    { "", 0, 0 }
};

SERVER_SHARED_MEMORY TailorCostList gTailorMaster = {0};

// Costume Parsing definitions
//------------------------------------------------------------------------------------

TokenizerParseInfo ParseMaskSet[] =
{
    { "Name",        TOK_STRING(CostumeMaskSet, name, 0)                        },
    { "displayName",TOK_STRING(CostumeMaskSet, displayName, 0)                },
    { "Keys",        TOK_STRINGARRAY(CostumeMaskSet, keys)                    },
    { "Key",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeMaskSet, keys)    },
    { "Product",    TOK_STRING(CostumeMaskSet, storeProduct, 0)                },
    // { "InternalProducts",    TOK_STRINGARRAY(CostumeMaskSet, internalProducts)    },
    { "Tag",        TOK_STRINGARRAY(CostumeMaskSet, tags)                    },
    { "Legacy",        TOK_INT(CostumeMaskSet, legacy, 0)                        },
    { "DevOnly",    TOK_INT(CostumeMaskSet, devOnly, 0)                        },
    { "COV",        TOK_INT(CostumeMaskSet, covOnly, 0)                        },
    { "COH",        TOK_INT(CostumeMaskSet, cohOnly, 0)                        },
    { "COHV",        TOK_INT(CostumeMaskSet, cohvShared, 0)                    },
    { "End",        TOK_END                                                    },
    { "EndMask",    TOK_END                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseColor[] =
{
    { "",            TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_F32_X, 0, 3 },
    { "\n",            TOK_END                                                },
    { "", 0, 0 }
};

TokenizerParseInfo ParseColorPalette[] =
{
    { "Color",         TOK_STRUCT(ColorPalette, color, ParseColor)            },
    { "Name",        TOK_POOL_STRING|TOK_STRING(ColorPalette, name, 0)    },
    { "End",        TOK_END                                                },
    { "EndPalette",    TOK_END                                                },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeCostumeTexSet[] =
{
    { "DisplayName",TOK_STRING(CostumeTexSet, displayName, 0)                },
    { "GeoName",    TOK_STRING(CostumeTexSet, geoDisplayName, 0)            },
    { "Geo",        TOK_STRING(CostumeTexSet, geoName, 0)                    },
    { "Tex1",        TOK_STRING(CostumeTexSet, texName1, 0)                    },
    { "Tex2",        TOK_STRING(CostumeTexSet, texName2, 0)                    },
    { "Fx",            TOK_STRING(CostumeTexSet, fxName, 0)                    },
    { "Product",    TOK_STRING(CostumeTexSet, storeProduct, 0)                },
    // { "InternalProducts",    TOK_STRINGARRAY(CostumeTexSet, internalProducts)    },
    { "Keys",        TOK_STRINGARRAY(CostumeTexSet, keys)                    },
    { "Key",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeTexSet, keys)    },
    { "Tag",        TOK_STRINGARRAY(CostumeTexSet, tags)                    },
    { "Flags",        TOK_STRINGARRAY(CostumeTexSet, flags)                    },
    { "Flag",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeTexSet, flags)    },
    { "DevOnly",    TOK_INT(CostumeTexSet, devOnly, 0)                        },
    { "COV",        TOK_INT(CostumeTexSet, covOnly, 0)                        },
    { "COH",        TOK_INT(CostumeTexSet, cohOnly, 0)                        },
    { "COHV",        TOK_INT(CostumeTexSet, cohvShared, 0)                    },
    { "IsMask",        TOK_INT(CostumeTexSet, isMask, 0)                        },
    { "Level",        TOK_INT(CostumeTexSet, levelReq, 0)                        },
    { "Legacy",        TOK_INT(CostumeTexSet, legacy, 0)                        },
    { "End",        TOK_END                                                    },
    { "EndTex",        TOK_END                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeFaceScaleSet[] = 
{
    { "DisplayName",TOK_STRING(CostumeFaceScaleSet, displayName, 0)    },
    { "Head",        TOK_VEC3(CostumeFaceScaleSet, head)                },
    { "Brow",        TOK_VEC3(CostumeFaceScaleSet, brow)                },
    { "Cheek",        TOK_VEC3(CostumeFaceScaleSet, cheek)            },
    { "Chin",        TOK_VEC3(CostumeFaceScaleSet, chin)                },
    { "Cranium",    TOK_VEC3(CostumeFaceScaleSet, cranium)            },
    { "Jaw",        TOK_VEC3(CostumeFaceScaleSet, jaw)                },
    { "Nose",        TOK_VEC3(CostumeFaceScaleSet, nose)                },
    { "COV",        TOK_INT(CostumeFaceScaleSet, covOnly, 0)        },
    { "COH",        TOK_INT(CostumeFaceScaleSet, cohOnly, 0)        },
    { "End",        TOK_END                                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeCostumeGeoSet[] =
{
    { "DisplayName",TOK_STRING(CostumeGeoSet, displayName, 0)                    },
    { "BodyPart",    TOK_STRING(CostumeGeoSet, bodyPartName, 0)                    },
    { "ColorLink",    TOK_STRING(CostumeGeoSet, colorLinked, 0)                    },
    { "Keys",        TOK_STRINGARRAY(CostumeGeoSet, keys)                        },
    { "Key",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeGeoSet, keys)        },
    { "Flags",        TOK_STRINGARRAY(CostumeGeoSet, flags)                        },
    { "Flag",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeGeoSet, flags)        },
    { "Product",    TOK_STRING(CostumeGeoSet, storeProduct, 0)                    },
    // { "InternalProducts",    TOK_STRINGARRAY(CostumeGeoSet, internalProducts)    },
    { "Type",        TOK_INT(CostumeGeoSet, type, 0)                                },
    { "AnimBits",    TOK_STRINGARRAY(CostumeGeoSet, bitnames)                    },
    { "ZoomBits",    TOK_STRINGARRAY(CostumeGeoSet, zoombitnames)                },
    { "DefaultView",TOK_VEC3(CostumeGeoSet, defaultPos)                            },
    { "ZoomView",    TOK_VEC3(CostumeGeoSet, zoomPos)                            },
    { "FourColor",    TOK_INT(CostumeGeoSet, numColor, 0)                            },
    { "NumColor",    TOK_REDUNDANTNAME|TOK_INT(CostumeGeoSet, numColor, 0)        },
    { "NoDisplay",    TOK_INT(CostumeGeoSet, isHidden, 0)                            },
    { "Info",         TOK_STRUCT(CostumeGeoSet, info, ParseCostumeCostumeTexSet)    },
    { "Mask",         TOK_STRUCT(CostumeGeoSet, mask, ParseMaskSet)                },
    { "Masks",        TOK_STRINGARRAY(CostumeGeoSet, masks)                        },
    { "MaskNames",    TOK_STRINGARRAY(CostumeGeoSet, maskNames)                    },
    { "Legacy",        TOK_INT(CostumeGeoSet, legacy, 0)                            },
    { "Face",        TOK_STRUCT(CostumeGeoSet, faces, ParseCostumeFaceScaleSet)    },
    { "COV",        TOK_INT(CostumeGeoSet, cov, 0)                                },
    { "COH",        TOK_INT(CostumeGeoSet, coh, 0)                                },
    { "End",        TOK_END                                                        },
    { "EndGeoSet",    TOK_END                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeBone[] =
{
    { "Name",        TOK_STRING(CostumeBoneSet, name, 0)                            },
    { "Filename",    TOK_CURRENTFILE(CostumeBoneSet, filename)                    },
    { "DisplayName",TOK_STRING(CostumeBoneSet, displayName, 0)                    },
    { "Keys",        TOK_STRINGARRAY(CostumeBoneSet, keys)                        },
    { "Key",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeBoneSet, keys)        },
    { "Product",    TOK_STRING(CostumeBoneSet, storeProduct, 0)                    },
    // { "InternalProducts",    TOK_STRINGARRAY(CostumeBoneSet, internalProducts)    },
    { "Flags",        TOK_STRINGARRAY(CostumeBoneSet, flags)                        },
    { "Flag",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeBoneSet, flags)    },
    { "GeoSet",        TOK_STRUCT(CostumeBoneSet, geo, ParseCostumeCostumeGeoSet)    },
    { "Legacy",        TOK_INT(CostumeBoneSet, legacy, 0),                            },
    { "COV",        TOK_INT(CostumeBoneSet, cov, 0)                                },
    { "COH",        TOK_INT(CostumeBoneSet, coh, 0)                                },
    { "End",        TOK_END                                                        },
    { "EndBoneSet",    TOK_END                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeRegion[] =
{
    { "Name",        TOK_STRING(CostumeRegionSet, name, 0)                        },
    { "Filename",    TOK_CURRENTFILE(CostumeRegionSet, filename)                    },
    { "DisplayName",TOK_STRING(CostumeRegionSet, displayName, 0)                },
    { "Keys",        TOK_STRINGARRAY(CostumeRegionSet, keys)                        },
    { "Key",        TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeRegionSet, keys)    },
    { "BoneSet",     TOK_STRUCT(CostumeRegionSet, boneSet, ParseCostumeBone)        },
    { "StoreCategory", TOK_STRING(CostumeRegionSet, storeCategory, 0)            },
    { "EndRegion",    TOK_END                                                        },
    { "End",        TOK_END                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeSetSet[] =
{
    { "Name",                TOK_STRING(CostumeSetSet, name, 0)                        },
    { "Filename",            TOK_CURRENTFILE(CostumeSetSet, filename)                },
    { "DisplayName",        TOK_STRING(CostumeSetSet, displayName, 0)                },
    { "NPCName",            TOK_STRING(CostumeSetSet, npcName, 0)                    },
    { "Keys",                TOK_STRINGARRAY(CostumeSetSet, keys)                    },
    { "Key",                TOK_REDUNDANTNAME|TOK_STRINGARRAY(CostumeSetSet, keys)    },
    { "ProductList",    TOK_STRINGARRAY(CostumeSetSet, storeProductList)        },
    { "End",                TOK_END                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeGeoData[] =
{
    { "Name",        TOK_STRUCTPARAM|TOK_STRING(CostumeGeoData, name, 0)    },
    { "Filename",    TOK_CURRENTFILE(CostumeGeoData, filename)            },
    { "ShieldXYZ",    TOK_VEC3(CostumeGeoData, shieldpos)                    },
    { "ShieldPYR",    TOK_VEC3(CostumeGeoData, shieldpyr)                    },
    { "End",        TOK_END                                                },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeOrigin [] =
{
    { "Name",        TOK_STRING(CostumeOriginSet, name, 0)                            },
    { "Filename",    TOK_CURRENTFILE(CostumeOriginSet, filename)                        },
    { "BodyPalette",TOK_STRUCT(CostumeOriginSet, bodyColors, ParseColorPalette)        },
    { "SkinPalette",TOK_STRUCT(CostumeOriginSet, skinColors, ParseColorPalette)        },
    { "PowerPalette",TOK_STRUCT(CostumeOriginSet, powerColors, ParseColorPalette)        },
    { "Region",     TOK_STRUCT(CostumeOriginSet, regionSet, ParseCostumeRegion)        },
    { "CostumeSet",    TOK_STRUCT(CostumeOriginSet, costumeSets, ParseCostumeSetSet)    },
    { "GeoData",    TOK_STRUCT(CostumeOriginSet, geoData, ParseCostumeGeoData)        },
    { "End",        TOK_END                                                            },
    { "EndOrigin",    TOK_END                                                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeBodySet [] =
{
    { "Name",    TOK_STRING(CostumeBodySet, name, 0)                            },
    { "Origin", TOK_STRUCT(CostumeBodySet, originSet, ParseCostumeOrigin)    },
    { "End",    TOK_END                                                        },
    { "EndBody",TOK_END                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeSet[] =
{
    { "Costume",    TOK_STRUCT(CostumeMasterList, bodySet, ParseCostumeBodySet)    },
    { "", 0, 0 }
};

SERVER_SHARED_MEMORY CostumeMasterList    gCostumeMaster = {0};
CostumeGeoSet        gSuperEmblem   = {0};
SERVER_SHARED_MEMORY ColorPalette        gSuperPalette  = {0};

#define CASE(s,ret) if (stricmp(gender, s)==0) return ret;
static const char *costumePrefixFromGender(const char *gender)
{
    CASE("Male",        "male");
    CASE("Female",        "fem");
    CASE("Huge",        "huge");
    CASE("BasicMale",    "bm");
    CASE("BasicFemale",    "bf");
    CASE("Villain",        "enemy");
    return "male";
}

static BodyType costumeBodyTypeFromGender(const char * gender)
{
    CASE("Male", kBodyType_Male);
    CASE("Female", kBodyType_Female);
    CASE("Huge", kBodyType_Huge);
    CASE("BasicMale", kBodyType_BasicMale);
    CASE("BasicFemale", kBodyType_BasicFemale);
    CASE("Villain", kBodyType_Enemy);
    return kBodyType_Male;
}

cStashTable costume_HashTable;
cStashTable costume_HashTableExtraInfo;
cStashTable costumeReward_HashTable;
cStashTable costume_PaletteTable;
static StashTable costume_TaggedPartTable;
cStashTable costume_GeoDataTable;

int getColorFromVec3( const Vec3 color )
{
    int r = color[0];
    int g = color[1];
    int b = color[2];

    return ( (r << 24) + (g << 16) + (b << 8) + 0x000000ff);
}

int getRGBAFromVec3( const Vec3 color )
{
    int r = color[0];
    int g = color[1];
    int b = color[2];

    return ( (r) + (g << 8) + (b << 16) + 0xff000000);
}

static void addPaletteToTables(const ColorPalette **palette)
{
    int k;
    for( k = 0; k < eaSize( (F32***)&palette[0]->color ); k++ )
    {
        int clr = getRGBAFromVec3( *palette[0]->color[k] );
        //printf("Adding color %08X\n", clr);
        // the value doesn't matter, the key is key
        if ( clr ) // if the color is 0, we can't stash it, but this is handled in the send/get mechanism
            stashIntAddIntConst(costume_PaletteTable, clr, 0xabcdef01, false); // value is arbitrary, a debug value
    }

}

typedef struct CostumeTaggedPart
{
#if CLIENT
    const CostumeRegionSet *rset;
    int boneSetIdx;
    const CostumeGeoSet *gset;
    int infoIdx;
#endif
    int maskIdx;
    CostumePart part;
} CostumeTaggedPart;

static CostumeTaggedPart* s_getTaggedPart(const char * tag, BodyType bodytype, int bRequirePart)
{
    CostumeTaggedPart *part;
    char * tag_body_modified;

    assert(costume_TaggedPartTable);

    estrCreate(&tag_body_modified);
    estrPrintf(&tag_body_modified, "%s_Bodytype%d", tag, bodytype);
    if (!stashFindPointer(costume_TaggedPartTable,tag_body_modified,&part))
    {
        if (bRequirePart)
        {
            part = calloc(1,sizeof(CostumeTaggedPart));
            part->part.color[0] = ColorWhite; // defaulting to white for now
            part->part.color[1] = ColorWhite; // currently, we're only going to use this for customizable weapons
            part->part.color[2] = ColorWhite; // if we do need to specify color, consider setting it outside, and
            part->part.color[3] = ColorWhite; // not making it be a part of the part tagging system
            part->maskIdx = -1;
            stashAddPointer(costume_TaggedPartTable,tag_body_modified,part,true);
        }
        else
        {
            part = NULL;
        }
    }
    estrDestroy(&tag_body_modified);
    return part;
}

CostumePart* costume_getTaggedPart(const char *tag, BodyType bodytype)
{
    if(costume_TaggedPartTable)
    {
        CostumeTaggedPart *part = s_getTaggedPart(tag, bodytype, false);
        if (part)
            return &part->part;
        else
            return NULL;
    }
    else
    {
        Errorf("Unknown Costume Part Tag %s",tag);
        return NULL;
    }
}

#if CLIENT
void costume_setTaggedPart(const char *tag)
{
    CostumeTaggedPart *part;
    if(costume_TaggedPartTable && stashFindPointer(costume_TaggedPartTable,tag,&part))
    {
        CostumeRegionSet*    mutable_rset = cpp_const_cast(CostumeRegionSet*)(part->rset);
        CostumeGeoSet*        mutable_gset = cpp_const_cast(CostumeGeoSet*)(part->gset);
        mutable_rset->currentBoneSet = part->boneSetIdx;
        mutable_gset->currentInfo = part->infoIdx;
        if(part->maskIdx >= 0)
            mutable_gset->currentMask = part->maskIdx;
    }
    else
    {
        Errorf("Unknown Costume Part Tag %s",tag);
    }
}
#endif

void costume_getShieldOffset(const char *geoname, Vec3 pos, Vec3 pyr)
{
    const CostumeGeoData *geodata;
    if(stashFindPointerConst(costume_GeoDataTable, geoname, &geodata) && devassert(geodata))
    {
        if(pos)
            copyVec3(geodata->shieldpos, pos);
        if(pyr)
            copyVec3(geodata->shieldpyr, pyr);
    }
}

static int s_costume_getGloveName(char **estr, const char *pre, const char *name)
{
    static const BodyPart *gloves;

    char *s;

    if(!gloves)
    {
        gloves = bpGetBodyPartFromName("Gloves");
        if(!gloves || gloves->num_parts < 2 || gloves->bone_num[1] < 0)
        {
            Errorf("Could not find glove body part, or part is wrong.");
            gloves = (BodyPart*)0x1; // don't init again
        }
    }

    if(gloves == (BodyPart*)0x1) // bad body part
        return 0;

    assert(pre);

    if(isNullOrNone(name)) // bad geo name
        return 0;

    // Auto-generate name via madness, this mimics doChangeGeo() with less nonsense
    estrClear(estr);
    if(s = strstri((char*)name, ".geo/"))
    {
        estrConcatFixedWidth(estr, name, s-name);
        if(strstriConst(*estr, "GLOVE"))
        {
            estrConcatf(estr, ".%s", s+5);
            if(s = strchr(*estr, '*'))
                *s = 'L';
        }
        else
        {
            Errorf("Invalid glove name %s for GeoData", name);
        }
    }
    else
    {
        estrConcatf(estr, "%s_%s.GEO_%sL_%s", pre, gloves->base_var, gloves->name_geo, name);
    }
    return 1;
}

static void s_addKeys(const char **keys)
{
    int i;
    for(i = eaSize(&keys)-1; i >= 0; --i)
    {
        if(stashAddPointerConst(costumeReward_HashTable, keys[i], 0 , false))
        {
#if SERVER
            rewardAddCostumePartToHashTable(keys[i]);
#endif
        }
    }
}

void costume_initHashTable(void)
{
    char *c;
    const char *costumePrefix=NULL;
    char *buf = estrTemp();
    int i, j, k, l, m, n, o=0;
    int server = 0;

#if SERVER
    server = 1;
#endif

    if( !costume_HashTable )
    {
        // Actual size of 1240, grows to 4096
        costume_HashTable = stashTableCreateWithStringKeys(4096, StashDefault );
        // Actual size of 57, grows to 128
        costume_HashTableExtraInfo = stashTableCreateWithStringKeys(256, StashDefault );
    }

    if( !costumeReward_HashTable )
    {
        costumeReward_HashTable = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);
    }

    if( !costume_PaletteTable ) {
        costume_PaletteTable = stashTableCreateInt(128);
    }

    if(!costume_TaggedPartTable)
        costume_TaggedPartTable = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);

    if(costume_GeoDataTable)
        stashTableClearConst(costume_GeoDataTable);
    else
        costume_GeoDataTable = stashTableCreateWithStringKeys(1024, StashDeepCopyKeys);

    {
        const ColorPalette *pSuperPalette = &gSuperPalette;
        addPaletteToTables(&pSuperPalette);
    }

    for( i = eaSize(&gCostumeMaster.bodySet)-1; i >= 0; i-- )
    {
        if( gCostumeMaster.bodySet[i]->name )
            stashAddPointerConst(costume_HashTableExtraInfo, gCostumeMaster.bodySet[i]->name, 0, false);
        costumePrefix = costumePrefixFromGender(gCostumeMaster.bodySet[i]->name);
        for( j = eaSize(&gCostumeMaster.bodySet[i]->originSet)-1; j >= 0; j-- )
        {
            const CostumeOriginSet *originSet = gCostumeMaster.bodySet[i]->originSet[j];
            // Add colors to palette table
            addPaletteToTables(originSet->skinColors);
            addPaletteToTables(originSet->bodyColors);

            for(k = eaSize(&originSet->geoData)-1; k >= 0; --k)
            {
                const CostumeGeoData *geodata = originSet->geoData[k];

                // index shield offset information by glove modelname
                if(!vec3IsZero(geodata->shieldpos) || !vec3IsZero(geodata->shieldpyr))
                    if(s_costume_getGloveName(&buf, costumePrefix, geodata->name))
                        if(!stashAddPointerConst(costume_GeoDataTable, buf, geodata, false))
                            ErrorFilenamef(geodata->filename, "Warning: duplicate GeoData name %s", geodata->name);
            }

            // Add strings to costume string hash table
            for( k = eaSize(&originSet->regionSet)-1; k >= 0; k--)
            {
                const CostumeRegionSet * rset = originSet->regionSet[k];
                
                if( rset->name )
                    stashAddPointerConst(costume_HashTableExtraInfo, rset->name, 0 , false);
                s_addKeys(rset->keys);
                for( l = eaSize( &rset->boneSet )-1; l >= 0; l--)
                {
                    CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[l]);
                    if( bset->name )
                        stashAddPointerConst(costume_HashTableExtraInfo, bset->name, 0 , false);
                    //if( bset->displayName)
                    //    stashAddPointerConst(costume_HashTableExtraInfo, bset->displayName, 0 , false);
                    s_addKeys(bset->keys);

                    for( m = eaSize(&bset->geo )-1; m >= 0; m--)
                    {
                        const CostumeGeoSet *gset = bset->geo[m];
                        s_addKeys(gset->keys);
                        for( n = eaSize(&gset->info)-1; n >= 0; n-- )
                        {
                            const CostumeTexSet *texSet = gset->info[n];

                            if( texSet->geoName )
                                stashAddPointerConst(costume_HashTable, texSet->geoName, 0, false);

                            if( texSet->texName1 )
                            {
                                if (c = strchr(texSet->texName1, '.'))
                                    *c=0;
                                stashAddPointerConst(costume_HashTable, texSet->texName1, 0, false);
                            }

                            if( texSet->texName2 )
                            {
                                if (c = strchr(texSet->texName2, '.'))
                                    *c=0;
                                stashAddPointerConst(costume_HashTable, texSet->texName2, 0 , false);
                            }

                            if( texSet->fxName )
                                stashAddPointerConst(costume_HashTable, texSet->fxName, 0 , false);

                            s_addKeys(texSet->keys);

                            for(o = eaSize(&texSet->tags)-1; o >= 0; --o)
                            {
                                CostumeTaggedPart *tagged = s_getTaggedPart(texSet->tags[o], costumeBodyTypeFromGender(gCostumeMaster.bodySet[i]->name), true);

                                // make the part easy to locate in the editor
#if CLIENT
                                tagged->rset = rset;
                                tagged->boneSetIdx = l;
                                tagged->gset = gset;
                                tagged->infoIdx = n;
#endif

                                // then generate the part for use in attaching to old players (this could be server-only)
                                tagged->part.displayName = gset->displayName;
                                tagged->part.bodySetName = bset->name;
                                tagged->part.regionName = rset->name;
                                tagged->part.pchName = gset->bodyPartName;
                                tagged->part.pchGeom = texSet->geoName;
                                tagged->part.pchTex1 = texSet->texName1;
                                tagged->part.pchFxName = texSet->fxName;
                                if( (texSet->texName2 && !texSet->texMasked && stricmp(texSet->texName2,"Mask")==0) ||
                                    !tagged->part.pchTex2 )
                                {
                                    tagged->part.pchTex2 = texSet->texName2;
                                }
                                // else it'll be set below
                            }
                        }

                        for( n = eaSize(&gset->masks)-1; n >= 0; n-- )
                        {
                            if( gset->masks[n] )
                                stashAddPointerConst(costume_HashTable, gset->masks[n], 0 , false);
                        }

                        for( n = eaSize(&gset->mask)-1; n >= 0; n-- )
                        {
                            const CostumeMaskSet *maskSet = gset->mask[n];
                            if( maskSet->name )
                                stashAddPointerConst(costume_HashTable, maskSet->name, 0 , false);
                            s_addKeys(maskSet->keys);

                            for(o = eaSize(&maskSet->tags)-1; o >= 0; --o)
                            {
                                CostumeTaggedPart* tagged = s_getTaggedPart(maskSet->tags[o], costumeBodyTypeFromGender(gCostumeMaster.bodySet[i]->name), true);
                                tagged->maskIdx = n;
                                tagged->part.pchTex2 = maskSet->name;
                            }
                        }
                    }
                }
            }
        }
    }

    estrDestroy(&buf);
}

//determines if a part is dev only
//this is the case for devOnly marked pieces and
//pieces that have not been published yet
bool isRestrictedPart( const void * part, int type) // 0 = tset, 1 = mset
{
    int dev = 0;

    if( type == 1)
    {
        const CostumeMaskSet * mset = cpp_reinterpret_cast(const CostumeMaskSet*)(part);
#if CLIENT
        dev = mset->devOnly | maskIsLegacy(mset);
#else
        dev = mset->devOnly | mset->legacy;
#endif

#if CLIENT
        // if you have the key or product code or there is a published stored product, the set is not restricted
        if(!costumereward_findall(mset->keys, getPCCEditingMode() ))
        {
            if(mset->storeProduct == NULL)
                dev = true;
            else if(mset->storeProduct != NULL && !AccountHasStoreProductOrIsPublished(inventoryClient_GetAcctInventorySet(), skuIdFromString(mset->storeProduct)))
                dev = true;
        }
#endif

#if SERVER
        if (mset->storeProduct != NULL && !accountCatalogIsSkuPublished(skuIdFromString(mset->storeProduct)))
            dev = true;
#endif
    }
    else if (type == 0)
    {
        const CostumeTexSet * tset = cpp_reinterpret_cast(const CostumeTexSet*)(part);
#if CLIENT
        dev = tset->devOnly|partIsLegacy(tset);
#else
        dev = tset->devOnly|tset->legacy;
#endif 

#if CLIENT
        // if you have the key or product code or there is a published stored product, the set is not restricted
        if(!costumereward_findall(tset->keys, getPCCEditingMode() ))
        {
            if(tset->storeProduct == NULL)
                dev = true;
            else if(tset->storeProduct != NULL && !AccountHasStoreProductOrIsPublished(inventoryClient_GetAcctInventorySet(), skuIdFromString(tset->storeProduct)))
                dev = true;
        }
#endif

#if SERVER
        if (tset->storeProduct != NULL && !accountCatalogIsSkuPublished(skuIdFromString(tset->storeProduct)))
            dev = true;
#endif
    }

    return dev;
}

bool costume_fillExtraData( ParseTable pti[], CostumeMasterList * costumeMaster, bool shared_memory )
{
    int i, j, k, m, n, p;
    int editnpc = 0;
    char *estrbuf = estrTemp();
#if CLIENT
    editnpc = game_state.editnpc;
#endif

    costumeMaster->bodyCount = eaSize(&costumeMaster->bodySet);

    for( i = 0; i < costumeMaster->bodyCount; i++ )
    {
        CostumeBodySet* bodyset = cpp_const_cast(CostumeBodySet*)(costumeMaster->bodySet[i]);
        bodyset->originCount = eaSize(&bodyset->originSet);

        if( !bodyset->name && editnpc )
            Errorf( "Costume Error, Missing Body Set Name" );

        for( j = 0; j < bodyset->originCount; j++ )
        {
            CostumeOriginSet *oset = cpp_const_cast(CostumeOriginSet*)(bodyset->originSet[j]);
            oset->regionCount = eaSize(&oset->regionSet);

            if( !oset->name && editnpc )
                ErrorFilenamef(oset->filename, "Costume Error, Missing Origin Set Name :(%s -> %s)", bodyset->name, oset->name );

            for( k = 0; k < oset->regionCount; k++ )
            {
                CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(oset->regionSet[k]);
                bool bFoundBoneSet = false;
                StashTable costume_BoneSetNames = stashTableCreateWithStringKeys(256, StashDefault );

                rset->idx = k;
                rset->boneCount = eaSize(&rset->boneSet);

                if( !rset->name && editnpc )
                    ErrorFilenamef( rset->filename, "Costume Error, Missing Region Set Name :(%s -> %s -> %s)", bodyset->name, oset->name, rset->name );

                for( m = 0; m < rset->boneCount; m++ )
                {
                    CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[m]);
                    bset->idx = m;
                    bset->rsetParent = rset;
                    bset->geoCount = eaSize(&bset->geo);
                    bset->restricted = bset->cohOnly = bset->covOnly = false; // cov/coh deprecated, just set them to false

                    if( !bset->name && editnpc )
                        ErrorFilenamef( rset->filename, "Costume Error, Missing Bone Set Name :(%s -> %s -> %s -> %s)", bodyset->name, oset->name, rset->name, bset->name );

                    if( stashFindElement(costume_BoneSetNames,bset->name,NULL) )
                        ErrorFilenamef( rset->filename, "Costume Error, Duplicate Bone Set Name :(%s -> %s -> %s -> %s)", bodyset->name, oset->name, rset->name, bset->name );
                    stashAddPointer(costume_BoneSetNames, bset->name, 0, 0 );

                    for( n = 0; n < bset->geoCount; n++ )
                    {
                        CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[n]);
                        bool bFoundTexSet = false, bFoundMask = false;
                        gset->restricted = gset->cohOnly = gset->covOnly = false; // cov/coh deprecated, just set them to false
#if CLIENT
                        if (!gset->hb)
                            gset->hb = calloc(1,sizeof(HybridElement));
#endif
                        if( !gset->bodyPartName && !eaSize(&gset->faces) )
                        {
                            ErrorFilenamef( bset->filename, "Costume Error, GeoSet Missing BodyPart Name :(%s -> %s -> %s -> %s -> %s)", bodyset->name, oset->name, rset->name, bset->name, gset->bodyPartName );
                            gset->restricted = true;
                            continue;
                        }

                        gset->idx = n;
                        gset->bsetParent = bset;
                        gset->infoCount = eaSize(&gset->info);
                        gset->maskCount = eaSize(&gset->mask);
                        gset->faceCount = eaSize(&gset->faces);

                        for( p = 0; p < gset->infoCount; p++ )
                        {
                            int q;
                            CostumeTexSet *a = cpp_const_cast(CostumeTexSet*)(gset->info[p]);

                            if(!isRestrictedPart(a, 0))
                            {
                                bFoundTexSet = true;
                            }
                            a->idx = p;
                            a->gsetParent = gset;

                            for(q = p+1; q <= p+1 && q < gset->infoCount && rset->idx < 3; q++ ) // check one past
                            {
                                CostumeTexSet *b = cpp_const_cast(CostumeTexSet*)(gset->info[q]);

                                if( stricmp( (a->geoName?a->geoName:""), (b->geoName?b->geoName:"") ) == 0 && //must match geo and tex1
                                    stricmp( (a->texName1?a->texName1:""), (b->texName1?b->texName1:"") )==0 &&
                                    !(
                                        #if CLIENT
                                            partIsLegacy(a) || partIsLegacy(b) ||
                                        #else
                                            a->legacy || b->legacy ||
                                        #endif
                                        ((eaSize(&a->keys) && !a->isMask) && (eaSize(&b->keys) && !b->isMask)) || // don't combine if they are special
                                        (a->devOnly && !b->devOnly) ||
                                        (b->devOnly && !a->devOnly)
                                        ) )
                                {
                                    if( a->texName2 && stricmp( a->texName2, "mask" )==0 )
                                    {
                                        b->texMasked = true;
                                        a->legacy = true;
                                    }
                                    if( b->texName2 && stricmp( b->texName2, "mask" )==0 )
                                    {
                                        a->texMasked = true;
                                        b->legacy = true;
                                    }
                                }
                            }
                        }

                        for( p = 0; p < gset->maskCount; p++ )
                        {
                            CostumeMaskSet *mask = cpp_const_cast(CostumeMaskSet*)(gset->mask[p]);
                            if(!isRestrictedPart(mask, 1))
                            {
                                bFoundMask = true;
                            }
                            mask->idx = p;
                            mask->gsetParent = gset;
                        }

                        for( p = 0; p < gset->faceCount; p++ )
                        {
                            CostumeFaceScaleSet *face = cpp_const_cast(CostumeFaceScaleSet*)(gset->faces[p]);
                            face->idx = p;
                            face->gsetParent = gset;
                        }

                        if( gset->infoCount && !bFoundTexSet )
                        {
                            if( editnpc )
                                ErrorFilenamef( bset->filename, "COH/COV will not work right with current costume files.  Costume GeoSet (%s -> %s -> %s -> %s -> %s) has texset with no valid entries", bodyset->name, oset->name, rset->name, bset->name, gset->bodyPartName );
                            else
                                gset->restricted = true;
                        }

                        if( gset->maskCount && !bFoundMask )
                        {
                            if( editnpc )
                                ErrorFilenamef( bset->filename, "COH/COV will not work right with current costume files.  Costume GeoSet (%s -> %s -> %s -> %s -> %s) has maskset with no valid masks", bodyset->name, oset->name, rset->name, bset->name, gset->bodyPartName );
                            else
                                gset->restricted = true;
                        }

                        if( gset->restricted && !editnpc )
                            bset->restricted = true;
                    }
                
                    if( !bset->restricted )
                        bFoundBoneSet = true;
                }

                stashTableDestroy(costume_BoneSetNames);
                costume_BoneSetNames = NULL;

                if( rset->idx < 3 )
                {
                    if( !bFoundBoneSet && !editnpc  )
                        ErrorFilenamef( rset->filename, "COV and COH will not work right with curent costume files. Costume Region Set has no viable bonesets: %s -> %s -> %s", bodyset->name, oset->name, rset->name );
                }
            }
        }
    }

    estrDestroy(&estrbuf);
    return true;
}

static bool costume_preprocess( ParseTable pti[], CostumeMasterList * costumeMaster )
{
    char *c;
    int i, j, k, l, m, n, o=0;

    for( i = eaSize(&costumeMaster->bodySet)-1; i >= 0; i-- )
    {
        for( j = eaSize(&costumeMaster->bodySet[i]->originSet)-1; j >= 0; j-- )
        {
            CostumeOriginSet *originSet = cpp_const_cast(CostumeOriginSet*)(costumeMaster->bodySet[i]->originSet[j]);

            for(k = eaSize(&originSet->geoData)-1; k >= 0; --k)
            {
                CostumeGeoData *geoData = cpp_const_cast(CostumeGeoData*)(originSet->geoData[k]);
                RADVEC3(geoData->shieldpyr);
            }

            for( k = eaSize(&originSet->regionSet)-1; k >= 0; k--)
            {
                CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(originSet->regionSet[k]);
                
                for( l = eaSize( &rset->boneSet )-1; l >= 0; l--)
                {
                    CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[l]);
                    for( m = eaSize(&bset->geo )-1; m >= 0; m--)
                    {
                        CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[m]);
                        for( n = eaSize(&gset->info)-1; n >= 0; n-- )
                        {
                            CostumeTexSet *texSet = cpp_const_cast(CostumeTexSet*)(gset->info[n]);

                            // make presentable for vars.attribute
                            forwardSlashes(cpp_const_cast(char*)(texSet->geoName));
                            forwardSlashes(cpp_const_cast(char*)(texSet->fxName));

                            if( texSet->texName1 )
                            {
                                if (c = strchr(texSet->texName1, '.'))
                                    *c=0;
                                if(stricmp(texSet->texName1, "!none")==0)
                                    StructReallocString(&cpp_const_cast(char*)(texSet->texName1), "none");
                            }

                            if( texSet->texName2 )
                            {
                                if (c = strchr(texSet->texName2, '.'))
                                    *c=0;
                                if(stricmp(texSet->texName2, "!none")==0)
                                    StructReallocString(&cpp_const_cast(char*)(texSet->texName2), "none");
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

#if CLIENT && !TEST_CLIENT        // Test clients can use shared memory for costume data and do not have to verify

static int costumeVerifyTextures( void * unused, CostumeMasterList * costumeMaster  )
{
    char    tmp[MAX_NAME_LEN], tmp1[ MAX_NAME_LEN ];
    int i, j, k, l, m, n, o=0;

    if (isProductionMode())
        return 1;
#ifdef TEST_CLIENT
    return 1;
#endif

    for( i = eaSize(&costumeMaster->bodySet)-1; i >= 0; i-- )
    {
        const CostumeBodySet *bodySet = costumeMaster->bodySet[i];
        Appearance dummy_appearance;
        if (!(stricmp(bodySet->name, "male")==0 || stricmp(bodySet->name, "fem")==0 || stricmp(bodySet->name, "huge")==0))
            continue;
        dummy_appearance.bodytype = GetBodyTypeForEntType(bodySet->name);
        for( j = eaSize(&bodySet->originSet)-1; j >= 0; j-- )
        {
            const CostumeOriginSet *originSet = costumeMaster->bodySet[i]->originSet[j];
            for( k = eaSize(&originSet->regionSet)-1; k >= 0; k--)
            {
                const CostumeRegionSet * rset = originSet->regionSet[k];

                for( l = eaSize( &rset->boneSet )-1; l >= 0; l--)
                {
                    const CostumeBoneSet *bset = rset->boneSet[l];
                    if (bset->devOnly)
                        continue;
                    for( m = eaSize(&bset->geo )-1; m >= 0; m--)
                    {
                        const CostumeGeoSet *gset = rset->boneSet[l]->geo[m];
                        const BodyPart *bodyPart = gset->bodyPartName?bpGetBodyPartFromName(gset->bodyPartName):NULL;

                        if (gset->devOnly)
                            continue;
                        if (!bodyPart && gset->bodyPartName)
                            ErrorFilenamef(bset->filename, "Invalid body part name: %s (%s.%s.%s)", gset->bodyPartName, originSet->name, rset->name, bset->name);
                        for( n = eaSize(&gset->info)-1; n >= 0; n-- )
                        {
                            const CostumeTexSet *texSet = gset->info[n];
                            int bad;
                            bool found1, found2;
                            if (texSet->devOnly)
                                continue;
                            if (bodyPart->bone_num[0] != -1) {
                                bad = determineTextureNames(bodyPart, &dummy_appearance, texSet->texName1, texSet->texName2, SAFESTR(tmp), SAFESTR(tmp1), &found1, &found2);
                                if (bad) {
                                    //printf("");
                                }
                                if (!found1 && stricmp(texSet->texName1, "none")!=0 && !quickload) {
                                    ErrorFilenamef(bset->filename, "Player texture 1 '%s' (%s) not found (%s.%s.%s.%s)", tmp, texSet->texName1, originSet->name, rset->name, bset->name, gset->bodyPartName);
                                }
                                if (!found2 && stricmp(texSet->texName2, "mask")!=0 && !quickload) {
                                    ErrorFilenamef(bset->filename, "Player texture 2 '%s' (%s) not found (%s.%s.%s.%s)", tmp1, texSet->texName2, originSet->name, rset->name, bset->name, gset->bodyPartName);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int loadCostume( CostumeMasterList * master, char * path )
{
    if (!ParserLoadFiles(NULL, "Menu/Costume/costume.ctm", NULL, 0, ParseCostumeSet, master, NULL, NULL, costume_preprocess))
    {
        printf("Couldn't load Costume!!\n");
        return 0;
    }

    costume_fillExtraData( NULL, master, false );
    return 1;
}

static void reloadCostumesCallback(const char *relpath, int when);

void loadCostumes()
{
    char *fileToLoad = "Menu/Costume/Localized/Costume.ctm";

//    if (isDevelopmentMode())
    if (game_state.editnpc)
        fileToLoad = "Menu/Costume/Costume.ctm"; // So we don't have to work in the localized directory

    writeConsole(OUTPUT_DEBUG, "Loading costume bins");
    if (!ParserLoadFiles(NULL, fileToLoad, "costume.bin", 0, ParseCostumeSet, &gCostumeMaster, NULL, NULL, costume_preprocess))
    {
        printf("Couldn't load Costume!!\n");
    }

    costume_fillExtraData( NULL, &gCostumeMaster, false );

    if (!ParserLoadFiles(NULL, "Menu/Costume/Localized/Common/supergroupEmblem.ctm", "supergroupEmblems.bin", 0, ParseCostumeCostumeGeoSet, &gSuperEmblem, NULL, NULL, NULL))
    {
        printf("Couldn't load supergroup emblems!!\n");
    }

    if (!ParserLoadFiles(NULL, "Menu/Costume/ColorPalettes/supergroupColors.ctm", "supergroupColors.bin", 0, ParseColorPalette, &gSuperPalette, NULL, NULL, NULL))
    {
        printf("Couldn't load supergroup colors!!\n");
    }

    if (!ParserLoadFiles(NULL, "Menu/Costume/TailorCost.ctm", "tailorcost.bin", 0, ParseTailorCostList, &gTailorMaster, NULL, NULL, NULL))
    {
        printf("Couldn't load Tailor Data!!\n");
    }

    costume_initHashTable();
    costume_fillExtraData( NULL, &gCostumeMaster, false );
    costumeVerifyTextures( NULL, &gCostumeMaster );

    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "menu/Costume/*.ctm", reloadCostumesCallback);
    writeConsole(OUTPUT_INFO, "Loaded costume bins");
}

void reloadCostumes(void)
{
    char *fileToLoad = "Menu/Costume/Localized/Costume.ctm";

    if (game_state.editnpc)
        fileToLoad = "Menu/Costume/Costume.ctm"; // So we don't have to work in the localized directory

    loadstart_printf("Reloading Costume..." );

    // Don't free data, we leave it around (leak like crazy)
    memset(&gCostumeMaster, 0, sizeof(gCostumeMaster));

    if (!ParserLoadFiles(NULL, fileToLoad, "costume.bin", 0, ParseCostumeSet, &gCostumeMaster, NULL, NULL, costume_preprocess ))
    {
        loadend_printf("Couldn't reload costume!!\n");
    }
    else 
    {

        stashTableDestroyConst(costume_HashTable);
        costume_HashTable = NULL;
        stashTableDestroyConst(costume_HashTableExtraInfo);
        costume_HashTableExtraInfo = NULL;
        stashTableDestroyConst(costumeReward_HashTable);
        costumeReward_HashTable= NULL;
        stashTableDestroyConst(costume_PaletteTable);
        costume_PaletteTable = NULL;

        costume_initHashTable();
        costume_fillExtraData(NULL, &gCostumeMaster, false);

        if (isMenu(MENU_COSTUME) || isMenu(MENU_TAILOR) || isMenu(MENU_GENDER) || isMenu(MENU_BODY)) 
        {
            tailor_RevertCostumePieces(playerPtr()); // Try to keep the current costume
            updateCostume(); // tell the costume UI to update it
        } 
        else
            entResetTrickFlags();

        loadend_printf("done.");
    }
    costumeVerifyTextures( NULL, &gCostumeMaster );
}

static void reloadCostumesCallback(const char *relpath, int when)
{
    int enpcsave=0;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    enpcsave = game_state.editnpc;
    game_state.editnpc = 1;
    reloadCostumes();
    game_state.editnpc = enpcsave;
}


#elif SERVER || TEST_CLIENT

void loadCostumes()
{
    char *fileToLoad = "Menu/Costume/Localized/Costume.ctm";

//    if (isDevelopmentMode())
//        fileToLoad = "Menu/Costume/Costume.ctm"; // So we don't have to work in the localized directory

    loadstart_printf("Loading Costume..." );

    if (!ParserLoadFilesShared(MakeSharedMemoryName("costume.bin"), NULL, fileToLoad, "costume.bin", 0, ParseCostumeSet, &gCostumeMaster, sizeof(gCostumeMaster), NULL, NULL, costume_preprocess, NULL, costume_fillExtraData))
    {
        printf("Couldn't load Costume!!\n");
    }

    // Needed for hashtable (and validation?)
    if (!ParserLoadFilesShared(MakeSharedMemoryName("supergroupColors.bin"), NULL, "Menu/Costume/ColorPalettes/supergroupColors.ctm", "supergroupColors.bin", 0, ParseColorPalette, &gSuperPalette, sizeof(gSuperPalette), NULL, NULL, NULL, NULL, NULL))
    {
        printf("Couldn't load Costume!!\n");
    }

    if (!ParserLoadFilesShared(MakeSharedMemoryName("tailorcost.bin"), NULL, "Menu/Costume/TailorCost.ctm", "tailorcost.bin", 0, ParseTailorCostList, &gTailorMaster, sizeof(gTailorMaster), NULL, NULL, NULL, NULL, NULL))
    {
        printf("Couldn't load Tailor Data!!\n");
    }

    costume_initHashTable();

    loadend_printf("");
}

#endif

int costume_checkOriginForClassAndSlot( const char * pchOrigin, const CharacterClass * pclass, int slotid )
{
    int retval = 0;

    // Villain EATs are only locked to their uniform in the first slot. If
    // there's no class, the character is probably corrupt in some other way
    // and we should assume the costume for a default character.
    if (slotid == 0 && pclass)
    {
        // Villain EAT costumes are stored in the corresponding 'origin' tree.
        // They can only use that tree - NOT the 'all' tree as well.
        if( class_MatchesSpecialRestriction( pclass, "ArachnosSoldier" ) )
        {
            if( stricmp( pchOrigin, "ArachnosSoldier" ) == 0 )
                retval = 1;
        }
        else if( class_MatchesSpecialRestriction( pclass, "ArachnosWidow" ) )
        {
            if( stricmp( pchOrigin, "ArachnosWidow" ) == 0 )
                retval = 1;
        }
        else
        {
            if( stricmp( pchOrigin, "all" ) == 0 )
                retval = 1;
        }
    }
    else
    {
        if( stricmp( pchOrigin, "all" ) == 0 )
        {
            retval = 1;
        }
    }

    return retval;
}

static int s_checkKeys(const char **keys, RewardToken **playerKeys, int isPCC )
{
#if SERVER
    int i;
    for(i = eaSize(&keys)-1; i >= 0; --i)
    {
        int j;
        for(j = eaSize(&playerKeys)-1; j >= 0; --j)
            if(playerKeys[j]->reward && stricmp(playerKeys[j]->reward, keys[i]) == 0)
                break;
        if(j < 0)
            return 1;
    }
    return 0;
#else
    return !costumereward_findall(keys, isPCC);
#endif
}

int AccountHasInternalProductCode(const char ** internalProductCodes, AccountInventorySet* invSet)
{
    int i;
    for(i = eaSize(&internalProductCodes)-1; i >= 0; --i)
    {
        if(AccountHasStoreProduct(invSet, skuIdFromString(internalProductCodes[i])))
            return 1;
    }
    return 0;
}

//checks to see if we have the proper key/store code unlock
int isInvalidPart(RewardToken ** playerKeys, int isPCC, int allowStoreParts, bool checkPublished, AccountInventorySet* invSet, const char ** costumeKeys, const char * productCode, const char ** internalProductCodes)
{
    int hasCostumeKey = costumeKeys != NULL;
    int hasProductCode = productCode != NULL;
    int hasInternalProductCodes = internalProductCodes != NULL;

    if (hasCostumeKey || hasProductCode || hasInternalProductCodes)
    {
        int playerMissingKey = s_checkKeys(costumeKeys, playerKeys, isPCC);
        int playerMissingInternalCode = internalProductCodes ? !AccountHasInternalProductCode(internalProductCodes, invSet) : 0;
        int playerMissingCode = productCode ? (checkPublished ? !AccountHasStoreProductOrIsPublished(invSet, skuIdFromString(productCode)) : !AccountHasStoreProduct(invSet, skuIdFromString(productCode))) : 0;

        if ((hasCostumeKey && !playerMissingKey) || 
            (hasInternalProductCodes && !playerMissingInternalCode) ||
            (hasProductCode && (allowStoreParts || !playerMissingCode)))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }

}

int costume_validatePart( BodyType bodytype, int bpid, const CostumePart *part, Entity *e, int isPCC, int allowStoreParts, int slotid )
{
    int i, j, k, l ,m ,n;

    int bMatchGeo = false;
    int bMatchTex1 = false;
    int bMatchTex2 = false;

    const char *bodySetName;
    RewardToken **playerKeys = NULL;
    const CharacterClass *pclass = NULL;
    AccountInventorySet * invSet;
    const char *pchName = bodyPartList.bodyParts[bpid]->name;
    
    if (!pchName)
        return false;        //    the body part isn't defined. This part should never be valid
                            //    did the bodyPart file get corrupted?

    if (e)
    {
        //    this should be the normal case
        if (!isPCC)
        {
            // this checking is for SoA, 
            // ignore this for PCC's. PCC's dont want to be referencing the entity characteristics anyways. -EL
            pclass = e->pchar->pclass;
        }
#if SERVER
        playerKeys = isPCC ? e->pl->ArchitectRewardTokens : e->pl->rewardTokens;
#endif
    }

    if( !part->pchGeom || !stricmp( part->pchGeom, "none" )  )
    {
        if( stricmp( pchName, "pants" ) == 0 ||  // These parts must be set
            stricmp( pchName, "chest" ) == 0 ||
            stricmp( pchName, "gloves") == 0 ||
            stricmp( pchName, "boots" ) == 0 )
        {
            return false;
        }
        else
            bMatchGeo = true;
    }

    if( !part->pchTex1 || !stricmp( part->pchTex1, "base" ) || !stricmp( part->pchTex1, "none" ) )
    {
        if( stricmp( pchName, "pants" ) == 0 ||  // These parts must be set
            stricmp( pchName, "chest" ) == 0 ||
            stricmp( pchName, "gloves") == 0 ||
            stricmp( pchName, "boots" ) == 0 )
        {
            return false;
        }
        else
            bMatchTex1 = true;
    }

    if( !part->pchTex2  || !stricmp( part->pchTex2, "base" ) || !stricmp( part->pchTex2, "none" ) )
        bMatchTex2 = true;

    if( bMatchGeo && bMatchTex1 && bMatchTex2 && isNullOrNone(part->pchFxName) )
        return true;

    if(!EAINRANGE(bodytype,gCostumeMaster.bodySet))
        return false;

    if(bodytype == kBodyType_Male)
        bodySetName = "Male";
    else if(bodytype == kBodyType_Female)
        bodySetName = "Female";
    else if(bodytype == kBodyType_Huge)
        bodySetName = "Huge";
    else
        return false;

    // HACK: costumeAwardAuthParts() will not properly award the whisp aura if the account inventory has not be received yet.
    // So, assume it is valid
    // TODO: since we will be moving away from auth user bits to account inventory and costumeAwardAuthParts() relies
    // on auth user bits, a long term solution to this problem is required.
    if (e && e->pl && e->pl->account_inv_loaded == 0 && stricmp( pchName, "aura") == 0 && strstriConst(part->pchFxName, "wisps") != NULL)
        return true;

#ifdef CLIENT
    invSet = inventoryClient_GetAcctInventorySet();
#else
    invSet = ent_GetProductInventory(e);
#endif

    for(i = eaSize(&gCostumeMaster.bodySet)-1; i >= 0; --i)
    {
        if(stricmp(gCostumeMaster.bodySet[i]->name,bodySetName))
            continue;

        for( j = eaSize(&gCostumeMaster.bodySet[i]->originSet)-1; j >= 0; j-- )
        {
            if(!costume_checkOriginForClassAndSlot(gCostumeMaster.bodySet[i]->originSet[j]->name,pclass,slotid))
                continue;

            for( k = eaSize(&gCostumeMaster.bodySet[i]->originSet[j]->regionSet)-1; k >= 0; k--)
            {
                CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(gCostumeMaster.bodySet[i]->originSet[j]->regionSet[k]);

                if(e && s_checkKeys(rset->keys, playerKeys, isPCC))
                    continue;

                for( l = eaSize( &rset->boneSet )-1; l >= 0; l--)
                {
                    CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[l]);
                    if(e && isInvalidPart(playerKeys, isPCC, allowStoreParts, false, invSet, COSTUME_LOCKS(bset)))
                        continue;

                    for( m = eaSize(&bset->geo )-1; m >= 0; m--)
                    {
                        CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[m]);

                        if(gset->devOnly)
                            continue;

                        if(e && isInvalidPart(playerKeys, isPCC, allowStoreParts, false, invSet, COSTUME_LOCKS(gset))) 
                            continue;

                        if( costume_GlobalPartIndexFromName( gset->bodyPartName ) == bpid )
                        {
                            int foundFx = 1;
                            if (!isNullOrNone(part->pchFxName))
                            {
                                foundFx = 0;
                            }
                            for( n = eaSize(&gset->info)-1; n >= 0; n-- )
                            {
                                if(e && isInvalidPart(playerKeys, isPCC, allowStoreParts, false, invSet, COSTUME_LOCKS(gset))) 
                                    continue;

                                if (!isNullOrNone(part->pchFxName))
                                {
                                    //    I feel like there should be:
                                    //    foundFx = 0;
                                    //    since just because we found a matching fx once in this geometry set, it doesn't mean everything else matched
                                    //    I'm not sure why things like geometry, tex1, and tex2 are working with this.
                                    //    In my opinion, it should be, match everything and then break.
                                    //    From this, it seems as if it is.. find a matching tex1 somewhere in the set, 
                                    //    find a matching tex2 somewhere in the set and now, find a matching fx somewhere in the set, but not necessarily all on the same piece. -EL
                                    if (!gset->info[n]->fxName)
                                    {
                                        continue;
                                    }
                                    if (stricmp(gset->info[n]->fxName, part->pchFxName) != 0)
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        foundFx = 1;
                                    }
                                }
                                if( gset->info[n]->devOnly )
                                {
                                    continue;
                                }

                                if( !bMatchGeo && !stricmp(gset->info[n]->geoName, part->pchGeom) )
                                    bMatchGeo = true;

                                if( !bMatchGeo )
                                    continue;

                                if( !bMatchTex1 && (gset->info[n]->texName1 && !stricmp(gset->info[n]->texName1, part->pchTex1)))
                                    bMatchTex1 = true;

                                if( !bMatchTex2 && (gset->info[n]->texName2 && !stricmp(gset->info[n]->texName2, part->pchTex2 )))
                                    bMatchTex2 = true;
                            }
                            if (!foundFx)
                            {
                                continue;
                            }

                            if( !bMatchGeo || !bMatchTex1 )
                                continue;

                            if( !bMatchTex2 )
                            {
                                for( n = eaSize(&gset->maskNames)-1; n >= 0; n-- )
                                {
                                    if( !stricmp(gset->maskNames[n],part->pchTex2) )
                                    {
                                        bMatchTex2 = true;
                                        break;
                                    }
                                }
                            }

                            if( !bMatchTex2 )
                            {
                                for( n = eaSize(&gset->masks)-1; n >= 0; n-- )
                                {
                                    if(e && isInvalidPart(playerKeys, isPCC, allowStoreParts, false, invSet, COSTUME_LOCKS(gset->mask[n]))) 
                                        continue;

                                    if( !stricmp(gset->masks[n],part->pchTex2) )
                                    {
                                        bMatchTex2 = true;
                                        break;
                                    }
                                }
                            }

                            if( !bMatchTex2 )
                            {
                                for( n = eaSize(&gset->mask)-1; n >= 0; n-- )
                                {
                                    if( !stricmp(gset->mask[n]->name,part->pchTex2) )
                                    {
                                        bMatchTex2 = true;
                                        break;
                                    }
                                }
                            }

                            if( bMatchTex2 )
                                return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool costume_isReward(const char *name)
{
    if( !name || !costumeReward_HashTable )
        return 0;

    return stashFindElementConst(costumeReward_HashTable, name, NULL);
}

#if CLIENT

int costume_save( char * filename )
{
    Entity *e = playerPtr();
    /*if( filename )
        return ParserWriteTextFile(filename, ParseCostumeSave, costumeBuildSave(&gCostumeSave) );    
    else
        return 0;*/
    return imageserver_WriteToCSV( e->costume, NULL, filename );
}


#ifndef COSTUME_PART_COUNT
#define COSTUME_PART_COUNT 30
#endif

char ** static_costume_strings = 0;

int costume_load( char * filename )
{
    Entity *e = playerPtr();

    Costume *pCostume = 0;
    Costume * plr_costume = costume_current(e);
    int slotid = 0;

    //imageserver_InitDummyCostume(&costume);
    pCostume = costume_create(MAX_COSTUME_PARTS);

    if ( !static_costume_strings )
    {
        int i;
        static_costume_strings = malloc(300*sizeof(char*));
        for (i=0;i<300;++i)
        {
            static_costume_strings[i] = calloc(32, 1);
        }
    }
    if( !imageserver_ReadFromCSV_EX( &pCostume, NULL, filename, static_costume_strings ) )
        return false;

    if (e && e->pl)
        slotid = e->pl->current_costume;

    if(costume_Validate(e, pCostume, slotid, NULL, NULL, 0, true))
        return false;

    costume_destroy(plr_costume);
    plr_costume = entSetCostumeAsUnownedCopy(e, pCostume);    // costume_clone(costume_as_const(pCostume));
    // @todo NEEDS_REVIEW don't we also need to save this into the player current costume slot since
    // we just deleted the costume that lives there?
//    e->pl->costume[e->pl->current_costume] = plr_costume;
    costume_destroy(pCostume);


    costume_Apply(e);
    gTailoredCostume = 0;

    return true;

}

char FACE_SCALE_FILENAME[] = "menu/Costume/faceScale.txt";

void saveCostumeFaceScale(void)
{
    CostumeFaceScaleSet fset = { 0,"Test" };
    Entity * e = playerPtr();

    memcpy( &fset.face, e->costume->appearance.f3DScales, sizeof(Vec3)*NUM_3D_BODY_SCALES );

    if(!ParserWriteTextFile(FACE_SCALE_FILENAME, ParseCostumeFaceScaleSet, &fset, 0, 0))
    {
        char buf[1000];
        sprintf(buf, "Error writing to file %s. Have you checked it out?", FACE_SCALE_FILENAME);
        dialogStd(DIALOG_OK, buf, NULL, NULL, NULL, NULL, 0);
    }
}

#endif // CLIENT






