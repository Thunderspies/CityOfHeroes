#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include "dbimport.h"
#include "dbdispatch.h"
#include "comm_backend.h"
#include "sql_fifo.h"
#include "container_merge.h"
#include <utilitieslib/utils/memcheck.h>
#include "namecache.h"

static FILE *mappingfile;
static FILE *brokenfile;
static FILE *progressfile;

static StashTable mappingTable;
static StashTable brokenTable;

// Keeps track of mapping references to actual container ids
static void readMappingFile()
{
    char line[512];
    mappingTable = stashTableCreateWithStringKeys(1000,StashDeepCopyKeys);
    fseek(mappingfile,0,0);
    while (fgets(line,512,mappingfile))
    {    
        char *val;
        int num;        
        val = strstr(line," ");
        *val = '\0';
        val++;
        num = atoi(val);
        stashAddInt(mappingTable,line,num,false);
    }
    fseek(mappingfile, 0, SEEK_END);
}

static void addMapping(char* key, int val)
{
    if (!stashFindElement(mappingTable,key,NULL))
    {
        fprintf(mappingfile,"%s %d\n",key,val);
        stashAddInt(mappingTable,key,val,0);
    }
}

// Keeps track of references that don't resolve
static void readBrokenFile()
{
    char line[512];
    brokenTable = stashTableCreateWithStringKeys(1000,StashDeepCopyKeys);    
    fseek(brokenfile,0,0);
    while (fgets(line,512,brokenfile))
    {    
        char *val, *nl;
        val = strstr(line," ");
        *val = '\0';
        val++;
        nl = strstr(val,"\n");
        if (nl)
        {
            *nl = '\0';
        }
        stashAddPointer(brokenTable,line,strdup(val),false);
    }
    fseek(brokenfile, 0, SEEK_END);
}

static void addBroken(char *conid, char *conname, char *convalue)
{
    char key[512];
    sprintf(key,"%s.%s",conid,conname);

    if (!stashFindElement(brokenTable,key,NULL))
    {
        fprintf(brokenfile,"%s %s\n",key,convalue);
        stashAddPointer(brokenTable,key,strdup(convalue),false);        
    }
}

static int s_splitNames = 0;

