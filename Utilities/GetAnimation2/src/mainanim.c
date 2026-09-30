#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <utilitieslib/stdtypes.h>

#ifdef _WIN32
#include <utilitieslib/utils/wininclude.h>
#endif

#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include "outputanim.h"
#include <utilitieslib/utils/file.h>
#include "processanim.h"
#include "process_animx.h"
#include "process_skelx.h"
#include "seq/animtrack.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/FileSystem.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/genericDialog.h>

#define kVersionString "2.07"

// Status Codes and human readable messages
typedef enum Reasons {
    REASON_PROCESSED,
    REASON_NOTYOURS,
    REASON_NOTNEWER,
    REASON_CHECKOUTFAILED,
    REASON_INVALID_SRC_DIR,
    REASON_BAD_DATA,
    REASON_INVALID_VERSION,

    REASON_MAX
} Reasons;

const char* g_ReasonStrings[] = {
    "Reason: processed normally",                            // REASON_PROCESSED
    "Reason: Perforce thinks it's not yours",        // REASON_NOTYOURS
    "Reason: File is not newer",                            // REASON_NOTNEWER
    "Reason: Checkout failed",                                // REASON_CHECKOUTFAILED
    "Reason: Invalid source directory",                // REASON_INVALID_SRC_DIR
    "Reason: Invalid source data",                        // REASON_BAD_DATA
    "Reason: Invalid source version",                    // REASON_INVALID_VERSION
};

GlobalState global_state;

int g_no_version_control;
bool g_force_rebuild = false;

static char    **names=0;

char **fileGetDirsRecurse(char *dir,int *count_ptr)
{
    FileListing *listing;

    char    buf[1200];

    fileLocateWrite(dir, buf);

    listing = fileSystemListNative(buf);
	for (size_t entry_index = 0; listing && entry_index < listing->count; entry_index++)
	{
		const FileSystemEntry *entry = &listing->entries[entry_index];
        if (entry->name[0] == '.' || entry->name[0] == '_')
            continue;
        sprintf(buf,"%s/%s",dir,entry->name);
        if (entry->kind == PG_ENTRY_DIRECTORY)
        {
            names = realloc(names,(*count_ptr+1) * sizeof(names[0]));
            names[*count_ptr] = malloc(strlen(buf)+1);
            strcpy(names[*count_ptr],buf);
            (*count_ptr)++;
            fileGetDirsRecurse(buf,count_ptr);
        }
        
    }
    fileListingFree(&listing);
    return names;
}

/*given a directory, it returns an string array of the full path to each of the files in that directory,
and it fills count_ptr with the number of files found.  files and sub-folders prefixed with "_" or "." 
are ignored.  Note: Jonathan's fileLocate is used, so if dir is absolute "C:\..", it's used unchanged, but 
if dir is relative, it will use the gamedir thing.  
*/
char **fileGetDirs(char *dir,int *count_ptr)
{
    names=0;
    *count_ptr = 0;
    return fileGetDirsRecurse(dir,count_ptr);
}

//############# END GET FLAT LIST OF ALL FILES IN DIRECTORY ##############################

/******************************************************************************
    Convert an animation source path into a truncated animation name that
    will be used by the runtime.

    For example, "/male/ready2" would be the animation name
    placed within the .anim the file:
        data/player_library/animations/male/ready2.anim 
    that came from:
        src/player_library/animations/male/models/ready2.ANIMX

    There are several constraints on where source files can be located in the
    folder hierarchy and the need for a good model name parent folder.
    We test those here and notify user/caller if there is a problem.
*****************************************************************************/
static bool convertSourceFilePathToAnimName( char * sourceFilePath, char * animName )
{
    char buffer[1000];
    char * s, * sb, * sf;
    char * folderName;
    char * modelName;

    strcpy(buffer, sourceFilePath);
    //Get modelName
    s = strstri( buffer, "models" );
    if (!s)        
    {
        printf("ERROR: '%s' is not contained in a 'models' folder\n", sourceFilePath );
        return false;
    }

    modelName = s+7;
    if (!modelName || modelName[0] == 0)        
    {
        printf("ERROR: '%s' could not determine model name\n", sourceFilePath );
        return false;
    }
    s = strrchr(modelName, '.');
    if (!s)        
    {
        printf("ERROR: '%s' must have an extension\n", sourceFilePath );
        return false;
    }
    *s = 0;

    //Get folderName
    s = strstri( buffer, "models" );
    s-- ;
    if (*s != '\\' && *s != '/')        
    {
        printf("ERROR: '%s' must have an parent of the 'models' folder\n", sourceFilePath );
        return false;
    }
    *s = 0;
    sb = strrchr(buffer, '\\');
    sf = strrchr(buffer, '/');
    folderName = MAX( sf, sb );
    folderName++;
    if (strstri( folderName, "player_library" ) || strstri( folderName, "animations" ))        
    {
        printf("ERROR: '%s' must not have 'player_library' or 'animations' as direct parents of the 'models' folder, i.e. no model 'name' folder\n", sourceFilePath );
        return false;
    }

    if ( (strlen(folderName) + strlen(modelName) + 2 ) >= MAX_ANIM_FILE_NAME_LEN)        
    {
        printf("ERROR: '%s' has an animation name that is too long to fit in the output data.\n", sourceFilePath );
        return false;
    }

    //Concatenate
    sprintf( animName, "%s/%s", folderName, modelName );

    return true;
}

