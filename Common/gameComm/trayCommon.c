//
// tray.c 1/2/03
//
// Handles powers tray info
//
//----------------------------------------------------------------

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/earray.h>

#include "gameComm/trayCommon.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/strings_opt.h>
#include "container/dbcontainerpack.h"

// Use the tray iterator functions when you want to iterate over every item in all trays
// Don't expose the guts of the player/server controlled trays to external files!
#define TOTAL_PLAYER_TRAYS        ( 9 )
#define TOTAL_SERVER_TRAYS        ( 1 )
// NOTE: Server trays are build-agnostic, so they aren't taken into account below.
#define TRAY_SLOTS_PER_BUILD    ( TRAY_SLOTS * TOTAL_PLAYER_TRAYS )
#define MAX_SLOT_COUNT            ( TRAY_SLOTS_PER_BUILD * MAX_BUILD_NUM )
#define MAX_SERVER_SLOT_COUNT    ( TRAY_SLOTS * TOTAL_SERVER_TRAYS )

// DO NOT EXPOSE THESE VARIABLES OUTSIDE OF THIS C FILE!
// I'm trying to modularize at least part of the Tray structure...
// Over time, it would be nice if everything in Tray migrated here...
typedef struct TrayInternals
{
    TrayObj     **slot;            // 0-9 are kTrayCategory_PlayerControlled tray index 1, 
                                // 10-19 are index 2... 80-89 are index 9.
                                // 90-179 are build 2... 630-719 are build 8.
    TrayObj        **serverSlot;    // 0-9 are the kTrayCategory_ServerControlled tray (unindexed).
                                // serverSlot is build-agnostic!
} TrayInternals;

LineDesc tray_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,     "Type",             OFFSET(TrayObj, type ),            },
        "The type of item<br>"
        "1 = Power<br>"
        "6 = Macro<br>"
        "9 = SystemMacro"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "InspirationCol",   OFFSET(TrayObj, iset ),            },
        "First index to power, players powerset index"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "InspirationRow",   OFFSET(TrayObj, ipow ),            },
        "Second index to power, player power index"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "PowerName",        OFFSET(TrayObj, pchPowerName ),    },
        "The powers name"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "PowerSetName",     OFFSET(TrayObj, pchPowerSetName ), },
        "The powerset name"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "CategoryName",     OFFSET(TrayObj, pchCategory ),     },
        "The category (Primary, Seconday, Pool, etc)"},

    {{ PACKTYPE_STR_UTF8,        SIZEOF2(TrayObj, command),        "Command",    OFFSET(TrayObj, command ),           },
        "The command performed if object is a macro"},

    {{ PACKTYPE_STR_UTF8,        SIZEOF2(TrayObj, iconName),        "Icon",        OFFSET(TrayObj, iconName ),           },
        "The name of the icon to display (if macro)"},

    {{ PACKTYPE_STR_UTF8,        SIZEOF2(TrayObj, shortName),    "Name",        OFFSET(TrayObj, shortName ),       },
        "The name overlayed on top of icon (if macro)"},

    { 0 },
};

StructDesc tray_desc[] =
{
    sizeof(TrayObj),
    {AT_EARRAY, OFFSET4_PTR3(Entity, pl, EntPlayer, tray, Tray, internals, TrayInternals, slot )},
    tray_line_desc,

    "This table contains all information needed for each individual items in a users tray.<br>"
    "Each row is a single tray item.<br>"
    "There are 10 slots per tray and 10 trays per entity."
};

MP_DEFINE(TrayObj);
TrayObj* trayobj_Create(void)
{
    TrayObj *res = NULL;

#if CLIENT
    MP_CREATE(TrayObj, 32);
#else
    MP_CREATE(TrayObj, 320);
#endif
    res = MP_ALLOC( TrayObj );

    return res;
}

void trayobj_Destroy( TrayObj *item )
{
    MP_FREE(TrayObj, item);
}

MP_DEFINE(Tray);
MP_DEFINE(TrayInternals);
Tray *tray_Create(void)
{
    Tray *tray;

    MP_CREATE(Tray, 4);
    MP_CREATE(TrayInternals, 4);
    tray = MP_ALLOC(Tray);
    tray->internals = MP_ALLOC(TrayInternals);
    eaCreate(&tray->internals->slot);
    eaCreate(&tray->internals->serverSlot);

    return tray;
}

