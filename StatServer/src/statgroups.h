#pragma once

// main
void stat_LeagueReset(void);
void stat_LeagueUpdateTick();
// containers
void stat_LeagueDeleteMe(int dbid);
void stat_LeagueUnpack(char *container_data, int dbid, int *members, int member_count);

// cmds

void stat_LeagueJoin(int leader_id, int joiner_id, int team_leader_id, char *invitee_name, int teamLockStatus1, int teamLockStatus2);
void stat_LeagueJoinTurnstile(int leader_id, int db_id, char *invitee_name, int desiredTeam, int isTeamLeader);
void stat_LeaguePromote(int old_leader_id, int new_leader_id);
void stat_LeagueChangeTeamLeaderTeam(int league_id, int old_teamLeaderId, int newTeamLeaderId);
void stat_LeagueChangeTeamLeaderSolo(int league_id, int db_id, int newTeamLeaderId);
void stat_LeagueUpdateTeamLock(int league_id, int team_leader_id, int teamLockStatus);
void stat_LeagueQuit(int quitter_id, int voluntaryLeave, int removeContainer);


