#############################################################
## Weapons_Shotgun.inc
#############################################################

## DRAW SHOTGUN ###########################################################

Move Draw_Shotgun
	Requires	COMBAT WEAPON GUN SPAWNBACK
	Priority        54
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        Draw_Shotgun_Post
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\draw_shotgun_basic 1 29 # i extended the draw so that even if the player tries to run off, we force him to draw the weapon
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_basic 1 29
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_basic 1 29
	TEnd
MEnd

Move Draw_Shotgun_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\draw_shotgun_basic 30
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_basic 30
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_basic 30
	TEnd
MEnd

#############################################################

Move A_Draw_Shotgun
	Requires	FLY COMBAT WEAPON GUN SPAWNBACK
	Priority	65
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_Draw_Shotgun_Post

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 29
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 29
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 29
	TEnd
MEnd

Move A_Draw_Shotgun_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_draw_shotgun_basic 30
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 30
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 30
	TEnd
MEnd


## SHOTGUN READIES ###########################################################

Move ShotgunReady_15
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\rocket_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\rocket_cycle 15 31
	TEnd
MEnd

Move ShotgunReady
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        PostShotgun
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\rocket_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\rocket_cycle 1 31
	TEnd
MEnd

#############################################################

#Move Ready_To_ShotgunReady
#	Requires	COMBAT WEAPON GUN
#	Priority        3
#	Interrupts	<Ready> <Combat> <WepCombat>
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc
#	NextMove        D_Shotgun_cycle
#	Flags		FixArmReg NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\draw_shotgun_basic 1
#	TEnd
#
#	Type		huge
#	  Anim          huge\draw_shotgun_basic 1
#	TEnd
#
#	Type		fem
#	  Anim          fem\draw_shotgun_basic 1
#	TEnd
#MEnd

#############################################################

Move D_Shotgun_cycle
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        PostShotgun
	Scale           0.50

	Type		male
	  Anim          male\shotgun_cycle 1
	TEnd

	Type		huge
	  Anim          huge\shotgun_cycle 1
	TEnd

	Type		fem
	  Anim          fem\shotgun_cycle 1
	TEnd
MEnd

#############################################################

Move A_ShotgunReady
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	Flags		Cycle
	NextMove        W_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_rocket_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_rocket_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\air_rocket_cycle 1 31
	TEnd
MEnd

#############################################################

Move A_ShotgunReady_15
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_rocket_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\air_rocket_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\air_rocket_cycle 15 31
	TEnd
MEnd

#############################################################

Move A_D_Shotgun_cycle
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	Flags		Cycle NOSIZESCALE
	NextMove        W_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_shotgun_cycle 1
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_cycle 1
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_cycle 1
	TEnd
MEnd

Move A_D_Shotgun_cycle_15
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove	A_D_Shotgun_cycle
	Flags		NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_shotgun_cycle 15
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_cycle 15
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_cycle 15
	TEnd
MEnd

#############################################################

Move PostShotgun
	Include sequencers\Members\Readies.inc
	NextMove        Ready

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

## ROCKET READY (Unused? Has same tags as ShotgunReady) ###########################################################

Move RocketReady
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\rocket_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\rocket_cycle 1 31
	TEnd
MEnd

Move RocketReady_15
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        RocketReady
	Flags		FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\rocket_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\rocket_cycle 15 31
	TEnd
MEnd


#############################################################
## JACKHAMMER
#############################################################

Move JackHammer_Draw
	Requires	COMBAT WEAPON GUN JACKHAMMER SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        JackHammer
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_basic 1 20
	TEnd
MEnd

Move JackHammer
	Requires	COMBAT WEAPON GUN JACKHAMMER
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        JackHammer_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\jackhammer_attack 1 40
	  PlayFX	fx\v_cov\weapons\jackhammersound.fx 0 Constant
	TEnd
MEnd

Move JackHammer_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\jackhammer_attack 41
	TEnd
MEnd

#############################################################
## SONIC GUN
#############################################################

Move SonicGun_Draw
	Requires	COMBAT WEAPON GUN SONICATTACK SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicGun
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\draw_sonicgun 1 90
	  PitchAngle	0.3
	TEnd
MEnd

Move SonicGun
	Requires	COMBAT WEAPON GUN SONICATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicGun_Post
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\shotgun_shoot 31 50
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move SonicGun_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\shotgun_shoot 51
	TEnd
MEnd

#############################################################

