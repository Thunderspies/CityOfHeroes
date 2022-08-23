#include "uiNPCProfile.h"
#include "uiNPCCostume.h"
#include "UI/uiCostume.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiUtilMenu.h"
#include <utilitieslib/language/MessageStoreUtil.h>        //    for textStd
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "game.h"
#include "cmdparse/cmdgame.h"
#include "gameComm/Npc.h"
#include "gameData/costume_critter.h"
#include "UI/uiAvatar.h"
#include "entity/entity.h"
#include "player/player.h"
#include "entity/costume.h"
#include "entity/costume_client.h"
#include "entity/entPlayer.h"
#include "uiScrollSelector.h"
#include "entity/PCC_Critter_Client.h"
#include "entity/CustomVillainGroup.h"
#include "gameData/costume_data.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/Cbox.h>
#include "UI/uiInput.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "seq/seq.h"
#include "graphics/FX/particle.h"
#include "graphics/ttFontUtil.h"
#include "seq/seqstate.h"
#include "UI/uiGender.h"
#include "uiNPCCreationNLM.h"
#include "UI/Hybrid/uiHybridMenu.h"

extern int gCostumeCritter;
extern int gCurrentBodyType;
extern CostumeMasterList gCostumeMaster;
static int prevCostumeCritter;
static int prevBodyType;
static CostumeMasterList tempMasterList;
static CustomNPCCostume **s_CurrentNPCCostume = NULL;
static const NPCDef *s_currentNPCDef = NULL;
static float cameraScale = 0;
static float entityScale = 0;
static int s_npcEditCostumeMode = 0;
#define ZOOM_X 865
#define ZOOM_Y 682
#define ZOOM_Z 5000
ShoppingCart sStorePartsContained;

static int mode = 0;
int getNPCEditCostumeMode()
{
    return s_npcEditCostumeMode;
}

