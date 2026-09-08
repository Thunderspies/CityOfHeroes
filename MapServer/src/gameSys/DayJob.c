/***************************************************************************
 *     Copyright (c) 2008, NC Nor Cal
 *     All Rights Reserved
 *     Confidential Property of NC Nor Cal
 ***************************************************************************/
#include "DayJob.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/eval.h>
#include "entity/powers.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StashTable.h>
#include "cmdparse/cmdserver.h"
#include "entity/entity.h"
#include "group/group.h"
#include "group/grouputil.h"
#include <utilitieslib/utils/timing.h>
#include "player/badges_server.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "entity/character_inventory.h"
#include "entity/character_level.h"
#include "TeamReward.h"
#include "player/pophelp.h"

StaticDefineInt DayJobPowerTypeParseEnum[] =
{
    DEFINE_INT
    { "TimeInGame",            kDayJobPowerType_TimeInGame    },
    { "TimeUsed",            kDayJobPowerType_TimeUsed    },
    { "Activation",            kDayJobPowerType_Activation    },
    DEFINE_END
};


static TokenizerParseInfo parse_dayjobpowers[] =
{
    { "{",                            TOK_START, 0},
    { "Power",                        TOK_STRING( DayJobPower, pchPower, 0 ),                },
    { "Salvage",                    TOK_STRING( DayJobPower, pchSalvage, 0 ),            },
    { "Requires",                    TOK_STRINGARRAY( DayJobPower, ppchRequires)            },
    { "Factor",                        TOK_F32(DayJobPower, fFactor,0),                    },
    { "Max",                        TOK_INT(DayJobPower, iMax, 0),                        },
    { "RemainderToken",                TOK_STRING( DayJobPower, pchRemainderToken, 0 ),    },
    { "Type",                        TOK_INT(DayJobPower, eType, kDayJobPowerType_TimeInGame), DayJobPowerTypeParseEnum },
    { "}",                            TOK_END,   0},
    { "", 0, 0 }
};


static TokenizerParseInfo parse_dayjobdetail[] =
{
    { "{",                            TOK_START, 0},
    { "SourceFile",                    TOK_CURRENTFILE                        ( DayJobDetail, pchSourceFile)        },
    { "Name",                        TOK_STRUCTPARAM | TOK_STRING        ( DayJobDetail, pchName, 0 ),           },
    { "DisplayName",                TOK_STRING( DayJobDetail, pchDisplayName, 0 ), },
    { "VolumeName",                    TOK_STRINGARRAY( DayJobDetail, ppchVolumeNames ), },
    { "ZoneName",                    TOK_STRING( DayJobDetail, pchZoneName, 0 ), },
    { "Stat",                        TOK_STRING( DayJobDetail, pchStat, 0 ), },
    { "Power",                        TOK_STRUCT( DayJobDetail, ppPowers, parse_dayjobpowers ) },
    { "}",              TOK_END,   0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseDayJobDetailDict[] =
{
    { "DayJob",                TOK_STRUCT(DayJobDetailDict, ppJobs, parse_dayjobdetail)    },
    { "MinTime",            TOK_INT(DayJobDetailDict, minTime, 0)                        },
    { "PatrolXPMultiplier",    TOK_F32(DayJobDetailDict, fPatrolXPMultiplier, 2.0f),        },
    { "PatrolScalar",        TOK_F32(DayJobDetailDict, fPatrolScalar, 0)                    }, // 0.0001f) },
    { "", 0, 0 }
};

SHARED_MEMORY DayJobDetailDict g_DayJobDetailDictDict;

//------------------------------------------------------------
//
//----------------------------------------------------------

static bool dayjobdetail_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    DayJobDetailDict * pdict = (DayJobDetailDict *)structptr;
    bool ret = true;

    // if the params are valid
    if( verify(pdict) && verify(pdict->haVolumeNames == NULL ))
    {
        int i, j, njobs, nzones, nvolumes;

        // --------------------
        //  create the hash tables

        // first calc size of each table
        njobs = eaSize( &pdict->ppJobs );
        nvolumes = nzones = 0;
        for( i = 0; i < njobs; ++i )
        {
            DayJobDetail *pJob = (DayJobDetail*)pdict->ppJobs[i];
            if(pJob->ppchVolumeNames != NULL)
                nvolumes += eaSize(&pJob->ppchVolumeNames);
            if(pJob->pchZoneName)
                nzones++;
        }

        pdict->haVolumeNames = stashTableCreateWithStringKeys( stashOptimalSize(nvolumes), stashShared(shared_memory) );
        pdict->haZoneNames = stashTableCreateWithStringKeys( stashOptimalSize(nzones), stashShared(shared_memory) );

        // --------------------
        // fill hashes
        for( i = 0; i < njobs; ++i )
        {
            DayJobDetail *pJob = (DayJobDetail*)pdict->ppJobs[i];

            // add the hash
            if(pJob->ppchVolumeNames != NULL)
            {
                for (j = 0; j < eaSize(&pJob->ppchVolumeNames); j++)
                {
                    if (!stashAddPointerConst(pdict->haVolumeNames, pJob->ppchVolumeNames[j], pJob, false))
                    {
                        Errorf("duplicate volume name %s on job %s", pJob->ppchVolumeNames[j], pJob->pchName );
                        ret = false;
                    }
                }                
            }

            if(pJob->pchZoneName != NULL && !stashAddPointerConst(pdict->haZoneNames, pJob->pchZoneName, pJob, false) )
            {
                Errorf("duplicate zone name %s on job %s", pJob->pchZoneName, pJob->pchName );
                ret = false;
            }

            for (j = 0; j < eaSize(&pJob->ppPowers); j++)
            {
                if (pJob->ppPowers[j]->ppchRequires)
                    chareval_Validate(pJob->ppPowers[j]->ppchRequires, pJob->pchSourceFile);
            }
        }
    }

    return ret;
}

//------------------------------------------------------------
//  Load the job definitions
//----------------------------------------------------------
void dayjobdetail_Load(void)
{
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName("SM_DayJobs"), "defs", ".dayjobs", "DayJobs.bin",
                          flags, ParseDayJobDetailDict, &g_DayJobDetailDictDict, sizeof(DayJobDetailDict),
                          NULL, NULL, NULL, NULL, dayjobdetail_FinalProcess);
}


