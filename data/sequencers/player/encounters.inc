#############################################################
## Encounters.inc - Animlists for NPCs
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

## CROSS ARMS ###########################################################

Move Cross_Arms
	Requires	ENCOUNTER COMMAND OBSERVE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Cross_Arms_look
	Scale		0.8

	Type		male
	  Anim          male\stand_Xarms 1 68
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 1
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 20
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\stand_Xarms 1 68
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\stand_Xarms 1 68
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\stand_Xarms 1 68
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd
MEnd

Move Cross_Arms_look
	Requires	ENCOUNTER COMMAND OBSERVE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Cross_Arms_look
	CycleMove	Arms_On_Hips
	Scale		0.5

	Type		male
	  Anim          male\idle_xarms_look 1
	TEnd
MEnd

Move Arms_On_Hips
	Requires	ENCOUNTER COMMAND OBSERVE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Arms_On_Hips_look
	Scale		0.8

	Type		male
	  Anim          male\stand_Xarms 92
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 12
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\stand_Xarms 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset5.fx 12
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\stand_Xarms 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset5.fx 12
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\stand_Xarms 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Idle_Headset5.fx 12
	TEnd
MEnd

Move Arms_On_Hips_look
	Requires	ENCOUNTER COMMAND OBSERVE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Cross_Arms
	CycleMove	Arms_On_Hips_look
	Scale		0.5

	Type		male
	  Anim          male\idle_2hips_look 1
	TEnd
MEnd

## GADGET FIX (TRICORDER) ###########################################################

Move Gadget_Fix
	Requires	ENCOUNTER MEDOTHER STRONG HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gadget_Fix_look

	Type		male
	  Anim          male\med_other 1
	  PlayFx        POWERS\Medicine\MAidWindUp2.fx  0 Constant
	  PlayFx        POWERS\Medicine\MAid.fx  56 Constant
	TEnd
	Type		PraetorianClockworkMale
	  Anim          male\med_other 1
	  PlayFx        POWERS\Medicine\MAidWindUp2.fx  0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        POWERS\Medicine\MAid.fx  56 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 150
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\med_other 1
	  PlayFx        POWERS\Medicine\MAidWindUp2.fx  0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        POWERS\Medicine\MAid.fx  56 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 150
	TEnd

	Type		fem
	  Anim          fem\med_other 1
	  PlayFx        POWERS\Medicine\MAidWindUp.fx  0 Constant
	  PlayFx        POWERS\Medicine\MAid.fx  56 Constant
	TEnd

	Type		PraetorianClockworkFem
	  Anim          fem\med_other 1
	  PlayFx        POWERS\Medicine\MAidWindUp.fx  0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        POWERS\Medicine\MAid.fx  56 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 150
	TEnd
MEnd

Move Gadget_Fix_look
	Requires	ENCOUNTER MEDOTHER STRONG HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gadget_Fix
	Scale		0.5

	Type		male
	  Anim          male\idle_look 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\idle_look 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 40
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\idle_look 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 40
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd

	Type		fem
	  Anim          fem\idle_look 1
	TEnd

	Type		PraetorianClockworkFem
	  Anim          fem\idle_look 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 40
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	TEnd
MEnd

## KNEELING LOOT ###########################################################

Move Encounter_ExamineGround
	Requires	ENCOUNTER LOOT LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        PostLoot
	Scale           0.50

	Type		male
	  Anim          male\examine_body 1
	TEnd
MEnd

##  PLANT BOMB ###########################################################

Move BombLoot_pre
	Requires	ENCOUNTER LOW LOOT
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        BombLoot
	Scale           0.70

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move BombLoot
	Requires	ENCOUNTER LOW LOOT
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Loot_Post
	Scale		0.5

	Type		male
	  Anim          male\examine_body 1
	TEnd
MEnd

## QUICK LOOT ###########################################################

Move Loot_readypre
	Requires	ENCOUNTER LOOT QUICK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Loot_ready
	Scale           0.70
	Sets            CANTMOVE

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move Loot_ready
	Requires	ENCOUNTER LOOT QUICK
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Loot_Post
	Scale		0.5
	Sets            CANTMOVE

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

#############################################################

Move Loot_Post
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ready

	Type		male
	  Anim          male\Loot 26
	TEnd

	Type		huge
	  Anim          huge\Loot 26
	TEnd

	Type		fem
	  Anim          fem\Loot 26
	TEnd
MEnd

## KNEELING LOOT ###########################################################

Move Loot_A
	Requires	ENCOUNTER LOOT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loot_A2
	NextMove        PostLoot
	Scale           0.50

	Type		male
	  Anim          male\examine_body 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	TEnd
MEnd

Move Loot_A2
	Requires	ENCOUNTER LOOT
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loot_A3
	NextMove        PostLoot

	Type		male
	  Anim          male\examine_pullguts 1 76
	  PlayFX		ENEMYFX\Ghouls\Ghoul_FleshTear_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_FleshTear_SFX.fx 38
	TEnd
MEnd

Move Loot_A3
	Requires	ENCOUNTER LOOT
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loot_A4
	NextMove        PostLoot

	Type		male
	  Anim          male\examine_pullguts 1 76
	  PlayFX		ENEMYFX\Ghouls\Ghoul_FleshTear_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_FleshTear_SFX.fx 38
	TEnd
MEnd

Move Loot_A4
	Requires	ENCOUNTER LOOT
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loot_A
	NextMove        PostLoot

	Type		male
	  Anim          male\examine_post_pullguts 1 76
	  PlayFX		ENEMYFX\Ghouls\Ghoul_FleshTear_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 46
	TEnd
MEnd

#############################################################

Move PostLoot
	Include sequencers\Members\Encounters_Idle.inc

	Type		male
	  Anim          male\loot 29
	TEnd
MEnd

## LOOT DEATH ###########################################################

Move LootDeath
	Requires	HIT DEATH LOOT
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Sets            NOCOLLISION

	Type		male
	  Anim          male\knockback2 65 67
	TEnd
MEnd

## DEAD SLAB ###########################################################

Move Slab
	Requires	ENCOUNTER DEATH LOOT HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION CANTMOVE

	Type		male
	  Anim          male\slab_on_table 1 10
	TEnd
MEnd

## KNEEL LOW ###########################################################

Move Encounter_Kneel
	Requires	ENCOUNTER KNEEL LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Kneel_getup

	Type		male
	  Anim          male\kneel_comp 12 14
	TEnd

	Type		huge
	  Anim          huge\kneel_comp 12 14
	TEnd
MEnd

#############################################################

Move DojoKneel_getup
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ready

	Type		male
	  Anim          male\Loot 26
	TEnd

	Type		huge
	  Anim          huge\Loot 26
	TEnd
MEnd

## DOJO KNEEL ###########################################################

Move DojoKneel
	Requires	ENCOUNTER KNEEL KUNGFU
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        DojoKneel_getup

	Type		male
	  Anim          male\kneel_comp 24 26
	TEnd

	Type		huge
	  Anim          huge\kneel_comp 24 26
	TEnd
MEnd

## DOJO MEDITATE ###########################################################

Move DojoMeditate
	Requires	ENCOUNTER KNEEL KUNGFU MEDITATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        DojoKneel_getup

	Type		male
	  Anim          male\kneel_comp 24 26
	  PlayFx        GENERIC\headglow.fx  0 Constant
	TEnd

	Type		huge
	  Anim          huge\kneel_comp 24 26
	  PlayFx        GENERIC\headglow.fx  0 Constant
	TEnd
MEnd

## DOJO KNEEL LOW ###########################################################

Move DojoKneel_Low
	Requires	ENCOUNTER KNEEL KUNGFU LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        DojoKneel_getup

	Type		male
	  Anim          male\kneel_comp 29 31
	TEnd

	Type		huge
	  Anim          huge\kneel_comp 29 31
	TEnd
MEnd

#############################################################
## MENACES
#############################################################

Move Menace_A
	Requires	ENCOUNTER MENACE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle  AlwaysSizeScale
	CycleMove       Menace_A2
	CycleMove       Menace_A3

	Type		male
	  Anim          male\smackhands_long 12
	  PlayFx        FEET\TAUNT.FX  21
	  PlayFx        FEET\TAUNT.FX  59
	  PlayFx        FEET\TAUNT.FX  97
	  PlayFx        FEET\TAUNT.FX  135
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\smackhands_long 12
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 15
	  PlayFx        FEET\TAUNT.FX  21
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	  PlayFx        FEET\TAUNT.FX  59
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 90
	  PlayFx        FEET\TAUNT.FX  97
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 130
	  PlayFx        FEET\TAUNT.FX  135
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\smackhands_long 12
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 15
	  PlayFx        FEET\TAUNT.FX  21
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	  PlayFx        FEET\TAUNT.FX  59
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 90
	  PlayFx        FEET\TAUNT.FX  97
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 130
	  PlayFx        FEET\TAUNT.FX  135
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\smackhands_long 12
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 15
	  PlayFx        FEET\TAUNT.FX  21
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 55
	  PlayFx        FEET\TAUNT.FX  59
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 90
	  PlayFx        FEET\TAUNT.FX  97
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 130
	  PlayFx        FEET\TAUNT.FX  135
	TEnd

	Type		crystal
	  Scale		0.20
	  Anim          male\drum_high 1
	TEnd
MEnd

Move Menace_A2
	Requires	ENCOUNTER MENACE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle AlwaysSizeScale
	CycleMove       Menace_A

	Type		male
	  Anim          male\threat_D 1 52
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\threat_D 1 52
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\threat_D 1 52
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\threat_D 1 52
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	TEnd

	Type		crystal
	  Scale		0.20
	  Anim          male\drum_high 1
	TEnd
MEnd

Move Menace_A3
	Requires	ENCOUNTER MENACE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle AlwaysSizeScale
	Scale           0.50
	CycleMove       Menace_A

	Type		male
	  Anim          male\yell 65 92
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\yell 65 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\yell 65 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\yell 65 92
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 20
	TEnd

	Type		crystal
	  Scale		0.20
	  Anim          male\drum_high 1
	TEnd
MEnd

## CRAZY MENACE ###########################################################

Move CrazyMenace_A
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	CycleMove       CrazyMenace_Aa

	Type		male
	  Anim          male\suspicious 1 50
	TEnd
MEnd

Move CrazyMenace_Aa
	Requires	ENCOUNTER STRONG MENACE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CrazyMenace_Ab

	Type		male
	  Anim          male\suspicious 1 50
	TEnd
MEnd

Move CrazyMenace_Ab
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	Interpolate     8
	CycleMove       CrazyMenace_A2

	Type		male
	  Anim          male\suspicious 63
	TEnd
MEnd

Move CrazyMenace_A2
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CrazyMenace_A3

	Type		male
	  Anim          male\suspicious 63
	TEnd
MEnd

Move CrazyMenace_A3
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CrazyMenace_A4

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move CrazyMenace_A4
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CrazyMenace_A5

	Type		male
	  Anim          male\threat_C 1
	TEnd
MEnd

Move CrazyMenace_A5
	Requires	ENCOUNTER MENACE STRONG
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CrazyMenace_A

	Type		male
	  Anim          male\vandal_kick_low 1
	TEnd
MEnd

## KNIFE MENACE ###########################################################

Move Knife_Menace_A
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           1.50
	CycleMove       Knife_Menace_A
	CycleMove       Knife_Menace_A2
	CycleMove       Knife_Menace_A4

	Type		male
	  Anim          male\knifetoss 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 30
	TEnd
MEnd

Move Knife_Menace_A2
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_A3

	Type		male
	  Anim          male\pistol_right 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
	TEnd
MEnd

Move Knife_Menace_A3
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_A

	Type		male
	  Anim          male\pistol_left 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_A4
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_A

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
	TEnd
MEnd

Move Knife_Menace_B
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_B2

	Type		male
	  Anim          male\threat_cycle 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_B2
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_B

	Type		male
	  Anim          male\root 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_B3
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.70
	CycleMove       Knife_Menace_B

	Type		male
	  Anim          male\yell 35 63
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_B4
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_B

	Type		male
	  Anim          male\knife_threatD 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_C
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_C2
	CycleMove       Knife_Menace_C3
	CycleMove       Knife_Menace_C5

	Type		male
	  Anim          male\knifetoss 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 25
	TEnd
MEnd

Move Knife_Menace_C2
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_C

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
	TEnd
MEnd

Move Knife_Menace_C3
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_C4

	Type		male
	  Anim          male\bat_overheadshake 1 50
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
	TEnd
MEnd

Move Knife_Menace_C4
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_C

	Type		male
	  Anim          male\root 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Menace_C5
	Requires	ENCOUNTER COMBAT WEAPON BLADE MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Knife_Menace_C

	Type		male
	  Anim          male\chant_1arm 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
		PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx
	TEnd
MEnd


## GUN MENACE ###########################################################

Move Gun_Menace_A
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\pistol_aim_frantic 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_B
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_B2
	CycleMove       Gun_Menace_B3
	CycleMove       Gun_Menace_B6

	Type		male
	  Anim          male\pistol_left 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_B2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_B

	Type		male
	  Anim          male\pistol_right 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_B3
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_B4

	Type		male
	  Anim          male\pistol_2hand_point 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_B4
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_B4
	CycleMove       Gun_Menace_B2

	Type		male
	  Anim          male\pistol_2hand_point 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_B6
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_B

	Type		male
	  Anim          male\knife_comehere 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_C
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50
	CycleMove       Gun_Menace_C2
	CycleMove       Gun_Menace_C3

	Type		male
	  Anim          male\pistol_suspicious 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_C2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50
	CycleMove       Gun_Menace_C

	Type		male
	  Anim          male\yell 35 63
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_C3
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_C4

	Type		male
	  Anim          male\pistol_left 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_C4
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_C5

	Type		male
	  Anim          male\pistol_gangsta 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Menace_C5
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Gun_Menace_C

	Type		male
	  Anim          male\pistol_gangsta_post 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd


## BAT MENACE ###########################################################

Move Bat_Menace_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\bat_twirl 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	  PlayFx        NPCFX\NPC_Whoosh_SFX.fx 10
	  PlayFx        WEAPONS\BASEBALL_BAT_SFX.FX 32
	TEnd

	Type		Tsoo
	  Anim          male\bat_twirl 1
	  PlayFx        WEAPONS\KATANA.FX  0 Constant
	  PlayFx        NPCFX\NPC_Whoosh_SFX.fx 10
	  PlayFx        WEAPONS\BASEBALL_BAT_SFX.FX 32
	TEnd
MEnd

Move Bat_Menace_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Bat_Menace_B

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	  PlayFx        WEAPONS\BASEBALL_BAT_SFX.FX 30
	TEnd
MEnd

#############################################################
## THREATS
#############################################################

Move Threat_A
	Requires	ENCOUNTER THREAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\threat_combo_A 1 134
		PlayFX		NPCFX\NPC_Whoosh_SFX.fx 70
	TEnd
MEnd

Move Threat_B
	Requires	ENCOUNTER THREAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\threat_combo_B 1 134
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 10
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 15
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 20
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 25
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 30
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 35
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 40
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 45
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 50
		PlayFX		NPCFX\NPC_Whoosh_SFX.fx 80
	TEnd
MEnd

Move Threat_C
	Requires	ENCOUNTER THREAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\threat_combo_C 1 134
		PlayFX		NPCFX\NPC_Whoosh_SFX.fx 10
	TEnd
MEnd

## GUN THREAT ###########################################################

Move Gun_Threat_A
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Gun_Threat_A2

	Type		male
	  Anim          male\pistol_left 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move Gun_Threat_A2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN THREAT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Priority	4
	CycleMove       Gun_Threat_A

	Type		male
	  Anim          male\pistol_right 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

## BAT THREAT ###########################################################

Move Bat_Threat_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\bat_threat_combo_A 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move Bat_Threat_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\knife_threat_combo_B 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move Bat_Threat_C
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

## KNIFE THREAT ###########################################################

Move Knife_Threat_A
	Requires	ENCOUNTER COMBAT WEAPON BLADE THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\knife_threat_combo_A 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Threat_B
	Requires	ENCOUNTER COMBAT WEAPON BLADE THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\knife_threat_combo_B 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move Knife_Threat_C
	Requires	ENCOUNTER COMBAT WEAPON BLADE THREAT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           1.50

	Type		male
	  Anim          male\knifetoss 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

#############################################################
## LOOKOUTS
#############################################################

Move Lookout_A
	Requires	ENCOUNTER LOOKOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Lookout_A
	CycleMove       Lookout_A2
	Scale           0.60

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd

Move Lookout_A2
	Requires	ENCOUNTER LOOKOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Lookout_A

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Lookout_B
	Requires	ENCOUNTER LOOKOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Lookout_B2
	CycleMove       Lookout_B3

	Type		male
	  Anim          male\Look_R 1
	TEnd
MEnd

Move Lookout_B2
	Requires	ENCOUNTER LOOKOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Lookout_B

	Type		male
	  Anim          male\Look_L 1
	TEnd
MEnd

Move Lookout_B3
	Requires	ENCOUNTER LOOKOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.60
	CycleMove       Lookout_B

	Type		male
	  Anim          male\Look_R 1
	TEnd
MEnd

## KNIFE LOOKOUT ###########################################################

Move knifeLookout_A
	Requires	ENCOUNTER COMBAT WEAPON BLADE LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       knifeLookout_A2

	Type		male
	  Anim          male\Look_L 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move knifeLookout_A2
	Requires	ENCOUNTER COMBAT WEAPON BLADE LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       knifeLookout_A

	Type		male
	  Anim          male\Look_R 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move knifeLookout_B
	Requires	ENCOUNTER COMBAT WEAPON BLADE LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50

	Type		male
	  Anim          male\pistol_suspicious 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

## GUN LOOKOUT ###########################################################

Move GunLookout_A
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       GunLookout_A2

	Type		male
	  Anim          male\pointpistol_look_R 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunLookout_A2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       GunLookout_A

	Type		male
	  Anim          male\pointpistol_look_L 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunLookout_B
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       GunLookout_B2
	CycleMove       GunLookout_B3

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunLookout_B2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50
	CycleMove       GunLookout_B

	Type		male
	  Anim          male\suspicious 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunLookout_B3
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       GunLookout_B4

	Type		male
	  Anim          male\pistol_left 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunLookout_B4
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       GunLookout_B

	Type		male
	  Anim          male\pistol_right 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

## SHOTGUN LOOKOUT ###########################################################

Move ShotgunLookout
	Requires	ENCOUNTER COMBAT WEAPON SHOTGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5
	CycleMove       ShotgunLookout
	CycleMove       ShotgunLookout2

	Type		male
	  Anim          male\shotgun_look 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

Move ShotgunLookout2
	Requires	ENCOUNTER COMBAT WEAPON SHOTGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.7
	CycleMove       ShotgunLookout2
	CycleMove       ShotgunLookout

	Type		male
	  Anim          male\shotgun_look 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

## BAT LOOKOUT ###########################################################

Move BatLookout_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move BatLookout_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.70
	CycleMove       BatLookout_B2

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move BatLookout_B2
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.90
	CycleMove       BatLookout_B

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

###################
### Chanting   #####
###################

Move Chant_A
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_A
	CycleMove       Chant_A2

	Type		male
	  Anim          male\cot_chant 1
	TEnd
MEnd

Move Chant_A2
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_A
	CycleMove       Chant_A2

	Type		male
	  Anim          male\cot_chantB 1
	TEnd
MEnd

Move Chant_B
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_B2

	Type		male
	  Anim          male\cot_chantB 5
	TEnd
MEnd

Move Chant_B2
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_B3
	CycleMove       Chant_B2

	Type		male
	  Anim          male\cot_chantB 1
	TEnd
MEnd

Move Chant_B3
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_B2
	CycleMove       Chant_B3

	Type		male
	  Anim          male\cot_chant 1
	TEnd
MEnd

Move Chant_C
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_C2

	Type		male
	  Anim          male\cot_chant 3
	TEnd
MEnd

Move Chant_C2
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Chant_C3
	CycleMove       Chant_C2

	Type		male
	  Anim          male\cot_chant 1
	TEnd
MEnd

Move Chant_C3
	Requires	ENCOUNTER CHANTING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.90
	CycleMove       Chant_C2
	CycleMove       Chant_C3

	Type		male
	  Anim          male\cot_chantB 1
	TEnd
MEnd

#############################################################
## CHEER
#############################################################

Move Cheer_A
	Requires	ENCOUNTER CHEER
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\chant_high 1
	TEnd
MEnd

Move Cheer_B
	Requires	ENCOUNTER CHEER
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

Move Cheer_C
	Requires	ENCOUNTER CHEER
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\listen_1 1 200
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 127
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 134
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 141
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 148
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 155
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 162
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 169
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 176
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 183
	  	 PlayFX		NPCFX\NPC_HandClap_SFX.fx 190
	TEnd
MEnd

## WEAPON CHEER ###########################################################

Move WeaponCheer
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.90

	Type		male
	  Anim          male\Chant_1arm 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

Move knifeCheer_A
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.80

	Type		male
	  Anim          male\Chant_low 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

## PISTOL CHEER ###########################################################

Move PistolCheer_A
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.80

	Type		male
	  Anim          male\Chant_1arm 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move PistolCheer_B
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.90

	Type		male
	  Anim          male\Chant_low 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

#############################################################
## VANDALIZE
#############################################################

Move Vandal_A
	Requires	ENCOUNTER VANDALIZE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\kneel_pound 1
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  9
	TEnd
MEnd


## KNIFE VANDALIZE ###########################################################

Move knifeVandal_A
	Requires	ENCOUNTER COMBAT WEAPON BLADE VANDALIZE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\vandal_stompkick 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

## PISTOL VANDALIZE ###########################################################

Move PistolVandal_A
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN VANDALIZE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\vandal_kick_low 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

## BAT VANDALIZE ###########################################################

Move BatVandal_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT VANDALIZE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\bat_smashcycle 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

###################
### SprayPaint Vandal   #####
###################

Move Spray_A
	Requires	ENCOUNTER SPRAYCAN
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spray_A2

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0	Constant
	TEnd
MEnd

Move Spray_A2
	Requires	ENCOUNTER SPRAYCAN
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spray_A3

	Type		male
	  Anim          male\spray_shakecan 1
	  PlayFX	Generic\spraycan.fx 0	Constant
	  PlayFX	Generic\spraycan_shake_SFX.fx 5
	TEnd
MEnd

Move Spray_A3
	Requires	ENCOUNTER SPRAYCAN
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spray_A
	CycleMove       Spray_A4

	Type		male
	  Anim          male\spray_cycle 1
	  PlayFX	Generic\spraycan.fx 0	Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

Move Spray_A4
	Requires	ENCOUNTER SPRAYCAN
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spray_A4
	CycleMove       Spray_A

	Type		male
	  Anim          male\spray_cycle 15
	  PlayFX	Generic\spraycan.fx 0	Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd


###################
### Vandal Kick   #####
###################

Move Vandal_Kick_A1
	Requires	ENCOUNTER KICK VANDALIZE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Vandal_Kick_A2

	Type		male
	  Anim          male\vandal_kick_low 1
	TEnd
MEnd

Move Vandal_Kick_A2
	Requires	ENCOUNTER KICK VANDALIZE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Vandal_Kick_A1

	Type		male
	  Anim          male\vandal_stompkick 1
	TEnd
MEnd

###################
### loiter (warm hands)  #####
###################

Move Loiter_A1
	Requires	ENCOUNTER HAND LOITER
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Loiter_A2

	Type		male
	  Anim          male\loiter_warmhands 1
	TEnd
MEnd

Move Loiter_A2
	Requires	ENCOUNTER HAND LOITER
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loiter_A3

	Type		male
	  Anim          male\loiter_warmdance 1
	TEnd
MEnd

