// Graphics Development HUD and debug shader control
#include "graphics/gfxDevHUD.h"
#include "graphics/gfxDebug.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "graphics/font.h"
#include "render/tex.h"
#include "render/tex_gen.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiInput.h"
#include "render/thread/rt_util.h"
#include "win/input.h"
#include <utilitieslib/utils/file.h> // for isDevelopmentOrQAMode
#include "render/renderUtil.h"
#include "UI/sprite/sprite_base.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/Cbox.h>
#include "win/win_init.h"
#include "UI/uiGame.h"
#include "UI/uiCursor.h"

#ifndef FINAL

ShaderDebugState g_mt_shader_debug_state;    // main thread view of shader debugging state

typedef enum EDevHudState
{
    kDH_Inactive,
    kDH_Activating,
    kDH_Loading,
    kDH_Active,
    kDH_Deactivating,
} EDevHudState;

static EDevHudState        g_dev_hud_state = kDH_Inactive;
static EDevHudState        g_dev_hud_state_next = kDH_Inactive;

static bool                g_hud_at_bottom = true;
static int                g_selected_tex = -1;
static int                g_selected_visualization = -1;

static int                g_uv_test_tex_id;

static int                g_popup_menu_id;
static int                g_pop_origin_x, g_pop_origin_y;
static mouse_input        g_hud_last_mouse, g_hud_last_click;
static int                g_restore_gfx_features;

#define kSD_CellWidth 100.0f
#define kSD_CellHeight 22.0f
#define kSD_GridRows 3
#define kSD_GridCols 11

#define kFontHeight        12
#define kNumberPanelSelectors        8
#define kBarHeight_Panel_Select        20
#define kBarCellWidth_Panel_Select    100
#define kLineHeight_Panel_Select    3
#define kLineWidth_Panel_Select        kNumberPanelSelectors*(kBarCellWidth_Panel_Select+2)

#define kBarHeight        20
#define kBarWidth        kLineWidth_Panel_Select
#define kBarCellWidth    20
#define kBarMarginX        5
#define kBarMarginY        2


#define kBarHeight_Status        20
#define kBarWidth_Status        500

#define kDH_Control_Panel_Height    kSD_CellHeight*kSD_GridRows
#define kDH_HUD_Y_MARGIN                15        // offset of top/bottom of hud from top/bottom of screen

enum
{
    kSD_Control_None = 0,

    kSD_Control_HudClose,
    kSD_Control_UI,
    kSD_Control_Bottom,
    kSD_Control_Wireframe,
    kSD_Control_Pause,
    kSD_Control_UseDebugShaders,
    kSD_Control_texoff,

    kSD_Control_MiscGroup,

    kSD_Control_MaterialGroup,
    kSD_Control_Mat_VertexColor,
    kSD_Control_Mat_Base,
    kSD_Control_Mat_Dual,
    kSD_Control_Mat_Multiply,
    kSD_Control_AddGlow,
    kSD_Control_BlendAlpha,
    kSD_Control_LodAlpha,

    kSD_Control_LightingGroup,
    kSD_Control_Ambient,
    kSD_Control_Diffuse,
    kSD_Control_Specular,
    kSD_Control_LightColor,
    kSD_Control_Gloss,
    kSD_Control_Bump,
    kSD_Control_Shadowed,

    kSD_Control_FeatureGroup,
    kSD_Control_Fog,
    kSD_Control_Reflection,
    kSD_Control_SSAO,
    kSD_Control_SubMaterial1,
    kSD_Control_SubMaterial2,
    kSD_Control_Building,
//    kSD_Control_Blend,
//    kSD_Control_FauxReflection,
    kSD_Control_FeatureGroup_END,

    kSD_Control_RenderGroup = kSD_Control_FeatureGroup_END,
    kSD_Control_Render_Sky,
    kSD_Control_Render_Environment,
    kSD_Control_Render_Entity,
    kSD_Control_Render_2D,
    kSD_Control_Render_Lines,
    kSD_Control_RenderGroup_End,

    kSD_Control_PassGroup = kSD_Control_RenderGroup_End,
    kSD_Control_Render_OpaquePass,
    kSD_Control_Render_AlphaPass,
    kSD_Control_Render_UnderwaterAlphaPass,
    kSD_Control_PassGroup_End,

    kSD_Control_Render_ForceFallback = kSD_Control_PassGroup_End,
    kSD_Control_Render_ForceLOD0,
    kSD_Control_Render_ForceLOD1,
    kSD_Control_Render_ForceLOD2,
    kSD_Control_Render_ForceLOD3,

    kSD_Control_TextureMenu,

    kSD_Control_TextureGroup,
    kSD_Control_Tex_Base1,
    kSD_Control_Tex_Dual1,
    kSD_Control_Tex_Bump1,
    kSD_Control_Tex_Multiply1,
    kSD_Control_Tex_Mask,
    kSD_Control_Tex_Base2,
    kSD_Control_Tex_Dual2,
    kSD_Control_Tex_Bump2,
    kSD_Control_Tex_Multiply2,
    kSD_Control_Tex_AddGlow,
    kSD_Control_Tex_AddGlowMask,
    kSD_Control_Tex_Cubemap,
    kSD_Control_TextureGroup_End,

    kSD_Control_ChannelGroup = kSD_Control_TextureGroup_End,
    kSD_Control_Tex_RGBA,
    kSD_Control_Tex_RGB,
    kSD_Control_Tex_ALPHA,

    kSD_Control_InfoGroup,
    kSD_Control_Info_rdrstats,
    kSD_Control_Info_graphfps,
    kSD_Control_Info_rdrinfo,
    kSD_Control_Info_showspecs,
    kSD_Control_InfoGroup_End,

    kSD_Control_Viz_ColorizeGroup = kSD_Control_InfoGroup_End,
    kSD_Control_Viz_uv0,
    kSD_Control_Viz_uv1,
    kSD_Control_Viz_Tangent,
    kSD_Control_Viz_Normal,
    kSD_Control_Viz_Bitangent,
    kSD_Control_Viz_Blendmode,

    kSD_Control_Viz_VectorGroup,
    kSD_Control_Viz_normal_vectors,
    kSD_Control_Viz_tangent_vectors,
    kSD_Control_Viz_bitangent_vectors,

    kSD_Control_Viz_seams,
    kSD_Control_Viz_lightdirs,

    kSD_Control_ParticleGroup,
    kSD_Control_Particles_Systems,
    kSD_Control_Particles_Render,
    kSD_Control_Particles_Texture,
    kSD_Control_Particles_Update,
    kSD_Control_Particles_Write,
    kSD_Control_Particles_FxGeo,

    kSD_Control_FxInfoGroup, 
    kSD_Control_Info_FxBasic,
    kSD_Control_Info_FxDetail,
    kSD_Control_Info_FxVerbose,
    kSD_Control_Info_FxSound,

    // panel selector control words
    kSD_Control_Panel_Material,
    kSD_Control_Panel_Texture,
    kSD_Control_Panel_Visualize,
    kSD_Control_Panel_Render,
    kSD_Control_Panel_FX,
    kSD_Control_Panel_Info,
    kSD_Control_Panel_Engine,

    kSD_Control_Number
};

// color set for control state rendering
typedef struct SDColorSet
{
    unsigned color_bg_enabled;        // rgba
    unsigned color_bg_disabled;        // rgba
    unsigned color_roll;            // rgba

    unsigned color_text_enabled;    // fontColor RGB
    unsigned color_text_disabled;    // fontColor RGB
} SDColorSet;

typedef struct SDControl
{
    int type;
    int id;
    char* name;
    char* name_disabled;
    char* name_tex;
    char* name_tex_disabled;
    int value;
    char* help;    // help text for rollover status panel
    int rollover;
} SDControl;

enum
{
    kSD_ControlType_None,
    kSD_ControlType_Word,
    kSD_ControlType_GroupWord,
    kSD_ControlType_PanelSelectWord,
    kSD_ControlType_Icon,
    kSD_ControlType_PopupMenu,

};

static SDColorSet s_controltype_colors[] = 
{
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,  0xFFFFFF,0xFFFFFF },    // kSD_ControlType_None
    {0x101010D0, 0x101010D0, 0x000080F0,  0x00FF00,0xFFFF00},    // kSD_ControlType_Word
    {0x404040D0, 0x404040D0, 0x000080F0,  0x00FF00,0xFFFF00},    // kSD_ControlType_GroupWord
    {0xA0A0A0D0, 0x404040D0, 0x4040F0F0,  0x00FFFF,0xA0A0A0},    // kSD_ControlType_PanelSelectWord
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,  0xFFFFFF,0xFFFFFF },    // kSD_ControlType_Icon
    {0xA0A0A0D0, 0x404040D0, 0x4040F0F0,  0x00FFFF,0xA0A0A0},    // kSD_ControlType_PopupMenu
};

