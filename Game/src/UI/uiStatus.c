/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "player/player.h"
#include "player/pmotion.h"
#include "entity/character_base.h"
#include "UI/uiStatus.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiToolTip.h"
#include "UI/uiUtilGame.h"
#include "UI/uiContextMenu.h"
#include "UI/uiInfo.h"
#include "language/langClientUtil.h"
#include "UI/uiInput.h"
#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiDialog.h"
#include "UI/uiBox.h"
#include "entity/entPlayer.h"
#include "UI/uiWindows_init.h"
#include "UI/uiOptions.h"
#include "entity/Supergroup.h"
#include "UI/uiBuff.h"
#include "entity/TaskforceParams.h"

#include "entity/character_level.h"

#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/textureatlas.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include "win/win_init.h"
#include "bases/baseedit.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiLevelingpact.h"

#define BAR_HT          10
#define BAR_HT_2        15
#define BAR_HT_SKILLS    8
#define BAR_HT_2_SKILLS  8



static ToolTip HealthTip = { 0 }, EnduranceTip = { 0 }, ExperienceTip = { 0 }, RageTip = { 0 }, NotifyTip = { 0 };
static ToolTipParent StatusTipParent = { 0 };
ContextMenu* gXpcm;
static int inSupergroupMode = 0;
static int showRage = 0;


char* statuscm_hp(void* unused)
{
	Entity* e = playerPtr();
	if (e->pchar->attrMax.fAbsorb > 0)
	{
		return textStd("HealthAndAbsorbTip", e->pchar->attrCur.fHitPoints, e->pchar->attrMax.fAbsorb, e->pchar->attrMax.fHitPoints);
	}
	else
	{
		return textStd("HealthTip", e->pchar->attrCur.fHitPoints, e->pchar->attrMax.fHitPoints);
	}
}

char* statuscm_end(void* unused)
{
	Entity* e = playerPtr();
	return textStd("EnduranceTip", (int)e->pchar->attrCur.fEndurance, (int)e->pchar->attrMax.fEndurance);
}

char* statuscm_insight(void* unused)
{
	Entity* e = playerPtr();
	return textStd("InsightTip", (int)e->pchar->attrCur.fInsight, (int)e->pchar->attrMax.fInsight);

}

char* statuscm_rage(void* unused)
{
	Entity* e = playerPtr();
	if (strncmp(e->pchar->pclass->pchName, "Class_Blaster", 13) == 0)
		//        return textStd( "DesperationTip", (int)(e->pchar->attrCur.fMeter*100), (int)(e->pchar->attrMax.fMeter*100) );
		return "";
	else if (strncmp(e->pchar->pclass->pchName, "Class_Brute", 13) == 0)
		return textStd("RageTip", (int)(e->pchar->attrCur.fMeter * 100), (int)(e->pchar->attrMax.fMeter * 100));
	else if (strncmp(e->pchar->pclass->pchName, "Class_Dominator", 15) == 0)
		return textStd("DominationTip", (int)(e->pchar->attrCur.fMeter * 100), (int)(e->pchar->attrMax.fMeter * 100));
	else if (strncmp(e->pchar->pclass->pchName, "Class_Primalist", 15) == 0)
		return textStd("PrimalEnergyTip", (int)(e->pchar->attrCur.fMeter * 100), (int)(e->pchar->attrMax.fMeter * 100));
	else if (strncmp(e->pchar->pclass->pchName, "Class_Rescued_Devoured", 22) == 0)
		return textStd("BattleEuphoriaTip", (int)(e->pchar->attrCur.fMeter * 100), (int)(e->pchar->attrMax.fMeter * 100));
	else if (strncmp(e->pchar->pclass->pchName, "Class_Sentinel", 14) == 0)
		return textStd("OpportunityTip", (int)(e->pchar->attrCur.fMeter * 100), (int)(e->pchar->attrMax.fMeter * 100));
	else
		return "";
}

char* statuscm_xp(void* unused)
{
	Entity* e = playerPtr();
	int experience_level = character_CalcExperienceLevel(e->pchar);
	float next_level_xp = g_ExperienceTables.aTables.piRequired[experience_level + 1] - g_ExperienceTables.aTables.piRequired[experience_level];
	float xp_towards_level = e->pchar->iExperiencePoints - g_ExperienceTables.aTables.piRequired[experience_level];

	return textStd("XpOnlyTip", (int)xp_towards_level, (int)next_level_xp);
}

char* statuscm_debt(void* unused)
{
	Entity* e = playerPtr();
	return textStd("DebtTip", e->pchar->iExperienceDebt);
}

char* statuscm_rest(void* unused)
{
	Entity* e = playerPtr();
	return textStd("PatrolTip", e->pchar->iExperienceRest);
}


//----------------------------------------------------------------------------------------------------------
// Notification System /////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

typedef struct StatusNotify
{
	void    (*code)();
	AtlasTex* icon;
	char* tooltip;
} StatusNotify;

StatusNotify** notificationQueue = 0;

int status_addNotification(AtlasTex* icon, void    (*code)(), char* tooltip)
{
	StatusNotify* sn;
	int i;

	if (!notificationQueue)
		eaCreate(&notificationQueue);

	for (i = 0; i < eaSize(&notificationQueue); i++)
	{
		if (icon == notificationQueue[i]->icon)
			return FALSE;
	}

	sn = calloc(1, sizeof(StatusNotify));
	sn->icon = icon;
	sn->code = code;
	sn->tooltip = tooltip;

	eaPush(&notificationQueue, sn);

	return TRUE;
}

void status_removeNotification(AtlasTex* icon)
{
	int i;

	if (!notificationQueue)
		return;

	for (i = 0; i < eaSize(&notificationQueue); i++)
	{
		if (icon == notificationQueue[i]->icon)
			// FIXEAR: Safe?
			eaRemove(&notificationQueue, i);
	}
}

#define BLINKS_PER_TURN    3
#define BLINK_SPEED        .1f