// Actually add the container to the database
static int pushContainer(int list_id, char *condata)
{
    DbContainer    *container;

    char *data,*new_data,old_name[200]="",new_name[200]="";
    int data_size;
    int returnid = -1;

    data =strdup(condata);

    // Rename ents and supergroups if needed
    if (list_id == CONTAINER_ENTS)
    {
        /* Let them import too many characters, ui will show subset
        if (!putCharacterVerifyFreeSlot(data))
        {
            // Do something to free up slots
            return -1;
        }*/

        findFieldText(data,"Name",old_name);
        new_data = putCharacterVerifyUniqueName(data);

        if (!new_data)
        {
            fprintf(progressfile,"Invalid name: %s\n",old_name);
            return -1;
        }

        if (new_data != data)
        {
            free(data);
            data = strdup(new_data);
        }
        findFieldText(data,"Name",new_name);
        data_size = (int)strlen(data)+1;
        if (stricmp(old_name,new_name)!=0)
        {
            // Grant the rename token
            char    db_flags_text[200] = "";
            U32        db_flags;

            findFieldText(data,"DbFlags",db_flags_text);
            db_flags = atoi(db_flags_text);
            db_flags |= 1 << 12; // DBFLAG_RENAMEABLE
            sprintf(db_flags_text,"DbFlags %u\n",db_flags);
            tpltUpdateData(&data, &data_size, db_flags_text,dbListPtr(CONTAINER_ENTS)->tplt);

            if (s_splitNames)
            { // We need to rename the OTHER person too.
                char cmd[1024];
                int id;
                U32 authId;
                strcpy(new_name, unescapeString(new_name));
                incrementName(new_name, 20); //MAX_PLAYER_NAME_LEN
                strcpy(new_name, escapeString(new_name));
                while (!playerNameValid(new_name, NULL, 0)) {
                    strcpy(new_name, unescapeString(new_name));
                    incrementName(new_name, 20); //MAX_PLAYER_NAME_LEN
                    strcpy(new_name, escapeString(new_name));
                }
                id = containerIdFindByElement(dbListPtr(CONTAINER_ENTS),"Name",old_name);
                if (id > 0)
                {
                    EntCatalogInfo *entInfo = entCatalogGet( id, false );    assert(entInfo);
                    authId = ( entInfo ) ? entInfo->authId : 0;
                    sqlFifoBarrier();
                    sprintf(cmd,"UPDATE dbo.Ents SET DbFlags = ISNULL(DbFlags,0) | 4096 WHERE ContainerId = %d;",id); // DBFLAG_RENAMEABLE
                    if (gDatabaseProvider == DBPROV_SQLITE) {
                        sqlExecAsyncEx(cmd, SQL_NTS, id, false);
                        sqlFifoFinish();
                    } else sqlConnExecDirect(cmd, SQL_NTS, SQLCONN_FOREGROUND, false);
                    sqlFifoBarrier();
                    sprintf(cmd,"UPDATE dbo.Ents SET Name = N'%s' WHERE ContainerId = %d;", new_name, id);
                    if (gDatabaseProvider == DBPROV_SQLITE) {
                        sqlExecAsyncEx(cmd, SQL_NTS, id, true);
                        sqlFifoFinish();
                    } else sqlConnExecDirect(cmd, SQL_NTS, SQLCONN_FOREGROUND, true);
                    sqlFifoBarrier();
                    playerNameDelete(old_name, id);    
                    playerNameCreate(new_name, id, authId);
                }
            }
        }
    }
    else if (list_id == CONTAINER_SUPERGROUPS)
    {
        findFieldText(data,"Name",old_name);
        new_data = putSupergroupVerifyUniqueName(data);
        if (!new_data)
        {
            fprintf(progressfile,"Invalid name: %s\n",old_name);
            return -1;
        }
        if (new_data != data)
        {
            free(data);
            data = strdup(new_data);
        }
    }

    // Grab a free containerid
    container = containerAlloc(dbListPtr(list_id),-1);
    if (!container)
    {
        fprintf(progressfile,"Unable to allocate container!\n");
        return -1;
    }

    // Insert the data
    container = containerUpdate_notdiff(dbListPtr(list_id),container->id,data);
    
    if (!container)
    {
        fprintf(progressfile,"Bad Container data!\n");
        return -1;
    }

    returnid = container->id;
    containerUnload(dbListPtr(list_id),returnid);
    free(data);
    return returnid;
}

// Reformat container to resolve references and handle result of setting it
static void importContainer(char *idstr,char *conbuff)
{
    static StuffBuff sb;
    int listid, conid, newconid;
    char *temp, *line;
    if (!sb.buff)
        initStuffBuff(&sb,1000);
    else 
        clearStuffBuff(&sb);

    if (stashFindElement(mappingTable,idstr,NULL))
    {
        // We already did this container
        return;
    }

    temp = strstr(idstr,":");
    *temp = '\0';
    listid = atoi(idstr);
    conid = atoi(temp+1);
    *temp = ':';

    while (line = strsep(&conbuff,"\n\r"))
    {                
        char *firstspace = strstr(line, " ");
        char *ref,left[512];
        int val;        
        if (!firstspace || strncmp(firstspace," CONREF",7) != 0)
        {
            // Doesn't have any references, just put it exactly
            addStringToStuffBuff(&sb,"%s\n",line);
            continue;
        }
        ref = firstspace + 1 + strlen("CONREF");
        strncpyt(left,line,firstspace - line + 1);
        if (stashFindInt(mappingTable,ref,&val))
        {
            // Resolve the reference
            addStringToStuffBuff(&sb,"%s %d\n",left,val);
        }
        else
        {
            // This references doesn't resolve, note for later
            addBroken(idstr,left,ref);
            addStringToStuffBuff(&sb,"%s %d\n",left,0);
        }
    }

    newconid = pushContainer(listid,sb.buff);
    fprintf(progressfile,"Put container: %s\n",idstr);
    fflush(progressfile);

    if (newconid == -1)
    {
        // didn't actually get added
        fprintf(progressfile,"Unable to put container: %s\n",sb.buff);
    }
    else
    {
        addMapping(idstr,newconid);
    }
}

