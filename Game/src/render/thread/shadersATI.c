// These routines are the ATI specific 'register combiner' programs for old ATI cards
// when ARBFP is not available
#include "render/thread/ogl.h"
#include "render/thread/shadersATI.h"
#include "render/renderUtil.h"
#include "assert.h"
#include <utilitieslib/utils/mathutil.h>

    //
    // Fragment shaders
    //

    int fsColorBlendDual = -1;
    int fsAddGlow = -1;
    int fsAlphaDetail = -1;
    int fsMultiply = -1;
    int fsBumpMultiply = -1;
    int fsBumpColorBlend = -1;


    // NOTE:
    //  Probably need to enable separate specular color ext. for specular blending

    void atiFSGrey()
    {
        glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, GL_NONE, GL_HALF_BIT_ATI,
            GL_ONE, GL_NONE, GL_NONE); CHECKGL;
        glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, GL_NONE,
            GL_ONE, GL_NONE, GL_NONE);  CHECKGL;

    }

    void atiFSColorBlendDual(void)
    {
        
        fsColorBlendDual = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsColorBlendDual); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;
        if (0) {
            atiFSGrey();
        } else {
            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;

            glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_CON_4_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_3_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE,
                                            GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_ALPHA, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_NONE, GL_NONE); CHECKGL;
            // Add specular using FF
            /*glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_SECONDARY_INTERPOLATOR_ATI, GL_NONE, GL_NONE); CHECKGL;  */
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;

        }
    
        glEndFragmentShaderATI(); CHECKGL;
    }

    void atiFSAddGlow(void)
    {
        fsAddGlow = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsAddGlow); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;
        
        if (0) {
            atiFSGrey();
        } else {
            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;

            glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_ALPHA, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_SECONDARY_INTERPOLATOR_ATI, GL_NONE, GL_NONE); CHECKGL;
        // GR ??? what to do about fog blending ???
        /*    glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_CON_5_ATI, GL_ALPHA, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_CON_5_ATI, GL_NONE, GL_NONE); CHECKGL;*/
        // ???
            glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            glAlphaFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE); CHECKGL;
        }
    
        glEndFragmentShaderATI(); CHECKGL;
    }

    void atiFSAlphaDetail(void)
    {
        fsAlphaDetail = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsAlphaDetail); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;

        if (0) {
            atiFSGrey();
        } else {
            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;

            glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_ALPHA, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE); CHECKGL;
            // Add specular using FF
            /*    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_SECONDARY_INTERPOLATOR_ATI, GL_NONE, GL_NONE); CHECKGL;*/
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
        }

        glEndFragmentShaderATI(); CHECKGL;
    }

    void atiFSMultiply(void)
    {
        fsMultiply = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsMultiply); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;

        if (0) {
            atiFSGrey();
        } else {
#if 1
            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;

            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE); CHECKGL;
        // Add specular using FF
        /*    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_SECONDARY_INTERPOLATOR_ATI, GL_NONE, GL_NONE); CHECKGL;*/
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE); CHECKGL;
#else
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_4X_BIT_ATI | GL_SATURATE_BIT_ATI,
                                            GL_ONE, GL_NONE, GL_NONE,
                                            GL_ONE, GL_NONE, GL_NONE); CHECKGL;