// "/male/ready2" -> "<game_data>/player_library/animations/male/ready2.anim"
static void convertAnimNameToTargetFilePath( char * animName, char * targetFilePath )
{
    sprintf( targetFilePath, "%s\\player_library\\animations\\%s.anim", fileDataDir(), animName );
    backSlashes(targetFilePath);
}

// "src/player_library/animations/male/models/ready2.ANIMX" -> "<game_data>/player_library/animations/male/ready2.anim"
static bool convertSourceFilePathToTargetFilePath( char * sourceFilePath, char * targetFilePath )
{
    char buffer[1000];

    if ( !convertSourceFilePathToAnimName( sourceFilePath, buffer ) )
        return false;
    convertAnimNameToTargetFilePath( buffer, targetFilePath );
    return true;
}


/******************************************************************************
  Extensive Perforce validation of files being used. Will checkout or add
    the destination if proper.
    Assumes that the source .ANIMX also become controlled source assets.
    When that happens we can consider turning this on and testing it.

    TODO: mirrors code used by other tools but probably needs some work
    when Perforce is not available
*****************************************************************************/
static Reasons checkIfFileShouldBeProcessed(const char* srcFile, const char* dstFile)
{
#if 1
    // When source .ANIMX are controlled by Perforce we can consider turning
    // the code below and testing it.
    return REASON_PROCESSED;
#else
    if  (g_force_rebuild || fileNewer(dstFile,srcFile) )
    {
        int ret;
        char srcFileCopy[MAX_PATH];

        /* rules for processing:
        When GetAnimation sees a .ANIMX file that is newer than its corresponding .anim file, it sees that this
        file needs to be processed.  Previously it would then just check out the .anim file and process it.
        Now, it will NOT process the file if the .ANIMX is checked out by someone else.  If no one has the
        file checked out, then it will only process it if you were the last person to check it in.  There
        should no longer be any issues with people getting their .anim files checked out by someone else.

        The correct procedure to things is :
        1. Make your changes (Check out .ANIMX file)
        2. Process the geometry (run GetAnimatino)
        3. TEST (Run the game)
        4. Check-in (or check-point) your files so other people can get them
        */

        const char *lastauthor = perforceQueryLastAuthor(srcFile);

        // We're going to cancel if the ANIMX locked and it's locked by someone other than me
        //   or it's not locked and I'm not the last author of the ANIMX
        if (!g_force_rebuild && !perforceQueryIsFileMine(srcFile))
        {
            printf("\n");
            consoleSetFGColor(COLOR_RED|COLOR_BRIGHT);
            printf("Warning: target .anim file is older than source .ANIMX file, BUT\n");
            printf("you do not have the .ANIMX file checked out. So its processing is being skipped.\n");
            if (stricmp(lastauthor, "You do not have the latest version")==0) {
                printf("You do not have the latest version of the .ANIMX file.\n");
            } else {
                printf("'%s' may have forgot to process it before checking it in.\n", lastauthor);
            }
            consoleSetFGColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE);

            return REASON_NOTYOURS;
        }

        printf("\n");
        strcpy(srcFileCopy, srcFile);
        mkdirtree(srcFileCopy);

        if (!g_no_version_control) {
            if (!perforceQueryIsFileLockedByMeOrNew(dstFile))
            {
                perforceSyncForce(dstFile, PERFORCE_PATH_FILE);
                ret=perforceEdit(dstFile, PERFORCE_PATH_FILE);
            }
            else
                ret = NO_ERROR;
        } else {
            _chmod(dstFile, _S_IWRITE | _S_IREAD);
            ret = NO_ERROR;
        }


        if (ret!=NO_ERROR && ret!=PERFORCE_ERROR_NOT_IN_DB && ret!=PERFORCE_ERROR_NO_SC && ret!=PERFORCE_ERROR_NO_DLL)
        {
            Errorf("Can't checkout %s. (%s)\n",dstFile,perforceOfflineGetErrorString(ret));
            if (strstriConst(perforceOfflineGetErrorString(ret), "already deleted")) {
                printfColor(COLOR_RED|COLOR_BRIGHT, "WARNING: \"%s\" has been previously marked as deleted,\n   you will need to manually re-add this file with Perforce\n", dstFile);
            } else {
                // Because of the checks above, if we get here, this file is one that should be processed!
                if (!g_force_rebuild)
                    FatalErrorf(".ANIMX file is owned by you, but .anim file is checked out by someone else!\n%s (%s)\n%s (%s)\n", srcFile, perforceQueryLastAuthor(srcFile), dstFile, perforceQueryIsFileLocked(dstFile)?perforceQueryIsFileLocked(dstFile):perforceQueryLastAuthor(dstFile));
                return REASON_CHECKOUTFAILED;
            }
        }

        if (!g_no_version_control)
        {
            if (perforceQueryIsFileBlocked(dstFile))
                perforceUnblockFile(dstFile);
        }

        // remove .bak file of the .anim, it may have inadvertently been created from the step
        // above that created an empty file so that it could be checked out
        {
            char bakname[MAX_PATH];
            strcpy(bakname, dstFile);
            strcat(bakname, ".bak");
            fileForceRemove(bakname);
        }

        return REASON_PROCESSED;
    } else {
        return REASON_NOTNEWER;
    }