// definitions must currently match order in the enum
static SDControl s_controls[] =
{
    {kSD_ControlType_None},

    {kSD_ControlType_Icon, kSD_Control_HudClose, "close", "", "button_windowclose.tga", "", 1, "Close the HUD."},
    {kSD_ControlType_Icon, kSD_Control_UI, "2D/UI", "", "button_windowresize_UL.tga", "button_windowresize_LR.tga", 1, "Toggle the UI."},
    {kSD_ControlType_Icon, kSD_Control_Bottom,  "move", "", "Jelly_arrow_up.tga", "Jelly_arrow_down.tga", 1, "Change location of HUD"},
    {kSD_ControlType_Icon, kSD_Control_Wireframe,  "wireframe", "", "button_windowresize_UL.tga", "button_windowresize_LR.tga", 0, "Enable wireframe rendering"},
    {kSD_ControlType_Icon, kSD_Control_Pause,  "pause", "", "button_windowresize_UL.tga", "button_windowresize_LR.tga", 0, "Pause time"},
    {kSD_ControlType_Icon, kSD_Control_UseDebugShaders,  "debug", "", "button_windowtrayvert.tga", "button_windowtrayhoriz.tga", 1, "Toggle use of debug shaders."},
    {kSD_ControlType_Icon, kSD_Control_texoff,  "texture enable", "", "button_windowresize_UL.tga", "button_windowresize_LR.tga", 0, "Toggle texturing (/texoff)"},

    {kSD_ControlType_GroupWord, kSD_Control_MiscGroup, "misc:", "", "", "", 1,  "misc items"},
    
    {kSD_ControlType_GroupWord, kSD_Control_MaterialGroup, "material:", "", "", "", 1,  "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Mat_VertexColor, "color", "", "", "", 1, "vertex color from vertex lighting or prebaked ambient groups"},
    {kSD_ControlType_Word, kSD_Control_Mat_Base, "base", "", "", "", 1, "enable base texture or none"},
    {kSD_ControlType_Word, kSD_Control_Mat_Dual, "dual", "", "", "", 1, "enable dual texture or none"},
    {kSD_ControlType_Word, kSD_Control_Mat_Multiply, "multiply", "", "", "", 1, "enable multiply texture or none"},
    {kSD_ControlType_Word, kSD_Control_AddGlow, "add glow", "", "", "", 1, "toggle addglow"},
    {kSD_ControlType_Word, kSD_Control_BlendAlpha,  "blend alpha", "", "", "", 1, "enable material alpha"},
    {kSD_ControlType_Word, kSD_Control_LodAlpha,  "lod alpha", "", "", "", 1,"enable lod alpha"},

    {kSD_ControlType_GroupWord, kSD_Control_LightingGroup, "lighting:", "", "", "", 1, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Ambient, "ambient", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Diffuse,  "diffuse", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Specular,  "specular", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_LightColor,  "light color", "", "", "", 1,"toggle using light colors OR PURE white"},
    {kSD_ControlType_Word, kSD_Control_Gloss, "gloss fact", "", "", "", 1, "enable constant gloss factor adjustment of specular"},
    {kSD_ControlType_Word, kSD_Control_Bump, "bump", "", "", "", 1,"enable bump mapping"},
    {kSD_ControlType_Word, kSD_Control_Shadowed, "shadowed", "", "", "", 1,"enable ultra shadowing"},

    {kSD_ControlType_GroupWord, kSD_Control_FeatureGroup, "feature:", "", "", "", 1, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Fog,  "fog", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Reflection,  "reflection", "", "", "", 1,"enable ultra reflection"},
    {kSD_ControlType_Word, kSD_Control_SSAO, "ssao", "", "", "", 1, "enable ssao"},
    {kSD_ControlType_Word, kSD_Control_SubMaterial1, "sub mat 1", "", "", "", 1,"enable sub material 1 of multi material"},
    {kSD_ControlType_Word, kSD_Control_SubMaterial2, "sub mat 2", "", "", "", 1,"enable sub material 2 of multi material"},
    {kSD_ControlType_Word, kSD_Control_Building, "building", "", "", "", 1,"enable 'building' variant of multi material"},
//    {kSD_ControlType_Word, kSD_Control_Blend,  "blend", "", "", "", 1},
//    {kSD_ControlType_Word, kSD_Control_FauxReflection,  "faux", "", "", "", 1},

    {kSD_ControlType_GroupWord, kSD_Control_RenderGroup, "render:", "", "", "", 1, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Render_Sky, "sky", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Render_Environment,  "environment", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Render_Entity,  "entity", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Render_2D,  "2D/UI", "", "", "", 0},    // keep in sync as inverse of kSD_Control_UI
    {kSD_ControlType_Word, kSD_Control_Render_Lines,  "lines", "", "", "", 1},

    {kSD_ControlType_GroupWord, kSD_Control_PassGroup, "pass:", "", "", "", 1, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Render_OpaquePass, "opaque", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Render_AlphaPass,  "alpha", "", "", "", 1},
    {kSD_ControlType_Word, kSD_Control_Render_UnderwaterAlphaPass,  "underwater a", "", "", "", 1},

    {kSD_ControlType_Word, kSD_Control_Render_ForceFallback,  "fallback", "", "", "", 0, "Force rendering with fallback materials"},
    {kSD_ControlType_Word, kSD_Control_Render_ForceLOD0,  "lod 0", "", "", "", 0, "Force rendering of LOD 0 for environment models"},
    {kSD_ControlType_Word, kSD_Control_Render_ForceLOD1,  "lod 1", "", "", "", 0, "Force rendering of LOD 1 for environment models"},
    {kSD_ControlType_Word, kSD_Control_Render_ForceLOD2,  "lod 2", "", "", "", 0, "Force rendering of LOD 2 for environment models"},
    {kSD_ControlType_Word, kSD_Control_Render_ForceLOD3,  "lod 3", "", "", "", 0, "Force rendering of LOD 3 for environment models"},
    
    {kSD_ControlType_PopupMenu, kSD_Control_TextureMenu, "menu", "", "", "", 0, "Click to display texture selection menu"},

    {kSD_ControlType_GroupWord, kSD_Control_TextureGroup, "texture:", "", "", "", 0, "Texture selected to right will be shown. click to deselect."},
    {kSD_ControlType_Word, kSD_Control_Tex_Base1,    "base 1", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Dual1,    "dual 1", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Bump1,    "bump 1", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Multiply1, "multiply 1", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Mask,     "mask", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Base2,    "base 2", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Dual2,    "dual 2", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Bump2,    "bump 2", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Multiply2, "multiply 2", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_AddGlow, "addglow", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_AddGlowMask, "addglow mask", "", "", "", 0},
    {kSD_ControlType_Word, kSD_Control_Tex_Cubemap,  "cubemap", "", "", "", 0},

    {kSD_ControlType_GroupWord, kSD_Control_ChannelGroup, "channels:", "", "", "", 0, "Choose which texture color channels are shown."},
    {kSD_ControlType_Word, kSD_Control_Tex_RGBA,  "rgba", "", "", "", 0, "Display all color components"},
    {kSD_ControlType_Word, kSD_Control_Tex_RGB,  "rgb", "", "", "", 1, "Display opaque texture (drop alpha)"},
    {kSD_ControlType_Word, kSD_Control_Tex_ALPHA,  "alpha", "", "", "", 0, "Display only alpha as an opaque grayscale."},

    {kSD_ControlType_GroupWord, kSD_Control_InfoGroup, "stats:", "", "", "", 0, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Info_rdrstats, "rdr stats", "", "", "", 0, "enable render stats display"},
    {kSD_ControlType_Word, kSD_Control_Info_graphfps,  "graph fps", "", "", "", 0, "enable fps graph"},
    {kSD_ControlType_Word, kSD_Control_Info_rdrinfo,  "rdr info", "", "", "", 0, "enable render info display"},
    {kSD_ControlType_Word, kSD_Control_Info_showspecs,  "specs", "", "", "", 0, "enable spec display"},

    {kSD_ControlType_GroupWord, kSD_Control_Viz_ColorizeGroup, "colorize:", "", "", "", 1, "Visualize as color. Click to disable selection."},
    {kSD_ControlType_Word, kSD_Control_Viz_uv0, "uv 0", "", "", "", 0, "visualize uv set 0"},
    {kSD_ControlType_Word, kSD_Control_Viz_uv1,  "uv 1", "", "", "", 0, "visualize uv set 1"},
    {kSD_ControlType_Word, kSD_Control_Viz_Tangent,  "tangent", "", "", "", 0, "visualize tangent as color [-1,1]->[0,1]"},
    {kSD_ControlType_Word, kSD_Control_Viz_Normal,  "normal", "", "", "", 0, "visualize normal as color [-1,1]->[0,1]"},
    {kSD_ControlType_Word, kSD_Control_Viz_Bitangent,  "bitangent", "", "", "", 0, "visualize bitangent as color [-1,1]->[0,1]"},
    {kSD_ControlType_Word, kSD_Control_Viz_Blendmode,  "blendmode", "", "", "", 0, "visualize blendmode as colors"},

    {kSD_ControlType_GroupWord, kSD_Control_Viz_VectorGroup, "vector:", "", "", "", 1, "Show quantiles as directed arrows. Click to clear/set all."},
    {kSD_ControlType_Word, kSD_Control_Viz_normal_vectors, "normal", "", "", "", 0, "Show normal vectors"},
    {kSD_ControlType_Word, kSD_Control_Viz_tangent_vectors,  "tangent", "", "", "", 0, "Show tangent vectors"},
    {kSD_ControlType_Word, kSD_Control_Viz_bitangent_vectors,  "bitangent", "", "", "", 0, "Show bitangent vectors"},
    {kSD_ControlType_Word, kSD_Control_Viz_seams,  "seams", "", "", "", 0, "Show seams"},
    {kSD_ControlType_Word, kSD_Control_Viz_lightdirs,  "light dirs", "", "", "", 0, "Show vector from vertex to light"},

    // FX
    {kSD_ControlType_GroupWord, kSD_Control_ParticleGroup, "particles:", "", "", "", 1, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Particles_Systems, "systems", "", "", "", 1, "enable particle systems"},
    {kSD_ControlType_Word, kSD_Control_Particles_Render, "render", "", "", "", 1, "Render particle systems"},
    {kSD_ControlType_Word, kSD_Control_Particles_Texture, "texture", "", "", "", 1, "texture particle systems"},
    {kSD_ControlType_Word, kSD_Control_Particles_Update, "update", "", "", "", 1, "Update particle systems"},
    {kSD_ControlType_Word, kSD_Control_Particles_Write, "write", "", "", "", 1, "Write particle vertex changes"},
    {kSD_ControlType_Word, kSD_Control_Particles_FxGeo, "fx geo", "", "", "", 1, "Show FX Geometry"},

    {kSD_ControlType_GroupWord, kSD_Control_FxInfoGroup, "FX Info:", "", "", "", 0, "Clicking here will toggle on or off all items in the row"},
    {kSD_ControlType_Word, kSD_Control_Info_FxBasic, "basic", "", "", "", 0, "show basic FX debug information"},
    {kSD_ControlType_Word, kSD_Control_Info_FxDetail,  "detail", "", "", "", 0, "show detailed FX debug information"},
    {kSD_ControlType_Word, kSD_Control_Info_FxVerbose,  "verbose", "", "", "", 0, "show verbose FX debug information"},
    {kSD_ControlType_Word, kSD_Control_Info_FxSound,  "sound", "", "", "", 0, "show FX sound info"},

    // panel selector control words
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Material, "Material", "", "", "", 1, "Select the material/lighting control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Texture, "Texture", "", "", "", 0, "Select the texture control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Visualize, "Visualize", "", "", "", 0, "Select the visualization control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Render, "Render", "", "", "", 0, "Select the render control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_FX, "FX", "", "", "", 0, "Select the FX/Particles control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Info, "Info", "", "", "", 0, "Select the information control panel."},
    {kSD_ControlType_PanelSelectWord, kSD_Control_Panel_Engine, "Engine", "", "", "", 0, "Select the engineering control panel."},

};

static int s_shader_debug_control_bar[] = {kSD_Control_None, kSD_Control_HudClose, kSD_Control_None, kSD_Control_Bottom, kSD_Control_None, kSD_Control_UI, kSD_Control_None, kSD_Control_Pause, kSD_Control_None, kSD_Control_Wireframe, kSD_Control_None, kSD_Control_texoff, kSD_Control_None, kSD_Control_UseDebugShaders };
static int s_panel_select_bar[] = {kSD_Control_Panel_Material, kSD_Control_Panel_Texture, kSD_Control_Panel_Visualize, kSD_Control_Panel_Render, kSD_Control_Panel_FX, kSD_Control_Panel_Info, kSD_Control_Panel_Engine };

static int s_menu_texture[] = {kSD_Control_Tex_Base1, kSD_Control_Tex_Dual1, kSD_Control_Tex_Bump1, kSD_Control_Tex_Multiply1, kSD_Control_Tex_Mask, kSD_Control_Tex_Cubemap };

static int s_control_panel_materials[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_MaterialGroup, kSD_Control_Mat_VertexColor, kSD_Control_Mat_Base, kSD_Control_Mat_Dual, kSD_Control_Mat_Multiply, kSD_Control_AddGlow, kSD_Control_BlendAlpha, kSD_Control_LodAlpha},
    {kSD_Control_LightingGroup, kSD_Control_Ambient, kSD_Control_Diffuse, kSD_Control_Specular, kSD_Control_LightColor, kSD_Control_Gloss, kSD_Control_Bump, kSD_Control_Shadowed},
    {kSD_Control_FeatureGroup, kSD_Control_Fog, kSD_Control_Reflection, kSD_Control_SSAO, kSD_Control_SubMaterial1, kSD_Control_SubMaterial2, kSD_Control_Building,},
};

