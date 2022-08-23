#include "entity/entserver.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdcontrols.h"
#include "svr/svr_base.h"
#include "player/clientEntityLink.h"
#include "player/parseClientInput.h"
#include "svr/svr_player.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/network/sock.h>
#include "dbcomm/dbcomm.h"
#include "gameComm/sendToClient.h"
#include <utilitieslib/assert/assert.h>
#include "group/groupnetdb.h"
#include <utilitieslib/utils/timing.h>
#include <process.h>
#include <utilitieslib/utils/utils.h>
#include "svr/gloophook.h"
#include "dbcomm/dbmapxfer.h"
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/utils/FolderCache.h>
#include "dbcomm/dbquery.h"
#include "entity/friends.h"
#include "container/containerEmail.h"
#include "gameComm/trading.h"
#include <utilitieslib/network/net_version.h>
#include "entity/entPlayer.h"
#include "entity/entsend.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "entity/character_level.h"
#include "entity/character_animfx.h"
#include "script/scriptengine.h"
#include "script/scriptdebug.h"
#include "gameComm/debugCommon.h"

#include "serverError.h"
#include "language/langServerUtil.h"
#include "entity/character_base.h"
#include "gameComm/automapServer.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/EString.h>
#include "storyarc/TeamTask.h"
#include "beacon/beaconDebug.h"
#include "entity/origins.h"
#include "svr/svr_bug.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "dbcomm/shardcomm.h"
#include "dbcomm/logcomm.h"
#include "group/groupnetdb.h"
#include "group/groupfileloadutil.h"
#include "group/groupfilesave.h"
#include "arenamapserver.h"
#include "seq/modelReload.h"
#include "cutScene.h"
#include <utilitieslib/components/earray.h>
#include "ai/entaiScript.h"
#include "storyarc/storyarcutil.h"
#include "generator/encounterPrivate.h"
#include "entity/entity.h"
#include "entity/entGameActions.h"
#include "ai/entaiPriority.h"
#include "ai/entaiprivate.h"
#include "seq/seq.h"
#include "storyarc/mission.h"
#include "ailib/aiBehaviorPublic.h"
#include "Reward.h"
#include "player/badges_server.h"

static int s_cutscene_pause;
static int s_choice_made;

int CutscenePaused(void)
{
    return s_cutscene_pause;
}

int MadeCutsceneMoralChoice(int choice)
{
    s_cutscene_pause = 0;
    s_choice_made = choice;
    return 0;
}

