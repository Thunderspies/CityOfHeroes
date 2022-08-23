#include <utilitieslib/stdtypes.h>
#include "randomCharCreate.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "gameComm/initClient.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include "entity/costume.h"
#include "entity/costume_client.h"
#include "gameData/costume_data.h"
#include "UI/uiCostume.h"
#include "randomName.h"
#include <string.h>
#include <conio.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/utils.h>
#include "UI/uiAvatar.h"
#include "gameComm/trayCommon.h"
#include "cmdparse/cmdgame.h"
#include "account/AccountData.h"
#include "auth/authUserData.h"

static int gCurrentClass;
static int gCurrentOrigin;
extern int gCurrentPower[2];
extern int gCurrentPowerSet[2];
extern int gCurrentGender;


int randInt2(int max) {
    return (rand()*max-1)/RAND_MAX;
}


char *genName() {
    static char name[256];
#ifdef TEST_CLIENT
    extern int gPlayerNumber;
    extern int test_client_num;
    if (test_client_num==0 && gPlayerNumber==0) {
        // @todo name of "TEST" is almost always guaranteed to conflict on anything but a brand new DB
//        return "TEST";
        sprintf(name, "TEST%05d", randInt2(100000));
        return name;
    }
#endif
    do {
        strcpy(name, randomName());
    } while (strlen(name) > 15);
    //sprintf(name, "Auto%04d", randInt2(10000));
    return name;
}

char *genDescription() {
    static char *desc[] = {
        "I was born to destroy newb's",
        "Real players bite\n\"bite me!\"",
        "Bots are the\tway to go",
    };
    return desc[randInt2(ARRAY_SIZE(desc))];
}

int promptRanged(char *str, int max) { // Assumes min of 1, max can't be more than 9
    int ret=-1;
    do {
        printf("%s: [1-%d] ", str, max);
        ret = getch() - '1';
        printf("\n");
    } while (ret<0 || ret>=max);
    return ret;
}

static void testChooseAnOrigin(int askuser)
{
    // pick a random Origin
    int max = eaSize( &g_CharacterOrigins.ppOrigins );
    if (askuser) 
    {
        int i;
        for (i=0; i<max; i++) 
        {
            printf("\t%d : %s\n", i+1, g_CharacterOrigins.ppOrigins[i]->pchName);
        }
        gCurrentOrigin=promptRanged("Choose an Origin", max);
    }
    else
    {
        gCurrentOrigin = randInt2(max);
    }
}

static void testChooseAClass(Entity *e, int askuser)
{
    int max = eaSize( &g_CharacterClasses.ppClasses );
    const CharacterClass *pclass;

    //run in infinite loop until test client chooses a valid class
    do 
    {
        if (askuser) 
        {
            int i;
            for (i=0; i<max; i++) 
            {
                printf("\t%d : %s\n", i+1, g_CharacterClasses.ppClasses[i]->pchName);
            }
            gCurrentClass=promptRanged("Choose a Class", max);
        } 
        else
        {
            gCurrentClass = randInt2(max);
        }

        pclass = g_CharacterClasses.ppClasses[gCurrentClass];

        if (!isAccountAbleToSelectClass(e, pclass))
        {
            if (askuser) 
            {
                printf("The class you chose is restricted!  Please choose another class.\n");
            }
            continue;
        }
        return;

    } while (1);

}

