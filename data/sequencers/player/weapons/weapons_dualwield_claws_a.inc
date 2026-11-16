#############################################################
## Weapons_DualWield_Claws_a.inc - Claw sequences (flying mode)
#############################################################

## AIR CLAW READY ###########################################################

Move A_ClawsReady
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle
	NextMove        DualWield_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 1
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 2 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 2 31
	TEnd
MEnd

Move A_ClawsReady_15
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady
	Flags		ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 15
	TEnd
MEnd

#############################################################

Move A_Claws_PostBlend
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		ReqInputs
	NextMove        A_ClawsReady
	Interpolate	15
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 1
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 2 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 2 31
	TEnd
MEnd

## AIR CLAW READY ###########################################################

Move A_LazyClawsReady
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle
	NextMove        DualWield_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 1
	TEnd
MEnd

Move A_LazyClawsReady_15
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_LazyClawsReady
	Flags		ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 15
	TEnd
MEnd

#############################################################

Move A_Claws_BrawlCycle_Half
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Scale           0.50
	Flags		NoSizeScale ReqInputs
	NextMove	A_Claws_BrawlCycle
	Interpolate	15

	Type		male
	  Anim          male\air_brawl_cycle 16 31
	TEnd

	Type		huge
	  Anim          huge\air_brawlcycle 16 31
	TEnd

	Type		fem
	  Anim          fem\air_brawlcycle 16 31
	TEnd
MEnd

Move A_Claws_BrawlCycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Scale           0.50
	Flags		CYCLE NoSizeScale
	NextMove	A_Claws_PostBlend

	Type		male
	  Anim          male\air_brawl_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_brawlcycle 1 31
	TEnd

	Type		femwwww
	  Anim          fem\air_brawlcycle 1 31
	TEnd
MEnd


## AIR CLAW DRAW ###########################################################

Move A_Claws_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_Draw_Cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Draw_Cycle
	Requires	FLY COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle
	NextMove        A_Claws_PostBlend

	Type		male
	  Anim          male\air_claw_out 20 40
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 20 40
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 20 40
	TEnd
MEnd

## CLAWS BRAWL - (Alternates between Dual Wield Kick Brawl and this) ###########################################################

Move A_Claws_BrawlRight
	Requires	FLY COMBAT WEAPON DUAL CLAW PUNCH BRAWL
	Priority	16 #Same Priority as DualWield Brawl move
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_BrawlRight_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_brawl_HIGH 1 25
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move A_Claws_BrawlRight_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_BrawlCycle_Half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_brawl_HIGH 26 48
	TEnd
MEnd

#############################################################

Move A_Claws_BrawlLeft
	Requires	FLY COMBAT WEAPON DUAL CLAW PUNCH BRAWL
	Priority	16 #Same Priority as DualWield Brawl move
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_BrawlLeft_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_brawl_HIGH 78 102
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move A_Claws_BrawlLeft_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_BrawlCycle_Half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_brawl_HIGH 103
	TEnd
MEnd


## CLAW ATTACK ###########################################################

Move A_ClawR_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawR
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_ClawR
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawR_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_mid 1 15
	TEnd

	Type		huge
	  Anim          huge\air_claw_mid 1 15
	TEnd

	Type		fem
	  Anim          fem\air_claw_mid 1 15
	TEnd
MEnd

Move A_ClawR_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claw_mid 16 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_mid 16 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_mid 16 31
	TEnd
MEnd


#############################################################

Move A_ClawL_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawL
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_ClawL
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawL_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_mid 1 15
	TEnd

	Type		huge
	  Anim          huge\air_claw_mid 1 15
	TEnd

	Type		fem
	  Anim          fem\air_claw_mid 1 15
	TEnd
MEnd

Move A_ClawL_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claw_mid 16 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_mid 16 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_mid 16 31
	TEnd
MEnd


## AIR CLAW LOW ###########################################################

Move A_Claws_Low_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW LOW ATTACK SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Low_Pre
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Low_Pre
	Requires	FLY COMBAT WEAPON DUAL CLAW LOW ATTACK
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Low
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_low 1 25
	TEnd
MEnd

Move A_Claws_Low
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_LazyClawsReady_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claw_low 25
	TEnd
MEnd

## AIR CLAW HIGH ###########################################################

Move A_Claws_High_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW HIGH ATTACK SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_High
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_High
	Requires	FLY COMBAT WEAPON DUAL CLAW HIGH ATTACK
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_High_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_upcut 1 26
	TEnd

	Type		huge
	  Anim          huge\air_claw_upcut 1 26
	TEnd

	Type		fem
	  Anim          fem\air_claw_upcut 1 26
	TEnd
MEnd

Move A_Claws_High_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs
	Scale           0.80

	Type		male
	  Anim          male\air_claw_upcut 27 65
	TEnd

	Type		huge
	  Anim          huge\air_claw_upcut 27 65
	TEnd

	Type		fem
	  Anim          fem\air_claw_upcut 27 65
	TEnd