void tray_Destroy(Tray *tray)
{
    eaDestroyEx(&tray->internals->slot, trayobj_Destroy);
    eaDestroyEx(&tray->internals->serverSlot, trayobj_Destroy);
    MP_FREE(TrayInternals, tray->internals);
    MP_FREE(Tray, tray);
}

// traySlots can be reallocated!
static TrayObj* createTrayObj(TrayObj ***traySlots, int index)
{
    if (index >= eaSize(traySlots))
    {
        eaSetSize(traySlots, index + 1);
    }

    if (!(*traySlots)[index])
    {
        (*traySlots)[index] = trayobj_Create();
    }

    return (*traySlots)[index];
}

static void destroyTrayObj(TrayObj **traySlots, int index)
{
    if (traySlots[index])
    {
        trayobj_Destroy(traySlots[index]);
        traySlots[index] = NULL;
    }
}

// Returns -1 if the indices are out of range OR if the TrayObj doesn't exist.
static int getTrayObjIndex(Tray *tray, TrayCategory category, int trayIndex, int slotIndex, int buildIndex, bool createIfNotFound)
{
    int index = -1;

    if (category < 0 || category >= kTrayCategory_Count)
    {
        return -1;
    }

    if (category == kTrayCategory_PlayerControlled)
    {
        if (trayIndex < 0 || trayIndex >= TOTAL_PLAYER_TRAYS
            || slotIndex < 0 || slotIndex >= TRAY_SLOTS
            || buildIndex < 0 || buildIndex >= MAX_BUILD_NUM)
        {
            return -1;
        }

        index = buildIndex * TRAY_SLOTS_PER_BUILD 
            + trayIndex * TRAY_SLOTS
            + slotIndex;

        if (createIfNotFound)
            createTrayObj(&tray->internals->slot, index);

        if (index < eaSize(&tray->internals->slot) 
            && tray->internals->slot[index])
        {
            tray->internals->slot[index]->icat = category;
            tray->internals->slot[index]->itray = trayIndex;
            tray->internals->slot[index]->islot = slotIndex;
        }
        else
        {
            return -1;
        }
    }
    else if (category == kTrayCategory_ServerControlled)
    {
        // trayIndex and buildIndex are ignored for server-controlled trays

        if (slotIndex < 0 || slotIndex >= TRAY_SLOTS)
        {
            return -1;
        }

        index = slotIndex;

        if (createIfNotFound)
            createTrayObj(&tray->internals->serverSlot, index);

        if (index < eaSize(&tray->internals->serverSlot) 
            && tray->internals->serverSlot[index])
        {
            tray->internals->serverSlot[index]->icat = category;
            tray->internals->serverSlot[index]->itray = trayIndex;
            tray->internals->serverSlot[index]->islot = slotIndex;
        }
        else
        {
            return -1;
        }
    }

    return index;
}

TrayObj* getTrayObj(Tray *tray, TrayCategory category, int trayIndex, int slotIndex, int buildIndex, bool createIfNotFound)
{
    int index = getTrayObjIndex(tray, category, trayIndex, slotIndex, buildIndex, createIfNotFound);
    
    if (index == -1)
        return NULL;
    else if (category == kTrayCategory_ServerControlled)
        return tray->internals->serverSlot[index];
    else
        return tray->internals->slot[index];
}

TrayItemType typeTrayObj(Tray *tray, TrayCategory category, int trayIndex, int slotIndex, int buildIndex)
{
    TrayObj* obj = getTrayObj(tray, category, trayIndex, slotIndex, buildIndex, false);

    if (obj)
        return obj->type;
    else
        return kTrayItemType_None;
}

void clearTrayObj(Tray *tray, TrayCategory category, int trayIndex, int slotIndex, int buildIndex)
{
    int index = getTrayObjIndex(tray, category, trayIndex, slotIndex, buildIndex, false);

    if (index != -1)
    {
        if (category == kTrayCategory_ServerControlled)
            destroyTrayObj(tray->internals->serverSlot, index);
        else
            destroyTrayObj(tray->internals->slot, index);
    }
}

