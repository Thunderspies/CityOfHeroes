#include "win/win_init.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/file.h>
#include "UI/uiUtil.h"
#include "graphics/font.h"
#include "gameComm/wdwbase.h"
#include "edit/ClickToSource.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/utils/utils.h>
#include "entity/entDebug.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include "render/renderprim.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/components/Earray.h>
#include "graphics/gfxwindow.h"
#include "graphics/camera.h"

#define CTS_LENGTH_OF_STATUS 8
#define CTS_STATUS_TICK 25

typedef struct SourceLinkState
{
    char    state[128];
    U32        time;
} SourceLinkState;

U32 g_ctsstate = CTS_SINGLECLICK;

static char currentFile[MAX_PATH];
#define SAVE_CURRENT_FILENAME(filename)    sprintf(currentFile, "%s/%s", fileDataDir(), filename);    forwardSlashes(currentFile);

static ContextMenu* subMenu = NULL;

//static ClickToSourceLink** sourceLinks = 0;
static StashTable currentLinkStates = 0;
static int visChecksAreCached = 0;

// Fix up the filename to get rid of the path
static const char* ctsFixFilename(const char* filename)
{
    const char* fixedName;
    if ((fixedName = strrchr(filename, '/')) || (fixedName = strrchr(filename, '\\')))
        return fixedName+1;
    return filename;
}

// Show the current state of the link
static void ctsUpdateStatus(char* filename, const char* newStatus)
{
    char* fixedFilename = filename + strlen(fileDataDir()) + 1;
    SourceLinkState* linkState = stashFindPointerReturnPointer(currentLinkStates, fixedFilename);
    if (!linkState)
    {
        linkState = calloc(1, sizeof(SourceLinkState));
        linkState->state[0] = '\0';
        stashAddPointer(currentLinkStates, fixedFilename, linkState, false);
    }
    Strncpyt(linkState->state, newStatus);
    linkState->time = timerSecondsSince2000();
}

// Handle the different messages perforce can give, returns 0 if no error
static int ctsHandlePerforceMessage(int perforceMessage)
{
    if (!perforceMessage)
        return 0;

    ctsUpdateStatus(currentFile, "");
    return perforceMessage;
}

// trys open, edit, then editplus
static void ctsOpenFile()
{
    if (fileExists(currentFile))
    {
        int ret = (int)ShellExecuteA(NULL, "open", currentFile, NULL, NULL, SW_SHOW);
        if (ret <= 32)
        {
            ret = (int)ShellExecuteA(NULL, "edit", currentFile, NULL, NULL, SW_SHOW);
            if (ret <= 32)
                ret = (int)ShellExecuteA(NULL, "EditPlus", currentFile, NULL, NULL, SW_SHOW);
        }
        if (ret <= 32)
            ctsUpdateStatus(currentFile, "Failed to open the file, tell Lincoln.");
        else
            ctsUpdateStatus(currentFile, "File opened.");
    }
    else
        ctsUpdateStatus(currentFile, "File does not exist.");
}

static void ctsOpenDirectory()
{
    char* directory = strrchr(currentFile, '/');
    *directory = '\0';
    ShellExecuteA(NULL, "open", currentFile, NULL, NULL, SW_SHOW);
    ctsUpdateStatus(currentFile, "Opening the directory containing this file.");
}

static void ctsCheckoutFile()
{
    if (fileExists(currentFile))
    {
        ctsUpdateStatus(currentFile, "File checked out.");
    }
    else
        ctsUpdateStatus(currentFile, "File does not exist.");
}

static void ctsCheckinFile()
{
    if (fileExists(currentFile))
    {
        ctsUpdateStatus(currentFile, "Checking in file...");
    }
    else
        ctsUpdateStatus(currentFile, "File does not exist.");
}

static void ctsUndoCheckout()
{
    if (fileExists(currentFile))
    {
        ctsUpdateStatus(currentFile, "File unchecked out.");
    }
    else
        ctsUpdateStatus(currentFile, "File does not exist.");
}

static void ctsGetLatest()
{
    ctsUpdateStatus(currentFile, "You just got the latest version.");
}

static void ctsCheckRevisions()
{
    if (fileExists(currentFile))
    {
        ctsUpdateStatus(currentFile, "Getting file revision information.");
    }
}

