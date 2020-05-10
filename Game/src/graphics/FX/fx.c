#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/memcheck.h>
#include "assert.h"
#include <utilitieslib/utils/utils.h>
#include "assert.h"
#include "graphics/font.h"
#include "cmdparse/cmdcommon.h"    //TIMESTEP
#include "cmdparse/cmdgame.h"
#include "seq/seq.h"
#include "graphics/FX/fx.h"
#include "fxutil.h"
#include "graphics/FX/fxlists.h"
#include "graphics/FX/fxgeo.h"
#include "fxinfo.h"
#include "graphics/camera.h"
#include "UI/uiConsole.h"
#include <utilitieslib/components/genericlist.h>
#include "group/group.h" //for lighting so I don't have to rewrite lightmodel to be more generic
#include "graphics/light.h"
#include "seq/seq.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/Earray.h>
#include "seq/seqsequence.h"
#include "entity/entclient.h" //for debug
#include "graphics/groupdraw.h"
#include "group/groupdynrecv.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiCursor.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/fileutil.h>
#include "graphics/FX/fxcapes.h"
#include "win/input.h"
#include "win/win_init.h"
#include "group/groupdyn.h"
#include "seq/seqstate.h"
#include "entity/entity.h"
#include "seq/anim.h"
#include "seq/seqskeleton.h"
#include <utilitieslib/components/mailbox.h>
#include "graphics/FX/particle.h"
#include "edit/ClickToSource.h"
#include "UI/uiUtil.h"
#include <utilitieslib/components/StashTable.h>
#include "graphics/FX/fxdebris.h"
#include "seq/tricks.h"
#include "graphics/gfx.h"

#define INITIAL_FXGEO_MEMPOOL_ALLOCATION    200
#define INITIAL_FX_MEMPOOL_ALLOCATION        100

#define TARGET_BODY_PART        1
#define TARGET_MISS_LOCATION    2
#define REGULAR_FX_INPUT        3
#define ORIGIN_BODY_PART        4
#define ORIGIN_MISS_LOCATION    5


FxEngine fx_engine;
//This is where fx report that they have hit their targets so any entity that is interested can find out
Mailbox globalEntityFxMailbox;

int maxSimultaneousFx = 0;
int fxCreatedCount = 0;
int fxDestroyedCount = 0;

//#####################################################################
//0. Debug Tools for Fxobjects (when -fxdebug)

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "player/player.h"

#ifdef NOVODEX_FLUIDS
#include "render/renderparticles.h"
#endif


static void readtfx()
{
    FILE    *file;
    char buf[1000];
    char * s;

    file = fopen("c:/tfx.txt","rt");
    if (file)
    {
        fgets(buf,sizeof(buf),file);
        s = buf + strlen(buf) - 1;
        if (*s == '\n')
            *s = 0;
        fclose(file);
        strcpy(game_state.tfx, buf);
    }
}

//Testing when you hit "_" plays fx in c:\tfx.txt on player
void fxTestFxInTfxTxt()
{
    Entity     * player;
    Entity     * target;

    //fxCreateWaterFx(player, player->mat, 0, 0 );
    //printf( "Clear\n\n");
    player = playerPtr();

    target = current_target;

    //Shoot an Effect
    if( player)
    {
        static int    fireball = 0;
        FxParams    fxp;

        fxInitFxParams(&fxp);

        //fxontarget will say play this on the guy I have targetted
        if( game_state.fxontarget && target )
        {
            fxp.keys[0].seq            = target->seq;
            fxp.keycount++;
        }
        else if( game_state.fxontarget )
        {
            //fxp.keys[0].seq            = NULL;
            //fxp.keycount++;
        }
        else
        {
            fxp.keys[0].seq            = player->seq;
            fxp.keys[0].bone        = BONEID_LARMR;
            fxp.keycount++;

            if(target)
            {
                fxp.keys[1].seq        = target->seq;
                fxp.keys[1].bone    = BONEID_CHEST;
                fxp.keycount++;
            }
            else
            {
                copyMat4( fxp.keys[0].seq->gfx_root->mat, fxp.keys[1].offset );
                fxp.keys[1].offset[3][0] += 20;
                fxp.keycount++;
            }
        }


        if( game_state.fxpower )
            fxp.power = game_state.fxpower;

        if(fireball)
            fxDelete(fireball, SOFT_KILL);

        readtfx();

        fireball = fxCreate(game_state.tfx, &fxp);
    }

    return;
}



//Have any of the files referred to by this fx file been updated since last we checked?
static int checkIfFxHasBeenUpdated(FxObject * fx)
{
    FxGeo * fxgeo;
    int older = 0, i;
    char buf[1000];
    int part_count;

    sprintf(buf,"%s%s", "fx/", fx->fxinfo->name);
    older += fileHasBeenUpdated(buf, &fx->infoage);
    for(fxgeo = fx->fxgeos ; fxgeo ; fxgeo = fxgeo->next)
    {
        if(fxgeo->bhvr)
        {
            sprintf(buf,"fx/%s", fxgeo->bhvr->name);
            older += fileHasBeenUpdated( buf, &fxgeo->bhvrage);
        }

#ifdef NOVODEX_FLUIDS
        if ( fxgeo->fluidEmitter.fluid )
        {
            sprintf(buf,"%s%s", "fx/", fxgeo->fluidEmitter.fluid->name);
            older += fileHasBeenUpdated( buf, &fxgeo->fluidage);
        }
#endif

        if(fxgeo->event)
        {
            part_count = eaSize( &fxgeo->event->part );
            for(i = 0 ; i < part_count ; i++)
            {
                sprintf(buf,"%s%s", "fx/", fxgeo->event->part[i]->params[0] );
                older += fileHasBeenUpdated( buf, &fxgeo->partage[i] );
            }
        }
        if (fxgeo->capeInst)
        {
            sprintf(buf,"%s%s", "fx/", fxgeo->capeInst->cape->name);
            older += fileHasBeenUpdated( buf, &fxgeo->capeInst->capeFileAge);
        }

    }
    return older; //number of older files
}

void fxDoForEachFx(SeqInst *seq, void(*applyFunction)(FxObject*, void*), void *data)
{
    FxObject* fx, *next;
    if(!seq || !applyFunction)
        return;

    for( fx = fx_engine.fx ; fx ; fx = next )
    {
        next = fx->next;    //this has to be here in case the fx is deleted by applyFunction
                            //in that case fx->next gets set to fx, and you end up in this loop
                            //for the rest of your life.  It is very sad.
        if(fx->handle_of_parent_seq == seq->handle)
        {
            applyFunction(fx, data);
        }
    }
}


/*Regets the file ages or all the bhvr files, particle systems, and infos
*/
static void  initFileAges(FxObject * fx)
{
    FxGeo * fxgeo;
    fx->infoage = 0;

    for(fxgeo = fx->fxgeos ; fxgeo ; fxgeo = fxgeo->next)
    {
#ifdef NOVODEX_FLUIDS
        fxgeo->fluidage =
#endif
        fxgeo->bhvrage = fxgeo->partage[0] = fxgeo->partage[1] = fxgeo->partage[2] = 0;
    }

    if( !global_state.no_file_change_check && isDevelopmentMode() )
    {
        checkIfFxHasBeenUpdated(fx); //
    }
}


static int fxCheckThatSequencersAreStillGood( FxParams * fxp )
{
    int i;

    for(i = 0 ; i < fxp->keycount  ; i++)
    {
        if( fxp->keys[i].seq ) //TO DO (add validator that this is a good sequencer?)
        {
            if( fxp->keys[i].seq->gfx_root->unique_id != fxp->keys[i].seq->gfx_root_unique_id ) //make sure all is right with this seq
            {
                //THis is a bad fx, probably created by the respawner.  Throw it away
                return 0;
            }
            //return fxp->keys[i].seq->handle;
        }
        if (fxp->keys[i].gfxnode)
        {
            // Hacky, and won't always work, but should stop it from crashing... 
            if (fxp->keys[i].gfxnode->model == (Model*)0xfafafafa)
                return 0;
        }
    }
    return 1;
}
/*Given an fx, replace it with a new copy of this effect **keeping the same handle**
Hack job to be cleaned up
*/
static void respawnFx(FxObject * fx)
{
    int        newhandle, oldhandle;
    FxObject * newfx;

    assert(fx && fx->fxinfo);
    //Record the old fx's handle
    oldhandle = fx->handle;

    //Create the new fx with old params, then delete old fx
    if( !fxCheckThatSequencersAreStillGood( &fx->fxp ) )
    {
        printToScreenLog(1, "Failed to respawn fx %s because the entity it was attached to is gone", fx->fxinfo->name);
        fxDelete( hdlGetHandleFromPtr(fx,fx->handle), HARD_KILL);
        return;
    }

    newhandle = fxCreate( fx->fxinfo->name, &fx->fxp );
    if(!newhandle)
    {
        printToScreenLog(1, "Failed to respawn fx %s", fx->fxinfo->name);
        fxDelete( hdlGetHandleFromPtr(fx,fx->handle), HARD_KILL);
        return;
    }
    newfx = hdlGetPtrFromHandle(newhandle);
    fxDelete( hdlGetHandleFromPtr(fx,fx->handle), HARD_KILL);

    //copy the ptr to the new fx over the ptr to the old fx in the old slot,
    //recover the old ID to the old slot, and clear out the new slot
    hdlMoveHandlePtr(oldhandle, newhandle);
    hdlClearHandle(newhandle);

    //set the new fx's internal record of it's handle back to the old handle
    newfx->handle = oldhandle;

    //newhandle is now garbage, and oldhandle holds the new fx
    assert(newfx == hdlGetPtrFromHandle(newfx->handle));
}


static void fxDoCheckForUpdatedFx(FxObject * firstfx)
{
    FxObject * fx;
    FxObject * fxnext;

        for( fx = fx_engine.fx ; fx ; fx = fxnext )
        {
            fxnext = fx->next;
            if( checkIfFxHasBeenUpdated(fx) )
                respawnFx(fx);
        }
}


typedef struct FxConsolidator
{
    int count;
    int particleCount;
    int particleSystemCount;
    int fxGeoCount;
    int iFramesLeft;
    StashTable soundTable;
    int iNumUniqueSounds;
} FxConsolidator;

static StashTable fxDebugInfoTable;

