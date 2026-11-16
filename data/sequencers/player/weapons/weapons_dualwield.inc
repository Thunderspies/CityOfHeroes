#############################################################
## Weapons_DualWield.inc - Duel	Wield Weapons
#############################################################

## KICK BRAWL ###########################################################

Move DualWield_Brawl
	Requires	COMBAT WEAPON DUAL PUNCH BRAWL
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Brawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\Dual_Brawl 5 30
	TEnd
MEnd

Move DualWield_Brawl_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Brawl_Blend

	Type		male
	  Anim          male\Dual_Brawl 31 50
	TEnd
MEnd

Move DualWield_Brawl_Blend
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Scale           0.50
	Interpolate	15

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

#############################################################

Move A_DualWield_Brawl
	Requires	FLY COMBAT WEAPON DUAL PUNCH BRAWL
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Brawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_dual_brawl 5 30
	TEnd
MEnd

Move A_DualWield_Brawl_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_DualWield_Brawl_Post_Blend

	Type		male
	  Anim          male\air_dual_brawl 31 50
	TEnd
MEnd

Move A_DualWield_Brawl_Post_Blend
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        A_Bow_Ready
	Interpolate	15
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd

## BOOSTUP ###########################################################

Move DualWield_BoostUp
	Requires	COMBAT WEAPON DUAL BOOSTUP
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_BoostUp_PostCycle
	Flags		NoSizeScale #ReqInputs

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

Move DualWield_BoostUp_PostCycle
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd

## BOOSTDOWN ###########################################################

Move DualWield_BoostDown
	Requires	COMBAT WEAPON DUAL BOOSTDOWN
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_BoostDown_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim		male\boost 1
	TEnd

	Type		huge
	  Anim		huge\boostdown 1
	TEnd

	Type		fem
	  Anim		fem\boostdown 1
	TEnd
MEnd

Move DualWield_BoostDown_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd

## BLOCKS ###########################################################

Move DualWield_Block_R
	Requires	COMBAT WEAPON BLOCK DUAL
	Priority	4
	Interrupts	<BlockIRQ> D_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove	DualWield_Ready
	Flags		PickRandomly

	Type		male
	  Anim		male\dual_dodge	1 49
	TEnd
MEnd

Move DualWield_Block_L
	Requires	COMBAT WEAPON BLOCK DUAL
	Priority	4
	Interrupts	<BlockIRQ> D_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove	DualWield_Ready

	Type		male
	  Anim		male\dual_dodge	52 104
	TEnd
MEnd

#############################################################

Move A_DualWield_Block_R
	Requires	FLY COMBAT WEAPON BLOCK	DUAL
	Priority	5
	Interrupts	<BlockIRQ> A_DualWield_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove	DualWield_FlyCycle

	Type		male
	  Anim		male\air_dual_dodge 1 49
	TEnd
MEnd

Move A_DualWield_Block_L
	Requires	FLY COMBAT WEAPON BLOCK	DUAL
	Priority	5
	Interrupts	<BlockIRQ> A_DualWield_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove	DualWield_FlyCycle

	Type		male
	  Anim		male\air_dual_dodge 52 104
	TEnd
MEnd

## HIT REACTIONS ###########################################################

Move DualWield_HitQuick
	Requires	COMBAT WEAPON QUICK HIT	DUAL
	Priority	5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Flags		HitReact

	Type		male
	  Anim		male\dual_HQ 1
	TEnd
MEnd

#############################################################

Move A_DualWield_HitQuick
	Requires	FLY COMBAT WEAPON QUICK	HIT DUAL
	Priority	7
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyCycle
	Flags		HitReact

	Type		male
	  Anim		male\air_dual_HQ 1
	TEnd
MEnd

#############################################################

#############################################################
##BRAWLING CUSTOM HIT REACTION
#############################################################

## Initial Strike Hit Quick #############################################

Move Dual_Initial_Strike_HQ
	Requires	BRAWLING WEAPON QUICK HIT DUAL
	Priority        5
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	NextMove        DualWield_Ready
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ_dualwield 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd
MEnd

Move Dual_C_Brawling_HitQuick
	Requires	COMBAT BRAWLING WEAPON QUICK HIT DUAL
	Priority        6
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	NextMove        DualWield_Ready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ_dualwield 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

## Spinning Strike Hit #############################################

Move Dual_Spinning_Strike_Hit
	Requires	BRAWLING HIT SPIN WEAPON DUAL
	Priority        5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        DualWield_Ready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_dualwield 1
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1
	TEnd

MEnd

Move Dual_C_Spinning_Strike_Hit
	Requires	COMBAT BRAWLING HIT SPIN WEAPON DUAL
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        DualWield_Ready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_dualwield 1
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1
	TEnd

MEnd

################################
######## Arm Guns SHOULDER_Blast ###
################################

Move ArmGun_ShoulderBlast
	Requires	COMBAT WEAPON DUAL SHOULDERBLAST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ArmGun_ShoulderBlast_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_shoulder_blast 1 90
	  PitchStart	11
	  PitchEnd	59
	TEnd
MEnd

Move ArmGun_ShoulderBlast_2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\armguns_shoulder_blast 90
	TEnd
MEnd

#############################################################