static int s_control_panel_tex[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_TextureGroup, kSD_Control_Tex_Base1, kSD_Control_Tex_Dual1, kSD_Control_Tex_Bump1, kSD_Control_Tex_Multiply1, kSD_Control_Tex_Mask, kSD_Control_Tex_Cubemap },
    {kSD_Control_None,    kSD_Control_Tex_Base2, kSD_Control_Tex_Dual2, kSD_Control_Tex_Bump2, kSD_Control_Tex_Multiply2,  kSD_Control_Tex_AddGlow, kSD_Control_Tex_AddGlowMask},
    {kSD_Control_ChannelGroup, kSD_Control_Tex_RGBA, kSD_Control_Tex_RGB, kSD_Control_Tex_ALPHA },
};

static int s_control_panel_render[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_RenderGroup, kSD_Control_Render_Sky, kSD_Control_Render_Environment, kSD_Control_Render_Entity, kSD_Control_Render_2D, kSD_Control_Render_Lines, },
    {kSD_Control_PassGroup, kSD_Control_Render_OpaquePass, kSD_Control_Render_AlphaPass, kSD_Control_Render_UnderwaterAlphaPass,  },
    {kSD_Control_MiscGroup, kSD_Control_Render_ForceFallback, kSD_Control_None , kSD_Control_Render_ForceLOD0, kSD_Control_Render_ForceLOD1, kSD_Control_Render_ForceLOD2, kSD_Control_Render_ForceLOD3 },
};

static int s_control_panel_visualize[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_Viz_ColorizeGroup, kSD_Control_Viz_uv0, kSD_Control_Viz_uv1, kSD_Control_Viz_Tangent, kSD_Control_Viz_Normal, kSD_Control_Viz_Bitangent, kSD_Control_Viz_Blendmode },
    {kSD_Control_Viz_VectorGroup, kSD_Control_Viz_normal_vectors, kSD_Control_Viz_tangent_vectors, kSD_Control_Viz_bitangent_vectors, kSD_Control_Viz_lightdirs, kSD_Control_Viz_seams },
};

static int s_control_panel_info[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_InfoGroup, kSD_Control_Info_rdrstats, kSD_Control_Info_graphfps, kSD_Control_Info_rdrinfo, kSD_Control_Info_showspecs },
//    {kSD_Control_FxInfoGroup, kSD_Control_Info_FxBasic, kSD_Control_Info_FxDetail, kSD_Control_Info_FxVerbose, kSD_Control_Info_FxSound },
};

static int s_control_panel_fx[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_ParticleGroup, kSD_Control_Particles_Systems, kSD_Control_Particles_Render, kSD_Control_Particles_Texture, kSD_Control_Particles_Update, kSD_Control_Particles_Write, kSD_Control_Particles_FxGeo },
    {kSD_Control_FxInfoGroup, kSD_Control_Info_FxBasic, kSD_Control_Info_FxDetail, kSD_Control_Info_FxVerbose, kSD_Control_Info_FxSound },
};

static int s_control_panel_engine[kSD_GridRows][kSD_GridCols] =
{
    {kSD_Control_MiscGroup,  },
    {kSD_Control_MiscGroup,  },
    {kSD_Control_MiscGroup,  },
};


