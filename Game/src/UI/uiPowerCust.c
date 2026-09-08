
#include <utilitieslib/stdtypes.h>
#include "UI/uiPowerCust.h"

#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>

#include <utilitieslib/utils/Cbox.h>
#include "entity/character_animfx_client.h"
#include "cmdparse/cmdgame.h"
#include "entity/costume_client.h"
#include <utilitieslib/components/earray.h>
#include "entity/entclient.h"
#include "entity/entity.h"
#include "graphics/font.h"
#include "graphics/FX/fx.h"
#include "win/input.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "gameComm/Npc.h"
#include "player/player.h"
#include "entity/power_customization.h"
#include "entity/powers.h"
#include "seq/seq.h"
#include "seq/seqskeleton.h"
#include "seq/seqstate.h"
#include "sound/sound.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiAvatar.h"
#include "UI/uiCostume.h"
#include "UI/uiDialog.h"
#include "UI/uiGame.h"
#include "UI/uiGender.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "uiSaveCostume.h"
#include "uiScrollSelector.h"
#include "UI/uiTailor.h"
#include "UI/uiToolTip.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "gameComm/VillainDef.h"
#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"

#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiPower.h"
#include "uiLoadCostume.h"
#include "uiSaveCostume.h"

#define REGION_X 30
#define REGION_WD 350
#define REGION_CENTER (REGION_X + REGION_WD / 2)
#define BS_SELECTOR_WD 300
#define REGION_Y 135
#define AREA_HT 560.0f
#define POWERSET_BUTTON_HT 50
#define GSET_HT 40
#define GSET_MAX_WD 300
#define GSET_MIN_WD 250
#define GSET_SPEED 10
#define RSET_SPEED 40
#define BULLET_WD 25
#define MAX_POWERSETS 9

static const Vec3 ZOOMED_OUT_POSITION = {0.7015f, 2.6f, 20.5f};
static const Vec3 ZOOMED_IN_POSITION = {0.7015f, 2.6f, 13.5f};
static const int CATEGORIES[] = {kCategory_Primary, kCategory_Secondary};
static const int NUM_CATEGORIES = ARRAY_SIZE(CATEGORIES);
static int s_outOfDate;
static int costumeButtonColor, costumeTextColor1, costumeTextColor2, costumeFrameBackColor1, costumeFrameBackColor2;
PowerCustomizationList *gCopyOfPowerCustList = NULL;

typedef enum PowerCustSet
{
    POWERCUSTSET_PRIMSEC,   // MENU_POWER_CUST
    POWERCUSTSET_POOLS,     // MENU_POWER_CUST_POWERPOOL
    POWERCUSTSET_INCARNATE, // MENU_POWER_CUST_INCARNATE

    POWERCUSTSET_COUNT
} PowerCustSet;

#define CURRENT_POWER_CUST_MENU (current_menu() - MENU_POWER_CUST)

static bool menuInitialized = false;
static const BasePowerSet* sSelectedPowerSets[POWERCUSTSET_COUNT];
static const BasePower* sSelectedPowers[POWERCUSTSET_COUNT];
static int gSelectedTintComponent = 1; // 1 == primary; 2 == secondary
static float gAnimationProgress;
static int gPowerColorsLinked;
int gZoomedInForPowers;
extern GenderChangeMode genderEditingMode;

static int sTailorHasPoolPowers;
static int sTailorHasIncarnatePowers;

typedef enum powerCust_linkedColorsMode
{
    POWERCUST_LINKED_COLORS_NONE = 0,
    POWERCUST_LINKED_COLORS_POWERSET = 1,
    POWERCUST_LINKED_COLORS_ALL,
} powerCust_linkedColorsMode;

#define MAX_ANIM_STAGES 100
#define MAX_SUSTAINED_SEQUENCES 100
#define MAX_SPAWNS 3

const Vec3 gSpawnOffsets[] = {{0, 0, 0}, {3, 0, 0}, {-2, 0, 1}};

static bool gLoopNow;
static float gAnimationProgress;
static int gAnimStage;
static const cCostume* gOwnedCostume;
#define MIN_EVENT_INTERVAL 150.0f

typedef enum
{
    POWERANIM_END = 0,
    POWERANIM_FX,
    POWERANIM_SEQ,
    POWERANIM_TRANSLUCENCY,
    POWERANIM_LOOP,
    POWERANIM_SPAWN,
    POWERANIM_SUST_SEQ_BEGIN,
    POWERANIM_SUST_SEQ_END,
    POWERANIM_COSTUME,
} PowerAnimType;

// Ordered in increasing priority
typedef enum
{
    TARGET_NONE,
    TARGET_LOCATION,
    TARGET_ENT,
    TARGET_CASTER,
} AnimTargetType;

typedef struct
{
    PowerAnimType type;
    F32 time;
    const char* fxName;
    int fxid;
    FxKey fxkey0;
    FxKey fxkey1;
    const int** seqBits;
    Entity* entity;
    float translucency;
    const char* pchCostumeName;
} PowerAnimEvent;

PowerAnimEvent gAnimQueue[MAX_ANIM_STAGES];
static Entity* gTargetEnt;

typedef struct
{
    Entity* entity;
    const int** bits;
    bool active;
} SustainedSequence;

SustainedSequence gSustainedSequences[MAX_SUSTAINED_SEQUENCES];
static int gNumSustainedSequences;

static ToolTip toolTipMatchPowerset;
static ToolTip toolTipMatchAll;

void resetPowerCustMenu()
{
    int i;
    for (i = 0; i < POWERCUSTSET_COUNT; ++i)
    {
        sSelectedPowerSets[i] = NULL;
        sSelectedPowers[i] = NULL;
    }
    gSelectedTintComponent = 1;
    menuInitialized = false;
}

static int s_currentTotal;
int getPowerCustCost()
{
    if (s_outOfDate)
    {
        Entity* e = playerPtr();
        if (e->db_id)
        {
            s_currentTotal = powerCust_GetTotalCost(e, gCopyOfPowerCustList, e->powerCust);
        }
        s_outOfDate = 0;
    }
    return s_currentTotal;
}

static void powerCust_displayPowerCost(const BasePower* power, float x, float y, float z, float sc)
{
    int dummyChange = 1;
    int totalCost = powerCust_CalcPowerCost(playerPtr(), gCopyOfPowerCustList, playerPtr()->powerCust, power, &dummyChange);
    F32 textXSc, textYSc;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculateSpriteScales(&textXSc, &textYSc);
    font(&game_12);
    if (totalCost > 0)
        font_color(CLR_WHITE, CLR_RED);
    else
        font_color(CLR_WHITE, CLR_WHITE);
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    prnt(x, y, z + 10, textXSc * sc, textYSc * sc, "(+%i)", totalCost);
    setSpriteScaleMode(ssm);
}

static void powerCust_displayPowerSetCost(const BasePowerSet* psetBase, float x, float y, float z, float sc)
{
    int dummyChange = 1;
    int totalCost = powerCust_PowerSetCost(playerPtr(), gCopyOfPowerCustList, playerPtr()->powerCust, psetBase, &dummyChange);
    F32 textXSc, textYSc;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculateSpriteScales(&textXSc, &textYSc);
    font(&game_12);
    if (totalCost > 0)
        font_color(CLR_WHITE, CLR_RED);
    else
        font_color(CLR_WHITE, CLR_WHITE);
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    prnt(x, y, z + 10, textXSc * sc, textYSc * sc, "(+%i)", totalCost);
    setSpriteScaleMode(ssm);
}
static PowerSet* getPowerSetByIndex(int index)
{
    if (index < 0 || index >= eaSize(&playerPtr()->pchar->ppPowerSets))
        return NULL;

    return playerPtr()->pchar->ppPowerSets[index];
}

static const BasePower* getBasePowerByIndex(int powerSetIndex, int powerIndex)
{
    PowerSet* powerSet = getPowerSetByIndex(powerSetIndex);

    if (powerSet == NULL || powerIndex < 0 || powerIndex >= eaSize(&powerSet->psetBase->ppPowers))
        return NULL;

    return powerSet->psetBase->ppPowers[powerIndex];
}

static PowerCustomization* getPowerCustomization(const BasePower* power)
{
    // Note that this is dangerous to cache when between menu invocations. Call with -1, -1 to clear the cache.
    Entity* e = playerPtr();

    if (!power || !e)
        return NULL;
    else
        return powerCust_FindPowerCustomization(e->powerCust, power);
}

static const PowerCustomFX* getCustomFX(const BasePower* power)
{
    const PowerCustomization* customization = getPowerCustomization(power);
    int i;

    if (!customization || !customization->token)
        return NULL;

    for (i = 0; i < eaSize(&power->customfx); ++i)
    {
        if (stricmp(power->customfx[i]->pchToken, customization->token) == 0)
            return power->customfx[i];
    }

    return NULL;
}

static const ColorPalette* getPaletteForPower(const BasePower* power)
{

    const CostumeOriginSet* originSet = costume_get_current_origin_set();
    const PowerCustomFX* customFX = getCustomFX(power);

    if (customFX)
    {
        char* paletteName = customFX->paletteName ? customFX->paletteName : "CustomPowerDefault";
        int i;

        for (i = 0; i < eaSize(&originSet->powerColors); ++i)
        {
            if (stricmp(paletteName, originSet->powerColors[i]->name) == 0)
                return originSet->powerColors[i];
        }
    }

    return NULL;
}

