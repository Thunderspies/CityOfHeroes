//// Code Set Bits
#include "seq/seqstate.h"
#include "string.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include "time.h"
#include <utilitieslib/components/earray.h>
#include "TriggeredMove.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>

static StaticDefineInt statebit_flags[] = 
{
    DEFINE_INT
    { "Predictable",    STATEBIT_PREDICTABLE    },
    { "Flash",            STATEBIT_FLASH            },
    { "Continuing",        STATEBIT_CONTINUING        },
//    { "CodeSet",        STATEBIT_CODESET        },  //You can't set this flag from the data file
    DEFINE_END
};


StateBit stateBits[ MAXSTATES ] =
{
    {    "EMPTY",        0,                            STATEBIT_CODESET | STATEBIT_PREDICTABLE }, //So Array idx and pound define can be the same
    {    "FORWARD",        STATE_FORWARD,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "BACKWARD",        STATE_BACKWARD,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "STEPLEFT",        STATE_STEPLEFT,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "STEPRIGHT",    STATE_STEPRIGHT,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "JUMP",            STATE_JUMP,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "AIR",            STATE_AIR,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "APEX",            STATE_APEX,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "FLY",            STATE_FLY,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "BIGFALL",        STATE_BIGFALL,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "SWIM",            STATE_SWIM,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "PROFILE",        STATE_PROFILE,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "IDLE",            STATE_IDLE,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "TURNRIGHT",    STATE_TURNRIGHT,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "TURNLEFT",        STATE_TURNLEFT,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "JETPACK",        STATE_JETPACK,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },  
    {    "LANDING",        STATE_LANDING,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "SLIDING",        STATE_SLIDING,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "BOUNCING",        STATE_BOUNCING,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "PLAYER",        STATE_PLAYER,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "COMBAT",        STATE_COMBAT,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "STUN",            STATE_STUN,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },    
    {    "ICESLIDE",        STATE_ICESLIDE,                STATEBIT_CODESET | STATEBIT_PREDICTABLE },

    //////////////////////Surface Bits  , MOSTLY FOR FX ALL MUST BE UNDER 255, SINCE BRUCE STORES THEM AS U8S
    ///// Steve says never used in animations
    {    "GRAVEL",        STATE_GRAVEL,                STATEBIT_CODESET  },
    {    "ROCK",            STATE_ROCK,                    STATEBIT_CODESET  },
    {    "TILE",            STATE_TILE,                    STATEBIT_CODESET  },
    {    "LAVA",            STATE_LAVA,                    STATEBIT_CODESET  },
    {    "GRASS",        STATE_GRASS,                STATEBIT_CODESET  },
    {    "CONCRETE",        STATE_CONCRETE,                STATEBIT_CODESET  },
    {    "MUD",            STATE_MUD,                    STATEBIT_CODESET  },
    {    "DIRT",            STATE_DIRT,                    STATEBIT_CODESET  },
    {    "PUDDLE",        STATE_PUDDLE,                STATEBIT_CODESET  },
    {    "METAL",        STATE_METAL,                STATEBIT_CODESET  },
    {    "WOOD",            STATE_WOOD,                    STATEBIT_CODESET  },
    {    "ICE",            STATE_ICE,                    STATEBIT_CODESET  },
    {    "CARPET",        STATE_CARPET,                STATEBIT_CODESET  }, 
    // additional surface states further down, starting at STATE_SNOW

    //////////////////////// Volume Bits //////////////////////////////////////////////////////
    ///// Steve says never used in animations
    {    "INDEEP",        STATE_INDEEP,                STATEBIT_CODESET  },
    {    "INWATER",        STATE_INWATER,                STATEBIT_CODESET  },
    {    "INLAVA",        STATE_INLAVA,                STATEBIT_CODESET  },
    {    "INSEWERWATER",    STATE_INSEWERWATER,            STATEBIT_CODESET  }, 
    {    "INREDWATER",    STATE_INREDWATER,            STATEBIT_CODESET  }, 

    ////////////////////////SETS BITS: ////////////////////////////////////////////////////////
    {    "PREDICTABLE",    STATE_PREDICTABLE,            STATEBIT_CODESET  },  //SPECIALLY CALCULATED, dont set on animation
    {    "NOCOLLISION",    STATE_NOCOLLISION,            STATEBIT_CODESET  },
    {    "WALKTHROUGH",    STATE_WALKTHROUGH,            STATEBIT_CODESET  },
    {    "CANTMOVE",        STATE_CANTMOVE,                STATEBIT_CODESET  },
    {    "HIDE",            STATE_HIDE,                    STATEBIT_CODESET  },

    //////////// Set in assorted places by the code:
    {    "JUSTFUCKINDOIT",STATE_JUSTFUCKINDOIT,        STATEBIT_CODESET  },  //Not used anymore
    {    "HIGH",            STATE_HIGH,                    STATEBIT_CODESET  },
    {    "LOW",            STATE_LOW,                    STATEBIT_CODESET  },
    {    "HIT",            STATE_HIT,                    STATEBIT_CODESET  },
    {    "DEATH",        STATE_DEATH,                STATEBIT_CODESET  },
    {    "ARENAEMERGE",    STATE_ARENAEMERGE,            STATEBIT_CODESET  },
    {    "KNOCKBACK",    STATE_KNOCKBACK,            STATEBIT_CODESET  },
    {    "BLOCK",        STATE_BLOCK,                STATEBIT_CODESET  },
    {    "STARTOFFENSESTANCE",    STATE_STARTOFFENSESTANCE,    STATEBIT_CODESET  },
    {    "STARTDEFENSESTANCE",    STATE_STARTDEFENSESTANCE,    STATEBIT_CODESET  },
    {    "THANK",        STATE_THANK,                STATEBIT_CODESET  },
    {    "RECLAIM",        STATE_RECLAIM,                STATEBIT_CODESET  },
    {    "WALK",            STATE_WALK,                    STATEBIT_CODESET  },
    {    "RUNIN",        STATE_RUNIN,                STATEBIT_CODESET  },    
    {    "RUNOUT",        STATE_RUNOUT,                STATEBIT_CODESET  },
    {    "EMERGE",        STATE_EMERGE,                STATEBIT_CODESET  },
    {    "HEADPAIN",        STATE_HEADPAIN,                STATEBIT_CODESET  },
    {    "FEAR",            STATE_FEAR,                    STATEBIT_CODESET  },
    {    "SHUT",            STATE_SHUT,                    STATEBIT_CODESET  },
    {    "OPEN",            STATE_OPEN,                    STATEBIT_CODESET  },
    {    "SWEEP",        STATE_SWEEP,                STATEBIT_CODESET },
    {    "TEST0",        STATE_TEST0,                STATEBIT_CODESET  },
    {    "TEST1",        STATE_TEST1,                STATEBIT_CODESET  },
    {    "TEST2",        STATE_TEST2,                STATEBIT_CODESET  },
    {    "TEST3",        STATE_TEST3,                STATEBIT_CODESET  },
    {    "TEST4",        STATE_TEST4,                STATEBIT_CODESET  },
    {    "TEST5",        STATE_TEST5,                STATEBIT_CODESET  },
    {    "TEST6",        STATE_TEST6,                STATEBIT_CODESET  },
    {    "TEST7",        STATE_TEST7,                STATEBIT_CODESET  },
    {    "TEST8",        STATE_TEST8,                STATEBIT_CODESET  },
    {    "TEST9",        STATE_TEST9,                STATEBIT_CODESET  },
    {    "STUMBLE",        STATE_STUMBLE,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  },
    {    "SPAWNEDASPET",    STATE_SPAWNEDASPET,            STATEBIT_CODESET  },
    {    "DISMISS",        STATE_DISMISS,                STATEBIT_CODESET  },
    {    "OFF",            STATE_OFF,                    STATEBIT_CODESET  },
    {    "RAGDOLLBELLY",    STATE_RAGDOLLBELLY,            STATEBIT_CODESET  },
    {    "UNLOCKING",    STATE_UNLOCKING,            STATEBIT_CODESET  },
    {    "LOCKING",        STATE_LOCKING,                STATEBIT_CODESET  },
    {    "RIGHTHAND",    STATE_RIGHTHAND,            STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "LEFTHAND",        STATE_LEFTHAND,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "TWOHAND",        STATE_TWOHAND,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "DUALHAND",        STATE_DUALHAND,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "EPICRIGHTHAND",STATE_EPICRIGHTHAND,        STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "SHIELD",        STATE_SHIELD,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "HOVER",        STATE_HOVER,                STATEBIT_CODESET | STATEBIT_PREDICTABLE  }, // custom weapon
    {    "COSTUMECHANGE",STATE_COSTUMECHANGE,        STATEBIT_CODESET  }, // costume change emote
    {    "NINJARUN",        STATE_NINJA_RUN,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "PLAYERWALK",    STATE_PLAYERWALK,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "AMMO",            STATE_AMMO,                    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "INCENDIARYROUNDS",    STATE_INCENDIARYROUNDS,    STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "CRYOROUNDS",    STATE_CRYOROUNDS,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "CHEMICALROUNDS",    STATE_CHEMICALROUNDS,    STATEBIT_CODESET | STATEBIT_PREDICTABLE },

    // additional surface types
    {    "SNOW",            STATE_SNOW,                    STATEBIT_CODESET  }, 
    {    "ASPHALT",        STATE_ASPHALT,                STATEBIT_CODESET  }, 
    {    "SAND",            STATE_SAND,                    STATEBIT_CODESET  }, 
    {    "CATWALK",        STATE_CATWALK,                STATEBIT_CODESET  }, 
    {    "METALPLATE",    STATE_METALPLATE,            STATEBIT_CODESET  }, 
    {    "WOODHOLLOW",    STATE_WOODHOLLOW,            STATEBIT_CODESET  }, 
    {    "GROUND",        STATE_GROUND,                STATEBIT_CODESET  }, 

    {    "BEASTRUN",        STATE_BEAST_RUN,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "STEAMJUMP",    STATE_STEAM_JUMP,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "HOVERBOARD",    STATE_HOVER_BOARD,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "MAGICCARPET",    STATE_MAGIC_CARPET,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },
    {    "NONHUMAN",        STATE_NONHUMAN,                STATEBIT_CODESET },
    {    "PARKOURRUN",    STATE_PARKOUR_RUN,            STATEBIT_CODESET | STATEBIT_PREDICTABLE },

};
//IMPORTANT NOTE : If you change this list, see bottom of seqState.h for what to do.

