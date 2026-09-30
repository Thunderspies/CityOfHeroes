#include <utilitieslib/utils/wininclude.h>  // JS: Can't find where this file includes <windows.h> so I'm just sticking this include here.
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>

#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/timing.h>
#include "player/player.h"
#include <utilitieslib/utils/FileSystem.h>
#include <utilitieslib/utils/fileutil.h>
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include <utilitieslib/utils/tga.h>
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/FileSystem.h>
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include "game.h"
#include "seq/anim.h"
#include "fxinfo.h"
#include "graphics/jpeg.h"
#endif

#include "imageServer.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/motion.h"
#include "entity/entity.h"
#include <utilitieslib/utils/file.h>
#include "gameData/BodyPart.h"
#include <utilitieslib/utils/error.h>

#if CLIENT
#include "cmdparse/cmdgame.h"
#include "render/thread/rt_init.h"
#include "render/thread/rt_queue.h"

#endif

#define COSTUME_STRING_LEN 512
#define COSTUME_PART_COUNT 30

static ImageServerParams imageserver_default_params =
{
    256,
    512,
    0xffffff,
    "Portrait_bkgd_contact.tga",
    "",
    "",
    FALSE,
    FALSE
};

// This is where we write logfiles for the image generator. We don't
// do any modification of the name because only one image generator is
// running at a time on a given data set and location.
static char* imageLogFileName = "image_generator";


/**********************************************************************func*
 * imageserver_MakeSafeFilename
 *
 * This function will modify the given a string (which is unique among a
 *   set of strings), such that it's safe for making using as a filename.
 * The filename will also be unique assuming that the incoming string
 *   only uses alphanumerics, spaces, and the apostrophe ('), hyphen (-),
 *   and  period(.) (Hero names follow these rules.)
 */
void imageserver_MakeSafeFilename(char *pch)
{

    // if the first character is a period, the file scan will not like it
    if (pch && *pch == '.')
        *pch = '!';

    while(pch && *pch)
    {
        switch(*pch)
        {
            case ' ':
                *pch='_';
                break;
            case '?':
                *pch='!';
                break;
            case '\\':
                *pch='(';
                break;
            case '//':
                *pch=')';
                break;
            case '*':
                *pch='@';
                break;
            case ':':
                *pch=';';
                break;
        }
        pch++;
    }

}


/**********************************************************************func*
 * imageserver_MakeSubdirFilename
 *
 * If subDir is non-zero, creates a directory name: "pchDir/subDir/pchSafeName" + "pchAppend"
 * or "misc" if the subDir is not between 'a' and 'z' (case-insensitive)
 *
 * If useSubdir is zero, creates a directory name: "pchDir/pchSafeName" + "pchAppend"
 *
 * The returned string is local to the function; copy it away.
 *
 */
char *imageserver_MakeSubdirFilename(const char *pchDir, const char *pchSafeName, const char *pchAppend, char subDir)
{
    static char ach[MAX_PATH];
    int count;

    if (subDir)
    {
        if (subDir >= 'a' && subDir <= 'z')
        {
            char achSubDirStr[2] = "";
            achSubDirStr[0] = subDir;
            sprintf(ach, "%s/%s/%s%s", pchDir, achSubDirStr, pchSafeName, pchAppend);
        }
        else if (subDir >= 'A' && subDir <= 'Z')
        {
            char achSubDirStr[2] = "";
            achSubDirStr[0] = _tolower(subDir);
            sprintf(ach, "%s/%s/%s%s", pchDir, achSubDirStr, pchSafeName, pchAppend);
        }
        else
        {
            sprintf(ach, "%s/misc/%s%s", pchDir, pchSafeName, pchAppend);
        }
    }
    else
    {
        sprintf(ach, "%s/%s%s", pchDir, pchSafeName, pchAppend);
    }

    count = 0;

    // Force the entire path to lower-case so it works on a Unix filesystem.
    while (ach[count] != '\0')
    {
        if (ach[count] >= 'A' && ach[count] <= 'Z')
        {
            ach[count] = _tolower(ach[count]);
        }

        count++;
    }

    return ach;
}