void rewardEverybody( char * rewardTable )
{
    int i;

    for(i=1;i<entities_max;i++)  
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity * e = entities[i];
            if( ENTTYPE(e) == ENTTYPE_PLAYER )
            {
                rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(rewardTable), VG_NONE, e->pchar->iLevel+1, false, REWARDSOURCE_SCRIPT, NULL);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Cut Scene Def Struct (here so encounters can load them

typedef struct CutSceneFileList {
    CutSceneDef ** cutSceneDefs;
} CutSceneFileList;

TokenizerParseInfo ParseCutSceneEvent[] =
{    
    { "Time",            TOK_STRUCTPARAM | TOK_F32       ( CutSceneEvent, time , 0)            },
    { "Actor",            TOK_STRUCTPARAM | TOK_STRING    ( CutSceneEvent, actor , 0)            },
    { "Dialog",            TOK_STRUCTPARAM | TOK_STRING    ( CutSceneEvent, dialog , 0)        },
    { "Action",            TOK_STRING                        ( CutSceneEvent, action , 0)        },
    { "Target",            TOK_STRING                        ( CutSceneEvent, target , 0)        },
    { "Position",        TOK_STRING                        ( CutSceneEvent, position , 0)        },
    { "MoveTo",            TOK_STRING                        ( CutSceneEvent, moveTo , 0)        },
    { "DepthOfField",    TOK_F32                            ( CutSceneEvent, depthOfField , 0)    },
    { "MoveToDepthOfField",    TOK_F32                        ( CutSceneEvent, moveToDepthOfField , 0)    },
    { "Over",            TOK_INT                            ( CutSceneEvent, over , 0)            },
    { "Music",            TOK_STRING                        ( CutSceneEvent, music , 0)            },
    { "SoundFx",        TOK_STRING                        ( CutSceneEvent, soundfx , 0)        },
    { "LeftText",        TOK_STRING                        ( CutSceneEvent, leftText , 0)        },
    { "LeftWatermark",    TOK_STRING                        ( CutSceneEvent, leftWatermark , 0)    },
    { "LeftObjective",    TOK_STRING                        ( CutSceneEvent, leftObjective , 0)    },
    { "LeftReward",        TOK_STRING                        ( CutSceneEvent, leftReward , 0)    },
    { "RightText",        TOK_STRING                        ( CutSceneEvent, rightText , 0)        },
    { "RightWatermark",    TOK_STRING                        ( CutSceneEvent, rightWatermark , 0)    },
    { "RightObjective",    TOK_STRING                        ( CutSceneEvent, rightObjective , 0)    },
    { "RightReward",    TOK_STRING                        ( CutSceneEvent, rightReward , 0)    },
    { ">",                TOK_END,            0                                                },
    { "", 0, 0 }
};

#define    CUTSCENE_ONENCOUNTERCREATE    (1 << 0)    
#define    CUTSCENE_FREEZEMAPSERVER    (1 << 1)
#define CUTSCENE_FROMSTRING            (1 << 2)

//THe flags are really for the encounter to read, I suppose they should go into a super structure...kinds like how spawndefs and spawninclude+missionencounter shouldn't be one big struct
StaticDefineInt ParseCutSceneFlags[] =
{
    DEFINE_INT
    { "OnEncounterCreate",    CUTSCENE_ONENCOUNTERCREATE },    //for the encounter only
    { "FreezeMapServer",    CUTSCENE_FREEZEMAPSERVER },        //for the encounter only
    DEFINE_END
};

TokenizerParseInfo ParseCutScene[] =
{
    { "{",            TOK_START,        0},
    { "Flags",        TOK_FLAGS(CutSceneDef,flags,            0),                    ParseCutSceneFlags }, 
    { "<",            TOK_STRUCT(CutSceneDef, events,        ParseCutSceneEvent) },
    { "}",            TOK_END,            0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCutSceneList[] =
{
    { "CutScene",    TOK_STRUCT( CutSceneFileList, cutSceneDefs, ParseCutScene) },
    { "", 0, 0 },
};

//TO DO add verification function

///End cut scene def struct
//////////////////////////////////////////////////////////////////////////////////////////

//A running cut scene, like an fx
typedef struct CutScene
{
    CutSceneParams params;        
    F32 time;
    int eventsDone;
    Mat4 cameraMat;
    F32 depthOfField;
    F32 startTime;
    Mat4 moveToCameraMat;
    F32 moveToDepthOfField;

    int    handle;
    char *  oldPLs[MAX_ACTORS]; //TEMP 
} CutScene;

CutScene ** g_cutScenes; //All the currently running cutScenes (only one can freeze the server, the rest are just running

MP_DEFINE(CutScene);


typedef struct CutSceneCamera
{
    Mat4 mat;
    char *name;
}CutSceneCamera;

CutSceneCamera ** g_CutSceneCameras;

CutSceneCamera *cutSceneCameraFind( char * name )
{
    int i;
    for(i=eaSize(&g_CutSceneCameras)-1; i>=0; i--)
    {
        if(stricmp(g_CutSceneCameras[i]->name, name) == 0 )
            return g_CutSceneCameras[i];
    }
    return 0;
}

void cutSceneAddMapCamera( Mat4 mat, char * name )
{
    CutSceneCamera *pCam = calloc(1, sizeof(CutSceneCamera));
    copyMat4(mat, pCam->mat);
    estrCreate(&pCam->name);
    estrConcatf(&pCam->name, "Camera%s", name); 
    eaPush(&g_CutSceneCameras, pCam);
}
//MissionFile (Last Mission) C:\game\data\scripts\StoryArcs\Lvl9_Taskforce_Statesman.storyarc
//MapFile: MapFile Maps\Missions\unique\StatesmanTaskForce\IOM_StopLordRecluse\IOM_StopLordRecluse.txt


CutScene * allocNewCutScene()
{
    CutScene* cutScene;
    MP_CREATE(CutScene, 10);
    cutScene = MP_ALLOC(CutScene);
    if (!g_cutScenes)
        eaCreate(&g_cutScenes);
    eaPush(&g_cutScenes, cutScene);
    return cutScene;
}

void freeCutScene(CutScene* cutScene)
{
    int i, n;
    n = eaSize(&g_cutScenes);
    for (i = n-1; i >= 0; i--)
    {
        if (g_cutScenes[i] == cutScene)
        {
            eaRemove(&g_cutScenes, i);
            break;
        }
    }
    if(cutScene->params.cutSceneDef->flags & CUTSCENE_FROMSTRING)
    {
        StructDestroy(ParseCutScene, cpp_const_cast(CutSceneDef*)cutScene->params.cutSceneDef);
        cutScene->params.cutSceneDef = NULL;
    }
    MP_FREE(CutScene, cutScene);
}

//
void sendCutSceneUpdate( Entity * e, Mat4 mat, Mat4 moveToMat, int over, int continueCutScene, F32 depthOfField, F32 moveToDepthOfField,
                        char * sound, char * music )
{
    START_PACKET(pak, e, SERVER_CUTSCENE_UPDATE);

    //Send toggle to keep going with the cutscene or to end it
    pktSendBits( pak, 1, continueCutScene ? 1 : 0 );  

    //Send Updated camera position, if any
    if( !mat )  
    {
        pktSendBits( pak, 1, 0 );
    }
    else
    {
        Vec3 pyr;
        pktSendBits( pak, 1, 1 );

        getMat3YPR(mat,pyr); 
        pyr[2] = 0; //no body wants any damn roll in their camera

        pktSendF32(pak, pyr[0]);
        pktSendF32(pak, pyr[1]);
        pktSendF32(pak, pyr[2]);

        pktSendF32(pak, mat[3][0]);
        pktSendF32(pak, mat[3][1]);
        pktSendF32(pak, mat[3][2]);

        pktSendBitsAuto( pak, over );
        if (over)
        {
            
            getMat3YPR(moveToMat,pyr); 
            pyr[2] = 0; //no body wants any damn roll in their camera

            pktSendF32(pak, pyr[0]);
            pktSendF32(pak, pyr[1]);
            pktSendF32(pak, pyr[2]);

            pktSendF32(pak, moveToMat[3][0]);
            pktSendF32(pak, moveToMat[3][1]);
            pktSendF32(pak, moveToMat[3][2]);

            pktSendF32(pak, moveToDepthOfField);
        }
    }

    if(music)
    {
        pktSendBits( pak, 1, 1 );
        pktSendString(pak,music);
    }
    else
        pktSendBits( pak, 1, 0 );

    if(sound)
    {
        pktSendBits( pak, 1, 1 );
        pktSendString(pak,sound);
    }
    else
        pktSendBits( pak, 1, 0 );

    pktSendF32( pak, depthOfField );

    END_PACKET
}

static Mat4 tempMat; //TO DO sae everybodies

void updateAllSoundMusic( char * sound, char * music )
{
    int i;

    for(i=1;i<entities_max;i++)  
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity * e = entities[i];
            if( ENTTYPE(e) == ENTTYPE_PLAYER )
                sendCutSceneUpdate( e, 0, 0, 0, 1, server_state.cutSceneDepthOfField, 0, sound, music );
        }
    }
}

void moveAllCamerasToNewMat( CutScene *cutScene, CutSceneEvent *event )
{
    int i;
    F32 depthOfField = cutScene->depthOfField;
    copyMat4( cutScene->cameraMat, server_state.cutSceneCameraMat );
    copyMat4( cutScene->moveToCameraMat, server_state.moveToCutSceneCameraMat );
    
    server_state.cutSceneDepthOfField = depthOfField;
    server_state.moveToCutSceneDepthOfField = cutScene->moveToDepthOfField;
    server_state.cutSceneOver = event->over;
    

    ///////////////////////////////////////////////////////
    for(i=1;i<entities_max;i++)  
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity * e = entities[i];
            if( ENTTYPE(e) == ENTTYPE_PLAYER )
            {
                sendCutSceneUpdate( e, cutScene->cameraMat, event->moveTo ? cutScene->moveToCameraMat : 0, event->over, 1, depthOfField, cutScene->moveToDepthOfField,0, 0 );
                //copyMat3( mat, e->mat ); //TO DO is this how?
                //entSetPos( e, mat[3] );
            }
        }
    }
}

