#############################################################
## Weapons_TwoHanded.inc
#############################################################

## DRAW 2hlarge ###########################################################

Move Draw_2hlarge
	Requires	COMBAT WEAPON 2HLARGE SPAWNBACK
	Priority        54
	#Interrupts	<AttackIRQ> <ReactIRQ> #<Ready> <Combat>
	#Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Interrupts	<AttackIRQ> <Ready> <Combat> <TwoHandMove>
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc  This is commented out because designers complained that players could draw and fire their power while running
	Member		<DeathIRQ> <AttackIRQ> <TwoHandCombat> <HitIRQ> <ReactIRQ> <BlockIRQ> <IRQRest>
	NextMove        Draw_2hlarge_Post
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move Draw_2hlarge_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	COMBAT WEAPON 2HLARGE
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\2hlarge_draw_basic 2
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 2
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 2
	TEnd

MEnd

## 2hlarge READIES ###########################################################

Move 2hlargeReady_15
	Requires	COMBAT WEAPON 2HLARGE
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\2hlarge_ready 15 31
	TEnd

	Type		huge
	  Anim          huge\2hlarge_ready 15 31
	TEnd

	Type		fem
	  Anim          fem\2hlarge_ready 15 31
	TEnd

MEnd

Move 2hlargeReady
	Requires	COMBAT WEAPON 2HLARGE
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg ReqInputs
	NextMove        PostShotgun
	CycleMove	2hlargeReady
	Scale           0.50

	Type		male
	  Anim          male\2hlarge_ready 1 31
	TEnd

	Type		huge
	  Anim          huge\2hlarge_ready 1 31
	TEnd

	Type		fem
	  Anim          fem\2hlarge_ready 1 31
	TEnd

MEnd

#############################################################
## 2hlarge Hit Quick
#############################################################

Move 2hlarge_HitQuick
	Requires	COMBAT WEAPON 2HLARGE QUICK HIT
	Priority        5
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        2hlargeReady
	Flags		HitReact

	Type		male
	  Anim          male\2hlarge_HQ_High 1
	TEnd

	Type		huge
	  Anim          huge\2hlarge_HQ_High 1
	TEnd

	Type		fem
	  Anim          fem\2hlarge_HQ_High 1
	TEnd

MEnd

## 2hlarge BLOCK ###########################################################

Move 2hlarge_Block_R
	Requires	COMBAT WEAPON 2HLARGE BLOCK
	Interrupts	<BlockIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        2hlargeReady
	Priority        4
	Flags		FixArmReg PickRandomly

	Type		male
	  Anim          male\2hlarge_block 1 48
	TEnd

	Type		huge
	  Anim          huge\2hlarge_block 1 48
	TEnd

	Type		fem
	  Anim          fem\2hlarge_block 1 48
	TEnd

MEnd

Move 2hlarge_Block_L
	Requires	COMBAT WEAPON 2HLARGE BLOCK
	Interrupts	<BlockIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        2hlargeReady
	Priority        4
	Flags		FixArmReg

	Type		male
	  Anim          male\2hlarge_block 50 92
	TEnd

	Type		huge
	  Anim          huge\2hlarge_block 50 92
	TEnd

	Type		fem
	  Anim          fem\2hlarge_block 50 92
	TEnd

MEnd

Move 2hlarge_BlockCycle
	Requires	COMBAT WEAPON 2HLARGE
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
	Flags		Cycle FixArmReg
	NextMove        2hlargeReady
	Priority        3
	Scale           0.50

	Type		male
	  Anim          male\2hlarge_ready 1 31
	TEnd

	Type		huge
	  Anim          huge\2hlarge_ready 1 31
	TEnd

	Type		fem
	  Anim          fem\2hlarge_ready 1 31
	TEnd

MEnd

#############################################################
## 2hlarge knockback
#############################################################

Move 2hlargeKnockback
	Requires	HIT KNOCKBACK PLAYER 2HLARGE
	Priority        6
	Interrupts	<HitIRQ> <DeathIRQ>
	Member		<TwoHandKnockback>
	NextMove        2hlargeKnockback_Travel
	Sets            CANTMOVE
	Flags		NoSizeScale
	Interpolate     3

	Type		male
	  Anim          male\2hlarge_knockback_fierce 3 15
	TEnd
MEnd

