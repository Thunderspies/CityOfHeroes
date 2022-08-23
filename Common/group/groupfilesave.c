#include "seq/anim.h"
#include <string.h>
#include <stdio.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netcomp.h>
#include "group/group.h"
#include <utilitieslib/utils/utils.h>
#include "group/groupProperties.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include "group/groupfilelib.h"
#include "time.h"
#include <float.h>
#include <utilitieslib/components/EString.h>
#include "bases/bases.h"
#include "bases/baseparse.h"
#include <utilitieslib/components/StashTable.h>
#include "group/grouputil.h"

#ifdef SERVER
#include "group/groupnetdb.h"
#include "group/groupdb_util.h"
#include "dbcomm/dbcomm.h"
#include "cmdparse/cmdserver.h"
#endif

int ignoreLayerWriting;

static char *groupCompressName(char *name,char *base)
{
    char    *s,*s2;

    s = strstri(name,base);
    s2 = s + strlen(base);
    if (s && (*s2 == '/' || *s2 == '\\'))
        s += strlen(base) + 1;
    else
        s = name;
    return s;
}

static char *modelName(Model *model)
{
    static    char    buf[1000];
    char    *s;

    s = strchr(model->gld->name,'/');
    if (!s)
        s = model->gld->name;
    else
        s++;
    strcpy(buf,s);
    s = strrchr(buf,'/');
    if (!s)
        s = buf + strlen(s);
    else
        s++;
    strcpy(s,model->name);
    return buf;
}

static safePrintVec3(char **filedatap, char *name, Vec3 v, int indent, int degrees)
{
    int        i;
    char    buf[100];

    for(i=0;i<indent;i++)
        estrConcatChar(filedatap, '\t');
    estrConcatCharString(filedatap, name);
    for(i=0;i<3;i++)
    {
        F32 f = v[i];
        if(degrees)
        {
            f = DEG(f);
            if(0)
                f = round(f*4.f) * 0.25f;
        }
        safe_ftoa(f, buf);
        estrConcatf(filedatap, " %s", buf);
    }
    estrConcatChar(filedatap, '\n');
}

static void writeMat(char **filedatap,const Mat4 mat_in,int indent)
{
    int        i,unit,zero,save_scale=0;
    Vec3    pyr,scale;
    Mat4    mat;

    copyMat4(mat_in,mat);
    unitZeroMat(mat,&unit,&zero);
    if (!unit)
    {
        extractScale(mat,scale);
        if (!nearSameVec3(scale,onevec3))
        {
            for(i=0;i<3;i++)
                scaleVec3(mat[i],1.f/scale[i],mat[i]);
            save_scale = 1;
        }
        getMat3YPR(mat,pyr);
        safePrintVec3(filedatap,"PYR",pyr,indent,1);
    }
    if (!zero)
        safePrintVec3(filedatap,"Pos",mat[3],indent,0);
}

static char *noTrickName(char *s,char *buf)
{
    strcpy(buf,s);
    s = strstr(buf,"__");
    if (s)
        *s = 0;
    return buf;
}

//// TO DO : fix up layer change writing ............................
typedef struct LayerInfo
{
    DefTracker * ref;
    char * fileData;
    char layerFileName[256];
} LayerInfo;

LayerInfo    layers[256];
int            layerCount = 0;