#define FX_DEBUG_FRAMES_DEFAULT 300
extern int debug_fxgeo_count;
static void fxPrintDebugInfo()
{
    if( game_state.fxdebug & FX_DEBUG_BASIC || game_state.renderinfo)
    {
        xyprintf(0,480/8-3 + TEXT_JUSTIFY,"FX Systems: % 4d", fx_engine.fxcount);
        xyprintf(0,480/8-2 + TEXT_JUSTIFY,"FX Geos   : % 4d", debug_fxgeo_count);
        #if NOVODEX
            xyprintf(0,480/8-1 + TEXT_JUSTIFY,"FX Debris : % 4d / % 4d", fxGetDebrisObjectCount(), fxGetDebrisMaxObjectCount());
        #endif
        //xyprintf(0,480/8-8 + TEXT_JUSTIFY,"FX Infos: % 4d", fx_engine.fxinfocount);
        //xyprintf(0,480/8-7 + TEXT_JUSTIFY,"FX Bhvrs: % 4d", fx_engine.fxbhvrcount);
    }
    if( game_state.fxdebug & FX_SHOW_DETAILS )
    {
        int i = 0, j = 0;
        FxGeo * fxgeo;
        FxObject * fx = 0;
        FxObject * myfx = 0;
        FxObject * detail_fx = 0;
        int fxcount, psystotal, ptotal;
        U8 r,g,b;
        char fxStages[4][100];

        if (!fxDebugInfoTable)
        {
            fxDebugInfoTable = stashTableCreateWithStringKeys(256, StashDefault);
        }

        strcpy( fxStages[FX_FULLY_ALIVE],                    "ALIVE  " );
        strcpy( fxStages[FX_GENTLY_KILLED_PRE_LAST_UPDATE], "DYING_P" );
        strcpy( fxStages[FX_GENTLY_KILLED_POST_LAST_UPDATE],"DYING  " );
        strcpy( fxStages[FX_FADINGOUT],                        "FADING " );

        fxcount = 1;
        psystotal = 0;
        ptotal = 0; 

        {
            FxObject * fx = 0;

            // reset consolidators
            StashTableIterator iter;
            StashElement elem;
            fxcount = 0;
            stashGetIterator(fxDebugInfoTable, &iter);
            while ( stashGetNextElement(&iter, &elem) )
            {
                FxConsolidator* pConsolidator = stashElementGetPointer(elem);
                pConsolidator->count = 0;
                pConsolidator->fxGeoCount = 0;
                pConsolidator->particleCount = 0;
                pConsolidator->particleSystemCount = 0;
            }

            for( fx = fx_engine.fx ; fx ; fx = fx->next )
            {
                FxConsolidator* pConsolidator;
                if ( !stashFindPointer(fxDebugInfoTable, fx->fxinfo->name, &pConsolidator) )
                {
                    // isn't already in the stash table
                    pConsolidator = calloc(sizeof(FxConsolidator), 1);
                    // add it to the table
                    stashAddPointer(fxDebugInfoTable, fx->fxinfo->name, pConsolidator, false);

                    pConsolidator->soundTable = NULL;

                    // figure out how many sounds there are in this fx
                    {
                        int i, j, k, num;
                        num = eaSize(&fx->fxinfo->conditions);
                        for( i = 0; i < num; i++ )
                        {
                            FxCondition *fxcon = fx->fxinfo->conditions[i];

                            int numevents = eaSize(&fxcon->events);
                            for( j = 0; j < numevents; j++ )
                            {
                                FxEvent *fxevent = fxcon->events[j];
                                int iNumSounds = eaSize(&fxevent->sound);
                                for (k=0; k<iNumSounds; ++k)
                                {
                                    SoundEvent* pSound = fxevent->sound[k];
                                    if ( !pConsolidator->soundTable )
                                        pConsolidator->soundTable = stashTableCreateWithStringKeys(8, StashDeepCopyKeys);
                                    if ( stashAddInt(pConsolidator->soundTable, pSound->soundName, 1, false) )
                                        pConsolidator->iNumUniqueSounds++;
                                }
                            }
                        }
                    }
                }



                pConsolidator->count++; // add this fx object
                pConsolidator->fxGeoCount += fx->fxgeo_count; // add the fx geos

                // add the particles
                psystotal = 0;
                ptotal = 0;
                for( fxgeo = fx->fxgeos ; fxgeo ; fxgeo = fxgeo->next )
                {
                    for( j = 0 ; j < MAX_PSYSPERFXGEO ; j++ )
                    {
                        if( partConfirmValid( fxgeo->psys[j], fxgeo->psys_id[j] ))
                        {
                            psystotal++;
                            ptotal += fxgeo->psys[j]->particle_count;
                        }
                    }
                }


                pConsolidator->particleSystemCount += psystotal;
                pConsolidator->particleCount += ptotal;

                // record how many frames we have left to display this
                pConsolidator->iFramesLeft = FX_DEBUG_FRAMES_DEFAULT;
            }
        }

        {
            StashTableIterator iter;
            StashElement elem;
            fxcount = 0;
            stashGetIterator(fxDebugInfoTable, &iter);
            while ( stashGetNextElement(&iter, &elem) )
            {
                FxConsolidator* pConsolidator = stashElementGetPointer(elem);

                // don't draw if it's out of frames
                if ( !pConsolidator->iFramesLeft )
                    continue;
                r = g = b = 255;
                if ( pConsolidator->iFramesLeft != FX_DEBUG_FRAMES_DEFAULT )
                {
                    // must be empty
                    pConsolidator->count = 0;
                    pConsolidator->fxGeoCount = 0;
                    pConsolidator->particleCount = 0;
                    pConsolidator->particleSystemCount = 0;
                    r = 165.0 + 90.0 * ((F32)pConsolidator->iFramesLeft / (F32)FX_DEBUG_FRAMES_DEFAULT);
                    g = b = 165.0 * (1.0f - ((F32)pConsolidator->iFramesLeft / (F32)FX_DEBUG_FRAMES_DEFAULT));
                }
                fxcount++;
                if ( game_state.fxdebug & FX_SHOW_SOUND_INFO )
                    fxcount += pConsolidator->iNumUniqueSounds;

                xyprintfcolor(40,480/8 - fxcount + TEXT_JUSTIFY,r,g,b, "#%d",pConsolidator->count );
                xyprintfcolor(46,480/8 - fxcount + TEXT_JUSTIFY,r,g,b, "fxg:%d",pConsolidator->fxGeoCount);
                xyprintfcolor(55,480/8 - fxcount + TEXT_JUSTIFY,r,g,b, "ps:%d", pConsolidator->particleSystemCount );
                xyprintfcolor(63,480/8 - fxcount + TEXT_JUSTIFY,r,g,b, "pt:%d", pConsolidator->particleCount );
                xyprintfcolor(72,480/8 - fxcount + TEXT_JUSTIFY,r,g,b, "%s", stashElementGetStringKey(elem) );

                if ( game_state.fxdebug & FX_SHOW_SOUND_INFO && pConsolidator->soundTable )
                {
                    StashTableIterator iter;
                    StashElement elem;
                    int iSoundPrinted = 0;

                    assert( pConsolidator->iNumUniqueSounds > 0);

                    stashGetIterator(pConsolidator->soundTable, &iter);
                    while (stashGetNextElement(&iter, &elem))
                    {
                        iSoundPrinted++;
                        xyprintfcolor(50,480/8 - fxcount + TEXT_JUSTIFY + iSoundPrinted,0,255,0, "%s", stashElementGetStringKey(elem) );
                    }
                }

                pConsolidator->iFramesLeft--;
                /*
                if (game_state.fxdebug & FX_ENABLE_DEBUG_UI)
                {
                    int x, y, w, h;
                    int fontWidth = 8;
                    int fontHeight = 8;
                    inpMousePos(&x,&y);
                    windowSize(&w,&h);

                    y = h - y;
                    if (!inpIsMouseLocked() && x >= 72*fontWidth && (x <= (int)(72+100)*fontWidth) && 
                        y >= (fxcount-1)*fontHeight && y < (fxcount-1+1)*fontHeight)
                    {
                        if(inpEdge(INP_LBUTTON))
                        {
                            fxDelete(fx->handle, SOFT_KILL);
                        }
                        r = 255;
                        g = 255;
                        b = 50;
                    }
                }
                */
            }
        }

        if( detail_fx )
        {
            fx = detail_fx;
            xyprintfcolor(2,480/8-70+TEXT_JUSTIFY,100,255,255, "%s", fx->fxinfo->name ); 
            xyprintfcolor(2,480/8-69+TEXT_JUSTIFY,100,255,255, "Net_Id:    %d", fx->net_id );
            xyprintfcolor(2,480/8-68+TEXT_JUSTIFY,100,255,255, "Age:       %0.0f", fx->age );
            xyprintfcolor(2,480/8-67+TEXT_JUSTIFY,100,255,255, "lifeStage: %d", fx->lifeStage );
            xyprintfcolor(2,480/8-66+TEXT_JUSTIFY,100,255,255, "Duration:  %0.0f", fx->duration );
            xyprintfcolor(2,480/8-65+TEXT_JUSTIFY,100,255,255, "Radius:    %0.0f", fx->radius );
            xyprintfcolor(2,480/8-64+TEXT_JUSTIFY,100,255,255, "Power:     %d", fx->power );
            xyprintfcolor(2,480/8-62+TEXT_JUSTIFY,100,255,255, "FxGeos:    %d", fx->fxgeo_count );

        }
    }
}

void fxClearPartHigh()
{
    FxObject * fx;
    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        fx->pmax_debug = 0;
    }
}

void fxSelectNextFxForDetailedInfo()
{
    FxObject * fx;
    int done = 0;
    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        if( fx->curr_detail_fx )
        {
            fx->curr_detail_fx = 0;
            done = 1;
            if( fx->prev )
                fx->prev->curr_detail_fx = 1;
        }
    }
    if( fx_engine.fx && !done )
    {
        //screwy
        for( fx = fx_engine.fx ; fx ; fx = fx->next )
            if(!fx->next)
                fx->curr_detail_fx = 1;
    }
}




typedef struct FxDebugInfo
{
    char name[256];
    int count;
} FxDebugInfo;


static int cmpFxDebugInfo(const FxDebugInfo* d1, const FxDebugInfo* d2)
{
    if( d1->count != d2->count)
        return (d2->count - d1->count);
    return strcmp((d1)->name, (d2)->name);
}


#define FX_INFO_MAX_COUNT 1000
#define FX_BUF_SIZE 2000
char * fxPrintFxDebug()
{
    static char buf[FX_BUF_SIZE];
    FxDebugInfo fxI[FX_INFO_MAX_COUNT];
    int fxICount;
    FxObject * fx;
    FxDebugInfo * myFxI = NULL;
    int i, found;

    fxICount = 0;
    memset( fxI, 0, FX_INFO_MAX_COUNT * sizeof(FxDebugInfo) );
    memset( buf, 0, FX_BUF_SIZE);

    //Build list of fx:
    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        found = 0;
        for( i = 0 ; i < fxICount ; i++  )
        {
            if( stricmp( fx->fxinfo->name, fxI[i].name ) == 0 )
            {
                found = 1;
                myFxI = &fxI[i];
                break;
            }
        }
        if( !found )
            myFxI = &fxI[ fxICount++ ];

        strcpy( myFxI->name, fx->fxinfo->name );
        myFxI->count++;

        if( fxICount >= FX_INFO_MAX_COUNT )
            break;
    }

    qsort( fxI, fxICount, sizeof( FxDebugInfo ), cmpFxDebugInfo );

    //Print list to string
    for( i = 0 ; i < fxICount ; i++ )
    {
        char cTempBuf[256];

        sprintf( cTempBuf, "%d%s \n", fxI[i].count, fxI[i].name );

        if ( strlen(buf) + strlen(cTempBuf) >= FX_BUF_SIZE )
            break;

        strcat(buf, cTempBuf);
    }

    return buf;
}

//##### End Debug/Testing Tools##################################################




//##### Other Tools##################################################

/*Instructs the mailbox to notify this mailbox when an event sets this flag
this function is not finished as it always assumes primehit is the event*/
/*void fxNotifyMailboxOnEvent(int fxid, Mailbox * mailbox, int flag) //TO DO savage hack
{
    FxObject * myfx;

    myfx = hdlGetPtrFromHandle(fxid);

    if(myfx && mailbox && mailbox->id)
    {
        myfx->notify_this_address_when_primehit.mailbox = mailbox;
        myfx->notify_this_address_when_primehit.mailbox_id = mailbox->id;
    }
}*/

FxGeo * getOldestFxGeo( FxObject * fx )
{
    FxGeo * fxgeo = 0;
    if( fx && fx->fxgeos )
    {
        fxgeo = fx->fxgeos;
        while( fxgeo->next )
            fxgeo = fxgeo->next;
    }
    return fxgeo;
}

/*Hacky, knows that first fxgeo is the root fxgeo, so just changed it's mat.  Since there
really isn't the formal concept of a root for an fx, which can have multiple connection
points to the world.  But World FX only have one always, so they use this
*/
void fxChangeWorldPosition( int fx_id, Mat4 mat)
{
    FxObject * fx;
    FxGeo * fxgeo;

    fx = hdlGetPtrFromHandle( fx_id );
    if( fx )
    {
        fxgeo = getOldestFxGeo( fx );
        if(fxgeo)
        {
            if( gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id ) )
                copyMat4(mat, fxgeo->gfx_node->mat);
        }
    }
}

//Simple query by somebody who has a fx handle, and is curious
int fxIsFxAlive(int fxid)
{
    return (fxid && hdlGetPtrFromHandle(fxid)) ? 1 : 0;
}

//authorize another x time for this fx.  Only good for fx that depend on their fx ->duration to
//tell them when to die.
//TO DO should I tie extending duration to the parent bits thing?  That might be cleaner
// This function should no longer get called, cuz the work it does is now done on the server.
void fxExtendDuration(int fxid, F32 added_duration)
{
    FxObject * fx = 0;
    if(fxid)
        fx = hdlGetPtrFromHandle(fxid);

    if(fx)
    {
        fx->duration = MAX((fx->age + added_duration), fx->duration);
    }
}

//#######################################################################
//1. #########Create An FX

//Use: 1. Make an FxParams and Init it.  2.  Fill it up with info  3. Pass it into fxcreate
void fxInitFxParams( FxParams * fxparams )
{
    int i;

    memset(fxparams, 0, sizeof(FxParams));
    for(i = 0 ; i < MAX_DUMMYINPUTS ; i++ )
        copyMat4( unitmat, fxparams->keys[i].offset );
    fxparams->power = 10;                                //full power
    fxparams->fxtype = FX_POWERFX; // Unless otherwise specified
}

/*Kind of screwy, but I probably won't change this.
It goes through the FxInfo's inputs, adding a new key to FxParam's keys whenever it finds an Input
that it recognizes as the name of a bone or ROOT.  Basically it pretends the caller had known he wanted
that bodypart to be a key, and makes one. It uses the First Sequencer given as the seq to find the bones on
origin (ex. CHEST). It uses the second sequencer for target bones (ex. T_CHEST).
*/
static void fxFindInputs(FxInfo * fxinfo, FxParams * fxp)
{
//    FxInput * fxinput;
    int i = 0;
    BoneId bone;
    FxParams fxpold;
    SeqInst * seq = NULL;
    char * inputname;
    int fxi, numinputs;
    int type = 0;

    if(fxp->dontRewriteKeys)
        return;

    fxpold = *fxp;

    //Clear out all the keys in the fxp
    for(i = 0 ; i < MAX_DUMMYINPUTS ; i++ )
    {
        memset(&(fxp->keys[i]), 0, sizeof(FxKey));
        copyMat4( unitmat, fxp->keys[i].offset );
    }
    fxp->keycount = 0;

    //Refill the keys, adding new ones as necessary
    numinputs = fxinfo->inputsRealCount; //eaSize(&fxinfo->inputs);
    i = 0;
    for (fxi = 0; fxi < numinputs; fxi++)
    {
        bool childBone=false;
        //fxinput = fxinfo->inputs[fxi];

        inputname = fxinfo->inputsReal[ fxi ]; //fxinput->name;
        type = 0;

        //1 Figure out what you've got:
        if(strnicmp(inputname, "T_", 2) == 0) ////So inpname 'T_CHEST' means add a key at the target's chest called T_CHEST
        {
            bone = bone_IdFromText(inputname + 2);

            if( bone == BONEID_INVALID && 0 != stricmp( &inputname[strlen(inputname)-4], "ROOT" ) )
            {
                type = REGULAR_FX_INPUT; //shouldn't happen
            }
            else
            {
                if( fxpold.keys[1].seq ) //This means I wasn't given an target seq. Possibly this is a miss, so use the position given
                    type = TARGET_BODY_PART;
                else
                    type = TARGET_MISS_LOCATION;
            }
        }
        else if (strnicmp(inputname, "C_", 2) == 0) //inpname 'C_Chest' means add a key at a child's chest
        {
            bone = bone_IdFromText(inputname + 2);
            childBone = true;

            if( bone == BONEID_INVALID && 0 != stricmp( &inputname[strlen(inputname)-4], "ROOT" ) )
            {
                type = REGULAR_FX_INPUT;
            }
            else
            {
                if( fxpold.keys[0].seq )  //This means I wasn't given an origin seq. Possibly this is a miss, so use the position given
                    type = ORIGIN_BODY_PART;
                else
                    type = ORIGIN_MISS_LOCATION;
            }
        }
        else //inpname 'Chest' means add a key at the origin's chest called Chest
        {
            bone = bone_IdFromText(inputname);

            if( bone == BONEID_INVALID && 0 != stricmp( &inputname[strlen(inputname)-4], "ROOT" ) )
            {
                type = REGULAR_FX_INPUT;
            }
            else
            {
                if( fxpold.keys[0].seq )  //This means I wasn't given an origin seq. Possibly this is a miss, so use the position given
                    type = ORIGIN_BODY_PART;
                else
                    type = ORIGIN_MISS_LOCATION;
            }
        }


        //2. Deal with it appropriately:
        assert( type );
        fxp->keys[fxp->keycount].childBone = 0;
        if( type == ORIGIN_BODY_PART || type == TARGET_BODY_PART )
        {
            if( type == TARGET_BODY_PART )
                seq = fxpold.keys[1].seq;
            else if ( type == ORIGIN_BODY_PART )
                seq = fxpold.keys[0].seq;

            assert( seq );

            if(    bone != BONEID_INVALID )
            {
                fxp->keys[fxp->keycount].seq = seq;
                fxp->keys[fxp->keycount].bone = bone;
                fxp->keys[fxp->keycount].childBone = childBone;
            }
            else if( 0 == stricmp(&inputname[strlen(inputname)-4], "ROOT") )
            {
                fxp->keys[fxp->keycount].seq = seq;
                fxp->keys[fxp->keycount].bone = 0;  //attach to hips, later subtract off hips
            }
            else
            {
                assert(0);
            }
        }
        else if( type == ORIGIN_MISS_LOCATION )
        {
            copyMat4( fxpold.keys[0].offset, fxp->keys[fxp->keycount].offset );
        }
        else if( type == TARGET_MISS_LOCATION )
        {
            copyMat4( fxpold.keys[1].offset, fxp->keys[fxp->keycount].offset );
        }
        else if( type == REGULAR_FX_INPUT )
        {
            assert(i <= fxpold.keycount);
            fxp->keys[fxp->keycount] = fxpold.keys[i]; //copy struct
            i++;
        }

        fxp->keycount++;
    }
}

