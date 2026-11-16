#############################################################
## ghouls_encounters.inc - all encounters and motion encounters
#############################################################


####################################
##### Encounters stuff   ###########
####################################

###################
### Menace Eat  #####
###################

Move Menace_eat
	Requires	ENCOUNTER EAT
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Menace_nibble
	CycleMove       Menace_lookquick
	CycleMove       Menace_lookquick_slow

	Type		ghoul
	  Anim          Beast\eat 1
	  PlayFx        GENERIC\meatbone.fx  0 Constant
	TEnd
MEnd

Move Menace_nibble
	Requires	ENCOUNTER EAT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_lookquick
	CycleMove       Menace_nibble_blend

	Type		ghoul
	  Anim          Beast\nibble 1
	  PlayFx        GENERIC\meatbone.fx  0 Constant
	TEnd
MEnd

Move Menace_nibble_blend
	Requires	ENCOUNTER EAT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_lookquick_slow

	Type		ghoul
	  Anim          Beast\nibble 11
	  PlayFx        GENERIC\meatbone.fx  0 Constant
	TEnd
MEnd

Move Menace_lookquick
	Requires	ENCOUNTER EAT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_eat

	Type		ghoul
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx  0 Constant
	TEnd
MEnd

Move Menace_lookquick_slow
	Requires	ENCOUNTER EAT
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_eat
	Scale		0.6

	Type		ghoul
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx  0 Constant
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
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
		PlayFX		ENEMYFX\Ghouls\Ghoul_HitQuick_SFX.fx 74
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 104
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
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
		PlayFX		ENEMYFX\Ghouls\Ghoul_HitLong_SFX.fx 80
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
		PlayFX		ENEMYFX\Ghouls\Ghoul_HitQuick_SFX.fx 5
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 14
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 44
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 74
		PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 104
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 37
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 74
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 37
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 74
	TEnd

	Type		goblin_beast
	  Anim          Beast\crouch 1    ####vampire_crouch 1
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
	  PlayFx		GENERIC\FX_bodyBagDispose.fx 0
	  PlayFx		GENERIC\FX_bodyBagLinger.fx 80
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX		ENEMYFX\Ghouls\Ghoul_HitQuick_SFX.fx 65
	  PlayFx    	NPCFX\NPC_Whoosh_SFX.fx 65
	  PlayFX		NPCFX\NPC_Bodybag_SFX.fx 80
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 120
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 150
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 180
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 37
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 33
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 33
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
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 90
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 120
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 150
		 PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 180
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
	  Anim          male\threat_C 1
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
	  Anim          male\threat_C 1
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
	  PlayFX	Feet\FistPunch1.fx 25
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 33
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 66
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 99
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
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
	  Anim          male\threat_C 1
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
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
	  PlayFX	Feet\Ghoul_Land1.fx 12
	  PlayFX	Feet\Ghoul_Land1.fx 34
	  PlayFX	Feet\Ghoul_Land1.fx 56
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
############
Move PostLoot
	Include sequencers\Members\Encounters_Idle.inc

	Type		male
	  Anim          male\loot 29
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

	Type		male
	  Anim          male\brawlcycle 1 31
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
	  # PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  # PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  # PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
	  # PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 90
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

## RUMBLE A ###########################################################

Move Rumble_A
	Requires	ENCOUNTER RUMBLEA
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\rumble_fists_A 1 124
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Swipe_SFX.fx 5
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 37
	  PlayFX		ENEMYFX\Ghouls\Ghoul_HitQuick_SFX.fx 37
  	  PlayFX		NPCFX\NPC_Kick_SFX.fx 50
	  PlayFX		ENEMYFX\Ghouls\Ghoul_HitQuick_SFX.fx 50
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 80


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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Run_SFX.fx
	  PlayFX		NPCFX\NPC_ClothLong_SFX.fx 1
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 32
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Swipe_SFX.fx 32
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 48
  	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 70
  	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 100

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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 90
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 120
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 150
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 180
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 30
	  PlayFX		ENEMYFX\Ghouls\Ghoul_SwipeBehavior_SFX.fx 60
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 90
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Run_SFX.fx 125
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 150
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 180
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Swipe_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 12
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 60
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 5
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx 60
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Swipe_SFX.fx 60
	  PlayFX		NPCFX\NPC_ClothShort_SFX.fx 58
	TEnd
