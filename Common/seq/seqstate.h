#ifndef _SEQSTATE_H
#define _SEQSTATE_H

#include <utilitieslib/stdtypes.h>

#define STATE_FORWARD                1
#define STATE_BACKWARD               2
#define STATE_STEPLEFT               3
#define STATE_STEPRIGHT              4
#define STATE_JUMP                   5
#define STATE_AIR                    6
#define STATE_APEX                   7
#define STATE_FLY                    8
#define STATE_BIGFALL                9
#define STATE_SWIM                   10
#define STATE_PROFILE                11
#define STATE_IDLE                   12
#define STATE_TURNRIGHT              13
#define STATE_TURNLEFT               14
#define STATE_JETPACK                15
#define STATE_LANDING                 16
#define STATE_SLIDING                 17
#define STATE_BOUNCING                 18
#define STATE_PLAYER                 19
#define STATE_COMBAT                 20
#define STATE_STUN                   21
#define STATE_ICESLIDE               22

#define STATE_GRAVEL                 23
#define STATE_ROCK                   24
#define STATE_TILE                   25
#define STATE_LAVA                   26
#define STATE_GRASS                  27
#define STATE_CONCRETE               28
#define STATE_MUD                    29
#define STATE_DIRT                   30
#define STATE_PUDDLE                 31
#define STATE_METAL                  32
#define STATE_WOOD                   33
#define STATE_ICE                     34
#define STATE_CARPET                 35
// additional surface types defined further down, starting at STATE_SNOW

#define STATE_INDEEP                 36
#define STATE_INWATER                37
#define STATE_INLAVA                 38
#define STATE_INSEWERWATER             39
#define STATE_INREDWATER             40

#define STATE_PREDICTABLE            41
#define STATE_NOCOLLISION            42
#define STATE_WALKTHROUGH            43
#define STATE_CANTMOVE               44
#define STATE_HIDE                   45

#define STATE_JUSTFUCKINDOIT         46
#define STATE_HIGH                   47
#define STATE_LOW                    48
#define STATE_HIT                    49
#define STATE_DEATH                  50
#define STATE_ARENAEMERGE             51
#define STATE_KNOCKBACK              52
#define STATE_BLOCK                  53
#define STATE_STARTOFFENSESTANCE     54
#define STATE_STARTDEFENSESTANCE     55
#define STATE_THANK                  56
#define STATE_RECLAIM                 57
#define STATE_WALK                   58
#define STATE_RUNIN                     59
#define STATE_RUNOUT                 60
#define STATE_EMERGE                 61
#define STATE_HEADPAIN               62
#define STATE_FEAR                   63

#define STATE_SHUT                   64
#define STATE_OPEN                   65
#define STATE_SWEEP                  66
#define STATE_TEST0                  67
#define STATE_TEST1                  68
#define STATE_TEST2                  69
#define STATE_TEST3                  70
#define STATE_TEST4                  71
#define STATE_TEST5                  72
#define STATE_TEST6                  73
#define STATE_TEST7                  74
#define STATE_TEST8                  75
#define STATE_TEST9                  76
#define STATE_STUMBLE                77
#define STATE_SPAWNEDASPET             78
#define STATE_DISMISS                 79
#define STATE_OFF                    80
#define STATE_RAGDOLLBELLY             81
#define STATE_UNLOCKING                 82
#define STATE_LOCKING                 83
#define STATE_RIGHTHAND                 84
#define STATE_LEFTHAND                 85
#define STATE_TWOHAND                 86
#define STATE_DUALHAND                 87
#define STATE_EPICRIGHTHAND             88
#define STATE_SHIELD                 89
#define STATE_HOVER                     90
#define STATE_COSTUMECHANGE             91
#define STATE_NINJA_RUN                 92
#define STATE_PLAYERWALK             93
#define STATE_AMMO                     94
#define STATE_INCENDIARYROUNDS         95
#define STATE_CRYOROUNDS             96
#define STATE_CHEMICALROUNDS         97

// additional surface types (for sound)
#define STATE_SNOW                     98
#define STATE_ASPHALT                 99
#define STATE_SAND                     100
#define STATE_CATWALK                 101
#define STATE_METALPLATE             102
#define STATE_WOODHOLLOW             103
#define STATE_GROUND                 104 // used internally only when entity is on ANY surface type, so scripts don't need to test each surface type individually

#define STATE_BEAST_RUN                 105
#define STATE_STEAM_JUMP             106
#define STATE_HOVER_BOARD             107
#define STATE_MAGIC_CARPET             108
#define STATE_NONHUMAN                 109
#define STATE_PARKOUR_RUN             110
#define CODE_DEFINED_STATE_COUNT     111 //One more than highest #define


//#####  TO ADD NEW STATE BITS:
// EITHER:
//  If the code doesn't need it defined anywhere, just add it to data/sequencers/statebits.statebit 
//
// OR:
//If you do need it defined in the code, 
//        1. add it to the end of this list (increment CODE_DEFINED_STATE_COUNT if needed)
//        2. add it to the end of the seqstate.c list.  (Note that the two lists have to be kept exactly the same, if they aren't, it will assert) Dumb, I know,  but I don't have time to make it smarter right now)
//        3. Increase CODE_DEFINED_STATE_COUNT if necessary. Note that if you change CODE_DEFINED_STATE_COUNT, you need to make a trivial change to the powers parse table so powers.bin will get rebuilt, since powers.bin stores bit number. Swapping the two lines at the bottom of attribmod.h would do the trick.
//
//In either case, if you know, flag whether it's continuing or flash and whether it's predictable)
//
//Finally, there is a MAXSTATES variable which is the actual limit on the number of states you can have.  It's around 1000, and we shouldn't hit that soon
//When we do, we can just up it, but we probably ought to look at Noun/Adjective thing that's disabled now 
//##### 



enum
{
    STATEBIT_CONTINUING        = ( 1 << 0 ),
    STATEBIT_FLASH            = ( 1 << 1 ),
    STATEBIT_PREDICTABLE    = ( 1 << 2 ),
    STATEBIT_CODESET        = ( 1 << 3 ),
};

typedef struct StateBit {
    char    *name;                        // internal name
    U32        bitNum;
    U32        flags;                
} StateBit;

extern StateBit stateBits[];
extern int g_MaxUsedStates;  //MAX STATES actually loaded (set again when bits get loaded from data)

void seqLoadStateBits();
int seqGetStateNumberFromName( const char * name );
char * seqGetStateNameFromNumber( int bitNum );
int seqStateBitsAreUpdated();
#endif