//Get parent of this fx.  (Kinda hacked, ?someday formalize: (fxp->origin, fxp->target, etc)
static int fxGetHandleOfIthSeqInFxParams( FxParams * fxp, int index )
{
    int i;

    for(i = 0 ; i < fxp->keycount  ; i++)
    {
        if( fxp->keys[i].seq ) //TO DO (add validator that this is a good sequencer?)
        {
            assert( fxp->keys[i].seq->gfx_root->unique_id == fxp->keys[i].seq->gfx_root_unique_id ); //make sure all is right with this seq
            if (index==0)
                return fxp->keys[i].seq->handle;
            index--;
        }
    }
    return 0;
}

//Returns 1, keep this fx, or 0, throw it away
static int fxDoRadiusCulling( FxParams * fxp, F32 radius )
{
    Vec3 fxPos;
    const F32 * playerPos = 0;
    int keepThisFx = 1;
    Vec3 dv;

    if( !fxp->keycount )//This is a bad fx, let something else flag it as so.
        return 1;

    //Get Player position
    if( game_state.viewCutScene )
    {
        playerPos = game_state.cutSceneCameraMat[3];
    }
    else
    {
        Entity * player;
        player = playerPtr();
        if( player )
            playerPos = ENTPOS(playerPtr());
        if( !playerPos )
            return 0;
    }

    //Get Fx Position TO DO This attempt to get the position of the fx is a bit hacky
    {
        FxKey * rootKey;
        rootKey = &fxp->keys[0];
        if( rootKey->seq  )
            copyVec3( rootKey->seq->gfx_root->mat[3], fxPos );
        else if( rootKey->gfxnode )
            return 1; //Dont cull on a gfx node with no sequencer right now.  Maybe do it in the future
        else
            copyVec3( rootKey->offset[3], fxPos );
    }

    //Test
    subVec3( fxPos, playerPos, dv );
    if( lengthVec3Squared( dv ) > ( radius * radius ) )
        return 0;
    else
        return 1;
}

// Call this function only when dealing with animal rig (check for NONHUMAN flag in enttype)
static int getAnimalRigBoneSubstitute(origBoneId)
{
    int substBoneId = BONEID_HIPS; // in case don't find a substitute, default to hips

    switch(origBoneId) {
        case BONEID_UARMR: substBoneId = BONEID_FORE_ULEGR; break;
        case BONEID_UARML: substBoneId = BONEID_FORE_ULEGL; break;
        case BONEID_LARMR: substBoneId = BONEID_FORE_LLEGR; break;
        case BONEID_LARML: substBoneId = BONEID_FORE_LLEGL; break;
        case BONEID_HANDR: substBoneId = BONEID_FORE_FOOTR; break;
        case BONEID_HANDL: substBoneId = BONEID_FORE_FOOTL; break;
        case BONEID_ULEGR: substBoneId = BONEID_HIND_ULEGR; break;
        case BONEID_ULEGL: substBoneId = BONEID_HIND_ULEGL; break;
        case BONEID_LLEGR: substBoneId = BONEID_HIND_LLEGR; break;
        case BONEID_LLEGL: substBoneId = BONEID_HIND_LLEGL; break;
        case BONEID_FOOTR: substBoneId = BONEID_HIND_FOOTR; break;
        case BONEID_FOOTL: substBoneId = BONEID_HIND_FOOTL; break;
        case BONEID_TOER: substBoneId = BONEID_HIND_TOER; break;
        case BONEID_TOEL: substBoneId = BONEID_HIND_TOEL; break;
    }

    return substBoneId;
}

/*Core fx create function, the ones all the other fxcreates use*/
int fxCreate( const char * fxname, FxParams * originalfxp )
{
    FxObject *    fx;
    FxInfo   *    fxinfo;
    //FxInput  *    fxinput;
    char    *   inputName;
    FxEvent        event;
    FxParams *  fxp;
    GfxNode  *    parentofkeygfxnode;
    int            fxgeoid;
    int            i;
    int            fxi;
    int            iSizeInputs;
    int            okToCreate;
    int         fxHandle;


    PERFINFO_AUTO_START("fxCreate", 1);
    
    if (game_state.tintFxByType && !originalfxp->numColors)
    {
        originalfxp->numColors = 1;
        originalfxp->colors[0][0] = 0;
        originalfxp->colors[0][1] = 0;
        originalfxp->colors[0][2] = 5;
        originalfxp->colors[0][3] = 255;
    }

    if( !originalfxp->power )
        originalfxp->power = 10;

    //###1. Get an fx and assign an fxinfo and fxparams to it
    fxinfo    = fxGetFxInfo(game_state.nofx?"GENERIC/dummy.fx":fxname);
    if(!fxinfo || !fxname)
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }

    if(    fxinfo->fxinfo_flags&FX_DONT_INHERIT_TEX_FROM_COSTUME )
    {
        originalfxp->textures[0] = 0;
        originalfxp->textures[1] = 0;
        originalfxp->textures[3] = 0;
    }

    //THis is a perfectly correct reason to not make an effect, and should not be flagged as an error
    //by other systems.  TO DO make sure no other system complains when this is returned.
    if( fxinfo->performanceRadius && !fxDoRadiusCulling( originalfxp, fxinfo->performanceRadius ))
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }

    fx = (FxObject*)mpAlloc(fx_engine.fx_mp);
    listAddForeignMember(&fx_engine.fx, fx);
    assert(fx);
    fx_engine.fxcount++;
    listScrubMember( fx, sizeof(FxObject) );
    fx->handle = hdlAssignHandle(fx);
    assert(fx->handle);

    fx->lifeStage = FX_FULLY_ALIVE;
    fx->fxinfo = fxinfo;

    fx->frameLastCondition = NULL;
    {
        U32 uiNumConds = eaSize(&fxinfo->conditions);
        if ( uiNumConds )
        {
            fx->frameLastCondition = calloc(sizeof(F32), uiNumConds);
        }
    }

    //Tricky stuff to handle allowing fxinfo to refer to bones without the fxp giving them as inputs
    //    (An excellent example of something I kinda hacked in for artist convenience that is now absolutely
    //    written in stone and necessary. It also makes the concept of parent seq necessary, which is also now
    //    written in stone)
    fx->fxp = *originalfxp;
    fxFindInputs( fxinfo, &fx->fxp );  //rewrites fx's fxp using fxinfo TO DO:rationalize
    fxp = &fx->fxp;
    if( originalfxp->duration )
        fx->duration = originalfxp->duration;
    else
        fx->duration = fxinfo->lifespan;

    //set stuff in fx based on fxp.
    fx->radius                  = fxp->radius;
    fx->power                  = fxp->power;
    fx->net_id                  = fxp->net_id;
    fx->externalWorldGroupPtr = fxp->externalWorldGroupPtr; //using a pointer.  should I use something else?

    //Look inside the original fxp for a sequencer.  Take the first one you find and assign it as
    //the parent_seq (so you can later read off animation state from it.)
    //Also used now so we don't recalculate the lighting for stuff you hold
    fx->handle_of_parent_seq = fxGetHandleOfIthSeqInFxParams( originalfxp, 0 );
    fx->handle_of_target_seq = originalfxp->targetSeqHandle;
    fx->last_move_of_parent = -1;

    //if this guy needs to calc it's own lighting, give it the tools
    //otherwise the lighting seqGfxData is the parent seqs, or nothing
    if( fxinfo->lighting == FX_DYNAMIC_LIGHTING )
    {
        fx->seqGfxData = calloc( 1, sizeof(SeqGfxData) );
    }

    //Set update state (later turn-offable if it's a WorldFx out of view )
    fx->currupdatestate = FX_UPDATE_AND_DRAW_ME;
    fx->visibility = FX_VISIBLE;


    //###2. Match the fxinfo's expected inputs with the fxparam's given inputs, creating a KEY fxgeo for each one
    iSizeInputs = fxinfo->inputsRealCount; //eaSize(&fxinfo->inputs);
    if ( fxp->keycount < iSizeInputs ) //error check
    {
        printToScreenLog(1,"FX: %s needs %d inputs, got %d",fxinfo->name,iSizeInputs,fxp->keycount);
        PERFINFO_AUTO_STOP();
        return 0;
    }

    //TO DO, did the new parser change the way these are evaluated? I dont think so...
    //for(i = 0; fxi >= 0 && i < fxp->keycount; i++, fxi--)
    iSizeInputs = fx->fxinfo->inputsRealCount; //eaSize(&fx->fxinfo->inputs);
    for(i = 0, fxi = 0; fxi < iSizeInputs && i < fxp->keycount; i++, fxi++)
    {
        okToCreate = 1;
        
        inputName = fx->fxinfo->inputsReal[ fxi ]; //fx->fxinfo->inputs[fxi];

        //Find the KeyGfxNode, if any, that will be this fxgeo's gfxnode's parent
        // JE: Changed this to use gfxnodes as higher priority so that ChildFx spawn at the right place (since ChildFx now get the sequencers passed on down)
        if(!fxp->keys[i].gfxnode && fxp->keys[i].seq && bone_IdIsValid( fxp->keys[i].bone ) ) //a:if you were give seq and bone, get the gfxnode
        {
            parentofkeygfxnode = seqFindGfxNodeGivenBoneNumOfChild(fxp->keys[i].seq, fxp->keys[i].bone, fxp->keys[i].childBone);

            if( 0 && !parentofkeygfxnode )    //should never happen anymore, but Steve doesn't want an error msg
                                            // DG - this can happen with the advent of Kheldians, if we try to 
                                            // attach an FX to their non-existant hands
            {
                Errorf("FX %s:%s doesn't have %s bone for ", fxname, "Hmm",
                    bone_NameFromId( fxp->keys[i].bone ) );
            }

            if( !parentofkeygfxnode )
            {
                //if (originalfxp->vanishIfNoBone)
                //{
                //    okToCreate = 0;
                //}
                //else
                {
                    int substituteBoneId = BONEID_HIPS; //substitute hips for missing bone

                    // special case for animal rigs, substitute the animal equivalent for arm/leg bones
                    if(TSTB(fxp->keys[i].seq->type->constantState, STATE_NONHUMAN)) {
                        substituteBoneId = getAnimalRigBoneSubstitute(fxp->keys[i].bone);
                        //if(substituteBoneId != BONEID_HIPS) {
                        //    printf("Substituting bone %d for bone %d for fx %s, sequencer %s\n", substituteBoneId, fxp->keys[i].bone, fxname, fxp->keys[i].seq->type->name);
                        //}
                    }
                    parentofkeygfxnode = seqFindGfxNodeGivenBoneNumOfChild(fxp->keys[i].seq, substituteBoneId, fxp->keys[i].childBone);
                }
            }
            //if( okToCreate && !parentofkeygfxnode && fxp->keys[i].childBone )
            if( !parentofkeygfxnode && fxp->keys[i].childBone )
            {
                parentofkeygfxnode = seqFindGfxNodeGivenBoneNumOfChild(fxp->keys[i].seq, BONEID_HIPS, 0); //use hips
            }
        }
        else //b: if you are given just a gfxnode, (or nothing), use that
        {
            if (fxp->keys[i].gfxnode || i==0 || !fxp->keys[0].gfxnode)
                parentofkeygfxnode = fxp->keys[i].gfxnode;
            else {
                // Fixes ChildFX problems, both V_COV\Powers\Necromacy\SummonHit.fx and WORLD\City\RunwayLights.fx
                parentofkeygfxnode = fxp->keys[0].gfxnode;
            }
        }

        if (okToCreate)
        {
            //Build a temporary event to create this key fxgeo
            memset(&event, 0, sizeof(event));
            event.bhvr = "behaviors/null.bhvr";
#ifdef NOVODEX_FLUIDS
            event.fluid = NULL;
#endif
            event.name = inputName;
            event.type = FxTypeCreate;
            event.inherit = 0;
            event.update = FX_POSITION | FX_ROTATION | FX_SCALE; // here!

            //Create the Key FxGeo
            fxgeoid = fxGeoCreate( &fx->fxgeos,
                &fx->fxgeo_count, parentofkeygfxnode,
                fxp->keys[i].offset,
                &event,
                FXGEO_IS_KEY,
                fxinfo,
                FXGEO_NOLIGHTTRACKER );
        }
    }

    // Calculate base Hue of FX system    if (originalfxp->isUserColored)
    if (originalfxp->isUserColored)
    {
        assert(originalfxp->numColors == 2);
    }
    else if (originalfxp->numColors > 0)
    {
        fxInfoCalcEffectiveHue(fx->fxinfo);
    }

    //debug : init the new fx file's older
    //if( game_state.fxdebug & FX_DEBUG_BASIC )
        initFileAges(fx);

    fxCreatedCount++;
    //end debug

    // Due to the code that prevents "unattachable" fx from starting, it's possible to get here with an fx that has no fxgeos
    // in it.  Due to code elsewhere that assumes (somewhat reasonably) that there's always at least one fxgeo, we have to
    // delete the fx now if it's been ceated empty.  Returning zero appears to be an acceptable error return value.  If this
    // causes problems, there are two possible solutions: 1. feep fixing things till it works, or 2. create a blank fx as
    // a default placeholder for thesem and play that guy instead.
    if (fx->fxgeos == NULL)
    {
        assert(fx->fxgeo_count == 0);
        fxDelete(hdlGetHandleFromPtr(fx, fx->handle), HARD_KILL);
        fxHandle = 0;
    }
    else
    {
        assert(fx->fxgeo_count != 0);
        fxHandle = fx->handle;
    }

    fx->last_day_time = server_visible_state.time;
    fx->day_time = server_visible_state.time;

    PERFINFO_AUTO_STOP();

    return fxHandle;
}

