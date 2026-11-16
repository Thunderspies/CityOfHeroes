#############################################################
## Weapons_DualWield_Claws.inc - Claw sequences
#############################################################

#Move Ready_To_ClawReady
#	Requires	COMBAT WEAPON DUAL CLAW
#	Priority	4
#	Interrupts	<Ready> <Combat> <WepCombat>
#	Include sequencers\Members\Combat_DualWeapon.inc
#	NextMove	Draw_Claws_Cycle
#	Flags		NoSizeScale
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim		male\claw_out 1 19
#	TEnd
#MEnd

#############################################################

Move ClawReady
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\claw_cycle	1
	TEnd

	Type		huge
	  Anim		huge\claw_cycle	1 31
	TEnd

	Type		fem
	  Anim		fem\claw_cycle 1 31
	TEnd

	Type		mako
	  Anim		male\claw_cycle_hands 1	31
	TEnd
MEnd

#############################################################

Move clawcycle_15
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawReady
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_cycle	15
	TEnd

	Type		huge
	  Anim		huge\claw_cycle	15 31
	TEnd

	Type		fem
	  Anim		fem\claw_cycle 15 31
	TEnd

	Type		mako
	  Anim		male\claw_cycle_hands 15 31
	TEnd
MEnd

#############################################################

Move Claws_PostBlend
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		NoSizeScale ReqInputs
	NextMove	ClawReady
	Interpolate	15
	Scale		0.50

	Type		male
	  Anim		male\claw_cycle	1
	TEnd

	Type		huge
	  Anim		huge\claw_cycle	1 31
	TEnd

	Type		fem
	  Anim		fem\claw_cycle 1 31
	TEnd

	Type		mako
	  Anim		male\claw_cycle_hands 1 31
	TEnd
MEnd


## LAZY	CLAW CYCLE ###########################################################

Move Lazy_clawcycle
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\claw_cycle	1
	TEnd
MEnd

Move Lazy_clawcycle_15
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_clawcycle
	Flags		ReqInputs
	Scale		0.50

	Type		male
	  Anim		male\claw_cycle	15
	TEnd
MEnd


## BLOCKS ###########################################################

Move Claws_Block_R
	Requires	COMBAT WEAPON DUAL CLAW BLOCK
	Priority	5
	Interrupts	<BlockIRQ> C_Dodge_L
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		PickRandomly
	NextMove	ClawReady

	Type		male
	  Anim		male\dual_dodge	1 49
	TEnd
MEnd

#############################################################

Move Claws_Block_L
	Requires	COMBAT WEAPON DUAL CLAW BLOCK
	Priority	5
	Interrupts	<BlockIRQ> C_Dodge_L
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		PickRandomly
	NextMove	ClawReady

	Type		male
	  Anim		male\dual_dodge	52 104
	TEnd
MEnd


## HIT REACTIONS ###########################################################

Move Claws_HitQuick
	Requires	COMBAT WEAPON DUAL CLAW QUICK HIT
	Priority	5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawReady
	Flags		HitReact

	Type		male
	  Anim		male\dual_HQ 1
	TEnd
MEnd

#############################################################

Move A_Claws_HitQuick
	Requires	FLY COMBAT WEAPON DUAL CLAW QUICK HIT
	Priority	6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	A_ClawsReady
	Flags		HitReact

	Type		male
	  Anim		male\air_dual_HQ 1
	TEnd
MEnd


## CLAW	DRAW ###########################################################

Move Draw_Claws
	Requires	COMBAT WEAPON DUAL CLAW SPAWNSIDE
	Priority	55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Draw_Claws_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Draw_Claws_Cycle
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove	ClawReady

	Type		male
	  Anim		male\claw_out 20 40
	TEnd

	Type		huge
	  Anim		huge\claw_out 20 40
	TEnd

	Type		fem
	  Anim		fem\claw_out 20	40
	TEnd
MEnd


## CLAWS BRAWL - (Alternates between Dual Wield Kick Brawl and this) ###########################################################

Move Claws_BrawlRight
	Requires	COMBAT WEAPON DUAL CLAW PUNCH BRAWL
	Priority        5 #Same Priority as DualWield Brawl move
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_BrawlRight_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawl_high2 1 25
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Claws_BrawlRight_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Brawlcycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawl_high2 26 48
	TEnd