void setUpLayerFileToWriteTo(DefTracker *ref, char *fnamebuf)
{
    char givenName[256];
    char *    layerName;
    char layerFileName[256];
    char * suffix;
    char fname[256];        //using this so we can destroy part of it in the case of autosave file names
    //int    checkoutSuccess = 0;
    PropertyEnt *layerProp, *shareProp;
    LayerInfo * li = 0;
    int autosave,clientsave;

    if(!stashFindPointer(ref->def->properties, "Layer", &layerProp))
        return;

    strcpy(fname,fnamebuf);
    autosave=strstr(fname,".autosave")!=NULL;
    clientsave=strstr(fname,".clientsave")!=NULL;
    if (autosave)
        strcpy(strstr(fname,".autosave"),strstr(fname,".autosave")+9);    //cuts out the ".autosave"
    if (clientsave)
        strcpy(strstr(fname,".clientsave"),strstr(fname,".clientsave")+11);    //cuts out the ".clientsave"

    //MW changed layerNmae from the value of the property to the name of the def.
    extractGivenName( givenName,ref->def->name );
    if( !givenName[0] )
    {
        Errorf( "The Group %s is tagged as a layer, but you didn't name it. Do that, and save it again", ref->def->name );
        layerName = "RenameMe";
    }
    else
    {
        layerName = givenName;
    }

    if(stashFindPointer(ref->def->properties, "SharedFrom", &shareProp))
    {
        char *s = strstri(shareProp->value_str, "_layer_");
        if(!s)
        {
            Errorf("Something weird happened.  It looks like a non-layer was imported as a shared layer (%s does not contain _layer_).  Please report this to a programmer.  Saving the layer locally instead.",
                    shareProp->value_str);
            shareProp = NULL;
        }
        if(strncmp(shareProp->value_str, fnamebuf, strstri(fnamebuf, ".txt") - fnamebuf) == 0)
        {
            Errorf("Something weird happened.  It looks like a local layer was imported as a shared layer (Layer: %s  Map: %s).  Please report this to a programmer.  Saving the layer locally instead.",
                    shareProp->value_str, fnamebuf);
            shareProp = NULL;
        }
        if(strnicmp(s+strlen("_layer_"), givenName, strlen(givenName)) != 0)
        {
            Errorf("Changing the name of SharedLayers is not supported (File: %s  New Name: %s).  Saving the layer locally instead. You may want to delete the layer and re-import it or get a programmer.",
                    shareProp->value_str, givenName);
            shareProp = NULL;
        }
    }

    if(!shareProp)
    {
        //for Layer file names ".../City_01_01.txt" becomes ".../City_01_01_layer_layerName.txt"
        strcpy(layerFileName, fname);
        suffix = strstri( layerFileName, ".txt" );
        if (autosave) {
            printf("autosaving layer %s\n",layerName);
            sprintf( suffix, "_layer_%s.autosave.txt", layerName );
        } else if (clientsave) {
            printf("clientsaving layer %s\n",layerName);
            sprintf( suffix, "_layer_%s.clientsave.txt", layerName );
        } else {
            sprintf( suffix, "_layer_%s.txt", layerName );
            printf("saving layer %s\n",layerName);
        }
    }
    else
    {
        strcpy(layerFileName, fname);
        forwardSlashes(layerFileName);
        if(!(suffix = strstri(layerFileName, "maps/")))
            suffix = layerFileName;
        strcpy(suffix, shareProp->value_str);
        suffix = strstri(layerFileName, ".txt");
        if (autosave) {
            printf("autosaving layer %s\n", shareProp->value_str);
            sprintf( suffix, ".autosave.txt");
        } else if (clientsave) {
            printf("clientsaving layer %s\n", shareProp->value_str);
            sprintf( suffix, ".clientsave.txt");
        } else {
            printf("saving layer %s\n", shareProp->value_str);
//            sprintf( suffix, ".txt");
        }
    }

    li = &layers[ layerCount++ ];
    assert( layerCount < 200 ); 
    memset( li, 0, sizeof(LayerInfo) );
    strcpy( li->layerFileName, layerFileName );
    li->ref = ref;


#ifdef SERVER
    //TO DO if no changes have been made to this layer, don't try to write to it
    if( !groupRefShouldAutoGroup(ref) || server_state.doNotAutoGroup)
    {
        //don't try to write anything if it's autogrouped
        //li->file = fileOpen(layerFileName,"w+b");
        estrCreate(&li->fileData);
    }
#endif
}


char **getLayerFileToWriteTo(GroupDef *def, char **layerFileName)
{
    LayerInfo * li;
    int i;

    for( i = 0 ; i < layerCount ; i++ )
    {
        li = &layers[ i ];
        if( li && li->ref->def == def )
        {
            if( layerFileName )
                *layerFileName = li->layerFileName;
            return &li->fileData;
        }
    }
    return 0;
}