// Only works for a few FX types currently
void fxChangeParams( int fxid, FxParams * fxp )
{
    FxGeo * fxgeo;
    FxObject * fx;

    fx = hdlGetPtrFromHandle(fxid);

    if( fx  )
    {
        if(fx->fxinfo && fx->fxinfo->fxinfo_flags&FX_DONT_INHERIT_TEX_FROM_COSTUME)
        {
            fxp->textures[0] = 0;
            fxp->textures[1] = 0;
            fxp->textures[3] = 0;
        }
        if (memcmp(&fx->fxp, fxp, sizeof(fx->fxp))==0)
            return;
        for(fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next)
        {
            if( fxgeo->capeInst )
                fxCapeInstChangeParams(fxgeo->capeInst, fxp);
            fxGeoParticlesChangeParams(fxgeo, fxp, fx->fxinfo);
            fxGeoAnimationChangeParams(fxgeo, fxp);

            // update customtint colors?
            if(fxgeo->bhvr->bTintGeom)
            {
                // check fx->fxp.isUserColored?
                fxgeo->customTint.primary.r = fxp->colors[0][0];
                fxgeo->customTint.primary.g = fxp->colors[0][1];
                fxgeo->customTint.primary.b = fxp->colors[0][2];
                fxgeo->customTint.secondary.r = fxp->colors[1][0];
                fxgeo->customTint.secondary.g = fxp->colors[1][1];
                fxgeo->customTint.secondary.b = fxp->colors[1][2];
            }

            if (fxgeo->childfxid) {
                fxChangeParams(fxgeo->childfxid, fxp);
            }
        }
        fx->fxp = *fxp;
    }
}


void fxFadeOut( int fxid )
{
    FxGeo * fxgeo;
    FxObject * fx;

    fx = hdlGetPtrFromHandle(fxid);

    if( fx && fx->lifeStage != FX_FADINGOUT )
    {
        for(fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next)
        {
            if( fxgeo->lifeStage == FXGEO_ACTIVE )
                fxGeoFadeOut( fxgeo );
        }
        fx->lifeStage = FX_FADINGOUT;
    }
}


void fxGentlyKill( int fxid )
{
    FxObject * fx;

    fx = hdlGetPtrFromHandle(fxid);

    if( fx && fx->lifeStage == FX_FULLY_ALIVE )
    {
        fx->fxobject_flags = FX_STATE_DEATH;
        if( !fx->fxinfo->hasEventsOnDeath )
        {
            fxFadeOut( fxid ); //this may not be necessary, it's kinda clunky
        }
        else
        {
            fx->lifeStage = FX_GENTLY_KILLED_PRE_LAST_UPDATE;
        }
    }
}
//##############################################################
//2. Update the Fx.

static void fxEventDestroy(FxObject * fx, FxEvent * fxevent )
{
    FxGeo * fxgeo;

    if( !_stricmp(fxevent->name, "All" ) )
    {
        fxGentlyKill( fx->handle );
    }
    else
    {
        fxgeo = findFxGeo( fx->fxgeos, fxevent->name );
        if( fxgeo && fxgeo->lifeStage == FXGEO_ACTIVE )
            fxGeoFadeOut( fxgeo );
    }
}

static void fxEventSetBits(FxObject * fx, FxEvent * fxevent )
{
    FxGeo * fxgeo;
    char stateNames[256];

    if (!fxevent->name) {
        printToScreenLog(1, "Event type SetState with no EName");
        return;
    }

    stateNames[0] = 0;
    strcpy( stateNames, fxevent->newstate );

    fxgeo = findFxGeo( fx->fxgeos, fxevent->name );

    if(fxgeo)
    {
        if( fxgeo->seq_handle )
        {
            SeqInst * seq;
            seq = hdlGetPtrFromHandle( fxgeo->seq_handle );
            
            if(seq)
                seqSetStateFromString( seq->state, stateNames );
            else
                fxgeo->seq_handle = 0;
        }

        if( fxgeo->childfxid )
        {
            FxObject * childfx;
            childfx = hdlGetPtrFromHandle( fxgeo->childfxid );
            if( childfx )
                seqSetStateFromString( childfx->state, stateNames );
            else
                fxgeo->childfxid = 0;
        }
    }
}


static int fxEventCreateFxGeo(FxObject * fx, FxEvent * fxevent, int isDeathEvent )
{
    FxGeo * parent;
    FxGeo * fxgeo;
    int     fxgeoid;
    F32        customscale;
    F32        animScale;
    GfxNode * gfxnode = 0;
    Mat4    offset;
    DefTracker * lighttracker;
    bool    attarget; // Whether or not the At of this event is the Target

    /*A. ### Find the gfxnode parent.
    Find Parent fxgeo; if an animpivot is given, look inside the animation attached to the parent
    node for the real parent node to use otherwise if no animpivot, use the parent's gfxnode
    If an altpivot is given, find it in the geometry and move the offset accordingly
    */
    parent = findFxGeo( fx->fxgeos, fxevent->at );

    if( !parent  || !gfxTreeNodeIsValid( parent->gfx_node, parent->gfx_node_id ) )
    {
        printToScreenLog( 1, "Can't find %s to attach to", fxevent->at );
        return 0;
    }

    //HACKY start at root doesn't work at time zero
    //because the root isn't subtracted off till too late.  This fixes that
    if(!stricmp(&(parent->name[strlen(parent->name)-4]), "ROOT") && parent->gfx_node->parent)
    {
        //invertMat4Copy(parent->gfx_node->parent->mat, parent->gfx_node->mat);
        transposeMat4Copy(parent->gfx_node->parent->mat, parent->gfx_node->mat);
    }
    //End Hacky

    //If animpiv, look in the parent fxgeo's animation for te animpiv
    if( fxHasAValue( fxevent->animpiv ) )
    {
        SeqInst * seq;

        seq = hdlGetPtrFromHandle(parent->seq_handle);
        if( seq && gfxTreeNodeIsValid(seq->gfx_root, seq->gfx_root_unique_id ) && seq->gfx_root->child)
        {
            BoneId bone = bone_IdFromText(fxevent->animpiv);
            if(bone != BONEID_INVALID)
                gfxnode = gfxTreeFindBoneInAnimation(bone, seq->gfx_root->child, seq->handle, 1 );
        }

        //describe error
        if(!gfxnode)
        {
            printToScreenLog( 1, "GEO: Couldn't find %s in animation", fxevent->animpiv );
        }

    }

    if( !gfxnode )
        gfxnode = parent->gfx_node;

    assert( gfxnode && gfxnode->unique_id != -1 );

    copyMat4(unitmat, offset);

    if(fxevent->altpiv && gfxnode && gfxnode->unique_id != -1)
    {
        Model *model;
        model = gfxnode->model;

        if(model && model->api && fxevent->altpiv > 0 && fxevent->altpiv <= model->api->altpivotcount )
        {
            copyMat4(model->api->altpivot[fxevent->altpiv - 1], offset);
        }
        else
        {
            printToScreenLog(1, "\nFX: Node %s has no altpivot %d", fxevent->at, fxevent->altpiv );
            if(model)
            {
                printToScreenLog(1, "  The model is %s ", model->name );
                if( model->api )
                    printToScreenLog(1, "  And it has %d altpivots", model->api->altpivotcount);
                else
                    printToScreenLog(1, "  And it has no altpivots");
            }
            else
                printToScreenLog(1, "It has no model, and thus can't have altpivots in the model");
        }
    }


    /*B. ###Create the FxGeo and Set it up according to it's event
    */

    /* /10 because Doug is building stuff
    */
    if(fxevent->name && stricmp(fxevent->name, "RadiusFxScale") == 0)
        customscale = fx->fxp.radius / 10; //note if radius = 0, then custom scale is ignored
    else
        customscale = 0;

    if(fx->light_age == fx->last_age) //since change was made before fxupdate was called
        lighttracker = fx->light;
    else
        lighttracker = 0;

    fxgeoid = fxGeoCreate(
        &fx->fxgeos,
        &fx->fxgeo_count,
        gfxnode,
        offset,
        fxevent,
        FXGEO_IS_NOT_KEY,
        fx->fxinfo,
        lighttracker);

    if(!fxgeoid)
        return 0;

    attarget = strStartsWith(fxevent->at, "T_") || stricmp(fxevent->at, "Target")==0; // If this event is on the Target, pass the target's sequencer to child FX

    animScale = fx->fxinfo->animScale;
    if (fx->fxinfo->fxinfo_flags & FX_INHERIT_ANIM_SCALE && fx->handle_of_parent_seq)
    {
        SeqInst * seq = hdlGetPtrFromHandle(fx->handle_of_parent_seq);
        if( seq )
            animScale *= seqGetAnimScale(seq);
    }

    fxgeo = hdlGetPtrFromHandle(fxgeoid);
    fxGeoAttachSeq(fxgeo, fxevent->anim);
    fxGeoAttachParticles(fxgeo, fxevent->part, fx->fxp.kickstart, fx->power, &fx->fxp, fx->fxinfo, animScale);
#ifdef NOVODEX_FLUIDS
    fxGeoCreateFluid(fxgeo);
#endif
    fxGeoAttachChildFx(fxgeo, fxevent->childfx, &fx->fxp, attarget?fx->handle_of_target_seq:fx->handle_of_parent_seq, attarget?fx->handle_of_parent_seq:fx->handle_of_target_seq);
    fxGeoAttachWorldGroup( fxgeo, fxevent->worldGroup, fx->externalWorldGroupPtr );
    fxGeoAttachCape(fxgeo, fxevent->capeFiles, fx->handle_of_parent_seq, &fx->fxp);
    fxGeoUpdateSound(fxgeo, fx->handle_of_parent_seq);    // Kick start sounds playing
    if ( fx->fxp.pGroupTint )
    {
        fxgeo->groupTint[0] = fx->fxp.pGroupTint[0];
        fxgeo->groupTint[1] = fx->fxp.pGroupTint[1];
        fxgeo->groupTint[2] = fx->fxp.pGroupTint[2];
    }
    else
    {
        fxgeo->groupTint[0]
        = fxgeo->groupTint[1]
        = fxgeo->groupTint[2]
        = 255;
    }

    if (fx->fxp.isUserColored || fxgeo->bhvr->bTintGeom)
    {
        fxgeo->customTint.primary.r = fx->fxp.colors[0][0];
        fxgeo->customTint.primary.g = fx->fxp.colors[0][1];
        fxgeo->customTint.primary.b = fx->fxp.colors[0][2];
        fxgeo->customTint.secondary.r = fx->fxp.colors[1][0];
        fxgeo->customTint.secondary.g = fx->fxp.colors[1][1];
        fxgeo->customTint.secondary.b = fx->fxp.colors[1][2];
    }
    else
    {
        fxgeo->customTint.primary.integer = 0xffffffff;
        fxgeo->customTint.secondary.integer = 0xffffffff;
    }

    fxGeoAnimationChangeParams(fxgeo, &fx->fxp);
    
    if (( fxgeo->gfx_node->model ) &&
        ( fxgeo->gfx_node->model->trick ) &&
        ( fxgeo->gfx_node->model->trick->info ) &&
        ( fxgeo->gfx_node->model->trick->info->stAnim ))
    {
        // This effect geo needs to maintain some animation state in
        // its trick node for the use of the render thread (animateSts()).
        //
        // An animation age of < 0.0f means "initialize me on the next 
        // animation update".
        fxgeo->gfx_node->model->trick->st_anim_age = -1.0f;
    }

    if ( fxevent->parentVelFraction > 0.0f && fx )
    {
        Vec3 vInheritedVel;
        
        scaleVec3(fx->parentLinVel, fxgeo->event->parentVelFraction, vInheritedVel);
        addVec3(fxgeo->velocity, vInheritedVel, fxgeo->velocity );

        /*
        crossVec3(fx->parentAngVel, fxgeo->startingOffset, vFromAngVel);
        scaleVec3(vFromAngVel, fxgeo->event->parentVelFraction, vFromAngVel);
        addVec3(vFromAngVel, fxgeo->velocity, fxgeo->velocity);
        */

    }

    if( fxevent->splat )
    {
        //TO DO check texture
        fxgeo->hasSplat = TRUE;
    }

    fxgeo->isDeathFxGeo = !!isDeathEvent; //If this fxgeo was created by death condition, keep the fx around till the fxgeo dies

    //Crazy debug stuff
    if( !global_state.no_file_change_check && isDevelopmentMode() )
    {
        int part_count, i;
        char buf[1000];
        sprintf(buf,"%s%s", "fx/", fxgeo->bhvr->name);
        fileHasBeenUpdated( buf, &fxgeo->bhvrage);

#ifdef NOVODEX_FLUIDS
        if ( fxgeo->fluidEmitter.fluid )
        {
            sprintf(buf,"%s%s", "fx/", fxgeo->fluidEmitter.fluid->name);
            fileHasBeenUpdated( buf, &fxgeo->fluidage);
        }
#endif

        if(fxgeo->event)
        {
            part_count = eaSize( &fxgeo->event->part );
            for(i = 0 ; i < part_count ; i++)
            {
                sprintf(buf,"%s%s", "fx/", fxgeo->event->part[i]->params[0] );
                fileHasBeenUpdated( buf, &fxgeo->partage[i] );
            }
        }
    }
    //End debug stuff

    return 1;
}

