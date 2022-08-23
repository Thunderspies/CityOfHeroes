/*\
 *
 *    storyarcutil.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    this file provides several utility functions for
 *  areas related to story systems
 *
 */
#include "storyarcprivate.h"
#include "pnpc.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include <utilitieslib/language/MessageStore.h>
#include "gridcoll/megaGrid.h"
#include "utils/position.h"
#include "entity/powers.h"
#include "entity/entPlayer.h"
#include "language/langServerUtil.h"
#include "group/groupnetsend.h"
#include "script/scriptengine.h"
#include "generator/encounter.h"
#include "entity/entserver.h"
#include "ai/entaiPriority.h"
#include "cmdparse/chat_emote.h" //for g_EmoteAnims
#include "ai/entaiprivate.h" //maybe move out
#include "generator/encounterPrivate.h"
#include "dbcomm/dbcomm.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "svr/svr_player.h"
#include "gameComm/svr_chat.h"
#include "entity/motion.h"
#include "ai/entai.h"
#include "gridcoll/gridcoll.h"
#include "entity/entGameActions.h"
#include "ai/entaiScript.h"
#include "beacon/beaconPath.h"
#include "entity/character_base.h"
#include "entity/origins.h"
#include "entity/teamCommon.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/language/AppLocale.h>
#include "language/commonLangUtil.h"
#include <utilitieslib/utils/tokenstore.h>
#include "ailib/aiBehaviorPublic.h"
#include "seq/AnimBitList.h"
#include "storyarc/pnpcCommon.h"
#include "entity/Supergroup.h"
#include "auth/authUserData.h"
#include <utilitieslib/utils/log.h>

#define SCRIPTS_DIR_SUBSTR SCRIPTS_DIR "/"

MultiMessageStore* g_storyarcMessages = 0;    // all story arc messages from scripts are combined here now
MultiMessageStore* g_staticMessages = 0;    // static strings that come from StoryarcStrings.ms
static char g_localize_ret[10000];        // all the localize functions for saUtil use this variable

// *********************************************************************************
//  Story system Preload/Load
// *********************************************************************************

void StoryPreload()
{
    loadstart_printf("Loading story arc text.. ");
    saUtilPreload();
    loadend_printf("");

    loadstart_printf("Loading story arcs.. ");

    // load up other subsystems
    scLoad();
    difficultyLoad();
    UniqueTaskPreload();
    MissionPreload();
    TaskSetPreload();
    StoryArcPreload();

    // other systems
    ContactPreload();
    PNPCPreload();

    loadend_printf("%i arcs, %i contacts, %i tasks", StoryArcNumDefinitions(),
        ContactNumDefinitions(), StoryArcTotalTasks() + TaskSetTotalTasks());

    // global dialog defs
    DialogDefList_Load();
}

void StoryLoad()
{
    StoryArcValidateAll();
    PNPCLoad();
}

// *********************************************************************************
//  Misc
// *********************************************************************************

void saUtilInitStoryarcMessageStore();

void saUtilPreload()
{
    saUtilInitStoryarcMessageStore();
}

// noop stubs for tree traversal
void* noopContextCreate() { return NULL; }
void noopContextCopy(void** src, void** dst) { *dst = *src; }
void noopContextDestroy(void* context) { }

// change to a relative path in the localized scripts dir
void saUtilCleanFileName(char buffer[MAX_PATH], const char* source)
{
    char* pos;

    strcpy(buffer, source);
    forwardSlashes(_strupr(buffer));
    pos = strstr(buffer, SCRIPTS_DIR_SUBSTR);
    if (pos)
    {
        pos += strlen(SCRIPTS_DIR_SUBSTR);
        memmove(buffer, pos, strlen(pos)+1);
    }
}

// verify that the filename was cleaned
bool saUtilVerifyCleanFileName(const char* source)
{
    const char *s;

    if (!source)
        return true;

    for (s = source; *s; s++)
        if (islower(*s) || *s=='\\' || (s>(source+1) && s[0]=='/' && s[-1]=='/'))
            return false;

    if (strstr(source, SCRIPTS_DIR_SUBSTR))
        return false;

    return true;
}

// change back to an absolute path
char* saUtilDirtyFileName(const char* source)
{
    static char dirtyName[MAX_PATH];
    sprintf(dirtyName, "%s%s", SCRIPTS_DIR_SUBSTR, source);
    return dirtyName;
}

// strip the directory and extension from the file
void saUtilFilePrefix(char buffer[MAX_PATH], const char* source)
{
    char* pos;

    // clean the filename
    strcpy(buffer, source);
    forwardSlashes(_strupr(buffer));

    // strip the directory
    pos = strrchr(buffer, '/');
    if (pos)
    {
        pos++;
        memmove(buffer, pos, strlen(pos)+1);
    }

    // strip the extension
    pos = strrchr(buffer, '.');
    if (pos)
        *pos = 0;
}


static const char* staticMessagesTranslate(int prepend_flags, int localeID, const char* str, va_list arg)
{
    static char* bufs[5] = {0};
    static U32 bufRotor = 0;    
    U32 bufCur = bufRotor++%ARRAY_SIZE( bufs );
    int requiredBufferSize;
    int strLength;

    if (!g_staticMessages)
        return str;
    if (!str)
        return NULL;

    requiredBufferSize = msxPrintfVA(g_staticMessages, NULL, 0, localeID, str, NULL, NULL, prepend_flags, arg);
    strLength = (int)strlen(str);
    requiredBufferSize = MAX(requiredBufferSize, strLength);
    estrSetLength( &bufs[bufCur], requiredBufferSize );

    if(-1 == msxPrintfVA(g_staticMessages, bufs[bufCur], requiredBufferSize + 1, localeID, str, NULL, NULL, prepend_flags, arg))
    {
        vsprintf_s( bufs[bufCur], requiredBufferSize, str, arg );
    }

    return bufs[bufCur];
}

// use the static string message store to localize the given string
const char* saUtilLocalize(const Entity *e, const char *string, ...)
{
    const char * msg;
    va_list    arg;
    int locale = getCurrentLocale();
    int prepend = 0;

    if( e )
    {
        if( ENT_IS_VILLAIN(e) )
        {
            if( ENT_IS_IN_PRAETORIA(e) )
                prepend |= TRANSLATE_PREPEND_L_P;
        }
        else
        {
            if( ENT_IS_IN_PRAETORIA(e) )
                prepend |= TRANSLATE_PREPEND_P;
            else
                prepend = TRANSLATE_NOPREPEND;
        }
        locale = e->playerLocale;
    }

    va_start(arg, string);
    msg = staticMessagesTranslate( prepend, locale, string, arg );
    va_end(arg);

    return msg;
}

// returns whether this var name is ok
int ExcludeBadVarNames(char* varname)
{
    // we don't want var names to overlap with rank or group enums
    if (StaticDefineIntGetInt(ParseVillainRankEnum, varname) != -1)
        return 0;
    if (StaticDefineIntGetInt(ParseVillainGroupEnum, varname) != -1)
        return 0;
    return 1;
}

// This function will process all filenames through saUtilCleanFileName and will
// check vars syntax (must use SCRIPTVARS_STD_FIELD)
bool saUtilPreprocessScripts(TokenizerParseInfo pti[], void* structptr)
{
    static char lastfilename[MAX_PATH] = "Unknown";    // HACK - assume we are looking at the file that was last used for TOK_CURRENTFILE
    int i;

    // look through token definitions for filenames and vars
    FORALL_PARSEINFO(pti, i)
    {
        // recurse through lower structures
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X)
        {
            int n, numelems = TokenStoreGetNumElems(pti, i, structptr);
            for (n = 0; n < numelems; n++)
            {
                void* substruct = TokenStoreGetPointer(pti, i, structptr, n);
                if (substruct)
                    saUtilPreprocessScripts(pti[i].subtable, substruct);
            }
        }
        else if (TOK_GET_TYPE(pti[i].type) == TOK_CURRENTFILE_X)
        {
            char* filename = TokenStoreGetString(pti, i, structptr, 0);
            if (filename)
            {
                saUtilCleanFileName(filename, filename);
                strcpy_s(SAFESTR(lastfilename), filename);
            }
        }
        else if (TOK_GET_TYPE(pti[i].type) == TOK_FILENAME_X)
        {
            int n, numelems = TokenStoreGetNumElems(pti, i, structptr);
            for (n = 0; n < numelems; n++)
            {
                char* filename = TokenStoreGetString(pti, i, structptr, n);
                if (filename)
                {
                    saUtilCleanFileName(filename, filename);
                }
            }
        }
        // check vars syntax
        else if (TOK_GET_TYPE(pti[i].type) == TOK_UNPARSED_X && !stricmp(pti[i].name,"vargroup"))
        {
            ScriptVarsScope* scope = (ScriptVarsScope*)TokenStoreGetEArray(pti, i, structptr);
            ScriptVarsVerifySyntax(scope, ExcludeBadVarNames, g_storyarcMessages, lastfilename);
        }
    }
    return true;
}