Move 2hlargeKnockback_Travel
	Requires	AIR
	NextMove        2hlargeKnockback_Impact
	Member		<TwoHandKnockback>
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\2hlarge_knockback_fierce 14 15
	TEnd
MEnd

Move 2hlargeKnockback_Apex
	Requires	AIR APEX
	Interrupts	<TwoHandKnockback>
	NextMove        2hlargeKnockback_FallCycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hlarge_knockback_fierce 17 24
	TEnd
MEnd

Move 2hlargeKnockback_FallCycle
	Requires	AIR
	NextMove        2hlargeKnockback_Impact
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\2hlarge_knockback_fierce 23 24
	TEnd
MEnd

Move 2hlargeKnockback_Impact
	NextMove        2hlarge_Quick_getup_back
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hlarge_knockback_fierce 25
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd
MEnd

#############################################################
## 2hlarge Sweeps
#############################################################

Move  2hlarge_Sweep_low
	Requires	PLAYER SWEEP LOW HIT 2HLARGE
	Priority        4
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        2hlarge_Sweep_impact
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hlarge_sweep 3 31
	  PlayFx        FEET\MALEDEATH5.FX
	TEnd

MEnd

#############################################################

Move 2hlarge_Sweep_High
	Requires	PLAYER SWEEP HIGH HIT 2HLARGE
	Priority        4
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        2hlarge_Sweep_impact
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\\2hlarge_sweep_flip 2 28
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd

MEnd

#############################################################

Move 2hlarge_Sweep_impact
	Requires	2HLARGE
	Member		<DeathIRQ>
	NextMove        2hlarge_Quick_getup_back
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hlarge_knockback_fierce 26
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd

MEnd

#############################################################
## 2hlarge only fast get up
#############################################################

Move 2hlarge_Quick_getup_back
	NextMove        Quick_getup_back2
	Sets            CANTMOVE
	Interpolate     0
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hlarge_quick_getup 1 25
	  PlayFx        FEET\JUMP1.FX 3
	TEnd

MEnd

###########################################################
####### Titan Weapon Power Attacks #################
###########################################################

## Shatter_Armor_Slow ###########################################################

Move Shatter_Armor_Slow_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK SLASH ALTERNATE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shatter_Armor_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move Shatter_Armor_Slow_Pre
	Requires	COMBAT WEAPON 2hlarge ATTACK SLASH ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shatter_Armor_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_shatterarmor_slow 1 25
	TEnd

	Type		huge
	  Anim          huge\titanweapon_shatterarmor_slow 1 25
	TEnd

	Type		fem
	  Anim          fem\titanweapon_shatterarmor_slow 1 25
	TEnd

MEnd

Move Shatter_Armor_Slow
	Requires	COMBAT WEAPON 2hlarge ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shatter_Armor_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_shatterarmor_slow 26 72
	TEnd

	Type		huge
	  Anim          huge\titanweapon_shatterarmor_slow 26 72
	TEnd

	Type		fem
	  Anim          fem\titanweapon_shatterarmor_slow 26 72
	TEnd

MEnd

Move Shatter_Armor_Slow_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_shatterarmor_slow 73
	TEnd

	Type		huge
	  Anim          huge\titanweapon_shatterarmor_slow 73
	TEnd

	Type		fem
	  Anim          fem\titanweapon_shatterarmor_slow 73
	TEnd

MEnd

## Shatter_Armor_Fast ###########################################################

Move Shatter_Armor_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK SLASH QUICK
	Priority        56
	Interrupts	<AttackIRQ> <twohandcombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shatter_Armor_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\TitanWeapon_ShatterArmor_Fast 10 11
	TEnd

	Type		huge
	  Anim		huge\TitanWeapon_ShatterArmor_Fast 10 11
	TEnd

	Type		fem
	  Anim		fem\TitanWeapon_ShatterArmor_Fast 10 11
	TEnd
MEnd

Move Shatter_Armor_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK SLASH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shatter_Armor_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\TitanWeapon_ShatterArmor_Fast 12 51
	TEnd

	Type		huge
	  Anim          huge\TitanWeapon_ShatterArmor_Fast 12 51
	TEnd

	Type		fem
	  Anim          fem\TitanWeapon_ShatterArmor_Fast 12 51
	TEnd
MEnd