//If this fx has a parent seq, set the pulsing for it.
//Pulse is a fire and forget thing.  The actual pulse is handled in entLight
void fxEventSetLight( FxObject * fx,  FxEvent * event )
{
    SeqInst * seq;

    //if( event->whatToLight == PARENT_SEQ )
    {
        seq = hdlGetPtrFromHandle( fx->handle_of_parent_seq );

        if( seq )
        {
            FxBhvr * bhvr = 0;
            EntLight * light;

            light = &seq->seqGfxData.light;

            light->use |= ENTLIGHT_CUSTOM_AMBIENT ;
            light->use |= ENTLIGHT_CUSTOM_DIFFUSE ;

            light->pulseEndTime = event->lifespan;
            light->pulseTime = 0;

            if( event->bhvr )
                bhvr = fxGetFxBhvr( event->bhvr ) ;

            if( bhvr )   
            {
                light->pulsePeakTime    = bhvr->pulsePeakTime;
                light->pulseBrightness    = bhvr->pulseBrightness;
                light->pulseClamp        = bhvr->pulseClamp;
            }
            else 
            {
                light->pulsePeakTime    = light->pulseEndTime / 2;
                light->pulseBrightness    = 1.8;
                light->pulseClamp        = 0.0;
            }
        }
    }
}

void fxGeoTransferMyBehaviorToParentEntity( FxObject * fx, FxGeo * fxgeo )
{
    SeqInst * seq;

    //if( event->whatToLight == PARENT_SEQ )
    {
        seq = hdlGetPtrFromHandle( fx->handle_of_parent_seq );

        if( seq )
        {
            seq->alphafx = fxgeo->alpha;

            // Because fx get created/ran *after* entity is created and alpha is
            // determined, the first frame an entity would be visible, do this here
            // to ensure that doesn't happen
            seq->seqGfxData.alpha = MIN(seq->seqGfxData.alpha, seq->alphafx);

            copyVec3( fxgeo->cumulativescale, seq->fxSetGeomScale );

            //TO DO pulsing and color to here?
            //rgb = getColorByAge( &fxgeo->bhvr, age );

            //Not really implemented, since it is never cleared out by the seq,
            //ANd not used by anyone.
            if( fxgeo->event->fxevent_flags & FXGEO_NO_RETICLE )
                seq->noReticle = SEQ_NO_RETICLE;
        }
    }
}

void fxEventIncludeFx( FxObject * fx, FxEvent * fxevent )
{

}

static int fxDoThisEvent(FxObject * fx, FxEvent * fxevent, int isDeathEvent )
{
    int success = 1;

    PERFINFO_AUTO_START("fxDoThisEvent", 1);

    switch (fxevent->type)
    {
    case FxTypeDestroy:
        PERFINFO_AUTO_START("destroy", 1);
        fxEventDestroy( fx, fxevent );
        break;
    case FxTypeCreate:
        {
            PERFINFO_AUTO_START("create",1);

            //don't make new stuff when you are fading out
            if( fx->lifeStage < FX_GENTLY_KILLED_POST_LAST_UPDATE )
            {
                PERFINFO_AUTO_START("fxEventCreateFxGeo", 1)
                    success = fxEventCreateFxGeo( fx, fxevent, isDeathEvent );
                PERFINFO_AUTO_STOP_CHECKED("fxEventCreateFxGeo");
            }
        }
        break;

    case FxTypeSetState:
        {
            PERFINFO_AUTO_START("SetState", 1);
            fxEventSetBits( fx, fxevent );
        }
        break;

    case FxTypeIncludeFx:
        {
            PERFINFO_AUTO_START("IncludeFx", 1);
            fxEventIncludeFx( fx, fxevent );
        }
        break;

    case FxTypeSetLight:
        {
            PERFINFO_AUTO_START("SetLight", 1);
            fxEventSetLight( fx, fxevent );
        }
        break;
    default:
        {
            PERFINFO_AUTO_START("unknownEvent", 1);
            printToScreenLog(1, "FX: Don't recognize event %s!!! ", fxevent->name);
            success = 0;
        }
    };

    PERFINFO_AUTO_STOP(); // Matches all the above starts.
    PERFINFO_AUTO_STOP_CHECKED("fxDoThisEvent");
    
    return success;
}




/*Check the somewhat hacked "root" of the fx for it's distance to the camera. If it's
too far, return 0.  Used right now by world fx, there is no corresponding create when near
thing, that's left to the world object that owns it.  TO DO speed this up, only checking
every once in a while?
*/
static int fxIsFxNearEnoughToCamera(FxObject * fx)
{
    Vec3 dv;
    F32  len = 0;
    Mat4Ptr mat;
    FxGeo * fxgeo = 0;
    int still_alive;

    fxgeo = fx->fxgeos;
    while(fxgeo->next)     //hack to make basic fxgeos go first
        fxgeo = fxgeo->next;
    mat = fxGeoGetWorldSpot(fxgeo->handle);
    if(mat)
    {
        subVec3( mat[3], cam_info.cammat[3], dv );
        len = lengthVec3( dv );
    }

    if( fx->fxp.fadedist < len )
        still_alive = 0;
    else
        still_alive = 1;

    return still_alive;
}

static int fxCheckForGeoFadeOut( FxObject * fx, FxGeo * fxgeo )
{
    FxEvent * event;
    int fade = 0;

    event = fxgeo->event;
    assert( event );
    if( event )
    {
        if( event->whilestate && !requiresMet( event->whilestate, fx->state) )
            fade = 1;
        else if ( event->untilstate && requiresMet( event->untilstate, fx->state) )
            fade = 1;
        else if ( fxgeo->lifespan && fxgeo->age > fxgeo->lifespan )
            fade = 1;
    }
    if( fxgeo->bhvr && fxgeo->bhvr->lifespan > 0 && fxgeo->age > fxgeo->bhvr->lifespan )
        fade = 1;

    return fade;
}

static int fxGeoUpdateAll( FxObject * fx, U8 inheritedAlpha, F32 animScale )
{
    FxGeo * fxgeo;
    FxGeo * prev;
    int non_key_fxgeo_count = 0;
    EntLight * entLight;
    int teleported;

    fxgeo = getOldestFxGeo( fx );


    //### Update all the fxgeos
    for( ; fxgeo ; fxgeo = prev)
    {
        prev = fxgeo->prev;

        //if( fxgeo->lifeStage != FXGEO_KEY ) //TO DO add this since keys don't need updating or initting

        if( !fxgeo->age )
            fxGeoInitialize(fxgeo, fx->fxgeos);

        //Event can decide to destroy itself sometimes
        if( fxgeo->lifeStage == FXGEO_ACTIVE && fxCheckForGeoFadeOut( fx, fxgeo ) )
            fxGeoFadeOut( fxgeo );

        //Update: If your alpha goes to zero, or your node goes bad, destroy
        if( !fxGeoUpdate( fxgeo, &fx->fxobject_flags, inheritedAlpha, animScale ) )
        {
            int hackDontKillMe;

            //This hack is to give particles created as the whole thing is destroyed a chance to be initted
            if( gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id) )
                fxGeoUpdateParticleSystems(fxgeo, fx->currupdatestate, 0, inheritedAlpha, animScale); // JE: Was 255, why?

            //HACK
            //If this fxgeo is managing its parent entity, you want to maintain your alpha on the parent until
            //The Fx itself is dead.  Otherwise when the alpha hits zero, the fxgeo will die, and the entity
            //Will pop back to full opaacity.  TO DO : Fade in and fade out are currently the only was to change
            //an fxgeo's alpha.  When you can change alpha like you change color, then this won't
            //be needed becuase you'll use that instead.
            if( fxgeo->event && ( fxgeo->event->fxevent_flags & FXGEO_ADOPT_PARENT_ENTITY ) && fx->lifeStage == FX_FULLY_ALIVE && gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id) )
                hackDontKillMe = 1;
            else
                hackDontKillMe = 0;

            // For the image capture renderpass, gfxUpdateFrame() is called to build a list of geometry
            // and then fxEngineRun is called several times to advance the effects and then the
            // screen is cleared and geometry re-rendered.  If we delete fxGeo, then the cached
            // list of geometry becomes corrupted.
            if (gfx_state.renderPass == RENDERPASS_IMAGECAPTURE)
                hackDontKillMe = 1;

            if( !hackDontKillMe )
                fxGeoDestroy(fxgeo, &fx->fxobject_flags, &fx->fxgeos, &fx->fxgeo_count, SOFT_KILL, fx->handle);
        }
        else
        {
            bool bDestroyed = false;
            if( fxgeo->lifeStage != FXGEO_KEY )
                non_key_fxgeo_count++;
            if ( fxgeo->event && fxgeo->event->atRayFx )
            {
                if (!fxgeo->event->fRayLength)
                {
                    printToScreenLog(1, "Must define a rayLength in event if you have an atRayFx");
                }
                else if ( fabsf(fxgeo->event->fRayLength) > 20.0f )
                {
                    printToScreenLog(1, "fRayLength can not be longer than 20 feet");
                }
                else
                {
                    // Do the raycast
                    CollInfo coll;
                    Entity *player = playerPtr();
                    Vec3 srcPos;
                    Vec3 dstPos;
                    copyVec3(fxgeo->world_spot[3], srcPos);
                    scaleVec3(fxgeo->world_spot[1], fxgeo->event->fRayLength, dstPos);
                    addVec3(srcPos, dstPos, dstPos);
                    if( collGrid(NULL, srcPos, dstPos, &coll, 0, COLL_NOTSELECTABLE | COLL_DISTFROMSTART) )
                    {
                        FxParams fxp;
                        int iChildEventGeoHandle;
                        fxInitFxParams( &fxp );
                        copyMat4(coll.mat, fxp.keys[fxp.keycount].offset);
                        fxp.keys[fxp.keycount].gfxnode = NULL;
                        fxp.keycount++;
                        fxp.fxtype = FX_POWERFX;
                        iChildEventGeoHandle = fxCreate( fxgeo->event->atRayFx, &fxp );
                        if ( iChildEventGeoHandle )
                        {
                            FxObject* cFx = hdlGetPtrFromHandle(iChildEventGeoHandle);
                            if ( cFx )
                            {
                                int i=0;
                            }
                        }
                    }
                }
            }

#ifdef NOVODEX
            if ( fxgeo->nxEmissary )
            {
                NwEmissaryData* pEmissary = nwGetNwEmissaryDataFromGuid(fxgeo->nxEmissary);
                if ( pEmissary )
                {
                    void* pActor = pEmissary->pActor;
                    if ( pActor )
                    {
                        Vec3 vCurVelocity;
                        nwGetActorLinearVelocity(pActor, vCurVelocity);
                        if ( lengthVec3Squared(vCurVelocity) < SQR(fxgeo->bhvr->physKillBelowSpeed))
                        {
                            fxGeoDestroy(fxgeo, &fx->fxobject_flags, &fx->fxgeos, &fx->fxgeo_count, SOFT_KILL, fx->handle);
                            bDestroyed = true;
                        }
                    }
                    if ( !bDestroyed && pEmissary->uPhysicsHitObject > 0 )
                    {
                        if ( !fxgeo->event )
                        {
                            Errorf("All physically enabled fxgeos should have events!\n");
                        }
                        else if ( nx_state.hardware || pEmissary->fPhysicsHitForce > fxgeo->event->cthresh )
                        {
                            FxParams fxp;
                            int iChildEventGeoHandle;
                            fxInitFxParams( &fxp );
                            if ( pEmissary->uPhysicsHitObject == 2 ) // 2 means we have pos/norm info, else just use the position of the object
                            {
                                if ( fxgeo->event->crotation == FX_CEVENT_USE_WORLD_UP )
                                {
                                    pEmissary->vPhysicsHitNorm[0] = 0.0f;
                                    pEmissary->vPhysicsHitNorm[1] = 1.0f;
                                    pEmissary->vPhysicsHitNorm[2] = 0.0f;
                                }
                                copyVec3(pEmissary->vPhysicsHitPos, fxp.keys[0].offset[3] );        
                                mat3FromUpVector( pEmissary->vPhysicsHitNorm, fxp.keys[0].offset );
                            }
                            else
                            {
                                copyMat4(fxgeo->gfx_node->mat, fxp.keys[0].offset);
                            }
                            fxp.keycount++;
                            fxp.fxtype = FX_POWERFX;
                            fxp.numColors = fx->fxp.numColors;
                            fxp.isUserColored = fx->fxp.isUserColored;
                            memcpy(fxp.colors, fx->fxp.colors, 
                                fxp.numColors * sizeof(fxp.colors[0]));
                            iChildEventGeoHandle = fxCreate( fxgeo->event->cevent, &fxp );
                            pEmissary->uPhysicsHitObject = 0;
                            pEmissary->fPhysicsHitForce = 0.0f;

                            if ( iChildEventGeoHandle )
                            {
                                FxObject* cFx = hdlGetPtrFromHandle(iChildEventGeoHandle);
                                if ( cFx )
                                {
                                    copyVec3(fxgeo->velocity, cFx->parentLinVel);
                                    copyVec3(fxgeo->spinrate, cFx->parentAngVel);
                                }
                            }

                            // should make sure it's not already destroyed?
                            if ( fxgeo->event->cdestroy )
                                fxGeoDestroy(fxgeo, &fx->fxobject_flags, &fx->fxgeos, &fx->fxgeo_count, SOFT_KILL, fx->handle);
                        }
                    }
                    if (!bDestroyed && pEmissary->bJointJustBroke )
                    {
                        FxParams fxp;
                        void* pActor = nwGetActorFromEmissaryGuid(fxgeo->nxEmissary);
                        int iJEventGeoHandle;
                        fxInitFxParams(&fxp);
                        nwGetActorMat4(pActor, (float*)fxp.keys[0].offset);
                        fxp.keycount++;
                        fxp.fxtype = FX_POWERFX;
                        iJEventGeoHandle = fxCreate( fxgeo->event->jevent, &fxp );
                        pEmissary->bJointJustBroke = false;

                        if ( fxgeo->event->jdestroy )
                            fxGeoDestroy(fxgeo, &fx->fxobject_flags, &fx->fxgeos, &fx->fxgeo_count, SOFT_KILL, fx->handle);
                    }
                }
            }
#endif
        }
    }


    //### Update world lighting if necessary
    //if there are any objects in this fx, you need to calculate the lighting for it
    //TO DO, optimize (not so often, or check for obj that needs it) and clean
    {
        SeqGfxData * seqGfxData = NULL; //toGive to this node for lighting
        SeqInst * seq = fx->handle_of_parent_seq ? hdlGetPtrFromHandle(fx->handle_of_parent_seq) : NULL;
        bool bHideGeo = false;

        //DYnamic Lighting really means static lighting, that is, it means, you need to figure out your 
        //own lighting and not inherit it from your parent seq.  But it's only done once now, since there's 
        //not really a way to 
        if( fx->fxinfo->lighting == FX_DYNAMIC_LIGHTING  ) //use your own lighting
        {

            fxgeo = getOldestFxGeo( fx );
            seqGfxData = fx->seqGfxData;

            assert( seqGfxData );
            if( fxgeo && seqGfxData && !seqGfxData->light.hasBeenLitAtLeastOnce )
            {
                Mat4Ptr mat;
                mat = fxGeoGetWorldSpot(fxgeo->handle);
                lightEnt( &seqGfxData->light, mat[3], 0.0, 1.0 );
            }

        }
        else if( ( fx->fxinfo->lighting == FX_PARENT_LIGHTING ) && seq  ) //Use you parent seq's lighting
        {
            seqGfxData = &seq->seqGfxData; //only used below, then could become invalid!
        }

        if( seqGfxData )
            entLight = &seqGfxData->light;
        else
            entLight = 0;

        teleported = (seq && seq->moved_instantly) ? 1 : 0;

        // check to see if this fx geo should be hidden
        if(seq != NULL) {
            bHideGeo = ( (fx->fxinfo->fxinfo_flags & FX_IS_SHIELD) && getMoveFlags(seq, SEQMOVE_HIDESHIELD) != 0);
            // usually dont need hideweapon since other code hides all WepL/WepR nodes, but for special cases that aren't on Wep bone need this...
            bHideGeo = bHideGeo || ( (fx->fxinfo->fxinfo_flags & FX_IS_WEAPON) && getMoveFlags(seq, SEQMOVE_HIDEWEAPON) != 0);
        }

#ifndef FINAL
        bHideGeo = bHideGeo || (game_state.fxdebug & FX_HIDE_FX_GEO);
#endif

        //### Update all the living fxgeo's particles, animations and sounds
        //(after all fxgeos have been processed so magnets etc are right)
        for( fxgeo = fx->fxgeos; fxgeo ; fxgeo = fxgeo->next) 
        {
            U8 actualAlpha;
            bool bHideParticles;

            actualAlpha = MIN( inheritedAlpha, fxgeo->alpha );

            // Ignore invalid fxgeo's.  The image capture render pass may not really
            // destroy fxgeo's and this can somehow lead to invalid fxgeo's in the list.
            if(!gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id))
                continue;

            // MS: I moved all the timers to be internal to the functions so they only time if necessary.

            fxGeoUpdateAnimation( fxgeo, fxgeo->seq_handle, fx->handle_of_parent_seq, fx->state, entLight, actualAlpha, animScale, fxgeo->event ? fxgeo->event->newstate : 0 );

            // hide particle systems that are childed off a hidden gfx_node (for hiding weapon fx while in beast run, etc)
            bHideParticles = !gfxTreeParentIsVisible(fxgeo->gfx_node);
            fxGeoUpdateParticleSystems(fxgeo, bHideParticles ? FX_UPDATE_BUT_DONT_DRAW_ME : fx->currupdatestate, teleported, inheritedAlpha, animScale ); //particles don't use fxgeo alpha

            fxGeoUpdateSound( fxgeo, fx->handle_of_parent_seq );

            fxGeoUpdateCape( fxgeo, fx );

            fxGeoUpdateWorldGroup( fxgeo, fx->currupdatestate, entLight, inheritedAlpha, fx->useStaticLighting );

            // Hide or unhide the geo.  Assumes no other code is hiding this node for any reason, or in the case  
            //    where geo is hidden elsewhere it gets hidden after this call and before rendering takes place (this
            //    is the case for animation triggered fx, see fxGeoUpdateAnimation).
            if(bHideGeo)
                fxgeo->gfx_node->flags |= GFXNODE_HIDE;
            else
                fxgeo->gfx_node->flags &= ~GFXNODE_HIDE;

            //THis is the kind of fxgeo that transfers its behavior to the parent entity
            //So you treat the entity as though it were the seq and geometry of this fxGeo
            //TO DO : Maybe move pulsing to this too, it wouls be more elegant.

            if( fxgeo->event && (fxgeo->event->fxevent_flags & FXGEO_ADOPT_PARENT_ENTITY) ) //or event flags with fxgeo flags
                fxGeoTransferMyBehaviorToParentEntity( fx, fxgeo );

            //TO DO Make an updateChildFx function to update child fx?
            //TO DO I made the child fx inherit the fx's state -- is that going to break anything?
            if( fxgeo->childfxid )
            {
                FxObject * childfx;
                childfx = hdlGetPtrFromHandle( fxgeo->childfxid );
                if( childfx )
                {
                    if( fxgeo->event && fxHasAValue( fxgeo->event->newstate ) )
                    {
                        seqSetStateFromString( childfx->state, fxgeo->event->newstate );
                    }
                    seqSetOutputs( childfx->state, fx->state ); 
                }
            }
            else
                fxgeo->childfxid = 0;

            if( fxgeo->hasSplat == TRUE )
            {
                fxGeoUpdateSplat( fxgeo, actualAlpha  );
            }

            fxgeo->gfx_node->seqGfxData = seqGfxData;
        }
    }

    return( non_key_fxgeo_count );
}