Move A_SonicGun_Stun_Draw
	Requires	FLY COMBAT WEAPON GUN SONICSTUN SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicGun_Stun
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_draw_sonicgun 1 90
	  PitchAngle	0.3
	TEnd
MEnd

Move A_SonicGun_Stun
	Requires	FLY COMBAT WEAPON GUN SONICSTUN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SonicGun_Stun_Post
	Sets            CANTMOVE
	Flags		PitchToTarget FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_shotgun_shoot 1 50
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_SonicGun_Stun_Post
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_shotgun_shoot 51
	TEnd
MEnd

#############################################################
## BUCKSHOT
#############################################################

Move BuckShot_Draw
	Requires	COMBAT WEAPON GUN LOW SHOTGUN SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BuckShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move BuckShot
	Requires	COMBAT WEAPON GUN LOW SHOTGUN
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BuckShot_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\shotgun_shoot 25 51
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 25 51
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\shotgun 25 51
	  PitchAngle	0.3
	TEnd
MEnd

Move BuckShot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\shotgun_shoot 52
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 52
	TEnd

	Type		fem
	  Anim          fem\shotgun 52
	TEnd
MEnd

#############################################################

Move A_BuckShot_Draw
	Requires	FLY COMBAT WEAPON GUN LOW SHOTGUN SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BuckShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BuckShot
	Requires	FLY COMBAT WEAPON GUN LOW SHOTGUN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BuckShot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_shotgun_shoot 24 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_shoot 24 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_shoot 24 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move A_BuckShot_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_shotgun_shoot 51
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_shoot 51
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_shoot 51
	TEnd
MEnd


#############################################################
## SLUG - Defiance version of 'buckshot'
#############################################################

Move Slug_Draw
	Requires	COMBAT WEAPON GUN SLOW SHOTGUN SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Slug
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move Slug
	#Requires	COMBAT WEAPON GUN
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Slug_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\shotgun_shoot 31 51
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 31 51
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\shotgun 31 51
	  PitchAngle	0.3
	TEnd
MEnd

Move 2nd_Slug
	Requires	COMBAT WEAPON GUN SLOW SHOTGUN
	Priority        5
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Slug_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\shotgun_shoot 1 51
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 1 51
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\shotgun 1 51
	  PitchAngle	0.3
	TEnd
MEnd

Move Slug_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\shotgun_shoot 51
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 51
	TEnd

	Type		fem
	  Anim          fem\shotgun 51
	TEnd
MEnd

#############################################################

Move A_Slug_Draw
	Requires	FLY COMBAT WEAPON GUN SLOW SHOTGUN SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BuckShot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Slug
	Requires	FLY COMBAT WEAPON GUN SLOW SHOTGUN
	Priority	16
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BuckShot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_shotgun_shoot 1 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_shoot 1 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_shoot 1 51
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move A_Slug_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_shotgun_shoot 52
	TEnd

	Type		huge
	  Anim          huge\air_shotgun_shoot 52
	TEnd

	Type		fem
	  Anim          fem\air_shotgun_shoot 52
	TEnd
MEnd

#############################################################
## BURST
#############################################################

Move Burst_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> #<Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Burst
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move Burst
	Requires	COMBAT WEAPON GUN MACHINEGUN
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Burst_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\machine_shoot 25 55
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\machine_shoot 25 55
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\machine 25 55
	  PitchAngle	0.3
	TEnd
MEnd

Move Burst_Post
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		FixArmReg NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\machine_shoot 56
	TEnd

	Type		huge
	  Anim          huge\machine_shoot 56
	TEnd

	Type		fem
	  Anim          fem\machine 56
	TEnd
MEnd

#############################################################

Move A_Burst_Draw
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Burst
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Burst
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN
	Priority	15
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Burst_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_machine_shoot 25 55
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		huge
	  Anim          huge\air_machine_shoot 25 55
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd

	Type		fem
	  Anim          fem\air_machine_shoot 25 55
	  PitchAngle	0.3
	  PitchEnd	48
	TEnd
MEnd

Move A_Burst_Post
	Requires	FLY COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_machine_shoot 56
	TEnd

	Type		huge
	  Anim          huge\air_machine_shoot 56
	TEnd

	Type		fem
	  Anim          fem\air_machine_shoot 56
	TEnd
MEnd

#############################################################
## ASSULTRIFLE HEAVYBURST
#############################################################