static const ColorPalette* getPaletteForSelection()
{
    if (sSelectedPowers[CURRENT_POWER_CUST_MENU])
        return getPaletteForPower(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    else
        return NULL;
}

static int RGBDistanceSquared(Color c1, Color c2)
{
    return ((c1.r - c2.r) * (c1.r - c2.r)) + ((c1.g - c2.g) * (c1.g - c2.g)) + ((c1.b - c2.b) * (c1.b - c2.b)) + ((c1.a - c2.a) * (c1.a - c2.a));
}
static void forceColorToPalette(Color* originalColor, const ColorPalette* palette)
{
    Color bestMatchColor;
    Color c1;
    int i, bestDistance = -1;

    if (!palette)
        return;

    c1 = colorFlip(*originalColor);
    bestMatchColor.integer = c1.integer;

    for (i = 0; i < eaSize((F32***)&palette->color); ++i)
    {
        Color currentColor;
        int currentDistance;
        currentColor.integer = getColorFromVec3(*(palette->color[i]));
        if (bestDistance == -1)
        {
            bestMatchColor.integer = currentColor.integer;
            bestDistance = RGBDistanceSquared(c1, currentColor);
        }
        currentDistance = RGBDistanceSquared(c1, currentColor);
        if (currentDistance < bestDistance)
        {
            bestDistance = currentDistance;
            bestMatchColor.integer = currentColor.integer;
        }
    }

    if (originalColor->integer != colorFlip(bestMatchColor).integer)
    {
        *originalColor = colorFlip(bestMatchColor);
    }
}

static Color getSelectedColor()
{
    ColorPair customTint;
    const PowerCustomization* customization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);

    if (!customization && (gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE))
        customization = playerPtr()->powerCust->powerCustomizations[0];

    customTint = customization ? customization->customTint : colorPairNone;

    if (gSelectedTintComponent == 1)
        return customTint.primary;
    if (gSelectedTintComponent == 2)
        return customTint.secondary;
    return ColorNull;
}

static void randomizePowerCustColors()
{
    Entity* e = playerPtr();
    int i;
    const ColorPalette* palette = costume_get_current_origin_set()->powerColors[0];
    Color newRandomColor1, newRandomColor2;

    newRandomColor1.integer = getColorFromVec3(*(palette->color[rand() % eaSize((F32***)&palette->color)]));
    newRandomColor2.integer = getColorFromVec3(*(palette->color[rand() % eaSize((F32***)&palette->color)]));
    newRandomColor1 = colorFlip(newRandomColor1);
    newRandomColor2 = colorFlip(newRandomColor2);

    for (i = 0; i < eaSize(&e->powerCust->powerCustomizations); ++i)
    {
        PowerCustomization* powerCust = e->powerCust->powerCustomizations[i];
        if (powerCust->customTint.primary.integer == ColorNull.integer)
            powerCust->customTint.primary = newRandomColor1;
        if (powerCust->customTint.secondary.integer == ColorNull.integer)
            powerCust->customTint.secondary = newRandomColor2;
    }
}

static void addTokenIfUnique(const char*** tokenList, const char* token)
{
    int i;
    for (i = 0; i < eaSize(tokenList); ++i)
    {
        if (stricmp((*tokenList)[i], token) == 0)
            return;
    }
    eaPushConst(tokenList, token);
}

// Make sure the power customizations for the entity's costume match the customizable powers in the correct
// order.

void resetCustomPowerCustomization(PowerCustomization* currentCustomization)
{
    int i = -1;
    const char* token = NULL;
    PowerCustomization* originalCustomization;

    if (!currentCustomization)
        return;

    originalCustomization = powerCust_FindPowerCustomization(gCopyOfPowerCustList, currentCustomization->power);

    if (originalCustomization)
    {
        if (powerCust_validate(originalCustomization, playerPtr(), 0))
        {
            StructCopy(ParsePowerCustomization, originalCustomization, currentCustomization, 0, 0);
        }
    }
}

void resetCustomPower(const BasePower* power)
{
    PowerCustomization* currentCustomization = powerCust_FindPowerCustomization(playerPtr()->powerCust, power);
    assert(currentCustomization);
    resetCustomPowerCustomization(currentCustomization);
}
void updateCustomPowerSet(const BasePowerSet* powerSet)
{
    if (powerSet->iCustomToken == -2)
    {
        // "Custom"
    }
    else if (powerSet->iCustomToken == -1)
    {
        // "Default"
        int i;
        for (i = 0; i < eaSize(&powerSet->ppPowers); ++i)
        {
            PowerCustomization* cust = getPowerCustomization(powerSet->ppPowers[i]);
            if (cust)
                cust->token = NULL;
        }
    }
    else
    {
        // Custom Token
        const char* token = powerSet->ppchCustomTokens[powerSet->iCustomToken];
        int i;
        for (i = 0; i < eaSize(&powerSet->ppPowers); ++i)
        {
            const BasePower* power = powerSet->ppPowers[i];
            PowerCustomization* cust = getPowerCustomization(power);
            bool found = false;
            int j;

            if (!cust)
                continue;

            for (j = 0; !found && j < eaSize(&power->customfx); ++j)
            {
                int k;
                PowerCustomFX* customfx = cpp_const_cast(PowerCustomFX*)(power->customfx[j]);

                if (stricmp(token, customfx->pchToken) == 0)
                {
                    cust->token = token;
                    found = true;
                }

                for (k = 0; !found && k < eaSize(&customfx->altThemes); ++k)
                {
                    if (stricmp(token, customfx->altThemes[k]) == 0)
                    {
                        cust->token = customfx->pchToken;
                        found = true;
                    }
                }
            }

            if (found)
            {
                forceColorToPalette(&(cust->customTint.primary), getPaletteForPower(power));
                forceColorToPalette(&(cust->customTint.secondary), getPaletteForPower(power));
            }
            else
            {
                cust->token = NULL;
            }
        }
    }
}

int getThemeMask(const BasePowerSet* powerset, const char* theme)
{
    int i;
    for (i = 0; i < eaSize(&powerset->ppchCustomTokens); ++i)
    {
        if (stricmp(theme, powerset->ppchCustomTokens[i]) == 0)
            return 0x1 << (i + 1);
    }

    return 0x0; // Should never get here.
}

static void resetCustomPowers(Entity* e)
{
    int i;
    int beenInGame = e->db_id ? 1 : 0;
    powerCust_syncPowerCustomizations(e);

    // Within this function, iCustomToken is reinterpreted as a bitmask for the purpose of
    // matching power themes, since a power theme can correspond to multiple powerset themes.
    // It should be converted back to an index before returning.

    for (i = 0; i < eaSize(&e->pchar->ppPowerSets); ++i)
    {
        BasePowerSet* mutable_pset = cpp_const_cast(BasePowerSet*)(e->pchar->ppPowerSets[i]->psetBase);
        mutable_pset->iCustomToken = -1;
    }

    for (i = 0; i < eaSize(&e->powerCust->powerCustomizations); ++i)
    {
        PowerCustomization* cust = e->powerCust->powerCustomizations[i];
        const BasePower* power = cust->power;

        resetCustomPowerCustomization(cust);

        if (isNullOrNone(cust->token))
        {
            BasePowerSet* mutable_pset = cpp_const_cast(BasePowerSet*)(power->psetParent);
            mutable_pset->iCustomToken &= 0x1;
        }
        else
        {
            int j;
            const PowerCustomFX* customfx = NULL;
            BasePowerSet* mutable_pset = cpp_const_cast(BasePowerSet*)(power->psetParent);

            for (j = 0; j < eaSize(&power->customfx); ++j)
            {
                if (stricmp(power->customfx[j]->pchToken, cust->token) == 0)
                {
                    customfx = power->customfx[j];
                    break;
                }
            }

            if (!customfx)
            {
                // Theoretically this should never happen.
                cust->token = NULL;
                mutable_pset->iCustomToken &= 0x1;
            }
            else
            {
                int mask = 0;
                mask |= getThemeMask(power->psetParent, cust->token);

                for (j = 0; j < eaSize(&customfx->altThemes); ++j)
                    mask |= getThemeMask(power->psetParent, customfx->altThemes[j]);

                mutable_pset->iCustomToken &= mask;
            }
        }
    }

    for (i = 0; i < eaSize(&e->pchar->ppPowerSets); ++i)
    {
        // Convert from mask back to index.
        BasePowerSet* powerset = cpp_const_cast(BasePowerSet*)(e->pchar->ppPowerSets[i]->psetBase);

        if (powerset->iCustomToken == 0)
            powerset->iCustomToken = -2;
        else
        {
            int mask = powerset->iCustomToken;
            powerset->iCustomToken = -1;

            while ((mask & 0x1) == 0)
            {
                ++powerset->iCustomToken;
                mask = mask >> 1;
            }
        }
    }
}

static void initializeTokens()
{
    int powerSetIndex;
    Entity* e = playerPtr();

    for (powerSetIndex = 0; powerSetIndex < eaSize(&e->pchar->ppPowerSets); ++powerSetIndex)
    {
        BasePowerSet* powerSet = cpp_const_cast(BasePowerSet*)(e->pchar->ppPowerSets[powerSetIndex]->psetBase);
        int powerIndex;

        eaDestroyConst(&powerSet->ppchCustomTokens);

        for (powerIndex = 0; powerIndex < eaSize(&powerSet->ppPowers); ++powerIndex)
        {
            const BasePower* power = powerSet->ppPowers[powerIndex];
            int i;

            for (i = 0; i < eaSize(&power->customfx); ++i)
                addTokenIfUnique(&powerSet->ppchCustomTokens, power->customfx[i]->pchToken);
        }
    }
}

static int repositioningCamera = 0;
static void initializeMenu()
{
    Entity* e = playerPtr();
    int playerRotation = 0;
    int beenInGame = e->db_id;

    assert(!beenInGame || gCopyOfPowerCustList);

    playerRotation = playerTurn_GetTicks();
    playerTurn(playerRotation - 20);

    repositioningCamera = 1;

    zoomAvatar(true, gZoomedInForPowers ? avatar_getDefaultPosition(0, 0) : ZOOMED_OUT_POSITION);
    resetCustomPowers(e);
    randomizePowerCustColors();
    initializeTokens();
    costume_Apply(playerPtr());
    restartPowerCustomizationAnimation();
    s_outOfDate = 1;
    gPowerColorsLinked = POWERCUST_LINKED_COLORS_NONE;
    menuInitialized = true;
    addToolTip(&toolTipMatchPowerset);
    addToolTip(&toolTipMatchAll);
}