float dayjob_GetPatrolXPMultiplier(void)
{
    return g_DayJobDetailDictDict.fPatrolXPMultiplier;
}

//------------------------------------------------------------
//  award player job credit
//----------------------------------------------------------
static void dayjob_awardCredit(Entity *pPlayer, const DayJobDetail *pJob, U32 timeSinceLastOn)
{
    int i;
    const BasePower *pBasePower = NULL;
    const SalvageItem *si = NULL;

    // see if we should give credit towards badge
    if (timeSinceLastOn > 0 && pJob->pchStat != NULL)
        badge_StatAdd(pPlayer, pJob->pchStat, timeSinceLastOn);

    // see if any powers should be awarded or updated
    for (i = 0; i < eaSize(&pJob->ppPowers); i++)
    {
        DayJobPower *pJobPower = pJob->ppPowers[i];

        if (pJobPower)
        {
            const char *filename = "";
            if (pJobPower->pchPower)
            {
                pBasePower = basepower_FromPath(pJobPower->pchPower);
                if (pBasePower)
                {
                    filename = pBasePower->pchSourceFile;
                }
            }
            else if (pJobPower->pchSalvage)
            {
                si = salvage_GetItem(pJobPower->pchSalvage);
                if (si)
                {
                    filename = "data/defs/salvage.salvage";
                }
            }

            if (pJobPower->ppchRequires == NULL || chareval_Eval(pPlayer->pchar, pJobPower->ppchRequires, filename))
            {
                int timeRemaining = 0;
                int usesRemaining = 0;

                if (pBasePower)
                {
                    // make sure player has power
                    Power *pPower = character_OwnsPower(pPlayer->pchar, pBasePower);

                    if (!pPower)
                    {
                        PowerSet *pset = character_OwnsPowerSet(pPlayer->pchar, pBasePower->psetParent);

                        if (pset == NULL)
                            pset=character_BuyPowerSet(pPlayer->pchar, pBasePower->psetParent);

                        if (!pset)
                            continue;

                        pPower=character_BuyPower(pPlayer->pchar, pset, pBasePower, 0);

                        if (!pPower)
                            continue;
                    } else {
                        if (pJobPower->eType == kDayJobPowerType_TimeInGame)
                            timeRemaining = pBasePower->fLifetimeInGame - pPower->fAvailableTime;
                        else if (pJobPower->eType == kDayJobPowerType_Activation)
                            usesRemaining = pPower->iNumCharges; 
                        else if (pJobPower->eType == kDayJobPowerType_TimeUsed)
                            timeRemaining = pPower->fUsageTime;

                    }

                    // adjust timing of power based on how long they were offline
                    if (pJobPower->eType == kDayJobPowerType_TimeInGame)
                    {
                        int delta = (int) ((float) timeSinceLastOn * pJobPower->fFactor);

                        if (pJobPower->iMax > 0 && delta > pJobPower->iMax)
                            delta = pJobPower->iMax;

                        timeRemaining += delta;
                        pPower->fAvailableTime = pBasePower->fLifetimeInGame - timeRemaining;
                        if (pPower->fAvailableTime < 0.0f)
                            pPower->fAvailableTime = 0.0f;
                    } 
                    else if (pJobPower->eType == kDayJobPowerType_TimeUsed) 
                    {
                        int delta = (int) ((float) timeSinceLastOn * pJobPower->fFactor);

                        if (pJobPower->iMax > 0 && delta > pJobPower->iMax)
                            delta = pJobPower->iMax;
                        timeRemaining += delta;
                        pPower->fUsageTime = timeRemaining;
                        if (pPower->fUsageTime > pBasePower->fUsageTime)
                            pPower->fUsageTime = pBasePower->fUsageTime;
                    }
                    else if (pJobPower->eType == kDayJobPowerType_Activation) 
                    {
                        RewardToken *pToken = NULL;    
                        int usesAdded, secondsNotUsed;

                        if (pJobPower->pchRemainderToken != NULL)
                        {
                            pToken = getRewardToken(pPlayer, pJobPower->pchRemainderToken);
                            if (pToken)
                            {
                                timeSinceLastOn += pToken->val;
                            } else {
                                pToken = giveRewardToken(pPlayer, pJobPower->pchRemainderToken);
                            }
                        }
                        usesAdded = (int) ((float) timeSinceLastOn * pJobPower->fFactor);
                        secondsNotUsed = timeSinceLastOn - (int) ((float) usesAdded / pJobPower->fFactor);  // removed used before clamping
                        if (pJobPower->iMax > 0 && usesAdded > pJobPower->iMax)
                            usesAdded = pJobPower->iMax;
                        usesRemaining += usesAdded;

                        if (usesRemaining > pBasePower->iNumCharges)
                            usesRemaining = pBasePower->iNumCharges;
                        pPower->iNumCharges = usesRemaining;

                        if (pToken)
                            pToken->val = secondsNotUsed;
                    }

                    // This forces a full redefine to be sent.
                    eaPush(&pPlayer->powerDefChange, (void *)-1);
                }
                else if (pJobPower->pchSalvage != NULL)
                {
                    int added = (int) ((float) timeSinceLastOn * pJobPower->fFactor);
                    int secondsNotUsed;
                    RewardToken *pToken = NULL;    
                    SalvageInventoryItem *sii = NULL;

                    if (pJobPower->pchRemainderToken != NULL)
                    {
                        pToken = getRewardToken(pPlayer, pJobPower->pchRemainderToken);
                        if (pToken)
                        {
                            timeSinceLastOn += pToken->val;
                        } else {
                            pToken = giveRewardToken(pPlayer, pJobPower->pchRemainderToken);
                        }
                    }

                    secondsNotUsed = timeSinceLastOn - (int) ((float) added / pJobPower->fFactor);  // removed used before clamping
                    if (pJobPower->iMax > 0 && added > pJobPower->iMax)
                        added = pJobPower->iMax;

                    sii = character_FindSalvageInv(pPlayer->pchar, pJobPower->pchSalvage);
                    if( sii )
                    {
                        if (pJobPower->iMax > 0 && ((int) sii->amount + added > pJobPower->iMax))
                        {
                            added = MAX(pJobPower->iMax - sii->amount, 0);
                        }
                    }
    
                    if( si )
                    {
                        if(character_CanAdjustSalvage(pPlayer->pchar, si, added) )
                            character_AdjustSalvage( pPlayer->pchar, si, added, "DayJob", false);
                    }

                    if (pToken)
                        pToken->val = secondsNotUsed;
                }
            }
        }
    }
}

