
#include <utilitieslib/stdtypes.h>
#include "backup.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include "container.h"
#include "comm_backend.h"
#include "dbdispatch.h"
#include <utilitieslib/network/net_typedefs.h>
#include "dbserver/servercfg.h"
#include "container_tplt_utils.h"
#include "container_merge.h"
#include <utilitieslib/utils/hoglib.h>
#include "clientcomm.h"
#include "namecache.h"
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>

typedef struct BackupHeader 
{
    //the first two ints here are used as a key for a hash table and must come first.
    //Scott N points out that the C definition does not guarantee the ordering within
    //a struct to remain the same, so if you've got a bug that might be caused by the
    //key being some nonsense at some point in the future, this may be a place to look.
    int        id;
    int        type;

    U32        date;
    U8        compressed:1;
    U8        newSinceDbStartup:1;
    char    *info;

}BackupHeader;

typedef struct HogDate
{
    HogFile * file;
    int date;
}HogDate;


HogDate ** file_list;

BackupHeader **backup_header;    //this needs to be maintained in order by date.

static StashTable s_backupStash;
#define BACKUP_STARTING_SIZE 1000

static FILE    *index_file,*write_file,*read_file;
static int gDbShuttingDown;

//--------------------------------------------------------------------------------------------
//  Utility functions /////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

static void s_initBackupStash()
{
    static int inited = 0;
    if(!inited)
    {
        inited = 1;
        s_backupStash = stashTableCreateFixedSize(BACKUP_STARTING_SIZE, 2*sizeof(U32));    //8 obviously--this is the first two ints in the structure.
    }
}

static void s_getBackupHeaderList(int id, int type, BackupHeader ***retHeaders)
{
    U32 key[2] = {0};
    StashElement pElement;
    eaSetSize(retHeaders, 0);
    key[0] = id;
    key[1] = type;
    stashFindElement(s_backupStash, key, &pElement);
    if(pElement)
        *retHeaders = stashElementGetPointer(pElement);//pElement->value;
    else
        *retHeaders = 0;
    //stashFindPointer(s_backupStash, key, ((void **)retHeaders));
    
}

//negative value means that a was earlier than b
static INLINEDBG int s_compareTimestamps(U32 a, U32 b) {return timerDayFromSecondsSince2000( a ) - timerDayFromSecondsSince2000( b );} 

static int compare_backups(const BackupHeader** bh1, const BackupHeader** bh2 )
{
    if( (*bh1)->type == (*bh2)->type )
    {
        if( (*bh1)->id == (*bh2)->id )
            return s_compareTimestamps( (*bh1)->date, (*bh2)->date );
        else
            return ((*bh1)->id - (*bh2)->id);
    }
    else
        return ((*bh1)->type - (*bh2)->type);
}

static BackupHeader *s_getBackupHeaderByTime(int id, int type, int timestamp)
{
    static BackupHeader **allTimes = 0;
    BackupHeader **retHeader = 0;
    static BackupHeader search_key = {0};
    BackupHeader *pointer = &search_key;
    StashElement pElement = NULL;
    search_key.id = id;
    search_key.type = type;
    search_key.date = timestamp;

    stashFindElement(s_backupStash, pointer, &pElement);
    if(pElement)
        allTimes = stashElementGetPointer(pElement);//pElement->value;
    else
        allTimes = 0;
    //stashFindPointer(s_backupStash, pointer, (void **)&allTimes);
    retHeader = eaBSearch(allTimes, compare_backups, pointer);
    return (retHeader)?*retHeader:NULL;
}