void getCutSceneCameraPos( Vec3 vec )
{
    CutScene * cs = (CutScene*)server_state.cutScene;

    if( cs )
        copyVec3( cs->cameraMat[3], vec );
    
}

/////////////////////////////////////////////////////////////////////////////////////
//Hacky way to hold the player's state so the cut scene can change them
typedef struct PlayerStateStorage
{
    EntityRef player;

} PlayerStateStorage;

PlayerStateStorage ** g_playerStateStorages; //All the currently running cutScenes (only one can freeze the server, the rest are just running

MP_DEFINE(PlayerStateStorage);

PlayerStateStorage * allocNewPlayerStateStorage()
{
    PlayerStateStorage* playerStateStorage;
    MP_CREATE(PlayerStateStorage, 10);
    playerStateStorage = MP_ALLOC(PlayerStateStorage);
    if (!g_playerStateStorages)
        eaCreate(&g_playerStateStorages);
    eaPush(&g_playerStateStorages, playerStateStorage);
    return playerStateStorage;
}

void freePlayerStateStorage(PlayerStateStorage* playerStateStorage)
{
    int i, n;
    n = eaSize(&g_playerStateStorages);
    for (i = n-1; i >= 0; i--)
    {
        if (g_playerStateStorages[i] == playerStateStorage)
        {
            eaRemove(&g_playerStateStorages, i);
            break;
        }
    }
    MP_FREE(PlayerStateStorage, playerStateStorage);
}

void copyPlayerIntoPlayerStateStorage( Entity * e )
{
    PlayerStateStorage * pss;
    if(!e)
        return;
    pss = allocNewPlayerStateStorage();

    ////////// Mirror of Below
    pss->player = erGetRef( e ); 
    ////////// End Mirror

}

void restorePlayerFromPlayerStateStorage( PlayerStateStorage * pss )
{
    Entity * e;
    if( !pss )
        return;

    ////////// Mirror of Above
    e =    erGetEnt( pss->player );
    if( e )
    {
        SET_ENTHIDE(e) = 0;
        e->controlsFrozen = 0;
        e->untargetable &= ~UNTARGETABLE_TRANSFERING; //TO DO fourth bit?
        e->invincible &= ~INVINCIBLE_TRANSFERING;
        e->move_type = MOVETYPE_WALK;
    }
    ////////// End Mirror

    freePlayerStateStorage( pss );
}

//TO DO this will break with more than one frozen cut scene running at a time.  Fortunately that can't happen...
void restorePlayersFromStateStorage()
{
    int i, n;

    n = eaSize(&g_playerStateStorages);
    for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
    {
        restorePlayerFromPlayerStateStorage( g_playerStateStorages[i] );
    }
}

void setEntityInCutScene( Entity * e )
{
    if( ENTTYPE(e) == ENTTYPE_PLAYER )
    {
        copyPlayerIntoPlayerStateStorage( e );

        setNoPhysics(e, 1);
        //setViewCutScene(e, 1);

        //SET_ENTHIDE(e) = 1;

        e->controlsFrozen = 1;
        e->untargetable |= UNTARGETABLE_TRANSFERING;
        e->invincible |= INVINCIBLE_TRANSFERING;
        e->move_type = MOVETYPE_NOCOLL;

        sendCutSceneUpdate( e, server_state.cutSceneCameraMat, server_state.moveToCutSceneCameraMat, server_state.cutSceneOver, 
            1, server_state.cutSceneDepthOfField, server_state.moveToCutSceneDepthOfField,0, 0 );
    }
    else 
    {
        //TO DO Turn off physics for NPCs?
        SET_ENTPAUSED(e) = 1;

    }
    e->cutSceneState = 1;

    //TO DO spawned dudes are frozen right now, but pets spawned by the encounter shouldn't be?
}

//Not perfect since it doesn't do restore players from state storage, though set_EntityInCutScene puts it in
void setEntityOutOfCutScene( Entity * e )
{
    if( ENTTYPE(e) == ENTTYPE_PLAYER )
    {
        setNoPhysics(e, 0);

        //TO DO save off old state to correctly restore...
        sendCutSceneUpdate( e, 0, 0, 0, 0, 0, 0,0, 0 );
    }
    else 
    {
        //TO DO Turn off physics for NPCs?
        SET_ENTPAUSED(e) = 0;
    }
    e->cutSceneState = 0;
}


// End Player State Storage //////////////////////////////////////////////////////////////