MEnd

#############################################################

Move Claws_BrawlLeft
	Requires	COMBAT WEAPON DUAL CLAW PUNCH BRAWL
	Priority        5 #Same Priority as DualWield Brawl move
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_BrawlLeft_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawl_high2 50 76
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Claws_BrawlLeft_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Brawlcycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawl_high2 77
	TEnd
MEnd


## SWIPE ###########################################################

Move ClawSwipe_Right_Draw
	Requires	COMBAT WEAPON DUAL CLAW ATTACK SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Right
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move ClawSwipe_Right
	Requires	COMBAT WEAPON DUAL CLAW ATTACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Right_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_mid2 1 20
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		huge
	  Anim		huge\claw_mid2 1 20
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		fem
	  Anim		fem\claw_mid 1 20
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		mako
	  Anim		male\claw_mid_hands 1 20
	TEnd
MEnd

Move ClawSwipe_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_mid2 21 31
	TEnd

	Type		huge
	  Anim		huge\claw_mid2 21 31
	TEnd

	Type		fem
	  Anim		fem\claw_mid 21	31
	TEnd

	Type		mako
	  Anim		male\claw_mid_hands 21 31
	TEnd
MEnd

#############################################################

Move ClawSwipe_Left_Draw
	Requires	COMBAT WEAPON DUAL CLAW ATTACK SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Left
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move ClawSwipe_Left
	Requires	COMBAT WEAPON DUAL CLAW ATTACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Left_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_mid2 40 60
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		huge
	  Anim		huge\claw_mid2 40 60
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		fem
	  Anim		fem\claw_mid 40 60
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd

	Type		mako
	  Anim		male\claw_mid_hands 40 60
	TEnd
MEnd

Move ClawSwipe_Left_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_mid2 61 71
	TEnd

	Type		huge
	  Anim		huge\claw_mid2 61 71
	TEnd

	Type		fem
	  Anim		fem\claw_mid 61 71
	TEnd

	Type		mako
	  Anim		male\claw_mid_hands 61 71
	TEnd
MEnd

## CLAW	LOW ATTACK ###########################################################

Move Claws_Low_Draw
	Requires	COMBAT WEAPON DUAL CLAW LOW ATTACK SPAWNSIDE
	Priority	57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Low_Pre
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd
MEnd

Move Claws_Low_Pre
	Requires	COMBAT WEAPON DUAL CLAW LOW ATTACK
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Low
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_low 1	25
	TEnd
MEnd

Move Claws_Low
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_ClawCycle_15
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_low 25
	TEnd
MEnd

## CLAW	HIGH ATTACK ###########################################################

Move Claws_High_Draw
	Requires	COMBAT WEAPON DUAL CLAW HIGH ATTACK SPAWNSIDE
	Priority	57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_High
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_High
	Requires	COMBAT WEAPON DUAL CLAW HIGH ATTACK
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_High_post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_upcut	1 26
	TEnd

	Type		huge
	  Anim		huge\claw_upcut	1 26
	TEnd

	Type		fem
	  Anim		fem\claw_upcut 1 26
	TEnd
MEnd

Move Claws_High_post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	clawcycle_15
	Scale		0.80
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_upcut	27
	TEnd

	Type		huge
	  Anim		huge\claw_upcut	27
	TEnd

	Type		fem
	  Anim		fem\claw_upcut 27
	TEnd
MEnd


## CLAW	FLURRY ###########################################################

Move Claws_Flurry_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWFLURRY SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Flurry
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd
MEnd

Move Claws_Flurry
	Requires	COMBAT WEAPON DUAL CLAW CLAWFLURRY
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Flurry_post
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\speedattack 1 90
	TEnd
MEnd

Move Claws_Flurry_post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_clawcycle_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\speedattack 91
	TEnd
MEnd


## CLAW	ONE\TWO	###########################################################

Move Claws_OneTwoPunch_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWSONETWO SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_OneTwoPunch
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_OneTwoPunch
	Requires	COMBAT WEAPON DUAL CLAW CLAWSONETWO
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_OneTwoPunch_Post
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\1_2punch 1	39
	  PlayFx	POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd

	Type		huge
	  Anim		huge\1_2punch 1	39
	  PlayFx	POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd

	Type		fem
	  Anim		fem\1_2punch 1 39
	  PlayFx	POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd
MEnd

Move Claws_OneTwoPunch_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Brawlcycle_15
	Scale		0.50
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\1_2punch 40
	TEnd

	Type		huge
	  Anim		huge\1_2punch 40
	TEnd

	Type		fem
	  Anim		fem\1_2punch 40
	TEnd
MEnd

## CLAW	GUT PUNCH ###########################################################

Move Claws_GutPunch_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWSGUTPUNCH SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_GutPunch
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_GutPunch
	Requires	COMBAT WEAPON DUAL CLAW CLAWSGUTPUNCH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_GutPunch_post
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\gut_punch 1 31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd

	Type		huge
	  Anim		huge\gut_punch 1 31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd

	Type		fem
	  Anim		fem\gut_punch 1	31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd
MEnd

Move Claws_GutPunch_post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Brawlcycle_15
	Scale		0.50
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\gut_punch 32
	TEnd

	Type		huge
	  Anim		huge\gut_punch 32
	TEnd

	Type		fem
	  Anim		fem\gut_punch 32
	TEnd
MEnd

## CLAW	SPEED ATTACK ###########################################################

Move Claws_Speed_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWSPEED SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Speed
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Speed
	Requires	COMBAT WEAPON DUAL CLAW CLAWSPEED
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Speed_post
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_speed	1 54
	TEnd

	Type		huge
	  Anim		huge\claw_speed	1 54
	TEnd

	Type		fem
	  Anim		fem\claw_speed 1 54
	TEnd
MEnd

Move Claws_Speed_post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	clawcycle_15
	Scale		0.50
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_speed	55
	TEnd

	Type		huge
	  Anim		huge\claw_speed	55
	TEnd

	Type		fem
	  Anim		fem\claw_speed 55
	TEnd
MEnd

## CLAW	SPIN ###########################################################

Move Claws_Spin_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWAREA SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Spin
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Spin
	Requires	COMBAT WEAPON DUAL CLAW CLAWAREA
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Spin_Land
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spin 1 32
	TEnd

	Type		huge
	  Anim		huge\claw_spin 1 32
	TEnd

	Type		fem
	  Anim		fem\claw_spin 1	32
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 1 32
	TEnd
MEnd

Move Claws_Spin_Land
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Spin_Post
	Interpolate	0
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spinpost 1 45
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		huge
	  Anim		huge\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		fem
	  Anim		fem\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 33	78
	  PlayFx	FEET\LAND1.FX  1
	TEnd
MEnd

Move Claws_Spin_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	clawcycle_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_spinpost 46
	TEnd

	Type		huge
	  Anim		huge\claw_spin 79
	TEnd

	Type		fem
	  Anim		fem\claw_spin 79
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 79
	TEnd
MEnd


## CLAW	AOE ###########################################################

Move Claws_JumpArea_Draw
	Requires	COMBAT WEAPON DUAL CLAW JUMPCLAW SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_JumpArea
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_JumpArea
	Requires	COMBAT WEAPON DUAL CLAW JUMPCLAW
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_JumpArea_Post
	Sets		CANTMOVE

	Type		male
	  Anim		male\claw_jump_area 1 70
	TEnd

	Type		RockSuit
	  Anim		beast\claw_jump_area 1 70
	TEnd

	Type		mako
	  Anim		male\claw_jump_area 1 70
	TEnd
MEnd

Move Claws_JumpArea_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_clawcycle
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_jump_area 71
	TEnd

	Type		RockSuit
	  Anim		huge\claw_jump_area 71
	TEnd

	Type		mako
	  Anim		male\claw_jump_area 71
	TEnd
MEnd

## CLAW	'BASEBALL' PUNCH ###########################################################

Move Claws_BBpunch_Draw
	Requires	COMBAT WEAPON DUAL CLAW BASEBALLPUNCH SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_BBpunch
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_BBpunch
	Requires	COMBAT WEAPON DUAL CLAW BASEBALLPUNCH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_BBpunch_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\baseball_punch 1 43
	TEnd

	Type		huge
	  Anim		huge\baseball_punch 1 43
	TEnd

	Type		fem
	  Anim		fem\baseball_punch 1 43
	TEnd
MEnd