static char* ctsGetBranchNumber(void* notUsed)
{
    static char branchNumber[128];
    static int init = 0;
    if (!init)
    {
        sprintf(branchNumber, "Branch (disabled)");
        init = 1;
    }
    return branchNumber;
}

// Show the current state of the link
static void ctsShowStatus(F32 x, F32 y, F32 z, const char* filename, CTSDisplayType displayType)
{
    SourceLinkState* linkState;
    if (!currentLinkStates)
        currentLinkStates = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);
    linkState = stashFindPointerReturnPointer(currentLinkStates, filename);
    if (!linkState)
    {
        linkState = calloc(1, sizeof(SourceLinkState));
        linkState->state[0] = '\0';
        stashAddPointer(currentLinkStates, filename, linkState, false);
    }
    if (linkState->state[0])
    {
        font_color(CLR_RED, CLR_RED);
        if (displayType == CTS_TEXT_REGULAR || displayType == CTS_TEXT_REGULAR_3D)
            prnt(x, y+10, z, 1.0, 1.0, linkState->state);
        else if (displayType == CTS_TEXT_DEBUG || displayType == CTS_TEXT_DEBUG_3D)
            fontSysText(x, y+2, linkState->state, 255, 0, 0);
        
        if (timerSecondsSince2000() - linkState->time > CTS_LENGTH_OF_STATUS)
            linkState->state[0] = '\0';
    }
}

static int ctsFileExists(void* notUsed)
{
    static CMVisType visType;
    if (!visChecksAreCached)
        visType = fileExists(currentFile)?CM_AVAILABLE:CM_VISIBLE;
    return visType;
}

static int ctsFileNotCheckedOut(void* notUsed)
{
    static CMVisType visType;
    if (!visChecksAreCached)
        visType = (fileExists(currentFile) && false)?CM_AVAILABLE:CM_VISIBLE;
    return visType;
}

static int ctsFileCheckedOutByYou(void* notUsed)
{
    static CMVisType visType;
    if (!visChecksAreCached)
    { 
        visType =(fileExists(currentFile) && false)?CM_AVAILABLE:CM_VISIBLE;
    }
    return visType;
}

static int ctsFileExistsWithSC(void* notUsed)
{
    static CMVisType visType;
    if (!visChecksAreCached)
        visType = (fileExists(currentFile) && false)?CM_AVAILABLE:CM_VISIBLE;
    return visType;
}

static void ctsShowContextMenu()
{
    visChecksAreCached = 0;
    contextMenu_display(subMenu);
    visChecksAreCached = 1;
}

// Setup the submenu for the right click filename options
static void ctsSetupSubMenu()
{
    subMenu = contextMenu_Create(NULL);
    contextMenu_addVariableText(subMenu, ctsGetBranchNumber, 0);
    contextMenu_addCode(subMenu, ctsFileExists, NULL, (CMCode)ctsOpenFile, NULL, "Open File", 0);
    contextMenu_addCode(subMenu, ctsFileExists, NULL, (CMCode)ctsOpenDirectory, NULL, "Open Directory", 0);
    contextMenu_addCode(subMenu, ctsFileNotCheckedOut, NULL, (CMCode)ctsCheckoutFile, NULL, "Checkout File", 0);
    contextMenu_addCode(subMenu, ctsFileCheckedOutByYou, NULL, (CMCode)ctsCheckinFile, NULL, "Checkin File", 0);
    contextMenu_addCode(subMenu, ctsFileCheckedOutByYou, NULL, (CMCode)ctsUndoCheckout, NULL, "Undo Checkout", 0);
    contextMenu_addCode(subMenu, ctsFileExistsWithSC, NULL, (CMCode)ctsGetLatest, NULL, "Get Latest", 0);
    contextMenu_addCode(subMenu, ctsFileExistsWithSC, NULL, (CMCode)ctsCheckRevisions, NULL, "Check Revisions", 0);
}

// Handles width differently depending on which display type we are using
static int ctsCalculateWidth(const char* toDisplay, CTSDisplayType displayType)
{
    if (displayType == CTS_TEXT_REGULAR || displayType == CTS_TEXT_REGULAR_3D)
        return str_wd(&game_9, 1.0, 1.0, toDisplay);
    else if (displayType == CTS_TEXT_DEBUG || displayType == CTS_TEXT_DEBUG_3D)
        return 8*strlen(toDisplay);
    return 0;
}

