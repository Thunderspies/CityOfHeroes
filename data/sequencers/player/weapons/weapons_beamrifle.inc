#############################################################
## Weapons_BeamRifle.inc
#############################################################

Move Draw_BeamRifle
	Requires	COMBAT WEAPON BEAMRIFLE TWOHAND GUN SPAWNBACK
	Priority        56
	Include sequencers\Members\Combat_Weapon_TwoHand.inc  #This is commented out because designers complained that players could draw and fire their power while running
	NextMove        Draw_BeamRifle_Post
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\beamrifle_draw 1 29 # i extended the draw so that even if the player tries to run off, we force him to draw the weapon
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 29
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 29
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move Draw_BeamRifle_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\beamrifle_draw 30
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 30
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 3
	TEnd


MEnd

#############################################################

Move A_Draw_BeamRifle
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWOHAND GUN SPAWNBACK
	Priority	67
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_Draw_BeamRifle_Post

	Type		male
	  Anim          male\air_beamrifle_draw 1 2
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 2
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 2
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd
MEnd

Move A_Draw_BeamRifle_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_beamrifle_draw 2
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 2
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 2
	TEnd
MEnd

#############################################################

Move BeamRifle_Ready
	Requires	COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        PostBeamRifle

	Type		male
	  Anim          male\beamrifle_ready 1 41
	TEnd

	Type		huge
	  Anim          huge\beamrifle_ready 1 41
	TEnd

	Type		fem
	  Anim          fem\beamrifle_ready 1 41
	TEnd

MEnd

Move PostBeamRifle
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		FixArmReg

	Type		male
	  Anim          male\post_shotgun 1
	TEnd

	Type		huge
	  Anim          huge\postback 3 40
	TEnd

	Type		fem
	  Anim          fem\post_ready 1
	TEnd
MEnd

#############################################################

Move A_BeamRifle_Ready
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	Flags		Cycle FixArmReg
	NextMove        W_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_beamrifle_ready 1 41
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_ready 1 41
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_ready 1 41
	TEnd
MEnd


##############################################################
### Beam Rifle Power Attacks
##############################################################

## GUN BRAWL ###########################################################

Move BeamRifle_GunBrawl_Draw
	Requires	COMBAT WEAPON GUN BEAMRIFLE TWOHAND PUNCH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_GunPunch
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_GunPunch
	Requires	COMBAT WEAPON GUN BEAMRIFLE TWOHAND PUNCH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_GunPunch2
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\beamrifle_punch 1 15
	TEnd

	Type		huge
	  Anim          huge\beamrifle_punch 1 15
	TEnd

	Type		fem
	  Anim          fem\beamrifle_punch 1 15
	TEnd
MEnd

Move BeamRifle_GunPunch2
	Requires	COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        BeamRifle_Ready
	Flags		FixArmReg NoSizeScale ReqInputs

	Type		male
	  Anim          male\beamrifle_punch 15
	TEnd

	Type		huge
	  Anim          huge\beamrifle_punch 15
	TEnd

	Type		fem
	  Anim          fem\beamrifle_punch 15
	TEnd
MEnd

####Single Shot############################

Move BeamRifle_SingleShot_Draw
	Requires	COMBAT WEAPON BEAMRIFLE QUICK SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_SingleShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_SingleShot
	Requires	COMBAT WEAPON BEAMRIFLE QUICK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_SingleShot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg PitchToTarget
	#Scale		1.633

	Type		male
	  Anim          male\beamrifle_singleshot 1 35
	TEnd

	Type		huge
	  Anim          huge\beamrifle_singleshot 1 35
	TEnd

	Type		fem
	  Anim          fem\beamrifle_singleshot 1 35
	TEnd

MEnd

Move BeamRifle_SingleShot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE FixArmReg ReqInputs
	Scale		1.633

	Type		male
	  Anim          male\beamrifle_singleshot 36
	TEnd

	Type		huge
	  Anim          huge\beamrifle_singleshot 36
	TEnd

	Type		fem
	  Anim          fem\beamrifle_singleshot 36
	TEnd