void imageserver_CopyParams(ImageServerParams *dest, ImageServerParams *src)
{
    if (!dest || !src)
        return;

    dest->bgcolor = src->bgcolor;
    strcpy(dest->bgtexture, src->bgtexture);
    dest->headshot = src->headshot;
    strcpy(dest->outputname, src->outputname);
    strncpy(dest->caption, src->caption, MAX_CAPTION-1);
    dest->x_res = src->x_res;
    dest->y_res = src->y_res;
}

// returns 0 if there is an error
int imageserver_ReadFromCSV_EX( Costume ** pCostume, ImageServerParams *imageParams, char * csvFileName, char **static_strings )
{
    Costume *costume;
    int success = 0;
    char * csv    = fileAlloc(csvFileName, NULL);
    char *tmp;
    static int static_string_idx = 0;

    Appearance * appearance;
    CostumePart * part;
    int idx = 0;
    int argnum;
    char *cursor;
    char *args[100];
    int i,argc,fileLine;

    if (!pCostume || !*pCostume || /*!imageParams ||*/ !csv)
        return 0;

    static_string_idx = 0;

    costume = *pCostume;
    appearance = &costume->appearance;
    appearance->iNumParts = eaSize(&bodyPartList.bodyParts);

    // Set defaults
    if (imageParams)
        imageserver_CopyParams(imageParams, &imageserver_default_params);

    //// Change delimiters
    //for (cursor = csv; *cursor; cursor++) {
    //    if (*cursor == ',')
    //        *cursor = '\t';
    //}

    cursor = csv;

    fileLine = 0;

    for (;;)
    {
        if (!cursor)
            break;

        if ((argc = tokenize_line_quoted_delim(cursor, args, 100, &cursor, " \t,", " \t,")) < 0)
            break;

        // Zero or more returned from the tokenize means it read a line
        // without error. Might've been blank, but at least it got up to
        // a newline.
        fileLine++;

        if (argc == 0)
            continue;

        if (args[0][0] == ';' || args[0][0]=='#' || (args[0][0]=='/' && args[0][1]=='/'))
            continue;

        if (stricmp(args[0], "PARAMS:")==0)
        {
            int i;
            // Parameter string!
            for (i=1; i<argc; i++)
            {
                if (strStartsWith(args[i], "X="))
                {
                    if (imageParams)
                        imageParams->x_res = atoi(args[i]+2);
                }
                else if (strStartsWith(args[i], "Y="))
                {
                    if (imageParams)
                        imageParams->y_res = atoi(args[i]+2);
                }
                else if (strStartsWith(args[i], "BGTEXTURE="))
                {
                    if (imageParams)
                        strcpy(imageParams->bgtexture, args[i]+strlen("BGTEXTURE="));
                }
                else if (strStartsWith(args[i], "BGCOLOR=0x"))
                {
                    if (imageParams)
                        sscanf(args[i]+strlen("BGCOLOR=0x"), "%x", &imageParams->bgcolor);
                }
                else if (strStartsWith(args[i], "BGCOLOR="))
                {
                    if (imageParams)
                        sscanf(args[i]+strlen("BGCOLOR="), "%d", &imageParams->bgcolor);
                }
                else if (strStartsWith(args[i], "HEADSHOT"))
                {
                    if (imageParams)
                        imageParams->headshot = TRUE;
                }
                else if (strStartsWith(args[i], "DELETECSV"))
                {
                    if (imageParams)
                        imageParams->deleteCSV = TRUE;
                }
                else if (strStartsWith(args[i], "OUTPUTNAME="))
                {
                    if (imageParams)
                        strcpy(imageParams->outputname, args[i]+strlen("OUTPUTNAME="));
                }
                else if (strStartsWith(args[i], "CAPTION="))
                {
                    i++;
                    if (imageParams && i < argc)
                        strncpy(imageParams->caption, args[i], MAX_CAPTION-1);
                }
#if defined(CLIENT)
                else if (strStartsWith(args[i], "COSTUMENAME="))
                {
                    char buf[100];
                    const NPCDef* npcDef;
                    strcpy(buf, args[i]+strlen("COSTUMENAME="));
                    npcDef = npcFindByName(buf, NULL);
                    if (npcDef != NULL) {
                        const cCostume *pNpcCostume = eaGetConst(&npcDef->costumes, 0);
                        if(pNpcCostume) {

                            *pCostume = costume_as_mutable_cast(pNpcCostume);
                            free(csv);
                            return 1;
                        }
                    }
                }
#endif
                else
                {
                    printf("Invalid PARAMS specified: %s; expected X=, Y=, BGTEXTURE=, BGCOLOR=, or OUTPUTNAME=\n", args[i]);
                    filelog_printf(imageLogFileName, "Invalid PARAMS specified: '%s', line %d - expected X=, Y=, BGTEXTURE=, BGCOLOR=, or OUTPUTNAME=\n", csvFileName, fileLine);
                }
            }
            continue;
        }


        argnum = 0;
        if (argc == 14+MAX_BODY_SCALES) 
        {
            // First arg is part idx
            idx = atoi( args[argnum++] );

            if (idx >= COSTUME_PART_COUNT) {
                printf("\nWarning, found more than %d costume parts, ignoring last ones\n", COSTUME_PART_COUNT);
                filelog_printf(imageLogFileName, "'%s' line %d: Found too few costume parts - abandoning\n", csvFileName, fileLine);
                free(csv);
                return 1;
            }

        }
#if CLIENT
        else if (!game_state.imageServer && argc == 13+MAX_BODY_SCALES)
#else
        else if (argc == 13+MAX_BODY_SCALES)
#endif
        {
            // Assume parts in artibrary and correct? order with valid name
        }
        else
        {
            printf("Invalid line in .csv file, found %d arguments, expected %d or %d\n", argc, 13+MAX_BODY_SCALES,14+MAX_BODY_SCALES);
#if CLIENT
            filelog_printf(imageLogFileName, "'%s' line %d: Found %d arguments, expected %d - abandoning\n", csvFileName, fileLine, argc, 14+MAX_BODY_SCALES);
#else
            filelog_printf(imageLogFileName, "'%s' line %d: Found %d arguments, expected %d or %d - abandoning\n", csvFileName, fileLine, argc, 13+MAX_BODY_SCALES, 14+MAX_BODY_SCALES);
#endif
            free(csv);
            return 0;
        }

        if (idx >= eaSize(&bodyPartList.bodyParts)) {
            printf("\nWarning, found more than %d costume parts, ignoring last ones\n", eaSize(&bodyPartList.bodyParts));
            filelog_printf(imageLogFileName, "'%s' line %d: Found too many costume parts - abandoning\n", csvFileName, fileLine);
            free(csv);
            return 1;
        }

        part = costume->parts[idx];

        //Assumes query went like this:
        //att2.name as Geom, att3.name as Tex1, att4.name as Tex2, att5.name as Name, Color1, Color2, Ents.BodyScale, Ents.BoneScale, Ents.BodyType

#define STRCPY_STATIC_STRING(dst, src) \
    (static_strings) ? strcpy(static_strings[static_string_idx], (src)), (dst)=static_strings[static_string_idx++]\
    : (strcpy((char*)(dst),(src)))

        STRCPY_STATIC_STRING( part->pchGeom, args[argnum++] );
        //if (static_strings)
        //{
        //    strcpy(static_strings[static_string_idx], args[0]), 
        //        part->pchGeom=static_strings[static_string_idx++];
        //}
        //else
        //    (strcpy(part->pchGeom,args[0]));

        STRCPY_STATIC_STRING( part->pchTex1, args[argnum++] );
        STRCPY_STATIC_STRING( part->pchTex2, args[argnum++] );

        if (argc == 14+MAX_BODY_SCALES) 
        {
            if (part->pchName)
                STRCPY_STATIC_STRING( part->pchName, bodyPartList.bodyParts[idx]->name );
            argnum++;
        }
        else
        {
            if (part->pchName)
                STRCPY_STATIC_STRING( part->pchName, args[argnum++] );
            idx++;
        }

        part->color[0].integer = atoi( args[argnum++] );
        part->color[1].integer = atoi( args[argnum++] );
        appearance->bodytype   = atoi( args[argnum++] );

        // Guard against being fed a corrupt body type in the data file
        // since we use it as an array index elsewhere in the code.
        if (appearance->bodytype != kBodyType_Male &&
            appearance->bodytype != kBodyType_Female &&
            appearance->bodytype != kBodyType_Huge)
        {
            appearance->bodytype = kBodyType_Male;
        }

        appearance->colorSkin.integer = atoi( args[argnum++] );

        for(i=0;i<MAX_BODY_SCALES;i++)
            appearance->fScales[i] = atof(args[argnum+i]);


        argnum += MAX_BODY_SCALES;
        if (args[argnum][0] && stricmp(args[argnum],"0")!=0)
        {
            STRCPY_STATIC_STRING( part->pchFxName, args[argnum] );
            part->color[2].integer = atoi( args[argnum + 1] );
            part->color[3].integer = atoi( args[argnum + 2] );
        }
        else
        {
            STRCPY_STATIC_STRING(part->pchFxName, "None");
            part->color[2].integer = 0;
            part->color[3].integer = 0;
        }

        tmp = strchr( args[argnum+3], '_' );
        if( tmp )
            *tmp = ' ';

        if (args[argnum+3][0] && stricmp(args[argnum+3],"0")!=0 && part->regionName)
            STRCPY_STATIC_STRING( part->regionName, args[argnum+3] );
        else if (part->regionName)
            STRCPY_STATIC_STRING(part->regionName, "None");

        if (args[argnum+4][0] && stricmp(args[argnum+4],"0")!=0 && part->bodySetName)
            STRCPY_STATIC_STRING( part->bodySetName, args[argnum+4] );
        else if (part->bodySetName)
            STRCPY_STATIC_STRING(part->bodySetName, "None");

        success = 1;
    }
    free(csv);

    return success;
}