// Does the initial import of the dump file
void importDump(int argc,char **argv)
{
    char basename[1024];
    char filename[1024];
    int suffix = 1;
    static StuffBuff sb;
    int len;
    char *mem;
    initStuffBuff(&sb,1000);

    if (argc == 0)
    {
        printf("Usage: dbserver -importdump dumpfile (mappingfile brokenfile splitnames)\n");
        return;
    }

    if (argv[0][0] == '.' || strstr(argv[0],":")) //do we look like an absolute path?
        strcpy(basename,argv[0]);
    else
        sprintf(basename,"./%s",argv[0]); //to keep filealloc from trying to read it from data

    strcpy(filename,basename);

    if (argc > 1)
        mappingfile = fopen(argv[1],"a+");
    else
        mappingfile = fopen("conids.map","a+");
    if (!mappingfile)
    {
        printf("Unable to load mapping file!\n");
        return;
    }

    if (argc > 2)
        brokenfile = fopen(argv[2],"a+");
    else
        brokenfile = fopen("links.bk","a+");
    if (!brokenfile)
    {
        printf("Unable to load broken links file!\n");
        return;
    }

    if (argc > 3)
        progressfile = fopen(argv[3],"a");
    else
        progressfile = fopen("progress.txt","a");
    if (!progressfile)
    {
        printf("Unable to open progress file for writing!\n");
        return;
    }

    if (argc > 4 && stricmp(argv[4],"splitnames") == 0)
    {
        printf("We will grant a rename token to everyone who shares a name!\n");
        s_splitNames = 1;
    }

    readMappingFile();
    readBrokenFile();

    while (mem = fileAlloc(filename, &len))
    {    
        char *temp = mem;
        char *line;

        while (line = strsep(&temp,"\n\r"))
        {            
            if (!line[0] || strncmp(line, "//",2) == 0)
            {
                // skip comments and blanks
                continue;
            }
            if (strncmp(line,"CONID",5) == 0)
            {
                char conid[512];                
                clearStuffBuff(&sb);                
                strncpyt(conid, line + strlen("CONID"), (int)(strlen(line) - strlen("CONID") + 1));
                while (line = strsep(&temp,"\n"))
                {
                    if (!line[0])
                    {
                        // if it's blank, this is the end of the container
                        break;
                    }
                    if (strncmp(line, "//",2) == 0)
                    {
                        // skip comments and blanks
                        continue;
                    }
                    else
                    {
                        addStringToStuffBuff(&sb,"%s\n",line);
                    }
                }
                importContainer(conid,sb.buff);
            }
        }
        fprintf(progressfile,"\nFinished loading file: %s\n",filename);
        fileFree(mem);
        sprintf(filename,"%s-%d",basename,suffix++);    
    }

    sqlFifoFinish();
    fprintf(progressfile,"Loading dump files complete.\n");

    fflush(mappingfile);    
    fflush(brokenfile);
    fflush(progressfile);    

    fclose(mappingfile);    
    fclose(brokenfile);
    fclose(progressfile);    
    
}

// Revisists DB and fixes any unresolved links
void fixImport(int argc,char **argv)
{
    DbContainer *container;    
    StashTableIterator iter;
    StashElement elem;

    if (argc > 0)
        mappingfile = fopen(argv[0],"a+");
    else
        mappingfile = fopen("conids.map","a+");
    if (!mappingfile)
    {
        printf("Unable to load mapping file!\n");
        return;
    }

    if (argc > 1)
        brokenfile = fopen(argv[1],"a+");
    else
        brokenfile = fopen("links.bk","a+");
    if (!brokenfile)
    {
        printf("Unable to load broken links file!\n");
        return;
    }

    if (argc > 2)
        progressfile = fopen(argv[2],"a");
    else
        progressfile = fopen("progress.txt","a");
    if (!progressfile)
    {
        printf("Unable to open progress file for writing!\n");
        return;
    }

    readMappingFile();
    readBrokenFile();

    stashGetIterator(brokenTable, &iter);

    // Try and resolve all the broken references
    while ( stashGetNextElement(&iter, &elem) )
    {
        int containerid, referenceid, listid;
        const char *key;
        char *val, *temp;
        char idstr[512],namestr[512],liststr[512], command[1024];

        key = stashElementGetStringKey(elem);
        val = stashElementGetPointer(elem);

        temp = strstr(key,".");
        strcpy(namestr,temp + 1);
        strncpyt(idstr,key,temp - key + 1);

        temp = strstr(key,":");
        strncpyt(liststr,key,temp - key + 1);
        listid = atoi(liststr);

        if (!stashFindInt(mappingTable,idstr,&containerid))
        {
            printf("Entry in broken link file %s refers to nonexistent container!\n",key);
            exit(1);
        }
        if (!stashFindInt(mappingTable,val,&referenceid))
        {
            fprintf(progressfile,"Broken link reference is still unresolved!: %s %s\n",key,val);
            continue;
        }
    
        sprintf(command,"%s %d",namestr,referenceid);
        
        fprintf(progressfile,"Fixing link: %d:%d %s\n",listid,containerid,command);

        container = containerLoad(dbListPtr(listid),containerid);
        container = containerUpdate(dbListPtr(listid),containerid,command,1);
        containerUnload(dbListPtr(listid),containerid);
    }    

    fprintf(progressfile,"Done fixing links.\n");

    sqlFifoFinish();

    // For whatever reason, if a base UserID is null, that is handled differently than 0
    if (gDatabaseProvider == DBPROV_SQLITE) {
        sqlExecAsync("UPDATE Base SET UserId = 0 WHERE UserId is NULL;", SQL_NTS);
        sqlFifoFinish();
    } else sqlConnExecDirect("UPDATE Base SET UserId = 0 WHERE UserId is NULL;", SQL_NTS, SQLCONN_FOREGROUND, false);

    fflush(mappingfile);    
    fflush(brokenfile);
    fflush(progressfile);    

    fclose(mappingfile);    
    fclose(brokenfile);
    fclose(progressfile);
}

