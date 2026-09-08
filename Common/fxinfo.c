#include "fxinfo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include "graphics/font.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/genericlist.h>
#include "time.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/fileutil.h>
#include "seq/animtrack.h"
#include <utilitieslib/utils/mathutil.h>
#include "cmdparse/cmdcommon.h"
#include "group/groupfilelib.h"
#include "fxbhvr.h"
#include <utilitieslib/utils/timing.h>


#if SERVER
    #include "cmdparse/cmdserver.h"
#endif

#if CLIENT
    #include "graphics/FX/fx.h"
    #include "cmdparse/cmdgame.h"
    #include "graphics/FX/particle.h"
    #include <utilitieslib/utils/rgb_hsv.h>
    #include "sound/sound.h"
    #include "seq/anim.h"
#endif

// global lists of parsed objects
typedef struct FxInfoList {
    FxInfo **fxinfos;
} FxInfoList;

SERVER_SHARED_MEMORY FxInfoList fx_infolist;

//#####################################################################
//#####Get an FxInfo (.fx file)

StaticDefineInt    ParseFxEventFlags[] =
{
    DEFINE_INT
    { "AdoptParentEntity",    FXGEO_ADOPT_PARENT_ENTITY    },
    { "NoReticle",            FXGEO_NO_RETICLE            },
    { "PowerLoopingSound",    FXGEO_POWER_LOOPING_SOUND    },
    { "OneAtATime",            FXGEO_ONE_AT_A_TIME            },
    DEFINE_END
};

StaticDefineInt FxTypeTable[] = {
    DEFINE_INT
    { "Create",                FxTypeCreate },
    { "Destroy",            FxTypeDestroy },
    { "Local",                FxTypeLocal },
    { "Start",                FxTypeStart },
    { "Posit",                FxTypePosit },
    { "StartPositOnly",        FxTypeStartPositOnly },
    { "PositOnly",            FxTypePositOnly },
    { "SetState",            FxTypeSetState },
    { "IncludeFx",            FxTypeIncludeFx },
    { "SetLight",            FxTypeSetLight },
    DEFINE_END
};

StaticDefineInt FxCeventCollisionRotation[] = {
    DEFINE_INT
    { "UseCollisionNormal",    FX_CEVENT_USE_COLLISION_NORM },
    { "UseWorldUp",            FX_CEVENT_USE_WORLD_UP   },
    DEFINE_END
};

StaticDefineInt FxTransformFlag[] = {
    DEFINE_INT
    { "None",    FX_NONE },
    { "Rotation",    FX_ROTATION },
    { "SuperRotation",    (FX_ROTATION | FX_ROTATE_ALL) },
    { "Position",    FX_POSITION },
    { "Scale",        FX_SCALE },
    { "All",    FX_ALL },
    DEFINE_END
};


StaticDefineInt    ParseFxInfoFlags[] =
{
    DEFINE_INT
    { "SoundOnly",            FX_SOUND_ONLY                },
    { "InheritAlpha",        FX_INHERIT_ALPHA            },
    { "IsArmor",            FX_IS_ARMOR                    },    /* no longer used */
    { "InheritAnimScale",    FX_INHERIT_ANIM_SCALE        },
    { "DontInheritBits",    FX_DONT_INHERIT_BITS        },
    { "DontSuppress",        FX_DONT_SUPPRESS            },
    { "DontInheritTexFromCostume", FX_DONT_INHERIT_TEX_FROM_COSTUME },
    { "UseSkinTint",        FX_USE_SKIN_TINT },
    { "IsShield",            FX_IS_SHIELD },
    { "IsWeapon",            FX_IS_WEAPON },
    { "NotCompatibleWithAnimalHead", FX_NOT_COMPATIBLE_WITH_ANIMAL_HEAD },
    { "InheritGeoScale",    FX_INHERIT_GEO_SCALE },
    { "UseTargetSeq",        FX_USE_TARGET_SEQ },
    DEFINE_END
};

TokenizerParseInfo parseSplatEvent[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_STRING(SplatEvent, texture1, 0) },
    {    "",                TOK_STRUCTPARAM | TOK_STRING(SplatEvent, texture2, 0) },
    {    "\n",            TOK_END,                                0},
    {    "", 0, 0 }
};

TokenizerParseInfo parseSoundEvent[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_STRING(SoundEvent, soundName, 0) },
    {    "",                TOK_STRUCTPARAM | TOK_F32(SoundEvent, radius,    100)  },
    {    "",                TOK_STRUCTPARAM | TOK_F32(SoundEvent, fade,        20) },
    {    "",                TOK_STRUCTPARAM | TOK_F32(SoundEvent, volume,    1) },
    {    "\n",            TOK_END,                         0},
    {    "", 0, 0 }                         
};                                         
extern TokenizerParseInfo ParseFxBhvr[];