// returns 0 if there was an error
int imageserver_WriteToCSV( const cCostume * pCostume, ImageServerParams *imageParams, char * csvFileName )
{
    int i,k;
    int n;
    FILE *fp;

    fp = fileOpen(csvFileName, "w");
    if(fp==NULL)
    {
        printf("*** Error: Unable to open file '%s'\n", csvFileName);
        filelog_printf(imageLogFileName, "Could not create output file '%s'\n", csvFileName);
        return 0;
    }

    // PARAMS: X=512,Y=512
    // PARAMS: BGTEXTURE=hazard_06_03.tga,BGCOLOR=0x9f9f9f
    // PARAMS: OUTPUTNAME=c:\is\work\test.jpg
    // Tight,Pants,Stars,Pants,-3684409,-3372800,NULL,NULL,1,-16756099,None,NULL,NULL

    if (imageParams)
    {
        fprintf(fp, "PARAMS: X=%d,Y=%d\n", imageParams->x_res, imageParams->y_res);
        
        if (imageParams->outputname[0])
            fprintf(fp, "PARAMS: OUTPUTNAME=%s\n", imageParams->outputname);

        if (imageParams->bgtexture[0])
            fprintf(fp, "PARAMS: BGTEXTURE=%s\n", imageParams->bgtexture);

        fprintf(fp, "PARAMS: BGCOLOR=%d\n", imageParams->bgcolor);
        
        if (imageParams->headshot)
            fprintf(fp, "PARAMS: HEADSHOT\n");

        if (imageParams->deleteCSV)
            fprintf(fp, "PARAMS: DELETECSV\n");

        if (imageParams->caption[0])
            fprintf(fp, "PARAMS: CAPTION= \"%s\"\n", imageParams->caption);
    }

    n = eaSize(&pCostume->parts);
    for(i=0; i<n; i++)
    {
        const CostumePart *ppart= pCostume->parts[i];

        if(ppart->pchGeom!=NULL && ppart->pchGeom)
        {
            char region[COSTUME_STRING_LEN];
            char *tmp;
            fprintf(fp, "%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d,%d",
                i,
                (ppart->pchGeom && ppart->pchGeom[0])?ppart->pchGeom:"None",
                (ppart->pchTex1 && ppart->pchTex1[0])?ppart->pchTex1:"None",
                (ppart->pchTex2 && ppart->pchTex2[0])?ppart->pchTex2:"None",
                (ppart->pchName && ppart->pchName[0])?ppart->pchName:"None",
                ppart->color[0].integer,
                ppart->color[1].integer,
                pCostume->appearance.bodytype,
                pCostume->appearance.colorSkin.integer);

            for(k=0;k<MAX_BODY_SCALES;k++)
                fprintf(fp,",%f", pCostume->appearance.fScales[k]);

            if(ppart->pchFxName)
            {
                fprintf(fp, ",\"%s\",%d,%d",
                    ppart->pchFxName,
                    ppart->color[2].integer,
                    ppart->color[3].integer);
            }
            else
            {
                fprintf(fp,",0,0,0");
            }

            if( ppart->regionName)
            {
                strcpy(region,ppart->regionName);
                tmp = strchr(region,' ');
                if( tmp )
                    *tmp = '_';
                
                fprintf(fp, ",\"%s\",\"%s\"", region, ppart->bodySetName);
            }
            else
            {
                fprintf(fp, ",0,0");
            }

            fprintf(fp, "\n");
        }
    }

    fclose(fp);
    return 1;
}