#endif
        }
        
        glEndFragmentShaderATI(); CHECKGL;
    }


    void atiFShaderBumpMultiply(void)
    {
        fsBumpMultiply = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsBumpMultiply); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;

        if (0) {
            atiFSGrey();
        } else {
            glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE2, GL_SWIZZLE_STQ_ATI); CHECKGL; // bumpmap
            glPassTexCoordATI(GL_REG_5_ATI, GL_TEXTURE3, GL_SWIZZLE_STR_ATI); CHECKGL; // halfway vector 

            // H.H
            glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE); CHECKGL;
            // r0 = (H(1 - H.H) + 2H) / 2  ->  norm(H)
            glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, GL_NONE, GL_HALF_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_COMP_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_2X_BIT_ATI); CHECKGL;
            // r0 = r0.bumpmap  -> H.L
            glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_BIAS_BIT_ATI | GL_2X_BIT_ATI); CHECKGL;
            // r0 = r0.r * r0.r
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_RED, GL_NONE,
                                            GL_REG_0_ATI, GL_RED, GL_NONE); CHECKGL;

            // r0 = r0.r * r0.r  -> (H.L)^4
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_RED, GL_NONE,
                                            GL_REG_0_ATI, GL_RED, GL_NONE); CHECKGL;

            // r3 = r2.a * r0.r  -> specular strength?
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_RED, GL_NONE,
                                            GL_REG_2_ATI, GL_ALPHA, GL_NONE); CHECKGL;

            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glPassTexCoordATI(GL_REG_3_ATI,GL_REG_3_ATI, GL_SWIZZLE_STR_ATI); CHECKGL;

            // r2 = 2 (r1 * r0)
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            // A: r2 = r1 * c3
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;

            // r0 = 4 (col * r2)
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_4X_BIT_ATI,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE); CHECKGL;

            // r0 = r0 + r3
            glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;
            // A: r0 = r0 * r2
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE); CHECKGL;
        }

        glEndFragmentShaderATI(); CHECKGL;
    }

    // this is a one pass version of the shader - not currently used
    void atiFShaderBumpMultiplyNew(void)
    {
        fsBumpMultiply = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsBumpMultiply); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;
        if (0) {
            atiFSGrey();
        } else {
            glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE2, GL_SWIZZLE_STQ_ATI); CHECKGL;
            glPassTexCoordATI(GL_REG_5_ATI, GL_TEXTURE3, GL_SWIZZLE_STR_ATI); CHECKGL;

            glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_4_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE); CHECKGL;
            glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_4_ATI, GL_NONE, GL_HALF_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_NONE, GL_COMP_BIT_ATI,
                                            GL_REG_5_ATI, GL_NONE, GL_2X_BIT_ATI); CHECKGL;

            glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_NONE, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_BIAS_BIT_ATI | GL_2X_BIT_ATI); CHECKGL;

            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_RED, GL_NONE,
                                            GL_REG_4_ATI, GL_RED, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_RED, GL_NONE,
                                            GL_REG_4_ATI, GL_RED, GL_NONE); CHECKGL;
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_4_ATI, GL_RED, GL_NONE,
                                            GL_REG_2_ATI, GL_ALPHA, GL_NONE); CHECKGL;
        
            glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE,
                                            GL_CON_3_ATI, GL_NONE, GL_NONE,
                                            GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;
            //glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_4X_BIT_ATI,
            //                                  GL_REG_2_ATI, GL_NONE, GL_NONE,
            //                                  GL_PRIMARY_COLOR_EXT, GL_NONE, GL_NONE); CHECKGL;
            //glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
            //                                  GL_REG_0_ATI, GL_NONE, GL_NONE,
            //                                  GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;

            glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                            GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI,
                                            GL_PRIMARY_COLOR_EXT, GL_NONE, GL_2X_BIT_ATI,
                                            GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;
            //glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
            //                                  GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI,
            //                                  GL_PRIMARY_COLOR_EXT, GL_NONE, GL_2X_BIT_ATI); CHECKGL;
                                              

            glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                            GL_REG_2_ATI, GL_NONE, GL_NONE,
                                            GL_REG_0_ATI, GL_NONE, GL_NONE); CHECKGL;
        }

        glEndFragmentShaderATI(); CHECKGL;
    }

    void atiFSTest(void)
    {
        // GL_PRIMARY_COLOR_ARB  GL_SECONDARY_INTERPOLATOR_ATI
        glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                GL_PRIMARY_COLOR_ARB, GL_NONE, GL_NONE); CHECKGL;
    }

