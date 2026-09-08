#include <string.h>
#include "graphics/sun.h"
#include "graphics/camera.h"
#include "seq/gfxtree.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiConsole.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/file.h>
#include "seq/anim.h"
#include "group/group.h"
#include "graphics/light.h"
#include "edit/edit_cmd.h"
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/error.h>
#include "render/render.h"
#include "assert.h"
#include "seq/anim.h"
#include "seq/tricks.h"
#include "win/win_init.h"
#include "group/groupfilelib.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "assert.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/utils.h>
#include "graphics/font.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/rgb_hsv.h>
#include "graphics/groupdraw.h"
#include "graphics/fog.h"
#include <utilitieslib/utils/timing.h>
#include "render/renderprim.h"
#include "render/renderstats.h"
#include "player/player.h"
#include "entity/entity.h"
#include "render/tex_gen.h"
#include "render/renderEffects.h"
#include "group/groupnetrecv.h"
#include <utilitieslib/utils/structHist.h>
#include <utilitieslib/components/EString.h>
#include "LWC.h"

// Sky value blending rules:
//  Scene file may reference multiple sky files
//    blend sky file values together to get effect from the sky files
//  Fog:
//    scene file override overrides both indoor lighting values and sky values
//    indoor lighting values override blended sky values

extern int sky_gfx_tree_inited;

#define SKY_NAMESIZE 64
#define MAXCLOUDS 8
#define MAXMOONS 32

#define SKY_TREE    0
#define GENERAL_TREE 1

#define INSIDE_SHADOW_ALPHA 0.35

#define DEFAULT_VALUE (-99997)

typedef struct SunTime
{
    F32        time;
    Vec3    ambient;
    Vec3    diffuse;
    Vec3    diffuse_reverse;    // 12/2009 deprecated
    Vec3    lightmap_diffuse;
    Vec3    fogcolor;
    Vec3    highfogcolor;
    Vec3    backgroundcolor;
    Vec2    fogdist;
    DOFValues dof_values;
    Vec4    shadowcolor;
    char    *skyname;
    GfxNode    *sky_node;
    Vec3    skypos;
    F32        moon_scales[MAXMOONS];
    F32        moon_alphas[MAXMOONS];
    F32        luminance;
} SunTime;


typedef struct
{
    char    *name;
    Vec2    heights;
    Vec2    fadetimes;
    F32        fademin;
} Cloud;

//This should be combined with ubersky into one structure, not a member of ubersky
typedef struct
{
    TokenizerParams **moon_names;

    Vec2            lamp_lighttimes;

    Vec2            cloud_fadetimes;
    F32                cloud_fademin;

    Vec2            fogheightrange;
    Vec2            fogdist;

    float            lightmap_time;
    float            lightmap_sun_angle;
    float            lightmap_sun_brightness;
    float            lightmap_sky_brightness;

} Sky;

typedef struct FogVals
{
    F32        dist[2];
    Vec3    color;
    U8        valid;
} FogVals;

typedef struct SkyWorkingValues
{
    FogVals fog_curr;
    SunLight sun_work;
} SkyWorkingValues;

TokenizerParseInfo SunLightParseInfo[] =
{
    { ".",    TOK_VEC4(SunLight, diffuse)},
    { ".",    TOK_VEC4(SunLight, diffuse_reverse)},
    { ".",    TOK_VEC4(SunLight, ambient)},
    { ".",    TOK_VEC4(SunLight, ambient_for_players)},
    { ".",    TOK_VEC4(SunLight, diffuse_for_players)},
    { ".",    TOK_VEC4(SunLight, diffuse_reverse_for_players)},
    { ".",    TOK_VEC4(SunLight, no_angle_light)},
    { ".",    TOK_F32(SunLight, luminance, 0)},
    { ".",    TOK_F32(SunLight, bloomWeight, 0)},
    { ".",    TOK_F32(SunLight, toneMapWeight, 0)},
    { ".",    TOK_F32(SunLight, desaturateWeight, 0)},
    { ".",    TOK_VEC4(SunLight, direction)},
    { ".",    TOK_VEC4(SunLight, direction_in_viewspace)},
    { ".",    TOK_VEC3(SunLight, shadow_direction)},
    { ".",    TOK_VEC3(SunLight, shadow_direction_in_viewspace)},
    { ".",    TOK_F32(SunLight, gloss_scale, 0)},
    { ".",    TOK_VEC3(SunLight, position)},
    { ".",    TOK_F32(SunLight, lamp_alpha, 0)},
    { ".",    TOK_U8(SunLight, lamp_alpha_byte, 0)},
    { ".",    TOK_VEC4(SunLight, shadowcolor)},
    { ".",    TOK_VEC2(SunLight, fogdist)},
    { ".",    TOK_VEC3(SunLight, fogcolor)},
    { ".",    TOK_VEC3(SunLight, bg_color)},
    { ".",    TOK_F32(SunLight, dof_values.focusDistance, 0)},
    { ".",    TOK_F32(SunLight, dof_values.focusValue, 0)},
    { ".",    TOK_F32(SunLight, dof_values.nearValue, 0)},
    { ".",    TOK_F32(SunLight, dof_values.nearDist, 0)},
    { ".",    TOK_F32(SunLight, dof_values.farValue, 0)},
    { ".",    TOK_F32(SunLight, dof_values.farDist, 0)},

    { "", 0, 0 },
};

TokenizerParseInfo SkyWorkingValuesParseInfo[] = // Only used for in-memory copies/math
{
    { ".",    TOK_VEC2(SkyWorkingValues, fog_curr.dist)    },
    { ".",    TOK_VEC3(SkyWorkingValues, fog_curr.color)    },
    { ".",    TOK_U8(SkyWorkingValues, fog_curr.valid, 0)    },
    { ".",    TOK_EMBEDDEDSTRUCT(SkyWorkingValues, sun_work, SunLightParseInfo)        },
    { "", 0, 0 }
};


//UberSky and Sky should themselves be merged
// stuct that holds the items read from the menu at the highest level
// leads to fun names like sky.sky_struct
typedef struct UberSky
{
    Sky        sky_struct;
    int        moon_count;        //Sun and moon(s), (number of moonNames) in the sky. sky[0]->moon_names. Kinda weird that it's not in the sky struct, but since there can be only one sky struct, we're fine

    Cloud    **clouds;        //sky geometry, including the skybowl
    int        cloud_count;    
    
    SunTime **times;        //time of day
    int        sun_time_count; //Number of times of day described in the <sun>.txt file

    SkyWorkingValues sky_work;

    GfxNode *moonNodes[MAXMOONS][2];    //[2] to add the glow
    GfxNode *cloudNodes[MAXCLOUDS];
    GfxNode *sunflare_node_ptr;                //Ptr to moonNodes[i][1], the lens flare node.
    bool    fixed_sun_pos;
    int        sunflare_node_ptr_id;
    float    sunflare_alpha;
    bool    ignoreLightClamping;

} UberSky;



extern SceneInfo scene_info;
//static UberSky g_sky;                //Parsed Sky file.
static UberSky **g_skies;
SunLight g_sun;

static FogVals fog_final;

static int luminance_valid = 0;
static int dof_values_valid = 0;
static int sun_values_valid = 0;

static int skyFadeOverride = 0;
static int skyFadeOverride1;
static int skyFadeOverride2;
static F32 skyFadeOverrideWeight;


//Ptrs to the various sky nodes in the sky tree for easy updating


F32 fixTime(F32 a)
{
    while(a >= 24.f)
        a -= 24.f;
    while(a < 0)
        a += 24.f;
    return a;
}


static void hackmoon(const UberSky IN *sky, SunLight OUT *sun, int idx, F32 distadd, F32 rot, const Vec3 cam_pos, GfxNode OUT *moon, F32 scale, F32 alpha, TokenizerParams **moon_params)
{
    int        num_params,i;
    Mat4    mat,mmat;
    struct
    {
        F32        speed;
        F32        pyr[3];
    } moon_types[MAXMOONS] = 
    {
        { 1, { 0, RAD(112.5), RAD(180)} },
        { 1, { -RAD(35), 0, 0} },
        { 1, { RAD(185), RAD(75), RAD(35)} },
        { 1.3, { RAD(180), RAD(-45), 0} },
        { 2, { RAD(180), RAD(90), RAD(-45)} },
        { 5, { RAD(5), 0, RAD(80)} },
        { 0.5, { RAD(10), RAD(70), RAD(-80)} },
        { 1, { RAD(15), RAD(-65), 0} },

        { 1, { 0, RAD(112.5), RAD(180)} },
        { 1, { 0, RAD(112.5), RAD(160)} },
        { 1, { 0, RAD(112.5), RAD(200)} },
        { 1, { 0, RAD(112.5), RAD(140)} },
        { 1, { 0, RAD(112.5), RAD(220)} },
        { 1, { 0, RAD(112.5), RAD(120)} },
        { 1, { 0, RAD(112.5), RAD(260)} },
        { 1, { 0, RAD(112.5), RAD(100)} },

        { 1, { 0, RAD(12.5), RAD(180)} },
        { 1, { 0, RAD(12.5), RAD(160)} },
        { 1, { 0, RAD(12.5), RAD(200)} },
        { 1, { 0, RAD(12.5), RAD(140)} },
        { 1, { 0, RAD(12.5), RAD(220)} },
        { 1, { 0, RAD(12.5), RAD(120)} },
        { 1, { 0, RAD(12.5), RAD(260)} },
        { 1, { 0, RAD(12.5), RAD(100)} },

        { 1, { RAD(180), RAD(12.5), RAD(180)} },
        { 1, { RAD(180), RAD(12.5), RAD(160)} },
        { 1, { RAD(180), RAD(12.5), RAD(200)} },
        { 1, { RAD(180), RAD(12.5), RAD(140)} },
        { 1, { RAD(180), RAD(12.5), RAD(220)} },
        { 1, { RAD(180), RAD(12.5), RAD(120)} },
        { 1, { RAD(180), RAD(12.5), RAD(260)} },
        { 1, { RAD(180), RAD(12.5), RAD(100)} },
    },*mtype;

    if (!moon)
        return;

    mtype = &moon_types[idx]; 
    
    num_params = eaSize((&(*moon_params)->params));

    if( idx != 0 || num_params > 1 ) //The sun gets broken by this
    {
        if (num_params > 1)
            mtype->speed = atof((*moon_params)->params[1]);
        for(i=0;i<3;i++)
        {
            if (i+2 < num_params)
                mtype->pyr[i] = RAD(atof((*moon_params)->params[2+i]));
        }
    }

    copyMat3(unitmat,mat);
    copyVec3(cam_pos,mat[3]);
    yawMat3(mtype->pyr[1],mat);
    rollMat3(mtype->pyr[2],mat);
    pitchMat3(mtype->speed * rot - PI + mtype->pyr[0],mat);

    copyMat4(unitmat,mmat);
    pitchMat3(RAD(-90),mmat);
    mmat[3][1] = 7500 + distadd;
    mmat[3][0] = 3000; 

    mulMat4(mat,mmat,moon->mat);
    scaleMat3(moon->mat,moon->mat,scale);

    {
        SkyNode *node = dynArrayAddStruct(&sun->sky_node_list.nodes, &sun->sky_node_list.count, &sun->sky_node_list.max);
        node->node = moon;
        node->alpha = alpha;
        if (moon == sky->sunflare_node_ptr)
        {
            // Special logic for sunflare
            node->alpha = sky->sunflare_alpha;
            node->snap = true;
        }
        else
        {
            gfxNodeSetAlpha(moon,0,1); // Hide it (will be unhidden later)
        }
    }
}


