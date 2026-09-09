#ifndef _RENDERSTATE_H
#define _RENDERSTATE_H

#include <utilitieslib/stdtypes.h>

//#define MODEL_PERF_TIMERS // Timer for each model
//#define MODEL_DRAW_TIMERS // Time the model drawing code
#ifdef MODEL_DRAW_TIMERS
#    define MODEL_PERFINFO_AUTO_START PERFINFO_AUTO_START
#    define MODEL_PERFINFO_AUTO_STOP_START PERFINFO_AUTO_STOP_START
#    define MODEL_PERFINFO_AUTO_STOP PERFINFO_AUTO_STOP
#else
#    define MODEL_PERFINFO_AUTO_START(a,b)
#    define MODEL_PERFINFO_AUTO_STOP_START(a,b)
#    define MODEL_PERFINFO_AUTO_STOP()
#endif

extern F32 constColor0[4],constColor1[4];

typedef enum DrawModeType
{
    DRAWMODE_SPRITE = 1,
    DRAWMODE_DUALTEX,
    DRAWMODE_COLORONLY,
    DRAWMODE_DUALTEX_NORMALS,
    DRAWMODE_DUALTEX_LIT_PP,            // vertex shader for per pixel lighting of simple materials previously only vertex lighting (used when shadowmappig is enabled), DRAWMODE_DUALTEX_NORMALS variant
    DRAWMODE_FILL,
    DRAWMODE_BUMPMAP_SKINNED,
    DRAWMODE_HW_SKINNED,
    DRAWMODE_BUMPMAP_NORMALS,
    DRAWMODE_BUMPMAP_NORMALS_PP,
    DRAWMODE_BUMPMAP_DUALTEX,
    DRAWMODE_BUMPMAP_RGBS,
    DRAWMODE_BUMPMAP_MULTITEX,
    DRAWMODE_BUMPMAP_MULTITEX_RGBS,
    DRAWMODE_BUMPMAP_SKINNED_MULTITEX,
    DRAWMODE_DEPTH_ONLY,
    DRAWMODE_DEPTHALPHA_ONLY,
    DRAWMODE_DEPTH_SKINNED,
    DRAWMODE_NUMENTRIES
} DrawModeType;

typedef enum BlendModeShader
{
    BLENDMODE_MODULATE,
    BLENDMODE_MULTIPLY,
    BLENDMODE_COLORBLEND_DUAL,
    BLENDMODE_ADDGLOW,            // building windows, neon, etc
    BLENDMODE_ALPHADETAIL,
    BLENDMODE_BUMPMAP_MULTIPLY,
    BLENDMODE_BUMPMAP_COLORBLEND_DUAL,
    BLENDMODE_WATER,
    BLENDMODE_MULTI,
    BLENDMODE_SUNFLARE,
    BLENDMODE_NUMENTRIES
} BlendModeShader;

typedef enum BlendModeBits
{
    //**
    // The first set of bits should be bits that are used to
    // define and select different shader variations through
    // compile time definitions.
    //
    // When we are compiling fragment programs we will iterate
    // over all the possible meaningful permutations of these
    // bits and compile a shader variant optimized for only those
    // capabilities.

    // generic blendmode bits applicable to any shader
    BMB_DEFAULT                =0,
    BMB_HIGH_QUALITY        =1<<0,
    BMB_SHADOWMAP            =1<<1,    // ultra feature
    BMB_CUBEMAP                =1<<2,    // ultra feature
    BMB_PLANAR_REFLECTION    =1<<3,    // ultra feature

    // blendmode bits specific to the MULTI material 'optimized' variants
    BMB_SINGLE_MATERIAL        =1<<4,        
    BMB_BUILDING            =1<<5,

    // Special "Development Only" shader variations
#ifndef FINAL
    BMB_DEBUG                =1<<6,                        // debug variant
    BMB_VARIANT_COUNT        =1<<7,                        // total possible permutations of compiled shader variants
#else
    BMB_VARIANT_COUNT        =1<<6,                        // total possible permutations of compiled shader variants
#endif

    BMB_VARIANT_MASK        = BMB_VARIANT_COUNT - 1,

    //**
    // There are additional blendmode bits that are used to
    // choose particular rendering options but these are handled
    // either through shader program constants (e.g BMB_OLDTINTMODE)
    // or only apply to a limited number of hardware configurations 
    // and are specially handled.

    // @todo update assets and deprecate BMB_OLDTINTMODE support
    BMB_OLDTINTMODE            =1<<14,    // Chooses and old style dual blending in the MULTI material (currently very limited use in assets)

    BMB_NV1XWORLD            =1<<15, // for nVidia legacy register combiner logic, not actually a fragment program variation
} BlendModeBits;

