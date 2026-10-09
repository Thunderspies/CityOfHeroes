#include <utilitieslib/stdtypes.h>
#include "entity/gametypes.h"
#include <stdio.h>

typedef struct Creation {
	PlayerType playerType;
	PraetorianProgress praetorian;
} Creation;

typedef struct Player {
	PraetorianProgress praetorianProgress;
} Player;

typedef struct Character {
	Player *pl;
	unsigned int db_flags;
} Character;

static unsigned int createOrigin(Creation *cbData)
{
	char cmd_buf[1000];
	int idx = 0;
#include "epic_origin.inc"
	unsigned int stored = 0;
	return sscanf(cmd_buf, "DbFlags %u", &stored) == 1 ? stored : ~0U;
}

static void chooseHero(Character *e)
{
#include "epic_hero.inc"
}

static void chooseVillain(Character *e)
{
#include "epic_villain.inc"
}

int main(void)
{
	struct {
		Creation character;
		unsigned int expected;
	} cases[] = {
		{{kPlayerType_Hero, kPraetorianProgress_PrimalBorn},
			DBFLAG_UNLOCK_HERO_EPICS},
		{{kPlayerType_Villain, kPraetorianProgress_PrimalBorn},
			DBFLAG_UNLOCK_VILLAIN_EPICS},
		{{kPlayerType_Hero, kPraetorianProgress_NeutralInPrimalTutorial},
			DBFLAG_UNLOCK_HERO_EPICS},
		{{kPlayerType_Hero, kPraetorianProgress_Tutorial}, 0},
		{{kPlayerType_Villain, kPraetorianProgress_Tutorial}, 0},
		{{kPlayerType_Hero, kPraetorianProgress_Praetoria}, 0},
		{{kPlayerType_Villain, kPraetorianProgress_Praetoria}, 0}
	};
	int valid = 1;
	for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
		valid &= createOrigin(&cases[i].character) == cases[i].expected;
	for (int villain = 0; villain <= 1; villain++) {
		Player player = {kPraetorianProgress_NeutralInPrimalTutorial};
		Character character = {&player,
			DBFLAG_UNLOCK_HERO_EPICS | DBFLAG_RENAMEABLE};
		if (villain)
			chooseVillain(&character);
		else
			chooseHero(&character);
		unsigned int expected = DBFLAG_RENAMEABLE | (villain ?
			DBFLAG_UNLOCK_VILLAIN_EPICS : DBFLAG_UNLOCK_HERO_EPICS);
		valid &= character.db_flags == expected;
		valid &= player.praetorianProgress == kPraetorianProgress_PrimalBorn;
	}
	puts(valid ? "Epic origin flags passed" : "Epic origin flags failed");
	return valid ? 0 : 1;
}