void status_notify(float x, float y, float z, float sc)
{
	Entity* e = playerPtr();
	static int current = 0;
	static float timer = 0;
	AtlasTex* dead = atlasLoadTexture("healthbar_notify_hospital.tga");
	AtlasTex* icon;
	CBox box;
	int i, isdead = 0;
	int xp_level = character_CalcExperienceLevel(e->pchar);

	BuildCBox(&box, x - 15 * sc, y - 15 * sc, 30 * sc, 30 * sc);

	font(&title_12);
	NotifyTip.constant_wd = 190 * sc;
	if (e->pchar->iCombatLevel < xp_level)
	{
		font_color(CLR_VILLAGON, CLR_VILLAGON);    // exemplar
		setToolTip(&NotifyTip, &box, textStd("StatusExemplarTip", e->pchar->iCombatLevel + 1, xp_level + 1), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
	}
	else if (e->pchar->iCombatLevel > xp_level)
	{
		font_color(CLR_PARAGON, CLR_PARAGON);        // sidekick
		setToolTip(&NotifyTip, &box, textStd("StatusSidekickTip", e->pchar->iCombatLevel + 1, xp_level + 1), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
	}
	else
	{
		font_color(CLR_WHITE, CLR_WHITE);
		setToolTip(&NotifyTip, &box, textStd("StatusLevelTip", e->pchar->iCombatLevel + 1, xp_level + 1), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
	}
	cprntEx(x - 1 * sc, y + 1 * sc, z, sc, sc, CENTER_X | CENTER_Y, "%i", e->pchar->iCombatLevel + 1);

	if (!notificationQueue || eaSize(&notificationQueue) == 0)
		return;

	// death is special case that disables everything else in queue
	for (i = 0; i < eaSize(&notificationQueue); i++)
	{
		if (notificationQueue[i]->icon == dead)
		{
			if (e->pchar->attrCur.fHitPoints > 0)
			{
				// FIXEAR: Safe?
				eaRemove(&notificationQueue, i);
				continue;
			}
			else
			{
				isdead = TRUE;
				current = i;
			}
		}
	}

	timer += TIMESTEP * BLINK_SPEED;

	if (timer > 6 * PI)
	{
		timer -= 6 * PI;

		if (!isdead)
			current++;
	}

	if (current > eaSize(&notificationQueue) - 1)
		current = 0;

	icon = notificationQueue[current]->icon;

	if (icon)
		display_sprite(icon, x - icon->width * sc / 2, y - icon->height * sc / 2, z, sc, sc, 0xffffff00 | (int)(192.f * (sinf(timer) + 1) / 2 + 63));

	BuildCBox(&box, x - icon->width / 2 * sc, y - icon->height / 2 * sc, icon->width * sc, icon->height * sc);
	if (mouseDownHit(&box, MS_LEFT))
	{
		if (notificationQueue[current]->code)
			notificationQueue[current]->code();
		else
			eaRemove(&notificationQueue, current);
	}

	if (notificationQueue[current]->tooltip)
		setToolTip(&NotifyTip, &box, notificationQueue[current]->tooltip, &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
}

//----------------------------------------------------------------------------------------------------------
// Status Window Stuff /////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

//
F32 getHeartBeatIntensity(float health_percentage, bool reset)
{
	static float t = -PI;
	static float sc;
	static U32 lastUpdateFrame;

	bool needUpdate = reset || (lastUpdateFrame != game_state.client_frame_timestamp);
	lastUpdateFrame = game_state.client_frame_timestamp;

	if (reset)
		t = 0;

	if (needUpdate)
	{
		float speed = 1.0 + .5 * (MAX_HEARTBEAT_HEALTH_PERCENTAGE - health_percentage);
		float amplitude = t < 4.0 * PI ? 1.0f : 0.0f;

		if (health_percentage <= 0.0f)
			speed = 0.5f;

		t += TIMESTEP * speed;
		sc = (1.f - cosf(t)) * .5 * amplitude;
		t = fmod(t, 8 * PI);
	}

	return sc;
}

void drawHeartBeatGlow(float x, float y, float z, float wd, float health_percentage, int color, float scale)
{
	static int    on = FALSE;
	bool reset = false;

	float sc;

	static AtlasTex* Lglow;
	static AtlasTex* Mglow;
	static AtlasTex* Rglow;
	static int texBindInit = 0;
	if (!texBindInit) {
		texBindInit = 1;
		Lglow = atlasLoadTexture("healthbar_heartglow_L.tga");
		Mglow = atlasLoadTexture("healthbar_heartglow_MID.tga");
		Rglow = atlasLoadTexture("healthbar_heartglow_R.tga");
	}

	if (health_percentage > MAX_HEARTBEAT_HEALTH_PERCENTAGE || health_percentage <= 0)
	{
		on = FALSE;
		return;
	}

	if (!on)
	{
		on = TRUE;
		reset = true;
	}

	sc = getHeartBeatIntensity(health_percentage, reset) * 0.6;
	display_sprite(Lglow, x + (5 - sc * Lglow->width) * scale, y + (1 - sc * Lglow->height / 2) * scale, z, sc * scale, sc * scale, color);
	display_sprite(Mglow, x + 5 * scale, y + (1 - sc * Mglow->height / 2) * scale, z, (wd - 5 * scale) / Mglow->width, sc * scale, color);
	display_sprite(Rglow, x + wd, y + (1 - sc * Rglow->height / 2) * scale, z, sc * scale, sc * scale, color);
}

#define RAGEPULSE_THRESHOLD .5f
//
void drawRagePulse(float x, float y, float z, float wd, float health_percentage, int color, float scale)
{
	static float t = -PI;
	static int    on = FALSE;

	float speed;
	float sc;

	static AtlasTex* Lglow;
	static AtlasTex* Mglow;
	static AtlasTex* Rglow;
	static int texBindInit = 0;
	if (!texBindInit) {
		texBindInit = 1;
		Lglow = atlasLoadTexture("healthbar_heartglow_L.tga");
		Mglow = atlasLoadTexture("healthbar_heartglow_MID.tga");
		Rglow = atlasLoadTexture("healthbar_heartglow_R.tga");
	}

	if (health_percentage < MAX_HEARTBEAT_HEALTH_PERCENTAGE || health_percentage >= 1.0)
	{
		on = FALSE;
		return;
	}

	if (!on)
	{
		on = TRUE;
		t = -PI;
	}

	if (t > 12 * PI)
	{
		t = -PI;
	}

	speed = 1.0 + .5 * (MAX_HEARTBEAT_HEALTH_PERCENTAGE - health_percentage);

	t += TIMESTEP * speed;

	sc = (1.f + sinf(t)) * .3;

	if (t > 3 * PI)
	{
		sc = 0.f;
		t += TIMESTEP * 10 * (MAX_HEARTBEAT_HEALTH_PERCENTAGE - health_percentage);
	}

	display_sprite(Lglow, x + (5 - sc * Lglow->width) * scale, y + (1 - sc * Lglow->height / 2) * scale, z, sc * scale, sc * scale, color);
	display_sprite(Mglow, x + 5 * scale, y + (1 - sc * Mglow->height / 2) * scale, z, (wd - 5 * scale) / Mglow->width, sc * scale, color);
	display_sprite(Rglow, x + wd, y + (1 - sc * Rglow->height / 2) * scale, z, sc * scale, sc * scale, color);
}


//
//
void drawHealthBarFrame(float x, float y, float z, float width, float scale, int color)
{
	static AtlasTex* left;
	static AtlasTex* right;
	static AtlasTex* pipe;
	static AtlasTex* back;
	static int texBindInit = 0;
	if (!texBindInit) {
		left = atlasLoadTexture("healthbar_framemain_L.tga");
		right = atlasLoadTexture("healthbar_framemain_R.tga");
		pipe = atlasLoadTexture("healthbar_framemain_horiz.tga");
		back = white_tex_atlas;
		texBindInit = 1;
	}

	// left bracket
	display_sprite(left, x, y, z, scale, scale, color);

	// right bracket
	display_sprite(right, x + (width - right->width * scale), y, z, scale, scale, color);

	// top pipe
	display_sprite(pipe, x + left->width * scale, y + 2 * scale, z,
		(width - (left->width + right->width) * scale) / pipe->width, scale, color);
	// bottom pipe
	display_sprite(pipe, x + left->width * scale, y - 2 * scale + (left->height - pipe->height) * scale, z,
		(width - (left->width + right->width) * scale) / pipe->width, scale, color);

	display_sprite(back, x + 2 + pipe->width * scale, y + pipe->height * scale + 2 * scale, z - 10,
		(width - 4 * pipe->width * scale) / back->width, scale * (left->height - 4 * pipe->height) / back->height, 0x00000088);
}

#define OUT_OF_PHASE_COLOR    0x888888ff
#define FULL_HEALTH_COLOR    0x50eb34ff
#define MED_HEALTH_COLOR    0xebff20ff
#define LOW_HEALTH_COLOR    0xf72200ff

#define FULL_HEALTH_THRESHOLD    .60f
#define MED_HEALTH_THRESHOLD    .40f
#define LOW_HEALTH_THRESHOLD    .20f

int getHealthColor(bool inPhase, float percentage)
{
	int color;

	// draw the health bar
	if (!inPhase)
	{
		color = OUT_OF_PHASE_COLOR;
	}
	else if (percentage > FULL_HEALTH_THRESHOLD)
	{
		color = FULL_HEALTH_COLOR;
	}
	else if (percentage > MED_HEALTH_THRESHOLD)
	{
		float transition_percent = (percentage - MED_HEALTH_THRESHOLD) / (FULL_HEALTH_THRESHOLD - MED_HEALTH_THRESHOLD);
		interp_rgba(transition_percent, MED_HEALTH_COLOR, FULL_HEALTH_COLOR, &color);
	}
	else if (percentage > LOW_HEALTH_THRESHOLD)
	{
		float transition_percent = (percentage - LOW_HEALTH_THRESHOLD) / (MED_HEALTH_THRESHOLD - LOW_HEALTH_THRESHOLD);
		interp_rgba(transition_percent, LOW_HEALTH_COLOR, MED_HEALTH_COLOR, &color);
	}
	else
		color = LOW_HEALTH_COLOR;

	return color;
}

#define ABSORB_COLOR    0x888888ff

int getAbsorbColor(float percentage)
{
	return ABSORB_COLOR;
}

#define FULL_ENDURANCE_COLOR    0x0005ffff
#define LOW_ENDURANCE_COLOR        0x000588ff

#define FULL_ENDURANCE_THRESHOLD    0.70f
#define LOW_ENDURANCE_THRESHOLD        0.30f

static int getEnduranceColor(float percentage)
{
	int color;

	// draw the health bar
	if (percentage > FULL_ENDURANCE_THRESHOLD)
	{
		color = FULL_ENDURANCE_COLOR;
	}
	else if (percentage > LOW_ENDURANCE_THRESHOLD)
	{
		float transition_percent = (percentage - LOW_ENDURANCE_THRESHOLD)
			/ (FULL_ENDURANCE_THRESHOLD - LOW_ENDURANCE_THRESHOLD);
		interp_rgba(transition_percent, LOW_ENDURANCE_COLOR, FULL_ENDURANCE_COLOR, &color);
	}
	else
		color = LOW_ENDURANCE_COLOR;

	return color;
}

#define FULL_INSIGHT_COLOR        0xc86400ff
#define LOW_INSIGHT_COLOR        0xa84400ff

#define FULL_INSIGHT_THRESHOLD    0.70f
#define LOW_INSIGHT_THRESHOLD        0.30f

static int getInsightColor(float percentage)
{
	int color;

	// draw the health bar
	if (percentage > FULL_INSIGHT_THRESHOLD)
	{
		color = FULL_INSIGHT_COLOR;
	}
	else if (percentage > LOW_INSIGHT_THRESHOLD)
	{
		float transition_percent = (percentage - LOW_INSIGHT_THRESHOLD)
			/ (FULL_INSIGHT_THRESHOLD - LOW_INSIGHT_THRESHOLD);
		interp_rgba(transition_percent, LOW_INSIGHT_COLOR, FULL_INSIGHT_COLOR, &color);
	}
	else
		color = LOW_INSIGHT_COLOR;

	return color;
}

#define FULL_RAGE_COLOR        0xc86400ff
#define LOW_RAGE_COLOR        0xa84400ff
#define OFF_RAGE_COLOR      0xa8440000

#define FULL_RAGE_THRESHOLD    0.20f
#define LOW_RAGE_THRESHOLD        0.10f

#define PULSE_SPEED 0.02f

static int getRageColor(float percentage)
{
	int color;
	static int dir = 1;
	static float step = 0;
	step += dir * PULSE_SPEED;
	if (step > 1.0f) {
		step = 1.0f;
		dir = -1;
	}
	if (step < 0.0f) {
		step = 0.0f;
		dir = 1;
	}
	// draw the health bar
	if (percentage > FULL_RAGE_THRESHOLD)
	{
		interp_rgba(step, OFF_RAGE_COLOR, FULL_RAGE_COLOR, &color);
	}
	else if (percentage > LOW_RAGE_THRESHOLD)
	{
		float transition_percent = (percentage - LOW_RAGE_THRESHOLD)
			/ (FULL_RAGE_THRESHOLD - LOW_RAGE_THRESHOLD);
		interp_rgba(transition_percent, LOW_RAGE_COLOR, FULL_RAGE_COLOR, &color);
	}
	else
		color = LOW_RAGE_COLOR;

	return color;
}

#define GLOW_MOVE_SPEED    60
#define GLOW_MOVE_RAGE 20

//
//
void drawHealthBar(float x, float y, float z, float width, float curr, float total, float scale, int frameColor, int barColor, int type)
{
	bool bDecorate;
	AtlasTex* icon = NULL;

	// This thing should probably be data driven instead of having all these
	// differently named vars and cascading ifs. --poz

	static float    last_hpercentage = 1.f, hglow_scale = 0.0f,
		last_absorb_percentage = 1.f, absorb_glow_scale = 0.0f,
		last_end_percentage = 1.f, end_glow_scale = 0.0f,
		last_ins_percentage = 1.f, ins_glow_scale = 0.0f,
		last_rage_percentage = 1.f, rage_glow_scale = 0.0f;

	static int    hmode = GLOW_NONE,
		absorb_mode = GLOW_NONE,
		end_mode = GLOW_NONE,
		ins_mode = GLOW_NONE,
		rage_mode = GLOW_NONE;

	float percentage = (float)(curr / total), glow_scale = 0.f;

	static AtlasTex* label_hp;
	static AtlasTex* label_end;
	static AtlasTex* label_ins;
	static AtlasTex* label_rage;
	static int texBindInit = 0;
	if (!texBindInit) {
		texBindInit = 1;
		label_hp = atlasLoadTexture("healthbar_label_hp.tga");
		label_end = atlasLoadTexture("healthbar_label_end.tga");
		label_ins = atlasLoadTexture("healthbar_label_end.tga"); // TODO
		label_rage = atlasLoadTexture("healthbar_label_dmg.tga"); // TODO
	}


	if (showRage)
	{
		bDecorate = (type != HEALTHBAR_EXPERIENCE_DEBT &&
			type != HEALTHBAR_HEALTH &&
			type != HEALTHBAR_ENDURANCE &&
			type != HEALTHBAR_INSIGHT &&
			type != HEALTHBAR_RAGE &&
			type != HEALTHBAR_EXPERIENCE);
	}
	else
	{
		bDecorate = type != HEALTHBAR_EXPERIENCE_DEBT;
	}

	if (percentage > 1.f)
		percentage = 1.f;

	if (type == HEALTHBAR_HEALTH)
	{
		handleGlowState(&hmode, &last_hpercentage, percentage, &hglow_scale, GLOW_MOVE_SPEED);
		percentage = last_hpercentage;
		glow_scale = hglow_scale;
		drawHeartBeatGlow(x, y + 4 * scale, z, percentage * (width - 8 * scale), percentage, barColor, scale);
		icon = label_hp;
	}
	else if (type == HEALTHBAR_ABSORB)
	{
		handleGlowState(&absorb_mode, &last_absorb_percentage, percentage, &absorb_glow_scale, GLOW_MOVE_SPEED);
		percentage = last_absorb_percentage;
		glow_scale = absorb_glow_scale;
	}
	else if (type == HEALTHBAR_ENDURANCE)
	{
		handleGlowState(&end_mode, &last_end_percentage, percentage, &end_glow_scale, GLOW_MOVE_SPEED);
		percentage = last_end_percentage;
		glow_scale = end_glow_scale;
		icon = label_end;
	}
	else if (type == HEALTHBAR_INSIGHT)
	{
		AtlasTex* bar = atlasLoadTexture("healthbar_slider.tga");

		handleGlowState(&ins_mode, &last_ins_percentage, percentage, &ins_glow_scale, GLOW_MOVE_SPEED);
		percentage = last_ins_percentage;
		glow_scale = ins_glow_scale;
		icon = label_ins;

		display_sprite(bar, x + 4 * scale, y + 4 * scale, z, 1.0 * (width - 8 * scale) / bar->width, scale, 0x00000088);
	}
	else if (type == HEALTHBAR_RAGE)
	{
		AtlasTex* bar = atlasLoadTexture("healthbar_slider.tga");
		handleGlowState(&rage_mode, &last_rage_percentage, percentage, &rage_glow_scale, GLOW_MOVE_RAGE);
		percentage = last_rage_percentage;
		glow_scale = rage_glow_scale;
		icon = label_rage;
		display_sprite(bar, x + 4 * scale, y + 4 * scale, z, 1.0 * (width - 8 * scale) / bar->width, scale, 0x00000088);
	}

	drawBar(x, y, z, width, scale, percentage, glow_scale, barColor, 0);

	// draw the frame
	if (bDecorate)
		drawHealthBarFrame(x, y, z - 2, width, scale, frameColor);

	display_sprite(icon, x + 5 * scale, y + 4 * scale, z + 2, scale, scale, 0xffffff88);

}

void checkEnableToolTips(CBox* box)
{
	if (mouseClickHit(box, MS_RIGHT))
		enableToolTips(true);
}


//----------------------------------------------------------------------------------------------------------
// Experience Stuff ////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

#define XP_INSET        10
#define XP_WD            222
#define XP_Y            43
#define HEALTH_YOFF        19
#define HEALTH_YOFF_SKILLS    17
#define HEALTH_XOFF        77.f
#define EMPTY_HOLE_CLR  0x061132ff
#define DEBT_HOLE_CLR    0x580000ff
#define REST_HOLE_CLR    0x114Fbfff  
#define XP_COLOR        0xd36efdff
#define XP_OFF_COLOR    0x000000ff
#define WISDOM_XP_COLOR 0x006efdff

void status_levelDialog()
{
	dialogStd(DIALOG_OK, "GoLevelUp", NULL, NULL, NULL, NULL, 1);
}

void drawExperienceFrame(float x, float y, float z, float wd, float scale, int color, int back_color)
{
	AtlasTex* left = atlasLoadTexture("healthbar_framemain_L.tga");
	AtlasTex* right = atlasLoadTexture("healthbar_framemain_R.tga");
	AtlasTex* pipe = atlasLoadTexture("healthbar_framemain_horiz.tga");
	AtlasTex* back = white_tex_atlas;
	AtlasTex* exp_back = atlasLoadTexture("healthbar_exp_black.tga");
	AtlasTex* exp_ring = atlasLoadTexture("healthbar_exp_ring.tga");
	AtlasTex* icon = atlasLoadTexture("healthbar_label_xp.tga");
	AtlasTex* task = atlasLoadTexture("healthbar_notify_taskforce.tga");
	Entity* e = playerPtr();

	// left bracket
	display_sprite(left, x + XP_INSET * scale, y + XP_Y * scale, z, scale, scale, color);
	display_sprite(icon, x + (XP_INSET + 4) * scale, y + (XP_Y + 3) * scale, z + 6, scale, scale, 0xffffff88);

	// middle
	display_sprite(pipe, x + (XP_INSET + left->width) * scale, y + (XP_Y + 2) * scale, z, (XP_WD - left->width) * scale / pipe->width, scale, color);
	display_sprite(pipe, x + (XP_INSET + left->width) * scale, y + (XP_Y - 2 + (left->height - pipe->height)) * scale, z, (XP_WD - left->width) * scale / pipe->width, scale, color);
	display_sprite(back, x + (XP_INSET + 4) * scale, y + (XP_Y + 4) * scale, z - 15, (float)(XP_WD - 5) * scale / back->width, (left->height - 8) * scale / back->height, back_color);

	// the bridge between bar and circle
	display_sprite(pipe, x + (XP_INSET + XP_WD) * scale, y + (XP_Y + 2) * scale, z, 8 * scale / pipe->width, scale, color);
	display_sprite(pipe, x + (XP_INSET + XP_WD) * scale, y + (XP_Y - 2 + (left->height - pipe->height)) * scale, z, 20 * scale / pipe->width, scale, color);

	// circular assembly
	display_sprite(exp_ring, x + wd - exp_ring->width * scale, y, z + 10, scale, scale, color);
	display_sprite(exp_back, x + wd - exp_ring->width * scale, y, z - 1, scale, scale, back_color);

	// task force
	if (e->pl->taskforce_mode)
	{
		if (e->onFlashback)
		{
			task = atlasLoadTexture("healthbar_notify_ouroboros.tga");
			display_sprite(task, x + wd - exp_ring->width * scale, y, z - 2, scale, scale, 0xffffffff);
		}
		else if (e->onArchitect)
		{
			task = atlasLoadTexture("healthbar_notify_architect.tga");
			display_sprite(task, x + wd - exp_ring->width * scale, y, z - 2, scale, scale, 0xffffffff);
		}
		else
			display_sprite(task, x + wd - exp_ring->width * scale, y, z - 2, scale, scale, (color & 0xffffff00) | 0x000000ff);
	}
	else if (e->pl->supergroup_mode && e->supergroup && e->supergroup->emblem)
	{
		AtlasTex* icon;
		char emblemTexName[256];
		float sc;

		if (e->supergroup->emblem[0] == '!')
		{
			emblemTexName[0] = 0;
		}
		else
		{
			strcpy(emblemTexName, "emblem_");
		}
		strcat(emblemTexName, e->supergroup->emblem);
		strcat(emblemTexName, ".tga");
		icon = atlasLoadTexture(emblemTexName);
		sc = 28.f / icon->width;

		display_sprite(icon, x + wd + (-icon->width * sc - exp_ring->width) * scale / 2, y + (exp_ring->width - icon->width * sc) * scale / 2, z - 2, sc, sc, 0xffffffff);
	}
}

enum
{
	XP_NONE,
	XP_NEW_TICK,
	XP_NEW_LEVEL,
};

#define PLUNGER_SPEED .01f

void drawExperience(float x, float y, float z, float wd, float scale, int color, int back_color)
{
	Entity* e = playerPtr();
	int i, experience_level, acquired_ticks, debt_ticks, xp_color;
	float next_level_xp, xp_towards_level, xp_per_tick, xp_towards_tick, xp_debt, percentage, glow_scale;
	CBox box;
	int bDebt = true;

	static float last_percentage = 0.0f, last_glow = 0.f;
	static int     xpmode, last_mode = GLOW_NONE, last_tick_num = 0;

	AtlasTex* levelup = atlasLoadTexture("healthbar_notify_levelup.tga");

	// get the static background out of the way
	drawExperienceFrame(x, y, z + 10, wd, scale, color, back_color);

	if (optionGet(kUO_NoXP))
		xp_color = XP_OFF_COLOR;
	else
		xp_color = XP_COLOR;
	experience_level = character_CalcExperienceLevel(e->pchar);

	if ((experience_level + 1) < MAX_PLAYER_SECURITY_LEVEL)
	{
		next_level_xp = g_ExperienceTables.aTables.piRequired[experience_level + 1] - g_ExperienceTables.aTables.piRequired[experience_level];
		xp_towards_level = e->pchar->iExperiencePoints - g_ExperienceTables.aTables.piRequired[experience_level];
	}
	else
	{
		// They can't level any more
		next_level_xp = 10;
		xp_towards_level = 10;
	}

	if (e->pchar->iExperienceRest > 0)
	{
		xp_debt = e->pchar->iExperienceRest;
		bDebt = false;
	}
	else {
		xp_debt = e->pchar->iExperienceDebt;
		bDebt = true;
	}


	xp_per_tick = (next_level_xp / 10);
	acquired_ticks = floor((xp_towards_level / next_level_xp) * 10);
	xp_towards_tick = xp_towards_level - acquired_ticks * xp_per_tick;
	debt_ticks = floor((xp_debt + xp_towards_tick) / xp_per_tick);
	percentage = xp_towards_tick / xp_per_tick;

	if (last_tick_num < acquired_ticks)
	{
		xpmode = XP_NEW_TICK;
		last_percentage = 0;
		last_mode = 0;
	}
	else if (last_tick_num > acquired_ticks)
	{
		xpmode = XP_NEW_LEVEL;
	}

	last_tick_num = acquired_ticks;

	if (xpmode == XP_NEW_LEVEL)
		acquired_ticks = 10;

	if (xpmode == XP_NEW_TICK || xpmode == XP_NEW_LEVEL)
	{
		AtlasTex* plunger = atlasLoadTexture("healthbar_exp_plunger.tga");
		AtlasTex* sweep = atlasLoadTexture("healthbar_exp_sweep.tga");

		float angle_max = -((28.f / 360) * acquired_ticks + (10.f / 360)) * 2 * PI;

		if (!last_mode)
			last_percentage += TIMESTEP * PLUNGER_SPEED;
		else
			last_percentage -= TIMESTEP * PLUNGER_SPEED * 4;

		if (!last_mode && last_percentage > 1.f)
			last_mode = 1;
		else if (last_mode && last_percentage < 0.f)
		{
			last_percentage = 0;
			xpmode = XP_NONE;
		}

		// draw bar
		set_scissor(TRUE);
		scissor_dims(x + (XP_INSET + 4) * scale, 0, (XP_WD + 6) * scale, 200000);
		if (!last_mode)
			drawBar(x + (XP_INSET + XP_WD * last_percentage) * scale, y + XP_Y * scale, z + 10, XP_WD * scale, scale, 1.f, 0, xp_color, 0);

		// draw plunger
		display_sprite(plunger, x + (XP_INSET + XP_WD * last_percentage + 4 - plunger->width) * scale, y + (XP_Y + 2) * scale, z + 5, scale, scale, CLR_WHITE);
		set_scissor(FALSE);

		// draw sweep
		if (!last_mode)
			display_rotated_sprite(sweep, x + wd - 64 * scale, y, z + 15, scale, scale, 0xffffff88, angle_max * (last_percentage * 5 - floor(last_percentage * 5)), 1);

		if (last_percentage > .2f)
			display_rotated_sprite(sweep, x + wd - 64 * scale, y, z + 15, scale, scale, (0xffffff00 | (int)(0x88 * (1 - (last_percentage * 5 - floor(last_percentage * 5))))), angle_max, 1);

		for (i = 0; i < 10 && window_getMode(WDW_STAT_BARS) == WINDOW_DISPLAYING; i++)
		{
			char sprName[128];
			AtlasTex* tick;
			int clr = CLR_WHITE;

			if (i < acquired_ticks)
			{
				sprintf(sprName, "healthbar_exp_dot_%02d.tga", i + 1);
				tick = atlasLoadTexture(sprName);

				if (!last_mode && i == acquired_ticks - 1)
					clr = 0xffffff00 | (int)(255.f * last_percentage);
			}
			else
			{
				sprintf(sprName, "healthbar_exp_dot_empty_%02d.tga", i + 1);
				tick = atlasLoadTexture(sprName);

				if (debt_ticks > 0)
				{
					if (bDebt)
						clr = DEBT_HOLE_CLR;
					else
						clr = REST_HOLE_CLR;
					debt_ticks--;
				}
				else
					clr = EMPTY_HOLE_CLR;
			}
			display_sprite(tick, x + wd - 64.3 * scale, y + 0.5 * scale, z + 13, scale, scale, clr);
		}
	}
	else
	{
		handleGlowState(&last_mode, &last_percentage, percentage, &last_glow, GLOW_MOVE_SPEED);
		percentage = last_percentage;
		glow_scale = last_glow;

		drawBar(x + XP_INSET * scale, y + XP_Y * scale, z, XP_WD * scale, scale, percentage, glow_scale, xp_color, 0);
		drawBar(x + (XP_INSET + percentage * (XP_WD - 8)) * scale, y + XP_Y * scale, z + 10, MIN((1 - percentage), (xp_debt / xp_per_tick)) * (XP_WD - 8 + 18) * scale, scale, 1.f, 0.f, bDebt ? DEBT_HOLE_CLR : REST_HOLE_CLR, 0);

		BuildCBox(&box, x + XP_INSET * scale, y + XP_Y * scale, XP_WD * scale, 15);
		setToolTip(&ExperienceTip, &box, textStd("ExperienceTip", (int)xp_towards_level, (int)next_level_xp, e->pchar->iExperienceDebt, e->pchar->iExperienceRest), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);

		for (i = 0; i < 10 && window_getMode(WDW_STAT_BARS) == WINDOW_DISPLAYING; i++)
		{
			char sprName[128];
			AtlasTex* tick;
			int clr = CLR_WHITE;

			if (i < acquired_ticks)
			{
				STR_COMBINE_BEGIN(sprName);
				STR_COMBINE_CAT("healthbar_exp_dot_");
				STR_COMBINE_CAT_D2(i + 1);
				STR_COMBINE_CAT(".tga");
				STR_COMBINE_END();
				tick = atlasLoadTexture(sprName);
			}
			else
			{
				STR_COMBINE_BEGIN(sprName);
				STR_COMBINE_CAT("healthbar_exp_dot_empty_");
				STR_COMBINE_CAT_D2(i + 1);
				STR_COMBINE_CAT(".tga");
				STR_COMBINE_END();
				tick = atlasLoadTexture(sprName);

				if (debt_ticks > 0)
				{
					if (bDebt)
						clr = DEBT_HOLE_CLR;
					else
						clr = REST_HOLE_CLR;
					debt_ticks--;
				}
				else
					clr = EMPTY_HOLE_CLR;
			}
			display_sprite(tick, x + wd - 64.3 * scale, y + 0.5 * scale, z + 13, scale, scale, clr);
		}
	}

	if (e->pchar->iLevel != character_CalcExperienceLevel(e->pchar))
		status_addNotification(levelup, status_levelDialog, NULL);
	else
		status_removeNotification(levelup);
}

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

static int gReverseBuffs;
//
//
static float status_drawEffects(Entity* e, float x, float y, float z, float wd, float scale)
{
	static char* apchEffects[] = { "FloatHeld",
									"FloatStunned",
									"FloatAsleep",
									"FloatTerrorized",
									"FloatImmobilized",
									"FloatConfused",
									"FloatOnlyAffectsSelf",
									"FloatTaunted",
									"FloatHidden",
									"FloatBlinded" };

	int i;
	float sc = 1.3 * scale;
	float yoff = y + 14 * sc;
	TTDrawContext* pFont = &entityNameText;
	font(pFont);
	for (i = 0; i < 10; i++)
	{
		// One-off green coloring for hidden, need better solution if we get more of these
		if (i == 8)
			font_color(0x00f000ff, 0x00ff0080);
		else
			font_color(0xf00000ff, 0xff000080);

		if (e->ulStatusEffects & (1 << i)) {
			int w = str_wd(pFont, sc, sc, apchEffects[i]);
			prnt(x + wd - (w + 7), yoff, z, sc, sc, apchEffects[i]);

			if (gReverseBuffs)
				yoff -= 13 * sc;
			else
				yoff += 13 * sc;
		}
	}

	if (e->pchar)
	{
		for (i = 0; i < eaSize(&e->pchar->designerStatuses); i++)
		{
			char* localizedEffect = textStd(e->pchar->designerStatuses[i]);
			int w = str_wd(pFont, sc, sc, localizedEffect);

			font_color(0xf00000ff, 0xff000080);
			prnt(x + wd - (w + 7), yoff, z, sc, sc, localizedEffect);

			if (gReverseBuffs)
				yoff -= 13 * sc;
			else
				yoff += 13 * sc;
		}
	}
	return yoff;
}

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

// Helper function that takes a number of seconds left and turns it into a string
char* calcTimeRemainingString(int seconds, bool pad_minutes)
{
	static char timer[9];
	timer[0] = 0;
	// Put a limit on what we display xx:xx:xx
	seconds = MAX(seconds, 0);
	seconds = MIN(seconds, 359999);
	if (seconds >= 3600)
	{
		strcatf(timer, "%i:", seconds / 3600);
		seconds %= 3600;
		strcatf(timer, "%d", seconds / 600);
		seconds %= 600;
		if (pad_minutes)
			strcatf(timer, "%d:00", seconds / 60);
		else
			strcatf(timer, "%d", seconds / 60);
	}
	else {
		seconds %= 3600;
		strcatf(timer, "%d", seconds / 600);
		seconds %= 600;
		strcatf(timer, "%d:", seconds / 60);
		seconds %= 60;
		strcatf(timer, "%d", seconds / 10);
		seconds %= 10;
		strcatf(timer, "%d", seconds);
	}
	return timer;
}


#define CHALLENGE_BUFF_SIZE        256
#define H_SPACING                2.0f
#define FONT_SCALING            1.0f
static float status_drawChallenges(Entity* e, float x, float y, float z, float wd, float scale, int flip)
{
	float yoff = y;
	float xoff = x + wd - H_SPACING * scale;
	int i, w, h;
	int time;
	CBox box;
	bool found = false;
	AtlasTex* ptex = NULL;
	AtlasTex* line = white_tex_atlas;
	TTDrawContext* pFont = &entityNameText;
	char buf[CHALLENGE_BUFF_SIZE];
	int color = 0xffffffff;
	float fontScale = FONT_SCALING * scale;

	static ToolTip DeathsTip = { 0 }, TimeLimitTip = { 0 }, BuffTip = { 0 }, DebuffTip = { 0 },
		DisablePowerTip = { 0 }, DisableEnhancementsTip = { 0 };
	static ToolTipParent ChallengeTipParent = { 0 };

	clearToolTip(&DeathsTip);
	clearToolTip(&TimeLimitTip);
	clearToolTip(&BuffTip);
	clearToolTip(&DebuffTip);
	clearToolTip(&DisablePowerTip);
	clearToolTip(&DisableEnhancementsTip);

	if (!e->pl->taskforce_mode)
		return y;

	font(pFont);
	font_color(0xffffffff, 0x000ff0ff);
	if (flip)
		yoff += 8.0 * scale;
	else
		yoff += 14.0 * scale;
	for (i = TFPARAM_INVALID + 1; i < TFPARAM_NUM; i++)
	{
		if (TaskForceIsParameterEnabled(e, i))
		{
			found = true;
			switch (i)
			{
			case TFPARAM_DEATHS:
				if (TaskForceGetParameter(e, i) <= TaskForceGetParameter(e, TFPARAM_DEATHS_MAX_VALUE))
				{
					sprintf_s(buf, CHALLENGE_BUFF_SIZE, "%d", TaskForceGetParameter(e, TFPARAM_DEATHS_MAX_VALUE) - TaskForceGetParameter(e, i));
				}
				else
				{
					sprintf_s(buf, CHALLENGE_BUFF_SIZE, "X");
				}
				w = str_wd(pFont, fontScale, fontScale, buf);
				h = str_height(pFont, fontScale, fontScale, false, buf);
				if (TaskForceGetParameter(e, TFPARAM_DEATHS_MAX_VALUE) > 0)
				{
					ptex = atlasLoadTexture("challengeStat_limit_defeat.tga");
				}
				else
				{
					ptex = atlasLoadTexture("challengeStat_no_defeat.tga");
				}
				xoff -= (w + H_SPACING * scale);
				prnt(xoff, yoff + h * 0.5f, z, fontScale, fontScale, buf);
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&DeathsTip, &box, "DefeatsTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&DeathsTip);

				//                    display_sprite( line, xoff, yoff  - (ptex->height * scale *0.35f), z, ptex->width*scale*0.7f/8, ptex->height*scale*0.7f/8, CLR_WHITE );
				//                    display_sprite( line, xoff, yoff, z, 65.0f*scale, 1.5f*scale/line->height, CLR_BLUE );
				break;
			case TFPARAM_TIME_LIMIT:
				ptex = atlasLoadTexture("challengeStat_limit_time.tga");
				time = TaskForceGetParameter(e, i) - timerSecondsSince2000();
				strcpy_s(buf, CHALLENGE_BUFF_SIZE, calcTimeRemainingString(time, false));
				w = str_wd(pFont, fontScale, fontScale, buf);
				h = str_height(pFont, fontScale, fontScale, false, buf);
				xoff -= (w + H_SPACING * scale);
				prnt(xoff, yoff + h * 0.5f, z, fontScale, fontScale, buf);
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&TimeLimitTip, &box, "TimeLimitTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&TimeLimitTip);
				break;
			case TFPARAM_TIME_COUNTER:
				break;
			case TFPARAM_DEBUFF_PLAYERS:
				ptex = atlasLoadTexture("challengeStat_debuff_player.tga");
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&DebuffTip, &box, "DebuffTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&DebuffTip);
				break;
			case TFPARAM_DISABLE_POWERS:
			{
				// need to switch based on what powers are disabled
				int type = TaskForceGetParameter(e, i);

				switch (type)
				{
				case TFPOWERS_NO_TEMP:
					ptex = atlasLoadTexture("challengeStat_no_power_temp.tga");
					break;
				case TFPOWERS_NO_TRAVEL:
					ptex = atlasLoadTexture("challengeStat_no_power_travel.tga");
					break;
				case TFPOWERS_NO_EPIC_POOL:
					ptex = atlasLoadTexture("challengeStat_no_power_epic.tga");
					break;
				case TFPOWERS_ONLY_AT:
					ptex = atlasLoadTexture("challengeStat_limit_power_at_only.tga");
					break;
				}
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				switch (type)
				{
				case TFPOWERS_NO_TEMP:
					setToolTip(&DisablePowerTip, &box, "NoTempTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
					break;
				case TFPOWERS_NO_TRAVEL:
					setToolTip(&DisablePowerTip, &box, "NoTravelTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
					break;
				case TFPOWERS_NO_EPIC_POOL:
					setToolTip(&DisablePowerTip, &box, "NoEpicPoolTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
					break;
				case TFPOWERS_ONLY_AT:
					setToolTip(&DisablePowerTip, &box, "ATOnlyTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
					break;
				}
				addToolTip(&DisablePowerTip);
			}
			break;
			case TFPARAM_DISABLE_ENHANCEMENTS:
				ptex = atlasLoadTexture("challengeStat_no_enhance.tga");
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&DisableEnhancementsTip, &box, "EnhancementsDisabledTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&DisableEnhancementsTip);
				break;
			case TFPARAM_NO_INSPIRATIONS:
				ptex = atlasLoadTexture("challengeStat_no_inspire.tga");
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&DisableEnhancementsTip, &box, "InspirationsDisabledTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&DisableEnhancementsTip);
				break;
			case TFPARAM_BUFF_ENEMIES:
				ptex = atlasLoadTexture("challengeStat_debuff_enemy.tga");
				xoff -= (ptex->width * scale * 0.7f + H_SPACING * scale);
				display_sprite(ptex, xoff, yoff - (ptex->height * scale * 0.35f), z, scale * 0.7f, scale * 0.7f, color);

				// tooltip 
				BuildCBox(&box, xoff, yoff - (ptex->height * scale * 0.35f), ptex->width * scale * 0.7f, ptex->height * scale * 0.7f);
				setToolTip(&BuffTip, &box, "BuffTooltip", NULL, MENU_GAME, WDW_STAT_BARS);
				addToolTip(&BuffTip);
				break;
			}
		}
	}

	if (found)
		if (flip)
			return y - (yoff + 14.0 * scale - y);
		else
			return y + (yoff + 8.0 * scale - y);
	else
		return y;
}


//------------------------------------------------------------------------------------------------------------
// Status window control
//------------------------------------------------------------------------------------------------------------
#define DEBT_XOFF        12
#define HEALTH_SPACING    15
#define MAX_XP            2000
#define BASE_ANGLE        PI/5


int rageVisible(void* foo)
{
	Entity* e = playerPtr();
	if (strncmp(e->pchar->pclass->pchName, "Class_Blaster", 13) == 0) return CM_HIDE;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Brute", 11) == 0) return CM_VISIBLE;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Dominator", 15) == 0) return CM_VISIBLE;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Primalist", 15) == 0) return CM_VISIBLE;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Rescued_Devoured", 22) == 0) return CM_VISIBLE;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Sentinel", 14) == 0) return CM_VISIBLE;
	return CM_HIDE;
}

//
//
static const char * statuscm_hpAdapter(void* arg0)
{
    return statuscm_hp((void*)arg0);
}

static const char * statuscm_endAdapter(void* arg0)
{
    return statuscm_end((void*)arg0);
}

static const char * statuscm_rageAdapter(void* arg0)
{
    return statuscm_rage((void*)arg0);
}

static const char * statuscm_xpAdapter(void* arg0)
{
    return statuscm_xp((void*)arg0);
}

static const char * statuscm_debtAdapter(void* arg0)
{
    return statuscm_debt((void*)arg0);
}

static const char * statuscm_restAdapter(void* arg0)
{
    return statuscm_rest((void*)arg0);
}

int statusWindow()
{
	static AtlasTex* heroIcon = 0, * vigilanteIcon = 0, * villainIcon = 0, * rogueIcon = 0, * loyalistIcon = 0, * resistanceIcon = 0, * neutralPrimalIcon = 0, * ringIcon = 0;
	AtlasTex* displayIcon;
	int        color, bcolor;
	float    x, y, z, hp, hp_total, end, end_total, meter, meter_total, scale, wd, ht, buff_spacer;
	Entity* e = playerPtr();
	CBox    box;
	int align_yoff, health_yoff, bar_ht, bar_ht_2 = 0, overStatus = 0;
	float finalY;

	if (!e)
		return 0;

	if (!heroIcon)
	{
		heroIcon = atlasLoadTexture("Align_Status_Hero.texture");
		vigilanteIcon = atlasLoadTexture("Align_Status_Vigilante.texture");
		villainIcon = atlasLoadTexture("Align_Status_Villain.texture");
		rogueIcon = atlasLoadTexture("Align_Status_Rogue.texture");
		loyalistIcon = atlasLoadTexture("Align_Status_Loyalist.texture");
		resistanceIcon = atlasLoadTexture("Align_Status_Resistance.texture");
		neutralPrimalIcon = atlasLoadTexture("Align_Status_Neutral.texture");
		ringIcon = atlasLoadTexture("tray_ring_power.tga");
	}

	if (!e || !e->pchar || !e->pchar->pclass)
		showRage = 0;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Blaster", 13) == 0)
		showRage = 0;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Brute", 11) == 0)
		showRage = 2;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Dominator", 15) == 0)
		showRage = 3;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Primalist", 15) == 0)
		showRage = 4;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Rescued_Devoured", 22) == 0)
		showRage = 5;
	else if (strncmp(e->pchar->pclass->pchName, "Class_Sentinel", 14) == 0)
		showRage = 6;
	else
		showRage = 0;

	addToolTip(&HealthTip);
	addToolTip(&EnduranceTip);
	addToolTip(&ExperienceTip);
	addToolTip(&RageTip);
	addToolTip(&NotifyTip);

	if (!gXpcm)
	{
		gXpcm = contextMenu_Create(NULL);
		contextMenu_addVariableText(gXpcm, statuscm_hpAdapter, NULL);
		contextMenu_addVariableText(gXpcm, statuscm_endAdapter, NULL);
		contextMenu_addVariableTextVisible(gXpcm, rageVisible, NULL, statuscm_rageAdapter, NULL);
		contextMenu_addVariableText(gXpcm, statuscm_xpAdapter, NULL);
		contextMenu_addCode(gXpcm, levelingpact_IsInPact, 0, levelingpact_openWindow, 0, "LevelingpactTip", 0);
		contextMenu_addVariableText(gXpcm, statuscm_debtAdapter, NULL);
		contextMenu_addVariableText(gXpcm, statuscm_restAdapter, NULL);
	}

	if (!window_getDims(WDW_STAT_BARS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor) || baseedit_Mode())
		return 0;

	BuildCBox(&box, x, y, wd, ht);
	if (mouseCollision(&box))
	{
		overStatus = 1;
		if (mouseClickHit(&box, MS_RIGHT))
			contextMenu_display(gXpcm);
	}

	if (showRage)
	{
		align_yoff = 13;
		health_yoff = HEALTH_YOFF_SKILLS;
		bar_ht_2 = BAR_HT_2_SKILLS;
		bar_ht = BAR_HT_SKILLS;
	}
	else
	{
		align_yoff = 9;
		health_yoff = HEALTH_YOFF;
		bar_ht_2 = BAR_HT_2;
		bar_ht = BAR_HT;
	}

	// alignment icon (need to show their old alignment until they've arrived)
	if (ENT_IS_IN_PRAETORIA(e) || ENT_IS_LEAVING_PRAETORIA(e))
	{
		if (ENT_IS_VILLAIN(e))
		{
			displayIcon = loyalistIcon;
		}
		else
		{
			displayIcon = resistanceIcon;
		}
	}
	else
	{
		if (e->pl->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial)
		{
			displayIcon = neutralPrimalIcon;
		}
		else if (ENT_IS_VILLAIN(e))
		{
			if (ENT_IS_ROGUE(e))
			{
				displayIcon = rogueIcon;
			}
			else
			{
				displayIcon = villainIcon;
			}
		}
		else
		{
			if (ENT_IS_ROGUE(e))
			{
				displayIcon = vigilanteIcon;
			}
			else
			{
				displayIcon = heroIcon;
			}
		}
	}

	display_sprite(displayIcon, x + 2 * scale, y + align_yoff * scale, z + 25, scale, scale, 0xffffffff);

	// absorb/hp bar
	hp_total = e->pchar->attrMax.fHitPoints + e->pchar->attrMax.fAbsorb;

	hp = e->pchar->attrCur.fHitPoints + e->pchar->attrMax.fAbsorb;
	drawHealthBar(x + 34 * scale, y + health_yoff * scale, z + 25, wd - HEALTH_XOFF * scale, hp, hp_total, scale, color, getHealthColor(true, e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints), HEALTHBAR_HEALTH);

	hp = e->pchar->attrMax.fAbsorb;
	drawHealthBar(x + 34 * scale, y + health_yoff * scale, z + 25, wd - HEALTH_XOFF * scale, hp, hp_total, scale, color, getAbsorbColor(e->pchar->attrMax.fAbsorb / e->pchar->attrMax.fHitPoints), HEALTHBAR_ABSORB);

	BuildCBox(&box, x + 34 * scale, y + (health_yoff + 4) * scale, wd - 82 * scale, bar_ht_2 * scale);
	if (overStatus)
	{
		if (e->pchar->attrMax.fAbsorb > 0)
			setToolTip(&HealthTip, &box, textStd("HealthAndAbsorbTip", e->pchar->attrCur.fHitPoints, e->pchar->attrMax.fAbsorb, e->pchar->attrMax.fHitPoints), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
		else
			setToolTip(&HealthTip, &box, textStd("HealthTip", e->pchar->attrCur.fHitPoints, e->pchar->attrMax.fHitPoints), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
	}

	// endurance bar
	end = e->pchar->attrCur.fEndurance;
	end_total = e->pchar->attrMax.fEndurance;
	drawHealthBar(x + 34 * scale, y + (health_yoff + bar_ht) * scale, z + 25, wd - HEALTH_XOFF * scale, end, end_total, scale, color, getEnduranceColor(end / end_total), HEALTHBAR_ENDURANCE);

	BuildCBox(&box, x + 34 * scale, y + (health_yoff + bar_ht + 4) * scale, wd - 82 * scale, bar_ht_2 * scale);
	if (overStatus)
		setToolTip(&EnduranceTip, &box, textStd("EnduranceTip", (int)end, (int)end_total), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);

	if (showRage)
	{
		// rage bar
		meter = e->pchar->attrCur.fMeter;
		meter_total = e->pchar->attrMax.fMeter;
		drawHealthBar(x + 34 * scale, y + (health_yoff + bar_ht * 2) * scale, z + 25, wd - HEALTH_XOFF * scale, meter, meter_total, scale, color, getRageColor(meter / meter_total), HEALTHBAR_RAGE);

		BuildCBox(&box, x + 34 * scale, y + (health_yoff + bar_ht * 2 + 4) * scale, wd - 82 * scale, bar_ht_2 * scale);
		if (overStatus) {
			if (showRage == 1)
				setToolTip(&RageTip, &box, textStd("DesperationTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
			else if (showRage == 2)
				setToolTip(&RageTip, &box, textStd("RageTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
			else if (showRage == 3)
				setToolTip(&RageTip, &box, textStd("DominationTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
			else if (showRage == 4)
				setToolTip(&RageTip, &box, textStd("PrimalEnergyTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
			else if (showRage == 5)
				setToolTip(&RageTip, &box, textStd("BattleEuphoriaTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
			else if (showRage == 6)
				setToolTip(&RageTip, &box, textStd("OpportunityTip", (int)(meter * 100), (int)(meter_total * 100)), &StatusTipParent, MENU_GAME, WDW_STAT_BARS);
		}

		drawFrame(PIX2, R4, x + 36 * scale, y + (health_yoff + 2) * scale, z + 22, wd - (HEALTH_XOFF + 4) * scale, (bar_ht * 3 + 4) * scale, scale, 0, CLR_BLACK);
		drawFrame(PIX2, R4, x + 36 * scale, y + (health_yoff + 2) * scale, z + 30, wd - (HEALTH_XOFF + 4) * scale, (bar_ht * 3 + 4) * scale, scale, color, 0);
		BuildCBox(&box, x + 37 * scale, y + (health_yoff + 2) * scale + ((bar_ht * 3 + 4) * scale) / 3, wd - (HEALTH_XOFF + 6) * scale, bar_ht);
		drawBox(&box, z + 21, 0xffffff44, 0);
	}
	else
	{
		clearToolTip(&RageTip);
	}

	// experience
	drawExperience(x + 35 * scale, y, z, wd - 35 * scale, scale, color, bcolor);
	status_notify(x + wd - 32 * scale, y + 32 * scale, z + 15, scale);

	// Draw buff list
	buff_spacer = status_drawBuffs(e, x, y, z, wd, ht, scale, gReverseBuffs, &StatusTipParent);

	// challenge parameters
	buff_spacer = status_drawChallenges(e, x, buff_spacer, z, wd, scale, gReverseBuffs);

	// status effects
	finalY = status_drawEffects(e, x, buff_spacer, z, wd, scale);

	if (gReverseBuffs)
	{
		if (finalY < 0)
			gReverseBuffs = false;
	}
	else
	{
		int w, h;
		windowClientSizeThisFrame(&w, &h);
		if (finalY > h)
			gReverseBuffs = true;
	}

	return 0;
}
/* End of File */