MEnd

#############################################################

Move A_BeamRifle_SingleShot_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE LOW SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_SingleShot
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

MEnd

Move A_BeamRifle_SingleShot
	Requires	FLY COMBAT WEAPON BEAMRIFLE LOW
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_SingleShot_Post
	#Scale		1.633
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_singleshot 1 35
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_singleshot 1 35
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_singleshot 1 35
	TEnd
MEnd

Move A_BeamRifle_SingleShot_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale		1.633

	Type		male
	  Anim          male\air_beamrifle_singleshot 36
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_singleshot 36
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_singleshot 36
	TEnd
MEnd

#### Charged Shot ###########################

Move BeamRifle_ChargedShot_Draw
	Requires	COMBAT WEAPON BEAMRIFLE HIGH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_ChargedShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_ChargedShot
	Requires	COMBAT WEAPON BEAMRIFLE HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_ChargedShot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg PitchToTarget
	scale		0.95

	Type		male
	  Anim          male\beamrifle_chargedshot 1 27
	TEnd


	Type		huge
	  Anim          huge\beamrifle_chargedshot 1 27
	TEnd

	Type		fem
	  Anim          fem\beamrifle_chargedshot 1 27
	TEnd
MEnd

Move BeamRifle_ChargedShot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE FixArmReg ReqInputs
	scale		0.95

	Type		male
	  Anim          male\beamrifle_chargedshot 28
	TEnd

	Type		huge
	  Anim          huge\beamrifle_chargedshot 28
	TEnd

	Type		fem
	  Anim          fem\beamrifle_chargedshot 28
	TEnd

MEnd

#############################################################

Move A_BeamRifle_ChargedShot_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE HIGH SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_ChargedShot
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

MEnd

Move A_BeamRifle_ChargedShot
	Requires	FLY COMBAT WEAPON BEAMRIFLE HIGH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_ChargedShot_Post
	scale		0.95
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_chargedshot 1 27
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_chargedshot 1 27
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_chargedshot 1 27
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_BeamRifle_ChargedShot_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs
	scale		0.95

	Type		male
	  Anim          male\air_beamrifle_chargedshot 28
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_chargedshot 28
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_chargedshot 28
	TEnd
MEnd

#### Cutting Beam############################

Move BeamRifle_CuttingBeam_Draw
	Requires	COMBAT WEAPON BEAMRIFLE MID SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_CuttingBeam
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg PitchToTarget

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd
MEnd

Move BeamRifle_CuttingBeam
	Requires	COMBAT WEAPON BEAMRIFLE MID
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_CuttingBeam_Attack
	Sets            CANTMOVE
	Scale		1.65
	Flags		NOSIZESCALE FixArmReg PitchToTarget

	Type		male
	  Anim          male\beamrifle_cuttingbeam 1 28
	TEnd

	Type		huge
	  Anim          huge\beamrifle_cuttingbeam 1 28
	TEnd


	Type		fem
	  Anim          fem\beamrifle_cuttingbeam 1 28
	TEnd


MEnd

Move BeamRifle_CuttingBeam_Attack
	Requires	COMBAT WEAPON BEAMRIFLE MID
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_CuttingBeam_Post
	Scale		1.65
	Sets		CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_cuttingbeam 28 73
	TEnd

	Type		huge
	  Anim          huge\beamrifle_cuttingbeam 28 73
	TEnd

	Type		fem
	  Anim          fem\beamrifle_cuttingbeam 28 73
	TEnd

MEnd

Move BeamRifle_CuttingBeam_Post
	Requires	COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Scale		1.65
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\beamrifle_cuttingbeam 73
	TEnd

	Type		huge
	  Anim          huge\beamrifle_cuttingbeam 73
	TEnd

	Type		fem
	  Anim          fem\beamrifle_cuttingbeam 73
	TEnd