static GfxNode *sunAddNode(char *name, int treeToInsertInto )
{
    GfxNode        *node;
    char        *anm_file;

    if( treeToInsertInto == SKY_TREE )
        node = gfxTreeInsertSky( 0 );
    else//  if( treeToInsertInto == GENERAL_TREE )
        node = gfxTreeInsert( 0 );

    anm_file = objectLibraryPathFromObj(name);
    if(!anm_file)
    {
        printf("\nSky builder can't find '%s' in the object_libray.\n", name);
    }
    node->model = modelFind(name, anm_file, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);

    if(node->model) {
        gfxTreeInitGfxNodeWithObjectsTricks(node, node->model->trick);
        node->trick_from_where = TRICKFROM_MODEL;
    }
    
    copyMat4(unitmat,node->mat);

    node->flags |= GFXNODE_SKY;

    return node;
}

void sunGlareDisableInternal(UberSky *sky)
{
    if( sky->sunflare_node_ptr )
        gfxNodeSetAlpha(sky->sunflare_node_ptr,0,1);
}
void sunGlareDisable(void)
{
    int i;
    for (i=0; i<eaSize(&g_skies); i++) 
        sunGlareDisableInternal(g_skies[i]);
}

void sunGlareEnable(void)
{
    int i;
    for (i=0; i<eaSize(&g_skies); i++)
    {
        UberSky *sky = g_skies[i];
        if( sky->sunflare_node_ptr )
        {
            gfxNodeSetAlpha(sky->sunflare_node_ptr,sky->sunflare_alpha*255,1);
        }
        
    }
}

//figure out if the sun's center is visible.  If so, apply lens flare.
static void sunVisibleInternal(UberSky *sky, float can_see)
{
    const float epsilon = 0.001f;
    float alpha = sky->sunflare_alpha;
    float diff = can_see - alpha;
    if(fabs(diff) < epsilon || !_finite(alpha)) {
        alpha = can_see;
    } else if(TIMESTEP_NOSCALE > epsilon) {
        alpha += diff * 0.5f * (1.0/30.0f) * TIMESTEP_NOSCALE;
    }

    sky->sunflare_alpha = CLAMP(alpha, 0.0f, 1.0f);
}

void sunVisible(void)
{
    int i;
    float can_see = 0.0f;

    if (!g_skies[0]->sunflare_node_ptr)
        return;

    if (!game_state.no_sun_flare)
    {
        Vec3 pos;
        copyVec3(g_skies[0]->sunflare_node_ptr->mat[3],pos);

        //if below the horizon, dont bother
        if( pos[1] < cam_info.cammat[3][1] ) 
        {
            can_see = 0.0f;
        }
        else
        {
            if (rdr_caps.supports_occlusion_query) {
                static float visibility = 0.0f;               
                rdrSunFlareUpdate(g_skies[0]->moonNodes[0][0], &visibility);
                can_see = visibility;
                //xyprintf( 50, 50, "%f", visibility );
            } else {
                int width, height;
                Vec2 pixel_pos;
                Vec3 xpos;
                mulVecMat4(pos,cam_info.viewmat,xpos);
                gfxWindowScreenPos(xpos,pixel_pos); 
                windowSize(&width,&height);
                if (pixel_pos[0] >= 0 && pixel_pos[0] < width && pixel_pos[1] >= 0 && pixel_pos[1] < height)
                {
                    static F32 depthbuf[16] = {1};
                    rdrGetPixelDepth(pixel_pos[0],pixel_pos[1], depthbuf);
                    if (depthbuf[0] > 0.999f) //1.0 doesn't seem to work. The sky gets in the way... 
                        can_see = 1.0f;
                    //xyprintf( 50, 50, "%f", depthbuf[0] );
                }
            }
        }
    }

    for (i=0; i<eaSize(&g_skies); i++)
        sunVisibleInternal(g_skies[i], can_see);
}

static void ratioVec3(Vec3 start,Vec3 end,F32 ratio,Vec3 dst)
{
    int        i;

    for(i=0;i<3;i++)
        dst[i] = start[i] * (1.f - ratio) + end[i] * ratio;
}

static void ratioVec2(Vec2 start,Vec2 end,F32 ratio,Vec2 dst)
{
    int        i;

    for(i=0;i<2;i++)
    {
        dst[i] = sqrt(start[i]) * (1.f - ratio) + sqrt(end[i]) * ratio;
        dst[i] = SQR(dst[i]);
    }
}

static int sceneFileFog(FogVals *fog_vals)
{
    F32        *c = scene_info.force_fog_color,fognear = scene_info.force_fog_near,fogfar = scene_info.force_fog_far;
    #define SCENEFOG(x) (x > -1024)

    if (SCENEFOG(c[0]))
    {
        copyVec3(c,fog_vals->color);
        if( server_visible_state.fog_color[0] || server_visible_state.fog_color[1] || server_visible_state.fog_color[2] )
            copyVec3(server_visible_state.fog_color,fog_vals->color);
    }
    if (SCENEFOG(fognear))
    {
        fog_vals->dist[0] = fognear;
        if( server_visible_state.fog_dist>0)
            fog_vals->dist[0] = 10.f;
    }
    if (SCENEFOG(fogfar))
    {
        fog_vals->dist[1] = fogfar;
        if( server_visible_state.fog_dist>0)
            fog_vals->dist[1] = server_visible_state.fog_dist;
    }
    if (SCENEFOG(c[0]) || SCENEFOG(fognear) || SCENEFOG(fogfar))
        fog_vals->valid = 1;
    else
        return 0;
    return 1;
}

void skyInvalidateFogVals(void)
{
    fog_final.valid = 0;
    luminance_valid = 0;
    dof_values_valid = 0;
    sun_values_valid = 0;
}

void skySetIndoorFogVals(int fognum, F32 fog_near, F32 fog_far, U8 color[4], FogVals *fog_vals)
{
    fog_vals[fognum].dist[0] = fog_near;
    fog_vals[fognum].dist[1] = fog_far;
    scaleVec3(color,(1.f/255.f),fog_vals[fognum].color);
    fog_vals[fognum].valid = 1;
}

static void skySetRenderValues(const Vec2 IN dist, const Vec3 IN fog_color, SunLight OUT *sun)
{
    F32        scale,max_half_fov;
    scale = 1.0;                //TO DO : expert level setting for super high draw_scale pushes out fog

    if( game_state.draw_scale < 1.0 ) // || game_state.allow_vis_scale_to_scale_up_fog )
    {
        //A total hack to make sure foggy world doesn't get foggier for low vis_scale settings 
        if( dist[1] > 400 ) //Less that this and you are in an especially foggy world : don't pull that in.  
            scale = game_state.draw_scale;
    } //End hack
    scaleVec2(dist, scale, sun->fogdist);
    copyVec3(fog_color, sun->fogcolor);
    //rdrSetFog(dist[0] * scale,dist[1] * scale,fog_color);

    // Divide by cosine of max fov/2 because we might not have radial fog.
    max_half_fov = MAX(gfx_window.fovx, gfx_window.fovy) * 0.5f;
    group_draw.fog_far_dist = dist[1] * scale / cosf(RAD(max_half_fov));
    group_draw.fog_clip = scene_info.clip_fogged_geometry || game_state.vis_scale < 0.7f;

    group_draw.minspec_fog_far_dist = dist[1] * MIN_SPEC_DRAWSCALE;

}

static void setSkyFog(const UberSky *sky, const SunTime *early, const SunTime *late, F32 ratio, SkyWorkingValues OUT *sky_work)
{
    Vec3    early_dv,late_dv,fogcolor,highfogcolor;
    F32        fogheightratio;
    int        i,t;

    if (sky->sky_struct.fogheightrange[0])
        fogheightratio = (cam_info.cammat[3][1] - sky->sky_struct.fogheightrange[0]) / (sky->sky_struct.fogheightrange[1] - sky->sky_struct.fogheightrange[0]);
    else
        fogheightratio = 0;
    fogheightratio = MINMAX(fogheightratio,0,1);

    if (early->fogdist[1] || game_state.fogdist[1])
    {
        Vec2    fogdist;

        // LRP
        scaleVec2(early->fogdist, (1-ratio), early_dv);
        scaleVec2(late->fogdist, ratio, late_dv);
        addVec2(early_dv, late_dv, fogdist);

        // Check for 0s
        if (!early->fogdist[1])
            copyVec2(late->fogdist,fogdist);
        if (!late->fogdist[1])
            copyVec2(early->fogdist,fogdist);

        // Check for debug override
        if( server_visible_state.fog_dist )
        {
            fogdist[0] = 10.f;
            fogdist[1] = server_visible_state.fog_dist;
        }
        
        if (game_state.fogdist[1])
            copyVec2(game_state.fogdist,fogdist);
        

        // LRP between this and a fog dist for high heights, but only let it get foggier?
        for(i=0;i<2;i++)
        {
            t = fogdist[i] * (1-fogheightratio) + sky->sky_struct.fogdist[i] * fogheightratio;
            fogdist[i] = MIN(t,fogdist[i]);
        }
        copyVec2(fogdist,sky_work->fog_curr.dist);
    }
    scaleVec3(early->fogcolor,1-ratio,early_dv);
    scaleVec3(late->fogcolor,ratio,late_dv);
    addVec3(early_dv,late_dv,fogcolor);

    scaleVec3(early->highfogcolor,1-ratio,early_dv);
    scaleVec3(late->highfogcolor,ratio,late_dv);
    addVec3(early_dv,late_dv,highfogcolor);

    scaleVec3(fogcolor,(1-fogheightratio),early_dv);
    scaleVec3(highfogcolor,fogheightratio,late_dv);
    addVec3(early_dv,late_dv,fogcolor);

    if (server_visible_state.fog_color[0] || server_visible_state.fog_color[1] || server_visible_state.fog_color[2])
    {
        copyVec3(server_visible_state.fog_color,fogcolor);
        scaleVec3(fogcolor,1.f/255.f,fogcolor);
    }
    if (game_state.fogcolor[0] || game_state.fogcolor[1] || game_state.fogcolor[2])
    {
        copyVec3(game_state.fogcolor,fogcolor);
        scaleVec3(fogcolor,1.f/255.f,fogcolor);
    }
    copyVec3(fogcolor,sky_work->fog_curr.color);
}