static void saveDef(char **filedatap,GroupDef *def,int map_def)
{
    char        *s,buf[1000],basename[1000],def_savename[1000],defname[1000];
    int            j;
    GroupEnt    *ent;

    groupDefGetFullName(def,SAFESTR(defname));
    groupBaseName(def->file->fullname,basename);
    if (map_def)
        strcpy(def_savename,def->name);
    else
        strcpy(def_savename,groupCompressName(defname,basename));
    if (groupInLib(def) && def->model && stricmp(def->name,noTrickName(def->model->name,buf))==0)
        estrConcatf(filedatap,"RootMod %s\n",def_savename);
    else
    {
        estrConcatf(filedatap,"Def %s\n",def_savename);
        if (def->model)
        {
        char    buf[256];

            strcpy(buf,modelName(def->model));
            s = strstr(buf,"__");
            if (s)
                *s = 0;
            estrConcatf(filedatap,"\tObj %s\n",groupCompressName(buf,basename));
        }
    }
    for(j=0;j<def->count;j++)
    {
        ent = &def->entries[j];

        //10.18.04 This fixes the problem that An entry to a def that's been 
        //culled (such as because its model doesn't exist) will be still be written.
        //Normally no big deal, but this can cause crazy problems if you have a dupe library piece (w/ noErrorCheck)
        //The degenerate def didn't get renamed in the duplication, so now you have a map with entry called 
        //"such_n_so&" as though it's in the object library.  
        if( ent->def->in_use == 0 ) 
            continue;

        groupDefGetFullName(ent->def,SAFESTR(defname));
        if (!groupInLib(ent->def))
            estrConcatf(filedatap,"\tGroup %s\n",ent->def->name);
        else
            estrConcatf(filedatap,"\tGroup %s\n",groupCompressName(defname,basename));
        writeMat(filedatap,ent->mat,2);
        estrConcatStaticCharArray(filedatap,"\tEnd\n");
    }
    if (def->has_tint_color)
        estrConcatf(filedatap, "\tTintColor %d %d %d   %d %d %d\n",def->color[0][0],def->color[0][1],def->color[0][2],def->color[1][0],def->color[1][1],def->color[1][2]);
    for(j=0;j<2;j++)
    {
        if (def->has_tex & (1<<j) && def->tex_names[j]) // If this is NULL, the has_tex flag was set because of a TexWord
            estrConcatf(filedatap, "\tReplaceTex %d %s\n",j,def->tex_names[j]);
    }
    if (def->has_alpha)
    {
        estrConcatf(filedatap,"\tAlpha %f\n",def->groupdef_alpha);
    }
    if (def->type_name && def->type_name[0])
        estrConcatf(filedatap,"\tType %s\n",def->type_name);

    {
        int lodfar,lodnear,lodscale;

        lodfar = def->model && (!def->lod_autogen && !def->lod_fromtrick);
        lodnear = def->model && (def->lod_near && !def->lod_fromtrick);
        lodscale = def->lod_scale && def->lod_scale != 1 && !def->lod_fromtrick;

        if (lodfar || lodnear || lodscale)
        {
            estrConcatStaticCharArray(filedatap,"\tLod\n");
            if (lodfar)
            {
                estrConcatf(filedatap,"\t\tFar %f\n",def->lod_far);
                estrConcatf(filedatap,"\t\tFarFade %f\n",def->lod_farfade);
            }
            if (lodnear)
                estrConcatf(filedatap,"\t\tNear %f\n",def->lod_near);
            if (lodscale)
                estrConcatf(filedatap,"\t\tScale %f\n",def->lod_scale);
            estrConcatStaticCharArray(filedatap,"\tEnd\n");
        }
    }
    if (def->has_ambient)
        estrConcatf(filedatap,"\tAmbient %d %d %d\n",def->color[0][0],def->color[0][1],def->color[0][2]);
    if (def->has_light)
    {
        estrConcatf(filedatap,"\tOmni %d %d %d %f %s\n",def->color[0][0],def->color[0][1],def->color[0][2],def->light_radius,(def->color[0][3] & 1) ? "Negative" : "");
    }
    if (def->has_cubemap)
    {
        estrConcatf(filedatap,"\tCubemap %d %d %f %f\n",def->cubemapGenSize, def->cubemapCaptureSize,
            def->cubemapBlurAngle, def->cubemapCaptureTime);
    }
    if (def->has_volume)
    {
        estrConcatf(filedatap,"\tVolume %f %f %f\n",def->box_scale[0],def->box_scale[1],def->box_scale[2]);
    }
    if (def->has_sound)
        estrConcatf(filedatap,"\tSound %s %f %f %f %s\n",def->sound_name,def->sound_vol / 128.f,def->sound_radius,def->sound_ramp,def->sound_exclude ? "Exclude" : "");
    if (def->lib_ungroupable || def->lod_fadenode || def->no_fog_clip || def->no_coll)
    {
        estrConcatStaticCharArray(filedatap, "\tFlags");
        if (def->lib_ungroupable)
            estrConcatStaticCharArray(filedatap, " Ungroupable");
        if (def->lod_fadenode==1)
            estrConcatStaticCharArray(filedatap, " FadeNode");
        if (def->lod_fadenode==2)
            estrConcatStaticCharArray(filedatap, " FadeNode2");
        if (def->no_fog_clip)
            estrConcatStaticCharArray(filedatap, " NoFogClip");
        if (def->no_coll)
            estrConcatStaticCharArray(filedatap, " NoColl");
        estrConcatChar(filedatap, '\n');
    }
    if (def->has_fog)
        estrConcatf(filedatap,"\tFog %f %f %f  %d %d %d  %d %d %d  %d\n",def->fog_radius,def->fog_near,def->fog_far,
            def->color[0][0],def->color[0][1],def->color[0][2],
            def->color[1][0],def->color[1][1],def->color[1][2],
            def->fog_speed);
    if (def->has_beacon)
        estrConcatf(filedatap, "\tBeacon %s %f\n", def->beacon_name, def->beacon_radius);
    if (def->has_properties)
        WriteGroupPropertiesToMemSimple(filedatap, def->properties);

//new texture swapping code
    for(j = 0; j < eaSize(&def->def_tex_swaps); j++)
        estrConcatf(filedatap, "\tTexSwap %s %s %i\n",def->def_tex_swaps[j]->tex_name,def->def_tex_swaps[j]->replace_name,def->def_tex_swaps[j]->composite);
    if (def->sound_script_filename)
        estrConcatf(filedatap, "\tSoundScript %s\n",def->sound_script_filename);

//end new texture swapping code

    estrConcatStaticCharArray(filedatap,"End\n\n");
    def->save_temp = 1;
    def->saved = 1;
}