void imageserver_InitDummyCostume( Costume * costume )
{
    int i;

    for(i = eaSize(&costume->parts)-1; i >= 0; --i)
        StructDestroy(ParseCostumePart, eaRemoveConst(&costume->parts, i));
    StructInitFields(ParseCostume, costume);

    for( i = 0 ; i < COSTUME_PART_COUNT ; i++ )
        eaPush(&costume->parts, StructAllocRaw(sizeof(CostumePart)));
    costume->appearance.iNumParts = eaSize(&bodyPartList.bodyParts);
}

//########################################################################################
//########################################################################################
//########################################################################################

#ifdef CLIENT

//##############################################################################################
//## Image Server Body Shots ###################################################################

//This whole thing is a big hack.
void imageserver_SaveShot( const cCostume * costume, ImageServerParams *imageParams, char * tgaFileAndPathName )
{
    Entity * e;

    e = entCreate("");
    entSetImmutableCostumeUnowned(e, costume);
    costume_Apply( e );
    entUpdateMat4Instantaneous(e, unitmat);
    copyMat4( ENTMAT(e), e->seq->gfx_root->mat );

    //int changeScale(Entity * e, Vec3 newscale ) //to do: this?

    animSetHeader( e->seq, 0 );

    {
        U8 * pixbuf;
        //Settable params
        int sizeOfPictureX = imageParams->x_res; // 256; //Size of shot on screen.
        int sizeOfPictureY = imageParams->y_res; // 512; //Powers of two!!! ARgh.
        Vec3 headshotCameraPos;
        F32 headShotFovMagic = 28;   //This is a weird magic thing
        int shouldIFlip = 0;
        BoneId centerBone = BONEID_HIPS;
        int old_useFBOs;
        
        if (imageParams->headshot) {
            centerBone = BONEID_HEAD;

            headshotCameraPos[0] = -0.05;
            headshotCameraPos[1] = 0.0;
            headshotCameraPos[2] = 15.0;
        } else {
            //TO DO: //DO I want to change the camera to reflect the bones? In any event extract the specific info from this func to here    
            if ( ( e->seq->type->flags & SEQ_USE_DYNAMIC_LIBRARY_PIECE ) || getRootHealthFxLibraryPiece( e->seq , (int)e->svr_idx ) )
            {
                Mat4 collMat;
                EntityCapsule cap;
                Vec3 p;

                if (getRootHealthFxLibraryPiece( e->seq , (int)e->svr_idx ))
                    manageEntityHitPointTriggeredfx(e);

                getCapsule(e->seq, &cap ); 
                positionCapsule( ENTMAT(e), &cap, collMat ); 

                p[0] = 0;
                p[1] = cap.length/2.0;
                p[2] = 0;

                mulVecMat4( p, collMat, headshotCameraPos );

//                headshotCameraPos[0];
//                headshotCameraPos[1];
                headshotCameraPos[2] += 22.0*512/sizeOfPictureY;
                
                
            } else {
                headshotCameraPos[0] = -0.05;
                headshotCameraPos[1] = -0.7;
                headshotCameraPos[2] = 22.0*512/sizeOfPictureY;
                if (stricmp(e->seq->type->name, "fem")==0) {
                    headshotCameraPos[2] /= 1.144;
                }
            }
        }
        //TO DO particles should be settable
        //End Settable params

        // Disable FBOs
        // Screenshot does not appear to work with FBOs enabled.
        rdrQueueFlush();
        old_useFBOs = rdr_caps.supports_fbos;
        rdr_caps.supports_fbos = 0;

        pixbuf = getThePixelBuffer( e->seq, headshotCameraPos, headShotFovMagic, sizeOfPictureX, sizeOfPictureY, centerBone, shouldIFlip, 1, atlasLoadTexture(imageParams->bgtexture), imageParams->bgcolor, 1, imageParams->caption );

        // HACK:  For some reason, the first time might show a low LOD model
        // So, throw out the result from the first time and do it again.
        free(pixbuf);
        pixbuf = getThePixelBuffer( e->seq, headshotCameraPos, headShotFovMagic, sizeOfPictureX, sizeOfPictureY, centerBone, shouldIFlip, 1, atlasLoadTexture(imageParams->bgtexture), imageParams->bgcolor, 1, imageParams->caption );

        rdrQueueFlush();
        rdr_caps.supports_fbos = old_useFBOs;

        //Write out the TGA
        {
            U8        *pix,*top,*bot,*t;
            int        w,h,y,linesize;
            w = sizeOfPictureX;
            h = sizeOfPictureY;
            pix = pixbuf;
            linesize = w*4;
            t = malloc(linesize);
            for(y=0;y<h/2;y++)
            {
                top = pix+y*linesize;
                bot = pix+(h-y-1)*linesize;
                memcpy(t,top,linesize);
                memcpy(top,bot,linesize);
                memcpy(bot,t,linesize);
            }

            if (!game_state.noSaveTga) {
                if (strEndsWith(imageParams->outputname, ".tga")) {
                    tgaSave( imageParams->outputname, pixbuf,w,h,3 );
                } else {
                    tgaSave( tgaFileAndPathName, pixbuf,w,h,3 );
                }
            }

            if (!game_state.noSaveJpg) {
                char * s;
                s = strrchr( tgaFileAndPathName, '.' );
                strcpy( s, ".jpg");
                if (strEndsWith(imageParams->outputname, ".jpg")) {
                    jpgSave( imageParams->outputname, pixbuf, 4, sizeOfPictureX, sizeOfPictureY );
                } else {
                    jpgSave( tgaFileAndPathName, pixbuf, 4, sizeOfPictureX, sizeOfPictureY );
                }
                strcpy( s, ".tga");
            }

            free(t);
        }
        free(pixbuf);
    }

    entFree( e );
}


