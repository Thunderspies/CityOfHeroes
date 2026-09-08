#ifndef UIGROUPWINDOW_H
#define UIGROUPWINDOW_H

#include "UI/uiInclude.h"
#include <utilitieslib/stdtypes.h>
#include "entity/character_base.h"
#include "entity/teamCommon.h"

int groupWindow();

typedef struct ComboBox ComboBox;
typedef struct TeamMembers TeamMembers;
void addLFG( ComboBox * cb );


int team_select( TeamMembers *members, int idx );
void setTeamBuffMode( int mode );
void searchClearComment();

void drawGroupInterior( float x, float y, float z, float wd, float scale, float hp_percent, float absorb_percent, float end_percent, int barColor, 
                       char * txt, int selected, int active, int offMap, int leader, AtlasTex *icon, int i, int isSelf, int pet, int currentTab, int oldStyleTeamUI );

typedef struct SearchOption
{
    int        bitmask;
    char    *displayName;
    char    *displayTitle;
    int        clear_others;
    char    *texName;
    int        color;
    int        icon_color;
}SearchOption;

SearchOption * lfg_getSearchOption( int lfg );
void league_updateTabCount();
typedef char MapName[256];
extern MapName teamMapNames[MAX_TEAM_MEMBERS];
extern MapName levelingpactMapNames[MAX_LEVELINGPACT_MEMBERS];
extern MapName leagueMapNames[MAX_LEAGUE_MEMBERS];

#endif