static int unflag(GroupDef * def, Mat4 parent_mat)
{
    if (!def) return 1;
    def->save_temp=0;
    return 1;
}

static void mapSaveDef( GroupDef *def, char **filedatap, char * fname )
{
    int            j;
    GroupEnt    *ent;
    char        **layerFileDatap = 0, **rootFileDatap = 0; 

    if (!def || def->save_temp || !def->in_use || groupInLib(def))
        return;

    //Layer : switch to new file to write to If there have been no changes to this layer, or you couldn't check it out, you are done.
    if(!ignoreLayerWriting && stashFindPointer(def->properties, "Layer", NULL))
    {
        PropertyEnt *shared;
        char *layerFileName;
        char *layerFileNameRelative;

        stashFindPointer(def->properties, "SharedFrom", &shared);

        // we have to unflag the defs so they will get saved again, in the event
        // that this layer has a def that was already saved in another layer
        groupProcessDef(&group_info,unflag);

        //Write future updates to the layer file till you leave this function
        layerFileDatap = getLayerFileToWriteTo(def, &layerFileName);

        //Somewhat hacky way to get relative name
        layerFileNameRelative = strstri( layerFileName, "maps/" ) ;
        if( !layerFileNameRelative )
            layerFileNameRelative = strstri( layerFileName, "maps\\" );
        if( !layerFileNameRelative )
            layerFileNameRelative = layerFileName;

        //Write to the main file the include command
        estrConcatf( filedatap, "Import %s\n", layerFileNameRelative );

        if( !layerFileDatap )
            return;
        rootFileDatap = filedatap; 
        filedatap = layerFileDatap;
    } 
    //End (you can't return  below this point without checking to see if you restore the file ptr.
    
    //Write the entries
    for( ent = def->entries, j=0 ; j<def->count ; j++,ent++ )
    {
        if (ent->def)
            mapSaveDef( ent->def, filedatap, fname );
    }
    saveDef(filedatap,def,1);
}