Power *testChooseAPowerSet(Entity *e, int askuser, int category) 
{
    PowerSet *pset;
    const BasePowerSet *pChosenPowerSet;
    int size;
    int i;

    do 
    {
        // Power Set
        size = eaSize( &e->pchar->pclass->pcat[category]->ppPowerSets );
        do 
        {
            if (askuser) 
            {
                printf("%s Power Set:\n", (category==0)?"Primary":"Secondary");
                for (i=0; i<size; i++) {
                    if( character_OwnsPowerSet( e->pchar, e->pchar->pclass->pcat[category]->ppPowerSets[i] ) ) {
                        consoleSetFGColor(COLOR_BRIGHT);
                        printf("\tX: %s\n", e->pchar->pclass->pcat[category]->ppPowerSets[i]->pchName);
                        consoleSetFGColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE);
                    } else {
                        printf("\t%d: %s\n", i+1, e->pchar->pclass->pcat[category]->ppPowerSets[i]->pchName);
                    }
                }
                gCurrentPowerSet[category] = promptRanged("Choose a Power Set", size);
            }
            else
            {
                //dynamically create a list of valid power set indices to restrict
                //random number generator to only pick valid power sets
                int *iValidPowerSetIndices = NULL;
                int numValidPowerSets = 0;
                int randomValidPowerSetNumber = 0;
                for (i=0; i<size; i++) 
                {
                    if( !character_OwnsPowerSet( e->pchar, e->pchar->pclass->pcat[category]->ppPowerSets[i] )
                            && isAccountAbleToSelectPowerSet(e, e->pchar->pclass->pcat[category]->ppPowerSets[i]) 
                            && character_IsAllowedToHavePowerSetHypothetically(e->pchar,e->pchar->pclass->pcat[category]->ppPowerSets[i]) )
                    {
                        eaiPush(&iValidPowerSetIndices, i);    
                    }
                }
                numValidPowerSets = eaiSize(&iValidPowerSetIndices);
                //there has to be at least one valid power set to choose from. If that's not the case, either test client picked a class
                //that has no valid power sets, or something is wrong with the eaiPush check above.
                assert(numValidPowerSets > 0);
                
                randomValidPowerSetNumber = randInt2(numValidPowerSets);
                gCurrentPowerSet[category] = iValidPowerSetIndices[randomValidPowerSetNumber];
                eaiDestroy(&iValidPowerSetIndices);
            }
            
            pChosenPowerSet = e->pchar->pclass->pcat[category]->ppPowerSets[gCurrentPowerSet[category]];

            //prevent user from choosing a restricted powerset
            if (askuser)
            {
                if (!isAccountAbleToSelectPowerSet(e, pChosenPowerSet) || !character_IsAllowedToHavePowerSetHypothetically(e->pchar, pChosenPowerSet))                
                {
                    printf("The power set you chose is restricted!  Please choose another power set.\n");
                    continue;
                }

                if( character_OwnsPowerSet( e->pchar, pChosenPowerSet ) )
                {
                    printf("You already own that power set!  Choose another.\n");
                    continue;
                }            
            }

            break;
        } while (true);

        size = eaSize( &pChosenPowerSet->ppPowers );

        if (askuser) 
        {
            int newmax=0;
            printf("%s Power:\n", (category==0)?"Primary":"Secondary");
            for (i=0; i<size; i++) {
                int available = pChosenPowerSet->piAvailable[i]==0;
                if (!available) {
                    consoleSetFGColor(COLOR_BRIGHT);
                } else {
                    newmax = i;
                }
                printf("\t%c: %s%s\n", available?('1'+i):' ', pChosenPowerSet->ppPowers[i]->pchName,
                    available?"":" [Unavailable]");
                consoleSetFGColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE);
            }
            gCurrentPower[category] = promptRanged("Choose a Power", newmax+1);
        } 
        else 
        {        
            //dynamically create a list of valid power indices to restrict
            //random number generator to only pick valid powers
            int *iValidPowerIndices = NULL;
            int numValidPowers = 0;
            int randomValidPowerNumber = 0;
            for (i=0; i<size; i++) 
            {
                if( pChosenPowerSet->piAvailable[i]==0 )
                {
                    eaiPush(&iValidPowerIndices, i);    
                }
            }
            numValidPowers = eaiSize(&iValidPowerIndices);
            //there has to be at least one valid power to choose from. If that's not the case, either test client picked a power set
            //that has no valid powers, or something is wrong with the eaiPush check above.
            assert(numValidPowers > 0);

            randomValidPowerNumber = randInt2(numValidPowers);
            gCurrentPower[category] = iValidPowerIndices[randomValidPowerNumber];
            eaiDestroy(&iValidPowerIndices);
        }

        if( gCurrentPower[category] >= 0 && gCurrentPowerSet[category] >= 0 && pChosenPowerSet->piAvailable[gCurrentPower[category]]==0)
        {
            Power *p;
            bool bAdded;

            printf("Cat %d: Chose powerset %s\n", category, pChosenPowerSet->pchName);
            printf("Cat %d: Chose power %s\n", category, pChosenPowerSet->ppPowers[gCurrentPower[category]]->pchName);

            pset = character_BuyPowerSet( e->pchar, pChosenPowerSet );
            p = character_BuyPowerEx(e->pchar, pset, pChosenPowerSet->ppPowers[gCurrentPower[category]], &bAdded, 0, 0, NULL);
            if(!p || !bAdded) 
            {
                printf("Purchasing requested power/powerset failed\n");
                continue;
            }
            return p;
        } 
        else if (askuser) 
        {
            printf("Chose bad/unavailable power/powerset\n");
        }
    } while (1);
    return NULL;
}

static float randFloat(float min, float max)
{
    return min + randInt2((max - min)*1000.f)/1000.f;
}