static F32 restPercent( Entity * e )
{    
    F32 fCurrent = 0.f;
    int iRest = e->pchar->iExperienceRest;
    int iLevel = character_CalcExperienceLevel(e->pchar) + 1;
    int iXPAtCurrentLevel = character_ExperienceNeeded(e->pchar);
    int iXPForLevel = character_AdditionalExperienceNeededForLevel(e->pchar, iLevel);

    if( iLevel >= 50 )
        return fCurrent;

    fCurrent += (float)MIN(iRest, iXPAtCurrentLevel) / (float) iXPForLevel;
    iRest -= iXPAtCurrentLevel;

    while(iRest > 0 && iLevel < 50)
    {
        iXPForLevel = character_AdditionalExperienceNeededForLevel(e->pchar, iLevel);

        fCurrent += (float) MIN(iRest,iXPForLevel) / (float) iXPForLevel;
        iRest -= iXPForLevel;
        iLevel++;
    }

    return fCurrent;
}

void awardRestExperience( Entity * e, F32 fNewPercent )
{
    F32 fCurrentPercent = restPercent(e);
    int iLevel = character_CalcExperienceLevel(e->pchar) + 1;
    int iHowMuchForThisLevel = character_AdditionalExperienceNeededForLevel(e->pchar, iLevel);

    fCurrentPercent += fNewPercent;

    // use it to pay debt if there is any
    if (e->pchar->iExperienceDebt > 0)
    {
        int iXPTowardsDebt = (int)(fCurrentPercent * iHowMuchForThisLevel);

        if (iHowMuchForThisLevel <= 0) 
        {    
            // for max level
            iXPTowardsDebt = (int)(fCurrentPercent * e->pchar->iExperiencePoints);
            if (iXPTowardsDebt > e->pchar->iExperienceDebt)
            {
                badge_RecordExperienceDebtRemoved(e, e->pchar->iExperienceDebt);    // giving credit for removal of debt
                e->pchar->iExperienceDebt = 0;
            } 
            else
            {
                badge_RecordExperienceDebtRemoved(e, iXPTowardsDebt);    // giving credit for removal of debt
                e->pchar->iExperienceDebt -= iXPTowardsDebt;
            }
            fCurrentPercent = 0.0f;
        } 
        else if (iXPTowardsDebt < e->pchar->iExperienceDebt)
        {
            badge_RecordExperienceDebtRemoved(e, iXPTowardsDebt);    // giving credit for removal of debt
            e->pchar->iExperienceDebt -= iXPTowardsDebt;
            fCurrentPercent = 0.0f;
        } 
        else
        {
            float fDebtPercentage = (float)e->pchar->iExperienceDebt / (float) iHowMuchForThisLevel;
            badge_RecordExperienceDebtRemoved(e, e->pchar->iExperienceDebt);    // giving credit for removal of debt
            e->pchar->iExperienceDebt = 0;
            fCurrentPercent -= fDebtPercentage;

            if (fCurrentPercent <= 0.0f)
                fCurrentPercent = 0.0f;
        }
    }

    // use it to calculate rest xp
    if (fCurrentPercent > 0.0f)
    {
        int iXPRemainingThisLevel = character_ExperienceNeeded(e->pchar);
        int iXPRest = (int)(fCurrentPercent * iHowMuchForThisLevel);
        int iCount = 1;
        // record percent for badge
        badge_StatAdd(e, "DayJob.Patrol_Percent", (int) (fNewPercent * 100));

        if (iXPRest > iXPRemainingThisLevel )
        {
            float fPercentThisLevel = (float) iXPRemainingThisLevel / (float) iHowMuchForThisLevel;
            iXPRest = iXPRemainingThisLevel;
            fCurrentPercent -= fPercentThisLevel;
            while( fCurrentPercent > 0 && iLevel + iCount < 50)
            {
                if( fCurrentPercent >= 1.f )
                    iXPRest += character_AdditionalExperienceNeededForLevel(e->pchar, iLevel + iCount);
                else
                    iXPRest += fCurrentPercent * character_AdditionalExperienceNeededForLevel(e->pchar, iLevel + iCount);

                fCurrentPercent -= 1.f;
                iCount++;
            }
        }

        if (getPopHelpStateByTag(e, "CODEPH_Patrol_XP") == PopHelpState_Untriggered)
        {
            triggerPopHelpEventHappenedByTag(e, "CODEPH_Patrol_XP");
        }

        e->pchar->iExperienceRest = iXPRest;
    }
}

