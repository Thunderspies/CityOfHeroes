#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include "seq/animtrack.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/textparser.h>
#include "seq/seqstate.h"
#include "seq/seq.h"
#include <utilitieslib/components/genericlist.h>
#include "graphics/font.h"
#include "time.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/components/HashFunctions.h>
#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include "graphics/FX/fx.h"
#include "graphics/clothnode.h"
#else
#include "cmdparse/cmdserver.h"
#endif
#include "cmdparse/cmdcommon.h"
#include "entity/entity.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>

#define DEFAULT_MOVE_INTERPOLATION_RATE 5
#define DEFAULT_ANIMATION_SCALE 1.0
#define DEFAULT_MOVERATE 0.0
#define DEFAULT_SMOOTHSPRINT  0.0
#define DEFAULT_PITCHANGLE  0.0
#define DEFAULT_PITCHRATE  0.05
#define SEQ_QUICKLOADMOVEHASH 12346
#define SEQ_MAX_PATH 300

// global lists of parsed objects
SERVER_SHARED_MEMORY SeqInfoList seqInfoList;

typedef struct SeqGlobals {
    SeqInfo * dev_seqInfosList;
    int dev_seqInfoCount;
} SeqGlobals;

SeqGlobals seqGlobals;

StaticDefineInt    MoveFlags[] =
{
    DEFINE_INT
    { "Cycle",            SEQMOVE_CYCLE            },
    { "FinishCycle",    SEQMOVE_FINISHCYCLE        },
    { "ReqInputs",        SEQMOVE_REQINPUTS        },
    { "NoInterp",        SEQMOVE_NOINTERP        },
    { "HitReact",        SEQMOVE_HITREACT        },
    { "NoSizeScale",    SEQMOVE_NOSIZESCALE        },
    { "AlwaysSizeScale",SEQMOVE_ALWAYSSIZESCALE    },
    { "FullSizeScale",    SEQMOVE_FULLSIZESCALE    },
    { "MoveScale",        SEQMOVE_MOVESCALE        },
    { "NotSelectable",    SEQMOVE_NOTSELECTABLE    },
    { "SmoothSprint",    SEQMOVE_SMOOTHSPRINT    },
    { "PitchToTarget",    SEQMOVE_PITCHTOTARGET    },
    { "PickRandomly",    SEQMOVE_PICKRANDOMLY    },
    { "FixArmReg",        SEQMOVE_FIXARMREG        },
    { "Hide",            SEQMOVE_HIDE            },
    { "NoCollision",    SEQMOVE_NOCOLLISION        },
    { "WalkThrough",    SEQMOVE_WALKTHROUGH        },
    { "CantMove",        SEQMOVE_CANTMOVE        },
    { "RunIn",            SEQMOVE_RUNIN            },
    { "FixHipsReg",        SEQMOVE_FIXHIPSREG        },
    { "DoNotDelay",        SEQMOVE_DONOTDELAY        },
    { "FixWeaponReg",    SEQMOVE_FIXWEAPONREG    },
    { "NoDraw",            SEQMOVE_NODRAW            },
    { "HideWeapon",        SEQMOVE_HIDEWEAPON        },
    { "HideShield",        SEQMOVE_HIDESHIELD        },
    DEFINE_END
};

StaticDefineInt PlayFxFlags[] =
{
    DEFINE_INT
    { "Constant",        SEQFX_CONSTANT },
    DEFINE_END
};

