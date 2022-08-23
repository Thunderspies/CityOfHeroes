//////////// Get Item of Power Data ////////////////////////////////////////////////////////////////////////

#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include "entity/entVarUpdate.h"        // for PKT_BITS_TO_REP_DB_ID
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/mathutil.h>
#include <time.h>
#include "iopdata.h"
#include <utilitieslib/utils/textparser.h> 

ItemOfPowerInfoList g_itemOfPowerInfoList;
StashTable    itemOfPowerInfo_ht = 0;

TokenizerParseInfo ParseItemOfPowerInfo[] =
{
    { "ItemOfPower",            TOK_IGNORE,                        0                                        }, // hack, for reloading
    { "Name",                    TOK_STRUCTPARAM | TOK_STRING( ItemOfPowerInfo, name, 0 )    },
    { "Unique",                    TOK_BOOLFLAG( ItemOfPowerInfo, unique, 0        )    },
    {    "{",                    TOK_START,                        0 },
    {    "}",                    TOK_END,                            0 },
    { "", 0, 0 }

};

TokenizerParseInfo ParseItemOfPowerInfoList[] = 
{
    { "DaysGameLasts",                TOK_INT( ItemOfPowerInfoList, daysGameLasts ,            30)        },
    { "DaysCathedralIsOpen",        TOK_INT( ItemOfPowerInfoList, daysCathedralIsOpen,        5 )        },
    { "ChanceOfGettingUnique",        TOK_INT( ItemOfPowerInfoList, chanceOfGettingUnique,    30 )    },
    { "allowScheduledRaids",        TOK_BOOL( ItemOfPowerInfoList, allowScheduledRaids,        0)        },
    { "ItemOfPower",                TOK_STRUCT( ItemOfPowerInfoList, infos, ParseItemOfPowerInfo)    },
    { "", 0, 0 }
};


void LoadItemOfPowerInfos()
{
    ItemOfPowerInfo * info;
    int i;
    int infoCount;

    if (itemOfPowerInfo_ht) stashTableDestroy(itemOfPowerInfo_ht);
    itemOfPowerInfo_ht = stashTableCreateWithStringKeys(300,  StashDeepCopyKeys | StashCaseSensitive );

    eaDestroy( &g_itemOfPowerInfoList.infos ); //TO DO : a smarter version of destroy, and load files shared
    ParserLoadFiles(0, "defs/itemOfPowerInfo.def", "itemOfPowerInfo.bin", PARSER_SERVERONLY, ParseItemOfPowerInfoList, &g_itemOfPowerInfoList, NULL, NULL, NULL);

    infoCount = eaSize( &g_itemOfPowerInfoList.infos );

    if( infoCount == 0 )
    {
        FatalErrorf( "Bad or missing itemOfPowerInfo.def, can't get the rules to the game, or any Items of Power to give out" );
    }

    for( i = 0 ; i < infoCount ; i++ )
    {
        int j;
        info = g_itemOfPowerInfoList.infos[i];
        stashAddPointer( itemOfPowerInfo_ht, info->name, info , false); 

        //Debug 
        for( j = 0 ; j < i ; j++ )
        {
            if( 0 == stricmp(g_itemOfPowerInfoList.infos[j]->name, info->name ) )
            {
                assert( 0 == stricmp(g_itemOfPowerInfoList.infos[j]->name, info->name ) );  //Hash Collision check, this is just temp until we decide whether to put this in the db, so don't worry about it's weirdness
                Errorf( "Multiple Items of Power with the same name in defs/itemOfPowerInfo.def for %s ", info->name );
            }
        }
        //End Debug 
    }
}

ItemOfPowerInfo * getItemOfPowerInfoFromList( char * name )
{
    if( !name )
        return 0;
    return stashFindPointerReturnPointer( itemOfPowerInfo_ht, name );
}