//------------------------------------------------------------
//  handle player tick
//----------------------------------------------------------
void dayjob_Tick(Entity *pPlayer)
{
    int    timeSinceLastOn;
    int bFound = false;
    U32 day_job_started = 0;

    //pPlayer->last_day_jobs_start is reset to zero once used.
    //we should only get past this if
    //(1) we just logged on OR
    //(2) we updated entity time fields in container load save.
    //   |--> happens every 8 minutes.
    if (!pPlayer || !pPlayer->last_day_jobs_start)
        return;

    day_job_started = pPlayer->last_day_jobs_start;
    //even if we don't apply the day job, last_day_jobs_start value has expired.
    pPlayer->last_day_jobs_start = 0;
    if(pPlayer->dayjob_applied)
        return;
    

    //last
    timeSinceLastOn = timerSecondsSince2000() - day_job_started;

    if (timeSinceLastOn <= g_DayJobDetailDictDict.minTime || day_job_started == 0)
    {
        pPlayer->dayjob_applied = true;
        return;
    }

    {
        Volume *entVolume = pPlayer->volumeList.materialVolume;
        if (entVolume && entVolume->volumePropertiesTracker)
        {
            const DayJobDetail *pJob = NULL;
            cStashElement elt;

            char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def, "NamedVolume");
            if (entVolumeName) // For now just ignore any volume without this property
            {
                stashFindElementConst( g_DayJobDetailDictDict.haVolumeNames, entVolumeName, &elt);

                if (elt)
                {
                    bFound = true;
                    pJob = (const DayJobDetail *) stashElementGetPointerConst(elt);

                    dayjob_awardCredit(pPlayer, pJob, timeSinceLastOn);
                }
            }
        }
    }

    // Look through zone list and see if it's in a target zone
    if (!bFound)
    {
        const DayJobDetail *pJob = NULL;
        cStashElement elt;
        char mapbase[MAX_PATH];
        saUtilFilePrefix(mapbase, world_name);

        stashFindElementConst( g_DayJobDetailDictDict.haZoneNames, mapbase, &elt);

        if (elt)
        {
            bFound = true;
            pJob = (const DayJobDetail *) stashElementGetPointerConst(elt);

            dayjob_awardCredit(pPlayer, pJob, timeSinceLastOn);
        }
    }

    // award Patrol rest time
     if (g_DayJobDetailDictDict.fPatrolScalar > 0.0)
    {
        float fAdditionalPercent = (float) timeSinceLastOn * g_DayJobDetailDictDict.fPatrolScalar;
        float fCurrentPercent = 0.0f;
        float fOldPercent = 0.0f;
        int iLevel = character_CalcExperienceLevel(pPlayer->pchar) + 1;
        int iHowMuchForThisLevel = character_AdditionalExperienceNeededForLevel(pPlayer->pchar, iLevel);

        // record time for badge
        badge_StatAdd(pPlayer, "DayJob.On_Patrol", timeSinceLastOn);

        // get percent of any rest they already have
        fCurrentPercent = restPercent(pPlayer);

        // new total percent
        if(fCurrentPercent + fAdditionalPercent > 1.f )
            fAdditionalPercent = 1.f - fCurrentPercent;
        
        // use it to calculate rest xp
        if( fAdditionalPercent > 0 )
            awardRestExperience(pPlayer, fAdditionalPercent);
    }

    pPlayer->dayjob_applied = true;
}


//------------------------------------------------------------
//  Used by halloween trick or treat code, determine if a
//  player is currently in a day job named volume
//----------------------------------------------------------
int dayjob_IsPlayerInAnyDayjobVolume(Entity *pPlayer)
{
    Volume *entVolume;
    DayJobDetail *pJob = NULL;
    cStashElement elt;
    char *entVolumeName;

    if (pPlayer == NULL)
    {
        // return 1 in error cases - this disables ToT'ing, which is
        // probably the safest way to fail gracefully
        return 1;
    }

    entVolume = pPlayer->volumeList.materialVolume;

    if (entVolume == NULL)
    {
        // entVolume must be non-NULL
        return 1;
    }

    if (entVolume->volumePropertiesTracker == NULL)
    {
        // However a NULL volumePropertiesTracker means we're not in any volumes,
        // therefore we can't possibly be in a dayjob volume
        return 0;
    }

    entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def, "NamedVolume");
    if (entVolumeName) // For now just ignore any volume without this property
    {
        stashFindElementConst( g_DayJobDetailDictDict.haVolumeNames, entVolumeName, &elt);

        if (elt)
        {
            return 1;
        }
    }

    return 0;
}


/* End of File */