Move Claws_BBpunch_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Claws_PostBlend #Combat_half
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\baseball_punch 44
	TEnd

	Type		huge
	  Anim		huge\baseball_punch 44
	TEnd

	Type		fem
	  Anim		fem\baseball_punch 44
	TEnd
MEnd


## CLAW	MAGIC THROW ###########################################################

Move Claws_Frisbee_Draw
	Requires	COMBAT WEAPON DUAL CLAW MAGICTHROW SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Frisbee
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Frisbee
	Requires	COMBAT WEAPON DUAL CLAW MAGICTHROW
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Frisbee_Post
	Sets		CANTMOVE
	Flags		NoSizeScale #PitchToTarget

	Type		male
	  Anim		male\frisbee 1 30
	TEnd

	Type		huge
	  Anim		huge\frisbee 1 30
	TEnd

	Type		fem
	  Anim		fem\frisbee 1 30
	TEnd
MEnd

Move Claws_Frisbee_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Claws_PostBlend #Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\frisbee 31	70
	TEnd

	Type		huge
	  Anim		huge\frisbee 31	70
	TEnd

	Type		fem
	  Anim		fem\frisbee 31 70
	TEnd
MEnd


## CLAWS BACKSTAB WINDUP ###########################################################

Move Claws_BackstabWindup_Draw
	Requires	COMBAT WEAPON DUAL CLAW WINDUPCLAWSBACKSTAB SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        Claws_BackstabWindup
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd
MEnd

Move Claws_BackstabWindup
	Requires	COMBAT WEAPON DUAL CLAW WINDUPCLAWSBACKSTAB
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        Claws_BackstabWindup_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\claws_backstab 1 69
	TEnd
MEnd

Move Claws_BackstabWindup_Cycle
	Requires	COMBAT WINDUPCLAWSBACKSTAB
	Priority        2
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\claws_backstab 75 125
	TEnd
MEnd

## CLAW	BACKSTAB ###########################################################

Move Claws_BackstabWindup_Quick_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWSBACKSTAB SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        ClawsBackstab
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd
MEnd

Move ClawsBackstab
	Requires	COMBAT WEAPON DUAL CLAW CLAWSBACKSTAB
	Priority	5
	Interrupts	<AttackIRQ> <ReactIRQ> <HitIRQ> Claws_BackstabWindup Claws_BackstabWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawsBackstab2
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claws_backstab 130	141
	TEnd
MEnd

Move ClawsBackstab2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawsBackstab_Postcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claws_backstab 141	231
	TEnd
MEnd

Move ClawsBackstab_Postcycle
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim		male\claws_backstab 235	284
	TEnd
MEnd


## CLAW	PLACATE	###########################################################

Move Claws_Placate_Draw
	Requires	COMBAT WEAPON DUAL CLAW PLACATE SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Placate
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Placate
	Requires	COMBAT WEAPON DUAL CLAW PLACATE
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Placate_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\placate 1 40
	TEnd
MEnd

Move Claws_Placate_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_clawcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\placate 41
	TEnd
MEnd


## CLAW	JUMPSPIN QUICK ###########################################################

Move Claws_JumpSpin
	Requires	COMBAT WEAPON DUAL CLAW CLAWAREA QUICK
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_JumpSpin_Land
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spin 1 32
	TEnd

	Type		huge
	  Anim		huge\claw_spin 1 32
	TEnd

	Type		fem
	  Anim		fem\claw_spin 1	32
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 1 32
	TEnd
MEnd

Move Claws_JumpSpin_Land
	Requires	COMBAT
	Member		<DeathIRQ>
	NextMove	Claws_JumpSpin_Post
	Interpolate	0
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spinpost 1 45
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		huge
	  Anim		huge\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		fem
	  Anim		fem\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 33	78
	  PlayFx	FEET\LAND1.FX  1
	TEnd
MEnd

Move Claws_JumpSpin_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	clawcycle_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_spinpost 46
	TEnd

	Type		huge
	  Anim		huge\claw_spin 79
	TEnd

	Type		fem
	  Anim		fem\claw_spin 79
	TEnd

	Type		mako
	  Anim		male\claw_spin_hands 79
	TEnd
MEnd


## CLAW	RAPID BLAST ###########################################################

Move Claws_Rapidblast_Draw
	Requires	COMBAT WEAPON DUAL CLAW RAPIDBLAST SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Rapidblast
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd
MEnd