MEnd

## BAT LOOKOUT ###########################################################

Move Emote_BatLookout_Pre
	Requires	EMOTE WEAPON BLUNT LOOKOUT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_BatLookout_Cycle
	Scale           0.50

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

Move Emote_BatLookout_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd


MEnd

#############################################################

Move Emote_BatLookout_Alt2_Pre
	Requires	EMOTE WEAPON BLUNT LOOKOUT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_BatLookout_Left
	Scale           0.70

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

Move Emote_BatLookout_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_BatLookout_Right
	CycleMove	Emote_BatLookout_Left
	Scale           0.70

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd


MEnd

Move Emote_BatLookout_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_BatLookout_Right
	CycleMove	Emote_BatLookout_Left
	Scale           0.90

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
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
	  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	  PlayFX		ENEMYFX\Ghouls\Ghoul_SwipeBehavior_SFX.fx 28
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

MEnd

#############################################################

Move Sit_Low_getup
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ready
	Interpolate     8

	Type		male
	  Anim          male\astro 78
	TEnd

MEnd

###################################################
#################   PL_OutCold   ##################
###################################################

Move OutCold01
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 1 3
	TEnd

MEnd

Move OutCold02
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 5 8
	TEnd

MEnd

Move OutCold03
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 10 13
	TEnd

MEnd

Move OutCold04
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 15 18
	TEnd

MEnd

###########MOTION ENCOUNTERS#########################################################################################

## LOITER READY ###########################################################

Move Loiter_ReadyA
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Ghoul_Left.fx 7
	  PlayFX		Feet\Ghoul_Right.fx 12
	  PlayFX		Feet\Ghoul_Left.fx 67
	  PlayFX		Feet\Ghoul_Right.fx 72
	TEnd

MEnd

#############################################################

Move Loiter_ReadyB
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.30

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Ghoul_Left.fx 7
	  PlayFX		Feet\Ghoul_Right.fx 12
	  PlayFX		Feet\Ghoul_Left.fx 67
	  PlayFX		Feet\Ghoul_Right.fx 72
	TEnd

MEnd

#############################################################

Move Loiter_ReadyC
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.80

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Ghoul_Left.fx 7
	  PlayFX		Feet\Ghoul_Right.fx 12
	  PlayFX		Feet\Ghoul_Left.fx 67
	  PlayFX		Feet\Ghoul_Right.fx 72
	TEnd

MEnd

## Sneak_Patrol_Ready Walk ###########################################################

Move Sneak_Patrol_Pre
	Requires	IDLE WALK SCARED
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Cycle
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 1 129
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd
MEnd

Move Sneak_Patrol_Cycle
	Requires	 IDLE WALK
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 129 257
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd
MEnd

Move Sneak_Patrol_Post
	Requires	IDLE WALK
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 257 321
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd

MEnd

## Sneak_Patrol_Ready Walk ###########################################################

Move Ghoul_Patrol_Pre
	Requires	FORWARD LOITER
	Priority	2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Ghoul_Patrol_Pre
	CycleMove        Ghoul_Patrol_Post
	CycleMove        Ghoul_Patrol_Cycle
	CycleMove        Ghoul_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 1 129
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd
MEnd

Move Ghoul_Patrol_Cycle
	Requires	FORWARD LOITER
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Ghoul_Patrol_Pre
	CycleMove        Ghoul_Patrol_Post
	CycleMove        Ghoul_Patrol_Pre
	CycleMove        Ghoul_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 129 257
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd
MEnd

Move Ghoul_Patrol_Post
	Requires	FORWARD LOITER
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        Ghoul_Patrol_Pre
	CycleMove        Ghoul_Patrol_Pre
	CycleMove        Ghoul_Patrol_Post
	CycleMove        Ghoul_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 257 321
	  PlayFX	Feet\Ghoul_Left.fx 0
	  PlayFX	Feet\Ghoul_Right.fx 16
	  PlayFX	Feet\Ghoul_Left.fx 32
	  PlayFX	Feet\Ghoul_Right.fx 48
	  PlayFX	Feet\Ghoul_Left.fx 64
	  PlayFX	Feet\Ghoul_Right.fx 80
	  PlayFX	Feet\Ghoul_Left.fx 96
	  PlayFX	Feet\Ghoul_Right.fx 112
	  PlayFX	Feet\Ghoul_Left.fx 128
	TEnd

