/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef COSTUME_H__
#define COSTUME_H__

#include <utilitieslib/utils/Color.h>
#include <utilitieslib/stdtypes.h>
#include "entity/character_base.h"

typedef struct Packet Packet;
typedef struct Entity Entity;

// do not change this unless you understand the impact it has on related code and the network protocol
#define MAX_COSTUME_PARTS    60

#define ARM_GROW_SCALE 5.0
#define ARM_SHRINK_SCALE 0.9

// Not quite the right place for these, but I'm not 100% sure where they ought to go
// Discount given for the vet reward tailor discount
#define    TAILOR_VET_REWARD_DISCOUNT_PERCENTAGE        50
// Name of the vet reward tailor token
#define    TAILOR_VET_REWARD_TOKEN_NAME                "TailorDiscountToken"

// This is the discount given for using "costume discount salvage"
#define TAILOR_SALVAGE_DISCOUNT_PERCENTAGE            25
// Name of the vet reward tailor token
#define    TAILOR_SALVAGE_TOKEN_NAME                    "S_TailorDiscountCoupon"

typedef enum BodyType
{
    kBodyType_Male        = 0,  // All the values in body type MUST match the
    kBodyType_Female      = 1,  // ones which are read in the menu file.
    kBodyType_BasicMale   = 2,
    kBodyType_BasicFemale = 3,
    kBodyType_Huge        = 4,
    kBodyType_Enemy       = 5,
    kBodyType_Villain     = 6, // Except this one. This one is only set by code.
    kBodyType_Enemy2      = 7, // Except this one. This one is only set by code.
    kBodyType_Enemy3      = 8, // Except this one. This one is only set by code.

} BodyType;

typedef enum CostumeLoadErrors{
    COSTUME_ERROR_NOERROR  = 0,    
    COSTUME_ERROR_INVALIDCOLORS = 1<<0,
    COSTUME_ERROR_NOTAWARDED = 1<<1,    
    COSTUME_ERROR_BADPART = 1<<2,
    COSTUME_ERROR_CAN_FORCE_LOAD = 1<<3,
    COSTUME_ERROR_GENDERCHANGE = 1<<4,
    COSTUME_ERROR_MUST_EXIT = 1<<5,
    COSTUME_ERROR_INVALID_NUM_PARTS = 1<<6,
    COSTUME_ERROR_INVALIDSCALES = 1<<7,
    COSTUME_ERROR_INVALIDFILE = 1<<8,

} CostumeLoadErrors;

typedef struct CostumePart
{
    const char* pchName;        // Name of the BodyPart where this costume part should go.
    Color color[4];                // Index into palette of primary-quaternary colors
    Color colorTmp[4];            // Used in supercostume color selection to remeber the original value

    const char *pchTex1;        // base name of primary texture for body part
    const char *pchTex2;        // base name of secondary texture for body part
    const char *pchGeom;        // base name of geometry for body part
    const char *pchFxName;        // base name of fx for body part

    const char *displayName;    // save us the trouble of back calculating this
    const char *regionName;        // which region it came from
    const char *bodySetName;    // meta-category name

    int costumeNum;                // Which costume this is a part of

    const char *sourceFile;        // which file this came from
} CostumePart;

// new body type scaling params

typedef enum {    kBodyScale_Global = 0,
                kBodyScale_Allbones,  
                kBodyScale_Head,        // this head scale is no longer used (replaced by X,Y,Z version below)
                kBodyScale_Shoulder,
                kBodyScale_Chest,
                kBodyScale_Waist,
                kBodyScale_Hip,
                kBodyScale_Leg,
                kBodyScale_Arm,
                kBodyScale_HeadX,
                kBodyScale_HeadY,
                kBodyScale_HeadZ,
                kBodyScale_BrowX,
                kBodyScale_BrowY,
                kBodyScale_BrowZ,
                kBodyScale_CheekX,
                kBodyScale_CheekY,
                kBodyScale_CheekZ,
                kBodyScale_ChinX,
                kBodyScale_ChinY,
                kBodyScale_ChinZ,
                kBodyScale_CraniumX,
                kBodyScale_CraniumY,
                kBodyScale_CraniumZ,
                kBodyScale_JawX,
                kBodyScale_JawY,
                kBodyScale_JawZ,
                kBodyScale_NoseX,
                kBodyScale_NoseY,
                kBodyScale_NoseZ,
                MAX_BODY_SCALES
}BodyScale_Type;    