Move Shatter_Armor_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\TitanWeapon_ShatterArmor_Fast 52
	TEnd

	Type		huge
	  Anim          huge\TitanWeapon_ShatterArmor_Fast 52
	TEnd

	Type		fem
	  Anim          fem\TitanWeapon_ShatterArmor_Fast 52
	TEnd
MEnd

# Crushing_Blow_Slow ###########################################################

Move Crushing_Blow_Slow_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK CHOP ALTERNATE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Crushing_Blow_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move Crushing_Blow_Slow_Pre
	Requires	COMBAT WEAPON 2hlarge ATTACK CHOP ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Crushing_Blow_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE #pitchtotarget

	Type		male
	  Anim          male\titanweapon_crushingblow_slow 1 25
	TEnd

	Type		huge
	  Anim          huge\titanweapon_crushingblow_slow 1 25
	TEnd

	Type		fem
	  Anim          fem\titanweapon_crushingblow_slow 1 25
	TEnd

MEnd

Move Crushing_Blow_Slow
	Requires	COMBAT WEAPON 2hlarge ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Crushing_Blow_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE #pitchtotarget

	Type		male
	  Anim          male\titanweapon_crushingblow_slow 26 72
	TEnd

	Type		huge
	  Anim          huge\titanweapon_crushingblow_slow 26 72
	TEnd

	Type		fem
	  Anim          fem\titanweapon_crushingblow_slow 26 72
	TEnd

MEnd

Move Crushing_Blow_Slow_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_crushingblow_slow 73
	TEnd

	Type		huge
	  Anim          huge\titanweapon_crushingblow_slow 73
	TEnd

	Type		fem
	  Anim          fem\titanweapon_crushingblow_slow 73
	TEnd

MEnd

## Crushing_Blow_Fast ###########################################################

Move Crushing_Blow_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK CHOP QUICK
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Crushing_Blow_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE #pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_crushingblow_fast 1 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_crushingblow_fast 1 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_crushingblow_fast 1 11
	TEnd
MEnd

Move Crushing_Blow_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK CHOP
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Crushing_Blow_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE #pitchtotarget

	Type		male
	  Anim          male\titanweapon_crushingblow_fast 12 44
	TEnd

	Type		huge
	  Anim          huge\titanweapon_crushingblow_fast 12 44
	TEnd

	Type		fem
	  Anim          fem\titanweapon_crushingblow_fast 12 44
	TEnd
MEnd

Move Crushing_Blow_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_crushingblow_fast 45
	TEnd

	Type		huge
	  Anim          huge\titanweapon_crushingblow_fast 45
	TEnd

	Type		fem
	  Anim          fem\titanweapon_crushingblow_fast 45
	TEnd
MEnd

# Defensive_Sweep_Slow ###########################################################

Move Defensive_Sweep_Slow_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK STAB ALTERNATE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Defensive_Sweep_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move Defensive_Sweep_Slow_Pre
	Requires	COMBAT WEAPON 2hlarge ATTACK STAB ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Defensive_Sweep_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_defensivesweep_slow 1 25
	TEnd

	Type		huge
	  Anim          huge\titanweapon_defensivesweep_slow 1 25
	TEnd

	Type		fem
	  Anim          fem\titanweapon_defensivesweep_slow 1 25
	TEnd

MEnd

Move Defensive_Sweep_Slow
	Requires	COMBAT WEAPON 2hlarge ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Defensive_Sweep_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_defensivesweep_slow 26 72
	TEnd

	Type		huge
	  Anim          huge\titanweapon_defensivesweep_slow 26 72
	TEnd

	Type		fem
	  Anim          fem\titanweapon_defensivesweep_slow 26 72
	TEnd

MEnd

Move Defensive_Sweep_Slow_Post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs

	Type		male
	  Anim          male\titanweapon_defensivesweep_slow 73
	TEnd

	Type		huge
	  Anim          huge\titanweapon_defensivesweep_slow 73
	TEnd

	Type		fem
	  Anim          fem\titanweapon_defensivesweep_slow 73
	TEnd

MEnd

## Defensive_Sweep_Fast ###########################################################

Move Defensive_Sweep_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK STAB QUICK
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Defensive_Sweep_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_defensivesweep_fast 10 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_defensivesweep_fast 10 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_defensivesweep_fast 10 11
	TEnd
MEnd