#endif
}

static int findBaseSkeletonsIndex( char ** srcnames, int src_cnt )
{
    int i, skeleton_idx;
    skeleton_idx = -1;
    for( i = 0 ; i < src_cnt ; i++ )
    {
        if(strstri(srcnames[i], ".skelx") && !strstri(srcnames[i], ".skelx."))
        {
            if( strstri( srcnames[i], "skel_" )    ) 
            {
                if( skeleton_idx == -1 )
                    skeleton_idx = i;
                else
                {
                    printf( "WARNING: More than one skeleton .skelx found. NOT using this one: '%s'\n", srcnames[i]);
                    skeleton_idx = -2;    // error!
                }
            }
        }
    }
    return skeleton_idx;
}

static int findLegacyBaseSkeletonsIndex( char ** srcnames, int src_cnt )
{
    int i, skeleton_idx;
    skeleton_idx = -1;
    for( i = 0 ; i < src_cnt ; i++ )
    {
        if(strstri(srcnames[i], ".WRL") && !strstri(srcnames[i], ".WRL."))
        {
            if( strstri( srcnames[i], "skel_" )    ) 
            {
                if( skeleton_idx == -1 )
                    skeleton_idx = i;
                else
                {
                    printf( "WARNING: More than one skeleton .WRL found. NOT using this one: '%s'\n", srcnames[i]);
                    skeleton_idx = -2;    // error!
                }
            }
        }
    }
    return skeleton_idx;
}

static void processAnimationANIMX( char * sourcepath, char * targetpath, char * baseSkeletonName, Node * skeletonRoot )
{
  char animName[256];
  SkeletonAnimTrack * pNewAnimTrack;

  printf( "Processing file: %s\n", sourcepath ); 

    // Generate name of this anim
    if ( !convertSourceFilePathToAnimName( sourcepath, animName ) )
        return; // source path has a problem

    pNewAnimTrack = calloc( 1, sizeof( SkeletonAnimTrack ) );

  outputResetVars(); 

  strcpy( pNewAnimTrack->name, animName );
  strcpy( pNewAnimTrack->baseAnimName, baseSkeletonName );

  if ( animConvert_ANIMX_To_AnimTrack( pNewAnimTrack, sourcepath, skeletonRoot ) )
  {
        SkeletonHeirarchy* pGameSkeletonHeirarchy = NULL;

    printf( " Writing output: \"%s\"\n", targetpath );

        // The in-game runtime skeleton hierarchy only needs to be placed in the base animation
        // file (skel_*.anim). So if we are processing the animation corresponding to this file
        // go ahead and generate the runtime hierarchy and place it in the file
        if ( stricmp( animName, baseSkeletonName ) == 0 )
        {
            printf( " Generating runtime skeleton hierarchy to install in base animation\n" ); 

            pGameSkeletonHeirarchy = calloc( 1, sizeof( SkeletonHeirarchy ) );
            if( pGameSkeletonHeirarchy )
            {
                memset( pGameSkeletonHeirarchy, 0, sizeof( SkeletonHeirarchy ) ); //BUTODO is 1 the right number?
                pGameSkeletonHeirarchy->heirarchy_root = skeletonRoot->anim_id;
                getSkeleton( skeletonRoot, pGameSkeletonHeirarchy );

                pNewAnimTrack->skeletonHeirarchy = pGameSkeletonHeirarchy;
            }
            else
            {
                Errorf("ERROR: Could not generate runtime skeleton hierarchy for: %s", targetpath);
            }
        }

    //write Anim Track data blocks to the binary
    // IMPORTANT NOTE: This obliterates the pointers in the AnimTrack as part of
    // the steps for creating the output data (replaced with offsets)
    // so you can't use the data anymore.
    // TODO: fix this, want to be able to clean up the track
    outputAnimTrackToAnimFile( pNewAnimTrack, targetpath );

        if( pGameSkeletonHeirarchy )
        {
            free(pGameSkeletonHeirarchy);    // clean up skeleton hier if we created it
        }
  }

  free( pNewAnimTrack );
}