// Timeouts for costume change emotes
// Basic timeout - the costume change will fire this many seconds after being sent to the client if the change emote failed for some reason
#define    CC_EMOTE_BASIC_TIMEOUT                10
// The server has to tell every client that receives it that a costume change is to be deferred.  This is done by setting a flag in the entity 
// on the server that goes away after five seconds.  If a costime change is sent on behalf of the entity in question during this timeout, the
// server sets the deferred flag on the change
#define    CC_EMOTE_DEFER_FLAG_TIMEOUT            5
// Because other things can cause a costume change (e.g. using a halloween costume / granite armor), it's possible to get a second costume
// change request sent from the server with the defer flag set.  So we set a second timer on the client, and if a costume change arrives
// from the server in that time period, with the deferred flag set, we ignore the deferred flag.
#define    CC_EMOTE_IGNORE_EXTRA_TIMEOUT        (CC_EMOTE_BASIC_TIMEOUT + 5)
// While I'm at it, let's make the costume chage timeouts on client and server into #defines.
#define CC_TIMEOUT_CLIENT                    30
// bumping this from 15 to 20 so it's 5 greater than CC_EMOTE_IGNORE_EXTRA_TIMEOUT
#define CC_TIMEOUT_SERVER                    20

#define NUM_2D_BODY_SCALES  (9)
#define NUM_3D_BODY_SCALES    (7)
#define NUM_DB_BODY_SCALES    (NUM_2D_BODY_SCALES + NUM_3D_BODY_SCALES)    
// This needs to match the number of SG color slots in containerLoadSave.c
#define NUM_SG_COLOR_SLOTS    (6)

typedef struct Appearance
{
    const char* entTypeFile;        // What kind of geometry and animation do I have?
    const char* costumeFilePrefix;    // What kind of prefix do I use to generate my costume parts geometry name?

    BodyType bodytype;    // Eventually specifies the geometry prefix
    int iVillainIdx;    // If bodytype is kBodyType_Villain, then this is used
    int convertedScale;

//    float fBodyScales[MAX_BODY_SCALES];
    union
    {
        struct {
            F32 fScale;       // ranges from -9 to 9. Yields approx +/-20% size change
            F32 fBoneScale;    // -1 = skinniest guy, 0 = normal guy, 1 = fattest guy
            F32 fHeadScale;
            F32 fShoulderScale;
            F32 fChestScale;
            F32 fWaistScale;
            F32 fHipScale;
            F32 fLegScale;
            F32 fArmScale;

            union
            {
                struct {
                    Vec3    fHeadScales;        // x,y,z scaling
                    Vec3    fBrowScales;
                    Vec3    fCheekScales;
                    Vec3    fChinScales;
                    Vec3    fCraniumScales;
                    Vec3    fJawScales;
                    Vec3    fNoseScales;
                };

                struct {
                    Vec3    f3DScales[NUM_3D_BODY_SCALES];
                };
            };
        };

        struct {
            float fScales[MAX_BODY_SCALES];
        };
    };
    int compressedScales[NUM_3D_BODY_SCALES];    // only compress scales for 3-dimensional bone scaling (ie Vec3)

    Color colorSkin;    // Index into palette of skin color.

    // Additional SG color slots
    SGColorBits superColorsPrimaryU[NUM_SG_COLOR_SLOTS];
    SGColorBits superColorsSecondaryU[NUM_SG_COLOR_SLOTS];
    SGColorBits superColorsPrimary2U[NUM_SG_COLOR_SLOTS];
    SGColorBits superColorsSecondary2U[NUM_SG_COLOR_SLOTS];
    SGColorBits superColorsTertiaryU[NUM_SG_COLOR_SLOTS];
    SGColorBits superColorsQuaternaryU[NUM_SG_COLOR_SLOTS];
    int currentSuperColorSet;

    int iNumParts;        // Number of parts in the costume

} Appearance;

typedef struct cCostume
{
    Appearance appearance;        // enttype and general appearance
    const CostumePart** parts;    // geom and texture info for each bone

    // not persisted
    int containsRestrictedStoreParts;        // flag to indicate if this costume has restricted parts that need to be bought
} cCostume;

typedef struct Costume
{
    Appearance appearance;                    // enttype and general appearance
    CostumePart** parts;                    // geom and texture info for each bone
    
    // not persisted
    int containsRestrictedStoreParts;        // flag to indicate if this costume has restricted parts that need to be bought
} Costume;

typedef struct CostumePartDiff
{
    int index;
    CostumePart *part;
}CostumePartDiff;
typedef struct CostumeDiff
{
    int costumeBaseNum;
    Appearance appearance;
    CostumePartDiff **differentParts;
}CostumeDiff;