static void blendVec3(const Vec3 start, const Vec3 end, F32 ratio, Vec3 OUT final)
{
    Vec3    start_dv,end_dv;

    scaleVec3(start,(1-ratio),start_dv);
    scaleVec3(end,ratio,end_dv);
    addVec3(start_dv,end_dv,final);
}

static F32 blendF32(F32 start,F32 end,F32 ratio)
{
    return start * (1-ratio) + end * ratio;
}

static void fogBlend(const FogVals *start, const FogVals *end, F32 ratio, F32 feet, FogVals OUT *final)
{
    int        i;

    blendVec3(start->color,end->color,MIN(ratio,1.0),final->color);
    for(i=0;i<2;i++)
    {
        F32        d,da;

        d = end->dist[i] - start->dist[i];
        da = MIN(fabs(d),feet * TIMESTEP);
        if (d < 0)
            da = -da;
        final->dist[i] = start->dist[i] + da;
    }
}

static void fogBlendWithLast(FogVals IN OUT *fog_final, const FogVals IN *fog_curr, F32 speed)
{
    if (!fog_final->valid)
        fogBlend(fog_final,fog_curr,1,50000,fog_final);
    fogBlend(fog_final,fog_curr,scene_info.fog_ramp_color*speed,scene_info.fog_ramp_distance*speed,fog_final);
    fog_final->valid = 1;
}

static void dualColoredFogBlend(FogVals fog_vals[2], FogVals *fog_out)
{
    if (!fog_vals[0].valid) {
        // Nothing, leave fog_out alone
    } else {
        static F32    ratio;
        F32            r;
        // Blend
        assert(fog_vals[1].valid); // Both must be valid

        ratio += 0.03 * TIMESTEP;
        if (ratio > 2 * PI)
            ratio -= 2 * PI;

        r = (sinf(ratio) + 1) * 0.5f;
        fogBlend(&fog_vals[0],&fog_vals[1],r,3,fog_out);
    }
}

F32 luminanceBlendWithLast(F32 newluminance, F32 oldlum)
{
    F32 percent = pow(0.9, TIMESTEP);
    F32 blendedLum;
    if (!luminance_valid)
        oldlum = newluminance;
    blendedLum = blendF32(newluminance, oldlum, percent);
    luminance_valid = 1;
    return blendedLum;
}

void dofBlend(const DOFValues *start, const DOFValues *end, F32 ratio, DOFValues *final)
{
    final->farDist = blendF32(start->farDist, end->farDist, ratio);
    final->farValue = blendF32(start->farValue, end->farValue, ratio);
    final->nearDist = blendF32(start->nearDist, end->nearDist, ratio);
    final->nearValue = blendF32(start->nearValue, end->nearValue, ratio);
    final->focusDistance = blendF32(start->focusDistance, end->focusDistance, ratio);
    final->focusValue = blendF32(start->focusValue, end->focusValue, ratio);
}

DOFValues custom_dof={0};
enum {
    DOF_NORMAL,
    DOF_FADING_TO_CUSTOM,
    DOF_CUSTOM,
    DOF_FADING_FROM_CUSTOM,
} custom_dof_state = DOF_NORMAL;
DOFSwitchType custom_dof_switch = 0;
F32 custom_dof_duration = 0;

void dofBlendWithLast(DOFValues *new_values, DOFValues *old_values, DOFValues *final)
{
    F32 percent = pow(0.95, TIMESTEP);
    if (!dof_values_valid)
        old_values = new_values;
    // Don't actually toggle this until inside sunApplyValues or we will still apply a fade
    // dof_values_valid = 1;

    if (custom_dof_state == DOF_FADING_TO_CUSTOM) {
        dofBlend(&custom_dof, old_values, percent, old_values);
        if (nearSameF32(custom_dof.focusDistance, old_values->focusDistance) && 
            nearSameF32(custom_dof.focusValue, old_values->focusValue) && 
            nearSameF32(custom_dof.nearDist, old_values->nearDist) && 
            nearSameF32(custom_dof.nearValue, old_values->nearValue) && 
            nearSameF32(custom_dof.farDist, old_values->farDist) && 
            nearSameF32(custom_dof.farValue, old_values->farValue))
        {
            custom_dof_state = DOF_CUSTOM;
        }
        if (custom_dof_duration) {
            custom_dof_duration -= TIMESTEP;
            if (custom_dof_duration <= 0) {
                // Set full custom DOF
                dofBlend(&custom_dof, &custom_dof, 1.0, final);
                custom_dof_state = DOF_FADING_FROM_CUSTOM;
            }
        }
    } else if (custom_dof_state == DOF_CUSTOM) {
        dofBlend(&custom_dof, &custom_dof, 1.0, final);
        if (custom_dof_switch == DOFS_SNAP)
            dof_values_valid = 0;
        if (custom_dof_duration) {
            custom_dof_duration -= TIMESTEP;
            if (custom_dof_duration <= 0) {
                if (custom_dof_switch == DOFS_SNAP) {
                    custom_dof_state = DOF_NORMAL;
                    dof_values_valid = 0;
                } else
                    custom_dof_state = DOF_FADING_FROM_CUSTOM;
            }
        }
    } else if (custom_dof_state == DOF_NORMAL || custom_dof_state == DOF_FADING_FROM_CUSTOM) {
        custom_dof_state = DOF_NORMAL;
        dofBlend(new_values, old_values, percent, final);
    } else
        assert(0);
}

void setCustomDepthOfField(DOFSwitchType IN type, F32 IN focusDistance, F32 IN focusValue, F32 IN nearDist, F32 IN nearValue, F32 IN farDist, F32 IN farValue, F32 IN duration, SunLight OUT *sun)
{
    switch (type) {
    xcase DOFS_SNAP:
        custom_dof_state = DOF_CUSTOM;
    xcase DOFS_SMOOTH:
        custom_dof_state = DOF_FADING_TO_CUSTOM;
    }
#define SET_IF_NEEDED(v) custom_dof.v = (DOF_LEAVE==v)?sun->dof_values.v:v
    SET_IF_NEEDED(focusDistance);
    SET_IF_NEEDED(focusValue);
    SET_IF_NEEDED(nearDist);
    SET_IF_NEEDED(nearValue);
    SET_IF_NEEDED(farDist);
    SET_IF_NEEDED(farValue);
#undef SET_IF_NEEDED
    custom_dof_duration = duration*30; // convert to ticks
    custom_dof_switch = type;
}

void unsetCustomDepthOfField(DOFSwitchType IN type, SunLight OUT *sun)
{
    if (custom_dof_state == DOF_CUSTOM ||
        custom_dof_state == DOF_FADING_TO_CUSTOM)
    {
        if (type == DOFS_SNAP) {
            custom_dof_state = DOF_NORMAL;
            dof_values_valid = 0;
        } else {
            custom_dof_state = DOF_CUSTOM;
            custom_dof_duration = -1;
        }
        custom_dof_switch = type;
    }
}



void sunRemoveFlare(UberSky *sky)
{
    if( gfxTreeNodeIsValid(sky->sunflare_node_ptr, sky->sunflare_node_ptr_id) )
    {
        gfxTreeDelete(sky->sunflare_node_ptr);
    }
    sky->sunflare_node_ptr = 0; //Global, used by lens flare
    sky->sunflare_node_ptr_id = 0;
}

void destroySky(UberSky *sky)
{
    sunRemoveFlare(sky);
    SAFE_FREE(sky->sky_work.sun_work.sky_node_list.nodes);
    sky->sky_work.sun_work.sky_node_list.count = sky->sky_work.sun_work.sky_node_list.max = 0;
}

void skyNotifyGfxTreeDestroyed(void)
{
    int i;
    for (i=0; i<eaSize(&g_skies); i++) {
        g_skies[i]->sunflare_node_ptr = NULL;
        g_skies[i]->sunflare_node_ptr_id = 0;
    }
}

void initializeSky(void)
{
    //Clear out the old sky
    //(doesn't clear out the sunTime->skys, that's done in the reparser)
    gfxTreeInitSkyTree();
    fog_final.valid = 0;    //Global
    luminance_valid = 0;
    dof_values_valid = 0;
    sun_values_valid = 0;

}

static void initializeSkyInternal(UberSky *sky, int init)
{
    int i;
    char    buf[200],*s;

    memset(sky->moonNodes,0,sizeof(sky->moonNodes));
    memset(sky->cloudNodes,0,sizeof(sky->cloudNodes));

    sunRemoveFlare(sky);

    if (!init)
        return;


    //Add the gfxNodes for all the sky geometries for all the  times (//sky had gfxNodes in the parsed struct, which is a little weird)
    //Stars, basically
    for( i = 0 ; i < sky->sun_time_count ; i++ )
    {
        SunTime * sunTime = sky->times[i];

        if ( sunTime->skyname  )
        {
            sunTime->sky_node = sunAddNode( sunTime->skyname, SKY_TREE );
            if ( sunTime->sky_node )
                copyVec3( sunTime->sky_node->mat[3], sunTime->skypos );
            else
                conPrintfVerbose( "Cant find sky: %s", sunTime->skyname );        
        }
        else
            sunTime->sky_node = 0;
    }
    

    //Add moon[0] : the sun's and the sun's glow
    sky->fixed_sun_pos = false;
    if( sky->moon_count )
    {
        char * sunName = sky->sky_struct.moon_names[0]->params[0];

        //Add the sun( I don't know why )
        sky->moonNodes[0][0] = sunAddNode( sunName, SKY_TREE );
        if (!sky->moonNodes[0])
            conPrintfVerbose( "cant find sun: %s", sunName );
            
        sky->fixed_sun_pos = (( eaSize( &sky->sky_struct.moon_names[0]->params ) >= 2 ) &&
                              ( atof(sky->sky_struct.moon_names[0]->params[1])==0.0f));

        //Add the sun glow
        strcpy(buf, sunName );
        s = strstr(buf,"__");
        if (s)
            *s = 0;
        strcat(buf,"glow__");
        sky->moonNodes[0][1] = sunAddNode( buf, GENERAL_TREE ); //sun glow is added to the general tree, and sorted.

        sky->sunflare_node_ptr = sky->moonNodes[0][1]; //a pseudonym
        sky->sunflare_node_ptr_id = sky->sunflare_node_ptr->unique_id;
    }

    //Add all the other moons
    for( i = 1 ; i < sky->moon_count ; i++ ) //Notice you skip i == 0, the sun
    {
        char * moonName = sky->sky_struct.moon_names[i]->params[0];

        //Add this Moon
        sky->moonNodes[i][0] = sunAddNode( moonName, SKY_TREE );
        if (!sky->moonNodes[i][0])
            conPrintfVerbose( "cant find moon: %s", moonName );

        //Add this Moon's glow, if there is one
        sky->moonNodes[i][1] = 0;// moonglow is a thing of the past sunAddNode(buf, SKY_TREE);
    }
    

    //clouds (dumb name) are geometry always there.  SkyBowl and the clouds are examples 
    for(i=0;i<sky->cloud_count;i++)
    {
        char * cloudName = sky->clouds[i]->name;

        sky->cloudNodes[i] = sunAddNode( cloudName, SKY_TREE );
        if ( !sky->cloudNodes[i] ) 
            conPrintfVerbose( "cant find cloud: %s", cloudName );
    }

}