/******************************************************************************
 Folder Process:
 When processing a folder of animations we can process the skeleton file once
 and use that during the processing of any animations that need attention.

  * Find the skeleton export file (.SKELX) and load it
    This file contains:
      - the skeleton hierarchy used for any animations in this folder
      - the base pose bone reference coordinate frames
  * Loop over all the loose '.ANIMX' files that need to be reprocessed. This step
    uses the skeleton node tree created from the .SKELX export.
*****************************************************************************/
static Reasons processAnimationFolder( char * sourcefolder, int force_rebuild )
{
    char ** src_names;
    int     src_cnt;
    int        i;
    int        update_anim;
    int        skeleton_idx;
    char * sourceFilePath;
    char targetFilePath[256];
    char skelAnimName[256];
    int        writeBaseSkel = 1;
  Node* skeletonTree = NULL;

    //####################
    //## Get list of full paths of all animation source data files anywhere in this folder or subfolders
    printf("Scanning folder for source animation files...");
    src_names = fileScanDir( sourcefolder, &src_cnt );
    printf("found %d files\n", src_cnt); 

    //####################
    //### Find the base skeleton, open and process.
    skeleton_idx = findBaseSkeletonsIndex( src_names, src_cnt ); 
  if (skeleton_idx == -1)
  {
    Errorf( "ERROR: No base skeleton ('skel_*.skelx') in this folder. Skipping.");
    return REASON_INVALID_SRC_DIR;
  }
    else if (skeleton_idx == -2)
    {
        Errorf( "ERROR: Too many ('skel_*.skelx') in this folder. There can only be one. Skipping.");
        return REASON_INVALID_SRC_DIR;
    }
    else
    {
        printf( "Using skeleton: '%s'\n", src_names[skeleton_idx]);
    }

    // Generate name of the skeleton '.anim' and validate source path
    if ( !convertSourceFilePathToAnimName( src_names[skeleton_idx], skelAnimName ) )
        return REASON_INVALID_SRC_DIR; // source path has a problem

    // Check skeleton export against the runtime .anim file for the base animation (skel_*anim).
    // This is the file that ends up containing the skeleton hierarchy for the runtime
    // that the other animations which reference this base animation rely upon.
    // If the skel_*.anim is older than the skeleton export then we can probably assume
    // that all the animations in the folder need reprocessing and we force that.
    convertAnimNameToTargetFilePath(skelAnimName, targetFilePath);
    if( fileNewer( targetFilePath, src_names[skeleton_idx] ) )
    {
        printf( "WARNING: Master skeleton (.SKELX) appears to have changed. You may need to force all animations that rely on this skeleton to be reprocessed.\n" );
        // @todo: When all animations have source data in the new .ANIMX format it might make sense to force the reprocess ourselves
        //        force_rebuild = 1;
    }

  // Load the skeleton hierarchy export file used for this folder of animations
    skeletonTree = LoadSkeletonSKELX( src_names[skeleton_idx] );
    if (!skeletonTree)
    {
        Errorf( "ERROR: skeleton file in this folder could not be processed: %s", src_names[skeleton_idx] );
        return REASON_INVALID_SRC_DIR;
    }

    //####################
    //#### Then process each independent animation source data file that uses that skeleton
  if (src_cnt>1) printf("\n");

    for( i = 0 ; i < src_cnt ; i++ )
    {
        sourceFilePath = src_names[i];
        //ignore non animation files and the base
        if(strstri(sourceFilePath, ".animx") && !strstri(sourceFilePath, ".animx.")) 
        {
            if ( !convertSourceFilePathToTargetFilePath( sourceFilePath, targetFilePath ) )
                continue;    // problem with this source path, so skip it

            //Decide if this animation file needs updating
            update_anim = force_rebuild;
            if( !update_anim )
                update_anim |= fileNewer( targetFilePath, sourceFilePath );

            //If you need to, process it
            if( update_anim )
                processAnimationANIMX( sourceFilePath, targetFilePath, skelAnimName, skeletonTree );
        }
    }

  // free the skeleton tree now that we are done processing the folder of files that should
  // be referencing it.
  treeFree();

    return REASON_PROCESSED;
}