int simulateCharacterCreate(int askuser, int connectionless) 
{
    Entity *e = NULL;
    int category;

    doCreatePlayer(connectionless);
    e = playerPtr();

    testChooseAnOrigin(askuser);
    printf("Chose origin %s\n", g_CharacterOrigins.ppOrigins[gCurrentOrigin]->pchName);

    testChooseAClass(e, askuser);
    printf("Chose class %s\n", g_CharacterClasses.ppClasses[gCurrentClass]->pchName);

    character_Create(e->pchar, e, g_CharacterOrigins.ppOrigins[gCurrentOrigin], g_CharacterClasses.ppClasses[gCurrentClass]);

    // Choose primary powers
    // Choose secondary powers
    for (category=0; category<2; category++) {
        testChooseAPowerSet(e, askuser, category);
    }

    //give costume parts dependent on selected power sets
    costumeAwardPowersetParts(e,0,0);

    // Gender, height, etc
    {
        Costume* mutable_costume = entGetMutableCostume(e);
        costume_init(mutable_costume);
        costume_SetScale(mutable_costume, (randInt2(2000) - 1000)/100.0f); // -10 to 10
        costume_SetBoneScale( mutable_costume, (randInt2(2000) - 1000)/1000.0f ); // -1 to 1
        // Other scales
        {
            float start;
            float fBoneScale = mutable_costume->appearance.fBoneScale;
            
            start = (((fBoneScale+1)*(2-1.0))/2)-1; // headScaleRange = 1.0
            mutable_costume->appearance.fHeadScale = randFloat(start, start + 1.0 );

            start = (((fBoneScale+1)*(2-1.0))/2)-1; // shoulderScaleRange = 1.0
            mutable_costume->appearance.fShoulderScale = randFloat(start, start + 1.0 );

            start = (((fBoneScale+1)*(2-0.75))/2)-1; // chestScaleRange = 0.75
            mutable_costume->appearance.fChestScale = randFloat(start, start + 0.75 );

            start = (((fBoneScale+1)*(2-1.1))/2)-1; // waistScaleRange
            mutable_costume->appearance.fWaistScale = randFloat(start, start + 1.1 );

            start = (((fBoneScale+1)*(2-1.0))/2)-1; // hipScaleRange
            mutable_costume->appearance.fHipScale = randFloat(start, start + 1.0 );
        }
    }

    gCurrentGender = randInt2(3);
    switch (gCurrentGender) {
        xcase 0:
            gCurrentGender = AVATAR_GS_FEMALE;
            setGender( e, "fem" );
        xcase 1:
            gCurrentGender = AVATAR_GS_MALE;
            setGender( e, "male" );
        xcase 2:
            gCurrentGender = AVATAR_GS_HUGE;
            setGender( e, "huge" );
    }

    // Name, etc
    if (askuser) {
        printf("Character Name: ");
        gets_s(e->name, sizeof(e->name));
    } else {
        strcpy(e->name, genName());
    }
    strcpy(e->strings->motto, e->name);
    strcpy(e->strings->playerDescription, genDescription());

    // Costume
    //initCostume();
    setDefaultCostume(false);
    randomCostume();

#ifndef TEST_CLIENT
    { // Fill in tray
        int i, j, tray = 0, slot = 0;
        int iSizeSets = eaSize( &e->pchar->ppPowerSets );
        for( i = 0; i < iSizeSets; i++ )
        {
            int iSizePows = eaSize( &e->pchar->ppPowerSets[i]->ppPowers );
            for( j = 0; j < iSizePows; j++)
            {
                if( e->pchar->ppPowerSets[i]->ppPowers[j]->ppowBase->eType != kPowerType_Auto )
                {
                    TrayObj *to = getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, tray, slot, 0, 1);
                    if (!to) // since we createIfNotFound, this only happens if we run off the end
                    {
                        i = iSizeSets; // outer break, effectively
                        break;
                    }

                    buildPowerTrayObj(to, i, j, 0);
                    slot++;
                    if (slot == TRAY_SLOTS)
                    {
                        slot = 0;
                        tray++;
                    }
                }
            }
        }
    }
#endif

    // Send to server
    checkForCharacterCreate();

    return 0;
}

int isAccountAbleToSelectClass(Entity *e, const CharacterClass *pClass)
{

    int allowed = 1;
    assert(e!=NULL);
    assert(pClass!=NULL);

    //special check for Peacebringer and Warshade classes
    if (class_MatchesSpecialRestriction(pClass, "Kheldian") && !authUserHasKheldian(e->pl->auth_user_data))
    {
        if (pClass->pchStoreRestrictions)
        {
            allowed &= accountEval(&e->pl->account_inventory, e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pClass->pchStoreRestrictions);
        } 
        else
        {
            allowed = false;
        }
    }
    //special check for Arachnos Widow and Arachnos Soldier classes
    else if ((class_MatchesSpecialRestriction(pClass, "ArachnosSoldier") || class_MatchesSpecialRestriction(pClass, "ArachnosWidow")) &&
        !authUserHasArachnosAccess(e->pl->auth_user_data))
    {
        if (pClass->pchStoreRestrictions)
        {
            allowed &= accountEval(&e->pl->account_inventory, e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pClass->pchStoreRestrictions);
        } 
        else 
        {
            allowed = false;
        }
    } 
    else
    {
        if (pClass->pchStoreRestrictions)
        {
            allowed &= accountEval(&e->pl->account_inventory, e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pClass->pchStoreRestrictions);
        }
    }

    return allowed;
    
}

int isAccountAbleToSelectPowerSet(Entity *e, const BasePowerSet *pPowerSet)
{
    assert(e!=NULL);
    assert(pPowerSet!=NULL);

    //power set has no store restrictions, so account is able to select it
    return !pPowerSet->pchAccountRequires || accountEval(&e->pl->account_inventory, e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pPowerSet->pchAccountRequires);
}