//Set Color and intensity of Shadows based on time of day
void setGlobalShadowColor( Vec4 earlyShadowColor, Vec4 lateShadowColor, F32 ratio, SunLight OUT *sun)
{
    extern int    camera_is_inside;
    Vec4 early_dv;
    Vec4 late_dv;

    if(!group_draw.see_outside)
    {
        scaleVec4(earlyShadowColor ,(1-ratio), early_dv);
        scaleVec4(lateShadowColor  ,ratio,     late_dv);
        addVec4(early_dv,late_dv,sun->shadowcolor);
    }
    else
    {
        sun->shadowcolor[0] = 0;
        sun->shadowcolor[1] = 0;
        sun->shadowcolor[2] = 0;
        sun->shadowcolor[3] = INSIDE_SHADOW_ALPHA;  //TO DO calculate this per entity using groupFindInside.
    }
}

bool isIndoors()
{
    // TODO: Also check onIndoorMission()?
    return !!cam_info.last_tray;
}

static F32 indoorLuminance()
{
    if (cam_info.last_tray && cam_info.last_tray->light_luminance) {
        return cam_info.last_tray->light_luminance;
    } else {
        return 1.4; // Shouldn't happen unless tray has no lights?
    }
}

static void setSunMoonRot(const UberSky IN *sky, float IN time, const SunTime IN *early, const SunTime IN *late, float IN ratio, const Vec3 IN plr_offset, SunLight OUT *sun)
{
    int i;
    F32 rot; 
    rot = 2.f * PI * (time - 12.f) / 24.f;
    rot = fixAngle(rot);
    rot = rot * sqrt(fabs(rot)) / sqrt(PI);

    for(i=0;i<sky->moon_count;i++)
    {
        F32    scale,alpha;

        scale = early->moon_scales[i] * (1-ratio) + late->moon_scales[i] * ratio;
        alpha = early->moon_alphas[i] * (1-ratio) + late->moon_alphas[i] * ratio;
        if (sky->moonNodes[i][0])
            hackmoon(sky, sun, i,0,rot,plr_offset,sky->moonNodes[i][0],scale,alpha,(&(sky->sky_struct.moon_names[i])));
        if (sky->moonNodes[i][1])
            hackmoon(sky, sun, i,0,rot,plr_offset,sky->moonNodes[i][1],scale,alpha,(&(sky->sky_struct.moon_names[i])));
    }

    if (sky->moonNodes[0][0])
    {
        copyVec3(sky->moonNodes[0][0]->mat[3],sun->position);
        subVec3(sun->position,plr_offset,sun->direction);
    } else {
        // Still want a sun position/direction?
        setVec3(sun->position, 2000, 7000, 2000);
        subVec3(sun->position,plr_offset,sun->direction);
        setVec3same(sun->diffuse, 0);
        setVec3same(sun->diffuse_for_players, 0);
        setVec3same(sun->diffuse_reverse, 0);
        setVec3same(sun->diffuse_reverse_for_players, 0);
    }
    normalVec3(sun->direction);
    if (sun->direction[1] < 0) {
        sun->direction[1] = -sun->direction[1];
        if (sky->moonNodes[0][0]) {
            // Flip sun position so it's above you (so bump mapped objects are rendered the same as everything else!)
            F32 diff = plr_offset[1] - sun->position[1];
            sun->position[1] = plr_offset[1] + diff;
        }
    }
}

static F32 playerAmbientAdjuster;    
static F32 playerDiffuseAdjuster;   
static F32 MINIMUM_AMBIENT; 
static F32 MINIMUM_PLAYER_AMBIENT;
static F32 MINIMUM_DIFFUSE;
static F32 MINIMUM_PLAYER_DIFFUSE;

static F32 MAXIMUM_AMBIENT ;
static F32 MAXIMUM_PLAYER_AMBIENT;
static F32 MAXIMUM_DIFFUSE;
static F32 MAXIMUM_PLAYER_DIFFUSE;

static void initLightingMinMaxes(void)
{
    static bool inited=false;
    static GfxFeatures using_bumpmaps;

    if (!inited || (rdr_caps.features & GFXF_BUMPMAPS)!=using_bumpmaps) {
        inited = true;
        using_bumpmaps = (rdr_caps.features & GFXF_BUMPMAPS);
    } else {
        return;
    }

    MAXIMUM_AMBIENT            = 1.0;
    MAXIMUM_PLAYER_AMBIENT    = 1.0;
    MAXIMUM_DIFFUSE            = 1.0;
    MAXIMUM_PLAYER_DIFFUSE    = 1.0;

    //TO DO : use this for inside player light, too.
    // Set player lighting based on whether or not we're doing bumpmapping
    if( (rdr_caps.features & GFXF_BUMPMAPS) )    
    {
        playerAmbientAdjuster    = 2.75;    //sun ambient x this = ambient for players
        playerDiffuseAdjuster    = 0.52;    //sun diffuse x this = duffuse for players
        MINIMUM_PLAYER_AMBIENT    = 0.12;    //player ambient R,G,or B can never be less than this
        MINIMUM_PLAYER_DIFFUSE    = 0.06;    //player diffuse R,G,or B cna never be less than this
    }
    else
    {
        playerAmbientAdjuster    = 1.3;    //sun ambient x this = ambient for players
        playerDiffuseAdjuster    = 0.7;    //sun diffuse x this = duffuse for players
        MINIMUM_PLAYER_AMBIENT    = 0.14;    //player ambient R,G,or B can never be less than this
        MINIMUM_PLAYER_DIFFUSE    = 0.10;    //player diffuse R,G,or B cna never be less than this
    }
    // World geometry is lit the same with/without bumpmaps
    MINIMUM_AMBIENT            = 0.08; //sun ambient R,G,or B can never be less than this 
    MINIMUM_DIFFUSE            = 0.05;    //sun diffuse R,G,or B cna never be less than this
}