static int drawBulletSquare(float x, float y, float z, float sc, Color color, int selected)
{
    AtlasTex* square = atlasLoadTexture("costume_color_ring_background_square");
    AtlasTex* swirl2 = atlasLoadTexture("costume_color_selected_square");
    AtlasTex* shadow = atlasLoadTexture("costume_color_selected_3d_square");
    CBox box;
    int ret = 0;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    BuildScaledCBox(&box, x - square->width * xposSc * sc / 2.f, y - square->height * yposSc * sc / 2.f, square->width * xposSc * sc,
                    square->height * yposSc * sc);
    if (mouseCollision(&box))
    {
        ret = D_MOUSEOVER;
        sc *= 0.92f;
        if (isDown(MS_LEFT))
        {
            sc *= .95f;
        }
        if (mouseClickHit(&box, MS_LEFT))
        {
            ret = D_MOUSEHIT;
            sndPlay("N_SelectSmall", SOUND_GAME);
        }
    }
    else
    {
        sc *= 0.75f; // asset reduction
    }
    display_sprite_positional(square, x, y, z, sc, sc, colorFlip(color).integer, H_ALIGN_CENTER, V_ALIGN_CENTER);
    if (selected && color.integer != 0)
    {
        display_sprite_positional(shadow, x, y, z, sc, sc, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER);
        display_sprite_positional(swirl2, x, y, z, sc, sc, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER);
    }

    return ret;
}

static float drawPower(HybridElement* hb, const BasePower* power, float x, float y, float z, float scale, int regionOpen, int mode, int* hovered,
                       int powerIndex)
{
    int categories = 1;
    float startY = y;
    float maxHt;
    float sc;
    int bpID = 0;
    BasePower* mutable_power = cpp_const_cast(BasePower*)(power);
    PowerCustomization* customization = getPowerCustomization(power);
    int selected;
    int flags = HB_ROUND_ENDS | HB_NO_HOVER_SCALING | HB_SMALL_FONT;
    int squareInteract;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    maxHt = GSET_HT * categories;

    if (power == sSelectedPowers[CURRENT_POWER_CUST_MENU] && regionOpen && scale > 0)
    {
        mutable_power->ht += TIMESTEP * GSET_SPEED;
        if (mutable_power->ht > maxHt)
            mutable_power->ht = maxHt;

        mutable_power->wd += TIMESTEP * GSET_SPEED;
        if (mutable_power->wd > GSET_MAX_WD)
            mutable_power->wd = GSET_MAX_WD;
    }
    else
    {
        mutable_power->ht -= TIMESTEP * GSET_SPEED;
        if (mutable_power->ht < 0)
            mutable_power->ht = 0;

        mutable_power->wd -= TIMESTEP * GSET_SPEED;
        if (mutable_power->wd < GSET_MIN_WD)
            mutable_power->wd = GSET_MIN_WD;
    }
    selected = (power == sSelectedPowers[CURRENT_POWER_CUST_MENU] ||
                (sSelectedPowers[CURRENT_POWER_CUST_MENU] != NULL && gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE)) &&
               gSelectedTintComponent == 1;
    squareInteract = drawBulletSquare((x + PIX1 + GSET_MAX_WD / 2 + (BULLET_WD / (2))) * xposSc, y * yposSc, z + 2, scale,
                                      (customization && !isNullOrNone(customization->token)) ? customization->customTint.primary : ARGBToColor(0), selected);

    if (D_MOUSEOVER <= squareInteract)
    {
        *hovered = powerIndex * 2;
    }
    if (D_MOUSEHIT == squareInteract)
    {
        gSelectedTintComponent = 1;
        if (power != sSelectedPowers[CURRENT_POWER_CUST_MENU])
        {
            sSelectedPowers[CURRENT_POWER_CUST_MENU] = power;
            restartPowerCustomizationAnimation();
            s_outOfDate = 1;
        }
    }
    selected = (power == sSelectedPowers[CURRENT_POWER_CUST_MENU] ||
                (sSelectedPowers[CURRENT_POWER_CUST_MENU] != NULL && gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE)) &&
               gSelectedTintComponent == 2;
    squareInteract = drawBulletSquare((x + PIX1 + GSET_MAX_WD / 2 + (3 * BULLET_WD / (2))) * xposSc, y * yposSc, z + 2, scale,
                                      (customization && !isNullOrNone(customization->token)) ? customization->customTint.secondary : ARGBToColor(0), selected);
    if (D_MOUSEOVER <= squareInteract)
    {
        *hovered = powerIndex * 2 + 1;
    }
    if (D_MOUSEHIT == squareInteract)
    {
        gSelectedTintComponent = 2;
        if (power != sSelectedPowers[CURRENT_POWER_CUST_MENU])
        {
            sSelectedPowers[CURRENT_POWER_CUST_MENU] = power;
            restartPowerCustomizationAnimation();
            s_outOfDate = 1;
        }
    }

    if (regionOpen && (scale > 0.0f) && (mode == COSTUME_EDIT_MODE_TAILOR))
        powerCust_displayPowerCost(power, (x + (0.3f * GSET_MAX_WD)) * xposSc, (y + 5) * yposSc, z + 5, scale);

    hb->text = power->pchDisplayName;
    if (strlen(textStd(hb->text)) > 25)
    {
        flags |= HB_SCALE_DOWN_FONT;
    }
    hb->icon0 = atlasLoadTexture(power->pchIconName);

    if (D_MOUSEHIT == drawHybridBar(hb, power == sSelectedPowers[CURRENT_POWER_CUST_MENU], (x - 15.f) * xposSc, y * yposSc, z + 2, GSET_MAX_WD + 30.f,
                                    scale * .7, flags, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER))
    {
        if (power == sSelectedPowers[CURRENT_POWER_CUST_MENU])
        {
            sSelectedPowers[CURRENT_POWER_CUST_MENU] = NULL;
        }
        else
        {
            sSelectedPowers[CURRENT_POWER_CUST_MENU] = power;
        }
        restartPowerCustomizationAnimation();
        s_outOfDate = 1;
    }

    y += GSET_HT;

    sc = MIN(1.f, power->ht / GSET_HT);
    if (sc)
    {
        if (selectCustomPower(power, x, y, z, scale * sc * .7, sc * power->wd, mode, 1.f))
        {
            restartPowerCustomizationAnimation();
            s_outOfDate = 1;
        }
        y += GSET_HT;
    }

    return GSET_HT + power->ht;
}

static F32 unrolledScale[MAX_POWERSETS];

static float drawPowerSetDetails(F32 x, F32 y, F32 z, F32 powersetHt, const BasePowerSet* powerSet, int uiIndex, int mode)
{
    static F32 ht[POWERCUSTSET_COUNT][MAX_POWERSETS];
    static HybridElement heSelectedPower;
    static F32 scrollOffset[MAX_POWERSETS];

    float selector_sc = 1.f;
    float scaleFactor = 1.f;
    int powerIndex;
    int geoCount = 0;
    int numberOfPowers = eaSize(&powerSet->ppPowers);
    int maxElements = numberOfPowers + 2; // 2 extra for selection elements
    F32 maxHt = GSET_HT * maxElements;
    UIBox setBox, powersBox;
    CBox box;
    F32 ywalk = y;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    if (sSelectedPowerSets[CURRENT_POWER_CUST_MENU] == powerSet)
    {
        // unroll down
        if (ht[CURRENT_POWER_CUST_MENU][uiIndex] < powersetHt)
        {
            ht[CURRENT_POWER_CUST_MENU][uiIndex] += TIMESTEP * RSET_SPEED;
        }
        else
        {
            ht[CURRENT_POWER_CUST_MENU][uiIndex] = powersetHt;
        }
    }
    else
    {
        // roll up
        if (ht[CURRENT_POWER_CUST_MENU][uiIndex] > 0)
        {
            ht[CURRENT_POWER_CUST_MENU][uiIndex] -= TIMESTEP * RSET_SPEED;
        }
        else
        {
            ht[CURRENT_POWER_CUST_MENU][uiIndex] = 0;
            scrollOffset[uiIndex] = 0;
        }
    }

    // only draw powers if there's something to show
    if (ht[CURRENT_POWER_CUST_MENU][uiIndex] > 0)
    {
        static int lastHoveredSquare = -1;
        int hovered = -1;

        // clip region for entire powerset
        uiBoxDefineScaled(&setBox, x * xposSc, (ywalk - GSET_HT / 2.f) * yposSc, 500.f * xposSc, ht[CURRENT_POWER_CUST_MENU][uiIndex] * yposSc);
        clipperPush(&setBox);

        // power set theme is using same scales and offsets as powers for now
        if (selectPowerSetTheme(cpp_const_cast(BasePowerSet*)(powerSet), REGION_CENTER, ywalk, z + 200, selector_sc * 0.7f * scaleFactor,
                                selector_sc * BS_SELECTOR_WD, 1.f))
        {
            restartPowerCustomizationAnimation();
            s_outOfDate = 1;
        }
        ywalk += GSET_HT;

        // clip region for just powers (excluding theme selector)
        uiBoxDefineScaled(&powersBox, x * xposSc, (ywalk - GSET_HT / 2.f) * yposSc, 500.f * xposSc,
                          MAX(0, MIN(ht[CURRENT_POWER_CUST_MENU][uiIndex] - GSET_HT * 1.5f, powersetHt - GSET_HT)) * yposSc);
        if (maxHt > powersetHt - GSET_HT * 0.5f)
        {
            BuildScaledCBox(&box, x * xposSc, (ywalk - GSET_HT / 2.f) * yposSc, ((REGION_CENTER - x) + GSET_MAX_WD / 2.f + 65.f) * xposSc,
                            (ht[CURRENT_POWER_CUST_MENU][uiIndex] - GSET_HT * 1.5f) * yposSc);
            drawHybridScrollBar((REGION_CENTER + GSET_MAX_WD / 2.f + 40.f) * xposSc, (ywalk - GSET_HT / 2.f) * yposSc, z, powersetHt - GSET_HT * 1.5f,
                                &scrollOffset[uiIndex], maxHt - (powersetHt - GSET_HT * 0.5f), &box);
            ywalk -= scrollOffset[uiIndex];
        }
        clipperPush(&powersBox);
        for (powerIndex = 0; powerIndex < numberOfPowers; powerIndex++)
        {
            ywalk += drawPower(&heSelectedPower, powerSet->ppPowers[powerIndex], REGION_CENTER, ywalk, z, scaleFactor,
                               sSelectedPowerSets[CURRENT_POWER_CUST_MENU] == powerSet, mode, &hovered, powerIndex);
        }
        clipperPop();
        clipperPop();

        if (lastHoveredSquare != hovered)
        {
            lastHoveredSquare = hovered;
            if (hovered > -1)
            {
                // new hovered color, play sound
                sndPlay("N_PopHover", SOUND_GAME);
            }
        }
    }
    if (((mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN)) && (geoCount == 0) && (ht[CURRENT_POWER_CUST_MENU][uiIndex] == 0))
    {
        return -1;
    }
    unrolledScale[uiIndex] = ht[CURRENT_POWER_CUST_MENU][uiIndex] / powersetHt;
    return ht[CURRENT_POWER_CUST_MENU][uiIndex];
}