Move Loiter_A3
	Requires	ENCOUNTER HAND LOITER
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Loiter_A1

	Type		male
	  Anim          male\loiter_blowhands 1
	TEnd
MEnd

#############################################################

Move Loiter_B1
	Requires	ENCOUNTER HAND LOITER
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.80

	Type		male
	  Anim          male\loiter_warmhands 1
	TEnd
MEnd

#############################################################

Move Loiter_C1
	Requires	ENCOUNTER HAND LOITER
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\loiter_blowhands 1
	TEnd
MEnd

## BAT CHEER ###########################################################

Move BatCheer_A
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\Chant_1arm 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move BatCheer_B
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       BatCheer_B2
	CycleMove       BatCheer_B3

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move BatCheer_B2
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       BatCheer_B

	Type		male
	  Anim          male\bat_overheadshake 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

Move BatCheer_B3
	Requires	ENCOUNTER COMBAT WEAPON CHEER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       BatCheer_B

	Type		male
	  Anim          male\yell 35 63
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd
## GUN OBSERVE ###########################################################

Move GunObserve
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	GunObserve
	CycleMove	GunObserve2
	CycleMove	GunObserve

	Type		male
	  Anim          male\pointpistol_look_R 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

Move GunObserve2
	Requires	ENCOUNTER COMBAT WEAPON HANDGUN OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	GunObserve
	CycleMove	GunObserve2

	Type		male
	  Anim          male\pointpistol_look_L 1
	  PlayFx        WEAPONS\REVOLVER.FX  0 Constant
	TEnd
MEnd

## SHOTGUN OBSERVE ###########################################################

Move ShotgunObserve
	Requires	ENCOUNTER COMBAT WEAPON SHOTGUN OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	ShotgunObserve
	CycleMove	ShotgunObserve2

	Type		male
	  Anim          male\shotgun_observe_L 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

Move ShotgunObserve2
	Requires	ENCOUNTER COMBAT WEAPON SHOTGUN OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	ShotgunObserve2
	CycleMove	ShotgunObserve

	Type		male
	  Anim          male\shotgun_observe_R 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

#############################################################
## OBSERVE
#############################################################

Move Observe
	Requires	ENCOUNTER OBSERVE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\Look_R 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\Look_R 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 50
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\Look_R 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 50
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\Look_R 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 50
	TEnd
MEnd

## KNIFE OBSERVE ###########################################################

Move KnifeObserve
	Requires	ENCOUNTER COMBAT WEAPON BLADE OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\Look_L 1
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	TEnd
MEnd

## BAT OBSERVE ###########################################################

Move BatObserve
	Requires	ENCOUNTER COMBAT WEAPON BLUNT OBSERVE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd
###################
### SpellCast (green)  #####
###################

Move Spellcast_levitate
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spellcast_cycle

	Type		male
	  Anim          male\cot_levitate 1
	PlayFx		POWERS\RadiationControl\RadiationEmission.fx
	TEnd
MEnd

Move Spellcast_cycle
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Spellcast_blinding

	Type		male
	  Anim          male\cot_combat_cycle 15
	TEnd
MEnd

Move Spellcast_blinding
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spellcast_cycle2

	Type		male
	  Anim          male\cot_blinding 1
	  PlayFx        POWERS\RadiationControl\EmpPulse.fx 40
	TEnd
MEnd

Move Spellcast_cycle2
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Spellcast_ballcast

	Type		male
	  Anim          male\cot_combat_cycle 15
	TEnd
MEnd

Move Spellcast_ballcast
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Spellcast_cycle3

	Type		male
	  Anim          male\cot_ballcast 1
	  PlayFx        POWERS\RadiationControl\AtomicBlast.fx 27
	TEnd
MEnd

Move Spellcast_cycle3
	Requires	ENCOUNTER SPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Spellcast_levitate

	Type		male
	  Anim          male\cot_combat_cycle 15
	TEnd
MEnd

###################
### SpellCast (blue)  #####
###################

Move blue_Spellcast_levitate
	Requires	ENCOUNTER SPELLCAST SAD
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       blue_Spellcast_cycle

	Type		male
	  Anim          male\cot_levitate 1
	PlayFx		POWERS\RadiationControl\blue\RadiationEmission.fx
	TEnd
MEnd

Move blue_Spellcast_cycle
	Requires	ENCOUNTER SPELLCAST SAD
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       blue_Spellcast_blinding

	Type		male
	  Anim          male\cot_combat_cycle 15
	TEnd
MEnd

Move blue_Spellcast_blinding
	Requires	ENCOUNTER SPELLCAST SAD
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       blue_Spellcast_cycle2

	Type		male
	  Anim          male\cot_blinding 1
	  PlayFx        POWERS\RadiationControl\blue\EmpPulse.fx 40
	TEnd
MEnd

Move blue_Spellcast_cycle2
	Requires	ENCOUNTER SPELLCAST SAD
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       blue_Spellcast_levitate   ##blue_Spellcast_ballcast

	Type		male
	  Anim          male\cot_combat_cycle 15
	TEnd
MEnd

#############################################################
## PURSE TUG ###########################################################
#############################################################

Move PurseTug
	Requires	ENCOUNTER PURSETUG
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale
	NextMove        P_PushDown
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\purse_tug 1
	TEnd

	Type		fem
	  Anim          BF\purse_tug 1
	  PlayFx        GENERIC\PURSE.FX  0 Constant
	TEnd
MEnd

Move P_PushDown
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        P_lookaround
	Sets            CANTMOVE

	Type		male
	  Anim          male\push 1 52
	TEnd

	Type		fem
	  Anim          BF\purse_getpushed 12
	TEnd
MEnd

Move P_lookaround
	Include sequencers\Members\Encounters_Idle.inc

	Type		male
	  Anim          male\lookquick 1
	TEnd

	Type		fem
	  Anim          BF\purse_shy 1
	TEnd
MEnd

## CHICKEN "PURSE" TUG ###########################################################

Move BF_ChickenTug
	Requires	ENCOUNTER PURSETUG QUICK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        P_GetPushed #BF_P_GetPushed
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          BF\purse_tug 1
	  PlayFx        GENERIC\purse_chicken.FX  0 Constant
	TEnd
MEnd

## RELIC TUG ###########################################################

Move RelicTug
	Requires	ENCOUNTER RELICTUG
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        P_PushDown
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\purse_tug 1
	TEnd
MEnd

## SHAKE\CHOKE ###########################################################

Move Shake
	Requires	ENCOUNTER SHAKE HIGH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        P_PushDown
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\shake 1 84
	TEnd
MEnd

## SHAKE REACT ###########################################################

Move Shake_react
	Requires	ENCOUNTER SHAKE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        P_GetPushed
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\shake_react 1 84
	TEnd
MEnd

Move P_GetPushed
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Post_push_cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\pushdown_react 12 190
	TEnd
MEnd

Move Post_push_cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\pushdown_react 191 229
	TEnd
MEnd

## PUSHAROUND (3 PERSON) ###########################################################

Move PushAround
	Requires	ENCOUNTER PUSH 2HAND HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        PushAround_Post
	Sets            CANTMOVE NOCOLLISION ## NEEDS A FINISHCYCLE FLAG - SEE WOOMER

	Type		male
	  Anim          male\push3 1 111
	TEnd
MEnd

Move PushAround_Post
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        PushAround_Laugh
	Sets            CANTMOVE

	Type		male
	  Anim          male\push3 1 53
	TEnd
MEnd

Move PushAround_Laugh
	Include sequencers\Members\Encounters_Idle.inc

	Type		male
	  Anim          male\heroic_laugh 1
	TEnd
MEnd

## PUSHAROUND REVERSE ###########################################################

Move PushAround_Rev
	Requires	ENCOUNTER PUSH 2HAND MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        PushAround_Rev_Post
	Sets            CANTMOVE NOCOLLISION ## NEEDS A FINISHCYCLE FLAG - SEE WOOMER

	Type		male
	  Anim          male\push3_reverse 1 111
	TEnd
MEnd

Move PushAround_Rev_Post
	Include sequencers\Members\Encounters_Idle.inc

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

## PUSHAROUND VICTIM ###########################################################

Move PushAround_victim
	Requires	ENCOUNTER PUSH 2HAND LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        PushAround_victim_Post
	Sets            CANTMOVE NOCOLLISION ## NEEDS A FINISHCYCLE FLAG - SEE WOOMER

	Type		male
	  Anim          male\push3_react 1 111
	TEnd
MEnd

Move PushAround_victim_Post
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        PushAround_victim_hit
	Sets            CANTMOVE

	Type		male
	  Anim          male\push3_react 1 42
	TEnd
MEnd

Move PushAround_victim_hit
	Include sequencers\Members\Encounters_Idle.inc
	Interpolate	5

	Type		male
	  Anim          male\impact_back 3
	TEnd
MEnd

## GROUND PUNCH ###########################################################

Move Ground_punch
	Requires	ENCOUNTER GROUNDPUNCH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        Lookout_A2

	Type		male
	  Anim          male\crouch_punch 1
	  PlayFX	Feet\FistPunch1.fx 30
	TEnd
MEnd

## GROUND PUNCH HIT REACTION ###########################################################

Move Ground_punch_hit
	Requires	ENCOUNTER GROUNDPUNCH LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        Revive_back

	Type		male
	  Anim          male\crouch_punch_react 1
	TEnd
MEnd

## GROUND PUNCH HIT REACTION B ###########################################################

Move Ground_punch_hit_B
	Requires	ENCOUNTER DIENOW
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        Ground_punch_hit_B_post

	Type		male
	  Anim          male\crouch_punch_react 1
	TEnd
MEnd

## GROUND PUNCH HIT REACTION POST ###########################################################

Move Ground_punch_hit_B_post
	Requires	ENCOUNTER SLEEP STOP
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	#NextMove        Revive_back

	Type		male
	  Anim          male\crouch_punch_react_post 1
	TEnd
MEnd


#############################################################
## RUMBLES
#############################################################

## MAGIC RUMBLE A ###########################################################

Move Magic_Rumble_A
	Requires	ENCOUNTER RUMBLEA MAGIC CAST
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\cot_ranged_rumble_A 1 254
	TEnd
MEnd

Move Magic_Rumble_B
	Requires	ENCOUNTER RUMBLEB MAGIC CAST
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\zombie_ranged_rumble_B 1 254
	TEnd
MEnd

## TSOO "KICK" RUMBLE A ###########################################################

Move Kick_Rumble_A
	Requires	ENCOUNTER RUMBLEA KICK ATTACK
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\tso_ranged_rumble_A 1 254
	TEnd
MEnd

## RUMBLE A ###########################################################

Move Rumble_A
	Requires	ENCOUNTER RUMBLEA
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_fists_A 1 124
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 37
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 50


	TEnd
MEnd

## RUMBLE B ###########################################################

Move Rumble_B
	Requires	ENCOUNTER RUMBLEB
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_fists_B 1 124
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 1
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 32
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 48
	TEnd
MEnd

## RUMBLE DIZZY A ###########################################################

Move Rumble_dizzy_A
	Requires	ENCOUNTER RUMBLEA LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_dizzy_A 1 219
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 15
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 65
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 160
	TEnd
MEnd

## RUMBLE DIZZY B ###########################################################

Move Rumble_dizzy_B
	Requires	ENCOUNTER RUMBLEB LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_dizzy_B 1 219
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 60
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 65
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 115
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 128
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 148
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 150
	TEnd
MEnd

## RUMBLE KNIFE A ###########################################################

Move Rumble_knife_A
	Requires	ENCOUNTER RUMBLEA KNIFE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_knife_A 1 119
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 63
	TEnd
MEnd

## RUMBLE KNIFE B ###########################################################

Move Rumble_knife_B
	Requires	ENCOUNTER RUMBLEB KNIFE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_knife_B 1 119
	  PlayFx        WEAPONS\KNIFE.FX  0 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 58
	TEnd
MEnd

## SWORD RUMBLE DIZZY A ###########################################################

Move SwordRumble_dizzy_A
	Requires	ENCOUNTER LOW SWORD RUMBLEA
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\sword_rumble_A 1 219
	  PlayFX	Weapons\sword.fx 1 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 10
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 58
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 145
	TEnd
MEnd

## SWORD RUMBLE DIZZY B ###########################################################

Move SwordRumble_dizzy_B
	Requires	ENCOUNTER LOW SWORD RUMBLEB
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\sword_rumble_dizzy_B 1 219
	  PlayFX	Weapons\sword.fx 1 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 65
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 63
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 145
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 160
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 155
	TEnd
MEnd


###################
### Rumble Bat Smash A   #####
###################

Move Rumble_Batsmash_A
	Requires	ENCOUNTER BLUNT RUMBLEA
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 28
	TEnd
MEnd

###################
### Rumble Bat Smash B   #####
###################

Move Rumble_Batsmash_B
	Requires	ENCOUNTER BLUNT RUMBLEB
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\bat_smash_react 1 47
	  PlayFX		NPCFX\NPC_Kick_SFX.fx 28
	TEnd
MEnd

###################
### Bowing   #####
###################

Move Bowing_A
	Requires	ENCOUNTER BOWING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.60

	Type		male
	  Anim          male\cot_kneel_bow 1
	TEnd
MEnd

#############################################################

Move Bowing_B
	Requires	ENCOUNTER BOWING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\cot_kneel_chantB 1
	TEnd
MEnd

#############################################################

Move Bowing_C
	Requires	ENCOUNTER BOWING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.60
	CycleMove       Bowing_C2

	Type		male
	  Anim          male\cot_kneel_bow 32
	TEnd
MEnd

Move Bowing_C2
	Requires	ENCOUNTER BOWING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.60
	CycleMove       Bowing_C2

	Type		male
	  Anim          male\cot_kneel_bow 1
	TEnd
MEnd

###################
### Meditate   #####
###################

Move Meditate_A
	Requires	ENCOUNTER MEDITATE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.60

	Type		male
	  Anim          male\cot_meditate 1
	TEnd
MEnd

###############################
### Speech (recruit w\ box)   #####
###############################

Move Pre_Speech
	Requires	ENCOUNTER SPEECH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Speech
	Flags		NoSizeScale

	Type		male
	  Anim          male\recruit 670 799
	  PlayFx        GENERIC\RECRUIT_BOX.FX  0 Constant
	TEnd
MEnd

Move Speech
	Requires	ENCOUNTER SPEECH
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        PostSpeech

	Type		male
	  Anim          male\recruit 1 800
	  PlayFx        GENERIC\RECRUIT_BOX.FX  0 Constant
	TEnd
MEnd

#############################################################

Move PostSpeech
	Include sequencers\Members\Encounters_Cycles.inc

	Type		male
	  Anim          male\comb_postjump 1
	TEnd
MEnd

###############################
### Speech (NO box)   #####
###############################

Move Pre_Speech_NObox
	Requires	ENCOUNTER SPEECH LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Speech_NObox
	Flags		NoSizeScale

	Type		male
	  Anim          male\recruit_nobox 670 799
	TEnd
MEnd

Move Speech_NObox
	Requires	ENCOUNTER SPEECH LOW
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\recruit_nobox 1 800
	TEnd
MEnd

###################
### Listen to Speech (recruit)   #####
###################

Move Listen_Speech
	Requires	ENCOUNTER SPEECH LISTEN
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\listen_1 1 ##200
	TEnd
MEnd

### Variation_B ##

Move Listen_Speech_B
	Requires	ENCOUNTER SPEECH LISTEN
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       Listen_Speech_B2
	Interpolate	15

	Type		male
	  Anim          male\stand_1armhips 1
	TEnd
MEnd

Move Listen_Speech_B2
	Requires	ENCOUNTER SPEECH LISTEN
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_B
	Interpolate	15

	Type		male
	  Anim          male\stand_clap 1
	TEnd
MEnd

### Variation_C ##

Move Listen_Speech_C
	Requires	ENCOUNTER SPEECH LISTEN
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\Look_L 1
	TEnd
MEnd

###################
### Listen to Speech HIGH (recruit)   #####
###################

Move Listen_Speech_High
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\listen_2 1 ##200
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 79
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 86
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 93
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 100
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 107
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 114
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 121
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 128
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 135
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 142
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 149
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 156
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 163
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 170
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 177
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 184
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 191
	TEnd
MEnd

### Variation_B ##

Move Listen_Speech_HighB
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       Listen_Speech_HighB2

	Type		male
	  Anim          male\stand_Xarms 1 200
	TEnd
MEnd

Move Listen_Speech_HighB2
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_HighB

	Type		male
	  Anim          male\stand_clap 1 200
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 79
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 86
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 93
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 100
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 107
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 114
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 121
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 128
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 135
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 142
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 149
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 156
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 163
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 170
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 177
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 184
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 191
	TEnd
MEnd

### Variation_C ##

Move Listen_Speech_HighC
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       Listen_Speech_HighC
	CycleMove       Listen_Speech_HighC2
	CycleMove       Listen_Speech_HighC

	Type		male
	  Anim          male\threat_cycle 1
	TEnd
MEnd

Move Listen_Speech_HighC2
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_HighC
	CycleMove       Listen_Speech_HighC3
	CycleMove       Listen_Speech_HighC4

	Type		male
	  Anim          male\smackhands_long 12
	  PlayFx        FEET\TAUNT.FX  21
	  PlayFx        FEET\TAUNT.FX  59
	  PlayFx        FEET\TAUNT.FX  97
	  PlayFx        FEET\TAUNT.FX  135
	TEnd
MEnd

Move Listen_Speech_HighC3
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Scale           0.60
	CycleMove       Listen_Speech_HighC3
	CycleMove       Listen_Speech_HighC4
	CycleMove       Listen_Speech_HighC
	CycleMove       Listen_Speech_HighC2

	Type		male
	  Anim          male\yell 1 35
	TEnd
MEnd

Move Listen_Speech_HighC4
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_HighC5
	CycleMove       Listen_Speech_HighC3
	CycleMove       Listen_Speech_HighC4

	Type		male
	  Anim          male\menace_look_N_nod 1
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 79
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 86
	TEnd
MEnd

Move Listen_Speech_HighC5
	Requires	ENCOUNTER SPEECH LISTEN HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_HighC2
	CycleMove       Listen_Speech_HighC3
	CycleMove       Listen_Speech_HighC4

	Type		male
	  Anim          male\menace_wistle 1
	  PlayFX		NPCFX\NPC_Whistle_SFX.fx 15
	TEnd
MEnd

###################
### Listen to Speech LOW (recruit)   #####
###################

Move Listen_Speech_Low
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\listen_3 1 ##200
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 79
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 86
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 93
	  PlayFX	NPCFX\NPC_Whistle_SFX.fx 120
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 160
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 167
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 174
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 181
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 188
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 195
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 202
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 209
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 216
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 223
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 230
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 244
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 258
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 524
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 538
	  PlayFX	NPCFX\NPC_Whistle_SFX.fx 715
	TEnd
MEnd

### Variation_B ##

Move Listen_Speech_LowB
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\stand_1armhips 1 200
	TEnd
MEnd

Move Listen_Speech_LowC
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       Listen_Speech_LowC
	CycleMove       Listen_Speech_LowC2
	CycleMove       Listen_Speech_LowC4

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

Move Listen_Speech_LowC2
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Scale           0.70
	CycleMove       Listen_Speech_LowC
	CycleMove       Listen_Speech_LowC3

	Type		male
	  Anim          male\yell 35 63
	TEnd
MEnd

Move Listen_Speech_LowC3
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_LowC
	CycleMove       Listen_Speech_LowC4

	Type		male
	  Anim          male\menace_look_N_nod 1
	TEnd
MEnd

Move Listen_Speech_LowC4
	Requires	ENCOUNTER SPEECH LISTEN LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Listen_Speech_LowC
	CycleMove       Listen_Speech_LowC2

	Type		male
	  Anim          male\menace_clap 1
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 7
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 79
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 86
  	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 93
  	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 100
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 118
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 132
	TEnd
MEnd

###################
### Broken (sprocket)   #####
###################

Move Broken
	Requires	ENCOUNTER REPAIR LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Boostup

	Type		male
	  Anim          male\broken 1 5
	TEnd
MEnd

###################
### Vandal Jumping   #####
###################

Move Vandal_Jump_A1
	Requires	ENCOUNTER HIGH VANDALIZE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        JumpPost

	Type		male
	  Anim          male\multi_jump 1
	TEnd
MEnd


###################
### Vandal Bat Smash   #####
###################

Move Vandal_Bat
	Requires	ENCOUNTER BLUNT VANDALIZE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\bat_smashcycle 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX  0 Constant
	TEnd
MEnd

###################
### Roll Dice   #####
###################

Move RollDice
	Requires	ENCOUNTER ROLLDICE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\roll_dice 1
	TEnd
MEnd


###################
### Sit_Meditate   #####
###################

Move Sit_Meditate
	Requires	ENCOUNTER SIT MEDITATE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Sit_Low_getup

	Type		male
	  Anim          male\astro 63 64
	TEnd
MEnd

###################
### Sit_Low   #####
###################

Move Sit_Low_A
	Requires	ENCOUNTER SIT LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        Sit_Low_getup

	Type		male
	  Anim          male\sit_comp 1 4
	TEnd

	Type		fem
	  Anim          male\dojo_kneel 67 70
	TEnd

	Type		Giant
	  Anim          huge\sit_ground 102
	TEnd

	Type		goblin_beast
	  Anim          beast\sit_comp 1 4
	TEnd

	Type		Beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

Move Sit_Low_B
	Requires	ENCOUNTER SIT LOW
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        Sit_Low_getup

	Type		male
	  Anim          male\sit_comp 5 9
	TEnd

	Type		Giant
	  Anim          huge\sit_ground 102
	TEnd

	Type		goblin_beast
	  Anim          beast\sit_comp 5 9
	TEnd

	Type		Beast
	  Anim          beast\sit_comp 5 9
	TEnd
MEnd

Move Sit_Low_C
	Requires	ENCOUNTER SIT LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        Sit_Low_getup

	Type		male
	  Anim          male\sit_comp 10 14
	TEnd

	Type		Giant
	  Anim          huge\sit_ground 102
	TEnd

	Type		goblin_beast
	  Anim          beast\sit_comp 10 14
	TEnd

	Type		Beast
	  Anim          beast\sit_comp 10 14
	TEnd
MEnd

#############################################################

Move Sit_Low_getup
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ready
	Interpolate     8

	Type		male
	  Anim          male\astro 78
	TEnd

	Type		goblin_beast
	  Anim          Beast\getup_back 17 100
	TEnd

	Type		Beast
	  Anim          Beast\getup_back 17 100
	TEnd
MEnd


###################
### Sit_High   #####
###################

Move Sit_High_A
	Requires	ENCOUNTER SIT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        P_lookaround

	Type		male
	  Anim          male\sit_comp 15 19
	  PlayFx        GENERIC\BUCKET.FX  0 Constant
	TEnd
MEnd

Move Sit_High_B
	Requires	ENCOUNTER SIT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        P_lookaround

	Type		male
	  Anim          male\sit_comp 20 24
	  PlayFx        GENERIC\BOX_SMALL.FX  0 Constant
	TEnd
MEnd

Move Sit_High_C
	Requires	ENCOUNTER SIT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove        P_lookaround

	Type		male
	  Anim          male\sit_comp 25 29
	  PlayFx        GENERIC\BOX_BIG.FX  0 Constant
	TEnd
MEnd


#######################################
###### SledgeHammer ATTACK   ######
#######################################

Move Sledgehammer
	Requires	ENCOUNTER WEAPON RAMPAGE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\sledgehammer 1 60
	  PlayFx        GENERIC\SLEDGEHAMMER.FX  0 Constant
	  PlayFx        GENERIC\SLEDGEHAMMER_HIT.FX
	TEnd
MEnd


###################
### Throw junk (2hands)  #####
###################

Move Throw_Junk_A1
	Requires	ENCOUNTER HIGH RAMPAGE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Throw_Junk_A2

	Type		male
	  Anim          male\throw_chunk 1 165
	  PlayFx        GENERIC\CHUNK_TOSS.FX
	TEnd
MEnd

Move Throw_Junk_A2
	Requires	ENCOUNTER HIGH RAMPAGE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.80
	CycleMove       Throw_Junk_A1

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd


###################
### Throw junk (1 hand)  #####
###################

Move 1H_Throw_Junk_A1
	Requires	ENCOUNTER LOW RAMPAGE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\1H_throw_chunk 1
	  PlayFx        GENERIC\1H_PULL_CHUNK.FX  0 Constant
	TEnd
MEnd


###################
### Toss Crate (2hands)  #####
###################

Move Toss_Crate_A1
	Requires	ENCOUNTER HIGH RAMPAGE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Toss_Crate_A2
	CycleMove       Toss_Crate_A1
	CycleMove       Toss_Crate_A3
	Interpolate	8

	Type		male
	  Anim          male\toss_crate 1
	  PlayFx	generic\toss_crate.fx 40
	TEnd
MEnd

Move Toss_Crate_A2
	Requires	ENCOUNTER HIGH RAMPAGE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.80
	CycleMove       Toss_Crate_A1
	CycleMove       Toss_Crate_A3
	Interpolate	8

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Toss_Crate_A3
	Requires	ENCOUNTER HIGH RAMPAGE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.80
	CycleMove       Toss_Crate_A1
	Interpolate	8

	Type		male
	  Anim          male\comb_beatchest 1 91
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 10
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 15
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 20
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 25
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 30
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 35
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 40
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 45
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 50
	TEnd
MEnd

###################
### Greedy  #####
###################

Move Greedy
	Requires	ENCOUNTER LOW OBSERVE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\greedy 1
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 1
	  PlayFX		NPCFX\HandRub_SFX.fx 30
	  PlayFX		NPCFX\HandRub_SFX.fx 45
	  PlayFX		NPCFX\HandRub_SFX.fx 60
	  PlayFX		NPCFX\HandRub_SFX.fx 77
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 115
	TEnd
MEnd


###################
### Battle Rifle  #####
###################

Move Batt_Rifle_A1
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_A2

	Type		male
	  Anim          male\sniper_new 35
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  56
	TEnd
MEnd

Move Batt_Rifle_A2
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Batt_Rifle_A1
	CycleMove       Batt_Rifle_A3

	Type		male
	  Anim          male\rocket_cycle 1 31
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd

Move Batt_Rifle_A3
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_A1

	Type		male
	  Anim          male\sniper_cock 3
	  PlayFx        WEAPONS\COCK2.FX  27
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd


###################
### Battle Rifle HIGH  #####
###################

Move Batt_Rifle_High_1
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_High_2
	CycleMove       Batt_Rifle_High_5

	Type		male
	  Anim          male\sniper 1
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  34
	TEnd
MEnd

Move Batt_Rifle_High_2
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Batt_Rifle_High_3
	CycleMove       Batt_Rifle_High_1
	CycleMove       Batt_Rifle_High_5

	Type		male
	  Anim          male\shotgun_cycle 1
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd

Move Batt_Rifle_High_3
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_High_4
	CycleMove       Batt_Rifle_High_3b

	Type		male
	  Anim          male\sniper 1 60
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  34
	TEnd
MEnd

Move Batt_Rifle_High_3b
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_High_3b
	CycleMove       Batt_Rifle_High_4

	Type		male
	  Anim          male\sniper 34 60
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX
	TEnd
MEnd

Move Batt_Rifle_High_4
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_High_2
	CycleMove       Batt_Rifle_High_5

	Type		male
	  Anim          male\sniper 61
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd

Move Batt_Rifle_High_5
	Requires	ENCOUNTER ATTACK RIFLE DEALING HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_High_3
	CycleMove       Batt_Rifle_High_1
	CycleMove       Batt_Rifle_High_2

	Type		male
	  Anim          male\sniper_cock 3
	  PlayFx        WEAPONS\COCK2.FX  27
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd


###################
### Battle Rifle LOW  #####
###################

Move Batt_Rifle_Low_1
	Requires	ENCOUNTER ATTACK RIFLE DEALING LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Batt_Rifle_Low_2
	CycleMove	Batt_Rifle_Low_1
	CycleMove	Batt_Rifle_Low_1
	Interpolate     8

	Type		male
	  Anim          male\sniper_low 34
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  57
	TEnd
MEnd

Move Batt_Rifle_Low_2
	Requires	ENCOUNTER ATTACK RIFLE DEALING LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Batt_Rifle_Low_2
	CycleMove	Batt_Rifle_Low_1
	Interpolate     8
	Scale	0.5

	Type		male
	  Anim          male\shotgun_kneel_cycle 1
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd


###################
### Salute Formal  #####
###################

Move Salute_Formal
	Requires	ENCOUNTER SALUTE HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\salute_formal 1 5
	TEnd
MEnd


###################
### Encounter At Ease  #####
###################

Move Enc_AtEase
	Requires	ENCOUNTER SALUTE HIGH LOW POSE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\at_ease 61 65
	TEnd

	Type		huge
	  Anim          huge\at_ease 61 65
	TEnd
MEnd

############################
###### Clipboard Encounter ######
############################

Move Enc_Clipboard
	Requires	ENCOUNTER CLIPBOARD
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Interpolate	8
	Scale		0.7

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

############################
###### Clipboard Talk ######
############################

Move Clipboard_Talk_pre
	Requires	ENCOUNTER CLIPBOARD TALK
	Priority	3
	Interrupts	<IRQEncounters> <Ready> <MonsterGroup> <PostMoveCycle> <Combat> Enc_Clipboard
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Clipboard_Talk

	Type		male
	  Anim          male\clipboard_t_xarms 1 74
	  PlayFx        GENERIC\FX_clipboard.fx 0 Constant
	TEnd
MEnd

Move Clipboard_Talk
	Requires	ENCOUNTER CLIPBOARD TALK
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove	Enc_Clipboard

	Type		male
	  Anim          male\clipboard_t_xarms 74 75
	  PlayFx        GENERIC\FX_clipboard.fx 0 Constant
	TEnd
MEnd

############################
###### Cellphone Encounter ######
###########################

Move Enc_Cellphone
	Requires	ENCOUNTER CELLPHONE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\cellphone_talk 1
	  PlayFx        GENERIC\FX_cellphone.FX  0 Constant
	TEnd

	Type		fem
	  Anim          fem\cellphone_talk 1
	  PlayFx        GENERIC\FX_cellphone.FX  0 Constant
	TEnd
MEnd

##########################
#### Newspaper Encounter #####
##########################
#
#Move Enc_newspaper
#	Requires	ENCOUNTER NEWSPAPER
#	Priority	2
#	Include sequencers\Interrupts\Encounters_Idle.inc
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle NoSizeScale FixArmReg
#
#	Type		male
#	  Anim          male\newspaper_cycle 1 156
#	  PlayFx        GENERIC\FX_newspaper.FX  0 Constant
#	TEnd
#
#	Type		fem
#	  Anim          fem\fem_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_female.FX  0 Constant
#	TEnd
#MEnd
#
##########################
#### Rogue Newspaper Encounter #####
##########################
#
#Move Rogue_newspaper
#	Requires	ENCOUNTER NEWSPAPER EVIL
#	Priority	3
#	Include sequencers\Interrupts\Encounters_Idle.inc
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle NoSizeScale FixArmReg
#
#	Type		male
#	  Anim          male\newspaper_cycle 1 156
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX  0 Constant
#	TEnd
#
#	Type		fem
#	  Anim          fem\fem_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX  0 Constant
#	TEnd
#
#	Type		huge
#	  Anim          huge\AFK_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX  0 Constant
#	TEnd
#MEnd

#########################
### Briefcase Encounter #####
#########################

Move Enc_briefcase
	Requires	ENCOUNTER BRIEFCASE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\briefcase_look 1
	  PlayFx        GENERIC\FX_briefcase_R.fx  0 Constant
	  Scale		0.5
	TEnd
MEnd

###################
###### Spellcast Fear  ######
###################

Move SpellcastFear
	Requires	ENCOUNTER SPELLCAST FEAR
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        SCastfall
	Scale           0.50

	Type		male
	  Anim          bis\cot_wriggle 19
	  PlayFx        WORLD\Lairs\cot\victim_Effect.fx  0 Constant
	TEnd

	Type		fem
	  Anim          BF\cot_wriggle 19
	  PlayFx        WORLD\Lairs\cot\victim_Effect.fx  0 Constant
	TEnd
MEnd

Move SCastfall
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        SC_postcycle

	Type           male
	  Anim          bis\cot_postfall 2
	TEnd

	Type		fem
	  Anim          BF\cot_postfall 2
	TEnd
MEnd

Move SC_postcycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          bis\cot_postfallcycle 1
	TEnd

	Type		fem
	  Anim          BF\cot_postfallcycle 1
	TEnd
MEnd


###################
### Thank   #####
###################

Move Thank_A
	Requires	ENCOUNTER THANK
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	CycleMove       Thank_A2
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          BF\chant 1
	TEnd
MEnd

Move Thank_A2
	Requires	ENCOUNTER THANK
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove       Thank_A
	Flags		Cycle

	Type		male
	  Anim          BF\chantB 1
	TEnd
MEnd

#############################################################

Move Thank_B
	Requires	ENCOUNTER THANK
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Thank_B2
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          BF\thank_A 1
	TEnd
MEnd

Move Thank_B2
	Requires	ENCOUNTER THANK
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          BF\request_noshake 1
	TEnd
MEnd

#####################
### Shotgun Alert  #####
####################

Move Shotgun_Alert_A
	Requires	ENCOUNTER SHOTGUN POSE MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale		0.5

	Type		male
	  Anim          male\rocket_cycle 1 31
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd

Move Shotgun_Alert_B
	Requires	ENCOUNTER SHOTGUN POSE MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale		0.4

	Type		male
	  Anim          male\shotgun_cycle 1 31
	  PlayFx        WEAPONS\ASSAULT_RIFLE.FX  0 Constant
	TEnd
MEnd


#####################
### Shotgun Point  #####
####################

Move Shotgun_Point
	Requires	ENCOUNTER SHOTGUN POSE POINT HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\shotgun_point 1 100   ##sniper_new 83 85
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd

	Type		fem
	  Anim          fem\sniper_point 1 100   ##sniper_new 83 85
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd
MEnd


#####################
### Crossbow Point  #####
####################

Move Crossbow_Point
	Requires	ENCOUNTER CROSSBOW POSE POINT HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\shotgun_point 1 100
	  PlayFx        WEAPONS\crossbow.FX  0 Constant
	TEnd
MEnd


############################
### Shotgun RollOut Left  #####
############################

Move Sg_RollOut_Left
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT LEFT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollOut_L_machine_burst

	Type		male
	  Anim          male\rollout_L 1 85
	  PlayFx        WEAPONS\Left_Assault_Rifle.fx  0 Constant
	TEnd
MEnd

#Move Sg_RollOut_L_sniper
#	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT LEFT
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle
#	CycleMove	Sg_RollBack_L
#	Priority	4
#
#
#	Type		male
#	  Anim          male\rollout_L_sniper 13 97
#	  PlayFx        WEAPONS\Left_Assault_Rifle.fx  0 Constant
#	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  47
#	TEnd
#MEnd
#
#Move Sg_RollOut_L_shotgun
#	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT LEFT
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle
#	CycleMove	Sg_RollBack_L
#	Priority	4
#
#
#	Type		male
#	  Anim          male\rollout_L_shotgun 1
#	  PlayFx        WEAPONS\Left_Assault_Rifle.fx  0 Constant
#	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  2
#	TEnd
#MEnd

Move Sg_RollOut_L_machine_burst
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT LEFT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollBack_L

	Type		male
	  Anim          male\rollout_L_machine_burst 30 60
	  PlayFx        WEAPONS\Left_Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\Basic_L_AssaultRifle01.fx  0
	TEnd
MEnd

Move Sg_RollBack_L
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT LEFT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollOut_Left

	Type		male
	  Anim          male\rollout_L 86
	  PlayFx        WEAPONS\Left_Assault_Rifle.fx  0 Constant
	TEnd
MEnd


############################
### Shotgun RollOut Right  #####
############################

Move Sg_RollOut_Right
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollOut_R_machinegun
	CycleMove	Sg_RollOut_R_machine_burst

	Type		male
	  Anim          male\rollout_R 1 85 ##shotgun_observe_R 1
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

#Move Sg_RollOut_R_sniper
#	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle
#	CycleMove	Sg_RollBack_R
#	Priority	4
#
#
#	Type		male
#	  Anim          male\rollout_R_sniper 13 97
#	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
#	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  47
#	TEnd
#MEnd
#
#Move Sg_RollOut_R_shotgun
#	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle
#	CycleMove	Sg_RollBack_R
#	Priority	4
#
#
#	Type		male
#	  Anim          male\rollout_R_shotgun 1
#	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
#	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  2
#	TEnd
#MEnd

Move Sg_RollOut_R_machinegun
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollBack_R

	Type		male
	  Anim          male\rollout_R_machinegun 35 170
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\Basic_AssaultRifle01_Long.fx  0
	TEnd
MEnd

Move Sg_RollOut_R_machine_burst
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollBack_R

	Type		male
	  Anim          male\rollout_R_machine_burst 30 60
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\Basic_AssaultRifle01.fx  0
	TEnd
MEnd

Move Sg_RollBack_R
	Requires	ENCOUNTER SHOTGUN POSE LOOKOUT RIGHT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_RollOut_Right

	Type		male
	  Anim          male\rollout_R 86
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd


############################
### Shotgun Duck & Shoot  #####
############################

Move Sg_Crouch_cycle
	Requires	ENCOUNTER SHOTGUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_PopUp

	Type		male
	  Anim          male\shotgun_duck 59 60 ##shotgun_observe_R 1
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move Sg_PopUp
	Requires	ENCOUNTER SHOTGUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_PopUp_sniper_quick

	Type		male
	  Anim          male\shotgun_duck 1 26
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move Sg_PopUp_sniper_quick
	Requires	ENCOUNTER SHOTGUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_Duck
	CycleMove	Sg_PopUp_sniper_quick
	CycleMove	Sg_Duck

	Type		male
	  Anim          male\sniper_quickblast 1
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  1
	TEnd
MEnd

Move Sg_Duck
	Requires	ENCOUNTER SHOTGUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Sg_Crouch_cycle
	Interpolate	8

	Type		male
	  Anim          male\shotgun_duck 30
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

############################
### Gun Duck & Shoot  #####
############################

Move Gun_Crouch_cycle
	Requires	ENCOUNTER GUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gun_PopUp

	Type		male
	  Anim          male\pistol_duck 49 50
	  PlayFx        WEAPONS\Automatic_Pistol.fx  0 Constant
	TEnd
MEnd

Move Gun_PopUp
	Requires	ENCOUNTER GUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gun_PopUp_shoot_quick

	Type		male
	  Anim          male\pistol_duck 1 24
	  PlayFx        WEAPONS\Automatic_Pistol.fx  0 Constant
	TEnd
MEnd

Move Gun_PopUp_shoot_quick
	Requires	ENCOUNTER GUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gun_Duck
	CycleMove	Gun_PopUp_shoot_quick
	Interpolate	0

	Type		male
	  Anim          male\pistol_center_shoot 1 25
	  PlayFx        WEAPONS\Automatic_Pistol.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  1
	TEnd
MEnd

Move Gun_Duck
	Requires	ENCOUNTER GUN LOW HIDE SHOOT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Gun_Crouch_cycle

	Type		male
	  Anim          male\pistol_duck 28
	  PlayFx        WEAPONS\Automatic_Pistol.fx  0 Constant
	TEnd
MEnd

#####################
### Hostage  #####
####################

Move Hostage
	Requires	ENCOUNTER FEAR POSE HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim		bis\scared 11 89
	TEnd
MEnd

#######################
### Wall Sneek Cycle  #####
#######################

Move WallSneek_cycle
	Requires	ENCOUNTER SNEAK POSE LOOKOUT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.7

	Type		male
	  Anim		male\sneekwall_cycle 1
	TEnd
MEnd

#######################
### Wall Peak Right  #####
#######################

Move WallPeak_Right
	Requires	ENCOUNTER SNEAK POSE LOOKOUT RIGHT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim		male\corner_peak_R 1     ##rollout_R 1 59
	TEnd
MEnd

#######################
### Wall Peak Left  #####
#######################

Move WallPeak_Left
	Requires	ENCOUNTER SNEAK POSE LOOKOUT LEFT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.8

	Type		male
	  Anim		male\corner_peak_L 1     ##rollout_L 1 59
	TEnd
MEnd

#####################
### Dig hole        #####
#####################

Move Dig_hole
	Requires	ENCOUNTER WORK GROUNDSUMMON LOW MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx  0 constant
	  PlayFx        GENERIC\shovel_dirt.fx  37
	  PlayFx        GENERIC\shovel_dirt.fx  69
	  PlayFx        NPCFX\ShovelDirt.fx  37
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx  0 constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 10
	  PlayFx        GENERIC\shovel_dirt.fx  37
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 50
	  PlayFx        GENERIC\shovel_dirt.fx  69
	  PlayFx        NPCFX\ShovelDirt.fx  37
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx  0 constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 10
	  PlayFx        GENERIC\shovel_dirt.fx  37
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 50
	  PlayFx        GENERIC\shovel_dirt.fx  69
	  PlayFx        NPCFX\ShovelDirt.fx  37
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx  0 constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 10
	  PlayFx        GENERIC\shovel_dirt.fx  37
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 50
	  PlayFx        GENERIC\shovel_dirt.fx  69
	  PlayFx        NPCFX\ShovelDirt.fx  37
	TEnd
MEnd

###################
### Shovel_dig   #####
###################

Move Shovel_dig
	Requires	ENCOUNTER WORK GROUNDSUMMON LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale

	Type		male
	  Anim          male\shovel_dig 1
	  PlayFx        GENERIC\Shovel.fx  0 Constant
	  PlayFx        NPCFX\ShovelDirt.fx  45
	  PlayFx        GENERIC\shovel_dirt.fx  45
	  PlayFx        GENERIC\shovel_dirt.fx  92
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\shovel_dig 1
	  PlayFx        GENERIC\Shovel.fx  0 Constant
	  PlayFx        NPCFX\ShovelDirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 35
	  PlayFx        GENERIC\shovel_dirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	  PlayFx        GENERIC\shovel_dirt.fx  92
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\shovel_dig 1
	  PlayFx        GENERIC\Shovel.fx  0 Constant
	  PlayFx        NPCFX\ShovelDirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 35
	  PlayFx        GENERIC\shovel_dirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	  PlayFx        GENERIC\shovel_dirt.fx  92
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\shovel_dig 1
	  PlayFx        GENERIC\Shovel.fx  0 Constant
	  PlayFx        NPCFX\ShovelDirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 35
	  PlayFx        GENERIC\shovel_dirt.fx  45
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 80
	  PlayFx        GENERIC\shovel_dirt.fx  92
	TEnd
MEnd

###################
### Pickaxe_dig   #####
###################

Move Pickaxe_dig
	Requires	ENCOUNTER WORK GROUNDSUMMON HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\sledgehammer 1 60
	  PlayFx        GENERIC\Pickaxe.fx 0 Constant
	  PlayFx        NPCFX\PickAxeDirt.fx 40 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\sledgehammer 1 60
	  PlayFx        GENERIC\Pickaxe.fx 0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        NPCFX\PickAxeDirt.fx 40 Constant
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\sledgehammer 1 60
	  PlayFx        GENERIC\Pickaxe.fx 0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        NPCFX\PickAxeDirt.fx 40 Constant
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\sledgehammer 1 60
	  PlayFx        GENERIC\Pickaxe.fx 0 Constant
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFx        NPCFX\PickAxeDirt.fx 40 Constant
	TEnd
MEnd

###################
### 2 Hand Dig   #####
###################

Move M_2Hdig
	Requires	ENCOUNTER WORK HAND STRONG
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          Beast\goblin_2Hdig 1 32
	  PlayFx        GENERIC\2Hdig_dirt.fx  0
	TEnd
MEnd

###################
### 1 Hand Dig   #####
###################

Move M_1Hdig
	Requires	ENCOUNTER WORK HAND STRONG
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale

	Type		male
	  Anim          Beast\goblin_1Hdig 1 40
	  PlayFx        GENERIC\1Hdig_dirt.fx  0
	TEnd
MEnd


#################################
### Read Inscriptions w\ prop  #####
################################

Move Inscription
	Requires	ENCOUNTER LOOKOUT WORK TRAIN
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	  PlayFx        GENERIC\COT_artifact.fx  0 Constant
	TEnd
MEnd


####################################
### Read Inscriptions (no prop)   #####
####################################

Move Inscription_NoProp
	Requires	ENCOUNTER LOOKOUT WORK TRAIN MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd


####################################
### Read Inscriptions LOW (no prop)   #####
####################################

Move Inscription_NoProp_Low
	Requires	ENCOUNTER LOOKOUT WORK TRAIN LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove	Kneel_getup
	Scale		0.5

	Type		male
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd


########################
### Read Scroll   #####
#######################

Move Scroll
	Requires	ENCOUNTER LOOKOUT WORK TRAIN WAIT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\scroll_cycle 1
	  #PlayFx        GENERIC\scroll.fx  0 Constant
	  PlayFx        GENERIC\scroll_Narrow.fx  0 Constant
	TEnd

	Type		Beast
	   Anim          Beast\combat_cycle 1 31
	TEnd
MEnd

########################
### Float Books  #####
#######################

Move FloatBooks
	Requires	ENCOUNTER LOOKOUT WORK BOX BODY
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx  0 Constant

	TEnd
MEnd

########################
### Swat Shield Block  #####
#######################

Move SwatShieldBlock
	Requires	ENCOUNTER SHIELD POSE WORK
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\shield_block 1
	  PlayFx        GENERIC\swat_shield.fx  0 Constant
	TEnd

	Type		5cn
	  Anim          male\shield_block 1
	  PlayFx        GENERIC\riot_shield.fx  0 Constant
	TEnd
MEnd

###################
### Beat Chest  #####
###################

Move Beatchest
	Requires	ENCOUNTER POSE TAUNTA ANGRY LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Beatchest2
	Interpolate	8

	Type		male
	  Anim          male\comb_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	  	PlayFX		NPCFX\NPC_ChestThump_SFX.fx 10
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 15
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 20
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 25
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 30
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 35
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 40
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 45
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 50
	TEnd
MEnd

Move Beatchest2
	Requires	ENCOUNTER POSE TAUNTA ANGRY LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Beatchest
	CycleMove	Beatchest2
	CycleMove	Beatchest2

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd
MEnd

#Move Beatchest3
#	Requires	ENCOUNTER POSE TAUNTA ANGRY LOW
#	Include sequencers\Members\Encounters_Cycles.inc
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle
#	CycleMove	Beatchest
#	CycleMove	Beatchest2
#	Priority	4
#	Scale		0.7
#
#	Type		male
#	  Anim          male\suspicious 1
#	TEnd
#MEnd

###################
### Rularuu Angry  #####
###################

Move Rularuu_Angry
	Requires	ENCOUNTER DEALING ANGRY
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Rularuu_Angry2
	Interpolate	8

	Type		male
	  Anim          male\mystic_deal 1
	TEnd
MEnd

Move Rularuu_Angry2
	Requires	ENCOUNTER DEALING ANGRY
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Rularuu_Angry3

	Type		male
	  Anim          male\mystic_float_no_cycle 1
	TEnd
MEnd

Move Rularuu_Angry3
	Requires	ENCOUNTER DEALING ANGRY
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Rularuu_Angry

	Type		male
	  Anim          male\mystic_frustrated 1
	TEnd
MEnd

######################
#### Protest_sign      #####
######################

Move Protest_sign
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign
	CycleMove	Protest_sign2
	CycleMove	Protest_sign3

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

Move Protest_sign2
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign3
	CycleMove	Protest_sign2
	CycleMove	Protest_sign
	Scale		0.9

	Type		male
	  Anim          male\sign_chant 3
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

Move Protest_sign3
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign
	CycleMove	Protest_sign3
	CycleMove	Protest_sign2
	Scale		0.8

	Type		male
	  Anim          male\sign_chant 5
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd


######################
#### Protest_sign Angry     #####
######################

Move Protest_sign_Angry_Alt
	Requires	ENCOUNTER PICKETANGRY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

###

Move Protest_sign_Angry_A
	Requires	ENCOUNTER PICKETANGRY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Angry_B
	CycleMove	Protest_sign_Angry_C

	Type		male
	  Anim          male\sign_root 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

Move Protest_sign_Angry_B
	Requires	ENCOUNTER PICKETANGRY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Angry_A
	Scale		0.5

	Type		male
	  Anim          male\sign_yell 1 29
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

Move Protest_sign_Angry_C
	Requires	ENCOUNTER PICKETANGRY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Protest_sign_Angry_A
	Scale		0.5

	Type		male
	  Anim          male\sign_yell 30 60
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

################################
####### Training Bow 	 ######
################################

Move M_TrainBow_mid
	Requires	ENCOUNTER TRAIN ARCHERY
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	M_TrainBow_cone
	CycleMove	M_TrainBow_heavy
	CycleMove	M_TrainBow_mid
	SticksOnChild   COMBAT WEAPON DUAL REV BOW MID ENCOUNTER

	Type		male
	  Anim          male\bow_midshoot 1 120
	  PlayFX	WEAPONS\Bow\Encounter_bowstring_mid.fx 0
	TEnd
MEnd

Move M_TrainBow_heavy
	Requires	ENCOUNTER TRAIN ARCHERY
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	M_TrainBow_mid
	CycleMove	M_TrainBow_cone
	CycleMove	M_TrainBow_heavy
	SticksOnChild   COMBAT WEAPON DUAL REV BOW STRONG ENCOUNTER

	Type		male
	  Anim          male\bow_snipershoot 1 130
	  PlayFX	WEAPONS\Bow\Encounter_bowstring_sniper.fx 0
	TEnd
MEnd

Move M_TrainBow_cone
	Requires	ENCOUNTER TRAIN ARCHERY
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	M_TrainBow_heavy
	CycleMove	M_TrainBow_mid
	CycleMove	M_TrainBow_cone
	SticksOnChild   COMBAT WEAPON DUAL REV BOW CONE ENCOUNTER

	Type		male
	  Anim          male\bow_coneshoot 1 120
	  PlayFX	WEAPONS\Bow\Encounter_bowstring_cone.fx 0
	TEnd
MEnd

######################
### Binocular idle   #####
#####################

Move Binocular_idle
	Requires	ENCOUNTER LOOKOUT POSE FAR
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Scale		0.7

	Type		male
	  Anim          male\binocular_scan 1
	  PlayFx        GENERIC\Binoculars.fx  0 Constant
	TEnd
MEnd


######################
### Walkie Talkie idle   #####
#####################

Move WalkieTalkie_idle
	Requires	ENCOUNTER CHAT POSE FAR
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\walkietalkie 1
	  PlayFx        GENERIC\Walkietalkie.fx  0 Constant
	  PlayFx        GENERIC\Walkietalkie_SFX.fx
	TEnd
MEnd


############################
### Drill March in place   #####
###########################

Move Drill_March
	Requires	ENCOUNTER BUSY TRAIN STOMP
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\drill_march_turn 1 225
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

#######################################
### Drill Yell (goes w\ Drill_March)  #####
#######################################

Move Drill_Yell
	Requires	ENCOUNTER BUSY TRAIN YELL
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\drill_yell 1 225
	  PlayFx        Feet\maletaunt4.fx  35   ##(offset from 45)
	  PlayFx        Feet\maletaunt5.fx  132  ##(offset from 142)
	TEnd
MEnd

###################
### Repair   #####
###################

Move Repair_A1
	Requires	ENCOUNTER REPAIR
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Repair_A2

	Type		male
	  Anim          male\repair_pound 10 80
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  14
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  28
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  42
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  56
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  70
	TEnd

	Type		Sprocket
	  Anim          male\repair_pound 10 80
	  PlayFx        GENERIC\sprocket_junk.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	TEnd
MEnd

Move Repair_A2
	Requires	ENCOUNTER REPAIR
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Repair_A3

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  0 Constant
	TEnd

	Type		Sprocket
	  Anim          male\lookquick 1
	  PlayFx        GENERIC\sprocket_junk.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	TEnd
MEnd

Move Repair_A3
	Requires	ENCOUNTER REPAIR
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Repair_A4

	Type		male
	  Anim          male\repair_pullguts 1
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  0 Constant
	TEnd

	Type		Sprocket
	  Anim          male\repair_pullguts 1
	  PlayFx        GENERIC\sprocket_junk.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	TEnd
MEnd

Move Repair_A4
	Requires	ENCOUNTER REPAIR
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.80
	CycleMove       Repair_A1

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  0 Constant
	TEnd

	Type		Sprocket
	  Anim          male\lookquick 1
	  PlayFx        GENERIC\sprocket_junk.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	TEnd
MEnd

#######################################
### Sprocket Build Diesel  #####
#######################################

Move Build_Repair
	Requires	ENCOUNTER SMASH POSE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NExtMove	Build_Repair2
	Scale		0.5

	Type		male
	  Anim          male\repair_pound 10 80
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  10
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  24
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  38
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  52
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  80
	TEnd
MEnd

Move Build_Repair2
	Requires	ENCOUNTER SMASH POSE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\repair_pound 10 80
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  10
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  24
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  38
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  52
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  66
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  80
	TEnd
MEnd

#############################################################

Move Build_Repair_low
	Requires	ENCOUNTER SMASH POSE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NExtMove	Build_Repair_low2
	Priority	3
	Scale		0.5

	Type		male
	  Anim          male\kneel_pound 1
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  10
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  24
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  38
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  52
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  66
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  80
	TEnd
MEnd

Move Build_Repair_low2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\kneel_pound 1
	  PlayFx        GENERIC\VANDAL_HAMMER.FX  0 Constant
          PlayFx        GENERIC\VANDAL_HAMMER2.FX  10
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  24
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  38
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  52
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  66
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX  80
	TEnd
MEnd

#############################################################

Move Build_Fix
	Requires	ENCOUNTER SMASH POSE HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NExtMove	Build_Fix2
	Scale		0.5

	Type		male
	  Anim          male\mekman_weld 1
	  PlayFx        GENERIC\welding_sparks.fx  0 Constant
	TEnd
MEnd

Move Build_Fix2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\mekman_weld 1
	  PlayFx        GENERIC\welding_sparks.fx  0 Constant
	TEnd
MEnd


############################
### Shotgun Ground to Air  #####
############################

Move SG_Ground_T_Air
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_2
	CycleMove	SG_Ground_T_Air_6
	CycleMove	SG_Ground_T_Air
	CycleMove	SG_Ground_T_Air_3

	Type		male
	  Anim          male\shot_shootup_L 1 88
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  26
	TEnd
MEnd

Move SG_Ground_T_Air_2
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_3
	CycleMove	SG_Ground_T_Air
	CycleMove	SG_Ground_T_Air_2
	Scale		0.2

	Type		male
	  Anim          male\shot_shootup_L 89
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move SG_Ground_T_Air_3
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_4
	CycleMove	SG_Ground_T_Air_5

	Type		male
	  Anim          male\shot_shootup_L 1 70
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  26
	TEnd
MEnd

Move SG_Ground_T_Air_4
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_6
	CycleMove	SG_Ground_T_Air_5
	CycleMove	SG_Ground_T_Air_4
	CycleMove	SG_Ground_T_Air_6

	Type		male
	  Anim          male\shot_shootup_L 26 70
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  1
	TEnd
MEnd

Move SG_Ground_T_Air_5
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air
	CycleMove	SG_Ground_T_Air_6
	CycleMove	SG_Ground_T_Air_2
	CycleMove	SG_Ground_T_Air_3

	Type		male
	  Anim          male\shot_shootup_L 71
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move SG_Ground_T_Air_6
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_7
	CycleMove	SG_Ground_T_Air_10
	CycleMove	SG_Ground_T_Air_8

	Type		male
	  Anim          male\shot_shootup_turn 1 30
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move SG_Ground_T_Air_7
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_6
	CycleMove	SG_Ground_T_Air
	CycleMove	SG_Ground_T_Air_3

	Type		male
	  Anim          male\shot_shootup_turn 31
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move SG_Ground_T_Air_8
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_9
	CycleMove	SG_Ground_T_Air_7
	CycleMove	SG_Ground_T_Air_9

	Type		male
	  Anim          male\shot_shootup_R 1 88
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  26
	TEnd
MEnd

Move SG_Ground_T_Air_9
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_10
	CycleMove	SG_Ground_T_Air_8
	CycleMove	SG_Ground_T_Air_7
	CycleMove	SG_Ground_T_Air_9
	Scale		0.2

	Type		male
	  Anim          male\shot_shootup_R 89
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

Move SG_Ground_T_Air_10
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_11
	CycleMove	SG_Ground_T_Air_12

	Type		male
	  Anim          male\shot_shootup_R 1 70
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  26
	TEnd
MEnd

Move SG_Ground_T_Air_11
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_11
	CycleMove	SG_Ground_T_Air_12
	CycleMove	SG_Ground_T_Air_7
	CycleMove	SG_Ground_T_Air_11

	Type		male
	  Anim          male\shot_shootup_R 26 70
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  1
	TEnd
MEnd

Move SG_Ground_T_Air_12
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW MID
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	SG_Ground_T_Air_8
	CycleMove	SG_Ground_T_Air_9
	CycleMove	SG_Ground_T_Air_10
	CycleMove	SG_Ground_T_Air_7

	Type		male
	  Anim          male\shot_shootup_R 71
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	TEnd
MEnd

###############################
###### Counter  	 ######
###############################

Move Counter
	Requires	ENCOUNTER OBSERVE LOW PROP WORK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\counter 1
	TEnd
MEnd


###############################
###### Vampire Stand  	 ######
###############################

Move VampireStand
	Requires	ENCOUNTER BUSY POSE MAGIC
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\vampire_stand 1
	TEnd
MEnd

###############################
###### Vampire Crouch  	 ######
###############################

Move VampireCrouch
	Requires	ENCOUNTER POSE CROUCH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\crouch_look 1    ####vampire_crouch 1
	TEnd

	Type		goblin_beast
	  Anim          Beast\crouch 1    ####vampire_crouch 1
	TEnd
MEnd

###############################
###### Ape Crouch  	 ######
###############################

Move ApeCrouch
	Requires	ENCOUNTER BUSY POSE MAGIC LOW STRONG
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          Beast\crouch 1    ####vampire_crouch 1
	TEnd
MEnd

##################################
### Skiff: Air 2 Ground attacks  #####
##################################

Move Air2Ground
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	NextMove	DriftDown
	Interpolate	30
	Sets		NOCOLLISION

	Type		male
	  Anim          male\jetpack_shoot 1
	  PlayFx        WEAPONS\Assault_Rifle.fx  0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX  116
	TEnd
MEnd

Move Air2Ground_2
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	NextMove	Air2Ground
	Interpolate	30
	Sets		NOCOLLISION

	Type		male
	  Anim          male\jetpack_shoot 116
	  PlayFx        WEAPONS\Assault_Rifle.fx 0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX 1
	TEnd
MEnd

Move Air2Ground_3
	Requires	ENCOUNTER LOITER ATTACK HIGH LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	NextMove	Air2Ground
	Interpolate	30
	Sets		NOCOLLISION

	Type		male
	  Anim          male\jetpack_shoot 50
	  PlayFx        WEAPONS\Assault_Rifle.fx 0 Constant
	  PlayFx        WEAPONFX\BASIC_SHOTGUNBUCKSHOT.FX 66
	TEnd
MEnd

Move DriftDown
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	JumpPost
	Interpolate	30
	Scale           0.50
	Sets		NOCOLLISION

	Type		male
	  Anim          male\air_rocket_cycle 1 31
	  PlayFx        WEAPONS\Assault_Rifle.fx 0 Constant
	TEnd
MEnd


###############################
### Weld Mid		#####
###############################

Move Weld_Mid
	Requires	ENCOUNTER WORK POSE WELDING MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\mekman_weld 1
	  PlayFx        GENERIC\welding_sparks.fx 0 Constant
	  PlayFx        GENERIC\welder.fx 0 Constant
	TEnd

	Type		mekman
	  Anim          male\mekman_weld 1
	  PlayFx        GENERIC\welding_sparks.fx 0 Constant
	TEnd
MEnd


###############################
### Weld LOW		#####
###############################

Move Weld_Low
	Requires	ENCOUNTER WORK POSE WELDING LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\mekman_weld_low 1
	  PlayFx        GENERIC\welding_sparks.fx 0 Constant
	  PlayFx        GENERIC\welder.fx 0 Constant
	TEnd

	Type		mekman
	  Anim          male\mekman_weld_low 1
	  PlayFx        GENERIC\welding_sparks.fx 0 Constant
	TEnd
MEnd


############################
### Ritchet Fix Mid  #####
###########################

Move RitchetFix_Mid
	Requires	ENCOUNTER WORK POSE CLUB MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\ratchet_fix 1 27
	  PlayFx        Generic\ratchet.fx 0 Constant
	  PlayFx        Generic\broken_turret.fx 2 Constant
	TEnd
MEnd


############################
### Gun Fix  #####
###########################

Move GunFix_1leg
	Requires	ENCOUNTER WORK POSE WEAPON REPAIR
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\gun_fix_1leg 1
	  PlayFx        Generic\crate_mystic.fx 0 Constant
	  PlayFx        Weapons\Assault_Rifle.fx 0 Constant
	TEnd
MEnd

#############################################################

Move GunFix_sit
	Requires	ENCOUNTER WORK POSE WEAPON REPAIR
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\gun_fix_sit 1
	  PlayFx        Generic\crate_mystic.fx 0 Constant
	  PlayFx        Weapons\Assault_Rifle.fx 0 Constant
	TEnd
MEnd

###################
### Robot off  #####
###################

Move Off
	Requires	ENCOUNTER QUICK SPAWN SLEEP
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable
	Sets            CANTMOVE
	NextMove        Activate

	Type		male
	  Anim          5cw\activate 1 5
	TEnd
MEnd

###################
### Robot Activate   #####
###################

Move Activate
	Requires	ENCOUNTER QUICK SPAWN SLEEP NO
	Priority	5
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable
	NextMove	Ready
	Sets            CANTMOVE
	Scale		0.75

	Type		male
	  Anim          5cw\activate 6
	  PlayFx	POWERS\Ability\Intelligence.fx
	  PlayFx	Powers\ElectricityControl\ElectricityBoltHit3.fx
	  PlayFx        FEET\ROBOFOOT2.FX 80
	  PlayFx        FEET\ROBOFOOT4.FX 107
	TEnd
MEnd

########################
### Encounter Boombox  #####
########################

Move Enc_Boombox
	Requires	ENCOUNTER BOX STRONG SHURIKEN
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_armsup 1
	  PlayFx        GENERIC\Boombox_sound_Loud.fx 0 Constant
	TEnd
MEnd

#############################################################
## MELEE FISTS
#############################################################

Move Melee_Fists_A
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_B

	Type		male
	  Anim          male\brawlcycle 15 31
	TEnd
MEnd

Move Melee_Fists_B
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_B2

	Type		male
	  Anim          male\brawl_high2 50 65
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Melee_Fists_B2
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_C
	SetsOnReactor	ENCOUNTER RUMBLEA PUNCH LOW HIT

	Type		male
	  Anim          male\brawl_high2 66
	TEnd
MEnd

Move Melee_Fists_C
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_C2
	SetsOnReactor	ENCOUNTER RUMBLEA PUNCH LOW HIT BLOCK

	Type		male
	  Anim          male\1_2punch 1 20
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX
	TEnd
MEnd

Move Melee_Fists_C2
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_D

	Type		male
	  Anim          male\1_2punch 21
	TEnd
MEnd

Move Melee_Fists_D
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_D2

	Type		male
	  Anim          male\gut_punch 1 14
	TEnd
MEnd

Move Melee_Fists_D2
	Requires	ENCOUNTER RUMBLEA PUNCH HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Melee_Fists_A
	SetsOnReactor	ENCOUNTER RUMBLEA PUNCH LOW HIT MID

	Type		male
	  Anim          male\gut_punch 15
	TEnd
MEnd

#############################################################
##  MELEE FISTS REACT
#############################################################

Move Melee_Fists_React
	Requires	ENCOUNTER RUMBLEA PUNCH LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
#	Cyclemove	Melee_Fists_React_Hit ### not sure why but this isnt setup with a next move but the fist_melee is. odd.

	Type		male
	  Anim          male\brawlcycle 15 31
	TEnd
MEnd

#############################################################

Move Melee_Fists_React_Hit
        Requires	ENCOUNTER RUMBLEA PUNCH LOW HIT
        Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Melee_Fists_React
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 3
	TEnd
MEnd

#############################################################

Move Melee_Fists_React_Hit_Mid
        Requires	ENCOUNTER RUMBLEA PUNCH LOW HIT MID
        Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Melee_Fists_React
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_mid 3
	TEnd
MEnd

#############################################################

Move Melee_Fists_React_Hit_Block
        Requires	ENCOUNTER RUMBLEA PUNCH LOW HIT BLOCK
        Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Melee_Fists_React
	Flags		NoSizeScale

  	Type		male
	  Anim          male\combat_block 1 50
	TEnd
MEnd

#############################################################
## STATESMAN SHACKLED
#############################################################

Move StatesShackle_A
	Requires	ENCOUNTER CAGE FORCE EVIL LOW HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	NextMove	States_Fall
	CycleMove	StatesShackle_B
	CycleMove	StatesShackle_A
	Scale           1.5
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\states_shackle_struggle_A 1
	  PlayFX	World\Events\StatesmanShackles.fx 0 Constant
	TEnd
MEnd

Move StatesShackle_B
	Requires	ENCOUNTER CAGE FORCE EVIL LOW HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	NextMove	States_Fall
	CycleMove	StatesShackle_A
	CycleMove	StatesShackle_B
	Scale           1.5
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\states_shackle_breath 1
	  PlayFX	World\Events\StatesmanShackles.fx 0 Constant
	TEnd
MEnd

#############################################################

Move States_Fall
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	States_Land
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\states_shackle_fall 1 89
	TEnd
MEnd

Move States_Land
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        States_Thank
	Scale		0.5
	Interpolate     8
	Sets            CANTMOVE

	Type		male
	  Anim          male\stand_2Hhips 10 64
	TEnd
MEnd

Move States_Thank
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        States_Done
	Sets            CANTMOVE

	Type		male
	  Anim          male\your_welcome_sm 1
	TEnd
MEnd

Move States_Done
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Ready_2Hips_look
	Scale		0.4
	Interpolate	8

	Type		male
	  Anim          male\stand_2Hhips 11 64
	TEnd
MEnd

## PLAYING DICE ###########################################################

Move PlayDice_cast
	Requires	ENCOUNTER POSE ROLLDICE HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	PlayDice_Winner
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Prop_Dice.fx 69
	TEnd
MEnd

Move PlayDice_Winner
	Requires	ENCOUNTER POSE ROLLDICE HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	PlayDice_Pickup
	NextMove        Ready
	SetsOnReactor	ENCOUNTER EMOTE ANGRY RUDE SHRUG

	Type		male
	  Anim          male\victory 1 195
	TEnd
MEnd

Move PlayDice_Pickup
	Requires	ENCOUNTER POSE ROLLDICE HIGH
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	PlayDice_cast
	NextMove        Ready

	Type		male
	  Anim          male\roll_dice 125 201
	TEnd
MEnd


## JEERING ###########################################################

Move Jeering_A
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_B
	CycleMove	Jeering_C
	CycleMove	Jeering_E
	CycleMove	Jeering_F
	NextMove        Ready

	Type		male
	  Anim          male\threat_combo_A 1 134
	TEnd
MEnd

Move Jeering_B
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_A
	CycleMove	Jeering_C
	CycleMove	Jeering_F
	Priority	4

	Type		male
	  Anim          male\comb_beatchest 1 91
	  	PlayFX		NPCFX\NPC_ChestThump_SFX.fx 10
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 15
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 20
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 25
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 30
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 35
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 40
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 45
		PlayFX		NPCFX\NPC_ChestThump_SFX.fx 50
	TEnd
MEnd

Move Jeering_C
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_E
	CycleMove	Jeering_B
	CycleMove	Jeering_D

	Type		male
	  Anim          male\threat_D 1 52
	TEnd
MEnd

Move Jeering_D
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_A
	CycleMove	Jeering_B
	CycleMove	Jeering_C
	CycleMove	Jeering_E

	Type		male
	  Anim          male\flex_A 5 54
	TEnd
MEnd

Move Jeering_E
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_D
	CycleMove	Jeering_F
	CycleMove	Jeering_B
	CycleMove	Jeering_C
	NextMove        Ready

	Type		male
	  Anim          male\flex_A 5 54
	TEnd
MEnd

Move Jeering_F
	Requires	ENCOUNTER ANGRY TAUNTA TAUNTB TAUNTC
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Jeering_B
	CycleMove	Jeering_B
	CycleMove	Jeering_B
	Interpolate	1

	Type		male
	  Anim          male\multi_jump 4
	  PlayFX	Feet\Foot_NPCMale.fx 12
	  PlayFX	Feet\Foot_NPCMale.fx 34
	  PlayFX	Feet\Foot_NPCMale.fx 56
	TEnd
MEnd


########################
### Magic Shield  #####
########################

Move Magic_Shield_A
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Magic_Shield_B
	CycleMove	Magic_Shield_C
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\heal 1
	TEnd
MEnd

Move Magic_Shield_B
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Magic_Shield_C
	CycleMove	Magic_Shield_A
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\hypnotize 1
	TEnd
MEnd

Move Magic_Shield_C
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Magic_Shield_B
	CycleMove	Magic_Shield_A
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\levitate 1
	TEnd
MEnd


###################
### Surrounded ####
####################

Move Surrounded_A
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	CycleMove       Surrounded_A2

	Type		male
	  Anim          male\suspicious 1 50
	TEnd
MEnd

Move Surrounded_A2
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	Interpolate     8
	CycleMove       Surrounded_A3

	Type		male
	  Anim          male\suspicious 63
	TEnd
MEnd

Move Surrounded_A3
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Surrounded_A4

	Type		male
	  Anim          male\suspicious 63
	TEnd
MEnd

Move Surrounded_A4
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Surrounded_A

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd


########################
### Fire Marshall  #####
#######################

Move FireMarshall_A
	Requires	ENCOUNTER WORK FIRE BACKPACK HELP
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	FireMarshall_B
	CycleMove	FireMarshall_A
	CycleMove	FireMarshall_D
	NextMove        Ready
	Interpolate	15

	Type		male
	  Anim          male\whistle 1 50
	  PlayFx        FEET\WHISTLE.fx 19
	  PlayFX	POWERS\ItemsAndTempPowers\WaterBackPack.fx 0 Constant
	  PlayFX	Generic\FireMarshall_props.fx 0 Constant
	TEnd
MEnd

Move FireMarshall_B
	Requires	ENCOUNTER WORK FIRE BACKPACK HELP
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	FireMarshall_A
	CycleMove	FireMarshall_C
	CycleMove	FireMarshall_D
	NextMove        Ready
	Interpolate	15
	Scale		0.5

	Type		male
	  Anim          male\yell 65 92
	  PlayFX	POWERS\ItemsAndTempPowers\WaterBackPack.fx 0 Constant
	  PlayFX	Generic\FireMarshall_props.fx 0 Constant
	TEnd
MEnd

Move FireMarshall_C
	Requires	ENCOUNTER WORK FIRE BACKPACK HELP
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	FireMarshall_A
	CycleMove	FireMarshall_D
	CycleMove	FireMarshall_B
	CycleMove	FireMarshall_A
	NextMove        Ready
	Interpolate	15

	Type		male
	  Anim          male\over_here 1 88
	  PlayFX	POWERS\ItemsAndTempPowers\WaterBackPack.fx 0 Constant
	  PlayFX	Generic\FireMarshall_props.fx 0 Constant
	TEnd
MEnd

Move FireMarshall_D
	Requires	ENCOUNTER WORK FIRE BACKPACK HELP
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	FireMarshall_A
	CycleMove	FireMarshall_B
	NextMove        Ready
	Interpolate	15

	Type		male
	  Anim          male\lookquick 1
	  PlayFX	POWERS\ItemsAndTempPowers\WaterBackPack.fx 0 Constant
	  PlayFX	Generic\FireMarshall_props.fx 0 Constant
	TEnd
MEnd


#######################
### Caged Croatoa  #####
########################

Move Caged_Croatoa
	Requires	ENCOUNTER FEAR CAGE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove	DojoKneel_getup

	Type		male
	  Anim          male\kneel_comp 24 26
	  PlayFX	WORLD\events\AmyJohnsonCage.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\kneel_comp 24 26
	  PlayFX	WORLD\events\AmyJohnsonCage.fx 0 constant
	TEnd
MEnd


######## talks  ###############################################

####################
### Talk (default) #####
####################

Move Talk_pre
	Requires	ENCOUNTER TALK
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Talk

	Type		male
	  Anim          male\clipboard_t_xarms 1
	TEnd
MEnd

Move Talk
	Requires	ENCOUNTER TALK
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\clipboard_t_xarms 74 75
	TEnd
MEnd




##############################
### Natterling break in  #####
#############################

Move Natterling_BreakIn_A
	Requires	ENCOUNTER VANDALIZE MID THROWKNIFE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\examine_pullguts 1 76   ##air_baseball_punch 23
	TEnd
MEnd

#############################################################

Move Natterling_BreakIn_B
	Requires	ENCOUNTER VANDALIZE MID THROWKNIFE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\multi_jump 1
	TEnd
MEnd

#############################################################

Move Natterling_BreakIn_C
	Requires	ENCOUNTER VANDALIZE MID THROWKNIFE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          male\jump_N_smash 1
	  Scale		1.5
	TEnd
MEnd


###################
### Smash Wall  #####
###################

Move smash_wall_A
	Requires	ENCOUNTER VANDALIZE WALL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          Vahzilok\bat_vandal 1
	TEnd
MEnd

#############################################################

Move smash_wall_B
	Requires	ENCOUNTER VANDALIZE WALL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          Vahzilok\front_pound 1
	TEnd
MEnd

#############################################################

Move smash_wall_C
	Requires	ENCOUNTER VANDALIZE WALL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly

	Type		male
	  Anim          Vahzilok\side_pound 1
	TEnd
MEnd

###################
### Mace Alert   #####
###################

Move Mace_Alert
	Requires	ENCOUNTER COMBAT WEAPON BLUNT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale		0.5

	Type		male
	  Anim          male\wep_cycle 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

###################
### Mace Threat   #####
###################

Move Mace_Threat_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Mace_Threat_A2
	CycleMove	Mace_Threat_A
	CycleMove	Mace_Threat_A3

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Threat_A2
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Mace_Threat_A3
	CycleMove	Mace_Threat_A
	CycleMove	Mace_Threat_A

	Type		male
	  Anim          male\bat_threatA 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Threat_A3
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Mace_Threat_A
	CycleMove	Mace_Threat_A2
	CycleMove	Mace_Threat_A

	Type		male
	  Anim          male\bat_threatB 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

#############################################################

Move Mace_Threat_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Mace_Threat_B2
	CycleMove	Mace_Threat_B
	Scale		0.7

	Type		male
	  Anim          male\bat_twirl 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Threat_B2
	Requires	ENCOUNTER COMBAT WEAPON BLUNT THREAT HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Mace_Threat_B2
	CycleMove	Mace_Threat_B
	CycleMove	Mace_Threat_B
	Scale		0.8

	Type		male
	  Anim          male\bat_twirl 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd


###########################################
### Mace MENACE (more looking arounds)   #####
############################################

Move Mace_Menace_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Mace_Menace_B2
	CycleMove       Mace_Menace_B4
	CycleMove       Mace_Menace_B3

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_B2
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_B
	CycleMove       Mace_Menace_B3
	CycleMove       Mace_Menace_B2
	Scale		0.7

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_B3
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_B
	CycleMove       Mace_Menace_B4
	Scale		0.5

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_B4
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_B
	Scale		0.8

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

#############################################################

Move Mace_Menace_C
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       Mace_Menace_C2
	CycleMove       Mace_Menace_C2
	CycleMove       Mace_Menace_C4
	Scale		0.5

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_C2
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_C
	CycleMove       Mace_Menace_C2
	Scale		0.8

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_C3
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_C
	CycleMove       Mace_Menace_C2

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

Move Mace_Menace_C4
	Requires	ENCOUNTER COMBAT WEAPON BLUNT MENACE HIGH
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Mace_Menace_C2
	CycleMove       Mace_Menace_C
	CycleMove       Mace_Menace_C3
	Scale		0.7

	Type		male
	  Anim          male\bat_cycle 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd


###################
### Mace Lookout   #####
###################

Move MaceLookout_A
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove       MaceLookout_B
	CycleMove       MaceLookout_C
	Scale           0.50

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

#############################################################

Move MaceLookout_B
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.90
	CycleMove       MaceLookout_C
	CycleMove       MaceLookout_A

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd

#############################################################

Move MaceLookout_C
	Requires	ENCOUNTER COMBAT WEAPON BLUNT LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.7
	CycleMove       MaceLookout_A
	CycleMove       MaceLookout_B

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 Constant
	TEnd
MEnd


###################
### Hands Up  #####
###################

Move handsup
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup_02
	CycleMove	handsup_03
	CycleMove	handsup_04
	CycleMove	handsup
	Interpolate	25

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move handsup_02
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup_03
	CycleMove	handsup_04
	CycleMove	handsup
	CycleMove	handsup_02
	interpolate	25

	Type		male
	  Anim          male\handsup_var 1
	TEnd
MEnd

Move handsup_03
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup_04
	CycleMove	handsup
	CycleMove	handsup_02
	CycleMove	handsup_03
	interpolate	25
	scale		0.8

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move handsup_04
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup
	CycleMove	handsup_02
	CycleMove	handsup_03
	CycleMove	handsup_04
	interpolate	25
	scale		0.8

	Type		male
	  Anim          male\handsup_var 1
	TEnd
MEnd

Move handsup_05
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup
	CycleMove	handsup_02
	CycleMove	handsup_03
	CycleMove	handsup_04
	interpolate	25
	scale		1.25

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move handsup_06
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup
	CycleMove	handsup_02
	CycleMove	handsup_03
	CycleMove	handsup_04
	interpolate	25
	scale		1.25

	Type		male
	  Anim          male\handsup_var 1
	TEnd
MEnd


#########################
### Hands Up Kneeling ###
#########################

Move handsup_knees
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsup_knees_02
	CycleMove	handsup_knees
	interpolate	20

	Type		male
	  Anim          male\handsup_knees 1
	TEnd
MEnd

Move handsup_knees_02
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	CycleMove	handsup_knees
	CycleMove	handsup_knees_02
	interpolate	25

	Type		male
	  Anim          male\handsup_knees_var 1
	TEnd
MEnd


######################
### Hands on Head  ###
######################

Move handsonhead
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD HIGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead_02
	CycleMove	handsonhead_03
	CycleMove	handsonhead
	interpolate	20

	Type		male
	  Anim          male\handsonhead 1
	TEnd
MEnd

Move handsonhead_02
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD HIGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead_03
	CycleMove	handsonhead
	CycleMove	handsonhead_02
	interpolate	30

	Type		male
	  Anim          male\handsonhead_var1 1
	TEnd
MEnd

Move handsonhead_03
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD HIGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead
	CycleMove	handsonhead_02
	CycleMove	handsonhead_03
	interpolate	20

	Type		male
	  Anim          male\handsonhead_var2 1
	TEnd
MEnd


###############################
### Hands on Head Kneeling  ###
###############################

Move handsonhead_knees
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD LOW
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead_knees_02
	CycleMove	handsonhead_knees_03
	CycleMove	handsonhead_knees
	interpolate	25

	Type		male
	  Anim          male\handsonhead_knees 1
	TEnd
MEnd

Move handsonhead_knees_02
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD LOW
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead_knees_03
	CycleMove	handsonhead_knees
	CycleMove	handsonhead_knees_02
	interpolate	25

	Type		male
	  Anim          male\handsonhead_knees_var1 1
	TEnd
MEnd

Move handsonhead_knees_03
	Requires	ENCOUNTER 2HAND STOP THREAT HYPNOTIZE HEAD LOW
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	handsonhead_knees
	CycleMove	handsonhead_knees_02
	CycleMove	handsonhead_knees_03
	interpolate	25

	Type		male
	  Anim          male\handsonhead_knees_var2 1
	TEnd
MEnd

########################
### Sitting on Ledge ###
########################

Move ledgesit_var1a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var1b
	CycleMove	ledgesit_var1c
	CycleMove	ledgesit_var1a

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

#############################################################

Move ledgesit_var1b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var1c
	CycleMove	ledgesit_var1a
	CycleMove	ledgesit_var1b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

#############################################################

Move ledgesit_var1c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var1a
	CycleMove	ledgesit_var1b
	CycleMove	ledgesit_var1c
	scale		0.75

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

#############################################################

Move ledgesit_var2a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var2b
	CycleMove	ledgesit_var2c
	CycleMove	ledgesit_var2a

	Type		male
	  Anim          male\ledgesit_var2 1
	TEnd
MEnd

Move ledgesit_var2b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var2c
	CycleMove	ledgesit_var2a
	CycleMove	ledgesit_var2b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var2 1
	TEnd
MEnd

Move ledgesit_var2c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var2a
	CycleMove	ledgesit_var2b
	CycleMove	ledgesit_var2c
	scale		0.75


	Type		male
	  Anim          male\ledgesit_var2 1
	TEnd
MEnd

#############################################################

Move ledgesit_var3a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var3b
	CycleMove	ledgesit_var3c
	CycleMove	ledgesit_var3a

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

Move ledgesit_var3b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var3c
	CycleMove	ledgesit_var3a
	CycleMove	ledgesit_var3b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

Move ledgesit_var3c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var3a
	CycleMove	ledgesit_var3b
	CycleMove	ledgesit_var3c
	scale		0.75

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

#############################################################

Move ledgesit_var4a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var4b
	CycleMove	ledgesit_var4c
	CycleMove	ledgesit_var4a

	Type		male
	  Anim          male\ledgesit_var4 1
	TEnd
MEnd

Move ledgesit_var4b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var4c
	CycleMove	ledgesit_var4a
	CycleMove	ledgesit_var4b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var4 1
	TEnd
MEnd

Move ledgesit_var4c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE
	Priority	6
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	ledgesit_var4a
	CycleMove	ledgesit_var4b
	CycleMove	ledgesit_var4c
	scale		0.75

	Type		male
	  Anim          male\ledgesit_var4 1
	TEnd
MEnd


######################################
### Soul give (Drugged Sleep w\ fx)  ###
######################################

Move Soulgive_sleep
	Requires	ENCOUNTER POSE SAD HEAD ZOMBIE QUICK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Sets            CANTMOVE
	interpolate	5
	nextmove	pronesleep_getup

	Type		male
	  Anim          male\pronesleep_cycle 1
	  PlayFx	POWERS\Ability\Enc_Willpower.fx 0 constant
	TEnd
MEnd


###########################
### Prone (Drugged Sleep)  ###
###########################

Move pronesleep
	Requires	ENCOUNTER POSE SAD HEAD ZOMBIE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Sets            CANTMOVE
	interpolate	5
	nextmove	pronesleep_getup
	CycleMove	pronesleep
	CycleMove	pronesleep2
	CycleMove	pronesleep3

	Type		male
	  Anim          male\pronesleep_cycle 1
	TEnd
MEnd

Move pronesleep2
	Requires	ENCOUNTER POSE SAD HEAD ZOMBIE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Sets            CANTMOVE
	interpolate	5
	nextmove	pronesleep_getup
	CycleMove	pronesleep
	CycleMove	pronesleep2
	CycleMove	pronesleep3
	scale		0.75

	Type		male
	  Anim          male\pronesleep_cycle 1
	TEnd
MEnd

Move pronesleep3
	Requires	ENCOUNTER POSE SAD HEAD ZOMBIE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Sets            CANTMOVE
	interpolate	5
	nextmove	pronesleep_getup
	CycleMove	pronesleep
	CycleMove	pronesleep2
	CycleMove	pronesleep3
	scale		1.5

	Type		male
	  Anim          male\pronesleep_cycle 1
	TEnd
MEnd

#############################################################

Move pronesleep_getup
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        pronesleep_getup_cycle
	Sets            CANTMOVE
	Interpolate     5

	Type		male
	  Anim          male\pronesleep_awaken 1
	TEnd
MEnd

Move pronesleep_getup_cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\zombie_ALT_ready 1
	TEnd
MEnd

#########################
### Snapping Picture  ###
#########################

Move picture_snap
	Requires	ENCOUNTER SHOOT POINT CONE QUICK HAPPY
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	interpolate	20

	Type		male
	  Anim          male\cameraman_takepicture 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	  PLayFX		NPCFX\CameraSnap.fx 233
	  PLayFX		NPCFX\CameraAdvance.fx 270
	TEnd
MEnd

#########################
### Cross Swords      ###
#########################

Move Cross_sword_l
	Requires	ENCOUNTER BLOCK LOW HIGH SWORD TELEKINESIS
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\block_sword_l 1
	  PlayFx	WEAPONS\sword_left.fx 0 Constant
	TEnd
MEnd

############################
###### MENACE Fire Toss ######
############################

Move MenaceFire_Idle_1
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	MenaceFire_Idle_2
	NextMove	MenaceFire_Toss
	Sets            CANTMOVE

	Type		male
	  Anim          male\bat_threat_combo_A 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move MenaceFire_Idle_2
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	MenaceFire_Idle_3
	NextMove	MenaceFire_Toss
	Sets            CANTMOVE

	Type		male
	  Anim          male\knife_lookquick 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move MenaceFire_Idle_3
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	MenaceFire_Idle_4
	NextMove	MenaceFire_Toss
	Sets            CANTMOVE

	Type		male
	  Anim          male\bat_overheadshake 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move MenaceFire_Idle_4
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT MENACE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	MenaceFire_Idle_1
	CycleMove	MenaceFire_Idle_4
	NextMove	MenaceFire_Toss
	Sets            CANTMOVE
	Scale		1.5

	Type		male
	  Anim          male\knifetoss 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move MenaceFire_Toss
	Include sequencers\Members\Encounters_Cycles.inc
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Toss 45 115
	  PlayFX	Explosions\medium\Enc_Expolsion_med.fx 52
	  PlayFX	WORLD\Ruins\Enc_MediumFireFast.fx 52
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd


######################
###### Fishman Leap from water  ######
#####################

Move Fishman_sleep
	Requires	ENCOUNTER LOW SPAWN WAIT HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale NotSelectable
	NextMove        Fishman_Leap
	Interpolate     0
	Sets            CANTMOVE NOCOLLISION
	Scale           0.02

	Type		male
	  Anim          Beast\fishman_leap 1 2
	  PlayFx        GENERIC\ENTITYVANISH.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Fishman_Leap
	Include sequencers\Members\Encounters_Cycles.inc
	Sets            CANTMOVE
	NextMove	Fishman_Leap_cycle
	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim          Beast\fishman_leap 3
	TEnd
MEnd

Move Fishman_Leap_cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#####################
### Katana Rumble A  ###
#####################

Move Katana_Rumble_A
	Requires	ENCOUNTER KATANA RUMBLEA LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\sword_rumble_A 1 219
	  PlayFX	Weapons\katana.fx 1 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 10
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 58
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 145

	TEnd
MEnd

#####################
### Katana Rumble B  ###
#####################

Move Katana_Rumble_B
	Requires	ENCOUNTER KATANA RUMBLEB LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\sword_rumble_dizzy_B 1 219
	  PlayFX	Weapons\katana.fx 1 Constant
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 65
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 63
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 145
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 160
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 155
	TEnd
MEnd

###################
### Katana Alert  #####
###################

Move Katana_Alert
	Requires	ENCOUNTER KATANA POSE MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Scale           0.50

	Type		male
	  Anim          male\katana_cycle 1 31
	  PlayFx	WEAPONS\katana_left.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 1 31
	  PlayFx	WEAPONS\katana_left.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 1 31
	  PlayFx	WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

###################
### Katana Point  #####
###################

Move Katana_Point
	Requires	ENCOUNTER COMBAT WEAPON KATANA POINT STRONG
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\katana_point 1
	  PlayFx        WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

###################
### Katana OBSERVE   #####
###################

Move Katana_Observe_A
	Requires	ENCOUNTER COMBAT WEAPON KATANA OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Katana_Observe_B
	CycleMove       Katana_Observe_A
	Scale           0.50

	Type		male
	  Anim          male\bat_look_R_down 1
	  PlayFx        WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

Move Katana_Observe_B
	Requires	ENCOUNTER COMBAT WEAPON KATANA OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.7
	CycleMove       Katana_Observe_A
	CycleMove       Katana_Observe_B

	Type		male
	  Anim          male\bat_look_L_down 1
	  PlayFx        WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

###########################################
### Katana Lookout (more looking arounds)   #####
############################################

Move Katana_Lookout_A
	Requires	ENCOUNTER COMBAT WEAPON KATANA LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Katana_Lookout_B
	CycleMove       Katana_Lookout_A
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	  PlayFx        WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

Move Katana_Lookout_B
	Requires	ENCOUNTER COMBAT WEAPON KATANA LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Katana_Lookout_A
	CycleMove       Katana_Lookout_B
	Scale		0.8

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

#########################
### Cross Katanas      ###
#########################

Move Cross_Katana_l
	Requires	ENCOUNTER BLOCK LOW HIGH KATANA TELEKINESIS
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\block_sword_l 1
	  PlayFx	WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd


############################
#### Guard Katana held up ###
############################

Move guard_Katana_l
	Requires	ENCOUNTER COMBAT WEAPON KATANA LOOKOUT HIGH LOW
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.3

	Type		male
	  Anim          male\guard_sword_l 1
  	  PlayFx	WEAPONS\katana_left.fx 0 Constant
	TEnd
MEnd

###################
### Sword Point  #####
###################

Move Sword_Point
	Requires	ENCOUNTER COMBAT WEAPON SWORD POINT STRONG
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\katana_point 1
	  PlayFx        WEAPONS\sword_left.fx 0 Constant
	TEnd
MEnd

###################
### Sword Alert   #####
###################

Move Sword_Alert
	Requires	ENCOUNTER SWORD POSE MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\wep_cycle 1
	  PlayFx        WEAPONS\sword.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\wep_cycle 1 31
	  PlayFx        WEAPONS\sword.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\wep_cycle 1 31
	  PlayFx        WEAPONS\sword.fx 0 Constant
	TEnd
MEnd


###################
### Sword OBSERVE   #####
###################

Move Sword_Observe_A
	Requires	ENCOUNTER COMBAT WEAPON SWORD OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Sword_Observe_B
	CycleMove       Sword_Observe_A
	Scale           0.50

	Type		male
	  Anim          male\bat_look_R_down 1
	  PlayFx        WEAPONS\SWORD.fx 0 Constant
	TEnd
MEnd

Move Sword_Observe_B
	Requires	ENCOUNTER COMBAT WEAPON SWORD OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.7
	CycleMove       Sword_Observe_A
	CycleMove       Sword_Observe_B

	Type		male
	  Anim          male\bat_look_L_down 1
	  PlayFx        WEAPONS\SWORD.fx 0 Constant
	TEnd
MEnd

############################
#### Guard SWORD held up ###
############################

Move guard_sword_l
	Requires	ENCOUNTER COMBAT WEAPON SWORD LOOKOUT HIGH LOW
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.3

	Type		male
	  Anim          male\guard_sword_l 1
	  PlayFx	WEAPONS\SWORD.fx 0 Constant
	TEnd
MEnd


###########################
### Bum begging with cup ###
###########################

Move bum_cup_cycle
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	cyclemove	bum_cup_cycle
	cyclemove	bum_cup_beg_r_pre
	cyclemove	bum_cup_jingle
	cyclemove	bum_cup_beg_l_pre

	Type		male
	  Anim          male\bum_cup_cycle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_l_pre
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	bum_cup_beg_l

	Type		male
	  Anim          male\bum_cup_beg_l 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_l
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Interpolate	13
	cyclemove	bum_cup_beg_l_post
	cyclemove	bum_cup_beg_r

	Type		male
	  Anim          male\bum_cup_beg_l 35 80 ##85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_l_post
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	cyclemove	bum_cup_jingle
	cyclemove	bum_cup_beg_l_pre
	cyclemove	bum_cup_cycle
	cyclemove	bum_cup_beg_r_pre

	Type		male
	  Anim          male\bum_cup_beg_l 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_r_pre
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	Interpolate	15
	NextMove	bum_cup_beg_r

	Type		male
	  Anim          male\bum_cup_beg_r 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_r
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Interpolate	13
	cyclemove	bum_cup_beg_r_post
	cyclemove	bum_cup_beg_l

	Type		male
	  Anim          male\bum_cup_beg_r 35 80 #85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_beg_r_post
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	cyclemove	bum_cup_jingle
	cyclemove	bum_cup_beg_l_pre
	cyclemove	bum_cup_cycle
	cyclemove	bum_cup_beg_r_pre

	Type		male
	  Anim          male\bum_cup_beg_r 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move bum_cup_jingle
	Requires	ENCOUNTER COWER AXE SAD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	bum_cup_cycle

	Type		male
	  Anim          male\bum_cup_jingle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

###############################
#### Leaning against a wall ###  requires flags are prolly wrong, ripped from ledge sit
##############################

Move wall_lean1
	Requires	ENCOUNTER POSE LEANWALL ONE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd

Move wall_lean2
	Requires	ENCOUNTER POSE LEANWALL ONE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\wall_lean2 1
	TEnd
MEnd

Move wall_lean3
	Requires	ENCOUNTER POSE LEANWALL ONE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\wall_lean3 1
	TEnd
MEnd

#Move wall_lean4					# commented out by bwyser on 06\26\06 per lead's request.  They just didn't like the pose.
#	Requires	ENCOUNTER ANGRY THREAT MENACE HIGH POSE
#	Include sequencers\Members\Encounters_Cycles.inc
#	Include sequencers\Members\Encounters_Cycles.inc
#	Flags		Cycle PICKRANDOMLY NoSizeScale
#	Priority	5
#
#
#	Type		male
#	  Anim          male\wall_lean4 1
#
#	TEnd
#MEnd

#############################################################
## Wall Lean Casual
#############################################################

Move wall_lean_casual_1
	Requires	ENCOUNTER POSE LEANWALL TWO
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd


##############################
### Bums sleeping against wall ###  requires flags are prolly wrong, ripped from ledge sit
##############################

Move bum_sleep1
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\bum_sleep1 1
	TEnd
MEnd

Move bum_sleep2
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\bum_sleep2 1
	TEnd
MEnd

Move bum_sleep3
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\bum_sleep3 1
	TEnd
MEnd

Move bum_sleep4
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\bum_sleep4 1
	TEnd
MEnd

Move bum_sleep5
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale

	Type		male
	  Anim          male\bum_sleep5 1
	TEnd
MEnd


##############################
### Bums sleeping against wall - CUSTOM for cutscene only
##############################

Move bum_sleep_cutscene
	Requires	ENCOUNTER SIT LOVE SAD HIGH POSE JUSTFUCKINDOIT
	Priority	7
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable Cycle

	Type		male
	  Anim          male\bum_sleep1 1
	TEnd
MEnd

#########################################
### chair sit (ledge sit sans chair) ###
#########################################

Move chair_var1a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	chair_var1b
	CycleMove	chair_var1c
	CycleMove	chair_var1a

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

Move chair_var1b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	chair_var1c
	CycleMove	chair_var1a
	CycleMove	chair_var1b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

Move chair_var1c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove	chair_var1a
	CycleMove	chair_var1b
	CycleMove	chair_var1c
	interpolate	30
	scale		0.75

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

Move chair_var3a
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove	chair_var3b
	CycleMove	chair_var3c
	CycleMove	chair_var3a
	interpolate	30

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

Move chair_var3b
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	chair_var3c
	CycleMove	chair_var3a
	CycleMove	chair_var3b
	scale		0.5

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

Move chair_var3c
	Requires	ENCOUNTER SIT THREAT MENACE HIGH POSE ARMCHAIR
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	30
	CycleMove	chair_var3a
	CycleMove	chair_var3b
	CycleMove	chair_var3c
	scale		0.75

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd


###############################
#### Archer looking around  ###
##############################

Move bow_lookout_A
	Requires	COMBAT WEAPON DUAL REV LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	bow_lookout_C
	CycleMove	bow_lookout_B
	CycleMove	bow_lookout_A
	Scale		0.7

	Type		male
	  Anim          male\bow_lookout 1
	  PlayFx        WEAPONS\Bow\New_bow.fx 0 Constant
	  PlayFx        WEAPONS\Bow\arrow_Enc.fx 0 Constant
	TEnd
MEnd

Move bow_lookout_B
	Requires	COMBAT WEAPON DUAL REV LOOKOUT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	bow_lookout_A
	CycleMove	bow_lookout_C
	Scale		0.5

	Type		male
	  Anim          male\bow_lookout 1
	  PlayFx        WEAPONS\Bow\New_bow.fx 0 Constant
	  PlayFx        WEAPONS\Bow\arrow_Enc.fx 0 Constant
	TEnd
MEnd

Move bow_lookout_C
	Requires	COMBAT WEAPON DUAL REV LOOKOUT
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	bow_lookout_B
	CycleMove	bow_lookout_A
	Scale		0.6

	Type		male
	  Anim          male\bow_lookout 1
	  PlayFx        WEAPONS\Bow\New_bow.fx 0 Constant
	  PlayFx        WEAPONS\Bow\arrow_Enc.fx 0 Constant
	TEnd
MEnd

####################
#### Protesting ####   ### needs pipe weapon instead of bat
###################    requires flags are prolly wrong, ripped from SWORD block\guard

Move protest_idle
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_shakefist_pre
	CycleMove	protest_yell_pre
	CycleMove	protest_idle
	CycleMove	protest_rally
	interpolate	25

	Type		male
	  Anim          male\protest_idle 1
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_shakefist_pre
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_shakefist_cycle
	interpolate	15

	Type		male
	  Anim          male\protest_pipe1 1 40
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_shakefist_cycle
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_shakefist_cycle
	CycleMove	protest_shakefist_cycle
	CycleMove	protest_shakefist_post
	CycleMove	protest_shakefist_cycle
	interpolate	15

	Type		male
	  Anim          male\protest_pipe1 20 40
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_shakefist_post
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_yell_pre
	CycleMove	protest_idle
	CycleMove	protest_rally
	CycleMove	protest_shakefist_pre
	interpolate	15

	Type		male
	  Anim          male\protest_pipe1 20
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_rally
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_rally
	CycleMove	protest_rally
	CycleMove	protest_rally_post
	interpolate	15
	scale		1.5

	Type		male
	  Anim          male\protest_pipe2 1
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_rally_post
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_shakefist_post
	CycleMove	protest_yell_post
	interpolate	15
	scale		1.5

	Type		male
	  Anim          male\protest_pipe2 1
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_yell_pre
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_yell_cycle
	interpolate	15
	scale		1.5

	Type		male
	  Anim          male\protest_pipe3 1 80
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_yell_cycle
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_yell_cycle
	CycleMove	protest_yell_cycle
	CycleMove	protest_yell_post
	interpolate	15
	scale		1.5

	Type		male
	  Anim          male\protest_pipe3 50 80
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move protest_yell_post
	Requires	ENCOUNTER ANGRY YELL SPEECH NO SCREAM BLUNT
	Priority	7
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	protest_rally
	CycleMove	protest_shakefist_pre
	CycleMove	protest_yell_pre
	CycleMove	protest_idle
	interpolate	15
	scale		1.5

	Type		male
	  Anim          male\protest_pipe3 50
    	  PlayFx	Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd


###################
### Enc Barf   #####
###################

Move Enc_Barf_A
	Requires	ENCOUNTER POISON BLOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg PickRandomly
	CycleMove	Enc_Barf_B
	CycleMove	Enc_Barf_C

	Type		male
	  Anim          male\barf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Enc_Barf_B
	Requires	ENCOUNTER POISON BLOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg PickRandomly
	CycleMove	Enc_Barf_A
	CycleMove	Enc_Barf_B
	CycleMove	Enc_Barf_C

	Type		male
	  Anim          male\barf_cycle 1 50
	TEnd

	Type		huge
	  Anim          huge\barf_cycle 1 50
	TEnd
MEnd

Move Enc_Barf_C
	Requires	ENCOUNTER POISON BLOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg PickRandomly
	CycleMove	Enc_Barf_A
	CycleMove	Enc_Barf_B
	CycleMove	Enc_Barf_C

	Type		male
	  Anim          male\quickbarf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\quickbarf 1
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd


###################
### Enc Flip coin  #####
###################

Move Enc_Flipcoin_A
	Requires	ENCOUNTER ROLLDICE CAST LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Flipcoin_C
	CycleMove	Enc_Flipcoin_B

	Type		male
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Prop_Coin.fx 56
	TEnd
MEnd

Move Enc_Flipcoin_B
	Requires	ENCOUNTER ROLLDICE CAST LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Flipcoin_A

	Type		male
	  Anim          male\aw_crap 1 45
	TEnd
MEnd

Move Enc_Flipcoin_C
	Requires	ENCOUNTER ROLLDICE CAST LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Flipcoin_A

	Type		male
	  Anim          male\shucks 1
	TEnd
MEnd

#############################################################
## Flipping Coin
#############################################################

Move Encounter_FlippingCoin
	Requires	ENCOUNTER FLIPPINGCOIN
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Scale		0.85
	Flags		Cycle NOSIZESCALE

	Type		male
	  Anim          male\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd
MEnd

#########################
### Enc Floating Lotus  #####
########################

Move Enc_FloatLotus
	Requires	ENCOUNTER FLOATLOTUS
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          fem\float_lotus 1
	TEnd
MEnd


#########################
### Enc Plant Trap  #####
########################

Move Enc_PlantTrap_A
	Requires	ENCOUNTER PLANTTRAP
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Enc_PlantTrap_C
	CycleMove	Enc_PlantTrap_B
	CycleMove	Enc_PlantTrap_A
	NextMove	Enc_PlantTrap_Free
	Sets            CANTMOVE
	Interpolate	15
	Scale		0.5

	Type		male
	  Anim          male\wriggle 18
	  PlayFx	Powers\PlantControl\Enc_StranglerRoots.fx 0 constant
	TEnd
MEnd

Move Enc_PlantTrap_B
	Requires	ENCOUNTER PLANTTRAP
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Enc_PlantTrap_C
	CycleMove	Enc_PlantTrap_B
	CycleMove	Enc_PlantTrap_A
	NextMove	Enc_PlantTrap_Free
	Sets            CANTMOVE
	Interpolate	15
	Scale		0.3

	Type		male
	  Anim          male\wriggle 18
	  PlayFx	Powers\PlantControl\Enc_StranglerRoots.fx 0 constant
	TEnd
MEnd

Move Enc_PlantTrap_C
	Requires	ENCOUNTER PLANTTRAP
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	CycleMove	Enc_PlantTrap_B
	CycleMove	Enc_PlantTrap_A
	CycleMove	Enc_PlantTrap_C
	NextMove	Enc_PlantTrap_Free
	Sets            CANTMOVE
	Interpolate	15
	Scale		0.7

	Type		male
	  Anim          male\wriggle 18
	  PlayFx	Powers\PlantControl\Enc_StranglerRoots.fx 0 constant
	TEnd
MEnd

Move Enc_PlantTrap_Free
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready
	Interpolate     8
	Sets            CANTMOVE

	Type		male
	  Anim          male\pre_poison 42
	TEnd
MEnd


#####################################################
#####  Newsie (using vendor as a placeholder anim)  #####
#####################################################

Move Newsie_YellRight
	Requires	ENCOUNTER NEWSIE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Newsie_Point
	CycleMove       Newsie_Sweep

	Type		male
	  Anim          male\VendorNew_YellRight 1
	  PlayFX	Generic\newspapers_root.fx 0 Constant
	TEnd
MEnd

Move Newsie_Point
	Requires	ENCOUNTER NEWSIE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Newsie_YellRight
	CycleMove       Newsie_Sweep

	Type		male
	  Anim          male\VendorNew_Point 1
	  PlayFX	Generic\newspapers_root.fx 0 Constant
	TEnd
MEnd

Move Newsie_Sweep
	Requires	ENCOUNTER NEWSIE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Newsie_Point
	CycleMove       Newsie_YellRight

	Type		male
	  Anim          male\VendorNew_Sweep 1
	  PlayFX	Generic\newspapers_root.fx 0 Constant
	TEnd
MEnd



#####################################################
#####  Nocart Vendor (vendor sans cart prop)  #####
#####################################################

Move NocartVendor_YellRight
	Requires	ENCOUNTER NOCARTVENDOR
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       NocartVendor_Point
	CycleMove       NocartVendor_Sweep

	Type		male
	  Anim          male\VendorNew_YellRight 1
	TEnd
MEnd

Move NocartVendor_Point
	Requires	ENCOUNTER NOCARTVENDOR
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       NocartVendor_YellRight
	CycleMove       NocartVendor_Sweep

	Type		male
	  Anim          male\VendorNew_Point 1
	TEnd
MEnd

Move NocartVendor_Sweep
	Requires	ENCOUNTER NOCARTVENDOR
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       NocartVendor_Point
	CycleMove       NocartVendor_YellRight

	Type		male
	  Anim          male\VendorNew_Sweep 1
	TEnd
MEnd

##################################
###### Hero Spellcast Trapped  ######
##################################

Move Hero_SpellcastFear
	Requires	ENCOUNTER HEROSPELLCAST
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Hero_SCastfall
	Scale           0.50
	Interpolate     8

	Type		male
	  Anim          bis\cot_wriggle 19
	  PlayFx        WORLD\Lairs\cot\victim_Effect.fx 0 Constant
	TEnd
MEnd

Move Hero_SCastfall
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Hero_SCastLand
	Interpolate	8
	Flags		CANTMOVE

	Type           male
	  Anim          male\hero_spellcast_fall 43
	TEnd
MEnd

Move Hero_SCastLand
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready
	Scale		0.7
	Flags		CANTMOVE

	Type		male
	  Anim          male\raisedead_post 1
	TEnd

	Type		huge
	  Anim          huge\raisedead_post 1
	TEnd

	Type		fem
	  Anim          fem\raisedead_post 1
	TEnd
MEnd


####################
###### Amused  ######
###################

Move Amused_pre
	Requires	ENCOUNTER AMUSED
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Amused_E
	Interpolate	15

	Type		male
	  Anim          male\your_welcome_sm 1
	TEnd
MEnd

Move Amused_B
	Requires	ENCOUNTER AMUSED
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Amused_C
	CycleMove       Amused_B
	CycleMove       Amused_D
	Interpolate	15
	Scale		0.8

	Type		male
	  Anim          male\idle_xarms_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_xarms_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_xarms_look 1
	TEnd
MEnd

Move Amused_C
	Requires	ENCOUNTER AMUSED
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Amused_B
	Interpolate	15

	Type		male
	  Anim          male\listen_1 111 260
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	TEnd

	Type		huge
	  Anim          huge\listen_1 111 190
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 16
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 23
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 30
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 37
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 44
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 51
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 58
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 65
	  PlayFX	NPCFX\NPC_HandClap_SFX.fx 72
	TEnd
MEnd

Move Amused_D
	Requires	ENCOUNTER AMUSED
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Amused_E
	Interpolate	15

	Type		male
	  Anim          male\thankyou_sm 1 ##39
	TEnd
MEnd

Move Amused_E
	Requires	ENCOUNTER AMUSED
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Amused_B
	Interpolate	15

	Type		male
	  Anim          male\no 1 115
	TEnd
MEnd

############################
### Emote FarwellNfadeout   #####
############################

Move Emote_Farewell	##one time event only - player should not reappear or fight
	Requires	ENCOUNTER FAREWELLNFADEOUT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Emote_Farewell_Laugh
	Interpolate	8

	Type		male
	  Anim          male\fancy_bow 1
	TEnd
MEnd

Move Emote_Farewell_Laugh
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Emote_farewell_cycle

	Type		male
	  Anim          male\heroic_laugh 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 25 constant
	TEnd
MEnd

Move Emote_farewell_cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable NOCOLLISION

	Type		male
	  Anim          male\centerstance_cycle 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd


###################
### Enc Molotov toss  #####
###################

Move Enc_Molotov_A
	Requires	ENCOUNTER MOLOTOV
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Molotov_B
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	  PlayFX		NPCFX\NPC_TorchWhooshSoft_SFX.fx 1
	  PlayFX		NPCFX\NPC_TorchWhooshSoft_SFX.fx 25
	  PlayFX		NPCFX\NPC_TorchWhooshSoft_SFX.fx 60
	TEnd
MEnd

Move Enc_Molotov_B
	Requires	ENCOUNTER MOLOTOV
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Molotov_C

	Type		male
	  Anim          male\molotov_lob 12
	  PlayFX	Explosions\medium\Enc_Expolsion_med.fx 45
	  PlayFX	WORLD\Ruins\molotov_MediumFireFast.fx 53
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	  PlayFX	NPCFX\NPC_TorchWhooshMedium_SFX.fx 1
	  PlayFX	NPCFX\NPC_TorchWhooshLoud_SFX.fx 20
	 TEnd
MEnd

Move Enc_Molotov_C
	Requires	ENCOUNTER MOLOTOV
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Molotov_D
	Interpolate	15

	Type		male
	  Anim          male\listen_3 1 263    ##heroic_laugh 1   ##vandal_kick_low 1
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 1
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 7
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 14
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 21
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 28
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 35
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 42
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 49
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 56
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 63
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 70
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 77
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 84
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 91
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 98
	  PlayFX		NPCFX\NPC_Whistle_SFX.fx 122
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 168
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 175
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 182
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 189
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 196
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 203
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 210
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 217
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 224
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 231
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 245
	  PlayFX		NPCFX\NPC_HandClap_SFX.fx 259
	TEnd
MEnd

Move Enc_Molotov_D
	Requires	ENCOUNTER MOLOTOV
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Molotov_E

	Type		male
	  Anim          male\draw_axe 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	  PlayFX		NPCFX\NPC_TorchWhooshMedium_SFX.fx 1
	TEnd
MEnd

Move Enc_Molotov_E
	Requires	ENCOUNTER MOLOTOV
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	CycleMove	Enc_Molotov_E
	CycleMove	Enc_Molotov_A
	CycleMove	Enc_Molotov_E
	Scale           1.50

	Type		male
	  Anim          male\knifetoss 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	  PlayFX		NPCFX\NPC_TorchWhooshMedium_SFX.fx 1
	  PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 20
	  PlayFX		NPCFX\NPC_TorchWhooshMedium_SFX.fx 27
	  PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 48
	TEnd
MEnd


##############################
### Dead guards ###
##############################

Move dead_guard_1
	Requires	ENCOUNTER POSE COWER SAD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	0

	Type		male
	  Anim          male\deadposes 1 2
	TEnd

	Type		fem
	  Anim          fem\deadposes 1 5
	TEnd
MEnd

Move dead_guard_2
	Requires	ENCOUNTER POSE COWER SAD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	0

	Type		male
	  Anim          male\deadposes 3 4
	TEnd

	Type		fem
	  Anim          fem\deadposes 1 5
	TEnd

MEnd

Move dead_guard_3
	Requires	ENCOUNTER POSE COWER SAD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	0

	Type		male
	  Anim          male\deadposes 5 6
	TEnd

	Type		fem
	  Anim          fem\deadposes 1 5
	TEnd

MEnd

Move dead_guard_4
	Requires	ENCOUNTER POSE COWER SAD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	0

	Type		male
	  Anim          male\deadposes 7 8
	TEnd

	Type		fem
	  Anim          fem\deadposes 1 5
	TEnd
MEnd

Move dead_guard_5
	Requires	ENCOUNTER POSE COWER SAD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	interpolate	0

	Type		male
	  Anim          male\deadposes 9 10
	TEnd

	Type		fem
	  Anim          fem\deadposes 1 5
	TEnd
MEnd


###################
### Toss BodyBag  #####
###################

Move Toss_BodyBag
	Requires	ENCOUNTER BODYBAGTOSS
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FixArmReg
	NextMove	Toss_BodyBag_post

	Type		male
	  Anim          male\BodyBag_Toss 1
	  PlayFx	GENERIC\FX_bodyBagDispose.fx 0
	  PlayFx	GENERIC\FX_bodyBagLinger.fx 80
	  PlayFx    	NPCFX\NPC_Whoosh_SFX.fx 65
	  PlayFX		NPCFX\NPC_Bodybag_SFX.fx 80
	  PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 133
	  PlayFX		NPCFX\NPC_HandPatSmall_SFX.fx 148
	TEnd
MEnd

Move Toss_BodyBag_post
	Requires	ENCOUNTER BODYBAGTOSS
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Interpolate	15

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

########  Door man moves  ##############################

Move Open
	Requires	ENCOUNTER OPEN
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Open2

	Type		male
	  Anim          male\VendorNew_Sweep 15 60
	TEnd
MEnd

Move Open2
	Requires	ENCOUNTER OPEN
	Priority	2
	NextMove        Ready_2Hips  ##Opened
	Sets            WALKTHROUGH

	Type		male
	  Anim          male\VendorNew_Sweep 60 83
	TEnd
MEnd


###########################
### Bane Spider Teleport Spawn #####
###########################

Move BaneSpiderTP
	Requires	ENCOUNTER BANESPIDERTELEPORTSPAWN
 	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready
 	Interpolate     0
 	Sets            CANTMOVE

	Type		male
	  Anim		male\Skel_ready2 1
	  PlayFx	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_DeployedSoldier.fx
	TEnd
MEnd


#########################
#### Cocoon Sleep  #####
########################

Move Cocoon_Sleep
	Requires	ENCOUNTER COCOONSLEEP
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE
	NextMove	Cocoon_Awake
	Interpolate	0

	Type		male
	  Anim          male\cocoon_sleep 1 5
	TEnd
MEnd

Move Cocoon_Awake
	NextMove        Cocoon_getup
	Interpolate     0
	Flags		NotSelectable NoSizeScale CANTMOVE ##NOCOLLISION

	Type		male
	  Anim          male\cocoon_sleep 6
	TEnd
MEnd

Move Cocoon_getup
	NextMove        Cocoon_cycle
	Interpolate     20
	Flags		NotSelectable NoSizeScale CANTMOVE ##NOCOLLISION

	Type		male
	  Anim          male\getup_belly 1 89
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1  89
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1 89
	TEnd
MEnd

Move Cocoon_cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Interpolate	15

	Type		male
	  Anim          male\skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd

## DRUM LOW ###########################################################

Move Drum_low
	Requires	ENCOUNTER CHANTING LOW WOOD
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\drum_low 1
	  PlayFx        GENERIC\drum_small.fx  0 Constant
	  PlayFx        FEET\Hidrum.fx  1
	  PlayFx        FEET\Hidrum.fx  4
	  PlayFx        FEET\Hidrum.fx  8
	  PlayFx        FEET\Hidrum.fx  12
	  PlayFx        FEET\Hidrum.fx  16
	  PlayFx        FEET\Hidrum.fx  20
	  PlayFx        FEET\Hidrum.fx  24
	  PlayFx        FEET\Hidrum.fx  28
	  PlayFx        FEET\Hidrum.fx  32
	  PlayFx        FEET\Hidrum.fx  36
	  PlayFx        FEET\Hidrum.fx  40
	  PlayFx        FEET\Hidrum.fx  44
	  PlayFx        FEET\Hidrum.fx  48
	  PlayFx        FEET\Hidrum.fx  52
	  PlayFx        FEET\Hidrum.fx  56
	TEnd
MEnd

## DRUM HIGH ###########################################################

Move Drum_high
	Requires	ENCOUNTER CHANTING HIGH WOOD
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	4

	Type		male
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\drum_big.fx  0 Constant
	  PlayFx        GENERIC\drum_sticks_bones.fx  0 Constant
	  PlayFx        FEET\Lowdrum.fx  1
	  PlayFx        FEET\Lowdrum.fx  30
	TEnd
MEnd

###################
### Tribal Dance   #####
###################

Move TribalDance_A
	Requires	ENCOUNTER DANCE STRONG CHANTING
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale
	CycleMove	TribalDance_A
	CycleMove	TribalDance_B
	CycleMove	TribalDance_C

	Type		male
	  Anim          male\tribal_dance 1
	TEnd

	Type		Beast
	  Anim          Beast\tribaldance 1
	TEnd
MEnd

Move TribalDance_B
	Requires	ENCOUNTER DANCE STRONG CHANTING
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale
	CycleMove	TribalDance_C
	CycleMove	TribalDance_A
	CycleMove	TribalDance_B
	Scale		1.2

	Type		male
	  Anim          male\tribal_dance_alt 1
	TEnd

	Type		Beast
	  Anim          Beast\tribaldance_alt 1
	TEnd
MEnd

Move TribalDance_C
	Requires	ENCOUNTER DANCE STRONG CHANTING
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale
	CycleMove	TribalDance_C
	CycleMove	TribalDance_A
	CycleMove	TribalDance_B

	Type		male
	  Anim          male\tribal_dance_alt 1
	TEnd

	Type		Beast
	  Anim          Beast\tribaldance_alt 1
	TEnd
MEnd

#############################################################
## BIG WAVE - "Over Here"
#############################################################

Move Encounter_BigWave
	Requires	ENCOUNTER BIGWAVE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Encounter_BigWave_Yell

	Type		male
	  Anim          male\over_here 1 88
	TEnd
MEnd

Move Encounter_BigWave_Yell
	Requires	ENCOUNTER BIGWAVE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Encounter_BigWave
	Scale		0.5

	Type		male
	  Anim          male\yell 65 92
	TEnd
MEnd

## WOUNDED ###########################################################

Move Wounded
	Requires	ENCOUNTER WOUNDED
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\dizzy_cycle 1 31
	TEnd
MEnd

#############################################################
## DANCES - Random
#############################################################

Move Encounter_Dance1
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################

Move Encounter_Dance2
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

#############################################################

Move Encounter_Dance3
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

#############################################################

Move Encounter_Dance4
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

#############################################################

Move Encounter_Dance5
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

#############################################################

Move Encounter_Dance6
	Requires	ENCOUNTER DANCE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

#############################################################
## Robot Dance
#############################################################

Move Encounter_DanceRobot_Pre
	Requires	ENCOUNTER DANCE ROBOT
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Encounter_DanceRobot_Cycle

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd

Move Encounter_DanceRobot_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd


## DEALING ###########################################################

Move Deal_A
	Requires	ENCOUNTER DEALING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\deal_A 1 238
	TEnd
MEnd


## RECEIVING  ###########################################################

Move Receiving_A
	Requires	ENCOUNTER RECEIVING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\deal_B 1 238
	TEnd
MEnd

## HOWL ###########################################################

Move Encounter_Howl
	Requires	ENCOUNTER HOWL
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Encounter_Howl_beatchest
	CycleMove       Encounter_Howl_growl
	Scale		0.5

	Type		male
	  Anim		Beast\howl 1
	  PlayFX	EnemyFx\5thcolumn\NightWolf\nightwolfhowl_B.fx 16
	TEnd
MEnd

Move Encounter_Howl_growl
	Requires	ENCOUNTER HOWL #ENCOUNTER YELL STRONG
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Encounter_Howl

	Type		male
	  Anim          Beast\growl 1
	  PlayFX	EnemyFx\5thcolumn\NightWolf\nightwolfgrowl1.fx
	TEnd
MEnd

Move Encounter_Howl_beatchest
	Requires	ENCOUNTER HOWL #ENCOUNTER YELL STRONG
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Encounter_Howl
	Scale		0.7

	Type		male
	  Anim          Beast\beatchest 1
	  PlayFX	EnemyFx\5thcolumn\NightWolf\nightwolfhowl.fx
	TEnd
MEnd

#########################
### Newspaper Encounter #####
#########################

Move Enc_newspaper
	Requires        ENCOUNTER NEWSPAPER GOOD
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale FixArmReg
	Interpolate	5

	Type            male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper.FX 0 Constant
	  PlayFX		NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd

	Type            fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 Constant
	  PlayFX		NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd

	Type            huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 Constant
	  PlayFX		NPCFX\NPC_Newspaper_Rustle_SFX.fx
	TEnd
MEnd

#########################
### Rogue Newspaper Encounter #####
#########################

Move Rogue_newspaper
	Requires	ENCOUNTER NEWSPAPER EVIL
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale FixArmReg
	Interpolate	5

	Type            male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
	TEnd

	Type            fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
	TEnd

	Type            huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
	TEnd
MEnd


#############################################################
## TRAINING - JUMPING JACKS
#############################################################

Move Encounter_JumpingJacks
	Requires	ENCOUNTER TRAIN LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags           Cycle

	Type            male
	  Anim          male\jumpingjax 1
	TEnd
MEnd

#############################################################
## TRAINING - KUNG FU
#############################################################

Move Encounter_TrainKungFu
	Requires        ENCOUNTER TRAIN KUNGFU
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags           Cycle

	Type            male
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET/SUPERJUMP1.FX 17
	  PlayFx        FEET/SUPERJUMP1.FX 74
	  PlayFx        FEET/SUPERJUMP1.FX 127
	  PlayFx        FEET/SUPERJUMP1.FX 194
	TEnd
MEnd

#############################################################
## WAITING
#############################################################

Move Encounter_Waiting_Cycle
	Requires	ENCOUNTER WAITING
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Encounter_Waiting_Cycle
	CycleMove	Encounter_Waiting_Cycle
	CycleMove	Encounter_Waiting_Cycle
	CycleMove	Encounter_Waiting_Break1

	Type		male
	  Anim          male\emote_waiting_cycle 1
	TEnd

	Type		fem
	  Anim          fem\emote_waiting_cycle 1
	TEnd

	Type		huge
	  Anim          huge\emote_waiting_cycle 1
	TEnd
MEnd

Move  Encounter_Waiting_Break1
	Requires	ENCOUNTER WAITING
	Priority	2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Encounter_Waiting_Cycle

	Type		male
	  Anim          male\emote_waiting_break1 1
	TEnd

	Type		fem
	  Anim          fem\emote_waiting_break1 1
	TEnd

	Type		huge
	  Anim          huge\emote_waiting_break1 1
	TEnd
MEnd

#############################################################
## Teabag Emote
#############################################################

Move Encounter_Teabag
	Requires	ENCOUNTER TEABAG
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle FIXARMREG

	Type		male
	  Anim          male\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Sitting Poses - For Vanguard Projection Room
#############################################################

Move Encounter_Vanguard_SitPose1
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE
	Scale		0.95

	Type		male
	  Anim          male\vanguard_sitpose1 1 121
	TEnd
MEnd

#############################################################

Move Encounter_Vanguard_SitPose1A
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE
	Scale		0.9

	Type		male
	  Anim          male\vanguard_sitpose1 1 121
	TEnd
MEnd

#############################################################

Move Encounter_Vanguard_SitPose2
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE

	Type		male
	  Anim          male\vanguard_sitpose2 1 121
	TEnd
MEnd

#############################################################

Move Encounter_Vanguard_SitPose2B
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE
	Scale		0.85

	Type		male
	  Anim          male\vanguard_sitpose2 1 121
	TEnd
MEnd

#############################################################

Move Encounter_Vanguard_SitPose3
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE
	Scale		0.75

	Type		male
	  Anim          male\vanguard_sitpose3 1 121
	TEnd
MEnd

#############################################################

Move Encounter_Vanguard_SitPose3C
	Requires	ENCOUNTER OBSERVE SIT POSE ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG PICKRANDOMLY CYCLE
	Scale		0.7

	Type		male
	  Anim          male\vanguard_sitpose3 1 121
	TEnd
MEnd

#############################################################
## SCORECARDS
#############################################################

Move Encounter_Judge1
	Requires	ENCOUNTER SCORE ONE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge1_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge1_Cycle
	Requires	ENCOUNTER SCORE ONE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge2
	Requires	ENCOUNTER SCORE TWO
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge2_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge2_Cycle
	Requires	ENCOUNTER SCORE TWO
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge3
	Requires	ENCOUNTER SCORE THREE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge3_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge3_Cycle
	Requires	ENCOUNTER SCORE THREE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge4
	Requires	ENCOUNTER SCORE FOUR
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge4_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge4_Cycle
	Requires	ENCOUNTER SCORE FOUR
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_4.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge5
	Requires	ENCOUNTER SCORE FIVE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge5_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge5_Cycle
	Requires	ENCOUNTER SCORE FIVE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge6
	Requires	ENCOUNTER SCORE SIX
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge6_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge6_Cycle
	Requires	ENCOUNTER SCORE SIX
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge7
	Requires	ENCOUNTER SCORE SEVEN
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge7_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge7_Cycle
	Requires	ENCOUNTER SCORE SEVEN
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge8
	Requires	ENCOUNTER SCORE EIGHT
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge8_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge8_Cycle
	Requires	ENCOUNTER SCORE EIGHT
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge9
	Requires	ENCOUNTER SCORE NINE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge9_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge9_Cycle
	Requires	ENCOUNTER SCORE NINE
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Encounter_Judge10
	Requires	ENCOUNTER SCORE TEN
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE
	CycleMove	Encounter_Judge10_Cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Judge10_Cycle
	Requires	ENCOUNTER SCORE TEN
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
#######		Soccer Encounter 	######################
#############################################################

Move Encounter_Soccer_Pre
	Requires	ENCOUNTER KICK SOCCER
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Cycle

	Type		male
	  Anim          male\Soccer_idle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Soccer_idle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Cycle
	Requires 	ENCOUNTER KICK SOCCER
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Cycle
	CycleMove	Encounter_Soccer_foot_juggle_pre

	Type		male
	  Anim          male\Soccer_idle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Soccer_idle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_foot_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd
MEnd

Move Encounter_Soccer_foot_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_foot_juggle
	CycleMove	Encounter_Soccer_foot_juggle
	CycleMove	Encounter_Soccer_knee_juggle_pre
	CycleMove	Encounter_Soccer_foot_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Encounter_Soccer_foot_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Cycle

	Type		male
	  Anim          male\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd
MEnd

Move Encounter_Soccer_knee_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_knee_juggle

	Type		male
	  Anim          male\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Encounter_Soccer_knee_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_knee_juggle
	CycleMove	Encounter_Soccer_knee_juggle
	CycleMove	Encounter_Soccer_head_juggle_pre
	CycleMove	Encounter_Soccer_knee_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd
MEnd

Move Encounter_Soccer_knee_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd
MEnd

Move Encounter_Soccer_head_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_head_juggle

	Type		male
	  Anim          male\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd
MEnd

Move Encounter_Soccer_head_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_head_juggle
	CycleMove	Encounter_Soccer_head_juggle
	CycleMove	Encounter_Soccer_head_juggle
	CycleMove	Encounter_Soccer_head_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd

Move Encounter_Soccer_head_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd


#############################################################

Move Encounter_Soccer_Talk_Pre
	Requires	ENCOUNTER TALK KICK SOCCER
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_Cycle

	Type		male
	  Anim          male\soccer_talk_cycle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\soccer_talk_cycle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Talk_Cycle
	Requires 	ENCOUNTER TALK KICK SOCCER
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_CycleBreak_A
	CycleMove	Encounter_Soccer_Talk_CycleBreak_B
	CycleMove	Encounter_Soccer_Talk_CycleBreak_C
	CycleMove	Encounter_Soccer_Talk_foot_juggle_pre

	Type		male
	  Anim          male\soccer_talk_cycle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\soccer_talk_cycle 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Talk_CycleBreak_A
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_Cycle
	CycleMove	Encounter_Soccer_Talk_CycleBreak_B
	CycleMove	Encounter_Soccer_Talk_CycleBreak_C
	CycleMove	Encounter_Soccer_Talk_foot_juggle_pre

	Type		male
	  Anim          male\soccer_idle_break_A 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\soccer_idle_break_A 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Talk_CycleBreak_B
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_Cycle
	CycleMove	Encounter_Soccer_Talk_CycleBreak_A
	CycleMove	Encounter_Soccer_Talk_CycleBreak_C
	CycleMove	Encounter_Soccer_Talk_foot_juggle_pre

	Type		male
	  Anim          male\soccer_idle_break_B 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\soccer_idle_break_B 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Talk_CycleBreak_C
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_Cycle
	CycleMove	Encounter_Soccer_Talk_CycleBreak_A
	CycleMove	Encounter_Soccer_Talk_CycleBreak_B
	CycleMove	Encounter_Soccer_Talk_foot_juggle_pre

	Type		male
	  Anim          male\soccer_idle_break_C 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\soccer_idle_break_C 1
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Soccer_Talk_foot_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 1 20
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_FootFlip.fx
	  PlayFX		NPCFX\SoccerBall_Knee.fx 5
	TEnd
MEnd

Move Encounter_Soccer_Talk_foot_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_foot_juggle
	CycleMove	Encounter_Soccer_Talk_foot_juggle
	CycleMove	Encounter_Soccer_Talk_knee_juggle_pre
	CycleMove	Encounter_Soccer_Talk_foot_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 21 47
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Encounter_Soccer_Talk_foot_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_Cycle

	Type		male
	  Anim          male\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 170 194
	  PlayFx        GENERIC\SoccerBall.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Catch.fx 12
	TEnd
MEnd

Move Encounter_Soccer_Talk_knee_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_knee_juggle

	Type		male
	  Anim          male\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 48 79
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Kick.fx 1
	  PlayFX		NPCFX\SoccerBall_Kick.fx 13
	TEnd
MEnd

Move Encounter_Soccer_Talk_knee_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_knee_juggle
	CycleMove	Encounter_Soccer_Talk_knee_juggle
	CycleMove	Encounter_Soccer_Talk_head_juggle_pre
	CycleMove	Encounter_Soccer_Talk_knee_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 80 111
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 16
	TEnd
MEnd

Move Encounter_Soccer_Talk_knee_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 200 215
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 15
	TEnd
MEnd

Move Encounter_Soccer_Talk_head_juggle_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_head_juggle

	Type		male
	  Anim          male\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 112 147
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Knee.fx 1
	  PlayFX		NPCFX\SoccerBall_Knee.fx 18
	TEnd
MEnd

Move Encounter_Soccer_Talk_head_juggle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_head_juggle
	CycleMove	Encounter_Soccer_Talk_head_juggle
	CycleMove	Encounter_Soccer_Talk_head_juggle
	CycleMove	Encounter_Soccer_Talk_head_juggle_post

	Type		male
	  Anim          male\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 148 163
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd

Move Encounter_Soccer_Talk_head_juggle_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Soccer_Talk_foot_juggle

	Type		male
	  Anim          male\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd

	Type		fem
	  Anim          fem\Soccer_juggle 220 243
	  PlayFx        GENERIC\SoccerBall_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\SoccerBall_Head.fx 1
	TEnd
MEnd


#############################################################
#######Basketball Encounter######################
#############################################################


Move Encounter_Basketball_Talk_Pre
	Requires	ENCOUNTER TALK HAND BASKETBALL
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_Cycle

	Type		male
	  Anim          male\Basketball_talk_cycle 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_talk_cycle 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_Talk_Cycle
	Requires 	ENCOUNTER TALK HAND BASKETBALL
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_CycleBreak_A
	CycleMove	Encounter_Basketball_Talk_CycleBreak_B
	CycleMove	Encounter_Basketball_dribble_pre
	CycleMove	Encounter_Basketball_around_waist_pre

	Type		male
	  Anim          male\Basketball_talk_cycle 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_talk_cycle 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_Talk_CycleBreak_A
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_Cycle
	CycleMove	Encounter_Basketball_Talk_CycleBreak_B
	CycleMove	Encounter_Basketball_around_waist_pre

	Type		male
	  Anim          male\basketball_idle_break_A 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\basketball_idle_break_A 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_Talk_CycleBreak_B
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_Cycle
	CycleMove	Encounter_Basketball_Talk_CycleBreak_A
	CycleMove	Encounter_Basketball_around_waist_pre

	Type		male
	  Anim          male\basketball_idle_break_B 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\basketball_idle_break_B 1
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_around_waist_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_around_waist

	Type		male
	  Anim          male\Basketball_around_waist 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_around_waist
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_around_waist
	CycleMove	Encounter_Basketball_around_waist
	CycleMove	Encounter_Basketball_around_waist
	CycleMove	Encounter_Basketball_around_waist_post

	Type		male
	  Anim          male\Basketball_around_waist 12 43
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_HandHit.fx 16
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 12 43
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_HandHit.fx 16
	TEnd
MEnd

Move Encounter_Basketball_around_waist_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_Cycle

	Type		male
	  Anim          male\Basketball_around_waist 44
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Basketball_around_waist 44
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_Basketball_dribble_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_dribble

	Type		male
	  Anim          male\Basketball_dribble_cycle 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 1 11
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	TEnd
MEnd

Move Encounter_Basketball_dribble
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_dribble
	CycleMove	Encounter_Basketball_dribble
	CycleMove	Encounter_Basketball_crossover_right_pre
	CycleMove	Encounter_Basketball_dribble_post

	Type		male
	  Anim          male\Basketball_dribble_cycle 12 27
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 12 27
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd
MEnd

Move Encounter_Basketball_dribble_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_Talk_Cycle

	Type		male
	  Anim          male\Basketball_dribble_cycle 168 197
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 168 197
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 8
	TEnd
MEnd

Move Encounter_Basketball_crossover_right_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_crossover_right

	Type		male
	  Anim          male\Basketball_dribble_cycle 28 60
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 28 60
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Encounter_Basketball_crossover_right
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_crossover_right
	CycleMove	Encounter_Basketball_crossover_right
	CycleMove	Encounter_Basketball_crossover_right_post
	CycleMove	Encounter_Basketball_crossover_left_pre

	Type		male
	  Anim          male\Basketball_dribble_cycle 61 99
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 61 99
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Encounter_Basketball_crossover_right_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_dribble

	Type		male
	  Anim          male\Basketball_dribble_cycle 200 236
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 200 236
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Encounter_Basketball_crossover_left_pre
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_crossover_left

	Type		male
	  Anim          male\Basketball_dribble_cycle 100 114
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 100 114
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd
MEnd

Move Encounter_Basketball_crossover_left
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_crossover_left
	CycleMove	Encounter_Basketball_crossover_left
	CycleMove	Encounter_Basketball_crossover_left
	CycleMove	Encounter_Basketball_crossover_left_post

	Type		male
	  Anim          male\Basketball_dribble_cycle 115 155
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 115 155
	  PlayFx        GENERIC\Basketball_Spinning.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	  PlayFX		NPCFX\Basketball_HandHit.fx 18
	  PlayFX		NPCFX\Basketball_Bounce.fx 27
	TEnd
MEnd

Move Encounter_Basketball_crossover_left_post
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE nosizescale
	Interpolate	0
	CycleMove	Encounter_Basketball_crossover_right

	Type		male
	  Anim          male\Basketball_dribble_cycle 239 254
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd

	Type		fem
	  Anim          fem\Basketball_dribble_cycle 239 254
	  PlayFx        GENERIC\Basketball.fx 0 CONSTANT
	  PlayFX		NPCFX\Basketball_HandHit.fx 1
	  PlayFX		NPCFX\Basketball_Bounce.fx 9
	TEnd
MEnd


#########Pool Cue NPC ready only##############################################################################################################

Move Encounter_PoolCue_Ready_Cycle
	Requires	ENCOUNTER STAND POOL
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	0
	CycleMove	Encounter_PoolCue_Ready_Cycle
	CycleMove	Encounter_PoolCue_Ready_Cycle
	CycleMove	Encounter_PoolCue_Ready_Cycle
	NextMove	Encounter_PoolCue_Ready_Cycle

	Type		male
	  Anim          male\poolcue_ready 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_ready 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd
##############Talking and chalking and ready stance ############################

Move Encounter_PoolCue_Talking_Cycle
	Requires	ENCOUNTER STAND POOL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Encounter_PoolCue_Talking_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakB_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakA_Cycle
	CycleMove	Encounter_PoolCue_Chalking_Cycle

	Type		male
	  Anim          male\poolcue_talk 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_talk 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_PoolCue_TalkBreakA_Cycle
	Requires	ENCOUNTER STAND POOL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Encounter_PoolCue_TalkBreakA_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakB_Cycle
	CycleMove	Encounter_PoolCue_Talking_Cycle

	Type		male
	  Anim          male\poolcue_talk_breakA 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_talk_breakA 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_PoolCue_TalkBreakB_Cycle
	Requires	ENCOUNTER STAND POOL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Encounter_PoolCue_TalkBreakA_Cycle
	CycleMove	Encounter_PoolCue_Talking_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakB_Cycle
	CycleMove	Encounter_PoolCue_Talking_Cycle

	Type		male
	  Anim          male\poolcue_talk_breakB 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_talk_breakB 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_PoolCue_Chalking_Cycle
	Requires	ENCOUNTER STAND POOL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Encounter_PoolCue_Talking_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakA_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakB_Cycle
	CycleMove	Encounter_PoolCue_Swinging_Around

	Type		male
	  Anim          male\poolcue_chalking 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_chalking 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Encounter_PoolCue_Swinging_Around
	Requires	ENCOUNTER STAND POOL HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE NoSizeScale
	Interpolate	8
	CycleMove	Encounter_PoolCue_Talking_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakA_Cycle
	CycleMove	Encounter_PoolCue_TalkBreakB_Cycle
	CycleMove	Encounter_PoolCue_Talking_Cycle


	Type		male
	  Anim          male\poolcue_goofingaround 1
	  PlayFx        GENERIC\PoolCue.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\poolcue_goofingaround 1
	  PlayFx        GENERIC\PoolCue_Fem.fx 0 CONSTANT
	TEnd
MEnd

## Jump Down Ambush ###########################################################

Move Jump_Down_Ambush
	Requires	ENCOUNTER ALTERNATE LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Sets            CANTMOVE NOCOLLISION
	Flags		NotSelectable
	NextMove        Jump_Down_Ambush_Drop

	Type		male
	  Anim          male\ghouls_ambush 1 10
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd

	Type		Marauder_Lambda
	  Anim          huge\marauder_leap_down 1 15
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd

MEnd

Move Jump_Down_Ambush_Drop
	Include sequencers\Interrupts\Encounters_Idle.inc
	Flags		NotSelectable
	Sets            CANTMOVE
	NextMove        Jump_Down_Ambush_Post

	Type		male
	  Anim          male\ghouls_ambush 10 60
	TEnd

	Type		Marauder_Lambda
	  Anim          huge\marauder_leap_down 15 60
	TEnd

MEnd

Move Jump_Down_Ambush_Post
	Include sequencers\Interrupts\Encounters_Idle.inc
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\ghouls_ambush 60 69
	TEnd

	Type		Marauder_Lambda
	  Anim          huge\marauder_leap_down 60 86
	TEnd

MEnd
#####Jump Up Escape######################################################

Move Jump_Up_Escape_Pre
	Requires	ENCOUNTER RUN ALTERNATE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Jump_Up_Escape_Post
	Flags		Nosizescale

	Type		male
	  Anim          male\ghouls_ambush_jump_up 1 22
	TEnd

MEnd

Move Jump_Up_Escape_Post
	Requires	ENCOUNTER RUN ALTERNATE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NotSelectable
	Sets            CANTMOVE
	Flags		Cycle

	Type		male
	  Anim          male\ghouls_ambush_jump_up 22 40
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd

MEnd

## Clockwork Ambush ###########################################################

Move Clockwork_Ambush
	Requires	SPAWN PPD ALTERNATE
	Priority	3
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	Sets            CANTMOVE NOCOLLISION
	Flags		Cycle
	NextMove        Clockwork_Ambush_Drop

	Type		male
	  Anim          male\clockwork_ambush 1 5
	TEnd

	Type		fem
	  Anim          fem\clockwork_ambush 1 5
	TEnd

	Type		huge
	  Anim          huge\clockwork_ambush 1 5
	TEnd

MEnd

Move Clockwork_Ambush_Drop
	Member		<Ready> <ReactIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ>
	Flags		NotSelectable
	Sets            CANTMOVE
	NextMove        Clockwork_Ambush_Post

	Type		male
	  Anim          male\clockwork_ambush 5 54
	TEnd

	Type		fem
	  Anim          fem\clockwork_ambush 5 54
	TEnd

	Type		huge
	  Anim          huge\clockwork_ambush 5 54
	TEnd

MEnd

Move Clockwork_Ambush_Post
	Member		<Ready> <ReactIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ>
	NextMove        CombatReady

	Type		male
	  Anim          male\clockwork_ambush 54 60
	TEnd

	Type		fem
	  Anim          fem\clockwork_ambush 54 60
	TEnd

	Type		huge
	  Anim          huge\clockwork_ambush 54 60
	TEnd

MEnd

#############################################################
## Yata Yata Encounter ( Cutscene Only
#############################################################

Move Encounter_YataYata
	Requires	ENCOUNTER KISSMYBUTT ALTERNATE
	Priority	3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Ready
	Scale		1.1

	Type		male
	  Anim          male\YataYata 1
	TEnd

	Type		huge
	  Anim          male\YataYata 1
	TEnd

	Type		fem
	  Anim          male\YataYata 1
	TEnd
MEnd

#############################################################
## Encounter Frustrated / Drat
#############################################################

Move Encounter_Drat
	Requires	ENCOUNTER ANGRY RUDE ALTERNATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Encounter_Drat2
	Interpolate	8

	Type		male
	  Anim          male\aw_crap 1 2
	TEnd

	Type		fem
	  Anim          male\aw_crap 1 2
	TEnd

	Type		huge
	  Anim          male\aw_crap 1 2
	TEnd
MEnd

Move Encounter_Drat2
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Encounter_Drat_cycle


	Type		male
	  Anim          male\aw_crap 3
	TEnd

	Type		fem
	  Anim          male\aw_crap 3
	TEnd

	Type		huge
	  Anim          male\aw_crap 3
	TEnd
MEnd

Move Encounter_Drat_cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		CYCLE

	Type		male
	  Anim          male\frustrated_cycle 1
	TEnd

	Type		fem
	  Anim          male\frustrated_cycle 1
	TEnd

	Type		huge
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd
#############################################################
## Spectrum_Collapse
#############################################################

Move SpectrumCollapse_In
	Requires	ENCOUNTER COLLAPSE LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	SpectrumCollapse_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd
MEnd

Move SpectrumCollapse_Cycle
	Requires	ENCOUNTER COLLAPSE LOW
	Priority        3
	Include 	sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	  PlayFX	Scripted\BlueSpectrum_Marker\BlueSpectrumMarker.fx 1 CONSTANT

	TEnd
MEnd

## PPD Gauntlet Cover  ###########################################################

Move Spectrum_Cover
	Requires	ENCOUNTER ENDGAME CAPTURED ALTERNATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover_Look_Left
	CycleMove 	Spectrum_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 1 60
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 1 60
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd
MEnd

Move Spectrum_Cover_Look_Right
	Requires	ENCOUNTER ENDGAME CAPTURED ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover_Look_Left
	CycleMove 	Spectrum_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 60 120
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 60 120
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd
MEnd

Move Spectrum_Cover_Look_Left
	Requires	ENCOUNTER ENDGAME CAPTURED ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover
	CycleMove 	Spectrum_Cover_Look_Left
	CycleMove 	Spectrum_Cover_Look_Right

	Type		male
	  Anim          male\ppd_gauntlet_cover 120 180
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ppd_gauntlet_cover 120 180
	  PlayFX	Scripted\Tutorial\Spectrum_Cover.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
##  Apparitions Captured
#############################################################

Move Captured_Apparitions_Cycle
	Requires	ENCOUNTER LEVITATE CAPTURED ALTERNATE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_NPC_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        ENEMYFX\Praetorian\Apparitions\CrushingHate_Conditional.fx 0 Constant
	TEnd
MEnd


#############################################################
## Carnival of Light Captured
#############################################################

Move Captured_CarnivalofLight_Cycle
	Requires	ENCOUNTER CAPTURED LEVITATE HIGH
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_NPC_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\Praetorian\Captured_CarnivalofLight\Captured_CarnivalOfLight.fx 0 Constant
 	  PlayFx        Scripted\Praetorian\Captured_CarnivalofLight\Captured_CarnivalOfLight_Beams.fx 0 Constant

	TEnd
MEnd

#############################################################
## TalonsofVengeance Captured
#############################################################

Move Captured_TalonsofVengeance_Cycle
	Requires	ENCOUNTER LEVITATE ALTERNATE ENDGAME
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_NPC_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\Praetorian\Captured_TalonsofVengeance\Captured_TalonsofVengeance.fx 0 Constant
	  PlayFX 	Scripted\Praetorian\Captured_TalonsofVengeance\Captured_TalonsofVengeance_Sparks.fx 25
	TEnd
MEnd

#############################################################
Move Captured_NPC_Fall
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        SC_postcycle
	Priority        1
	#Sets		CANTMOVE

	Type		male
	  Anim          BF\cot_postfall 2
	TEnd
MEnd

Move Captured_NPC_Fall_PostCycle
	Include sequencers\Members\Encounters_Scripted.inc
	#Sets		CANTMOVE
	Priority        1
	NextMove	Ready

	Type		male
	  Anim          BF\cot_postfallcycle 1
	TEnd
MEnd


########################
## Awakened Indoctrinate #####
########################

Move Indoctrinate_A
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST ALTERNATE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Indoctrinate_B
	CycleMove	Indoctrinate_C
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\heal 1
	  PlayFx        Scripted\Praetorian\Indoctrinate\IndoctrinateAwakened.fx 0 constant
	TEnd
MEnd

Move Indoctrinate_B
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST ALTERNATE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Indoctrinate_C
	CycleMove	Indoctrinate_A
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\hypnotize 1
	  PlayFx        Scripted\Praetorian\Indoctrinate\IndoctrinateAwakened.fx 0 constant
	TEnd
MEnd

Move Indoctrinate_C
	Requires	ENCOUNTER SHIELD POSE MAGIC CAST ALTERNATE
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Indoctrinate_B
	CycleMove	Indoctrinate_A
	NextMove        Ready
	Interpolate	8

	Type		male
	  Anim          male\levitate 1
	  PlayFx        Scripted\Praetorian\Indoctrinate\IndoctrinateAwakened.fx 0 constant
	TEnd
MEnd

#### CarnivalofLight Teleport In  #########################################################

Move CarnivalofLight_TeleportIn
	Requires	ENCOUNTER TELEPORT ALTERNATE HIGH
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        CarnivalofLight_TeleportIn_LookQuick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFx        Scripted\Praetorian\CarnivalOfLight_TeleportIn\CarnivalOfLight_TeleportIn.fx 0 constant
	  PlayFX	GENERIC\EntityFadeIn_Immediate.fx 30
	TEnd
MEnd

Move CarnivalofLight_TeleportIn_LookQuick
	Requires	ENCOUNTER TELEPORT ALTERNATE HIGH
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	CarnivalofLight_TeleportIn_PostReady

	Type		male
	  Anim          male\lookquick 1
	TEnd

MEnd

Move CarnivalofLight_TeleportIn_PostReady
	Requires	ENCOUNTER TELEPORT ALTERNATE HIGH
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd

#### CarnivalofLight Teleport Out  #########################################################

Move CarnivalofLight_TeleportOut
	Requires	ENCOUNTER TELEPORT ALTERNATE ENDGAME
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        CarnivalofLight_TeleportOut_Post
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFx        Scripted\Praetorian\CarnivalOfLight_TeleportIn\CarnivalOfLight_TeleportOut.fx 0 constant
	TEnd
MEnd

Move CarnivalofLight_TeleportOut_Post
	Requires	ENCOUNTER TELEPORT ALTERNATE ENDGAME
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable Cycle
	Sets            CANTMOVE
	Scale		0.5

	Type		male
	  Anim          male\ready 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

## Apparition_Possessor ###########################################################

Move Apparition_Possessor
	Requires	ENCOUNTER POSSESSOR LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Apparition_Possessor_Cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\psionic_cycle 2 40
	TEnd

	Type		huge
	  Anim          male\psionic_cycle 2 40
	TEnd

	Type		fem
	  Anim          fem\psionic_cycle 2 40
	TEnd
MEnd

Move Apparition_Possessor_Cycle
	Requires	ENCOUNTER POSSESSOR LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Apparition_Possessor_Post
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\psionic_cycle 40 80
	TEnd

	Type		huge
	  Anim          male\psionic_cycle 40 80
	TEnd

	Type		fem
	  Anim          fem\psionic_cycle 40 80
	TEnd
MEnd

Move Apparition_Possessor_Post
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\psionic_cycle 160
	TEnd

	Type		huge
	  Anim          male\psionic_cycle 160
	TEnd

	Type		fem
	  Anim          fem\psionic_cycle 160
	TEnd
MEnd


#############################################################
## Posssessed Conduit
#############################################################

Move Posssessed_Conduit_Float_In
	Requires	ENCOUNTER LEVITATE ONE ALTERNATE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	Posssessed_Conduit_Float_Cycle

	Type		male
	  Anim          fem\OracleFloatTransIn01 1 46
	  PlayFX	Scripted\Praetorian\PossessedCondiut_Float\PossessedConduit_Float.fx 0 CONSTANT
	TEnd
MEnd

Move Posssessed_Conduit_Float_Cycle
	Requires	ENCOUNTER LEVITATE ONE ALTERNATE
	Priority        4
	Member		<DeathIRQ>
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          fem\OracleFloat01 1 241
	  PlayFX	Scripted\Praetorian\PossessedCondiut_Float\PossessedConduit_Float.fx 0 CONSTANT
	TEnd
MEnd

Move Posssessed_Conduit_Float_Out
	Requires	ENCOUNTER LEVITATE TWO ALTERNATE
	Priority        4
	Interrupts	Posssessed_Conduit_Float_Cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	Posssessed_Conduit_Float_Out_PostCycle
	Interpolate	15

	Type		male
	  Anim          fem\OracleFloatTransOut01 1 61
	TEnd
MEnd

Move Posssessed_Conduit_Float_Out_PostCycle
	Requires	ENCOUNTER LEVITATE TWO ALTERNATE
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd

#############################################################
## Ascension Ritual
#############################################################

Move Ascension_Ritual_Float_In
	Requires	ENCOUNTER LEVITATE ONE ENDGAME
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	Ascension_Ritual_Float_Cycle

	Type		male
	  Anim          fem\OracleFloatTransIn01 1 46
	  PlayFX	Scripted\Praetorian\Talons_Ritual_Receive_Float\Talons_Ritual_Receive_Float.fx 0 CONSTANT
	TEnd
MEnd

Move Ascension_Ritual_Float_Cycle
	Requires	ENCOUNTER LEVITATE ONE ENDGAME
	Priority        4
	Member		<DeathIRQ>
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          fem\OracleFloat01 1 241
	  PlayFX	Scripted\Praetorian\Talons_Ritual_Receive_Float\Talons_Ritual_Receive_Float.fx 0 CONSTANT
	TEnd
MEnd

Move Ascension_Ritual_Float_Out
	Requires	ENCOUNTER LEVITATE TWO ENDGAME
	Priority        4
	Interrupts	Ascension_Ritual_Float_Cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	Ascension_Ritual_Float_Out_PostCycle
	Interpolate	15

	Type		male
	  Anim          fem\OracleFloatTransOut01 1 61
	  PlayFX		Scripted\Praetorian\Talons_Ritual_Receive_Float\Talons_Ritual_Receive_Float.fx 0 CONSTANT
	TEnd
MEnd

Move Ascension_Conduit_Float_Out_PostCycle
	Requires	ENCOUNTER LEVITATE TWO ENDGAME
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          fem\Skel_ready2 1
	  PlayFX	Scripted\Praetorian\Talons_Ritual_Receive_Float\Talons_Ritual_Receive_Float.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Judgment_Receive
#############################################################

Move Judgment_Receive
	Requires	ENCOUNTER PSIONIC ENDGAME
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Judgement_Receive_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\sonicpain 1
	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
 	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
 	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0 constant
	TEnd

MEnd


Move Judgement_Receive_Cycle
	Requires	ENCOUNTER PSIONIC ENDGAME
	Priority        3
	NextMove        weak_post
	Flags		Cycle NoSizeScale
	Include sequencers\Members\Encounters_Cycles.inc

	Type		male
	  Anim          male\sonicpain 1
	  PlayFX	scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
 	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	  PlayFX	Scripted\Praetorian\Seers_Judgement_Receive\Seers_Judgement_Receive.fx 0
	TEnd

MEnd

## KNEEL Rifle Low Alert ###########################################################

Move Encounter_Rifle_Alert_Kneeling_NoCustomWeapon
	Requires	ENCOUNTER KNEEL LOW ALTERNATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Encounter_Rifle_Alert_Kneeling_NoCustomWeapon
	CycleMove	Encounter_Rifle_Alert_Kneeling_NoCustomWeapon
	CycleMove	Encounter_Rifle_Alert_Kneeling_NoCustomWeapon
	CycleMove	Encounter_Rifle_Alert_Kneeling_Break01_NoCustomWeapon
	NextMove        Kneel_getup
	Scale		0.25

	Type		male
	  Anim          male\emote_sniper_alert_kneeling 1
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd

MEnd


Move Encounter_Rifle_Alert_Kneeling_Break01_NoCustomWeapon
	Requires	ENCOUNTER KNEEL LOW ALTERNATE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Encounter_Rifle_Alert_Kneeling_NoCustomWeapon
	NextMove        Kneel_getup
	Scale		0.5

	Type		male
	  Anim          male\emote_sniper_alert_kneeling_break1 1
	  PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd

MEnd

## Ape Crouch ###########################################################

Move Encounter_Ape_Crouch_Cycle
	Requires	ENCOUNTER SIT LOW ENDGAME
	Priority	4
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Encounter_Ape_Crouch_Cycle
	Scale		0.75

	Type		male
	  Anim          male\emote_feral_insanity_cycle 1
	  PlayFx        Scripted\Praetorian\Awakened_Feral_Insanity\Awakened_Feral_Insanity.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_feral_insanity_cycle 1
PlayFx        Scripted\Praetorian\Awakened_Feral_Insanity\Awakened_Feral_Insanity.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_feral_insanity_cycle 1
PlayFx        Scripted\Praetorian\Awakened_Feral_Insanity\Awakened_Feral_Insanity.fx 0 constant
	TEnd


MEnd

###################
###### Spellcast Fear  ######
###################

Move Floating_Struggle_Apparition
	Requires	ENCOUNTER SPELLCAST ENDGAME
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	NextMove        Floating_Struggle_Apparition_Fall
	Scale           0.50

	Type		male
	  Anim          bis\cot_wriggle 19
	  PlayFx        Scripted\Floating_Struggle_Apparition\Boss_RippedSpecter.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\Floating_Struggle_Apparition\Boss_RippedSpecter.fx 0 CONSTANT
	TEnd
MEnd

Move Floating_Struggle_Apparition_Fall
	Requires	ENCOUNTER SPELLCAST ENDGAME
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Floating_Struggle_Apparition_postcycle

	Type           male
	  Anim          bis\cot_postfall 2
	TEnd

	Type		fem
	  Anim          BF\cot_postfall 2
	TEnd
MEnd

Move Floating_Struggle_Apparition_postcycle
	Requires	ENCOUNTER SPELLCAST ENDGAME
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          bis\cot_postfallcycle 1
	TEnd

	Type		fem
	  Anim          BF\cot_postfallcycle 1
	TEnd
MEnd

#############################################################
## sonic\psi pain react
#############################################################

Move Apparition_Monarch_Reabsorb_Pre
	Requires	ENCOUNTER PSIONIC ENDGAME ONE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Apparition_Monarch_Reabsorb

	Type		male
	  Anim          male\sonicpain 1
	  PlayFX	"Scripted\Apparition_Monarch_Reabsorb\Apparition_Monarch_Reabsorb.fx" 1
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
	  PlayFX	"Scripted\Apparition_Monarch_Reabsorb\Apparition_Monarch_Reabsorb.fx" 1
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	  PlayFX	"Scripted\Apparition_Monarch_Reabsorb\Apparition_Monarch_Reabsorb.fx" 1
	TEnd

MEnd

Move Apparition_Monarch_Reabsorb
	Requires	ENCOUNTER PSIONIC ENDGAME ONE
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Apparition_Monarch_Reabsorb_post

	Type		male
	  Anim          male\drain 40 110
	TEnd

	Type		huge
	  Anim          huge\drain 40 110
	TEnd

	Type		fem
	  Anim          fem\drain 40 110
	TEnd

MEnd

Move Apparition_Monarch_Reabsorb_post
	Requires	ENCOUNTER PSIONIC ENDGAME ONE
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	CombatReady

	Type		male
	  Anim          male\draintocombatready 110 135
	TEnd

MEnd

#############################################################
##  Collapse
#############################################################

Move Body_Collapse_In
	Requires	ENCOUNTER DROP LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Body_Collapse_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

MEnd

Move Body_Collapse_Cycle
	Requires	ENCOUNTER DROP LOW
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd
MEnd

#############################################################
##  Collapse (Short cycle)
#############################################################

Move Body_Collapse_Short
	Requires	ENCOUNTER DROP HIGH
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Body_Collapse_Short_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd
MEnd

Move Body_Collapse_Short_Cycle
	Requires	ENCOUNTER DROP HIGH
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd
MEnd


#### Murder of Crows  Teleport In  #########################################################

Move MurderofCrows_TeleportIn
	Requires	ENCOUNTER TELEPORT ALERT LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        MurderofCrows_TeleportIn_LookQuick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\MurderOfCrows_TeleportIn.fx 1
	TEnd
MEnd

Move MurderofCrows_TeleportIn_LookQuick
	Requires	ENCOUNTER TELEPORT ALERT LOW
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	MurderofCrows_TeleportIn_PostReady

	Type		male
	  Anim          male\lookquick 1
	TEnd

MEnd

Move MurderofCrows_TeleportIn_PostReady
	Requires	ENCOUNTER TELEPORT ALERT LOW
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd

#### CarnivalofLight Teleport Out  #########################################################

Move MurderofCrows_TeleportOut_Pre
	Requires	ENCOUNTER TELEPORT ALERT HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        MurderofCrows_TeleportOut
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\murderofcrows_teleportout 1 51
	  PlayFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\MurderOfCrows_TeleportOut.fx 1 CONSTANT

	TEnd
MEnd

Move MurderofCrows_TeleportOut
	Requires	ENCOUNTER TELEPORT ALERT HIGH
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable
	NextMove	MurderofCrows_TeleportOut_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\murderofcrows_teleportout 52 67
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move MurderofCrows_TeleportOut_Post
	Requires	ENCOUNTER TELEPORT ALERT HIGH
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable Cycle
	Sets            CANTMOVE
	NextMove	MurderofCrows_TeleportOut_Post
	CycleMove	MurderofCrows_TeleportOut_Post

	Type		male
	  Anim          male\murderofcrows_teleportout 67 68
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd


## Ape Crouch ###########################################################

Move Encounter_Casting_Heal_Kneel_Cycle
	Requires	ENCOUNTER SIT HEAL ENDGAME
	Priority	4
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Encounter_Casting_Heal_Kneel_Cycle
	Scale		0.75

	Type		male
	  Anim          male\emote_Casting_Heal_Kneel 1
	  PlayFx        Scripted\Praetorian\Carnival_Casting_Heal_Kneel\Carnival_Casting_Heal_Kneel.fx 0 constant
	TEnd

#	Type		fem
#	  Anim          fem\emote_feral_insanity_cycle 1
#	PlayFx        Scripted\Praetorian\Awakened_Feral_Insanity\Awakened_Feral_Insanity.fx 0 constant
#	TEnd
#
#	Type		huge
#	  Anim          huge\emote_feral_insanity_cycle 1
#	PlayFx        Scripted\Praetorian\Awakened_Feral_Insanity\Awakened_Feral_Insanity.fx 0 constant
#	TEnd


MEnd

#############################################################
##  Malaise Collapse
#############################################################

Move Malaise_Collapse_Pre
	Requires	ENCOUNTER DROP LOW ENDGAME
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Malaise_Collapse

	Type		male
	  Anim          male\Recluse_Cutscene01 1 75
		playfx	ENEMYFX\Praetorian\MoM\Possesion_BodyAura.fx
	TEnd

MEnd

Move Malaise_Collapse
	Requires	ENCOUNTER DROP LOW ENDGAME
	Priority        4
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Malaise_Collapse_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
		playfx	ENEMYFX\Praetorian\MoM\Shalice_Emergence.fx
	TEnd

MEnd


Move Malaise_Collapse_Cycle
	Requires	ENCOUNTER DROP LOW ENDGAME
	Priority        4
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale notselectable

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	 # PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

MEnd

#### Shalice eleport In  #########################################################

Move Shalice_TeleportIn
	Requires	ENCOUNTER TELEPORT ENDGAME ONE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Shalice_TeleportIn_LookQuick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFx       ENEMYFX\Praetorian\MoM\Shalice_Appearance.fx 0 constant
	TEnd

	Type		fem
	  Anim          male\absorb 1
	  PlayFx       ENEMYFX\Praetorian\MoM\Shalice_Appearance.fx 0 constant
	TEnd
MEnd

Move Shalice_TeleportIn_LookQuick
	Requires	ENCOUNTER TELEPORT ENDGAME ONE
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Shalice_TeleportIn_PostReady

	Type		male
	  Anim          male\lookquick 1
	  PlayFx       ENEMYFX\Praetorian\MoM\Shalice_TeleportIn.fx 0 constant
	TEnd

	Type		fem
	  Anim          male\lookquick 1
	  PlayFx       ENEMYFX\Praetorian\MoM\Shalice_TeleportIn.fx 0 constant
	TEnd

MEnd

Move Shalice_TeleportIn_PostReady
	Requires	ENCOUNTER TELEPORT ENDGAME ONE
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd

	Type		fem
	  Anim          male\suspicious 1
	TEnd
MEnd

## Encounter - Dimension Shift - Malaise Illusion Transform ###########################################################

Move Encounter_Dimension_Shift_Pre
	Requires	ENCOUNTER TRANSFORM HIGH
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Encounter_Dimension_Shift_Cycle
	Flags		NoSizeScale
	SticksOnChild	EMOTE SHIFT

	Type		male
	  Anim          male\flameaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Male.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Huge.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Female.fx" 0 CONSTANT
	TEnd

MEnd

Move Encounter_Dimension_Shift_Cycle
	Requires	ENCOUNTER TRANSFORM HIGH
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Encounter_Dimension_Shift_Post
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\emote_dimension_shift 1
	TEnd

MEnd

Move Encounter_Dimension_Shift_Post
	Requires	ENCOUNTER TRANSFORM HIGH
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0
	TEnd

	Type		huge
	  Anim          huge\fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0
	TEnd

	Type		fem
	  Anim          fem\fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0
	TEnd

MEnd

#############################################################
##  Plotting
#############################################################

Move Encounter_Plotting
	Requires	ENCOUNTER PLOTTING LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Encounter_Plotting_Cycle
	scale		1.5
	flags		FIXARMREG

	Type		male
	  Anim          male\emote_scheme 1 50
	TEnd

	Type		fem
	  Anim          fem\emote_scheme 1 50
	TEnd

	Type		huge
	  Anim          huge\emote_scheme 1 50
	TEnd
MEnd

Move Encounter_Plotting_Cycle
	Requires	ENCOUNTER PLOTTING LOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG Cycle


	Type		male
	  Anim          male\emote_scheme 50
	TEnd

	Type		fem
	  Anim          fem\emote_scheme 50
	TEnd

	Type		huge
	  Anim          huge\emote_scheme 50
	TEnd

MEnd

#############################################################
##  Baton Twirling
#############################################################

Move Baton_Twirling
	Requires	ENCOUNTER COMBAT WEAPON CLUB SPIN
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Baton_Twirling_Cycle
	flags		FIXARMREG

	Type		male
	  Anim          male\baton_twirl 1 61
	TEnd

MEnd

Move Baton_Twirling_Cycle
	Requires	ENCOUNTER COMBAT WEAPON CLUB SPIN
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		FIXARMREG Cycle


	Type		male
	  Anim          male\baton_twirl 1 61

	TEnd

MEnd

#############################################################
##  Luring
#############################################################

Move Luring
	Requires	ENCOUNTER LURE
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	Luring_Cycle
	scale		1.5

	Type		male
	  Anim          male\lure 1 37
	TEnd

MEnd

Move Luring_Cycle
	Requires	ENCOUNTER LURE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle


	Type		male
	  Anim          male\lure 1 37
	TEnd

MEnd


## INCLUDES ###########################################################

Include sequencers\player\encounters_motion.inc
Include sequencers\player\encounters_CustomWeapons.inc
Include sequencers\player\praetorian_encounters.inc
Include sequencers\player\praetorian_encounters_motion.inc
Include sequencers\player\Brian_encounter.inc
Include sequencers\player\CBruce_encounter.inc
Include sequencers\player\Ron_encounter.inc