void freezeServerForCutScene( CutScene * cs )
{
    int i;

    //Get rid of old cut scene
    if( server_state.viewCutScene ) //TO DO this works, right?
        assert( 0 && "How did I get here?");

    server_state.viewCutScene = 1;
    server_state.cutScene = cs; //Global cut scene that has frozen the server
    copyMat4( cs->cameraMat, server_state.cutSceneCameraMat );
    server_state.cutSceneDepthOfField = cs->depthOfField;

    //Freeze passage of time for powers, mission timers, scripts, 
    //Encounters cease to update
    //Find a good spot for the camera

    ///////////////////////////////////////////////////////
    for(i=1;i<entities_max;i++)  
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity * e = entities[i];
            setEntityInCutScene( e );
        }
    }

    //Unpause everybody in the cutScene
    for( i = 0 ; i < cs->params.castCount ; i++ )
    {
        Entity * e = erGetEnt(cs->params.cast[i].entRef);
        if( e )
            SET_ENTPAUSED(e) = 0;
    }

    //Move player cameras into position (TO DO correctly restore
    //Note this implicitly sets viewCutScene on the Client
    //moveAllCamerasToNewMat( cs->cameraMat );
}

void unfreezeServerForCutScene( CutScene * cs )
{
    int i;

    server_state.viewCutScene = 0;
    server_state.cutScene = 0; //Global cut scene that has frozen the serv

    //Restore all the player states.
    //TO DO what was I trying to do here?  Why not just put this in the setEntityOutOfCutScene function
    restorePlayersFromStateStorage();
    
    for(i=1;i<entities_max;i++)
    { 
        if (entity_state[i] & ENTITY_IN_USE )
        {
            setEntityOutOfCutScene(  entities[i] );
        }
    }
}




//TO DO Brute force right now, maybe make a hash...
//Also, I'm just using as a boolean now
void * cutSceneFromHandle( int handle )
{
    int i,n;
    n = eaSize(&g_cutScenes);
    for (i = n-1; i >= 0; i--)
    {
        if ( g_cutScenes[i]->handle == handle )
        {
            return (void*)g_cutScenes[i];
        }
    }
    return 0;
}



//TO DO, use different names?  Villain name, etc.
Entity * getCastMember( CutScene * cutScene, char * name )
{
    Entity * e = 0;
    int n, i;
    n = cutScene->params.castCount; //eaiSize(&cutScene->params.cast);

    for (i = n-1; i >= 0; i--) 
    {
        CutSceneCastMember * actor = &cutScene->params.cast[i];

        if( actor->role && 0 == stricmp( actor->role, name ) )
        {
            e = erGetEnt(actor->entRef);
            return e;
        }
    }

    return 0;
}

//TO DO
F32 orientCameraTowardPoint( Vec3 point, Mat4 camMat )
{
    Vec3 d;
    Vec3 dest_pyr;

    subVec3( camMat[3], point, d);
    camLookAt(d, camMat);
    getMat3YPR(camMat, dest_pyr); //These two lines necessary?
    dest_pyr[2] = 0;
    createMat3PYR(camMat, dest_pyr);

    return distance3( camMat[3], point );
}

F32 orientCameraTowardTarget( Entity * target, Mat4 newMat )
{ 
    Mat4 camMat;
    Mat4 targetMat;

    copyMat4( newMat, camMat );  
    copyMat4( ENTMAT(target), targetMat ); 
    targetMat[3][1] += target->seq->type->capsuleSize[1]*0.75; //Rough approximation of the chest
    {
        Vec3 d;
        Vec3 dest_pyr;

        subVec3( camMat[3],targetMat[3], d);
        camLookAt(d, camMat);
        getMat3YPR(camMat, dest_pyr); //These two lines necessary?
        dest_pyr[2] = 0;
        createMat3PYR(camMat, dest_pyr);
    }

    copyMat4( camMat, newMat );

    return distance3( camMat[3], targetMat[3] );
}



#define POWERPREFIX "Power:"
#define CAMERANAME "Camera"
#define FXNAME "FX"
#define SCRIPTNAME "Script"
#define CAPTIONNAME "Caption"
#define ENTCAMERAZOFFSET 1.6
#define ENTCAMERAYOFFSET 5
#define ENCOUNTERCAMZOFFSET 20
#define ENCOUNTERCAMPOSITIONNAME "Encounter"
#define CSDBPRINT if(server_state.cutSceneDebug)printf(

#define MORALCHOICE "MoralChoice"
#define MORALCHOICERESPONSE "MoralChoiceResponse"


void setCameraToShowEncounter( CutScene * cutScene )
{
    const CutSceneDef * def;
    Mat4 camMat;
    Mat4 targetMat;
    Entity * e = 0;
    int eventCount, i;

    //A bit of a hack
    def = cutScene->params.cutSceneDef;

    //Find the first referenced entity
    eventCount = eaSize( &def->events );
    for (i = 0 ; i < eventCount ; i++ ) 
    {
        if( def->events[i]->actor )  
            e = getCastMember( cutScene, def->events[i]->actor );
        if( e )
            break;
    }

    if( e )
    {
        copyMat4( ENTMAT(e), camMat ); 

        //TO DO take into account the camera moved into head space and 
        //TO DO make first person on the client
        camMat[3][0] += ENCOUNTERCAMZOFFSET;
        camMat[3][1] += ENTCAMERAYOFFSET;
        camMat[3][2] += 0;

        copyMat4( ENTMAT(e), targetMat ); 
        targetMat[3][1] +=5;

        //TO DO cast ray and collide to get best cam pos

        //Look at encounter
        {
            Vec3 d;
            Vec3 dest_pyr;

            subVec3( camMat[3], targetMat[3], d);
            //d[1] = 0;
            camLookAt(d, camMat);
            getMat3YPR(camMat, dest_pyr); //These two lines necessary?
            createMat3PYR(camMat, dest_pyr);
        }

        copyMat4( camMat, cutScene->cameraMat );
        cutScene->depthOfField = distance3( cutScene->cameraMat[3], targetMat[3] );
    }
}