// Returns 1 if the submenu has been opened
// x, y, and z are 2d or 3d coordinates depending on what displayType you choose
// If display string is NULL, it will just use the pathless filename
int ClickToSourceDisplay(F32 x, F32 y, F32 z, F32 yShift, int color, const char* filename, const char* displayString, CTSDisplayType displayType)
{
    CBox box, menuBox;
    const char* toDisplay;
    int width;
    AtlasTex* menuBoxTex;
    F32 clickScale = 0.8;
    F32 centerOffset = 0.0;

//    if (isProductionMode() || !filename)
//        return 0;

    menuBoxTex = atlasLoadTexture("bdiamd01.tga");
    toDisplay = displayString?displayString:ctsFixFilename(filename);

    // Width will be different depending on display type
    width = ctsCalculateWidth(toDisplay, displayType);

    // If we were passed 3d coords, get the 2d equivalent
    if (displayType == CTS_TEXT_DEBUG_3D || displayType == CTS_TEXT_REGULAR_3D)
    {
        Vec3 worldSpace = {x, y, z};
        Vec2 screenSpace;
        Vec3 targetPoint;
        Mat4 loc = {0};
        Mat4 screen = {0};
        int  w,h;

        copyVec3(worldSpace, loc[3]);
        mulMat4(cam_info.viewmat, loc, screen);
        gfxWindowScreenPos(screen[3], screenSpace);

        subVec3(worldSpace, cam_info.cammat[3], targetPoint);
        if (dotVec3(targetPoint, cam_info.cammat[2]) > 0)
            return 0;
        
        windowSize(&w,&h);
        x = screenSpace[0];
        y = h - screenSpace[1];
        z = 100;
        if (y < 0 || y > h || x < 0 || x > w)
            return 0;

        // Now fix the x so that it displays centered
        x -= width/2;
    }

    // Shift the y here by the passed in parameter
    // Used to stack ctsDisplays that are called from 3d, so they line up bottom to top properly
    y += yShift;

    BuildCBox(&box, x, y-10, width, 8);
    BuildCBox(&menuBox, x+width, y-8, 16, 8);

    // Make yellow when user hovers mouse over, upon clicks, start handling all the options
     if (!isProductionMode() && filename && mouseCollision(&box) && CTS_SINGLE_CLICK)
    {
        color = CLR_YELLOW;
        if (mouseDown(MS_LEFT))
        {
            SAVE_CURRENT_FILENAME(filename)
            ctsOpenFile();
        }
        else if(mouseClickHit(&box, MS_RIGHT))
        {
            if (!subMenu)
                ctsSetupSubMenu();
            SAVE_CURRENT_FILENAME(filename)
            ctsShowContextMenu();
            return 1;
        }
    }

    // Controls the diamond that opens the submenu
    if (!isProductionMode() && filename && mouseCollision(&menuBox))
    {
        clickScale = 1.0;
        centerOffset = 1.0;
        if (mouseClickHit(&menuBox, MS_LEFT))
        {
            if (!subMenu)
                ctsSetupSubMenu();
            SAVE_CURRENT_FILENAME(filename)
            ctsShowContextMenu();
            return 1;
        }
    }

    // Do the actual displaying of the filename/status/diamond menu box
    font(&game_9);
    font_color(color, color);
    if (displayType == CTS_TEXT_REGULAR || displayType == CTS_TEXT_REGULAR_3D)
        prnt(x, y, z, 1.0, 1.0, toDisplay);
    else if (displayType == CTS_TEXT_DEBUG || displayType == CTS_TEXT_DEBUG_3D)
        fontSysText(x, y-8.0, toDisplay, (color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF);
    display_sprite(menuBoxTex, x+width+1-centerOffset, y-11-centerOffset, z, clickScale, clickScale, CLR_WHITE);
    if (!isProductionMode() && filename)
        ctsShowStatus(x, y, z, filename, displayType);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/// Output a object costume
#include "gameComm/npc.h"
#include "seq/seq.h"
#include "seq/seqtype.h"
#include "entity/costume.h"
#include <utilitieslib/utils/FolderCache.h>

int WriteAnObject( const char * libraryPieceName )
{
    int idx;
    const char * entTypeFileName;
    const NPCDef * costumeDef = 0;
    const SeqType * entTypeDef = 0;

    if( !libraryPieceName )
        return 0;

    //////////////////////////////////////////////////////////////
    ///////////// Costume File /////////////////////////////
    costumeDef = npcFindByName( libraryPieceName, &idx );

    //If no costume of this name, make one
    if( !costumeDef || !idx ) //Returns Mek_Man on failure
    {
        char costumeFileName[256];
        const char * userName;
        FileWrapper * file = 0;

        //////// Check out and open the costume
        userName = "Unknown";
        if(userName)
            sprintf( costumeFileName, "Defs/Objects/Objects%s.nd", userName );
        else
            sprintf( costumeFileName, "Defs/Objects/Objects.nd" );

        file = fileOpen( costumeFileName, "at" );
        if (!file)
        {
            Errorf( "Error opening costume file %s.", costumeFileName );
        }

        if( !file )
            return 0;

        //If you succeeded, write the costume
        if( file )
        {
            fprintf( file, "\n\n"
                "\n// AUTOGENERATED COSTUME named after a library piece"
                "\nNPC %s\n"
                "{\n"
                "\tDisplayName \"%s\"\n"
                "\tCostume\n"
                "\t{\n"
                "\t\tEntTypeFile %s\n"
                "\t}\n"
                "}",
                libraryPieceName, libraryPieceName, libraryPieceName
                );
            fclose(file);
        }
    }

    //Now try to find it again (You definitely should now (Autoreload should immediately find it if it wasn't there before)
    FolderCacheDoCallbacks();
    costumeDef = npcFindByName( libraryPieceName, &idx );
    if( !costumeDef || !idx )
    {
        Errorf( "5 (Make Object failed). Failed to find updated costumeDef. Tell Woomer." );
        return 0;
    }

    //Now Open Costume File (TO DO maybe go to the line where the costuem file in question is?
    {
        char fullPath[400];
        sprintf( fullPath, "%s/%s",  fileDataDir(), costumeDef->fileName );
        fileOpenWithEditor(fullPath);
    }

    //////////////////////////////////////////////////////////
    ///////////// Enttype File /////////////////////////////
        
    entTypeFileName = costumeDef->costumes[0]->appearance.entTypeFile;

    //Look for Enttype file from CostumeDef.
    entTypeDef = seqTypeFind( entTypeFileName );

    //Compose a new EntType if needed
    if( !entTypeDef )
    {
        FileWrapper * file = 0;
        char entTypeFileNameWithPath[400];

        sprintf( entTypeFileNameWithPath, "ent_types/%s.txt", entTypeFileName ); 

        file = fileOpen( entTypeFileNameWithPath, "wt" );
        if (!file)
        {
            Errorf( "9 (Make Object failed) Error opening %s EntType, tell Woomer", entTypeFileNameWithPath );
        }


        if( !file )
            return 0;

        if( file )
        {
            fprintf( file,    "\n# AUTOGENERATED ENTTYPE named after a library piece"
                "\nType\n"
                "\nSequencer        Prop_Basic.txt"
                "\nSequencerType    Prop_Basic"
                "\n"
                "\nHealthFx"
                "\n\tRange\t0 100"
                "\n\tLibraryPiece\t%s"
                "\n\t#ContinuingFX\t<fxName>"
                "\n\t#OneShotFX\t<fxName>"
                "\nEnd"
                "\n"
                "\nCapsuleSize    3.8 7.3 3.8"
                "\nCapsuleOffset  0 0 0"
                "\n"
                "\nFadeOutStart     500"
                "\nFadeOutFinish    550"
                "\nShadowType       None"
                "\n"
                "\nPlacement        DeadOn"
                "\nSelection        LibraryPiece"
                "\nCollisionType    LibraryPiece"
                "\n"
                "\nTicksToLingerAfterDeath        25"
                "\nTicksToFadeAwayAfterDeath      1"
                "\n\nEnd",
                libraryPieceName
                );
            fclose(file);
        }
    }

    //Now try again to get enttpe file (should always succeed)
    FolderCacheDoCallbacks();
    entTypeDef = seqTypeFind( entTypeFileName );
    if( !entTypeDef )
    {
        Errorf( "11 Failed to load Enttype file %s. Tell Woomer", entTypeFileName );
        return 0;
    }
    
    //Open the EntType file
    {
        char fullPath[400];
        sprintf( fullPath, "%s/%s",  fileDataDir(), entTypeDef->filename );
        fileOpenWithEditor(fullPath);
    }

    return 1;
}


