#define CLOUD_FADE_RATE 1.f
static F32 getCloudAlpha(F32 time, Vec2 *fadetimes, F32 fademin)
{
    F32 fogtimefade;
    if (time > (*fadetimes)[1])
        fogtimefade = MIN(1+(((*fadetimes)[1]-time)/CLOUD_FADE_RATE),1);        //    controls the fade out, for 1 hour after fade out time, it ramps down to 0 alpha (negative but it kepts capped)
    else
        fogtimefade = MIN(1+((time-(*fadetimes)[0])/CLOUD_FADE_RATE),1);        //    controls the fade in, for 1 hour before fade in time, it ramps up to 1 alpha
    return MAX((fademin/256.f),fogtimefade);                                //    minimum alpha for this texture
}
#undef CLOUD_FADE_RATE
//Updates sun, moon, sky, fog
// Only modifies sky-> if initializing. (to put on gfxtree nodes)
static void sunUpdateInternal(UberSky IN OUT *sky, SunLight OUT *sun, int IN init)
{
    F32        time;                //Global time (server_visible_state.time)
    SunTime    *early=0,*late=0;    //Prev and next Suntimes to interpolate between
    F32        ratio;                //percent of late value to draw early ( 1-r )
    int        i;
    Vec3    plr_offset;            //Position to draw the sky.

    time = server_visible_state.time;

    //Debug// If sky.sun_time_count == 0 gfxReload() has been called, but sceneLoad has
    //never been called, so don't do anything.  If !sky.sky_struct, then no sky has ever been 
    //loaded
    if ( sky->sun_time_count == 0)
        return;
    assert( !sky_gfx_tree_root || sky_gfx_tree_root->unique_id > 0 );
    //End Debug

    //Get player's x and y values to center sky on
    copyVec3(cam_info.cammat[3],plr_offset);
    //plr_offset[1] = 0; Do this only for clouds, not skies/suns/moons
    //End get pos

    //Init Sky if needed
    if ( init || !sky_gfx_tree_inited)
        initializeSkyInternal(sky, init);
    //End Init Sky

    // Clear list of sky nodes
    sun->sky_node_list.count = 0;

    //Calculate clouds
    {
        F32    fogtimefade;
        F32 ftf, cloudRatio;

        fogtimefade = getCloudAlpha(time, &sky->sky_struct.cloud_fadetimes, sky->sky_struct.cloud_fademin);

        for(i=0;i<sky->cloud_count;i++)
        {
            if (!sky->cloudNodes[i])
                continue;

            ftf = fogtimefade;
            if (sky->clouds[i]->heights[1])
            {
                cloudRatio = (cam_info.cammat[3][1] - sky->clouds[i]->heights[0]) / (sky->clouds[i]->heights[1] - sky->clouds[i]->heights[0]);
                cloudRatio = MINMAX(cloudRatio,0,1);
                ftf *= cloudRatio;
            }

            if( sky->cloudNodes[i]->model) {
                F32 alpha = ftf;
                SkyNode *node;
                if (strstri( sky->cloudNodes[i]->model->name, "sunskirt" )) {
                    //Hack : sky bowl never fades
                    alpha = 1.0;
                }

                node = dynArrayAddStruct(&sun->sky_node_list.nodes, &sun->sky_node_list.count, &sun->sky_node_list.max);
                node->node = sky->cloudNodes[i];
                if (sky->clouds[i]->fademin >= 0.0f)
                {
                    alpha = getCloudAlpha(time, &sky->clouds[i]->fadetimes, sky->clouds[i]->fademin);
                }
                node->alpha = alpha;
                gfxNodeSetAlpha(sky->cloudNodes[i],(U8)0,1); // Will get un-hidden in applying

                copyVec3( plr_offset,sky->cloudNodes[i]->mat[3]);
                sky->cloudNodes[i]->mat[3][1] = 0.0; // Let people fly closer to clouds
            }
        }
    }
    //End calculate clouds

    //Get early (prev) and late (next) suntimes, and the ratio between them to use
    //Rewritten because it had a intermittent logic bug I couldn't find
    {
        int lateSkyTimeIdx=0, earlySkyTimeIdx=0;

        //Find the earlyearlySkyTimeIdx and lateSkyTimeIdx to blend between
        int success, i;
        F32 earlyTime, lateTime;

        assert( time < 24.0 && time >= 0.0 );
        assertmsg( sky->sun_time_count > 0 && sky->sun_time_count < 100, "Sky file corrupted after load" );

        if( sky->sun_time_count == 1 )
        {
            earlySkyTimeIdx = 0;
            lateSkyTimeIdx = 0;
        }
        else
        {
            success = 0;

            for( i=0 ; i < sky->sun_time_count ; i++ )
            {
                earlySkyTimeIdx = i;
                if( i == (sky->sun_time_count - 1) ) //on the last one, wrap
                    lateSkyTimeIdx  = 0;
                else
                    lateSkyTimeIdx  = i+1;

                earlyTime = fixTime( sky->times[earlySkyTimeIdx]->time);
                lateTime  = fixTime( sky->times[lateSkyTimeIdx ]->time);

                if( earlyTime < lateTime )
                {
                    if( time >= earlyTime && time < lateTime )
                        success = 1;
                }
                else if( earlyTime > lateTime )
                {
                    if( time >= earlyTime || time < lateTime )
                        success = 1;
                }
                else assertmsg( 0, "Bad Sky : has same time twice" );

                if( success )
                    break;
            }
            assertmsg( success != 0, "Your Sky Makes no sense" );
        }

        //Get early and late times and the ratio between them
        early = sky->times[ earlySkyTimeIdx ];
        late = sky->times[ lateSkyTimeIdx ];

        if( sky->sun_time_count > 1 )
        {
            F32 t_dist, t_len;

            t_dist = fixTime(time - early->time); 
            t_len = fixTime(late->time - early->time);
            if (!t_len)
                ratio = 0;
            else
                ratio = t_dist / t_len;
            ratio = MINMAX( ratio, 0, 1 );
        }
        else
        {
            ratio = 1;
        }
    }

    // Hide all sky nodes (used ones are unhid below)
    for( i=0 ; i < sky->sun_time_count ; i++ )
    {
        if( sky->times[i]->sky_node )
            gfxNodeSetAlpha(sky->times[i]->sky_node,0,1);
    }

    //Cross Fade Between Early and Late skies. This crossfade is very fast, I don't really get it
    {
        F32 r = ratio;

        //If they are the smae models, just draw one of them
        if ( early->sky_node && late->sky_node && early->sky_node->model == late->sky_node->model )
            r = 0;

        if( early->sky_node && r != 1.0 )
        {
            SkyNode *node = dynArrayAddStruct(&sun->sky_node_list.nodes, &sun->sky_node_list.count, &sun->sky_node_list.max);
            node->node = early->sky_node;
            node->alpha = 1.0 - r;

            addVec3(early->skypos,plr_offset,early->sky_node->mat[3]);
        }

        if( late->sky_node && late->sky_node != early->sky_node && r != 0.0 )
        {
            SkyNode *node = dynArrayAddStruct(&sun->sky_node_list.nodes, &sun->sky_node_list.count, &sun->sky_node_list.max);
            node->node = late->sky_node;
            node->alpha = r;

            addVec3(late->skypos,plr_offset,late->sky_node->mat[3]);
        }
    }
    //End fade

    //Set Up Fog Values
    setSkyFog( sky, early, late, ratio, &sky->sky_work ); // Sets sky_work->fog_curr
    blendVec3( early->backgroundcolor, late->backgroundcolor, ratio, sun->bg_color);
    //End Fog

    //Set Sun and Moon rotations (not really sure how exactly)
    setSunMoonRot(sky, time, early, late, ratio, plr_offset, sun);

    //Set the Current Lamp Alpha for NightLight Trick
    {
        F32 r;
        if (time > sky->sky_struct.lamp_lighttimes[1])
            r = MIN(sky->sky_struct.lamp_lighttimes[1]+1-time,1);
        else
            r = MIN(time-(sky->sky_struct.lamp_lighttimes[0]-1),1);
        r = MAX(r,0);
        r = 1-r;
        sun->lamp_alpha = r * 255;
        sun->lamp_alpha_byte = sun->lamp_alpha;
        sun->fixed_position = sky->fixed_sun_pos;
    }
    //End Set Lamp Alpha

    //Set Shadow
    setGlobalShadowColor( early->shadowcolor, late->shadowcolor, ratio, sun );
    //End Set Shadow

    //Set Global Sun Light ( sun_ambient, sun_diffuse, glob_sun_pos (for bumpmapping), and sun_dir )
    {
        #define COLOR_SCALE (1.f/255.f)
        Vec3 ambient,diffuse;
        F32 newluminance;
        DOFValues new_dof_values;

        blendVec3( early->ambient, late->ambient, ratio, ambient );

        blendVec3( early->diffuse, late->diffuse, ratio, diffuse );

        // Find new luminance
        if (!init && isIndoors()) {
            if (scene_info.indoorLuminance) {
                newluminance = scene_info.indoorLuminance;
            } else {
                newluminance = indoorLuminance();
            }
        } else {
            newluminance = blendF32( early->luminance, late->luminance, ratio );
        }
        sun->luminance = luminanceBlendWithLast(newluminance, sun->luminance);
        
        dofBlend(&early->dof_values, &late->dof_values, ratio, &new_dof_values);
        dofBlendWithLast(&new_dof_values, &sun->dof_values, &sun->dof_values);
            
        //Kinda crazy scaling, because I don't want change how it used to be done and break it. 
        //Someday just remove this
        scaleVec3(ambient,COLOR_SCALEUB(255),sun->ambient);
        scaleVec3(diffuse,COLOR_SCALEUB(255),sun->diffuse);
        scaleVec3(sun->ambient,COLOR_SCALE,sun->ambient);
        scaleVec3(sun->diffuse,COLOR_SCALE,sun->diffuse);

        sun->bloomWeight = scene_info.bloomWeight;
        sun->toneMapWeight = scene_info.toneMapWeight;
        sun->desaturateWeight = scene_info.desaturateWeight;
    }
    //End Set Global Sun Light

    // Do mins/maxes

    sun->ambient[3] = 1;   
    sun->diffuse[3] = 1;    
    copyVec4( sun->ambient, sun->ambient_for_players );         
    copyVec4( sun->diffuse, sun->diffuse_for_players ); 
    scaleVec3( sun->ambient_for_players, playerAmbientAdjuster, sun->ambient_for_players );     
    scaleVec3( sun->diffuse_for_players, playerDiffuseAdjuster, sun->diffuse_for_players );  

    if (!sky->ignoreLightClamping)
    {
        for(i=0;i<3;i++)  
        {
            sun->ambient[i]                        = MAX( sun->ambient[i],                        MINIMUM_AMBIENT            ); 
            sun->ambient_for_players[i]            = MAX( sun->ambient_for_players[i],            MINIMUM_PLAYER_AMBIENT    );
            sun->diffuse[i]                        = MAX( sun->diffuse[i],                        MINIMUM_DIFFUSE            ); 
            sun->diffuse_for_players[i]            = MAX( sun->diffuse_for_players[i],            MINIMUM_PLAYER_DIFFUSE    );
            // it's okay for the reverse light to be black

            sun->ambient[i]                        = MIN( sun->ambient[i],                        MAXIMUM_AMBIENT            ); 
            sun->ambient_for_players[i]            = MIN( sun->ambient_for_players[i],            MAXIMUM_PLAYER_AMBIENT    );
            sun->diffuse[i]                        = MIN( sun->diffuse[i],                        MAXIMUM_DIFFUSE            ); 
            sun->diffuse_for_players[i]            = MIN( sun->diffuse_for_players[i],            MAXIMUM_PLAYER_DIFFUSE    );
        }
    }

    addVec3( sun->ambient, sun->diffuse, sun->no_angle_light );
    sun->no_angle_light[3] = 1.0;
    // End mins/maxes

    sun->gloss_scale = scene_info.scale_specularity;
}

static const char* s_color(const Vec3 color, F32 scale_inv)
{
    static char *str = NULL;
    switch(game_state.lightdebug)
    {
        xcase 2:    estrPrintf(&str, "%3d %3d %3d", (U8)(scale_inv*color[0]), (U8)(scale_inv*color[1]), (U8)(scale_inv*color[2]));
        xcase 3:    estrPrintf(&str, "#%02X%02X%02X", (U8)(scale_inv*color[0]), (U8)(scale_inv*color[1]), (U8)(scale_inv*color[2]));
        xdefault:    estrPrintf(&str, "%f %f %f", color[0], color[1], color[2]);
    }
    return str;
}

static void showLightDebug(const SunLight IN *sun, const FogVals IN *fog_final, const FogVals IN *fog_curr)
{
    //Debug 
    if(game_state.lightdebug){ 
        int y = 1;
        int x = 2;
        const char *caveat = game_state.lightdebug == 1 ? "" : "(skyfile colors, could be wrong -GG)";
        const F32 screwed_scaling_inv = 256.f*255.f/63.f;
        xyprintf( x, y++, "Time %f", server_visible_state.time );  
        y++;
        xyprintf( x, y++, "Adjusters" );
        xyprintfcolor( x, y++, 255, 255, 100, "    playerAmbientAdjuster  %f", playerAmbientAdjuster );
        xyprintfcolor( x, y++, 255, 255, 100, "    playerDiffuseAdjuster  %f", playerDiffuseAdjuster );
        xyprintfcolor( x, y++, 255, 255, 100, "    MINIMUM_AMBIENT        %f", MINIMUM_AMBIENT );
        xyprintfcolor( x, y++, 255, 255, 100, "    MINIMUM_DIFFUSE        %f", MINIMUM_DIFFUSE );
        xyprintfcolor( x, y++, 255, 255, 100, "    MINIMUM_PLAYER_AMBIENT %f", MINIMUM_PLAYER_AMBIENT );
        xyprintfcolor( x, y++, 255, 255, 100, "    MINIMUM_PLAYER_DIFFUSE %f", MINIMUM_PLAYER_DIFFUSE );
        y++;
        xyprintf( x, y++, "Current Light %s", caveat );
        xyprintf( x, y++, "    ambient       %s", s_color(sun->ambient, screwed_scaling_inv) );
        xyprintf( x, y++, "    diffuse       %s", s_color(sun->diffuse, screwed_scaling_inv) );
        xyprintf( x, y++, "    playerAmbient %s", s_color(sun->ambient_for_players, screwed_scaling_inv/playerAmbientAdjuster) );
        xyprintf( x, y++, "    playerDiffuse %s", s_color(sun->diffuse_for_players, screwed_scaling_inv/playerDiffuseAdjuster) );
        y++;
        xyprintf( x, y++, "Light Position/Direction" );
        xyprintf( x, y++, "    position      %f %f %f", sun->position[0], sun->position[1], sun->position[2] );
        xyprintf( x, y++, "    direction     %f %f %f", sun->direction[0], sun->direction[1], sun->direction[2] );
        xyprintf( x, y++, "    direction_in_vs %f %f %f", sun->direction_in_viewspace[0], sun->direction_in_viewspace[1], sun->direction_in_viewspace[2] );
        y++;
        xyprintf( x, y++, "Fog Values %s", caveat );
        xyprintf( x, y++, "    final      %f %f  %s", fog_final->dist[0], fog_final->dist[1], s_color(fog_final->color, 256.f) );
        xyprintf( x, y++, "    curr       %f %f  %s", fog_curr->dist[0], fog_curr->dist[1], s_color(fog_curr->color, 256.f) );
        y++;
        xyprintf( x, y++, "Rendering Effects Values" );
        xyprintf( x, y++, "    luminance  %f", sun->luminance );
#define GSOVERRIDE(v) (game_state.dof_values.v?game_state.dof_values.v:sun->dof_values.v)
        xyprintf( x, y++, "    DOF        focus: %0.3f @ %1.2f  near: %0.3f @ %0.3f   far: %0.3f @ %1.2f",
            MAX(GSOVERRIDE(focusValue), game_state.camera_blur), GSOVERRIDE(focusDistance),
            MAX(GSOVERRIDE(nearValue)*game_state.dofWeight, game_state.camera_blur), GSOVERRIDE(nearDist),
            MAX(GSOVERRIDE(farValue)*game_state.dofWeight, game_state.camera_blur), GSOVERRIDE(farDist));
#undef GSOVERRIDE
        y++;
        xyprintf( x, y++, "Sky Fade Values" );
        xyprintf( x, y++, "    Base:                      Sky1 %d    Sky2 %d   Weight %f", game_state.skyFade1, game_state.skyFade2, game_state.skyFadeWeight );
        xyprintf( x, y++, "    Volume Overrde: On/Off %d   Sky1 %d    Sky2 %d   Weight %f", skyFadeOverride, skyFadeOverride1, skyFadeOverride2, skyFadeOverrideWeight );
    }
    //End Debug
}