static int getMatForPos(CutScene *cutScene, Entity *target, char *position, Mat4 cameraMat, F32 *depthOfField)
{
    Entity * camPosEnt;
    CutSceneCamera * cam = cutSceneCameraFind( position );

    if( cam )
    {
        if( !target )
            copyMat4( cam->mat, cameraMat ); // use the cameras entire mat
        else 
            copyVec3( cam->mat[3], cameraMat[3] ); // just loacation
    }
    //Is the position two actors?  Then put the camera between them
    else if( strstri( position , "+" ) )
    {
        char name1[128];
        Entity * e1;
        Entity * e2;
        char * name2;
        Vec3 pyr1;
        Vec3 pyr2;
        Vec3 pyrBlend;
        Mat4 blendMat;
        Vec3 midPos1;
        Vec3 midPos2;
        F32 spaceBetween;
        Vec3 pullBack;
        EntityCapsule cap1;
        EntityCapsule cap2;
        Mat4 collMat1;
        Mat4 collMat2;
        Vec3 p;
        F32 h1;
        F32 h2;
        int found1 = 0, found2 = 0;

        //Get the Entities to split between

        strncpy( name1, position, 128 ); 
        name2 = strstri( name1 , "+" );
        name2[0] = 0;
        name2++;

        //If you wanted entities, get entities
        e1 = getCastMember( cutScene, name1 );
        if( e1 )
        {
            //Figure out the middle of the capsules
            getCapsule(e1->seq, &cap1 ); 
            positionCapsule( ENTMAT(e1), &cap1, collMat1 ); 
            p[0] = 0;
            p[1] = cap1.length;
            p[2] = 0;
            mulVecMat4( p, collMat1, midPos1 );

            getMat3YPR( ENTMAT(e1), pyr1 );

            h1 = cap1.length;

            found1 = 1;
        }

        e2 = getCastMember( cutScene, name2 );
        if( e2 )
        {
            //Figure out the middle of the capsules
            getCapsule(e2->seq, &cap2 ); 
            positionCapsule( ENTMAT(e2), &cap2, collMat2 ); 
            p[0] = 0;
            p[1] = cap2.length;
            p[2] = 0;
            mulVecMat4( p, collMat2, midPos2 );

            getMat3YPR( ENTMAT(e2), pyr2 );

            h2 = cap2.length;

            found2 = 1;
        }

        //If you wanted encounter positions, get those
        if( !found1 )
        {
            int pos;
            char * name = name1;
            if( 0 == strnicmp( &name1[0], "e", 1 ) )
                name = &name1[1];
            pos = atoi( name );

            if( pos > 0 && pos <= MAX_CAMERA_MATS )
            {
                Vec3 t, t2;    //This just moves the camera position to the encounter position's head so the camera is coming out of
                // the geometry's eyes, not his feet for ease of use with existing encounters
                Mat4Ptr mat;
                t[0]=0;t[1]=5.0;t[2]=0;
                mat = cutScene->params.cameraMats[ pos ];
                mulVecMat4(t,mat,t2);
                copyVec3(t2, mat[3]);

                getMat3YPR( mat, pyr1 );
                copyVec3( mat[3], midPos1 );

                h1 = 3.0;

                found1 = 1;
            }
        }

        if( !found2 ) 
        {
            int pos;
            char * name = name2;
            if( 0 == strnicmp( &name2[0], "e", 1 ) )
                name = &name2[1];
            pos = atoi( name );

            if( pos > 0 && pos <= MAX_CAMERA_MATS )
            {
                Vec3 t, t2;    //This just moves the camera position to the encounter position's head so the camera is coming out of
                // the geometry's eyes, not his feet for ease of use with existing encounters
                Mat4Ptr mat;
                t[0]=0;t[1]=5.0;t[2]=0;
                mat = cutScene->params.cameraMats[ pos ];
                mulVecMat4(t,mat,t2);
                copyVec3(t2, mat[3]);

                getMat3YPR( mat, pyr2 );
                copyVec3( mat[3], midPos2 );

                h2 = 3.0;

                found2 = 1;
            }
        }

        if( !found1 || !found2 )
            return 0;

        //////// USE the numbers you got to find a nice camera position

        //Get a nice blend of the two mats
        interpPYR( 0.5, pyr1, pyr2, pyrBlend );
        createMat3PYR( blendMat, pyrBlend );

        addVec3( midPos1, midPos2,  blendMat[3] );
        scaleVec3( blendMat[3], 0.5, blendMat[3] );

        //Figure out a reasonable pullback Fairly random calculation on how far to pull back and up
#define TWOMANENTCAMERAZOFFSET 5.0
#define DEFAULT_SPACEBETWEEN 10.0//IF the space between the two is more than this, then start pulling the camera back more
        zeroVec3( pullBack );
        spaceBetween = distance3( midPos1, midPos2 );
        pullBack[2] = ( MAX(h1,h2) * TWOMANENTCAMERAZOFFSET ) + ( MAX( 0, ( spaceBetween-DEFAULT_SPACEBETWEEN ) ) ) ; 

        //Position camera with pullback, and turn camera to face the center point
        mulVecMat4( pullBack, blendMat, cameraMat[3] );
        (*depthOfField) = orientCameraTowardPoint( blendMat[3], cameraMat );
    }
    //Is the position an actor?  Then put the camera in front of him, move it forward a few feet, up a few feet and 
    //Turn it to face the actor
    else if( camPosEnt = getCastMember( cutScene, position ) ) 
    {
        Mat4 mat;
        Vec3 unit;
        Vec3 orientedUnit;

        unit[0] = 0; unit[1] = 0 ; unit[2] = 1;  
        copyMat4( ENTMAT(camPosEnt), mat );
        mulVecMat3( unit, mat, orientedUnit );
        scaleVec3( orientedUnit, (camPosEnt->seq->type->capsuleSize[0] * ENTCAMERAZOFFSET)+5.0, orientedUnit );
        addVec3( mat[3], orientedUnit, mat[3] ); 
        mat[3][1] += camPosEnt->seq->type->capsuleSize[1]*0.75; //Rough approximation of the chest

        copyMat4( mat, cameraMat );
        (*depthOfField) = orientCameraTowardTarget( camPosEnt, cutScene->cameraMat );

    }
    else if( 0 == stricmp( position, ENCOUNTERCAMPOSITIONNAME ) )
    {
        setCameraToShowEncounter( cutScene );
    }
    else //Try to make it an encounter position
    {
        int pos;
        char * name = position;
        if( 0 == strnicmp( &position[0], "e", 1 ) )
            name = &position[1];
        pos = atoi( name );

        if( pos > 0 && pos <= MAX_CAMERA_MATS )
        {
            Vec3 t, t2;    //This just moves the camera position to the encounter position's head so the camera is coming out of
            // the geometry's eyes, not his feet for ease of use with existing encounters
            Mat4Ptr mat;
            t[0]=0;t[1]=5.0;t[2]=0;
            mat = cutScene->params.cameraMats[ pos ];
            mulVecMat4(t,mat,t2);
            copyVec3(t2, mat[3]);
            copyMat4( mat, cameraMat );
            (*depthOfField) = 0; //Encounter positions don't know what they are looking at (below a target can correct them)
        }
    }
    return 1;
}