MEnd

#############################################################

Move A_BeamRifle_CuttingBeam_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE MID SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_CuttingBeam
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

MEnd

Move A_BeamRifle_CuttingBeam
	Requires	FLY COMBAT WEAPON BEAMRIFLE MID
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_CuttingBeam_Attack
	Sets            CANTMOVE
	Scale		1.65
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_cuttingbeam 1 28
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_cuttingbeam 1 28
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_cuttingbeam 1 28
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_BeamRifle_CuttingBeam_Attack
	Requires	FLY COMBAT WEAPON BEAMRIFLE MID
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_CuttingBeam_Post
	Flags		FixArmReg NOSIZESCALE PitchToTarget
	Scale		1.65
	Sets		CANTMOVE

	Type		male
	  Anim          male\air_beamrifle_cuttingbeam 28 73
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_cuttingbeam 28 73
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_cuttingbeam 28 73
	TEnd
MEnd


Move A_BeamRifle_CuttingBeam_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Scale		1.65
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_beamrifle_cuttingbeam 73
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_cuttingbeam 73
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_cuttingbeam 73
	TEnd
MEnd


#############################################################
## BeamRifle_LancerShot
#############################################################

Move BeamRifle_LancerShot_Draw
	Requires	COMBAT WEAPON BEAMRIFLE ONE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_LancerShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_LancerShot
	Requires	COMBAT WEAPON BEAMRIFLE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_LancerShot_Post
	Sets            CANTMOVE
	Scale		1.95
	Flags		NOSIZESCALE FixArmReg PitchToTarget
	Interpolate	8

	Type		male
	  Anim          male\beamrifle_lancershot 1 60
	TEnd

	Type		huge
	  Anim          huge\beamrifle_lancershot 1 60
	TEnd

	Type		fem
	  Anim          fem\beamrifle_lancershot 1 60
	TEnd

MEnd

Move BeamRifle_LancerShot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Scale		1.95
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\beamrifle_lancershot 61
	TEnd


	Type		huge
	  Anim          huge\beamrifle_lancershot 61
	TEnd

	Type		fem
	  Anim          fem\beamrifle_lancershot 61
	TEnd
MEnd

#############################################################

Move A_BeamRifle_LancerShot_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE ONE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_LancerShot
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BeamRifle_LancerShot
	Requires	FLY COMBAT WEAPON BEAMRIFLE ONE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_LancerShot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE
	Scale		1.95

	Type		male
	  Anim          male\air_beamrifle_lancershot 1 60
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_lancershot 1 60
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_lancershot 1 60
	TEnd
MEnd

Move A_BeamRifle_LancerShot_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale		1.95

	Type		male
	  Anim          male\air_beamrifle_lancershot 61
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_lancershot 61
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_lancershot 61
	TEnd
MEnd


## BeamRifle_PenetratingRay ################################################

Move BeamRifle_PenetratingRay_Windup_Draw
	Requires	COMBAT WEAPON BEAMRIFLE TWOHAND GUN SNIPERWINDUP SPAWNBACK
	Priority        60
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Windups.inc
	NextMove        BeamRifle_PenetratingRay_SniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_PenetratingRay_SniperWindup
	Requires	COMBAT WEAPON BEAMRIFLE TWOHAND GUN SNIPERWINDUP
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg
	CycleMove	BeamRifle_PenetratingRay_SniperWindup_Cycle
	NextMove        BeamRifle_Ready

	Type		male
	  Anim          male\beamrifle_penetratingray_windup 1
	TEnd

	Type		huge
	  Anim          huge\beamrifle_penetratingray_windup 1
	TEnd

	Type		fem
	  Anim          fem\beamrifle_penetratingray_windup 1
	TEnd


MEnd