Move HeavyBurst_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN HIGH SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HeavyBurst
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move HeavyBurst
	Requires	COMBAT WEAPON GUN MACHINEGUN HIGH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HeavyBurst_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd
MEnd

Move HeavyBurst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd

	Type		fem
	  Anim          fem\heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd

	Type		huge
	  Anim          huge\heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd
MEnd

#############################################################

Move A_HeavyBurst_Draw
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN HIGH SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_HeavyBurst
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_HeavyBurst
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN HIGH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_HeavyBurst_Post
	Interpolate     5
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_heavy_machinegun 31 75
	  PitchAngle	0.3
	TEnd
MEnd

Move A_HeavyBurst_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\air_heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd

	Type		fem
	  Anim          fem\air_heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd

	Type		huge
	  Anim          huge\air_heavy_machinegun 76
	  PitchAngle	0.3
	  PitchEnd	78
	TEnd
MEnd

#############################################################
## FULL AUTO
#############################################################

Move FullAuto_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FullAuto
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move FullAuto
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Interpolate	5
	NextMove        FullAuto_Post
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	Sets            CANTMOVE

	Type		male
	  Anim          male\machinegun_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 31 151 #178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 31 151 #178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move FullAuto_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\machinegun_long 179
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 179
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 179
	TEnd
MEnd

#############################################################

Move A_FullAuto_Draw
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN FULLAUTO SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_FullAuto
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_FullAuto
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN FULLAUTO
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_FullAuto_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_machinegun_long 31 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 31 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 31 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move A_FullAuto_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_machinegun_long 179
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 179
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 179
	TEnd
MEnd

#############################################################
## FULL AUTO QUICK - Used by players
#############################################################

Move FullAutoQuick_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FullAutoQuick
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move FullAutoQuick
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Interpolate	5
	NextMove        FullAutoQuick_Post
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	Sets            CANTMOVE

	Type		male
	  Anim          male\machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move FullAutoQuick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\machinegun_long 179
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 179
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 179
	TEnd
MEnd

#############################################################

Move A_FullAutoQuick_Draw
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN FULLAUTO SPAWNBACK
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_FullAutoQuick
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_FullAutoQuick
	Requires	FLY COMBAT WEAPON GUN MACHINEGUN FULLAUTO
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_FullAutoQuick_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 31 151
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move A_FullAutoQuick_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_machinegun_long 179
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 179
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 179
	TEnd
MEnd

#############################################################
## FLAMETHROWER
#############################################################

Move FlameThrower_Draw
	Requires	COMBAT WEAPON GUN FLAMETHROWER SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FlameThrower
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move FlameThrower
	Requires	COMBAT WEAPON GUN FLAMETHROWER PROFILE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FlameThrower_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move FlameThrower_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\flamethrower_long 179
	TEnd

	Type		huge
	  Anim          huge\flamethrower_long 179
	TEnd

	Type		fem
	  Anim          fem\flamethrower_long 179
	TEnd
MEnd

#############################################################

Move A_Flamethrower_Draw
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Flamethrower
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Flamethrower
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Flamethrower_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move A_Flamethrower_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_flamethrower_long 179
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower_long 179
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower_long 179
	TEnd
MEnd

#############################################################
## FLAMETHROWER MID
#############################################################

Move FlameThrower_Mid_Draw
	Requires	COMBAT WEAPON GUN FLAMETHROWER MID SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FlameThrower_Mid
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move FlameThrower_Mid
	Requires	COMBAT WEAPON GUN FLAMETHROWER MID
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FlameThrower_Mid_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd
MEnd

Move FlameThrower_Mid_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd
MEnd

#############################################################

Move A_Flamethrower_Mid_Draw
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER MID SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Flamethrower_Mid
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Flamethrower_Mid
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER MID
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Flamethrower_Mid_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower_long 31 101
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Flamethrower_Mid_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower_long 163
	  PitchEnd	165
	  PitchAngle	0.3
	TEnd
MEnd

#############################################################
## IGNITE
#############################################################

Move Ignite_Draw
	Requires	COMBAT WEAPON GUN FLAMETHROWER QUICK SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ignite
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move Ignite
	Requires	COMBAT WEAPON GUN
	Priority        3
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ignite_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\flamethrower 31 70
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\flamethrower 31 70
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\flamethrower 31 70
	  PitchAngle	0.3
	TEnd
MEnd

Move 2nd_Ignite
	Requires	COMBAT WEAPON GUN FLAMETHROWER QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ignite_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\flamethrower 1 70
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\flamethrower 1 70
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\flamethrower 1 70
	  PitchAngle	0.3
	TEnd
MEnd

Move Ignite_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\flamethrower 71
	  PitchAngle	0.3
	  PitchEnd	76
	TEnd

	Type		huge
	  Anim          huge\flamethrower 71
	  PitchAngle	0.3
	  PitchEnd	76
	TEnd

	Type		fem
	  Anim          fem\flamethrower 71
	  PitchAngle	0.3
	  PitchEnd	76
	TEnd
MEnd

#############################################################

Move A_Ignite_Draw
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER QUICK SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Ignite
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 31
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Ignite
	Requires	FLY COMBAT WEAPON GUN
	Priority	14
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Ignite_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_flamethrower 31 70
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower 31 70
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower 31 70
	TEnd
MEnd

Move A_2nd_Ignite
	Requires	FLY COMBAT WEAPON GUN FLAMETHROWER QUICK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Ignite_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_flamethrower 1 70
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower 1 70
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower 1 70
	TEnd
MEnd

Move A_Ignite_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_D_Shotgun_cycle
	Flags		NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_flamethrower 71
	  PitchAngle	0.3
	  PitchEnd	76
	TEnd

	Type		huge
	  Anim          huge\air_flamethrower 71
	TEnd

	Type		fem
	  Anim          fem\air_flamethrower 71
	TEnd
MEnd

#############################################################
## BAYONET LUNGE
#############################################################

Move BayonetLunge_Draw
	Requires	COMBAT WEAPON GUN STAB SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        BayonetLunge
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\draw_shotgun_point 1 39
	TEnd
MEnd

Move BayonetLunge
	Requires	COMBAT WEAPON GUN STAB
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BayonetLunge_Post
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\bayonet_lunge 1 50
	TEnd
MEnd

Move BayonetLunge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady_15
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bayonet_lunge 51
	TEnd
MEnd

#############################################################

Move A_BayonetLunge_Draw
	Requires	FLY COMBAT WEAPON GUN STAB SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BayonetLunge
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 39
	  PitchAngle	0.3
	TEnd
MEnd

Move A_BayonetLunge
	Requires	FLY COMBAT WEAPON GUN STAB
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BayonetLunge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_bayonet_lunge 1 50
	TEnd
MEnd

Move A_BayonetLunge_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady_15
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bayonet_lunge 51
	TEnd
MEnd

#############################################################
## SNIPER
#############################################################

Move SniperWindup_Draw
	Requires	COMBAT WEAPON GUN SNIPERWINDUP SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Windups.inc
	NextMove        SniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\sniper_new 1 74
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		huge
	  Anim          huge\sniper_new 1 35
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		fem
	  Anim          fem\sniper_new 1 35
	  PitchAngle	0.5
	  PitchStart	22
	TEnd
MEnd

Move SniperWindup
	Requires	COMBAT WEAPON GUN SNIPERWINDUP
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg
	CycleMove	SniperWindup_Cycle
	NextMove        ShotgunReady

	Type		male
	  Anim          male\sniper_new 35 90
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\sniper_new 35 80
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\sniper_new 35 80
	  PitchAngle	0.5
	TEnd
MEnd

Move SniperWindup_Cycle
	Requires	COMBAT WEAPON GUN SNIPERWINDUP
	Priority        4
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg
	NextMove        SniperShoot

	Type		male
	  Anim          male\sniper_new 75 90
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\sniper_new 80 90
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\sniper_new 80 90
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move SniperShoot
	Requires	COMBAT WEAPON GUN SHOOT
	Priority        4
	Interrupts	<AttackIRQ> SniperWindup SniperWindup_Cycle
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        SniperShoot_Post
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim          male\sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\sniper_new 91 110
	  PitchAngle	0.5
	TEnd
MEnd

Move SniperShoot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\sniper_new 111
	TEnd

	Type		huge
	  Anim          huge\sniper_new 111
	TEnd

	Type		fem
	  Anim          fem\sniper_new 111
	TEnd
MEnd

#############################################################