TokenizerParseInfo ParseSeqFx[] =
{
    { "",                TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(SeqFx, name, 0)                        }, //Name on line trick
    { "",                TOK_STRUCTPARAM | TOK_INT(SeqFx, delay, 0)                    },
    { "",                TOK_STRUCTPARAM | TOK_FLAGS(SeqFx, flags, 0),    PlayFxFlags    },
    { "\n",                TOK_END                                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseAnimP[] =
{
    { "",                TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(AnimP, name, 0)                            }, //Name on line trick
    { "",                TOK_STRUCTPARAM | TOK_INT(AnimP, firstFrame, 0)                    },
    { "",                TOK_STRUCTPARAM | TOK_INT(AnimP, lastFrame, 0)                    },
    { "\n",                TOK_END                                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTypeGfxP[] =
{
    { "",                TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(TypeGfx, type, 0)        }, //Name on line trick
    { "Anim",            TOK_STRUCT(TypeGfx, animP, ParseAnimP)  },
    { "Ragdoll",        TOK_INT(TypeGfx, ragdollFrame,-1)},
    { "PlayFx",            TOK_STRUCT(TypeGfx, seqFx, ParseSeqFx)  },
    { "Scale",            TOK_F32(TypeGfx, scale, DEFAULT_ANIMATION_SCALE)        },
    { "MoveRate",        TOK_F32(TypeGfx, moveRate,0)            },
    { "SmoothSprint",    TOK_F32(TypeGfx, smoothSprint,0)    },
    { "PitchAngle",        TOK_F32(TypeGfx, pitchAngle,0)        },
    { "PitchRate",        TOK_F32(TypeGfx, pitchRate,0)            },
    { "PitchStart",        TOK_F32(TypeGfx, pitchStart,    0)            },
    { "PitchEnd",        TOK_F32(TypeGfx, pitchEnd,    -1)            },
    { "ScaleRootBone",    TOK_STRING(TypeGfx, scaleRootBone,    0)        },
    { "fn",                TOK_POOL_STRING | TOK_CURRENTFILE(TypeGfx, filename),        },

    { "TEnd",            TOK_END                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCycleMoveP[] =
{
    { "",    TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(CycleMoveP,name, 0)    },
    { "\n",    TOK_END,    0,                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseNextMoveP[] =
{
    { "",    TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(NextMoveP,name, 0)    },
    { "\n",    TOK_END,    0,                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseMoveP[] =
{
    { "",                TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(SeqMove, name, 0)                                            }, //Name on line trick
    { "MoveRate",        TOK_F32(SeqMove, moveRate, DEFAULT_MOVERATE)                        },
    { "Interpolate",    TOK_INT(SeqMove, interpolate, DEFAULT_MOVE_INTERPOLATION_RATE)            },
    { "Priority",        TOK_INT(SeqMove, priority, 0)                                                    },
    { "Scale",            TOK_F32(SeqMove, scale, DEFAULT_ANIMATION_SCALE)                    },
    { "NextMove",        TOK_STRUCT(SeqMove, nextMove,  ParseNextMoveP)    },
    { "CycleMove",        TOK_STRUCT(SeqMove, cycleMove, ParseCycleMoveP)    },
    { "Type",            TOK_STRUCT(SeqMove, typeGfx,      ParseTypeGfxP) },
    { "Wind",            TOK_POOL_STRING | TOK_STRING(SeqMove, windInfoName, 0)                                                },
    { "Member",            TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, memberStr)                                                }, //Array of strings
    { "Interrupts",        TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, interruptsStr)                                            }, //Array of strings
    { "SticksOnChild",    TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, sticksOnChildStr)                                            }, //Array of strings
    { "SetsOnChild",    TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, setsOnChildStr)                                            }, //Array of strings
    { "SetsOnReactor",    TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, setsOnReactorStr)                                            }, //Array of strings
    { "Sets",            TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, setsStr)                                                    }, //Array of strings
    { "Requires",        TOK_POOL_STRING | TOK_STRINGARRAY(SeqMove, requiresStr)                                                }, //Array of strings
    { "Flags",            TOK_FLAGS(SeqMove, flags, 0),    MoveFlags                                    },

    // the following fields are filled in at bin time (during preload)
    { "",    TOK_INT16(SeqMove, raw.idx, 0)            },
    { "",    TOK_U8(SeqMove, raw.nextMoveCnt, 0)        },
    { "",    TOK_U8(SeqMove, raw.cycleMoveCnt, 0)    },
    { "",    TOK_FIXED_ARRAY | TOK_INT16_X, offsetof(SeqMove, raw.nextMove), MAX_NEXT_MOVES        },
    { "",    TOK_FIXED_ARRAY | TOK_INT16_X, offsetof(SeqMove, raw.cycleMove), MAX_CYCLE_MOVES    },
    { "",    TOK_INT(SeqMove, raw.required.count, 0)    },    
    { "",    TOK_FIXED_ARRAY | TOK_INT16_X, offsetof(SeqMove, raw.required.bits), MAX_SPARSE_BITS    },
    { "",    TOK_FIXED_ARRAY | TOK_INT_X, offsetof(SeqMove, raw.memberBitArray), MAX_IRQ_ARRAY_SIZE    },
    { "",    TOK_FIXED_ARRAY | TOK_INT_X, offsetof(SeqMove, raw.interruptsBitArray), MAX_IRQ_ARRAY_SIZE    },

    { "MEnd",            TOK_END                                                                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseGroupP[] =
{
    { "",    TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(GroupP,name,0)        },
    { "\n",    TOK_END,    0,                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseTypeDefP[] =
{
    { "",                TOK_POOL_STRING | TOK_STRUCTPARAM | TOK_FILENAME(TypeP,name, 0)            },
    { "BaseSkeleton",    TOK_POOL_STRING | TOK_FILENAME(TypeP,baseSkeleton, 0)    },
    { "ParentType",        TOK_POOL_STRING | TOK_STRING(TypeP,parentType, 0)        },
    { "TypeDefEnd",        TOK_END,    0,                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseSeqInfo[] =
{
    { "Sequencer",    TOK_IGNORE,    0}, // hack, so we can load up an indivudual file
    { "",            TOK_CURRENTFILE(SeqInfo, name)    /*Full Path    (UC,FWD)*/                    },
    { "TypeDef",    TOK_STRUCT(SeqInfo, types,        ParseTypeDefP)    },
    { "Group",        TOK_STRUCT(SeqInfo, groups,        ParseGroupP)        },
    { "Move",        TOK_STRUCT(SeqInfo, moves,    ParseMoveP)        },
    { "SeqEnd",        TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseSeqInfoList[] =
{
    { "Sequencer",    TOK_STRUCT(SeqInfoList,seqInfos, ParseSeqInfo) },
    { "", 0, 0 },
};


bool seqGetMoveIdxFromName( const char * moveName, const SeqInfo * seqInfo, U16* iResult )
{
    U16 i;
    U16 numMoves;

    numMoves = (U16)eaSize( &seqInfo->moves );

    for( i = 0 ; i < numMoves ; i++ )
    {
        if ( 0 == stricmp( moveName, seqInfo->moves[i]->name ) )
        {
            *iResult = i;
            return true;
        }
    }
    return false;
}


static int cmpIntQSort(const int * int1, const int * int2)
{
    if(*int1 < *int2)
        return -1;
    if(*int1 == *int2)
        return 0;
    return 1;
}


static void UpCase(char *s)
{
    for(;*s;s++) *s = toupper(*s);
}

/*Return true if all bits set in state are also set in test_state*/
static int areOnlyTheseBitsSet(SparseBits * state, U32 * test_state)
{
    U32 i;
    int j;
    U32* denseBits = (U32*)_alloca(STATE_ARRAY_SIZE * sizeof(U32));
    memset(denseBits, 0, STATE_ARRAY_SIZE * sizeof(U32));


    for (i = 0; i < state->count; ++i)
        SETB(denseBits, state->bits[i]);

    for( j = 0 ; j < STATE_ARRAY_SIZE ; j++ )
    {
        if(denseBits[j] & ~test_state[j])
            return 0;
    }

    return 1;
}

/*Sets whether or not this move should be predicted on the client.  The set_on_client
list is all the bits that are set on the client, and if a move is set only by some
combination of those bits, it should be predictable on the client.*/

static void setPredictability( SeqMove * move )
{
    static int bits_ok_to_predict[STATE_ARRAY_SIZE];
    static int initted = 0;
    int i;

    if( !initted )
    {
        initted = 1;
        for( i = 0 ; i < g_MaxUsedStates ; i++ )
        {
            if( stateBits[i].name )
            {
                if( stateBits[i].flags & STATEBIT_PREDICTABLE )
                    SETB( bits_ok_to_predict, stateBits[i].bitNum );
            }
        }
    }

    if( areOnlyTheseBitsSet( &move->raw.required, bits_ok_to_predict) )
        move->flags |= SEQMOVE_PREDICTABLE;
    else
        move->flags &= ~SEQMOVE_PREDICTABLE;
}


static void setSparseBit(SparseBits* bitfield, int bit)
{
    U32 i;
    
    assert(bit >= 0 && bit < 0xffffUL);

    for (i = 0; i < bitfield->count; ++i)
    {
        if (bitfield->bits[i] == bit)
            return;
    }

    bitfield->bits[bitfield->count++] = bit;
}

void setBitsFromString( const char ** strings, SparseBits * bitfield, const char * nameForDebug, const char* debug_filename )
{
    int i,bit,count;

    count = eaSize( &strings );
    assert(count + bitfield->count <= MAX_SPARSE_BITS);

    for(i = 0 ; i < count ; i++ )
    {
        bit = seqGetStateNumberFromName( strings[i] );

        if( bit == -1 )
        {
            char fileName[1000];
            sprintf(fileName, "sequencers/%s", debug_filename);
            ErrorFilenamef( fileName, "The move %s/%s has the state %s, which doesn't exist", debug_filename, nameForDebug, strings[i] );
        }
        else
        {
            setSparseBit( bitfield, bit );
        }
    }
}

//Debug Function
void seqDebugHashCollision( const SeqInfo * seqInfo, int hashSeed, const char *debug_filename )
{
    char buf[MAX_PATH];
    int i, *hashes = NULL;

    sprintf(buf, "sequencers/%s", debug_filename);

    //Check for collisions
    for(i = 0; i < eaSize(&seqInfo->moves); i++)
    {
        int hash = hashString(seqInfo->moves[i]->name, true);
        if(eaiSortedFind(&hashes, hash) >= 0)
            ErrorFilenamef(buf, "Two moves with the same name: %s (may be an include file from %s)", seqInfo->moves[i]->name, debug_filename);
        else
            eaiSortedPush(&hashes, hash);
    }

    for(i = 0; i < eaSize(&seqInfo->groups); i++)
    {
        int hash = hashString(seqInfo->groups[i]->name, true);
        if(eaiSortedFind(&hashes, hash) >= 0)
            ErrorFilenamef(buf, "Two groups or a group and move with the same name: %s (may be an include file from %s)", seqInfo->groups[i]->name, debug_filename);
        else
            eaiSortedPush(&hashes, hash);
    }

    eaiDestroy(&hashes);
}
//End debug

static void setUpRawNextMoves(SeqMove * move, const char * debug_filename, SeqInfo * seqInfo) 
{
    int i;

    move->raw.nextMoveCnt = eaSize( &move->nextMove );

    if (move->raw.nextMoveCnt > MAX_NEXT_MOVES)
    {
        ErrorFilenamef(debug_filename, "Move %s in seq %s has %d next moves! Max is %d",
            move->name, seqInfo->name, eaSize(&move->nextMove), MAX_NEXT_MOVES);
        move->raw.nextMoveCnt = MAX_NEXT_MOVES;
    }

    for( i = 0 ; i < eaSize(&move->nextMove) ; i++ )
    {
        if ( !move->nextMove[i]->name )
        {
            ErrorFilenamef( debug_filename, "Missing Next Move %s in move %s in seq %s", move->nextMove[i]->name, move->name, seqInfo->name );
            if (i < MAX_NEXT_MOVES)
                move->raw.nextMove[i] = 0;
        }
        else
        {
            if(!seqGetMoveIdxFromName( move->nextMove[i]->name, seqInfo, &move->raw.nextMove[i]))
            {
                //DEBUG only 
                //    ErrorFilenamef( debug_filename, "Bad Next Move %s in move %s in %s seq %s", move->nextMove[i]->name, move->name, seqInfo->name );
                //END debug
                if (i < MAX_NEXT_MOVES)
                    move->raw.nextMove[i] = 0;
            }
            // Don't free the name since it is now in the StringCache (has TOK_POOL_STRING flag) and is probably pointed to by other strings
            //StructFreeString(move->nextMove[i]->name);
        }
        StructFree(cpp_const_cast(NextMoveP*)(move->nextMove[i]));
    }

    eaDestroyConst(&move->nextMove);
    if( !move->raw.nextMoveCnt ) //nextmove defaults to ready
    {
        move->raw.nextMoveCnt = 1;
        move->raw.nextMove[0] = 0; //0 is always the ready animation
    }
}

static void setUpRawCycleMoves(SeqMove * move, const char * debug_filename, SeqInfo * seqInfo) 
{
    int i;

    move->raw.cycleMoveCnt = eaSize( &move->cycleMove );
    if (move->raw.cycleMoveCnt > MAX_CYCLE_MOVES)
    {
        ErrorFilenamef(debug_filename, "Move %s in seq %s has %d cycle moves! Max is %d",
            move->name, seqInfo->name, eaSize(&move->nextMove), MAX_CYCLE_MOVES);
        move->raw.cycleMoveCnt = MAX_CYCLE_MOVES;
    }
    for( i = 0 ; i < eaSize(&move->cycleMove) ; i++ )
    {
        if(i < MAX_CYCLE_MOVES && !seqGetMoveIdxFromName(move->cycleMove[i]->name, seqInfo, &move->raw.cycleMove[i]))
        {
            ErrorFilenamef( debug_filename, "Bad cycle move %s in move %s", move->cycleMove[i]->name, move->name );
            move->raw.cycleMove[i] = 0;
        }

        // Don't free the name since it is now in the StringCache (has TOK_POOL_STRING flag) and is probably pointed to by other strings
        //StructFreeString(move->cycleMove[i]->name);
        StructFree(cpp_const_cast(CycleMoveP*)(move->cycleMove[i]));
    }
    eaDestroyConst(&move->cycleMove);
    if( move->raw.cycleMoveCnt )
        move->flags |= SEQMOVE_COMPLEXCYCLE;
}

//Exists because we can't allocate dynamic raw data in the bin.
//Otherwise part of this could be preprocessed
static void seqLoadTimeProcessor( SeqInfo * seqInfo, const char *debug_filename )
{    
    int i, numMoves;

    numMoves = eaSize( &seqInfo->moves );
    if(numMoves > U16_MAX) // we store move indexes as U16s
        ErrorFilenamef(debug_filename, "Too many moves included in seq %s, some moves will be inaccessible or wrong!", seqInfo->name);
    
    PERFINFO_AUTO_START("top", 1);

    //For Each Move in the sequencer
    for( i = 0 ; i < numMoves ; i++ )
    {
        SeqMove *move = cpp_const_cast(SeqMove*)(seqInfo->moves[i]);

        // verify that this data was correctly preloaded
        assert( move->raw.idx == i );
        if(move->flags & SEQMOVE_COMPLEXCYCLE) {
            assert(move->raw.cycleMoveCnt);
        }
    }

    PERFINFO_AUTO_STOP_START("middle", 1);

#if 0 // fpe disabled 10/27/10. This code was already essentially disabled by a bug in the code, and fixing that bug
        // yielded a bunch of error lines which turns out to be false since the Groups are not always defined
        // at the top of a sequence file (eg player.txt), instead they are declared "just in time" at usage
        // in the *.inc files.

    //DEBUG only: Extra bit of debugging for interrupts
    //Is every interrupt either a group member or a move?
#ifdef CLIENT
    if( isDevelopmentMode() && game_state.fxdebug )
    {
        SeqMove *move;
        int i, j, numMoves;
        int k, validName;
        int irqCnt, groupCnt, memberCnt;
        char * irqStr;
        char * memberStr;

        numMoves = eaSize( &seqInfo->moves );
        for( i = 0 ; i < numMoves ; i++ )
        {
            move = seqInfo->moves[i];

            //Check interrupt strings
            validName = 0;
            irqCnt = eaSize( &move->interruptsStr );

            for( j = 0 ; j < irqCnt ; j++ )
            {
                irqStr = move->interruptsStr[j];

                //Is this the name of another move?
                for( k = 0 ; k < numMoves && !validName ; k++ )
                {
                    if( 0 == stricmp( seqInfo->moves[k]->name, irqStr ) )
                        validName = 1;
                }

                //Is this the name of a group?
                groupCnt = eaSize( &seqInfo->groups );
                for( k = 0 ; k < groupCnt && !validName ; k++ )
                {
                    if( 0 == stricmp( seqInfo->groups[k]->name, irqStr ) )
                        validName = 1;
                }

                if( !validName )
                {
                    Errorf( "SEQINFO %s, move %s : Interrupts %s is not a group or a move",
                        seqInfo->name, move->name, irqStr );
                }
            }

            //Check member strings
            validName = 0;
            memberCnt = eaSize( &move->memberStr );

            for( j = 0 ; j < memberCnt ; j++ )
            {
                memberStr = move->memberStr[j];

                //Is this the name of a group?
                groupCnt = eaSize( &seqInfo->groups );
                for( k = 0 ; k < groupCnt && !validName ; k++ )
                {
                    if( 0 == stricmp( seqInfo->groups[k]->name, memberStr ) )
                        validName = 1;
                }

                if( !validName )
                {
                    Errorf( "SEQINFO %s, move %s : Members %s is not a group",
                        seqInfo->name, move->name, memberStr );
                }
            }

        }
    }
#endif
    //End Debug only
#endif

    PERFINFO_AUTO_STOP();
}

static void seqCacheMovesPerBit(SeqInfo* seqInfo, bool shared_memory)
{
    int i;    
    int numMoves = eaSize(&seqInfo->moves);
    U32 *moveIsOnAList = _alloca(SIZB(numMoves)*sizeof(U32));
    SeqBitInfo* newSeqBitInfo;

    if(shared_memory) {
        newSeqBitInfo = (SeqBitInfo*)sharedMalloc(sizeof(SeqBitInfo) * g_MaxUsedStates);
        memset(newSeqBitInfo, 0, sizeof(SeqBitInfo) * g_MaxUsedStates);
    } else {
        newSeqBitInfo = (SeqBitInfo*)calloc(g_MaxUsedStates, sizeof(SeqBitInfo));
    }

    // If we got this far, we need to add it to the hash table
    memset(moveIsOnAList, 0, SIZB(numMoves)*sizeof(U32));

    for( i = 0 ; i < numMoves ; i++ )
    {    
        U32 h;
        const SeqMove *move = seqInfo->moves[i];

        for( h = 0; h < move->raw.required.count; ++h )
        {
            int bit = move->raw.required.bits[h];
            if( stateBits[bit].name &&            //If this is a bit that's used 
                !TSTB(moveIsOnAList, i) &&        //And this move hasn't been put on the check list of another one of its required bits already
                eaSize(&move->interruptsStr) )    //And this move actually interrupts something
            {
                eaiPush( &newSeqBitInfo[bit].movesRemainingThatRequireMe, i ); //Put this move index on this bit's list
                SETB(moveIsOnAList, i);  //And don't put it on any other bit's list
            }
        }
    }
    //Any move that requires no bits can't interrupt anything anyway, so anything left over can be ignored

    //Assign bit->cnt for easier referencing, and compress arrays if using shared memory
    {
        int j, total=0;
        for( i = 0 ; i < g_MaxUsedStates ; i++ ) 
        {
            SeqBitInfo * bit = &newSeqBitInfo[i];
            bit->cnt = eaiSize( &bit->movesRemainingThatRequireMe );
            if(shared_memory && bit->movesRemainingThatRequireMe) 
            {
                int* oldArray = bit->movesRemainingThatRequireMe;
                eaiCompress(&bit->movesRemainingThatRequireMe, &oldArray, customSharedMalloc, NULL);
                eaiDestroy(&oldArray);
            }

            //Debug info 
            if( 0 )
            {
                if( stateBits[i].name )
                {
                    consoleSetFGColor( COLOR_RED | COLOR_BRIGHT);   
                    printf( "\n%d Bit: %s (%d):  ", i, stateBits[i].name, bit->cnt ); 
                    consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
                    for( j = 0 ; j < bit->cnt ; j++ )
                    {
                        printf( " %s ", seqInfo->moves[eaiGet(&bit->movesRemainingThatRequireMe,j)]->name ); 
                    }
                    total += bit->cnt;
                }
                else 
                {
                    consoleSetFGColor( COLOR_BRIGHT | COLOR_BLUE | COLOR_GREEN);  
                    printf( "\n%d UNUSED BIT NUMBER", i ); 
                    consoleSetFGColor(COLOR_GREEN | COLOR_RED | COLOR_BLUE);
                    assert( !bit->cnt );
                }
            }
            //End Debug
        }
        if( 0 )printf( "\nTotal Moves assigned %d", total );
    }

    // assign bits to the seqInfo
    if(shared_memory)
    {
        assert(!seqInfo->bits);
    }
    else if(seqInfo->bits)
    {
        // free any previous entry (only happens if reloading in dev mode)
        free(cpp_const_cast(SeqBitInfo*)(seqInfo->bits));
    }
    seqInfo->bits = newSeqBitInfo;
}

int seqAttachAnAnim( TypeGfx * typeGfx, int animLoadType )
{
    int success = 1;

    typeGfx->animTrack = animGetAnimTrack( typeGfx->animP[0]->name, animLoadType, typeGfx->filename );

    //Debug only
    if( !typeGfx->animTrack )
    {
        ErrorFilenamef(typeGfx->filename, "MISSING ANIMATION %s ", typeGfx->animP[0]->name );
        typeGfx->animTrack = animGetAnimTrack( "MALE/THUMBSUP", animLoadType, NULL );
        success = 0;
    } //End debug

    //Calculate the last frame if needed
    if (!typeGfx->animP[0]->lastFrame)
        (cpp_const_cast(AnimP*)(typeGfx->animP[0]))->lastFrame = typeGfx->animTrack->length;

    return success;
}


//For each type get a ptr to the animation (and set lastFrame if needed)
//******Must be done at load time********
static void seqAttachAnimations( SeqInfo * seqInfo, int loadType )
{
    int    animLoadType;
    int i,j, numMoves;

    if( loadType == SEQ_LOAD_FULL ) //TO DO sometime make it so a second call to load with a new load_type works
        animLoadType = LOAD_ALL;
    else if ( loadType == SEQ_LOAD_FULL_SHARED )
        animLoadType = LOAD_ALL_SHARED;
    else if (loadType == SEQ_LOAD_HEADER_ONLY )
        animLoadType = LOAD_SKELETONS_ONLY;

    ///Set up pointers in each move
    numMoves = eaSize( &seqInfo->moves );

    for( i = 0 ; i < numMoves ; i++ )
    {
        int typeGfxCount;

        SeqMove *move = cpp_const_cast(SeqMove*)(seqInfo->moves[i]);

        //For each type get a ptr to the animTracks, baseAnimTrack and calculate the last frame if needed
        typeGfxCount = eaSize( &move->typeGfx );
        for( j = 0 ; j < typeGfxCount ; j++ )
        {
            int success;
            TypeGfx * typeGfx = cpp_const_cast(TypeGfx*)(move->typeGfx[j]);
            
            if(eaSize(&typeGfx->animP) != 1)
            {
                ErrorFilenamef(typeGfx->filename, "Bad animP count: %d", eaSize(&typeGfx->animP));
            }
            else
            {
                success = seqAttachAnAnim( typeGfx, animLoadType );
                //Debug
                if( !success )
                    ErrorFilenamef(typeGfx->filename, "MISSING ANIMATION %s in move %s in sequencer %s", typeGfx->animP[0]->name, move->name, seqInfo->name );
                //End Debug
            }
        }
    }
}

static void seqInitializePreLoad(SeqInfo * seqInfo)
{
    int i, j, k, numMoves;

    if (seqInfo->initialized)
        return;

    seqCleanSeqFileName(cpp_const_cast(char*)(seqInfo->name), strlen(seqInfo->name)+1, seqInfo->name);

    numMoves = eaSize( &seqInfo->moves );
    if(numMoves > U16_MAX) {
        Errorf("Too many moves included in seq %s, some moves will be inaccessible or wrong!", seqInfo->name); 
    }

    for( j = 0; j < numMoves; j++ )
    {
        SeqMove *move = cpp_const_cast(SeqMove*)(seqInfo->moves[j]);
        move->raw.idx = j;

        //Clean up each type within each move
        {
            char fxName[512];
            int typeCount;

            typeCount = eaSize( &move->typeGfx );
            for( k = 0 ; k < typeCount ; k++ )
            {            
                TypeGfx * typeGfx = cpp_const_cast(TypeGfx*)(move->typeGfx[k]);
                int fxCount, m;

                //Clean up pitch info
                if( !typeGfx->pitchRate )
                    typeGfx->pitchRate = DEFAULT_PITCHRATE;

                //Clean up Fx names
                fxCount = eaSize( &typeGfx->seqFx );
                for ( m = 0 ; m < fxCount ; m++ )
                {
                    SeqFx * seqfx = cpp_const_cast(SeqFx*)(typeGfx->seqFx[m]);
                    fxCleanFileName( fxName, seqfx->name );
                    seqfx->name = allocAddString(fxName);

                    //Debug check that CONSTANT flag is set or fx played has a lifespan
                    #ifdef CLIENT
                    if( game_state.fxdebug )
                    {
                        char * colon;
                        FxDebugAttributes attribs;

                        //After the fxName can be a colon, followed by it's Var name by which it is replaced in the enttype or behavior system
                        //Fx    generic/shotgun.fx:SHOTGUN
                        strcpy( fxName, seqfx->name );
                        colon = strchr( fxName, ':' );
                        if( colon )
                            *colon = 0;

                        if( fxName[0] ) //if it started with a colon, it has *only* replaced fx, no default
                        {
                            if( FxDebugGetAttributes( 0, fxName, &attribs ) )
                            {
                                //Constant seqfx turn themselves on and off and don't have to have a lifespan
                                if( !(seqfx->flags & SEQFX_CONSTANT) && attribs.lifeSpan <= 0 )
                                    printf( "FX %s used by move %s with no CONSTANT flag, but has no lifespan\n", fxName, move->name );
                            }
                            else
                            {
                                printf("Nonexistent Fx %s in move %s \n", fxName, move->name );
                            }
                        }
                    }
                    #endif
                    //End debug check
                }
            }
        }

        //UPPER CASIFY all the interrupts and groups
        for( k = 0 ; k < eaSize( &move->interruptsStr ) ; k++ )
            UpCase( cpp_const_cast(char*)(move->interruptsStr[k]) );

        for( k = 0 ; k < eaSize( &move->memberStr ) ; k++ )
            UpCase( cpp_const_cast(char*)(move->memberStr[k]) );

        setUpRawNextMoves(move, seqInfo->name, seqInfo);
        setUpRawCycleMoves(move, seqInfo->name, seqInfo);

        /////////// set raw bitfields
        setBitsFromString( move->requiresStr, &move->raw.required, move->name, seqInfo->name );
        // Don't free the requireStr since it is now in the StringCache (has TOK_POOL_STRING flag) and is probably pointed to by other strings
        //for (k = 0; k < eaSize(&move->requiresStr); ++k)
        //    StructFreeString(move->requiresStr[k]);
        eaDestroyConst(&move->requiresStr);

        ///////////PREDICTABILITY (must be after setBits Requires (duh)
        setPredictability( move );

        ////////// Add Move Sets to the Move Flags, since they are the same thing 
        //(Move Sets is deprecated, and this code is just for backward compatibility, setsStr should eventaully be removed
        {
            int count = eaSize( &move->setsStr );
            for(k = 0 ; k < count ; k++ )
            {
                const char* setsStr = move->setsStr[k];
                if( 0 == stricmp( setsStr, "Walkthrough" ) )
                    move->flags |= SEQMOVE_WALKTHROUGH;
                else if( 0 == stricmp( setsStr, "Hide" ) )
                    move->flags |= SEQMOVE_HIDE;
                else if( 0 == stricmp( setsStr, "CantMove" ) )
                    move->flags |= SEQMOVE_CANTMOVE;
                else if( 0 == stricmp( setsStr, "NoCollision" ) )
                    move->flags |= SEQMOVE_NOCOLLISION;
                else if( 0 == stricmp( setsStr, "RunIn" ) )
                    move->flags |= SEQMOVE_RUNIN;
                else if( 0 == stricmp( setsStr, "DieNow" ) || 0 == stricmp( setsStr, "Fly" ) || 0 == stricmp( setsStr, "Target" ) )
                    ; //Old flags i've told steve to remove
                else if( 0 == stricmp( setsStr, "CostumeChange" ) )
                    //Costume Change flag added for magic pack
                    move->flags |= SEQMOVE_COSTUMECHANGE;
                else
                    ErrorFilenamef( seqInfo->name, "Bad Sets %s in move %s", setsStr, move->name );

                // Don't free the setsStr since it is now in the StringCache (has TOK_POOL_STRING flag) and is probably pointed to by other strings
                //StructFreeString(setsStr);
            }
            eaDestroyConst(&move->setsStr);
        }
    } // end for move

    //Fill out interruptsBitArray and memberBitArray
    {
        int totalIrqStrings, irqCnt, irqVal;
        StashTable    irqName_ht;

        //Check for collisions (may be an issue: no guarantee group names are exactly the same as the ones used in the moves
        if( isDevelopmentMode() )
            seqDebugHashCollision( seqInfo, SEQ_QUICKLOADMOVEHASH, seqInfo->name ); //Debug only

        //Set up hash table for all strings used as interrupts
        irqName_ht = stashTableCreateWithStringKeys(MAX_IRQ_ARRAY_SIZE*32, StashDeepCopyKeys | StashCaseSensitive );

        //1. Build a hash table of all Move and Group Names used in any move->interruptsStr assigning each string a bit array position, 
        //   And also Fill out each move's interruptsBitArray of interruptsStrs
        totalIrqStrings = 1; //Skip zero on purpose so stashFindPointerReturnPointer return 0 can == 'failed to find'
        
        for( i = 0 ; i < numMoves ; i++ )
        {
            SeqMove *move = cpp_const_cast(SeqMove*)(seqInfo->moves[i]);
            irqCnt = eaSize( &move->interruptsStr );
            for( j = 0 ; j < irqCnt ; j++ )
            {
                irqVal = (int)(intptr_t)stashFindPointerReturnPointer( irqName_ht, move->interruptsStr[j] );
                if( !irqVal )
                {
                    irqVal = totalIrqStrings++;
                    //if( strstri(debug_filename, "player") ) { // test code for listing all interrupt strings used by player
                    //    printf("%s: move \"%s\", interrupt \"%s\" (%d)\n", debug_filename, move->name, move->interruptsStr[j], totalIrqStrings);
                    //}
                    stashAddInt(irqName_ht, move->interruptsStr[j], irqVal, false);
                }
                SETB( move->raw.interruptsBitArray, irqVal );
            }
        }

        if( totalIrqStrings >= MAX_IRQ_ARRAY_SIZE*32 )
            FatalErrorf( "Please ask a programmer to increase MAX_IRQ_ARRAY_SIZE" ); //Not dynamic because it's checked so often I want the memory right here
        
        //2. Fill out each move's memberBit array with any of that move's name or memberStrs that some other move actually uses as an interruptStr
        for( i = 0 ; i < numMoves ; i++ )
        {
            int memberCnt, memberVal; 
            SeqMove *move = cpp_const_cast(SeqMove*)(seqInfo->moves[i]);

            memberVal = (int)(intptr_t)stashFindPointerReturnPointer( irqName_ht, move->name );
            if( memberVal ) //If no other move says it interrupts this move specifically, ignore
                SETB( move->raw.memberBitArray, memberVal );

            memberCnt = eaSize( &move->memberStr );
            for( j = 0 ; j < memberCnt ; j++ )
            {
                memberVal = (int)(intptr_t)stashFindPointerReturnPointer( irqName_ht, move->memberStr[j] );
                if( memberVal ) //If no other move refers to this group, ignore it (maybe tell Steve, it really shouldn't be here)
                    SETB( move->raw.memberBitArray, memberVal );
            }
        }

        stashTableDestroy( irqName_ht );
    }

}

static void seqInitializePostLoad(SeqInfo* seqInfo, int load_type)
{
    if (!seqInfo->initialized)
    {
        PERFINFO_AUTO_START("seqLoadTimeProcessor", 1);        
            seqLoadTimeProcessor( seqInfo, seqInfo->name ); //TO DO move this to preprecessor function
        PERFINFO_AUTO_STOP();
        if( !STATE_STRUCT.quickLoadAnims )
        {
            PERFINFO_AUTO_START("seqAttachAnimations", 1);
                seqAttachAnimations( seqInfo, load_type );
            PERFINFO_AUTO_STOP();
        }
        
        // delayed until Final
        //seqCacheMovesPerBit( seqInfo );
        //seqInfo->initialized = 1;
    }
}

static void seqInitializeFinalLoad(SeqInfo* seqInfo, bool shared_memory)
{
    if(!seqInfo->initialized)
    {
        seqCacheMovesPerBit(seqInfo, shared_memory);
        seqInfo->initialized = 1;
    }
}

const TypeP * seqGetTypeP( const SeqInfo * info, const char * typeName )
{
    int i;
    const TypeP * typeP;
    int typePCount;

    PERFINFO_AUTO_START("seqGetTypeP", 1);
        typePCount = eaSize(&info->types);
        for( i = 0 ; i < typePCount ; i++ )
        {
            typeP = info->types[i];
            if( 0 == stricmp( typeP->name, typeName ) )
            {
                PERFINFO_AUTO_STOP();
                return typeP;
            }
        }
    PERFINFO_AUTO_STOP();
    return 0;
}


/*
Given a move and a typeName, choose the right animation to play
returns animTrack         : the animation to play.
        defaultAnimTrack : the default anim of the animTracks animSet.  Two purposes:
            -if animTrack is missing any bones, get them from defaultAnimTrack
            -compare defaultAnimTrack bones to the Type's baseSkeleton bones to fit the animation onto the Type's skeleton
*/
const TypeGfx * seqGetTypeGfx( const SeqInfo * info, const SeqMove * move, const char * typeName )
{
    const TypeGfx * myTypeGfx = 0;
    int i;

    assert( typeName );

    if( 0 == stricmp( typeName, "None" ) || 0 == stricmp( typeName, "" ) )
        return 0;

    //Find the right typeGfx
    for( i = eaSize(&move->typeGfx)-1; i >= 0 ; i-- )
    {
        const TypeGfx * typeGfx = move->typeGfx[i];
        if( 0 == stricmp( typeGfx->type, typeName ) )
        {
            myTypeGfx = typeGfx;
            break;
        }
    }

    //If you didn't find one, look for the parentType's typeGfx
    if( !myTypeGfx )
    {
        const TypeP * typeP = seqGetTypeP( info, typeName );

        if( typeP )
            myTypeGfx = seqGetTypeGfx( info, move, typeP->parentType );
    }

    return myTypeGfx;
}

//Load a single sequencer at run time  development only
static SeqInfo * seqLoadSeqInfoDevelopment( char fname[] )
{
    SeqInfo * seqInfo = 0;
    int         fileisgood = 0;
    TokenizerHandle tok;

    tok = TokenizerCreate(fname);
    if (tok)
    {
        seqInfo = listAddNewMember(&seqGlobals.dev_seqInfosList, sizeof(SeqInfo));
        assert(seqInfo);
        listScrubMember(seqInfo, sizeof(*seqInfo));
        fileisgood = TokenizerParseList(tok, ParseSeqInfo, seqInfo, TokenizerErrorCallback);
        if( !fileisgood )
        {
            listFreeMember( seqInfo, &seqGlobals.dev_seqInfosList );
            seqInfo = 0;
        }
        else
        {
            assert(sharedMemoryGetMode() == SMM_DISABLED);
            seqInitializePreLoad(seqInfo);
#ifdef SERVER 
            seqInitializePostLoad(seqInfo, SEQ_LOAD_FULL_SHARED);
#else
            seqInitializePostLoad(seqInfo, SEQ_LOAD_FULL);
#endif
            seqInitializeFinalLoad(seqInfo, false);
            seqGlobals.dev_seqInfoCount++;
        }
    }
    TokenizerDestroy(tok);

    return seqInfo;
}

static SeqInfo* seqGetDevSequencer(SeqInfo* seqInfo, const char* seqInfoNameCleanedUp, int reloadForDev)
{
    SeqInfo * dev_seqInfo;  //LIFO list means you always get the latest
    char buf[500];

    // always prefer a data directory version to the bin file version
    for( dev_seqInfo = seqGlobals.dev_seqInfosList ; dev_seqInfo ; dev_seqInfo = cpp_const_cast(SeqInfo*)(dev_seqInfo->next) )
    {
        if( !stricmp(seqInfoNameCleanedUp, dev_seqInfo->name ) )
        {
            seqInfo = dev_seqInfo;
            break;
        }
    }

    //sprintf(buf, "%s%s", "sequencers/", seqInfoNameCleanedUp); // JE: sprintf is slow, I hates it
    STR_COMBINE_BEGIN(buf)
        STR_COMBINE_CAT("sequencers/");
        STR_COMBINE_CAT(seqInfoNameCleanedUp);
    STR_COMBINE_END();

    //Reload the sequencer from the file under these conditions
    if( !seqInfo ||                                //it's brand new entType
        reloadForDev == SEQ_RELOAD_FOR_DEV  ||    //If there's a chance the animations have changed
        (seqInfo && fileHasBeenUpdated( buf, &seqInfo->fileAge ))) //the seqInfo file itself has changed
    {
        seqInfo = seqLoadSeqInfoDevelopment( buf );
    }
    
    return seqInfo;
}

static void seqRemoveFromDevSequencers(const char* seqInfoNameCleanedUp)
{
    SeqInfo * checkerSeqInfo;
    SeqInfo * checkerSeqInfo_next;
    for( checkerSeqInfo = seqGlobals.dev_seqInfosList ; checkerSeqInfo ; checkerSeqInfo = checkerSeqInfo_next )
    {
        checkerSeqInfo_next = cpp_const_cast(SeqInfo*)(checkerSeqInfo->next);
        if( !checkerSeqInfo->name || !stricmp( seqInfoNameCleanedUp, checkerSeqInfo->name ) )
        {
            listFreeMember( checkerSeqInfo, &seqGlobals.dev_seqInfosList );
            seqGlobals.dev_seqInfoCount--;
            assert( seqGlobals.dev_seqInfoCount >= 0 );
        }
    }
}

static const SeqInfo* findSequencerByName(const char* findThisSeqName)
{
    // get from hashtable
    const SeqInfo* seqInfo = NULL;
    if(seqInfoList.seqInfoHashes)
        seqInfo = stashFindPointerReturnPointerConst(seqInfoList.seqInfoHashes, findThisSeqName);
    return seqInfo;
}

static const SeqInfo* findPlayerSequencer(const char* seqInfoName)
{
    const SeqInfo* seqInfo;
    
    // Note: we only come here when we failed to find a sequencer, so log the missing sequencer
    log_printf("SeqNotFound", "\n(SeqNotFound)%s", seqInfoName);
    
    seqInfo = findSequencerByName("player.txt");
    if( seqInfo )
        return seqInfo;
    
#if 0 // why bother, dont trust your hash?
    // Do linear search for "player", just to be sure.
    for( i = eaSize(&seqInfoList.seqInfos) - 1; i >= 0; i-- )
    {
        if( !stricmp( seqInfoList.seqInfos[i]->name, "player.txt" ) )
        {
            return seqInfoList.seqInfos[i];
        }
    }
#endif

    //Failure in production mode == very bad data.
    FatalErrorf( "Sequencer data (seqInfoList) is corrupt? Couldn't load '%s' or 'player' sequencers", seqInfoName );
    
    return NULL;
}

// Get sequencer, get particles, get fxinfo, and get behaviors are all exactly the same structure,
//and entType (seqType) are close and need to be made the same
//The only different is since we bind the sequencer tightly to the animation files in the initialization step,
//if this is a reloadseqs, we have to always reload the sequencers since the animations might have changed without
//our knowledge, so this has the reloadForDev param while the others don't.
const SeqInfo * seqGetSequencer( const char seqInfoName[], int loadType, int reloadForDev )
{
    SeqInfo * seqInfo;
    char seqInfoNameCleanedUp[SEQ_MAX_PATH];

    writeConsole(OUTPUT_DEBUG, "\tReading %s", seqInfoName);
    PERFINFO_AUTO_START("top", 1);
        seqCleanSeqFileName(SAFESTR(seqInfoNameCleanedUp), seqInfoName);
        seqInfo = cpp_const_cast(SeqInfo*)(findSequencerByName(seqInfoNameCleanedUp));
        if( seqInfo )
        {
            //Development only
            if (isDevelopmentMode() && !isSharedMemory(seqInfo))
                seqInfo->fileAge = fileLastChanged( "bin/sequencers.bin" );
            //End development only
        }

        //So the game doesn't crash.
        if( !seqInfo && isProductionMode() )
        {
            return findPlayerSequencer(seqInfoName);
        }

    PERFINFO_AUTO_STOP_START("middle", 1);

        if (!global_state.no_file_change_check && isDevelopmentMode() && !isSharedMemory(seqInfo))
        {
            // Get the local dev sequencer from the unbinned data, if necessary.
            seqInfo = seqGetDevSequencer(seqInfo, seqInfoNameCleanedUp, reloadForDev);
            seqInitializePreLoad(seqInfo);
            seqInitializePostLoad(seqInfo, loadType);
            seqInitializeFinalLoad(seqInfo, false);
        }

    PERFINFO_AUTO_STOP_START("middle2", 1);

        if( seqInfo )
        {
            assert(seqInfo->initialized);

            //Development Only -- initialize the fileAge to current time
            if (!global_state.no_file_change_check && isDevelopmentMode() && !isSharedMemory(seqInfo))
            { //development mode initialization
                char buf[500];
                (cpp_const_cast(SeqInfo*)(seqInfo))->fileAge = 0;
                //sprintf(buf, "%s%s", "sequencers/", seqInfoNameCleanedUp); // JE: sprintf is slow, I hates it
                STR_COMBINE_BEGIN(buf)
                    STR_COMBINE_CAT("sequencers/");
                    STR_COMBINE_CAT(seqInfoNameCleanedUp);
                STR_COMBINE_END();
                fileHasBeenUpdated( buf, &seqInfo->fileAge );
            }
            //end development only
        }
        else
        {
            Errorf( "INFO: Failed to get seqInfo %s", seqInfoNameCleanedUp );
        }

    PERFINFO_AUTO_STOP_START("bottom", 1);

        // ## Developnment only: if seqInfo fails for any reason, try to remove it from the run time load list
        if( !seqInfo )
        {
            seqRemoveFromDevSequencers(seqInfoNameCleanedUp);
        }
        //## end development only
        
    PERFINFO_AUTO_STOP();
    
    return seqInfo;
}

#if CLIENT
void seqSetWindInfoPointersOnInfo(SeqInfo *seqInfo)
{
    int j, num_moves;
    num_moves = eaSize(&seqInfo->moves);
    for (j=0; j<num_moves; j++) {
        (cpp_const_cast(SeqMove*)(seqInfo->moves[j]))->windInfo = getWindInfoConst(seqInfo->moves[j]->windInfoName);
    }
}

void seqSetWindInfoPointers()
{
    int num_structs, i;

    // Set windInfo pointers
    num_structs = eaSize(&seqInfoList.seqInfos);
    for (i = 0; i < num_structs; i++)
    {
        seqSetWindInfoPointersOnInfo( cpp_const_cast(SeqInfo*)(seqInfoList.seqInfos[i]) );
    }

    // Fix those referenced on live entities
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE && entities[i]->seq ) //to do: better validating?
        {
            seqSetWindInfoPointersOnInfo( cpp_const_cast(SeqInfo*)(entities[i]->seq->info) );
        }
    }
}
#endif

static bool seqPreloadSeqInfoPreProcessor(TokenizerParseInfo *pti, void* structptr)
{
    int i, num_structs;
    SeqInfoList* slist = (SeqInfoList*)structptr;
    num_structs = eaSize(&(slist->seqInfos));
    for (i = 0; i < num_structs; i++)
    {
        SeqInfo * seqInfo = cpp_const_cast(SeqInfo*)(slist->seqInfos[i]);
        seqInitializePreLoad(seqInfo);
    }

    return true;
}

static bool seqPreloadSeqInfoPostProcessor(TokenizerParseInfo pti[], SeqInfoList* slist)
{
    int num_structs, i;

    //Clean Up Bhvr names, and sort them
    num_structs = eaSize(&(slist->seqInfos));
    for (i = 0; i < num_structs; i++)
    {
        SeqInfo * seqInfo = cpp_const_cast(SeqInfo*)(slist->seqInfos[i]);
#if CLIENT
        seqInitializePostLoad(seqInfo, SEQ_LOAD_FULL);
#else
        seqInitializePostLoad(seqInfo, SEQ_LOAD_FULL_SHARED);
#endif
    }

    return true;
}

static bool seqPreloadSeqInfoFinalProcessor(TokenizerParseInfo tpi[], SeqInfoList* slist, bool shared_memory)
{
    int num_structs, i;

    num_structs = eaSize(&(slist->seqInfos));

    assert(!slist->seqInfoHashes);
    slist->seqInfoHashes = stashTableCreateWithStringKeys(stashOptimalSize(num_structs), stashShared(shared_memory));
    
    for (i = 0; i < num_structs; i++)
    {
        const SeqInfo * seqInfo = slist->seqInfos[i];    
        const SeqInfo * duplicateEntry;

        if (duplicateEntry = stashFindPointerReturnPointerConst(slist->seqInfoHashes, seqInfo->name))
        {
            Errorf("Sequencer name conflict for %s", seqInfo->name);
        }
        else
            stashAddPointerConst(slist->seqInfoHashes, seqInfo->name, seqInfo, false);

        seqInitializeFinalLoad(cpp_const_cast(SeqInfo*)(seqInfo), shared_memory);
    }

    return true;
}

static bool seqPreloadSeqInfoFinalProcessorCallback(ParseTable pti[], void* structptr, bool shared_memory)
{
    return seqPreloadSeqInfoFinalProcessor(pti, (SeqInfoList*)structptr, shared_memory);
}

static bool seqPreloadSeqInfoPostProcessorCallback(ParseTable pti[], void* structptr)
{
    return seqPreloadSeqInfoPostProcessor(pti, (SeqInfoList*)structptr);
}

void seqPreloadSeqInfos()
{
#if SERVER
    ParserLoadFilesShared("SM_SEQINFO", "sequencers", ".txt", "sequencers.bin", 0, ParseSeqInfoList, &seqInfoList, sizeof(seqInfoList), NULL, NULL,
                seqPreloadSeqInfoPreProcessor, seqPreloadSeqInfoPostProcessorCallback, seqPreloadSeqInfoFinalProcessorCallback);
#else
    ParserLoadFiles("sequencers", ".txt", "sequencers.bin", 0, ParseSeqInfoList, &seqInfoList, NULL, NULL, seqPreloadSeqInfoPreProcessor);
    seqPreloadSeqInfoPostProcessor(ParseSeqInfoList, &seqInfoList);
    seqPreloadSeqInfoFinalProcessor(ParseSeqInfoList, &seqInfoList, false);
    seqSetWindInfoPointers();
#endif
}
