/*\
 *
 *    contactdef.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles static contact definitions and initialization checks
 *    on these
 *
 */

#include "contact.h"
#include "storyarcprivate.h"
#include "pnpc.h"
#include "gameComm/Npc.h"
#include "player/badges.h"
#include "player/badges_server.h"
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "language/commonLangUtil.h"
#include "storyarc/pnpcCommon.h"
#include "taskforce.h"
#include "entity/teamCommon.h"
#include "entity/character_eval.h"
#include "Reward.h"
#include "taskdef.h"
#include "entity/character_level.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "svr/svr_base.h"
#include "comm_game.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/staticMapInfo.h"
#include "dialogdef.h"

#if SERVER
    #include "cmdparse/cmdserver.h"
#endif

SHARED_MEMORY DesignerContactTipTypes g_DesignerContactTipTypes;

// *********************************************************************************
//  Contact definition parsing
// *********************************************************************************

TokenizerParseInfo ParseStoryArcRef[] = {
    { "", TOK_STRUCTPARAM | TOK_FILENAME(StoryArcRef,filename,0) },
    { "\n",                TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoryTaskInclude[] = {
    { "", TOK_STRUCTPARAM | TOK_FILENAME(StoryTaskInclude,filename,0) },
    { "\n",                TOK_END,        0 },
    { "", 0, 0 }
};

int g_disableAutoDismiss = 0;

StaticDefineInt ParseContactFlag[] = {
    DEFINE_INT
    { "Deprecated",                CONTACT_DEPRECATED            },
    { "TaskForceContact",        CONTACT_TASKFORCE            },
    { "CanTrain",                CONTACT_TRAINER                },
    { "TutorialContact",        CONTACT_TUTORIAL            },
    { "TeleportOnComplete",        CONTACT_TELEPORTONCOMPLETE    },
    { "AutoIssueContact",        CONTACT_ISSUEONINTERACT        },
    { "IssueOnInteract",        CONTACT_ISSUEONINTERACT        },
    { "AutoHideContact",        CONTACT_AUTOHIDE            },
    { "NoFriendIntroduction",    CONTACT_NOFRIENDINTRO        },
    { "NoCompleteIntroduction",    CONTACT_NOCOMPLETEINTRO        },
    { "DontIntroduceMe",        CONTACT_DONTINTRODUCEME        },
    { "NoIntroductions",        CONTACT_NOINTRODUCTIONS        },
    { "NoHeadshot",                CONTACT_NOHEADSHOT            },
    { "NoAboutPage",            CONTACT_NOABOUTPAGE            },
    { "CanTailor",                CONTACT_TAILOR                },
    { "CanRespec",                CONTACT_RESPEC                },
    { "NotorietyContact",        CONTACT_NOTORIETY            },
    { "Broker",                    CONTACT_BROKER                },
    { "Newspaper",                CONTACT_NEWSPAPER            },
    { "CanTeach",                CONTACT_TEACHER                },
    { "ScriptControlled",        CONTACT_SCRIPT                },
    { "PvPContact",                CONTACT_PVP                    },
    { "NoCellphone",            CONTACT_NOCELLPHONE            },
    { "Registrar",                CONTACT_REGISTRAR            },
    { "SupergroupMissionContact",CONTACT_SUPERGROUP            },
    { "MetaContact",            CONTACT_METACONTACT            },
    { "IssueOnZoneEnter",        CONTACT_ISSUEONZONEENTER    },
    { "HideOnComplete",            CONTACT_HIDEONCOMPLETE        },
    { "Flashback",                CONTACT_FLASHBACK            },
    { "PlayerMissionGiver",        CONTACT_PLAYERMISSIONGIVER    },
    { "StartWithCellPhone",        CONTACT_STARTWITHCELLPHONE    },
    { "IsTip",                    CONTACT_TIP                    },
    { "Dismissable",            CONTACT_DISMISSABLE            },
    { "AutoDismiss",            CONTACT_AUTODISMISS            },
    DEFINE_END
};

StaticDefineInt ParseContactAlliance[] = {
    DEFINE_INT
    { "Both",                    ALLIANCE_NEUTRAL },
    { "Neutral",                ALLIANCE_NEUTRAL },
    { "Hero",                    ALLIANCE_HERO },
    { "Villain",                ALLIANCE_VILLAIN },
    DEFINE_END
};

StaticDefineInt ParseContactUniverse[] = {
    DEFINE_INT
    { "Both",                    UNIVERSE_BOTH },
    { "Neutral",                UNIVERSE_BOTH },
    { "Primal",                    UNIVERSE_PRIMAL },
    { "Praetorian",                UNIVERSE_PRAETORIAN },
    DEFINE_END
};

StaticDefineInt ParseContactTipType[] = {
    DEFINE_INT
    { "None",                    TIPTYPE_NONE },
    { "AlignmentTip",            TIPTYPE_ALIGNMENT },
    { "MoralityTip",            TIPTYPE_MORALITY },
    { "Special",                TIPTYPE_SPECIAL },
    DEFINE_END
};

TokenizerParseInfo ParseContactDef[] = {
    { "{",                            TOK_START,        0},
    { "",                            TOK_CURRENTFILE(ContactDef,filename) },

    // strings
    { "Name",                        TOK_STRING(ContactDef,displayname,0) },
    { "Gender",                        TOK_INT(ContactDef,gender,0),        ParseGender },
    { "Status",                        TOK_INT(ContactDef,status,0),        ParseStatus },
    { "Alliance",                    TOK_INT(ContactDef,alliance,ALLIANCE_UNSPECIFIED),    ParseContactAlliance },
    { "Universe",                    TOK_INT(ContactDef,universe,UNIVERSE_UNSPECIFIED),    ParseContactUniverse },
    { "TipType",                    TOK_STRING(ContactDef,tipTypeStr,0) },
    { "DescriptionString",            TOK_STRING(ContactDef,description,0) },
    { "ProfessionString",            TOK_STRING(ContactDef,profession,0) },
    { "HelloString",                TOK_STRING(ContactDef,hello,0) },
    { "SOLHelloString",                TOK_STRING(ContactDef,SOLhello,0) },
    { "IDontKnowYouString",            TOK_STRING(ContactDef,dontknow,0) },
    { "SOLIDontKnowYouString",        TOK_STRING(ContactDef,SOLdontknow,0) },
    { "FirstVisitString",            TOK_STRING(ContactDef,firstVisit,0) },
    { "SOLFirstVisitString",        TOK_STRING(ContactDef,SOLfirstVisit,0) },
    { "NoTasksRemainingString",        TOK_STRING(ContactDef,noTasksRemaining,0) },
    { "SOLNoTasksRemainingString",    TOK_STRING(ContactDef,SOLnoTasksRemaining,0) },
    { "PlayerBusyString",            TOK_STRING(ContactDef,playerBusy,0) },
    { "SOLPlayerBusyString",        TOK_STRING(ContactDef,SOLplayerBusy,0) },
    { "ComeBackLaterString",        TOK_STRING(ContactDef,comeBackLater,0) },
    { "SOLComeBackLaterString",        TOK_STRING(ContactDef,SOLcomeBackLater,0) },
    { "ZeroContactPointsString",    TOK_STRING(ContactDef,zeroRelPoints,0) },        // for tutorial contact
    { "SOLZeroContactPointsString",    TOK_STRING(ContactDef,SOLzeroRelPoints,0) },    // for tutorial contact
    { "NeverIssuedTaskString",        TOK_STRING(ContactDef,neverIssuedTask,0) },        // for tutorial contact
    { "SOLNeverIssuedTaskString",    TOK_STRING(ContactDef,SOLneverIssuedTask,0) },        // for tutorial contact
    { "WrongAllianceString",        TOK_STRING(ContactDef,wrongAlliance,0) },
    { "SOLWrongAllianceString",        TOK_STRING(ContactDef,SOLwrongAlliance,0) },
    { "WrongUniverseString",        TOK_STRING(ContactDef,wrongUniverse,0) },
    { "SOLWrongUniverseString",        TOK_STRING(ContactDef,SOLwrongUniverse,0) },
    { "CallTextOverride",            TOK_STRING(ContactDef,callTextOverride,0) },
    { "AskAboutTextOverride",        TOK_STRING(ContactDef,askAboutTextOverride,0) },
    { "LeaveTextOverride",            TOK_STRING(ContactDef,leaveTextOverride,0) },
    { "ImageOverride",                TOK_STRING(ContactDef,imageOverride,0) },
    { "DismissOptionOverride",        TOK_STRING(ContactDef,dismissOptionOverride,0) },
    { "DismissConfirmTextOverride",    TOK_STRING(ContactDef,dismissConfirmTextOverride,0) },
    { "DismissConfirmYesOverride",    TOK_STRING(ContactDef,dismissConfirmYesOverride,0) },
    { "DismissConfirmNoOverride",    TOK_STRING(ContactDef,dismissConfirmNoOverride,0) },
    { "DismissSuccessOverride",        TOK_STRING(ContactDef,dismissSuccessOverride,0) },
    { "DismissUnableOverride",        TOK_STRING(ContactDef,dismissUnableOverride,0) },
    { "TaskDeclineOverride",        TOK_STRING(ContactDef,taskDeclineOverride,0) },
    { "NoCellOverride",                TOK_STRING(ContactDef,noCellOverride,0) },

    { "MinutesTillExpire",            TOK_F32(ContactDef,minutesExpire,0) },
    { "RelationshipChangeString_ToFriend",        TOK_STRING(ContactDef,relToFriend,0) },
    { "RelationshipChangeString_ToConfidant",    TOK_STRING(ContactDef,relToConfidant,0) },
    { "IntroString",                TOK_STRING(ContactDef,introduction,0) },
    { "IntroAcceptString",            TOK_STRING(ContactDef,introductionAccept,0) },
    { "IntroSendoffString",            TOK_STRING(ContactDef,introductionSendoff,0) },
    { "SOLIntroSendoffString",        TOK_STRING(ContactDef,SOLintroductionSendoff,0) },
    { "IntroduceOneContact",        TOK_STRING(ContactDef,introduceOneContact,0) },
    { "IntroduceTwoContacts",        TOK_STRING(ContactDef,introduceTwoContacts,0) },
    { "FailSafeLevelString",        TOK_STRING(ContactDef,failsafeString,0) },
    { "SOLFailSafeLevelString",        TOK_STRING(ContactDef,SOLfailsafeString,0) },
    { "LocationString",                TOK_STRING(ContactDef,locationName,0) },

    // Subcontacts for MetaContacts
    { "SubContacts",                TOK_STRINGARRAY(ContactDef,subContacts) },
    { "SubContactsDisplay",            TOK_STRINGARRAY(ContactDef,subContactsDisplay) },

    //contact requires statement
    { "InteractionRequires",        TOK_STRINGARRAY(ContactDef,interactionRequires) },
    { "ContactRequires",            TOK_REDUNDANTNAME | TOK_STRINGARRAY(ContactDef,interactionRequires) },
    { "InteractionRequiresText",    TOK_STRING(ContactDef,interactionRequiresFailString,0) },
    { "ContactRequiresText",        TOK_REDUNDANTNAME | TOK_STRING(ContactDef,interactionRequiresFailString,0) },

    // Badge Requirements
    { "RequiredBadge",                TOK_STRING(ContactDef,requiredBadge,0) },
    { "BadgeNeededString",            TOK_STRING(ContactDef,badgeNeededString,0) },
    { "SOLBadgeNeededString",        TOK_STRING(ContactDef,SOLbadgeNeededString,0) },
    { "BadgeFirstTick",                TOK_STRING(ContactDef,badgeFirstTick,0) },
    { "BadgeSecondTick",            TOK_STRING(ContactDef,badgeSecondTick,0) },
    { "BadgeThirdTick",                TOK_STRING(ContactDef,badgeThirdTick,0) },
    { "BadgeFourthTick",            TOK_STRING(ContactDef,badgeFourthTick,0) },

    // Map Token Requirement
    { "MapTokenRequired",            TOK_STRING(ContactDef,mapTokenRequired,0) },

    // task force strings
    { "RequiredToken",                TOK_STRING(ContactDef,requiredToken,0) },
    { "DontHaveToken",                TOK_STRING(ContactDef,dontHaveToken,0) },
    { "SOLDontHaveToken",            TOK_STRING(ContactDef,SOLdontHaveToken,0) },
    { "PlayerLevelTooLow",            TOK_STRING(ContactDef,levelTooLow,0) },
    { "PlayerLevelTooHigh",            TOK_STRING(ContactDef,levelTooHigh,0) },
    { "NeedLargeTeam",                TOK_STRING(ContactDef,needLargeTeam,0) },
    { "NeedTeamLeader",                TOK_STRING(ContactDef,needTeamLeader,0) },
    { "NeedTeamOnMap",                TOK_STRING(ContactDef,needTeamOnMap,0) },
    { "BadTeamLevel",                TOK_STRING(ContactDef,badTeamLevel,0) },
    { "TaskForceInvite",            TOK_STRING(ContactDef,taskForceInvite,0) },
    { "TaskForceRewardRequires",    TOK_STRINGARRAY(ContactDef,taskForceRewardRequires) },
    { "TaskForceRewardRequiresText",TOK_STRING(ContactDef,taskForceRewardRequiresText,0) },
    { "TaskForceName",                TOK_STRING(ContactDef,taskForceName,0) },
    { "TaskForceLevelAdjust",        TOK_INT(ContactDef,taskForceLevelAdjust,0) },

    // deprecated
    { "StoreTitleString",            TOK_STRING(ContactDef,old_text,0) },
    { "Icon",                        TOK_STRING(ContactDef,old_text,0) },
    { "StoreYouBoughtXXXString",    TOK_STRING(ContactDef,old_text,0) },

    // logical fields
    { "TaskDef",                    TOK_STRUCT(ContactDef,mytasks,ParseStoryTask) },
    { "TaskInclude",                TOK_STRUCT(ContactDef,taskIncludes,ParseStoryTaskInclude) },
    { "",                            TOK_AUTOINTEARRAY(ContactDef,tasks) },
    // this will allow me to force the shared memory to allocate however much space I need
    { "StoryArc",                    TOK_STRUCT(ContactDef,storyarcrefs,ParseStoryArcRef) },
    { "Dialog",                        TOK_STRUCT(ContactDef,dialogDefs,ParseDialogDef) },

    { "Stature",                    TOK_INT(ContactDef,stature,0) },
    { "StatureSet",                    TOK_STRING(ContactDef,statureSet,0) },
    { "NextStatureSet",                TOK_STRING(ContactDef,nextStatureSet,0) },
    { "NextStatureSet2",            TOK_STRING(ContactDef,nextStatureSet2,0) },
    { "ContactsAtOnce",                TOK_INT(ContactDef,contactsAtOnce,0) },
    { "FriendCP",                    TOK_INT(ContactDef,friendCP,0) },
    { "ConfidantCP",                TOK_INT(ContactDef,confidantCP,0) },
    { "CompleteCP",                    TOK_INT(ContactDef,completeCP,0) },
    { "HeistCP",                    TOK_INT(ContactDef,heistCP,0) },
    { "CompletePlayerLevel",        TOK_INT(ContactDef,completePlayerLevel,0) },
    { "FailsafePlayerLevel",        TOK_INT(ContactDef,failsafePlayerLevel,0) },
    { "MinPlayerLevel",                TOK_INT(ContactDef,minPlayerLevel,1) },
    { "MaxPlayerLevel",                TOK_INT(ContactDef,maxPlayerLevel,MAX_PLAYER_SECURITY_LEVEL) },
    { "Store",                        TOK_EMBEDDEDSTRUCT(ContactDef, store, ParseContactStore) },

    { "StoreCount",                 TOK_INT(ContactDef,stores.iCnt,0) },
    { "Stores",                     TOK_STRINGARRAY(ContactDef,stores.ppchStores) },

    { "AccessibleContact",            TOK_INT(ContactDef,accessibleContactValue,0) },

    { "KnownVillainGroups",            TOK_INTARRAY(ContactDef,knownVillainGroups), ParseVillainGroupEnum},

    { "DismissReward",                TOK_STRUCT(ContactDef,dismissReward,ParseStoryReward) },

    // flags
    { "Deprecated",                    TOK_BOOLFLAG(ContactDef,deprecated,0) },
    { "SupergroupContact",            TOK_BOOLFLAG(ContactDef,deprecated,0) }, // DEPRECATED
    { "CanTrain",                    TOK_BOOLFLAG(ContactDef,canTrain,0) },
    { "TutorialContact",            TOK_BOOLFLAG(ContactDef,tutorialContact,0) },
    { "TeleportOnComplete",            TOK_BOOLFLAG(ContactDef,teleportOnComplete,0) },
    { "AutoIssueContact",            TOK_BOOLFLAG(ContactDef,autoIssue,0) },
    { "AutoHideContact",            TOK_BOOLFLAG(ContactDef,autoHide,0) },
    { "TaskForceContact",            TOK_BOOLFLAG(ContactDef,taskforceContact,0) },
    { "NoFriendIntroduction",        TOK_BOOLFLAG(ContactDef,noFriendIntro,0) },
    { "MinTaskForceSize",            TOK_INT(ContactDef,minTaskForceSize,BEGIN_TASKFORCE_SIZE) },

    // new flags field
    { "Flags",                        TOK_FLAGARRAY(ContactDef,flags,CONTACT_DEF_FLAGS_SIZE),    ParseContactFlag },

    { "}",                            TOK_END,            0},
    SCRIPTVARS_STD_PARSE(ContactDef)
    { "", 0, 0 }
};

TokenizerParseInfo ParseContactDefList[] = {
    { "ContactDef",        TOK_STRUCT(ContactList,contactdefs,ParseContactDef) },
    { "", 0, 0 }
};

SHARED_MEMORY ContactList g_contactlist = {0};
Contact* g_contacts;

static void ContactSetupAccessibleContactLists(bool shared_memory);

static int s_getDesignerTipIndex(const char *tokenType)
{
    int i;
    for (i = 0; i < eaSize(&g_DesignerContactTipTypes.designerTips); ++i)
    {
        if (stricmp(g_DesignerContactTipTypes.designerTips[i]->tokenTypeStr, tokenType) == 0)
        {
            return i+TIPTYPE_STANDARD_COUNT+1;
        }
    }
    return -1;
}
// cycle through all story arcs referred to by contacts
static void ContactAttachStoryArcs(ContactDef* def)
{
    int i, iSize = eaSize(&def->storyarcrefs);
    for (i = 0; i < iSize; i++)
    {
        StoryArcRef* ref = cpp_const_cast(StoryArcRef*)(def->storyarcrefs[i]);
        ref->sahandle = StoryArcGetHandle(ref->filename);
        if (!ref->sahandle.context)
            ErrorFilenamef(def->filename, "Invalid story arc specified in contact file (%s)", ref->filename);
    }
}

static int ContactValidate(ContactDef* def, char ** outputString)
{
#define REQUIRE(str, err) \
    if (!(str)) \
    { ErrorFilenamef(def->filename, err); ret = 0; \
    if(outputString){estrConcatf(outputString, "%s\n", err);}}
#define REQUIRE2(str, err, param) \
    if (!(str)) \
    { ErrorFilenamef(def->filename, err, param); ret = 0; \
    if(outputString){estrConcatf(outputString, err"\n", param);}}

    int taskforce = CONTACT_IS_TASKFORCE(def);
    int taskCursor;
    int ret = 1;

    if (def->stature == NO_MANS_CONTACTLEVEL)
    {
        REQUIRE( CONTACT_IS_SPECIAL(def) || (CONTACT_IS_NOFRIENDINTRO(def) && CONTACT_IS_NOCOMPLETEINTRO(def) ), 
            "Contact has 0 stature level but allows introductions\n");
    }
    REQUIRE(def->statureSet,    "Contact must have a valid stature set\n");

    // Verify CP
    if(def->heistCP && (def->friendCP || def->confidantCP || def->completeCP))
    { ErrorFilenamef(def->filename, "If a contact offers heists(HeistCP > 0), Friend/Confidant/CompleteCP must all be 0\n"); ret = 0; }

    // Verify MetaContacts
    if (CONTACT_IS_METACONTACT(def) && !eaSize(&def->subContacts) && !eaSize(&def->subContactsDisplay))
        REQUIRE(NULL, "Meta Contacts must have SubContacts!\n");
    if (eaSize(&def->subContacts) != eaSize(&def->subContactsDisplay))
        REQUIRE(NULL, "Contact must have same number of SubContact and SubContactDisplays\n");

    // Verify Badge Required string exists if needed
    // Also make sure the badge exists
    if (def->requiredBadge)
    {
        REQUIRE(badge_GetBadgeByName(def->requiredBadge), "Required Badge does not exist\n");
        REQUIRE(def->badgeNeededString,    "Contact requires a badge, but has no badge needed string\n");
        saUtilVerifyContactParams(def->badgeNeededString, def->filename, 1, taskforce, 1, 0, 0, 0);
        saUtilVerifyContactParams(def->badgeFirstTick, def->filename, 1, taskforce, 1, 0, 0, 0);
        saUtilVerifyContactParams(def->badgeSecondTick, def->filename, 1, taskforce, 1, 0, 0, 0);
        saUtilVerifyContactParams(def->badgeThirdTick, def->filename, 1, taskforce, 1, 0, 0, 0);
        saUtilVerifyContactParams(def->badgeFourthTick, def->filename, 1, taskforce, 1, 0, 0, 0);
    }

    if (def->interactionRequires)
    {
        chareval_Validate(def->interactionRequires, def->filename);
        REQUIRE(def->interactionRequiresFailString, "Contact has a ContactRequires statement but no ContactRequiresText\n");
    }

    if (def->taskForceRewardRequires)
    {
        chareval_Validate(def->taskForceRewardRequires, def->filename);
    }

    // verify that correct contact strings exist
    if (!CONTACT_IS_SPECIAL(def) ) // strings aren't required for trainers
    {
        REQUIRE(def->displayname,    "Contact has no display name\n");
        REQUIRE(def->profession,    "Contact has no profession string\n");
        REQUIRE(def->description,    "Contact has no description string\n");
        REQUIRE(def->hello,            "Contact has no hello string\n");
        REQUIRE(def->dontknow,        "Contact has no \"don't know\" string\n");
        REQUIRE(def->locationName,    "Contact has no location string\n");

        // task force error message strings
        if (taskforce)
        {
            if (def->requiredToken)
                REQUIRE(def->dontHaveToken,    "Task Force contact has no DontHaveToken string\n");
            REQUIRE(def->levelTooLow,    "Task Force contact has no LevelTooLow string\n");
            REQUIRE(def->levelTooHigh,    "Task Force contact has no LevelTooHigh string\n");
            REQUIRE(def->needLargeTeam,    "Task Force contact has no NeedLargeTeam string\n");
            REQUIRE(def->needTeamLeader,"Task Force contact has no needTeamLeader string\n");
            REQUIRE(def->needTeamOnMap,    "Task Force contact has no needTeamOnMap string\n");
            REQUIRE(def->badTeamLevel,    "Task Force contact has no badTeamLevel string\n");
            REQUIRE(def->taskForceInvite,"Task Force contact has no taskForceInvite string\n");
        }
        else // introduction and relationship strings are not required for task force contacts
        {
            if (!CONTACT_IS_TUTORIAL(def))
            {
                REQUIRE(def->relToFriend,    "Contact has no relToFriend string\n");
                REQUIRE(def->relToConfidant,"Contact has no relToConfidant string\n");
            }
            REQUIRE(def->firstVisit,        "Contact has no firstVisit string\n");
            REQUIRE(def->introduction,        "Contact has no introduction string\n");
            REQUIRE(def->introductionAccept,"Contact has no introductionAccept string\n");
            REQUIRE(def->introductionSendoff,"Contact has no introductionSendoff string\n");
            REQUIRE(def->introduceOneContact,"Contact has no introduceOneContact string\n");
            REQUIRE(def->introduceTwoContacts,"Contact has no introduceTwoContacts string\n");
            REQUIRE(def->failsafeString,    "Contact has no failsafeString string\n");
        }
    }
    // If contact is a pvp contact, enforce that an alliance is specified
    if (CONTACT_IS_PVPCONTACT(def) && def->alliance == ALLIANCE_NEUTRAL)
        REQUIRE(NULL,    "PvPContact: ContactAlliance cannot be neutral\n");
        
    REQUIRE((def->minutesExpire>=0),    "Contact expires in negative time\n");

    REQUIRE(def->alliance != ALLIANCE_UNSPECIFIED,    "Contact alliance unspecified\n");

    // verify that contact strings use correct parameters
    saUtilVerifyContactParams(def->displayname, def->filename, 0, 0, 0, 0, 0, 0);
    saUtilVerifyContactParams(def->profession, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->description, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->hello, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->dontknow, def->filename, 1, taskforce, 1, 0, 1, 0);
    saUtilVerifyContactParams(def->firstVisit, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->relToFriend, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->relToConfidant, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->introduction, def->filename, 1, taskforce, 1, 1, 0, 0);
    saUtilVerifyContactParams(def->introductionAccept, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->introductionSendoff, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->introduceOneContact, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->introduceTwoContacts, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->failsafeString, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->noTasksRemaining, def->filename, 1, taskforce, 1, 0, 1, 0);
    saUtilVerifyContactParams(def->zeroRelPoints, def->filename, 1, taskforce, 1, 0, 0, 0);
    saUtilVerifyContactParams(def->neverIssuedTask, def->filename, 1, taskforce, 1, 0, 0, 0);

    // verify each task has a reward
    for(taskCursor = 0; taskCursor < eaSize(&def->mytasks); taskCursor++)
    {
        StoryTask* task = cpp_const_cast(StoryTask*)(def->mytasks[taskCursor]);

        // ignore task if it is deprecated
        if (task->deprecated) continue;

        REQUIRE2(eaSize(&task->returnSuccess), "Contact has no ReturnSuccess reward for task %s\n", task->logicalname);
        if (!TaskPreprocess(task, def->filename, taskforce, 0))    ret = 0;
    }

    // verify task force storyarc/task restrictions
    if (taskforce)
    {
        // task force contacts shouldn't have any regular tasks
        REQUIRE(!eaSize(&def->tasks), "Task force contact has regular tasks\n");
        REQUIRE(!eaSize(&def->taskIncludes), "Task force contact is using a task include statement\n");

        // task force must have exactly one story arc
        REQUIRE(eaSize(&def->storyarcrefs) == 1, "Task force contact doesn't have exactly one story arc\n");
        if (eaSize(&def->storyarcrefs) == 1)
        {
            // the story arc must start with an episode with only a long task
            StoryArcRef* ref = cpp_const_cast(StoryArcRef*)(def->storyarcrefs[0]);
            StoryTaskHandle sahandle = StoryArcGetHandle(ref->filename);
            const StoryArc* sa = StoryArcDefinition(&sahandle);

            REQUIRE(sa, "Task force contact has invalid story arc\n");
            if(sa && sa->episodes) 
                REQUIRE2(eaSize(&sa->episodes), "Task force contact has empty story arc (%s)\n", sa->filename);
            if (sa && eaSize(&sa->episodes))
            {
                StoryEpisode* ep1 = sa->episodes[0];
                int i, n, count = 0; // count number of non-deprecated tasks
                int bad = 0; // task was wrong length

                n = eaSize(&ep1->tasks);
                for (i = 0; i < n; i++)
                {
                    if (!(ep1->tasks[i]->flags & TASK_LONG))
                        bad = 1;
                    if (ep1->tasks[i]->deprecated)
                        continue;
                    count++;
                }
                REQUIRE2(!bad && count == 1,
                    "Task force storyarc doesn't have exactly one long task in first episode (%s)\n", sa->filename);
            }
        }
    }

    // verify contact stores
    if(def->stores.ppchStores && store_AreStoresLoaded())
    {
        int i;

        def->stores.iCnt = eaSize(&def->stores.ppchStores);

        for(i=0; i<def->stores.iCnt; i++)
        {
            REQUIRE2(store_Find(def->stores.ppchStores[i]), 
                "Contact refers to unknown store (%s).\n", def->stores.ppchStores[i]);
        }
    }

    REQUIRE(eaSize(&def->dismissReward) <= 1, "Contact has more than 1 dismissReward\n");

    rewardVerifyErrorInfo(def->displayname, def->filename);

    if(def->dismissReward)
    {
        // fpe merge comment 7/9/11: they had the following line, need to keep this?
        //VerifyRewardBlock(def->dismissReward[0], "ContactDismiss", def->filename, 1, 0, 0);
        rewardVerifyErrorInfo(def->displayname, def->filename);
        if (!rewardSetsVerifyAndProcess(cpp_const_cast(RewardSet**)(def->dismissReward[0]->rewardSets))) ret = 0;
        if (def->dismissReward[0]->rewardDialog) saUtilVerifyContactParams(def->dismissReward[0]->rewardDialog, def->filename, 1, 1, 0, 0, 0, 0);
        REQUIRE(eaSize(&def->dismissReward[0]->addclues) == 0, "Contact tries to add normal clues when dismissed\n");
    }

    if(def->dialogDefs)
    {
        DialogDefs_Verify(def->filename, def->dialogDefs);
    }

    return ret;
#undef REQUIRE
#undef REQUIRE2
}

static int ContactPreprocess(ContactDef* def)
{
    saUtilPreprocessScripts(ParseContactDef, def);
    return true;
}

bool ContactPreprocessAll(ParseTable* tpi, void* structptr)
{
    ContactList* clist = (ContactList*)structptr;
    int contactCursor;
    bool ret = true;

    for (contactCursor = 0; contactCursor < eaSize(&clist->contactdefs); contactCursor++)
        ret &= ContactPreprocess(cpp_const_cast(ContactDef*)(clist->contactdefs[contactCursor]));
    return ret;
}

// allocate space in tasks array for however many we are going to need
static int ContactGetTaskSpace(ContactDef* def)
{
    int i, j, iSize = eaSize(&def->taskIncludes);
    int ret = 1;
    int numtasks = eaSize(&def->mytasks);
    for (i = 0; i < iSize; i++)
    {
        const StoryTaskSet* set = TaskSetFind(def->taskIncludes[i]->filename);
        if (!set)
        {
            ErrorFilenamef(def->filename, "Invalid TaskInclude statement: %s", def->taskIncludes[i]->filename);
            ret = 0;
        }
        else
        {
            int taskCount = eaSize(&set->tasks);
            numtasks += taskCount;

            // need to check for Flashback MissionArcs
            for (j = 0; j < taskCount; j++)
            {
                if (TaskDefIsFlashback(set->tasks[j]))
                {
                    // need to make space in the storyarc ref array
                    StoryArcRef* ref = StructAllocRaw(sizeof(StoryArcRef));

                    ref->filename = StructAllocString(set->tasks[j]->logicalname);
                    eaPushConst(&def->storyarcrefs, ref);
                }
            }
        }
    }
    eaCreateConst(&def->tasks);
    eaSetSizeConst(&def->tasks, numtasks);
    return ret;
}

// for every contact...
bool ContactGetTaskSpaceForAll(ParseTable* tpi, void* structptr)
{
    ContactList* clist = (ContactList*)structptr;
    int i, n = eaSize(&clist->contactdefs);
    bool ret = true;
    for (i = 0; i < n; i++)
        ret &= ContactGetTaskSpace(cpp_const_cast(ContactDef*)(clist->contactdefs[i]));
    return ret;
}

// set up references to task sets to our task array (mytasks + taskIncludes)
int ContactAttachTaskSets(ContactDef* def)
{
    int i, iSize;
    int ret = 1;
    int pos = 0;

    iSize = eaSize(&def->mytasks);
    for (i = 0; i < iSize; i++)
        eaSetConst(&def->tasks, def->mytasks[i], pos++);

    iSize = eaSize(&def->taskIncludes);
    for (i = 0; i < iSize; i++)
    {
        const StoryTaskSet* set = TaskSetFind(def->taskIncludes[i]->filename);
        if (set) // !set should be caught in ContactGetTaskSpace
        {
            int t, iSizeTasks = eaSize(&set->tasks);
            for (t = 0; t < iSizeTasks; t++)
                eaSetConst(&def->tasks, set->tasks[t], pos++);
        }
    }
    return ret;
}

// Reattachs the contacts tasksets, done when the taskset has changed
void ContactReattachTaskSets(StoryTaskSet* taskset)
{
    int i, n = eaSize(&g_contactlist.contactdefs);
    for (i = 0; i < n; i++)
    {
        ContactDef* contactdef = cpp_const_cast(ContactDef*)(g_contactlist.contactdefs[i]);
        int j, numInc = eaSize(&contactdef->taskIncludes);
        int found = 0;
        for (j = 0; j < numInc; j++)
        {
            if (!stricmp(contactdef->taskIncludes[j]->filename, taskset->filename))
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            ContactGetTaskSpace(contactdef);
            ContactAttachTaskSets(contactdef);
        }
    }
}

// Match brokers up with their counterpart
static void ContactAttachBrokers(ContactDef* def)
{
    if (CONTACT_IS_BROKER(def) && !def->brokerFriend)
    {
        int i, n = eaSize(&g_contactlist.contactdefs);
        for (i = 0; i < n; i++)
        {
            ContactDef* otherBroker = (ContactDef*)g_contactlist.contactdefs[i];
            if (CONTACT_IS_BROKER(otherBroker) && def->stature == otherBroker->stature && stricmp(def->filename, otherBroker->filename))
            {
                def->brokerFriend = StoryIndexToHandle(i);
                otherBroker->brokerFriend = ContactGetHandle(def->filename);
            }
        }
    }
}

static ContactDef **tempRequiresList = 0;

// Pass in 0 if you are unsure of the handle and it will be found
// Mainly a speed issue
static int ContactPostprocess(ContactDef* def, ContactHandle currHandle)
{
    int taskCursor;
    int storyarcCursor;
    int onlyStoryArc;
    int ret = 1;
    int hasMiniArc = 0;

    ContactValidateStoreItems(def);
    ContactAttachTaskSets(def);
    ContactAttachStoryArcs(def);
    ContactAttachBrokers(def);

    ContactValidate(def, NULL);

    if (def->tipTypeStr)
    {
        def->tipType = s_getDesignerTipIndex(def->tipTypeStr);
        if (def->tipType == -1)
        {
            def->tipType = StaticDefineIntLookupInt(ParseContactTipType, def->tipTypeStr);
        }
    }
    // Add the contacts to the hash
    if (def->statureSet && stricmp("None", def->statureSet) && !stashFindElementConst(g_contactlist.contactFromStature, def->statureSet, NULL))
        stashAddPointerConst(g_contactlist.contactFromStature, def->statureSet, def, false);

    if(currHandle)
        stashAddIntConst(g_contactlist.contactHandleFromName, def->filename, currHandle, false);

    if (CONTACT_IS_GRSYSTEM_TIP(def) && def->interactionRequires)
    {
        int listLength;
        int requiresIndex;
        int requiresLength;
        int wordIndex;

        listLength = eaSize(&tempRequiresList);

        for (requiresIndex = 0; requiresIndex < listLength; requiresIndex++)
        {
            const char **requiresStatement = tempRequiresList[requiresIndex]->interactionRequires;
            requiresLength = eaSize(&requiresStatement);

            if (requiresLength == eaSize(&def->interactionRequires))
            {
                for (wordIndex = 0; wordIndex < requiresLength; wordIndex++)
                {
                    if (stricmp(requiresStatement[wordIndex], def->interactionRequires[wordIndex]))
                    {
                        break;
                    }
                }
                if (wordIndex == requiresLength)
                {
                    def->interactionRequiresTipIndex = requiresIndex + 1;
                    break;
                }
            }
        }

        if (requiresIndex == listLength)
        {
            if (requiresIndex >= 32)
            {
                assert(0 && "There are too many interactionRequires statements on tips, get a programmer!");
            }

            eaPush(&tempRequiresList, def);
            def->interactionRequiresTipIndex = requiresIndex + 1;
        }
    }

    // postprocess each task
    for (taskCursor = 0; taskCursor < eaSize(&def->mytasks); taskCursor++)
    {
        StoryTask* task = cpp_const_cast(StoryTask*)(def->mytasks[taskCursor]);
        TaskPostprocess(task, def->filename);    
        TaskProcessAndCheckLevels(task, def->filename, def->minPlayerLevel, def->maxPlayerLevel);
    }

    // let the storyarc do some processing with knowledge of my level
    onlyStoryArc = ContactHasOnlyStoryArcTasks(def) && !CONTACT_IS_TASKFORCE(def);
    for (storyarcCursor = 0; storyarcCursor < eaSize(&def->storyarcrefs); storyarcCursor++)
    {
        StoryArcRef* storyarcref = cpp_const_cast(StoryArcRef*)(def->storyarcrefs[storyarcCursor]);
        const StoryArc* storyarc = StoryArcDefinition(&storyarcref->sahandle);
        if (storyarc) 
            StoryArcProcessAndCheckLevels(cpp_const_cast(StoryArc*)(storyarc), def->minPlayerLevel, def->maxPlayerLevel);

        // Make sure contacts with only StoryArcs just have mini-arcs
        // Flag that the contact has a mini arc because if they have a mini arc they can also have a long arc
        // The mini arc must come first because arcs are issued in order
        if (storyarc && (storyarc->flags & STORYARC_MINI))
            hasMiniArc = 1;
        if (storyarc && onlyStoryArc && !hasMiniArc && !(storyarc->flags & STORYARC_MINI))
        {
            ErrorFilenamef(def->filename, "Contact with only Story Arcs must give Mini Arcs\n");
            ret = 0;
        }
    }
    return ret;
}

int AccessibleContactComparator(const void* lhsData, const void* rhsData)
{
    const ContactDef ** lhs = (const ContactDef **)lhsData;
    const ContactDef ** rhs = (const ContactDef **)rhsData;
    // Higher values go first in the list
    return (*rhs)->accessibleContactValue - (*lhs)->accessibleContactValue;
}

// this function is for pointer-postprocessing, and will only set up references
// If something is added to the postprocessing, make sure that you also handle it in ContactDefReloadCallback()
bool ContactPostprocessAll(ParseTable* pti, void* structptr, bool shared_memory)
{
    ContactList* clist = (ContactList*)structptr;
    bool ret = true;
    ContactDef** contactDefs = (ContactDef**)clist->contactdefs;
    int i, size = eaSize(&contactDefs);
    
    // create shared stashtables, they get filled out in ContactPostprocess
    // See svr_init.c::loadConfigFiles() for where -leveleditor does equivalent initialization!
    assert(!clist->contactFromStature);
    assert(!clist->contactHandleFromName);
    clist->contactFromStature = stashTableCreateWithStringKeys(stashOptimalSize(size), stashShared(shared_memory));
    clist->contactHandleFromName = stashTableCreateWithStringKeys(stashOptimalSize(size), stashShared(shared_memory));

    // for each contact
    for(i = 0; i < size; i++)
        ret = ret && ContactPostprocess(contactDefs[i], StoryIndexToHandle(i));

    ContactSetupAccessibleContactLists(shared_memory);

    return ret;
}

// Search for a recently reloaded contact def that still needs to be preprocessed
// Different search because the def has not yet been processed
static ContactDef* ContactDefFindUnprocessed(const char* relpath, int* handle)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_contactlist.contactdefs);
    strcpy(cleanedName, strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_contactlist.contactdefs[i]->filename)) {
            *handle = StoryIndexToHandle(i);
            return (ContactDef*)g_contactlist.contactdefs[i];

        }
    *handle = 0;
    return NULL;
}

static void ContactSetupAccessibleContactList(const ContactDef *def)
{
    if (def && def->accessibleContactValue)
    {
        ContactUniverse universeValues[ACCESSIBLE_UNIVERSE_COUNT] = {UNIVERSE_PRIMAL, UNIVERSE_PRAETORIAN};
        ContactAlliance allianceValues[ACCESSIBLE_ALLIANCE_COUNT] = {ALLIANCE_HERO, ALLIANCE_VILLAIN};
        int universeIndex, allianceIndex, levelIndex, accessibleIndex;

        for (universeIndex = 0; universeIndex < ACCESSIBLE_UNIVERSE_COUNT; universeIndex++)
        {
            if (!PlayerUniverseMatchesDefUniverse(universeValues[universeIndex], def->universe))
                continue;

            for (allianceIndex = 0; allianceIndex < ACCESSIBLE_ALLIANCE_COUNT; allianceIndex++)
            {
                if (!PlayerAllianceMatchesDefAlliance(allianceValues[allianceIndex], def->alliance))
                    continue;

                accessibleIndex = universeIndex * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT
                    + allianceIndex * ACCESSIBLE_LEVEL_COUNT
                    + MAX(0, def->minPlayerLevel - 1);

                // translate level from one-based to zero-based, maximum is inclusive
                for (levelIndex = MAX(0, def->minPlayerLevel - 1); levelIndex <= MIN(ACCESSIBLE_LEVEL_COUNT - 1, def->maxPlayerLevel - 1); levelIndex++)
                {
                    eaPushConst(&g_contactlist.accessibleContactDefLists[accessibleIndex], def);
                    accessibleIndex++;
                }
            }
        }
    }
}

