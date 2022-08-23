#define RT_PRIVATE
#include "render/tex.h"
#include "render/model.h"
#include "graphics/font.h"
#include "render/thread/rt_state.h"
#include "group/group.h"
#include "edit/edit_select.h"
#include "win/win_init.h"
#include "edit/edit_cmd.h"
#include "edit_info.h"
#include "group/grouptrack.h"
#include <utilitieslib/utils/mathutil.h>
#include "seq/anim.h"
#include "graphics/vistray.h"
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>
#include "render/thread/rt_init.h"
#include "seq/AutoLOD.h"

EditStats edit_stats;
int editorUndosRemaining;
int editorRedosRemaining;

extern TokenizerParseInfo parse_auto_lod[];

void editGetSelectedLodParams(char *buffer)
{
    buffer[0] = 0;
    if (sel_count == 1)
    {
        DefTracker *tracker = sel_list[0].def_tracker;
        GroupDef    *def = tracker->def;

        if (def->model)
        {
            Model *model = def->model;
            if (model->lod_info)
            {
                int numLODs = eaSize(&model->lod_info->lods), i;

                strcat(buffer, "Trick ");
                strcat(buffer, model->name);
                strcat(buffer, "\n");
                for (i = 0; i < numLODs; i++)
                {
                    char *estr = 0, *str;

                    strcat(buffer, "\tAutoLOD\n");

                    ParserWriteText(&estr, parse_auto_lod, model->lod_info->lods[i], 0, 0);
                    for (str = strtok(estr, "\n\r"); str; str = strtok(0, "\n\r"))
                    {
                        if (strcmp(str, "End")!=0)
                        {
                            strcat(buffer, "\t");
                            strcat(buffer, str);
                            strcat(buffer, "\n");
                        }
                    }
                    estrDestroy(&estr);

                    strcat(buffer, "\tEnd\n");
                }
                strcat(buffer, "End\n");
            }

        }
    }
}

static const char *texNameDetail(const char *texname)
{
    static int i=0;
    static char *buf[4];
    BasicTexture *tex = texFind(texname);
    if (!tex)
        return texname;
    if (tex==tex->actualTexture)
        return tex->name;
    // It's swapped!
    if (!buf[i])
        buf[i] = calloc(256, 1);
    sprintf(buf[i], "%s(SWAPPED from %s)", tex->actualTexture->name, tex->name);
    texname = buf[i];
    i++;
    if (i==ARRAY_SIZE(buf))
        i = 0;
    return texname;
}



