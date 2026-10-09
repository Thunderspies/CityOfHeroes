#ifndef TEAMUP_H
#define TEAMUP_H

typedef struct Teamup Teamup;
typedef struct TeamMembers TeamMembers;
typedef struct League League;
typedef struct Supergroup Supergroup;
typedef struct SupergroupStats SupergroupStats;
typedef struct TaskForce TaskForce;
typedef struct Entity Entity;

void destroyTeamMembersContents(TeamMembers* members);

Teamup *createTeamup(void);
int team_IsMember( Entity *e, int db_id );
void destroyTeamup(Teamup *teamup);
void clearTeamup(Teamup *teamup);

int isTeamupAlignmentMixedClientside(Teamup *teamup);
bool entsAreTeamed( Entity* a, Entity* b, bool checkLeague );

TaskForce *createTaskForce(void);
void destroyTaskForce(TaskForce *taskforce);
void clearTaskForce(TaskForce *taskforce);


League *createLeague(void);
int league_IsMember( Entity *e, int db_id );
void destroyLeague(League *league);
void clearLeague(League *league);
int league_isMemberTeamLocked(Entity *e, int db_id);

int league_teamCount(League *league);
int league_getTeamLeadId(League *league, int teamIndex);
int league_getTeamMemberCount(League *league, int teamIndex);
int league_getTeamLockStatus(League *league, int teamIndex);

// useful supergroup defines
#define SGROUP_TITHE_FACTOR        100                            // displayed in whole percentage points
#define SGROUP_DEMOTE_FACTOR    (24 * 60 * 60)                // displayed in whole days
#define SGROUP_TITHE_MIN        (0 * SGROUP_TITHE_FACTOR)        
#define SGROUP_TITHE_MAX        (100 * SGROUP_TITHE_FACTOR)        
#define SGROUP_DEMOTE_MIN        (15 * SGROUP_DEMOTE_FACTOR)        
#define SGROUP_DEMOTE_MAX        (45 * SGROUP_DEMOTE_FACTOR)        

#endif