MEnd


## AIR CLAWS 1-2 PUNCH ###########################################################

Move A_Claws_OneTwo2Punch_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSONETWO SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_OneTwo2Punch
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_OneTwo2Punch
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSONETWO
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_OneTwo2Punch_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_1_2punch 1 39
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd

	Type		huge
	  Anim          huge\air_1_2punch 1 39
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd

	Type		fem
	  Anim          fem\air_1_2punch 1 39
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX 14
	TEnd
MEnd

Move A_Claws_OneTwo2Punch_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_BrawlCycle_Half
	Scale           0.50
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_1_2punch 40
	TEnd

	Type		huge
	  Anim          huge\air_1_2punch 40
	TEnd

	Type		fem
	  Anim          fem\air_1_2punch 40
	TEnd
MEnd


########################
###### Air claw Gut Punch  ######
#######################

Move A_Claws_GutPunch_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSGUTPUNCH SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_GutPunch
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_GutPunch
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSGUTPUNCH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_GutPunch_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_gut_punch 1 31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd

	Type		huge
	  Anim          huge\air_gut_punch 1 31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd

	Type		fem
	  Anim          fem\air_gut_punch 1 31
	  PlayFx	POWERS\Claws\ClawsStrike.fx
	TEnd
MEnd

Move A_Claws_GutPunch_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_BrawlCycle_Half
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_gut_punch 32
	TEnd

	Type		huge
	  Anim          huge\air_gut_punch 32
	TEnd

	Type		fem
	  Anim          fem\air_gut_punch 32
	TEnd
MEnd


###################
###### Air claw speed  ######
###################

Move A_Claws_Speed_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSPEED SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Speed
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Speed
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSPEED
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Speed_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_speed 1 54
	TEnd

	Type		huge
	  Anim          huge\air_claw_speed 1 54
	TEnd

	Type		fem
	  Anim          fem\air_claw_speed 1 54
	TEnd
MEnd

Move A_Claws_Speed_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_claw_speed 55
	TEnd

	Type		huge
	  Anim          huge\air_claw_speed 55
	TEnd

	Type		fem
	  Anim          fem\air_claw_speed 55
	TEnd
MEnd


## AIR QUICK JUMPSPIN ###########################################################

Move A_Claws_Jumpspin_Quick
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWAREA QUICK
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Clawspin
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 1 32
	TEnd

	Type		huge
	  Anim          huge\air_claw_spin 1 32
	TEnd

	Type		fem
	  Anim          fem\air_claw_spin 1 32
	TEnd
MEnd

###################
###### Air claw spin  ###### Activate powers do not go into a particular mode
###################

Move A_Claws_Spin_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWAREA SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Spin_Pre
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Spin_Pre
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWAREA
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Spin
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 1 32
	TEnd

	Type		huge
	  Anim          huge\air_claw_spin 1 32
	TEnd

	Type		fem
	  Anim          fem\air_claw_spin 1 32
	TEnd
MEnd

Move A_Claws_Spin
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Spin_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 33 78
	TEnd

	Type		huge
	  Anim          huge\air_claw_spin 33 78
	  PlayFx        FEET\LAND1.FX  1
	TEnd

	Type		fem
	  Anim          fem\air_claw_spin 33 78
	  PlayFx        FEET\LAND1.FX  1
	TEnd
MEnd

Move A_Claws_Spin_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claw_spin 79
	TEnd

	Type		huge
	  Anim          huge\air_claw_spin 79
	TEnd

	Type		fem
	  Anim          fem\air_claw_spin 79
	TEnd
MEnd


###################
###### Air Claw Jump Area ######
###################

Move A_Claws_JumpArea_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW JUMPCLAW SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_JumpArea
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_JumpArea
	Requires	FLY COMBAT WEAPON DUAL CLAW JUMPCLAW
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_JumpArea_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_jump_area 1 70
	TEnd

	Type		huge
	  Anim          huge\air_claw_jump_area 1 70
	TEnd

	Type		fem
	  Anim          fem\air_claw_jump_area 1 70
	TEnd
MEnd

Move A_Claws_JumpArea_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_claw_jump_area 71
	TEnd

	Type		huge
	  Anim          huge\air_claw_jump_area 71
	TEnd

	Type		fem
	  Anim          fem\air_claw_jump_area 71
	TEnd
MEnd


###################
###### Air Claw Baseball punch ######
###################

Move A_Claws_BBpunch_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW BASEBALLPUNCH SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_BBpunch
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_BBpunch
	Requires	FLY COMBAT WEAPON DUAL CLAW BASEBALLPUNCH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_BBpunch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch 1 43
	TEnd

	Type		huge
	  Anim          huge\air_baseball_punch 1 43
	TEnd

	Type		fem
	  Anim          fem\air_baseball_punch 1 43
	TEnd