//Given an effect, try bracket it in screen coordinates
//right now it just works for geometry and
void fxFetchSides( int fxid, Vec3 ul, Vec3 lr )
{


}

static int fxUpdateFxStateFromParentSeq( FxObject * fx )
{
    SeqInst * seq;
    U32 * state;
    const SeqMove * parent_move;
    int parent_move_num;
    int i;
 
    if( fx->fxinfo->fxinfo_flags & FX_USE_TARGET_SEQ )
        seq = hdlGetPtrFromHandle( fx->handle_of_target_seq );
    else
        seq = hdlGetPtrFromHandle( fx->handle_of_parent_seq );

    if( fx->fxinfo->fxinfo_flags & FX_DONT_INHERIT_BITS )
        return 0;

    state = fx->state;

    if( seq ) 
    {
        //#### First just copy off the bits from the sequencer
        seqSetOutputs(state, seq->state);

        //#### Then do some guessing:
        //#### Figure out which sequencer bits were set on the seq last frame and
        //#### set the same bits on the fx.
        parent_move = seq->animation.move;
        assert(parent_move);

        parent_move_num = parent_move->raw.idx;
 
        //is this move new, or are we in a cycle move?
        if( 1 ) //parent_move_num != fx->last_move_of_parent
            //|| ( parent_move->flags & ( SEQMOVE_CYCLE | SEQMOVE_REQINPUTS ) ) )
        {
            //If so, assume these bits need to be sent again
            fx->last_move_of_parent = parent_move_num;

            //by checking requiremrents
            seqSetSparseOutputs( state, &parent_move->raw.required );

            //and by checking children to do make it sets on children
            if( eaSize( &parent_move->setsOnChildStr ) ) //99.9% none
            {
                for( i = 0 ; i < eaSize( &parent_move->setsOnChildStr ) ; i++ )
                {
                    seqSetStateByName( state, 1, parent_move->setsOnChildStr[i] );
                }
            }
        }

        //Hackery. Combat 

        if( eaSize( &parent_move->sticksOnChildStr ) ) //99.9% none
        {
            for( i = 0 ; i < eaSize( &parent_move->sticksOnChildStr ) ; i++ )
            {
                seqSetStateByName( state, 1, parent_move->sticksOnChildStr[i] );
            }
        }

        //#### Figure out what surface the parent seq is on and set those bits on the fx
        //And, if the surface has changed, flag that, and clear the flag suppressing redoing
        //the condition
        //if( seq->surface != fx->surface )
        //{
        //    fx->surface        = seq->surface;
        //    fx->newsurface    = TRUE;
        //}
    }
    else
    {
        if( fx->fxinfo->fxinfo_flags & FX_USE_TARGET_SEQ )
            fx->handle_of_target_seq = 0;
        else
            fx->handle_of_parent_seq = 0;
        seqClearState(state);

        //fx->surface = 0;
    }
    return 1;
}

//This is here so I can nab the parent sequencers states before they get cleared
//possible problem if, say an fx gets created after this is called but before the
//fxUpdate is called and it needs to know its parent's state.  But that doesn't
//seem to make much sense.
void fxUpdateFxStateFromParentSeqs()
{
    FxObject * fx;
    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        if( fx->handle_of_parent_seq )
            fxUpdateFxStateFromParentSeq( fx );
    }
}

//Multiple trigger bits fields mean "or".  So if FX file says
//TriggerBits CONCRETE
//TriggerBits MUD
//This means do this if concrete -or- mud is true.
int checkTriggerBits( FxCondition * fxcon, U32 state[] )
{
    int condition_met = FALSE;
    int num_triggers, i;

    num_triggers = eaSize( &fxcon->triggerbits );
    for( i = 0 ; i < num_triggers ; i++ )
    {
        if( requiresMet( fxcon->triggerstates[i], state) )
        {
            condition_met = TRUE;
            break;
        }
    }
    return condition_met;
}


#if 0 // no longer needed now that we set STATE_GROUND when on any surface
#define ENDSTATELIST 10000000
U32 surfaceBits[] =
{
    STATE_CONCRETE,
    STATE_GRASS,
    STATE_LAVA,
    STATE_PUDDLE,
    STATE_MUD,
    STATE_DIRT,
    STATE_WOOD,
    STATE_METAL,
    STATE_TILE,
    STATE_ROCK,
    STATE_GRAVEL,
    STATE_CARPET,
    ENDSTATELIST,
};
#endif

static int areAnySurfaceBitsSet( U32 * state )
{
#if 0
    int hits = 0, i = 0;
    while( surfaceBits[i] != ENDSTATELIST )
    {
        if( TSTB( state, surfaceBits[i] ) )
            return TRUE;
        i++;
    }

    return FALSE;
#else
    // STATE_GROUND is set when on any surface, so that's all we need to check for
    if( TSTB( state, STATE_GROUND) )
        return TRUE;
    return FALSE;
#endif
}

static void fxDoEvent(FxObject* fx, FxCondition* fxcon, int iFxIndex)
{
    FxGeo * currFxgeo = 0;
    FxEvent* fxevent = fxcon->events[iFxIndex];

    if( !(fx->power >= fxevent->power[ MIN_ALLOWABLE_POWER ] && fx->power <= fxevent->power[ MAX_ALLOWABLE_POWER ] ) )
        return;

    #if NOVODEX
    if (
        ( fxevent->bHardwareOnly && !nx_state.hardware)
        || ( fxevent->bSoftwareOnly && nx_state.hardware)
        || (fxevent->bPhysicsOnly && !nwEnabled() )
        )
        return;
    #endif

    //If this event is only allowed to have one with this name at a time
    if( fxevent->fxevent_flags & FXGEO_ONE_AT_A_TIME )  
    {
        if ( !fxevent->name )
        {
            printToScreenLog(1, "FX: Must give event name if Flags OneAtATime is set!!" );
            return;
        }
        if ( ( currFxgeo = findFxGeo( fx->fxgeos, fxevent->name ) ) && (currFxgeo->lifeStage == FXGEO_ACTIVE || currFxgeo->lifeStage == FXGEO_FADINGOUT) )
            return;
    }

    if( fx->power >= fxevent->power[ MIN_ALLOWABLE_POWER ] && fx->power <= fxevent->power[ MAX_ALLOWABLE_POWER ] )
    {
        bool event_success = fxDoThisEvent( fx, fxevent, (fxcon->trigger & FX_STATE_DEATH)? 1 : 0 );     //TO DO can any flags get set inside?
        if(!event_success)
            printToScreenLog(1, "FX: Event failed %s", fxevent->name);
    }
}

static int fxWithinTimeOfDayRange(F32 test_time, F32 day_start, F32 day_end)
{
    if (day_start < day_end && day_start <= test_time && test_time < day_end)
    {
        return 1;
    }
    else if (day_start > day_end && day_start <= test_time && test_time < 24.0f)
    {
        return 1;
    }
    else if (day_start > day_end && 0.0f <= test_time && test_time < day_end)
    {
        return 1;
    }

    return 0;
}