static void sunBlendSkies(const SkyWorkingValues IN *sky_work1, const SkyWorkingValues IN *sky_work2, F32 ratio, SkyWorkingValues OUT *sky_work_out)
{
    int i;
    SkyWorkingValues temp1 = {0};
    SkyWorkingValues temp2 = {0};
    SkyNodeList list_temp = sky_work_out->sun_work.sky_node_list;
    ParserCopyStruct(SkyWorkingValuesParseInfo, (void*)sky_work1, sizeof(temp1), &temp1);
    shDoOperationSetFloat(ratio);
    shDoOperation(STRUCTOP_MUL, SkyWorkingValuesParseInfo, &temp1, OPERAND_FLOAT);
    ParserCopyStruct(SkyWorkingValuesParseInfo, (void*)sky_work2, sizeof(temp2), &temp2);
    shDoOperationSetFloat(1-ratio);
    shDoOperation(STRUCTOP_MUL, SkyWorkingValuesParseInfo, &temp2, OPERAND_FLOAT);
    shDoOperation(STRUCTOP_ADD, SkyWorkingValuesParseInfo, &temp1, &temp2);
    ParserCopyStruct(SkyWorkingValuesParseInfo, (void*)&temp1, sizeof(temp1), sky_work_out);
    // Blend Sky nodes
    sky_work_out->sun_work.sky_node_list = list_temp;
    sky_work_out->sun_work.sky_node_list.count = 0;
    for (i=0; i<sky_work1->sun_work.sky_node_list.count; i++) {
        SkyNode *node = dynArrayAddStruct(&sky_work_out->sun_work.sky_node_list.nodes, &sky_work_out->sun_work.sky_node_list.count, &sky_work_out->sun_work.sky_node_list.max);
        *node = sky_work1->sun_work.sky_node_list.nodes[i];
        node->alpha *= ratio;
    }
    for (i=0; i<sky_work2->sun_work.sky_node_list.count; i++) {
        SkyNode *node = dynArrayAddStruct(&sky_work_out->sun_work.sky_node_list.nodes, &sky_work_out->sun_work.sky_node_list.count, &sky_work_out->sun_work.sky_node_list.max);
        *node = sky_work2->sun_work.sky_node_list.nodes[i];
        node->alpha *= 1-ratio;
    }
}

static int skyNodeCmp(const SkyNode *sky_node1, const SkyNode *sky_node2)
{
    GfxNode *node1 = sky_node1->node;
    GfxNode *node2 = sky_node2->node;
    int i;
    if (node1->model != node2->model) {
        return node1->model - node2->model;
    }
    for (i=0; i<3; i++) 
        if (!nearSameDoubleTol(node1->mat[3][i],node2->mat[3][i], 0.01))
            return ((node1->mat[3][i] - node2->mat[3][i])>0)?1:-1;
    return 0;
}

static int skyNodeCmp(const SkyNode *sky_node1, const SkyNode *sky_node2);
static int skyNodeCmpCallback(const void* arg0, const void* arg1)
{
    return skyNodeCmp((const SkyNode *)arg0, (const SkyNode *)arg1);
}

static void collapseSkyNodes(SkyNodeList *sky_node_list)
{
    int i;
    // Collapse similar/same SkyNodes so that we don't draw two of the same at 50% alpha each and expect it to look reasonable
    qsort(sky_node_list->nodes, sky_node_list->count, sizeof(sky_node_list->nodes[0]), skyNodeCmpCallback);
    for (i=sky_node_list->count - 2; i>=0; i--) {
        if (skyNodeCmp(&sky_node_list->nodes[i], &sky_node_list->nodes[i+1]) == 0) {
            sky_node_list->nodes[i].alpha += sky_node_list->nodes[i+1].alpha;
            memmove(&sky_node_list->nodes[i+1], &sky_node_list->nodes[i+2], sizeof(sky_node_list->nodes[0])*(sky_node_list->count - i - 2));
            sky_node_list->count--;
        }
    }
}

// 1.0 = use dest, 0.0 = use src
static void blendSkyNodes(const SkyNodeList IN *src, SkyNodeList IN OUT *dest, F32 ratio)
{
    int i, j;
    bool *used = _alloca(dest->count*sizeof(bool));
    ZeroMemory(used, dest->count*sizeof(bool));
    for (i=0; i<src->count; i++) {
        bool foundit=false;
        for (j=0; j<dest->count; j++) {
            if (used[j])
                continue;
            if (skyNodeCmp(&src->nodes[i], &dest->nodes[j])!=0)
                continue;
            // Same
            if (src->nodes[i].snap) {
                dest->nodes[j].alpha = src->nodes[i].alpha;
            } else {
                dest->nodes[j].alpha = blendF32(src->nodes[i].alpha, dest->nodes[j].alpha, ratio);
            }
            used[j] = true;
            foundit = true;
        }
        if (!foundit) {
            // New!
            SkyNode *node = dynArrayAddStruct(&dest->nodes, &dest->count, &dest->max);
            node->node = src->nodes[i].node;
            node->snap = src->nodes[i].snap;
            if (node->snap) {
                node->alpha = src->nodes[i].alpha;
            } else {
                node->alpha = blendF32(src->nodes[i].alpha, 0, ratio);
            }
        }
    }
    // Prune unused
    for (i=dest->count - 1; i>=0; i--) {
        if (!used[i]) {
            if (dest->nodes[i].snap) {
                dest->nodes[i].alpha = 0;
            } else {
                dest->nodes[i].alpha = blendF32(0, dest->nodes[i].alpha, ratio);
            }
            if (dest->nodes[i].alpha < 0.001) {
                // Remove it!
                memmove(&dest->nodes[i], &dest->nodes[i+1], (dest->count - i - 1)*sizeof(dest->nodes[i]));
                dest->count--;
            }
        }
    }
}

static void sunApplyValues(const SkyWorkingValues IN *sky_work, SunLight IN OUT *sun, FogVals IN OUT *fog_final)
{
    FogVals fog_out = {0};
    bool init = false;
    SkyNodeList list_temp = sun->sky_node_list; // Save sky_node_list
    F32 ratio = pow(1.0 - scene_info.sky_fade_rate, TIMESTEP);

    // Copy/update generic parameters (Must be *before* fog)
    if (!sun_values_valid) {
        *sun = sky_work->sun_work;
        sun_values_valid = true;
        init = true;
        ratio = 0.0;
    } else {
        // Blend against current values based on timestep
        SunLight temp1 = {0};
        SunLight temp2 = {0};
        ParserCopyStruct(SunLightParseInfo, (void*)&sky_work->sun_work, sizeof(temp1), &temp1);
        shDoOperationSetFloat(1-ratio);
        shDoOperation(STRUCTOP_MUL, SunLightParseInfo, &temp1, OPERAND_FLOAT);
        ParserCopyStruct(SunLightParseInfo, (void*)sun, sizeof(temp2), &temp2);
        shDoOperationSetFloat(ratio);
        shDoOperation(STRUCTOP_MUL, SunLightParseInfo, &temp2, OPERAND_FLOAT);
        shDoOperation(STRUCTOP_ADD, SunLightParseInfo, &temp1, &temp2);
        ParserCopyStruct(SunLightParseInfo, (void*)&temp1, sizeof(temp1), sun);
    }

    if(!dof_values_valid) {
        dofBlend(&sky_work->sun_work.dof_values, &sky_work->sun_work.dof_values, 1.0, &sun->dof_values);
        dof_values_valid = true;
    }

    sun->sky_node_list = list_temp; // restore sky_node_list

    // Apply Fog
    {
        U8 color[3];
        FogVals fog_vals[2] = {0};
        F32 speed;

        if (sceneFileFog(&fog_out)) {
            // Using scene file fog
        } else {
            fog_out = sky_work->fog_curr;
        }

        PERFINFO_AUTO_START("fogUpdate", 1);
        color[0] = fog_out.color[0]*255;
        color[1] = fog_out.color[1]*255;
        color[2] = fog_out.color[2]*255;
        // May set fog_vals[0+1] from indoor
        speed = fogUpdate(fog_out.dist[0], fog_out.dist[1], color, scene_info.colorFogNodes, scene_info.doNotOverrideFogNodes, fog_vals); // Update fog values from Fog nodes
        PERFINFO_AUTO_STOP();

        // If fog_vals have been set, blends fog_vals[0/1] into fog_out
        dualColoredFogBlend(fog_vals, &fog_out);

        fogBlendWithLast(fog_final, &fog_out, speed); // Sets fog_final from fog_curr

        skySetRenderValues( fog_final->dist, fog_final->color, sun );  
    }
    // End Apply Fog

    // Set alpha on gfxnodes
    {
        int i;
        blendSkyNodes(&sky_work->sun_work.sky_node_list, &sun->sky_node_list, ratio);

        // Set alpha
        for (i=0; i<sun->sky_node_list.count; i++) 
            gfxNodeSetAlpha(sun->sky_node_list.nodes[i].node,(U8)((sun->sky_node_list.nodes[i].alpha) * 255),1);
    }
    // End alpha on gfxnodes

    // Debug
    showLightDebug(sun, fog_final, &fog_out);
}

static void sunUpdateGlobals(void)
{
    //Set the global time to betwen 0 and 24 (do outside this function?)
    F32 time = server_visible_state.time + server_visible_state.timescale * TIMESTEP / (60.f * 60.f * 30.f);
    server_visible_state.time = time = fixTime(time);
    //End Set global time

    initLightingMinMaxes();
}

static void sunInitNodes(SkyNodeList *dest, const SkyNodeList *src)
{
    int i;
    
    for (i=0; i<src->count; i++) {
        SkyNode *node = dynArrayAddStruct(&dest->nodes, &dest->count, &dest->max);
        *node = src->nodes[i];
    }
}