static int doCameraEvent( CutScene * cutScene, CutSceneEvent * event, Entity * target )
{
    if( !cutScene->params.freezeServerAndMoveCameras )
        return 0;

    // assume the previous DOF movement (if there was one) has concluded
    if(cutScene->moveToDepthOfField)
        cutScene->depthOfField = cutScene->moveToDepthOfField;

    if( event->position )
    {
        if (!getMatForPos(cutScene, target, event->position, cutScene->cameraMat, &cutScene->depthOfField))
            return 0;
        if (event->moveTo)
        {
            if (!event->over)
                return 0;
            if (!getMatForPos(cutScene, target, event->moveTo, cutScene->moveToCameraMat, &cutScene->moveToDepthOfField))
                return 0;
        }
    }

    //TO DO specify new target for camera? and adjust the matrix accordingly
    if( target )
    {
        cutScene->depthOfField = orientCameraTowardTarget( target, cutScene->cameraMat );
        
        if(event->moveTo && event->over)
            cutScene->moveToDepthOfField = orientCameraTowardTarget( target, cutScene->moveToCameraMat );
        else
            cutScene->moveToDepthOfField = cutScene->depthOfField;
    }

    //If a depth of field is explicitly given, use it
    if( event->depthOfField )
        cutScene->depthOfField = cutScene->moveToDepthOfField = event->depthOfField;
    
    //If there is a move to depth of field explicitly given, use it
    if( event->moveToDepthOfField && event->over )
        cutScene->moveToDepthOfField = event->moveToDepthOfField;

    moveAllCamerasToNewMat( cutScene, event );

    return 1;
}



static int doCaptionEvent( CutSceneEvent * event )
{
    ScriptVarsTable vars = {0};

    if( !event->dialog )
        return 0;

    if( OnMissionMap() )
        saUtilCaption( saUtilTableLocalize(event->dialog, &g_activemission->vars) );
    else
        saUtilCaption( saUtilTableLocalize(event->dialog, &vars) );

    return 1;
}

// Don't use this for anything other than full screen FX.  It does some extremely nasty hacky stuff
// to find a suitable entity to anchor the FX to.  It just scans the entity list looking for an entry
// that is (a) in use, and (b) a player.
static int doFXEvent( CutSceneEvent * event )
{
    int i;
    Entity *e;
    Character *src;
    Vec3 target = { 0 };

    if (server_state.cutSceneDebug && event->action == NULL) //DEBUG
    {
        Errorf( "CSEVENT FX at time %f has no FX", event->time );
    } //END DEBUG

    src = NULL;
    for(i = 0; i < entities_max; i++)
    {
        e = entities[i];

        if(entInUse(i) && e && e->pchar && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER)
        {
            src = e->pchar;
            break;
        }
    }

    if (src != NULL)
    {
        character_PlayFXLocation(src, NULL, target, event->action, colorPairNone, 0, 0, PLAYFX_NO_TINT);
    }
    return 1;
}

static int doScriptEvent( CutSceneEvent * event )
{
    if (server_state.cutSceneDebug && event->action == NULL) //DEBUG
    {
        Errorf( "CSEVENT FX at time %f has no FX", event->time );
    } //END DEBUG

    ScriptSendScriptMessage(event->action);
    return 1;
}

static int doActorEvent( CutScene * cutScene, CutSceneEvent * event, Entity * target )
{
    Entity * actor = 0;

    //Get actor  TO DO (this doesn't support positions at all yet)
    if( event->actor )
        actor = getCastMember( cutScene, event->actor );

    if( server_state.cutSceneDebug && !actor ) //DEBUG
    {
        if( !event->actor )
            Errorf( "CSEVENT at time %f has no actor", event->time );
        else
            Errorf( "CSEVENT couldn't find actor %s among the cast members.", event->actor );
    } //END DEBUG

    if( !actor )
        return 0;

    //Set target
    if( target )
        aiSetAttackTarget(actor, target, NULL, NULL, 1, 0);

    //Set action //TO DO support new action types?
    if( event->action )
    {
        if( 0 == strnicmp( event->action, POWERPREFIX, strlen(POWERPREFIX) ) )
        {
            aiCritterSetCurrentPowerByName( actor, &event->action[ strlen( POWERPREFIX ) ] );
            //entaiScriptSetPriorityList( actor, "PL_UsePowerNow" );
            aiCritterUsePowerNow( actor );
        }
        else
        {
            aiBehaviorAddFlag( actor, ENTAI(actor)->base,
                aiBehaviorGetTopBehaviorByName( actor, ENTAI(actor)->base, "CutScene"), event->action);
            //aiScriptSetPriorityList( actor, event->action );
        }
    }

    //Set dialog
    if( event->dialog )
    {
        ScriptVarsTable vars = {0};

        if(EncounterVarsFromEntity(actor))
            saUtilEntSays( actor, ENTTYPE(actor) == ENTTYPE_NPC, saUtilTableLocalize(event->dialog, EncounterVarsFromEntity(actor)) );
        else if( OnMissionMap() )
            saUtilEntSays( actor, ENTTYPE(actor) == ENTTYPE_NPC, saUtilTableLocalize(event->dialog, &g_activemission->vars) );
        else
            saUtilEntSays( actor, ENTTYPE(actor) == ENTTYPE_NPC, saUtilTableLocalize(event->dialog, &vars) );
    }

    ///////////End Actor manager

    return 1;
}

