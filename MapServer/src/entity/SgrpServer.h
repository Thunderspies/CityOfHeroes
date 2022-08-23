#ifndef SGRPSERVER_H
#define SGRPSERVER_H

#include <utilitieslib/stdtypes.h>
#include "entity/Supergroup.h" // need enum, but don't want to break edit-and-continue

typedef struct Entity                Entity;
typedef struct Packet                Packet;
typedef struct Supergroup            Supergroup;
typedef struct BaseUpkeepPeriod        BaseUpkeepPeriod;
typedef struct SpecialDetail        SpecialDetail;
typedef struct SupergroupStats        SupergroupStats;

void sgroup_SetMode( Entity* player, int mode, int forceMode );
void sgroup_ToggleMode( Entity* player );
int sgroup_NameExists( char * name );
int sgroup_IsMember( Entity* leader, int db_id );
int supergroup_IsMember(Supergroup *sg, int idEnt);
int sgroup_rank( Entity *e, int db_id );
int sgroup_Create( Entity * creator, Supergroup *sg );
int sgroup_Update( Entity * e, Supergroup *sg, int on_create );
int sgroup_CreateDebug( Entity * creator, char * name );
int sgroup_JoinDebug(Entity *e, char *name);
int sgroup_ReceiveUpdate( Entity * e, Packet * pak);
void sgroup_AcceptOffer( Entity *leader, int new_dbid, char * new_name );
void sgroup_AcceptRelay(Entity* e, int sg_id, int invited_by);
void sgroup_AltRelay(Entity* e, int sg_id, int sg_type, int inviter_id, int inviter_auth_id);
void sgroup_KickMember( Entity *leader, char *name );
void sgroup_KickRelay(Entity* e, int kicked_by);
void sgroup_MemberQuit( Entity *quitter );
void sgroup_sendList( Packet * pak, Entity * e, int full_update );
void sgroup_refreshStats(int db_id); 
void sgroup_clearStats( Entity* e );
void sgroup_getStats( Entity * e );
SupergroupStats* sgroup_memberStats(Entity* e, U32 dbid);
void sgroup_promote( Entity *e, int promoted_by, int promoter_group, int promoter_rank, int new_rank );
void sgroup_promoteCsr( Entity *e, int promoted_by, int promoter_group, int promoter_rank, int new_rank );
void sgroup_initiate( Entity *e );
void sgroup_renameRank( Entity * e, char * name, int rank );
void sgroup_UpdateAllMembers(Supergroup* group);
void sgroup_UpdateAllMembersAnywhere(Supergroup* group);
void sgroup_setSuperCostume( Entity *e, int send );
void sgroup_startConfirmation(Entity *e, char *player_name);
int sgroup_verifyCostume( Packet *pak, Entity *e );
int sgroup_verifyCostumeEx( Packet *pak, Entity *e );
void sgroup_sendResponse( Entity * e, int valid );
void sgroup_rename(Entity* player, char* oldname, char* newname, int dbid_response);
void sgroup_CsrWho( Entity * e, int leaderOnly );
char *sgroup_FindLongestWithPermission(Entity* ent, sgroupPermissionEnum permission);
void sgroup_RefreshClient(Entity *e);
void sgroup_RefreshSGMembers(int sg_dbid);
bool sgroup_LockAndLoad(int sgid, Supergroup *sg);
void sgroup_UpdateUnlock(int sgid, Supergroup *sg);

void sgroup_setGeneric(Entity *e, int permission, void(*func)(Entity*, void*, void*), void* param, void* param2, char* actionName);
void sgroup_genSetMOTD(Entity *e, void* msg, void* notused);
void sgroup_genSetMotto(Entity *e, void* msg, void* notused);
void sgroup_genSetDescription(Entity* e, void* str, void* notused);
void sgroup_genSetTithe(Entity* e, void* tithe, void* notused);
void sgroup_genSetDemoteTimeout(Entity* e, void* demote, void* notused);
void sgroup_genSetRankName(Entity* e, void* rank, void* name);
void sgroup_genRenameRank(Entity* e, void* rank, void* name);
void sgroup_genSetRank(Entity* e, void* dbid, void* newrank);
void sgroup_genSetIsDark(Entity* e, void* dbid, void* unUsed);
void sgroup_genConvertRanks(Entity* e, void* notused, void* notused2);
void sgroup_genFixLeaderPermissions(Entity* e, void* notused, void* notused2);
void sgroup_genFixRankNames(Entity* e, void* notused, void* notused2);
void sgroup_genLockSGPlayerType(Entity* e, void* notused, void* notused2);

int sgroup_TitheClamp(int tithe);
int sgroup_DemoteClamp(int demotetimeout);

void sgroup_setIsDark(Entity *e);

void sgroup_sendRegistrationList(Entity* e, int sort_by_prestige, char *name);

int entity_AddSupergroupRewardToken(Entity *, const char *);
bool entity_RemoveSupergroupRewardToken(Entity *, const char *);

int entity_AddSupergroupRaidPoints(Entity *e, int Points, int bFromRaid);
void entity_GetSupergroupRaidPoints(Entity *e, int *Points, int *raidPoints, int *raidTime, int *copPoints, int *copTime);

typedef struct Detail Detail;
typedef struct Base Base;

void sgroup_AddSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation, U32 timeExpiration, U32 iFlags);
void sgroup_RemoveSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation);
void sgroup_ExtendSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation, U32 extendAdditionalTime);
void sgroup_ExchangeIoPs(Entity *e, const Detail *pOldDetail, U32 timeOldCreation, const Detail *pNewDetail, U32 timeNewCreation, U32 timeExpiration, U32 iFlags);
void sgroup_ValidateSpecialDetails(Base *pBase, Supergroup * sg);
SpecialDetail *FindSpecialDetail(Supergroup *supergroup, const Detail *pDetail, U32 timeCreation);
SpecialDetail *FindSpecialDetailByName(Supergroup *supergroup, const char *pchName);

int sgroup_CalcSpacesForItemsOfPower(Base *pBase, Supergroup *sg);
bool sgroup_CanPortToBase(Entity *e);
float sgroup_XPBonus(Entity *e);


void sgroup_UpdatePrestigeBaseAndUpkeep(Supergroup *sg, int idSgrp, bool locked);
void sgroup_SaveBase(Base *pBase, Supergroup *sgAlreadyLocked, int force_save);

// These lock and modify the supergroup recipe list
void sgrp_SetRecipe(Entity *e, DetailRecipe const *recipe, int count, bool isUnlimited);
void sgrp_AdjRecipe(Entity *e, DetailRecipe const *recipe, int count, bool isUnlimited);

void sgroup_Tick(Entity *e);



int sgroup_buyBase( Entity * e );

bool sgroup_canPayRent( Entity * e );
int sgroup_payRent( Entity * e );
int sgroup_buyBase( Entity * e );

void sgroup_UpdateBonusPrestige(Entity *e, bool bLocked, int iExtraDelta);

int checkSgrpMembers(Entity* e, char* calledFrom);
int sgroup_RepairCorruption(Supergroup *sg, int justChecking);

int sgrp_CanSetPermissions(Entity *e);

#endif