#define BACKUP_TIME_TO_KEEP (60*60*24*14) //     in seconds


static int writeToDiskAndFree(char *fname,char **filedatap)
{
    FILE    *real_file;
    char*    realFilename;
    char    buf[MAX_PATH];
    int ret;

    fileMakeLocalBackup(fname, BACKUP_TIME_TO_KEEP);
    realFilename = fileLocateWrite(fname, buf);
    mkdirtree(realFilename);
    real_file = fileOpen(fname,"wb");
    if (!real_file)
    {
        log_printf("groupsave.log","Couldn't open %s for writing\n",fname);
        printf("%s:\n  Couldn't open for writing. (ignore if you weren't editing it)\n",fname);
        return 0;
    }
    ret=fwrite(*filedatap,estrLength(filedatap),1,real_file);

    if (!ret)
    {
        char buf[2048];
        sprintf(buf,"Failed to save %s.  lastWinErr()=%s",fname,lastWinErr());
        buf[strlen(buf)-2]=0;    // trims crap off end of lastWinErr()
        log_printf("groupsave.log","%s",buf);
        assert((!"Save failed - get Jonathan"));
    }

    if (fileLastChanged(fname)+30<time(NULL))
    {
        char buf[2048];
        sprintf(buf,"Failed to save %s.  Timestamp did not change after saving.",fname);
        log_printf("groupsave.log","%s",buf);
        assert((!"Save failed - get Jonathan"));
    }

    estrDestroy(filedatap);
    fclose(real_file);
    return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef TEST_CLIENT
#include "group/groupMetaMinimap.h"
#endif
static int groupSaveMapInternal(char *fname,DefTracker **trackers,int refCount,char **filedatap)
{
    int                i,size;
    DefTracker        *ref;
    char            basename[256],defname[256],*filedata=0;//,**filedatap=&filedata;
    const char *s;
    GroupFile        *gfile;
    bool            bIsAutoSave = (fname && (strstr(fname, ".autosave") || strstr(fname, ".clientsave")));

    if(fname && !bIsAutoSave)
        minimap_saveHeader(fname);

    if (!group_info.files)
        groupFileNew();
    gfile = group_info.files[0];
    gfile->modified_on_disk = 1;

    if (!filedatap)
        filedatap = &filedata;
    //If file is itself a layer that you are editing, ignore all the other layers stuff and just save it the old way.
    //Also, library pieces know nothing about layers
    if( !fname || strstri( fname, "_layer_" ) )
        ignoreLayerWriting = 1;
    else
        ignoreLayerWriting = 0;

    //TO DO : if ignore layer writing is true, make sure there's only only one layer, and that layer is the same name
    //as the file.

    layerCount = 0;

    //Layer : Make a list of all the layers that have changed, and you could check out
    if( !ignoreLayerWriting )
    {
        int i;

        for( i=0 ; i<refCount ; i++ )
        {
            ref = trackers[i];

            //If this is a layer and has changed, try to check it out.
            if(ref && ref->def)
                setUpLayerFileToWriteTo(ref, fname); // does nothing if not a layer
        }
    }

    //So each def is only written once, gets unset if we start a new layer, however,
    //since each layer needs to have a full definition of each def
    for(i=0;i<gfile->count;i++)
        gfile->defs[i]->save_temp = 0;

    //Write header
    estrConcatStaticCharArray(filedatap, "Version 2\n");

    
    if ( group_info.scenefile[0] ) 
        estrConcatf(filedatap,"Scenefile %s\n\n",group_info.scenefile);

    if ( group_info.loadscreen[0] ) 
        estrConcatf(filedatap,"Loadscreen %s\n\n",group_info.loadscreen);


    //Save defs referred to by refs
    for( i=0 ; i < refCount ; i++ )
    {
        if (trackers[i])
            mapSaveDef(trackers[i]->def,filedatap,fname);
    }

    //Save refs
    for( i=0 ; i<refCount ; i++ )
    {
        ref = trackers[i];

        if (!ref || !ref->def || !ref->def->in_use)
            continue;

        //Don't save autogrouped stuff
        //if( 0 == strnicmp( ref->def->name, "grpa", 4 ) )
        //    continue;//assertmsg( 0, "Trying to save an auto grouped group. Should be able to get here");

        groupDefGetFullName(ref->def,SAFESTR(defname));
        if (groupInLib(ref->def))
        {
            groupBaseName(fname,basename);
            s = groupCompressName(defname,basename);
        }
        else
            s = ref->def->name;

        //Layer : If this is a layer's ref, write to layer file, if needed/possible
        //(otherwise just write to the main file)
        {
            char **rootFileDatap = filedatap;
            if(!ignoreLayerWriting && stashFindPointer(ref->def->properties, "Layer", NULL))
                filedatap = getLayerFileToWriteTo( ref->def, 0 );

            if( filedatap )
            {
                estrConcatf(filedatap,"Ref %s\n",s);
                writeMat(filedatap,ref->mat,1);
                estrConcatStaticCharArray(filedatap, "End\n\n");
            }
            filedatap = rootFileDatap;
        }
    }

    //Layer : Close all the layer files
    { int i; for( i=0 ; i < layerCount ; i++ ) {
        if( layers[ i ].fileData )
        {  //If this layer was changed and grouped
            writeToDiskAndFree(layers[i].layerFileName,&layers[i].fileData);
        }
    }}  
    //End Layers


    //////////////Close file
    estrConcatStaticCharArray(filedatap, "\n\n"); //parser needs this to not be confused by the "include" command
    size = estrLength(filedatap);
    if (fname)
    {
        writeToDiskAndFree(fname,filedatap);
    }
    group_info.lastSavedTime = ++group_info.ref_mod_time;
    return size;
}

int groupSaveToMem(char **str_p)
{
    return groupSaveMapInternal(0,group_info.refs,group_info.ref_count,str_p);
}

int groupSave(char *fname)
{
    if (g_base.rooms)
        return baseToFile(fname,&g_base);
    else
        return groupSaveMapInternal(fname,group_info.refs,group_info.ref_count,0);
}

int groupSaveSelection(char *fname,DefTracker **trackers,int count)
{
    return groupSaveMapInternal(fname,trackers,count,0);
}

static void getSaveOrder(GroupFile *file,GroupDef *def,int *order,U8 *used,int *idx)
{
    int        i;

    if (def->file != file || used[def->id])
        return;
    for(i=0;i<def->count;i++)
    {
        getSaveOrder(file,def->entries[i].def,order,used,idx);
    }
    used[def->id] = 1;
    order[(*idx)++] = def->id;
}

static int saveGroupFile(GroupFile *file)
{
    int        *order,count=0;
    U8        *used;
    int        i;
    char    buf[1000];
    char    *filedata=0;

    file->modified_on_disk = 1;
    used = calloc(sizeof(used[1]),file->count);
    order = calloc(sizeof(order[0]),file->count);
    for(i=0;i<file->count;i++)
    {
        if (groupInLib(file->defs[i]) && !groupIsPrivate(file->defs[i]))
            getSaveOrder(file,file->defs[i],order,used,&count);
    }
    for(i=0;i<count;i++)
        saveDef(&filedata,file->defs[order[i]],0);
    fileLocateWrite(file->fullname,buf);
    mkdirtree(buf);
    writeToDiskAndFree(buf,&filedata);
    free(order);
    free(used);
    return fileSize(buf);
}

int groupSaveLibs()
{
    int            i,bytes_saved=0;
    GroupFile    *file;

    for(i=1;i<group_info.file_count;i++)
    {
        file = group_info.files[i];
        if (!file->unsaved)
            continue;
        bytes_saved += saveGroupFile(file);
        file->unsaved = 0;
    }
    return bytes_saved;
}

int groupSavePrunedLibs()
{
    return groupSaveLibs();
}