static void s_indexBackupHeader(BackupHeader* bh)
{
    static BackupHeader **allTimes = 0;
    int nOldResults;
    StashElement pElement;

    stashFindElement(s_backupStash, bh, &pElement);
    if(pElement)
        allTimes = stashElementGetPointer(pElement);//pElement->value;
    else 
        allTimes = 0;
    //stashFindPointer(s_backupStash, bh, (void **)&allTimes);
    //every time we add a header, we check to see if placing it on the end (usual case) would maintain order by time.
    //if not, we actually have to bsearch out its place.
    nOldResults = eaSize(&allTimes);
    if(nOldResults && (s_compareTimestamps(allTimes[nOldResults-1]->date, bh->date)>0))
    {
        int i = 2;    //linear is OK because we should always be dealing with very small numbers of backups.
        int insertPosition;
        while((nOldResults -i>= 0) && (s_compareTimestamps(allTimes[nOldResults-i]->date, bh->date)>0))
        {
            i++;
        }
        insertPosition = nOldResults-(i-1);
        eaInsert(&allTimes, bh, insertPosition);
    }
    else
        eaPush(&allTimes, bh);

    devassert(nOldResults+1 == eaSize(&allTimes));

    stashAddPointer(s_backupStash, bh, (void **)allTimes, 1);
}

static char *backupDir()
{
    char    *s;
    static char backup_dir[MAX_PATH];

    strcpy(backup_dir,getLogDir());
    s = strrchr(backup_dir,'/');
    if (s)
        *s = 0;
    strcat(backup_dir,"/db_backup/");
    mkdirtree(backup_dir);
    s = strrchr(backup_dir,'/');
    if (s)
        *s = 0;
    return backup_dir;
}

static char *backupIndexFileName(int date)
{
    static char index_name[MAX_PATH];

    sprintf(index_name,"%s/backup_%i.log",backupDir(), timerDayFromSecondsSince2000(date));
    return index_name;
}

static char *backupDataFileName(int date)
{
    static char data_name[MAX_PATH];
    sprintf(data_name,"%s/backup_%i.data",backupDir(), timerDayFromSecondsSince2000(date));
    return data_name;
}