#define MAX_FILE 100000000

static FILE *dump_file;
static char outname[1024];

// Write the dump to the file, opening a new one if we're past the max size
static void printDump(char *container)
{
    static int writeout = 0;
    static int suffix = 1;
    writeout += fprintf(dump_file,"%s\n",container);

    if (writeout < 0)
    {
        printf("Bad write to file: %s\n", outname);
        exit(1);
    }

    if (writeout > MAX_FILE)
    {
        char tempfile[1024];

        fflush(dump_file);
        fclose(dump_file);
        sprintf(tempfile,"%s-%d",outname,suffix++);
        dump_file = fopen(tempfile,"wb");

        if (!dump_file)
        {
            printf("Bad output file: %s\n", tempfile);
            exit(1);
        }
        writeout = 0;
    }
}

#define PREFETCH 1024

// Outputs a raw container dump, which needs to be processed by a mapserver
void exportRawDump(int argc,char **argv)
{
    static int    *ilist,ilist_max,*loadlist,loadlist_max;
    int        list_id,count,i,j;
    int        lists[] = {CONTAINER_ENTS,CONTAINER_SUPERGROUPS,CONTAINER_BASE,CONTAINER_ITEMSOFPOWER,CONTAINER_SGRPSTATS,0};
    static StuffBuff sb;

    initStuffBuff(&sb,10000);

    if (argc == 0)
    {
        printf("Usage: dbserver -exportdump dumpfile\n");
        return;
    }

    strcpy(outname,argv[0]);
    dump_file = fopen(outname,"wb");

    if (!dump_file)
    {
        printf("Bad output file: %s\n", outname);
        return;
    }

    j = 0;
    while (lists[j]!= 0)
    {
        list_id = lists[j];
        count = allContainerIds(&ilist, &ilist_max, list_id, 0);

        for (i = 0; i < count && i < PREFETCH; i++)
            sqlReadContainerAsync(list_id, ilist[i], NULL, NULL, NULL);

        for (i = 0; i < count; i++)
        {
            DbContainer    *c;
            char        *text;

            if (i+PREFETCH<count)
                sqlReadContainerAsync(list_id, ilist[i+PREFETCH], NULL, NULL, NULL);

            while (sqlIsAsyncReadPending(list_id, ilist[i])) {
                sqlFifoTick();
                Sleep(1);
            }

            c = containerLoad(dbListPtr(list_id),ilist[i]);
            clearStuffBuff(&sb);
            addStringToStuffBuff(&sb,"CONID%d:%d\n",list_id,ilist[i]);
            text = containerGetText(c);
            addSingleStringToStuffBuff(&sb,text);
            printDump(sb.buff);
            containerUnload(dbListPtr(list_id),ilist[i]);
        }
        j++;
    }
    sqlFifoFinish();
    fclose(dump_file);
}