TokenizerParseInfo ParseFxEvent[] =
{
    { "EName",        TOK_STRING(FxEvent,name, 0)        },
    { "Type",         TOK_INT(FxEvent,type,FxTypeCreate), FxTypeTable    },
    { "Inherit",    TOK_FLAGS(FxEvent,inherit,0), FxTransformFlag    },
    { "Update",        TOK_FLAGS(FxEvent,update,0), FxTransformFlag    },
    { "At",            TOK_POOL_STRING | TOK_STRING(FxEvent,at,"Origin")    },
    { "Bhvr",        TOK_STRING(FxEvent,bhvr, 0)        },    //BHVR can't be part of pool memory, because the filename might get expanded.
    { "BhvrOverride",TOK_STRUCT(FxEvent,bhvrOverride, ParseFxBhvr)        },
    { "JEvent",        TOK_STRING(FxEvent,jevent, 0)    },
    { "CEvent",        TOK_STRING(FxEvent,cevent, 0)        },
    { "CDestroy",    TOK_U8(FxEvent,cdestroy, 0)        },
    { "JDestroy",    TOK_U8(FxEvent,jdestroy, 0)        },
    { "CRotation",    TOK_FLAGS(FxEvent,crotation, 0), FxCeventCollisionRotation },
    { "ParentVelocityFraction",    TOK_F32(FxEvent,parentVelFraction, 0) },
    { "CThresh",    TOK_F32(FxEvent,cthresh, 0)        },
    { "HardwareOnly",TOK_BOOLFLAG(FxEvent,bHardwareOnly, 0)        },
    { "SoftwareOnly",TOK_BOOLFLAG(FxEvent,bSoftwareOnly, 0)        },
    { "PhysicsOnly",TOK_BOOLFLAG(FxEvent,bPhysicsOnly, 0)        },
    { "CameraSpace",TOK_BOOLFLAG(FxEvent,bCameraSpace, 0)        },
    { "RayLength",    TOK_F32(FxEvent,fRayLength, 0.0f)        },
    { "AtRayFx",    TOK_STRING(FxEvent,atRayFx, 0)    },
    { "Geom",        TOK_UNPARSED(FxEvent,geom)        },
    { "Cape",        TOK_UNPARSED(FxEvent,capeFiles)    },
    { "AltPiv",        TOK_INT(FxEvent,altpiv, 0)    },
    { "AnimPiv",    TOK_STRING(FxEvent,animpiv, 0)    },
    { "Part1",        TOK_REDUNDANTNAME | TOK_UNPARSED(FxEvent,part)    },
    { "Part2",        TOK_REDUNDANTNAME | TOK_UNPARSED(FxEvent,part)    },
    { "Part3",        TOK_REDUNDANTNAME | TOK_UNPARSED(FxEvent,part)    },
    { "Part4",        TOK_REDUNDANTNAME | TOK_UNPARSED(FxEvent,part)    },
    { "Part5",        TOK_REDUNDANTNAME | TOK_UNPARSED(FxEvent,part)    },
    { "Part",        TOK_UNPARSED(FxEvent,part)    },
    { "Anim",        TOK_STRING(FxEvent,anim, 0)        },
    { "SetState",    TOK_POOL_STRING | TOK_STRING(FxEvent,newstate, 0)    },
    { "ChildFx",    TOK_STRING(FxEvent,childfx, 0)    },
    { "Magnet",        TOK_STRING(FxEvent,magnet, 0)    },
    { "LookAt",        TOK_STRING(FxEvent,lookat, 0)    },
    { "PMagnet",    TOK_STRING(FxEvent,pmagnet, 0)    },
    { "POther",        TOK_STRING(FxEvent,pother, 0)    },
    { "Splat",        TOK_STRUCT(FxEvent,splat,    parseSplatEvent) }, //Convert sound, part and others over to this
    { "Sound",        TOK_STRUCT(FxEvent,sound,    parseSoundEvent) },
    { "SoundNoRepeat", TOK_INT(FxEvent, soundNoRepeat, 0)    },
    { "LifeSpan",    TOK_F32(FxEvent,lifespan, 0)    },
    { "LifeSpanJitter",    TOK_F32(FxEvent,lifespanjitter, 0)    },
    { "Power",        TOK_RG(FxEvent,power)        },
    { "While",        TOK_UNPARSED(FxEvent,whilebits)},
    { "Until",        TOK_UNPARSED(FxEvent,untilbits)},
    { "WorldGroup",    TOK_STRING(FxEvent,worldGroup, 0)},
    { "Flags",        TOK_FLAGS(FxEvent,fxevent_flags,    0),    ParseFxEventFlags    },

    { "End",        TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseFxCondition[] =
{
    { "On",            TOK_POOL_STRING | TOK_STRING(FxCondition,on, 0)    },
    { "Time",        TOK_F32(FxCondition,time, 0)                    },
    { "DayStart",    TOK_F32(FxCondition,dayStart, 0)                },
    { "DayEnd",        TOK_F32(FxCondition,dayEnd, 0)                    },
    { "Dist",        TOK_F32(FxCondition,dist, 0)                    },
    { "Chance",        TOK_F32(FxCondition,chance, 0)                    },
    { "DoMany",        TOK_U8(FxCondition,domany, 0)                    },
    { "Repeat",        TOK_U8(FxCondition,repeat, 0)                    },
    { "RepeatJitter",TOK_U8(FxCondition,repeatJitter, 0)            },
    { "TriggerBits",TOK_UNPARSED(FxCondition,triggerbits)            },
    { "Event",        TOK_STRUCT(FxCondition,events,ParseFxEvent)        },
    { "Random",        TOK_U8(FxCondition,randomEvent, 0)                },
    { "ForceThreshold", TOK_F32(FxCondition,forceThreshold,0.0f)    },
    { "End",        TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseFxInput[] =
{
    {"InpName",        TOK_STRING(FxInput,name, 0)        },
    {"End",            TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseFxInfo[] =
{
    { "FxInfo",        TOK_IGNORE,    0 }, // hack so we can use the existing list structure for a bit
    { "Name",        TOK_CURRENTFILE(FxInfo,name)                                                },
    { "FileAge",    TOK_TIMESTAMP(FxInfo,fileAge)                                },
    { "LifeSpan",    TOK_INT(FxInfo,lifespan, 0)                                                },
    { "Lighting",    TOK_INT(FxInfo,lighting, 0)                                                },
    { "Input",        TOK_STRUCT(FxInfo,inputs,ParseFxInput)        },
    { "Condition",    TOK_STRUCT(FxInfo,conditions,ParseFxCondition)    },
    { "Flags",        TOK_FLAGS(FxInfo,fxinfo_flags,0),            ParseFxInfoFlags    },
    { "PerformanceRadius",TOK_F32(FxInfo,performanceRadius, 0)                                        },
    { "OnForceRadius", TOK_F32(FxInfo,onForceRadius, 0.0f)                },
    { "AnimScale",    TOK_F32(FxInfo,animScale,        1),                                        },
    { "ClampMinScale",    TOK_VEC3(FxInfo, clampMinScale), },
    { "ClampMaxScale",    TOK_VEC3(FxInfo, clampMaxScale), },
    { "End",        TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseFxInfoList[] =
{
    { "FxInfo",        TOK_STRUCT(FxInfoList,fxinfos,    ParseFxInfo) },
    { "", 0, 0 },
} ;

int fxInfoNameCmp(const void* info1Data, const void* info2Data)
{
    const FxInfo ** info1 = (const FxInfo **)info1Data;
    const FxInfo ** info2 = (const FxInfo **)info2Data;
    return stricmp( (*info1)->name, (*info2)->name );
}

void fxExpandFileName(char **str, char *thisfxpath)
{
    char buf[256];
    if (!*str)
        return;
    forwardSlashes(*str);
    while (**str=='/')
        strcpy(*str, *str + 1);
    if(strnicmp(*str, ":", 1) == 0)  //Bhvrs
    {
        sprintf(buf, "%s/%s",thisfxpath , (*str)+1); //get rid of "fx/" and ":"
        ParserFreeString(*str);
        *str = ParserAllocString(buf);
    }
}

int fxVerifyAndFixFxInfo(FxInfo *fxinfo)
{
    int numcon, i;
    char thisfxpath[256];
    char *slash;
    int ret=1;
    char filename[MAX_PATH];
    int didCycleWarning=0; // Once per .fx file, please!

    sprintf(filename, "FX/%s", fxinfo->name);

    strcpy(thisfxpath, fxinfo->name);
    slash =  MAX((strrchr(thisfxpath, '/')), (strrchr(thisfxpath, '\\')) );
    if (slash)
        *slash = 0;

    numcon = eaSize(&fxinfo->conditions);
    for( i = 0; i < numcon; i++ )
    {
        FxCondition *fxcon = fxinfo->conditions[i];
        int numevents, j;

        numevents = eaSize(&fxcon->events);
        for (j=0; j<numevents; j++) {
            FxEvent *fxevent = fxcon->events[j];
            int numparts, k, numgeos;

            if (!didCycleWarning && fxcon->on && stricmp(fxcon->on, "cycle")==0) {
                if (fxevent->lifespan == 0) {
                    // Cycling, no lifespan
                    ErrorFilenamef(filename, "The FX %s has an event On CYCLE, but has no lifespan!  Things will build up forever.", fxinfo->name);
                    didCycleWarning = 1;
                    ret = 0;
                }
            }

            // Scale up CThresh (and square it) so that we don't have huge, unmanagable numbers in the .fx file
            if ( fxevent->cthresh )
            {
                fxevent->cthresh *= 1000.0f;
                fxevent->cthresh *= fxevent->cthresh;
            }

            numparts = eaSize(&fxevent->part);
            for (k=0; k<numparts; k++) {
                char partFilename[MAX_PATH];
                char *partName;

                fxExpandFileName(&fxevent->part[k]->params[0], thisfxpath);
                partName = fxevent->part[k]->params[0];

                sprintf(partFilename, "FX/%s", partName);
                if (!strEndsWith(partName, ".part")) {
                    ErrorFilenamef(filename, "References particle system that does not end in .part! ('%s')\n", partName);
                    ret=0;
                }else {
#if CLIENT
                    if (game_state.fxdebug) {
                        if (!fileExists(partFilename)) {
                            ErrorFilenamef(filename, "References particle system that does not exist! ('%s')\n", partName);
                            ret=0;
                        }
                    }
#endif
                }
            }

            numgeos = eaSize(&fxevent->geom);
            // TODO: After inital bunch are fixed, change this to happen on server as well and remove fxdebug->rebuild bins from down below
#if CLIENT
            if (game_state.fxdebug) {
                for (k=0; k<numgeos; k++) {
                    char *geometryName = fxevent->geom[k]->params[0];
                    if( !geometryName || 0 == stricmp( geometryName, "Parent" ) ) {
                        // OK!
                    } else {
                        // Verify!
                        char *path;
                        if( (path = objectLibraryPathFromObj( geometryName ) ) ) {
                            // OK!
                        } else {
                            ErrorFilenamef(filename, "References geometry that does not exist! ('%s')\n", geometryName);
                            ret = 0;
                        }
                    }
                }
            }
#endif
        }
    }

    return ret;
}

bool fxPreloadFxInfoPreProcess(ParseTable* a, void* structptr)
{
    FxInfoList * fx_infolist = (FxInfoList *)structptr;
    //woomer changes, check with Mark that these are OK
    char buf2[1024];
    int num_structs, i;
    FxInfo * fxinfo;
    int ret=1;

    //Clean Up FxInfo names, and sort them
    num_structs = eaSize(&fx_infolist->fxinfos);
    for (i = 0; i < num_structs; i++)
    {
        fxinfo = fx_infolist->fxinfos[i];
        fxCleanFileName(buf2, fxinfo->name); //(buf2 will always be shorter than info->name)
        strcpy(fxinfo->name, buf2);
        ret &= fxVerifyAndFixFxInfo(fxinfo);
    }
    qsort(fx_infolist->fxinfos, num_structs, sizeof(void*), fxInfoNameCmp);
    return ret;
}

// load all the behaviors and infos in the fx directory
void fxPreloadFxInfo()
{
    char *dir=STATE_STRUCT.nofx?NULL:"fx";
    char *filetype=STATE_STRUCT.nofx?"fx/generic/dummy.fx":".fx";
    int flags=0;
    char *persistFilename = STATE_STRUCT.nofx?0:"fxinfo.bin";
    //loadstart_printf("Preloading fx info");
#ifdef SERVER
    //JE: This can only be shared on the server, the client munges too much data, and we don't care
    // about sharing this on the client anyway!
    ParserLoadFilesShared("SM_FXinfo_SERVER", dir, filetype, persistFilename, flags, ParseFxInfoList, &fx_infolist, sizeof(fx_infolist), NULL, NULL, fxPreloadFxInfoPreProcess, NULL, NULL);
#else
    if (game_state.fxdebug)
        flags = PARSER_FORCEREBUILD;
    ParserLoadFiles(dir, filetype, persistFilename, flags, ParseFxInfoList, &fx_infolist, NULL, NULL, fxPreloadFxInfoPreProcess);
#endif

    //loadend_printf("");
}

void fxBuildFxStringHandles()
{
    int i;
    int iSize = eaSize(&fx_infolist.fxinfos);

    // If this assert goes off, either move the code that is adding things to the StringTable *before* the FX loading code,
    //   or we need to change how the FX string table is handled (make it a seperate string table from the rest of the world)
    assertmsg(numIndexedStringsInStringTable()==0, "Someone added a string to the string table before the fx strings were loaded.  This is not supported.");

    for(i = 0; i < iSize; i++)
    {
        FxInfo* info = fx_infolist.fxinfos[i];
        allocAddIndexedString(info->name);
    }

    // Sort strings in the "string table" to make sure that fx names are always in the
    // same order.
    stringReorder();
}



#ifdef CLIENT

extern FxEngine fx_engine;

typedef struct ConditionFlag
{
    char condition[FXSTRLEN];
    int  flag;
} ConditionFlag;

static ConditionFlag allconditions[] =
{
    { "none"        ,    0                        },
    { "time"        ,    FX_STATE_TIME            },
    { "timeofday"    ,    FX_STATE_TIMEOFDAY        },
    { "primedied"    ,    FX_STATE_PRIMEDIED        },
    { "primehit"    ,    FX_STATE_PRIMEHIT        },
    { "primebeamhit",   FX_STATE_PRIMEBEAMHIT    },
    { "cycle"        ,    FX_STATE_CYCLE            },
    { "prime1hit"    ,    FX_STATE_PRIME1HIT        }, //these hits should be made procedural
    { "prime2hit"    ,    FX_STATE_PRIME2HIT        },
    { "prime3hit"    ,    FX_STATE_PRIME3HIT        },
    { "prime4hit"    ,    FX_STATE_PRIME4HIT        },
    { "prime5hit"    ,    FX_STATE_PRIME5HIT        },
    { "prime6hit"    ,    FX_STATE_PRIME6HIT        },
    { "prime7hit"    ,    FX_STATE_PRIME7HIT        },
    { "prime8hit"    ,    FX_STATE_PRIME8HIT        },
    { "prime9hit"    ,    FX_STATE_PRIME9HIT        },
    { "prime10hit"    ,    FX_STATE_PRIME10HIT        },
    { "triggerbits"    ,    FX_STATE_TRIGGERBITS    },
    { "death"        ,    FX_STATE_DEATH            },
    { "collision"    ,    FX_STATE_COLLISION        },
    { "force"        ,    FX_STATE_FORCE            },
    { "defaultSurface",    FX_STATE_DEFAULTSURFACE    }, //if you did no other condition this tick, do this.  Hack to let sound guy specify default sounds.
    { 0                ,    0                        },
};

void addToRealInputs( char * inp, char usedInputs[128][32], int * usedInputsCount )
{
    int i, alreadyListed = 0;

    if( inp )
    {
        for( i = 0 ; i < *usedInputsCount ; i++ )
        {
            if( 0 == stricmp(inp, usedInputs[i] ) )
                alreadyListed = 1;
        }
        if( !alreadyListed )
        {
            strcpy( usedInputs[*usedInputsCount], inp );
            (*usedInputsCount)++;
        }
        assert( *usedInputsCount < 128 );
    }
}

int fxInitFxInfo(FxInfo* fxinfo)
{
    int i, j;
    int num, numevents;

    int usedInputsCount = 0;
    char usedInputs[128][32];

    //For figuring out the properties of the system
    int totalParticleSystems = 0;
    int totalGeometries = 0;
    int totalCapes = 0;
    int totalAnimations = 0;
    int totalSounds = 0;
    int totalSplats = 0;
    int totalWorldGroups = 0;
    int totalChildFx = 0;
    int totalSoundRadius = 0;

    //2. ############ Error check ########################################
    //NOte this was gummed up a little when the new parser arrived, so check.
    if(    eaSize(&fxinfo->conditions) > MAX_DUMMYCONDITIONS ||
        //eaSize(&fxinfo->inputs) == 0 ||
        eaSize(&fxinfo->inputs) > MAX_DUMMYINPUTS ||
        eaSize(&fxinfo->conditions) == 0 ||
        eaSize(&fxinfo->conditions[0]->events) == 0 ||
        fxinfo->conditions[0]->events[0]->type == FxTypeNone
        )
    {
        printToScreenLog( 1, "FXINFO: Syntax Error." );
        return 0;
    }
    //############################ End Error Check #########################################

    if (fxinfo->animScale == 0.0) {
        fxinfo->animScale = 1.0;
    }

    strcpy( usedInputs[0], "Origin" ); 
    usedInputsCount = 1;

    //5. Do all the general conversions on conditions:
    //TO DO, this is silly, it either needs to work for multiple conditions, or throw it away.
    num = eaSize(&fxinfo->conditions);
    for( i = 0; i < num; i++ )
    {
        FxCondition *fxcon = fxinfo->conditions[i];

        //A. Find the condition trigger and set it
        fxcon->trigger = 0; //redundant?

        //if no on was given, see if you can guess it
        if( !fxcon->on )
        {
            if(fxcon->triggerbits)
            {
                fxcon->on = malloc( strlen("TriggerBits") + 1 );
                strcpy( fxcon->on, "TriggerBits" );
            }
            else //if no other condition, assume Time 0
            {
                fxcon->on = malloc( strlen("Time") + 1 );
                strcpy( fxcon->on, "Time" );
            }
        }
        
        if ( 0 == stricmp(fxcon->on, "force") )
        {
            fxinfo->hasForceNode = 1;
        }

        //Then set the conditions, maybe revisit
        for( j = 0 ; allconditions[j].condition[0] ; j++ )
        {
            if( 0 == _stricmp(fxcon->on, allconditions[j].condition ) )
            {
                fxcon->trigger |= allconditions[j].flag;
                break;
            }
            if( allconditions[j].condition == 0 )
                break;
        }
        //############## Error Checking  #####################
        if( !fxcon->trigger )
            printToScreenLog(1, "FXINFO: Don't recognize trigger: %s", fxcon->on);

        if(fxcon->triggerbits && !( fxcon->trigger & FX_STATE_TRIGGERBITS ))
            printToScreenLog(1, "FX: %s, Parent Bits set in condition, with no flag to check parent bits", fxinfo->name);

        //Since the whole default thing is a bit of a hack anyway, I decided to not rearrange conditions myself but just throw up an error.
        if( ( fxcon->trigger & FX_STATE_DEFAULTSURFACE ) && num-1 != i )
            Errorf( "Condition with trigger 'default' is not last in the fx script.  Erratic behavior is possible");
        //############# End Error checking ###############


        //B. Find the condition trigger and set it
        if( fxcon->triggerbits )
        {
            int num_triggers, num_bits, k, j, success;
            //assert( eaSize( &fxcon->triggerbits ) == 1 ); //only one line of triggerbits
            num_triggers = eaSize( &fxcon->triggerbits );
            fxcon->triggerstates = malloc(num_triggers * sizeof(int *));
            for( j = 0 ; j < num_triggers ; j++ )
            {
                fxcon->triggerstates[j] = calloc(STATE_ARRAY_SIZE, sizeof(int));
                num_bits = eaSize( &fxcon->triggerbits[j]->params );
                for( k = 0 ; k < num_bits ; k++ )
                {
                    success = seqSetStateFromString( fxcon->triggerstates[j], fxcon->triggerbits[j]->params[k] );
                    if( !success )
                        printToScreenLog(1, "FX: I don't recognize the bit %s", fxcon->triggerbits[j]->params[k]);
                }
            }
        }

        //Tricky bit.  If fx has events triggered when the fx is killed, it needs to
        //handle itself a little differently.  Theoretically, I shouldn't need this, and
        //all death would be handled the same way, but it would require substantial reorganizing
        //and possibly mess up already existing fx scripts...
        if( fxcon->trigger & FX_STATE_DEATH )
        {
            fxinfo->hasEventsOnDeath = 1;
        }
    }

    //replace ":" in particle system and behavior's in the events with the folder of this fx
    //And get sound idxs
    //And init default values for event powers
    //And build list of actually used inputs
    {
        char * slash;
        char thisfxpath[256];
        int part_count, cape_count;
        int i, j, k;

        strcpy(thisfxpath, fxinfo->name);
        slash =  MAX((strrchr(thisfxpath, '/')), (strrchr(thisfxpath, '\\')) );
        if (slash)
            *slash = 0;
        num = eaSize(&fxinfo->conditions);
        for( i = 0; i < num; i++ )
        {
            FxCondition *fxcon = fxinfo->conditions[i];

            numevents = eaSize(&fxcon->events);
            for( j = 0; j < numevents; j++ )
            {
                FxEvent *fxevent = fxcon->events[j];

                fxExpandFileName(&fxevent->bhvr, thisfxpath);
                fxExpandFileName(&fxevent->jevent, thisfxpath);
                fxExpandFileName(&fxevent->cevent, thisfxpath);
                fxExpandFileName(&fxevent->childfx, thisfxpath);
#ifdef NOVODEX_FLUIDS
                fxExpandFileName(&fxevent->fluid, thisfxpath);
#endif

                //For each particle, Fill out the rest of the path if there's a colon
                part_count = eaSize( &fxevent->part );
                if (part_count > MAX_PSYSPERFXGEO ) {
                    // Too many!
                    char filename[MAX_PATH];
                    sprintf(filename, "FX/%s", fxinfo->name);
                    ErrorFilenamef(filename, "Error: too many particle systems, can have at most %d, found %d", MAX_PSYSPERFXGEO, part_count);
                    part_count = MAX_PSYSPERFXGEO;
                }
                for(k = 0 ; k < part_count ; k++)
                {
                    fxExpandFileName(&fxevent->part[k]->params[0], thisfxpath);

                    //TO DO: possibly check the power vals on the particle system?
                }

                cape_count = eaSize( &fxevent->capeFiles );
                for(k = 0 ; k < cape_count ; k++)
                {
                    fxExpandFileName(&fxevent->capeFiles[k]->params[0], thisfxpath);
                }

                 if (fxcon->trigger & FX_STATE_CYCLE) {
                    if (fxevent->lifespan == 0) {
                        printToScreenLog(1, "The FX %s has an event On CYCLE, but has no lifespan!  Things will build up forever.", fxinfo->name);
                    }
                }

                //For figuring out if this is a sound only system
                totalParticleSystems+= eaSize( &fxevent->part );
                totalGeometries        += eaSize( &fxevent->geom );
                totalCapes            += eaSize( &fxevent->capeFiles );
                totalSounds            += eaSize( &fxevent->sound );
                totalSplats            += eaSize( &fxevent->splat );

                totalChildFx        += fxevent->childfx ? 1 : 0;
                totalWorldGroups    += fxevent->worldGroup ? 1 : 0;
                totalAnimations        += fxevent->anim ? 1 : 0;

                //Get sound radius
                {
                    int k;
                    SoundEvent * sound;
                    int iSize = eaSize( &fxevent->sound );

                    for( k = 0; k < iSize; k++ )
                    {
                        sound = fxevent->sound[k];
                        sound->isLoopingSample = sndIsLoopingSample(sound->soundName);

                        totalSoundRadius = MAX( totalSoundRadius, sound->radius );
                    }
                }

                if (fxevent->soundNoRepeat && (fxevent->soundNoRepeat >= eaSize(&fxevent->sound)))
                {
                    char filename[MAX_PATH];
                    sprintf(filename, "FX/%s", fxinfo->name);
                    ErrorFilenamef(filename, "Error: SoundNoRepeat exceeds the number of sounds in the event.");
                    fxevent->soundNoRepeat = eaSize(&fxevent->sound);
                }

                //Set defaults for event powers
                if( fxevent->power[ MIN_ALLOWABLE_POWER ] == 0 )
                    fxevent->power[ MIN_ALLOWABLE_POWER ] = 1;
                if( fxevent->power[ MAX_ALLOWABLE_POWER ] == 0 )
                    fxevent->power[ MAX_ALLOWABLE_POWER ] = 10;

                if( fxevent->power[ MIN_ALLOWABLE_POWER ] > 10 )
                {
                    printToScreenLog(1, "FXINFO: Bad Power Level in this FX" );
                    fxevent->power[ MIN_ALLOWABLE_POWER ] = 10;
                }
                if( fxevent->power[ MAX_ALLOWABLE_POWER ] > 10 )
                {
                    printToScreenLog(1, "FXINFO: Bad Power Level in this FX" );
                    fxevent->power[ MAX_ALLOWABLE_POWER ] = 10;
                }

                if( fxevent->power[ MIN_ALLOWABLE_POWER ] > fxevent->power[1] )
                {
                    U8 temp;
                    printToScreenLog(1, "FXINFO: Bad Power Level in this FX" );
                    temp = fxevent->power[ MIN_ALLOWABLE_POWER ];
                    fxevent->power[ MIN_ALLOWABLE_POWER ] = fxevent->power[1];
                    fxevent->power[ MAX_ALLOWABLE_POWER ] = temp;
                }

                // This is to keep backwards compatibility with "type" system
                // for describing what is inherited and what is updated in terms
                // of the parent matrix
                switch (fxevent->type)
                {
                case FxTypeLocal:
                    fxevent->inherit = 0;
                    fxevent->update = FX_ROTATION | FX_SCALE | FX_POSITION;
                    fxevent->type = FxTypeCreate;
                    break;
                case FxTypeStart:
                    fxevent->inherit = FX_ROTATION | FX_SCALE | FX_POSITION;
                    fxevent->update = 0;
                    fxevent->type = FxTypeCreate;
                    break;
                case FxTypePosit:
                    fxevent->inherit = FX_ROTATION | FX_SCALE | FX_POSITION;
                    fxevent->update = FX_POSITION;
                    fxevent->type = FxTypeCreate;
                    break;
                case FxTypeStartPositOnly:
                    fxevent->inherit = FX_POSITION;
                    fxevent->update = 0;
                    fxevent->type = FxTypeCreate;
                    break;
                case FxTypePositOnly:
                    fxevent->inherit = FX_POSITION;
                    fxevent->update = FX_POSITION;
                    fxevent->type = FxTypeCreate;
                    break;
                case FxTypeCreate:
                    break;
                case FxTypeSetState:
                case FxTypeIncludeFx:
                case FxTypeSetLight:
                case FxTypeDestroy:
                    break;
                }

                //Set While Bits, if needed
                if( fxevent->whilebits )
                {
                    int i, num_bits;
                    assert( eaSize( &fxevent->whilebits ) == 1 ); //only one line of whilebits
                    fxevent->whilestate = calloc(STATE_ARRAY_SIZE, sizeof(int));
                    num_bits = eaSize( &fxevent->whilebits[0]->params );
                    for( i = 0 ; i < num_bits ; i++ )
                        seqSetStateFromString( fxevent->whilestate, fxevent->whilebits[0]->params[i] );
                }


                //Set Until bits, if needed
                if( fxevent->untilbits )
                {
                    int i, num_bits;
                    assert( eaSize( &fxevent->untilbits ) == 1 ); //only one line of whilebits
                    fxevent->untilstate = calloc(STATE_ARRAY_SIZE, sizeof(int));
                    num_bits = eaSize( &fxevent->untilbits[0]->params );
                    for( i = 0 ; i < num_bits ; i++ )
                        seqSetStateFromString( fxevent->untilstate, fxevent->untilbits[0]->params[i] );
                }

                //Build a list of all inputs that are actually used in the fx
                //Check against actual list below
                addToRealInputs( fxevent->at, usedInputs, &usedInputsCount );
                addToRealInputs( fxevent->lookat, usedInputs, &usedInputsCount );
                addToRealInputs( fxevent->magnet, usedInputs, &usedInputsCount );
                addToRealInputs( fxevent->pother, usedInputs, &usedInputsCount );
                addToRealInputs( fxevent->pmagnet, usedInputs, &usedInputsCount );
                //End build usedInputs list
            }
        }

        if( totalParticleSystems || totalGeometries || totalCapes || totalSplats || totalChildFx || totalWorldGroups    || totalAnimations  )
        {
            fxinfo->fxinfo_flags &= ~FX_SOUND_ONLY;
        }
        else
        {
            fxinfo->fxinfo_flags |= FX_SOUND_ONLY;
            if( fxinfo->lifespan > 0 && fxinfo->lifespan < 30 )
                fxinfo->performanceRadius = totalSoundRadius;
        }
        //TO DO add flags for distance to bother with the effect.


        //Get inputs that are actually used
        {
            int i;

            for( i = 0 ; i < usedInputsCount ; i++ )
            { 
                char * input;
                int okInput;

                input  = usedInputs[i];
                okInput = 0;

                //Is this an input?
                if( 0 == stricmp( input, "Origin" ) )
                    okInput = 1;
                else if( 0 == stricmp( input, "Target" ) )
                    okInput = 1;
                else if( 0 == stricmp( input, "Root" ) )
                    okInput = 1;
                else if( 0 == stricmp( input, "T_Root" ) )
                    okInput = 1;
                else if( 0 == strnicmp( input, "T_", 2 ) )
                    okInput = bone_NameIsValid( input + 2 );
                else if( 0 == strnicmp( input, "C_", 2 ) )
                    okInput = bone_NameIsValid( input + 2 );
                else
                    okInput = bone_NameIsValid( input );

                if( okInput )
                {
                    fxinfo->inputsReal[fxinfo->inputsRealCount] = calloc( 1, strlen( input ) + 1 );
                    strcpy( fxinfo->inputsReal[fxinfo->inputsRealCount], input);
                    fxinfo->inputsRealCount++;
                }
            }

            //Then do a dumb trick to make target in the two spot
            for( i = 0 ; i < fxinfo->inputsRealCount ; i++ )
            {
                if( i > 1 && 0 == stricmp( "Target", fxinfo->inputsReal[i] ) )
                {
                    //Swap origin to the top
                    char * temp;
                    temp = fxinfo->inputsReal[i];
                    fxinfo->inputsReal[i] = fxinfo->inputsReal[1];
                    fxinfo->inputsReal[1] = temp;
                }
            }
        }
        //End find actually used inputs
    }

    return 1;
}


//Development only func, for loading fxfiles not in the binary.
static FxInfo * fxLoadFxInfo(char fname[])
{
    FxInfo    * fxinfo = 0;
    int        fileisgood = 0;
    TokenizerHandle    tok;
    char buf[1000];

    errorLogFileIsBeingReloaded(fname);

    tok = TokenizerCreate(fname);
    if (tok)
    {
        fxinfo = (FxInfo*)listAddNewMember(&fx_engine.fxinfos, sizeof(FxInfo));
        assert(fxinfo);
        listScrubMember(fxinfo, sizeof(FxInfo));
        ParserInitStruct(fxinfo, 0, ParseFxInfo);
        fileisgood = TokenizerParseList(tok, ParseFxInfo, fxinfo, TokenizerErrorCallback);
        if( !fileisgood )
        {
            listFreeMember( fxinfo, &fx_engine.fxinfos );
            fxinfo = 0;
        }
        else
        {
            fxCleanFileName(buf, fxinfo->name); //(buf will always be shorter than info->name)
            strcpy(fxinfo->name, buf);
            fxVerifyAndFixFxInfo(fxinfo);
            fx_engine.fxinfocount++;
        }
    }
    TokenizerDestroy(tok);

    return fxinfo;
}

int fxInfoNameCmp2(const void* info1Data, const void* info2Data)
{
    const FxInfo * info1 = (const FxInfo *)info1Data;
    const FxInfo ** info2 = (const FxInfo **)info2Data;
    return stricmp( info1->name, (*info2)->name );
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Look for the FxInfo already loaded.  If not loaded, or if always reload, load it up.
FxInfo * fxGetFxInfo(const char fx_name[])
{
    FxInfo * fxinfo = 0;
    char fx_name_cleaned_up[FX_MAX_PATH];
    int numinfos;
    FxInfo dummy = {0};
    FxInfo * * dptr;

    if( !fx_name )
        return NULL;

    fxCleanFileName(fx_name_cleaned_up, fx_name);

    // ## Search Mark's binary of loaded fxinfos.
    dummy.name = fx_name_cleaned_up;
    numinfos = eaSize(&fx_infolist.fxinfos);
    dptr = bsearch(&dummy, fx_infolist.fxinfos, numinfos,
        sizeof(FxInfo*),fxInfoNameCmp2);
    if( dptr )
    {
        fxinfo = *dptr;
        assert(fxinfo);
        //Development only
        if (!global_state.no_file_change_check && isDevelopmentMode())
            fxinfo->fileAge = fileLastChanged( "bin/fxinfo.bin" );
        //End developmetn only
    }

    //Failure in production mode == very bad data.

    // ## Development only.
    if (!global_state.no_file_change_check && isDevelopmentMode())
    {
        // always prefer the debug version to the binary version  (Notice youngest always found first)
        {
            FxInfo * dev_fxinfo;
            for( dev_fxinfo = fx_engine.fxinfos ; dev_fxinfo ; dev_fxinfo = dev_fxinfo->next)
            {
                if(!_stricmp(fx_name_cleaned_up, dev_fxinfo->name))
                {
                    fxinfo = dev_fxinfo;
                    break;
                }
            }
        }

        {
            char buf[500];
            STR_COMBINE_BEGIN(buf);
            STR_COMBINE_CAT("fx/");
            STR_COMBINE_CAT(fx_name_cleaned_up);
            STR_COMBINE_END();
            //sprintf(buf,"%s%s", "fx/", fx_name_cleaned_up );

            //Reload the fxinfo from the file under these conditions
            if( !fxinfo || //it's brand new sysinfo
                (fxinfo && fileHasBeenUpdated( buf, &fxinfo->fileAge )) )  //the fxinfo file itself has changed
            {
                fxinfo = fxLoadFxInfo(buf);
            }
        }
    }
    // ## End development only

    if( fxinfo )
    {
        if (!fxinfo->initialized)
        {
            if( !fxInitFxInfo( fxinfo ) )
            {
                Errorf( "Error in file %s", fx_name_cleaned_up );
                return 0;
            }
            fxinfo->initialized = 1;
        }
    }
    else
    {
        printToScreenLog( 1, "FXINFO: Failed to get %s", fx_name_cleaned_up );
    }

    // ## Development only: if fxinfo fails for any reason, try to remove it from the run time load list
    if( 0 && !fxinfo )
    {
        FxInfo * checkerinfo;
        FxInfo * checkerinfo_next;
        for(checkerinfo = fx_engine.fxinfos ; checkerinfo ; checkerinfo = checkerinfo_next)
        {
            checkerinfo_next = checkerinfo->next;
            if( !checkerinfo->name || !_stricmp( fx_name_cleaned_up, checkerinfo->name ) )
            {
                listFreeMember( checkerinfo, &fx_engine.fxinfos );
                fx_engine.fxinfocount--;
                assert( fx_engine.fxinfocount >= 0 );
            }
        }
    }
    //## end development only

    return fxinfo;
}

// Determine "average" hue of particle system, and use this to decide the hue shift
static F32 oneOver255 = 1.0/255.;
void fxInfoCalcEffectiveHue(FxInfo *fxinfo)
{
#define GRANULARITY 36 // How many segments to use while histograming the colors
    F32 histogram[GRANULARITY]={0};
    int best=0;
    int i, j, conditionNum, eventNum;
    Vec3 rgb, hsv;

    if (fxinfo->effectiveHueCalculated)
        return;

    //printf("%d Conditions\n", eaSize(&fxinfo->conditions));
    for (conditionNum=eaSize(&fxinfo->conditions)-1; conditionNum>=0; conditionNum--) {
        FxCondition *cond = fxinfo->conditions[conditionNum];
        //printf("\t%d Events\n", eaSize(&cond->events));
        for (eventNum=eaSize(&cond->events)-1; eventNum>=0; eventNum--) {
            FxEvent *event = cond->events[eventNum];
            //printf("\t\t%d Parts\n", eaSize(&event->part));
            for(i=eaSize( &event->part )-1; i>=0; i--) 
            {
                ParticleSystemInfo *sysInfo = partGetSystemInfo(event->part[i]->params[0]);
                if (sysInfo) {
                    ColorPath *path = &sysInfo->colorPathDefault;
                    int efflength = MAX(path->length, sysInfo->fade_out_by); // Grow to length of fade out
                    efflength = MIN(300, efflength); // cap to 300 in case it never fades out or something...
                    for (j=0; j<efflength; j++) {
                        // Calc weights based on alpha and size and number of particles emitted
                        F32 weight;
                        F32 alphaPercent;
                        int pathOffs = MIN(path->length-1, j);
                        if (j> sysInfo->fade_out_by)
                            break;

                        if(    j >= sysInfo->fade_out_start ) //Im fading out
                        {
                            if (sysInfo->fade_out_by <= sysInfo->fade_out_start)
                                alphaPercent = 0.0;
                            else
                                alphaPercent = 1.0 - ( (j - sysInfo->fade_out_start)/ (sysInfo->fade_out_by - sysInfo->fade_out_start) );
                        }
                        else if( j < sysInfo->fade_in_by )  //Im fading in
                        {
                            alphaPercent = j / sysInfo->fade_in_by;
                        }
                        else //Im not fading anywhere
                        {
                            alphaPercent = 1.0;
                        }
                        alphaPercent = sysInfo->alpha[0]*oneOver255 * alphaPercent;
                        weight = alphaPercent * sysInfo->new_per_frame[0];

                        rgb[0] = path->path[pathOffs*3+0]*oneOver255;
                        rgb[1] = path->path[pathOffs*3+1]*oneOver255;
                        rgb[2] = path->path[pathOffs*3+2]*oneOver255;
                        rgbToHsv(rgb, hsv);
                        if (hsv[0] >= 0.0) {
                            int index = hsv[0] * GRANULARITY/360.f;
                            if (index >= GRANULARITY)
                                index -= GRANULARITY;
                            histogram[index]+=weight;
                        }
                    }
                }
            }
        }
    }

    for (i=1; i<ARRAY_SIZE(histogram); i++) {
        if (histogram[i] > histogram[best])
            best = i;
    }

    fxinfo->effectiveHueCalculated = 1;
    fxinfo->effectiveHue = best*360.f/GRANULARITY;
}

bool fxInfoHasFlags(FxInfo *fxinfo, FxInfoFlags flags, bool bIncludeChildFx)
{
    int conditionNum, eventNum;

    if(fxinfo->fxinfo_flags & flags)
        return true;

    if(!bIncludeChildFx)
        return false;

    // run thru all conditions and events looking for childfx, and recursively check for all children
    for (conditionNum=eaSize(&fxinfo->conditions)-1; conditionNum>=0; conditionNum--)
    {
        FxCondition *cond = fxinfo->conditions[conditionNum];
        for (eventNum=eaSize(&cond->events)-1; eventNum>=0; eventNum--)
        {
            FxEvent *event = cond->events[eventNum];
            char * childfx = event->childfx;
            if(childfx && childfx[0] != 0)
            {
                FxInfo * child_fxinfo = fxGetFxInfo(childfx);
                if(child_fxinfo && fxInfoHasFlags(child_fxinfo, flags, true))
                    return true;
            }
        }
    }
    
    return false;
}

static const char* staticMatchStr;

static FileScanAction preloadFxGeometryCallback(char* dir, struct _finddata32_t* data)
{
    if(!(data->attrib & _A_SUBDIR))
    {
        const char* name = data->name;
        
        if(    strEndsWith(name, ".geo") && 
            (!staticMatchStr || strstriConst(name, staticMatchStr)))
        {
            char buffer[MAX_PATH];
            STR_COMBINE_BEGIN(buffer);
            STR_COMBINE_CAT(dir);
            STR_COMBINE_CAT("/");
            STR_COMBINE_CAT(name);
            STR_COMBINE_END();
            //printf("preloading: %s\n", buffer);
            geoLoad(buffer, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
        }
    }
    
    return FSA_EXPLORE_DIRECTORY;
}

static void fxPreloadGeometryHelper(const char* dir, const char* matchStr)
{
    writeConsole(OUTPUT_DEBUG, "\tPreloading %s/", dir);
    staticMatchStr = matchStr;
    fileScanAllDataDirs(dir, preloadFxGeometryCallback);
}

void fxPreloadGeometry()
{
    const char* fxDirs[] = { "fx/", "object_library/fx/", "item_library/",  "object_library/item_library/" };
    int            fxDirLen[ARRAY_SIZE(fxDirs)];
    
    int i;
    
    for(i = 0; i < ARRAY_SIZE(fxDirs); i++)
    {
        fxDirLen[i] = strlen(fxDirs[i]);
    }
    
    // Load all the geometry.

    //fxPreloadGeometryHelper("object_library", NULL);
    //fxPreloadGeometryHelper("player_library", NULL);
    //fxPreloadGeometryHelper("object_library/fx", NULL);
    //fxPreloadGeometryHelper("object_library/item_library", NULL);
    
    // Load all the cape geometry.
    
    fxPreloadGeometryHelper("player_library", "cape");
    fxPreloadGeometryHelper("player_library", "door");
    fxPreloadGeometryHelper("object_library/FX/Flags", NULL);
    
    for(i = 0; i < eaSize(&fx_infolist.fxinfos); i++)
    {
        FxInfo* info = fx_infolist.fxinfos[i];
        int j;
        for(j = 0; j < eaSize(&info->conditions); j++)
        {
            FxCondition* cond = info->conditions[j];
            int k;
            for(k = 0; k < eaSize(&cond->events); k++)
            {
                FxEvent* event = cond->events[k];
                int i;
                for(i = 0; i < eaSize(&event->geom); i++)
                {
                    char* geoName = event->geom[i]->params[0];
                    
                    if(geoName && stricmp(geoName, "Parent"))
                    {
                        char* geoFile = objectLibraryPathFromObj(geoName);
                        
                        if(geoFile)
                        {
                            int j;
                            for(j = 0; j < ARRAY_SIZE(fxDirs); j++)
                            {
                                if(!strnicmp(geoFile, fxDirs[j], fxDirLen[j]))
                                {
                                    break;
                                }
                            }
                            
                            if(j < ARRAY_SIZE(fxDirs))
                            {
                                //printf("loading:  %s/%s\n", geoFile, geoName);
                                geoLoad(geoFile, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
                            }
                            else
                            {
                                //printf("ignoring: %s/%s\n", geoFile, geoName);
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif
