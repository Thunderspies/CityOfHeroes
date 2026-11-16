#############################################################
## Praetorian Encounters
##
## Interrupts
## ----------------------------------------------
## Include sequencers\Interrupts\Encounters_Idle.inc - Basic Encounter move
## Include sequencers\Interrupts\Encounters_Scripted.inc - Encounter move for cutscene or any situation where it MUST play
##
## Members
## ----------------------------------------------
## Include sequencers\Members\Encounters_Idle.inc - Basic Encounter move.  Interrptible by other Encounter moves.
## Include sequencers\Members\Encounters_Cycles.inc - Not interruptible by other Encounters.  Must use with Requires bits
## Include sequencers\Members\Encounters_Scripted.inc - Only interruptible by death
##
#############################################################

## Rigid Attention ###########################################################

Move Rigid_Attention_Cycle_One
	Requires	ENCOUNTER POSE STAND BUSY
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_Two

	Type		male
	  Anim          male\stand_rigid 1 120
	TEnd

MEnd

Move Rigid_Attention_Cycle_Two
	Requires	ENCOUNTER POSE STAND BUSY
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_Two
	CycleMove	Rigid_Attention_Cycle_One

	Type		male
	  Anim          male\stand_rigid 120 180
	TEnd
MEnd

## Contemplate ###########################################################

Move Contemplate_Cycle_One
	Requires	ENCOUNTER POSE THINK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Contemplate_Cycle_One
	CycleMove	Contemplate_Cycle_One
	CycleMove	Contemplate_Cycle_One
	CycleMove	Contemplate_Cycle_Two

	Type		male
	  Anim          male\contemplate 20 80
	TEnd

	Type		huge
	  Anim          huge\contemplate 20 80
	TEnd

MEnd

Move Contemplate_Cycle_Two
	Requires	ENCOUNTER POSE THINK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Contemplate_Cycle_Two
	CycleMove	Contemplate_Cycle_One
	CycleMove	Contemplate_Cycle_One
	CycleMove	Contemplate_Cycle_One

	Type		male
	  Anim          male\contemplate 80 197
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 90
	TEnd


	Type		huge
	  Anim          huge\contemplate 80 197
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 90
	TEnd

MEnd
## PPD Gauntlet Type  ###########################################################

Move PPD_Gauntlet_Type
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Cycle_Stand

	Type		male
	  Anim          male\ppd_stand_type 1 23
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 1 23
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 1 23
	TEnd
MEnd

Move PPD_Gauntlet_Cycle_Stand
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Cycle_Stand
	CycleMove	PPD_Gauntlet_Type_Pre
	CycleMove	PPD_Gauntlet_Type_Pre

	Type		male
	  Anim          male\ppd_stand_type 23 102
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 23 102
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 23 102
	TEnd
MEnd

Move PPD_Gauntlet_Type_Pre
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Type_Cycle
	Scale		0.75

	Type		male
	  Anim          male\ppd_stand_type 102 121
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 102 121
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 102 121
	TEnd
MEnd

Move PPD_Gauntlet_Type_Cycle
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Type_Cycle
	CycleMove	PPD_Gauntlet_Type_Cycle
	CycleMove	PPD_Gauntlet_Type_Cycle
	CycleMove	PPD_Gauntlet_Type_Post

	Type		male
	  Anim          male\ppd_stand_type 121 175
	  PlayFX ENEMYFX\V_PraetorianPolice\ForceGlove\Gauntlet_Type_SFX.fx 4
	  PlayFX ENEMYFX\V_PraetorianPolice\ForceGlove\Gauntlet_Type_SFX.fx 19
	  PlayFX ENEMYFX\V_PraetorianPolice\ForceGlove\Gauntlet_Type_SFX.fx 34
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 121 175
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 121 175
	TEnd

MEnd

Move PPD_Gauntlet_Type_Post
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Type_Post_Stand
	CycleMove	PPD_Gauntlet_Type_Post_Stand
	CycleMove	PPD_Gauntlet_Cycle_Stand
	CycleMove	PPD_Gauntlet_Cycle_Stand
	Scale		0.75

	Type		male
	  Anim          male\ppd_stand_type 175 204
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 175 204
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 175 204
	TEnd

MEnd

Move PPD_Gauntlet_Type_Post_Stand
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Type_Cycle_Center_Stance
	CycleMove	PPD_Gauntlet_Type_Cycle_Center_Stance
	CycleMove	PPD_Gauntlet_Type_Cycle_Center_Stance
	CycleMove	PPD_Gauntlet_Type
	Scale		0.75

	Type		male
	  Anim          male\ppd_stand_type 204 243
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 204 243
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 204 243
	TEnd

MEnd

Move PPD_Gauntlet_Type_Cycle_Center_Stance
	Requires	ENCOUNTER PPD PATROL TOUCHSCREEN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PPD_Gauntlet_Type_Cycle_Center_Stance
	CycleMove	PPD_Gauntlet_Type_Cycle_Center_Stance
	CycleMove	PPD_Gauntlet_Type
	CycleMove	PPD_Gauntlet_Type

	Type		male
	  Anim          male\ready 1
	TEnd

MEnd

## PPD Gauntlet Cover  ###########################################################

Move PPD_Gauntlet_Cover
	Requires	ENCOUNTER PPD CAPTURED
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover_Look_Left
	CycleMove 	PPD_Gauntlet_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 1 60
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 1 60
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd
MEnd

Move PPD_Gauntlet_Cover_Look_Right
	Requires	ENCOUNTER PPD CAPTURED
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover_Look_Left
	CycleMove 	PPD_Gauntlet_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 60 120
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 60 120
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd
MEnd

Move PPD_Gauntlet_Cover_Look_Left
	Requires	ENCOUNTER PPD CAPTURED
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover
	CycleMove 	PPD_Gauntlet_Cover_Look_Left
	CycleMove 	PPD_Gauntlet_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 120 180
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 120 180
	  PlayFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Glow.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Praetorian Salute
#############################################################

Move PratorianSalute
	Requires	ENCOUNTER SALUTE THREE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	PratorianSalute_PostCycle
	Interpolate	5
	Flags		FIXARMREG NoSizeScale

	Type		male
	  Anim          male\PraetorianSalute 1 65
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 15
	  PlayFX		NPCFX\NPC_ChestThump_SFX.fx 27
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 40
	TEnd

MEnd

Move PratorianSalute_PostCycle
	Requires	ENCOUNTER SALUTE THREE
	Flags		CYCLE FIXARMREG NoSizeScale
	Scale		0.75
	NextMove	PratorianSalute_AtEase

	Type		male
	  Anim          male\PraetorianSalute 65 125
	TEnd

MEnd

#############################################################

Move PratorianSalute_Post
	Requires	ENCOUNTER SALUTE HIGH LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	PratorianSalute_AtEase_Cycle
	Flags		FIXARMREG NoSizeScale

	Type		male
	  Anim          male\PraetorianSalute 125
	TEnd

MEnd

Move PratorianSalute_AtEase_Cycle
	Requires	ENCOUNTER SALUTE HIGH LOW
	Flags		CYCLE FIXARMREG NoSizeScale

	Type		male
	  Anim          male\PraetorianSalute 175 175
	TEnd

MEnd

### Recruit ###########################################################

Move Recruit_One
	Requires	ENCOUNTER POSE RECRUIT
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Recruit_One
	CycleMove	Recruit_Cycle_Four
	CycleMove	Recruit_Cycle_Two
	CycleMove	Recruit_Cycle_Three

	Type		male
	  Anim          male\praetorian_recruit 1 140
	TEnd
MEnd

Move Recruit_Cycle_Two
	Requires	ENCOUNTER POSE RECRUIT
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Recruit_One
	CycleMove	Recruit_Cycle_Four
	CycleMove	Recruit_Cycle_Two
	CycleMove	Recruit_Cycle_Three

	Type		male
	  Anim          male\praetorian_recruit 140 285
	TEnd
MEnd

Move Recruit_Cycle_Three
	Requires	ENCOUNTER POSE RECRUIT
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Recruit_One
	CycleMove	Recruit_Cycle_Four
	CycleMove	Recruit_Cycle_Two
	CycleMove	Recruit_Cycle_Three

	Type		male
	  Anim          male\praetorian_recruit 285 472
	TEnd

	Type		huge
	  Anim          huge\praetorian_recruit 285 472
	TEnd

MEnd

Move Recruit_Cycle_Four
	Requires	ENCOUNTER POSE RECRUIT
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Recruit_One
	CycleMove	Recruit_Cycle_Four
	CycleMove	Recruit_Cycle_Two
	CycleMove	Recruit_Cycle_Three
	Scale		0.5

	Type		male
	  Anim          male\idle_2hips_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_2hips_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_2hips_look 1
	TEnd
MEnd
### Threaten ###########################################################

Move Threaten_Cycle_One
	Requires	ENCOUNTER POSE THREAT FEAR
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Four

	Type		male
	  Anim          male\praetorian_threaten 1 76
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 40
	TEnd
MEnd

Move Threaten_Cycle_Two
	Requires	ENCOUNTER POSE THREAT FEAR
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_One
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Four

	Type		male
	  Anim          male\praetorian_threaten 76 155
	TEnd
MEnd

Move Threaten_Cycle_Three
Requires	ENCOUNTER POSE THREAT FEAR
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_Four
	CycleMove	Threaten_Cycle_Four
	CycleMove	Threaten_Cycle_Four
	CycleMove	Threaten_Cycle_Four

	Type		male
	  Anim          male\praetorian_threaten 155 275
	TEnd
MEnd

Move Threaten_Cycle_Four
	Requires	ENCOUNTER POSE THREAT FEAR
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_Five
	CycleMove	Threaten_Cycle_Six
	CycleMove	Threaten_Cycle_Four
	CycleMove	Threaten_Cycle_Three

	Type		male
	  Anim          male\praetorian_threaten 1 1
	TEnd
MEnd

Move Threaten_Cycle_Five
	Requires	ENCOUNTER POSE THREAT FEAR
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_One
	CycleMove	Threaten_Cycle_Four
	CycleMove	Threaten_Cycle_Three
	CycleMove	Threaten_Cycle_Three

	Type		male
	  Anim          male\praetorian_threaten 1 1
	TEnd
MEnd

Move Threaten_Cycle_Six
	Requires	ENCOUNTER POSE THREAT FEAR
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Threaten_Cycle_Five
	CycleMove	Threaten_Cycle_Five
	CycleMove	Threaten_Cycle_Five
	CycleMove	Threaten_Cycle_Five

	Type		male
	  Anim          male\praetorian_threaten 155 275
	TEnd
MEnd


#############################################################
## CLOCKWORK Power Down
#############################################################

Move Clockwork_PowerDown
	Requires	ENCOUNTER POSE SLEEP DOWN
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Clockwork_PowerDown_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\Clockwork_Powerdown 1 56
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_PowerDown_SFX.fx
	TEnd
MEnd

Move Clockwork_PowerDown_PostCycle
	Requires	ENCOUNTER POSE SLEEP DOWN
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	Flags		CYCLE NoSizeScale

	Type		male
	  Anim          male\Clockwork_Powerdown 56
	TEnd
MEnd

#############################################################
## CLOCKWORK Power Up
#############################################################

Move Clockwork_PowerUp
	Requires	ENCOUNTER POSE SLEEP UP
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Clockwork_PowerUp_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\Clockwork_Powerup 1 56
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_PowerUp_SFX.fx
	TEnd
MEnd

Move Clockwork_PowerUp_PostCycle
	Requires	ENCOUNTER POSE SLEEP UP
	Include sequencers\Members\Reactions.inc
	Interrupts	<HitIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\Clockwork_Powerup 56
	TEnd
MEnd


###################
### SprayPaint ####
###################

Move SprayPaint_A
	Requires	ENCOUNTER SPRAYCAN KNEEL LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       SprayPaint_A2

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0	Constant
	TEnd
MEnd

Move SprayPaint_A2
	Requires	ENCOUNTER SPRAYCAN KNEEL LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       SprayPaint_A3

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 1 40
	  PlayFX	Generic\spraycan.fx 0	Constant
	TEnd
MEnd

Move SprayPaint_A3
	Requires	ENCOUNTER SPRAYCAN KNEEL LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       SprayPaint_A4

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 40 137
	  PlayFX	Generic\spraycan.fx 0	Constant
	  PlayFX	Generic\spraycan_shake_SFX.fx 5
	TEnd
MEnd

Move SprayPaint_A4
	Requires	ENCOUNTER SPRAYCAN KNEEL LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       SprayPaint_A5
	CycleMove       SprayPaint_A3

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 137 218
	  PlayFX	Generic\spraycan.fx 0	Constant
  	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

Move SprayPaint_A5
	Requires	ENCOUNTER SPRAYCAN KNEEL LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       SprayPaint_A5
	CycleMove       SprayPaint_A3

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 152 218
	  PlayFX	Generic\spraycan.fx 0	Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

###################
### Praetorian Peace Stand ####
###################

Move Praetorian_Peace_Stand_Cycle
	Requires	ENCOUNTER POSE STAND BOWING
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\praetorian_peace_stand 1
  	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	TEnd

MEnd

############################
###### Clipboard Write ######
############################

Move Praetorian_Clipboard_Write
	Requires	ENCOUNTER CLIPBOARD ALTERNATE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Interpolate	8
	Scale		0.7
	CycleMove       Praetorian_Clipboard_Write
	CycleMove       Praetorian_Clipboard_Write
	CycleMove       Praetorian_Clipboard_Talk
	CycleMove       Praetorian_Clipboard_Talk

	Type		male
	  Anim          male\clipboard_write 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	  PlayFX		GENERIC\FX_clipboardPraetorian_SFX.fx 1
	  PlayFX		GENERIC\FX_clipboardPraetorian_SFX.fx 55
	TEnd

	Type		fem
	  Anim          fem\fem_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	  PlayFX		GENERIC\FX_clipboardPraetorian_SFX.fx 1
	  PlayFX		GENERIC\FX_clipboardPraetorian_SFX.fx 55
	TEnd
MEnd

Move Praetorian_Clipboard_Talk
	Requires	ENCOUNTER CLIPBOARD ALTERNATE LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Praetorian_Clipboard_Talk_Cycle

	Type		male
	  Anim          male\praetorian_clipboard_talk 1 36
	  PlayFx        GENERIC\FX_clipboard.fx 0 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	TEnd
MEnd

Move Praetorian_Clipboard_Talk_Cycle
	Requires	ENCOUNTER CLIPBOARD ALTERNATE LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Praetorian_Clipboard_Talk_Post

	Type		male
	  Anim          male\praetorian_clipboard_talk 36 88
	  PlayFx        GENERIC\FX_clipboard.fx 0 Constant
	TEnd
MEnd

Move Praetorian_Clipboard_Talk_Post
	Requires	ENCOUNTER CLIPBOARD ALTERNATE LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Praetorian_Clipboard_Write
	CycleMove       Praetorian_Clipboard_Write
	CycleMove       Praetorian_Clipboard_Write
	CycleMove       Praetorian_Clipboard_Talk

	Type		male
	  Anim          male\praetorian_clipboard_talk 88 125
	  PlayFx        GENERIC\FX_clipboard.fx 0 Constant
	  	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	TEnd
MEnd

## Clockwork Diagnostic ###########################################################

Move Clockwork_Diagnostic
	Requires	ENCOUNTER POSE STAND CALCULATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Diagnostic_Post_Chest
	CycleMove	Clockwork_Diagnostic_Post_Chest
	# CycleMove	Clockwork_Diagnostic_Stand
	# CycleMove	Clockwork_Diagnostic_Stand
	SticksonChild	POSE OBSERVE HIGH
	Scale		0.60

	Type		male
	  Anim          male\clockwork_diagnostic 1 162
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArm1.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArm2.fx 40
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 60
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 68
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 78
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset2.fx 80
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset3.fx 90
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArm1.fx 100
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArm2.fx 140
	TEnd

MEnd

Move Clockwork_Diagnostic_Post_Chest
	Requires	ENCOUNTER POSE STAND CALCULATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Diagnostic_Post
	SticksonChild	POSE TYPING LOW
	Scale		0.60

	Type		male
	  Anim          male\clockwork_diagnostic 162 251
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FixChestOpen_SFX.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset2.fx 25
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset3.fx 30
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 43
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 49
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 56
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 63
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 69
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_FixArmBeep.fx 75
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FixChestClose_SFX.fx 88
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FixChestScan_SFX.fx 88
	TEnd

MEnd

Move Clockwork_Diagnostic_Post
	Requires	ENCOUNTER POSE STAND CALCULATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Diagnostic
	CycleMove	Clockwork_Diagnostic
	# CycleMove	Clockwork_Diagnostic_Stand
	# CycleMove	Clockwork_Diagnostic_Stand
	Scale		0.60

	Type		male
	  Anim          male\clockwork_diagnostic 251 347
	TEnd

MEnd


Move Clockwork_Diagnostic_Stand
	Requires	ENCOUNTER POSE STAND CALCULATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Diagnostic_Stand
	CycleMove	Clockwork_Diagnostic_Stand
	CycleMove	Clockwork_Diagnostic
	CycleMove	Clockwork_Diagnostic
	Scale		0.60

	Type		male
	  Anim          male\clockwork_ready 1
	TEnd

MEnd

## Clockwork Washwall ###########################################################

Move Clockwork_Washwall_Pre
	Requires	ENCOUNTER STAND HAND SWIPE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Washwall

	Type		male
	  Anim          male\clockwork_washwall 1 10
	TEnd

MEnd

Move Clockwork_Washwall
	Requires	ENCOUNTER STAND HAND SWIPE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Washwall_left
	CycleMove	Clockwork_Washwall_right
	CycleMove	Clockwork_Washwall_stand

	Type		male
	  Anim          male\clockwork_washwall 11 232
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 11
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 25
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 50
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 75
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 100
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 125
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 150
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 175
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 200
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 225
	TEnd

MEnd

Move Clockwork_Washwall_left
	Requires	ENCOUNTER STAND HAND SWIPE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Washwall
	CycleMove	Clockwork_Washwall_right
	CycleMove	Clockwork_Washwall_stand

	Type		male
	  Anim          male\clockwork_washwall 233 400
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 1
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 25
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 50
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 75
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 100
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 125
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 150
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowLean_SFX.fx 20
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowLean_SFX.fx 105
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowLean_SFX.fx 160
	TEnd