#Move SniperShoot_Quick
#	Requires	COMBAT WEAPON GUN QUICK
#	Priority        4
#	Interrupts	<AttackIRQ> SniperWindup SniperWindup_Cycle <Defiance>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SniperShoot_Post
#	Flags		PitchToTarget NOSIZESCALE FixArmReg
#	Sets            CANTMOVE
#	#Interpolate	0
#
#	Type		male
#	  Anim          male\sniper_new 35 110
#	  PitchAngle	0.5
#	TEnd
#
#	Type		huge
#	  Anim          huge\sniper_new 35 110
#	  PitchAngle	0.5
#	TEnd
#
#	Type		fem
#	  Anim          fem\sniper_new 35 110
#	  PitchAngle	0.5
#	TEnd
#MEnd
#
#Move SniperShoot_Quick_Post
#	Requires	COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc
#	NextMove        ShotgunReady
#	Flags		FixArmReg NOSIZESCALE ReqInputs
#
#	Type		male
#	  Anim          male\sniper_new 111
#	TEnd
#
#	Type		huge
#	  Anim          huge\sniper_new 111
#	TEnd
#
#	Type		fem
#	  Anim          fem\sniper_new 111
#	TEnd
#MEnd

#############################################################

Move A_Sniper_Windup_Draw
	Requires	FLY COMBAT WEAPON GUN SNIPERWINDUP SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_Sniper_Windup
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_sniper_new 1 34
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 1 34
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 1 34
	  PitchAngle	0.5
	  PitchStart	22
	TEnd
MEnd

Move A_Sniper_Windup
	Requires	FLY COMBAT WEAPON GUN SNIPERWINDUP
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle PitchToTarget NOSIZESCALE
	CycleMove	A_Sniper_WindupCycle
	NextMove        A_ShotgunReady
	Scale		0.625

	Type		male
	  Anim          male\air_sniper_new 35 90
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 35 90
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 35 90
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Sniper_WindupCycle
	Requires	FLY COMBAT WEAPON GUN SNIPERWINDUP
	Priority	15
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle PitchToTarget NOSIZESCALE
	CycleMove	A_Sniper_Shoot
	NextMove        A_ShotgunReady

	Type		male
	  Anim          male\air_sniper_new 90 90
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 90 90
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 90 90
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Sniper_Shoot
	Requires	FLY COMBAT WEAPON GUN SHOOT
	Priority	15
	Interrupts	<AttackIRQ> A_Sniper_Windup A_Sniper_WindupCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sniper_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	Interpolate	0

	Type		male
	  Anim          male\air_sniper_new 90 110
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 90 110
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 90 110
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Sniper_Shoot_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_sniper_new 111
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 111
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 111
	TEnd
MEnd

################################
###### Quick Sniper Shoot    ######
###############################

Move SniperQuick_Draw
	Requires	COMBAT WEAPON GUN QUICK SHOOT SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance> <MoveIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        SniperQuick_Shoot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		huge
	  Anim          huge\sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		fem
	  Anim          fem\sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd
MEnd

Move SniperQuick
	Requires	COMBAT WEAPON GUN QUICK SHOOT
	Priority        5
	Interrupts	<AttackIRQ> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SniperQuick_Shoot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\sniper_new 35 80
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\sniper_new 35 80
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\sniper_new 35 80
	  PitchAngle	0.5
	TEnd
MEnd

Move SniperQuick_Shoot
	Requires	COMBAT WEAPON GUN
	Priority        3
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SniperQuick_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\sniper_new 91 110
	  PitchAngle	0.5
	TEnd
MEnd

Move SniperQuick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\sniper_new 111
	TEnd

	Type		huge
	  Anim          huge\sniper_new 111
	TEnd

	Type		fem
	  Anim          fem\sniper_new 111
	TEnd
MEnd

#############################################################

Move A_SniperQuick_Draw
	Requires	FLY COMBAT WEAPON GUN QUICK SHOOT SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ> <Defiance> <MoveIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_SniperQuick_Shoot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 1 80
	  PitchAngle	0.5
	  PitchStart	22
	TEnd
MEnd

Move A_SniperQuick
	Requires	FLY COMBAT WEAPON GUN QUICK SHOOT
	Priority	16
	Interrupts	<AttackIRQ> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SniperQuick_Shoot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_sniper_new 35 80
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 35 80
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 35 80
	  PitchAngle	0.5
	TEnd
MEnd

Move A_SniperQuick_Shoot
	Requires	FLY COMBAT WEAPON GUN
	Priority        14
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SniperQuick_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 91 110
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 91 110
	  PitchAngle	0.5
	TEnd
MEnd