static void drawPowerSets(CostumeEditMode mode)
{
    static HybridElement hePowerSetBar[POWERCUSTSET_COUNT][MAX_POWERSETS] = {
        {{0, 0, 0, "icon_power_primary_0", "icon_power_primary_1"}, {0, 0, 0, "icon_power_secondary_0", "icon_power_secondary_1"}}};
    F32 y = REGION_Y;
    F32 z = ZOOM_Z;
    Entity* e = playerPtr();
    int powersetsShowing;
    int iShowing, iCustomCategory, categoryIndex, iPowerSet;
    int numShowing = 0;
    int numCustomCategories = eaSize(&gPowerCustomizationMenu.pCategories);
    int numPowerSets = eaSize(&e->pchar->ppPowerSets);
    const PowerCategory* pool = powerdict_GetCategoryByName(&g_PowerDictionary, "Pool");
    const PowerCategory* epic = powerdict_GetCategoryByName(&g_PowerDictionary, "Epic");

    F32 powersetHt;
    static F32 powersetSc[POWERCUSTSET_COUNT];
    F32 barSc = 1.f;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    if (!hePowerSetBar[CURRENT_POWER_CUST_MENU][0].icon0)
    {
        int i;
        for (i = 0; i < MAX_POWERSETS; ++i)
        {
            hybridElementInit(&hePowerSetBar[CURRENT_POWER_CUST_MENU][i]);
        }
    }

    // go through powersets and pick out the ones we will customize
    for (iPowerSet = 0; iPowerSet < numPowerSets && (numShowing < MAX_POWERSETS); ++iPowerSet)
    {
        const BasePowerSet* powerSet = getPowerSetByIndex(iPowerSet)->psetBase;

        switch (current_menu())
        {
            case MENU_POWER_CUST_INCARNATE:
                for (iCustomCategory = 0; iCustomCategory < numCustomCategories; ++iCustomCategory)
                {
                    PowerCustomizationMenuCategory* menuCategory = gPowerCustomizationMenu.pCategories[iCustomCategory];
                    const PowerCategory* allowedCategory = powerdict_GetCategoryByName(&g_PowerDictionary, menuCategory->categoryName);
                    if (powerSet->pcatParent == allowedCategory)
                    {
                        if (!chareval_requires(e->pchar, menuCategory->hideIf, "Defs/powercustomizationmenu.def"))
                        {
                            int i;
                            for (i = 0; i < eaSize(&menuCategory->powerSets); ++i)
                            {
                                if (stricmp(powerSet->pchName, menuCategory->powerSets[i]) == 0)
                                {
                                    hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].index = iPowerSet;
                                    hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].text = powerSet->pchDisplayName;
                                    ++numShowing;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case MENU_POWER_CUST_POWERPOOL:
            {
                // add all owned epic and pool powersets
                if (powerSet->pcatParent == pool || powerSet->pcatParent == epic)
                {
                    hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].index = iPowerSet;
                    hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].text = powerSet->pchDisplayName;
                    ++numShowing;
                }
            }
            break;
            case MENU_POWER_CUST:
            default:
                // find primary, then secondary
                for (categoryIndex = 0; categoryIndex < NUM_CATEGORIES; ++categoryIndex)
                {
                    if (powerSet->pcatParent == e->pchar->pclass->pcat[CATEGORIES[categoryIndex]])
                    {
                        hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].index = iPowerSet;
                        hePowerSetBar[CURRENT_POWER_CUST_MENU][numShowing].text = powerSet->pchDisplayName;
                        ++numShowing;
                        break;
                    }
                }
        }
    }

    powersetsShowing = MIN(numShowing, MAX_POWERSETS);
    powersetSc[CURRENT_POWER_CUST_MENU] = stateScaleSpeed(powersetSc[CURRENT_POWER_CUST_MENU], 0.1f, sSelectedPowerSets[CURRENT_POWER_CUST_MENU] != NULL);
    if (powersetsShowing > 2)
    {
        // scale based on how many powersets are showing
        //(this is some magic number scale; will have to be adjusted if there's a lot of powers)
        F32 diff = MAX(0.f, (powersetsShowing - 2.f) * 0.35f / (MAX_POWERSETS - 2.f));
        barSc = 1.f - powersetSc[CURRENT_POWER_CUST_MENU] * diff;
    }
    powersetHt = AREA_HT - powersetsShowing * POWERSET_BUTTON_HT * barSc; // calculate the maximum height of one powerset subsection
    for (iShowing = 0; iShowing < numShowing; ++iShowing)
    {
        const BasePowerSet* thisPowerSet;
        int powerSetIndex = hePowerSetBar[CURRENT_POWER_CUST_MENU][iShowing].index;
        thisPowerSet = getPowerSetByIndex(powerSetIndex)->psetBase;
        if (mode == COSTUME_EDIT_MODE_TAILOR)
            powerCust_displayPowerSetCost(thisPowerSet, (REGION_CENTER + REGION_WD / 6.f) * xposSc, (y + 5.f) * yposSc, z + 5.f, 1.f);

        if (D_MOUSEHIT == drawHybridBar(&hePowerSetBar[CURRENT_POWER_CUST_MENU][iShowing], sSelectedPowerSets[CURRENT_POWER_CUST_MENU] == thisPowerSet,
                                        REGION_X * xposSc, y * yposSc, z, 354.f, 0.8f * barSc, HB_TAIL_RIGHT_ROUND, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER))
        {
            restartPowerCustomizationAnimation();
            sSelectedPowers[CURRENT_POWER_CUST_MENU] = NULL;
            if (sSelectedPowerSets[CURRENT_POWER_CUST_MENU] != thisPowerSet)
            {
                sSelectedPowerSets[CURRENT_POWER_CUST_MENU] = thisPowerSet;
                sndPlay("N_MenuExpand", SOUND_UI_ALTERNATE);
            }
            else
            {
                sSelectedPowerSets[CURRENT_POWER_CUST_MENU] = NULL;
            }
            s_outOfDate = 1;
        }
        drawSectionArrow((REGION_X + 330.f * (0.9f + 0.1f * hePowerSetBar[CURRENT_POWER_CUST_MENU][iShowing].percent)) * xposSc, y * yposSc, z, barSc,
                         hePowerSetBar[CURRENT_POWER_CUST_MENU][iShowing].percent, 1.f - unrolledScale[iShowing]);

        y += POWERSET_BUTTON_HT * barSc;
        y += drawPowerSetDetails(0.f, y, z, powersetHt, thisPowerSet, iShowing, mode);
    }
}

static int drawHideMenuSelector(float x, float y)
{
    static int s_hideMenus = 0;
    static HybridElement sButtonHide = {0, NULL, NULL, "icon_power_hidemenu_0"};
    sButtonHide.text = s_hideMenus ? "ShowUIString" : "HideUIString";
    sButtonHide.desc1 = s_hideMenus ? "showMenuButton" : "hideMenuButton";

    if (D_MOUSEHIT == drawHybridButton(&sButtonHide, x, y, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING))
    {
        s_hideMenus = !s_hideMenus;
    }

    return s_hideMenus;
}

static bool drawPowerCustLinkSelector(float x, float y, float scale, int* variable, F32 alphaScale)
{
    char* colorLinkTextureName = (*variable == POWERCUST_LINKED_COLORS_POWERSET) ? "icon_costume_colorbody_linked" : "icon_costume_colorbody";
    int alpha = 0xffffff00 | (int)(255 * alphaScale);
    bool modeSwitch = false;
    static HybridElement sButtonLinkPS = {0, NULL, "linkPowersetColorButton", NULL};
    static HybridElement sButtonLinkAP = {0, NULL, "linkPowersColorButton", NULL};
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    sButtonLinkPS.iconName0 = colorLinkTextureName;

    if (D_MOUSEHIT == drawHybridButton(&sButtonLinkPS, x, y, ZOOM_Z, 0.6f * scale, CLR_WHITE & alpha, HB_DRAW_BACKING))
    {
        if (*variable == POWERCUST_LINKED_COLORS_POWERSET)
            *variable = POWERCUST_LINKED_COLORS_NONE;
        else
        {
            if (*variable == POWERCUST_LINKED_COLORS_NONE) // only when we are switching from none do we need to refresh
            {
                modeSwitch = true;
            }
            *variable = POWERCUST_LINKED_COLORS_POWERSET;
        }
    }
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&hybridbold_12);
    font_color(CLR_WHITE & alpha, CLR_WHITE & alpha);
    prnt(x + 20.f * xposSc, y + 5.f * yposSc, ZOOM_Z, textXSc, textYSc * scale, "MatchColorsAcrossPowerset");
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    colorLinkTextureName = (*variable == POWERCUST_LINKED_COLORS_ALL) ? "icon_costume_colorbody_linked" : "icon_costume_colorbody";
    sButtonLinkAP.iconName0 = colorLinkTextureName;
    if (D_MOUSEHIT == drawHybridButton(&sButtonLinkAP, x, y + 33.f * yposSc, ZOOM_Z, 0.6f * scale, CLR_WHITE & alpha, HB_DRAW_BACKING))
    {
        if (*variable == POWERCUST_LINKED_COLORS_ALL)
            *variable = POWERCUST_LINKED_COLORS_NONE;
        else
        {
            modeSwitch = true; // always need to refresh when switching to all
            *variable = POWERCUST_LINKED_COLORS_ALL;
        }
    }
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    prnt(x + 20.f * xposSc, y + 38.f * yposSc, ZOOM_Z, textXSc, textYSc * scale, "MatchColorsAcrossAllPowers");
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);

    return modeSwitch;
}