// starting panel
static int* s_current_panel = (int*)&s_control_panel_materials;


static void set_flags( unsigned int* var, unsigned int bit_flags, bool value )
{
    if (value)
    {
        *var |= bit_flags;
    }
    else
    {
        *var &= ~bit_flags;
    }
}

// update relevant control states with current engine flags on entry
// since many of the flags controlled by the hud can also be controlled
// 'externally' by the tweak menu or slash commands
// @todo currently if you use tweak/slash commands at the same time as the hud
// is up you can get state out of sync
static void sync_controls_to_flags(void)
{
//    s_controls[kSD_Control_UI].value = (game_state.perf & GFXDEBUG_PERF_SKIP_2D) != 0;
    s_controls[kSD_Control_Wireframe].value = game_state.wireframe != 0;
    s_controls[kSD_Control_texoff].value = game_state.texoff != 0;
    s_controls[kSD_Control_Render_ForceFallback].value = game_state.force_fallback_material != 0;

    g_restore_gfx_features = (rdr_caps.features & GFXF_AMBIENT);
    s_controls[kSD_Control_SSAO].value = (rdr_caps.features & GFXF_AMBIENT) != 0;

}

// keep flags used by hud in sync with the ui controls
static void sync_flags_to_controls(void)
{
    s_controls[kSD_Control_HudClose].value = true;

    // reset certain controls/state based on current panel
    // texture display mode enable should turn off when we leave that panel
    if (!s_controls[kSD_Control_Panel_Texture].value)
    {
        s_controls[kSD_Control_TextureGroup].value = false;
    }

    // @todo maybe it makes more sense to use the 'disable2D' flag for this purpose?
    // does that also disable font rendering?
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_2D, s_controls[kSD_Control_UI].value );

    // we have a quick press toggle to disable debug shaders for comparisons w/o dismissing HUD
    g_mt_shader_debug_state.flag_use_debug_shaders = s_controls[kSD_Control_UseDebugShaders].value;

    // SSAO disabled?
    set_flags( (unsigned*)&rdr_caps.features, GFXF_AMBIENT, s_controls[kSD_Control_SSAO].value );

    // kSD_Control_MaterialGroup
    g_mt_shader_debug_state.use_vertex_color = s_controls[kSD_Control_Mat_VertexColor].value;
    g_mt_shader_debug_state.use_blend_base = s_controls[kSD_Control_Mat_Base].value;
    g_mt_shader_debug_state.use_blend_dual = s_controls[kSD_Control_Mat_Dual].value;
    g_mt_shader_debug_state.use_blend_multiply = s_controls[kSD_Control_Mat_Multiply].value;
    g_mt_shader_debug_state.use_blend_addglow = s_controls[kSD_Control_AddGlow].value;
    g_mt_shader_debug_state.use_blend_alpha = s_controls[kSD_Control_BlendAlpha].value;
    g_mt_shader_debug_state.use_lod_alpha = s_controls[kSD_Control_LodAlpha].value;

    // kSD_Control_LightingGroup
    g_mt_shader_debug_state.use_ambient = s_controls[kSD_Control_Ambient].value;
    g_mt_shader_debug_state.use_diffuse = s_controls[kSD_Control_Diffuse].value;
    g_mt_shader_debug_state.use_specular = s_controls[kSD_Control_Specular].value;
    g_mt_shader_debug_state.use_light_color = s_controls[kSD_Control_LightColor].value;
    g_mt_shader_debug_state.use_gloss_factor = s_controls[kSD_Control_Gloss].value;
    g_mt_shader_debug_state.use_bump = s_controls[kSD_Control_Bump].value;
    g_mt_shader_debug_state.use_shadowed = s_controls[kSD_Control_Shadowed].value;

    // kSD_Control_FeatureGroup
//    g_mt_shader_debug_state.use_material = s_controls[kSD_Control_Blend].value;
//    g_mt_shader_debug_state.use_faux_reflection = s_controls[kSD_Control_FauxReflection].value;
    g_mt_shader_debug_state.use_fog = s_controls[kSD_Control_Fog].value;
    g_mt_shader_debug_state.use_reflection = s_controls[kSD_Control_Reflection].value;
    g_mt_shader_debug_state.use_sub_material_1 = s_controls[kSD_Control_SubMaterial1].value;
    g_mt_shader_debug_state.use_sub_material_2 = s_controls[kSD_Control_SubMaterial2].value;
    g_mt_shader_debug_state.use_material_building = s_controls[kSD_Control_Building].value;

    // kSD_Control_RenderGroup
// see kSD_Control_UI    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_2D, !s_controls[kSD_Control_Render_2D].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_SKY, !s_controls[kSD_Control_Render_Sky].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_GROUPDEF_TRAVERSAL, !s_controls[kSD_Control_Render_Environment].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_GFXNODE_TREE_TRAVERSAL, !s_controls[kSD_Control_Render_Entity].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_LINES, !s_controls[kSD_Control_Render_Lines].value );

    game_state.texoff = s_controls[kSD_Control_texoff].value ? 1 : 0;

    // kSD_Control_PassGroup
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_OPAQUE_PASS, !s_controls[kSD_Control_Render_OpaquePass].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_ALPHA_PASS, !s_controls[kSD_Control_Render_AlphaPass].value );
    set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_UNDERWATER_ALPHA_PASS, !s_controls[kSD_Control_Render_UnderwaterAlphaPass].value );

    // misc
    game_state.force_fallback_material = s_controls[kSD_Control_Render_ForceFallback].value ? 1 : 0;

    // force lod
    if (s_controls[kSD_Control_Render_ForceLOD0].value)
    {
        g_client_debug_state.lod_model_override = 1;    // desired lod + 1
    }
    else if (s_controls[kSD_Control_Render_ForceLOD1].value)
    {
        g_client_debug_state.lod_model_override = 2;
    }
    else if (s_controls[kSD_Control_Render_ForceLOD2].value)
    {
        g_client_debug_state.lod_model_override = 3;
    }
    else if (s_controls[kSD_Control_Render_ForceLOD3].value)
    {
        g_client_debug_state.lod_model_override = 4;
    }
    else
    {
        g_client_debug_state.lod_model_override = 0;    // no lod override
    }



    // kSD_Control_TextureGroup
    g_mt_shader_debug_state.mode_texture_display = g_selected_tex >= 0 && s_controls[kSD_Control_Panel_Texture].value;
    g_mt_shader_debug_state.selected_texture = g_selected_tex;
    g_mt_shader_debug_state.tex_display_rgba = s_controls[kSD_Control_Tex_RGBA].value;;
    g_mt_shader_debug_state.tex_display_rgb = s_controls[kSD_Control_Tex_RGB].value;;
    g_mt_shader_debug_state.tex_display_alpha = s_controls[kSD_Control_Tex_ALPHA].value;

    // info
    game_state.renderinfo = s_controls[kSD_Control_Info_rdrinfo].value ? 1 : 0;
    game_state.graphfps = s_controls[kSD_Control_Info_graphfps].value ? 7 : 0;
    game_state.showrdrstats = s_controls[kSD_Control_Info_rdrstats].value ? 3 : 0;
    game_state.showSpecs = s_controls[kSD_Control_Info_showspecs].value ? 1 : 0;

    // Visualize
    set_flags( &game_state.gfxdebug, GFXDEBUG_NORMALS, s_controls[kSD_Control_Viz_normal_vectors].value );
    set_flags( &game_state.gfxdebug, GFXDEBUG_TANGENTS, s_controls[kSD_Control_Viz_tangent_vectors].value );
    set_flags( &game_state.gfxdebug, GFXDEBUG_BINORMALS, s_controls[kSD_Control_Viz_bitangent_vectors].value );
    set_flags( &game_state.gfxdebug, GFXDEBUG_NORMALS, s_controls[kSD_Control_Viz_normal_vectors].value );
    set_flags( &game_state.gfxdebug, GFXDEBUG_SEAMS, s_controls[kSD_Control_Viz_seams].value );
    set_flags( &game_state.gfxdebug, GFXDEBUG_LIGHTDIRS, s_controls[kSD_Control_Viz_lightdirs].value );

    if (s_controls[kSD_Control_Viz_normal_vectors].value || s_controls[kSD_Control_Viz_tangent_vectors].value ||
        s_controls[kSD_Control_Viz_bitangent_vectors].value || s_controls[kSD_Control_Viz_seams].value ||
        s_controls[kSD_Control_Viz_lightdirs].value)
    {
        cmdParse("novbos 1");
    }
    else
    {
        cmdParse("novbos 0");
    }

    if (g_selected_visualization >= 0 && s_controls[kSD_Control_Panel_Visualize].value)
    {
        g_mt_shader_debug_state.mode_visualize = 1;
        g_mt_shader_debug_state.selected_visualization = g_selected_visualization;
        g_mt_shader_debug_state.visualize_tex_id = g_uv_test_tex_id;
    }
    else
    {
        g_mt_shader_debug_state.mode_visualize = 0;
        g_mt_shader_debug_state.selected_visualization = 0;
        g_mt_shader_debug_state.visualize_tex_id = 0;
    }

    // FX/Particles
    game_state.noparticles = !s_controls[kSD_Control_Particles_Systems].value ? 1 : 0;
    game_state.whiteParticles = !s_controls[kSD_Control_Particles_Texture].value ? 1 : 0;
    set_flags( &game_state.fxdebug, FX_DISABLE_PART_RENDERING, !s_controls[kSD_Control_Particles_Render].value );
    set_flags( &game_state.fxdebug, FX_HIDE_FX_GEO, !s_controls[kSD_Control_Particles_FxGeo].value );
    set_flags( &game_state.fxdebug, FX_DISABLE_PART_CALCULATION, !s_controls[kSD_Control_Particles_Update].value );
    set_flags( &game_state.fxdebug, FX_DISABLE_PART_ARRAY_WRITE, !s_controls[kSD_Control_Particles_Write].value );

    set_flags( &game_state.fxdebug, FX_DEBUG_BASIC, s_controls[kSD_Control_Info_FxBasic].value );
    set_flags( &game_state.fxdebug, FX_SHOW_DETAILS, s_controls[kSD_Control_Info_FxDetail].value );
    set_flags( &game_state.fxdebug, FX_PRINT_VERBOSE, s_controls[kSD_Control_Info_FxVerbose].value );
    set_flags( &game_state.fxdebug, FX_SHOW_SOUND_INFO, s_controls[kSD_Control_Info_FxSound].value );
}

