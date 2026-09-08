
#ifndef ATTRIB_DESCRIPTION_H
#define ATTRIB_DESCRIPTION_H

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/textparser_types.h>
#include <utilitieslib/stdtypes.h>

typedef struct Packet Packet;
typedef struct Entity Entity;
typedef struct BasePower BasePower;
typedef struct AttribMod AttribMod;
typedef struct AttribModTemplate AttribModTemplate;

typedef enum AttribType
{
    kAttribType_Cur,
    kAttribType_Str,
    kAttribType_Res,
    kAttribType_Max,
    kAttribType_Mod,
    kAttribType_Abs,
    kAttribType_Special,
    kAttribType_Count,
}AttribType;


typedef enum AttribStyle
{
    kAttribStyle_None,
    kAttribStyle_Percent,
    kAttribStyle_Magnitude,
    kAttribStyle_Distance,
    kAttribStyle_PercentMinus100,
    kAttribStyle_PerSecond,
    kAttribStyle_Speed,
    kAttribStyle_ResistanceDuration,
    kAttribStyle_Multiply,
    kAttribStyle_Integer,
    kAttribStyle_EnduranceReduction,
    kAttribStyle_InversePercent,
}AttribStyle;

typedef enum AttribSource
{
    kAttribSource_Unknown,
    kAttribSource_Self,
    kAttribSource_Player,
    kAttribSource_Critter,
}AttribSource;

typedef struct AttribContributer
{
    const BasePower *ppow;
    char *pchSrcDisplayName;
    F32 fMag;
    F32 fChance;
    int svr_id;
    AttribSource eSrcType;
}AttribContributer;

typedef struct AttribDescription
{
    char *pchName;
    char *pchDisplayName;
    char *pchToolTip;
    AttribType eType;
    AttribStyle eStyle;
    int iKey;
    int offAttrib;
    F32 fVal;
    int buffOrDebuff;
    AttribContributer **ppContributers;
    bool bHide;
    bool bShowBase;
}AttribDescription;


typedef struct AttribCategory
{
    char *pchDisplayName;
    AttribDescription **ppAttrib;
    bool bOpen;
}AttribCategory;

typedef struct AttribCategoryList
{
    const AttribCategory ** ppCategories;
}AttribCategoryList;
extern SERVER_SHARED_MEMORY AttribCategoryList g_AttribCategoryList;

typedef struct CombatMonitorStat
{
    int iKey;
    int iOrder;
}CombatMonitorStat;

#define MAX_COMBAT_STATS 10

extern StaticDefineInt AttribTypeEnum[];
extern StaticDefineInt AttribStyleEnum[];

extern TokenizerParseInfo ParseAttribDescription[];
extern TokenizerParseInfo ParseAttribCategory[];
extern TokenizerParseInfo ParseAttribCategoryList[];

void sendAttribDescription( Entity *e, Packet *pak, int send_mods );
void sendCombatMonitorStats( Entity *e, Packet *pak );

void receiveAttribDescription( Entity *e, Packet *pak, int get_mod, int oo_pak );
void receiveCombatMonitorStats( Entity *e, Packet *pak );

int modGetBuffHash( const AttribModTemplate * t );
bool attribDescriptionPreprocess(TokenizerParseInfo *pti, void* structptr);

void updateCombatMonitorStats( Entity *e );

void attribDescriptionCreateStash();
void attribNameSetAll(void);
AttribDescription * attribDescriptionGet( int hash );
char * getAttribName( int offset );
char *getAttribNameForTranslation( int offset );

void csrViewAttributesTarget( Entity *e );
void clearViewAttributes( Entity *e );
void petViewAttributes( Entity *e, int svr_id );

extern Entity * g_pAttribTarget;

#endif