Move Defensive_Sweep_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK STAB
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Defensive_Sweep_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_defensivesweep_fast 12 38
	TEnd

	Type		huge
	  Anim          huge\titanweapon_defensivesweep_fast 12 38
	TEnd

	Type		fem
	  Anim          fem\titanweapon_defensivesweep_fast 12 38
	TEnd
MEnd

Move Defensive_Sweep_Fast_Post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs

	Type		male
	  Anim          male\titanweapon_defensivesweep_fast 39
	TEnd

	Type		huge
	  Anim          huge\titanweapon_defensivesweep_fast 39
	TEnd

	Type		fem
	  Anim          fem\titanweapon_defensivesweep_fast 39
	TEnd
MEnd

## Follow_Through_Fast ###########################################################

Move Follow_Through_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK UPPERCUT QUICK
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Follow_Through_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_follow_thru_fast 10 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_follow_thru_fast 10 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_follow_thru_fast 10 11
	TEnd
MEnd

Move Follow_Through_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK UPPERCUT
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Follow_Through_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_follow_thru_fast 12 30
	TEnd

	Type		huge
	  Anim          huge\titanweapon_follow_thru_fast 12 30
	TEnd

	Type		fem
	  Anim          fem\titanweapon_follow_thru_fast 12 30
	TEnd
MEnd

Move Follow_Through_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_follow_thru_fast 31
	TEnd

	Type		huge
	  Anim          huge\titanweapon_follow_thru_fast 31
	TEnd

	Type		fem
	  Anim          fem\titanweapon_follow_thru_fast 31
	TEnd
MEnd

## Whirling_Slice_Fast ###########################################################

Move Whirling_Slice_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK SMASH CLOSE
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Whirling_Slice_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_whirling_strike_fast 10 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_whirling_strike_fast 10 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_whirling_strike_fast 10 11
	TEnd
MEnd

Move Whirling_Slice_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK SMASH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whirling_Slice_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_whirling_strike_fast 12 44
	TEnd

	Type		huge
	  Anim          huge\titanweapon_whirling_strike_fast 12 44
	TEnd

	Type		fem
	  Anim          fem\titanweapon_whirling_strike_fast 12 44
	TEnd
MEnd

Move Whirling_Slice_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_whirling_strike_fast 45
	TEnd

	Type		huge
	  Anim          huge\titanweapon_whirling_strike_fast 45
	TEnd

	Type		fem
	  Anim          fem\titanweapon_whirling_strike_fast 45
	TEnd
MEnd

# Sweeping_Strike_Slow ###########################################################

Move Sweeping_Strike_Slow_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK SWIPE ALTERNATE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Sweeping_Strike_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 35
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 35
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 35
	TEnd

MEnd

Move Sweeping_Strike_Slow_Pre
	Requires	COMBAT WEAPON 2hlarge ATTACK SWIPE ALTERNATE
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Sweeping_Strike_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_sweeping_strike_slow 1 25
	TEnd

	Type		huge
	  Anim          huge\titanweapon_sweeping_strike_slow 1 25
	TEnd

	Type		fem
	  Anim          fem\titanweapon_sweeping_strike_slow 1 25
	TEnd

MEnd

Move Sweeping_Strike_Slow
	Requires	COMBAT WEAPON 2hlarge ATTACK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Sweeping_Strike_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_sweeping_strike_slow 26 72
	TEnd

	Type		huge
	  Anim          huge\titanweapon_sweeping_strike_slow 26 72
	TEnd

	Type		fem
	  Anim          fem\titanweapon_sweeping_strike_slow 26 72
	TEnd

MEnd

Move Sweeping_Strike_Slow_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_sweeping_strike_slow 73
	TEnd

	Type		huge
	  Anim          huge\titanweapon_sweeping_strike_slow 73
	TEnd

	Type		fem
	  Anim          fem\titanweapon_sweeping_strike_slow 73
	TEnd

MEnd

## Sweeping_Strike_Fast ###########################################################

Move Sweeping_Strike_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK PUNCH QUICK
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Sweeping_Strike_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_sweeping_strike_fast 10 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_sweeping_strike_fast 10 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_sweeping_strike_fast 10 11
	TEnd
MEnd