static int doCutsceneMoralChoice( CutScene *cs, CutSceneEvent * cse )
{
    int i;

    for(i=1;i<entities_max;i++)  
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity * e = entities[i];
            if( ENTTYPE(e) == ENTTYPE_PLAYER )
            {
                sendMoralChoice(e, cse->leftText, cse->rightText, cse->leftWatermark, cse->rightWatermark, false);
            }
        }
    }
    s_cutscene_pause = 1;

    return 1;
}


int doCutSceneEvent( CutScene * cutScene, CutSceneEvent * event )
{
    Entity * target = 0;

    if( event->music || event->soundfx )
        updateAllSoundMusic( event->soundfx , event->music );

    if( !event->actor )
        return 0;

    //Get the target
    if( event->target )    
        target = getCastMember( cutScene, event->target );

    /////////// Camera Manager If this is the camera moving, take care of that
    if( 0 == stricmp( event->actor, CAMERANAME ) )
        return doCameraEvent(cutScene, event, target);

    if( 0 == stricmp( event->actor, CAPTIONNAME ) )
        return doCaptionEvent( event );

    if( 0 == stricmp( event->actor, FXNAME ) )
        return doFXEvent( event );

    if( 0 == stricmp( event->actor, SCRIPTNAME ) )
        return doScriptEvent( event );

    if( 0 == stricmp( event->actor, MORALCHOICE ) )
        return doCutsceneMoralChoice( cutScene, event );

    if( 0 == stricmp( event->actor, MORALCHOICERESPONSE ) )
    {
        if (s_choice_made == 1)
        {
            if (event->leftObjective)
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, event->leftObjective);
            if (event->leftReward)
                rewardEverybody(event->leftReward);
        }
        else if (s_choice_made == 2)
        {
            if (event->rightObjective)
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, event->rightObjective);
            if (event->rightReward)
                rewardEverybody(event->rightReward);
        }
    }

    return doActorEvent( cutScene, event, target );
}

int processCutScene( CutScene * cutScene, F32 timestep )
{
    F32 oldTime = cutScene->time;
    int eventCount, i;
    const CutSceneDef * def = cutScene->params.cutSceneDef;
    int theEnd = 0;
    F32 thisEventsTime = 0;

    if (CutscenePaused())
        return 1;

    cutScene->time += timestep;

    eventCount = eaSize( &def->events );

    //Check each event and see if it's time to process it
    for (i = 0 ; i < eventCount ; i++ ) 
    {
        CutSceneEvent * event = def->events[i];
    
        if( oldTime <= thisEventsTime*30.0f && thisEventsTime*30.0f < cutScene->time )
        {
            doCutSceneEvent( cutScene, event );
            cutScene->eventsDone++;
        }
        thisEventsTime += event->time; //Each time is a delta on the last time
    }

    //thisEventsTime now being the sum of all times in the cutscene, if we are past that, we are done
    if( cutScene->time > thisEventsTime*30 ) 
    {
        assert( cutScene->eventsDone == eventCount ); 
        return 0;
    }

    return 1;
}


void endCutScene( CutScene * cutScene )
{
    if( cutScene->params.freezeServerAndMoveCameras )
        unfreezeServerForCutScene( cutScene );

    //RESTORE PL TO DO clean this up
    {
        Entity * e;
        int n, i;
        n = cutScene->params.castCount; //eaiSize(&cutScene->params.cast);

        for (i = n-1; i >= 0; i--) 
        {
            e = erGetEnt(cutScene->params.cast[i].entRef);
            if( e )
            {
                e->cutSceneActor = 0;
                e->cutSceneHandle = 0;

                //aiBehaviorAddFlag( e, aiBehaviorGetTopBehaviorByName(e, "CutScene"), "StopCutScene" );
                aiBehaviorMarkFinished( e, ENTAI(e)->base, aiBehaviorGetTopBehaviorByName(e, ENTAI(e)->base, "CutScene") );
                
                //aiBehaviorAddString( e, "Clean" );

                //if( cutScene->oldPLs[i] )
                //{
                //    aiScriptSetPriorityList( e, cutScene->oldPLs[i] );
                //}
            }
        }
    }
    //End Restore PL
    
    freeCutScene( cutScene );
}


void processCutScenes( F32 timestep )
{
    int i, n;

    n = eaSize(&g_cutScenes);
    for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
    {
        if( 0 == processCutScene( g_cutScenes[i], timestep ) )
            endCutScene( g_cutScenes[i] );
    }
}



