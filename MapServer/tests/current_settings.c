#include "container/dbcontainerpack.h"
#include "entity/gametypes.h"
#include "entity/chatSettings.h"
#include "entity/RewardToken.h"
#include "gameComm/pet.h"
#include <stdio.h>
#include <string.h>

#include "pet_names.inc"
#include "chat_tabs.inc"
#include "reward_tokens.inc"

static int checkChatTabs(void)
{
	StructDesc desc = {
		sizeof(ChatTabSettings), {AT_NOT_ARRAY, {{0}}},
		chat_settings_tab_line_desc
	};
	ChatTabSettings original = {0}, loaded = {0};
	strcpy(original.name, "Current custom tab");
	original.userBF = 5;
	original.optionsBF = 7;
	original.defaultChannel = 12;
	original.defaultType = 2;
	for (int i = 0; i < SYSTEM_CHANNEL_BITFIELD_SIZE; i++)
		original.systemChannels[i] = 0xa5a5a5a5 ^ i;
	for (int pass = 0; pass < 2; pass++) {
		char *record = dbContainerPackage(&desc, &original);
		int valid = strstr(record, "SystemChannelsBitField ") &&
			!strstr(record, "SystemChannels ");
		dbContainerUnpack(&desc, record, &loaded);
		free(record);
		if (!valid || memcmp(&original, &loaded, sizeof(original))) return 0;
		original = loaded;
	}
	return 1;
}

static int checkPetNames(void)
{
	StructDesc desc = {
		sizeof(PetName), {AT_NOT_ARRAY, {{0}}}, petname_line_desc
	};
	for (int number = 0; number < 3; number++) {
		PetName original = {0}, loaded = {0};
		strcpy(original.pchEntityDef, "current_pet_definition");
		strcpy(original.petName, "Custom pet \xc3\xa9");
		original.petNumber = number;
		for (int pass = 0; pass < 2; pass++) {
			char *record = dbContainerPackage(&desc, &original);
			int valid = !strstr(record, "PowerName ");
			dbContainerUnpack(&desc, record, &loaded);
			free(record);
			if (!valid || memcmp(&original, &loaded, sizeof(original))) return 0;
			original = loaded;
		}
	}
	return 1;
}

static int checkRewards(void)
{
	StructDesc desc = {
		sizeof(RewardToken), {AT_NOT_ARRAY, {{0}}}, reward_token_line_desc
	};
	const char *rewards[] = {
		"back_regular_cape", "auras_common", "event_valentine_pattern"
	};
	for (int i = 0; i < ARRAY_SIZE(rewards); i++) {
		RewardToken original = {0}, loaded = {0};
		original.reward = rewards[i];
		original.val = 1;
		original.timer = 100123;
		for (int pass = 0; pass < 2; pass++) {
			char *record = dbContainerPackage(&desc, &original);
			dbContainerUnpack(&desc, record, &loaded);
			free(record);
			if (!loaded.reward || strcmp(original.reward, loaded.reward) ||
				original.val != loaded.val || original.timer != loaded.timer)
				return 0;
			original = loaded;
		}
	}
	return 1;
}

int main(void)
{
	int valid = checkChatTabs() & checkPetNames() & checkRewards();
	puts(valid ? "Current settings passed" : "Current settings failed");
	return valid ? 0 : 1;
}
