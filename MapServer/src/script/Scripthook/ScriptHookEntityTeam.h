/*\
 *
 *    scripthookentityteam.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to script entity/teams
 *
 */

#ifndef SCRIPTHOOKENTITYTEAM_H
#define SCRIPTHOOKENTITYTEAM_H

int ScriptTeamValid(const char* name);
int NumEntitiesInTeam(TEAM team);
int NumEntitiesInTeamEvenDead(TEAM team);
int NumTargetableEntitiesInTeam(TEAM team);
int NumRescueValidEntitiesInTeam(TEAM team, ENTITY exclude);
ENTITY GetEntityFromTeam(TEAM team, NUMBER number);        // number is 1-based
ENTITY GetPlayerFromPlayerTeam(TEAM team, int index);    // number is 1-based - only works with playerteams
ENTITY GetPlayerTeamFromPlayer(ENTITY player);            // converts a player into a playerteam
int NumTeamsInTeam(TEAM team);                            // these two functions get ai teams
TEAM GetTeamFromTeam(TEAM team, NUMBER number);            // number is 1-based
void Kill(TEAM team, NUMBER giveKillCredit );
// void Destroy(TEAM team);                                // This DESTROYS the ent.
void Say(ENTITY entity, STRING text, int priorityLevel ); //priorityLevel is 0(LOW) 1(DONT USE YET) or 2(HIGH) 3(AUDIENCE ONLY)
void ScriptServeFloater(ENTITY entity, STRING text);
void ScriptSendChatMessage(ENTITY entity, STRING text);
void SetAudience(ENTITY entity, ENTITY audience);
void SayOnClick(ENTITY entity, STRING text);
void RewardOnClick(TEAM team, STRING reward);
void ConditionOnClick(TEAM team, OnClickConditionType type, STRING condition, STRING say, STRING reward);
void ConditionOnClickClear(TEAM team);
void SayOnKillHero(TEAM team, STRING text);
ENTITY CreateVillainNearEntity(TEAM teamName, STRING villainType, NUMBER level, STRING name, ENTITY target, FRACTION radius);
ENTITY CreateVillain(TEAM teamName, STRING villainType, NUMBER level, STRING name, LOCATION location);
ENTITY CreateDoppelganger(TEAM teamName, STRING doppleFlags, NUMBER level, STRING name, LOCATION location, ENTITY original, STRING customCritterDef,
                          STRING villainRank, STRING displayGroup, bool bossReduction);
void EntityRotateToFaceDirection(ENTITY entity, FRACTION facing);
void SetScriptTeam( TEAM currentTeam, TEAM additionalTeam );
void SwitchScriptTeam( TEAM team, TEAM fromTeam, TEAM newTeam );
void LeaveAllScriptTeams(ENTITY entity);
STRING EntityName(ENTITY ent);
STRING EntityPowerCreatedMe(ENTITY ent);
ENTITY EntityFromDBID( NUMBER dbid);
NUMBER DBIDFromPlayer(ENTITY player);
NUMBER AreAllTeamMembersPresent( ENTITY ent );
void OverridePrisonGurney( ENTITY ent, NUMBER overrideFlag );
void OverridePrisonGurneyWithTarget( ENTITY ent, NUMBER overrideFlag, STRING spawnTarget );
void TeleportToLocation(ENTITY team, LOCATION loc);
void ScriptEnterScriptDoor(TEAM team, STRING zoneName, STRING spawnName);
void ScriptSetSpecialReturnInProgress(TEAM team);
void ScriptDestroyAllPets(TEAM team);
void EntityActivatePower(ENTITY entity, ENTITY target, STRING powerTitle);
void EntityDeactivateTogglePower(ENTITY entity, STRING powerTitle);
ENTITY MyEntity();