// set state of all controls in a row to value. (does not include end_id)
static void set_group_row_state( int first_id, int end_id, bool value )
{
    int i;
    for (i=first_id; i < end_id; ++i)
    {
        s_controls[i].value = value;
    }
}

// handle a control click
static _process_control( int ctrl_id )
{
    SDControl*ctrl = &s_controls[ctrl_id];

    // some controls are special purpose. Handle them here.
    switch (ctrl_id)
    {
        case kSD_Control_HudClose:
            gfxDevHudToggle();        // close the HUD
            break;

        // keep these two in sync, one is the inverse of the other
        case kSD_Control_UI:
            ctrl->value = !ctrl->value;    // update toggle state
            s_controls[kSD_Control_Render_2D].value = !ctrl->value;
            break;
        case kSD_Control_Render_2D:
            ctrl->value = !ctrl->value;    // update toggle state
            s_controls[kSD_Control_UI].value = !ctrl->value;
            break;

        case kSD_Control_Wireframe:
//            ctrl->value = (ctrl->value + 1)%3; // actually 3 different wireframe states, not sure if any real distinction for our purposes
            ctrl->value = !ctrl->value;    // update toggle state
            game_state.wireframe = ctrl->value;
            break;

        case kSD_Control_Pause:
            ctrl->value = !ctrl->value;    // update toggle state
            if (ctrl->value)
            {
                cmdParse("timestepscale .000001");
            }
            else
            {
                cmdParse("timestepscale 0");
            }
            break;

        case kSD_Control_Bottom:
            ctrl->value = !ctrl->value;    // update toggle state
            g_hud_at_bottom = ctrl->value;        // position the HUD
            break;

        // clicking on the group item toggle the whole group to on of off
        case kSD_Control_MaterialGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_MaterialGroup+1,kSD_Control_LightingGroup,ctrl->value);
            break;

        case kSD_Control_LightingGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_LightingGroup+1,kSD_Control_FeatureGroup,ctrl->value);
            break;

        case kSD_Control_FeatureGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_FeatureGroup+1,kSD_Control_FeatureGroup_END,ctrl->value);
            break;

        //**
        // Render Panel

        case kSD_Control_RenderGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_RenderGroup+1,kSD_Control_RenderGroup_End,ctrl->value);
            break;

        case kSD_Control_PassGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_PassGroup+1,kSD_Control_PassGroup_End,ctrl->value);
            break;

        case kSD_Control_Render_ForceLOD0:    
        case kSD_Control_Render_ForceLOD1:    
        case kSD_Control_Render_ForceLOD2:    
        case kSD_Control_Render_ForceLOD3:
            if (s_controls[ctrl_id].value)    //already enabled, click to disable
            {
                s_controls[ctrl_id].value = false;
            }
            else
            {
                set_group_row_state(kSD_Control_Render_ForceLOD0,kSD_Control_Render_ForceLOD3+1,false);    // clear all
                s_controls[ctrl_id].value = true;    // set new 'radio' option
            }
            break;


        //**
        // Texture Panel
        case kSD_Control_TextureGroup:    // acts as a clear/disable
            set_group_row_state(kSD_Control_Tex_Base1,kSD_Control_TextureGroup_End,false); // clear all
            g_selected_tex = -1;
            break;

        case kSD_Control_Tex_Base1:
        case kSD_Control_Tex_Dual1:
        case kSD_Control_Tex_Bump1:
        case kSD_Control_Tex_Multiply1:
        case kSD_Control_Tex_Mask:
        case kSD_Control_Tex_Base2:
        case kSD_Control_Tex_Dual2:
        case kSD_Control_Tex_Bump2:
        case kSD_Control_Tex_Multiply2:
        case kSD_Control_Tex_AddGlow:
        case kSD_Control_Tex_AddGlowMask:
            // if was already set then clicking should disable
            if (s_controls[ctrl_id].value)
            {
                set_group_row_state(kSD_Control_Tex_Base1,kSD_Control_TextureGroup_End,false); // clear all
                g_selected_tex = -1;
            }
            else
            {
                set_group_row_state(kSD_Control_Tex_Base1,kSD_Control_TextureGroup_End,false); // clear all
                ctrl->value = true;
                g_selected_tex = ctrl_id - kSD_Control_Tex_Base1;
            }
            break;

        case kSD_Control_Tex_RGBA:    
        case kSD_Control_Tex_RGB:    
        case kSD_Control_Tex_ALPHA:    
            set_group_row_state(kSD_Control_Tex_RGBA,kSD_Control_Tex_ALPHA+1,false);    // clear all
            s_controls[ctrl_id].value = true;    // set 'radio' option
            break;


        case kSD_Control_Viz_ColorizeGroup:        // acts as a disable
            set_group_row_state(kSD_Control_Viz_uv0,kSD_Control_Viz_Blendmode+1,false);    // clear all
            g_selected_visualization = -1;
            break;

        case kSD_Control_Viz_uv0:    
        case kSD_Control_Viz_uv1:    
        case kSD_Control_Viz_Tangent:    
        case kSD_Control_Viz_Normal:
        case kSD_Control_Viz_Bitangent:
        case kSD_Control_Viz_Blendmode:
            // if was already set then clicking should disable
            if (s_controls[ctrl_id].value)
            {
                set_group_row_state(kSD_Control_Viz_uv0,kSD_Control_Viz_Blendmode+1,false);    // clear all
                g_selected_visualization = -1;
            }
            else
            {
                set_group_row_state(kSD_Control_Viz_uv0,kSD_Control_Viz_Blendmode+1,false);    // clear all
                s_controls[ctrl_id].value = true;    // set 'radio' option
                g_selected_visualization = ctrl_id - kSD_Control_Viz_uv0;
            }
            break;

        case kSD_Control_Viz_VectorGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_Viz_normal_vectors,kSD_Control_Viz_bitangent_vectors+1,ctrl->value);
            break;

        //**
        // Info
        case kSD_Control_InfoGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_InfoGroup+1,kSD_Control_InfoGroup_End,ctrl->value);
            break;

        //**
        // FX
        case kSD_Control_ParticleGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_ParticleGroup+1,kSD_Control_FxInfoGroup,ctrl->value);
            break;
            
        case kSD_Control_FxInfoGroup:
            ctrl->value = !ctrl->value;    // update toggle state
            set_group_row_state(kSD_Control_FxInfoGroup+1,kSD_Control_Info_FxSound+1,ctrl->value);
            break;
            
        //**
        // PANELS
        case kSD_Control_Panel_Material:    // panel selectors, only one at a time
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_materials;
            break;

        case kSD_Control_Panel_Render:    // panel selectors, only one at a time
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_render;
            break;

        case kSD_Control_Panel_Texture:
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_tex;
            break;

        case kSD_Control_Panel_Visualize:
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_visualize;
            break;

        case kSD_Control_Panel_FX:
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_fx;
            break;

        case kSD_Control_Panel_Info:
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_info;
            break;

        case kSD_Control_Panel_Engine:
            set_group_row_state(kSD_Control_Panel_Material,kSD_Control_Number,false);    // clear all
            s_controls[ctrl_id].value = true;    // set new panel
            s_current_panel = (int*)&s_control_panel_engine;
            break;

        case kSD_Control_TextureMenu:
            g_popup_menu_id = kSD_Control_TextureMenu;
            break;

        default:
            // most of the controls are just toggle switches for features
            // so we toggle the flag and then the 'sync' method will update
            // the appropriate engine variable from the control state
            ctrl->value = !ctrl->value;    // update toggle state
            break;
    }
}

