#############################################################
## Eat - Donut
#############################################################

Move Eat_Donut_Loop1
	Requires	ENCOUNTER EATDONUT
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	Scale		0.875
	CycleMove       Eat_Donut_Loop1
	CycleMove       Eat_Donut_Loop2
	CycleMove       Eat_Donut_Bite1

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Eat_Donut_Loop2
	Requires	ENCOUNTER EATDONUT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	Scale		0.75
	CycleMove       Eat_Donut_Bite1
	CycleMove       Eat_Donut_Bite1
	CycleMove       Eat_Donut_Loop1

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	  PlayFx	FEET\Foot_NPCMale.fx 25
	  PlayFx	FEET\Foot_NPCMale.fx 45
	  PlayFx	FEET\Foot_NPCMale.fx 205
	  PlayFx	FEET\Foot_NPCMale.fx 225
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Eat_Donut_Bite1
	Requires	ENCOUNTER EATDONUT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	CycleMove       Eat_Donut_Wipe
	CycleMove       Eat_Donut_Wipe
	CycleMove       Eat_Donut_Bite1
	CycleMove       Eat_Donut_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_Eat_SFX.fx
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_Eat_SFX.fx
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_Eat_SFX.fx
	TEnd
MEnd

Move Eat_Donut_Wipe
	Requires	ENCOUNTER EATDONUT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	CycleMove       Eat_Donut_Loop1
	CycleMove       Eat_Donut_Bite1

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 15
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 35
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 15
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 35
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 1
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 15
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 35
	TEnd
MEnd

#############################################################
## Eat - Food (Random)
##############################################################

Move Eat_Food_Loop1
	Requires	ENCOUNTER EATFOOD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	CycleMove       Eat_Food_Loop1
	CycleMove       Eat_Food_Loop2
	CycleMove       Eat_Food_Bite1

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Eat_Food_Loop2
	Requires	ENCOUNTER EATFOOD
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	Scale		0.9
	CycleMove       Eat_Food_Bite1
	CycleMove       Eat_Food_Bite1
	CycleMove       Eat_Food_Loop1

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Eat_Food_Bite1
	Requires	ENCOUNTER EATFOOD
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	CycleMove       Eat_Food_Wipe
	CycleMove       Eat_Food_Wipe
	CycleMove       Eat_Food_Bite1
	CycleMove       Eat_Food_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Eat_Food_Wipe
	Requires	ENCOUNTER EATFOOD
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	CycleMove       Eat_Food_Loop1
	CycleMove       Eat_Food_Bite1

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
##  Disable PCM Device (Female Only)
#############################################################

Move Disable_PCM_Device
	Requires	ENCOUNTER DISABLEPCM
	Priority        2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		NoSizeScale FixArmReg
	NextMove        Ready

	Type		male
	  Anim          fem\disable_pcm_device 1
	  PlayFx        POWERS\ItemsAndTempPowers\PCM_Explosion\PCM_Explosion.fx 0 Constant
	TEnd
MEnd

#############################################################
## Read Blueprints
#############################################################

Move Read_Blueprints_Loop
	Requires	ENCOUNTER READBLUEPRINTS
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale FixArmReg
	CycleMove       Read_Blueprints_Loop
	CycleMove       Read_Blueprints_Loop2
	CycleMove       Read_Blueprints_Break1

	Type		male
	  Anim          male\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		NPC_male
	  Anim          male\npc_read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd
MEnd

Move Read_Blueprints_Loop2
	Requires	ENCOUNTER READBLUEPRINTS
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale FixArmReg
	Scale		0.875
	CycleMove       Read_Blueprints_Loop
	CycleMove       Read_Blueprints_Loop2
	CycleMove       Read_Blueprints_Break1

	Type		male
	  Anim          male\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		NPC_male
	  Anim          male\npc_read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\read_blueprints_loop 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd
MEnd

Move Read_Blueprints_Break1
	Requires	ENCOUNTER READBLUEPRINTS
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale FixArmReg
	Scale		1.125
	CycleMove       Read_Blueprints_Loop

	Type		male
	  Anim          male\read_blueprints_break1 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		NPC_male
	  Anim          male\npc_read_blueprints_break1 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\read_blueprints_break1 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\read_blueprints_break1 1
	  PlayFx        Generic\Read_Blueprints.fx 0 Constant
	TEnd
MEnd

#############################################################
## Sitting Poses
#############################################################

Move Sitting_Pose1
	Requires	ENCOUNTER SIT POSE ONE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 1 2
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 1 2
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 1 2
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 1 2
	TEnd
MEnd

#############################################################

Move Sitting_Pose2
	Requires	ENCOUNTER SIT POSE TWO
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 3 4
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 3 4
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 3 4
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 3 4
	TEnd
MEnd

#############################################################

Move Sitting_Pose3
	Requires	ENCOUNTER SIT POSE THREE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 5 6
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 5 6
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 5 6
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 5 6
	TEnd
MEnd

#############################################################

Move Sitting_Pose4
	Requires	ENCOUNTER SIT POSE FOUR
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 7 8
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 7 8
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 7 8
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 7 8
	TEnd
MEnd

#############################################################

Move Sitting_Pose5
	Requires	ENCOUNTER SIT POSE FIVE
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 9 10
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 9 10
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 9 10
	TEnd
MEnd

#############################################################

Move Sitting_Pose6
	Requires	ENCOUNTER SIT POSE SIX
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 11 12
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 11 12
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 11 12
	TEnd
MEnd

#############################################################

Move Sitting_Pose7
	Requires	ENCOUNTER SIT POSE SEVEN
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 13 14
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 13 14
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 13 14
	TEnd
MEnd

#############################################################

Move Sitting_Pose8
	Requires	ENCOUNTER SIT POSE EIGHT
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 15 16
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 15 16
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 15 16
	TEnd
MEnd

#############################################################

Move Sitting_Pose9
	Requires	ENCOUNTER SIT POSE NINE
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 17 18
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 17 18
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 11 12
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 17 18
	TEnd
MEnd

#############################################################

Move Sitting_Pose10
	Requires	ENCOUNTER SIT POSE TEN
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 19 20
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 19 20
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 19 20
	TEnd
MEnd

#############################################################

Move Sitting_Pose11
	Requires	ENCOUNTER SIT POSE TEN ONE
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\male_sitting_poses 21 22
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 21 22
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 13 14
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 21 22
	TEnd
MEnd

#############################################################
## Lord Recluse - Cutscene ArachnosPTT Buff
#############################################################

Move LordReclusePTTBuff_1
	Requires	ENCOUNTER RECLUSEBUFF
	Priority        2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        LordReclusePTTBuff_3
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\emote_VillainLaugh 1 136
	  PlayFx        Scripted\LordReclusePTTBuff\LordReclusePTTBuff_Explosion.fx 0
	TEnd
MEnd

Move LordReclusePTTBuff_3
	Include sequencers\Members\Encounters_Scripted.inc
	#NextMove        LordReclusePTTBuff_5
	NextMove       Lazy_CombatReady
	Interpolate     15
	Flags		CANTMOVE NoSizeScale
	Scale		0.875

	Type		male
	  Anim          male\pre_poison 1 126
	TEnd
MEnd

Move LordReclusePTTBuff_5
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove       Lazy_CombatReady
	#Interpolate     15
	Flags		NoSizeScale
	Scale		0.375

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd


#############################################################
## Penelop Yin - Captured by Rikti
#############################################################

Move Captured_Penelope_Cycle
	Requires	ENCOUNTER LEVITATE CAPTURED PENELOPE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_Penelope_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\CapturedPenelopeYin\CapturedPenelopeYin.fx 0 Constant
	TEnd
MEnd

Move Captured_Penelope_Fall
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        SC_postcycle
	Priority        1
	Sets		CANTMOVE

	Type		male
	  Anim          BF\cot_postfall 2
	TEnd
MEnd

Move Captured_Penelope_Fall_PostCycle
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Priority        1
	NextMove	Ready

	Type		male
	  Anim          BF\cot_postfallcycle 1
	TEnd
MEnd

#############################################################
## Generic - Captured by Rikti
#############################################################

Move Captured_RiktiEnergy_Cycle
	Requires	ENCOUNTER CAPTURED LEVITATE RIKTI
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_Penelope_Fall
	Scale           0.50

	Type		male
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\CapturedPenelopeYin\GenericRiktiEnergyField.fx 0 Constant
	TEnd
MEnd

#############################################################
## Sonic Death
#############################################################

Move Encounter_Death1_Pre
	Requires	ENCOUNTER HIT DEATH PSIONIC
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Interpolate     8
	Sets            NOCOLLISION
	Flags		Cycle
	CycleMove	Encounter_Death1

	Type		male
	  Anim          male\sonicpain 1 124
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1 124
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1 124
	TEnd
MEnd

Move Encounter_Death1
	Requires	ENCOUNTER HIT DEATH PSIONIC
	Priority        4
	Include sequencers\Members\Encounters_Scripted.inc
	Interpolate     12
	Sets            NOCOLLISION
	Flags		Cycle
	CycleMove	Encounter_Death1_Post

	Type		male
	  Anim          male\death_front 46
	TEnd

	Type		huge
	  Anim          huge\death_front 46
	TEnd

	Type		fem
	  Anim          fem\death_front 46
	TEnd