// from C:/game/srcDir/stuff/capnAmaxing.csv in C:/game/srcDir  to C:/game/targetDir
//generate C:/game/targetDir/stuff/capnAmaxing.csv
static void getTargetPathAndName( char * csvFilePathAndName, char * sourceDir, char * targetDir, char * tgaFilePathAndName )
{
    int len; 

    //Clean up paths
    forwardSlashes( sourceDir ); // TO DO, what is cleaning function?
    forwardSlashes( targetDir );
    forwardSlashes( csvFilePathAndName );

    len = strlen( sourceDir );
    if( sourceDir[len-1] == '/' )
        sourceDir[len-1] = 0;

    len = strlen( targetDir );
    if( sourceDir[len-1] == '/' )
        targetDir[len-1] = 0;

    //concateneate to target path
    len = strlen( sourceDir );
    assert( csvFilePathAndName[len] == '/' );

    sprintf( tgaFilePathAndName, "%s%s", targetDir, &csvFilePathAndName[len] );

    //switch to .tga
    len = strlen( tgaFilePathAndName );
    assert( strEndsWith(tgaFilePathAndName, ".csv" ) );
    if (game_state.noSaveTga) {
        sprintf( &tgaFilePathAndName[len-4], ".jpg" );
    } else {
        sprintf( &tgaFilePathAndName[len-4], ".tga" );
    }

    //make sure the folder exists
    {
        //char * tmp;
        //tmp = strrchr( tgaFilePathAndName, '/' );
        //*tmp = 0;
        mkdirtree(tgaFilePathAndName);
        //*tmp = '/';
    }
}