static void ContactSetupAccessibleContactListReload(ContactDef *def)
{
    int accessibleIndex;

    for (accessibleIndex = 0; accessibleIndex < ACCESSIBLE_UNIVERSE_COUNT * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT; accessibleIndex++)
    {
        eaFindAndRemove(&g_contactlist.accessibleContactDefLists[accessibleIndex], def);
    }

    ContactSetupAccessibleContactList(def);

    for (accessibleIndex = 0; accessibleIndex < ACCESSIBLE_UNIVERSE_COUNT * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT; accessibleIndex++)
    {
        eaQSortConst(g_contactlist.accessibleContactDefLists[accessibleIndex], AccessibleContactComparator);
    }
}

static void ContactSetupAccessibleContactLists(bool shared_memory)
{
    int contactCursor, size = eaSize(&g_contactlist.contactdefs);
    int accessibleIndex;

    for (contactCursor = 0; contactCursor < size; contactCursor++)
    {
        ContactSetupAccessibleContactList(g_contactlist.contactdefs[contactCursor]);
    }

    for (accessibleIndex = 0; accessibleIndex < ACCESSIBLE_UNIVERSE_COUNT * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT; accessibleIndex++)
    {
        const ContactDef** contactList = g_contactlist.accessibleContactDefLists[accessibleIndex];
        eaQSortConst(contactList, AccessibleContactComparator);

        if(shared_memory)
        {
            // move to shared memory
            eaCompressConst(&g_contactlist.accessibleContactDefLists[accessibleIndex], &contactList, customSharedMalloc, NULL);
            eaDestroyConst(&contactList);
        }
    }
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void ContactDefReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseContactDefList, sizeof(ContactList), cpp_const_cast(ContactList*)(&g_contactlist), NULL, NULL))
    {
        int handle;
        ContactDef* contactdef = ContactDefFindUnprocessed(relpath, &handle);
        ContactPreprocess(contactdef);
        ContactGetTaskSpace(contactdef);
        ContactPostprocess(contactdef, handle);
        ContactSetupAccessibleContactListReload(contactdef);
        BadgeFixPreload(contactdef, handle);
        if (contactdef->statureSet && stricmp("None", contactdef->statureSet))
            stashAddPointerConst(g_contactlist.contactFromStature, contactdef->statureSet, contactdef, false);
    }
    else
    {
        Errorf("Error reloading contact: %s\n", relpath);
    }
}