static void drawDisableParticleEffectsSelector(F32 x, F32 y)
{
    AtlasTex *check, *checkD;
    AtlasTex * glow   = atlasLoadTexture("costume_button_link_glow.tga");
    int checkColor = CLR_WHITE;
    int checkColorD = CLR_WHITE;
    CBox box;
    Entity *pe = playerPtrForShell(0);
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    if (pe && pe->seq && pe->seq->type && pe->seq->type->fx)
    {
        if( particleSystemGetDisableMode() )
        {
            if (game_state.skin != UISKIN_HEROES)
            {
                check  = atlasLoadTexture("costume_button_link_check_grey.tga");
                checkD = atlasLoadTexture("costume_button_link_check_press_grey.tga");
            }
            else
            {
                check  = atlasLoadTexture("costume_button_link_check.tga");
                checkD = atlasLoadTexture("costume_button_link_check_press.tga");
            }
        }
        else
        {
            if (game_state.skin != UISKIN_HEROES)
            {
                check   = atlasLoadTexture("costume_button_link_off_tintable.tga");
                checkD  = atlasLoadTexture("costume_button_link_off_press_tintable.tga");
            }
            else
            {
                check   = atlasLoadTexture("costume_button_link_off.tga");
                checkD  = atlasLoadTexture("costume_button_link_off_press.tga");
            }
        }
        BuildScaledCBox( &box, x - check->width*xposSc/2, y - check->width*yposSc/2, check->width*xposSc, check->height*yposSc );
        if( mouseCollision( &box ))
        {
            display_sprite_positional( glow, x, y, 5000, 1.f, 1.f, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );

            if( isDown( MS_LEFT ) )
                display_sprite_positional( checkD, x, y, 5001, 1.f, 1.f, checkColorD, H_ALIGN_CENTER, V_ALIGN_CENTER );
            else
                display_sprite_positional( check, x, y, 5001, 1.f, 1.f, checkColor, H_ALIGN_CENTER, V_ALIGN_CENTER );

            if( mouseClickHit( &box, MS_LEFT) )
            {
                particleSystemSetDisableMode(!particleSystemGetDisableMode());
            }
        }
        else
            display_sprite_positional( check, x, y, 5001, 1.f, 1.f, checkColor, H_ALIGN_CENTER, V_ALIGN_CENTER );

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x + (check->width/2 + 5)*xposSc, y+5*yposSc, 5001, textXSc, textYSc, textStd("DisableParticleEffectsString"));
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    }
}
static void drawNPCCostumeZoomButtons(float screenScaleX, float screenScaleY)
{
    AtlasTex *zoomIn, *zoomInD;
    AtlasTex *zoomOut, *zoomOutD;
    AtlasTex *glow = atlasLoadTexture("costume_button_link_glow.tga");
    float screenScale = MIN(screenScaleX, screenScaleY);
    CBox box;

        if (game_state.skin != UISKIN_HEROES)
        {
            zoomOut  = atlasLoadTexture("costume_button_zoom_out_tintable.tga");
            zoomOutD = atlasLoadTexture("costume_button_zoom_out_press_tintable.tga");
            zoomIn  = atlasLoadTexture("costume_button_zoom_in_tintable.tga");
            zoomInD = atlasLoadTexture("costume_button_zoom_in_press_tintable.tga");
        }
        else
        {
            zoomOut  = atlasLoadTexture("costume_button_zoom_out.tga");
            zoomOutD = atlasLoadTexture("costume_button_zoom_out_press.tga");
            zoomIn  = atlasLoadTexture("costume_button_zoom_in.tga");
            zoomInD = atlasLoadTexture("costume_button_zoom_in_press.tga");
        }

    BuildCBox( &box, ZOOM_X - zoomOut->width*1.5, ZOOM_Y - zoomOut->width/2, zoomOut->width, zoomIn->height );

    if( mouseCollision( &box ) )
    {
        display_sprite( glow, ZOOM_X*screenScaleX-glow->width*screenScale/2, (ZOOM_Y - (zoomOut->width + glow->width/2))- glow->height/2, ZOOM_Z-1, screenScale, screenScale, CLR_WHITE );
        display_sprite( zoomOutD, ZOOM_X*screenScaleX-zoomOutD->width*screenScale/2, (ZOOM_Y- (zoomOut->width + glow->width/2)) - zoomOutD->height/2, ZOOM_Z, screenScale, screenScale, CLR_WHITE );
        if (isDown(MS_LEFT))
        {
            cameraScale -= 1.0f;
        }
    }
    else
        display_sprite( zoomOut, ZOOM_X*screenScaleX-zoomOut->width*screenScale/2, ((ZOOM_Y*screenScaleY) - (zoomOut->width*1.5)) - zoomOut->height/2, ZOOM_Z, screenScale, screenScale, CLR_WHITE );

    BuildCBox( &box, ZOOM_X + zoomIn->width/2, ZOOM_Y - zoomIn->width/2, zoomIn->width, zoomIn->height );
    if( mouseCollision( &box ) )
    {
        display_sprite( glow, ZOOM_X*screenScaleX-glow->width*screenScale/2, (ZOOM_Y+ (zoomIn->width-(glow->width/2))) - glow->height/2, ZOOM_Z-1, screenScale, screenScale, CLR_WHITE );
        display_sprite( zoomInD, ZOOM_X*screenScaleX-zoomInD->width*screenScale/2, (ZOOM_Y  + (zoomIn->width-(zoomInD->width/2)))- zoomInD->height/2, ZOOM_Z, screenScale, screenScale, CLR_WHITE );
        if (isDown(MS_LEFT))
        {
            cameraScale += 1.0f;
        }
    }
    else
        display_sprite( zoomIn, ZOOM_X*screenScaleX-zoomIn->width*screenScale/2, ((ZOOM_Y*screenScaleY)+ zoomIn->width/2) - zoomIn->height/2, ZOOM_Z, screenScale, screenScale, CLR_WHITE );

    cameraScale = CLAMPF32(cameraScale, -15, 20);
    zoomAvatar(false, gZoomed_in ? avatar_getZoomedInPosition(COSTUME_EDIT_MODE_NPC,0) : avatar_getDefaultPosition(COSTUME_EDIT_MODE_NPC,0));
}