/******************************************************************************
    Single File Process:

    This routine is used to reprocess a single ANIMX file export and is generally
    going to be called while monitoring.
        * Find the skeleton export file for anim and load it to a skeleton node tree
        * Reprocess the requested animation using the skeleton node tree

    Unfortunately since we don't know the name of the skeleton file we need to
    search for it so we essentially do the same processing as the folder
    processing but only process a single ANIMX.

    NOTE: Skeleton file updates should be processed 'manually' and cannot be monitored.
    Update them by explicitly running the tool to process the folder of animations so 
    that the skeleton update will be applied to all animations that use that skeleton
*****************************************************************************/
static Reasons processAnimationSingle( char * sourceFilePath )
{
    char ** src_names;
    int     src_cnt;
    int        skeleton_idx;
    char source_folder[256];
    char targetFilePath[256];
    char skelAnimName[256];
    Node* skeletonTree = NULL;
    Reasons    status;

    // Generate name of the target '.anim' and validate source path
    if ( !convertSourceFilePathToTargetFilePath( sourceFilePath, targetFilePath ) )
        return REASON_INVALID_SRC_DIR; // source path has a problem

    // Decide if this animation file needs updating and can be updated
    status = checkIfFileShouldBeProcessed( sourceFilePath, targetFilePath );
    if (status != REASON_PROCESSED )
    {
        return status;
    }

    // the skeleton should reside in the same folder as the animation
    // we are going to have to go searching for it...
    strncpy_s( source_folder, 256, sourceFilePath, _TRUNCATE );
    {
        // strip file name
        char* sb = strrchr(source_folder, '\\');
        char* sf = strrchr(source_folder, '/');
        char* s = MAX( sf, sb );
        if (s)
        {
            *s = 0;
        }
    }
    src_names = fileScanDir( source_folder, &src_cnt );

    // Find the base skeleton
    skeleton_idx = findBaseSkeletonsIndex( src_names, src_cnt ); 
    if (skeleton_idx == -1)
    {
        Errorf( "ERROR: No base skeleton ('skel_*.SKELX') in this folder. Animation cannot be processed.");
        return REASON_INVALID_SRC_DIR;
    }
    else if (skeleton_idx == -2)
    {
        Errorf( "ERROR: Too many ('skel_*.skelx') in this folder. There can only be one. Skipping.");
        return REASON_INVALID_SRC_DIR;
    }
    else
    {
        printf( "Using skeleton: '%s'\n", src_names[skeleton_idx]);
    }

    // Generate name of the skeleton '.anim' and validate source path
    if ( !convertSourceFilePathToAnimName( src_names[skeleton_idx], skelAnimName ) )
        return REASON_INVALID_SRC_DIR; // source path has a problem

    // Load the skeleton hierarchy export file used for this folder of animations
    skeletonTree = LoadSkeletonSKELX( src_names[skeleton_idx] );
    if (!skeletonTree)
    {
        Errorf( "ERROR: skeleton file in this folder could not be processed: %s", src_names[skeleton_idx] );
        return REASON_INVALID_SRC_DIR;
    }

    // Now process the animation that needs refreshing
    processAnimationANIMX( sourceFilePath, targetFilePath, skelAnimName, skeletonTree );

    treeFree();    // free the skeleton tree

    return status;
}

/******************************************************************************
    Root Scan And Processing:

    This routine starts at a root path and visits all of the subfolders looking
    for animations to process. Generally if a source animation file is found to be
    newer than the in-game processed data it will be regenerated. However, there
    are command line flags that will force all animations to be reprocessed
    regardless of age.

    Calls processAnimationFolder on every ".../models" folder it finds beneath the
    supplied root scan path (recursively)

  thus, passing:
    C:\game\src\player_library\animations [-f] (will reprocess entire library)
*****************************************************************************/
static Reasons scanAndProcessRootPath( char * rootPath ) 
{
    char source_folder[1000];
    char ** foldernames;
    int    foldercount;
    char *s, *sb, *sf;
    int i, is_models_folder;
    Reasons    status = REASON_PROCESSED;

    consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("Scanning and processing root path: '%s'\n", rootPath );
    consoleSetDefaultColor();
    printf("##########################################################################\n" );

    foldernames = fileGetDirs( rootPath, &foldercount ); //Get a list of all the folders in the hierarchy
    for(i = 0 ; i < foldercount ; i++)
    {
        strcpy(source_folder, foldernames[i]);
        //is this a models folder?
        sb = strrchr(source_folder, '\\');
        sf = strrchr(source_folder, '/');
        s = MAX( sf, sb );

        if( s && strnicmp("models", s+1, 6) == 0 )
            is_models_folder = 1;
        else
            is_models_folder = 0;

        //If yes
        if( is_models_folder )
        {
            printf( "Processing folder... \"%s\"\n", source_folder );
            processAnimationFolder( source_folder, g_force_rebuild );
            printf("##########################################################################\n" );
        }
    }
    return status;
}

//look for an argument in command line params
static int checkForArg(int argc, char **argv, char * str)
{
    int i;
    for(i = 0 ; i < argc ; i++)
    {
        if (strstr(argv[i], str))
            return 1;
    }
    return 0;
}