static char ** static_costume_strings = 0;

static int serveUpAnImage( char * csvFilePathAndName, char * tgaFilePathAndName, const char * relPath )
{
    static Costume costume;
    Costume *pCost = &costume;
    int succeeded=0;
    char cmdline[MAX_PATH + 12];

    static ImageServerParams imageParams;
    imageParams.bgcolor = 0xffffffff;
    imageParams.bgtexture[0] = 0;
    imageParams.outputname[0] = 0;
    imageParams.caption[0] = 0;
    imageParams.x_res = 256;
    imageParams.y_res = 512;
    imageParams.headshot = FALSE;
    imageParams.deleteCSV = FALSE;

    printf("Processing %s into %s... ", csvFilePathAndName, tgaFilePathAndName);
    filelog_printf(imageLogFileName, "Reading costume file '%s'\n", csvFilePathAndName);

    imageserver_InitDummyCostume( &costume );

    updateGameTimers();

    if ( !static_costume_strings )
    {
        int i;
        static_costume_strings = malloc(300*sizeof(char*));
        for (i=0;i<300;++i)
        {
            static_costume_strings[i] = malloc(32);
        }
    }

    //Fill out the costume structure and image generation parameters
    if( imageserver_ReadFromCSV_EX( &pCost, &imageParams, csvFilePathAndName , static_costume_strings ) )
    {
        printf("Successfully read %d body parts\n", costume.appearance.iNumParts);
        //Do the headshot
        //Costume  * costumex = npcDefsGetCostume( 10, 0 ); //debug

        filelog_printf(imageLogFileName, "Succeeded - writing image file '%s'\n", tgaFilePathAndName);

        imageserver_SaveShot( costume_as_const(pCost), &imageParams, tgaFilePathAndName );
        succeeded = 1;
    } else {
        filelog_printf(imageLogFileName, "Failed - no image written\n");

        printf("Failed to parse costume\n");
        succeeded = 0;
    }

    if (imageParams.deleteCSV)
    {
        backSlashes(csvFilePathAndName);
        sprintf(cmdline, "del /Q /F %s > nul", csvFilePathAndName);
    }
    else
    {
        // Save request
        char newname[MAX_PATH];
        if (succeeded)
        {
            sprintf(newname, "%s/processed", game_state.imageServerSource);
        }
        else
        {
            sprintf(newname, "%s/failed", game_state.imageServerSource);
        }
        strcpy(newname, imageserver_MakeSubdirFilename(newname, relPath, "", relPath[0]));

        mkdirtree(newname);
        backSlashes(csvFilePathAndName);
        backSlashes(newname);

        sprintf(cmdline, "move /Y %s %s > nul", csvFilePathAndName, newname);
    }

    system(cmdline);

    return succeeded;
}