int NPCCostume_canEditCostume(const cCostume *costume)
{
    int colorTintField;
    colorTintField = costume_setColorTintablePartsBitField(costume);
    mode = costume_isThereSkinEffect(costume) ? COSTUME_EDIT_MODE_NPC : COSTUME_EDIT_MODE_NPC_NO_SKIN;
    if ( (colorTintField == 0) && (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN) )
    {
        return 0;
    }
    return 1;
}
void CustomNPCEdit_setup(const NPCDef *contactDef, const VillainDef *vDef, int index, CustomNPCCostume **npcCostume, int npcCostumeEditMode, char **displayNamePtr, char **descriptionPtr, char **vg_name, int vg_name_locked)
{
    Entity *e;
    const NPCDef *npcDef = NULL;
    if (contactDef)
    {
        npcDef = contactDef;
    }
    if (vDef)
    {
        if (vDef && vDef->levels && vDef->levels[0] && vDef->levels[0]->costumes && EAINRANGE(index, vDef->levels[0]->costumes) &&
            vDef->levels[0]->costumes[index])
        {
            npcDef = npcFindByName(vDef->levels[0]->costumes[index], NULL);
        }
    }
    if(!npcDef)
    {
        start_menu(MENU_GAME);
        return;
    }

    NPCProfileMenu_setCurrentVDef(vDef);
    setupNPCProfileTextBlocks(displayNamePtr, descriptionPtr, vg_name, vg_name_locked);

    setPCCEditingMode(PCC_EDITING_MODE_CREATION);        //    falling back into old habits.. using the pcc entity instead of the player
    //    using the pcc entity has the benefit of not affecting the player costume or it's sequencers...
    e = createPCC_Entity();
    if (npcCostumeEditMode == 1)
    {
        const cCostume *origCostume = NULL;
        Costume *modifiedCostume = NULL;
        s_currentNPCDef = npcDef;
        prevBodyType = gCurrentBodyType;
        tempMasterList = gCostumeMaster;
        prevCostumeCritter = gCostumeCritter;
        loadCritterCostume( npcDef );
        if (*npcCostume)
        {
            int i;
            origCostume = npcDef->costumes[0];
            modifiedCostume = costume_clone(npcDef->costumes[0]);
            for (i = (eaSize(&(modifiedCostume->parts))-1); i >= eaSize(&origCostume->parts); --i)
            {
                StructDestroy(ParseCostumePart,modifiedCostume->parts[i]);
                eaRemove(&modifiedCostume->parts, i);
            }
            costumeApplyNPCColorsToCostume(modifiedCostume, *npcCostume);
            npcDef->costumes[0] = costume_as_const(modifiedCostume);
        }
        findCritterCostume( npcDef );
        updateCostume();
        s_CurrentNPCCostume = npcCostume;
        cameraScale = 0;
        if (*npcCostume)
        {
            npcDef->costumes[0] = origCostume;
            costume_destroy(modifiedCostume);
        }
        start_menu(MENU_NPC_COSTUME);
        s_npcEditCostumeMode = 1;
    }
    else
    {
        e->costume = npcDef->costumes[0];
        start_menu(MENU_NPC_PROFILE);
        s_npcEditCostumeMode = 0;
    }
    setPCCEditingMode(PCC_EDITING_MODE_CREATION * -1);
}

void CustomNPCEdit_exit(int saveChanges)
{
    Entity *e;
    if (getPCCEditingMode() < 0)
    {
        setPCCEditingMode(getPCCEditingMode()*-1);
    }
    destroyNPCProfileTextBlocks();
    NPCProfileMenu_setCurrentVDef(NULL);
    if (s_npcEditCostumeMode )
    {
        if (saveChanges)
        {
            if (s_CurrentNPCCostume)
            {
                if (*s_CurrentNPCCostume)
                {
                    StructDestroy(ParseCustomNPCCostume, *s_CurrentNPCCostume);
                    *s_CurrentNPCCostume = NULL;
                }
                costume_NPCCostumeSave(s_currentNPCDef->costumes[0], s_currentNPCDef->name, s_CurrentNPCCostume);
            }
        }

        gCostumeMaster = tempMasterList;
        gCurrentBodyType = prevBodyType;
        gCostumeCritter = prevCostumeCritter;
    }
    e = playerPtr();
    destroyPCC_Entity(e);
    setPCCEditingMode(PCC_EDITING_MODE_NONE);
    particleSystemSetDisableMode(0);
    start_menu(MENU_GAME);
}
static void NPCCostumeResetColors()
{
    clearSelectedGeoSet();
    findCritterCostume( s_currentNPCDef );
    updateCostume();
}
static const Vec3 ZOOMED_OUT_POSITION = {0.7015, 2.6, 20.5};
static int sZoomedInForNPC = 0;
static int init = 0;
static void NPCcostumeMenuExitCode(void)
{
    init = 0;
}
static int NPCcostumeMenuHiddenCode(void)
{
    return (!getNPCEditCostumeMode());
}
NonLinearMenuElement NPC_costume_NLME = { MENU_NPC_COSTUME, { "CC_CostumeString", 0, 0 },
                            NPCcostumeMenuHiddenCode, NPCcostumeMenuHiddenCode, 0, 0, NPCcostumeMenuExitCode };