static HogFile * getDataHogFileByDate( int date )
{
    int i, time = timerDayFromSecondsSince2000(date);
    for( i = eaSize(&file_list)-1; i>=0; i-- )
    {
        if( file_list[i]->date == time )
            return file_list[i]->file;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------
//   File/Contianer Work /////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
static StashTable st_SavedSupergroups;

bool backupSaveContainer( AnyContainer * con, U32 timestamp )
{
    char * str, extra_info[256], filenamebuf[256];
    BackupHeader * bh;
    DbContainer * container = (DbContainer*)con;
    HogFile * hog;
    int size;
    if ( server_cfg.disableContainerBackups )
        return 0;
    if (!container || gDbShuttingDown)
        return 0;

    // Supergroups and supergroup bases only saved once per day
    if( container->type == CONTAINER_SUPERGROUPS )
    {
        if(!st_SavedSupergroups)
            st_SavedSupergroups = stashTableCreateInt(256);

        if(stashIntFindInt(st_SavedSupergroups, container->id, 0))
            return 0; // saved today already
        else
            stashIntAddInt(st_SavedSupergroups, container->id, 0, 1 );
    }

    bh = calloc(1, sizeof(BackupHeader));
    if( timestamp )
        bh->date = timestamp;
    else
        bh->date = timerSecondsSince2000();
    bh->type = container->type;
    bh->id = container->id;
    // no compression yet, save character
    str = strdup(containerGetText(container));
    size = (int)strlen(str)+1;

    hog = getDataHogFileByDate( bh->date );
    if( !hog )
    {
        HogDate * hd = malloc(sizeof(HogDate));
        hd->file = hog = hogFileReadOrCreate(backupDataFileName(bh->date),0);
        hd->date = timerDayFromSecondsSince2000(bh->date);
        eaPush(&file_list,hd);
        if(st_SavedSupergroups) stashTableClear(st_SavedSupergroups); // New day!
    }
    sprintf( filenamebuf, "%i_%i_%i", bh->id, bh->type, bh->date );
    hogFileModifyUpdateNamed( hog, filenamebuf, str, size, bh->date );

    if( container->type == CONTAINER_ENTS)
    {
        int xp,inf;
        containerGetField(dbListPtr(CONTAINER_ENTS),container,CFTYPE_INT,"ExperiencePoints",0,&xp,0);
        containerGetField(dbListPtr(CONTAINER_ENTS),container,CFTYPE_INT,"InfluencePoints",0,&inf,0);
        sprintf( extra_info, "%i / %i / %i", ((EntCon*)container)->level, xp, inf );
        bh->info = strdup(extra_info);
    }
    else if( container->type == CONTAINER_SUPERGROUPS)
    {
        int prestige,prestigeBase;
        containerGetField(dbListPtr(CONTAINER_SUPERGROUPS),container,CFTYPE_INT,"Prestige",0,&prestige,0);
        containerGetField(dbListPtr(CONTAINER_SUPERGROUPS),container,CFTYPE_INT,"PrestigeBase",0,&prestigeBase,0);
        sprintf( extra_info, "%i / %i", prestige, prestigeBase );
        bh->info = strdup(extra_info);
    }
    else if ( container->type == CONTAINER_BASE )
    {
        int sgid;
        containerGetField(dbListPtr(CONTAINER_BASE),container,CFTYPE_INT,"SupergroupId",0,&sgid,0);
        sprintf( extra_info, "%i", sgid );
        bh->info = strdup(extra_info);
    }
    else
        bh->info = strdup( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );

    // update index file  // id type date compressed info
    sprintf( filenamebuf, "/db_backup/backup_%i", timerDayFromSecondsSince2000(bh->date) );
    filelog_printf( filenamebuf, "%i %i %u %i \"%s\"\n", bh->id, bh->type, bh->date, 0, bh->info );
    eaPush(&backup_header,bh);
    s_indexBackupHeader(bh);
    return 1;
}

static char *getBackup(BackupHeader *bh)
{
    int        file_idx, size;
    char    *data,filenamebuf[256];
    HogFile *hog;

    hog = getDataHogFileByDate(bh->date);
    if( !hog )
        return 0;
    
    sprintf( filenamebuf, "%i_%i_%i", bh->id, bh->type, bh->date );
    file_idx = hogFileFind( hog, filenamebuf );
    if( file_idx < 0 )
        return 0;

    data = hogFileExtract( hog, file_idx, &size );

    if( bh->type == CONTAINER_ENTS )
        tpltUpdateData(&data, &size, "TeamupsId 0\nTaskforcesId 0\n", dbListPtr(CONTAINER_ENTS)->tplt);

    return data;
}


// This will load a backup, into the current container.  
static int backupLoadContainer( int id, int type, U32 date )
{
    DbContainer *container = containerPtr(dbListPtr(type),id);
    BackupHeader *bh;
    DbList * list = dbListPtr(type);

    if (container)
        return 0; // don't overwrite container if it's loaded and online already

     bh = s_getBackupHeaderByTime(id,type,date);
     if( bh )
    {
        char * data = getBackup(bh);
        container = containerLoad(list,id);
        if( !data )
            return 0;

        // save container before we overwrite it
         if( container )
            backupSaveContainer( container, 0 );
        
        handleContainerUpdate(type,id,1,data);

        if( type == CONTAINER_SUPERGROUPS ) // if its a supergroup, restore the base
        {
            char buf[32];
            int base_id;
            DbContainer *con;

            sprintf( buf, "%i", id );
            base_id = containerIdFindByElement( dbListPtr(CONTAINER_BASE), "SupergroupId", buf );
            con = containerLoad(dbListPtr(CONTAINER_BASE),base_id);

            if( con && findFieldTplt(dbListPtr(CONTAINER_SUPERGROUPS)->tplt,&container->line_list,"Ownsbase",buf) && atoi(buf) > 0 )
            {
                containerUnload(dbListPtr(CONTAINER_BASE),base_id);
                backupLoadContainer(base_id, CONTAINER_BASE, date );
            }
            else if( con )
                containerDelete( con );
        }

        containerUnload(list,id);

        // This way should've worked but sql query failed at execution!
        //handleContainerUpdate(type,id,1,data);
        //containerUnload(dbListPtr(type),id);
        free(data);

        return 1;
    }
    return 0; // no match
}

static char *backupView( int id, int type, U32 date )
{
    AnyContainer *container = containerPtr(dbListPtr(type),id);
    BackupHeader *bh;

    bh = s_getBackupHeaderByTime(id,type,date);
    if( bh )
    {
        return getBackup(bh);
    }
    return 0;
}

void backupLoadIndexFiles(void)
{
    U32 time, date = timerSecondsSince2000();
    int i;
    U64 available_space = fileGetFreeDiskSpace(backupDir());

    if( gDbShuttingDown )
        return;

    loadstart_printf("Reading Backup Files...");
    s_initBackupStash();

    while( !isDevelopmentMode() && available_space < 6000000000 )
    {
        // less than 6 gigs, oh noes!
        int response = MessageBoxA( compatibleGetConsoleWindow(), "This DB server has less than 6gigs of HD space.  This is pretty low, and backups will no longer be done. You can delete some files and retry. You can ignore this for a short time, but it really needs to be fixed soon.", "DB error!", MB_ABORTRETRYIGNORE | MB_ICONQUESTION);

        if(response==IDABORT)
        {    
            exit(1);
        }
        else if(response==IDRETRY) 
        {
            available_space = fileGetFreeDiskSpace(backupDir());
        }
        else if(response==IDIGNORE) 
            return;
    }

    // load time period files
    for( i = 0; i < server_cfg.backup_period; i++ )
    {
        U32 time = date - (SECONDS_PER_DAY*i);
        if( fileExists(backupDataFileName(time)) && fileExists(backupIndexFileName(time)) ) // check hogg file first
        {
            if( fileSize(backupIndexFileName(time)) > 10000000 ) // Files bigger than 10 MB are way to big
            {
                printf("\n WARNING! Backup header file is greater than 10Mb and will not be loaded.  Please contact a developer if there are lots of these warnings.\n" );
            }
            else
            {
                HogDate * hd = malloc(sizeof(HogDate));
                char *s,*args[100],*fileStart;
                int count;
                hd->file = hogFileReadOrCreate(backupDataFileName(time),NULL);
                hd->date = timerDayFromSecondsSince2000(time);
                eaPush(&file_list, hd);

                index_file = fopen(backupIndexFileName(time),"rb");
                s = fileStart = fileAlloc(backupIndexFileName(time),0);

                while(count=tokenize_line(s,args,&s))
                {
                    char filenamebuf[256];
                    BackupHeader * bh;
                    if( count != 9 )
                        continue;

                    bh = malloc(sizeof(BackupHeader));
                    // log printf prepends date_buf (0), pid (1), uid_local (2).

                    // id type date size compressed info
                    bh->id = atoi(args[4]);
                    bh->type = atoi(args[5]);
                    bh->date = atoi(args[6]); // I didn't want to bother unstringing date_buf
                    bh->compressed = atoi(args[7]);
                    bh->info = strdup(args[8]);

                    // if file doesn't exist for entry we must've crashed and gotten out of sync
                    sprintf( filenamebuf, "%i_%i_%i", bh->id, bh->type, bh->date );
                    if( hogFileFind(hd->file,filenamebuf) >= 0 ) 
                    {
                        int arrayIndex = eaPush(&backup_header,bh);
                        s_indexBackupHeader(bh);
                    }
                    else
                        free(bh);
                }
                free(fileStart);
                fclose(index_file);
                index_file = 0;
            }
        }
    }
    
    // delete older files - check 30 days past
    time = date - (server_cfg.backup_period*SECONDS_PER_DAY);
    while( time > date - ((server_cfg.backup_period+30)*SECONDS_PER_DAY)  ) 
    {
        if( fileExists(backupIndexFileName(time)) )
        {
            if(remove(backupIndexFileName(time)) != 0)
                printf("Warning: Could not remove character backup index %s\n", backupIndexFileName(time));
            if(remove(backupDataFileName(time)) != 0)
                printf("Warning: Could not remove character backup data %s\n", backupDataFileName(time));
        }
        time -= SECONDS_PER_DAY;
    }

    // sort list for binary searching
    eaQSort(backup_header, compare_backups );

    loadend_printf("");
}

void backupUnloadIndexFiles(void)
{
    while( eaSize(&file_list) )
    {
        HogDate * rem = eaRemove(&file_list,0);
        hogFileDestroy( rem->file );
        SAFE_FREE(rem);
    }

    gDbShuttingDown = true;
}

//--------------------------------------------------------------------------------------------
// Deal with requests from mapserver /////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

void handleBackup(Packet *pak, NetLink *link)
{
    int db_id = pktGetBitsPack(pak,1);
    int type = pktGetBitsPack(pak,1);
    AnyContainer *container = containerPtr(dbListPtr(type),db_id);
    
    if(container && !gDbShuttingDown) 
        backupSaveContainer(container, 0);
}

static void sendBackupHeader(Packet * pak, BackupHeader * bh)
{
    pktSendBits(pak, 1, 1);
    pktSendBitsPack( pak, 1, bh->date );
    pktSendString( pak, bh->info );
}

static void s_reportBackupResults(Packet *pak_out, BackupHeader ***backups)
{
    int nBackups = eaSize(backups);
    int i;
    for(i = 0; i < nBackups; i++)
    {
        sendBackupHeader(pak_out, (*backups)[i]);
    }
}
void handleBackupSearch(Packet *pak, NetLink *link)
{
    BackupHeader **searchResults = 0;
     int id = pktGetBitsPack(pak,1);
     int type = pktGetBitsPack(pak,1);
    int return_id = pktGetBitsPack(pak,1);
    Packet *pak_out = pktCreateEx(link,DBSERVER_BACKUP_SEARCH);

    if( gDbShuttingDown )
        return;

    pktSendBitsPack( pak_out, 1, return_id );
    pktSendBitsPack(pak_out,1,type);

    pktSendBitsPack( pak_out, 1, id );

    s_getBackupHeaderList(id, type, &searchResults);
    s_reportBackupResults(pak_out, &searchResults);

    pktSendBits(pak_out, 1, 0);
    pktSend(&pak_out,link);
}

void handleBackupApply(Packet *pak, NetLink *link)
{
    Packet* pak_out = NULL;

    int id = pktGetBitsPack(pak,1);
    int type = pktGetBitsPack(pak,1);
    U32 date = pktGetBitsPack(pak,1);
    int return_id = pktGetBitsPack(pak,1);

    if( gDbShuttingDown )
        return;

    pak_out = pktCreateEx(link,DBSERVER_BACKUP_APPLY);
    pktSendBitsPack(pak_out, 1, return_id);
    pktSendBitsPack(pak_out,1,type);

    if(    backupLoadContainer(id,type,date) )
        pktSendBits(pak_out, 1, 1);
    else
        pktSendBits(pak_out, 1, 0);
 
    pktSend(&pak_out,link);
}

void handleBackupView(Packet *pak, NetLink *link)
{
    char * out;
     int id = pktGetBitsPack(pak,1);
    int type = pktGetBitsPack(pak,1);
    U32 date = pktGetBitsPack(pak,1);
    int return_id = pktGetBitsPack(pak,1);

    if( gDbShuttingDown )
        return;

    out = backupView(id,type,date);
    if( out )
    {
        Packet* pak_out = pktCreateEx(link,DBSERVER_BACKUP_VIEW);
        pktSendBitsPack(pak_out,1,return_id);
        pktSendBitsPack(pak_out,1,type);
        pktSendString(pak_out, out);
        pktSend(&pak_out,link);
    }
}