Move A_SniperQuick_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_sniper_new 111
	TEnd

	Type		huge
	  Anim          huge\air_sniper_new 111
	TEnd

	Type		fem
	  Anim          fem\air_sniper_new 111
	TEnd
MEnd

## CONSTANT BEAM GUN ###########################################################

Move ConstantBeam_Draw
	Requires	COMBAT WEAPON GUN STRONG MACHINEGUN FIERCE HIGH SPAWNBACK
	Priority        58
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ConstantBeam_Pre
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 39
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 39
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 39
	  PitchAngle	0.3
	TEnd
MEnd

Move ConstantBeam_Pre
	Requires	COMBAT WEAPON GUN STRONG MACHINEGUN FIERCE HIGH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ConstantBeam
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\machinegun_long 31 161
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 31 161
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 31 161
	  PitchAngle	0.3
	TEnd
MEnd

Move ConstantBeam
	Requires	COMBAT WEAPON GUN
	Priority        3
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ConstantBeam_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\constantbeam 1 300
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\constantbeam 1 300
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\constantbeam 1 300
	  PitchAngle	0.3
	TEnd
MEnd

Move ConstantBeam_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\machinegun_long 162
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 162
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 162
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

#############################################################

Move A_ConstantBeam_Draw
	Requires	FLY COMBAT WEAPON GUN STRONG MACHINEGUN FIERCE HIGH SPAWNBACK
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget NOSIZESCALE
	NextMove        A_ConstantBeam_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 39
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_draw_shotgun_basic 1 39
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_draw_shotgun_basic 1 39
	  PitchAngle	0.3
	TEnd
MEnd

Move A_ConstantBeam_Pre
	Requires	FLY COMBAT WEAPON GUN STRONG MACHINEGUN FIERCE HIGH
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ConstantBeam
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_machinegun_long 31 161
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 31 161
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 31 161
	  PitchAngle	0.3
	TEnd
MEnd

Move A_ConstantBeam
	Requires	FLY COMBAT WEAPON GUN
	Priority	14
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ConstantBeam_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE


	Type		male
	  Anim          male\air_constantbeam 1 300
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_constantbeam 1 300
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_constantbeam 1 300
	  PitchAngle	0.3
	TEnd
MEnd

Move A_ConstantBeam_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_machinegun_long 162
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\air_machinegun_long 162
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\air_machinegun_long 162
	  PitchEnd	172
	TEnd
MEnd

## RIFLE BUTT ###########################################################

Move Rifle_Butt_Draw
	Requires	COMBAT WEAPON GUN SHOTGUN PUNCH SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        Rifle_Butt
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_basic 1 20
	TEnd
MEnd

Move Rifle_Butt
	Requires	COMBAT WEAPON GUN SHOTGUN PUNCH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rifle_Butt_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\rifel_butt 1 50
	TEnd
MEnd

Move Rifle_Butt_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\rifel_butt 51 103
	TEnd
MEnd

## AIR RIFLE BUTT ###########################################################

Move A_Rifle_Butt_Draw
	Requires	FLY COMBAT WEAPON GUN SHOTGUN PUNCH SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Rifle_Butt
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_draw_shotgun_basic 1 20
	TEnd
MEnd

Move A_Rifle_Butt
	Requires	FLY COMBAT WEAPON GUN SHOTGUN PUNCH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Rifle_Butt_Draw_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_rifel_butt 1 30
	TEnd
MEnd

Move A_Rifle_Butt_Draw_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_rifel_butt 31 103
	TEnd
MEnd

#############################################################
## ASSULTRIFLE HEAVYBURST
#############################################################

Move Rifle_Mystic_Burst_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN ALTERNATE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rifle_Mystic_Burst
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move Rifle_Mystic_Burst
	Requires	COMBAT WEAPON GUN MACHINEGUN ALTERNATE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rifle_Mystic_Burst_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\rifle_mystic_burst 1 25
	TEnd

	Type		fem
	  Anim          fem\rifle_mystic_burst 1 25
	TEnd

	Type		huge
	  Anim          male\rifle_mystic_burst 1 25
	TEnd
MEnd

Move Rifle_Mystic_Burst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ShotgunReady
	Scale		0.75
	Flags		NOSIZESCALE FixArmReg ReqInputs

	Type		male
	  Anim          male\rifle_mystic_burst 26
	TEnd

	Type		fem
	  Anim          fem\rifle_mystic_burst 26
	TEnd

	Type		huge
	  Anim          male\rifle_mystic_burst 26
	TEnd
MEnd