#include <utilitieslib/utils/timing.h>
#define CHECK {result = gldGetError(); if(result!=0) { printf("%s: Error Compiling\n", timerGetTimeString());} }
    void atiFSBumpColorBlend(void)
    {
        int result=0;
        fsBumpColorBlend = glGenFragmentShadersATI(1); CHECKGL;
        glBindFragmentShaderATI(fsBumpColorBlend); CHECKGL;
        glBeginFragmentShaderATI(); CHECKGL;

        if (0) {
            atiFSTest();
        } else {
    glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL;    // bumpmap
    glPassTexCoordATI(GL_REG_5_ATI, GL_TEXTURE3, GL_SWIZZLE_STR_ATI); CHECKGL; // halfway vector (halfWayTS from ATI VS)
    glPassTexCoordATI(GL_REG_4_ATI, GL_TEXTURE4, GL_SWIZZLE_STR_ATI); CHECKGL; // light direction (lightDirTS from ATI VS)
//JE: Sadly, GL_PRIMARY_COLOR_ARB is only valid as an input in the second pass of a 2-pass shader like this one
//    glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
//                            GL_PRIMARY_COLOR_ARB, GL_NONE, GL_NONE); CHECKGL;
//    glColorFragmentOp1ATI(GL_MOV_ATI, GL_REG_5_ATI, GL_NONE, GL_NONE,
//                            GL_SECONDARY_INTERPOLATOR_ATI, GL_NONE, GL_NONE); CHECKGL;

    // r0 = H.H
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                    GL_REG_5_ATI, GL_NONE, GL_NONE,
                                    GL_REG_5_ATI, GL_NONE, GL_NONE); CHECKGL;   //halfWayTS 
    
    //A: r0 = (r4.b * r4.b) * 4   -> 4Lz^2
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_4X_BIT_ATI | GL_SATURATE_BIT_ATI,
                                    GL_REG_4_ATI, GL_BLUE, GL_NONE,     //lightDirTS
                                    GL_REG_4_ATI, GL_BLUE, GL_NONE); CHECKGL;    //lightDirTS

    // r1 = (H(1 - H.H) + 2H) / 2  ->  norm(H)
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_1_ATI, GL_NONE, GL_HALF_BIT_ATI,
                                    GL_REG_5_ATI, GL_NONE, GL_NONE,             //halfWayTS
                                    GL_REG_0_ATI, GL_NONE, GL_COMP_BIT_ATI,
                                    GL_REG_5_ATI, GL_NONE, GL_2X_BIT_ATI); CHECKGL;   //halfWayTS
    
    //A: r1 = r0 * c0 -> 4Lz^2 * gloss
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_1_ATI, GL_NONE,
                                    GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_CON_0_ATI, GL_NONE, GL_NONE); CHECKGL;  //Gloss[3]

    // r0 = lightDir.bumpmap -> N.L
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                    GL_REG_4_ATI, GL_NONE, GL_NONE,    //lightDirTS
                                    GL_REG_2_ATI, GL_NONE, GL_BIAS_BIT_ATI | GL_2X_BIT_ATI); CHECKGL; //Bumpmap

    // r1 = r1.bumpmap  -> N.H
    glColorFragmentOp2ATI(GL_DOT3_ATI, GL_REG_1_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                    GL_REG_1_ATI, GL_NONE, GL_NONE,
                                    GL_REG_2_ATI, GL_NONE, GL_BIAS_BIT_ATI | GL_2X_BIT_ATI); CHECKGL;
    
    //A: r1 = r1 * r2 -> 4Lz^2 * gloss * bumpmap.w
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_1_ATI, GL_NONE,
                                    GL_REG_1_ATI, GL_NONE, GL_NONE,
                                    GL_REG_2_ATI, GL_NONE, GL_NONE); CHECKGL;

    // r3 = r0 * c2 + c1   -> diffuse lighting + ambient
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_3_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_CON_2_ATI, GL_NONE, GL_NONE,   //Diffuse
                                    GL_CON_1_ATI, GL_NONE, GL_NONE); CHECKGL;  //Ambient

    //A: r3 = r1.b * r1.b  -> specular^2
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_3_ATI, GL_NONE,
                                    GL_REG_1_ATI, GL_BLUE, GL_NONE,
                                    GL_REG_1_ATI, GL_BLUE, GL_NONE); CHECKGL;

    // r0 = r3.a * r3.a  -> specular^4
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_REG_3_ATI, GL_ALPHA, GL_NONE,
                                    GL_REG_3_ATI, GL_ALPHA, GL_NONE); CHECKGL;

    //A: r0 = r1 * r3 -> 4Lz^2 * gloss * bumpmap.w * specular^2
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                    GL_REG_1_ATI, GL_NONE, GL_NONE,
                                    GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;

    // r4 = r0 * r0.a -> specular^6 * 4Lz^2 * gloss * bumpmap.w
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_ALPHA, GL_NONE); CHECKGL;

    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0, GL_SWIZZLE_STQ_ATI); CHECKGL; //Bizarrely doesn't care which texture you declare 
    // JE: the texture arguments specifie the texture *coordinates* not the texture value (which is implicty from the GL_REGx parameter)
    glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1, GL_SWIZZLE_STQ_ATI); CHECKGL; //""
    glPassTexCoordATI(GL_REG_3_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI); CHECKGL; //Preserves register contents across passes
    glPassTexCoordATI(GL_REG_4_ATI, GL_REG_4_ATI, GL_SWIZZLE_STR_ATI); CHECKGL; //""
    
    // r2 = lerp(r1, c3, c4)
    glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_2_ATI, GL_NONE, GL_NONE,
                                    GL_REG_1_ATI, GL_NONE, GL_NONE,
                                    GL_CON_3_ATI, GL_NONE, GL_NONE, //Color0
                                    GL_CON_4_ATI, GL_NONE, GL_NONE); CHECKGL;//Color1

    // r2 = r3 * r2
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_NONE,
                                    GL_REG_3_ATI, GL_NONE, GL_NONE, // diffuse
                                    GL_REG_2_ATI, GL_NONE, GL_NONE); CHECKGL; // lerp above

    // r3 = r3 * r0
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_3_ATI, GL_NONE, GL_NONE,
                                    GL_REG_3_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_NONE, GL_NONE); CHECKGL;

    // r2 = r2 * r3
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_2_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
                                    GL_REG_2_ATI, GL_NONE, GL_NONE,
                                    GL_REG_3_ATI, GL_NONE, GL_NONE); CHECKGL;

    // r0 = lerp( r0, r3, r2)
    glColorFragmentOp3ATI(GL_LERP_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_ALPHA, GL_NONE,
                                    GL_REG_3_ATI, GL_NONE, GL_NONE,
                                    GL_REG_2_ATI, GL_NONE, GL_NONE); CHECKGL;
    
    // r0 = r0 + r4
    glColorFragmentOp2ATI(GL_ADD_ATI, GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_REG_0_ATI, GL_NONE, GL_NONE,
                                    GL_REG_4_ATI, GL_NONE, GL_NONE); CHECKGL;

    //A: r0 = c3 * r1
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_0_ATI, GL_NONE,
                                    GL_CON_3_ATI, GL_NONE, GL_NONE, //Color0
                                    GL_REG_1_ATI, GL_NONE, GL_NONE); CHECKGL;                                
        }
    
        glEndFragmentShaderATI(); CHECKGL;
    }

    

    F32 atiAmbientColorScaleFloat(F32 x) {
        // From empirical data:
        //    NV, ATI
        //    0.0, 0.0
        //    0.14, 0.3
        //    0.27, 0.5
        //    0.65, 0.8
        //    0.8, 0.9
        //    1.0, 1.0
        return 2.459*x -2.622*x*x + 1.165*x*x*x;
        //    // And yet it still didn't look quite right, so I tweaked the numbers :(
        //    return 2.3*x -2.6*x*x + 1.165*x*x*x;
    }

    void atiAmbientColorScale(Vec3 color) {
        static Vec3 lastInColor={0}, lastOut={0};
        if (sameVec3(color, lastInColor)) {
            copyVec3(lastOut, color);
        } else {
            copyVec3(color, lastInColor);
            color[0] = atiAmbientColorScaleFloat(color[0]);
            color[1] = atiAmbientColorScaleFloat(color[1]);
            color[2] = atiAmbientColorScaleFloat(color[2]);
            copyVec3(color, lastOut);
        }
    }

    F32 atiDiffuseScaleAtAmbientZero(F32 x) {
        // From empirical data:
        //    NV, ATI
        //    1.0,    1.18
        //    0.7,    1
        //    0.53    0.9
        //    0.35    0.8
        //    0.14,    0.5
        //    0,    0
        return 3.9220582890909936*x -5.701895280934311*x*x + 2.9182240979874607*x*x*x;
    }

    F32 atiDiffuseScaleAtAmbientZeroPointThree(F32 x) {
        // From emprical data:
        //    NV, ATI
        //    0,    0
        //    0.17,    0.1
        //    0.37,    0.2
        //    0.58,    0.3
        //    0.85,    0.4
        //    1.0,    0.45
        return 0.6015287615976362*x -0.152746354901567*x*x;
    }

    F32 atiDiffuseColorScaleFloat(F32 x, F32 ambient) {
        if (ambient < 0.3) {
            F32 d0 = atiDiffuseScaleAtAmbientZero(x);
            F32 d1 = atiDiffuseScaleAtAmbientZeroPointThree(x);
            return (d1 - d0) / 0.3 * ambient + d0;
        } else {
            return atiDiffuseScaleAtAmbientZeroPointThree(x);
        }
    }

    void atiDiffuseColorScale(Vec3 color, Vec3 origAmbient) {
        static Vec3 lastInColor={0}, lastInAmbient={0}, lastOut={0};
        if (sameVec3(lastInColor, color) &&
            sameVec3(lastInAmbient, origAmbient))
        {
            copyVec3(lastOut, color);
        } else {
            copyVec3(origAmbient, lastInAmbient);
            copyVec3(color, lastInColor);
            color[0] = atiDiffuseColorScaleFloat(color[0], origAmbient[0]);
            color[1] = atiDiffuseColorScaleFloat(color[1], origAmbient[0]);
            color[2] = atiDiffuseColorScaleFloat(color[2], origAmbient[0]);
            copyVec3(color, lastOut);
        }
    }