static void fxCheckConditions( FxObject * fx )
{
    FxInfo        * fxinfo;
    FxCondition * fxcon;
    int        condition_met, fxc, fxe, done;
    int        conditionCount, conditionsMetCount, repeat, totalRepeats, repeatJitter;

    fxinfo = fx->fxinfo;
    conditionCount = eaSize(&fxinfo->conditions);
    conditionsMetCount = 0;  //for hair brained default surface scheme

    for(fxc = 0; fxc < conditionCount ; fxc++ )
    {
        fxcon = fxinfo->conditions[fxc];
        repeatJitter = 2 * (int)fxcon->repeatJitter;

        totalRepeats = CLAMP(fxcon->repeat + (fxcon->repeatJitter ? qrand()%((int)fxcon->repeatJitter * 2) : 0) - fxcon->repeatJitter + 1, 1, 255);

        //has this condition already been met?
        assert( fxc < MAXCONDITIONS ); //TO DO : dynamically alloc
        if(!fxcon->domany && !fxcon->repeat)
            done = (fx->conditionsMet[fxc / 32] & (1 << (fxc % 32) ) ) ? 1 : 0;
        else
            done = 0;

        condition_met = FALSE;

        if( (fx->fxobject_flags & fxcon->trigger) && !done )
            condition_met = TRUE;
        else if ( ( fxcon->trigger & FX_STATE_FORCE ) && fx->lastForcePower  && fx->lastForcePower >= fxcon->forceThreshold )
        {
            if ( !fxcon->time || !fx->frameLastCondition || (fx->age - fx->frameLastCondition[fxc]) >= fxcon->time )
            {
                condition_met = TRUE;
            }
        }
        else if( ( fxcon->trigger & FX_STATE_TIME) && fx->age >= fxcon->time && fx->last_age <= fxcon->time )
            condition_met = TRUE;
        else if( ( fxcon->trigger & FX_STATE_TIMEOFDAY) )
        {
            condition_met = fxWithinTimeOfDayRange(fx->day_time, fxcon->dayStart, fxcon->dayEnd);
            fx->visibility = condition_met ? FX_VISIBLE : FX_NOTVISIBLE;
        }
        else if( ( fxcon->trigger & FX_STATE_CYCLE) && ((int)fx->age/(int)fxcon->time != (int)fx->last_age/(int)fxcon->time) )
            condition_met = TRUE;
        else if( ( fxcon->trigger & FX_STATE_TRIGGERBITS ) && !done )
            condition_met = checkTriggerBits( fxcon, fx->state );
        else if ( (fxcon->trigger & FX_STATE_DEFAULTSURFACE) && fx->last_age == 0.0 && !done && conditionsMetCount == 0 ) //Something of a hack job. Must always be the last condition evaluated  This Default Trigger Condition must always be the last condition in the fx script If it exists, it means, if you haven't done anything else, do this. //Mostly it's for sounds, so if you are running on a surface you don't recognize, just play this sound
            condition_met = areAnySurfaceBitsSet( fx->state );

        //TO DO clear doneflags when PARENT BIT gets unset. Work on doneflags, maybe I need to set done bits per condition...
        if(condition_met && fxcon->chance)
            condition_met = (fxcon->chance >= qufrand());

        //If condition's requirements are met, do each event in the condition (from first to last)
        if(condition_met == TRUE)
        {
            if ( fx->frameLastCondition )
            {
                fx->frameLastCondition[fxc] = fx->age;
            }

            for (repeat=0; repeat < totalRepeats; ++repeat )
            {
                int iSize = eaSize(&fxcon->events);

                if ( fxcon->randomEvent ) // choose one at random and do that
                {
                    if ( iSize > 0 )
                    {
                        fxe = qrand() % iSize;
                        fxDoEvent( fx, fxcon, fxe);
                    }
                }
                else // do them all
                {
                    for(fxe = 0; fxe < iSize ; fxe++)
                    {
                        fxDoEvent( fx, fxcon, fxe);
                    }
                }
            }

            //Set this condition as done at least once
            fx->conditionsMet[fxc / 32] |= (1 << (fxc % 32) );
            conditionsMetCount++;
        }
    }
}

static int fxUpdateFxObject(FxObject * fx)
{
    U8            inheritedAlpha;
    int            still_alive;
    int            non_key_fxgeo_count;
    F32            effTimestep=TIMESTEP;
    F32            animScale=fx->fxinfo->animScale;
    SeqInst        *seq;
    F32            potential_day_time;

    if(fx_engine.fx_auto_timers)
    {
        PERFINFO_AUTO_START_STATIC(fx->fxinfo->name, &fx->fxinfo->perfInfo, 1);
    }

PERFINFO_AUTO_START("start",1);
    seq = hdlGetPtrFromHandle(fx->handle_of_parent_seq);
    if(seq != NULL)
    {
        if (fx->fxinfo->fxinfo_flags & FX_INHERIT_ANIM_SCALE ) {
            animScale *= seqGetAnimScale(seq);
            if( seq->animation.typeGfx->scale != 1 )
                animScale *= seq->animation.typeGfx->scale;
            else
                animScale *= seq->animation.move->scale;
        }
    }

    effTimestep *= animScale;

    //Update State
    still_alive    = 1;
    fx->last_age = fx->age;
    fx->age += effTimestep;
    potential_day_time = server_visible_state.time;
    if (potential_day_time >= fx->day_time || fx->day_time - potential_day_time > 12.0F)
    {
        fx->last_day_time = fx->day_time;
        fx->day_time = server_visible_state.time;
    }

    //Figure out how much work you should do
    if (fx->visibility == FX_NOTVISIBLE) {
        fx->currupdatestate = FX_UPDATE_BUT_DONT_DRAW_ME;
        if (fx_engine.no_expire_worldfx)
        {
            PERFINFO_AUTO_STOP();
            if(fx_engine.fx_auto_timers)
            {
                PERFINFO_AUTO_STOP();
            }
            return still_alive;
        }
    } else
        fx->currupdatestate = FX_UPDATE_AND_DRAW_ME;

    //If the fx had instructions to tell someone else when something happens, then do that
    //TO DO, this needs to be more general, and more clear
    if(!fx->primeHasHit && fx->net_id && (fx->fxobject_flags & FX_STATE_PRIMEHIT))//only networked fx will need to report their doings
    {
        putMsgInMailbox(&globalEntityFxMailbox, fx->net_id, FX_STATE_PRIMEHIT );
        
        fx->primeHasHit = 1;
    }

    //Check each condition: if its requirements are met, do all its events
    PERFINFO_AUTO_START("fxCheckConditions", 1);
        fxCheckConditions( fx );
    PERFINFO_AUTO_STOP_CHECKED("fxCheckConditions");
    
    fx->fxobject_flags = 0; //They've all had their chance to be handled.
    fx->lastForcePower = 0; //Same with force triggers

    //Some fx use their parent seq's alpha, like machine guns that fade out with the dying dude
    inheritedAlpha = 255;
    if( fx->fxinfo->fxinfo_flags & FX_INHERIT_ALPHA && seq != NULL )
    {
        inheritedAlpha = seq->seqGfxData.alpha; //Kind of a hack to extract this
    }

    // Shut off fx which are on the player when the camera is near the player.
    // This keeps fill rate slow-down from happening as often.
    if(fx->fxp.isPlayer)
    {
        if(fx->fxp.isPersonal && !(fx->fxinfo->fxinfo_flags & FX_DONT_SUPPRESS) && game_state.suppressCloseFx)
        {
            if (control_state.first || cam_info.lastZDist <= game_state.suppressCloseFxDist)
            {
                inheritedAlpha = 0;
            }
        }
    }

    if (inheritedAlpha && seq && seq->forceInheritAlpha) 
    {
        // well, in case we still have a visible effect, we need to check if the seq wants it to be faded;
        // wish there was a better way to do this, as this might cause cache misses
        inheritedAlpha = seq->seqGfxData.alpha; //Kind of a hack to extract this
    }

    //You've had your final chance to create stuff, no more!
    if( fx->lifeStage == FX_GENTLY_KILLED_PRE_LAST_UPDATE )
        fx->lifeStage = FX_GENTLY_KILLED_POST_LAST_UPDATE;
PERFINFO_AUTO_STOP_START("fxGeoUpdateAll",1);
    //Update Geo Nodes -- or rather, let the Geo Nodes update themselves
    non_key_fxgeo_count = fxGeoUpdateAll(fx, inheritedAlpha, animScale ); //if no fxgeos left, shut it down (only in extreme circumstances)
PERFINFO_AUTO_STOP_CHECKED("fxGeoUpdateAll");
PERFINFO_AUTO_START("end",1);

    seqClearState(fx->state);      //Note: it's strange to clear fx->fxobject_flags before fxGeoUpdate, and fx->state after. (fx->fxobject_flags can be set by fxgeoUpdate, while fx->state needs fxGeoUpdate to propagate it to animations.)

    //Check remaining death conditions
    if( fx->lifeStage == FX_FULLY_ALIVE )
    {
        if( fx->duration && fx->age > fx->duration )
            fxGentlyKill( fx->handle );
        if( fx->fxp.fadedist && !fxIsFxNearEnoughToCamera( fx ) && !fx_engine.no_expire_worldfx ) //TO DO, make sense of this (is now just for world fx)
            fxGentlyKill( fx->handle );    //could do this only occasionally
    }
    else //( fx->lifeStage != FX_FULLY_ALIVE )
    {
        if( !non_key_fxgeo_count )
            still_alive &= 0;
    }
    //if a death fxgeos have been created, then fadeout the fx when the last one dies
    if( fx->lifeStage == FX_GENTLY_KILLED_POST_LAST_UPDATE )
    {
        FxGeo * fxgeo;
        int deathFxGeoExists = 0;
        for( fxgeo = getOldestFxGeo( fx ) ; fxgeo ; fxgeo = fxgeo->prev)
        {
            if( fxgeo->isDeathFxGeo )
                deathFxGeoExists = 1;
        }
        if( !deathFxGeoExists )
            fxFadeOut( fx->handle );
    }


    // World FX get set to not visible every frame.  They must be set to visible when traversing the group tree to get drawn.
    if( fx->fxp.fxtype == FX_WORLDFX)
        fx->visibility = FX_NOTVISIBLE;

    PERFINFO_AUTO_STOP();

    if(fx_engine.fx_auto_timers)
    {
        PERFINFO_AUTO_STOP();
    }
    
    return still_alive;
}

void fxRunParticleSystems(FxObject* fx, void* notUsed)
{
    int j;
    FxGeo *fxgeo;
    if(!fx)
        return;

    for( fxgeo = fx->fxgeos; fxgeo ; fxgeo = fxgeo->next) 
    {
        for(j = 0; j < fxgeo->psys_count; j++)
        {
            ParticleSystem *system = fxgeo->psys[j];
            if(system && system->sysInfo)
            {
                partRunSystem(system);
            }
        }
    }
}

void fxAdvanceEffect(FxObject* fx, void* notUsed)
{
    int still_going;
    if(!fx)
        return;

    still_going = fxUpdateFxObject( fx );
    if(!still_going || game_state.fxkill == 1)
        fxDestroyForReal( hdlGetHandleFromPtr(fx, fx->handle), SOFT_KILL );

    //Debug
    if( game_state.fxkill == 2) //Soft kill for testing
        fxDelete( hdlGetHandleFromPtr(fx, fx->handle), SOFT_KILL );

}


//###################################################################
//3. Destroy Fx
typedef struct SeqGfxDataHolder
{
    SeqGfxData * seqGfxData;
} SeqGfxDataHolder;

SeqGfxDataHolder * fxSeqGfxDatasToBeFreed;
int fxSeqGfxDatasToBeFreedCount;
int fxSeqGfxDatasToBeFreedMax;

void fxDestroyForReal( int fxid, int part_kill_type )
{
    FxGeo * fxgeo;
    FxGeo * next;
    FxObject * fx;

    fx = hdlGetPtrFromHandle(fxid);

    if( fx )
    {
        //THIS MAY BE A HACK.  ALSO IT NEEDS TO BE PUT IN A FAILED CREATE, TOO IF IT'S
        //GOING TO BE A ROBUST FIX. Plus there are other ways traveling projectiles could screw up, too.
        //Like they aren't given a failsafe lifespan.  This will just choke.  Maybe fx who lose their
        //Valid Targets should die immediately?
        //PLus the general awkwardness of all these fx that could never hit anything reporting they have hit...
        //If you are a traveling projectile, and you haven't told anyone you've hit anything,
        //maybe you better report that you did hit, just so nothing goes bad...
        if(!fx->primeHasHit && fx->net_id )//only networked fx will need to report their doings
        {
            putMsgInMailbox(&globalEntityFxMailbox, fx->net_id, FX_STATE_PRIMEHIT );
        }
        //END MINOR HACK

        //If you were using your own light, clean it up
        //MINOR HACK: The seqGfxData from this fx might have already been given to the
        //drawing code before the fx discovered that it was to die this frame, therefore, postpone
        //the freeing of this seqGfxData till after the drawing is done
        if( fx->fxinfo->lighting == FX_DYNAMIC_LIGHTING && fx->seqGfxData )
        {
            SeqGfxDataHolder * seqGfxDataHolder;
            seqGfxDataHolder = dynArrayAdd( &fxSeqGfxDatasToBeFreed, sizeof(SeqGfxDataHolder), &fxSeqGfxDatasToBeFreedCount, &fxSeqGfxDatasToBeFreedMax, 1);
            seqGfxDataHolder->seqGfxData = fx->seqGfxData;
            //free( fx->seqGfxData );
        }
        //End MINOR HACK

        for( fxgeo = fx->fxgeos ; fxgeo ; fxgeo = next )
        {
            next = fxgeo->next;
            fxGeoDestroy(fxgeo, &fx->fxobject_flags, &fx->fxgeos, &fx->fxgeo_count, part_kill_type, fx->handle);
        }
        fxDestroyedCount++;
        listRemoveMember(fx, &fx_engine.fx);
        free(fx->frameLastCondition);
        hdlClearHandle(fxid);
        mpFree( fx_engine.fx_mp, fx );
        fx_engine.fxcount--;
    }
}


void fxCleanUp(void)
{
    SeqGfxDataHolder seqGfxDataHolder;
    int    i;

    //Clean up after dead fx
    for( i = 0 ; i < fxSeqGfxDatasToBeFreedCount; i++ )
    {
        seqGfxDataHolder = fxSeqGfxDatasToBeFreed[i];
        free( seqGfxDataHolder.seqGfxData);
    }
    fxSeqGfxDatasToBeFreedCount = 0;
}


int fxDelete( int fxid, int killtype )
{
    if( killtype == SOFT_KILL )
        fxGentlyKill( fxid );
    else //killtype == HARD_KILL
        fxDestroyForReal( fxid, HARD_KILL );
    return 0;
}



