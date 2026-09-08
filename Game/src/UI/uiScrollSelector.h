

#ifndef UISCROLLSELECTOR_H
#define UISCROLLSELECTOR_H


typedef struct CostumeOriginSet CostumeOriginSet;
typedef struct CostumeRegionSet CostumeRegionSet;
typedef struct CostumeGeoSet CostumeGeoSet;
typedef struct BasePower BasePower;
typedef struct BasePowerSet BasePowerSet;
typedef struct MMRegion MMRegion;
typedef struct MMElementList MMElementList;
typedef struct MMScrollSet_Mission MMScrollSet_Mission;

void selectCostumeSet(const CostumeOriginSet *oset, float x, float y, float z, float sc, float wd, F32 alpha);
int selectCustomPower(const BasePower *power, float x, float y, float z, float sc, float wd, int mode, F32 alpha);
int selectPowerSetTheme(BasePowerSet * powerSet, float x, float y, float z, float sc, float wd, F32 alpha );
void selectBoneset( const CostumeRegionSet * region, float x, float y, float z, float sc, float wd, int mode, F32 alpha );

void scrollSet_ElementListSelector( MMScrollSet_Mission *pMission, MMElementList * pList, float x, float y, float z, float sc, float wd );
void selectVillainGroup( MMElementList *pList, float x, float y, float z, float sc, float wd, int isRightList);
typedef struct GenericTextList
{
    char **textItems;
    int count;
    int current;
} GenericTextList;

void scrollSet_TextListSelector( GenericTextList *pList, float x, float y, float z, float sc, float wd, int active );

typedef enum ScrollType
{
    kScrollType_CostumeSet,
    kScrollType_Boneset,
    kScrollType_Geo,
    kScrollType_GeoTex,
    kScrollType_Tex1,
    kScrollType_Tex2,
    kScrollType_Face,
    kScrollType_GenericRegionSet,
    kScrollType_CustomPowerSet,
    kScrollType_CustomPower,
    kScrollType_GenericElementList,
    kScrollType_GenericTextList,
    kScrollType_LeftVillainGroupList,
    kScrollType_RightVillainGroupList,
}ScrollType;

void selectGeoSet( CostumeGeoSet * gset, int type, float x, float y, float z, float sc, float wd, int mode, F32 alpha );
void changeGeoSet( CostumeGeoSet * gset, int type, int forward );

typedef struct ScrollSelector ScrollSelector;
int updateScrollSelector(ScrollSelector * ss, F32 min, F32 max );
int updateScrollSelectors(F32 min, F32 max);
void clearSS();
void applyCostumeSet(const CostumeOriginSet *oset, int selectedCostumeSet);

#endif
