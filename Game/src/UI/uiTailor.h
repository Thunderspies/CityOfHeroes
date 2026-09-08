#ifndef UITAILOR_H
#define UITAILOR_H

void tailorMenuEx(int sgMode);
void tailorMenu();
void tailorMenuEnterCode();

typedef struct CostumeOriginSet CostumeOriginSet;
typedef struct CostumeRegionSet CostumeRegionSet;
typedef struct CostumeBoneSet CostumeBoneSet;
typedef struct CostumeGeoSet CostumeGeoSet;
typedef enum TailorType TailorType;

void tailor_pushSGColors(Costume *costume);
void tailor_SGColorSetSwitch(int colorSet);

void tailor_RevertRegionToOriginal( const CostumeOriginSet *cset, const CostumeRegionSet *region );
void tailor_RevertCostumePieces(Entity *e);

int tailor_displayCost( const CostumeBoneSet *bset, const CostumeGeoSet *gset, float x, float y, float z, float scale, int grey, int display );

void tailor_DisplayRegionTotal( const char *regionName, float x, float y, float z );
int tailor_RevertBone( int boneID, const CostumeBoneSet *bset, int type );

void tailor_init(int sgMode);
void tailor_exit(int doNotReturnToGame);
void tailorMenu_applyChanges();
int tailor_drawTotal( Entity *e, int genderChange, float screenScaleX, float screenScaleY );

extern int FreeTailorCount;
extern bool bFreeTailor;
extern bool bUsingDayJobCoupon;
extern bool bUltraTailor;
extern bool bVetRewardTailor;
extern int gTailorStanceBits;
#define    TAILOR_STANCE_PROFILE        (1 << 0)
#define    TAILOR_STANCE_HIGH            (1 << 1)
#define    TAILOR_STANCE_COMBAT        (1 << 2)
#define    TAILOR_STANCE_WEAPON        (1 << 3)
#define    TAILOR_STANCE_SHIELD        (1 << 4)
#define    TAILOR_STANCE_RIGHTHAND        (1 << 5)
#define    TAILOR_STANCE_EPICRIGHTHAND    (1 << 6)
#define    TAILOR_STANCE_LEFTHAND        (1 << 7)
#define    TAILOR_STANCE_TWOHAND        (1 << 8)
#define    TAILOR_STANCE_DUALHAND        (1 << 9)
#define    TAILOR_STANCE_KATANA        (1 << 10)
#define    TAILOR_STANCE_GUN            (1 << 11)
#define TAILOR_STANCE_TWOHAND_LARGE    (1 << 12)
#define TAILOR_STATE_WEAPON            seqGetStateNumberFromName("WEAPON")
#define TAILOR_STATE_GUN            seqGetStateNumberFromName("GUN")
#define TAILOR_STATE_KATANA            seqGetStateNumberFromName("KATANA")
#define TAILOR_STATE_TWOHAND_LARGE    seqGetStateNumberFromName("2HLARGE")
typedef struct Costume Costume;
void setSeqFromStanceBits(int stance);
extern Costume *gTailoredCostume;
int tailorPriceOptions_getStartingState();
int tailorPriceOptions_getNextState();
char *tailorPriceOptions_getButtonText();
char * tailorPriceOptions_GreyPay(int cost, int influencePoints);
void tailorPriceOptionsSelectionUpdate();
int tailor_hasStoreCostumeParts();
void tailor_drawPayOptions(int rawCost, int changes, float screenScaleX, float screenScaleY);

#endif