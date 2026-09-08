// These routines are the ATI specific 'register combiner' programs for old ATI cards
// when ARBFP is not available
#ifndef __shaders_ati__
#define __shaders_ati__

// adjust if necessary
#define ATI_MAX_BONES 20

extern int fsColorBlendDual;
extern int fsAddGlow;
extern int fsAlphaDetail;
extern int fsMultiply;
extern int fsBumpMultiply;
extern int fsBumpColorBlend;

void atiFShaderBumpVertDiffuse(void);
void atiFShaderBumpMultiply(void);
void atiFSColorBlendDual(void);
void atiFSAddGlow(void);
void atiFSAlphaDetail(void);
void atiFSMultiply(void);
void atiFSBumpColorBlend(void);

// Scale an ambient color value to make the ATI FSBumpColorBlend produce the same resutls as nVidia/ARBfp shaders
void atiAmbientColorScale(Vec3 color);
// Scale an diffuse color value, based on ambient, to make the ATI FSBumpColorBlend produce the same resutls as nVidia/ARBfp shaders
void atiDiffuseColorScale(Vec3 color, Vec3 origAmbient);

#endif //  __shaders_ati__