static void showObjInfo(int xpos,int ypos,GroupDef *def,float dist,Mat4 mat)
{
    int        i,alpha,x=xpos,y=ypos;
    TexBind    *bind;
    Model *model = def->model;

    if (!model || !(model->loadstate & LOADED))
        return;

    xyprintf(x,y++,"FullName:%s   Sorttype:%s   BlendMode:%s   Flags:%s%s",model->name,
        (model->flags & OBJ_FANCYWATER) ? "WATER" :    (model->flags & OBJ_ALPHASORT) ? "ALPHASORT" : "OPAQUE",
            (model->common_blend_mode.shader<=0)?"various":renderBlendName(model->common_blend_mode),
            (model->flags & OBJ_FULLBRIGHT)?"FULLBRIGHT ":"", (model->flags & OBJ_NOLIGHTANGLE)?"NOLIGHTANGLE ":"");

    if (model->flags & TRICK_NOT_SELECTABLE)
        xyprintf(10,10,"Not selectable");

    y++;

    xyprintf(x,y++, "Distance: %f", dist);

    y++;

    if (model->lod_info)
    {
        U8 r, g, b;

        // AutoLOD
        int numLODs = eaSize(&model->lod_info->lods);

        xyprintf(x,y++, "AutoLODs: %d", numLODs);
        assert(def->auto_lod_models);

        xyprintf(x+5,y,  "LOD #");
        xyprintf(x+15,y, "Tri Count");
        xyprintf(x+30,y, "Lod Near");
        xyprintf(x+44,y, " Lod Far");
        xyprintf(x+60,y, "Allowed Error");
        xyprintf(x+80,y, "Flags");
        y++;
        for (i = 0; i < numLODs; i++)
        {
            if (!def->auto_lod_models || !def->auto_lod_models[i])
                continue;

            if (dist >= model->lod_info->lods[i]->lod_near * game_state.lod_scale && dist <= model->lod_info->lods[i]->lod_far * game_state.lod_scale)
            {
                r = 255; g = 0; b = 0;
            }
            else
            {
                r = 255; g = 255; b = 255;
            }

            xyprintfcolor(x+5, y, r, g, b, "% 5d", i);
            xyprintfcolor(x+15,y, r, g, b, "% 9d", def->auto_lod_models[i]->tri_count);
            xyprintfcolor(x+30,y, r, g, b, "% 8.2f", model->lod_info->lods[i]->lod_near);
            xyprintfcolor(x+44,y, r, g, b, "% 8.2f", model->lod_info->lods[i]->lod_far);
            xyprintfcolor(x+60,y, r, g, b, "% 13.1f", model->lod_info->lods[i]->max_error);
            y++;
        }
    }

    y++;

    for(i=0;i<model->tex_count;i++)
    {
        y++;

        bind = model->tex_binds[i];
        alpha = (model->flags & OBJ_ALPHASORT) ? 1 : 0;

        xyprintf(x   ,y,"%s",texNeedsAlphaSort(bind, bind->bind_blend_mode) ? "ALPHA " : "OPAQUE");
        xyprintf(x+8 ,y,"%s",renderBlendName(bind->bind_blend_mode));
        if (model->blend_modes[i].shader == BLENDMODE_MULTI)
        {
            xyprintf(x+24,y,"%s/%s  Base1:%s Mult1:%s ...",bind->dirname, bind->name, 
                texNameDetail(bind->tex_layers[TEXLAYER_BASE1]->name), 
                texNameDetail(bind->tex_layers[TEXLAYER_MULTIPLY1]->name));
        } else {
            xyprintf(x+24,y,"%s/%s  %s %s",bind->dirname, texNameDetail(bind->name), 
                texNameDetail(bind->tex_layers[TEXLAYER_GENERIC]?bind->tex_layers[TEXLAYER_GENERIC]->name:""), 
                texNameDetail(bind->tex_layers[TEXLAYER_BUMPMAP1]?bind->tex_layers[TEXLAYER_BUMPMAP1]->name:""));
        }
    }
}

void showDefInfo(int xpos,int ypos,GroupDef *def)
{
    char    buf[1000];

    if (!def)
        return;
    strcpy(buf,"DefFlags: ");
    if (def->is_tray)
        strcat(buf,def->outside?"OutsideTray ":"Tray ");
    //if (def->shell)
    //    strcat(buf,"Shell ");
    if (def->vis_window)
        strcat(buf,"Window ");
    if (def->vis_blocker)
        strcat(buf,"VisBlocker ");
    if (def->vis_angleblocker)
        strcat(buf,"VisAngleBlocker ");
    if (def->has_ambient)
        strcat(buf,"Ambient ");
    if (def->has_light)
        strcat(buf,"Light ");
    if (def->has_cubemap)
        strcat(buf,"Cubemap ");
    if (def->has_sound)
        strcat(buf,"Sound ");
    if (def->lod_fromtrick)
        strcat(buf,"LodFromTrick ");
    else if (def->lod_autogen)
        strcat(buf,"LodAutoGen ");
    else if (def->lod_fromfile)
        strcat(buf,"LodFromGroup ");
    if (def->has_beacon)
        strcat(buf,"Beacon ");
    if (def->has_tint_color)
        strcat(buf,"TintColor ");
    if (def->vis_doorframe)
        strcat(buf,"VisDoorFrame ");
    if (def->parent_fade)
        strcat(buf,"ParentFade ");
    if (def->volume_trigger)
        strcat(buf,"VolumeTrigger ");
    if (def->editor_visible_only)
        strcat(buf,"EditorVisibleOnly ");
    xyprintf(xpos,ypos,"%s",buf);
}

void showTrackerInfo(int xpos,int ypos,DefTracker *tracker)
{
    //int        i;
    VisTrayInfo *trayinfo = vistrayGetInfo(tracker);

    if (trayinfo)
    {
        xyprintf(xpos,ypos,"portals: %d   (%p)", trayinfo->portal_count, tracker);
        //for(i=0;i<tracker->portal_count;i++)
        //{
        //    xyprintf(xpos+2,ypos+1+i,"%08x %s",tracker->portals[i],tracker->portals[i] ? tracker->portals[i]->def->name : "unconnected");
        //}
        xyprintf(xpos,++ypos,"details: %d",trayinfo->detail_count);
    }
}


