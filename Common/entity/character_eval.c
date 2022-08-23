/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/components/EString.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/Supergroup.h"
#include "gameComm/VillainDef.h"
#include "entity/costume.h"
#include "gameComm/Npc.h"

#include "player/badges.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "entity/RewardToken.h"
#include "auth/authUserData.h"

#include "entity/character_eval.h"
#include "supergroup_eval.h"
#include "entity/character_level.h"
#include "entity/TaskforceParams.h"
#include <utilitieslib/language/AppLocale.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "account/AccountInventory.h"
#include "account/AccountData.h"
#include "account/AccountCatalog.h"
#include "language/commonLangUtil.h"

#if SERVER
#include "entity/character_combat.h"
#include "cmdparse/cmdserver.h"
#include "player/badges_server.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/SouvenirClue.h"
#include "container/mapgroup.h"
#include "Reward.h"
#include "storyarc/mission.h"
#include "gameSys/arenamap.h"
#include "gameSys/sgraid.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbcomm.h"
#include "script/scriptengine.h"
#elif CLIENT
#include "cmdparse/cmdgame.h"
#include "entity/costume_client.h"
#include "player/player.h"
#endif

EvalContext *s_pCharEval;

//***************************************************************************
//***************************************************************************
//
// Helpers - These take get their arguments from the function call (as
//           opposed to the EvalContext) and put their result back into the
//           EvalContext. They are meant to be used by regular EvalFuncs.
//
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * chareval_EntityFetchHelper
 *
 * e may be NULL!
 */