MEnd

Move A_Claws_BBpunch_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_baseball_punch 44
	TEnd

	Type		huge
	  Anim          huge\air_baseball_punch 44
	TEnd

	Type		fem
	  Anim          fem\air_baseball_punch 44
	TEnd
MEnd


##################
###### Air Claw Stab  ######
###################

Move A_ClawStab_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK POISON SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawStab
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_ClawStab
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK POISON
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawStab_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_poison_stab 1 25
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 1 25
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 1 25
	TEnd
MEnd

Move A_ClawStab_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_poison_stab 26 46
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 26 46
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 26 46
	TEnd
MEnd


###################
###### Air Claw Poison Mode (MR.Spikey)  ######
###################

Move A_Claws_Poison_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW POISON SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Poison
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Poison
	Requires	FLY COMBAT WEAPON DUAL CLAW POISON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Poison_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 1 88
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 1 88
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 1 88
	TEnd
MEnd

Move A_Claws_Poison_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_pre_poison 89
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 89
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 89
	TEnd
MEnd


##################################
###### Air Claw Boost_up (Just for buffs) ######
##################################

Move A_Claws_Boostup_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW BOOSTUP SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Boostup
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Boostup
	Requires	FLY COMBAT WEAPON DUAL CLAW BOOSTUP
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostup 1
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1
	TEnd
MEnd

################################
###### Air Claw Rapidblast  ######
################################

Move A_Claws_Rapidblast_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW RAPIDBLAST SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Rapidblast
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Rapidblast
	Requires	FLY COMBAT WEAPON DUAL CLAW RAPIDBLAST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Rapidblast_Post
	Sets            CANTMOVE
	Flags		PITCHTOTARGET NoSizeScale

	Type		male
	  Anim          male\air_claws_rapidblast 1 70
	TEnd
MEnd

Move A_Claws_Rapidblast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_LazyClawsReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claws_rapidblast 70
	TEnd
MEnd

###################
###### Air claw Throw Knife  ######
###################

Move A_Claws_ThrowKnife_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWKNIFETHROW SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_ThrowKnife
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_ThrowKnife
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWKNIFETHROW
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_ThrowKnife_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_knifethrow 1 23
	TEnd

	Type		huge
	  Anim          huge\air_knifethrow 1 23
	TEnd

	Type		fem
	  Anim          fem\air_knifethrow 1 23
	TEnd
MEnd

Move A_Claws_ThrowKnife_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_knifethrow 24
	TEnd

	Type		huge
	  Anim          huge\air_knifethrow 24
	TEnd

	Type		fem
	  Anim          fem\air_knifethrow 24
	TEnd
MEnd


####################################
###### Air Claw magic throw (frisbe)  ######
####################################

Move A_Claws_Frisbee_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW MAGICTHROW SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Frisbee
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Frisbee
	Requires	FLY COMBAT WEAPON DUAL CLAW MAGICTHROW
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Frisbee_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_frisbee 1 30
	TEnd

	Type		huge
	  Anim          huge\air_frisbe 1 30
	TEnd

	Type		fem
	  Anim          fem\air_frisbe 1 30
	TEnd
MEnd

Move A_Claws_Frisbee_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_frisbee 31 70
	TEnd

	Type		huge
	  Anim          huge\air_frisbe 31 70
	TEnd

	Type		fem
	  Anim          fem\air_frisbe 31 70
	TEnd
MEnd


###############################
###### Air Claws Backstab Windup  ###
###############################

Move A_ClawsBackstabWindup_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW WINDUPCLAWSBACKSTAB SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_ClawsBackstabWindup
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd
MEnd

Move A_ClawsBackstabWindup
	Requires	FLY COMBAT WEAPON DUAL CLAW WINDUPCLAWSBACKSTAB
	Priority	16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_ClawsBackstabWindup_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claws_backstab 1 69
	TEnd
MEnd

Move A_ClawsBackstabWindup_Cycle
	Requires	FLY CLAW WINDUPCLAWSBACKSTAB
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_claws_backstab 75 125
	TEnd
MEnd

#############################################################

Move A_ClawsBackstabWindup_Quick_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSBACKSTAB SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_ClawsBackstab
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd
MEnd

Move A_ClawsBackstab
	Requires	FLY COMBAT WEAPON DUAL CLAW CLAWSBACKSTAB
	Priority	16
	Interrupts	<AttackIRQ> A_ClawsBackstabWindup A_ClawsBackstabWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ClawsBackstab2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claws_backstab 130 161
	TEnd
MEnd

Move A_ClawsBackstab2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsBackstab_Postcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claws_backstab 162 231
	TEnd
MEnd

Move A_ClawsBackstab_Postcycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_claws_backstab 235 284
	TEnd