void selShow()
{
    int            i,ypos=46,xpos = 1,pcount=0,ocount=0,grcount=0;
    DefTracker    *ref;
    float        beaconDist = 0;
    float        beaconDistXZ = 0;
    float        yaw = 0;
    float        pitch = 0;

    if (edit_state.quickPlacement)
    {
        Vec3 pyr;
        getMat3YPR(edit_state.quickPlacementMat3, pyr);
        xyprintf(xpos,ypos - 2,"quick placement object: %s", edit_state.quickPlacementObject);
        xyprintf(xpos,ypos - 1,"                   pyr: %1.2f, %1.2f, %1.2f", pyr[0], pyr[1], pyr[2]);
    }
    
    for(i = 0; i < sel_count - 1; i++)
    {
        if(sel_list[i].def_tracker->def->has_beacon && sel_list[i+1].def_tracker->def->has_beacon)
        {
            beaconDist += distance3(sel_list[i].def_tracker->mat[3], sel_list[i+1].def_tracker->mat[3]);
            beaconDistXZ += distance3XZ(sel_list[i].def_tracker->mat[3], sel_list[i+1].def_tracker->mat[3]);
        }
        else
        {
            break;
        }
    }
    
    if(sel_count >= 3){
        for(i = 0; i < 3; i++)
        {
            if(!sel_list[i].def_tracker->def->has_beacon)
            {
                break;
            }
        }
        
        if(i == 3)
        {
            Vec3 diff1, diff2;
            float yaw1, yaw2;
            
            subVec3(sel_list[1].def_tracker->mat[3], sel_list[0].def_tracker->mat[3], diff1);
            subVec3(sel_list[2].def_tracker->mat[3], sel_list[0].def_tracker->mat[3], diff2);
            
            getVec3YP(diff1, &yaw1, &pitch);
            getVec3YP(diff2, &yaw2, &pitch);
            
            yaw = fabs(subAngle(yaw1, yaw2));
        }
    }

    if(beaconDist){
        xyprintf(xpos,ypos - 2,"beacon connection distance: %1.2fft, %1.2fft horiz, yaw: %1.2fdeg", beaconDist, beaconDistXZ, DEG(yaw));
    }

    if (!sel_count)
        return;
    ypos = windowScaleY(ypos);
    for(i=0;i<sel_count;i++)
    {
        if (!sel_list[i].def_tracker)
            continue;
        ref = groupRefFind(sel_list[i].id);
        if (!ref || !ref->def)
        {
            sel_count = 0;
            updatePropertiesMenu = true;
            return;
        }

        if (edit_state.polycount && edit_stats.poly_recount)
        {
            pcount += groupPolyCount(sel_list[i].def_tracker->def);
            ocount += groupObjectCount(sel_list[i].def_tracker->def);
            grcount += 1 + sel_list[i].def_tracker->def->recursive_count;
        }
        if (i<50)
            xyprintf(xpos,ypos+1+i,"%s/%s",sel_list[i].def_tracker->def->file->basename,sel_list[i].def_tracker->def->name);
    }
    if (edit_stats.poly_recount)
    {
        edit_stats.last_polycount = pcount;
        edit_stats.last_objcount = ocount;
        edit_stats.last_totalgroups = grcount;
        edit_stats.poly_recount = 0;
    }

    xyprintf(xpos,ypos,"%d groups (%d tris, %d objects, %d total groups)",sel_count,edit_stats.last_polycount,edit_stats.last_objcount, edit_stats.last_totalgroups);
    if (sel_count == 1)
    {
        DefTracker *tracker = sel_list[0].def_tracker;
        GroupDef    *def = tracker->def;

        if(edit_state.objinfo)
        {
            showDefInfo(xpos+2,ypos+3,sel_list[0].def_tracker->def);

            if(sel_list[0].def_tracker->def->model)
            {
                Vec3 v;
                subVec3(cam_info.cammat[3], tracker->mid, v);
                showObjInfo(xpos+2,ypos+5,def,lengthVec3(v),tracker->mat);
            }
            else
                xyprintf(xpos+2,ypos+5, "No geometry");
            xyprintf(xpos+2,ypos-10, "Radius: %f  Vis dist: %f  UID: %d",def->radius,def->vis_dist, trackerGetUID(tracker));
            showTrackerInfo(xpos+2,ypos-5,sel_list[0].def_tracker);
        }
        else if(game_state.sound_info && def->has_sound)
        {
            bool bIsScript = def->sound_script_filename && strlen(def->sound_script_filename);
            xyprintf(xpos,ypos-2, "Sound: %s(%s), Vol %.2f, Radius: %.1f  Ramp %.1f  %s",
                bIsScript ? def->sound_script_filename : def->sound_name,
                bIsScript ? "SCR" : "SPH",
                def->sound_vol/128.f,
                def->sound_radius,
                def->sound_ramp,
                def->sound_exclude ? "EXCLUDE" : "");
        }
    }
}