static void ContactSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.contact", ContactDefReloadCallback);
}

/**
* Setup the contacts array to have a one to one mapping for
* entries in the contact definitions array.
*/
static void ContactSetupInstances()
{
    int i;

    if (g_contacts)
        free(g_contacts);

    // Resize the contacts to match the contactdefs and setup the links
    g_contacts = calloc(eaSize(&g_contactlist.contactdefs), sizeof(Contact));

    // Setup pointers to make the user code easier to port
    for (i = 0; i < eaSize(&g_contactlist.contactdefs); i++)
        g_contacts[i].def = g_contactlist.contactdefs[i];
}

void ContactPreload()
{
    int flags = PARSER_SERVERONLY;
    if (g_contactlist.contactdefs)
        flags = PARSER_FORCEREBUILD; // Re-load!

    ParserUnload(ParseContactDefList, &g_contactlist, sizeof(g_contactlist));

    if (!ParserLoadFilesShared("SM_contacts.bin", SCRIPTS_DIR, ".contact", "contacts.bin", flags, ParseContactDefList, &g_contactlist, sizeof(g_contactlist),
            NULL, NULL, ContactPreprocessAll, ContactGetTaskSpaceForAll, ContactPostprocessAll))
    {
        log_printf("contact", "Error loading contact definitions");
    }

    BadgeFixPreloadAll();
    ContactSetupCallbacks();

#ifdef SERVER
    if (!server_state.tsr)
#endif
    {
        ContactSetupInstances();
    }
}