static int tool__lock_handle=0;
static char *lockfilename = "c:\\getanimation2.lock";
static void releaseGetAnimLock() {
    _close(tool__lock_handle);
    fileForceRemove(lockfilename);
    tool__lock_handle = 0;
}

void moveFromCRoot(char* path);
static void waitForGetAnimLock() {
    char realname[MAX_PATH];
    strcpy_s(realname, MAX_PATH, lockfilename);
    moveFromCRoot(realname);
    fileForceRemove(lockfilename);
    while ((tool__lock_handle = _open(realname, _O_CREAT | _O_EXCL | _O_WRONLY)) <= 0) {
        Sleep(500);
        fileForceRemove(lockfilename);
    }
    if (tool__lock_handle > 0) {
        _write(tool__lock_handle, "GetAnimation2.exe\r\n", 12);
    }
}

static void print_file_error( char const* path, Reasons error )
{
    printf("\nDidn't reprocess '%s'\n", path);
    if ( error >= 0 && error < REASON_MAX )
    {
        printf("%s\n",g_ReasonStrings[error]);
    }
    else
        printf("    unknown reason: %d \n", error );
}

static void reprocessAnim(char *fullpath, int print_errors)
{
    static char lastpath[MAX_PATH];
    static U32 lasttime=0;
    Reasons reason = REASON_NOTNEWER;

    fileWaitForExclusiveAccess(fullpath);

    waitForGetAnimLock();
    reason = processAnimationSingle( fullpath );
    releaseGetAnimLock();

    if (reason == REASON_NOTNEWER && stricmp(lastpath, fullpath)==0 && timerCpuSeconds() - lasttime  < 20) {
        // print nothing
    } else if (reason != REASON_PROCESSED && print_errors) {
        print_file_error(fullpath, reason);
    }

    strcpy(lastpath, fullpath);
    lasttime = timerCpuSeconds();

    printf("\r%-200c\r", ' ');
}

char basePath[MAX_PATH];

static void reprocessOnTheFly(const char *relpath, int when)
{
    char fullpath[MAX_PATH];

    printf("\n");
    printf("--------------------------------------------------------------------------\n" );
    consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("File '%s' has been updated...\n", relpath);
    consoleSetDefaultColor();

    if (strstr(relpath, "/_"))
    {
        printf("File '%s' begins with an underscore, not processing.\n", relpath);
        return;
    }

    sprintf(fullpath, "%s%s", basePath, relpath);
    reprocessAnim(fullpath, 1);
    printf("\ndone.\n");
    printf("--------------------------------------------------------------------------\n" );
}

static void reprocessOnTheFlyFileChanged(const FileChange *change, void *user)
{
	(void)user;
	reprocessOnTheFly(change->entry.path, change->kind);
}

static void reprocessSkeletonOnTheFly(const char *relpath, int when)
{
    char fullpath[MAX_PATH];
    char source_folder[MAX_PATH];

    printf("\n");
    printf("--------------------------------------------------------------------------\n" );
    consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("File '%s' has been updated...\n", relpath);
    consoleSetDefaultColor();

    if (strstr(relpath, "/_"))
    {
        printf("File '%s' begins with an underscore, not processing.\n", relpath);
        return;
    }

    sprintf(fullpath, "%s%s", basePath, relpath);

    // the skeleton resides in the same folder as all the animations
    // that depend on it
    strncpy_s( source_folder, 256, fullpath, _TRUNCATE );
    {
        // strip file name
        char* sb = strrchr(source_folder, '\\');
        char* sf = strrchr(source_folder, '/');
        char* s = MAX( sf, sb );
        if (s)
        {
            *s = 0;
        }
    }

    printf( "Reprocessing all dependent animations in folder... \"%s\"\n", source_folder );
    processAnimationFolder( source_folder, true );
    printf("\ndone.\n");
    printf("--------------------------------------------------------------------------\n" );
}

static void reprocessSkeletonOnTheFlyFileChanged(const FileChange *change, void *user)
{
	(void)user;
	reprocessSkeletonOnTheFly(change->entry.path, change->kind);
}

void getAnimMonitor(char *folder)
{
    FileSystem *fc_anim_lib = fileSystemCreate();
    char path[MAX_PATH];

    //folder_cache_debug = 2;

    //fileSystemSetMode(FILE_MODE_HYBRID);
    strcpy(path, folder);
    forwardSlashes(path);
    if (!strEndsWith(path, "/"))
        strcat(path, "/");
    strcpy(basePath,path);
    loadstart_printf("\nCaching %s...", path);
    fileSystemAddSource(fc_anim_lib, path, 0);
    fileSystemDispatch();
    loadend_printf("");

    consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("Now monitoring for changes in .ANIMX and .SKELX files.\n");
    consoleSetDefaultColor();

    fileSystemSubscribe(NULL, "*.animx", FILE_CHANGE_UPDATE, reprocessOnTheFlyFileChanged, NULL);
    fileSystemSubscribe(NULL, "*.skelx", FILE_CHANGE_UPDATE, reprocessSkeletonOnTheFlyFileChanged, NULL);
    while(true)
    {
        fileSystemDispatch();
        Sleep(200);
    }
}