static int s_timer = 0;
static int served_image_count=0;
static void serverImageCallback(const char *relPath, int when) 
{
    char csvFilePathAndName[1024];
    char tgaFilePathAndName[1024];
    
    if (strstr(relPath, "failed/") || strstr(relPath, "processed/")) {
        // Don't process things in a 'failed' or 'processed' subdirectory
        return;
    }

    sprintf( csvFilePathAndName, "%s/%s", game_state.imageServerSource, relPath );

    fileWaitForExclusiveAccess(csvFilePathAndName);

    //Generate name C:/game/targetDir/stuff/capnAmazing.tga
    getTargetPathAndName( csvFilePathAndName, game_state.imageServerSource, game_state.imageServerTarget, tgaFilePathAndName );

    serveUpAnImage( csvFilePathAndName, tgaFilePathAndName, relPath );

    served_image_count++;
    if (served_image_count%50==0) {
        // Free all data once and a while to keep us from running out of memory!
        modelFreeAllCache(0);
        fxPreloadGeometry();
    }

    timerStart(s_timer);
}

static void serverImageCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	serverImageCallback(change->entry.path, change->kind);
}


static FileScanAction imageServerProcessor(char *dir, struct _finddata32_t* data)
{
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s/%s", dir, data->name);
    if (simpleMatch("*.csv", fullpath)) {
        char *relpath = fullpath + strlen(game_state.imageServerSource);
        serverImageCallback(relpath, 0);
    }
    return FSA_EXPLORE_DIRECTORY;
}