Move BeamRifle_PenetratingRay_SniperWindup_Cycle
	Requires	COMBAT WEAPON BEAMRIFLE SNIPERWINDUP
	Priority        4
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg
	CycleMove	BeamRifle_PenetratingRay
	NextMove        BeamRifle_Ready

	Type		male
	  Anim          male\beamrifle_penetratingray_windup 40 56
	TEnd

	Type		huge
	  Anim          huge\beamrifle_penetratingray_windup 40 56
	TEnd

	Type		fem
	  Anim          fem\beamrifle_penetratingray_windup 40 56
	TEnd

MEnd

## BeamRifle_PenetratingRay ################################################

#Move BeamRifle_PenetratingRay_Draw
#	Requires	COMBAT WEAPON BEAMRIFLE FIVE SPAWNBACK
#	Priority        55
#	Interrupts	<AttackIRQ> <Defiance>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        BeamRifle_PenetratingRay
#	Sets            CANTMOVE
#	Flags		PitchToTarget NOSIZESCALE FixArmReg
#
#	Type		male
#	  Anim          male\beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	TEnd
#
#MEnd

Move BeamRifle_PenetratingRay
	Requires	COMBAT WEAPON BEAMRIFLE FIVE
	Priority        6
	Interrupts	<AttackIRQ> BeamRifle_PenetratingRay_SniperWindup BeamRifle_PenetratingRay_SniperWindup_Cycle
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_PenetratingRay_Post
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim          male\beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

MEnd

Move BeamRifle_PenetratingRay_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\beamrifle_penetratingray 16
	TEnd

	Type		huge
	  Anim          huge\beamrifle_penetratingray 16
	TEnd


	Type		fem
	  Anim          fem\beamrifle_penetratingray 16
	TEnd


MEnd

######## PenetratingRay #####################################################

Move A_BeamRifle_PenetratingRay_Windup_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWOHAND GUN SNIPERWINDUP SPAWNBACK
	Priority	70
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_BeamRifle_PenetratingRay_SniperWindup
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

MEnd

Move A_BeamRifle_PenetratingRay_SniperWindup
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWOHAND GUN SNIPERWINDUP
	Priority	17
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle PitchToTarget FixArmReg NOSIZESCALE
	CycleMove	A_BeamRifle_PenetratingRay_SniperWindup_Cycle
	NextMove        A_BeamRifle_Ready

	Type		male
	  Anim          male\air_beamrifle_penetratingray_windup 1
	  PitchAngle	0.5
	TEnd


	Type		huge
	  Anim          huge\air_beamrifle_penetratingray_windup 1
	  PitchAngle	0.5
	TEnd


	Type		fem
	  Anim          fem\air_beamrifle_penetratingray_windup 1
	  PitchAngle	0.5
	TEnd
MEnd

Move A_BeamRifle_PenetratingRay_SniperWindup_Cycle
	Requires	FLY COMBAT WEAPON BEAMRIFLE SNIPERWINDUP
	Priority	15
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle PitchToTarget FixArmReg NOSIZESCALE
	CycleMove	A_BeamRifle_PenetratingRay
	NextMove        A_BeamRifle_Ready

	Type		male
	  Anim          male\air_beamrifle_penetratingray_windup 40 56
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_penetratingray_windup 40 56
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_penetratingray_windup 40 56
	  PitchAngle	0.5
	TEnd

MEnd

#########PenetratingRay ####################################################

#Move A_BeamRifle_PenetratingRay_Draw
#	Requires	FLY COMBAT WEAPON BEAMRIFLE FIVE SPAWNBACK
#	Priority	66
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_BeamRifle_PenetratingRay
#	Sets            CANTMOVE
#	Flags		PitchToTarget FixArmReg NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	  PitchAngle	0.3
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	  PitchAngle	0.3
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_beamrifle_draw 1 30
#	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
#	  PitchAngle	0.3
#	TEnd
#MEnd