/******************************************************************************
    Walk through the animation folders rooted at the supplied directory
    and for any legacy .WRL animation assets lookup the corresponding runtime .anim
    and use that to make source assets for the new pipeline, i.e., .ANIMX and .SKELX

    TODO: Currently only .SKEL files are generated, still need to finish .ANIMX
*****************************************************************************/

static Reasons BatchConvertLegacySourceFiles( char* sourcefolder )
{
    char ** src_names;
    int     src_cnt, i;

    //####################
    //## Get list of full paths of all animation source data files anywhere in this folder or subfolders
    printf("Scanning folder for source animation files...");
    src_names = fileScanDir( sourcefolder, &src_cnt );
    printf("found %d files\n", src_cnt); 

    if (src_cnt>1) printf("\n");

    for( i = 0 ; i < src_cnt ; i++ )
    {
        char* sourceFilePath = src_names[i];
        char targetFilePath[256];

        // TODO, currently we only convert .WRL associated .anim to a .SKELX, need to do all for .ANIMX
        if(strstri(sourceFilePath, ".WRL") && !strstri(sourceFilePath, ".WRL.") && strstri( sourceFilePath, "skel_" ) ) 
        {
            if ( !convertSourceFilePathToTargetFilePath( sourceFilePath, targetFilePath ) )
                continue;    // problem with this source path, so skip it

            outputAnimFileToSourceAssets( targetFilePath, sourceFilePath );
        }
    }

    return REASON_PROCESSED;
}


static void BatchConvertLegacySourceFolders( char* startPath )
{
    // Calls processAnimationFolder on every ".../models" folder it finds
    // beneath the current working directory (recursively)
    // thus, cwd = C:\game\src\player_library\animations [-f] (will reprocess entire library)
    printf("##########################################################################\n" );
    {
        char source_folder[1000];
        char ** foldernames;
        int    foldercount;
        int i;

        foldernames = fileGetDirs( startPath, &foldercount ); //Get a list of all the folders in the hierarchy
        for(i = 0 ; i < foldercount ; i++)
        {
            char *s, *sb, *sf;
            strcpy(source_folder, foldernames[i]);
            //is this a models folder?
            sb = strrchr(source_folder, '\\');
            sf = strrchr(source_folder, '/');
            s = MAX( sf, sb );

            if( s && strnicmp("models", s+1, 6) == 0 )
            {
                printf( "Batch converting folder... \"%s\"\n", source_folder );
                BatchConvertLegacySourceFiles( source_folder );
                printf("##########################################################################\n" );
            }
        }
    }
}

static void usage(void)
{
    printf(
        "This tool converts exported animation data (.SKELX, .ANIMX) into game ready animation files (.anim).\n"
        "Game ready animation assets are placed here: <game data dir>\\player_library\\animations\n"
        "There are two modes of operation: explicit folder processing and folder monitoring.\n"
        "\n"
        "In explicit folder processing the tool will start at the current working directory (CWD)\n"
        "and process all the newer animation files it finds. If a newer skeleton .SKELX file is\n"
        "found this will force reprocessing of any animations that rely on the changed skeleton.\n"
        "This mode is typically run without any command line arguments.\n"
        "\n"
        "In monitor mode the supplied animation folder will be monitored for updates and any changes\n"
        "to the exported .ANIMX files will cause the corresponding .anim file to be regenerated.\n"
        "\n"
        "If a skeleton export file (.SKELX) is updated then all the animations in the skeleton's\n"
        "folder are reprocessed as they all rely on the skeleton information.\n"
        "\n"
        "The '-prescan' flag can be used in conjunction with '-monitor' mode.\n"
        "When present monitor mode will scan through the target hierarchy when it first starts and.\n"
        "convert any animations for which it finds new exports that have not been updated. (e.g.,\n"
        "animations exported when the tool was not running in monitor mode and not explicitly hand processed.)\n"
        "By default monitor mode just immediately start monitoring and does not do a prescan.\n"
        "\n"
        "Command Line Flags:\n"
        "\n"
        "-monitor           Continue running and monitor the given folder for animation changes\n"
        "-prescan            When starting monitor mode scan the hierarchy for files that need processing\n"
        "-f                 Force all animations to be reprocessed regardless of age comparison\n"
        "-noperforce        Does not access asset version control system during processing. This can be used to\n"
        "                     work offline from Perforce. Also handy for development testing.\n"
        "\n"
        "-e <.anim>         Given a path to a runtime .anim file this will convert it to human readable\n"
        "                     text files and then quit. Useful for debugging and examining the runtime data.\n"
        "-s <.anim> <dest>  Given a path to a runtime .anim file this will convert it to source asset\n"
        "                     files, .SKELX and .ANIMX, placed in the destination.\n"
        "                     These files can be fed back as source assets or imported to MAX.\n"
        "                     DEVELOPMENT IN PROGRESS - DO NOT USE!\n"
        "-batch_src         Start at the current working directory and convert old .WRL associated .anim\n"
        "                     files to the new source animation formats: .SKELX and .ANIMX.\n"
        "                     Don't use this command unless you are aware of the files it will overwrite.\n"
        "                     DEVELOPMENT IN PROGRESS - DO NOT USE!\n"
        "\n"
        "-?                 Print this help\n"
        "\n"
        "\n"
        "\n"
        );

    printf( "\nPress ENTER to exit.\n" );
    getchar();
    exit(0);
}