Move A_ArmGun_ShoulderBlast
	Requires	FLY COMBAT WEAPON DUAL SHOULDERBLAST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ArmGun_ShoulderBlast_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_armguns_shoulder_blast 1 90
	  PitchStart	11
	  PitchEnd	59
	TEnd
MEnd

Move A_ArmGun_ShoulderBlast_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_armguns_shoulder_blast 90
	TEnd
MEnd

################################
######## Arm Guns Dual Blast ###
################################

Move ArmGun_Blast
	Requires	COMBAT WEAPON DUAL ARMGUNBLAST
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ArmGun_Blast_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_blast 1 40
	  PitchEnd	30
	  PitchAngle	0.1
	TEnd

	Type		PPD_HardSuit_Boss
	  Anim          male\dual_guns_single_shot 1 35
	  PitchAngle	0.3
	TEnd
MEnd

Move ArmGun_Blast_2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\armguns_blast 40
	TEnd

	Type		PPD_HardSuit_Boss
	  Anim          male\dual_guns_single_shot 35
	  PitchAngle	0.3
	  PitchEnd	51
	TEnd
MEnd

#############################################################

Move A_ArmGun_Blast
	Requires	FLY COMBAT WEAPON DUAL ARMGUNBLAST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ArmGun_Blast_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_armguns_blast 1 40
	  PitchEnd	30
	  PitchAngle	0.1
	TEnd
MEnd

Move A_ArmGun_Blast_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_armguns_blast 40
	TEnd
MEnd

#################################
######### Arm Guns Dual Barrage ###
#################################
#
#Move ArmGun_DualBarrage
#	Requires	test4  ##COMBAT WEAPON DUAL ARMGUNBARRAGE
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        ArmGun_DualBarrage_2
#	Priority        5
#	Flags		CANTMOVE NoSizeScale
#
#	Type		male
#	  Anim          male\armguns_dualcone 1 80
#	TEnd
#MEnd
#
#Move ArmGun_DualBarrage_2
#	Requires	COMBAT WEAPON DUAL
#	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
#	NextMove        DualWield_Ready
#	Priority        4
##
#	Type		male
#	  Anim          male\armguns_dualcone 81
#	TEnd
#
#MEnd

########################
######## Arm Guns Cone ###
#######################

Move ArmGun_Cone
	Requires	COMBAT WEAPON DUAL ARMGUNCONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ArmGun_Cone_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_singlecone_quick 1 45
	  PitchAngle	0.1
	TEnd
MEnd

Move ArmGun_Cone_2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_singlecone_quick 45
	  PitchEnd	61
	  PitchAngle	0.1
	TEnd
MEnd

#############################################################

Move A_ArmGun_Cone
	Requires	FLY COMBAT WEAPON DUAL ARMGUNCONE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ArmGun_Cone_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_armguns_singlecone_quick 1 45
	  PitchAngle	0.1
	TEnd
MEnd

Move A_ArmGun_Cone_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_armguns_singlecone_quick 45
	  PitchEnd	61
	  PitchAngle	0.1
	TEnd
MEnd

########################
######## Arm Guns Heavy Burst ###
#######################

Move ArmGun_Heavy
	Requires	COMBAT WEAPON DUAL ARMGUNHEAVYBURST
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ArmGun_Heavy_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_heavy 1 80
	TEnd
MEnd

Move ArmGun_Heavy_2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_heavy 80
	  PitchEnd	85
	TEnd
MEnd

#############################################################

Move A_ArmGun_Heavy
	Requires	FLY COMBAT WEAPON DUAL ARMGUNHEAVYBURST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ArmGun_Heavy_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_armguns_heavy 1 80
	TEnd
MEnd

Move A_ArmGun_Heavy_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_armguns_heavy 80
	  PitchEnd	85
	TEnd
MEnd

########################
######## Arm Guns Burst ###
#######################

Move ArmGun_burst
	Requires	COMBAT WEAPON DUAL ARMGUNBURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ArmGun_burst_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_burst 1 50
	  PitchAngle	0.3
	TEnd
MEnd

Move ArmGun_burst_2
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\armguns_burst 50
	  PitchEnd	55
	  PitchAngle	0.3
	TEnd
MEnd

#############################################################

Move A_ArmGun_burst
	Requires	FLY COMBAT WEAPON DUAL ARMGUNBURST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ArmGun_burst_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_armguns_burst 1 50
	  PitchAngle	0.3
	TEnd
MEnd

Move A_ArmGun_burst_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	DualWield_FlyReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_armguns_burst 50
	  PitchEnd	55
	  PitchAngle	0.3
	TEnd
MEnd

## INCLUDES ###########################################################

Include sequencers\player\Weapons\Weapons_DualWield_Blades.inc
Include sequencers\player\Weapons\Weapons_DualWield_Blades_a.inc
Include sequencers\player\Weapons\Weapons_DualWield_Pistols.inc
Include sequencers\player\Weapons\Weapons_DualWield_Claws.inc
Include sequencers\player\Weapons\Weapons_DualWield_Claws_a.inc
Include sequencers\player\Weapons\LeftHand\Weapons_DualWield_Archery.inc
Include sequencers\player\Weapons\LeftHand\Weapons_DualWield_Archery_a.inc
Include sequencers\player\Weapons\LeftHand\Weapons_DualWield_Katana.inc
Include sequencers\player\Weapons\LeftHand\Weapons_DualWield_Katana_a.inc