MEnd

Move Clockwork_Washwall_right
	Requires	ENCOUNTER STAND HAND SWIPE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Washwall
	CycleMove	Clockwork_Washwall_left
	CycleMove	Clockwork_Washwall_stand

	Type		male
	  Anim          male\clockwork_washwall 401 592
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 1
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 25
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 50
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 75
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 100
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 125
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 150
  	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindow_SFX.fx 200
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowLean_SFX.fx 65
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowLean_SFX.fx 170
	TEnd

MEnd

Move Clockwork_Washwall_stand
	Requires	ENCOUNTER STAND HAND SWIPE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Washwall_right
	CycleMove	Clockwork_Washwall_left
	CycleMove	Clockwork_Washwall

	Type		male
	  Anim          male\clockwork_washwall 593 700
   	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WashWindowStand_SFX.fx 1
	TEnd

MEnd

## Clockwork StreetClean ###########################################################

Move Clockwork_Streetclean_Pre
	Requires	ENCOUNTER HAND BLOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Streetclean

	Type		male
	  Anim          male\clockwork_streetclean 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_CleanGround_SFX.fx 1
	TEnd
MEnd

Move Clockwork_Streetclean
	Requires	ENCOUNTER HAND BLOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Streetclean_left
	CycleMove	Clockwork_Streetclean_right

	Type		male
	  Anim          male\clockwork_streetclean 66 400
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 0 CONSTANT
	TEnd

MEnd

Move Clockwork_Streetclean_right
	Requires	ENCOUNTER HAND BLOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Streetclean
	CycleMove	Clockwork_Streetclean_left

	Type		male
	  Anim          male\clockwork_streetclean 401 580
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 0 CONSTANT
	TEnd

MEnd

Move Clockwork_Streetclean_left
	Requires	ENCOUNTER HAND BLOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Streetclean
	CycleMove	Clockwork_Streetclean_right

	Type		male
	  Anim          male\clockwork_streetclean 581 780
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 0 CONSTANT
	TEnd

MEnd

## Clockwork welding ###########################################################

Move Clockwork_Welding_Pre
	Requires	ENCOUNTER POSE REPAIR STAND
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding

	Type		male
	  Anim          male\Clockwork_wielding_mid 1 15
	TEnd

MEnd

Move Clockwork_Welding
	Requires	ENCOUNTER POSE REPAIR STAND
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding
	CycleMove	Clockwork_Welding
	CycleMove	Clockwork_Welding_break

	Type		male
	  Anim          male\Clockwork_wielding_mid 16 75
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_Welding.fx 0 CONSTANT
	TEnd

MEnd

Move Clockwork_Welding_break
	Requires	ENCOUNTER POSE REPAIR STAND
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding

	Type		male
	  Anim          male\Clockwork_wielding_mid 76 155
	TEnd

MEnd

#############################################################

Move Clockwork_Welding_Low_Pre
	Requires	ENCOUNTER POSE REPAIR LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding_Low

	Type		male
	  Anim          male\Clockwork_wielding_low 1 15
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_Welding.fx 0 CONSTANT
	TEnd

MEnd

Move Clockwork_Welding_Low
	Requires	ENCOUNTER POSE REPAIR LOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding_Low
	CycleMove	Clockwork_Welding_Low
	CycleMove	Clockwork_Welding_Low_break

	Type		male
	  Anim          male\Clockwork_wielding_low 16 75
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_Welding.fx 0 CONSTANT
	TEnd

MEnd

Move Clockwork_Welding_Low_break
	Requires	ENCOUNTER POSE REPAIR LOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Clockwork_Welding_Low

	Type		male
	  Anim          male\Clockwork_wielding_low 76 155
	TEnd

MEnd

### RIFLE' - Kneel ###########################################################

Move Resistance_Enc_SniperRifle_Point
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE LOW TWOHAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\resistance_sniper_kneel 1
	TEnd
MEnd

#############################################################
## Binoculars Kneel
#############################################################

Move Praetorian_Binoculars
	Requires	ENCOUNTER BINOCULARS LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Scale		0.7

	Type		male
	  Anim          male\binocular_scan_kneel 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd

MEnd

######################
### Walkie Talkie Kneel   #####
#####################

Move Praetorian_WalkieTalkie
	Requires	ENCOUNTER CHAT LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\walkietalkie_kneel 1
	  PlayFx        GENERIC\Walkietalkie.fx  0 Constant
	  PlayFx        GENERIC\Walkietalkie_SFX.fx
	TEnd
MEnd

#############################################################
## Pace Talk
#############################################################

Move Praetorian_Pace_Talk_Cycle
	Requires	ENCOUNTER POSE PACE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pace_Talk_Post
	CycleMove	Praetorian_Pace_Talk_Post
	CycleMove	Praetorian_Pace_Talk_Cycle
	CycleMove	Praetorian_Pace_Talk_Cycle
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pace_talk 1
	TEnd

MEnd

Move Praetorian_Pace_Talk_Post
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pace_Talk_Post
	CycleMove	Praetorian_Pace_Talk_Post
	CycleMove	Praetorian_Pace_Talk_Post
	CycleMove	Praetorian_Pace_Talk_Cycle

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

MEnd

####Praetorian Teleport In  #########################################################

Move Praetorian_TeleportIn
	Requires	ENCOUNTER TELEPORT DOWN
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Praetorian_TeleportIn_LookQuick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	#  PlayFx        GENERIC\entityFadeIn.fx 0
	  PlayFx        ENEMYFX\Seers\Summon_Portal_In.fx 0
	TEnd
MEnd

Move Praetorian_TeleportIn_LookQuick
	Requires	ENCOUNTER TELEPORT DOWN
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Praetorian_TeleportIn_PostReady

	Type		male
	  Anim          male\lookquick 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\lookquick 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\lookquick 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\lookquick 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	TEnd
MEnd

Move Praetorian_TeleportIn_PostReady
	Requires	ENCOUNTER TELEPORT DOWN
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 43
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 60
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 43
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 60
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 43
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 60
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd
MEnd

####Praetorian Teleport Out  #########################################################

Move Praetorian_TeleportOut
	Requires	ENCOUNTER TELEPORT UP
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Praetorian_TeleportOut_Post
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	#  PlayFx        GENERIC\entityFadeOut.fx 0
	  PlayFx        ENEMYFX\Seers\Summon_Portal_Out.fx 0
	TEnd
MEnd

Move Praetorian_TeleportOut_Post
	Requires	ENCOUNTER TELEPORT UP
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable Cycle
	Sets            CANTMOVE
	Scale		0.5

	Type		male
	  Anim          male\ready 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Secret Science Offer
#############################################################

Move Praetorian_Secret_Offer_Pre
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Cycle_Variant
	CycleMove	Praetorian_Secret_Offer_Cycle_Variant
	CycleMove	Praetorian_Secret_Offer_Cycle_Four


	Type		male
	  Anim          male\praetorian_secret_offer 1 17
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Variant
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Cycle_Variant_Two


	Type		male
	  Anim          male\praetorian_secret_offer 17	45
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 13
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Variant_Two
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Cycle_One


	Type		male
	  Anim          male\praetorian_secret_offer 45	60
	  PlayFX	GENERIC\FX_SecretOffer_Props.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_One
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Cycle_Two
	CycleMove	Praetorian_Secret_Offer_Cycle_One


	Type		male
	  Anim          male\praetorian_secret_offer 60 143
	  PlayFX	GENERIC\FX_SecretOffer_Props.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 20
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 55
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Two
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Cycle_Two_Variant

	Type		male
	  Anim          male\praetorian_secret_offer 143 169
	  PlayFX	GENERIC\FX_SecretOffer_Props.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 5
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Two_Variant
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Pre
	CycleMove	Praetorian_Secret_Offer_Cycle_Three
	CycleMove	Praetorian_Secret_Offer_Pre


	Type		male
	  Anim          male\praetorian_secret_offer 170 187
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Three
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Pre
	CycleMove	Praetorian_Secret_Offer_Pre
	CycleMove	Praetorian_Secret_Offer_Cycle_Three


	Type		male
	  Anim          male\praetorian_secret_offer 187 250
	TEnd

MEnd

Move Praetorian_Secret_Offer_Cycle_Four
	Requires	ENCOUNTER SECRET
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Secret_Offer_Pre
	CycleMove	Praetorian_Secret_Offer_Pre
	CycleMove	Praetorian_Secret_Offer_Cycle_Three

	Type		male
	  Anim          male\praetorian_secret_offer 204 250
	TEnd

MEnd

#############################################################
## Pamphlet Sift
#############################################################

Move Praetorian_Pamphlet_Sift_Pre
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Pre

	Type		male
	  Anim          male\praetorian_pamphlet_sift 1 17
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX	fx\GENERIC\FX_PamphletStack_Right.fx 0
	PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx
	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 1 17
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX	fx\GENERIC\FX_PamphletStack_Right.fx 0
	PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx
	TEnd

MEnd

Move Praetorian_Pamphlet_Sift_Cycle_Pre
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle
	CycleMove	Praetorian_Pamphlet_Sift_Cycle
	CycleMove	Praetorian_Pamphlet_Sift_Pre

	Type		male
	  Anim          male\praetorian_pamphlet_sift 17 35
#	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Grab_SFX.fx 13
	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 17 35
#	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Grab_SFX.fx 13
	TEnd

MEnd

Move Praetorian_Pamphlet_Sift_Cycle
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle
	CycleMove	Praetorian_Pamphlet_Sift_Cycle
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Post
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Post

	Type		male
	  Anim          male\praetorian_pamphlet_sift 35 80
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT

	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 35 80
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT

	TEnd

MEnd

Move Praetorian_Pamphlet_Sift_Cycle_Post
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Two

	Type		male
	  Anim          male\praetorian_pamphlet_sift 80 100
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx
	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 80 100
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx
	TEnd

MEnd

Move Praetorian_Pamphlet_Sift_Cycle_Two
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Two
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Two_Post

	Type		male
	  Anim          male\praetorian_pamphlet_sift 100 145
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT

	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 100 145
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT

	TEnd

MEnd

Move Praetorian_Pamphlet_Sift_Cycle_Two_Post
	Requires	ENCOUNTER PAPERS STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pamphlet_Sift_Cycle_Pre

	Type		male
	  Anim          male\praetorian_pamphlet_sift 145 160
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx

	TEnd

	Type		fem
	  Anim          fem\praetorian_pamphlet_sift 145 160
	  PlayFX	fx\GENERIC\FX_PamphletSingle_Left.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Paper_Rustle_SFX.fx

	TEnd

MEnd

#############################################################
## Pack Boxes
#############################################################

Move Praetorian_Pack_Boxes_Prepre
	Requires	ENCOUNTER PACK STAND
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Pre
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 1 1
	  PlayFX	WORLD\Objects\PraetorianBox\PraetorianBox_Open.fx 0
	TEnd

MEnd

Move Praetorian_Pack_Boxes_Pre
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Cycle_One
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 1 10
	TEnd

MEnd

Move Praetorian_Pack_Boxes_Cycle_One
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Cycle_Two
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 11 36
	  	  PlayFX		NPCFX\NPC_Pack_Grab_SFX.fx
		  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	  	  PlayFX		NPCFX\NPC_Pack_Set_SFX.fx 25
		  PlayFX	  	WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

Move Praetorian_Pack_Boxes_Cycle_Two
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Cycle_Three
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 36 70
	  	  PlayFX		NPCFX\NPC_Pack_Grab_SFX.fx
		  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	TEnd

MEnd

Move Praetorian_Pack_Boxes_Cycle_Three
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Cycle_Four
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 70 100
	  	  # PlayFX		NPCFX\NPC_Pack_Grab_SFX.fx 12
		  # PlayFX		NPCFX\NPC_ClothLong_SFX.fx
		  PlayFX		NPCFX\NPC_Pack_Set_SFX.fx 57
	TEnd

MEnd

Move Praetorian_Pack_Boxes_Cycle_Four
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Cycle_Post
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 100 145
	  	  PlayFX		NPCFX\NPC_Pack_Grab_SFX.fx 0
		  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
		  PlayFX		NPCFX\NPC_Pack_Set_SFX.fx 40
		  PlayFX	  	WORLD\Objects\PraetorianBottle\PraetorianBottle2.fx 0 CONSTANT
	TEnd

