// These routines are the OpenGL generic 'register combiner' texture environments
// used when ARBFP is not available

#define WCW_STATEMANAGER
#define RT_PRIVATE 1
#define RT_ALLOW_BINDTEXTURE
#include "shadersTexEnv.h"
#include "render/thread/ogl.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include "assert.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/thread/rt_tex.h"

typedef enum TexEnvFlags {
    TEXENV_NOLIGHTING=1<<0,
    TEXENV_NOBLEND=1<<1,
    TEXENV_CONSTANTASCOLOR=1<<2,
} TexEnvFlags;

typedef struct TextureOp {
    char **raw;
    int op;
    int numOperands;
    struct {
        int source;
        int sourceOp;
    } operand[3];
    F32 scale;
} TextureOp;

typedef struct TextureStage
{
    TextureOp rgb;
    TextureOp alpha;
    GLenum mode;
    int texunitSource;
} TextureStage;

typedef struct TexEnvProg {
    TextureStage **stages;
    int flags;
    int numstages;
} TexEnvProg;

TexEnvProg **eaTexEnvProgs=NULL;

static StaticDefineInt texEnvFlags[] = {
    DEFINE_INT
    { "NoLighting",        TEXENV_NOLIGHTING },
    { "NoBlend",        TEXENV_NOBLEND },
    { "ConstantAsColor",TEXENV_CONSTANTASCOLOR},
    DEFINE_END
};

static StaticDefineInt texEnvModes[] = {
    DEFINE_INT
    { "Combine",    GL_COMBINE_ARB},
    { "Decal",        GL_DECAL},
    { "Modulate",    GL_MODULATE},
    DEFINE_END
};

TokenizerParseInfo parseTextureOp[] = {
    { "{",        TOK_IGNORE | TOK_STRUCTPARAM },
    { "result",    TOK_STRINGARRAY(TextureOp,raw)},
    { "Scale",    TOK_F32(TextureOp,scale, 1)},
    { "}",        TOK_END},
    { "", 0, 0 }
};

TokenizerParseInfo parseTextureStage[] = {
    { "rgb",        TOK_EMBEDDEDSTRUCT(TextureStage,rgb,parseTextureOp)},
    { "alpha",        TOK_EMBEDDEDSTRUCT(TextureStage,alpha,parseTextureOp)},
    { "Mode",        TOK_INT(TextureStage,mode,    GL_COMBINE_ARB),        texEnvModes},
    { "Texture",    TOK_INT(TextureStage,texunitSource,    -1),            NULL},
    { "}",            TOK_END},
    { "", 0, 0 }
};

TokenizerParseInfo parse_tec_file[] = {
    { "{",        TOK_STRUCT(TexEnvProg,stages,parseTextureStage) },
    { "Flags",    TOK_FLAGS(TexEnvProg,flags,0),texEnvFlags},
    { "", 0, 0 }
};