Move Sweeping_Strike_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK PUNCH
	Priority	5
	Interrupts	<AttackIRQ>  <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sweeping_Strike_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_sweeping_strike_fast 12 38
	TEnd

	Type		huge
	  Anim          huge\titanweapon_sweeping_strike_fast 12 38
	TEnd

	Type		fem
	  Anim          fem\titanweapon_sweeping_strike_fast 12 38
	TEnd
MEnd

Move Sweeping_Strike_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_sweeping_strike_fast 39
	TEnd

	Type		huge
	  Anim          huge\titanweapon_sweeping_strike_fast 39
	TEnd

	Type		fem
	  Anim          fem\titanweapon_sweeping_strike_fast 39
	TEnd
MEnd



# Arc_Of_Destruction_Slow ###########################################################

Move Arc_Of_Destruction_Slow_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK CAST ALTERNATE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Arc_Of_Destruction_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move Arc_Of_Destruction_Slow_Pre
	Requires	COMBAT WEAPON 2hlarge ATTACK CAST ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Arc_Of_Destruction_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_wave_of_destruction_slow 1 25
	TEnd

	Type		huge
	  Anim          huge\titanweapon_wave_of_destruction_slow 1 25
	TEnd

	Type		fem
	  Anim          fem\titanweapon_wave_of_destruction_slow 1 25
	TEnd

MEnd

Move Arc_Of_Destruction_Slow
	Requires	COMBAT WEAPON 2hlarge ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Arc_Of_Destruction_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\titanweapon_wave_of_destruction_slow 26 80
	TEnd

	Type		huge
	  Anim          huge\titanweapon_wave_of_destruction_slow 26 80
	TEnd

	Type		fem
	  Anim          fem\titanweapon_wave_of_destruction_slow 26 80
	TEnd

MEnd

Move Arc_Of_Destruction_Slow_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_wave_of_destruction_slow 81
	TEnd

	Type		huge
	  Anim          huge\titanweapon_wave_of_destruction_slow 81
	TEnd

	Type		fem
	  Anim          fem\titanweapon_wave_of_destruction_slow 81
	TEnd

MEnd

## Arc_Of_Destruction_Fast ###########################################################

Move Arc_Of_Destruction_Fast_Draw
	Requires	COMBAT WEAPON 2hlarge ATTACK CAST QUICK
	Priority        56
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Arc_Of_Destruction_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\titanweapon_wave_of_destruction_fast 10 11
	TEnd

	Type		huge
	  Anim		huge\titanweapon_wave_of_destruction_fast 10 11
	TEnd

	Type		fem
	  Anim		fem\titanweapon_wave_of_destruction_fast 10 11
	TEnd
MEnd

Move Arc_Of_Destruction_Fast
	Requires	COMBAT WEAPON 2hlarge ATTACK CAST
	Priority	5
	Interrupts	<AttackIRQ> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Arc_Of_Destruction_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE #pitchtotarget

	Type		male
	  Anim          male\titanweapon_wave_of_destruction_fast 12 54
	TEnd

	Type		huge
	  Anim          huge\titanweapon_wave_of_destruction_fast 12 54
	TEnd

	Type		fem
	  Anim          fem\titanweapon_wave_of_destruction_fast 12 54
	TEnd
MEnd

Move Arc_Of_Destruction_Fast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady

	Type		male
	  Anim          male\titanweapon_wave_of_destruction_fast 55
	TEnd

	Type		huge
	  Anim          huge\titanweapon_wave_of_destruction_fast 55
	TEnd

	Type		fem
	  Anim          fem\titanweapon_wave_of_destruction_fast 55
	TEnd
MEnd

################################
###### Two Handed Boostup  ######
################################

Move 2hlarge_Boostup_Draw
	Requires	COMBAT WEAPON 2hlarge WEPBOOSTUP HIGH SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move 2hlarge_Boostup
	Requires	COMBAT WEAPON 2hlarge WEPBOOSTUP HIGH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Boostup_Post
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\2hlarge_absorb 1 10
	TEnd

	Type		huge
	  Anim          huge\2hlarge_absorb 1 10
	TEnd

	Type		fem
	  Anim          fem\2hlarge_absorb 1 10
	TEnd
MEnd

Move 2hlarge_Boostup_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\2hlarge_absorb 11
	TEnd

	Type		huge
	  Anim          huge\2hlarge_absorb 11
	TEnd

	Type		fem
	  Anim          fem\2hlarge_absorb 11
	TEnd