void saUtilVerifyContactParams(const char* string, const char* filename, int allowGeneral, int allowSuperGroupName, int allowPlayerInfo, int allowAccept, int allowLastContactName, int allowRescuer)
{
    const char* localizedstring;

    if (!string) return; // ok
    localizedstring = msxGetUnformattedMessage(g_storyarcMessages, SAFESTR(g_localize_ret), getCurrentLocale(), string);
    if (!allowGeneral)
    {
        if (strstr(localizedstring, "{HeroName}"))
            ErrorFilenamef(filename, "{HeroName} is an invalid auto-parameter for \"%s\"", localizedstring);
        if (strstr(localizedstring, "{ContactName}"))
            ErrorFilenamef(filename, "{ContactName} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
    if (!allowPlayerInfo)
    {
        if (strstr(localizedstring, "{HeroClass}"))
            ErrorFilenamef(filename, "{HeroClass} is an invalid auto-parameter for \"%s\"", localizedstring);
        if (strstr(localizedstring, "{HeroOrigin}"))
            ErrorFilenamef(filename, "{HeroOrigin} is an invalid auto-parameter for \"%s\"", localizedstring);
        if (strstr(localizedstring, "{HeroAdjective}"))
            ErrorFilenamef(filename, "{HeroAdjective} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
    if (!allowSuperGroupName)
    {
        if (strstr(localizedstring, "{SuperGroupName}"))
            ErrorFilenamef(filename, "{SuperGroupName} is an invalid auto-parameter for \"%s\"", localizedstring);
        if (strstr(localizedstring, "{TaskForceName}"))
            ErrorFilenamef(filename, "{TaskForceName} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
    if (!allowAccept)
    {
        if (strstr(localizedstring, "{Accept}"))
            ErrorFilenamef(filename, "{Accept} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
    if (!allowLastContactName)
    {
        if (strstr(localizedstring, "{LastContactName}"))
            ErrorFilenamef(filename, "{LastContactName} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
    if (!allowRescuer)
    {
        if (strstr(localizedstring, "{Rescuer}"))
            ErrorFilenamef(filename, "{Rescuer} is an invalid auto-parameter for \"%s\"", localizedstring);
    }
}

static void saUtilReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
     errorLogFileIsBeingReloaded(relpath);
    saUtilPreload();
}

static void saUtilSetupCallback(char* directory)
{
    char fullpath[MAX_PATH];
    if (!isDevelopmentMode())
        return;
    sprintf(fullpath, "%s/*.ms", mmsGetRealMessageFilename(directory, getCurrentLocale()));
    forwardSlashes(fullpath);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, fullpath+1, saUtilReloadCallback);
}


// builds the global story arc message store
void saUtilInitStoryarcMessageStore()
{
    static bool initted = false;

    int i;
    int locale = getCurrentLocale();
    MessageStoreLoadFlags flags = MSLOAD_SERVERONLY;

    char *staticMessageFiles[] = {
        "/texts/StoryarcStrings.ms",    "/texts/StoryarcStrings.types",
        NULL
    };

    // each directory that storyarc-related messages can live in
    char* storyarcMessageDirs[] = {         
        "/texts/Contacts",
        "/texts/V_Contacts",
        "/texts/NotorietyContacts",
        "/texts/NPCs",
        "/texts/V_NPCs",
        "/texts/spawndefs",
        "/texts/StoryArcs",
        "/texts/Alignment_Missions",
        "/texts/SupergroupContacts",
        "/texts/Player_Created",
//        "/texts/SouvenirClues", // This is no longer used - bundled in the storyarc that issues the clue
        "/texts/StrikeTeams",
        "/texts/Test",            // Testing purposes - No longer used, removed since it is causing warning
        "/texts/scriptdefs",
        "/texts/dialogdefs",
        "/texts/defs",            // used to localize names in the broker generated tasks
                                // raidserver.c and arenaserver.c load this same store for /texts/defs
        NULL
    };

    if(server_state.create_bins)
    {
        locale = -1; // load all locales
        flags |= MSLOAD_FORCEBINS;
    }

    LoadMultiMessageStore(&g_storyarcMessages,"storyarcmsg","g_storyarcMessages",locale,"v_",NULL,storyarcMessageDirs,installCustomMessageStoreHandlers,flags);
    mmsLoadLocale(g_storyarcMessages, DEFAULT_LOCALE_ID); // these are needed for some post processing while loading tasks
    LoadMultiMessageStore(&g_staticMessages,"staticmsg","g_staticMessages",locale,"v_",staticMessageFiles,NULL,installCustomMessageStoreHandlers,flags);

    if(!initted)
    {
        for(i = 0; storyarcMessageDirs[i]; ++i)
            saUtilSetupCallback(storyarcMessageDirs[i]);
        initted = true;
    }
}

// entities is optional and will receive the entity list
//extern Grid ent_player_grid;
//extern Grid ent_grid;

int saUtilPlayerWithinDistance(const Mat4 pos, F32 distance, Entity** result, int velpredict)
{
    return entWithinDistance(pos, distance, result, ENTTYPE_PLAYER, velpredict);
}

int saUtilVillainWithinDistance(const Mat4 pos, F32 distance, Entity** result, int velpredict)
{
    return entWithinDistance(pos, distance, result, ENTTYPE_CRITTER, velpredict);
}

//Includes offset and calculates a position in front of the entity if that's what you ask for
void getLocationFromEntity( Entity * e, int inFrontOf, Mat4 result, F32 inFrontOffset )
{
    Vec3 mid;
    EntityCapsule cap;
    Mat4 collMat;
    Mat4 collCenter;  //The center of the collision facing forward

    //Figure out where the collision capsule is
    cap = e->motion->capsule; 
    positionCapsule( ENTMAT(e), &cap, collMat ); 

    //aiEntSendMatrix(collMat, "CollMat", 0); 

    //Use that to find the collision's midpoint
    copyMat3( ENTMAT(e), collCenter );
    mid[0] = 0;
    mid[1] = cap.length / 2.0;
    mid[2] = 0;
    mulVecMat4( mid, collMat, collCenter[3] );

    copyMat4( collCenter, result ); 

    //aiEntSendMatrix(collCenter, "CollCenter", 0); 

    //If you are looking for a spot in front of this entity, find that
    if( inFrontOf )
    {
        Vec3 look;

        look[0]=0;
        look[1]=0;
        if( cap.dir == 2 )  
            look[2] = cap.length/2 + cap.radius ;
        else
            look[2] = cap.radius;

        look[2] += inFrontOffset; //This is typically the radius of the entity requesting a location to go to

        mulVecMat4( look, collCenter, result[3] );
    }

    //Weirdness to try to put this location on the ground
    result[3][1] = ENTPOSY(e);
    {
        CollInfo coll;
        Vec3 high;
        Vec3 low;
        copyVec3( result[3], high );
        high[1] += cap.length; //random amount, maybe also plus radius *2 ?
        copyVec3( result[3], low );
        low[1] -= cap.radius; //also totally random
        if( collGrid(NULL, high, low, &coll, 0, COLL_NOTSELECTABLE | COLL_DISTFROMSTART | COLL_BOTHSIDES) )
            copyVec3( coll.mat[3], result[3] );
        else
            ;//to do something is wrong
    }

}

#define LOCATION_WAS_NOT_FOUND    0
#define LOCATION_WAS_ENTITY        1
#define LOCATION_WAS_NOT_ENTITY    2

int getLocation( Entity * e, char * loc, Mat4 result, int inFrontOf )
{
    int success = LOCATION_WAS_NOT_FOUND;

    if( !loc )
        return LOCATION_WAS_NOT_FOUND;

    if( !success && e && 0==stricmp( loc, "objective") || 0==stricmp( loc, "o") ) 
    {
        AIVars* ai; 
        Entity * objective = 0;

        ai = ENTAI_BY_ID(e->owner);

        objective = erGetEnt(e->objective); //first look for testing tool to make yousay person the target

        if( !objective && ai && ai->attackTarget.entity ) //Then look for your attack target
            objective = ai->attackTarget.entity; //(Martin says this is always valid or null

        if( objective )
        {
            success = LOCATION_WAS_ENTITY;
            getLocationFromEntity( objective, inFrontOf, result, e->seq->type->capsuleSize[0] ); //TO DO get capsule on this
        }
    }
    if( !success && e && 0==stricmp( loc, "audience") || 0==stricmp( loc, "a") )  //Your target   
    {
        AIVars* ai; 
        Entity * audience = 0;

        ai = ENTAI_BY_ID(e->owner);

        audience = erGetEnt(e->audience); //first look for testing tool to make yousay person the target

        if( !audience && ai && ai->attackTarget.entity ) //Then look for your attack target, a nice generic thing 
            audience = ai->attackTarget.entity; //(Martin says this is always valid or null

        if( !audience ) //then look to see if somebody saved you
            audience = EncounterWhoSavedMe(e); 

        if( audience )
        {
            success = LOCATION_WAS_ENTITY;
            getLocationFromEntity( audience, inFrontOf, result, e->seq->type->capsuleSize[0]/2.0); 
        }
    }
    //Face the camera
    if( !success && 0==stricmp( loc, "camera") ) 
    {
        if( server_state.viewCutScene )
        {
            success = LOCATION_WAS_NOT_ENTITY;
            copyMat4( server_state.cutSceneCameraMat, result );
        }
    }

    if( !success && e ) //If you are in a cut scene, maybe it's a cut scene actor
    {
        Entity * cutSceneActor;
        
        cutSceneActor = getActorFromCutScene( e->cutSceneHandle, loc );

        if( cutSceneActor )
        {
            getLocationFromEntity( cutSceneActor, inFrontOf, result, e->seq->type->capsuleSize[0]/2.0 ); 
            success = LOCATION_WAS_ENTITY;
        }
    }
    if( !success && e ) //Ok, so maybe you are in an encounter, and you're looking for an encounter bud?
    {
        if( e->encounterInfo && e->encounterInfo->parentGroup )
        {
            Entity * encounterActor;
            EncounterGroup * group = 0;

            group = e->encounterInfo->parentGroup;
            encounterActor =  EncounterActorFromEncounterActorName( group, loc );

            if( encounterActor )
            {
                getLocationFromEntity( encounterActor, inFrontOf, result, e->seq->type->capsuleSize[0]/2.0 ); 
                success = LOCATION_WAS_ENTITY;
            }
        }
    }
    if( !success ) //are you refering to a script marker? (Don't need an entity for that)
    {
        char faceScript[200]; //TO DO clean

        if( strchr( loc, ':') ) //if this is in the long format, use it as is
            strncpy( faceScript, loc, 200 );
        else
            sprintf( faceScript, "marker:%s", loc );

        if( GetMatFromLocation( faceScript, result, 0 ) )
            success = LOCATION_WAS_NOT_ENTITY;
    }
    //Do you mean an encounter position?
    if( !success && e )
    {
        int encounterPos = 0;

        if( 0==strnicmp( loc, "e", 1) )
            encounterPos = atoi( loc+1 );
        else if( 0==strnicmp( loc, "ep", 2) )
            encounterPos = atoi( loc+2 );
        else
            encounterPos = atoi( loc );

        if( encounterPos > 0 && encounterPos < EG_MAX_ENCOUNTER_POSITIONS )  //Your encounter's encounter position   
        {
            if( e->encounterInfo && e->encounterInfo->parentGroup ) 
            {
                int pos = atoi( loc+1 );
                if( EncounterMatFromPos( e->encounterInfo->parentGroup,  pos, result) )
                    success = LOCATION_WAS_NOT_ENTITY;
            }
        }
    }


    //a null Vec is certain failure, don't go running off into the void
    if (nearSameVec3( zerovec3, result[3] ) )
        success = LOCATION_WAS_NOT_FOUND;

    //aiEntSendAddText(result[3], "Location!", 0);
    return success;
}


//MW not sure where this should live
void turnToFace( Entity * e, char * face )
{
    AIVars* ai = ENTAI(e);
    Mat4 result;
    int getLocationResult;

    getLocationResult = getLocation( e, face, result, 0 );

    if( getLocationResult != LOCATION_WAS_NOT_FOUND ) 
    {
        Vec3 dir;
        subVec3( result[3], ENTPOS(e), dir ); 
        dir[1]=0;
        normalVec3(dir);
        aiTurnBodyByDirection(e, ai, dir);
    }
}


//say pending chat
#define USE_FANCY_NPC_DIALOG 1
#define TIME_FOR_ANIM_TO_SETTLE_AFTER_MOVE 1.0 //Wait one second after arriving at a target to say anything else, so you can do emotes
void handleFutureChatQueue( Entity * e )
{
    int i;
    char * say;

    if( !USE_FANCY_NPC_DIALOG ) 
        return;

    ///////// Handle the goto tag
    //This if means I set a movement, but it is now done and I'm back to chat mode, so pick up where we left off
    if( e->IAmMovingToMyChatTarget && aiBehaviorCurMatchesName(e, ENTAI(e)->base, "Chat") ) 
    {
        e->IAmMovingToMyChatTarget = 0;
        e->queuedChatTimeStop = ABS_TIME + SEC_TO_ABS( TIME_FOR_ANIM_TO_SETTLE_AFTER_MOVE );
        if( e->queuedChatCount ) //Takes advantage to the fact that you really only get one queued chat, and also breaks it
        {
            e->queuedChat[0].sayCondition = SAYAFTERTIMEELAPSES;
            e->queuedChat[0].timeToSay = ABS_TIME + SEC_TO_ABS( TIME_FOR_ANIM_TO_SETTLE_AFTER_MOVE ); 
        }

        if( e->queuedChatTurnToFaceOnArrival )
        {
            Mat4 mat;
            Vec3 pyr;
            int getLocationResult;

            getLocationResult = getLocation( 0, e->queuedChatTurnToFaceOnArrival, mat, 0 );

            //Special rule for GOTO: 
            //If you are told to GOTO an entity, turnToFaceOnArrival defaults to the actual entity; run up to in front of the target, then turn to face it
            //then if you are going to a script marker, instead of turning to face the marker, take the pyr of the marker.
            //If you are told to goto a location, (encounterPosition or scriptmarker), instead you take the orientation of the location hen you arrive
            if( getLocationResult == LOCATION_WAS_ENTITY )
            {
                getMat3PYR( mat, pyr );
                aiTurnBodyByPYR( e, ENTAI_BY_ID(e->owner), pyr);
            }
            if( getLocationResult == LOCATION_WAS_NOT_ENTITY )
            {
                turnToFace( e, e->queuedChatTurnToFaceOnArrival );
            }
            
            free( e->queuedChatTurnToFaceOnArrival );
            e->queuedChatTurnToFaceOnArrival = 0;
        }
    }

    ////// Say anything you have new to say (moving to chat target pretty much ruins for good multiple chats working at once)
    for( i = 0 ; i < e->queuedChatCount ; i++) 
    {
        if( !e->IAmMovingToMyChatTarget && e->queuedChat[i].timeToSay <= ABS_TIME ) 
        {
            int privateMsg = e->queuedChat[i].privateMsg;
            say = e->queuedChat[i].string;

            //Collapse list (do it up here because saUtilntSays can add to this list
            memmove( &e->queuedChat[i], &e->queuedChat[i+1], ( sizeof(QueuedChat) * (e->queuedChatCount-i-1)) );
            e->queuedChatCount--;
            i--;

            if (privateMsg)
            {
                sendPrivateCaptionMsg(e, say);
            }
            else
            {
                saUtilEntSays( e, ENTTYPE(e) == ENTTYPE_NPC, say);
            }

            free(say); //all things in queuedChat are alloced strings
        }
    }    

    ////// Clean up Emote Chat Mode up after the talking and moving end (This implicitly pops the Chat behavior as well.)
    if( e->queuedChatTimeStop < ABS_TIME && !e->IAmMovingToMyChatTarget )
        e->IAmInEmoteChatMode = 0;
}



//Get bubble tag and params:
//If this has any pauses for new bubbles in it, strip them out and cache them for later
//(If this string doesn't start with <bubble...>, it's the start of a string, read an
//implied <bubble time = 2>)

const char * passSpaces( const char * s )
{
    if( s )while( (s[0] == ' ' || s[0] == '\t') && s[0] )s++;
    return s;
}

const char * passSpacesAndEquals( const char * s )
{
    if( s )while( (s[0] == '=' || s[0] == ' ' || s[0] == '\t') && s[0] )s++;
    return s;
}

//lookBackMeans I should also look one past beginning of string for delimiters
int isSameToken( const char * s, const char * tok, int lookBack  )
{
    unsigned int len = (unsigned int)strlen(tok);
    if( s && len && 0 == strnicmp( s, tok, len ) && (strlen(s) >= len) )
    {
        if( s[len] == '\0' || s[len] == ' ' || s[len] == '=' || s[len] == '\t' || s[len] == '>'  )
        {
            if( !lookBack )
                return 1;
            else
            {
                const char * v = s-1;
                if(v[0] == '\0' || v[0] == ' ' || v[0] == '=' || v[0] == '\t' || v[0] == '<') 
                    return 1;
            }
        }
    }
    return 0;
}

static int findVal( const char * string, const char * name, char * val )
{
    const char * s;
    int foundName = 0;

    
    if( !string || !name )
        return 0;

    //Search through the string for this tag
    s = string;
    while(1)
    {
        s = strstriConst( s, name );
        if(!s)
            break;
        if( isSameToken( s, name, s!=string ) )
        {
            foundName = 1;
            break;
        }
        s++;
    }
        

    if( foundName )
    {
        while( s[0] != '\0' && s[0] != ' ' && s[0] != '=' && s[0] != '\t') s++; //advance to end of tag name

        s = passSpacesAndEquals(s);

        if(s && s[0])
        {
            const char * end = s;
            int len;
            while( end[0] != '\0' && end[0] != ' ' && end[0] != '=' && end[0] != '\t')  //get the end of the string
                end++;

            if( val )
            {
                len = (int)(end-s);
                strncpy( val, s, len );
                val[len] = 0;
            }
        }
        return 1;
    }
    return 0;
}


//These tags mean this is a chat tag (you can leave out the bubble if you hae any of the other parameters)
int isAChatTag( const char * s ) //TO DO, just use emote and em?
{
    if( isSameToken( s, "bubble", 0)    || 
        isSameToken( s, "caption", 0)    || 
        isSameToken( s, "em", 0)        || 
        isSameToken( s, "emote", 0)        ||
        isSameToken( s, "time", 0)        ||
        isSameToken( s, "t", 0)            ||
        isSameToken( s, "duration", 0)    ||
        isSameToken( s, "d", 0)            ||
        isSameToken( s, "f", 0)            ||
        isSameToken( s, "face", 0)        || 
        isSameToken( s, "shout", 0)        ||
        isSameToken( s, "anim", 0)        ||
        isSameToken( s, "goto", 0)        ||
        isSameToken( s, "x", 0)        ||
        isSameToken( s, "y", 0)    
        )         
        return 1;
    return 0;
}

typedef enum
{
    DIALOG_FACE_LISTENER    = 1 << 0,
    DIALOG_FACE_TOPIC        = 1 << 1,
} DIALOG_TAGS;
 

static void parseBubble( const char * tag, F32 *durationPtr, F32 *position )
{
    char durationStr[100];
    char xStr[100];
    char yStr[100];

    durationStr[0] = 0;
    xStr[0] = 0;
    yStr[0] = 0;

    if( !findVal( tag, "duration", durationStr ) )
        findVal( tag, "d", durationStr );

    if( durationStr[0] )
        *durationPtr = atof(durationStr);

    if ( position )
    {
        findVal( tag, "x", xStr );

        if( xStr[0] )
            position[0] = atof(xStr);

        findVal( tag, "y", yStr );

        if( yStr[0] )
            position[1] = atof(yStr);
    }
}


static void parseDialog( const char * tag, F32 * timePtr, F32 *durationPtr, char * animation, char * face,
                        int * shout, int *isCaption, F32 * position, char * gotoLoc, char *animList)
{
    char timeStr[100];
    timeStr[0] = 0;

    parseBubble(tag, durationPtr, position);

    if( !findVal( tag, "emote", animation ) )
        findVal( tag, "em", animation );
        
    if( !findVal( tag, "time", timeStr ) )
        findVal( tag, "t", timeStr );

    if( timeStr[0] )
        *timePtr = atof(timeStr);

    if( !findVal( tag, "face", face ) )
        findVal( tag, "f", face );

    if( findVal( tag, "shout", 0 ) )
        *shout = 1;
    else
        *shout = 0;

    *isCaption = findVal(tag, "caption", 0);

    findVal( tag, "goto", gotoLoc );

    findVal( tag, "anim", animList );
}

// you must free dialogToSayNowPtr and dialogToSayLaterPtr if they are both non-NULL
// if dialogToSayNowPtr is not NULL but dialogToSayLaterPtr is NULL, you must NOT free either!
int parseDialogForBubbleTags( const char * dialog, char * animation, const char ** dialogToSayNowPtr, const char ** dialogToSayLaterPtr, F32 * timePtr, 
     F32 *durationPtr, char * face, int * shout, int *isCaption, F32 *captionPosition, char * gotoLoc, char *animList)
{
    //Get defaults if no opening tag is found
    const char * dialogToSayNow    = dialog;
    char * dialogToSayLater = 0;
    int dialogStartsWithTag = 0;

    animation[0]            = 0; 
    face[0]                    = 0;
    *timePtr                = DEFAULT_BUBBLE_TIME;
    *durationPtr            = DEFAULT_BUBBLE_DURATION;
    *shout                    = 0;
    gotoLoc[0]                = 0; 
    animList[0]                = 0; 

    /////////////First parse out opening tag, if any
    {
        const char * s; 
        s = dialog;
        s = passSpaces(s);
        if( s[0] == '<')
        {
            s++;
            s = passSpaces(s);
            if( isAChatTag( s ) ) 
            {
                //find the end, parse it, and get dialog to say now
                char * end;
                end = strchr( s, '>' );
                if( end )
                {
                    char tag[255];
                    int len = (int)(end-s);
                    strncpy( tag, s, len );
                    tag[len] = 0;
                    parseDialog( tag, timePtr, durationPtr, animation, face, shout, isCaption, captionPosition, gotoLoc, animList );
                    dialogStartsWithTag = 1; //return value
                    dialogToSayNow = end + 1;
                    if( dialogToSayNow[0] == 0 )
                        dialogToSayNow = 0;
                }
                else
                {
                    //MALFORMED TAG, no closing ">"
                    dialogToSayNow = 0;
                }
            }
        }
    }

    ////////////// Now find the next <bubble tag to be dialog to say later
    if( dialogToSayNow )
    {
        const char * s;
        char * later;
        
        later = strchr( dialogToSayNow, '<' );
        while(later && later[0] != '\0')
        {
            s = later+1;
            s = passSpaces(s);

            if( isAChatTag( s ) )
            {
                if( later && later[0] != 0 )
                    dialogToSayLater = later;
                break;
            }
            later = strchr( later+1, '<' );
        }
    }

    ///////////copy dialogToSayLater to it's own string to hand off to chatQueue
    if( dialogToSayLater )
    {
        int len;
        char * s;
        len = (int)strlen(dialogToSayLater);
        s = malloc( len + 1);
        strcpy( s, dialogToSayLater );
        dialogToSayLater = s; 
    }

    ///////////If needed, copy dialogToSayNow to it's own string so you can end cap it
    if( dialogToSayLater )  
    {
        int lenNow;
        char * s;
        lenNow = (int)(strlen(dialogToSayNow) - strlen(dialogToSayLater));
        s = malloc( lenNow + 1);
        strncpy( s, dialogToSayNow, lenNow );
        s[lenNow] = 0;
        dialogToSayNow = s;
    }

    //////////copy pointers to params
    *dialogToSayNowPtr = dialogToSayNow;
    *dialogToSayLaterPtr = dialogToSayLater;

    return dialogStartsWithTag;
}


// checks for players nearby entity and sends dialog to chat window
void saUtilEntSays(Entity* ent, int flag, const char* dialog)
{
    char animation[100]; //TO DO safen
    char face[100];
    const char * dialogToSayNow = 0;
    char * dialogToSayLater = 0;
    F32    time = 0;
    F32    duration = 0;
    int flags = 0;
    int shout = 0;
    int isCaption = 0;
    int dialogStartsWithTag = 0;
    char gotoLoc[100];
    int goingToLoc = 0;
    char animList[1000];
    F32 captionPosition[] = {0.5, 1.0};
    ScriptVarsTable vars = {0};

    if (!ent || !dialog || !dialog[0]) 
        return; 

    //////Parse dialog for <bubble.. 
    if( USE_FANCY_NPC_DIALOG ) //run the fancy dialog
    {
        dialogStartsWithTag = parseDialogForBubbleTags( dialog, animation, &dialogToSayNow, &dialogToSayLater, &time, &duration, face, 
        &shout, &isCaption, captionPosition, gotoLoc, animList );
    }
    else //disable
        dialogToSayNow = dialog;

    if(OnMissionMap())
        shout = 1;

    //////(If you don't start with a tag, but you have a tag embedded later, then this is emote chat, and 
    //////you have an implied empty tag at the start of this string)

    ////// Do Tag Commands : If you have a tag, go into emote chat mode, and do what the tag says
    if( dialogStartsWithTag || dialogToSayLater ) //MAX in case you have multiple queues going (you can't right now)
    {
        ent->queuedChatTimeStop = MAX( ent->queuedChatTimeStop, ABS_TIME + SEC_TO_ABS(time) );

        if(!ent->IAmInEmoteChatMode)
        {
            ent->IAmInEmoteChatMode = 1;
            aiBehaviorAddString(ent, ENTAI(ent)->base, "Chat");
        }

        ////Go to this location.  Use getting there as the time to do next bubble
        if( flag < kEntSay_PetCommand && gotoLoc[0] )
        {
            Mat4 loc;
            //Try to acquire target
            if( getLocation( ent, gotoLoc, loc, 1 ) )
            {
                static char buf[128];
                sprintf(buf, "MoveToPos(pos(%f,%f,%f)),SetNewSpawnPos(\"coord:%f,%f,%f\")", loc[3][0], loc[3][1], loc[3][2], loc[3][0], loc[3][1], loc[3][2]);
                aiBehaviorAddString(ent, ENTAI(ent)->base, buf);

                aiEntSendMatrix(loc, "Going here", 0); 
                ent->IAmMovingToMyChatTarget = 1;
                time = 0; //Dont use time (though leaving this would be fine, probably)
                goingToLoc = 1; //For 
            }
        }

        ////Turn to face right direction
        if( face[0] && !goingToLoc )
        {
            turnToFace( ent, face );
        }

        ////If goingToLoc, defer facing the right direction till you get there
        if( goingToLoc ) 
        {
            if( face[0] )
                ent->queuedChatTurnToFaceOnArrival = strdup( face );
            else
                ent->queuedChatTurnToFaceOnArrival = strdup( gotoLoc ); //Default
        }

        ////Play the animation found, if any (all flash bits)
        if( animation[0] )
        {
            int i;
            // Let's see if this is one of the animated emotes
            for(i=eaSize(&g_EmoteAnims.ppAnims)-1; i>=0; i--)
            {
                const EmoteAnim *anim = g_EmoteAnims.ppAnims[i];
                if( stricmp(anim->pchDisplayName, animation)== 0
                    && chatemote_CanDoEmote(anim,ent))
                {
                    const int *piList = anim->piModeBits;
                    int count = eaiSize(&piList);
                    if(piList!=NULL && count>0)
                    {
                        int j;
                        U32* pState = ent->seq->state;
                        for(j=count-1; j>=0; j--) {
                            SETB(pState, piList[j]);
                        }
                    }
                }
            }
        }

        if ( flag < kEntSay_PetCommand && animList[0] )
        {
            alSetAIAnimListOnEntity(ent, animList);
        }
    }
    //End Do Tag Commands

    /////// Say the chat you're supposed to say right now
    if( dialogToSayNow && dialogToSayNow[0]) //(dont say "")
    {
        int i;
        char *displayname = 0;
        Entity * parent = NULL;

        //We've decided if you get another thing to say, you dump what you were saying
        for( i = 0 ; i < ent->queuedChatCount ; i++)
            free(ent->queuedChat[i].string);
        ent->queuedChatCount = 0;

        // look up appropriate display name
        if (ent->name[0]) 
            displayname = ent->name;
        if( flag == kEntSay_PetSay )
        {
            parent = erGetEnt(ent->erOwner);
            if( ent->petName && ent->petName[0] )
                displayname = ent->petName;
        }

        if( flag == kEntSay_PetCommand )
        {
            chatSendPetSay( ent, localizedPrintf(ent,"EntitySays", displayname, dialogToSayNow) );
        }
        else
        {
            if (flag == kEntSay_NPCDirect) 
            {
                Entity * hearingEnt = erGetEnt( ent->audience );

                if( hearingEnt ) 
                {
                    if (isCaption)
                    {
                        sendChatMsgEx(hearingEnt, ent, dialogToSayNow, INFO_CAPTION, duration, 
                                      hearingEnt->db_id, hearingEnt->db_id, 0, captionPosition);
                    }
                    else
                        serveFloater(hearingEnt, ent, dialogToSayNow, 0.0f, kFloaterStyle_Chat, 0);
                }
            } else {
                //MW switched to this way to send to players, so I can do cut scene check
                for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
                {
                    const F32 * hearerPos;
                    Entity * hearingEnt = player_ents[PLAYER_ENTS_ACTIVE].ents[i];

                    if( server_state.viewCutScene )
                        hearerPos = server_state.cutSceneCameraMat[3];
                    else
                        hearerPos = ENTPOS(hearingEnt);

                    if (isCaption)
                    {
                        sendChatMsgEx(hearingEnt, ent, dialogToSayNow, INFO_CAPTION, duration, 
                                      hearingEnt->db_id, hearingEnt->db_id, 0, captionPosition);
                    }
                    else if( (shout && flag != kEntSay_PetSay) || ent_close( 100, ENTPOS(ent), hearerPos ) )
                    {
                        if( flag == kEntSay_PetSay)
                            sendChatMsgFrom(hearingEnt, ent, INFO_PET_COM, duration, localizedPrintf(hearingEnt,"PetSays", parent->name, displayname, dialogToSayNow));
                        else
                            sendChatMsgFrom(hearingEnt, ent, INFO_NPC_SAYS, duration, localizedPrintf(hearingEnt,"EntitySays", displayname, dialogToSayNow));
                    }
                }
            }
        }
    }

    ///// Add the chat you're supposed to say in the future to the queue
    if( dialogToSayLater )
    {
        QueuedChat * addedQueuedChat;
        int i;

        //We've decided if you get another thing to say, you dump what you were saying
        for( i = 0 ; i < ent->queuedChatCount ; i++)
            free(ent->queuedChat[i].string);
        ent->queuedChatCount = 0;
        //comment the above out to resume multiples.

        addedQueuedChat = dynArrayAdd(&ent->queuedChat,sizeof(ent->queuedChat[0]),&ent->queuedChatCount,&ent->queuedChatCountMax,1);
        addedQueuedChat->string = malloc(strlen(dialogToSayLater)+1);
        addedQueuedChat->privateMsg = 0;
        strcpy(addedQueuedChat->string, dialogToSayLater);

        addedQueuedChat->timeToSay = ABS_TIME + SEC_TO_ABS(time);
    }

    if( dialogToSayNow && dialogToSayLater ) //slightly ungainly, we only made a copy of the string if we needed to null terminate it
    {
        free((void*)dialogToSayNow);
        free((void*)dialogToSayLater);
    }
}



static int parseCaptionForBubbleTags( const char * caption, const char ** parsedCaptionPtr,
    F32 *durationPtr, F32* position )
{
    //Get defaults if no opening tag is found
    const char * parsedCaption        = caption;
    int captionStartsWithTag    = 0;

    *durationPtr                = DEFAULT_BUBBLE_DURATION;
    position[0]                    = 0.5;
    position[1]                    = 1.0;

    /////////////First parse out opening tag, if any
    {
        const char * s; 
        s = caption;
        s = passSpaces(s);
        if( s[0] == '<')
        {
            s++;
            s = passSpaces(s);
            if( isAChatTag( s ) ) 
            {
                //find the end, parse it, and get dialog to say now
                char * end;
                end = strchr( s, '>' );
                if( end )
                {
                    char tag[255];
                    int len = (int)(end-s);
                    strncpy( tag, s, len );
                    tag[len] = 0;
                    parseBubble( tag, durationPtr, position );
                    captionStartsWithTag = 1; //return value
                    parsedCaption = end + 1;
                    if( parsedCaption[0] == 0 )
                        parsedCaption = 0;
                }
                else
                {
                    //MALFORMED TAG, no closing ">"
                    parsedCaption = 0;
                }
            }
        }
    }

    //////////copy pointers to params
    *parsedCaptionPtr = parsedCaption;

    return captionStartsWithTag;
}


void saUtilCaption(const char* caption)
{
    const char * parsedCaption = 0;
    F32    duration = 0;
    Vec2 position = { 0.f, 0.f };
    int captionStartsWithTag = 0;
    ScriptVarsTable vars = {0};

    if (!caption || !caption[0]) return; 

    // Currently captions are only used within cutscenes.
    if (!server_state.viewCutScene) return;

    //////Parse dialog for <bubble.. 
    if( USE_FANCY_NPC_DIALOG ) //run the fancy dialog
    {
        captionStartsWithTag = parseCaptionForBubbleTags( caption, 
            &parsedCaption, &duration, position );
    }
    else //disable
        parsedCaption = caption;

    /////// Say the chat you're supposed to say right now
    if( parsedCaption && parsedCaption[0]) //(dont say "")
    {
        int i;
        char *displayname = 0;

        for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
        {
            Entity * ent = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
            sendChatMsgEx(ent, 0, parsedCaption, INFO_CAPTION, duration, ent->db_id, ent->db_id, 0, position);
        }
    }
}

// replaces all instances of <from>
void saUtilReplaceChars(char* string, char from, char to)
{
    register char* s;
    for (s = string; *s; s++)
    {
        if (*s == from) *s = to;
    }
}

static U32 sa_c = 0xf244f343;

void saSeed(unsigned int seed)
{
    sa_c = seed;
}

unsigned int saSeedRoll(unsigned int seed, unsigned int mod)
{
    sa_c = seed;
    return saRoll(mod);
}

// from rule30
unsigned int saRoll(unsigned int mod)
{
    unsigned int l,r;
    l = r = sa_c;
    l = _lrotr(l, 1);/* bit i of l equals bit just left of bit i in c */
    r = _lrotl(r, 1);/* bit i of r euqals bit just right of bit i in c */
    sa_c |= r;
    sa_c ^= l;           /* c = l xor (c or r), aka rule 30, named by wolfram */
    return sa_c % mod;
}

char* saUtilGetPowerName(PowerNameRef* power)
{
    static char buffer[1024];
    const BasePower* basePower;

    basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, power->powerCategory, power->powerSet, power->power);
    if(!basePower)
    {
        sprintf(buffer, "%s.%s.%s", power->powerCategory, power->powerSet, power->power);
        return buffer;
    }

    return saUtilGetBasePowerName(basePower);
}

char* saUtilGetBasePowerName(const BasePower* basePower)
{
    return localizedPrintf(0,basePower->pchDisplayName);
}

char *saUtilEntityVariableReplacement( const char * in_str, Entity * e )
{
    char *pCursor, *out_str, *pronoun;
    const char *pStart;

    if(!e) // can't replace varibles if we don't know the ent
        return g_localize_ret;

    pStart = in_str; 
    pCursor = strchr( in_str, '$' );

    if(!pCursor) // nothing found, quit out
        return g_localize_ret;

    estrCreate(&out_str);
    while( pCursor )
    {
        pCursor++;
        if( pCursor )
        {
            estrConcatFixedWidth(&out_str, pStart, pCursor-pStart-1);
            if( strnicmp( pCursor, "name", 4) == 0 )
            {
                estrConcatCharString(&out_str, e->name);
                pStart = pCursor = pCursor + 4;
            }
            else if( strnicmp( pCursor, "title1", 6) == 0 )
            {
                if( e->pl->titleCommon && *e->pl->titleCommon )
                    estrConcatCharString(&out_str, e->pl->titleCommon);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "title2", 6) == 0 )
            {
                if( e->pl->titleOrigin && *e->pl->titleOrigin )
                    estrConcatCharString(&out_str, e->pl->titleOrigin);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "target", 6) == 0 )
            {
                estrConcatCharString(&out_str, e->name);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "class", 5) == 0 )
            {
                char * pchClass = localizedPrintf(e, e->pchar->pclass->pchDisplayName );
                estrConcatCharString(&out_str, pchClass);
                pStart = pCursor = pCursor + 5;
            }
            else if( strnicmp( pCursor, "primary", 7) == 0 )
            {
                char * pchPrimary = NULL;
                int i;
                for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
                {
                    if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
                        pchPrimary = localizedPrintf(e, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName);
                }
                estrConcatCharString(&out_str, pchPrimary);
                pStart = pCursor = pCursor + 7;
            }
            else if( strnicmp( pCursor, "secondary", 9) == 0 )
            {
                char * pchSecondary = NULL;
                int i;
                for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
                {
                    if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
                        pchSecondary = localizedPrintf(e, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName);
                }
                estrConcatCharString(&out_str, pchSecondary);
                pStart = pCursor = pCursor + 9;
            }
            else if( strnicmp( pCursor, "archetype", 9) == 0 )
            {
                char * pchClass = localizedPrintf(e, e->pchar->pclass->pchDisplayName );
                estrConcatCharString(&out_str, pchClass);
                pStart = pCursor = pCursor + 9;
            }
            else if( strnicmp( pCursor, "origin", 6) == 0 )
            {
                char * pchOrigin = localizedPrintf(e, e->pchar->porigin->pchDisplayName );
                estrConcatCharString(&out_str, pchOrigin);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "supergroup", 10) == 0 )
            {
                char * pchSupergroup;
                if( e->supergroup && e->supergroup->name )
                    pchSupergroup = e->supergroup->name;
                else
                    pchSupergroup = localizedPrintf(e, "NoSupergroupString" );
                estrConcatCharString(&out_str, pchSupergroup);
                pStart = pCursor = pCursor + 10;
            }
            else if( strnicmp( pCursor, "sgmotto", 7) == 0 )
            {
                char * pchSupergroup;
                if( e->supergroup && e->supergroup->motto )
                    pchSupergroup = e->supergroup->motto;
                else
                    pchSupergroup = localizedPrintf(e, "NoSupergroupString" );
                estrConcatCharString(&out_str, pchSupergroup);
                pStart = pCursor = pCursor + 7;
            }
            else if( strnicmp( pCursor, "level", 5) == 0 )
            {
                estrConcatf(&out_str, "%i", e->pchar->iLevel + 1);
                pStart = pCursor = pCursor + 5;
            }
            else if( strnicmp( pCursor, "server", 6) == 0 )
            {
                estrConcatCharString(&out_str, localizedPrintf(e,db_state.server_name));
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "side", 4) == 0 )
            {
                char * side;
                if (ENT_IS_HERO(e))
                    side = localizedPrintf(e, "HeroOnlyString" );
                else
                    side = localizedPrintf(e, "VillainOnlyString" );
                estrConcatCharString(&out_str, side);
                pStart = pCursor = pCursor + 4;
            }
            else if( strnicmp( pCursor, "heshe", 5) == 0 )
            {
                if( e->gender == GENDER_FEMALE)
                    pronoun = localizedPrintf(e, "PronounSheString" );
                else
                    pronoun = localizedPrintf(e, "PronounHeString" );


                if(*pCursor == 'H')
                    *pronoun = toupper(*pronoun);

                estrConcatCharString(&out_str, pronoun);
                pStart = pCursor = pCursor + 5;
            }
            else if( strnicmp( pCursor, "himher", 6) == 0 )
            {
                if( e->gender == GENDER_FEMALE)
                    pronoun = localizedPrintf(e, "PronounHerString" ); 
                else
                    pronoun = localizedPrintf(e, "PronounHimString" );

                if(*pCursor == 'H')
                    *pronoun = toupper(*pronoun);

                estrConcatCharString(&out_str, pronoun);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "hisher", 6) == 0 )
            {
                if( e->gender == GENDER_FEMALE)
                    pronoun = localizedPrintf(e, "PronounHerString" ); 
                else
                    pronoun = localizedPrintf(e, "PronounHisString" );

                if(*pCursor == 'H')
                    *pronoun = toupper(*pronoun);

                estrConcatCharString(&out_str, pronoun);
                pStart = pCursor = pCursor + 6;
            }
            else if( strnicmp( pCursor, "sirmam", 6) == 0 )
            {
                if( e->gender == GENDER_FEMALE)
                    pronoun = localizedPrintf(e, "PronounMamString" );
                else
                    pronoun = localizedPrintf(e, "PronounSirString" ); 

                if(*pCursor == 'S')
                    *pronoun = toupper(*pronoun);

                estrConcatCharString(&out_str, pronoun);
                pStart = pCursor = pCursor + 6;
            }
            else
            {
                estrConcatChar(&out_str, '$');
                pStart = pCursor;
            }
        }
        pCursor = strchr( pCursor, '$' );
    }

    estrConcatCharString(&out_str, pStart);
    strcpy_s(SAFESTR(g_localize_ret), out_str);
    estrDestroy(&out_str);
    return g_localize_ret;
}

// localize using just a var table
const char* saUtilTableLocalize(const char* string, ScriptVarsTable* table)
{
    if (!string) 
        return NULL;
    if( table )
        string = ScriptVarsTableLookup(table, string);
    if (!g_storyarcMessages)
        return string;
    msxPrintf(g_storyarcMessages, SAFESTR(g_localize_ret), getCurrentLocale(), string, NULL, table, HANDLE_PARAMS_ANYWAY);

    return g_localize_ret;
}

const char * saUtilTableAndEntityLocalize( const char * string, ScriptVarsTable* table, Entity * e )
{
    const char * pchTranslated = saUtilTableLocalize(string, table);
    if( pchTranslated )
        return saUtilEntityVariableReplacement(pchTranslated, e);
    else
        return NULL;
}

// localize a string that is given in a script file, without any variable lookups
const char* saUtilScriptLocalize(const char* string)
{
    if (!string) return NULL;
    if (!g_storyarcMessages)
        return string;
    msxPrintf(g_storyarcMessages, SAFESTR(g_localize_ret), getCurrentLocale(), string, NULL, NULL, 0);
    return g_localize_ret;
}

// *********************************************************************************
//  Random Fame Strings - a place to put strings on a player, may be said by npcs later
// *********************************************************************************

// need to wipe the random fame strings when we have a playerrename
void RandomFameWipe(Entity* player)
{
    if (!player || !player->pl) return;
    memset(player->pl->fameStrings, 0, sizeof(player->pl->fameStrings));
}

void RandomFameAdd(Entity* player, const char* famestring)
{
    int i;

    if (!player || !player->pl || !famestring) return;

    // add to the first empty slot
    for (i = 0; i <    NUM_FAME_STRINGS; i++)
    {
        if (!player->pl->fameStrings[i][0])
        {
            strncpy(player->pl->fameStrings[i], famestring, MAX_FAME_STRING);
            player->pl->fameStrings[i][MAX_FAME_STRING-1] = 0;
            return;
        }
    }

    // or force it to go somewhere
    i = saRoll(NUM_FAME_STRINGS);
    strncpy(player->pl->fameStrings[i], famestring, MAX_FAME_STRING);
    player->pl->fameStrings[i][MAX_FAME_STRING-1] = 0;
}

// looking for npc's nearby who aren't persistent npc's
static Entity* FindNearbyNpc(const Mat4 mat)
{
    Entity* result[MAX_ENTITIES_PRIVATE];
    int count = entWithinDistance(mat, RANDOM_FAME_DIST, result, ENTTYPE_NPC, 0);
    int i;

    // get rid of any persistent npc's
    for (i = count-1; i >= 0; i--)
    {
        if (result[i]->persistentNPCInfo || result[i]->encounterInfo)
        {
            memmove(&result[i], &result[i+1], sizeof(result[0]) * (ENT_FIND_LIMIT - i - 1));
            count--;
        }
    }

    if (!count) return NULL;
    i = saRoll(count);
    return result[i];
}

// see if the player can get a nearby npc to say something
void RandomFameTick(Entity* player)
{
    Entity* npc;
    int slots[NUM_FAME_STRINGS];
    int numslots, i;

    if (player->pl && !OnInstancedMap())
    {
        U32 curtick = dbSecondsSince2000();

        if (ENTINFO(player).hide) return; // don't show for admins

        if (!player->pl->lastFameTime)
        {
            player->pl->lastFameTime = curtick;
            return;
        }

        if (curtick - player->pl->lastFameTime > RANDOM_FAME_TIME &&
            saRoll(100) < RANDOM_FAME_SAY_PROB)
        {
            player->pl->lastFameTime = curtick;
            
            // try to find a npc
            npc = FindNearbyNpc(ENTMAT(player));
            if (!npc) return;

            // try to find a string
            numslots = 0;
            for (i = 0; i < NUM_FAME_STRINGS; i++)
            {
                if (player->pl->fameStrings[i][0])
                    slots[numslots++] = i;
            }

            // go
            if (numslots)
            {
                i = saRoll(numslots);
                i = slots[i];
                saUtilEntSays(npc, 1, player->pl->fameStrings[i]);
                player->pl->fameStrings[i][0] = 0;
            }
        }
    }
}

void RandomFameMoved(Entity* player)
{
    if (player->pl)
        player->pl->lastFameTime = dbSecondsSince2000();
}

// for use by vars
static char g_adj[500];
static char g_class[500];
static char g_origin[500];

void saBuildScriptVarsTable(ScriptVarsTable        *svt,                // Table* to add our scopes to - should be not NULL
                            const Entity        *ent,                // Entity* to draw name, gender, alignment from - can be NULL, in which case ent_db_id is used instead
                            int                    ent_db_id,            // db_id to draw name, gender, alignment from if ent == NULL, unless 0 in which case they are not filled
                            ContactHandle        contactHandle,        // ContactHandle to draw contact name and var scope from - can be 0 (treated as NULL)
                            const StoryTaskInfo *sti,                // StoryTaskInfo* to draw arc/parent/task/mission var scope from, as well as taskRandom fields and seed - can be NULL
                                                                    //   the seed here supercedes seed field (for convenience's sake) - can be NULL
                            const StoryTask        *st,                // StoryTask* to draw arc/parent/task/mission var scope from if sti == NULL - can be NULL
                            const StoryTaskHandle *sahandle,        // StoryTaskHandle* to draw arc/parent/task/mission var scope from if sti == NULL && st == NULL - can be NULL
                            const StoryArcInfo    *sai,                // StoryArcInfo* to draw arc var scope and seed from if there is no current assigned task - can be NULL - sti supercedes
                            const ContactTaskPickInfo    *pickInfo,    // ContactTaskPickInfo* to draw arc/parent/task/mission var scope and random villainGroup from - trumps st & sahandle
                            VillainGroupEnum    villainGroup,        // VillainGroupEnum to draw random villainGroup from if no other source available
                            unsigned int        seed,                // seed to use if sti == NULL and sai == NULL - can be 0
                            bool                permanent)            // true if this table is intended for permanent use (uses SV_COPYVALUE) and false otherwise
{
    // Verify we have a ScriptVarsTable
    if (!svt) return;

    // Verify which Entity are using, assume that ent_db_id takes priority over ent if there is a mismatch, but use ent anyway in a pinch
    //  (this is to recreate existing prior behavior)
    if(ent_db_id && SAFE_MEMBER(ent, db_id) != ent_db_id)
    {
        const Entity *priorityEnt = entFromDbId(ent_db_id);

        if(priorityEnt)
        {
            LOG(LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "saBSVT resolved mismatch: Ent: %s DBID: %d PriorityEnt: %s", ent ? ent->name : "none", ent_db_id, priorityEnt->name);
            ent = priorityEnt;
        }
        else
        {
            LOG(LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "saBSVT unresolved mismatch: Ent: %s DBID: %d", ent ? ent->name : "none", ent_db_id);
        }
    }

    // 1 - Add the Entity basics
    // If ent is supplied and not permanent table, use the E type (invalid once the entity is freed)
    if (!permanent && ent)
    {
        ScriptVarsTablePushVarEx(svt, "Hero",        (char*) ent, 'E', 0);
        ScriptVarsTablePushVarEx(svt, "HeroName",    (char*) ent, 'E', 0);            // deprecated
        ScriptVarsTablePushVarEx(svt, "Villain",    (char*) ent, 'E', 0);            // deprecated
        ScriptVarsTablePushVarEx(svt, "VillainName",(char*) ent, 'E', 0);            // deprecated
    
        if(SAFE_MEMBER3(ent, pchar, pclass, pchDisplayName))
        {
            strcpy_s(SAFESTR(g_class), localizedPrintf(ent, SAFE_MEMBER3(ent, pchar, pclass, pchDisplayName)));
            ScriptVarsTablePushVar(svt, "HeroClass", g_class);
        }

        if(SAFE_MEMBER3(ent, pchar, porigin, pchDisplayName))
        {
            strcpy_s(SAFESTR(g_origin), localizedPrintf(ent, SAFE_MEMBER3(ent, pchar, porigin, pchDisplayName)));
            ScriptVarsTablePushVar(svt, "HeroOrigin", g_origin);
        }

        if(SAFE_MEMBER2(ent, pl, titleOrigin) && ent->pl->titleOrigin[0])
            ScriptVarsTablePushVar(svt, "HeroAdjective", ent->pl->titleOrigin);
        else if (SAFE_MEMBER2(ent, pl, titleCommon) && ent->pl->titleCommon[0])
            ScriptVarsTablePushVar(svt, "HeroAdjective", ent->pl->titleCommon);
        else
        {
            strcpy_s(SAFESTR(g_adj), saUtilLocalize(ent, "DefaultHeroAdjective"));
            ScriptVarsTablePushVar(svt, "HeroAdjective", g_adj);
        }

        if (ent->taskforce_id)
            ScriptVarsTablePushVar(svt, "TaskForceName", SAFE_MEMBER2(ent, taskforce, name));
        
        if (ent->supergroup_id)
            ScriptVarsTablePushVar(svt, "SuperGroupName", SAFE_MEMBER2(ent, supergroup, name));
    }
    // If db_id is supplied, use the D type (always valid) - otherwise do not add anything
    else if(ent_db_id > 0)
    {
        ScriptVarsTablePushVarEx(svt, "Hero",        (char*)(intptr_t)ent_db_id, 'D', 0);
        ScriptVarsTablePushVarEx(svt, "HeroName",    (char*)(intptr_t)ent_db_id, 'D', 0);            // deprecated
        ScriptVarsTablePushVarEx(svt, "Villain",    (char*)(intptr_t)ent_db_id, 'D', 0);            // deprecated
        ScriptVarsTablePushVarEx(svt, "VillainName",(char*)(intptr_t)ent_db_id, 'D', 0);            // deprecated

        if(ent && permanent)
        {
            if (ent->taskforce_id)
                ScriptVarsTablePushVarEx(svt, "TaskForceName", SAFE_MEMBER2(ent, taskforce, name), 's', SV_COPYVALUE);

            if (ent->supergroup_id)
                ScriptVarsTablePushVarEx(svt, "SuperGroupName", SAFE_MEMBER2(ent, supergroup, name), 's', SV_COPYVALUE);
        }
    }

    // 2 - Add the Contact var scope and name
    // If contact not supplied, do nothing
    //  NB: There are probably times where we could find the contact if we had a proper sti and the ent which owned the task via TaskGetContactHandle,
    //      but this is beyond the current scope of this project
    if (contactHandle || sai)
    {
        const ContactDef *contactDef = contactHandle ? ContactDefinition(contactHandle) : sai->contactDef;
        
        if(contactDef)
        {
            ScriptVarsTablePushScope(svt, &contactDef->vs);
            ScriptVarsTablePushVarEx(svt, "Contact",    (char*)contactDef, 'C', 0);
            ScriptVarsTablePushVarEx(svt, "ContactName",(char*)contactDef, 'C', 0);            // deprecated
        }
    }

    // 3 - Add the arc/parent/task/mission var scope && random var scope
    //  NB: The changes to utilize the vs.higherscope pointers correctly to build the var chain makes this simple, we simply add the lowest available scope
    // If sti was supplied, use that as it contains the best possible data
    if (sti || st || sahandle || pickInfo)
    {
        const StoryTask *task = NULL;
        const MissionDef *mission = NULL;
        
        // Where do we get our task definition from? Prefer sti to taskDef to sahandle
        if (sti)
        {
            task = sti->def;

            // This can occur when looking at player created missions, the def doesn't exist in memory unless it's loaded from the player created arc file
            if(!task)
                task = TaskDefinition(&sti->sahandle);
        }
        else if (pickInfo)
            task = pickInfo->taskDef;
        else if (st)
            task = st;
        else if (sahandle)
            task = TaskDefinition(sahandle);

        if (task)
        {
            // Check for mission scope (lowest possible) or use the task scope instead
            mission = TaskMissionDefinitionFromStoryTask(task);

            if (mission)
                ScriptVarsTablePushScope(svt, &mission->vs);
            else
                ScriptVarsTablePushScope(svt, &task->vs);

            // Add Random vars (Newspaper/Police Band missions)
            if (ent)
            {
                if (sti)
                    randomAddVarsScopeFromStoryTaskInfo(svt, ent, sti);
                else if (pickInfo)
                    randomAddVarsScopeFromContactTaskPickInfo(svt, ent, pickInfo);
                else if (villainGroup)
                    randomAddGroupVarsScope(svt, ent, villainGroup);
            }
        }
    }
    // 3a - Add the arc var scope if all we were given was a StoryArcInfo - this occurs while the Arc is in progress but there is no assigned task
    else if(SAFE_MEMBER(sai, def))
    {
        ScriptVarsTablePushScope(svt, &sai->def->vs);
    }

    // 4 - Set the seed
    if(sti)
        ScriptVarsTableSetSeed(svt, sti->seed);
    else if(sai)
        ScriptVarsTableSetSeed(svt, sai->seed);
    else if(seed)
        ScriptVarsTableSetSeed(svt, seed);
}