typedef struct CustomNPCCostumePart
{
    const char *regionName;
    Color color[2];
}CustomNPCCostumePart;
typedef struct CustomNPCCostume
{
    char *originalNPCName;
    CustomNPCCostumePart **parts;
    Color skinColor;
}CustomNPCCostume;

Costume* costume_create(int numParts);
void costume_init(Costume* costume);
int costume_get_max_slots(Entity* e);
int costume_get_max_earned_slots(Entity* e);
int costume_get_num_slots(Entity *e);

const cCostume* costume_as_const(Costume * src);
#if CLIENT
Costume* costume_as_mutable_cast(const cCostume* src);    // necessary client hack at the moment
#endif
Costume* costume_clone(const cCostume* src);
Costume* costume_clone_nonconst(Costume* src);
Costume* costume_new_slot(Entity* e);
void costume_destroy(Costume* costume);

// Convenience function to return a cached string or the cached string "none" if
// the input string is NULL or empty or None/NONE/none/etc
const char *costume_GetCachedStringOrNone(const char *string);

void costume_SetScale(Costume* costume, float fScale);
// Set the size of the character. Ranges from -9 to 9 which corresponds
// to +- 20% or so.

void costume_SetBoneScale(Costume* costume, float fBoneScale);
// -1 is skinniest guy, 0 is normal, +1 is largest guy

void costume_SetBodyScale(Costume * costume, BodyScale_Type type, float fScale);
// -1 is the smallest scale setting, 1 is the largest

void costume_SetColorsLinked(Costume* costume, int bLinked);

void costume_SetBodyType(Costume* costume, BodyType bodytype);
// The body type sets what basic model is used for the body. Behind
// the scenes it also is the place where geometry prefixes are handled.
// If set to kBodyType_Villain, the villain index is used for determining
// prefixes.

void costume_SetBodyName(Costume* costume, const char* bodyName);
// The body type defines where custom costume parts' geometry
// comes from when default parts are overridden.  This works in conjunction
// with the costume's specified bodytype.
//
// Example:
//        If the body name is "thug" then the custom geomtry for the thug's
//        pants will come from "thug_pants.geo"
//
// The only reason why this function is called at all is so that a dummy CostumeFilePrefix
// field can be outputted with the "save visuals" button.  Otherwise, the real
// contents of this field should always be loaded from data files.
//
// FIXME!!!
// Instead of each costume part having some partial geomtry and texture name, it should
// specify the exact piece of geometry and texture.  This requires fixes to lots of data files
// however.


void costume_SetVillainIdx(Costume* costume, int i);
// Specifies which villain index (in the global villains[] array)
// this entity is associated with. Used to determine geometry and
// texture prefixes.

void costume_SetSkinColor(Costume* costume, Color color);
// Sets the primary color of any texture with "SKIN" in its name.
// Color is an index out of the skin palette.

void costume_PartSetName(Costume* costume, int iIdxBone, const char* name);

void costume_PartSetColor(Costume* costume, int iIdxBone, int colornum, const Color color);
// Sets the primary/secondary color of texture for a particular body part.
// Color is an index out of the skin palette.
void costume_PartForceTwoColor(Costume *costume, int iIdxBone); // copies colors 1&2 over 3&4

void costume_PartSetPath( Costume* costume, int iIdxBone, const char* displayName, const char* region, const char* set );

void costume_PartSetTexture1(Costume* costume, int iIdxBone, const char *pchTex);
void costume_PartSetTexture2(Costume* costume, int iIdxBone, const char *pchTex);
void costume_PartSetFx(Costume* costume, int iIdxBone, const char *pchFx);
// Sets the primary/secondary base texture name for a body part.
// This name will be magically munged with the bodytype's texture prefix.

void costume_PartSetGeometry(Costume* costume, int iIdxBone, const char *pchGeom);
// Sets the geometry to be used for a body part.
// This name will be magically munged with the bodytype's entname prefix.

void costume_PartSetAll(Costume* costume, int iIdxBone,
                        const char *pchGeom, const char *pchTex1, const char *pchTex2,
                        Color color1, Color color2);
// A convenience function for setting all the attribs for a part at once.

bool costume_StripBodyPart( Costume* costume, const char* part );
// Reset the contents of a given body part to empty.

void costume_AdjustAllCostumesForPowers(Entity *e);
// If the character has any powers which require certain costume pieces (eg.
// Arachnos Crab Spider needs the backpack), add them to all costume slots.

int    costume_PartIndexFromName( Entity* e, const char* name );
//    Return the costume part index into the character part set from a given BodyPart name
//

int    costume_GlobalPartIndexFromName( const char* name );
//    Return the costume part index into the character part set from a given BodyPart name
//