static char *getTexName(Model *model,int idx)
{
    int        i,base=0;

    if (!model)
        return "";
    for(i=0;i<model->tex_count;i++)
    {
        if (base <= idx && base + model->tex_idx[i].count > idx)
        {
            char    **texnames = model->gld->texnames.strings;
            char    *texname = texnames[model->tex_idx[i].id];

            return texname;
        }
        base += model->tex_idx[i].count;
    }
    return "";
}

TexBind *getTexWithReplaces(DefTracker *tracker, int idx)
{
    char *texname;
    TexBind *bind;
    DefTracker * original;
    if (tracker==NULL || tracker->def==NULL)
        return 0;
    texname = getTexName(tracker->def->model,idx);
    bind = texFindComposite(texname);
    original=tracker;
    if (bind && bind->tex_layers[TEXLAYER_BASE] && bind->tex_layers[TEXLAYER_BASE]->flags & TEX_REPLACEABLE) {
        // It's a replaceable texture, look up the tree for any replaces
        while (tracker) {
            if (tracker->def && tracker->def->tex_names[0] && tracker->def->tex_names[0][0]) {
                texname = tracker->def->tex_names[0];
                break;
            }
            tracker = tracker->parent;
        }
    }

    //accounting for new texture swapping swaps
    tracker=original;
    {
        while (tracker) {
            int i;
            for (i=0;i<eaSize(&tracker->def->def_tex_swaps);i++) {
                DefTexSwap * dts=tracker->def->def_tex_swaps[i];
                if (strcmp(texname,dts->tex_name)==0)
                    texname=dts->replace_name;
            }
            tracker=tracker->parent;
        }
    }
    //end new stuff

    return texFindComposite(texname);
}

const char *getTexNameWithReplaces(DefTracker *tracker, int idx)
{
    char *texname;
    TexBind *bind;
    DefTracker * original;
    if (tracker==NULL || tracker->def==NULL)
        return "";
    texname = getTexName(tracker->def->model,idx);
    bind = texFindComposite(texname);
    original=tracker;
    if (bind && bind->tex_layers[TEXLAYER_BASE] && bind->tex_layers[TEXLAYER_BASE]->flags & TEX_REPLACEABLE) {
        // It's a replaceable texture, look up the tree for any replaces
        while (tracker) {
            if (tracker->def && tracker->def->tex_names[0] && tracker->def->tex_names[0][0]) {
                texname = tracker->def->tex_names[0];
                break;
            }
            tracker = tracker->parent;
        }
    }

    //accounting for new texture swapping swaps
    tracker=original;
    {
        while (tracker) {
            int i;
            for (i=0;i<eaSize(&tracker->def->def_tex_swaps);i++) {
                DefTexSwap * dts=tracker->def->def_tex_swaps[i];
                if (strcmp(texname,dts->tex_name)==0)
                    texname=dts->replace_name;
            }
            tracker=tracker->parent;
        }
    }
    //end new stuff
    bind = texFindComposite(texname);
    if (bind)
        return bind->name;
    return texname;
}