void NPCCostumeMenu()
{
    
    Entity *e;
    const CostumeGeoSet *selectedGeoSet;
    int handUsage = 0;
    F32 xposSc, yposSc, xp, yp;
    SpriteScalingMode ssm = getSpriteScaleMode();

    if (getPCCEditingMode() < 0)
    {
        setPCCEditingMode(getPCCEditingMode()*-1);
    }
    e = playerPtr();
    if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
        return;
    if (!init)
    {
        Entity *pe;
        Costume* mutable_costume_pe;
        Costume* mutable_costume_e;

        init = 1;
        clearSelectedGeoSet();
        pe = playerPtrForShell(1);
        if (pe && pe->seq && pe->seq->type && pe->seq->type->geomscale && pe->seq->type->geomscale[1] > 1.0f)
        {
            entityScale = (100.0f/pe->seq->type->geomscale[1])-100;
        }
        else
        {
            entityScale = 0;
        }
        entSetSeq(pe, seqLoadInst( s_currentNPCDef->costumes[0]->appearance.entTypeFile, 0, SEQ_LOAD_FULL, e->randSeed, e ));
        mutable_costume_pe = costume_as_mutable_cast(pe->costume);
        mutable_costume_pe->appearance.bodytype = s_currentNPCDef->costumes[0]->appearance.bodytype;
        mutable_costume_e = costume_as_mutable_cast(e->costume);
        mutable_costume_e->appearance.bodytype = s_currentNPCDef->costumes[0]->appearance.bodytype;
        if (pe->seq->type->selection == SEQ_SELECTION_BONES)
        {
            int j;
            int prefix_len = strcspn(s_currentNPCDef->costumes[0]->appearance.entTypeFile,"_");
            for(j=0; j<getBodyTypeCount(); j++)
            {
                if(strnicmp(s_currentNPCDef->costumes[0]->appearance.entTypeFile, g_BodyTypeInfos[j].pchEntType, prefix_len) == 0)
                {
                    mutable_costume_pe->appearance.bodytype = mutable_costume_e->appearance.bodytype = g_BodyTypeInfos[j].bodytype;
                    break;
                }
            }
            mutable_costume_pe->appearance.entTypeFile = mutable_costume_e->appearance.entTypeFile = s_currentNPCDef->costumes[0]->appearance.entTypeFile;
        }
        else
        {
            mutable_costume_pe->appearance.entTypeFile = mutable_costume_e->appearance.entTypeFile = NULL;
        }
    }

    setSpriteScaleMode(SPRITE_XY_SCALE);
    drawNonLinearMenu( NLM_NPCCreationMenu(), 20, 10, 20, 1000,20, 1.0f, NPC_costume_NLME.listId, NPC_costume_NLME.listId, 0 );
    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    xp = DEFAULT_SCRN_WD/2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);

    // frame around guy
    toggle_3d_game_modes(SHOW_TEMPLATE);
    updateScrollSelectors( 32, 712 );
    costume_drawRegions( mode, &sStorePartsContained );
    costume_drawColorsScales( mode );
    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    drawDisableParticleEffectsSelector(xp - 274.f*xposSc, 670.f*yposSc);
    xp = DEFAULT_SCRN_WD/2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    drawAvatarControls(xp, 675.f*yposSc, &gZoomed_in, 0);
    zoomAvatar(false, sZoomedInForNPC ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));
    convergeFaceScale();
    selectedGeoSet = getSelectedGeoSet(mode);
    if (selectedGeoSet && (stricmp(selectedGeoSet->bodyPartName, "wepr") == 0))
    {
        handUsage |= 1;
        if (!isNullOrNone(e->costume->parts[24]->pchFxName))
        {
            handUsage |= 2;
        }
    }
    if (selectedGeoSet && (stricmp(selectedGeoSet->bodyPartName, "wepl") == 0))
    {
        handUsage |= 2;
        if (!isNullOrNone(e->costume->parts[12]->pchFxName))
        {
            handUsage |= 1;
        }
    }

    if (handUsage)
    {
        switch (handUsage)
        {
        case 1:
            seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_RIGHTHAND);
            break;
        case 2:
            seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_LEFTHAND);
            break;
        case 3:
            seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_TWOHAND);
            break;
        }
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_COMBAT);
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE);
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, 112);
    }
    else
    {
        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model
        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH ); // freeze the player model
    }
    scaleHero(e, entityScale+(cameraScale*(100.0f-entityScale)/100.0f) );
    if ( drawStdButton( 500, 740, 10, 150, 40, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_NAVY ), "ResetString", 1.f, 0) == D_MOUSEHIT )
    {
        NPCCostumeResetColors();
    }
    if ( drawNextButton( 40,  740, 10, 1.f, 0, 0, 0, "BackString" ) )
    {
        NPCcostumeMenuExitCode();
        CustomNPCEdit_exit(0);
    }
    if ( drawNextButton( 980, 740, 10, 1.f, 1, 0, 0, "NextString" ))
    {
        NPCcostumeMenuExitCode();
        start_menu(MENU_NPC_PROFILE);
    }

    setScalingModeAndOption(ssm, SSO_SCALE_ALL);
    if (getPCCEditingMode() > 0)
    {
        setPCCEditingMode(getPCCEditingMode()*-1);
    }
}