MEnd

Move Encounter_Death1_Post
	Requires	ENCOUNTER HIT DEATH PSIONIC
	Priority        4
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\death_front 128 129
	TEnd

	Type		huge
	  Anim          huge\death_front 128 129
	TEnd

	Type		fem
	  Anim          fem\death_front 128 129
	TEnd
MEnd

## INVISIBLE ###########################################################

Move Encounter_HideNow
	Requires	ENCOUNTER HIDE
	Priority	2
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoDraw NotSelectable
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\Skel_ready2 1
	  PlayFX	GENERIC\entityVanish.fx
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	  PlayFX	GENERIC\entityVanish.fx
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	  PlayFX	GENERIC\entityVanish.fx
	TEnd
MEnd

#############################################################
## Ledge Sits (more skirt/toga friendly)
#############################################################

Move Encounter_LedgeSit5
	Requires	ENCOUNTER POSE LEDGESIT TWO
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		CYCLE FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var5 1
	TEnd
MEnd

#############################################################

Move Encounter_LedgeSit6
	Requires	ENCOUNTER POSE LEDGESIT TWO
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		CYCLE FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var6 1
	TEnd
MEnd

#############################################################
## Midnight Squad Sits
#############################################################

Move Encounter_Sit_Chair_Teabag
	Requires	ENCOUNTER POSE SIT TEABAG ONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Encounter_Sit_Chair_Teabag_Cycle
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_sit_chair_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_sit_chair_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd
MEnd

#############################################################

Move Encounter_Sit_Sofa_Teabag
	Requires	ENCOUNTER POSE SIT TEABAG TWO
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Encounter_Sit_Sofa_Teabag_Cycle
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_sit_sofa_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_sit_sofa_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd
MEnd

#############################################################
## Reichsman Captured State
#############################################################

Move Enc_Reichsman_Captured_Pre
	Requires	ENCOUNTER CAPTURED REICHSMAN ONE
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	Interpolate	0
	Wind		FLAG_VERTICAL
	Scale		0.25
	NextMove	Enc_Reichsman_Captured_Cycle

	Type		male
	  Anim          male\Reichsman_Captured_Cycle 1 60
	  PlayFX	Scripted\CapturedReichsman\CapturedReichsman.fx 0 CONSTANT
	TEnd
MEnd

Move Enc_Reichsman_Captured_Cycle
	Requires	ENCOUNTER CAPTURED REICHSMAN ONE
	Priority        3
	Flags		Cycle NoSizeScale NotSelectable
	Wind		FLAG_VERTICAL
	Scale		0.25
	NextMove	Enc_Reichsman_Captured_Out

	Type		male
	  Anim          male\Reichsman_Captured_Cycle 1 60
	  PlayFX	Scripted\CapturedReichsman\CapturedReichsman.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Enc_Reichsman_Captured_Out
	Requires	ENCOUNTER CAPTURED REICHSMAN TWO
	Priority        3
	Interrupts	Reichsman_Captured_Pre Reichsman_Captured_Cycle
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	NextMove	Enc_Reichsman_Captured_Out_PostCycle
	Scale		0.5

	Type		male
	  Anim          fem\OracleFloatTransOut01 1 61
	TEnd
MEnd

Move Enc_Reichsman_Captured_Out_PostCycle
	Requires	ENCOUNTER CAPTURED REICHSMAN ONE
	Priority        3
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd

#############################################################
## REICHSMAN - EMPOWERMENT MACHINE
#############################################################

Move Enc_Reichsman_Empowerment
	Requires	ENCOUNTER REICHSMAN COMBAT MAGICWINDUP
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle NoSizeScale NotSelectable
	NextMove        Magic_WindUp_post

	Type		male
	  Anim          male\ground_summon 65 104
	  PlayFX	ENEMYFX\5thColumn\EnvironmentalFX\Reichsman_Untargetable.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## REICHSMAN - EMPOWERMENT MACHINE
#############################################################

Move Enc_5thColumnForceField
	Requires	ENCOUNTER FIFTHCOLUMN HIBERNATE
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle NoSizeScale NotSelectable

	Type		male
	  Anim          male\Stance_Villainous01 1
	  PlayFX	ENEMYFX\5thColumn\ForceField\BaseForceBubble.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Stance_Villainous02 1
	  PlayFX	ENEMYFX\5thColumn\ForceField\BaseForceBubble.fx 0 CONSTANT
	TEnd
MEnd