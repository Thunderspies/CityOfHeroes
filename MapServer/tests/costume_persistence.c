#include "container/dbcontainerpack.h"
#include "entity/entity.h"
#include "entity/powers.h"
#include "entity/costume_db.h"
#include <utilitieslib/components/earray.h>
#include <math.h>
#include <stdarg.h>

// The game dictionary and list allocation are boundaries for this test.
// Production packing, slot selection, scale codecs and descriptors run below.
TokenizerParseInfo ParsePowerCustomization[] = {{0}};
TokenizerParseInfo ParseDBPowerCustomization[] = {{0}};
static PowerCategory category = {.pchName = "CurrentCategory"};
static BasePowerSet powerset = {.pchName = "CurrentSet", .pcatParent = &category};
static BasePower power = {.pchName = "CurrentPower", .psetParent = &powerset};
static int invalid_records;

int costume_get_num_slots(Entity *e) { return 2; }
int GetBodyPartCount(void) { return 0; }
Costume *costume_create(int parts) { return calloc(1, sizeof(Costume)); }
void dbLog(const char *name, Entity *e, const char *format, ...)
{
	invalid_records++;
}
void initializePowerCustomizationPower(DBPowerCustomization *db,
	PowerCustomization *custom)
{
	custom->power = !stricmp(db->powerName, power.pchName) ? &power : NULL;
}
void powerCustList_allocPowerCustomizationList(PowerCustomizationList *list,
	int count)
{
	while (eaSize(&list->powerCustomizations) < count)
		eaPush(&list->powerCustomizations, StructAllocRaw(sizeof(PowerCustomization)));
}
int comparePowerCustomizations(const void *a, const void *b) { return 0; }
void powerCust_handleAllEmptyPowerCust(Entity *e) {}

#include "costume_descriptors.inc"
#include "costume_scales.inc"
#include "costume_pack.inc"
#include "costume_unpack.inc"

static void clearPowerLists(Entity *e)
{
	for (int i = 0; i < 2; i++) {
		PowerCustomizationList *list = e->pl->powerCustomizationLists[i];
		if (!list) continue;
		for (int j = 0; j < eaSize(&list->powerCustomizations); j++)
			StructDestroy(ParsePowerCustomization, list->powerCustomizations[j]);
		eaDestroy(&list->powerCustomizations);
		StructFree(list);
		e->pl->powerCustomizationLists[i] = NULL;
	}
}

static int checkPowerSlots(Entity *e)
{
	DBPowerCustomizationList *db = calloc(1, sizeof(*db));
	for (int i = 0; i < 5; i++) {
		db->powerCustomization[i].powerName = power.pchName;
		db->powerCustomization[i].powerCatName = category.pchName;
		db->powerCustomization[i].powerSetName = powerset.pchName;
		db->powerCustomization[i].token = "currenttoken";
		db->powerCustomization[i].customTint.primary.integer = 0x11223344;
	}
	db->powerCustomization[0].slotId = 2;
	db->powerCustomization[1].slotId = 1;
	db->powerCustomization[2].slotId = 0;
	db->powerCustomization[3].slotId = -1;
	db->powerCustomization[4].slotId = 3;
	invalid_records = 0;
	puts("Checking valid and invalid customization slots");
	unpackPowerCustomizations(e, db);
	int valid = invalid_records == 3;
	for (int i = 0; i < 2; i++) {
		PowerCustomizationList *list = e->pl->powerCustomizationLists[i];
		valid &= eaSize(&list->powerCustomizations) == 1;
		valid &= list->powerCustomizations[0]->customTint.primary.integer == 0x11223344;
	}
	for (int pass = 0; pass < 2; pass++) {
		puts("Round-tripping current power customizations");
		StuffBuff buffer;
		initStuffBuff(&buffer, 4096);
		packagePowerCustomizations(e, &buffer, powercustomization_list_desc);
		memset(db, 0, sizeof(*db));
		dbContainerUnpack(powercustomization_list_desc, buffer.buff, db);
		free(buffer.buff);
		valid &= db->powerCustomization[0].slotId == 1;
		valid &= db->powerCustomization[1].slotId == 2;
		valid &= db->powerCustomization[0].token &&
			!strcmp(db->powerCustomization[0].token, "currenttoken");
		clearPowerLists(e);
		unpackPowerCustomizations(e, db);
	}
	clearPowerLists(e);
	puts("Checking overfull customization slot");
	for (int i = 0; i <= MAX_POWERS; i++) {
		db->powerCustomization[i] = db->powerCustomization[0];
		db->powerCustomization[i].slotId = 1;
	}
	invalid_records = 0;
	unpackPowerCustomizations(e, db);
	valid &= invalid_records == 1;
	valid &= eaSize(&e->pl->powerCustomizationLists[0]->powerCustomizations) == MAX_POWERS;
	clearPowerLists(e);
	free(db);
	return valid;
}

static int checkAppearance(Entity *e)
{
	DBAppearance db = {0};
	for (int i = 0; i < 2; i++) {
		e->pl->costume[i] = calloc(1, sizeof(Costume));
		Appearance *a = &e->pl->costume[i]->appearance;
		a->bodytype = i + 1;
		a->colorSkin.integer = 0x12345678 + i;
		a->convertedScale = 1;
		for (int j = 0; j < MAX_BODY_SCALES; j++)
			a->fScales[j] = (i + 1) * 0.125f;
		for (int j = 0; j < NUM_3D_BODY_SCALES; j++)
			for (int k = 0; k < 3; k++)
				a->f3DScales[j][k] = (k - 1) * 0.5f;
	}
	int valid = 1;
	for (int pass = 0; pass < 2; pass++) {
		puts("Round-tripping current appearance scales");
		StuffBuff buffer;
		initStuffBuff(&buffer, 4096);
		packageAppearances(e, &buffer, appearances_desc);
		valid &= !strstr(buffer.buff, "ConvertedScale");
		dbContainerUnpack(appearances_desc, buffer.buff, &db);
		free(buffer.buff);
		for (int i = 0; i < 2; i++)
			memset(&e->pl->costume[i]->appearance, 0, sizeof(Appearance));
		unpackAppearance(e, &db);
		for (int i = 0; i < 2; i++) {
			Appearance *a = &e->pl->costume[i]->appearance;
			valid &= a->convertedScale == 1 && a->bodytype == i + 1;
			valid &= a->colorSkin.integer == 0x12345678 + i;
			// Arm scale has no persisted column in the current schema.
			for (int j = 0; j < NUM_2D_BODY_SCALES - 1; j++)
				valid &= fabsf(a->fScales[j] - (i + 1) * 0.125f) < 0.0001f;
			for (int j = 0; j < NUM_3D_BODY_SCALES; j++)
				for (int k = 0; k < 3; k++)
					valid &= fabsf(a->f3DScales[j][k] - (k - 1) * 0.5f) < 0.011f;
		}
	}
	for (int i = 0; i < 2; i++) free(e->pl->costume[i]);
	return valid;
}

int main(void)
{
	setbuf(stdout, NULL);
	Entity *e = calloc(1, sizeof(*e));
	e->pl = calloc(1, sizeof(*e->pl));
	e->pl->num_costumes_stored = 2;
	int valid = checkPowerSlots(e) & checkAppearance(e);
	free(e->pl);
	free(e);
	puts(valid ? "Costume persistence passed" : "Costume persistence failed");
	return valid ? 0 : 1;
}