typedef union _BlendModeType {
    U32 intval;
    struct {
        S16 shader;            // type BlendModeShader    @todo currently needs to be signed until logic in resetModelFlags() and sort compares are updated
        U16 blend_bits;        // type BlendModeBits
    };
} BlendModeType;

BlendModeType BlendMode(BlendModeShader shader, BlendModeBits blend_bits);
bool eqBlendMode(BlendModeType a, BlendModeType b);

extern char *blend_mode_names[BLENDMODE_NUMENTRIES];

extern BlendModeType curr_blend_state;
extern int curr_blend_lightmap_use;
extern DrawModeType curr_draw_state;
extern int curr_draw_lightmap_use;

typedef enum RenderPassType {
    RENDERPASS_UNKNOWN,
    RENDERPASS_COLOR,
    RENDERPASS_SHADOW_CASCADE1,        // Keep the shadow cacscades right after color since
    RENDERPASS_SHADOW_CASCADE2,        // the view cache has issues if other viewports are
    RENDERPASS_SHADOW_CASCADE3,        // rendered between shadow cascades and color viewports
    RENDERPASS_SHADOW_CASCADE4,
    RENDERPASS_CUBEFACE,
    RENDERPASS_REFLECTION,
    RENDERPASS_REFLECTION2,            // used by water if game_state.separate_reflection_viewport
    RENDERPASS_IMAGECAPTURE,        // used by some non-runtime image-capture logic
    RENDERPASS_TAKEPICTURE,            // used by runtime image-capture logic
    RENDERPASS_COUNT,
    RENDERPASS_SHADOW_CASCADE_LAST = RENDERPASS_SHADOW_CASCADE4,
    RENDERPASS_STATS_COUNT = RENDERPASS_REFLECTION2 + 1, // last pass that we display renderstats for, broken out by pass
} RenderPassType;

typedef struct RdrFrameState
{
    U32     curFrame;
    U8      sliClear;
    U8      numFBOs;
    U8      sliLimit;
} RdrFrameState;