static void drawColors()
{
    F32 y = 95.f;
    F32 z = 5000.f;
    F32 x = 420.f;
    F32 wd = 253.f;
    F32 ht = 458.f;
    PowerCustomization* customization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    ColorPair* customTint = customization ? &customization->customTint : &colorPairNone;
    ColorPair colorNullCopy = colorPairNone;
    int color1;
    int color2;
    static bool color_selected = true;
    Color newColor;
    const ColorPalette* palette = getPaletteForSelection();
    static HybridElement hb;
    static F32 alphaScale;
    int alpha;
    bool modeSwitch;
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    if (customization)
        customTint = &customization->customTint;
    else if ((gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE))
        customTint = &playerPtr()->powerCust->powerCustomizations[0]->customTint;
    else
        customTint = &colorNullCopy;

    forceColorToPalette(&(customTint->primary), palette);
    forceColorToPalette(&(customTint->secondary), palette);
    color1 = colorFlip(customTint->primary).integer;
    color2 = colorFlip(customTint->secondary).integer;

    if ((sSelectedPowers[CURRENT_POWER_CUST_MENU] && palette))
    {
        alphaScale = stateScale(alphaScale, 1);
    }
    else
    {
        alphaScale = stateScale(alphaScale, 0);
    }
    drawHybridDescFrame(x * xposSc, y * yposSc, z, wd, ht, HB_DESCFRAME_FADEDOWN, 0.25f, alphaScale, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);
    alpha = 0xffffff00 | (int)(255.f * alphaScale);
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_12);
    font_outl(0);
    font_color(CLR_BLACK & alpha, CLR_BLACK & alpha);
    prnt((x + 15.f) * xposSc + 2.f, (y + 24.f) * yposSc + 2.f, z, textXSc, textYSc, "ColorHeading");
    font_color(CLR_WHITE & alpha, CLR_WHITE & alpha);
    prnt((x + 15.f) * xposSc, (y + 24.f) * yposSc, z, textXSc, textYSc, "ColorHeading");
    font_outl(1);
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    modeSwitch = drawPowerCustLinkSelector((x + 20.f) * xposSc, (y + ht + 10.f) * yposSc, 1.f, &gPowerColorsLinked, alphaScale);

    newColor =
        drawColorPalette((x + 135.f) * xposSc, (y + 45.f) * yposSc, z, 1.f, palette, getSelectedColor(), getSelectedColor().integer != ColorNull.integer);
    if (newColor.integer != ColorNull.integer)
    {
        if (!customization && (gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE))
            customization = playerPtr()->powerCust->powerCustomizations[0];

        if (customization)
        {
            if (gSelectedTintComponent == 1)
                customization->customTint.primary = newColor;
            else if (gSelectedTintComponent == 2)
                customization->customTint.secondary = newColor;
            restartPowerCustomizationAnimation();
            s_outOfDate = 1;
        }
    }

    if (newColor.integer != ColorNull.integer || modeSwitch)
    {
        if ((gPowerColorsLinked != POWERCUST_LINKED_COLORS_NONE) && customization && customization->token)
        {
            Entity* e = playerPtr();
            int i;
            ColorPair linkedColors;

            if (!customization)
                customization = e->powerCust->powerCustomizations[0];

            linkedColors = customization->customTint;

            for (i = eaSize(&e->powerCust->powerCustomizations) - 1; i >= 0; --i)
            {
                int doApply = 0;
                ColorPair currentApplyColor = linkedColors;
                const BasePower* power = e->powerCust->powerCustomizations[i]->power;

                if (power)
                {
                    forceColorToPalette(&(currentApplyColor.primary), getPaletteForPower(power));
                    forceColorToPalette(&(currentApplyColor.secondary), getPaletteForPower(power));

                    if (gPowerColorsLinked == POWERCUST_LINKED_COLORS_ALL ||
                        gPowerColorsLinked == POWERCUST_LINKED_COLORS_POWERSET && sSelectedPowers[CURRENT_POWER_CUST_MENU]->psetParent == power->psetParent)
                    {
                        e->powerCust->powerCustomizations[i]->customTint = currentApplyColor;
                    }
                }
            }
        }
    }
}

const PowerFX* getSelectedPowerFX(const BasePower* power)
{
    int i;
    PowerCustomization* cust;

    if (!power || !sSelectedPowers[CURRENT_POWER_CUST_MENU])
        return NULL;

    cust = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);

    if (!cust || isNullOrNone(cust->token))
        return &power->fx;

    for (i = 0; i < eaSize(&power->customfx); ++i)
    {
        int j;
        if (stricmp(cust->token, power->customfx[i]->pchToken) == 0)
            return &power->customfx[i]->fx;

        for (j = 0; j < eaSize(&power->customfx[i]->altThemes); ++j)
        {
            if (stricmp(cust->token, power->customfx[i]->altThemes[j]) == 0)
                return &power->customfx[i]->fx;
        }
    }
    return NULL;
}

static F32 getAttackTime(const BasePower* power)
{
    F32 result;
    const PowerFX* fx = getSelectedPowerFX(power);

    if (!power)
        return 0.0f;

    result = power->fInterruptTime * 30.0f;

    return result;
}

static F32 getHitTime(const BasePower* power, F32 dist)
{
    const PowerFX* fx = getSelectedPowerFX(power);
    F32 result = power ? power->fInterruptTime * 30.0f : 0.0f;

    if (fx)
    {
        if (fx->piInitialAttackBits)
            result += fx->iInitialFramesBeforeHit;
        else
            result += fx->iFramesBeforeHit;

        if (fx->bDelayedHit && fx->fProjectileSpeed != 0.0f)
            result += 30.0f * dist / fx->fProjectileSpeed;
    }

    return result;
}

void setSequence(Entity* entity, const U32** sequenceBits)
{
    int i;

    for (i = 0; i < eaiSize(sequenceBits); ++i)
    {
        if ((*sequenceBits)[i] >= 0)
        {
            seqSetState(entity->seq->state, TRUE, (*sequenceBits)[i]);
        }
    }
}

static int playFX(const char* fxName, FxKey input0, FxKey input1)
{
    Entity* e = playerPtrForShell(1);
    PowerCustomization* customization;
    FxParams fxp;

    if (!sSelectedPowers[CURRENT_POWER_CUST_MENU])
        return 0;

    customization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    fxInitFxParams(&fxp);

    if (e->seq)
    {
        fxp.keys[fxp.keycount++] = input0;
        fxp.keys[fxp.keycount++] = input1;
    }

    if (customization && customization->token != NULL || sSelectedPowers[CURRENT_POWER_CUST_MENU]->fx.defaultTint.primary.a != 0 ||
        sSelectedPowers[CURRENT_POWER_CUST_MENU]->fx.defaultTint.secondary.a != 0)
    {
        int i;
        ColorPair tints =
            (customization && customization->token != NULL) ? customization->customTint : sSelectedPowers[CURRENT_POWER_CUST_MENU]->fx.defaultTint;
        fxp.numColors = 2;

        for (i = 0; i < 4; ++i)
        {
            fxp.colors[0][i] = tints.primary.rgba[i];
            fxp.colors[1][i] = tints.secondary.rgba[i];
        }

        fxp.isUserColored = 1;
    }

    fxp.radius = 10;
    fxp.power = 10;
    fxp.isPlayer = true;
    fxp.isPersonal = true;

    return fxCreate(fxName, &fxp);
}

static void transformToEnt(Entity* e, Mat4* mat)
{
    Mat4 offset;
    copyMat4(*mat, offset);
    mulMat4(ENTMAT(e), offset, *mat);
}

static void spawnPet(Entity* entity)
{
    // Assume that all pets are pets of the player. If pets ever spawn other pets this will need to be revisited.
    transformToEnt(playerPtrForShell(1), &entity->fork);
    costume_Apply(entity);
    copyMat4(ENTMAT(entity), entity->seq->gfx_root->mat);
    animSetHeader(entity->seq, 0);
    SETB(entity->seq->state, STATE_SPAWNEDASPET);
    entity->seq->seqGfxData.alpha = 255;
    manageEntityHitPointTriggeredfx(entity);
    SET_ENTHIDE(entity) = 0;
    entity->showInMenu = 1;
    entity->currfade = 255;
}

static void setAlpha(Entity* entity, float translucency)
{
    entity->xlucency = translucency;
}

static void setCostume(Entity* e, const char* pchCostumeName)
{
    int index;
    const NPCDef* npc = npcFindByName(pchCostumeName, &index);
    const cCostume* npcCostume = npcDefsGetCostume(index, 0);

    if (e && npcCostume)
    {
        if (e->costume != gOwnedCostume && e == playerPtrForShell(0))
            gOwnedCostume = e->costume;

        e->costume = npcCostume;
        e->npcIndex = index;
        costume_Apply(e);
    }
}

static void revertPlayerCostume(Entity* e)
{
    if (e && gOwnedCostume)
    {
        e->costume = gOwnedCostume;
        gOwnedCostume = NULL;
        e->npcIndex = 0;
        costume_Apply(e);
    }
}

static void beginSustainedSequence(PowerAnimEvent* event)
{
    gSustainedSequences[gNumSustainedSequences].entity = event->entity;
    gSustainedSequences[gNumSustainedSequences].bits = event->seqBits;
    gSustainedSequences[gNumSustainedSequences].active = true;
    ++gNumSustainedSequences;
}

static void endSustainedSequence(PowerAnimEvent* event)
{
    int i;

    for (i = 0; i < gNumSustainedSequences; ++i)
    {
        if (gSustainedSequences[i].bits == event->seqBits)
        {
            gSustainedSequences[i].active = false;
            break;
        }
    }
}

static void processAnimEvent(PowerAnimEvent* event)
{
    switch (event->type)
    {
        case POWERANIM_SEQ:
            setSequence(event->entity, event->seqBits);
            break;
        case POWERANIM_FX:
            if (event->fxkey0.seq == NULL)
                transformToEnt(event->entity, &event->fxkey0.offset);
            if (event->fxkey1.seq == NULL)
                transformToEnt(event->entity, &event->fxkey1.offset);
            event->fxid = playFX(event->fxName, event->fxkey0, event->fxkey1);
            break;
        case POWERANIM_SPAWN:
            spawnPet(event->entity);
            break;
        case POWERANIM_TRANSLUCENCY:
            setAlpha(event->entity, event->translucency);
            break;
        case POWERANIM_LOOP:
            gLoopNow = true;
            break;
        case POWERANIM_SUST_SEQ_BEGIN:
            beginSustainedSequence(event);
            break;
        case POWERANIM_SUST_SEQ_END:
            endSustainedSequence(event);
            break;
        case POWERANIM_COSTUME:
            setCostume(event->entity, event->pchCostumeName);
            break;
    }
}