MEnd

## Encounter - Ghouls Ambush ###########################################################

Move Ghouls_Ambush
	Requires	SPAWN ALTERNATE
	Priority	2
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	Sets            CANTMOVE NOCOLLISION
	Flags		Cycle
	NextMove        Ghouls_Ambush_Drop

	Type		male
	  Anim          male\ghouls_ambush 1 10
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd

MEnd

Move Ghouls_Ambush_Drop
	Flags		NotSelectable
	Sets            CANTMOVE
	NextMove        Ghouls_Ambush_Post

	Type		male
	  Anim          male\ghouls_ambush 10 60
	TEnd

MEnd

Move Ghouls_Ambush_Post
	Member		<Ready> <ReactIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ>
	NextMove        Ready
	#Interpolate	8

	Type		male
	  Anim          male\ghouls_ambush 60 69
	TEnd

MEnd

#####Jump Back Up######################################################

Move Ghouls_Ambush_Jump_Up_Pre
	Requires	ENCOUNTER RUN ALTERNATE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ghouls_Ambush_Jump_Up_Post

	Type		male
	  Anim          male\ghouls_ambush_jump_up 1 22
	TEnd

MEnd

Move Ghouls_Ambush_Jump_Up_Post
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


#####Ghoul Float Cycle####################################################

Move Ghouls_Float_Cycle_Pre
	Requires	ENCOUNTER FLOATSTUN ALTERNATE ONE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ghouls_Float_Cycle
	Flags		NotSelectable
	Sets            CANTMOVE

	Type		male
	  Anim          male\ghoul_float 1 50
	TEnd

MEnd

Move Ghouls_Float_Cycle
	Requires	ENCOUNTER FLOATSTUN ALTERNATE ONE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NotSelectable
	Sets            CANTMOVE
	Flags		Cycle

	Type		male
	  Anim          male\ghoul_float 1 50
	TEnd

MEnd

#####Ghoul Beam Out ####################################################

Move Ghouls_Beam_Out_Pre
	Requires	ENCOUNTER TELEPORT ALTERNATE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Ghouls_Beam_Out_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\ghoul_beam_out 1 75
	TEnd

MEnd

Move Ghouls_Beam_Out_Post
	Requires	ENCOUNTER TELEPORT ALTERNATE
	Include sequencers\Interrupts\Encounters_Idle.inc
	Flags		NotSelectable
	Sets            CANTMOVE

	Type		male
	  Anim          male\ghoul_beam_out 75
	  PlayFx        ENEMYFX\Seers\Summon_Portal_Out.fx 0
	TEnd

MEnd


####Praetorian Teleport In  #########################################################

Move Ghouls_TeleportIn
	Requires	ENCOUNTER TELEPORT ALTERNATE DOWN
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ghouls_TeleportIn_LookQuick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFx        ENEMYFX\Seers\Summon_Portal_In.fx 0
	TEnd
MEnd

Move Ghouls_TeleportIn_LookQuick
	Requires	ENCOUNTER TELEPORT ALTERNATE DOWN
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\lookquick 1
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


#########################################

#############################################################
## Captured_Ghoul_Cycle
#############################################################

Move Captured_Ghoul_Cycle
	Requires	ENCOUNTER CAPTURED LOW BIGFALL LANDING
	Priority        5
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Sets		CANTMOVE
	Flags		Cycle
	NextMove        Captured_Ghoul_Fall
	Scale           0.50

	Type		ghoul
	  Anim          BF\cot_wriggle 19
	  PlayFx        Scripted\CapturedPenelopeYin\CapturedPenelopeYin.fx 0 Constant
	TEnd
MEnd

Move Captured_Ghoul_Fall
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Ghouls_Ready_B
	Priority        1
	Sets		CANTMOVE

	Type		ghoul
	  Anim          male\ghoul_fall_getup 1 130
	TEnd

	Type		Huge
	  Anim          huge\ghoul_fall_getup 1 130
	TEnd

	Type		fem
	  Anim          fem\ghoul_fall_getup 1 130
	TEnd
MEnd
