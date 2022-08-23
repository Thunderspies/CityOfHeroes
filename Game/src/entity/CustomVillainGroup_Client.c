#include "entity/CustomVillainGroup.h"
#include "entity/CustomVillainGroup_Client.h"
#include <utilitieslib/utils/file.h>                            //    for file writing
#include <utilitieslib/utils/sysutil.h>                        //    for getting executable dir
#include <utilitieslib/utils/utils.h>
#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"                //    for client critter things
#include "UI/uiMissionMakerScrollSet.h"        //    for update custom critter list
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>                //    for textStd
#include <utilitieslib/utils/FolderCache.h>
#include "UI/uiCustomVillainGroupWindow.h"

extern MMScrollSet missionMaker;

char * getCustomVillainGroupDir(void)
{
    static char path[MAX_PATH];

    if(!path[0])
    {
        if(isDevelopmentMode())
        {
            char *s;
            strcpy(path, fileDataDir());
            if(s = strrchr(path, '/'))
                *s = '\0';
        }
        else
        {
            getExecutableDir(path);
        }
        strcat(path, "/CustomVillainGroup");
    }
    return path;
}
static void loadCustomVG(CustomVG *cvg)
{
    int i,j;
    char *displayName = NULL;
    //    validate the custom villain group
    if (CVG_isValidCVG(cvg, NULL,0,0) != CVG_LE_VALID)
    {
        return;
    }


    //    if it already existed as a file
    //    remove the old version
    for (j = (eaSize(&g_CustomVillainGroups)-1); j >= 0; --j)
    {
        //    find if it already existed
        if ( g_CustomVillainGroups[j]->sourceFile &&    (stricmp(g_CustomVillainGroups[j]->displayName, textStd("AllCritterListText")) != 0) && 
                                                        (stricmp(g_CustomVillainGroups[j]->sourceFile, cvg->sourceFile) == 0) )
        {
            //    clear out all the standard villains..
            int k;
            for (k = (eaSize(&g_CustomVillainGroups[j]->existingVillainList) -1); k >= 0; --k)
            {
                StructDestroy(parse_ExistingVillain, g_CustomVillainGroups[j]->existingVillainList[k]);
            }
            eaDestroy(&g_CustomVillainGroups[j]->existingVillainList);
            eaDestroy(&g_CustomVillainGroups[j]->customVillainList);
            for ( i = (eaSize(&g_CustomVillainGroups[j]->dontAutoSpawnEVs)-1); i>=0;--i)
            {
                StructFreeString(g_CustomVillainGroups[j]->dontAutoSpawnEVs[i]);
            }
            eaDestroy(&g_CustomVillainGroups[j]->dontAutoSpawnEVs);
            for (i = (eaSize(&g_CustomVillainGroups[j]->dontAutoSpawnPCCs)-1); i>=0; --i)
            {
                StructFreeString(g_CustomVillainGroups[j]->dontAutoSpawnPCCs[i]);
            }
            eaDestroy(&g_CustomVillainGroups[j]->dontAutoSpawnPCCs);
        }
    }

    //    search for the custom villain group
    i = CVG_CVGExists(cvg->displayName);
    //    if it already exists
    if (i == -1)
    {
        CustomVG *newCVG;
        newCVG = StructAllocRaw(sizeof(CustomVG));
        newCVG->displayName = StructAllocString(cvg->displayName);
        i = eaPush(&g_CustomVillainGroups, newCVG);
    }

    if (g_CustomVillainGroups[i]->sourceFile)    StructFreeString(g_CustomVillainGroups[i]->sourceFile);
    if (cvg->sourceFile)                        g_CustomVillainGroups[i]->sourceFile = StructAllocString(cvg->sourceFile);

    for ( j = 0; j < eaSize(&cvg->existingVillainList); ++j )
    {
        CVG_ExistingVillain *newStandardCritter;
        newStandardCritter = StructAllocRaw(sizeof(CVG_ExistingVillain));
        StructCopy(parse_ExistingVillain, cvg->existingVillainList[j], newStandardCritter, 0,0 );
        eaPush(&g_CustomVillainGroups[i]->existingVillainList, newStandardCritter);
    }
    for ( j = 0; j < eaSize(&cvg->customVillainList); ++j )
    {
        PCC_Critter *existingCritter;
        existingCritter = getCustomCritterByReferenceFile(cvg->customVillainList[j]->referenceFile);
        if (existingCritter == NULL)
        {
            char fileName[MAX_PATH];
            char *oldSource;
            PCC_Critter *returnCritter;
            //    otherwise, add the new critter to our critter list
            sprintf(fileName, "%s/%s", getCustomCritterDir(), cvg->customVillainList[j]->referenceFile);
            //    ParserWriteTextFile(fileName, parse_PCC_Critter, cvg->customVillainList[j], 0,0);
            oldSource = cvg->customVillainList[j]->sourceFile;
            cvg->customVillainList[j]->sourceFile = fileName;
            returnCritter = addCustomCritter(cvg->customVillainList[j], NULL);
            cvg->customVillainList[j]->sourceFile = oldSource;
            //    if the critter's villain group isn't this group, add the pointer to it into this group
            if (returnCritter && stricmp(cvg->customVillainList[j]->villainGroup, returnCritter->villainGroup ) != 0)
            {
                eaPush(&g_CustomVillainGroups[i]->customVillainList, returnCritter);
            }
        }
        else
        {
            //    If we already have a critter with that referenceFile, use that
            //    make sure the critter isn't already in this list (don't want to double insert
            int k = -1;
            for (k = 0; k < eaSize(&g_CustomVillainGroups[i]->customVillainList); k++)
            {
                if (stricmp(g_CustomVillainGroups[i]->customVillainList[k]->referenceFile, existingCritter->referenceFile) == 0)
                {
                    break;
                }
            }
            if (!EAINRANGE( k, g_CustomVillainGroups[i]->customVillainList))
                eaPush(&g_CustomVillainGroups[i]->customVillainList, existingCritter);
        }
    }

    for (j = 0; j < eaSize(&cvg->dontAutoSpawnPCCs); j++)
    {
        eaPush(&g_CustomVillainGroups[i]->dontAutoSpawnPCCs, StructAllocString(cvg->dontAutoSpawnPCCs[j]));
    }
    for (j = 0; j < eaSize(&cvg->dontAutoSpawnEVs); j++)
    {
        eaPush(&g_CustomVillainGroups[i]->dontAutoSpawnEVs, StructAllocString(cvg->dontAutoSpawnEVs[j]));
    }

}
static FileScanAction  processCVG(char *dir, struct _finddata32_t *data)
{
    CustomVG *tempVG;
    char filename[MAX_PATH];

    sprintf(filename, "%s/%s", dir, data->name);

    if(strEndsWith(filename, ".cvg" ) && fileExists(filename)) // an actual file, not a directory
    {
        tempVG = StructAllocRaw(sizeof(CustomVG));
        if (ParserLoadFiles( NULL, filename, NULL, 0, parse_CustomVG, tempVG, 0, 0, NULL))
        {
            loadCustomVG(tempVG);
        }
        StructDestroy(parse_CustomVG, tempVG);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

void loadCVGFromFiles()
{
    fileScanDirRecurseEx(getCustomVillainGroupDir(), processCVG);
}

int CVG_removeCustomVillainGroup(char *displayName, int removeSource)
{
    int i;
    int removed = 0;
    CustomVG *allList = NULL;
    if (stricmp(displayName, textStd("AllCritterListText")) == 0)
    {
        return 0;
    }

    //    loop through the list to find the CVG
    for (i = (eaSize(&g_CustomVillainGroups)-1); i >= 0 ; --i)
    {
        //    found it
        if (stricmp(displayName, g_CustomVillainGroups[i]->displayName) == 0)
        {
            int j;
            CustomVG *cvgToRemove;
            cvgToRemove = g_CustomVillainGroups[i];
    
            //    what to do w/ the critters?
            for (j = (eaSize(&cvgToRemove->customVillainList)-1); j >= 0; --j)
            {
                PCC_Critter *pCritter;
                pCritter = cvgToRemove->customVillainList[j];
                //    remove the pointer
                eaRemove(&cvgToRemove->customVillainList, j);

                //    if they belong to another group, that's fine.
                if (stricmp(pCritter->villainGroup, displayName) == 0)
                {
                    int k;
                    if (!allList)
                    {
                        for ( k = 0; k < eaSize(&g_CustomVillainGroups); ++k)
                        {
                            if (stricmp(g_CustomVillainGroups[k]->displayName, textStd("AllCritterListText")) == 0)
                            {
                                allList = g_CustomVillainGroups[k];
                                break;
                            }
                        }
                    }
                    //    this really shouldn't happen. The "all list" should be added any time a critter is added so if there is a critter, there should be an "all list"
                    //    but just in case, don't crash.
                    if (!allList)
                    {
                        allList = StructAllocRaw(sizeof(CustomVG));
                        allList->displayName = StructAllocString(textStd("AllCritterListText"));
                        eaPush(&g_CustomVillainGroups, allList);
                    }

                    for (k = (eaSize(&allList->customVillainList)-1); k >= 0; k--)
                    {
                        if (stricmp(allList->customVillainList[k]->referenceFile, pCritter->referenceFile) == 0)
                        {
                            eaRemove(&allList->customVillainList, k);
                        }
                    }
                    //    if they belong to this group, put them in the "All Critter List"
                    StructFreeString(pCritter->villainGroup);
                    pCritter->villainGroup = StructAllocString(textStd("AllCritterListText"));
                    ParserWriteTextFile(pCritter->sourceFile, parse_PCC_Critter, pCritter, 0,0);
                    eaPush(&allList->customVillainList, pCritter);
                }
            }
            eaDestroy(&cvgToRemove->customVillainList);
            //    delete the correct one
            if (cvgToRemove->sourceFile && removeSource)
            {
                fileMoveToRecycleBin(cvgToRemove->sourceFile);
            }
            StructDestroy(parse_CustomVG, cvgToRemove);
            eaRemove(&g_CustomVillainGroups, i);
            removed = 1;
            break;
        }
    }
    return removed;
}

static void reloadCVGFiles( const char *relPath, int reloadWhen)
{
    loadCVGFromFiles();
    updateCustomCritterList(&missionMaker);
    populateCVGSS();
}
void initCVGFolderCache()
{
    char reloadpath[MAX_PATH];
    FolderCache *fc;

    sprintf(reloadpath, "%s/", getCustomVillainGroupDir());
    mkdirtree(reloadpath);

    fc = FolderCacheCreate();
    FolderCacheAddFolder(fc, getCustomVillainGroupDir(), 0);
    FolderCacheQuery(fc, NULL);
    sprintf(reloadpath, "*%s", ".cvg");
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, reloadpath, reloadCVGFiles);
}


int CVG_removeAllEmptyCVG(CustomVG **vgList)
{
    int i;
    int groupRemoved = 0;
    for (i = (eaSize(&vgList)-1); i >= 0; i--)
    {
        if (    (eaSize(&vgList[i]->customVillainList) == 0) &&
            (eaSize(&vgList[i]->existingVillainList) == 0) && !vgList[i]->reinsertRemovedCritter)
        {
            CVG_removeCustomVillainGroup(g_CustomVillainGroups[i]->displayName,1);
            groupRemoved = 1;
        }
    }
    return groupRemoved;
}