void imageserver_Go()
{
    static FileSystem * fcImageServer = 0;
    static int startedUp = 0;

    if( startedUp!=5 )
    {
        startedUp++;
        if (startedUp!=5)
            return;
        printf( "###Image Server Mode! Taking Costumes from %s and putting them in %s\n", game_state.imageServerSource, game_state.imageServerTarget );
        filelog_printf(imageLogFileName, "-----------------------------------------\n");
        filelog_printf(imageLogFileName, "Character image server started\n");
        filelog_printf(imageLogFileName, "-----------------------------------------\n");
        filelog_printf(imageLogFileName, "Costume source file directory: '%s'\n", game_state.imageServerSource);
        filelog_printf(imageLogFileName, "Image destination file directory: '%s'\n", game_state.imageServerTarget);

        s_timer = timerAlloc();

        // Look for pending requests
        fileScanDirRecurseEx(game_state.imageServerSource, imageServerProcessor);

        //Activate Folder Monitor
        {
            char folderToMonitor[1024];

            fcImageServer = fileSystemCreate();

            sprintf( folderToMonitor, "%s/", game_state.imageServerSource );
            fileSystemAddSource(fcImageServer, folderToMonitor, 0);
            fileSystemDispatch();

            fileSystemSubscribe(NULL, "*.csv", (FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE), serverImageCallbackFileChanged, NULL);
            printf( "###Image Server Mode! Now monitoring for .csv files in %s\n", game_state.imageServerSource);

        }

        timerStart(s_timer);

    }

    //Do Jimb's monitor every frame
    if( fcImageServer )
    {
        if (timerElapsed(s_timer) > 5) {
            // Look for files that showed up that the folder cache missed
            //  this can happen if the server is hung for a long time
            fileScanDirRecurseEx(game_state.imageServerSource, imageServerProcessor);
            timerStart(s_timer);
        }
        fileSystemDispatch(); // Just to get Update
        Sleep(1);
    }
}

#endif //CLIENT