void printSequence(const char* text)
{
    static char buffer[5][128];
    static F32 bufferAge[5];
    int i;

    if (strcmp(text, buffer[0]) != 0)
    {
        for (i = 4; i > 0; --i)
        {
            strcpy(buffer[i], buffer[i - 1]);
            bufferAge[i] = bufferAge[i - 1];
        }

        strcpy(buffer[0], text);
    }

    bufferAge[0] = 0.0f;

    for (i = 1; i < 5; ++i)
        bufferAge[i] += TIMESTEP;

    for (i = 0; i < 5; ++i)
    {
        U8 red = 255 * exp(-0.01 * bufferAge[i]);
        U8 green = 255 * exp(-0.03 * bufferAge[i]);
        U8 blue = 255 * exp(-0.1 * bufferAge[i]);
        xyprintfcolor(20, 70 - i, red, green, blue, buffer[i]);
    }
}

void powerCust_handleAnimation()
{
    const PowerFX* fx = getSelectedPowerFX(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    Entity* e = playerPtrForShell(1);
    FxKey playerRoot = {0};
    FxKey playerChest = {0};
    FxKey target = {0};
    int i;

    if (gLoopNow)
    {
        gLoopNow = false;
        restartPowerCustomizationAnimation();
    }

    seqClearState(e->seq->state);

    if (gTargetEnt)
        seqClearState(gTargetEnt->seq->state);

    if (!fx)
    {
        // freeze the player model
        seqSetState(e->seq->state, TRUE, STATE_PROFILE);
        seqSetState(e->seq->state, TRUE, STATE_HIGH);
    }
    else
    {
        if (gAnimStage == 0.0f)
            seqOrState(e->seq->state, 1, STATE_STARTOFFENSESTANCE);

        for (i = 0; i < gNumSustainedSequences; ++i)
        {
            SustainedSequence* sustSeq = gSustainedSequences + i;
            if (sustSeq->active)
                setSequence(sustSeq->entity, sustSeq->bits);
        }

        while (gAnimStage < MAX_ANIM_STAGES && gAnimationProgress >= gAnimQueue[gAnimStage].time)
            processAnimEvent(&gAnimQueue[gAnimStage++]);

        gAnimationProgress += TIMESTEP;
    }

    if (game_state.debugPowerCustAnim)
        printSequence(seqGetStateString(e->seq->state));
}

static void insertAnimEvent(PowerAnimEvent* event)
{
    while (event > gAnimQueue && (event - 1)->time > event->time)
    {
        PowerAnimEvent swapTemp = *event;
        *event = *(event - 1);
        *(event - 1) = swapTemp;
        --event;
    }
}

static Entity* createPowerCustEntity(const char* costumeName)
{
    int npcIndex;
    // const NPCDef* npcDef = npcFindByName(costumeName, &npcIndex);
    const NPCDef* npcDef = (stricmp(costumeName, "DOPPLEGANGER") != 0) ? npcFindByName(costumeName, &npcIndex) : NULL;
    Entity* result = entCreate("");
    result->fade = 0;
    result->showInMenu = true;
    // result->costume = npcDef->costumes[0];
    result->costume = (npcDef) ? npcDef->costumes[0] : entGetConstCostume(playerPtr());
    // entSetSeq(result, seqLoadInst(npcDef->costumes[0]->appearance.entTypeFile, 0, SEQ_LOAD_FULL_SHARED, result->randSeed, result));
    if (npcDef)
    {
        entSetSeq(result, seqLoadInst(npcDef->costumes[0]->appearance.entTypeFile, 0, SEQ_LOAD_FULL_SHARED, result->randSeed, result));
    }
    else
    {
        entSetSeq(result, seqLoadInst(entTypeFileName(/*playerPtr()*/ playerPtrForShell(1)), 0, SEQ_LOAD_FULL_SHARED, result->randSeed, result));
    }

    return result;
}

static AnimTargetType mapToTargetType(TargetType type)
{
    switch (type)
    {
        case kTargetType_Caster:
            return TARGET_CASTER;

        case kTargetType_Player:
        case kTargetType_PlayerHero:
        case kTargetType_PlayerVillain:
        case kTargetType_Teammate:
        case kTargetType_Villain:
        case kTargetType_NPC:
        case kTargetType_DeadOrAliveFriend:
        case kTargetType_Friend:
        case kTargetType_DeadOrAliveFoe:
        case kTargetType_Foe:
        case kTargetType_Any:
        case kTargetType_DeadOrAliveTeammate:
        case kTargetType_DeadOrAliveMyPet:
        case kTargetType_DeadOrAliveMyCreation:
        case kTargetType_MyPet:
        case kTargetType_MyCreation:
        case kTargetType_MyOwner:
        case kTargetType_MyCreator:
        case kTargetType_DeadPlayer:
        case kTargetType_DeadPlayerFriend:
        case kTargetType_DeadPlayerFoe:
        case kTargetType_DeadVillain:
        case kTargetType_DeadFriend:
        case kTargetType_DeadFoe:
        case kTargetType_DeadTeammate:
        case kTargetType_DeadMyPet:
        case kTargetType_DeadMyCreation:
        case kTargetType_Leaguemate:
        case kTargetType_DeadLeaguemate:
        case kTargetType_DeadOrAliveLeaguemate:
            return TARGET_ENT;

        case kTargetType_Location:
        case kTargetType_Teleport:
        case kTargetType_Position: // ARM NOTE:  I think this should be here...?
            return TARGET_LOCATION;

        case kTargetType_None:
        default:
            return TARGET_NONE;
    }
}

static AnimTargetType getTargetType(const BasePower* power)
{
    return mapToTargetType(power->eTargetType);
}

static F32 calcSelectedPowerRange()
{
    F32 range = sSelectedPowers[CURRENT_POWER_CUST_MENU] ? sSelectedPowers[CURRENT_POWER_CUST_MENU]->fRange : 15.0f;
    return MINMAX(range, 2.0f, 15.0f);
}

static void ensureTargetEnt(F32* offset)
{
    Vec3 defaultOffset = {0};

    if (!offset)
        defaultOffset[2] = calcSelectedPowerRange();

    if (!gTargetEnt)
    {
        gTargetEnt = createPowerCustEntity("Invisible_Male");
        gTargetEnt->fork[0][0] = -1;
        gTargetEnt->fork[2][2] = -1;
        copyVec3((offset ? offset : defaultOffset), gTargetEnt->fork[3]);
        spawnPet(gTargetEnt);
    }
}

static PowerAnimEvent* createPrimaryAnimationEvents(PowerAnimEvent* event, Entity* caster, const BasePower* power, F32 timeOffset)
{
    const PowerFX* fx = &power->fx;

    // May not be the power passed in as a parameter.
    PowerCustomization* selectedCustomization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    Vec3 targetOffset = {0, 0, 8};
    FxKey target = {0};
    bool targetCaster = false;

    if (selectedCustomization && !isNullOrNone(selectedCustomization->token))
    {
        int i;
        for (i = 0; i < eaSize(&power->customfx); ++i)
        {
            if (stricmp(selectedCustomization->token, power->customfx[i]->pchToken) == 0)
            {
                fx = &power->customfx[i]->fx;
                break;
            }
        }
    }

    targetOffset[2] = calcSelectedPowerRange();

    switch (getTargetType(power))
    {
        case TARGET_NONE:
            // TBD
            break;

        case TARGET_CASTER:
            target.seq = caster->seq;
            targetCaster = true;
            break;

        case TARGET_ENT:
            ensureTargetEnt(targetOffset);
            target.seq = gTargetEnt->seq;
            break;

        case TARGET_LOCATION:
            copyMat3(unitmat, target.offset);
            copyVec3(targetOffset, target.offset[3]);
            break;
    }

    if (!gTargetEnt)
        ensureTargetEnt(NULL);

    if (fx->piModeBits)
    {
        event->type = POWERANIM_SUST_SEQ_BEGIN;
        event->entity = caster;
        event->seqBits = cpp_const_cast(const int**)(&fx->piModeBits);
        event->time = timeOffset;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->piActivationBits && (fx->piWindUpBits || power->eType == kPowerType_Toggle))
    {
        event->type = POWERANIM_SEQ;
        event->seqBits = cpp_const_cast(const int**)(&fx->piActivationBits);
        event->time = timeOffset;
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->pchActivationFX)
    {
        event->type = POWERANIM_FX;
        event->fxName = fx->pchActivationFX;
        event->fxkey0.seq = caster->seq;
        event->fxkey1.seq = caster->seq;
        event->fxkey1.bone = 2;
        event->entity = caster;
        event->time = timeOffset;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->piWindUpBits)
    {
        event->type = POWERANIM_SUST_SEQ_BEGIN;
        event->entity = caster;
        event->seqBits = cpp_const_cast(const int**)(&fx->piWindUpBits);
        event->time = timeOffset;
        insertAnimEvent(event);
        ++event;
        event->type = POWERANIM_SUST_SEQ_END;
        event->entity = caster;
        event->seqBits = cpp_const_cast(const int**)(&fx->piWindUpBits);
        event->time = timeOffset + power->fInterruptTime * 30.0f;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->pchWindUpFX)
    {
        event->type = POWERANIM_FX;
        event->fxName = fx->pchWindUpFX;
        event->fxkey0.seq = caster->seq;
        event->fxkey1.seq = caster->seq;
        event->fxkey1.bone = 2;
        event->entity = caster;
        event->time = timeOffset;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->piInitialAttackBits)
    {
        event->type = POWERANIM_SEQ;
        event->seqBits = cpp_const_cast(const int**)(&fx->piInitialAttackBits);
        event->time = timeOffset + getAttackTime(power);
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }
    else if (fx->piAttackBits)
    {
        event->type = POWERANIM_SEQ;
        event->seqBits = cpp_const_cast(const int**)(&fx->piAttackBits);
        event->time = timeOffset + getAttackTime(power);
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->pchInitialAttackFX)
    {
        event->type = POWERANIM_FX;
        event->fxName = fx->pchInitialAttackFX;
        event->fxkey0.seq = caster->seq;
        event->fxkey1 = target;
        // Delay transforming the target key until the fx is actually played,
        // since the avatar may move before then.
        event->time = timeOffset + getAttackTime(power) + fx->iInitialAttackFXFrameDelay;
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }
    else if (fx->pchAttackFX)
    {
        event->type = POWERANIM_FX;
        event->fxName = fx->pchAttackFX;
        event->fxkey0.seq = caster->seq;
        event->fxkey1 = target;
        // Delay transforming the target key until the fx is actually played,
        // since the avatar may move before then.
        event->time = timeOffset + getAttackTime(power) + fx->iInitialAttackFXFrameDelay;
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->piHitBits && (gTargetEnt || targetCaster))
    {
        event->type = POWERANIM_SEQ;
        event->seqBits = cpp_const_cast(const int**)(&fx->piHitBits);
        event->time = timeOffset + getHitTime(power, targetOffset[2]);
        event->entity = targetCaster ? caster : gTargetEnt;
        insertAnimEvent(event);
        ++event;
    }

    if (fx->pchHitFX)
    {
        event->type = POWERANIM_FX;
        event->fxName = fx->pchHitFX;
        event->fxkey0 = target;
        event->fxkey1.seq = caster->seq;
        // Delay transforming the target key until the fx is actually played,
        // since the avatar may move before then.
        event->time = timeOffset + getHitTime(power, targetOffset[2]);
        event->entity = caster;
        insertAnimEvent(event);
        ++event;
    }

    return event;
}

static void setPetCostumeColors(Entity* pPet, const AttribModTemplate* mod)
{
    int i;
    PowerCustomization* customization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);

    if (!customization || mod->bDoNotTintCostume || isNullOrNone(customization->token) || customization->customTint.primary.a == 0 ||
        customization->customTint.secondary.a == 0)
    {
        return;
    }

    {
        Costume* mutable_costume = entGetMutableCostume(pPet);
        mutable_costume->appearance.colorSkin.integer = customization->customTint.primary.integer;
        for (i = 0; i < mutable_costume->appearance.iNumParts; ++i)
        {
            mutable_costume->parts[i]->color[0].integer = customization->customTint.primary.integer;
            mutable_costume->parts[i]->color[1].integer = customization->customTint.secondary.integer;
        }
    }

    pPet->ownedPowerCustomizationList = powerCustList_clone(pPet->powerCust);
}