int startCutScene( CutSceneParams * cutSceneParams )
{
    static int staticCutSceneHandles = 10; 
    CutScene * cutScene;
    const CutSceneDef * def;

    if( !cutSceneParams || !cutSceneParams->cutSceneDef )
        return 0;
    def = cutSceneParams->cutSceneDef;

    //If there is only one event and it is empty, then don't do a cut scene
    if( !def->events || ((!def->events[0] || !def->events[0]->actor) && eaSize( &def->events ) <= 1 ))
        return 0;
 

    //Get rid of old cut scene (must be done above alloc new)
    if( server_state.cutScene && cutSceneParams->freezeServerAndMoveCameras )
        endCutScene( server_state.cutScene );

    ////////////////////////////////////////////////////////
    //Get info set up
    cutScene = allocNewCutScene(); 
    cutScene->handle = staticCutSceneHandles++;

    ///Get cut Scene Def if needed

    cutScene->params = *cutSceneParams;  //note struct copy;

    //TO DO this default is a little weak
    //Now this means just start with Encounter Position One.  Poor default
    if( cutSceneParams->cameraMats[1][3][0] || cutSceneParams->cameraMats[1][3][1] || cutSceneParams->cameraMats[1][3][2] )
        copyMat4( cutSceneParams->cameraMats[1], cutScene->cameraMat );
    else //Use the first actor to be refered to in the script
    {  
        setCameraToShowEncounter( cutScene );
    }

    //////////////////////////////////////////////////////////
    //Bring down the server if needed
    if( cutScene->params.freezeServerAndMoveCameras )
    {
        freezeServerForCutScene( cutScene );
    }

    //TO DO clean this up record the old states of the guys before I start messing with them
    {
        Entity * e;
        int n, i;
        n = cutScene->params.castCount; //eaiSize(&cutScene->params.cast);

        for (i = n-1; i >= 0; i--) 
        {
            cutScene->oldPLs[i] = 0;
            e = erGetEnt(cutScene->params.cast[i].entRef);
            if( e )
            {    
                AIVars* ai = ENTAI(e);
                if(ai)
                    ai->timerTryingToSleep = 0;
                SET_ENTSLEEPING(e) = 0;

                //aiBehaviorAddPermFlag( e, "DoNotGoToSleep" );
                aiBehaviorAddString( e, ENTAI(e)->base, "CutScene( DoNotGoToSleep, DoNothing )" );
                //cutScene->oldPLs[i] = aiGetEntityPriorityListName(e);
                e->cutSceneActor = 1;
                e->cutSceneHandle = cutScene->handle;
            }
        }
    }
    //

    
    return cutScene->handle;
}


Entity * getActorFromCutScene( int cutSceneHandle, char * actorName )
{
    Entity * e = 0;
    CutScene * cutScene;

    if( !cutSceneHandle || !actorName )
        return 0;

    cutScene = cutSceneFromHandle( cutSceneHandle );
    if( cutScene )
    {
        e = getCastMember( cutScene, actorName );
    }
    return e;
}



int cutscene_has_objective(const CutSceneDef ** csd, const char *objective)
{
    int idef, ievent, ndefs, nevents;
    CutSceneEvent ** events;

    if (!csd)
        return 0; //If there's no cutscene, then it can't have the objective.

    ndefs = eaSize(&csd);
    for (idef = 0; idef < ndefs; idef++)
    {
        if (!csd[idef]->events)
            continue;
        events = csd[idef]->events;
        nevents = eaSize(&events);
        for (ievent = 0; ievent < nevents; ievent++)
        {
            if (StringEqual(events[ievent]->leftObjective, objective))
                return 1;
            if (StringEqual(events[ievent]->rightObjective, objective))
                return 1;
        }
    }
    return 0;
}

static CutSceneDef* ReadCutSceneFromString(const char* cutSceneString)
{
    int ok;
    CutSceneDef *cutscene;

    if(!cutSceneString)
        return NULL;

    cutscene = StructAllocRaw(sizeof(CutSceneDef));
    ok = ParserReadText(cutSceneString, -1, ParseCutScene, cutscene);

    if (ok)
    {
        cutscene->flags |= CUTSCENE_FROMSTRING;
        return cutscene;
    }
    else
    {
        StructDestroy(ParseCutScene, cutscene);
        return NULL;
    }
}

void StartCutSceneFromString(const char* cutSceneString, EncounterGroup *group)
{
    CutSceneParams csp;

    memset(&csp, 0, sizeof(CutSceneParams));
    csp.cutSceneDef = ReadCutSceneFromString(cutSceneString);
    if(!csp.cutSceneDef)
    {
        Errorf("Couldn't start cutscene from string %s", cutSceneString);
        return;
    }

    if(group && group->active)
    {
        int i;
        for (i = 0; i < eaSize(&group->active->ents); i++) 
        {
            char * name;
            csp.cast[csp.castCount].entRef = erGetRef(group->active->ents[i]);
            name = EncounterActorNameFromActor(group->active->ents[i]);
            if( name )
                strncpy( csp.cast[csp.castCount].role, name, MAX_ROLE_STR_LEN-1 );

            csp.castCount++;

            assert( csp.castCount < MAX_ACTORS );
        }

        {
            EncounterPoint * point;
            int pidx = group->active->point;
            if (pidx < 0 || pidx >= group->children.size)
                assert(0);
            point = group->children.storage[pidx];
            if( point)
            {
                int i;
                for (i = 0; i <= MAX_CAMERA_MATS && i <= EG_MAX_ENCOUNTER_POSITIONS ; i++) 
                {
                    Vec3 pyr;
                    copyVec3( point->pyrpositions[i], pyr );
                    pyr[0] = -pyr[0]; //Camera points the wrong way
                    pyr[1] = addAngle( pyr[1], RAD(180) ); //Camera points the wrong way
                    pyr[2] = 0; //camera never rolls
                    createMat3YPR( csp.cameraMats[i], pyr );
                    copyVec3( point->positions[i], csp.cameraMats[i][3] );
                }
            }
        }
    }

    csp.freezeServerAndMoveCameras = 1;

    startCutScene( &csp );
}