static Tray *s_currentIteratedTray = NULL;
static int s_currentTrayIteratorIndex = -1;
static TrayCategory s_currentTrayIteratorCategoryIndex = -1;

static int s_currentlyIteratingOnlyThisBuild = -1;
static TrayCategory s_currentlyIteratingOnlyThisCategory = -1;

void resetTrayIterator(Tray *tray, int onlyThisBuild, TrayCategory onlyThisCategory)
{
    s_currentIteratedTray = tray;
    s_currentlyIteratingOnlyThisBuild = onlyThisBuild;
    s_currentlyIteratingOnlyThisCategory = onlyThisCategory;

    if (onlyThisBuild == -1)
        s_currentTrayIteratorIndex = -1;
    else
        s_currentTrayIteratorIndex = onlyThisBuild * TRAY_SLOTS_PER_BUILD - 1;

    if (onlyThisCategory == kTrayCategory_ServerControlled)
        s_currentTrayIteratorCategoryIndex = kTrayCategory_ServerControlled;
    else
        s_currentTrayIteratorCategoryIndex = kTrayCategory_PlayerControlled;
}

static bool isTrayIteratorValid()
{
    if (s_currentTrayIteratorIndex < 0 || !s_currentIteratedTray)
    {
        return false;
    }

    if (s_currentTrayIteratorCategoryIndex == kTrayCategory_PlayerControlled)
    {
        if (s_currentTrayIteratorIndex >= MAX_SLOT_COUNT
            || (s_currentlyIteratingOnlyThisBuild != -1 && s_currentTrayIteratorIndex >= (s_currentlyIteratingOnlyThisBuild + 1) * TRAY_SLOTS_PER_BUILD))
        {
            return false;
        }
    }
    else if (s_currentTrayIteratorCategoryIndex == kTrayCategory_ServerControlled)
    {
        if (s_currentTrayIteratorIndex >= MAX_SERVER_SLOT_COUNT)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static void incrementTrayIterator()
{
    s_currentTrayIteratorIndex++;

    // This logic ignores s_currentlyIteratingOnlyThisBuild
    // because server-controlled trays are build-agnostic.
    if (s_currentlyIteratingOnlyThisCategory == -1 
        && s_currentTrayIteratorCategoryIndex == kTrayCategory_PlayerControlled
        && !isTrayIteratorValid())
    {
        s_currentTrayIteratorIndex = 0;
        s_currentTrayIteratorCategoryIndex = kTrayCategory_ServerControlled;
    }
}

static bool getCurrentTrayObjFromIterator(TrayObj **obj)
{
    if (!isTrayIteratorValid())
    {
        (*obj) = NULL;
        return false;
    }

    if (s_currentTrayIteratorCategoryIndex == kTrayCategory_PlayerControlled)
    {
        if (s_currentTrayIteratorIndex >= eaSize(&s_currentIteratedTray->internals->slot))
            eaSetSize(&s_currentIteratedTray->internals->slot, s_currentTrayIteratorIndex + 1);

        (*obj) = s_currentIteratedTray->internals->slot[s_currentTrayIteratorIndex];
    }
    else
    {
        if (s_currentTrayIteratorIndex >= eaSize(&s_currentIteratedTray->internals->serverSlot))
            eaSetSize(&s_currentIteratedTray->internals->serverSlot, s_currentTrayIteratorIndex + 1);

        (*obj) = s_currentIteratedTray->internals->serverSlot[s_currentTrayIteratorIndex];
    }
    return true;
}

// Items can be NULL and still in the list,
// so "is iterator finished" has to be different than "returning NULL item from list".
bool getNextTrayObjFromIterator(TrayObj **obj)
{
    incrementTrayIterator();

    return getCurrentTrayObjFromIterator(obj);
}

// create one in the spot you returned last time getNextTrayObjFromIterator() was called.
// if there is one there already, just return it.
void createCurrentTrayObjViaIterator(TrayObj **obj)
{
    if (getCurrentTrayObjFromIterator(obj) && !(*obj))
    {
        if (s_currentTrayIteratorCategoryIndex == kTrayCategory_PlayerControlled)
        {
            (*obj) = createTrayObj(&s_currentIteratedTray->internals->slot, s_currentTrayIteratorIndex);
        }
        else
        {
            (*obj) = createTrayObj(&s_currentIteratedTray->internals->serverSlot, s_currentTrayIteratorIndex);
        }

        getCurrentTrayIteratorIndices(&((*obj)->icat), &((*obj)->itray), &((*obj)->islot), NULL);
    }
}

// clear the one you returned last time getNextTrayObjFromIterator() was called.
void destroyCurrentTrayObjViaIterator()
{
    if (isTrayIteratorValid())
    {
        if (s_currentTrayIteratorCategoryIndex == kTrayCategory_PlayerControlled)
        {
            destroyTrayObj(s_currentIteratedTray->internals->slot, s_currentTrayIteratorIndex);
        }
        else
        {
            destroyTrayObj(s_currentIteratedTray->internals->serverSlot, s_currentTrayIteratorIndex);
        }
    }
}

void getCurrentTrayIteratorIndices(TrayCategory *category, int *tray, int *slot, int *build)
{
    if (category)
        (*category) = s_currentTrayIteratorCategoryIndex;
    if (build)
        (*build) = s_currentTrayIteratorIndex / TRAY_SLOTS_PER_BUILD;
    if (tray)
        (*tray) = s_currentTrayIteratorIndex % TRAY_SLOTS_PER_BUILD / TRAY_SLOTS;
    if (slot)
        (*slot) = s_currentTrayIteratorIndex % TRAY_SLOTS;
}

void trimExcessTrayObjs(Tray *tray, int buildCount)
{
    // Remove corrupted data that was inserted in previous builds
    if (eaSize(&tray->internals->slot) > buildCount * TRAY_SLOTS_PER_BUILD)
    {
        eaSetSize(&tray->internals->slot, buildCount * TRAY_SLOTS_PER_BUILD);
    }
}

void buildPowerTrayObj(TrayObj *pto, int iset, int ipow, int autoPower )
{
    memset(pto, 0, sizeof(TrayObj));
    pto->type = kTrayItemType_Power;
    pto->iset = iset;
    pto->ipow = ipow;
    pto->autoPower = autoPower;
    pto->scale = 1.f;
}

void buildInspirationTrayObj(TrayObj *pto, int iCol, int iRow)
{
    memset(pto, 0, sizeof(TrayObj));
    pto->type = kTrayItemType_Inspiration;
    pto->iset = iCol;
    pto->ipow = iRow;
    pto->scale = 1.f;
}

void builPlayerSlotTrayObj( TrayObj *pto, int src_idx, char * name )
{
    memset(pto, 0, sizeof(TrayObj));
    pto->type = kTrayItemType_PlayerSlot;
    pto->invIdx = src_idx;
    strncpyt( pto->shortName, name, sizeof(pto->shortName) );
}

void buildSpecializationTrayObj(TrayObj *pto, int inventory, int iSet, int iPow, int iSpec )
{
    memset(pto, 0, sizeof(TrayObj));
    if( inventory )
    {
        pto->type = kTrayItemType_SpecializationInventory;
        pto->ispec = iSpec;
    }
    else
    {
        pto->type = kTrayItemType_SpecializationPower;
        pto->iset = iSet;
        pto->ipow = iPow;
        pto->ispec = iSpec;
    }
    pto->scale = 1.f;
}

void buildMacroTrayObj( TrayObj *pto, const char * command, const char *shortName, const char *iconName, int hideName )
{
    memset(pto, 0, sizeof(TrayObj));
    pto->scale = 1.f;

    if( hideName )
        pto->type = kTrayItemType_MacroHideName;
    else
        pto->type = kTrayItemType_Macro;

    if( command )
        strncpyt( pto->command, command, sizeof(pto->command) );

    if( shortName )
        strncpyt( pto->shortName, shortName, sizeof(pto->shortName) );

    if( iconName )
        strncpyt( pto->iconName, iconName, sizeof(pto->iconName) );
}

//
//
static void tray_getPowerStrings( Entity *e, TrayObj *to, int iset, int ipow )
{
    assert( e->pchar );

    if(e->pchar->ppPowerSets[iset]->ppPowers[ipow])
    {
        // Power Redirection Fix - this is relevant only on the server
        const BasePower *ppowBase = e->pchar->ppPowerSets[iset]->ppPowers[ipow]->ppowOriginal;
        to->pchPowerName = ppowBase->pchName;
    }
    else
        to->pchPowerName = NULL;

    to->pchPowerSetName    = e->pchar->ppPowerSets[iset]->psetBase->pchName;
    to->pchCategory        = e->pchar->ppPowerSets[iset]->psetBase->pcatParent->pchName;
}

//
//

//------------------------------------------------------------
// send the tray
// -AB: refactored for new cur tray :2005 Jan 26 02:53 PM
//----------------------------------------------------------

// Doesn't quite correspond to receiveTrayObj... receiveTrayObjs receives some of the data.
void sendTrayObj(Packet *pak, TrayObj *obj)
{
    if (!obj)
    {
        pktSendBits(pak, 5, kTrayItemType_None);
    }
    else
    {                        
        pktSendBits(pak, 5, obj->type );
        if (obj->type == kTrayItemType_Power)
        {
            pktSendBits(pak, 32, obj->iset);
            pktSendBits(pak, 32, obj->ipow);
        }
        else if (obj->type == kTrayItemType_Inspiration)
        {
            pktSendBitsPack(pak, 3, obj->iset);
            pktSendBitsPack(pak, 3, obj->ipow);
        }
        else if (IS_TRAY_MACRO(obj->type))
        {
            pktSendString(pak, obj->command);
            pktSendString(pak, obj->shortName);
            pktSendString(pak, obj->iconName);
        }
    }
}

// This function only receives the player-controlled tray.
// The server-controlled tray is dictated by the server state, but managed independently on the client.
void sendTray( Packet *pak, Entity *e )
{
    int i;
    int build;
    TrayObj *obj;

    assert( e->pl->tray );

    for( i = 0; i < kCurTrayType_Count; ++i )
    {
        pktSendBits( pak, 32, e->pl->tray->current_trays[i]);
    }
     pktSendBits( pak, 32, e->pl->tray->trayIndexes );

    for (build = 0; build < MAX_BUILD_NUM; build++)
    {
        resetTrayIterator(e->pl->tray, build, kTrayCategory_PlayerControlled);
        while (getNextTrayObjFromIterator(&obj))
        {
            sendTrayObj(pak, obj);
        }
    }

    if(e->pchar->ppowDefault!=NULL)
    {
        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 32, e->pchar->ppowDefault->psetParent->idx );
        pktSendBits(pak, 32, e->pchar->ppowDefault->idx  );
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

//
//
//------------------------------------------------------------
// receive the tray
// -AB: refactored for new cur tray func :2005 Jan 26 02:55 PM
//----------------------------------------------------------

// Doesn't quite correspond to sendTrayObj... receiveTrayObjs receives some of the data
void receiveTrayObj(Packet *pak, TrayObj *obj, Entity *e, int type)
{
    int    ipow, iset;

    if ( type == kTrayItemType_Power )
    {
        iset = pktGetBits(pak,32);
        ipow = pktGetBits(pak,32);

        if(e &&    e->pchar)
        {
            if(iset    >= 0
                && iset<eaSize(&e->pchar->ppPowerSets)
                && ipow    >= 0
                && ipow<eaSize(&e->pchar->ppPowerSets[iset]->ppPowers))
            {
                buildPowerTrayObj(obj, iset, ipow, 0 );
                tray_getPowerStrings(e,    obj, iset, ipow);
            }
        }
    }
    else if    ( type == kTrayItemType_Inspiration    )
    {
        int    iCol = pktGetBitsPack(pak,3);
        int    iRow = pktGetBitsPack(pak,3);

        if(e &&    e->pchar)
        {
            if(iCol    >= 0
                && iCol<e->pchar->iNumInspirationCols
                && iRow    >= 0
                && iRow<e->pchar->iNumInspirationRows)
            {
                if(e->pchar->aInspirations[iCol][iRow]!=NULL)
                {
                    buildInspirationTrayObj(obj, iCol, iRow);
                }
            }
        }
    }
    else if( IS_TRAY_MACRO(type) )
    {
        memset(obj,    0, sizeof(TrayObj) );

        strncpyt( obj->command,    pktGetString( pak ), sizeof(obj->command) );
        strncpyt( obj->shortName, pktGetString(    pak    ), sizeof(obj->shortName) );
        strncpyt( obj->iconName, pktGetString( pak ), sizeof(obj->iconName)    );

        obj->type =    type;
    }
}

void receiveTrayObjs(Packet *pak, Entity *e)
{
    TrayObj    *obj;

    while (getNextTrayObjFromIterator(&obj))
    {
        int    type = pktGetBits(pak,5);

        if (type == kTrayItemType_None)
        {
            destroyCurrentTrayObjViaIterator();
        }
        else
        {
            if (!obj)
            {
                createCurrentTrayObjViaIterator(&obj);
            }
            receiveTrayObj(pak, obj, e, type);
        }
    }
}

// This function only receives the player-controlled tray.
// The server-controlled tray is dictated by the server state, but managed independently on the client.
void receiveTray( Packet *pak, Entity *e )
{
    int i;
    int build;
    int curBuild;

    for( i = 0; i < kCurTrayType_Count; ++i )
    {
        e->pl->tray->current_trays[i] = pktGetBits( pak, 32);

        // check that the number is in range, and silently fix it if not
        if( e->pl->tray->current_trays[i] < 0 || e->pl->tray->current_trays[i] >= TOTAL_PLAYER_TRAYS )
        {
            e->pl->tray->current_trays[i] = 0;
        }        
    }
    e->pl->tray->trayIndexes = pktGetBits(pak, 32);

    curBuild = character_GetActiveBuild(e);
    for (build = 0; build < MAX_BUILD_NUM; build++)
    {
        character_SelectBuildLightweight(e, build);
        resetTrayIterator(e->pl->tray, build, kTrayCategory_PlayerControlled);
        receiveTrayObjs(pak, e);
    }

    character_SelectBuildLightweight(e, curBuild);

    if(e)
        e->pchar->ppowDefault = NULL;

    if(pktGetBits(pak, 1))
    {
        int iset = pktGetBits(pak, 32);
        int ipow = pktGetBits(pak, 32);

        if(e && e->pchar
            && iset >= 0
            && iset<eaSize(&e->pchar->ppPowerSets)
            && ipow >= 0
            && ipow<eaSize(&e->pchar->ppPowerSets[iset]->ppPowers))
        {
            e->pchar->ppowDefault = e->pchar->ppPowerSets[iset]->ppPowers[ipow];
        }
    }
}


//------------------------------------------------------------
// convert a CurTrayType to an alt mask.
// -AB: created :2005 Jan 31 12:17 PM
//----------------------------------------------------------
CurTrayMask curtraytype_to_altmask( CurTrayType var ) 
{
    // if not one of the primary or alt trays, no mask is applicable
    if(INRANGE(var,kCurTrayType_Primary,kCurTrayType_Count))
        return (CurTrayMask)(1<<(var));
    return 0;
}



/* End of File */

InventoryType InventoryType_FromTrayItemType(TrayItemType type)
{
    static struct TrayToInv
    {
        int tray;
        int inv;
    } invs[] = 
        {
            { kTrayItemType_Salvage, kInventoryType_Salvage },
            { kTrayItemType_ConceptInvItem, kInventoryType_Concept },
            { kTrayItemType_Recipe, kInventoryType_Recipe },
        };


    int i;
    for( i = 0; i < ARRAY_SIZE( invs ); ++i ) 
    {
        if(invs[i].tray == type)
            break;
    }
    if(i<ARRAY_SIZE( invs ))
        return invs[i].inv;
    else
        return kInventoryType_Count;
}

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>

StaticDefineInt parse_TrayItemTypeEnum[] =
{
    DEFINE_INT
    {"None", kTrayItemType_None}, 
    {"Power", kTrayItemType_Power},
    {"Inspiration", kTrayItemType_Inspiration},
    {"BodyItem", kTrayItemType_BodyItem},
    {"EnhancementPower", kTrayItemType_SpecializationPower},
    {"Enhancement", kTrayItemType_SpecializationInventory},
    {"Macro", kTrayItemType_Macro},
    {"RespecPile", kTrayItemType_RespecPile},
    {"Tab", kTrayItemType_Tab},
    {"ConceptInvItem", kTrayItemType_ConceptInvItem},
    {"PetCommand", kTrayItemType_PetCommand},
    {"Salvage", kTrayItemType_Salvage},
    {"Recipe", kTrayItemType_Recipe},
    {"StoredInspiration", kTrayItemType_StoredInspiration},
    {"StoredEnhancement", kTrayItemType_StoredEnhancement},
    {"StoredSalvage", kTrayItemType_StoredSalvage},
    {"PersonalStoredSalvage", kTrayItemType_StoredSalvage},
    {"StoredRecipe", kTrayItemType_StoredRecipe},
    {"MacroHideName", kTrayItemType_MacroHideName},
    {"PLayerSlot", kTrayItemType_PlayerSlot},
    {"PlayerCreatedMission", kTrayItemType_PlayerCreatedMission},
    {"PlayerCreatedDetail", kTrayItemType_PlayerCreatedDetail},
    {"GroupMember", kTrayItemType_GroupMember},
    {"Count", kTrayItemType_Count},
    DEFINE_END
};

STATIC_ASSERT(ARRAY_SIZE( parse_TrayItemTypeEnum ) == kTrayItemType_Count+3);

TokenizerParseInfo parse_TrayItemIdentifier[] = 
{
    { "Type",        TOK_STRUCTPARAM|TOK_INT(TrayItemIdentifier,type,0),        parse_TrayItemTypeEnum},
    { "Name",        TOK_STRUCTPARAM|TOK_STRING(TrayItemIdentifier,name,0),    0},
    { "Level",        TOK_STRUCTPARAM|TOK_INT(TrayItemIdentifier,level,0),    0},
    { "", 0, 0 }
};

char* TrayItemIdentifier_Str(TrayItemIdentifier *ptii)
{
    static char ret[1024];
    if ( !ptii )
        return NULL;
    STR_COMBINE_BEGIN(ret);
    STR_COMBINE_CAT_D(ptii->type);
    STR_COMBINE_CAT_C(' ');
    STR_COMBINE_CAT(ptii->name);
    if ( ptii->level > 0 )
    {
        STR_COMBINE_CAT_C(' ');
        STR_COMBINE_CAT_D(ptii->level);
    }
    STR_COMBINE_END();
    return ret;
}

TrayItemIdentifier* TrayItemIdentifier_Struct(const char *str)
{
    static TrayItemIdentifier tii = {0};
    StructClear(parse_TrayItemIdentifier,&tii);
    memset(&tii,0,sizeof(TrayItemIdentifier));
    if(str && str[0] && ParserReadText(str,-1,parse_TrayItemIdentifier,&tii) && tii.name)
        return &tii;
    return NULL;
}

const char* TrayItemType_Str(TrayItemType type)
{
    return StaticDefineIntRevLookup(parse_TrayItemTypeEnum,type);
}

//
//
void tray_verifyNoDoubles(Tray *tray, int build)
{
    int i,j,k;

    for( i = 0; i < TOTAL_PLAYER_TRAYS; i++)
    {
        for( j = 0; j < TRAY_SLOTS; j++ )
        {
            TrayObj *trayj = getTrayObj(tray, kTrayCategory_PlayerControlled, i, j, build, false);

            if(!trayj)
                continue;

            if( trayj->type == kTrayItemType_Inspiration )
                clearTrayObj(tray, kTrayCategory_PlayerControlled, i, j, build);                    

            for( k = j+1; k < TRAY_SLOTS; k++ )
            {
                TrayObj *trayk = getTrayObj(tray, kTrayCategory_PlayerControlled, i, k, build, false);
                if(!trayk)
                    continue;
                if( trayj->type == kTrayItemType_Power &&
                    trayk->type == kTrayItemType_Power &&
                    trayj->type == trayk->type &&
                    trayj->ipow == trayk->ipow &&
                    trayj->iset == trayk->iset )
                {
                    clearTrayObj(tray, kTrayCategory_PlayerControlled, i, k, build);                    
                }
            }
        }
    }
}

int tray_changeIndex(TrayCategory category, int previousValue, int forward)
{
    if (category == kTrayCategory_PlayerControlled)
    {
        return (forward 
                ? SEQ_NEXT(previousValue, 0, (TOTAL_PLAYER_TRAYS)) 
                : SEQ_PREV(previousValue, 0, (TOTAL_PLAYER_TRAYS)));
    }
    else
    {
        return previousValue;
    }
}
