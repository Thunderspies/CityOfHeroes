
#ifndef COSTUME_DATA_H
#define COSTUME_DATA_H

#include <utilitieslib/stdtypes.h>

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct TrickInfo TrickInfo;
typedef struct CostumePart CostumePart;
typedef enum BodyType BodyType;
typedef struct CharacterClass CharacterClass;

// costume save data structure defs
//-------------------------------------------------------------------------------------------------------------------

typedef struct GeoSave
{
    const char    *setName;
    const char    *geoName;
    const char    *texName1;
    const char    *texName2;
    const char    *maskName;
    const char    *fxName;
}GeoSave;

typedef struct RegionSave
{
    const char *boneset;
    GeoSave **geoSave;

} RegionSave;


typedef struct CostumeSave
{
    int gender;
    float fScale;
    float fBoneScale;
    float fHeadScale;
    float fShoulderScale;
    float fChestScale;
    float fWaistScale;
    float fHipScale;
    float fLegScale;
    float fArmScale;

    RegionSave head;
    RegionSave upper;
    RegionSave lower;
    RegionSave weapon;
    RegionSave shield;
    RegionSave cape;
    RegionSave glowie;
    
    int color1;
    int color2;
    int color3;
    int color4;
    int colorSkin;
}CostumeSave;

extern CostumeSave gCostumeSave;

// tailor data structure defs
//-------------------------------------------------------------------------------------------------------------------

typedef struct TailorCost
{
    int min_level;
    int max_level;

    int entryfee;
    int global;

    int head;
    int head_subitem;

    int upper;
    int upper_subitem;

    int lower;
    int lower_subitem;

    int num_costumes;

} TailorCost;

typedef struct TailorCostList
{
    const TailorCost     **tailor_cost;
} TailorCostList;

extern SERVER_SHARED_MEMORY TailorCostList gTailorMaster;

// costume structure definitions
//------------------------------------------------------------------------------------

// sub structure to store an array of colors (in 255, 255, 255)
//
typedef struct ColorPalette
{
    const Vec3 **color;
    const char* name;
}ColorPalette;

// sub structure to store an array fo text
//
typedef struct TextArray
{
    char ***text;
}TextArray;

typedef struct CostumeGeoSet CostumeGeoSet;
typedef struct CostumeBoneSet CostumeBoneSet;
typedef struct CostumeRegionSet CostumeRegionSet;
// Structur to hold mask name/display name pairs
typedef struct CostumeMaskSet
{
    int            idx;
    const char    *name;
    const char    *displayName;
    const char    **keys;
    const char    *storeProduct;
    const char    **internalProducts;

    const char    **tags; // used to identify a particular costume piece

    int        legacy;
    int        devOnly;
    int        covOnly;
    int        cohOnly;
    int        cohvShared; // deprecated

    const CostumeGeoSet *gsetParent;

} CostumeMaskSet;

// Stucture to store the texture names and display names in a set
//
typedef struct CostumeTexSet
{
    int        idx;
    const char    *displayName;
    const char    *geoName;
    const char    *geoDisplayName;
    const char    *texName1;
    const char    *texName2;
    const char    *fxName;
    const char    **keys;

    const char    *storeProduct;
    const char    **internalProducts;

    const char    **tags; // used to identify a particular costume piece

    const char    **flags; // things to mark this piece as different

    int        legacy;
    int        levelReq;
    int        devOnly;
    int        covOnly;
    int        cohOnly;
    int        texMasked;
    int        isMask;            // used to identify masks for special textures
    int        cohvShared; // deprecated

    const CostumeGeoSet *gsetParent;

}CostumeTexSet;

typedef struct CostumeFaceScaleSet
{
    int        idx;
    const char    *displayName;
    union
    {
        struct {
            Vec3    head;
            Vec3    brow;
            Vec3    cheek;
            Vec3    chin;
            Vec3    cranium;
            Vec3    jaw;
            Vec3    nose;
        };

        struct {
            Vec3    face[8];
        };
    };

    int                covOnly;
    int                cohOnly;

    const CostumeGeoSet *gsetParent;

}CostumeFaceScaleSet;

typedef struct HybridElement HybridElement;
// Stores a list of geo names and one or more texture sets
//
typedef struct CostumeGeoSet
{
    int                idx;
    const char        *name;
    const char        *displayName;
    const char        *bodyPartName;
    const char        *colorLinked;
    const char        **keys;
    const char        **flags; // things to mark this piece as different
    const char        *storeProduct;
    const char        **internalProducts;
    int                type;
    const char        **bitnames;
    const char        **zoombitnames;
    Vec3            defaultPos;
    Vec3            zoomPos;

    int                legacy;
    int                isOpen;
    int                isHidden;
    int                currentInfo;
    int                currentMask;
    int                origInfo;
    int                origMask;
    int                geo_num;
    int                geoLocs[128];
    int                numColor;
    int                devOnly;
    int                covOnly;
    int                cohOnly;

    int                coh; // unchanged values loaded from data
    int                cov;

    float            ht;
    float            wd;
    float            openScale;

    const char        **masks;
    const char        **maskNames;
    
    const CostumeMaskSet    **mask;
    int                maskCount;

    const CostumeTexSet     **info;
    int                infoCount;

    const CostumeFaceScaleSet  **faces;
    int                    faceCount;

    int                restricted;
    int                restrict_face;
    int                restrict_info;
    int                restrict_mask;

    const CostumeBoneSet    *bsetParent;
    const HybridElement    *hb;

}CostumeGeoSet;