// *********************************************************************************
//  Contacts, Contact defs and handles
// *********************************************************************************

const ContactDef* ContactDefinition(ContactHandle handle)
{
    int n = eaSize(&g_contactlist.contactdefs);
    int id = handle-1;
    if (id < 0 || id >= n) return NULL;
    return g_contactlist.contactdefs[id];
}

Contact* GetContact(ContactHandle handle)
{
    int n = eaSize(&g_contactlist.contactdefs);
    int id = handle-1;
    if (id < 0 || id >= n) return NULL;
    return &g_contacts[id];
}

void ContactValidateAllManually(char **errorString)
{
    int i;
    int n = eaSize(&g_contactlist.contactdefs);
    ContactDef* contact;

    for(i=0; i < n; i++)
    {
        contact = (ContactDef*)g_contactlist.contactdefs[i];
        ContactValidate(contact, errorString);
    }
}

void ContactGetValidTips(Entity *e, int **handlesEai, TipType type)
{
    int badIndices = 0;
    int goodIndices = 0;
    int i, j;
    int nContacts;
    int player_level = character_GetExperienceLevelExtern(e->pchar);
    char name[32];
    RewardToken *token;

    if(!devassert(!(type <= TIPTYPE_NONE || type == TIPTYPE_GRSYSTEM_MAX || type > TIPTYPE_STANDARD_COUNT+eaSize(&g_DesignerContactTipTypes.designerTips))))
        type = TIPTYPE_ALIGNMENT;
    nContacts = eaSize(&g_contactlist.contactdefs);
    eaiPopAll(handlesEai);
    for(i = 0; i < nContacts; i++)
    {
        const ContactDef *contact = g_contactlist.contactdefs[i];
        int contactHandle = StoryIndexToHandle(i);
        int streakBroken = false;
        
        if (contact->tipType != type)
            continue;
        if (contact->interactionRequiresTipIndex && (badIndices & (1 << (contact->interactionRequiresTipIndex - 1))))
            continue;
        if (contact->interactionRequiresTipIndex && !(goodIndices & (1 << (contact->interactionRequiresTipIndex - 1))))
        {
            int result = chareval_Eval(e->pchar, contact->interactionRequires, contact->filename);
            if (result)
            {
                goodIndices |= (1 << (contact->interactionRequiresTipIndex - 1));
                // don't continue
            }
            else
            {
                badIndices |= (1 << (contact->interactionRequiresTipIndex - 1));
                continue;
            }
        }
        if (!contact->interactionRequiresTipIndex && contact->interactionRequires
            && !chareval_Eval(e->pchar, contact->interactionRequires, contact->filename))
        {
            continue;
        }
        if (contact->minPlayerLevel > player_level)
            continue;
        if (contact->maxPlayerLevel < player_level)
            continue;
        // ContactCannotInteract?
        if (ContactGetInfo(e->storyInfo, contactHandle))
            continue; // don't award contacts the player already has
        if (type == TIPTYPE_ALIGNMENT)
        {
            for (j = 0; j < GRTIP_STREAKBREAKER_COUNT; j++)
            {
                if (ENT_IS_ON_RED_SIDE(e))
                {
                    sprintf(name, "%s%d", GRTIP_STREAKBREAKER_REDNAME, j);
                }
                else
                {
                    sprintf(name, "%s%d", GRTIP_STREAKBREAKER_BLUENAME, j);
                }

                // BAD CODE!  HACK!  Don't store contact handles permanently,
                // since they can change whenever a designer adds a new contact!
                // ...we just don't have access to vars.attribute on the mapserver.
                // TODO:  stop using tokens for this and make it standard database fare?
                if ((token = getRewardToken(e, name)) && token->val == contactHandle)
                {
                    streakBroken = true;
                    break;
                }
            }            
        }
        if (streakBroken)
            continue;

        eaiSortedPushUnique(handlesEai, contactHandle);
    }
}