MEnd

Move Praetorian_Pack_Boxes_Cycle_Post
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Praetorian_Pack_Boxes_Pre
	Scale		0.7

	Type		male
	  Anim          male\praetorian_pack_boxes 145 165
	  	  # PlayFX		NPCFX\NPC_Pack_Grab_SFX.fx 12
		  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
		  PlayFX		NPCFX\NPC_Pack_Set_SFX.fx 57
	TEnd

MEnd

#############################################################
## Enriche Drink
#############################################################

Move Praetorian_Enriche_Drink
	Requires	ENCOUNTER DRINK ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Praetorian_Enriche_Drink_cycle_A
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

MEnd

Move Praetorian_Enriche_Drink_cycle_A
	Requires	ENCOUNTER DRINK ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Praetorian_Enriche_Drink_cycle_A
	CycleMove	Praetorian_Enriche_Drink_cycle_B
	CycleMove	Praetorian_Enriche_Drink_cycle_C
	CycleMove	Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

Move Praetorian_Enriche_Drink_cycle_B
	Requires	ENCOUNTER DRINK ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

MEnd

Move Praetorian_Enriche_Drink_cycle_C
	Requires	ENCOUNTER DRINK ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 5
	TEnd

MEnd

## Ground Kick ###########################################################

Move Ground_Kick_Cycle_Pre
	Requires	ENCOUNTER KICK ATTACK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stand_One

	Type		male
	  Anim          male\praetorian_ground_kick 1 22
	TEnd

MEnd

Move Ground_Kick_Cycle_Stand_One
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Kick_One


	Type		male
	  Anim          male\praetorian_ground_kick 22 70
	TEnd

MEnd

Move Ground_Kick_Cycle_Kick_One
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Kick_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT HIGH

	Type		male
	  Anim          male\praetorian_ground_kick 107 142
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	  PlayFX		NPCFX\NPC_Kick_SFX.fx 13
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

Move Ground_Kick_Cycle_Kick_Two
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Kick_Three
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT HIGH

	Type		male
	  Anim          male\praetorian_ground_kick 107 142
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_Kick_SFX.fx 13
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

Move Ground_Kick_Cycle_Kick_Three
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Kick_Four
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT HIGH

	Type		male
	  Anim          male\praetorian_ground_kick 107 142
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_Kick_SFX.fx 13
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

Move Ground_Kick_Cycle_Kick_Four
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stand_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT HIGH

	Type		male
	  Anim          male\praetorian_ground_kick 107 142
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_Kick_SFX.fx 13
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

Move Ground_Kick_Cycle_Stand_Two
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stand_Three


	Type		male
	  Anim          male\praetorian_ground_kick 22 70
	TEnd

MEnd

Move Ground_Kick_Cycle_Stand_Three
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stomp_One


	Type		male
	  Anim          male\praetorian_ground_kick 22 70
	TEnd

MEnd

Move Ground_Kick_Cycle_Stomp_One
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stomp_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT MID

	Type		male
	  Anim          male\praetorian_ground_kick 70 107
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx
	  PlayFX		NPCFX\NPC_Stomp_SFX.fx 14
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

Move Ground_Kick_Cycle_Stomp_Two
	Requires	ENCOUNTER KICK ATTACK
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Cycle_Stand_One
	SetsOnReactor	ENCOUNTER KICK ATTACK HIT MID

	Type		male
	  Anim          male\praetorian_ground_kick 70 107
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx
	  PlayFX		NPCFX\NPC_Stomp_SFX.fx 14
	  PlayFX		Feet\Foot_NPCMale.fx 32
	TEnd

MEnd

## Ground Kick Receive ###########################################################

Move Ground_Kick_Receive_Cycle_Pre
	Requires	ENCOUNTER KICK ATTACK LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stand_One

	Type		male
	  Anim          male\praetorian_ground_kick_receive 1 22
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Stand_One
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Kick_One

	Type		male
	  Anim          male\praetorian_ground_kick_receive 22 70
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Kick_One
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Kick_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT HIGH BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 107 142
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Kick_Two
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Kick_Three
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT HIGH BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 107 142
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Kick_Three
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Kick_Four
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT HIGH BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 107 142
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Kick_Four
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stand_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT HIGH BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 107 142
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Stand_Two
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stand_Three

	Type		male
	  Anim          male\praetorian_ground_kick_receive 22 70
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Stand_Three
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stomp_One

	Type		male
	  Anim          male\praetorian_ground_kick_receive 22 70
	TEnd

MEnd


Move Ground_Kick_Receive_Cycle_Stomp_One
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stomp_Two
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT MID BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 70 107
	TEnd

MEnd

Move Ground_Kick_Receive_Cycle_Stomp_Two
	Requires	ENCOUNTER KICK ATTACK LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Ground_Kick_Receive_Cycle_Stand_One
	SetsOnReactor	ENCOUNTER KICK ATTACK LOW HIT MID BLOCK

	Type		male
	  Anim          male\praetorian_ground_kick_receive 70 107
	TEnd

MEnd

######################
#### Protest_sign Loyalists      #####
######################

Move Protest_sign_Loyalists
	Requires	ENCOUNTER PROTEST LOYALIST
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Loyalists
	CycleMove	Protest_sign2_Loyalists
	CycleMove	Protest_sign3_Loyalists

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx  0 Constant
	TEnd
MEnd

Move Protest_sign2_Loyalists
	Requires	ENCOUNTER PROTEST LOYALIST
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign3_Loyalists
	CycleMove	Protest_sign2_Loyalists
	CycleMove	Protest_sign_Loyalists
	Scale		0.9

	Type		male
	  Anim          male\sign_chant 3
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx  0 Constant
	TEnd
MEnd

Move Protest_sign3_Loyalists
	Requires	ENCOUNTER PROTEST LOYALIST
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Loyalists
	CycleMove	Protest_sign3_Loyalists
	CycleMove	Protest_sign2_Loyalists
	Scale		0.8

	Type		male
	  Anim          male\sign_chant 5
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx  0 Constant
	TEnd
MEnd

######################
#### Protest_sign Resistance   #####
######################

Move Protest_sign_Resistance
	Requires	ENCOUNTER PROTEST RESISTANCE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Resistance
	CycleMove	Protest_sign2_Resistance
	CycleMove	Protest_sign3_Resistance

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx  0 Constant
	TEnd