MEnd


#################
## 2hlarge Taunt ##
#################

Move 2hlarge_Taunt_Draw
	Requires	COMBAT WEAPON 2hlarge TAUNT VARIATIONA SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move 2hlarge_Taunt
	Requires	COMBAT WEAPON 2hlarge TAUNT VARIATIONA
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Taunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\2hlarge_taunt 1 30
	TEnd

	Type		Huge
	  Anim          male\2hlarge_taunt 1 30
	TEnd

	Type		fem
	  Anim          male\2hlarge_taunt 1 30
	TEnd
MEnd

Move 2hlarge_Taunt_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		NOSIZESCALE FixArmReg


	Type		male
	  Anim          male\2hlarge_taunt 31
	TEnd

	Type		Huge
	  Anim          male\2hlarge_taunt 31
	TEnd

	Type		fem
	  Anim          male\2hlarge_taunt 31
	TEnd
MEnd

## PUMMEL ###########################################################

Move 2hlarge_brawl
	Requires	COMBAT WEAPON 2hlarge PUNCH BRAWL
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_brawl_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\2hlarge_brawl 1 15
	TEnd

	Type		fem
	  Anim          fem\2hlarge_brawl 1 15
	TEnd

	Type		huge
	  Anim          huge\2hlarge_brawl 1 15
	TEnd
MEnd

Move 2hlarge_brawl_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\2hlarge_brawl 16 63
	TEnd

	Type		fem
	  Anim          fem\2hlarge_brawl 16 63
	TEnd

	Type		huge
	  Anim          huge\2hlarge_brawl 16 63
	TEnd
MEnd

## DUAL THROWBRAWL ###########################################################

Move 2hlarge_ThrowBrawl
	Requires	WEAPON COMBAT 2hlarge THROWBRAWL
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\titanweapon_knife_throw 1 25
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\titanweapon_knife_throw 1 25
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\titanweapon_knife_throw 1 25
	  PitchAngle	0.5
	TEnd
MEnd

Move 2hlarge_ThrowBrawl_post
	Requires	WEAPON COMBAT 2hlarge
	Priority        3
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        2hlargeReady
	Flags		PitchToTarget

	Type		male
	  Anim          male\titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd


	Type		huge
	  Anim          huge\titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd

#################
## 2hlarge Cast ##
#################

Move 2hlarge_Cast_Draw
	Requires	COMBAT WEAPON 2hlarge CAST VARIATIONA SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Cast
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move 2hlarge_Cast
	Requires	COMBAT WEAPON 2hlarge CAST VARIATIONA
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Cast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\2hlarge_cast 1 80
	TEnd

	Type		huge
	  Anim          huge\2hlarge_cast 1 80
	TEnd

	Type		fem
	  Anim          fem\2hlarge_cast 1 80
	TEnd
MEnd

Move 2hlarge_Cast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\2hlarge_cast 81
	TEnd

	Type		huge
	  Anim          huge\2hlarge_cast 81
	TEnd

	Type		fem
	  Anim          fem\2hlarge_cast 81
	TEnd
MEnd

#################
## 2hlarge Headcast ##
#################

Move 2hlarge_Headcast_Draw
	Requires	COMBAT WEAPON 2hlarge HEADCAST VARIATIONA SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Headcast
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move 2hlarge_Headcast
	Requires	COMBAT WEAPON 2hlarge HEADCAST VARIATIONA
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Headcast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\2hlarge_eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\2hlarge_eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\2hlarge_eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move 2hlarge_Headcast_Post
	Requires	COMBAT WEAPON 2hlarge
	Priority        2
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\2hlarge_eye 51
	TEnd

	Type		huge
	  Anim          huge\2hlarge_eye 51
	TEnd

	Type		fem
	  Anim          fem\2hlarge_eye 51
	TEnd
MEnd

###################
## Danger Sense  ##
###################

Move 2hlarge_Danger_Sense
	Requires	COMBAT WEAPON 2hlarge KUNGFUAREA KUNGFU LOOKOUT OBSERVE
	Priority        7
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlarge_Danger_Sense_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move 2hlarge_Danger_Sense_cycle
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle NoSizeScale
	Scale		0.7

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd
#############################################################

Include sequencers\player\WEAPONS\Weapons_LongStaff.inc