static float calc_hud_height()
{
    float y = 0.0f;
    y += kBarHeight_Panel_Select + kLineHeight_Panel_Select + 3;
    y += kDH_Control_Panel_Height + kLineHeight_Panel_Select + 3;
    y += kBarHeight + 4;
    y += kBarHeight_Status;
    return y;
}

static void get_hud_location( float* pX, float* pY )
{
    float sx = 20.0f;
    float sy;
    // default hud position is either at top or the
    // bottom
    if (g_hud_at_bottom)
    {
        int screen_width, screen_height;
        windowClientSize(&screen_width, &screen_height);
        sy = screen_height - (calc_hud_height() + kDH_HUD_Y_MARGIN);
    }
    else
    {
        sy = kDH_HUD_Y_MARGIN;
    }
    *pX = sx;
    *pY = sy;
}

static void get_panel_select_location( float* pX, float* pY )
{
    get_hud_location( pX, pY );
}

static void get_panel_location( float* pX, float* pY )
{
    get_panel_select_location( pX, pY );
    *pY += kBarHeight_Panel_Select + kLineHeight_Panel_Select + 3;
}

static void get_bar_location( float* pX, float* pY )
{
    get_panel_location( pX, pY );
    *pY += kDH_Control_Panel_Height + kLineHeight_Panel_Select + 3;
}

static void get_status_location( float* pX, float* pY )
{
    get_bar_location( pX, pY );
    *pY += kBarHeight + 4;
}