MEnd

Move Protest_sign2_Resistance
	Requires	ENCOUNTER PROTEST RESISTANCE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign3_Resistance
	CycleMove	Protest_sign2_Resistance
	CycleMove	Protest_sign_Resistance
	Scale		0.9

	Type		male
	  Anim          male\sign_chant 3
	  PlayFx        GENERIC\ProtestSign_Resistance.fx  0 Constant
	TEnd
MEnd

Move Protest_sign3_Resistance
	Requires	ENCOUNTER PROTEST RESISTANCE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Resistance
	CycleMove	Protest_sign3_Resistance
	CycleMove	Protest_sign2_Resistance
	Scale		0.8

	Type		male
	  Anim          male\sign_chant 5
	  PlayFx        GENERIC\ProtestSign_Resistance.fx  0 Constant
	TEnd
MEnd

#########################
### Praetorian Newspaper Encounter #####
#########################

Move Enc_Praetorian_newspaper
	Requires	ENCOUNTER NEWSPAPER NEUTRAL
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale FixArmReg
	Interpolate	5

	Type            male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Male.fx 0 Constant
	  PlayFX	NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd

	Type            fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Female.fx 0 Constant
	  PlayFX	NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd

	Type            huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Huge.fx 0 Constant
	  PlayFX	NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd
MEnd

###################
### Seer Contact Ready ##
###################

Move Seer_Contact_Levitate_Cycle
	Requires	ENCOUNTER SEER HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Seer_Contact_Levitate_Cycle

	Type		fem
	  Anim          fem\seer_unbound_ready 1
	  PlayFX	ENEMYFX\Seers\Aura_SFX.fx 1 CONSTANT
	TEnd

	Type		Seer_Unbound
	  Anim          fem\seer_unbound_ready 1
	  PlayFX	ENEMYFX\Seers\Aura_SFX.fx 1 CONSTANT
	TEnd
MEnd

###################
### Seer Bound Contact Ready ##
###################

Move Seer_Contact_Levitate_Cycle_Bound
	Requires	ENCOUNTER SEER LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Seer_Contact_Levitate_Cycle_Bound

	Type		fem
	  Anim          fem\seer_ready 1
	  PlayFX	ENEMYFX\Seers\Aura_SFX.fx 1 CONSTANT
	TEnd

	Type		Seer
	  Anim          fem\seer_ready 1
	  PlayFX	ENEMYFX\Seers\Aura_SFX.fx 1 CONSTANT
	TEnd

	Type		Seer_Unbound
	  Anim          fem\seer_ready 1
	  PlayFX	ENEMYFX\Seers\Aura_SFX.fx 1 CONSTANT
	TEnd

MEnd

##############Seer_Teleporting_Out####################################################################################################

Move Seer_Teleporting_Out
	Requires	ENCOUNTER SEER TELEPORT HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Seer_Teleporting_Outpost

	Type		fem
	  Anim          fem\Seer_summon 1 56
	  PlayFx        ENEMYFX\Seers\Summon_Portal_Out.fx 0
	TEnd

	Type		Seer_Unbound
	  Anim          fem\Seer_summon 1 56
	  PlayFx        ENEMYFX\Seers\Summon_Portal_Out.fx 0
	TEnd

MEnd

Move Seer_Teleporting_Outpost
	Requires	ENCOUNTER SEER TELEPORT HIGH
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		fem
	  Anim         fem\Seer_summon 56 57
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		Seer_Unbound
	  Anim         fem\Seer_summon 56 57
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

MEnd

###########Seer_Teleporting_In #######################################################################################################

Move Seer_Teleporting_In
	Requires	ENCOUNTER SEER TELEPORT LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Seer_Teleporting_Inpost

	Type		fem
	  Anim          fem\Seer_summon 57 116
	  PlayFx        ENEMYFX\Seers\Summon_Portal_In.fx 0
	TEnd

	Type		Seer_Unbound
	  Anim          fem\Seer_summon 57 116
	  PlayFx        ENEMYFX\Seers\Summon_Portal_In.fx 0
	TEnd

MEnd

Move Seer_Teleporting_Inpost
	Requires	ENCOUNTER SEER TELEPORT LOW
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Seer_Contact_Levitate_Cycle
	Interpolate	10

	Type		fem
	  Anim         fem\Seer_summon 116 125
	TEnd

	Type		Seer_Unbound
	  Anim         fem\Seer_summon 116 125
	TEnd

MEnd

###################
###Clockwork Malfunction ##
###################

Move Clockwork_NPC_Malfunction_Pre
	Requires	ENCOUNTER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Clockwork_NPC_Malfunction_Pre
	NextMove	Clockwork_NPC_Malfunction_Pre
	Scale 		0.75
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_malfunction 1
	  PlayFX		ENEMYFX\Clockwork\Antimatter\Sparks_Body.fx 0 CONSTANT
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_01.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_02.fx 57
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_03.fx 114
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_04.fx 171
	TEnd

MEnd

Move Clockwork_NPC_Malfunction_Post
	Requires	ENCOUNTER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Clockwork_NPC_Malfunction_Pre
	NextMove	Clockwork_NPC_Malfunction_Pre
	Scale 		0.75
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_malfunction 90
	  PlayFX		ENEMYFX\Clockwork\Antimatter\Sparks_Body.fx 0 CONSTANT
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_03.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_01.fx 57
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_02.fx 114
	TEnd

MEnd

Move Clockwork_NPC_Malfunction_PostTwo
	Requires	ENCOUNTER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Clockwork_NPC_Malfunction_Pre
	NextMove	Clockwork_NPC_Malfunction_Pre
	Scale 		0.75
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_malfunction 10
	  PlayFX		ENEMYFX\Clockwork\Antimatter\Sparks_Body.fx 0 CONSTANT
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_02.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_01.fx 57
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_03.fx 114
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_04.fx 171
	TEnd

MEnd

Move Clockwork_NPC_Malfunction_PostThree
	Requires	ENCOUNTER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Clockwork_NPC_Malfunction_Pre
	NextMove	Clockwork_NPC_Malfunction_Pre
	Scale 		0.75
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_malfunction 130
	  PlayFX		ENEMYFX\Clockwork\Antimatter\Sparks_Body.fx 0 CONSTANT
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_01.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_03.fx 57
	TEnd