typedef struct StateBitList{
    StateBit ** stateBits;
} StateBitList;

StateBitList fromDataStateBitList;

static int stateBitsMightBeRenumbered = 0;

#define SORTED_STATEBIT_LIST 1
#if SORTED_STATEBIT_LIST
static StateBit *s_SortedStateBitList_Name[MAXSTATES];
#endif

int g_MaxUsedStates = CODE_DEFINED_STATE_COUNT;  //MAX STATES actually loaded (set again when bits get loaded from data)

TokenizerParseInfo ParseStateBit[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_STRING(StateBit, name, 0)},
    {   "",                TOK_STRUCTPARAM | TOK_FLAGS(StateBit, flags, 0), statebit_flags},
    {   "\n",            TOK_END,                            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseStateBitList[] =
{
    {    "StateBit",        TOK_STRUCT(StateBitList, stateBits, ParseStateBit) },
    {    "", 0, 0 }
};

#if SORTED_STATEBIT_LIST
// Both strings should be upper case
int CompareFunc_StateBit_Name(const void *pA, const void *pB)
{
    const StateBit **pSB_A = pA;
    const StateBit **pSB_B = pB;

    // NOTE: we do a strcmp since both strings should be upper case
    return strcmp( (*pSB_A)->name, (*pSB_B)->name );
}
#endif // SORTED_STATEBIT_LIST

void seqLoadStateBits()
{
    int i;
    ParserLoadInfo pli;
    int flags = 0;

    //ParserLoadFilesShared("XXXXX.bin", NULL, "sequencers/statebits.statebits", "XXXXXXXX.bin", 
    //    PARSER_TRYPERSIST, ParseStateBitList, &fromDataStateBitList, sizeof(fromDataStateBitList), NULL, NULL, NULL, NULL, NULL);

    //setting this flags like this means the server will have a different, but identical seqstates.bin
    //The reason is that powers.bin is split up in the server and client data, and when running off the same data, each needs to be able to independently detect
    //whether stateBitsMightBeRenumbered should be true even or else only one of the powers.bin  

#ifdef SERVER
    flags |=PARSER_SERVERONLY;
#endif

    ParserLoadFiles("sequencers",
        ".statebits",
        "seqstatebits.bin",
        flags,
        ParseStateBitList,
        &fromDataStateBitList,
        NULL,
        &pli,
        NULL);

    stateBitsMightBeRenumbered = stateBitsMightBeRenumbered || pli.updated;

    //TO DO: this
    for (i = 0; i <= CODE_DEFINED_STATE_COUNT; i++) 
    {
        StateBit * statebit = &stateBits[ i ];
        if( statebit->bitNum && statebit->bitNum != i )
            FatalErrorf( "seqstate.h out of synch %i != % i", statebit->bitNum, i );
        
        //TO DO change this to this
        //if( statebit->codeGlobalPtr )
        //    statebit->codeGlobalPtr = i;
        //put codeGlobalPtr in place of bitNum, use &STATE_FORWARD, and change #define STATE_FORWARD to U32 STATE_FORWARD;
    }

    //Add this EArray into a non EArray of the same structures (name ptrs continue to point into the EArray)
    {
        int addedBitNum, statesToAddCnt, i;

        statesToAddCnt = eaSize( &fromDataStateBitList.stateBits );
        addedBitNum = g_MaxUsedStates;

        for (i = 0; i < statesToAddCnt; i++)
        {
            char *c;
            int j;
            bool isDuplicate = false;

            // Convert all of the loaded strings to upper case
            for (c = fromDataStateBitList.stateBits[i]->name; *c; c++)
            {
                *c = toupper(*c);
            }

            // See if this state name has already been defined before adding this one (keep the first instance)
            for (j = 0; j < addedBitNum; j++)
            {
                if (strcmp(fromDataStateBitList.stateBits[i]->name, stateBits[j].name) == 0)
                {
                    isDuplicate = true;
                    break;

                }
            }
            
            if (isDuplicate)
            {
                printf("Ignoring duplicate state bit %s\n", fromDataStateBitList.stateBits[i]->name);
            }
            else
            {
                StateBit * statebit = &stateBits[ addedBitNum ];
                *statebit = *fromDataStateBitList.stateBits[i];  //Struct copy
                statebit->bitNum = addedBitNum;
                addedBitNum++;
                if(addedBitNum >= MAXSTATES) {
                    FatalErrorf("MAXSTATES needs to be increased above %d (to at least %d) -- let a programmer know ASAP!",
                        addedBitNum, statesToAddCnt + CODE_DEFINED_STATE_COUNT + 1 );
                }
            }
        }
        
        stateBits[ addedBitNum ].name = 0; //for things that check that

        g_MaxUsedStates = addedBitNum;
    }

#if SORTED_STATEBIT_LIST
    // Make sorted lists by name and number
    for (i = 0; i < g_MaxUsedStates; i++)
    {
        char *c;

        devassert(stateBits[i].name);

        if (i < CODE_DEFINED_STATE_COUNT)
        {
            // Make sure all of the predefined strings are upper case
            for (c = stateBits[i].name; *c; c++)
            {
                // stateBits[i].name needs to be upper case if this asserts
                devassert(!isalpha(*c) || isupper(*c));
            }
        }
        else
        {
            // Convert all of the loaded strings to upper case
            for (c = stateBits[i].name; *c; c++)
            {
                *c = toupper(*c);
            }
        }

        s_SortedStateBitList_Name[i] = &stateBits[i];
    }

    qsort(s_SortedStateBitList_Name, g_MaxUsedStates, sizeof(StateBit*), CompareFunc_StateBit_Name);

    // Sanity check
    assert(stateBits[0].bitNum == 0);
    for (i = 1; i < g_MaxUsedStates; i++)
    {
        assert(stateBits[i].bitNum == i);
        assert(strcmp(s_SortedStateBitList_Name[i - 1]->name, s_SortedStateBitList_Name[i]->name) != 0);
    }
#endif // SORTED_STATEBIT_LIST
}

int seqGetStateNumberFromName( const char * name )
{
#if SORTED_STATEBIT_LIST
    char stringBuf[100];
    char *c;
    StateBit key;
    StateBit *pKey = &key;
    StateBit **pMatch;

    if (!name)
        return -1;

    devassert(strlen(name) < 100);

    // Make the string upper-case in the key
    for (c = stringBuf; *name; name++, c++)
    {
        *c = toupper(*name);
    }
    *c = 0;

    key.name = (char *) stringBuf;

    pMatch = bsearch(&pKey, s_SortedStateBitList_Name, g_MaxUsedStates, sizeof(StateBit*), CompareFunc_StateBit_Name);

    if (pMatch)
    {
        return (*pMatch)->bitNum;
    }

    return -1;
#else
    int i;
    for( i = 0 ; i < g_MaxUsedStates ; i++ )
    {
        if( stateBits[i].name && !stricmp( stateBits[i].name, name ) )
            return stateBits[i].bitNum;
    }
    return -1;
#endif 
}

char * seqGetStateNameFromNumber( int bitNum )
{
    // The stateBits list is always sorted and the bitNum always equals the index
    if (bitNum < g_MaxUsedStates)
        return stateBits[bitNum].name;

    return 0;
}

//Has Steve or anyone else messed with the defined bit names
int seqStateBitsAreUpdated()
{
    return stateBitsMightBeRenumbered;
}