NUMBER GetAIGang(ENTITY entity);
NUMBER GetAIAlly(ENTITY entity);
NUMBER GetPlayerType(ENTITY entity);
NUMBER GetPlayerTypeByLocation(ENTITY entity);
NUMBER GetPlayerSGID(ENTITY entity);
NUMBER GetPlayerRaidID(ENTITY entity);
int IsEntityHero(ENTITY entity);
int IsEntityOnBlueSide(ENTITY entity);
int IsEntityVillain(ENTITY entity);
int IsEntityOnRedSide(ENTITY entity);
int IsEntityPrimal(ENTITY entity);
int IsEntityPraetorian(ENTITY entity);
int IsEntityPraetorianTutorial(ENTITY entity);
int IsEntityMonster(ENTITY entity);
int IsEntityPlayer(ENTITY entity);
int IsEntityCritter(ENTITY entity);
int IsEntityHidden(ENTITY entity);
int IsEntityOnTeam(ENTITY entity);
int IsEntityOnScriptTeam(ENTITY player, TEAM team);
int IsEntityOutside(ENTITY entity);
int IsEntityNearGround(ENTITY entity, FRACTION dist);
int IsEntityMoving(ENTITY entity);
int IsEntityUntargetable(ENTITY entity);
int IsEntityAFK(ENTITY entity);
int IsEntityPhased(ENTITY entity);
int IsEntityActive(ENTITY entity);
int IsEntityOnTaskforce(ENTITY entity);
int IsEntityOnFlashback(ENTITY entity);
int IsEntityOnArchitect(ENTITY entity);
STRING GetOrigin(ENTITY entity);
STRING GetClass(ENTITY entity);
STRING GetGender(ENTITY entity);
STRING GetPlayerAlignmentString(ENTITY entity);
STRING GetPlayerPraetorianProgressString(ENTITY entity);
STRING GetAllyString(ENTITY entity);
int EvalEntityRequires(ENTITY entity, STRING expr, STRING dataFilename);

NUMBER GetMonsterCreateTime(ENTITY entity);

FRACTION GetHealth(ENTITY entity);
FRACTION GetEndurance(ENTITY entity);
FRACTION GetAbsorb(ENTITY entity);
FRACTION SetHealth(ENTITY entity, FRACTION percentHealth, NUMBER unAttackedOnly );
void SetPosition( ENTITY entity, LOCATION loc );
void SetMap (ENTITY entity, NUMBER map, STRING spawnTarget);
void ScriptApplyKnockBack(ENTITY player, Vec3 push, int dokb);
STRING GetVillainDefName( ENTITY entity );
ENTITY GetVillainByDefName( STRING defName, ENTITY cachedEntity );
STRING GetVillainDefClassName( ENTITY entity );
NUMBER GetVillainGroupID( ENTITY entity );
NUMBER GetVillainGroupIDFromName( STRING name );
STRING GetDisplayName( ENTITY entity );
NUMBER GetLevel( ENTITY entity );
NUMBER GetCombatLevel( ENTITY entity );
NUMBER GetExperienceLevel( ENTITY entity );
NUMBER CanLevel( ENTITY entity );
NUMBER GetAccessLevel( ENTITY entity );
FRACTION GetReputation( ENTITY entity );
NUMBER GetNPCIndex( ENTITY entity );
NUMBER SameTeam( ENTITY ent1, ENTITY ent2 );
NUMBER ValidForPvPReward( ENTITY killer, ENTITY target);
void SetSpecialMapReturnData(TEAM team, STRING type, STRING value);
STRING GetSpecialMapReturnData(ENTITY player);
void ScriptSendDialog(TEAM team, STRING message);
NUMBER GetTeamSizeOverride( void );

NUMBER GetAllEntities( ENTITY    * entList, 
                   STRING    villainGroup,
                   STRING    villainDef,
                   STRING    villainHasTag,
                   STRING    entType,        //Not yet implemented
                   STRING  scriptTeam,    
                   FRACTION radius, 
                   LOCATION center,
                   AREA    area,
                   NUMBER  isAlive,
                   NUMBER  isDead,
                   NUMBER  max,
                   NUMBER  onceOnly
                   );

NUMBER GetAllPets(    ENTITY    source,
                    ENTITY    * entList, 
                    STRING    villainGroup,    
                    STRING    villainDef,
                    STRING    villainHasTag,
                    STRING    entType,        //Not yet implemented
                    STRING  scriptTeam,        //Not yet implemented
                    FRACTION radius, 
                    LOCATION center,
                    AREA    area,
                    NUMBER  isAlive,
                    NUMBER  isDead,
                    NUMBER  max
                    );

char * getContactHeadshotScriptSMF(ENTITY entContact, ENTITY entPlayer);
#endif