#define STREQ(a,b) (stricmp((a),(b))==0)
#define TOKEQ(a) STREQ(token,a)
static int processTextureOp(TextureOp *texop, GLenum defaultSourceOp)
{
    int i=0;
    int operandnum=-1;
    char *token;
    static char *delims = ",=.()\t ";
    char buffer[1024]="";
    if (!texop->raw) {
        Errorf("Missing required parameter: result");
        return 0;
    }
    for (i=0; i<eaSize(&texop->raw); i++) {
        strcat(buffer, texop->raw[i]);
        strcat(buffer, " ");
    }
    assert(strlen(buffer) < ARRAY_SIZE(buffer));
    token = strtok(buffer, delims);
    texop->op = GL_REPLACE;
    texop->numOperands = 1;
    while (token) {
        if (TOKEQ("lerp")) {
            texop->op = GL_INTERPOLATE_ARB;
            texop->numOperands = 3;
        } else if (TOKEQ("*") || TOKEQ("MUL")) {
            texop->op = GL_MODULATE;
            texop->numOperands = 2;
        } else if (TOKEQ("+") || TOKEQ("ADD")) {
            texop->op = GL_ADD;
            texop->numOperands = 2;
        } else if (TOKEQ("+-") || TOKEQ("ADDS")) {
            texop->op = GL_ADD_SIGNED_ARB;
            texop->numOperands = 2;
        } else if (TOKEQ("-") || TOKEQ("SUB")) {
            texop->op = GL_SUBTRACT_ARB;
            texop->numOperands = 2;
        } else if (TOKEQ("previous") || TOKEQ("texture") || TOKEQ("primary") || TOKEQ("constant")) {
            operandnum++;
            if (operandnum >= ARRAY_SIZE(texop->operand)) {
                Errorf("Too many operands");
                return 0;
            }
            if (TOKEQ("previous")) {
                texop->operand[operandnum].source = GL_PREVIOUS_ARB;
            } else if (TOKEQ("texture")) {
                texop->operand[operandnum].source = GL_TEXTURE;
            } else if (TOKEQ("primary")) {
                texop->operand[operandnum].source = GL_PRIMARY_COLOR_ARB;
            } else if (TOKEQ("constant")) {
                texop->operand[operandnum].source = GL_CONSTANT_ARB;
            } else assert(0);
            texop->operand[operandnum].sourceOp = defaultSourceOp;
        } else if (TOKEQ("rgb") || TOKEQ("alpha") || TOKEQ("a") || TOKEQ("xyz") || TOKEQ("w")) {
            if (operandnum < 0) {
                Errorf("Got .rgb or .alpha before an operand!");
                return 0;
            }
            if (TOKEQ("rgb") || TOKEQ("xyz")) {
                texop->operand[operandnum].sourceOp = GL_SRC_COLOR;
            } else if (TOKEQ("alpha") || TOKEQ("a") || TOKEQ("w")) {
                texop->operand[operandnum].sourceOp = GL_SRC_ALPHA;
            } else assert(0);
        } else {
            Errorf("Unrecognized token: %s", token);
            return 0;
        }
        token = strtok(NULL, delims);
    }

    if (operandnum+1 != texop->numOperands) {
        Errorf("Incorrect number of operands : %d, expected %d", operandnum+1, texop->numOperands);
        return 0;
    }
    
    return 1;
}

static int processRawText(TokenizerParseInfo pti[], TexEnvProg* texEnvProg)
{
    int numstages=0;
    int ret=1;
    int i;
    numstages = eaSize(&texEnvProg->stages);
    for (i=0; i<numstages; i++) {
        TextureStage *stage = texEnvProg->stages[i];
        if (stage->mode == GL_COMBINE_ARB) {
            ret &= processTextureOp(&stage->rgb, GL_SRC_COLOR);
            ret &= processTextureOp(&stage->alpha, GL_SRC_ALPHA);
        }
        if (stage->texunitSource == -1) {
            stage->texunitSource = i;
        }
    }
    if (numstages == 0)
        return 0;
    return ret;
}

#define MAX_STAGES 3
int renderTexEnvparse(char *filename, BlendModeShader blendMode)
{
    TexEnvProg *texEnvProg;
    int ret;
    int numstages;
    int i, j;

    if (texEnvProg = eaGet(&eaTexEnvProgs, blendMode)) {
        // free the old one
        ParserDestroyStruct(parse_tec_file, texEnvProg);
        ParserFreeStruct(texEnvProg);
        eaSet(&eaTexEnvProgs, NULL, blendMode);
    }

    // alloc new one
    texEnvProg=ParserAllocStruct(sizeof(TexEnvProg));
    if (!eaTexEnvProgs)
        eaCreate(&eaTexEnvProgs);
    if (eaSize(&eaTexEnvProgs) < blendMode+1)
        eaSetSize(&eaTexEnvProgs, blendMode+1);
    eaSet(&eaTexEnvProgs, texEnvProg, blendMode);

    ret = ParserLoadFiles(NULL, filename, NULL, 0, parse_tec_file, texEnvProg, NULL, NULL, (ParserLoadPreProcessFunc)processRawText);

    numstages = eaSize(&texEnvProg->stages);
    if (ret && numstages) {
        for (i=0; i<numstages; i++) {
            TextureStage *stage = texEnvProg->stages[i];
            // Enable appropriate texture unit
            glActiveTextureARB(GL_TEXTURE0+i); CHECKGL;
            glEnable(GL_TEXTURE_2D); CHECKGL;

            // Set up GL_ARB_texture_env_combine mode for this unit
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, stage->mode); CHECKGL;
            if (stage->mode == GL_COMBINE_ARB) {
                // Set the color combination Operator
                glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, stage->rgb.op); CHECKGL;
                // Set the color operands
                for (j=0; j<stage->rgb.numOperands; j++) {
                    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB+j, stage->rgb.operand[j].source); CHECKGL;
                    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB+j, stage->rgb.operand[j].sourceOp); CHECKGL;
                }
                // Set the scale (normally 1)
                glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, stage->rgb.scale); CHECKGL;

                // Set the alpha combination Operator
                glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, stage->alpha.op); CHECKGL;
                // Set the alpha operands
                for (j=0; j<stage->alpha.numOperands; j++) {
                    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB+j, stage->alpha.operand[j].source); CHECKGL;
                    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB+j, stage->alpha.operand[j].sourceOp); CHECKGL;
                }
            }
        }

        // Turn off the other stages
        for (i=numstages; i<MAX_STAGES; i++) {
            glActiveTextureARB(GL_TEXTURE0+i); CHECKGL;
            glDisable(GL_TEXTURE_2D); CHECKGL;
        }
    }
    texEnvProg->numstages = numstages;

    return ret;
}