bool costume_PartGeomPartialMatch( Costume* costume, char* part, const char* name );
// Check if the specified part's geometry on the costume contains name within it
//

bool costume_PartFxPartialMatch( Costume* costume, char* part, const char* name );
// Check if the specified part's FX on the costume contains name within it
//

typedef struct SeqInst SeqInst;
int costume_ValidateScales( SeqInst *seq, float fBonescale, float fShoulderScale, float fChestScale, float fWaistScale, float fHipScale, float fLegScale, float fArmScale);
// returns true is the scales are ok, false if not

int costume_Validate( Entity *e, Costume * costume, int slotid, int **badPartList, int **badColorList, int isPCC, int allowStoreParts );
// returns true if costume is legit, false if not


Costume * costume_current( Entity * e );
const cCostume * costume_current_const( Entity * e );
// returns players current costume

void getCurrentBodyTypeAndOrigin(Entity *e, Costume *costume, int *costumeBodyType, int *originSet);
//    sets costume body type and origin set from entity class and costume.appearance.bodytype
//-------------------------------------------------------------------------------------
// Costume transmission
//-------------------------------------------------------------------------------------
int costume_receive(Packet *pak, Costume* costume);
void costume_send(Packet* pak, Costume * costume, int send_names);
int NPCcostume_receive(Packet *pak, CustomNPCCostume *costume);
void NPCcostume_send(Packet *park, CustomNPCCostume *costume);
int costume_change( Entity *e, int num );
#if CLIENT
int costume_change_emote_check( Entity *e, int num );
#endif

//-------------------------------------------------------------------------------------
// Costume color pallette
//-------------------------------------------------------------------------------------
int    getCostumeColorSize();
int    getSkinColorSize();
Color getDefaultCostumeColor();
Color getDefaultSkinColor();
Color getCostumeColor(unsigned int index);
Color getSkinColor(unsigned int index);
unsigned int getCostumeColorIndex(Color color);
int costumeCorrectColors(Entity *e, Costume *costume, int **badColorList);
//-------------------------------------------------------------------------------------
// Costume definiton loading
//-------------------------------------------------------------------------------------
#include <utilitieslib/utils/textparser.h>
extern TokenizerParseInfo ParseCostume[];
extern TokenizerParseInfo ParseCostumePart[];
extern TokenizerParseInfo ParseCostumePartDiff[];
extern TokenizerParseInfo ParseCostumeDiff[];
extern TokenizerParseInfo ParseCustomNPCCostume[];
extern TokenizerParseInfo ParseCustomNPCCostumePart[];
void costume_WriteTextFile(Costume* costume, char* filename);


//-------------------------------------------------------------------------------------
// Temp home for bodyName() and related stuff
//-------------------------------------------------------------------------------------
typedef struct BodyTypeInfo
{
    BodyType bodytype;
    char *pchEntType;
    char *pchTexPrefix;
} BodyTypeInfo;
extern BodyTypeInfo g_BodyTypeInfos[];
int getBodyTypeCount();
BodyType GetBodyTypeForEntType(const char *pch);
BodyTypeInfo* GetBodyTypeInfo(const char* type);

const char *costumePrefixName( const Appearance *appearance );
const char *entTypeFileName( const Entity *pe );

bool isNullOrNone(const char *str);
bool costume_isPartEmpty(const CostumePart *part);

//-------------------------------------------------------------------------------------
// functions to deal with SG color field
//-------------------------------------------------------------------------------------

typedef enum SGColorType
{
    SGC_UNDEFINED,
    SGC_DEFAULT,
    SGC_PRIMARY,
    SGC_SECONDARY,
} SGColorType;

void costume_SGColorsExtract(Entity* e, Costume* costume, SGColorBits* primU, SGColorBits* secU, SGColorBits* prim2U, SGColorBits* sec2U, SGColorBits* threeU, SGColorBits* fourU);
void costume_baseColorsExtract( const cCostume *costume, unsigned int colorArray[][4], unsigned int *skinColor );
void ExtractCostumeColorsForCompareinTailor( const cCostume *costume, unsigned int colorArray[][2], unsigned int *skinColor );
void costume_SGColorsApplyToCostume(Entity* e, Costume* costume, SGColorBits primU, SGColorBits secU, SGColorBits prim2U, SGColorBits sec2U, SGColorBits threeU, SGColorBits fourU);
void costume_baseColorsApplyToCostume( Costume *costume, unsigned int colorArray[][4] );
void costume_SGColorsApply( Entity *e );
Costume * costume_makeSupergroup( Entity *e );