static PowerAnimEvent* createPowerAnimationEvents(PowerAnimEvent* event, Entity* caster, const BasePower* power, F32 timeOffset);

static PowerAnimEvent* createSpawnAnimationEvents(PowerAnimEvent* event, Entity* caster, const AttribModTemplate* mod, int spawnIndex, F32 timeOffset)
{
    const VillainDef* def = (strstriConst(mod->pchEntityDef, DOPPEL_NAME))
                                ? villainFindByName("CustomCritter_Dopplepet")
                                : villainFindByName(mod->pchEntityDef); // if the entity is a doppleganger, use the CustomCritter_Dopplepet villaindef
    const VillainLevelDef* levelDef = GetVillainLevelDef(def, 1);
    const char* costumeName = levelDef->costumes[0];
    static int* spawnBits;
    F32 spawnTime = timeOffset;
    int i;
    //Entity* pet = createPowerCustEntity(costumeName);
    Entity* pet = (strstriConst(mod->pchEntityDef, DOPPEL_NAME)) ? createPowerCustEntity("DOPPLEGANGER") : createPowerCustEntity(costumeName);

    if (!spawnBits)
        eaiPush(&spawnBits, STATE_SPAWNEDASPET);

    event->type = POWERANIM_SPAWN;
    event->time = spawnTime;
    event->entity = pet;
    copyVec3(gSpawnOffsets[spawnIndex], pet->fork[3]);
    pet->fork[3][2] += calcSelectedPowerRange();

    setPetCostumeColors(pet, mod);

    insertAnimEvent(event);
    ++event;

    event->type = POWERANIM_SEQ;
    event->time = spawnTime;
    event->seqBits = &spawnBits;
    event->entity = pet;
    insertAnimEvent(event);
    ++event;

    for (i = 0; i < eaSize(&def->powers); ++i)
    {
        int numPowers;
        const BasePower** powers;
        const BasePower* singlePower;
        int j;

        if (!strcmp(def->powers[i]->power, "*"))
        {
            const BasePowerSet* powerSet = powerdict_GetBasePowerSetByName(&g_PowerDictionary, def->powers[i]->powerCategory, def->powers[i]->powerSet);
            numPowers = eaSize(&powerSet->ppPowers);
            powers = powerSet->ppPowers;
        }
        else
        {
            numPowers = 1;
            singlePower = powerdict_GetBasePowerByName(&g_PowerDictionary, def->powers[i]->powerCategory, def->powers[i]->powerSet, def->powers[i]->power);
            powers = &singlePower;
        }

        for (j = 0; j < numPowers; ++j)
        {
            const BasePower* power = powers[j];

            if (!power || power->eType != kPowerType_Auto)
                continue;

            event = createPowerAnimationEvents(event, pet, power, timeOffset);
        }
    }

    return event;
}

static bool isPreviewMod(const AttribModTemplate* mod, Entity* pSrc)
{
    PowerCustomization* customization = getPowerCustomization(sSelectedPowers[CURRENT_POWER_CUST_MENU]);
    const char* token = customization ? customization->token : NULL;

    combateval_StoreCustomFXToken(token);

    if (mod->ppchApplicationRequires && combateval_EvalFromBasePower(pSrc, gTargetEnt, sSelectedPowers[CURRENT_POWER_CUST_MENU], mod->ppchApplicationRequires,
                                                                     sSelectedPowers[CURRENT_POWER_CUST_MENU]->pchSourceFile) == 0.0f)
        return false;

    // A power will "always" have a default attrib mod with a chance of 1.0f.
    // All other attrib mods will have a starting chance of 0.0f.
    if (mod->fChance <= 0.5f)
        return false;

    return true;
}

static PowerAnimEvent* createPowerAnimationEvents(PowerAnimEvent* event, Entity* caster, const BasePower* power, F32 timeOffset)
{
    int i;
    int numSpawns = 0;

    event = createPrimaryAnimationEvents(event, caster, power, timeOffset);

    for (i = 0; i < eaSize(&power->ppTemplates); ++i)
    {
        const AttribModTemplate* mod = power->ppTemplates[i];
        F32 modTime;

        if (!isPreviewMod(mod, caster))
            continue;

        modTime = timeOffset + mod->fDelay * 30.0f + getHitTime(power, mod->eTarget == kModTarget_Caster ? 0 : distance3(caster->fork[3], gTargetEnt->fork[3]));

        if (mod->pchEntityDef && numSpawns < MAX_SPAWNS)
        {
            event = createSpawnAnimationEvents(event, caster, mod, numSpawns++, modTime);
        }

        if (mod->pchConditionalFX)
        {
            const PowerFX* pfx = getSelectedPowerFX(power);
            if (pfx == NULL)
                Errorf("Missing custom PFX config for power: %s", power->pchFullName);
            else
            {
                const char* fxName = basepower_GetAttribModFX(mod->pchConditionalFX, getSelectedPowerFX(power)->pchConditionalFX);
                event->type = POWERANIM_FX;
                event->fxName = fxName;
                event->fxkey0.seq = (mod->eTarget == kModTarget_Caster) ? caster->seq : gTargetEnt->seq;
                event->fxkey1.seq = caster->seq;
                event->time = modTime;
                event->entity = caster;
                insertAnimEvent(event);
                ++event;
            }
        }

        if (mod->piConditionalBits)
        {
            event->type = POWERANIM_SEQ;
            event->seqBits = cpp_const_cast(const int**)(&mod->piConditionalBits);
            event->entity = (mod->eTarget == kModTarget_Caster) ? caster : gTargetEnt;
            event->time = modTime;
            insertAnimEvent(event);
            ++event;
        }

        if (mod->pchContinuingFX)
        {
            const PowerFX* pfx = getSelectedPowerFX(power);
            if (pfx == NULL)
                Errorf("Missing custom PFX config for power: %s", power->pchFullName);
            else
            {
                const char* fxName = basepower_GetAttribModFX(mod->pchContinuingFX, getSelectedPowerFX(power)->pchContinuingFX);
                event->type = POWERANIM_FX;
                event->fxName = fxName;
                event->fxkey0.seq = (mod->eTarget == kModTarget_Caster) ? caster->seq : gTargetEnt->seq;
                event->fxkey1.seq = caster->seq;
                event->time = modTime;
                event->entity = caster;
                insertAnimEvent(event);
                ++event;
            }
        }

        if (mod->piContinuingBits)
        {
            event->type = POWERANIM_SEQ;
            event->seqBits = cpp_const_cast(const int**)(&mod->piContinuingBits);
            event->entity = (mod->eTarget == kModTarget_Caster) ? caster : gTargetEnt;
            event->time = modTime;
            insertAnimEvent(event);
            ++event;
        }

        if (mod->offAttrib == kSpecialAttrib_Translucency)
        {
            event->type = POWERANIM_TRANSLUCENCY;
            event->entity = (mod->eTarget == kModTarget_Caster) ? caster : gTargetEnt;
            event->time = modTime;
            event->translucency = mod->fScale;
            insertAnimEvent(event);
            ++event;
        }

        if (mod->offAttrib == kSpecialAttrib_SetCostume)
        {
            event->type = POWERANIM_COSTUME;
            event->entity = (mod->eTarget == kModTarget_Caster) ? caster : gTargetEnt;
            event->time = modTime;
            event->pchCostumeName = mod->pchCostumeName;
            insertAnimEvent(event);
            ++event;
        }
    }

    return event;
}

static void compressAnimationQueue()
{
    PowerAnimEvent* event = gAnimQueue;
    F32 lastEvent = 0.0f;
    F32 offset = 0.0f;

    while (event->type != POWERANIM_END)
    {
        event->time -= offset;

        if (event->time > lastEvent + MIN_EVENT_INTERVAL)
        {
            offset += event->time - (lastEvent + MIN_EVENT_INTERVAL);
            event->time = lastEvent + MIN_EVENT_INTERVAL;
        }

        lastEvent = event->time;
        ++event;
    }
}