MEnd

Move Clockwork_NPC_Malfunction_PostFour
	Requires	ENCOUNTER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Clockwork_NPC_Malfunction_Pre
	NextMove	Clockwork_NPC_Malfunction_Pre
	Scale 		0.75
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_malfunction 150
	  PlayFX		ENEMYFX\Clockwork\Antimatter\Sparks_Body.fx 0 CONSTANT
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_04.fx 0
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Malfunction_SFX_02.fx 57
	TEnd

MEnd
#########
Move Bobcat_OnCouch
	Requires	ENCOUNTER BOBCAT ONE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Bobcat_OnCouch
	SticksOnChild	TAIL BOBCAT ONE

	Type		FEM
	  Anim          FEM\bobcat_contact_on_couch 1
	#playfx		WORLD\Objects\Bobcat_Couch\Bobcat_Couch.fx
	TEnd

MEnd

#####################################################
#####  Praetorian Newsie (using vendor as a placeholder anim)  #####
#####################################################

Move Praetorian_Newsie_YellRight
	Requires	ENCOUNTER NEWSIE NEUTRAL
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove	Praetorian_Newsie_Point
	CycleMove       Praetorian_Newsie_Point
	CycleMove       Praetorian_Newsie_Sweep

	Type		male
	  Anim          male\VendorNew_YellRight 1
	  PlayFX	Generic\newspapers_praetorian_root.fx 0 Constant
	TEnd
MEnd

Move Praetorian_Newsie_Point
	Requires	ENCOUNTER NEWSIE NEUTRAL
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove	Praetorian_Newsie_Point
	CycleMove       Praetorian_Newsie_YellRight
	CycleMove       Praetorian_Newsie_Sweep

	Type		male
	  Anim          male\VendorNew_Point 1
	  PlayFX	Generic\newspapers_praetorian_root.fx 0 Constant
	TEnd
MEnd

Move Praetorian_Newsie_Sweep
	Requires	ENCOUNTER NEWSIE NEUTRAL
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove	Praetorian_Newsie_Point
	CycleMove       Praetorian_Newsie_Point
	CycleMove       Praetorian_Newsie_YellRight

	Type		male
	  Anim          male\VendorNew_Sweep 1
	  PlayFX	Generic\newspapers_praetorian_root.fx 0 Constant
	TEnd
MEnd

#############################################################
## Praetorian Pamphlet
#############################################################

Move Encounter_Praetorian_Pamphlet
	Requires	ENCOUNTER PROP LOITER NEUTRAL ALTERNATE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Praetorian_Pamphlet.fx 0 Constant
	TEnd
MEnd

#############################################################
## Studio 55 Pamphlet
#############################################################

Move Encounter_Studio55_Praetorian_Pamphlet
	Requires	ENCOUNTER PROP LOITER NEUTRAL ONE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Studio55_Pamphlet.fx 0 Constant
	TEnd
MEnd

#############################################################
## CDEC Pamphlet
#############################################################

Move Encounter_CDEC_Praetorian_Pamphlet
	Requires	ENCOUNTER PROP LOITER NEUTRAL TWO
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_CDEC_Pamphlet.fx 0 Constant
	TEnd
MEnd


#############################################################

Move Enc_Desdemona_Mask_Pickup
	Requires	ENCOUNTER DESDEMONA PURSEPICKUP
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	NextMove	CombatReady

	Type		fem
	  Anim          fem\desdemona_mask_pickup 1
	Playfx		"WORLD\Objects\Carnival_Mask\FX_Geo_Head_Mistress.fx"
	TEnd

MEnd

##### Float Cycle####################################################

Move Float_Cycle_Pre
	Requires	ENCOUNTER FLOATSTUN ALTERNATE ONE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Float_Cycle
	Flags		NotSelectable
	Sets            CANTMOVE

	Type		male
	  Anim          male\ghoul_float 1 50
	TEnd

MEnd

Move Float_Cycle
	Requires	ENCOUNTER FLOATSTUN ALTERNATE ONE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NotSelectable
	Sets            CANTMOVE
	Flags		Cycle

	Type		male
	  Anim          male\ghoul_float 1 50
	TEnd

MEnd


####seer judgement########################################################

Move Seer_Judgement
	Requires	ENCOUNTER POSSESSOR HIGH
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Seer_Judgement_Cycle
	Sets            CANTMOVE

	Type		Seer_Unbound
	  Anim          fem\seer_judgement 1 44
	 PlayFX		Scripted\Praetorian\Seers_Judgement\Seers_Judgement.fx 0 constant
	TEnd
MEnd

Move Seer_Judgement_Cycle
	Requires	ENCOUNTER POSSESSOR HIGH
	Priority        3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle Nosizescale
	NextMove        CombatReady

	Type		Seer_Unbound
	  Anim         fem\seer_judgement 45 120
	  PlayFX	Scripted\Praetorian\Seers_Judgement\Seers_Judgement.fx 0 constant
	TEnd

MEnd

Move Seer_Judgement_post
	Flags		Nosizescale
	NextMove        CombatReady
	Priority	1
	Include sequencers\Members\Encounters_Scripted.inc

	Type		Seer_Unbound
	  Anim         fem\seer_judgement 121
# 	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0 constant
	TEnd

MEnd

#############################################################
##  Awakened Captured
#############################################################

Move Captured_Awakened_Cycle
	Requires	ENCOUNTER LEVITATE CAPTURED LOW
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_NPC_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakened.fx 0 Constant
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakenedflash.fx 25 Constant
	TEnd

	Type		Seer
	  Anim          fem\seer_ready 1
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakened.fx 0 Constant
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakenedflash.fx 25 Constant
	TEnd

	Type		Corrupt_Seer
	  Anim          fem\corrupt_seer_ready 1
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakened.fx 0 Constant
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakenedflash.fx 25 Constant
	TEnd

	Type		Seer_Unbound
	  Anim          fem\seer_unbound_ready 1
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakened.fx 0 Constant
	  PlayFx        Scripted\Praetorian\Captured_Awakened\CapturedAwakenedflash.fx 25 Constant
	TEnd

MEnd

