#pragma once

// main
void stat_LevelingPactReset(void);
void stat_LevelingPactUpdateTick(int flush);
void stat_LevelingPactRequestActivityCheckList();
void stat_LeagueReset(void);
void stat_LeagueUpdateTick();
// containers
void stat_LevelingPactDeleteMe(int dbid);
void stat_LevelingPactUnpack(char *container_data, int dbid, int *members, int member_count);
void stat_LeagueDeleteMe(int dbid);
void stat_LeagueUnpack(char *container_data, int dbid, int *members, int member_count);

// cmds
// void stat_LevelingPactQuit(int entid); // this happens on the map now
void stat_LevelingPactAddXP(int dbid, int xp, int time);
void stat_LevelingPactAddInf(int entDbid, int inf, int isHero);
void stat_LevelingPactGetInf(int entDbid);
void stat_LevelingPactDbidToAuth(int dbid, const char * auth);
void stat_LevelingPactRemoveInactive(int dbid);
void stat_levelingPactUpdateVersion(int dbid, int isVillain);

void stat_LeagueJoin(int leader_id, int joiner_id, int team_leader_id, char *invitee_name, int teamLockStatus1, int teamLockStatus2);
void stat_LeagueJoinTurnstile(int leader_id, int db_id, char *invitee_name, int desiredTeam, int isTeamLeader);
void stat_LeaguePromote(int old_leader_id, int new_leader_id);
void stat_LeagueChangeTeamLeaderTeam(int league_id, int old_teamLeaderId, int newTeamLeaderId);
void stat_LeagueChangeTeamLeaderSolo(int league_id, int db_id, int newTeamLeaderId);
void stat_LeagueUpdateTeamLock(int league_id, int team_leader_id, int teamLockStatus);
void stat_LeagueQuit(int quitter_id, int voluntaryLeave, int removeContainer);


