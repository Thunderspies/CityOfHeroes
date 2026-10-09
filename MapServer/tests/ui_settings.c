// Exercise the production routines and real entity layout without a shard.
#include "container/dbcontainerpack.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gameComm/trayCommon.h"
#include "ui_settings.inc"

static int checkOptions(Entity *e, uiSetting *settings, int count, int pattern)
{
	int expected[32];
	for (int i = 0; i < count; i++) {
		int *value = siApplyMultipleIndirections((char *)e,
			settings[i].indirection, MAX_INDIRECTIONS);
		expected[i] = pattern == 0 ? 0 : pattern == 1 ? 1 : i % 2;
		*value = expected[i];
	}
	packageUISettings(e);
	for (int i = 0; i < count; i++) {
		int *value = siApplyMultipleIndirections((char *)e,
			settings[i].indirection, MAX_INDIRECTIONS);
		*value = !expected[i];
	}
	unpackUISettings(e);
	for (int i = 0; i < count; i++) {
		int *value = siApplyMultipleIndirections((char *)e,
			settings[i].indirection, MAX_INDIRECTIONS);
		if (*value != expected[i]) {
			fprintf(stderr, "UI option %s failed pattern %d\n",
				settings[i].indirection[1].fieldName, pattern);
			return 0;
		}
	}
	return 1;
}

int main(void)
{
	Entity *e = calloc(1, sizeof(*e));
	e->pl = calloc(1, sizeof(*e->pl));
	e->pl->tray = calloc(1, sizeof(*e->pl->tray));
	int valid = 1;
	for (int pattern = 0; pattern < 3; pattern++) {
		valid &= checkOptions(e, uiSettings, ARRAY_SIZE(uiSettings), pattern);
		valid &= checkOptions(e, uiSettings2, ARRAY_SIZE(uiSettings2), pattern);
		valid &= checkOptions(e, uiSettings3, ARRAY_SIZE(uiSettings3), pattern);
		valid &= checkOptions(e, uiSettings4, ARRAY_SIZE(uiSettings4), pattern);
	}
	free(e->pl->tray);
	free(e->pl);
	free(e);
	puts(valid ? "Current UI options passed" : "Current UI options failed");
	return valid ? 0 : 1;
}