Move A_BeamRifle_PenetratingRay
	Requires	FLY COMBAT WEAPON BEAMRIFLE FIVE
	Priority	17
	Interrupts	<AttackIRQ> A_BeamRifle_PenetratingRay_SniperWindup A_BeamRifle_PenetratingRay_SniperWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_PenetratingRay_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_penetratingray 1 16
	  PitchAngle	0.5
	TEnd

MEnd

Move A_BeamRifle_PenetratingRay_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_beamrifle_penetratingray 17
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_penetratingray 17
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_penetratingray 17
	TEnd

MEnd

##############################################################
### BeamRifle_Overcharge
##############################################################
#
#Move BeamRifle_Overcharge_Draw
#	Requires	COMBAT WEAPON BEAMRIFLE THREE SPAWNBACK
#	Priority        55
#	Interrupts	<AttackIRQ> <Defiance>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        BeamRifle_Overcharge
#	Sets            CANTMOVE
#	Flags		PitchToTarget NOSIZESCALE FixArmReg
#
#	Type		male
#	  Anim          male\beamrifle_draw 1 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\beamrifle_draw 1 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\beamrifle_draw 1 30
#	TEnd
#
#MEnd
#
#Move BeamRifle_Overcharge
#	Requires	COMBAT WEAPON BEAMRIFLE THREE
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        BeamRifle_Overcharge_Post
#	Sets            CANTMOVE
#	Scale		1.3
#	Flags		NOSIZESCALE FixArmReg
#	Interpolate	8
#
#	Type		male
#	  Anim          male\beamrifle_overcharge 1 90
#	TEnd
#
#	Type		huge
#	  Anim          huge\beamrifle_overcharge 1 90
#	TEnd
#
#	Type		fem
#	  Anim          fem\beamrifle_overcharge 1 90
#	TEnd
#
#MEnd
#
#Move BeamRifle_Overcharge_Post
#	Requires	COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc
#	NextMove        BeamRifle_Ready
#	Flags		NOSIZESCALE FixArmReg ReqInputs
#	Scale		1.3
#
#	Type		male
#	  Anim          male\beamrifle_overcharge 91
#	TEnd
#
#	Type		huge
#	  Anim          huge\beamrifle_overcharge 91
#	TEnd
#
#	Type		fem
#	  Anim          fem\beamrifle_overcharge 91
#	TEnd
#
#MEnd
#
##############################################################
#
#Move A_BeamRifle_Overcharge_Draw
#	Requires	FLY COMBAT WEAPON BEAMRIFLE THREE SPAWNBACK
#	Priority	66
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_BeamRifle_Overcharge
#	Sets            CANTMOVE
#	Flags		PitchToTarget FixArmReg NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_beamrifle_draw 1 30
#	  PitchAngle	0.3
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_beamrifle_draw 1 30
#	  PitchAngle	0.3
#	TEnd
#
#
#	Type		fem
#	  Anim          fem\air_beamrifle_draw 1 30
#	  PitchAngle	0.3
#	TEnd
#
#
#MEnd
#
#Move A_BeamRifle_Overcharge
#	Requires	FLY COMBAT WEAPON BEAMRIFLE THREE
#	Priority	15
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_BeamRifle_Overcharge_Post
#	Sets            CANTMOVE
#	Scale		1.3
#	Flags		PitchToTarget FixArmReg NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_beamrifle_overcharge 1 90
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_beamrifle_overcharge 1 90
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_beamrifle_overcharge 1 90
#	TEnd
#
#MEnd
#
#Move A_BeamRifle_Overcharge_Post
#	Requires	FLY COMBAT WEAPON BEAMRIFLE
#	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
#	NextMove        A_BeamRifle_Ready
#	Flags		FixArmReg NOSIZESCALE ReqInputs
#	Scale		1.3
#
#	Type		male
#	  Anim          male\air_beamrifle_overcharge 91
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_beamrifle_overcharge 91
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_beamrifle_overcharge 91
#	TEnd
#MEnd

#############################################################
## BeamRifle_Overcharge (New Overcharge)
#############################################################