static void displayTrickNameFromTracker(int x, int y, DefTracker *tracker, int idx)
{
    if (tracker->def->model) {
        TrickNode *trick = tracker->def->model->trick;
        if (trick && trick->info) {
            fontTextf(x,y,"Trick: %s", trick->info->name);
            y-=8;
        }
    }
    if (0) { // Texopts are almost always the same as the texture name, so we don't care
        char *texname = getTexName(tracker->def->model,mouseover_ctri_idx);
        TexBind *bind = texFindComposite(texname);
        if (bind) {
            TexOpt *texopt;
            char    buf[MAX_PATH];
            sprintf(buf, "%s/%s", bind->dirname, bind->name);
            texopt = trickFromTextureName(buf, NULL);
            if (texopt) {
                fontTextf(x,y,"TexOpt: %s", texopt->name);
                y-=8;
            }
        }
    }
}

void printEditStats()
{
    int        count=0,x = TEXT_JUSTIFY + 56*8-40,y = TEXT_JUSTIFY + 53*8;
    char    *axis[] = {"XZ", "ZY", "YX"};
    char    *axis_rot[] = {"Y", "X", "Z"};

    if (!edit_state.gridsize)
        sprintf(edit_stats.gridsize,"Grid 0.5");
    else
        sprintf(edit_stats.gridsize,"Grid %d",(1 << edit_state.gridsize) >> 1);
    sprintf(edit_stats.rotsize,"Deg %2.1f",lock_angs[edit_state.rotsize]);
    if (edit_state.scale)
        sprintf(edit_stats.posrot,"SCALE");
    else
        sprintf(edit_stats.posrot,edit_state.posrot ? "MoveRot" : "MovePos");
    if (edit_state.posrot)
        sprintf(edit_stats.axis,"Axis %s",axis_rot[edit_state.plane]);
    else if(edit_state.plane >= 0 && edit_state.plane < ARRAY_SIZE(axis))
    {
        sprintf(edit_stats.axis,"Axis %s",axis[edit_state.plane]);
        if (edit_state.singleaxis)
            edit_stats.axis[strlen(edit_stats.axis)-1] = 0;
    }
    fontDefaults();
    fontSet(0);
    fontScale(29* 8,24 * 8);
    fontColor(0x000000);
    fontAlpha(0x80);
    fontText(x-8,y-8,"\001");
    fontDefaults();

    //fontTextf(x+0*8,y+2*8,"Geo %s",edit_stats.geo);
    //fontTextf(x+0*8,y+3*8,"Grp %s",edit_stats.group);
    //printDef(groupDefFind(edit_stats.group),3);
    fontTextf(x+0*8,y+1*8,"%-3.2f %-3.2f %-3.2f",edit_stats.cursor_pos[0],edit_stats.cursor_pos[1],edit_stats.cursor_pos[2]);

    fontTextf(x+0*8,y+2*8,"undos/redos %d/%d",editorUndosRemaining,editorRedosRemaining);

    if (edit_state.noerrcheck)
    {
    static int timer;

        timer++;
        if ((timer >> 2)&1)
            fontColor(0xff0000);
        else
            fontColor(0xffff00);
        fontTextf(x+9*8,y+2*8,"noerrcheck");
        fontColor(0xffffff);
    }

    if (edit_state.editorig)
    {
    static int timer;

        timer++;
        if ((timer >> 4)&1)
            fontColor(0xff0000);
        else
            fontColor(0xffff00);
        if (edit_state.editorig == 2)
            fontTextf(x+0*8,y+3*8,"EditLib");
        else
            fontTextf(x+0*8,y+3*8,"EditOrig");
        fontColor(0xffffff);
    }
    else
        fontTextf(x+0*8,y+3*8,"EditCopy");

    if (edit_state.beaconSelect || edit_state.quickPlacement)
    {
    static int timer;

        timer++;
        if ((timer >> 2)&1)
            fontColor(0xff0000);
        else
            fontColor(0xffff00);

        if(edit_state.quickPlacement)
            fontTextf(x+9*8,y+3*8,"QuickPlacing");
        else
            fontTextf(x+9*8,y+3*8,"BeaconSelect");
            
        fontColor(0xffffff);
    }

    if (edit_state.snaptovert)
        fontTextf(x+9*8,y+3*8,"SnapToVert");
        

    fontTextf(x+0*8,y+4*8,edit_stats.posrot);
    fontTextf(x+9*8,y+4*8,edit_stats.snap);

    if(edit_state.useObjectAxes)
        fontColor(0x00ffff);
    fontTextf(x+0*8,y+5*8,edit_stats.gridsize);
    fontTextf(x+9*8,y+5*8,edit_stats.rotsize);
    if(edit_state.useObjectAxes)
        fontColor(0xffffff);

    fontTextf(x+0*8,y+6*8,edit_stats.axis);
    fontTextf(x+9*8,y+6*8,edit_state.snap3 ? "Snap3" : "");
    fontTextf(x+9*8,y+6*8,edit_state.colormemory ? "RgbMem" : "");
    fontTextf(x+9*14,y+6*8,edit_state.ignoretray ? "NoTray" : "");

    if (sel_count == 0 && mouseover_tracker && mouseover_ctri_idx >= 0)
    {
#if 0
        Model    *model = mouseover_tracker->def->model;
        int        i=0,vidx=0;

        xyprintf(5,10,"tri_idx: %d",mouseover_ctri_idx);
#if 0
        if (!rdr_caps.use_vbos && model && model->vbo)
        {
            for(i=0;i<3;i++)
            {
                vidx = model->vbo->tris[mouseover_ctri_idx*3+i];
                xyprintf(2,11+i,"vidx %d",vidx);
                xyprintf(14,11+i,"st0 %f %f",model->vbo->sts[vidx][0],model->vbo->sts[vidx][1]);
                xyprintf(40,11+i,"st1 %f %f",model->vbo->sts2[vidx][0]/model->vbo->sts[vidx][0],model->vbo->sts2[vidx][1]/model->vbo->sts[vidx][1]);
                xyprintf(70,11+i,"vert: %f %f %f",model->vbo->verts[vidx][0],model->vbo->verts[vidx][1],model->vbo->verts[vidx][2]);
            }
        }
#endif
#endif
        {
            const char *texname = getTexNameWithReplaces(mouseover_tracker,mouseover_ctri_idx);
            TexBind *texbind = texFindComposite(texname);
            fontTextf(x+0,y-1*8-2,"%s",getTexNameWithReplaces(mouseover_tracker,mouseover_ctri_idx));
            if (texbind) {
                BasicTexture *tex = texbind->tex_layers[TEXLAYER_BASE];
                if (tex && tex->actualTexture != tex) {
                    tex = tex->actualTexture;
                    fontColor(0xffffff00);
                    fontTextf(x+0,y-0*8-2,"SWAPPED: %s",tex->name);
                    fontColor(0xffffffff);
                } else if (tex) {
                    fontTextf(x+0,y-0*8-2,"Base: %s",tex->name);
                }
            }
        }
        displayTrickNameFromTracker(x+0, y-2*8-2, mouseover_tracker,mouseover_ctri_idx);
    }
    if (sel_count == 1)
    {
    DefTracker    *tracker;

        tracker = sel_list[0].def_tracker;
        if (!tracker->def->count)
        {
            fontTextf(x+0*8, y-1*8,"Near  %3.1f",tracker->def->lod_near);
            fontTextf(x+12*8,y-1*8,"Far   %3.1f",tracker->def->lod_far);
            fontTextf(x+0*8, y+0*8,"NearF %3.1f",tracker->def->lod_nearfade);
            fontTextf(x+12*8,y+0*8,"FarF  %3.1f",tracker->def->lod_farfade);
        }
        else
            fontTextf(x+0,y-1*8,"LodScale % -3.2f",tracker->def->lod_scale);
    }
    
    if (sel.group_parent_def)
    {
    int            i,depth,idxs[100],y = 41;
    GroupDef    *def;
    DefTracker    *ref;

        xyprintf(1,y+20,"Default parent:");
        depth = trackerPack(sel.group_parent_def,idxs,NULL,0);
        ref = groupRefFind(sel.group_parent_refid);
        def = ref->def;
        if (def)
        {
            for(i=0;i<depth+1;i++)
            {
                xyprintf(1+i,y+21+i,"%s",def->name);
                if (i < depth)
                    def = def->entries[idxs[i]].def;
            }
        }
    }            
}

void printDef(GroupDef *def,int xoff)
{
    int    i;

    if (!def)
        return;

    if (!def)
        return;
    for(i=0;i<def->count;i++)
    {
        if (def->entries[i].def)
        {
            printDef(def->entries[i].def,xoff + 10);
            xyprintf(xoff,8+i,"%s",def->entries[i].def->name);
        }
    }
}