// stores a bone and its list of geosets
//
typedef struct CostumeBoneSet
{
    const char        *filename;

    int                idx;
    const char        *name;
    const char        *displayName;
    const char        **keys;
    const char        *storeProduct;
    const char        **internalProducts;
    const char        **flags;
    int                legacy;
    int                currentSet;
    int                origSet;
    const CostumeGeoSet    **geo;
    int                geoCount;

    int                devOnly;
    int                covOnly;
    int                cohOnly;
    int                cov;
    int                coh;
    int                restricted;

    const CostumeRegionSet *rsetParent;

}CostumeBoneSet;

// Stores data for a costume region, regions are HEAD, UPPER, and LOWER
//
typedef struct CostumeRegionSet
{
    const char        *filename;

    int                idx;
    const char        *name;
    const char        *displayName;
    const char        **keys;
    int                currentBoneSet;
    int                origBoneSet;
    const CostumeBoneSet    **boneSet;
    int                boneCount;
    const char        *storeCategory;
} CostumeRegionSet;

// Stores data for a costume set
//
typedef struct CostumeSetSet
{
    const char *filename;
    const char *name;
    const char *displayName;
    const char *npcName;
    const char **keys;                    // the set isn't shown unless the player has all keys or
    const char **storeProductList;        // the set isn't shown unless the player has all products
} CostumeSetSet;

// stores metadata on geos
typedef struct CostumeGeoData
{
    const char *name;        // equivalent to CostumeTexSet::geoName
    const char *filename;
    Vec3 shieldpos;    // offset for attached shield fx (copied into SeqGfxData)
    Vec3 shieldpyr;    // offset for attached shield fx (copied into SeqGfxData)
} CostumeGeoData;

// stores data costume data a specific origin
//
typedef struct CostumeOriginSet
{
    const char                *name;
    const char                *filename;
    const ColorPalette        **bodyColors;
    const ColorPalette        **skinColors;
    const ColorPalette        **powerColors;

    const CostumeRegionSet    **regionSet;
    int                    regionCount;

    const CostumeSetSet        **costumeSets;
    const CostumeGeoData        **geoData;

} CostumeOriginSet;

// stores costume information for a particular body type
//
typedef struct CostumeBodySet
{
    const char                *name;
    const CostumeOriginSet     **originSet;
    int                    originCount;
}CostumeBodySet;

// stores ALL costume information
//
typedef struct CostumeMasterList
{
    const CostumeBodySet     **bodySet;
    int                bodyCount;
} CostumeMasterList;

//-------------------------------------------------------------------------------------------------------------------

extern SERVER_SHARED_MEMORY CostumeMasterList    gCostumeMaster;
extern CostumeGeoSet        gSuperEmblem;
extern SERVER_SHARED_MEMORY ColorPalette            gSuperPalette;
extern cStashTable            costume_HashTable;
extern cStashTable            costume_HashTableExtraInfo;
extern cStashTable            costume_PaletteTable;

typedef struct Entity Entity;
typedef struct AccountInventorySet AccountInventorySet;
typedef struct RewardToken RewardToken;
void loadCostumes(void);
int loadCostume( CostumeMasterList * master, char * path );
void reloadCostumes(void);
void loadSuperPalette(void);
int costume_checkOriginForClassAndSlot( const char * pchOrigin, const CharacterClass * pclass, int slotid );
int AccountHasInternalProductCode(const char ** internalProductCodes, AccountInventorySet* invSet);
int isInvalidPart(RewardToken ** playerKeys, int isPCC, int allowStoreParts, bool checkPublished, AccountInventorySet* invSet, const char ** costumeKeys, const char * productCode, const char ** internalProductCodes);
int costume_validatePart( BodyType bodytype, int bpid, const CostumePart *part, Entity *e, int isPCC, int allowStoreParts, int slotid );
bool costume_isReward(const char *name);
int getColorFromVec3( const Vec3 color );
void saveCostumeFaceScale(void);

CostumePart* costume_getTaggedPart(const char *tag, BodyType bodytype);

#define COSTUME_LOCKS(obj)        obj->keys, obj->storeProduct, obj->internalProducts

#if CLIENT
void costume_setTaggedPart(const char *tag);
#endif

void costume_getShieldOffset(const char *geoname, Vec3 pos, Vec3 pyr);

int costume_save( char * filename );
int costume_load( char * filename );

typedef struct ParseTable ParseTable;
bool costume_fillExtraData( ParseTable *pti, CostumeMasterList * costumeMaster, bool shared_memory );

#endif