Move BeamRifle_Overcharge_Draw
	Requires	COMBAT WEAPON BEAMRIFLE THREE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_Overcharge
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_Overcharge
	Requires	COMBAT WEAPON BEAMRIFLE THREE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_Overcharge_Blast
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg PitchToTarget
	Interpolate	8

	Type		male
	  Anim          male\beamrifle_disintegrate 1 63
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 1 63
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 1 63
	TEnd

MEnd

Move BeamRifle_Overcharge_Blast
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Overcharge_Post
	Flags		NOSIZESCALE FixArmReg ReqInputs PitchToTarget

	Type		male
	  Anim          male\beamrifle_disintegrate 64 85
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 64 85
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 64 85
	TEnd

MEnd


Move BeamRifle_Overcharge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\beamrifle_disintegrate 86
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 86
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 86
	TEnd

MEnd

#############################################################

Move A_BeamRifle_Overcharge_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE THREE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_Overcharge
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BeamRifle_Overcharge
	Requires	FLY COMBAT WEAPON BEAMRIFLE THREE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_Overcharge_Blast
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_disintegrate 1 63
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 1 63
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 1 63
	TEnd
MEnd

Move A_BeamRifle_Overcharge_Blast
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Overcharge_Post
	Flags		FixArmReg NOSIZESCALE ReqInputs PitchToTarget

	Type		male
	  Anim          male\air_beamrifle_disintegrate 64 85
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 64 85
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 64 85
	TEnd
MEnd

Move A_BeamRifle_Overcharge_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_beamrifle_disintegrate 86
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 86
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 86
	TEnd
MEnd

###Beam Rifle Aim ##########################

Move BeamRifle_Aim_Draw
	Requires	COMBAT WEAPON BEAMRIFLE BOOSTUP SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> #<Defiance>
	#Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Aim
	#Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE #FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_Aim
	Requires	COMBAT WEAPON BEAMRIFLE BOOSTUP
	Priority        4
	Interrupts	<AttackIRQ>
	#Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Aim_Post
	#Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\beamrifle_aim 1 100
	TEnd

	Type		huge
	  Anim          huge\beamrifle_aim 1 100
	TEnd

	Type		fem
	  Anim          fem\beamrifle_aim 1 100
	TEnd

MEnd

Move BeamRifle_Aim_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\beamrifle_aim 100
	TEnd

	Type		huge
	  Anim          huge\beamrifle_aim 100
	TEnd

	Type		fem
	  Anim          fem\beamrifle_aim 100
	TEnd

MEnd

#############################################################

Move A_BeamRifle_Aim_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE BOOSTUP SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	#Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Aim
	#Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

MEnd

Move A_BeamRifle_Aim
	Requires	FLY COMBAT WEAPON BEAMRIFLE BOOSTUP
	Priority	15
	Interrupts	<AttackIRQ>
	#Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Aim_Post
	#Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_aim 1 100
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_aim 1 100
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_aim 1 100
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_BeamRifle_Aim_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_beamrifle_aim 100
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_aim 100
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_aim 100
	TEnd
MEnd

#############################################################
## BeamRifle_Disintegrate
#############################################################

Move BeamRifle_Disintegrate_Draw
	Requires	COMBAT WEAPON BEAMRIFLE FOUR SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_Disintegrate
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_Disintegrate
	Requires	COMBAT WEAPON BEAMRIFLE FOUR
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_Disintegrate_Blast
	Sets            CANTMOVE
	Scale		1.8
	Flags		NOSIZESCALE FixArmReg PitchToTarget
	Interpolate	8

	Type		male
	  Anim          male\beamrifle_disintegrate 1 46
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 1 46
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 1 46
	TEnd

MEnd

Move BeamRifle_Disintegrate_Blast
	Requires	COMBAT WEAPON BEAMRIFLE FOUR
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_Disintegrate_Post
	Scale		1.8
	Flags		NOSIZESCALE FixArmReg PitchToTarget

	Type		male
	  Anim          male\beamrifle_disintegrate 46 79
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 46 79
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 46 79
	TEnd

