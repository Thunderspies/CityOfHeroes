#ifndef UIBOX_H
#define UIBOX_H

#include <utilitieslib/utils/Cbox.h>

typedef struct UIBox
{
    union
    {
        PointFloatXY origin;

        struct{
            float x;
            float y;
        };
    };
    float width;
    float height;
} UIBox;

UIBox* uiBoxCreate(void);
void uiBoxDestroy(UIBox* box);

void uiBoxNormalize(UIBox* box);
void uiBoxToCBox(UIBox* uiBox, CBox* cBox);
void uiBoxFromCBox(UIBox* uiBox, CBox* cBox);
int uiBoxIntersects(UIBox* box1, UIBox* box2);
//int uiBoxEncloses(UIBox* outterBox, UIBox* innerBox); not tested


typedef enum
{
    UIBAD_NONE    = 0,
    UIBAD_LEFT    = (1 << 0),
    UIBAD_TOP    = (1 << 1),
    UIBAD_RIGHT    = (1 << 2),
    UIBAD_BOTTOM= (1 << 3),
    UIBAD_ALL = UIBAD_LEFT | UIBAD_TOP | UIBAD_RIGHT | UIBAD_BOTTOM,
} UIBoxAlterDirection;
typedef enum
{
    UIBAT_GROW = 1,
    UIBAT_SHRINK = -1,
} UIBoxAlterType;

void uiBoxAlter(UIBox* box, UIBoxAlterType type, UIBoxAlterDirection direction, unsigned int magnitude);

int uiMouseClickHit(UIBox* box, int button);
int uiMouseUpHit(UIBox* box, int button);
int uiMouseCollision(UIBox* box);

void uiDrawBox(UIBox* uiBox, int z, int borderColor, int interiorColor);
int uiBoxDrawStdButton(UIBox buttonBox, float z, int color, char *txt, float txtScale, int flags);

void uiBoxDefine(UIBox* box, float x, float y, float width, float height);
void uiBoxDefineScaled(UIBox * uibox, F32 xp, F32 yp, F32 wd, F32 ht);
void uiBoxMakeInbounds(UIBox const *parent, UIBox *child);


#endif