Move Claws_Rapidblast
	Requires	COMBAT WEAPON DUAL CLAW RAPIDBLAST
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Rapidblast_Post
	Sets		CANTMOVE
	Flags		PITCHTOTARGET NoSizeScale

	Type		male
	  Anim		male\claws_rapidblast 1 60
	TEnd
MEnd

Move Claws_Rapidblast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Lazy_clawcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claws_rapidblast 60
	TEnd
MEnd


## CLAW	BOOSTUP	###########################################################

Move Claws_Boostup_Draw
	Requires	COMBAT WEAPON DUAL CLAW BOOSTUP SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Boostup
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Boostup
	Requires	COMBAT WEAPON DUAL CLAW BOOSTUP
	Priority	5
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Claws_Boostup_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim		male\absorb 1
	TEnd

	Type		huge
	  Anim		huge\boostup 1
	TEnd

	Type		fem
	  Anim		fem\boostup 1
	TEnd
MEnd

Move Claws_Boostup_PostCycle
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale
	NextMove	Claws_PostBlend

	Type		male
	  Anim          male\Skel_ready2 1 60
	TEnd

	Type		huge
	  Anim          huge\mon_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd


## CLAW	PRE-POISON ###########################################################

Move Claws_Poison_Draw
	Requires	COMBAT WEAPON DUAL CLAW POISON SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Poison
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Poison
	Requires	COMBAT WEAPON DUAL CLAW POISON
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_Poison_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\pre_poison	1 88
	TEnd

	Type		huge
	  Anim		huge\pre_poison	1 88
	TEnd

	Type		fem
	  Anim		fem\pre_poison 1 88
	TEnd
MEnd

Move Claws_Poison_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Claws_PostBlend
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\pre_poison	89
	TEnd

	Type		huge
	  Anim		huge\pre_poison	89
	TEnd

	Type		fem
	  Anim		fem\pre_poison 89
	TEnd
MEnd


## CLAW	KNIFE THROW ###########################################################

Move Claws_ThrowKnife_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWKNIFETHROW SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_ThrowKnife
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_ThrowKnife
	Requires	COMBAT WEAPON DUAL CLAW CLAWKNIFETHROW
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_ThrowKnife_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\wm_throwknife 1 23
	TEnd

	Type		huge
	  Anim		huge\wm_throwknife 1 23
	TEnd

	Type		fem
	  Anim		fem\wm_throwknife 1 23
	TEnd
MEnd

Move Claws_ThrowKnife_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	clawcycle_15
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\wm_throwknife 24
	TEnd

	Type		huge
	  Anim		huge\wm_throwknife 24
	TEnd

	Type		fem
	  Anim		fem\wm_throwknife 24
	TEnd
MEnd


## CLAW	TAUNT ###########################################################

Move Claws_Taunt_Draw
	Requires	COMBAT WEAPON DUAL CLAW TAUNT VARIATIONC SPAWNSIDE
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_Taunt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd
MEnd

Move Claws_Taunt
	Requires	COMBAT WEAPON DUAL CLAW TAUNT VARIATIONC
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_Taunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claws_taunt 1 45
	  PlayFX	feet\maletaunt4.fx
	TEnd

	Type		huge
	  Anim          male\claws_taunt 1 45
	  PlayFX	FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          male\claws_taunt 1 45
	  PlayFX	FEET\femtaunt10.FX
	TEnd
MEnd

Move Claws_Taunt_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Lazy_clawcycle_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\claws_taunt 45 85
	TEnd
MEnd


## CLAWS POISON STAB ###########################################################

Move Claws_PoisonStab_Draw
	Requires	COMBAT WEAPON DUAL CLAW ATTACK POISON SPAWNSIDE
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_PoisonStab

	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\claw_out 1 19
	TEnd
MEnd

Move Claws_PoisonStab
	Requires	COMBAT WEAPON DUAL CLAW ATTACK POISON
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_PoisonStab_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\poison_stab 1 25
	TEnd

	Type		huge
	  Anim          huge\poison_stab 1 25
	TEnd

	Type		fem
	  Anim          fem\poison_stab 1 25
	TEnd
MEnd

Move Claws_PoisonStab_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        clawcycle_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\poison_stab 26 46
	TEnd

	Type		huge
	  Anim          huge\poison_stab 26 46
	TEnd

	Type		fem
	  Anim          fem\poison_stab 26 46
	TEnd
MEnd


## CLAW WRIST SHOOT ###########################################################

Move Claws_WristShoot_Draw
	Requires	COMBAT WEAPON DUAL CLAW CAST HAND CROSSBOW LOW SPAWNSIDE
	Priority        59
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_WristShoot
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\claw_out 1 19
	TEnd
MEnd

Move Claws_WristShoot
	Requires	COMBAT WEAPON DUAL CLAW CAST HAND CROSSBOW LOW
	Priority        8
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_WristShoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim		male\claw_wristblast 1 45
	  PitchStart	5
	TEnd
MEnd

Move Claws_WristShoot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Lazy_clawcycle
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  PitchEnd	30
	  Anim		male\claw_wristblast 46
	TEnd
MEnd

#############################################################

Move Claws_PsiBlast_Quick_Draw
	Requires	COMBAT WEAPON DUAL CLAW PSIONICBLAST HIGH SPAWNSIDE
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_PsiBlast_Quick
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\claw_out 1 19
	TEnd
MEnd

Move Claws_PsiBlast_Quick
	Requires	COMBAT WEAPON DUAL CLAW PSIONICBLAST HIGH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		2.0

	Type		male
	  Anim          male\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move Claws_PsiBlast
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_PsiBlast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 96 121
	  PitchAngle	0.2
	TEnd
MEnd

Move Claws_PsiBlast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Claws_PostBlend
	Flags		PitchToTarget NoSizeScale ReqInputs

	Type		male
	  Anim          male\psionic 122
	TEnd

	Type		huge
	  Anim          huge\psionic 122
	TEnd

	Type		fem
	  Anim          fem\psionic 122
	TEnd
MEnd

#############################################################

Move Claws_Mouth_SlowCast_Draw
	Requires	COMBAT WEAPON DUAL CLAW HEADCAST HIGH STRONG SPAWNSIDE
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_Mouth_SlowCast
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\claw_out 1 19
	TEnd
MEnd

Move Claws_Mouth_SlowCast
	Requires	COMBAT WEAPON DUAL CLAW HEADCAST HIGH STRONG
	Priority        7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_Mouth_SlowCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.6

	Type		male
	  Anim          male\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move Claws_Mouth_SlowCast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Claws_PostBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\eye 50 60
	TEnd

	Type		huge
	  Anim          huge\eye 50
	TEnd

	Type		fem
	  Anim          fem\eye 50
	TEnd
MEnd

#############################################################

Move Claws_GrenadeLob_Draw
	Requires	COMBAT WEAPON DUAL CLAW GRENADE SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Claws_GrenadeLob
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\claw_out 1 19
	TEnd
MEnd

Move Claws_GrenadeLob
	Requires	COMBAT WEAPON DUAL CLAW GRENADE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>3
	NextMove        Claws_GrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\grenade_lob 1 40
	TEnd

	Type		huge
	  Anim          huge\grenade_lob 1 40
	TEnd

	Type		fem
	  Anim          fem\grenade_lob 1 40
	TEnd
MEnd

Move Claws_GrenadeLob_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Claws_PostBlend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\grenade_lob 41
	TEnd

	Type		huge
	  Anim          huge\grenade_lob 41
	TEnd

	Type		fem
	  Anim		fem\grenade_lob 41
	TEnd
MEnd

## CLAW	WIDOW SLASH ###########################################################

Move Claws_WidowSlash_Draw
	Requires	COMBAT WEAPON DUAL CLAW WIDOWSLASH SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_WidowSlash
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Claws_WidowSlash
	Requires	COMBAT WEAPON DUAL CLAW WIDOWSLASH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_WidowSlash2
	Sets		CANTMOVE
	Flags		NoSizeScale
	Scale		0.75

	Type		male
	  Anim		male\widow_Slash 5 25
	TEnd
MEnd

Move Claws_WidowSlash2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Claws_WidowSlash_Post
	Sets		CANTMOVE
	Flags		NoSizeScale
	Scale		0.875

	Type		male
	  Anim		male\widow_Slash 26 40
	TEnd
MEnd

Move Claws_WidowSlash_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Scale		0.875
	Flags		NoSizeScale

	Type		male
	  Anim		male\widow_Slash 41 50
	TEnd
MEnd