void ContactValidateEntityAll(Entity *e, char **errorString)
{
    int i, j = 0;
    StoryContactInfo **contactsSrc = StoryInfoFromPlayer(e)->contactInfos;
    int n = eaSize(&contactsSrc);
    const ContactDef* contact;
    static StoryContactInfo **contactsDst = NULL;
    ContactGetValidList(e, &contactsDst, &contactsSrc);

    for(i=0; i < n; i++)
    {
        int foundMatch=0;
        contact=NULL;

        if(j <eaSize(&contactsDst))
        {
            contact = contactsDst[j]->contact->def;
            if(contactsSrc[i] == contactsDst[j])
            {
                foundMatch = 1;
                j++;
                if(!CONTACT_IS_AVAILABLE(e, contact))
                    estrConcatf(errorString, "Contact %s (%s) %s still ACTIVE when it should not be.\n", 
                        contact->filename, contact->interactionRequires, ContactAlignmentName(contact->alliance));
            }
        }
        if(!foundMatch)
        {
            contact = contactsSrc[i]->contact->def;
            if(CONTACT_IS_AVAILABLE(e, contact))
                estrConcatf(errorString, "Contact %s (%s) %s (IN)ACTIVE when it should not be.\n", 
                    contact->filename, contact->interactionRequires, ContactAlignmentName(contact->alliance));
        }
    }
}

const ContactDef* ContactFromStature(const char* stature)
{
    return stashFindPointerReturnPointerConst(g_contactlist.contactFromStature, stature);
}

