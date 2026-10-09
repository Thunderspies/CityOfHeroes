// Exercise the production routines and real entity layout without a shard.
#include "container/dbcontainerpack.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include "gameComm/trayCommon.h"
#include "ui_settings.inc"

static int invalid_builds;
void dbLog(const char *name, Entity *e, const char *format, ...)
{
	invalid_builds++;
}

static int checkBuildInitialization(Entity *e)
{
	for (int build = -1; build <= MAX_BUILD_NUM; build++) {
		e->pchar->iCurBuild = build;
		e->pchar->iActiveBuild = -1;
		for (int i = 0; i < MAX_BUILD_NUM; i++)
			e->pchar->iBuildLevels[i] = i + 10;
		invalid_builds = 0;
		#include "build_initialization.inc"
		int invalid = build < 0 || build >= MAX_BUILD_NUM;
		int expected = invalid ? 0 : build;
		if (e->pchar->iCurBuild != expected ||
			e->pchar->iActiveBuild != expected || invalid_builds != invalid)
			return 0;
		for (int i = 0; i < MAX_BUILD_NUM; i++)
			if (e->pchar->iBuildLevels[i] != i + 10) return 0;
	}
	return 1;
}

static int checkOptions(Entity *e, uiSetting *settings, int count, int pattern)
{
	int expected[32];
	for (int i = 0; i < count; i++) {
		if (!settings[i].indirection[0].fieldName) continue;
		int *value = siApplyMultipleIndirections((char *)e,
			settings[i].indirection, MAX_INDIRECTIONS);
		expected[i] = pattern == 0 ? 0 : pattern == 1 ? 1 : i % 2;
		*value = expected[i];
	}
	packageUISettings(e);
	for (int i = 0; i < count; i++) {
		if (!settings[i].indirection[0].fieldName) continue;
		int *value = siApplyMultipleIndirections((char *)e,
			settings[i].indirection, MAX_INDIRECTIONS);
		*value = !expected[i];
	}
	unpackUISettings(e);
	for (int i = 0; i < count; i++) {
		if (!settings[i].indirection[0].fieldName) continue;
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

static int checkCurrentPreferenceBits(Entity *e)
{
    unsigned int expected = (1u << 21) | (1u << 24) | (1u << 28);
    e->pl->uiSettings2 = expected | (1u << 20);
    unpackUISettings(e);
    if (!e->pl->hideDeleteSalvagePrompt || !e->pl->hideDeleteRecipePrompt ||
        !e->pl->is_a_spammer || e->pl->declineSuperGroupInvite ||
        e->pl->declineTradeInvite || e->pl->webHideBadges ||
        e->pl->webHideFriends || e->pl->hideCoopPrompt)
        return 0;
    packageUISettings(e);
    return (unsigned int)e->pl->uiSettings2 == expected;
}
int main(void)
{
	Entity *e = calloc(1, sizeof(*e));
	e->pl = calloc(1, sizeof(*e->pl));
	e->pchar = calloc(1, sizeof(*e->pchar));
	e->pl->tray = calloc(1, sizeof(*e->pl->tray));
	int valid = checkBuildInitialization(e) && checkCurrentPreferenceBits(e);
	for (int pattern = 0; pattern < 3; pattern++) {
		valid &= checkOptions(e, uiSettings, ARRAY_SIZE(uiSettings), pattern);
		valid &= checkOptions(e, uiSettings2, ARRAY_SIZE(uiSettings2), pattern);
		valid &= checkOptions(e, uiSettings3, ARRAY_SIZE(uiSettings3), pattern);
		valid &= checkOptions(e, uiSettings4, ARRAY_SIZE(uiSettings4), pattern);
	}
	free(e->pl->tray);
	free(e->pl);
	free(e->pchar);
	free(e);
	puts(valid ? "Current UI options passed" : "Current UI options failed");
	return valid ? 0 : 1;
}