MEnd


Move BeamRifle_Disintegrate_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Scale		1.8
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\beamrifle_disintegrate 80
	TEnd

	Type		huge
	  Anim          huge\beamrifle_disintegrate 80
	TEnd

	Type		fem
	  Anim          fem\beamrifle_disintegrate 80
	TEnd

MEnd

#############################################################

Move A_BeamRifle_Disintegrate_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE FOUR SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_Disintegrate
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BeamRifle_Disintegrate
	Requires	FLY COMBAT WEAPON BEAMRIFLE FOUR
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_Disintegrate_Blast
	Sets            CANTMOVE
	Scale		1.8
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_disintegrate 1 45
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 1 45
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 1 45
	TEnd
MEnd

Move A_BeamRifle_Disintegrate_Blast
	Requires	FLY COMBAT WEAPON BEAMRIFLE FOUR
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_Disintegrate_Post
	Flags		FixArmReg NOSIZESCALE PitchToTarget
	Scale		1.8

	Type		male
	  Anim          male\air_beamrifle_disintegrate 46 79
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 46 79
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 46 79
	TEnd
MEnd

Move A_BeamRifle_Disintegrate_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale		1.8

	Type		male
	  Anim          male\air_beamrifle_disintegrate 80
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_disintegrate 80
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_disintegrate 80
	TEnd
MEnd

##### Piercing Beam ########################################################

Move BeamRifle_PiercingBeam_Draw
	Requires	COMBAT WEAPON BEAMRIFLE TWO SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_PiercingBeam
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		huge
	  Anim          huge\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

	Type		fem
	  Anim          fem\beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	TEnd

MEnd

Move BeamRifle_PiercingBeam
	Requires	COMBAT WEAPON BEAMRIFLE TWO
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BeamRifle_PiercingBeam_Post
	Sets            CANTMOVE
	Scale		1.3
	Flags		NOSIZESCALE FixArmReg PitchToTarget

	Type		male
	  Anim          male\beamrifle_piercingbeam 1 63
	TEnd

	Type		huge
	  Anim          huge\beamrifle_piercingbeam 1 63
	TEnd

	Type		fem
	  Anim          fem\beamrifle_piercingbeam 1 63
	TEnd

MEnd

Move BeamRifle_PiercingBeam_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BeamRifle_Ready
	Flags		NOSIZESCALE FixArmReg ReqInputs
	Scale		1.3

	Type		male
	  Anim          male\beamrifle_piercingbeam 64
	TEnd

	Type		huge
	  Anim          huge\beamrifle_piercingbeam 64
	TEnd

	Type		fem
	  Anim          fem\beamrifle_piercingbeam 64
	TEnd

MEnd

#############################################################

Move A_BeamRifle_PiercingBeam_Draw
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWO SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_PiercingBeam
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd


	Type		huge
	  Anim          huge\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_draw 1 30
	  PlayFX	CustomizeablePowers\BeamRifle\Subtractive\BeamRifle_Draw.fx
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BeamRifle_PiercingBeam
	Requires	FLY COMBAT WEAPON BEAMRIFLE TWO
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BeamRifle_PiercingBeam_Post
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE
	Scale		1.3

	Type		male
	  Anim          male\air_beamrifle_piercingbeam 1 63
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_piercingbeam 1 63
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_piercingbeam 1 63
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_BeamRifle_PiercingBeam_Post
	Requires	FLY COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_BeamRifle_Ready
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale		1.3

	Type		male
	  Anim          male\air_beamrifle_piercingbeam 64
	TEnd

	Type		huge
	  Anim          huge\air_beamrifle_piercingbeam 64
	TEnd

	Type		fem
	  Anim          fem\air_beamrifle_piercingbeam 64
	TEnd
MEnd