#ifndef POWER_CUSTOMIZATION_H__
#define POWER_CUSTOMIZATION_H__

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/Color.h>

typedef struct Entity Entity;
typedef struct Packet Packet;
typedef struct BasePowerSet BasePowerSet;
typedef struct BasePower BasePower;
typedef struct PowerCustomization
{
    const char *powerFullName; // sometimes this points at an allocated string, sometimes it points into a BasePower
    const BasePower *power;
    ColorPair customTint;
    const char* token;
} PowerCustomization;
typedef struct DBPowerCustomization
{
    const char * powerCatName;
    const char * powerSetName;
    const char * powerName;
    ColorPair customTint;
    const char* token;
    int slotId;                    //    base 1
}DBPowerCustomization;
typedef struct PowerCustomizationList
{
    PowerCustomization **powerCustomizations;
}PowerCustomizationList;

typedef struct PowerCustomizationCost
{
    int min_level;
    int max_level;
    int entry_fee;
    int power_token_cost;
    int power_color_cost;

}PowerCustomizationCost;

typedef struct PowerCustomizationLevelsCost
{
    PowerCustomizationCost **powerCustCostList;
}PowerCustomizationLevelsCost;

typedef struct PowerCustomizationMenuCategory
{
    char *categoryName;
    char *hideIf;
    char **powerSets;
}PowerCustomizationMenuCategory;
typedef struct PowerCustomizationMenu
{
    PowerCustomizationMenuCategory **pCategories;
}PowerCustomizationMenu;
extern TokenizerParseInfo ParsePowerCustomization[];
extern TokenizerParseInfo ParsePowerCustomizationList[];
extern TokenizerParseInfo ParseDBPowerCustomization[];
extern TokenizerParseInfo ParsePowerCustomizationCost[];
extern TokenizerParseInfo ParsePowerCustomizationLevelsCost[];
extern SERVER_SHARED_MEMORY PowerCustomizationMenu gPowerCustomizationMenu;
int comparePowerCustomizations(const void* lhsData, const void* rhsData);
void loadPowerCustomizations();
PowerCustomizationList * powerCustList_current( Entity * e );
// Return the PowerCustomization associated with this power, or NULL if not customizable.
PowerCustomization* powerCust_FindPowerCustomization(PowerCustomizationList* powerCust, const BasePower *power);
PowerCustomizationList * powerCustList_clone(PowerCustomizationList *powerCustList);
void powerCustList_initializePowerFromFullName(PowerCustomizationList *powerCustList);
int powerCustList_receive(Packet *pak, Entity *e, PowerCustomizationList* powerCustList);
void powerCustList_send(Packet* pak, PowerCustomizationList *powerCustList);
void powerCustList_allocPowerCustomizationList(PowerCustomizationList *powerCustList, int numPowersToAlloc);
int powerCustList_validate(PowerCustomizationList *powerCustList, Entity *e, int fixup);
void initializePowerCustomizationPower(DBPowerCustomization *dbPower, PowerCustomization *power);
void powerCustList_destroy( PowerCustomizationList *powerCustList);
PowerCustomizationList *powerCustList_new_slot( Entity *e);
int powerCust_validate(PowerCustomization *customization, Entity *e, int fixup);
int powerCust_CalcPowerCost( Entity *e, PowerCustomizationList *originalPowerCustList, PowerCustomizationList *newPowerCustList, const BasePower *power, int *colorChangeCharged);
int powerCust_PowerSetCost( Entity *e, PowerCustomizationList *originalPowerCustList, PowerCustomizationList *newPowerCustList, const BasePowerSet *psetBase, int *colorChangeCharged );
int powerCust_GetChangeCost( Entity *e, PowerCustomizationList *oldList, PowerCustomizationList *newList, int *colorChangeOccurred );
int powerCust_GetFee(Entity *e);
int powerCust_GetTotalCost( Entity *e, PowerCustomizationList *originalPowerCustList, PowerCustomizationList *newPowerCustList );
void powerCust_initHashTable();
void powerCust_syncPowerCustomizations(Entity* e);
void powerCust_handleAllEmptyPowerCust(Entity *e);
#endif