MEnd


##################
##### Air Claws_Placate ####
##################

Move A_Claws_Placate_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW PLACATE SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Placate
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Placate
	Requires	FLY COMBAT WEAPON DUAL CLAW PLACATE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Placate_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_placate 1 40
	TEnd
MEnd

Move A_Claws_Placate_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_placate 41
	TEnd
MEnd

#####################
## Air Claws Taunt ##
#####################

Move A_Claws_Taunt_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW TAUNT VARIATIONC SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Taunt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Taunt
	Requires	FLY COMBAT WEAPON DUAL CLAW TAUNT VARIATIONC
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Taunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claws_taunt 1 45
	TEnd
MEnd

Move A_Claws_Taunt_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_LazyClawsReady_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_claws_taunt 45 85
	TEnd
MEnd


## AIR CLAWS POISON STAB - RIGHT ###########################################################

Move A_Claws_PoisonStabRight_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK POISON SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PoisonStabRight
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_PoisonStabRight
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK POISON
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PoisonStabRight_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_poison_stab 1 22
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 1 22
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 1 22
	TEnd
MEnd

Move A_Claws_PoisonStabRight_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_poison_stab 23 46
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 23 46
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 23 46
	TEnd
MEnd

#############################################################

Move A_Claws_PoisonStabLeft
	Requires	FLY COMBAT WEAPON DUAL CLAW ATTACK POISON
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PoisonStabLeft_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_poison_stab 48 68
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 48 68
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 48 68
	TEnd
MEnd

Move A_Claws_PoisonStabLeft_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_poison_stab 69
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 69
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 69
	TEnd
MEnd


## AIR CLAWS WRIST SHOOT ###########################################################

Move A_Claws_WristShoot_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW CAST HAND CROSSBOW LOW SPAWNSIDE
	Priority	80
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_WristShoot
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_WristShoot
	Requires	FLY COMBAT WEAPON DUAL CLAW CAST HAND CROSSBOW LOW
	Priority	19
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_WristShoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_claw_wristblast 1 45
	  PitchStart	5
	TEnd
MEnd

Move A_Claws_WristShoot_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_LazyClawsReady
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  PitchEnd	30
	  Anim          male\air_claw_wristblast 46
	TEnd
MEnd

## CLAWS PSIBLAST ###########################################################

Move A_Claws_PsiBlast_Quick_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW PSIONICBLAST HIGH SPAWNSIDE
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PsiBlast_Quick
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_PsiBlast_Quick
	Requires	FLY COMBAT WEAPON DUAL CLAW PSIONICBLAST HIGH
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	scale		2.0

	Type		male
	  Anim          male\air_psionics 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move A_Claws_PsiBlast
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_PsiBlast_Post
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 96 121
	  PitchAngle	0.2
	TEnd
MEnd

Move A_Claws_PsiBlast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_ClawsReady_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_psionics 122
	TEnd

	Type		huge
	  Anim          huge\air_psionic 122
	TEnd

	Type		fem
	  Anim          fem\air_psionic 122
	TEnd
MEnd


#############################################################

Move A_Claws_Mouth_SlowCast_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW HEADCAST HIGH STRONG SPAWNSIDE
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Mouth_SlowCast
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_Mouth_SlowCast
	Requires	FLY COMBAT WEAPON DUAL CLAW HEADCAST HIGH STRONG
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_Mouth_SlowCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.6

	Type		male
	  Anim          male\air_mouthcast 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move A_Claws_Mouth_SlowCast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_mouthcast 50
	TEnd

	Type		huge
	  Anim          huge\air_mouth 50
	TEnd

	Type		fem
	  Anim          fem\air_mouth 50
	TEnd
MEnd


#############################################################

Move A_Claws_GrenadeLob_Draw
	Requires	FLY COMBAT WEAPON DUAL CLAW GRENADE SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_GrenadeLob
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_out 1 19
	TEnd

	Type		huge
	  Anim          huge\air_claw_out 1 19
	TEnd

	Type		fem
	  Anim          fem\air_claw_out 1 19
	TEnd
MEnd

Move A_Claws_GrenadeLob
	Requires	FLY COMBAT WEAPON DUAL CLAW GRENADE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Claws_GrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_grenade_lob 1 40
	TEnd

	Type		huge
	  Anim          huge\air_grenade_lob 1 40
	TEnd

	Type		fem
	  Anim          fem\air_grenade_lob 1 40
	TEnd
MEnd

Move A_Claws_GrenadeLob_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Claws_PostBlend
	Flags		ReqInputs

	Type		male
	  Anim          male\air_grenade_lob 41
	TEnd

	Type		huge
	  Anim          huge\air_grenade_lob 41
	TEnd

	Type		fem
	  Anim          fem\air_grenade_lob 41
	TEnd
MEnd