ContactHandle ContactGetHandle(const char *filename)
{
    int handle = 0;
    char buf[MAX_PATH];
    saUtilCleanFileName(buf, filename);
    if( stashFindInt(g_contactlist.contactHandleFromName, buf, &handle))
        return handle;
    return 0;
}

// less restrictive - for debug commands
ContactHandle ContactGetHandleLoose(const char* filename)
{
    char buf[MAX_PATH], buf2[MAX_PATH];
    int i, n;

    // try to look it up exactly
    ContactHandle ret = ContactGetHandle(filename);
    if (ret) return ret;

    // otherwise, try matching the initial part of the filename
    saUtilFilePrefix(buf, filename);
    n = eaSize(&g_contactlist.contactdefs);
    for (i = 0; i < n; i++)
    {
        saUtilFilePrefix(buf2, g_contactlist.contactdefs[i]->filename);
        if (!stricmp(buf, buf2))
            return StoryIndexToHandle(i);
    }
    return 0;
}

ContactHandle ContactGetHandleFromDefinition(const ContactDef* definition)
{
    int index = eaFind(&g_contactlist.contactdefs, definition); // returns -1 if none found
    return StoryIndexToHandle(index);
}

// Chooses the right newspaper
ContactHandle ContactGetNewspaper(Entity* player)
{
    static int villainNewspaper = 0;
    static int heroNewspaper = 0;
    if (!villainNewspaper)
        villainNewspaper = ContactGetHandle("V_Contacts/Brokers/Newspaper.contact");
    if (!heroNewspaper)
        heroNewspaper = ContactGetHandle("Contacts/Police_Band/Police_Radio.contact");
    if (ENT_IS_ON_RED_SIDE(player))
        return villainNewspaper;
    return heroNewspaper;
}

const char *ContactFileName(ContactHandle handle)
{
    const ContactDef* contact = ContactDefinition(handle);
    if (!contact) return NULL;
    return contact->filename;
}

int ContactNumDefinitions()
{
    return eaSize(&g_contactlist.contactdefs);
}

// *********************************************************************************
//  Utility functions using only a handle
// *********************************************************************************

const char *ContactDisplayName(const ContactDef* contact, Entity *player)
{
    ScriptVarsTable vars = {0};

    if (!contact) 
        return NULL;

    if( player && CONTACT_IS_PLAYERMISSIONGIVER(contact) && player->taskforce && player->taskforce->architect_contact_override>=0 )
    {
        return player->taskforce->architect_contact_name;
    }

    ScriptVarsTablePushScope(&vars, &contact->vs);
    return saUtilTableLocalize(contact->displayname, &vars);
}

// Used to get the NPC number, which is used for determining the NPC
// costume.
int ContactNPCNumber(const ContactDef *cdef, Entity *player)
{
    int i;

     if( CONTACT_IS_PLAYERMISSIONGIVER(cdef) && player->taskforce && player->taskforce->architect_contact_override>=0 )
    {
            return player->taskforce->architect_contact_override;
    }

    for(i=eaSize(&g_pnpcdeflist.pnpcdefs)-1; i>=0; i--)
    {
        if(g_pnpcdeflist.pnpcdefs[i]->contact
            && stricmp(g_pnpcdeflist.pnpcdefs[i]->contact, cdef->filename)==0)
        {
            int idx;
            if(npcFindByName(g_pnpcdeflist.pnpcdefs[i]->model, &idx))
            {
                return idx;
            }
            else
            {
                return -1;
            }
        }
    }

    return -1;
}

// returns what character levels this contact is designed for
void ContactLevelRange(ContactHandle handle, int* minlevel, int* maxlevel)
{
    const ContactDef* contact = ContactDefinition(handle);
    *minlevel = 1;
    *maxlevel = MAX_PLAYER_SECURITY_LEVEL;    // just some sane defaults
    if (contact)
    {
        *minlevel = contact->minPlayerLevel;
        *maxlevel = contact->maxPlayerLevel;
    }
}

char const* ContactAlignmentName(int alliance)
{
    int i;
    for(i = 0; i < ARRAY_SIZE(ParseContactAlliance); i++)
    {
        if(ParseContactAlliance[i].value == alliance)
            return ParseContactAlliance[i].key;
    }
    return "Unknown";
}

// *********************************************************************************
//  Badge Auto-Fix system
// *********************************************************************************

// DESIGN NOTE: this system gives a badge to anyone who completed a task that
// might have rewarded the badge.  It gives the badge to them the next time they log
// in.  One consequence is that the badge must be given to players whether or not
// they successfully completed the mission.  This is in keeping with how players
// feel about the badges on the boards.  It doesn't fix badges that were given at
// the end of story arcs (no such badges exist right now).

typedef struct BadgeFixRecord 
{
    ContactHandle contacthandle;
    TaskSubHandle taskhandle;
    const char* badgename;
} BadgeFixRecord;

static BadgeFixRecord** badgefixes = 0;

static void BadgeFixAdd(ContactHandle contacthandle, TaskSubHandle taskhandle, const char* badgename)
{
    BadgeFixRecord* rec;
    int i, n;

    if (!badgefixes)
        eaCreate(&badgefixes);
    n = eaSize(&badgefixes);
    for (i = 0; i < n; i++)
    {
        if (badgefixes[i]->contacthandle == contacthandle &&
            badgefixes[i]->taskhandle == taskhandle &&
            !stricmp(badgefixes[i]->badgename, badgename))
            return;
    }
    rec = calloc(sizeof(*rec), 1);
    rec->contacthandle = contacthandle;
    rec->taskhandle = taskhandle;
    rec->badgename = badgename;
    eaPush(&badgefixes, rec);
}

void BadgeFixAddReward(ContactHandle contacthandle, TaskSubHandle taskhandle, StoryReward* reward)
{
    int i, n;

    n = eaSize(&reward->primaryReward);
    for (i = 0; i < n; i++)
    {
        if (badge_FindName(reward->primaryReward[i]))
            BadgeFixAdd(contacthandle, taskhandle, reward->primaryReward[i]);
    }
    n = eaSize(&reward->secondaryReward);
    for (i = 0; i < n; i++)
    {
        if (badge_FindName(reward->secondaryReward[i]))
            BadgeFixAdd(contacthandle, taskhandle, reward->secondaryReward[i]);
    }
}

void BadgeFixAddTask(ContactHandle contacthandle, TaskSubHandle taskhandle, StoryTask* task)
{
    int i, n;

    if (task->taskBegin)                BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskBegin[0]));
    if (task->taskBeginFlashback)        BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskBeginFlashback[0]));
    if (task->taskBeginNoFlashback)        BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskBeginNoFlashback[0]));
    if (task->taskSuccess)                BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskSuccess[0]));
    if (task->taskSuccessFlashback)        BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskSuccessFlashback[0]));
    if (task->taskSuccessNoFlashback)    BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskSuccessNoFlashback[0]));
    if (task->taskFailure)                BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskFailure[0]));
    if (task->taskAbandon)                BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->taskAbandon[0]));
    if (task->returnSuccess)            BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->returnSuccess[0]));
    if (task->returnSuccessFlashback)    BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->returnSuccessFlashback[0]));
    if (task->returnSuccessNoFlashback)    BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->returnSuccessNoFlashback[0]));
    if (task->returnFailure)            BadgeFixAddReward(contacthandle, taskhandle, cpp_const_cast(StoryReward*)(task->returnFailure[0]));
    n = eaSize(&task->children);
    for (i = 0; i < n; i++)
        BadgeFixAddTask(contacthandle, taskhandle, cpp_const_cast(StoryTask*)(task->children[i]));
}

// Setup records for badges
// contacthandle is the handle, unless 0, then we need to look it up
void BadgeFixPreload(const ContactDef* contact, ContactHandle handle)
{
    int t, numt = eaSize(&contact->tasks);
    ContactHandle realHandle = handle;
    if (!realHandle)
        realHandle = ContactGetHandle(contact->filename);
    if (contact->dismissReward)    BadgeFixAddReward(handle, 0, cpp_const_cast(StoryReward*)(contact->dismissReward[0]));
    for (t = 0; t < numt; t++)
        BadgeFixAddTask(realHandle, StoryIndexToHandle(t), cpp_const_cast(StoryTask*)(contact->tasks[t]));
}

// basically just cycle through every contact's tasks and set up records for badges
void BadgeFixPreloadAll(void)
{
    int c, numc = eaSize(&g_contactlist.contactdefs);
    for (c = 0; c < numc; c++)
        BadgeFixPreload(g_contactlist.contactdefs[c], StoryIndexToHandle(c));
}