void preRenderTexEnvSetupBlendmode(BlendModeShader type)
{
    TexEnvProg *texEnvProg= eaTexEnvProgs[type];
    int i;
    int numstages;

    if (texEnvProg) {
        numstages = texEnvProg->numstages;
    } else {
        assert(type == BLENDMODE_MODULATE);
        numstages = 2;
    }

    for (i=0; i<numstages; i++) {
        WCW_EnableTexture(GL_TEXTURE_2D, TEXLAYER_BASE+i);
    }
    for (i=numstages; i<MAX_STAGES; i++) {
        WCW_DisableTexture(TEXLAYER_BASE+i);
    }
    // All lists return to this active texture at the end
    WCW_ActiveTexture(0);
}

void renderTexEnvSetupBlendmode(BlendModeShader type)
{
    static bool colorHasBeenSet=false;
    bool colorHasBeenSetThisCall=false;
    TexEnvProg *texEnvProg= eaTexEnvProgs[type];
    int i;
    int numstages;
    
    if (texEnvProg) {
        numstages = texEnvProg->numstages;
    } else {
        assert(type == BLENDMODE_MODULATE);
        numstages = 2;
    }

    for (i=0; i<numstages; i++) {
        int texunitSource = i;
        if (texEnvProg) {
            texunitSource = texEnvProg->stages[i]->texunitSource;
        }

        WCW_EnableTexture(GL_TEXTURE_2D, TEXLAYER_BASE+i);

        if (boundTextures[texunitSource]) {
            WCW_BindTexture(GL_TEXTURE_2D, TEXLAYER_BASE+i, boundTextures[texunitSource]);
        }
        if (texCoordPointersEnabled[texunitSource]) {
            WCW_EnableClientState(GLC_TEXTURE_COORD_ARRAY_0 + i);
            WCW_TexCoordPointer(i, 2, GL_FLOAT, texCoordStrides[texunitSource], texCoordPointers[texunitSource]);
        } else {
            WCW_DisableClientState(GLC_TEXTURE_COORD_ARRAY_0 + i);
        }
    }
    for (i=numstages; i<MAX_STAGES; i++) {
        WCW_DisableTexture(TEXLAYER_BASE+i);
        WCW_DisableClientState(GLC_TEXTURE_COORD_ARRAY_0 + i);
    }
    if (texEnvProg && texEnvProg->flags & TEXENV_NOLIGHTING) {
        WCW_OverrideGL_LIGHTING(0);
    } else {
        WCW_CancelOverrideGL_LIGHTING();
    }
    if (texEnvProg) {
        if (texEnvProg->flags & TEXENV_NOBLEND) {
            WCW_DisableBlend();
        } else {
            WCW_EnableBlend();
        }
        if (texEnvProg->flags & TEXENV_CONSTANTASCOLOR) {
            extern float combiconst[2][4];
            WCW_Color4(combiconst[1][0] * 255, combiconst[1][1] * 255, combiconst[1][2] * 255, combiconst[1][3] * 255); CHECKGL;
            colorHasBeenSet = true;
            colorHasBeenSetThisCall = true;
        }
    }
    if (!colorHasBeenSetThisCall && colorHasBeenSet) {
        colorHasBeenSet = false;
        WCW_Color4(255,255,255,255);
    }
}