typedef struct RdrViewState
{
    Vec4    sun_ambient;
    Vec4    sun_diffuse;
    Vec4    sun_diffuse_reverse;
    Vec4    sun_ambient_for_players;
    Vec4    sun_diffuse_for_players;
    Vec4    sun_diffuse_reverse_for_players;
    Vec4    sun_direction;
    Vec4    sun_direction_in_viewspace;
    Vec4    sun_no_angle_light;
    Vec3    shadow_direction;
    Vec3    shadowmap_light_direction;
    Vec3    shadowmap_light_direction_in_viewspace;
    F32        shadowmap_extrusion_scale;
    F32        lowest_visible_point_last_frame;
    Vec3    sun_position;
    Vec4    shadowcolor;
    Mat4    viewmat;
    Mat4    inv_viewmat;
    F32        nearz,farz;
    Mat44    projectionMatrix;
    Mat44    projectionClipMatrix;        // sheared version of projection to do a user clip plane (e.g. reflection clipping)
    F32        gloss_scale;
    U32        gfxdebug;
    U32        wireframe : 3;
    U32        shadowvol : 3;
    U32        wireframe_seams : 1;
    U32        force_double_sided : 1;
    U32        write_depth_only : 1;
    U32        fixed_sun_pos : 1;
    int        width,height;    // window size
    int        origWidth, origHeight; // Saved values if pbuffers changed them
    bool    override_window_size;
    F32        client_loop_timer;
    F32        time;
    F32        lamp_alpha;
    F32        toneMapWeight;
    F32        bloomWeight;
    F32        luminance;
    F32        desaturateWeight;
    U8        bloomScale;
    U8        lamp_alpha_byte;
    int        white_tex_id, dummy_bump_tex_id, grey_tex_id, black_tex_id, building_lights_tex_id;
    int        texAnisotropic;
    int        antialiasing;
    int        renderScaleFilter;
    Vec3    fogcolor;
    Vec2    fogdist;
    F32        dofFocusDistance;
    F32        dofFocusValue;
    F32        dofNearDist;
    F32        dofNearValue;
    F32        dofFarDist;
    F32        dofFarValue;
    F32        base_reflectivity;
    S32        renderPass;

    int        cubemapMode;
    int        shadowMode;

    F32        waterRefractionSkewNormalScale;
    F32        waterRefractionSkewDepthScale;
    F32        waterRefractionSkewMinimum;
    int        waterReflectionOverride;
    F32        waterReflectionSkewScale;
    F32        waterReflectionStrength;
    F32        waterFresnelBias;
    F32        waterFresnelScale;
    F32        waterFresnelPower;
    F32        waterTODscale;

} RdrViewState;

extern RdrFrameState rdr_frame_state;
extern RdrViewState    rdr_view_state;

typedef enum ModelStateSetFlags {
    ONLY_IF_NOT_ALREADY = 0,
    FORCE_SET = 1<<0,
    BIT_HIGH_QUALITY_VERTEX_PROGRAM = 1<<1,            // Bit used to select "High Quality" Vertex Program variant for DRAWSTATE
} ModelStateSetFlags;

void OnGLContextChange(void);
void modelBlendStateInit(void);
char *renderBlendName(BlendModeType blend);
void modelBlendState(BlendModeType num, int force_set);
void modelDrawState(DrawModeType num, ModelStateSetFlags flags);
void rdrPrepareForDraw(void); // Last minute GL parameter changes based on blend mode
void modelSetAlpha(U8 a);
void rdrInitTopOfFrameDirect(RdrFrameState *rdr);
void rdrInitTopOfViewDirect(RdrViewState *rdr);
void rdrFinish2DRendering(void);
void rdrSetup2DRendering(void);
void bumpInit(void);
BlendModeType promoteBlendMode(BlendModeType oldBlendMode, BlendModeType newBlendMode);
bool blendModeHasBump(BlendModeType blend);
bool blendModeHasDualColor(BlendModeType blend);

#ifndef FINAL
#define MARKERS_ENABLED
#endif

#ifdef MARKERS_ENABLED

void rdrBeginMarkerFrame();
void rdrBeginMarkerThreadFrame(int enable);

extern __declspec(thread) int markersEnabled;
void _rdrBeginMarker(const char * name, ...);
void _rdrEndMarker(void);
void _rdrSetMarker(const char * msg, ...);

#define rdrBeginMarker(...) do { \
	if (!markersEnabled) \
		__nop(); \
	else \
		_rdrBeginMarker(__VA_ARGS__); \
} while (0)
#define rdrEndMarker() do { if(!markersEnabled) { __nop(); } else { _rdrEndMarker(); } } while(0)
#define rdrSetMarker(...) do { \
	if (!markersEnabled) \
		__nop(); \
	else \
		_rdrSetMarker(__VA_ARGS__); \
} while (0)

#else

#define rdrBeginMarkerFrame() do {} while(0)
#define rdrBeginMarkerThreadFrame(_enable) do {} while(0)

#define rdrBeginMarker(...) do {} while(0)
#define rdrEndMarker() do {} while(0)
#define rdrSetMarker(...) do {} while(0)

#endif

#endif