static void render_horizontal_span( float x, float y, float z, float dx, float dy, unsigned int color )
{
    //separator line
    AtlasTex *line = white_tex_atlas;
    display_sprite_immediate_ex(line,NULL, x, y, z, dx/line->width, dy/line->height,
                                    color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
}

static void render_horizontal_tab( float x, float y, float z, float dx, float dy, unsigned int color, bool flip_vertical )
{
    AtlasTex * end = atlasLoadTexture("Tab_end.tga");
    AtlasTex * mid = atlasLoadTexture("Tab_mid.tga");
    float x_tab, w_tab;

    float margin_x = 2.0f;
    float margin_y = 2.0f;
    int additive = 0;

    x_tab = x+margin_x;
    w_tab = dx - margin_x;
    // tab curved 'book ends'
    display_sprite_immediate_ex(end,NULL, x_tab, y+margin_y, z,
        1.0f, (dy-margin_y)/end->height,
        color, color, color, color, 0, 0, 1, 1, 0, additive, clipperGetCurrent(), 0 );
    // flip other tab end horizontally
    display_sprite_immediate_ex(end,NULL, x_tab + w_tab - end->width, y+margin_y, z,
        1.0f, (dy-margin_y)/end->height,
        color, color, color, color, 1, 0, 0, 1, 0, additive, clipperGetCurrent(), 0 );
    // middle rectangular part of tab
    display_sprite_immediate_ex(mid,NULL, x_tab + end->width, y+margin_y, z,
        (w_tab - 2.0f*end->width)/mid->width, (dy-margin_y)/end->height,
        color, color, color, color, 0, 0, 1, 1, 0, additive, clipperGetCurrent(), 0 );
}

// render one of the simple text word toggle controls
static void render_word_control(SDControl*ctrl, float x, float y, float z, float dx, float dy )
{
    if (!ctrl->name || *ctrl->name == 0)
        return;    // don't draw the control

    // draw the background cell
    if (ctrl->type == kSD_ControlType_PanelSelectWord)    // use a curved tab background for panel select words
    {
        SDColorSet* colors = &s_controltype_colors[ctrl->type];
        unsigned int color = ctrl->value ? colors->color_bg_enabled : colors->color_bg_disabled;
        if (ctrl->rollover)
        {
            color = colors->color_roll;
        }
        render_horizontal_tab(x, y, z, dx, dy, color,false);
    }
//    else if (ctrl->type == kSD_Control_TextureMenu)    // use a curved tab background for panel select words
//    {
//    }
    else
    {
        SDColorSet* colors = &s_controltype_colors[ctrl->type];
        unsigned int color = ctrl->value ? colors->color_bg_enabled : colors->color_bg_disabled;
        AtlasTex *line = white_tex_atlas;
        float margin_x = 2.0f;
        float margin_y = 2.0f;
        int additive = 0;
        if (ctrl->rollover)
        {
            color = colors->color_roll;
            // additive = 1;
        }
        display_sprite_immediate_ex(line,NULL, x+margin_x, y+margin_y, z,
            (dx-margin_x)/line->width, (dy-margin_y)/line->height,
            color, color, color, color, 0, 0, 1, 1, 0, additive, clipperGetCurrent(), 0 );
    }

    // now render the text on top
    {
        float txt_width = fontStringWidth( ctrl->name );
        float x_offset = (dx-txt_width)/2;
        float y_offset = (dy-kFontHeight)/2;
        SDColorSet* colors = &s_controltype_colors[ctrl->type];
        unsigned int color = ctrl->value ? colors->color_text_enabled : colors->color_text_disabled;;
        fontColor( color );
        fontText(x+x_offset, y+y_offset, ctrl->name);
    }

    // dim it if HUD is not really active
    if (g_dev_hud_state != kDH_Active)
    {
        unsigned int color = 0x00000060;
        AtlasTex *line = white_tex_atlas;
        float margin_x = 2.0f;
        float margin_y = 2.0f;
        display_sprite_immediate_ex(line,NULL, x+margin_x, y+margin_y, z+100,
            (dx-margin_x)/line->width, (dy-margin_y)/line->height,
            color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
    }

}

static void render_control_bar(void)
{
    float sx, sy;
    float x,y;
    float z = 500.0f;
    float dx = kBarCellWidth;
    int j;

    get_bar_location( &sx, &sy );

    x = sx; y = sy;

    // background bar
    render_horizontal_span(x,y,z,kBarWidth,kBarHeight,0x404040A0);
#if 0
    {
        AtlasTex * header_r = atlasLoadTexture( "header_bar_002_r.tga" );
        AtlasTex * header_m = atlasLoadTexture( "header_bar_002_mid.tga" );
        AtlasTex * header_l = atlasLoadTexture( "header_bar_002_l.tga" );
        unsigned int color = 0x80808080;

        display_sprite_immediate_ex(header_l,NULL, x, y, z, 1.0, kBarHeight/(float)header_m->height, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
        display_sprite_immediate_ex(header_m,NULL, x+header_l->width, y, z, (kBarWidth-header_l->width-header_r->width)/(float)header_m->width, kBarHeight/(float)header_m->height, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
        display_sprite_immediate_ex(header_r,NULL, x -header_r->width + kBarWidth -, y, z, 1.0, kBarHeight/(float)header_m->height, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
    }
#endif
    // controls
    x += kBarMarginX;

    for (j=0; j < sizeof(s_shader_debug_control_bar)/sizeof(int) ; ++j)
    {
        int ctrl_id = s_shader_debug_control_bar[j];
        if (ctrl_id != kSD_Control_None)
        {
            SDControl*ctrl = &s_controls[ctrl_id];
            char* texName = ctrl->name_tex;
            if (ctrl->name_tex_disabled && *ctrl->name_tex_disabled != 0)
                texName = ctrl->value ? ctrl->name_tex : ctrl->name_tex_disabled;

            if (texName)
            {
                AtlasTex * button = atlasLoadTexture( texName );
                float sc = 1.0f;
                float yo = y;
                unsigned int color = 0xFFFFFFFF;
                int cell_height = kBarHeight-2*kBarMarginY;

                if (button->height > cell_height)
                {
                    sc = (float)(cell_height)/(float)button->height;
                    yo = y + (kBarHeight - button->height*sc)/2;
                }

                color = ctrl->value ? 0xFFFFFFFF : 0xFFFF00FF;
                if ( ctrl->rollover )
                    color = 0x00FF00FF; // (color & 0xFFFF00FF) | ((color^0xFF00)&0xFFFF00FF);

                display_sprite_immediate_ex(button,NULL, x, yo, z, sc, sc, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
            }
        }
        x += dx;
    }
}

//render a single row/bar of word controls
void render_word_control_bar(float x, float y, float z, float dx, float dy, int num_controls, int* control_ids )
{
    int j;

    //render the controls in the bar
    for (j=0; j < num_controls; ++j)
    {
        int ctrl_id = control_ids[j];
        if (ctrl_id != kSD_Control_None)
        {
            SDControl*ctrl = &s_controls[ctrl_id];
            render_word_control( ctrl, x, y, z, dx, dy );
        }
        x += dx;
    }
}

//render a popup menu of choices
void render_popup_list(float x, float y, float z, float dx, float dy, int num_controls, int* control_ids )
{
    int j;
    
    // menu background
    x += dx/4;
    y += 3;

    render_horizontal_span( x,y,z, dx, dy*num_controls, 0xFFFFFFFF );

    //render the controls in the bar
    for (j=0; j < num_controls; ++j)
    {
        int ctrl_id = control_ids[j];
        if (ctrl_id != kSD_Control_None)
        {
            SDControl*ctrl = &s_controls[ctrl_id];
            render_word_control( ctrl, x, y, z, dx, dy );
        }
        y += dy; //!g_hud_at_bottom ? dy:-dy;
    }
}

void render_panel_select_bar(void)
{
    float dx = kBarCellWidth_Panel_Select;
    float dy = kSD_CellHeight;

    float sx,sy,x,y,z = 500.0f;

    get_panel_select_location(&sx,&sy);
    x = sx;
    y = sy;

    //render the controls in the panel
    fontDefaults();
    fontSet(0);
    render_word_control_bar(x,y,z,dx,dy,sizeof(s_panel_select_bar)/sizeof(int),s_panel_select_bar);

    //separator line
    render_horizontal_span( x, sy + kSD_CellHeight, z, kLineWidth_Panel_Select, kLineHeight_Panel_Select, 0xA0A0A0A0 );
}


void render_control_panel(void)
{
    int i;
    float dx = kSD_CellWidth;
    float dy = kSD_CellHeight;

    float sx,sy,x,y,z = 500.0f;

    get_panel_location(&sx,&sy);
    x = sx;
    y = sy;

    //render the controls in the panel
    fontDefaults();
    fontSet(0);
    for (i=0; i < kSD_GridRows; ++i)
    {
        render_word_control_bar(x,y,z, dx,dy,kSD_GridCols,s_current_panel+i*kSD_GridCols);
        y += dy;
    }

    //separator line
    render_horizontal_span( x, sy + kDH_Control_Panel_Height + 1, z, kLineWidth_Panel_Select, kLineHeight_Panel_Select, 0xA0A0A0A0 );
}

static void render_status_bar(void)
{
    bool bShowingHelp = false;
    float sx, sy, x, y, z=500.0f;

    get_status_location(&sx,&sy);
    x = sx;
    y = sy;

    fontDefaults();
    fontSet(0);

    // make a big 'loading' message so user knows why client is 'frozen'
    if (!g_mt_shader_debug_state.flag_loaded)
    {
        AtlasTex *line = white_tex_atlas;
        unsigned int color = 0x101010A0;
        char* msg = "Debug shaders are loading. Please wait, this could take a minute...";
        float txt_width;
        int screen_width, screen_height;
        int xo, yo;
        int alertHeight = 20;

        windowClientSize(&screen_width, &screen_height);
        fontScale(1.5f, 1.5f);
        txt_width = fontStringWidth(msg);
        xo = (screen_width > txt_width) ? (screen_width - txt_width)/2 : 10;
        yo = (screen_height - alertHeight)/2;

        display_sprite_immediate_ex(line,NULL, xo, yo, z, (txt_width + 10)/(float)line->width, (alertHeight)/(float)line->height, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
        fontColor( 0xFFFF00 );
        fontText(xo+5, yo+3, msg );
        bShowingHelp = true;
    }

    {
        // display rollover help, if any
        int i;
        for (i=0; i<sizeof(s_controls)/sizeof(SDControl); ++i)
        {
            SDControl*ctrl = &s_controls[i];
            onlydevassert( ctrl->id == i );        // control array order must currently match order in the enum
            if (ctrl->rollover && ctrl->help)
            {
                AtlasTex *line = white_tex_atlas;
                unsigned int color = 0x101010A0;
                float txt_width;

                fontScale(1.0f, 1.0f);
                txt_width = fontStringWidth(ctrl->help);
                display_sprite_immediate_ex(line,NULL, x, y, z, (txt_width + 10)/(float)line->width, (kBarHeight_Status)/(float)line->height, color, color, color, color, 0, 0, 1, 1, 0, 0, clipperGetCurrent(), 0 );
                fontColor( 0xFFFFFF );
                fontText(x+5, y+1, ctrl->help );
                bShowingHelp = true;
            }
        }
    }

    if (!bShowingHelp)            // show color under the cursor
    {
        char text[256];
        U8 color[4];
        Vec3 v;
        HDC hdc;
        hdc = GetDC(NULL); // Get the DrawContext of the desktop
        if (hdc) {
            COLORREF cr;
            POINT pCursor;
            GetCursorPos(&pCursor);
            cr = GetPixel(hdc, pCursor.x, pCursor.y);
            ReleaseDC(NULL, hdc);
            *(int*)color = cr;
            color[3] = 255;
        }

        // also show color decoded as if it was a range mapped vector (e.g.,normal map)
        v[0] = (color[0]/255.f)*2.0f - 1.0f;
        v[1] = (color[1]/255.f)*2.0f - 1.0f;
        v[2] = (color[2]/255.f)*2.0f - 1.0f;
        sprintf_s(text,sizeof(text),"Color under cursor: %d %d %d  (%1.2f %1.2f %1.2f) (%1.2f %1.2f %1.2f)",
                    color[0], color[1], color[2], color[0]/255.f, color[1]/255.f, color[2]/255.f, v[0],v[1],v[2]);
        fontColor( 0xFFFFFF );
        fontScale(1.0f, 1.0f);
        fontText(x+5, y+1, text );
    }

    fontDefaults();
    fontSet(0);
    fontScale(1.0f, 1.0f);
}

// render additional help information on the screen when in
// particular modes.
// For example when visualizing blendmodes write the legend on
// screen which identifies what colors map to what blendmode
static void render_help_info(void)
{
    if (!g_mt_shader_debug_state.flag_enable)
        return;

    fontDefaults();
    fontSet(0);
    fontScale(1.0f, 1.0f);

    if (g_mt_shader_debug_state.flag_use_debug_shaders)
    {
        int screen_width, screen_height;
        int xo, yo;

        windowClientSize(&screen_width, &screen_height);
        xo = screen_width - 100;
        yo = 6;
        fontColor( 0xFFFFFF );
        fontText(xo, yo, "*DEBUG*");
    }

    if (g_mt_shader_debug_state.mode_visualize && g_mt_shader_debug_state.flag_use_debug_shaders
         && g_mt_shader_debug_state.selected_visualization ==( kSD_Control_Viz_Blendmode -kSD_Control_Viz_uv0) )
    {
        int i = 0;
        float x = 10.0f;
        float y = 200.0f;

        fontColor( 0xFFFFFF );
        fontText(x, y + i++*10, "red = MULTIPLY");
        fontText(x, y + i++*10, "green = COLORBLEND_DUAL");
        fontText(x, y + i++*10, "yellow = ADDGLOW");
        fontText(x, y + i++*10, "blue = ALPHADETAIL");
        fontText(x, y + i++*10, "magenta = BUMPMAP_MULTIPLY");
        fontText(x, y + i++*10, "cyan = BUMPMAP_COLORBLEND_DUAL");
        fontText(x, y + i++*10, "orange = WATER");
        fontText(x, y + i++*10, "purple = MULTI");

        fontText(x, y + i++*10, "black checker = SINGLE_MATERIAL");
        fontText(x, y + i++*10, "misc checker = HIGH_QUALITY_BUMP");
        fontText(x, y + i++*10, "purple checker = BUILDING");
        fontText(x, y + i++*10, "green checker = OLDTINTMODE");
    }
}

// handle input to dev HUD
static void gfxDevHudInput_Keyboard()
{
    int num_input = 0;
    KeyInput* input;

    for (input = inpGetKeyBuf(); input; inpGetNextKey(&input))
    {
        if(input->type != KIT_EditKey) {
            continue;
        }

        // Shift + Ctrl + G to toggle it on and off
        // @todo - this doesn't work because control keys get remapped with a call to
        // MapVirtualKeyEx() which returns 0 on most keys. Is that remap really necessary?
        if((input->attrib & KIA_SHIFT) && (input->attrib & KIA_CONTROL) && (input->key == 0x39))
        {
            gfxDevHudToggle();
        }
    }
}

// mouse testing and processing for rollover/click on a given control
// return true if mouse down in control
static bool mouse_control( int ctrl_id, float x, float y, float dx, float dy )
{
    int boxClick = FALSE;
    if (ctrl_id != kSD_Control_None)
    {
        SDControl*ctrl = &s_controls[ctrl_id];
//        if (ctrl->name)
        {
            // test for mouse collision with grid cell
            CBox box;
            int button = MS_LEFT;
            int inset_test = 0;
            BuildCBox(&box, x+inset_test, y+inset_test, dx-2*inset_test, dy-2*inset_test);

            //                        boxClick = mouseClickHit(&box, MS_LEFT);

            g_hud_last_mouse = gMouseInpCur;

            if ( point_cbox_clsn(gMouseInpCur.x, gMouseInpCur.y, &box) )
            {
                ctrl->rollover = 1;
            }
            else
            {
                ctrl->rollover = 0;
            }

            {
                // test for mouse hit and then swallow the mouse input
                int k;
                for ( k = 0; k < gInpBufferSize; k++ )
                {
                    if ( ( button == MS_LEFT  && gMouseInpBuf[k].left  == MS_DOWN ) ||
                        ( button == MS_RIGHT && gMouseInpBuf[k].right == MS_DOWN) )
                    {
                        float xp = gMouseInpBuf[k].x;
                        float yp = gMouseInpBuf[k].y;

                        if ( scaled_point_cbox_clsn( xp, yp, &box ) && !outside_scissor( xp, yp ) )
                        {
                            boxClick = TRUE;
                            g_hud_last_click = gMouseInpBuf[k];
//                            break;
                        }
                    }
                }
            }
            if ( boxClick || ctrl->rollover )
            {
                // if we take the mouse input clear the buffer so
                // nobody else acts on it
                collisions_off_for_rest_of_frame = true;

                memset( &gMouseInpBuf, 0, sizeof(mouse_input) * MOUSE_INPUT_SIZE );
                gInpBufferSize = 0;
            }
        }
    }
    return boxClick;
}


//mouse test a single row/bar of controls
void mouse_control_bar(float x, float y, float dx, float dy, int num_controls, int* control_ids )
{
    int j;

    //render the controls in the bar
    for (j=0; j < num_controls; ++j)
    {
        int ctrl_id = control_ids[j];
        // process mouse rollover and clicks
        bool ctrl_click = mouse_control(ctrl_id,x,y,dx,dy);
        if ( ctrl_click )
        {
            _process_control( ctrl_id );
        }
        x += dx;
    }
}

static void mouse_input_control_bar(void)
{
    float x,y;
    float dx = kBarCellWidth;
    float dy = kBarHeight;

    get_bar_location( &x, &y );

    // controls
    x += kBarMarginX;    // bar margin
    mouse_control_bar(x,y,dx,dy,sizeof(s_shader_debug_control_bar)/sizeof(int),s_shader_debug_control_bar);
}

static void mouse_input_select_bar(void)
{
    float x,y;
    float dx = kSD_CellWidth;
    float dy = kSD_CellHeight;

    get_panel_select_location( &x, &y );

    // controls
    mouse_control_bar(x,y,dx,dy,sizeof(s_panel_select_bar)/sizeof(int),s_panel_select_bar);
}

// pick the
static int* get_selected_panel(void)
{
 return (int*)s_control_panel_materials;
}

static void mouse_input_panel(void)
{
    float dx = kSD_CellWidth;
    float dy = kSD_CellHeight;
    float x,y;
    int i;

    get_panel_location(&x,&y);
    for (i=0; i < kSD_GridRows; ++i)
    {
        mouse_control_bar(x,y,dx,dy,kSD_GridCols,s_current_panel+i*kSD_GridCols);
        y += dy;
    }
}

static void gfxDevHudInput_Mouse(void)
{
    if (g_dev_hud_state == kDH_Inactive)
    {
        return;    // not enabled, nothing to do
    }

    // If mouse is currently in a drag then we ignore it
    if (mouseDragging())
    {
        return;
    }

    set_scrn_scaling_disabled(1);

        mouse_input_control_bar();
        mouse_input_select_bar();
        mouse_input_panel();

    set_scrn_scaling_disabled(0);
}

static prepare_hud_resources(void)
{
    onlydevassert(rdr_caps.filled_in);
    setCursor( "cursor_wait.tga", NULL, FALSE, 11, 10 );

    // load any hud specific rendering textures
    {
        BasicTexture*    uv_test_tex = texFind("uvtest");
        g_uv_test_tex_id = texDemandLoad(uv_test_tex);
    }

    // have render thread load the debug shaders if necessary and enable them
    rdrQueueCmd(DRAWCMD_SHADERDEBUGCONTROL);    // toggle debug shaders on
    rdrQueueFlush();    // we block main thread here

    setCursor( NULL, NULL, FALSE, 2, 2 );
}

static void restore_state_on_exit(void)
{
    // restore back to 'normal' state in case an debug features were enabled
    // make sure to show the UI when closing
    if (s_controls[kSD_Control_UI].value)
    {
        set_flags( &game_state.perf, GFXDEBUG_PERF_SKIP_2D, false );
        //                s_controls[kSD_Control_UI].value = 0; // @todo remember state here?
    }

    game_state.wireframe = 0;
    s_controls[kSD_Control_Wireframe].value = 0;

    if (s_controls[kSD_Control_Pause].value)
    {
        cmdParse("timestepscale 0");
        s_controls[kSD_Control_Pause].value = 0;
    }

    rdr_caps.features |= g_restore_gfx_features;
}

// a frame tick for graphics dev hud services
void gfxDevHudUpdate(void)
{
    // transition to the new state
    g_dev_hud_state = g_dev_hud_state_next;
    switch( g_dev_hud_state )
    {
        case kDH_Activating:
            // we give a frame to display the loading message
            g_dev_hud_state_next = kDH_Loading;
            sync_controls_to_flags();
            sync_flags_to_controls();
            break;
        case kDH_Loading:
            // we gave a frame to display the loading message
            // now prepare the necessary resources (e.g. debug shaders)
            prepare_hud_resources();
            g_dev_hud_state_next = kDH_Active;
            break;    
        case kDH_Active:
            sync_flags_to_controls(); // keep engine flags in sync with controls
            break;
        case kDH_Deactivating:
            g_dev_hud_state_next = kDH_Inactive;
            if (rdr_caps.filled_in) {
                rdrQueueCmd(DRAWCMD_SHADERDEBUGCONTROL);    // toggle debug shaders off
                rdrQueueFlush();    // we block here
            }
            restore_state_on_exit();
            break;    
    }

}

// get input for dev hud services
void gfxDevHudInput(void)
{
    gfxDevHudInput_Keyboard();
    gfxDevHudInput_Mouse();
}

// toggle the Dev HUD to be active or inactive
void gfxDevHudToggle(void)
{
    if (g_dev_hud_state == kDH_Inactive)
    {
        g_dev_hud_state_next = kDH_Activating;
    }
    else if (g_dev_hud_state == kDH_Active)
    {
        g_dev_hud_state_next = kDH_Deactivating;
    }
}

// render popup menu if any
static void render_popup_menu(void)
{
    if ( g_popup_menu_id )
    {
        render_popup_list( g_hud_last_click.x, g_hud_last_click.y, 600.0f, kSD_CellWidth, kSD_CellHeight, sizeof(s_menu_texture)/sizeof(int),s_menu_texture );
    }
}

void gfxDevHudRender(void)
{
    if (g_dev_hud_state == kDH_Inactive)
    {
        return;    // not enabled, nothing to do
    }

    rdrBeginMarker(__FUNCTION__);
    set_scrn_scaling_disabled(1);
    rdrSetup2DRendering();

    render_control_bar();
    render_panel_select_bar();
    render_control_panel();
    render_status_bar();
    render_help_info();

    render_popup_menu();

    fontDefaults();
    fontSet(0);
    fontScale(1.0f, 1.0f);

    rdrFinish2DRendering();
    set_scrn_scaling_disabled(0);
    rdrEndMarker();
}

#else

void gfxDevHudToggle(void)
{}
void gfxDevHudUpdate(void)
{}
void gfxDevHudInput(void)
{}
void gfxDevHudRender(void)
{}

#endif // FINAL
