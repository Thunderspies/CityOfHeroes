#ifndef UIGENDER_H
#define UIGENDER_H

#include "UI/uiInclude.h"
#include "entity/costume.h"

typedef struct BodyConfigScales
{
    F32     global;
    F32     bone;
    F32        head;
    F32     chest;
    F32     shoulder;
    F32        waist;
    F32     hip;
    F32        leg;
    F32        arm;

    Vec3    face[NUM_3D_BODY_SCALES];

}BodyConfigScales;

typedef struct BodyConfig{

    char *    displayName;
    char *    bodyBuild;    // athletic, 
    char *    bodyType;    // male, female, huge    

    union
    {
        struct {
            BodyConfigScales scales;
        };

        struct {
            float fScales[MAX_BODY_SCALES];
        };
    };


}BodyConfig;

typedef enum BuildPresetTypes
{
    BUILD_SLIM,
    BUILD_AVERAGE,
    BUILD_ATHLETIC,
    BUILD_HEAVY,
    BUILD_NUM,
} BuildPresetTypes;

extern BodyConfig sBodyConfig;

// Displays the gender selection menu
//
void genderMenu();

void gender_reset();
void LoadConfigs();
void convergeConfigs();
void setConfigs();
void tailorGenderInit( Entity * e );

void genderScale(            float x, float y, float screenScaleX, float screenScaleY);
void genderBoneScale(        float x, float y, float screenScaleX, float screenScaleY, float uiScale, int costume);
void genderLegScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume);
void genderChestScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume);
void genderWaistScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume);
void genderHipScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume);
void genderShoulderScale(    float x, float y, float screenScaleX, float screenScaleY, float uiScale, int costume);
void genderArmScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume);
void genderFaceScale(        float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume, int part);

F32 adjustScale(F32 orig, F32 range);
void genderSetScalesFromApperance( Entity * e );
void setAnim(int i, int *flashBits);
void setAnims(const char **bitnames);

void SelectConfig(int idx, int prevIdx);
int buildMatchConfig(void);

extern int green_blobby_fx_id;
extern int gCurrentGender;
extern int gCurrentBuild;
void load_MenuAnimations();
typedef struct ComboBox ComboBox;
extern ComboBox comboAnims;
#endif