void sunUpdate(SunLight OUT *sun, int IN init)
{
    int i;
    int skyFade1 = skyFadeOverride?skyFadeOverride1:game_state.skyFade1;
    int skyFade2 = skyFadeOverride?skyFadeOverride2:game_state.skyFade2;
    F32 skyFadeWeight = skyFadeOverride?skyFadeOverrideWeight:game_state.skyFadeWeight;
    if (skyFadeOverride > 1)
        skyFadeOverride = 1;
    if (skyFadeOverride < 0)
        skyFadeOverride = 0;
    // Do any init
    if (!sky_gfx_tree_inited)
    {
        initializeSky();

        // Don't do sky initialization until stage 2 data is available
        if (LWC_IsActive() && LWC_GetLastCompletedDataStage() < LWC_STAGE_2)
            init = 0;
    }

    // Update global sun information
    sunUpdateGlobals();
    
    if(init)
        sun->sky_node_list.count = 0;

    // Update individual skies
    for (i=0; i<eaSize(&g_skies); i++) {
        sunUpdateInternal(g_skies[i], &g_skies[i]->sky_work.sun_work, init);
        if(init)
            sunInitNodes(&sun->sky_node_list, &g_skies[i]->sky_work.sun_work.sky_node_list);
    }
    sky_gfx_tree_inited = 1;
    if (skyFade1 >= eaSize(&g_skies))
        skyFade1 = 0;
    if (skyFade2 >= eaSize(&g_skies))
        skyFade2 = 0;
    if (skyFade1==skyFade2) {
        collapseSkyNodes(&g_skies[skyFade1]->sky_work.sun_work.sky_node_list);
        sunApplyValues(&g_skies[skyFade1]->sky_work, sun, &fog_final);
    } else if (skyFadeWeight == 0.0) {
        collapseSkyNodes(&g_skies[skyFade1]->sky_work.sun_work.sky_node_list);
        sunApplyValues(&g_skies[skyFade1]->sky_work, sun, &fog_final);
    } else if (skyFadeWeight== 1.0) {
        collapseSkyNodes(&g_skies[skyFade2]->sky_work.sun_work.sky_node_list);
        sunApplyValues(&g_skies[skyFade2]->sky_work, sun, &fog_final);
    } else {
        static SkyWorkingValues result={0}; // Static to reuse sky_node_list.nodes pointer
        // Blend SunLights together (snapping)
        // 0.0 = sky1, 1.0 = sky2
        sunBlendSkies(&g_skies[skyFade1]->sky_work, &g_skies[skyFade2]->sky_work, 1.0 - skyFadeWeight, &result);
        // Remove redundant info
        collapseSkyNodes(&result.sun_work.sky_node_list);
        // Blend resulting value with current (smooth)
        sunApplyValues(&result, sun, &fog_final);
    }
}

void sunSetSkyFadeClient(int sky1, int sky2, F32 weight) // 0.0 = all sky1
{
    // Note: gets overridden by mapservers
    // Do *not* call this in sceneLoad/skyLoad, otherwise artists can't reload scene files and test things easily
    game_state.skyFade1 = sky1;
    game_state.skyFade2 = sky2;
    game_state.skyFadeWeight = weight;
}

void sunSetSkyFadeOverride(int sky1, int sky2, F32 weight) // Override for volume triggers
{
    skyFadeOverride++; // Increment so we can leave one and enter another on the same frame
    skyFadeOverride1 = sky1;
    skyFadeOverride2 = sky2;
    skyFadeOverrideWeight = weight;
}

void sunUnsetSkyFadeOverride(void) // Override for volume triggers
{
    skyFadeOverride--;
}



TokenizerParseInfo suntime_parse[] =
{
    { "Time",            TOK_F32(SunTime, time, 0)                },
    { "FogColor",        TOK_VEC3(SunTime, fogcolor)        }, //div 256
    { "FogDist",        TOK_VEC2(SunTime, fogdist)            },
    { "DOFFocusDist",    TOK_F32(SunTime, dof_values.focusDistance,    DEFAULT_VALUE)},
    { "DOFFocusValue",    TOK_F32(SunTime, dof_values.focusValue,        DEFAULT_VALUE)},
    { "DOFNearValue",    TOK_F32(SunTime, dof_values.nearValue,        DEFAULT_VALUE)},
    { "DOFNearDist",    TOK_F32(SunTime, dof_values.nearDist,            DEFAULT_VALUE)},
    { "DOFFarValue",    TOK_F32(SunTime, dof_values.farValue,            DEFAULT_VALUE)},
    { "DOFFarDist",        TOK_F32(SunTime, dof_values.farDist,            DEFAULT_VALUE)},
    { "HighFogColor",    TOK_VEC3(SunTime, highfogcolor)        },//div 256
    { "BackGroundColor",TOK_VEC3(SunTime, backgroundcolor)    },//div 256
    { "Ambient",        TOK_VEC3(SunTime, ambient)            },//div 256
    { "LightmapDiffuse",TOK_VEC3(SunTime, lightmap_diffuse)    },// Unused, but kept for compatibility
    { "Diffuse",        TOK_VEC3(SunTime, diffuse)            },//div 256
    { "DiffuseReverse",    TOK_VEC3(SunTime, diffuse_reverse)    },//div 256
    { "Diffuse1",        TOK_REDUNDANTNAME|TOK_VEC3(SunTime, diffuse)            },//div 256
    { "Diffuse2",        TOK_REDUNDANTNAME|TOK_VEC3(SunTime, diffuse_reverse)    },//div 256
    { "SkyName",        TOK_STRING(SunTime, skyname, 0)            },
    { "MoonScale",        TOK_F32(SunTime, moon_scales[1], 0)    },
    { "SunScale",        TOK_F32(SunTime, moon_scales[0], 0)    },
    { "MoonScale2",        TOK_F32(SunTime, moon_scales[2], 0)    },
    { "MoonScale3",        TOK_F32(SunTime, moon_scales[3], 0)    },
    { "MoonScale4",        TOK_F32(SunTime, moon_scales[4], 0)    },
    { "MoonScale5",        TOK_F32(SunTime, moon_scales[5], 0)    },
    { "MoonScale6",        TOK_F32(SunTime, moon_scales[6], 0)    },
    { "MoonScale7",        TOK_F32(SunTime, moon_scales[7], 0)    },
    { "MoonScale8",        TOK_F32(SunTime, moon_scales[8], 0)    },
    { "MoonScale9",        TOK_F32(SunTime, moon_scales[9], 0)    },
    { "MoonScale10",    TOK_F32(SunTime, moon_scales[10], 0)    },
    { "MoonScale11",    TOK_F32(SunTime, moon_scales[11], 0)    },
    { "MoonScale12",    TOK_F32(SunTime, moon_scales[12], 0)    },
    { "MoonScale13",    TOK_F32(SunTime, moon_scales[13], 0)    },
    { "MoonScale14",    TOK_F32(SunTime, moon_scales[14], 0)    },
    { "MoonScale15",    TOK_F32(SunTime, moon_scales[15], 0)    },
    { "MoonScale16",    TOK_F32(SunTime, moon_scales[16], 0)    },
    { "MoonScale17",    TOK_F32(SunTime, moon_scales[17], 0)    },
    { "MoonScale18",    TOK_F32(SunTime, moon_scales[18], 0)    },
    { "MoonScale19",    TOK_F32(SunTime, moon_scales[19], 0)    },
    { "MoonScale20",    TOK_F32(SunTime, moon_scales[20], 0)    },
    { "MoonScale21",    TOK_F32(SunTime, moon_scales[21], 0)    },
    { "MoonScale22",    TOK_F32(SunTime, moon_scales[22], 0)    },
    { "MoonScale23",    TOK_F32(SunTime, moon_scales[23], 0)    },
    { "MoonScale24",    TOK_F32(SunTime, moon_scales[24], 0)    },
    { "MoonScale25",    TOK_F32(SunTime, moon_scales[25], 0)    },
    { "MoonScale26",    TOK_F32(SunTime, moon_scales[26], 0)    },
    { "MoonScale27",    TOK_F32(SunTime, moon_scales[27], 0)    },
    { "MoonScale28",    TOK_F32(SunTime, moon_scales[28], 0)    },
    { "MoonScale29",    TOK_F32(SunTime, moon_scales[29], 0)    },
    { "MoonScale30",    TOK_F32(SunTime, moon_scales[30], 0)    },
    { "MoonScale31",    TOK_F32(SunTime, moon_scales[31], 0)    },
    { "SunAlpha",        TOK_F32(SunTime, moon_alphas[0], 0)    },
    { "MoonAlpha",        TOK_F32(SunTime, moon_alphas[1], 0)    },
    { "MoonAlpha2",        TOK_F32(SunTime, moon_alphas[2], 0)    },
    { "MoonAlpha3",        TOK_F32(SunTime, moon_alphas[3], 0)    },
    { "MoonAlpha4",        TOK_F32(SunTime, moon_alphas[4], 0)    },
    { "MoonAlpha5",        TOK_F32(SunTime, moon_alphas[5], 0)    },
    { "MoonAlpha6",        TOK_F32(SunTime, moon_alphas[6], 0)    },
    { "MoonAlpha7",        TOK_F32(SunTime, moon_alphas[7], 0)    },
    { "MoonAlpha8",        TOK_F32(SunTime, moon_alphas[8], 0)    },
    { "MoonAlpha9",        TOK_F32(SunTime, moon_alphas[9], 0)    },
    { "MoonAlpha10",    TOK_F32(SunTime, moon_alphas[10], 0)    },
    { "MoonAlpha11",    TOK_F32(SunTime, moon_alphas[11], 0)    },
    { "MoonAlpha12",    TOK_F32(SunTime, moon_alphas[12], 0)    },
    { "MoonAlpha13",    TOK_F32(SunTime, moon_alphas[13], 0)    },
    { "MoonAlpha14",    TOK_F32(SunTime, moon_alphas[14], 0)    },
    { "MoonAlpha15",    TOK_F32(SunTime, moon_alphas[15], 0)    },
    { "MoonAlpha16",    TOK_F32(SunTime, moon_alphas[16], 0)    },
    { "MoonAlpha17",    TOK_F32(SunTime, moon_alphas[17], 0)    },
    { "MoonAlpha18",    TOK_F32(SunTime, moon_alphas[18], 0)    },
    { "MoonAlpha19",    TOK_F32(SunTime, moon_alphas[19], 0)    },
    { "MoonAlpha20",    TOK_F32(SunTime, moon_alphas[20], 0)    },
    { "MoonAlpha21",    TOK_F32(SunTime, moon_alphas[21], 0)    },
    { "MoonAlpha22",    TOK_F32(SunTime, moon_alphas[22], 0)    },
    { "MoonAlpha23",    TOK_F32(SunTime, moon_alphas[23], 0)    },
    { "MoonAlpha24",    TOK_F32(SunTime, moon_alphas[24], 0)    },
    { "MoonAlpha25",    TOK_F32(SunTime, moon_alphas[25], 0)    },
    { "MoonAlpha26",    TOK_F32(SunTime, moon_alphas[26], 0)    },
    { "MoonAlpha27",    TOK_F32(SunTime, moon_alphas[27], 0)    },
    { "MoonAlpha28",    TOK_F32(SunTime, moon_alphas[28], 0)    },
    { "MoonAlpha29",    TOK_F32(SunTime, moon_alphas[29], 0)    },
    { "MoonAlpha30",    TOK_F32(SunTime, moon_alphas[30], 0)    },
    { "MoonAlpha31",    TOK_F32(SunTime, moon_alphas[31], 0)    },
    { "ShadowColor",    TOK_FIXED_ARRAY | TOK_F32_X, offsetof(SunTime, shadowcolor),    3 },//div 256
    { "ShadowAlpha",    TOK_F32(SunTime, shadowcolor[3], 0)    },//div 256
    { "Luminence",        TOK_REDUNDANTNAME|TOK_F32(SunTime, luminance, 0)        },//div 255
    { "Luminance",        TOK_F32(SunTime, luminance, 0)        },//div 255
    { "End",            TOK_END                                            },
    { "", 0, 0 }
};