//-------------------------------------------------------------------------------------
// data validation for tailored costumes
//-------------------------------------------------------------------------------------

typedef enum TailorType
{
    TAILOR_REGION,
    TAILOR_GEOTEX,
    TAILOR_GEO,
    TAILOR_TEX,
    TAILOR_MASK,
    TAILOR_COLOR,
    TAILOR_FX,
    TAILOR_TOTAL,
} TailorType;

typedef enum GenderChangeMode
{
    GENDER_CHANGE_MODE_NONE,
    GENDER_CHANGE_MODE_BASIC,
    GENDER_CHANGE_MODE_DIFFERENT_GENDER,
}GenderChangeMode;

int costume_isDifference( const cCostume * c1, const cCostume * c2 );
bool costume_changedScale( const cCostume * c1, const cCostume * c2 );
int tailor_CalcPartCost( Entity *e, const CostumePart *cpart1, const CostumePart *cpart2 );
int tailor_getSubItemCost( Entity *e, const CostumePart *cpart );
int tailor_getRegionCost( Entity *e, const char *regionName );
int tailor_getNumSlots( Entity *e );
int tailor_getFee( Entity *e );
int tailor_getGlobalCost( Entity *e );

typedef struct PowerCustomizationList PowerCustomizationList;
Costume* costume_recieveTailor( Packet *pak, Entity *e, int genderChange );
void costume_applyTailorChanges(Entity *e, int genderChange, Costume *costume);
int tailor_RegionCost( Entity *e, const char *regionName, const cCostume* costume, int reverse );
int tailor_CalcTotalCost( Entity *e, const cCostume* costume, int genderChange );


//-------------------------------------------------------------------------------------
// costume piece award
//-------------------------------------------------------------------------------------
typedef struct RewardToken RewardToken;
void costume_Award( Entity *e, const char * costumePiece, int notifyPlayer, int isPCC);
void costume_Unaward( Entity *e, const char * costumePiece, int isPCC);
void ent_SendCostumeRewards(Packet *pak, Entity *e);
void ent_ReceiveCostumeRewards(Packet *pak, Entity *e);

//-------------------------------------------------------------------------------------
// sg colors
//-------------------------------------------------------------------------------------
void receiveSGColorData(Packet *pak);
void costume_sendSGColors(Entity *e);
int costume_getNumSGColorSlot(Entity *e);

//-------------------------------------------------------------------------------------
// costume change emotes
//-------------------------------------------------------------------------------------
void sendCostumeChangeEmoteList(Entity *e);

//-------------------------------------------------------------------------------------
// CSR costume
//-------------------------------------------------------------------------------------
void costume_setDefault(Entity * e, int number);

//-------------------------------------------------------------------------------------
// Dealing with legacy scaling
//-------------------------------------------------------------------------------------
void costume_retrofitBoneScale( Costume * costume );

//-------------------------------------------------------------------------------------
// For compacting three-diminsional bone scales into a single integer
//-------------------------------------------------------------------------------------
void compressAppearanceScales(Appearance * a);
void decompressAppearanceScales(Appearance * a);


void costume_sendEmblem(Packet * pak, Entity *e );
void costume_recieveEmblem(Packet * pak, Entity *e, Costume * costume );

void costumeAwardAuthParts(Entity *e);
void costumeAwardPowersetParts(Entity *e, int notifyPlayer, int isPCC);
void costumeUnawardPowersetParts(Entity *e, int isPCC);
typedef struct BasePowerSet BasePowerSet;
void costumeAwardPowerSetPartsFromPower(Entity *e, const BasePowerSet* powerSet, int notifyPlayer, int isPCC);
void costumeUnawardPowerSetPartsFromPower(Entity *e, const BasePowerSet *powerSet, int isPCC);
void costumeFillPowersetDefaults(Entity *e);

#ifdef SERVER
void costumeValidateCostumes(Entity *e);
void PrintCostumeToLog(Entity* e);
#endif
void costume_getDiffParts( const cCostume *c1, const cCostume *c2, int **diffParts);
void costume_createDiffCostume(const cCostume *baseCostume, const cCostume *compareCostume, CostumeDiff *resultingCostume);
void costumeApplyNPCColorsToCostume(Costume *costume, CustomNPCCostume *npcCostume);

int validateCustomNPCCostume(CustomNPCCostume *npcCostume);
extern Vec3    gFaceDest[NUM_3D_BODY_SCALES];
extern Vec3    gFaceDestSpeed[NUM_3D_BODY_SCALES];

#endif /* #ifndef COSTUME_H__ */
/* End of File */