void BadgeFixCheckPlayer(Entity* player)
{
    int i, n, b, numb;

    // can't check players who are in task force mode
    if (!player->pl || player->pl->taskforce_mode)
        return;

    // look through contacts
    numb = eaSize(&badgefixes);
    n = eaSize(&player->storyInfo->contactInfos);
    for (i = 0; i < n; i++)
    {
        StoryContactInfo* contactInfo = player->storyInfo->contactInfos[i];

        // look through all the badge fixes
        for (b = 0; b < numb; b++)
        {
            if (badgefixes[b]->contacthandle == contactInfo->handle)
            {
                // if issued, and player doesn't still have mission..
                if (BitFieldGet(contactInfo->taskIssued, TASK_BITFIELD_SIZE, badgefixes[b]->taskhandle-1) &&
                    !TaskGetInfo(player, tempStoryTaskHandle(contactInfo->handle, badgefixes[b]->taskhandle,0, TaskForceIsOnArchitect(player))) )
                {
                    badge_Award(player, badgefixes[b]->badgename, 1);
                }
            }
        }
    }
}

static const ContactDef *AccessibleContacts_getCurrentInternal(Entity *player, int index, int *countOutput)
{
    int universeIndex;
    int allianceIndex;
    int levelIndex;
    int accessibleIndex;
    int accessibleCount;

    if (!player || !player->pchar)
        return NULL;

    if (ENT_IS_LEAVING_PRAETORIA(player))
        return NULL;

    universeIndex = ENT_IS_IN_PRAETORIA(player) ? 1 : 0;
    allianceIndex = ENT_IS_VILLAIN(player) ? 1 : 0;
    levelIndex = character_GetExperienceLevelExtern(player->pchar) - 1;
    accessibleIndex = universeIndex * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT
        + allianceIndex * ACCESSIBLE_LEVEL_COUNT
        + levelIndex;
    accessibleCount = eaSize(&g_contactlist.accessibleContactDefLists[accessibleIndex]);

    if (countOutput)
        *countOutput = accessibleCount;

    if (!accessibleCount
        || index < 0 
        || index >= accessibleCount)
    {
        return NULL;
    }

    return g_contactlist.accessibleContactDefLists[accessibleIndex][index];
}

const ContactDef *AccessibleContacts_getCurrent(Entity *player, int *countOutput)
{
    return AccessibleContacts_getCurrentInternal(player, player->pl->currentAccessibleContactIndex, countOutput);
}

void AccessibleContacts_DebugOutput(Entity *player)
{
    int accessibleCount;
    const ContactDef *contactDef = AccessibleContacts_getCurrent(player, &accessibleCount);

    if (!accessibleCount)
    {
        sendInfoBox(player, INFO_SVR_COM, "Your current level/alignment/universe combination doesn't have any accessible contacts!");
        return;
    }

    sendInfoBox(player, INFO_SVR_COM, "Current Accessible Contact Index: %d", player->pl->currentAccessibleContactIndex);

    if (contactDef)
    {
        sendInfoBox(player, INFO_SVR_COM, "Current Accessible Contact Name: %s", ContactDisplayName(contactDef, player));
    }
    else
    {
        sendInfoBox(player, INFO_SVR_COM, "Current Accessible Contact is NULL.  You're probably out of bounds.");
    }
}

static bool AccessibleContacts_IsContactAcceptable(Entity *player, const ContactDef *contactDef)
{
    if (contactDef->interactionRequires && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename))
        return false;

    return true;
}

// Assumes player != NULL and player->pl != NULL, check these if you call this function.
static int AccessibleContacts_FindInternal(Entity *player, int index, int step)
{
    const ContactDef *contactDef;

    do 
    {
        // Explicitly doesn't check the initial index value!
        index += step;

        contactDef = AccessibleContacts_getCurrentInternal(player, index, NULL);
    }
    // if contactDef != NULL, then index must be in range!
    while (contactDef && !AccessibleContacts_IsContactAcceptable(player, contactDef));

    return index;
}

int AccessibleContacts_FindNext(Entity *player)
{
    if (!player || !player->pl)
        return -1;

    return AccessibleContacts_FindInternal(player, player->pl->currentAccessibleContactIndex, 1);
}

int AccessibleContacts_FindPrevious(Entity *player)
{
    if (!player || !player->pl)
        return -1;

    return AccessibleContacts_FindInternal(player, player->pl->currentAccessibleContactIndex, -1);
}

int AccessibleContacts_FindFirst(Entity *player)
{
    if (!player || !player->pl)
        return -1;

    return AccessibleContacts_FindInternal(player, -1, 1);
}

// Assumes e->pl->currentAccessibleContactIndex points at a valid contact.
// If you aren't supposed to be able to access this contact, you shouldn't be pointing to it here.
void AccessibleContacts_ShowCurrent(Entity *player)
{
    int accessibleCount;
    const ContactDef *contactDef;
    int imageNumber;
    int showPrev;
    int showTele;
    int showNext;
    StaticMapInfo *info;

    if (player->pl->currentAccessibleContactIndex == -1)
    {
        player->pl->currentAccessibleContactIndex = AccessibleContacts_FindFirst(player);
    }

    contactDef = AccessibleContacts_getCurrent(player, &accessibleCount);

    if (!contactDef)
    {
        // must be off the edge of the array...
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoContacts"));
        return;
    }

    info = staticMapInfoFind(db_state.base_map_id);

    if ((info && info->introZone) || g_MapIsTutorial)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoTutorial"));
        return;
    }

    if (db_state.is_endgame_raid)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoEndgameRaid"));
        return;
    }

    if (PlayerInTaskForceMode(player))
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoTaskforce"));
        return;
    }

    showPrev = (AccessibleContacts_FindPrevious(player) >= 0);
    showTele = !ContactGetInfoFromDefinition(player->storyInfo, contactDef); // always player's info, never TF's
    showNext = (AccessibleContacts_FindNext(player) < accessibleCount);

    if (contactDef->imageOverride)
    {
        // stuff
    }
    else
    {
        imageNumber = ContactNPCNumber(contactDef, player);
    }

    START_PACKET(pak, player, SERVER_SHOW_CONTACT_FINDER_WINDOW);
    pktSendBitsAuto(pak, imageNumber);
    pktSendString(pak, ContactDisplayName(contactDef, player));
    pktSendString(pak, saUtilScriptLocalize(contactDef->profession));
    pktSendString(pak, saUtilScriptLocalize(contactDef->locationName));
    pktSendString(pak, saUtilScriptLocalize(contactDef->description));
    pktSendBitsAuto(pak, showPrev);
    pktSendBitsAuto(pak, showTele);
    pktSendBitsAuto(pak, showNext); 
    END_PACKET
}

// Assumes e->pl->currentAccessibleContactIndex points at a valid contact.
// If you aren't supposed to be able to access this contact, you shouldn't be pointing to it here.
void AccessibleContacts_TeleportToCurrent(ClientLink *client, Entity *player)
{
    const ContactDef *contactDef = AccessibleContacts_getCurrent(player, NULL);
    StoryContactInfo *contactInfo;
    StaticMapInfo *info;

    if (!contactDef)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoContacts"));
        return;
    }

    info = staticMapInfoFind(db_state.base_map_id);

    if ((info && info->introZone) || g_MapIsTutorial)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoTutorial"));
        return;
    }

    contactInfo = ContactGetInfoFromDefinition(player->storyInfo, contactDef); // always player's info, never TF's

    if (contactInfo)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorCantTPToKnownContact"));
        return;
    }

    ContactGoto(client, player, contactDef->filename, 1);
}

// Assumes e->pl->currentAccessibleContactIndex points at a valid contact.
// If you aren't supposed to be able to access this contact, you shouldn't be pointing to it here.
void AccessibleContacts_SelectCurrent(ClientLink *client, Entity *player)
{
    const ContactDef *contactDef = AccessibleContacts_getCurrent(player, NULL);
    StoryContactInfo *contactInfo;
    StaticMapInfo *info;

    if (!contactDef)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoContacts"));
        return;
    }

    info = staticMapInfoFind(db_state.base_map_id);

    if ((info && info->introZone) || g_MapIsTutorial)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorNoTutorial"));
        return;
    }

    contactInfo = ContactGetInfoFromDefinition(player->storyInfo, contactDef); // always player's info, never TF's

    if (!contactInfo)
    {
        sendInfoBox(player, INFO_SVR_COM, textStd("ContactFinderErrorCantSelectUnknownContact"));
        return;
    }

    ContactMarkSelected(player, player->storyInfo, contactInfo);
}