//##################################################################
//FxEngine #########################################################
void fxEngineInitialize()
{
    if(fx_engine.fxgeo_mp)
        destroyMemoryPoolGfxNode(fx_engine.fxgeo_mp);
    if(fx_engine.fx_mp)
        destroyMemoryPoolGfxNode(fx_engine.fx_mp);

    memset( &fx_engine, 0, sizeof(FxEngine) );

    fx_engine.fxgeo_mp = createMemoryPool();
        initMemoryPool( fx_engine.fxgeo_mp, sizeof(FxGeo), INITIAL_FXGEO_MEMPOOL_ALLOCATION );
    fx_engine.fx_mp = createMemoryPool();
        initMemoryPool( fx_engine.fx_mp, sizeof(FxObject), INITIAL_FX_MEMPOOL_ALLOCATION );

    clearMailbox(&globalEntityFxMailbox); //for passing msg from fx to ents

    fx_engine.fx_on = 1;
}

/*Gets rid of all fx
*/
void fxReInit()
{
    FxObject * fx;
    FxObject * fxnext;

    for( fx = fx_engine.fx ; fx ; fx = fxnext )
    {
        fxnext = fx->next;
        fxDelete( hdlGetHandleFromPtr(fx, fx->handle), HARD_KILL );
    }
}

#ifdef NOVODEX_FLUIDS
// This is very temporary, something to draw all the fluids hacked into the end
// of the partRunEngine() function.
void fxDrawFluids()
{
    FxObject * fx;
    FxObject * fxnext;
    FxGeo* fxgeo;
    for( fx = fx_engine.fx ; fx ; fx = fxnext )
    {
        fxnext = fx->next;
        for (fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next)
        {
            if ( fxgeo->fluidEmitter.fluid && fxgeo->fluidEmitter.currentNumParticles > 0)
            {
                drawFluid(&fxgeo->fluidEmitter);
            }

        }

    }
}
#endif

// Resets the count of cts fx counts on the seqs
static void fxResetCTSCount(void)
{
    FxObject * fx;
    FxObject * fxnext;
    for( fx = fx_engine.fx ; fx ; fx = fxnext )
    {
        SeqInst* seq = hdlGetPtrFromHandle(fx->handle_of_parent_seq);
        if (seq) seq->ctsFxCount = 0;
        fxnext = fx->next;
    }
}

// Shows click to source stuff in dev mode
static void fxShowClickToSource(FxObject* fx)
{
    SeqInst* seq = hdlGetPtrFromHandle(fx->handle_of_parent_seq);
    if (seq && fx->fxinfo && fx->fxinfo->name)
    {
        char pathName[MAX_PATH];
        sprintf(pathName, "fx/%s", fx->fxinfo->name);
        ClickToSourceDisplay(seq->gfx_root->mat[3][0], seq->gfx_root->mat[3][1], seq->gfx_root->mat[3][2], seq->ctsFxCount*8, CLR_WHITE, pathName, NULL, CTS_TEXT_DEBUG_3D);
        seq->ctsFxCount++;
    }
}

/*Updates all the FX and the gfx tree but doesn't draw anything or update individual particle systems
*/
void fxRunEngine()
{
    FxObject * fx;
    FxObject * fxnext;
    int still_going;
    static int compact_counter = 0;

    PERFINFO_AUTO_START("misc start",1);

    assert( listCheckList(&fx_engine.fx, fx_engine.fxcount) );
    maxSimultaneousFx = MAX( fx_engine.fxcount, maxSimultaneousFx ); //debug

    //End debug
    PERFINFO_AUTO_STOP_CHECKED("misc start");

    if( fx_engine.fx_on )
    {
#ifndef FINAL //Debug
        PERFINFO_AUTO_START("dev checks",1);
        if( !global_state.no_file_change_check && isDevelopmentMode() && ( game_state.fxdebug & FX_DEBUG_BASIC ) )
        {
            static F32 time = 0;
            time += TIMESTEP;
            if(time > 30.0)
            {
                fxDoCheckForUpdatedFx(fx_engine.fx);//note particle system uses this result too (a little sloppy, but hey)
                time = 0;
            }
        }
        PERFINFO_AUTO_STOP_CHECKED("dev checks");
#endif

        PERFINFO_AUTO_START("updates",1);

        if (CTS_SHOW_FX)
            fxResetCTSCount();

        for( fx = fx_engine.fx ; fx ; fx = fxnext )
        {
            fxnext = fx->next;

            if (CTS_SHOW_FX)
                fxShowClickToSource(fx);

            still_going = fxUpdateFxObject( fx );
            if(!still_going || game_state.fxkill == 1)
                fxDestroyForReal( hdlGetHandleFromPtr(fx, fx->handle), SOFT_KILL );

            //Debug
            if( game_state.fxkill == 2) //Soft kill fo testing
                fxDelete( hdlGetHandleFromPtr(fx, fx->handle), SOFT_KILL );
            //End Debug
        }
        PERFINFO_AUTO_STOP_CHECKED("updates");
        PERFINFO_AUTO_START("end stuff",1);

        //Debug
        game_state.fxkill = 0;
        if( game_state.fxkillDueToInternalError )
        {
            game_state.fxkill = 1;
            game_state.fxkillDueToInternalError = 0;
        }
        fxPrintDebugInfo();
        //End Debug
        PERFINFO_AUTO_STOP_CHECKED("end stuff");

    }
    
    if (compact_counter++ > 15)
    {
        void vhCompact(void);
        compact_counter = 0;
        vhCompact();
    }
}


///////############################################################################################
///////##Little functions to ease interaction with Lennard and Bruce's stuff       ################
//Really belongs somewhere else, but here is OK for now...
int fxManageWorldFx( int fxid, char * fxname, Mat4 mat, F32 fadestart, F32 fadedist, DefTracker * light, U8* pTintRGB)
{
    FxObject * fx = 0;
    FxParams fxp;

    fx = hdlGetPtrFromHandle( fxid );
    if( fx ) //if this is a valid fx, update it, flag it to be drawn
    {
        char buf[FX_MAX_PATH];
        char buf2[FX_MAX_PATH];

        fx->visibility = FX_VISIBLE;

        //#For the editor
        fxChangeWorldPosition( fxid, mat );
        fxCleanFileName(buf, fxname);
        fxCleanFileName(buf2, fx->fxinfo->name); //this is unneeded
        if(strcmp(buf, buf2) != 0) //if a new fx name is given, change it out.
            fxid = fxDelete( fxid, SOFT_KILL );
        //#End for the editor
    }
    else
    {
        fxid = 0;
    }

    //if this is not an fx already, or we just deleted the fx, create the fx
    if( !fxid )
    {
        Vec3 dv;
        F32  len;

        subVec3( mat[3], cam_info.cammat[3], dv ); //if this effect is just gonna fade out right away, don't bother
        len = lengthVec3( dv );
        if( fadestart + fadedist >= len )
        {
            fxInitFxParams(&fxp);
            copyMat4(mat, fxp.keys[0].offset);
            fxp.keycount++;
            fxp.fadedist = fadestart + fadedist;
            fxp.kickstart = PARTICLE_KICKSTART;
            fxp.fxtype = FX_WORLDFX;
            fxp.pGroupTint = pTintRGB;

            fxid = fxCreate( fxname, &fxp );
            //TO DO add fxAddWorldLightParameters that extracts needed info
        }
    }

    fx = hdlGetPtrFromHandle( fxid ); //should always be true
    if(fx)
    {
        fx->light_age = fx->age;
        fx->light = light;
    }
    return  fxid;
}

void fxSetUseStaticLighting( int fxid, int useStaticLighting )
{
    FxObject * fx = 0;
    fx = hdlGetPtrFromHandle( fxid );
    if( fx )
        fx->useStaticLighting = useStaticLighting;
}

//clears the lighting on all fxgeos so that it will be recalculated next frame
void fxResetStaticLighting( int fxid )
{
    FxGeo *fxgeo;
    FxObject *fx = 0;
    fx = hdlGetPtrFromHandle( fxid );
    if( !fx )
        return;

    for (fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next)
    {
        if (fxgeo->fxMiniTracker)
            fxgeo->fxMiniTracker->lighttracker = 0;
    }
}


FxDebugAttributes * FxDebugGetAttributes( int fxid, const char * name, FxDebugAttributes * attribs )
{
    FxObject * fx;
    FxInfo * fxinfo = 0;
    FxCondition * condition;
    FxEvent * event;
    int conditionCount, eventCount, i , j;
    int conditionLifeSpan;

    memset( attribs, 0, sizeof( FxDebugAttributes ) );

    if( fxid )
    {
        fx = hdlGetPtrFromHandle( fxid );
        if( !fx )
            return 0;
        fxinfo = fx->fxinfo;
    }
    else if( name )
    {
        fxinfo = fxGetFxInfo( name );
    }
    else
        return 0;

    if( !fxinfo )
        return 0;

    conditionLifeSpan = 0;

    conditionCount = eaSize( &fxinfo->conditions );

    for( i = 0 ; i < conditionCount ; i++ )
    {
        condition = fxinfo->conditions[i];
        eventCount = eaSize( &condition->events );

        for( j = 0 ; j < eventCount ; j++ )
        {
            event = condition->events[j];

            if( event->name && 0 == stricmp( event->name, "All" ) && event->type == FxTypeDestroy )
            {
                if( condition->trigger & ( FX_STATE_TIME | FX_STATE_TIMEOFDAY ) )
                {
                    attribs->type |= FXTYPE_LIFESPAN_IN_CONDITIONS;
                    conditionLifeSpan = condition->time;
                }
                else if( condition->trigger & ( FX_STATE_PRIMEHIT | FX_STATE_PRIMEBEAMHIT |FX_STATE_PRIME1HIT| FX_STATE_PRIME2HIT | FX_STATE_PRIME3HIT | FX_STATE_PRIME4HIT | FX_STATE_PRIME5HIT ) )
                {
                    attribs->type |= FXTYPE_TRAVELING_PROJECTILE;
                }
                else
                {
                    attribs->type |= FXTYPE_PECULIAR_DEATH_CONDITIONS;
                }
            }
        }
    }

    attribs->lifeSpan = fxinfo->lifespan;

    if( attribs->type & FXTYPE_LIFESPAN_IN_CONDITIONS )
    {
        if( !attribs->lifeSpan || attribs->lifeSpan > conditionLifeSpan )
            attribs->lifeSpan = conditionLifeSpan;
    }

    if( !attribs->lifeSpan )
        attribs->type |= FXTYPE_NEVER_DIES;

    return attribs;
}

void fxSetIndividualAutoTimersOn(int on)
{
    fx_engine.fx_auto_timers = on ? 1 : 0;
}

void fxChangeSeqHandles(int oldSeqHandle, int newSeqHandle)
{
    FxObject * fx;

    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        FxGeo * fxgeo;
        if (fx->handle_of_parent_seq == oldSeqHandle) {
            fx->handle_of_parent_seq = newSeqHandle;
        }

        for (fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next) {
            if (fxgeo->seq_handle == oldSeqHandle) {
                fxgeo->seq_handle = newSeqHandle;
            }
        }
    }
}

void fxReloadSequencers(void)
{
    FxObject * fx;

    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        FxGeo * fxgeo;
        for (fxgeo = fx->fxgeos; fxgeo; fxgeo = fxgeo->next) {
            if (fxgeo->seq_handle) {
                SeqInst *seq = hdlGetPtrFromHandle(fxgeo->seq_handle);
                if (seq) {
                    seqReinitSeq( seq, SEQ_LOAD_FULL, qfrand(), NULL );
                    animSetHeader( seq, 0 );
                }
            }
        }
    }
}


void fxActivateForceFxNodes(Vec3 vEpicenter, F32 fRadius, F32 fPower)
{
    FxObject* fx;
    PERFINFO_AUTO_START("fxActivateForceFxNodes", 1);
    for( fx = fx_engine.fx ; fx ; fx = fx->next )
    {
        if ( fx->fxinfo->hasForceNode )
        {
            F32 fRadiusToCheck = fRadius + fx->fxinfo->onForceRadius;
            // do the distance check
            FxGeo* fxgeo = getOldestFxGeo( fx );
            if(fxgeo)
            {
                if( gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id ) )
                {
                    Vec3 vToFxGeo;
                    F32 fDistanceSquared;
                    Mat4Ptr mGeo = fxGeoGetWorldSpot(fxgeo->handle);
                    subVec3(mGeo[3], vEpicenter, vToFxGeo);
                    fDistanceSquared = lengthVec3Squared(vToFxGeo);
                    if ( fDistanceSquared < fRadiusToCheck * fRadiusToCheck )
                    {
                        F32 fDistance = MAX(sqrtf(fDistanceSquared) - fx->fxinfo->onForceRadius, 0.0f);
                        F32 fNetPower = fabsf(fPower);
                        if ( fDistance > 0.0f )
                        {
                            fNetPower *= (1.0f - ( fDistance / fRadius ));
                        }
                        fx->lastForcePower = MAX(fNetPower,fx->lastForcePower);
                    }
                }
            }
        }
    }
    PERFINFO_AUTO_STOP_CHECKED("fxActivateForceFxNodes");
}

/*
 * Determine if the named fx is a cameraspace deal.  We do this separately, since this information is only useful if
 * we know the target Entity of the fx.  That information doesn't make it's way into fxCreate(...), so we pass this
 * back to whoever wants to call fxCreate(...), and let them take responsibility for culling.
 */
int fxIsCameraSpace(const char *fxname)
{
    int i;
    int j;
    FxInfo *fxinfo;
    FxCondition *fxcondition;

    if (game_state.nofx || fxname == NULL || fxname[0] == 0)
    {
        return 0;
    }
    fxinfo = fxGetFxInfo(fxname);
    if (fxinfo == NULL)
    {
        return 0;
    }
    for (i = eaSize(&fxinfo->conditions) - 1; i >= 0; i--)
    {
        fxcondition = fxinfo->conditions[i];
        for (j = eaSize(&fxcondition->events) - 1; j >= 0; j--)
        {
            if (fxcondition->events[j]->bCameraSpace)
            {
                return 1;
            }
        }
    }
    return 0;
}