TokenizerParseInfo cloud_parse[] =
{
    { "Name",        TOK_STRING(Cloud, name, 0)        },
    { "Height",        TOK_VEC2(Cloud, heights)    },
    { "CloudFadeTime",    TOK_VEC2(Cloud, fadetimes)    },
    { "CloudFadeMin",    TOK_F32(Cloud, fademin, -1)    },
    { "ScrollRatio",TOK_IGNORE,                                },
    { "End",        TOK_END                                    },
    { "", 0, 0 }
};

TokenizerParseInfo sky_parse[] =
{
    { "MoonName",        TOK_UNPARSED(Sky, moon_names)        },
    { "CloudFadeTime",    TOK_VEC2(Sky, cloud_fadetimes)    },
    { "LampLightTime",    TOK_VEC2(Sky, lamp_lighttimes)    },
    { "CloudFadeMin",    TOK_F32(Sky, cloud_fademin, 0)    },
    { "FogHeightRange",    TOK_VEC2(Sky, fogheightrange)    },
    { "FogDist",        TOK_VEC2(Sky, fogdist)            },
    { "LightmapTime",    TOK_F32(Sky, lightmap_time,            11)    },
    { "LightmapSunAngle", TOK_F32(Sky, lightmap_sun_angle,        1)    },
    { "LightmapSunBrightness", TOK_F32(Sky, lightmap_sun_brightness, 256)    },
    { "LightmapSkyBrightness", TOK_F32(Sky, lightmap_sky_brightness, 200)    },
    { "End",            TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo all_parse[] =
{
    { "sun",        TOK_EMBEDDEDSTRUCT(UberSky, sky_struct, sky_parse)    },
    { "cloud",        TOK_STRUCT(UberSky, clouds, cloud_parse)            },
    { "suntime",    TOK_STRUCT(UberSky, times, suntime_parse)            },
    { "ignoreLightClamping",    TOK_BOOL(UberSky, ignoreLightClamping, 0)    },
    { "", 0, 0 },
};


void sceneAdjustColor(Vec3 rgb_in,Vec3 rgb)
{
    int            i,count;
    ColorSwap    **swaps;
    F32            mag = 1,inv_mag;
    Vec3        hsv,total;
    ColorSwap    *swap;

    copyVec3(rgb_in,rgb);
    swaps = sceneGetColorSwaps(&count);
    if (!count)
        return;
    mag = MAX(mag,rgb[0]);
    mag = MAX(mag,rgb[1]);
    mag = MAX(mag,rgb[2]);
    inv_mag = 1.f/mag;
    scaleVec3(rgb,inv_mag,rgb);
    rgbToHsv(rgb,hsv);
    for(i=0;i<count;i++)
    {
        swap = swaps[i];
        if (hsv[0] >= swap->match_hue[0] && hsv[0] < swap->match_hue[1] || (!swap->match_hue[0] && !swap->match_hue[1]))
        {
            if (swap->reset)
                zeroVec3(hsv);
            hsvAdd(hsv,swaps[i]->hsv,total);
            hsvToRgb(total,rgb);
            if (swap->magnify)
                mag *= swap->magnify;
            break;
        }
    }
    scaleVec3(rgb,mag,rgb);
}

static void adjustScaleColor(Vec3 rgb)
{
    Vec3    rgb_temp;
    const float f = 1.f/256.f;

    if (!rgb[0] && !rgb[1] && !rgb[2])
        return;
    scaleVec3(rgb,f,rgb_temp);
    sceneAdjustColor(rgb_temp,rgb);
}

static void divideNumbersby256(UberSky *sky)
{
    float        f = 1.f/256.f;
    int            i;
    int            count;
    ColorSwap    **swaps = sceneGetColorSwaps(&count);

    for(i=0;i<count;i++)
    {
        swaps[i]->hsv[1] *= 0.01;
        swaps[i]->hsv[2] *= 0.01;
    }
    for( i = 0; i < sky->sun_time_count; i++ )
    {
        adjustScaleColor( sky->times[i]->fogcolor);
        adjustScaleColor( sky->times[i]->highfogcolor);
        adjustScaleColor( sky->times[i]->backgroundcolor);
        adjustScaleColor( sky->times[i]->ambient);
        adjustScaleColor( sky->times[i]->diffuse);
        adjustScaleColor( sky->times[i]->shadowcolor);
        sky->times[i]->shadowcolor[3] *= f;

        if (sky->times[i]->luminance) {
            sky->times[i]->luminance *=1.f/255.f;
        } else {
            Vec3 luminance_vector = {0.2125, 0.7154, 0.0721};
            sky->times[i]->luminance = dotVec3(luminance_vector, sky->times[i]->ambient) +
                dotVec3(luminance_vector, sky->times[i]->diffuse);
        }

        // Set defaults (can't use parser because of float<->int conversion)
#define SET_DEFAULT(f, v) if (sky->times[i]->dof_values.f == DEFAULT_VALUE) sky->times[i]->dof_values.f = v;
        // TODO: Should we check isIndoors()?
        if (onIndoorMission()) {
            SET_DEFAULT(focusDistance, 200);
            SET_DEFAULT(focusValue, 0);
            SET_DEFAULT(nearValue, 0.2);
            SET_DEFAULT(nearDist, 0.73);
            SET_DEFAULT(farValue, 0.60);
            SET_DEFAULT(farDist, 1200);
        } else {
            SET_DEFAULT(focusDistance, 400);
            SET_DEFAULT(focusValue, 0);
            SET_DEFAULT(nearValue, 0.2);
            SET_DEFAULT(nearDist, 0.73);
            SET_DEFAULT(farValue, 0.6);
            SET_DEFAULT(farDist, 2080);
        }
#undef SET_DEFAULT
    }
}

static void setDefaultMoonInfo(UberSky *sky)
{
    int        i,j;
    F32        scale,alpha;
    SunTime    *suntime;

    for(j=0;j<MAXMOONS;j++)
    {
        scale = alpha = 0;
        for(i=0;i<sky->sun_time_count;i++)
        {
            suntime = sky->times[i];
            alpha += suntime->moon_alphas[j];
            scale += suntime->moon_scales[j];
        }
        for(i=0;i<sky->sun_time_count;i++)
        {
            suntime = sky->times[i];
            suntime->moon_alphas[j] /= 255;
            if (!alpha)
                suntime->moon_alphas[j] = 1;
            if (!scale)
                suntime->moon_scales[j] = 1;
        }
    }
}

static void skyLoadInternal( const char *skyFileName, const char *sceneFileName, UberSky OUT *sky, SunLight OUT *sun)
{
    TokenizerHandle tok;

    ParserDestroyStruct(all_parse, sky);    
    tok = TokenizerCreate(skyFileName);
    if( !tok )
        FatalErrorf( "This is not a valid sky file %s ", skyFileName );
    //ParserLoadFiles("scenes/skies/", ".txt", "scenes.bin", 1, all_parse, &sky, NULL);
    TokenizerParseList( tok, all_parse, sky, TokenizerErrorCallback );
    TokenizerDestroy(tok);

    //Initialize Parsed Files, set 
    sky->sun_time_count        = eaSize( &sky->times );
    sky->moon_count            = eaSize( &sky->sky_struct.moon_names );
    sky->cloud_count        = eaSize( &sky->clouds );
    setDefaultMoonInfo(sky);
    divideNumbersby256(sky);

}

void skyLoad( char ** skyFileNames, char *sceneFileName )
{
    char    buf[1000];
    int i;
    for (i=0; i<eaSize(&g_skies); i++) {
        destroySky(g_skies[i]);
        ParserDestroyStruct(all_parse, g_skies[i]);
        ParserFreeStruct(g_skies[i]);
        g_skies[i] = NULL;
    }
    eaDestroy(&g_skies);

    initializeSky();

    for (i=0; i<eaSize(&skyFileNames); i++) 
    {
        eaPush(&g_skies, ParserAllocStruct(sizeof(*g_skies[i])));
        sprintf(buf,"scenes/skies/%s",skyFileNames[i]);
        if (!fileExists(buf)) {
            ErrorFilenamef(sceneFileName, "Scene file references missing sky file: %s", skyFileNames[i]);
        } else {
            skyLoadInternal(buf, sceneFileName, g_skies[i], &g_skies[i]->sky_work.sun_work);
        }
    }
    sunUpdate(&g_sun, SUN_INITIALIZE);
}

TexSwap **sceneGetTexSwaps(int *count)
{
    *count = eaSize(&scene_info.tex_swaps);
    return scene_info.tex_swaps;
}

TexSwap **sceneGetMaterialSwaps(int *count)
{
    *count = eaSize(&scene_info.material_swaps);
    return scene_info.material_swaps;
}

ColorSwap **sceneGetColorSwaps(int *count)
{
    *count = eaSize(&scene_info.color_swaps);
    return scene_info.color_swaps;
}

F32 *sceneGetAmbient()
{
    F32    *c = scene_info.force_ambient_color;

    if (c[0] > -1024)
        return c;
    return 0;
}

F32 *sceneGetMinCharacterAmbient(void)
{
    F32 *c = scene_info.character_min_ambient_color;

    if (c[0] > -1024)
        return c;
    return 0;
}

F32 *sceneGetCharacterAmbientScale(void)
{
    F32 *c = scene_info.character_ambient_scale;

    if (c[0] >= 0)
        return c;
    return 0;
}

static SunLight sun_stack[1];
static int sun_stack_depth=0;
void sunPush(void)
{
    assert(sun_stack_depth==0);
    sun_stack[sun_stack_depth] = g_sun;
    sun_stack_depth++;
}

void sunPop(void)
{
    assert(sun_stack_depth==1);
    sun_stack_depth--;
    g_sun = sun_stack[sun_stack_depth];
}