int main(int argc,char **argv)
{
    bool    no_pig = false;
    bool    bMonitorMode = false;
    bool    bMonitorMode_DoPreScan = false;
    char    cwd[256];

    EXCEPTION_HANDLER_BEGIN

    {
        char title[256];
        int k;
        sprintf_s( title, sizeof(title), "%s v%s", "GetAnimation2", kVersionString);
        setConsoleTitle(title);
        printf("%s\n", title);
        for ( k = 0; k<argc; ++k)
            printf("%s ", argv[k]);
        printf("\n");
    }

    sharedMemorySetMode(SMM_DISABLED);
    setAssertMode((!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0) |
        ASSERTMODE_DEBUGBUTTONS);


    _getcwd(cwd, 128);

    if( checkForArg(argc, argv, "-?") )
    {
        usage();
    }

    // -e converts a single runtime .anim to text representations
    // (useful for debugging and inspection)
    if(checkForArg(argc, argv, "-e"))
    {
        if (argc == 3)
        {
            char* pPath = argv[2];
            outputAnimFileToText( pPath );
            exit(0);
        }
        else
        {
            usage();
        }
    }

    // -s extracts source files such as .SKELX skeleton hierarchy from the given
    // runtime .anim that can be used for animation processing
    if(checkForArg(argc, argv, "-s"))
    {
        if (argc > 2)
        {
            char* pSrcAnim = argv[2];
            char* pDst = (argc > 3) ? argv[3] : NULL;
            outputAnimFileToSourceAssets( pSrcAnim, pDst );
            exit(0);
        }
        else
        {
            usage();
        }
    }

    // Walk through the animation folders rooted at the current working directory
    // and for any legacy .WRL animation assets lookup the corresponding runtime .anim
    // and use that to make source assets for the new pipeline, i.e., .ANIMX and .SKELX
    if(checkForArg(argc, argv, "-batch_src"))
    {
        BatchConvertLegacySourceFolders( cwd );
        printf( "\nFinished. Press ENTER to exit.\n" );
        getchar();
        exit(0);
    }

    //-f reprocess all files(slow)
    if(checkForArg(argc, argv, "-f"))
        g_force_rebuild = 1;
    else
        g_force_rebuild = 0;

    g_no_version_control = 1;

    if(checkForArg(argc, argv, "-monitor"))
    {
        // stay resident and monitor for updated animation exports
        // that need conversion to in-game format
        bMonitorMode = true;
        // do we also want to prescan and process assets
        // exported when we were not monitoring?
        if(checkForArg(argc, argv, "-prescan"))
        {
            bMonitorMode_DoPreScan = true;
        }
    }

    if(checkForArg(argc, argv, "-nopig"))
    {
        no_pig = true;
    }

    if (no_pig)
    {
        fileSystemSetMode(FILE_MODE_LOOSE);
    }
    else
    {
        fileSystemSetMode(FILE_MODE_DYNAMIC);
    }

    fileAutoDataDir(true);

    _getcwd(cwd, 128);

    treeInit();
    
    if (bMonitorMode)
    {
        if (bMonitorMode_DoPreScan)
        {    
            // Initially we scan target path recursively looking for new animation exports that
            // need processing (or they might all the reprocess with -f flag was given)
            scanAndProcessRootPath(cwd); // CWD is our root path
        }

        // stay resident and monitor for updated animation exports
        getAnimMonitor( cwd );
    }
    else
    {
        // Scan target path recursively looking for new animation exports that
        // need processing (or they might all the reprocess with -f flag was given)
        scanAndProcessRootPath(cwd); // CWD is our root path

        printf( "\nFinished. Press ENTER to exit.\n" );
        getchar();
    }

    EXCEPTION_HANDLER_END
}