void chareval_EntityFetchHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    static StashTable enumFromParam = NULL;
    bool valid_param;
    const char* lookupString = NULL;

    // simple values
    // to add a new value:
    // * add the name to PARAMLIST, which will be used in eval statements
    // * add a case to switch(param) below
    // i didn't really want to do it this way myself, but since aaron started it, i thought i'd go all the way ;)
    #define PARAMLIST            \
        ELEM(InTaskforce)        \
        ELEM(level)                \
        ELEM(levelingpact)        \
        ELEM(accesslevel)        \
        ELEM(combatlevel)        \
        ELEM(securitylevel)        \
        ELEM(maxsecuritylevel)    \
        ELEM(Supergroup)        \
        ELEM(kHitPoints)        \
        ELEM(kAbsorb)            \
        ELEM(kEndurance)        \
        ELEM(ispet)                \
        ELEM(origin)            \
        ELEM(entref)            \
        ELEM(gender)            \
        ELEM(type)                \
        ELEM(typebylocation)    \
        ELEM(subtype)            \
        ELEM(praetorianprogress)\
        ELEM(alignment)            \
        ELEM(allyID)            \
        ELEM(group)                \
        ELEM(enttype)            \
        ELEM(costume)            \
        ELEM(curcostume)        \
        ELEM(SupergroupRank)    \
        ELEM(SupergroupMode)    \
        ELEM(reputation)        \
        ELEM(CanEnterHeroZone)    \
        ELEM(CanEnterVillainZone)            \
        ELEM(CanEnterPraetorianZone)        \
        ELEM(CanEnterPraetorianTutorial)    \


    // enumeration of values
    typedef enum ParamEnum
    {
        #define ELEM(X) ParamEnum_##X,
        PARAMLIST // the syntax highlighter is pretty angry about this...
        ParamEnumCount // no underscore, to avoid a name collision
        #undef ELEM
    } ParamEnum;
    ParamEnum param;

    // mapping of string to enum
    if(!enumFromParam)
    {
        enumFromParam = stashTableCreateWithStringKeys(ParamEnumCount*2, StashDefault);
        #define ELEM(X)    if(!stashAddInt(enumFromParam, #X, ParamEnum_##X, false)) \
                            FatalErrorf("duplicate param name %s", #X);
        PARAMLIST
        #undef ELEM

        // SCD 07/01/2011
        // The typical eval names of "kHitPoints%" and "kEndurance%" in the data don't work with the above macro scheme.
        // I am adding them explicitly such that those names work with shared memory and don't incur extra overhead at runtime

        // @todo
        // The programmers originally added a slower support both for those names to truncate the '%' and then do the lookup.
        // As those eval values actually are percentages the 'legacy' name is more descriptive.
        // Apparently "kEndurance%" hasn't been used but "kHitPoints%" is used in 100s of places.
        // We should see if the data ever uses the names without the "%" and remove support for them or change all
        // the data over to names without percent that are more descriptive, or change the implementation completely...

        if(!stashAddInt(enumFromParam, "kHitPoints%", ParamEnum_kHitPoints, false))
            FatalErrorf("duplicate param name %s", "kHitPoints%");
        if(!stashAddInt(enumFromParam, "kEndurance%", ParamEnum_kEndurance, false))
            FatalErrorf("duplicate param name %s", "kEndurance%");
    }

    // early out.
    // we don't check e here, because we need to ensure that we always return the correct type
    // from the rhs that we were given.
    if (!rhs)
    {
        eval_IntPush(pcontext, 0);
        return;
    }

    STATIC_INFUNC_ASSERT(sizeof(int) == sizeof(param)); // oh the things i do for typing...
    valid_param = stashFindInt(enumFromParam, rhs, (int*)&param);

    if(valid_param)
    {
        switch(param)
        {
            #define CASE(X) xcase ParamEnum_##X

            CASE(InTaskforce):        eval_IntPush(pcontext, SAFE_MEMBER2(e, pl, taskforce_mode));
            CASE(level):            eval_IntPush(pcontext, SAFE_MEMBER2(e, pchar, iLevel + 1));
            CASE(levelingpact):        eval_IntPush(pcontext, SAFE_MEMBER(e, levelingpact_id));
            CASE(accesslevel):        eval_IntPush(pcontext, SAFE_MEMBER(e, access_level));
            CASE(combatlevel):        eval_IntPush(pcontext, SAFE_MEMBER2(e, pchar, iCombatLevel + 1));
            CASE(securitylevel):    eval_IntPush(pcontext, SAFE_MEMBER2(e, pchar, iBuildLevels[e->pchar->iCurBuild] + 1));
            CASE(maxsecuritylevel):    eval_IntPush(pcontext, e ? character_PowersGetMaximumLevel(e->pchar) + 1 : 0);
            CASE(Supergroup):        eval_IntPush(pcontext, SAFE_MEMBER(e, supergroup_id));
            CASE(kHitPoints):        eval_IntPush(pcontext, e && e->pchar ? ceil(e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints * 100.0f) : 0);
            CASE(kAbsorb):            eval_IntPush(pcontext, e && e->pchar ? ceil(e->pchar->attrMax.fAbsorb / e->pchar->attrMax.fHitPoints * 100.0f) : 0);
            CASE(kEndurance):        eval_IntPush(pcontext, e && e->pchar ? ceil(e->pchar->attrCur.fEndurance / e->pchar->attrMax.fEndurance * 100.0f) : 0);
            CASE(ispet):            eval_IntPush(pcontext, e ? !!e->erOwner : 0);
            CASE(origin):            eval_StringPush(pcontext, e && e->pchar ? e->pchar->porigin->pchName : "unknown");
            CASE(entref):            eval_StringPush(pcontext, erGetRefString(e));

            CASE(gender):
                if (e)
                    lookupString = StaticDefineIntRevLookup(ParseGender, e->gender);
                eval_StringPush(pcontext, lookupString ? lookupString : "undefined");

            CASE(type):
                STATIC_INFUNC_ASSERT(kPlayerType_Count == 2)
                #if SERVER
                    if (e && e->villainDef)
                        eval_StringPush(pcontext, e->villainDef->name);
                    else
                #endif
                    {
                        if (!e)
                            eval_StringPush(pcontext, "unknown");
                        else if (ENT_IS_VILLAIN(e))
                            eval_StringPush(pcontext, "villain");
                        else
                            eval_StringPush(pcontext, "hero");
                    }
            CASE(typebylocation):
                STATIC_INFUNC_ASSERT(kPlayerType_Count == 2)
                if (!e)
                    eval_StringPush(pcontext, "unknown");
                else if (ENT_IS_ON_RED_SIDE(e))
                    eval_StringPush(pcontext, "villain");
                else
                    eval_StringPush(pcontext, "hero");

            CASE(subtype):
                STATIC_INFUNC_ASSERT(kPlayerSubType_Count == 3)
                if (e && e->pl)
                    lookupString = StaticDefineIntRevLookup(ParsePlayerSubType, e->pl->playerSubType);
                eval_StringPush(pcontext, lookupString ? lookupString : "normal");

            CASE(praetorianprogress):
                STATIC_INFUNC_ASSERT(kPraetorianProgress_Count == 7)
                if (e && e->pl)
                    lookupString = StaticDefineIntRevLookup(ParsePraetorianProgress, e->pl->praetorianProgress);
                eval_StringPush(pcontext, lookupString ? lookupString : "normal");

            CASE(alignment):
                STATIC_INFUNC_ASSERT(kPlayerType_Count == 2 && kPlayerSubType_Count == 3)
                #if SERVER
                    if (e && e->villainDef)
                        eval_StringPush(pcontext, e->villainDef->name);
                    else
                #endif
                    {
                        if (e && e->pl)
                            lookupString = StaticDefineIntRevLookup(ParseAlignment, getEntAlignment(e));
                        eval_StringPush(pcontext, lookupString ? lookupString : "hero");
                    }
            CASE(allyID):
                if (e && e->pchar)
                    lookupString = StaticDefineIntRevLookup(ParseTeamNames, e->pchar->iAllyID);
                eval_StringPush(pcontext, lookupString ? lookupString : "other");

            CASE(group):
                #if SERVER
                    if (e && e->villainDef)
                        eval_StringPush(pcontext, villainGroupGetName(e->villainDef->group));
                    else
                #endif
                        eval_StringPush(pcontext, "NoGroup");

            CASE(enttype):
                if (!e)
                    eval_StringPush(pcontext, "unknown");
                else if (ENTTYPE(e)==ENTTYPE_CRITTER)
                    eval_StringPush(pcontext, "critter");
                else
                    eval_StringPush(pcontext, "player");

            CASE(costume):
                if (!e)
                    eval_StringPush(pcontext, "unknown");
                else if (ENTTYPE(e)==ENTTYPE_CRITTER)
                {
                    #if SERVER
                        const VillainLevelDef *pDef = NULL;
                        if(e->villainDef && e->pchar)
                            pDef = GetVillainLevelDef(e->villainDef, e->pchar->iLevel + 1);
                        if(pDef && e->costumeNameIndex<eaSize(&pDef->costumes))
                            eval_StringPush(pcontext, pDef->costumes[e->costumeNameIndex]);
                        else
                    #endif
                            eval_StringPush(pcontext, "unknown");
                }
                else
                {
                    BodyType bodytype = kBodyType_Male;
                    if(e->pl && e->pl->costume[0])
                        bodytype = e->pl->costume[0]->appearance.bodytype;
                    else if(e->costume)
                        bodytype = e->costume->appearance.bodytype;
                    lookupString = StaticDefineIntRevLookup(ParseBodyType, bodytype);
                    eval_StringPush(pcontext, lookupString ? lookupString : "other");
                }

            CASE(curcostume):
                if (!e)
                    eval_StringPush(pcontext, "unknown");
                else if (ENTTYPE(e)==ENTTYPE_CRITTER)
                {
                    #if SERVER
                        const NPCDef *def = e->npcIndex ? eaGetConst(&npcDefList.npcDefs, e->npcIndex) : NULL;
                        if(def)
                            eval_StringPush(pcontext, def->name);
                        else
                    #endif
                            eval_StringPush(pcontext, "unknown");
                }
                else
                {
                    lookupString = StaticDefineIntRevLookup(ParseBodyType, e->costume ? e->costume->appearance.bodytype : kBodyType_Male);
                    eval_StringPush(pcontext, lookupString ? lookupString : "other");
                }

            CASE(SupergroupRank):
                if (e && e->supergroup)
                {
                    int i;
                    for(i = 0; i < eaSize(&e->supergroup->memberranks); i++)
                    {
                        if(e->supergroup->memberranks[i]->dbid == e->db_id)
                        {
                            eval_IntPush(pcontext, e->supergroup->memberranks[i]->rank);
                            break;
                        }
                    }
                }
                else
                {
                    eval_IntPush(pcontext, -1);
                }

            CASE(SupergroupMode):
                if (e && e->pl)
                    eval_IntPush(pcontext, e->pl->supergroup_mode);
                else
                    eval_IntPush(pcontext, 0);

            CASE(reputation):
                #ifdef SERVER
                    eval_IntPush(pcontext, (e && e->pl ? e->pl->reputationPoints : 0));
                #else
                    eval_IntPush(pcontext, 0);
                #endif

            CASE(CanEnterHeroZone):
                eval_IntPush(pcontext, e ? ENT_CAN_GO_HERO(e) : 0);

            CASE(CanEnterVillainZone):
                eval_IntPush(pcontext, e ? ENT_CAN_GO_VILLAIN(e) : 0);

            CASE(CanEnterPraetorianZone):
                eval_IntPush(pcontext, e ? ENT_CAN_GO_PRAETORIAN(e) : 0);

            CASE(CanEnterPraetorianTutorial):
                eval_IntPush(pcontext, e ? ENT_CAN_GO_PRAETORIAN_TUTORIAL(e) : 0);

            xdefault:
                estrConcatf(&pcontext->ppchError, "\nThe evaluator couldn't understand '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
                eval_IntPush(pcontext, 0);

            #undef CASE
        };
        return;
    }

    // a couple of legacy values
    if(strnicmp(rhs, "arch", 4)==0 || strnicmp(rhs, "rank", 4)==0)
    {
        eval_StringPush(pcontext, e && e->pchar && e->pchar->pclass ? e->pchar->pclass->pchName : "unknown");
        return;
    }

    // attrib lookup
    {
        extern StaticDefine ParsePowerDefines[]; // load_def.c

        const char *pch;
        const char *pchAttrib;
        
        if((pchAttrib=strchr(rhs, '.'))!=NULL)
            pchAttrib++;
        else
            pchAttrib = rhs;
        
        if((pch=StaticDefineLookup(ParsePowerDefines, pchAttrib))!=0)
        {
            float fval = 0.0f;
            
            int i = atoi(pch);
            if (i < sizeof(CharacterAttributes) && e && e->pchar)
            {
                float *pf;
                CharacterAttributes *pattr = &e->pchar->attrCur;
                
                if(pchAttrib!=rhs)
                {
                    if(strnicmp(rhs, "cur.", 4)==0 || strnicmp(rhs, "current.", 8)==0)
                    {
                        pattr = &e->pchar->attrCur;
                    }
                    else if(strnicmp(rhs, "str.", 4)==0 || strnicmp(rhs, "strength.", 9)==0)
                    {
                        pattr = &e->pchar->attrStrength;
                    }
                    else if(strnicmp(rhs, "res.", 4)==0 || strnicmp(rhs, "resistance.", 11)==0)
                    {
                        pattr = &e->pchar->attrResistance;
                    }
                    else if(strnicmp(rhs, "max.", 4)==0 || strnicmp(rhs, "maximum.", 8)==0)
                    {
                        pattr = &e->pchar->attrMax;
                    }
                    else if(strnicmp(rhs, "mod.", 4)==0 || strnicmp(rhs, "modifier.", 9)==0)
                    {
                        pattr = &e->pchar->attrMod;
                    }
                }
                
                pf = ATTRIB_GET_PTR(pattr, i);
                fval = *pf;
            }
            
            eval_FloatPush(pcontext, fval);
        }
        else
        {
            // Gets here when all else fails.
            estrConcatf(&pcontext->ppchError, "\nThe evaluator couldn't understand '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
            eval_IntPush(pcontext, 0);
        }
    }
}

/**********************************************************************func*
 * chareval_EntityHasTagHelper
 *
 */
void chareval_EntityHasTagHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
#if SERVER
    if(e && rhs && e->villainDef)
    {
        if(villainDefHasTag(e->villainDef, rhs))
        {
            eval_IntPush(pcontext, 1);
            return;
        }
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the entity tag '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
#endif
    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * EntityInModeHelper
 *
 */
void chareval_EntityInModeHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int pushMe = 0;
    extern StaticDefine ParsePowerDefines[]; // load_def.c
    if (rhs)
    {
        const char *pch = StaticDefineLookup(ParsePowerDefines, rhs);
        if (pch)
        {
            int mode = atoi(pch);
            int i;

            if (e && e->pchar)
            {
                for (i = eaiSize(&e->pchar->pPowerModes) - 1; i >= 0; i--)
                {
                    if (mode == e->pchar->pPowerModes[i])
                    {
                        pushMe = 1;
                        break;
                    }
                }
            }
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power mode but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }
    eval_IntPush(pcontext, pushMe);
}

void chareval_EntityIsToggleActiveHelper(EvalContext *pcontext, Entity *e, const char *powerFullName)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;
    int pushMe = 0;
    
    if (!powerFullName)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (!powerdict_GetBasePowerByFullName(&g_PowerDictionary, powerFullName))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found the word '%s'. It found this while analyzing '%s'.", powerFullName, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (e && e->pchar)
    {
        for (ppowListItem = powlist_GetFirst(&e->pchar->listTogglePowers, &iter);
            ppowListItem != NULL;
            ppowListItem = powlist_GetNext(&iter))
        {
            if (!stricmp(powerFullName, ppowListItem->ppow->ppowBase->pchFullName))
            {
                pushMe = 1;
                break;
            }
        }
    }

    eval_IntPush(pcontext, pushMe);
}

/**********************************************************************func*
 * chareval_EntityHasSouvenirClueHelper
 *
 */
void chareval_EntityHasSouvenirClueHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
#if SERVER
    if (rhs)
    {
        const SouvenirClue* clue = scFindByName(rhs);
        if (clue)
        {
            // Does the player already have this clue?
            if (e && e->storyInfo && eaFind(&e->storyInfo->souvenirClues, clue) >= 0)
            {
                eval_IntPush(pcontext, 1);
                return;
            }
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a souvenir clue name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the souvenir clue '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
#endif
    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
* chareval_EntityHasClueHelper
*
*/
void chareval_EntityHasClueHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
#if SERVER
    if(e && rhs)
    {
        eval_IntPush(pcontext, CluePlayerHasByName(e, rhs));
        return;
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the clue '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
#endif
    eval_IntPush(pcontext, 0);
}


/**********************************************************************func*
 * chareval_EntityEventCountHelper
 *
 */
void chareval_EntityEventCountHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    extern StaticDefineInt PowerEventEnum[];
    int ival = 0;
    const char *pch;

    if(rhs)
    {
        if ((pch = StaticDefineIntLookup(PowerEventEnum, rhs)) != 0)
        {
            if (e && e->pchar)
            {
                int i = atoi(pch);
                if (i >= 0 && i < kPowerEvent_Count)
                {
                    ival = e->pchar->aulPhaseOneEventCounts[i];
                }
            }
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power event name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }

    eval_IntPush(pcontext, ival);
}

/**********************************************************************func*
 * chareval_EntityEventTimeHelper
 *
 */
void chareval_EntityEventTimeHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    extern StaticDefineInt PowerEventEnum[];
    int ival = 0;
    const char *pch;

    if(rhs)
    {
        if ((pch = StaticDefineIntLookup(PowerEventEnum, rhs)) != 0)
        {
            if (e && e->pchar)
            {
                int i = atoi(pch);
                if (i >= 0 && i < kPowerEvent_Count)
                {
                    ival = e->pchar->aulEventTimes[i];
                }
            }
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power event name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }

    eval_IntPush(pcontext, ival);
}

/**********************************************************************func*
 * chareval_EntityTokenOwnedHelper
 *
 */
void chareval_EntityTokenOwnedHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int i = -1;

    if (e && e->pl)
    {
        i = rewardtoken_IdxFromName(&e->pl->rewardTokens, rhs);
        if (i == -1)
        {
#if SERVER
            if (e->teamup_activePlayer)
            {
                i = rewardtoken_IdxFromName(&e->teamup_activePlayer->activePlayerRewardTokens, rhs);
            }
            else
#endif
            {
                i = rewardtoken_IdxFromName(&e->pl->activePlayerRewardTokens, rhs);
            }
        }
    }
    else if (e && e->pchar && (ENTTYPE(e) == ENTTYPE_CRITTER))
    {
        i = rewardtoken_IdxFromName(&e->pchar->critterRewardTokens, rhs);
    }

    eval_IntPush(pcontext, i >= 0 ? 1 : 0);
}

/**********************************************************************func*
 * chareval_EntityTokenValHelper
 *
 */
void chareval_EntityTokenValHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int i = -1;
    int val = 0;

    if (e && e->pl)
    {
        i = rewardtoken_IdxFromName(&e->pl->rewardTokens, rhs);
        if (i == -1)
        {
#if SERVER
            if (e->teamup_activePlayer)
            {
            i = rewardtoken_IdxFromName(&e->teamup_activePlayer->activePlayerRewardTokens, rhs);

                if (i >= 0)
                {
                    val = e->teamup_activePlayer->activePlayerRewardTokens[i]->val;
                }
            }
            else
#endif
            {
                i = rewardtoken_IdxFromName(&e->pl->activePlayerRewardTokens, rhs);

                if (i >= 0)
                {
                    val = e->pl->activePlayerRewardTokens[i]->val;
                }
            }    
        }
        else
        {
            val = e->pl->rewardTokens[i]->val;
        }
    }
    else if (e && e->pchar && (ENTTYPE(e) == ENTTYPE_CRITTER))
    {
        i = rewardtoken_IdxFromName(&e->pchar->critterRewardTokens, rhs);
        if(i >= 0)
            val = e->pchar->critterRewardTokens[i]->val;
    }

    eval_IntPush(pcontext, val);
}

/**********************************************************************func*
 * chareval_EntityTokenTimeHelper
 *
 */
void chareval_EntityTokenTimeHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int i = -1;
    U32 ret = 0;

    if (e && e->pl)
    {
        i = rewardtoken_IdxFromName(&e->pl->rewardTokens, rhs);
        if (i == -1)
        {
#if SERVER
            if (e->teamup_activePlayer)
            {
                i = rewardtoken_IdxFromName(&e->teamup_activePlayer->activePlayerRewardTokens, rhs);

                if (i >= 0)
                {
                    ret = e->teamup_activePlayer->activePlayerRewardTokens[i]->timer;
                }
            }
            else
#endif
            {
                i = rewardtoken_IdxFromName(&e->pl->activePlayerRewardTokens, rhs);

                if (i >= 0)
                {
                    ret = e->pl->activePlayerRewardTokens[i]->timer;
                }
            }
        }
        else
        {
            ret = e->pl->rewardTokens[i]->timer;
        }
    }
    else if (e && e->pchar && (ENTTYPE(e) == ENTTYPE_CRITTER))
    {
        i = rewardtoken_IdxFromName(&e->pchar->critterRewardTokens, rhs);
        if(i >= 0)
            ret = e->pchar->critterRewardTokens[i]->timer;
    }

    eval_IntPush(pcontext, ret);
}


//***************************************************************************
//***************************************************************************
//
// EvalFuncs - These take their arguments from the EvalContext and put their
//             results back into the EvalContext. Any or all of these could
//             be broken up into EvalFuncs and Helpers (above) if the need
//             arises. Helpers have only been made for things which actually
//             needed to be helpers.
//
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * chareval_Now
 *
 */
void chareval_Now(EvalContext *pcontext)
{
    eval_IntPush(pcontext, timerServerTime());
}

/**********************************************************************func*
 * chareval_MapTokenValHelper
 *
 */
void chareval_MapTokenValHelper(EvalContext *pcontext, const char *rhs)
{
#if SERVER
    int i = -1;
    MapDataToken *pTok;

    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = getMapDataToken(rhs);

    if (pTok != NULL)
        eval_IntPush(pcontext, pTok->val);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the map token '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif

}

/**********************************************************************func*
* chareval_InstanceTokenValHelper
*
*/
void chareval_InstanceTokenValHelper(EvalContext *pcontext, const char *rhs)
{
#if SERVER
    int i = -1;
    const char *pTok;

    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = GetInstanceVar(rhs);

    if (pTok != NULL)
        eval_StringPush(pcontext, pTok);
    else
        eval_StringPush(pcontext, "UNKNOWN");
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the instance token '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_StringPush(pcontext, "UNKNOWN");
#endif

}


/**********************************************************************func*
* chareval_TeamSizeHelper
*
*/
void chareval_TeamSizeHelper(EvalContext *pcontext, Entity *e, float fRadius)
{
#if SERVER
    if (e && ENTTYPE(e) == ENTTYPE_PLAYER && e->teamup)
    {
        if (fRadius > 0.0f)
        {
            int i, count = e->teamup->members.count;
            int close = 0;
            for (i = 0; i < count; i++)
            {
                Entity* teamMember = entFromDbId(e->teamup->members.ids[i]);

                if (teamMember)
                {
                    if (distance3( ENTPOS(e), ENTPOS(teamMember) ) <= fRadius)
                        close++;
                }
            }
            eval_IntPush(pcontext, close);        
        } else {
            eval_IntPush(pcontext, e->teamup->members.count);        
        }
    } else {
        eval_IntPush(pcontext, 1);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for the number of players within a %f radius on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", fRadius, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 1);
#endif
}
/**********************************************************************func*
 * chareval_SystemFetch
 *
 */
void chareval_SystemFetch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);

    if(rhs)
    {
        if(stricmp(rhs, "betabase")==0)
        {
#if SERVER
            eval_IntPush(pcontext, server_state.beta_base);
#elif CLIENT
            eval_IntPush(pcontext, game_state.beta_base);
#else
            eval_IntPush(pcontext, 0);
#endif
        }
        else if(stricmp(rhs, "enableinvention")==0)
        {
            eval_IntPush(pcontext, 1);
        }
        else if(stricmp(rhs, "locale")==0)
        {
            int loc = getCurrentLocale();
            static char *apchLocales[] =  
            {
                "LOCALE_ID_ENGLISH",
                "LOCALE_ID_TEST",
                "LOCALE_ID_CHINESE_TRADITIONAL",
                "LOCALE_ID_KOREAN",
                "LOCALE_ID_GERMAN",
                "LOCALE_ID_FRENCH",
                "LOCALE_ID_SPANISH",
            };
            if(loc<ARRAY_SIZE(apchLocales))
            {
                eval_StringPush(pcontext, apchLocales[loc]);
            }
            else
            {
                eval_StringPush(pcontext, "*unknown*");
            }
        }
        else if(stricmp(rhs, "ShardEvent.Name")==0)
        {
#if SERVER
            if(g_mapgroup.activeEvent[0])
            {
                eval_StringPush(pcontext, g_mapgroup.activeEvent);
            }
            else
            {
                eval_StringPush(pcontext, "UNKNOWN");
            }
#else
            {
                //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for a shard event name on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
                eval_StringPush(pcontext, "UNKNOWN");
            }
#endif
        }
        else if(stricmp(rhs, "ShardEvent.Signal")==0)
        {
#if SERVER
            if(g_mapgroup.eventSignal[0])
            {
                eval_StringPush(pcontext, g_mapgroup.eventSignal);
            }
            else
            {
                eval_StringPush(pcontext, "UNKNOWN");
            }
#else
            {
                //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for a shard event signal on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
                eval_StringPush(pcontext, "UNKNOWN");
            }
#endif
        }
        else if(stricmp(rhs, "ShardEvent.SignalTime")==0)
        {
#if SERVER
            if(g_mapgroup.eventSignal[0])
            {
                eval_IntPush(pcontext, g_mapgroup.eventSignalTS);
            }
            else
            {
                eval_IntPush(pcontext, 0);
            }
#else
            {
                //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to look for a shard event signal time on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
                eval_IntPush(pcontext, 0);
            }
#endif
        }
        else if(stricmp(rhs, "MapTokenVal")==0)
        {
            const char *rhs2 = eval_StringPop(pcontext);
            chareval_MapTokenValHelper(pcontext, rhs2);
        }
        else if(stricmp(rhs, "InstanceTokenVal")==0)
        {
            const char *rhs2 = eval_StringPop(pcontext);
            chareval_InstanceTokenValHelper(pcontext, rhs2);
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a valid parameter but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
            eval_IntPush(pcontext, 0);
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a parameter but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* chareval_CanBuyPowerWithoutOverflow
*
*/
void chareval_CanBuyPowerWithoutOverflow(EvalContext *pcontext)
{
    Entity *e = NULL;
    bool pushMe = false;
    const char *rhs = eval_StringPop(pcontext);
    const BasePower *ppowBase = powerdict_GetBasePowerByFullNameEx(&g_PowerDictionary, rhs, false);

    if (ppowBase)
    {
        bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);
        if (bFound && ppowBase && e && e->pchar)
        {
            pushMe = character_CanBuyPowerWithoutOverflow(e->pchar, ppowBase);
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    eval_BoolPush(pcontext, pushMe);
}

/**********************************************************************func*
 * chareval_OwnsPower
 *
 */
void chareval_OwnsPower(EvalContext *pcontext, const char *entString)
{
    bool pushMe = false;
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    PowerCategory *pCat;
    BasePowerSet *psetBase = NULL;
    BasePower *ppowBase = NULL;
    if (powerdict_GetBasePtrsByStringEx(&g_PowerDictionary, rhs,
        &pCat, &psetBase, &ppowBase, NULL, false))
    {
        bool bFound = eval_FetchInt(pcontext, entString, (int *)&e);
        if (bFound && e && e->pchar)
        {
            if (ppowBase != NULL)
            {
                pushMe = (character_OwnsPower(e->pchar, ppowBase) != NULL);
            }
            else if (psetBase != NULL)
            {
                pushMe = (character_OwnsPowerSet(e->pchar, psetBase) != NULL);
            }
        }
    }
    else
    {
        // This message is generic because typos will get caught here.  The character evaluator defaults to this function.
        estrConcatf(&pcontext->ppchError, "\nThe evaluator couldn't understand '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    eval_BoolPush(pcontext, pushMe);
}
/**********************************************************************func*
 * chareval_EntityOwnsPower
 *
 */
void chareval_EntityOwnsPower(EvalContext *pcontext)
{
    chareval_OwnsPower(pcontext, "Entity");
}

/**********************************************************************func*
 * chareval_OwnsPowerNum
 *
 */
void chareval_OwnsPowerNum(EvalContext *pcontext, const char *entString)
{
    int pushMe = 0;
    const char *rhs = eval_StringPop(pcontext);
    const BasePower *ppowBase = powerdict_GetBasePowerByFullNameEx(&g_PowerDictionary, rhs, false);

    if (ppowBase)
    {
        Entity *e = NULL;
        bool bFound = eval_FetchInt(pcontext, entString, (int *)&e);
        if (bFound && e && e->pchar)
            pushMe = character_OwnsPowerNum(e->pchar, ppowBase);
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    eval_IntPush(pcontext, pushMe);
}

/**********************************************************************func*
 * chareval_EntityOwnsPowerNum
 *
 */
void chareval_EntityOwnsPowerNum(EvalContext *pcontext)
{
    chareval_OwnsPowerNum(pcontext, "Entity");
}

/**********************************************************************func*
* chareval_EntityAllowedToActivatePower
*
* ONLY WORKS ON THE SERVER!
* NOTE: It's potentially possible to get this to return true on auto powers that have already expired
*       due to fLifetime (RL time, not in-game time) before the first call to ExpirePowers_Tick.
*/
void chareval_EntityAllowedToActivatePower(EvalContext *pcontext)
{
    bool pushMe = false;
    const char *rhs = eval_StringPop(pcontext);
#if SERVER
    Entity *e = NULL;
    const BasePower *ppowBase = powerdict_GetBasePowerByFullNameEx(&g_PowerDictionary, rhs, false);

    if (ppowBase)
    {
        bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

        if(bFound && e && e->pchar)
        {
            Power *power = character_OwnsPower(e->pchar, ppowBase);
            if (power != NULL)
            {
                pushMe = character_AllowedToActivatePower(e->pchar, power, false);
            }
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if an entity was allowed to activate the power '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
#endif //SERVER

    eval_BoolPush(pcontext, pushMe);
}

/**********************************************************************func*
* chareval_EntityFetch
*
*/
void chareval_EntityFetch(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * chareval_EntitysOwnerFetch
 *
 */
void chareval_EntitysOwnerFetch(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (e && e->erOwner)
    {
        chareval_EntityFetchHelper(pcontext, erGetEnt(e->erOwner), rhs);
    }
    else
    {
        // entity is its own owner
        chareval_EntityFetchHelper(pcontext, e, rhs);
    }
}

/**********************************************************************func*
 * chareval_EntityHasTag
 *
 */
void chareval_EntityHasTag(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    chareval_EntityHasTagHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * chareval_EntityInMode
 *
 */
void chareval_EntityInMode(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    chareval_EntityInModeHelper(pcontext, e, rhs);
}

void chareval_EntityHasPowerset(EvalContext *pcontext)
{
    int pushMe = 0;
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);
    const BasePowerSet *psetBase;

    if (!(psetBase = powerdict_GetBasePowerSetByFullNameEx(&g_PowerDictionary, rhs, false)))
    {
        assert(0); // hack remove me!
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power set full name but found the word '%s'. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (bFound && e && e->pchar)
    {
        pushMe = (character_OwnsPowerSet(e->pchar, psetBase) != NULL);
    }

    eval_IntPush(pcontext, pushMe);
}

void chareval_EntityHasEnabledPower(EvalContext *pcontext)
{
    int pushMe = 0;
    const char *powerFullName = eval_StringPop(pcontext);
    const BasePower *ppowBase;
    Entity *e = NULL;
    bool bFound;
    Power *ppow;

    ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, powerFullName);

    if (ppowBase)
    {
        bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);
        if (bFound)
        {
            ppow = character_OwnsPower(e->pchar, ppowBase);
            if (ppow)
                pushMe = !!ppow->bEnabled;
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a power full name but found the word '%s'. It found this while analyzing '%s'.", powerFullName, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    eval_IntPush(pcontext, pushMe);
}

void chareval_EntityIsToggleActive(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *powerFullName = eval_StringPop(pcontext);

    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityIsToggleActiveHelper(pcontext, e, powerFullName);
}

/**********************************************************************func*
 * chareval_EntityHasSouvenirClue
 *
 */
void chareval_EntityHasSouvenirClue(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);

    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityHasSouvenirClueHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* chareval_EntityHasClue
*
*/
void chareval_EntityHasClue(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);

    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityHasClueHelper(pcontext, e, rhs);
}


/**********************************************************************func*
 * chareval_EntityEventCount
 *
 */
void chareval_EntityEventCount(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityEventCountHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * chareval_EntityEventTime
 *
 */
void chareval_EntityEventTime(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityEventTimeHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * chareval_EntityTokenOwned
 *
 */
void chareval_EntityTokenOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityTokenOwnedHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* chareval_EntityArchitectTokenOwned
*
*/
void  chareval_EntityArchitectTokenOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    eval_IntPush(pcontext, playerCreatedStoryArc_CheckUnlockKey(e, rhs));
}

/**********************************************************************func*
* chareval_EntityTokenOwned
*
*/
void chareval_EntityCostumeKeyOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound && e->pchar)
    {
        int i = -1;
#if SERVER
        if (e->pl)
        {
            i = rewardtoken_IdxFromName(&e->pl->rewardTokens, rhs);
        }
        eval_IntPush(pcontext, i>=0 ? 1:0);
#else
        eval_IntPush(pcontext, costumereward_find( rhs, 0 ));
#endif
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* chareval_EntityOnTaskForce
*
*/
void chareval_EntityOnTaskForce(EvalContext *pcontext)
{
#ifdef SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound)
    {
        eval_IntPush(pcontext, PlayerInTaskForceMode(e));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to check if the entity was on a task force on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_EntityOnFlashback
*
*/
void chareval_EntityOnFlashback(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityTokenOwnedHelper(pcontext, e, "OnFlashback");
}

/**********************************************************************func*
* chareval_EntityOnArchitect
*
*/
void chareval_EntityOnArchitect(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityTokenOwnedHelper(pcontext, e, "OnArchitect");
}

/**********************************************************************func*
 * chareval_EntityTokenVal
 *
 */
void chareval_EntityTokenVal(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityTokenValHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * chareval_EntityTokenTime
 *
 */
void chareval_EntityTokenTime(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityTokenTimeHelper(pcontext, e, rhs);
}


/**********************************************************************func*
* chareval_EntityTeamSize
*
*/
void chareval_EntityTeamSize(EvalContext *pcontext)
{
    float fRadius = eval_FloatPop(pcontext);
#if SERVER
    Entity *e = NULL;
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_TeamSizeHelper(pcontext, e, fRadius);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of teammates for an entity within %f distance on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", fRadius, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_MissionObjective
*                Only valid server side
*/
void chareval_MissionObjective(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    Entity *e = NULL;
#if SERVER
    if (OnMissionMap())
    {
        eval_IntPush(pcontext, MissionObjectiveIsComplete(rhs, 1));
    }
    else
    {
        eval_FetchInt(pcontext, "Entity", (int *)&e);
        // TODO: Is there a way to do this validation at load-time?
        if (e) // don't error here during a load-time check, since we do that on static maps
            estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the mission objective '%s' has been completed on a static map server, but this operation is only supported on mission map servers. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the mission objective '%s' has been completed on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_TaskOwner
*                Only valid server side
*                Only valid if task is loaded before evaluation or on mission map
*/
void chareval_TaskOwner(EvalContext *pcontext)
{
#if SERVER
    Entity *e = NULL;
    StoryTaskInfo *pTask; 
    bool bTaskFound = eval_FetchInt(pcontext, "Task", (int *)&pTask);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (bTaskFound && pTask)
    {
        eval_IntPush(pcontext, (e ? pTask->assignedDbId == e->db_id : 0));
    } 
    else if (OnMissionMap())    // might be on a mission map?
    {
        eval_IntPush(pcontext, (e ? g_activemission->ownerDbid == e->db_id : 0));
    }
    else
    {
        // TODO: Is there a way to do this validation at load-time?
        if (e) // don't error here during a load-time check, since we do that on static maps
            estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the entity owns the current task on a server, but this operation is only supported on a mission map server or in a task evaluator statement. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the entity owns the current task on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_MissionStarted
*                Only valid server side
*                Only valid if task is loaded before evaluation
*/
void chareval_MissionStarted(EvalContext *pcontext)
{
#if SERVER
    Entity *e = NULL;
    StoryTaskInfo *pTask;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);
    bool bTaskFound = eval_FetchInt(pcontext, "Task", (int *)&pTask);

    if (!bTaskFound)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current mission has been started, but this operation is only supported in task evaluator statements. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (bFound && e && e->teamup)
    {
        StoryTaskInfo *pActive = e->teamup->activetask;
        eval_IntPush(pcontext, (e->teamup->map_id != 0 && pActive->assignedDbId == pTask->assignedDbId && isSameStoryTaskPos( &pActive->sahandle, &pTask->sahandle ))); 
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current mission has been started on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif

}

void chareval_IsOnStoryArc(EvalContext *pcontext)
{
    const char *storyArcName = eval_StringPop(pcontext);
#if SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound)
    {
        eval_IntPush(pcontext, entity_IsOnStoryArc(e, storyArcName));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity is on the story arc named '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", storyArcName, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

void chareval_EntityHasAnyActiveMission(EvalContext *pcontext)
{
#if SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound)
    {    
        Teamup* team = e->teamup;
        if (team && team->activetask && team->activetask->state != TASK_NONE)
        {
            eval_IntPush(pcontext, 1);
        }
        else
        {
            eval_IntPush(pcontext, 0);
        }
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity has any active mission on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsPvPMap
*                Only valid server side
*/
void chareval_IsPvPMap(EvalContext *pcontext)
{
#if SERVER

    if (server_visible_state.isPvP || OnArenaMap() || RaidIsRunning())
    {
        eval_IntPush(pcontext, 1);
    } else {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a PvP map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsMissionMap
*                Only valid server side
*/
void chareval_IsMissionMap(EvalContext *pcontext)
{
#if SERVER

    if (OnMissionMap())
    {
        eval_IntPush(pcontext, 1);
    } else {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a mission map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsArchitectMap
*                Only valid server side
*/
void chareval_IsArchitectMap(EvalContext *pcontext)
{
#if SERVER
    if (g_ArchitectTaskForce)
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#elif CLIENT
    if( game_state.mission_map && playerPtr() && playerPtr()->onArchitect )
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is an architect map on an executable that doesn't support the operation. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif 
}

/**********************************************************************func*
* chareval_IsHeroOnlyMap
*                Only valid server side
*/
void chareval_IsHeroOnlyMap(EvalContext *pcontext)
{
#if SERVER
    const MapSpec *temp;
    if ((temp = MapSpecFromMapName(db_state.map_name)) && MAP_ALLOWS_HEROES_ONLY(temp))
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a hero-only map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsVillainOnlyMap
*                Only valid server side
*/
void chareval_IsVillainOnlyMap(EvalContext *pcontext)
{
#if SERVER
    const MapSpec *temp;
    if ((temp = MapSpecFromMapName(db_state.map_name)) && MAP_ALLOWS_VILLAINS_ONLY(temp))
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a villain-only map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsHeroAndVillainMap
*                Only valid server side
*/
void chareval_IsHeroAndVillainMap(EvalContext *pcontext)
{
#if SERVER
    const MapSpec *temp;
    if ((temp = MapSpecFromMapName(db_state.map_name)) && MAP_ALLOWS_HEROES(temp) && MAP_ALLOWS_VILLAINS(temp))
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map allows both heroes and villains on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsHazardMap
*                Only valid server side
*/
void chareval_IsHazardMap(EvalContext *pcontext)
{
#if SERVER
    if (strstr(db_state.map_name, "Hazard"))
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a hazard map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_IsTrialMap
*                Only valid server side
*/
void chareval_IsTrialMap(EvalContext *pcontext)
{
#if SERVER
    if (strstr(db_state.map_name, "Trial"))
        eval_IntPush(pcontext, 1);
    else
        eval_IntPush(pcontext, 0);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current map is a trial map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_MapTeamArea
*
* This returns the team area of the last static map that the player was in.
* It defaults to this map's team area if it can't get the player.
*                Only valid server side
*/
void chareval_MapTeamAreaHelper(EvalContext *pcontext, Entity *e)
{
#if SERVER
    const MapSpec *temp;
    int id = e ? e->static_map_id : 0;

    if (id && (temp = MapSpecFromMapId(id)))
    {
        switch (temp->teamArea)
        {
        xcase MAP_TEAM_HEROES_ONLY:
            eval_StringPush(pcontext, "HeroesOnly");
        xcase MAP_TEAM_VILLAINS_ONLY:
            eval_StringPush(pcontext, "VillainsOnly");
        xcase MAP_TEAM_PRIMAL_COMMON:
            eval_StringPush(pcontext, "PrimalCommon");
        xcase MAP_TEAM_PRAETORIANS:
            eval_StringPush(pcontext, "Praetoria");
        xcase MAP_TEAM_EVERYBODY:
            eval_StringPush(pcontext, "Everybody");
        xdefault:
            eval_StringPush(pcontext, "Unknown");
        }
    }
    else
    {
        eval_StringPush(pcontext, "Unknown");
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the MapTeamArea of the current map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_StringPush(pcontext, "Unknown");
#endif
}

void chareval_MapTeamArea(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_MapTeamAreaHelper(pcontext, e);
}

static char s_chareval_mapName[256];

void chareval_MapName(EvalContext *pcontext)
{
#if SERVER
    char *shortpath;
    char *dot;
    
    strncpy(s_chareval_mapName, db_state.map_name, 256);
    shortpath = strrchr(s_chareval_mapName, '/');

    if (shortpath)
    {
        shortpath[0] = 0;
        shortpath++;
    }
    else if (shortpath = strrchr(s_chareval_mapName, '\\'))
    {
        shortpath[0] = 0;
        shortpath++;
    }
    else
    {
        shortpath = s_chareval_mapName;
    }

    dot = strchr(shortpath, '.');
    if (dot)
    {
        dot[0] = 0;
    }

    eval_StringPush(pcontext, shortpath);
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to retrieve the name of the current map on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_StringPush(pcontext, "Unknown");
#endif
}

/**********************************************************************func*
 * chareval_EntityBadgeStatFetch
 *
 */
void chareval_EntityBadgeStatFetch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
#if SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound && e && e->pl)
    {
        eval_IntPush(pcontext, badge_StatGet(e, rhs));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to retrieve the badge stat '%s' from the current entity on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_EntityOwnsBadgeHelper
*
*/
void chareval_EntityOwnsBadgeHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int i = 0;
    if (rhs)
    {
        int iIdx;
        if (badge_GetIdxFromName(rhs, &iIdx) && iIdx <= g_BadgeDefs.idxMax)
        {
            if (e && e->pl)
            {
#if CLIENT
                // This may actually work on the server too, but it's only
                //   updated periodically.
                int iBadge = e->pl->aiBadges[iIdx];
                i = BADGE_IS_OWNED(iBadge);
#else
                i = BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, iIdx);
#endif
            }
        }
        else
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a badge named '%s', but that name didn't correspond to a badge. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a badge, but didn't receive a badge name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    eval_IntPush(pcontext, i);
}


/**********************************************************************func*
 * chareval_EntityOwnsBadge
 *
 */
void chareval_EntityOwnsBadge(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    chareval_EntityOwnsBadgeHelper(pcontext, e, rhs);
}


/**********************************************************************func*
* chareval_EntityLoyaltyOwnedHelper
*
*/
void chareval_EntityLoyaltyOwnedHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int retval = 0;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a loyalty item, but didn't receive a name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (!accountLoyaltyRewardFindNode(rhs))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns the loyalty item '%s', but that name doesn't correspond to a loyalty item. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (e && e->pl && accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, rhs))
    {
        retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_LoyaltyOwned
*
*/
void chareval_LoyaltyOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);

    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityLoyaltyOwnedHelper(pcontext, e, rhs);
}


/**********************************************************************func*
* chareval_EntityLoyaltyTierOwnedHelper
*
*/
void chareval_EntityLoyaltyTierOwnedHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int retval = 0;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a loyalty tier, but didn't receive a name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (!accountLoyaltyRewardFindTier(rhs))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns the loyalty tier '%s', but that name doesn't correspond to a loyalty tier. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (e && e->pl && accountLoyaltyRewardMasteredTier(e->pl->loyaltyStatus, rhs))
    {
        retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_LoyaltyOwned
*
*/
void chareval_LoyaltyTierOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityLoyaltyTierOwnedHelper(pcontext, e, rhs);
}



/**********************************************************************func*
* chareval_EntityLoyaltyLevelOwnedHelper
*
*/
void chareval_EntityLoyaltyLevelOwnedHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int retval = 0;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a loyalty level, but didn't receive a name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (!accountLoyaltyRewardFindLevel(rhs))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns the loyalty level '%s', but that name doesn't correspond to a loyalty level. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (e && e->pl && accountLoyaltyRewardHasLevel(e->pl->loyaltyPointsEarned, rhs))
    {
        retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_LoyaltyLevelOwned
*
*/
void chareval_LoyaltyLevelOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityLoyaltyLevelOwnedHelper(pcontext, e, rhs);
}


/**********************************************************************func*
* chareval_EntityLoyaltyNodesBoughtHelper
*
*/
void chareval_EntityLoyaltyNodesBoughtHelper(EvalContext *pcontext, Entity *e)
{
    int retval = 0;
    if(e && e->pl)
    {
        retval = accountLoyaltyRewardGetNodesBought(e->pl->loyaltyStatus);
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_LoyaltyNodesBought
*
*/
void chareval_LoyaltyNodesBought(EvalContext *pcontext)
{
    Entity *e = NULL;

    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityLoyaltyNodesBoughtHelper(pcontext, e);
}

/**********************************************************************func*
* chareval_EntityLoyaltyPointsEarnedHelper
*
*/
void chareval_EntityLoyaltyPointsEarnedHelper(EvalContext *pcontext, Entity *e)
{
    int retval = 0;
    if(e && e->pl)
    {
        retval = e->pl->loyaltyPointsEarned;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_LoyaltyPointsEarned
*
*/
void chareval_LoyaltyPointsEarned(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityLoyaltyPointsEarnedHelper(pcontext, e);
}

/**********************************************************************func*
* chareval_EntityProductOwnedHelper
*
*/
void chareval_EntityProductOwnedHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int retval = 0;
    SkuId sku_id;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a product, but didn't receive a SKU. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if ((sku_id = skuIdFromString(rhs)).u64 == kSkuIdInvalid.u64)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns the product '%s', but that SKU doesn't correspond to a product. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (e && AccountHasStoreProduct(ent_GetProductInventory(e), sku_id))
    {
        retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_ProductOwned
*
*/
void chareval_ProductOwned(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityProductOwnedHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* chareval_EntityProductAvailableHelper
*
*/
void chareval_EntityProductAvailableHelper(EvalContext *pcontext, Entity *e, const char *rhs)
{
    int retval = 0;
    SkuId sku_id;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns a product, but didn't receive a SKU. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if ((sku_id = skuIdFromString(rhs)).u64 == kSkuIdInvalid.u64)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine whether the current entity owns the product '%s', but that SKU doesn't correspond to a product. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (accountCatalogIsSkuPublished(sku_id))
    {
        retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_ProductAvailable
*
*/
void chareval_ProductAvailable(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    chareval_EntityProductAvailableHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* chareval_EntityIsVIPHelper
*
*/
void chareval_EntityIsVIPHelper(EvalContext *pcontext, Entity *e)
{
    int retval = 0;
    if (e && e->pl)
    {
        if (AccountIsVIP(ent_GetProductInventory(e), e->pl->account_inventory.accountStatusFlags))
            retval = 1;
    }

    eval_IntPush(pcontext, retval);
}


/**********************************************************************func*
* chareval_IsVIP
*
*/
void chareval_IsVIP(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityIsVIPHelper(pcontext, e);
}

/**********************************************************************func*
* chareval_EntityIsAccountServerAvailableHelper
*
*/
void chareval_EntityIsAccountServerAvailableHelper(EvalContext *pcontext, Entity *e)
{
    int retval = 0;

    if (e && e->pl && e->pl->accountInformationAuthoritative)
        retval = 1;

    eval_IntPush(pcontext, retval);
}

/**********************************************************************func*
* chareval_IsAccountServerAvailable
*
*/
void chareval_IsAccountServerAvailable(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_EntityIsAccountServerAvailableHelper(pcontext, e);
}

/**********************************************************************func*
* chareval_IsAccountInventoryLoadedHelper
*
*/
void chareval_IsAccountInventoryLoadedHelper(EvalContext *pcontext, Entity *e)
{
    int retval = 0;

    if (e && e->pl && e->pl->account_inv_loaded)
        retval = 1;

    eval_IntPush(pcontext, retval);
}

/**********************************************************************func*
* chareval_IsAccountInventoryLoaded
*
*/
void chareval_IsAccountInventoryLoaded(EvalContext *pcontext)
{
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_IsAccountInventoryLoadedHelper(pcontext, e);
}

/**********************************************************************func*
 * chareval_EntityCountBadges
 *
 */
void chareval_EntityCountBadges(EvalContext *pcontext)
{
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (bFound && e && e->pl)
    {
        int i;
        int iCnt = 0;
        int iSize = eaSize(&g_BadgeDefs.ppBadges);
        for (i = 0; i < iSize; i++)
        {
            if (g_BadgeDefs.ppBadges[i]->eCategory != kBadgeType_Internal
                && !g_BadgeDefs.ppBadges[i]->bDoNotCount
                && g_BadgeDefs.ppBadges[i]->iIdx <= g_BadgeDefs.idxMax)
            {
#if CLIENT
                int iBadge = e->pl->aiBadges[g_BadgeDefs.ppBadges[i]->iIdx];
                if (BADGE_IS_OWNED(iBadge))
#else
                if (BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, g_BadgeDefs.ppBadges[i]->iIdx))
#endif
                {
                    iCnt++;
                }
            }
        }
        eval_IntPush(pcontext, iCnt);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

void chareval_EntityEmptySlots(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of empty slots in a category, but didn't receive a category name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    if (!stricmp(rhs, "boost"))
    {
        eval_IntPush(pcontext, e && e->pchar ? character_BoostInventoryEmptySlots(e->pchar) : 0);
    }
    else if (!stricmp(rhs, "inspiration"))
    {
        eval_IntPush(pcontext, e && e->pchar ? character_InspirationInventoryEmptySlots(e->pchar) : 0);
    }
    else if (!stricmp(rhs, "recipe"))
    {
        eval_IntPush(pcontext, e && e->pchar ? character_InventoryEmptySlots(e->pchar, kInventoryType_Recipe) : 0);
    }
    else if (!stricmp(rhs, "salvage"))
    {
        eval_IntPush(pcontext, e && e->pchar ? character_InventoryEmptySlots(e->pchar, kInventoryType_Salvage) : 0);
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of empty slots in the category '%s', but that name doesn't correspond to a category. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* chareval_EntityBoostsSlotted
*
*/
void chareval_EntityBoostsSlotted(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);
    int i, j, k;
    int count = 0;
    int numPowerSets, numPowers, numBoosts;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of enhancements slotted, but didn't receive a enhancement name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (bFound && e && e->pchar)
    {
        numPowerSets = eaSize(&e->pchar->ppPowerSets);
        for (i = 0; i < numPowerSets; i++)
        {
            PowerSet *pPowerSet = e->pchar->ppPowerSets[i];

            if (!pPowerSet)
                continue;

            numPowers = eaSize(&pPowerSet->ppPowers);
            for (j = 0; j < numPowers; j++)
            {
                Power *pPower = pPowerSet->ppPowers[j];

                if (!pPower)
                    continue;

                numBoosts = eaSize(&pPower->ppBoosts);
                for (k = 0; k < numBoosts; k++)
                {
                    Boost *pBoost = pPower->ppBoosts[k];

                    if (!pBoost)
                        continue;

                    if (!stricmp(rhs, pBoost->ppowBase->pchName))
                        count++;
                }
            }
        }
        eval_IntPush(pcontext, count);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


void chareval_CertificationsPurchased(EvalContext *pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of certifications bought, but didn't receive a certification name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (!accountCatalogGetProductByRecipe(rhs))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of '%s' certifications bought, but that name doesn't correspond to a certification. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (bFound && e && e->pchar && e->pl && e->pl->account_inv_loaded)
    {
        eval_IntPush(pcontext, AccountInventoryGetCertBought(e, rhs));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

void chareval_VouchersUnclaimed(EvalContext * pcontext)
{
    Entity *e = NULL;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of vouchers unclaimed, but didn't receive a voucher name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (!accountCatalogGetProductByRecipe(rhs))
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine the number of '%s' vouchers unclaimed, but that name doesn't correspond to a voucher. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (bFound && e && e->pchar && e->pl && e->pl->account_inv_loaded)
    {
        eval_IntPush(pcontext, AccountInventoryGetVoucherAvailible(e, rhs));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

void chareval_EntityHasContact(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
#if SERVER
    Entity *e = NULL;
    ContactHandle contact = ContactGetHandleLoose(rhs);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (!contact)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity has the contact '%s', but that name doesn't correspond to a contact. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
    else if (bFound && e)
    {
        StoryInfo* info = e->storyInfo;
        eval_IntPush(pcontext, ContactGetInfo(info, contact)?1:0);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    {
        //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity has the contact '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        eval_IntPush(pcontext, 0);
    }
#endif
}

void chareval_EntityIsInVolume(EvalContext *pcontext, const char *ent)
{
    const char *volumeName = eval_StringPop(pcontext);
    int pushMe = 0;
#if SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, ent, (int *)&e);
    
    if (bFound && e)
    {
        VolumeList *volumeList = &e->volumeList;
        
        int i;

        for (i = 0; i < volumeList->volumeCount; i++)
        {
            DefTracker* volume = volumeList->volumes[i].volumePropertiesTracker;
            PropertyEnt* prop = (volume && volume->def) ? stashFindPointerReturnPointer(volume->def->properties, "NamedVolume") : 0;
            if (prop && !stricmp(prop->value_str, volumeName))
            {
                pushMe = 1;
                break;
            }
        }
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity is in the volume '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", volumeName, pcontext->ppchExpr[pcontext->iCurToken - 1]);
#endif
    eval_IntPush(pcontext, pushMe);
}

void chareval_SelfIsInVolume(EvalContext *pcontext)
{
    chareval_EntityIsInVolume(pcontext, "Entity");
}

void chareval_IsBetaShard(EvalContext *pcontext)
{
    eval_IntPush(pcontext, cfg_IsBetaShard() ? 1 : 0);
}

void chareval_IsReactivationActive(EvalContext *pcontext)
{
    eval_IntPush(pcontext, authUserIsReactivationActive());
}

/**********************************************************************func*
* chareval_EntityCanGiveRespec
*
*/
void chareval_EntityCanGiveRespec(EvalContext *pcontext)
{
#if SERVER
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (bFound && e)
    {
        eval_IntPush(pcontext, playerCanGiveRespec(e));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
#else
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current entity can give a respec on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

/**********************************************************************func*
* chareval_EntityHasFreespec
*
*/
void chareval_EntityHasFreespec(EvalContext *pcontext)
{
    Entity *e = NULL;
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if (bFound && e && e->pl)
    {
        eval_IntPush(pcontext, e->pl->respecTokens&(1<<6));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * chareval_AuthFetch
 *
 */
void chareval_AuthFetchHelper(EvalContext *pcontext, U32 *auth_user_data, const char *rhs)
{
    int i = 0;

    if (!rhs)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current player has an auth bit, but didn't receive an auth bit name. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (auth_user_data)
    {
        i = authUserGetFieldByName(auth_user_data, rhs);
        if (i < 0) // correct -1 error value to expected error value
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to determine if the current player has the auth bit '%s', but that name doesn't correspond to an auth bit. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
            i = 0;
        }
    }

    eval_IntPush(pcontext, i);
}

void chareval_AuthFetch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    Entity *e = NULL;
    
    eval_FetchInt(pcontext, "Entity", (int *)&e);
    chareval_AuthFetchHelper(pcontext, e && e->pl ? e->pl->auth_user_data : NULL, rhs);
}

void chareval_ScriptFetch(EvalContext *pcontext)
{
#if SERVER
    scripteval_CharEval(pcontext);
#else
    const char *rhs = eval_StringPop(pcontext);
    //estrConcatf(&pcontext->ppchError, "\nThe evaluator tried to retrieve script information about '%s' on the client, but this operation is only supported on the server. It found this while analyzing '%s'.", rhs, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    eval_IntPush(pcontext, 0);
#endif
}

#if SERVER
////////////////////////////////////////////////////////////////////////////////////////
// Calls to here from the character evaluator do not have a script context active.
// So I use a closure to throw this at all running contexts.  The variable needs to start 
// with the instance name of the script it cares about
//
int g_scriptCharEvalResult;

void scripteval_CharEval(EvalContext *pcontext)
{
    const char *variable = eval_StringPop(pcontext);
    g_scriptCharEvalResult = 0;
    ScriptSendScriptMessage(variable);
    eval_IntPush(pcontext, g_scriptCharEvalResult);
}
#endif

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * chareval_Init
* 
 */
void chareval_Init(void)
{
    static bool s_bInitDone = false;

    if(s_bInitDone)
        return;

    // Create the evaluation context used by everyone.
    s_pCharEval = eval_Create();
    s_pCharEval->bVerifyStackSize = true;

    chareval_AddDefaultFuncs(s_pCharEval);

    eval_RegisterFunc(s_pCharEval,   "num>",       chareval_EntityOwnsPower,       1, 1);
    eval_ReRegisterFunc(s_pCharEval, "auto>",      chareval_EntityFetch,           1, 1);

    s_bInitDone=true;
}

/**********************************************************************func*
 * chareval_JustEval
 *
 */
void chareval_JustEval(Character *p, const char * const *ppchExpr)
{
    if (p && p->entParent)
        eval_StoreInt(s_pCharEval, "Entity", (int)(intptr_t)p->entParent);
    else
        eval_ForgetInt(s_pCharEval, "Entity");
    eval_ForgetInt(s_pCharEval, "Task");
    eval_ClearStack(s_pCharEval);

    eval_Evaluate(s_pCharEval, ppchExpr);
}

/**********************************************************************func*
* chareval_Eval
*
*/
int chareval_Eval(Character *p, const char * const *ppchExpr, const char *dataFilename)
{
    eval_SetBlame(s_pCharEval, dataFilename);
    chareval_JustEval(p, ppchExpr);
    return eval_IntPeek(s_pCharEval);
}

#if SERVER
/**********************************************************************func*
* chareval_JustEvalWithTask
*
*/
static void chareval_JustEvalWithTask(Character *p, const char **ppchExpr, StoryTaskInfo *pTask)
{
    if (p)
        eval_StoreInt(s_pCharEval, "Entity", (int)(intptr_t)p->entParent);
    else
        eval_ForgetInt(s_pCharEval, "Entity");
    eval_StoreInt(s_pCharEval, "Task", (int)(intptr_t)pTask);
    eval_ClearStack(s_pCharEval);

    eval_Evaluate(s_pCharEval, ppchExpr);
}

/**********************************************************************func*
* chareval_EvalWithTask
*
*/
int chareval_EvalWithTask(Character *p, const char **ppchExpr, StoryTaskInfo *pTask, const char *dataFilename)
{
    eval_SetBlame(s_pCharEval, dataFilename);
    chareval_JustEvalWithTask(p, ppchExpr, pTask);
    return eval_IntPeek(s_pCharEval);
}
#endif

/**********************************************************************func*
 * chareval_Context
 *
 */
EvalContext *chareval_Context(void)
{
    return s_pCharEval;
}

void chareval_Validate(const char **ppchExpr, const char *dataFilename)
{
#if SERVER
    // only validate on the map, no reason to duplicate work
    chareval_Eval(NULL, ppchExpr, dataFilename);
#endif
}

#if SERVER
void chareval_ValidateWithTask(const char **ppchExpr, const char *dataFilename)
{
#if SERVER
    // only validate on the map, no reason to duplicate work
    chareval_EvalWithTask(NULL, ppchExpr, NULL, dataFilename);
#endif
}
#endif

/**********************************************************************func*
 * chareval_AddDefaultFuncs
 *
 */
void chareval_AddDefaultFuncs(EvalContext *pContext)
{
    if(!pContext)
        return;

    // Basic entity queries
    eval_RegisterFunc(pContext, "char>",                        chareval_EntityFetch,                      1, 1);
    eval_RegisterFunc(pContext, "ent>",                            chareval_EntityFetch,                      1, 1);
    eval_RegisterFunc(pContext, "owner>",                        chareval_EntitysOwnerFetch,                1, 1);

    eval_RegisterFunc(pContext, "EventTime>",                    chareval_EntityEventTime,                  1, 1);
    eval_RegisterFunc(pContext, "EventCount>",                    chareval_EntityEventTime,                  1, 1);

    eval_RegisterFunc(pContext, "now",                            chareval_Now,                              0, 1);

    eval_RegisterFunc(pContext, "stat>",                        chareval_EntityBadgeStatFetch,            1, 1);

    eval_RegisterFunc(pContext, "HasTag?",                        chareval_EntityHasTag,                    1, 1);
    eval_RegisterFunc(pContext, "HasSouvenir?",                    chareval_EntityHasSouvenirClue,            1, 1);
    eval_RegisterFunc(pContext, "HasClue?",                        chareval_EntityHasClue,                    1, 1);
    eval_RegisterFunc(pContext, "TokenOwned?",                    chareval_EntityTokenOwned,                1, 1);
    eval_RegisterFunc(pContext, "CostumeKey?",                    chareval_EntityCostumeKeyOwned,            1, 1);
    eval_RegisterFunc(pContext, "OnTaskForce?",                    chareval_EntityOnTaskForce,                0, 1);
    eval_RegisterFunc(pContext, "OnFlashback?",                    chareval_EntityOnFlashback,                0, 1);
    eval_RegisterFunc(pContext, "OnArchitect?",                    chareval_EntityOnArchitect,                0, 1);
    eval_RegisterFunc(pContext, "TokenVal>",                    chareval_EntityTokenVal,                   1, 1);
    eval_RegisterFunc(pContext, "TokenTime>",                    chareval_EntityTokenTime,                  1, 1);
    eval_RegisterFunc(pContext, "TeamSize>",                    chareval_EntityTeamSize,                1, 1);
    eval_RegisterFunc(pContext, "badgecount",                    chareval_EntityCountBadges,                0, 1);
    eval_RegisterFunc(pContext, "owns?",                        chareval_EntityOwnsBadge,                  1, 1);
    eval_RegisterFunc(pContext, "owned?",                        chareval_EntityOwnsBadge,                  1, 1);
    eval_RegisterFunc(pContext, "BadgeOwned?",                    chareval_EntityOwnsBadge,                  1, 1);
    eval_RegisterFunc(pContext, "mode?",                        chareval_EntityInMode,                     1, 1);
    eval_RegisterFunc(pContext, "powerset?",                    chareval_EntityHasPowerset,                1, 1);
    eval_RegisterFunc(pContext, "enabledpower?",                chareval_EntityHasEnabledPower,            1, 1);
    eval_RegisterFunc(pContext, "ToggleActive?",                chareval_EntityIsToggleActive,            1, 1);
    eval_RegisterFunc(pContext, "CanActivate?",                    chareval_EntityAllowedToActivatePower,    1, 1);
    eval_RegisterFunc(pContext, "ownPower?",                    chareval_EntityOwnsPower,                1, 1);
    eval_RegisterFunc(pContext, "ownPowerNum?",                    chareval_EntityOwnsPowerNum,            1, 1);
    eval_RegisterFunc(pContext, "EmptySlots>",                    chareval_EntityEmptySlots,                1, 1);
    eval_RegisterFunc(pContext, "CanGiveRespec?",                chareval_EntityCanGiveRespec,            0, 1);
    eval_RegisterFunc(pContext, "HasFreespec?",                    chareval_EntityHasFreespec,                0, 1);
    eval_RegisterFunc(pContext, "BoostsSlotted>",                chareval_EntityBoostsSlotted,            1, 1);
    eval_RegisterFunc(pContext, "hasContact?",                    chareval_EntityHasContact,                1, 1);
    eval_RegisterFunc(pContext, "inVolume>",                    chareval_SelfIsInVolume,                1, 1);

    eval_RegisterFunc(pContext, "MissionObjective?",            chareval_MissionObjective,                1, 1);
    eval_RegisterFunc(pContext, "TaskOwner?",                    chareval_TaskOwner,                        0, 1);
    eval_RegisterFunc(pContext, "MissionStarted?",                chareval_MissionStarted,                0, 1);
    eval_RegisterFunc(pContext, "OnStoryArc?",                    chareval_IsOnStoryArc,                    1, 1);
    eval_RegisterFunc(pContext, "HasAnyActiveMission?",            chareval_EntityHasAnyActiveMission,        0, 1);
    eval_RegisterFunc(pContext, "isPvPMap?",                    chareval_IsPvPMap,                        0, 1);
    eval_RegisterFunc(pContext, "isMissionMap?",                chareval_IsMissionMap,                    0, 1);
    eval_RegisterFunc(pContext, "isArchitectMap?",                chareval_IsArchitectMap,                0, 1);
    eval_RegisterFunc(pContext, "isHeroOnlyMap?",                chareval_IsHeroOnlyMap,                    0, 1);
    eval_RegisterFunc(pContext, "isVillainOnlyMap?",            chareval_IsVillainOnlyMap,                0, 1);
    eval_RegisterFunc(pContext, "isHeroAndVillainMap?",            chareval_IsHeroAndVillainMap,            0, 1);
    eval_RegisterFunc(pContext, "isHazardMap?",                    chareval_IsHazardMap,                    0, 1);
    eval_RegisterFunc(pContext, "isTrialMap?",                    chareval_IsTrialMap,                    0, 1);
    eval_RegisterFunc(pContext, "mapTeamArea>",                    chareval_MapTeamArea,                    0, 1);

    eval_RegisterFunc(pContext, "mapname>",                        chareval_MapName,                        0, 1);
    eval_RegisterFunc(pContext, "architect.TokenOwned?",        chareval_EntityArchitectTokenOwned,        1, 1);                                                        

    eval_RegisterFunc(pContext, "isBetaShard?",                    chareval_IsBetaShard,                    0, 1);
    eval_RegisterFunc(pContext, "IsReactivationActive?",        chareval_IsReactivationActive,            0, 1);
    eval_RegisterFunc(pContext, "system>",                        chareval_SystemFetch,                    1, 1);
    eval_RegisterFunc(pContext, "auth>",                        chareval_AuthFetch,                        1, 1);
    eval_RegisterFunc(pContext, "CertsPurchased>",                chareval_CertificationsPurchased,        1, 1);
    eval_RegisterFunc(pContext, "VouchersPurchased>",            chareval_CertificationsPurchased,        1, 1);
    eval_RegisterFunc(pContext, "VouchersUnclaimed>",            chareval_VouchersUnclaimed,                1, 1);
    eval_RegisterFunc(pContext, "LoyaltyOwned?",                chareval_LoyaltyOwned,                    1, 1);
    eval_RegisterFunc(pContext, "LoyaltyTierOwned?",            chareval_LoyaltyTierOwned,                1, 1);
    eval_RegisterFunc(pContext, "LoyaltyLevelOwned?",            chareval_LoyaltyLevelOwned,                1, 1);
    eval_RegisterFunc(pContext, "LoyaltyNodesBought>",            chareval_LoyaltyNodesBought,            0, 1);
    eval_RegisterFunc(pContext, "LoyaltyPointsEarned>",            chareval_LoyaltyPointsEarned,            0, 1);
    eval_RegisterFunc(pContext, "CanBuyPowerWithoutOverflow?",    chareval_CanBuyPowerWithoutOverflow,    1, 1);
    eval_RegisterFunc(pContext, "ProductOwned?",                chareval_ProductOwned,                    1, 1);
    eval_RegisterFunc(pContext, "ProductAvailable?",            chareval_ProductAvailable,                1, 1);
    eval_RegisterFunc(pContext, "isVIP?",                        chareval_IsVIP,                            0, 1);
    eval_RegisterFunc(pContext, "isAccountServerAvailable?",    chareval_IsAccountServerAvailable,        0, 1);
    eval_RegisterFunc(pContext, "isAccountInventoryLoaded?",    chareval_IsAccountInventoryLoaded,        0, 1);

    eval_RegisterFunc(pContext, "ZoneEvent>",                    chareval_ScriptFetch,                    1, 1);

    // Supergroup stuff
    sgeval_AddDefaultFuncs(pContext);

    // Taskforce challenge stuff
    TaskForceEvalAddDefaultFuncs(pContext);
}

// ------------------------------------
// Sticking this function here for no particular reason.
//

int chareval_requires(Character *pchar, char *required, const char *dataFilename)
{
    char        *requires_copy;
    char        *argv[100];
    StringArray expr;
    int            argc;
    int            i;
    int            result;

    result = false;
    requires_copy = strdup(required);
    argc = tokenize_line(requires_copy, argv, 0);
    if (argc > 0)
    {
        eaCreate(&expr);
        for (i = 0; i < argc; i++)
            eaPush(&expr, argv[i]);

        result = chareval_Eval(pchar, expr, dataFilename);
        eaDestroy(&expr);
    }
    if (requires_copy)
        free(requires_copy);

    return result;
}

void chareval_requiresValidate(char *required, const char *dataFilename)
{
    char        *requires_copy;
    char        *argv[100];
    StringArray expr;
    int            argc;
    int            i;

    requires_copy = strdup(required);
    argc = tokenize_line(requires_copy, argv, 0);
    if (argc > 0)
    {
        eaCreate(&expr);
        for (i = 0; i < argc; i++)
            eaPush(&expr, argv[i]);

        chareval_Validate(expr, dataFilename);
        eaDestroy(&expr);
    }
    if (requires_copy)
        free(requires_copy);
}

/* End of File */