static void createAnimationQueue()
{
    PowerAnimEvent* event = gAnimQueue;
    Entity* player = playerPtrForShell(1);

    event->type = POWERANIM_END;
    event->time = 1e6;
    ++event;

    if (!sSelectedPowers[CURRENT_POWER_CUST_MENU])
        return;

    event = createPowerAnimationEvents(event, player, sSelectedPowers[CURRENT_POWER_CUST_MENU], 0.0f);

    compressAnimationQueue();

    if (event != gAnimQueue + 1)
    {
        event->type = POWERANIM_LOOP;
        event->time = (event - 2)->time + 90.0f;
        insertAnimEvent(event);
        ++event;
    }
}

static void clearPowerCustomizationAnimation()
{
    PowerAnimEvent* event = gAnimQueue;
    Entity* e = playerPtrForShell(0);

    while (event->type != POWERANIM_END)
    {
        if (event->type == POWERANIM_FX && event->fxid != 0)
        {
            fxDelete(event->fxid, HARD_KILL);
            event->fxid = 0;
        }
        else if (event->type == POWERANIM_SPAWN)
            entFree(event->entity);
        ++event;
    }

    if (gTargetEnt)
    {
        entFree(gTargetEnt);
        gTargetEnt = NULL;
    }

    memset(gAnimQueue, 0, sizeof(gAnimQueue));

    gAnimationProgress = 0.0f;
    gAnimStage = 0;
    gNumSustainedSequences = 0;

    if (e)
    {
        e->seq->forceInterrupt = true;
        e->xlucency = 1.0f;
        e->curr_xlucency = 1.0f;
        revertPlayerCostume(e);
    }
}

void restartPowerCustomizationAnimation()
{
    clearPowerCustomizationAnimation();
    createAnimationQueue();
}

void resetPowerCustomizationPreview()
{
    clearPowerCustomizationAnimation();
    gLoopNow = true;
}

void powerCustMenuExit()
{
    Entity* pe = playerPtrForShell(1);

    clearPowerCustomizationAnimation();
    seqClearState(pe->seq->state);
    // freeze the player model
    seqSetState(pe->seq->state, TRUE, STATE_PROFILE);
    seqSetState(pe->seq->state, TRUE, STATE_HIGH);

    toggle_3d_game_modes(SHOW_NONE);
}

char* powerCustExt()
{
    return ".powerCust";
}
char* powerCustPath()
{
    static char path[MAX_PATH];

    if (!path[0])
    {
        if (isDevelopmentMode())
        {
            char* s;
            strcpy(path, fileDataDir());
            if (s = strrchr(path, '/'))
                *s = '\0';
        }
        else
        {
            getExecutableDir(path);
        }
        strcat(path, "/PowerCust");
    }

    return path;
}
void powerCustList_tailorInit(Entity* e)
{
    assert(e->powerCust == powerCustList_current(e));
    powerCust_syncPowerCustomizations(e); // verify all available custom powers have been populated
    gCopyOfPowerCustList = e->powerCust;
    e->powerCust = powerCustList_clone(e->powerCust);

    // check if there are any pool/epic or incarnate powers
    {
        int iCustomCategory, iPowerSet;
        int numCustomCategories = eaSize(&gPowerCustomizationMenu.pCategories);
        int numPowerSets = eaSize(&e->pchar->ppPowerSets);
        const PowerCategory* pool = powerdict_GetCategoryByName(&g_PowerDictionary, "Pool");
        const PowerCategory* epic = powerdict_GetCategoryByName(&g_PowerDictionary, "Epic");

        sTailorHasPoolPowers = 0;
        sTailorHasIncarnatePowers = 0;

        for (iPowerSet = 0; iPowerSet < numPowerSets && !(sTailorHasPoolPowers && sTailorHasIncarnatePowers); ++iPowerSet)
        {
            const BasePowerSet* powerSet = getPowerSetByIndex(iPowerSet)->psetBase;

            // check for existence of pool/epic power
            if (powerSet->pcatParent == pool || powerSet->pcatParent == epic)
            {
                sTailorHasPoolPowers = 1;
            }

            // check for existence of incarnate power
            for (iCustomCategory = 0; iCustomCategory < numCustomCategories && !sTailorHasIncarnatePowers; ++iCustomCategory)
            {
                PowerCustomizationMenuCategory* menuCategory = gPowerCustomizationMenu.pCategories[iCustomCategory];
                const PowerCategory* allowedCategory = powerdict_GetCategoryByName(&g_PowerDictionary, menuCategory->categoryName);
                if (powerSet->pcatParent == allowedCategory)
                {
                    if (!chareval_requires(e->pchar, menuCategory->hideIf, "Defs/powercustomizationmenu.def"))
                    {
                        int i;
                        for (i = 0; i < eaSize(&menuCategory->powerSets); ++i)
                        {
                            if (stricmp(powerSet->pchName, menuCategory->powerSets[i]) == 0)
                            {
                                sTailorHasIncarnatePowers = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    s_outOfDate = 1;
}

void powerCustomizationMenu()
{
    Entity* e = playerPtr();
    int hideMenu = 0;
    static float timer = 0;
    int totalCost = 0;

    int inTailor = getLastNavMenuType() == HYBRID_MENU_TAILOR;
    char* greyPay = NULL;
    float screenScaleX, screenScaleY;
    Vec3 avatarPos;
    F32 xposSc, yposSc, xp, yp, textXSc, textYSc;

    if (drawHybridCommon(inTailor ? HYBRID_MENU_TAILOR : HYBRID_MENU_CREATION))
        return;

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);

    // Refresh in case of power reloads
    if (isDevelopmentMode())
        initializeTokens();

    updateScrollSelectors(32, 712);

    repositioningCamera = repositionCamera(repositioningCamera);

    setScreenScaleOption(SSO_SCALE_ALL);
    hideMenu = drawHideMenuSelector(200.f, 710.f);
    setScreenScaleOption(SSO_SCALE_TEXTURE);
    if (!hideMenu)
    {
        F32 saveloadY = inTailor ? 700.f : 710.f;
        static HybridElement sButtonSave = {0, "SaveString", "savePowerCustButton", "icon_costume_save"};
        static HybridElement sButtonLoad = {0, "LoadString", "loadPowerCustButton", "icon_costume_load"};
        // default text

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_14);
        font_outl(0);
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(30.f * xposSc + 2.f, 102.f * yposSc + 2.f, 5.f, textXSc, textYSc, "CustomizePowerString");
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(30.f * xposSc, 102.f * yposSc, 5.f, textXSc, textYSc, "CustomizePowerString");
        font_outl(1);
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawPowerSets(e->db_id ? COSTUME_EDIT_MODE_TAILOR : COSTUME_EDIT_MODE_STANDARD);
        drawColors();
        drawAvatarControls(xp - 159.f * xposSc, 640.f * yposSc, &gZoomedInForPowers, 0);

        setScreenScaleOption(SSO_SCALE_ALL);
        if (drawHybridButton(&sButtonSave, 820.f, saveloadY, 5000.f, 1.f, CLR_WHITE, HB_DRAW_BACKING) == D_MOUSEHIT)
        {
            saveCostume_start(MENU_POWER_CUST);
        }

        if (drawHybridButton(&sButtonLoad, 890.f, saveloadY, 5000.f, 1.f, CLR_WHITE, HB_DRAW_BACKING) == D_MOUSEHIT)
        {
            start_menu(MENU_LOAD_POWER_CUST);
        }
        setScreenScaleOption(SSO_SCALE_TEXTURE);
    }
    copyVec3(gZoomedInForPowers ? ZOOMED_IN_POSITION : ZOOMED_OUT_POSITION, avatarPos);
    avatarPos[0] = 1.f - (159.f * xposSc) / (xp / 2.f);
    zoomAvatar(true, avatarPos);

    if (inTailor)
    {
        tailorPriceOptionsSelectionUpdate();
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        totalCost = tailor_drawTotal(e, genderEditingMode, screenScaleX, screenScaleY);
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    }

    scaleAvatar();

    powerCust_handleAnimation();

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    drawHybridNext(DIR_LEFT, 0, "BackString");

    if (!inTailor || !isHybridLastMenu())
    {
        drawHybridNext(DIR_RIGHT, 0, "NextString");
    }
    else
    {
        int bGrayedButton;
        // draw alternate payments button
        tailor_drawPayOptions(totalCost, totalCost > 0, screenScaleX, screenScaleY);

        // check whether we have the required funds to continue, lock the button gray if not
        greyPay = tailorPriceOptions_GreyPay(totalCost, e->pchar->iInfluencePoints);
        bGrayedButton = greyPay != 0;
        if (drawHybridNext(DIR_RIGHT, bGrayedButton, bGrayedButton ? greyPay : "NextString"))
        {
            Entity* e = playerPtr();

            assert(gCopyOfPowerCustList);
            tailorMenu_applyChanges(); // buy power cust & costume & apply costume to character

            // tailor cleanup and return to game
            tailor_exit(0);
        }
    }
}

void powerCustMenuEnter()
{
    Entity* e = playerPtr();

    int inTailor = getLastNavMenuType() == HYBRID_MENU_TAILOR;
    gOwnedCostume = NULL;

    // create costume if there isn't one
    if (!inTailor)
    {
        costumeInit();
        setDefaultCostume(true);
    }

    clearSS();

    toggle_3d_game_modes(SHOW_TEMPLATE);
    if (!menuInitialized)
    {
        initializeMenu();
    }
    else
    {
        if (!inTailor)
        {
            // rebuild customizable powers in case the player has changed powersets
            // only applicable in the character creator
            resetCustomPowers(e);
            initializeTokens();
        }

        // costume must be applied again in case the player is coming from the body menu
        costume_Apply(e);
    }

    if (!inTailor && !checkCostumeMatchesBody(e->costume->appearance.bodytype, gCurrentGender))
    {
        setDefaultCostume(false);
    }
}

int canCustomizePoolPowers(int foo)
{
    return sTailorHasPoolPowers;
}

int canCustomizeIncarnatePowers(int foo)
{
    return sTailorHasIncarnatePowers;
}
