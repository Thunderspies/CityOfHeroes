#############################################################
## Weapons_TwoHanded.inc
#############################################################

## DRAW 2hlarge ###########################################################

Move A_Draw_2hlarge
	Requires	FLY COMBAT WEAPON 2HLARGE SPAWNBACK
	Priority        65
	#Interrupts	<AttackIRQ> <ReactIRQ> #A_Weapon_Draw #<FlyWepCombat> <flyCombat>
	Interrupts	<AttackIRQ> <Ready> <Combat> <TwoHandMove>
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc  This is commented out because designers complained that players could draw and fire their power while running
	Member		<DeathIRQ> <AttackIRQ> <TwoHandCombat> <HitIRQ> <ReactIRQ> <BlockIRQ> <IRQRest>
	NextMove        A_Draw_2hlarge_Post
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move A_Draw_2hlarge_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	FLY COMBAT WEAPON 2HLARGE
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_2hlarge_draw_basic 2
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 2
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 2
	TEnd

MEnd

## 2hlarge READIES ###########################################################

Move A_2hlargeReady_15
	Requires	FLY COMBAT WEAPON 2HLARGE
	#Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand> <WepMove> #<BlockIRQ> <IRQRest> <MoveIRQ> <IRQEmotesALL> <IRQEmotesDualhand> <IRQEncounters>
	NextMove        A_2hlargeReady
	Flags		FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_2hlarge_ready 15 31
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_fly_ready 15 31
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_ready 15 31
	TEnd

MEnd

Move A_2hlargeReady
	Requires	FLY COMBAT WEAPON 2HLARGE
	#Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand> <WepMove> #<BlockIRQ> <IRQRest> <MoveIRQ> <IRQEmotesALL> <IRQEmotesDualhand> <IRQEncounters>
	Flags		Cycle FixArmReg ReqInputs
	NextMove        W_PreFall
	CycleMove	A_2hlargeReady
	Scale           0.50

	Type		male
	  Anim          male\air_2hlarge_ready 1 31
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_fly_ready 1 31
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_ready 1 31
	TEnd

MEnd

#############################################################
## 2hlarge Hit Quick
#############################################################

Move A_2hlarge_HitQuick
	Requires	FLY COMBAT WEAPON QUICK HIT 2HLARGE
	Priority        6
	Interrupts	<HitIRQ> A_FearStun_cycle A_FearStun
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_2hlargeReady
	Flags		HitReact

	Type		male
	  Anim          male\air_2hlarge_HQ 1
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_HQ 1
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_HQ 1
	TEnd

MEnd

###########################################################


###########################################################
####### Titan Weapon Power Attacks #################
###########################################################

## Shatter_Armor_Slow ###########################################################

Move A_Shatter_Armor_Slow_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SLASH ALTERNATE SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shatter_Armor_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move A_Shatter_Armor_Slow_Pre
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SLASH ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shatter_Armor_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_shatterarmor_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_shatterarmor_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_shatterarmor_slow 1 25
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Shatter_Armor_Slow
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shatter_Armor_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_shatterarmor_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_shatterarmor_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_shatterarmor_slow 26 72
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Shatter_Armor_Slow_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_shatterarmor_slow 73
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_shatterarmor_slow 73
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_shatterarmor_slow 73
	TEnd

MEnd

## Shatter_Armor_Fast ###########################################################

Move A_Shatter_Armor_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SLASH QUICK
	Priority        67
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shatter_Armor_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_TitanWeapon_ShatterArmor_Fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_TitanWeapon_ShatterArmor_Fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_TitanWeapon_ShatterArmor_Fast 10 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Shatter_Armor_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SLASH
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shatter_Armor_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_TitanWeapon_ShatterArmor_Fast 12 51
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_TitanWeapon_ShatterArmor_Fast 12 51
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_TitanWeapon_ShatterArmor_Fast 12 51
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Shatter_Armor_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_TitanWeapon_ShatterArmor_Fast 52
	TEnd

	Type		huge
	  Anim          huge\air_TitanWeapon_ShatterArmor_Fast 52
	TEnd

	Type		fem
	  Anim          fem\air_TitanWeapon_ShatterArmor_Fast 52
	TEnd
MEnd

# Crushing_Blow_Slow ###########################################################

Move A_Crushing_Blow_Slow_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CHOP ALTERNATE SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Crushing_Blow_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move A_Crushing_Blow_Slow_Pre
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CHOP ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Crushing_Blow_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_crushingblow_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_crushingblow_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_crushingblow_slow 1 25
	  PitchAngle	0.3
	TEnd

MEnd


Move A_Crushing_Blow_Slow
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Crushing_Blow_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_crushingblow_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_crushingblow_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_crushingblow_slow 26 72
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Crushing_Blow_Slow_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_crushingblow_slow 73
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_crushingblow_slow 73
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_crushingblow_slow 73
	TEnd

MEnd

## Crushing_Blow_Fast ###########################################################

Move A_Crushing_Blow_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CHOP QUICK
	Priority        67
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Crushing_Blow_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_titanweapon_crushingblow_fast 1 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_titanweapon_crushingblow_fast 1 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_titanweapon_crushingblow_fast 1 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Crushing_Blow_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CHOP
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Crushing_Blow_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_crushingblow_fast 12 44
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_crushingblow_fast 12 44
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_crushingblow_fast 12 44
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Crushing_Blow_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_crushingblow_fast 45
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_crushingblow_fast 45
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_crushingblow_fast 45
	TEnd
MEnd

# Defensive_Sweep_Slow ###########################################################

Move A_Defensive_Sweep_Slow_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK STAB ALTERNATE SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Defensive_Sweep_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move A_Defensive_Sweep_Slow_Pre
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK STAB ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       A_Defensive_Sweep_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_defensivesweep_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_defensivesweep_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_defensivesweep_slow 1 25
	  PitchAngle	0.3
	TEnd

MEnd


Move A_Defensive_Sweep_Slow
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       A_Defensive_Sweep_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_defensivesweep_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_defensivesweep_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_defensivesweep_slow 26 72
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Defensive_Sweep_Slow_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_defensivesweep_slow 73
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_defensivesweep_slow 73
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_defensivesweep_slow 73
	TEnd

MEnd

## Defensive_Sweep_Fast ###########################################################

Move A_Defensive_Sweep_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK STAB QUICK
	Priority        67
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Defensive_Sweep_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_titanweapon_defensivesweep_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_titanweapon_defensivesweep_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_titanweapon_defensivesweep_fast 10 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Defensive_Sweep_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK STAB
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Defensive_Sweep_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_defensivesweep_fast 12 38
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_defensivesweep_fast 12 38
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_defensivesweep_fast 12 38
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Defensive_Sweep_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_defensivesweep_fast 39
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_defensivesweep_fast 39
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_defensivesweep_fast 39
	TEnd
MEnd

## Follow_Through_Fast ###########################################################

Move A_Follow_Through_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK UPPERCUT QUICK
	Priority        68
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Follow_Through_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_titanweapon_follow_thru_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_titanweapon_follow_thru_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_titanweapon_follow_thru_fast 10 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Follow_Through_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK UPPERCUT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Follow_Through_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_follow_thru_fast 12 30
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_follow_thru_fast 12 30
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_follow_thru_fast 12 30
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Follow_Through_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_follow_thru_fast 31
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_follow_thru_fast 31
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_follow_thru_fast 31
	TEnd
MEnd

## Whirling_Slice_Fast ###########################################################

Move A_Whirling_Slice_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SMASH CLOSE
	Priority        68
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Whirling_Slice_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_titanweapon_whirling_strike_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_titanweapon_whirling_strike_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_titanweapon_whirling_strike_fast 10 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Whirling_Slice_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SMASH
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whirling_Slice_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_whirling_strike_fast 12 44
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_whirling_strike_fast 12 44
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_whirling_strike_fast 12 44
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Whirling_Slice_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_whirling_strike_fast 45
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_whirling_strike_fast 45
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_whirling_strike_fast 45
	TEnd
MEnd

# Sweeping_Strike_Slow ###########################################################

Move A_Sweeping_Strike_Slow_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SWIPE ALTERNATE SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Sweeping_Strike_Slow_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 35
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

MEnd

Move A_Sweeping_Strike_Slow_Pre
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK SWIPE ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       A_Sweeping_Strike_Slow
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_sweeping_strike_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_sweeping_strike_slow 1 25
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_sweeping_strike_slow 1 25
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Sweeping_Strike_Slow
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       A_Sweeping_Strike_Slow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_sweeping_strike_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_sweeping_strike_slow 26 72
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_sweeping_strike_slow 26 72
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Sweeping_Strike_Slow_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_sweeping_strike_slow 73
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_sweeping_strike_slow 73
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_sweeping_strike_slow 73
	TEnd

MEnd

## Sweeping_Strike_Fast ###########################################################

Move A_Sweeping_Strike_Fast_Draw
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK PUNCH QUICK
	Priority        67
	Interrupts	<AttackIRQ> <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Sweeping_Strike_Fast
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget
	Interpolate	25

	Type		male
	  Anim		male\air_titanweapon_sweeping_strike_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_titanweapon_sweeping_strike_fast 10 11
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_titanweapon_sweeping_strike_fast 10 11
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Sweeping_Strike_Fast
	Requires	FLY COMBAT WEAPON 2hlarge ATTACK PUNCH
	Priority        16
	Interrupts	<AttackIRQ>  <TwoHandCombatAir>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sweeping_Strike_Fast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget

	Type		male
	  Anim          male\air_titanweapon_sweeping_strike_fast 12 38
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_sweeping_strike_fast 12 38
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_sweeping_strike_fast 12 38
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Sweeping_Strike_Fast_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady

	Type		male
	  Anim          male\air_titanweapon_sweeping_strike_fast 39
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_sweeping_strike_fast 39
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_sweeping_strike_fast 39
	TEnd
MEnd

## Arc_Of_Destruction_Slow ###########################################################
#
#Move A_Arc_Of_Destruction_Slow_Draw
#	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CAST ALTERNATE SPAWNBACK
#	Priority        68
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove	A_Arc_Of_Destruction_Slow_Pre
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_2hlarge_draw_basic 1 35
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_2hlarge_draw_basic 1 35
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_2hlarge_draw_basic 1 35
#	TEnd
#
#MEnd
#
#Move A_Arc_Of_Destruction_Slow_Pre
#	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CAST ALTERNATE
#	Priority        17
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove       A_Arc_Of_Destruction_Slow
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_titanweapon_wave_of_destruction_slow 1 25
#	TEnd
#
#MEnd
#
#Move A_Arc_Of_Destruction_Slow
#	Requires	FLY COMBAT WEAPON 2hlarge ATTACK
#	Priority        15
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove       A_Arc_Of_Destruction_Slow_Post
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_titanweapon_wave_of_destruction_slow 26 72
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_titanweapon_wave_of_destruction_slow 26 72
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_titanweapon_wave_of_destruction_slow 26 72
#	TEnd
#
#MEnd
#
#Move A_Arc_Of_Destruction_Slow_Post
#	Requires	FLY COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
#	NextMove        A_2hlargeReady
#	Flags		ReqInputs
#	#Scale		1.2
#
#	Type		male
#	  Anim          male\air_titanweapon_wave_of_destruction_slow 73
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_titanweapon_wave_of_destruction_slow 73
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_titanweapon_wave_of_destruction_slow 73
#	TEnd
#
#MEnd
#
### Arc_Of_Destruction_Fast ###########################################################
#
#Move A_Arc_Of_Destruction_Fast_Draw
#	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CAST QUICK
#	Priority        67
#	Interrupts	<AttackIRQ> <TwoHandCombatAir>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove	A_Arc_Of_Destruction_Fast
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#	Interpolate	25
#
#	Type		male
#	  Anim		male\air_titanweapon_wave_of_destruction_fast 10 11
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_titanweapon_wave_of_destruction_fast 10 11
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_titanweapon_wave_of_destruction_fast 10 11
#	TEnd
#MEnd
#
#Move A_Wave_Of_Destruction_Fast
#	Requires	FLY COMBAT WEAPON 2hlarge ATTACK CAST
#	Priority        16
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_Wave_Of_Destruction_Fast_Post
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_titanweapon_wave_of_destruction_fast 12 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_titanweapon_wave_of_destruction_fast 12 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_titanweapon_wave_of_destruction_fast 12 30
#	TEnd
#MEnd
#
#Move A_Wave_Of_Destruction_Fast_Post
#	Requires	FLY COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
#	NextMove        A_2hlargeReady
#	Flags		ReqInputs
#	#Scale		1.2
#
#	Type		male
#	  Anim          male\air_titanweapon_wave_of_destruction_fast 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_titanweapon_wave_of_destruction_fast 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_titanweapon_wave_of_destruction_fast 31
#	TEnd
#MEnd

################################
###### Two Handed Boostup  ######
################################

Move A_2hlarge_Boostup_Draw
	Requires	FLY COMBAT WEAPON 2hlarge WEPBOOSTUP HIGH SPAWNBACK
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move A_2hlarge_Boostup
	Requires	FLY COMBAT WEAPON 2hlarge WEPBOOSTUP HIGH
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_Boostup_Post
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_2hlarge_absorb 1 10
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_absorb 1 10
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_absorb 1 10
	TEnd
MEnd

Move A_2hlarge_Boostup_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_2hlarge_absorb 11
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_absorb 11
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_absorb 11
	TEnd
MEnd


#################
## 2hlarge Taunt ##
#################

Move A_2hlarge_Taunt_Draw
	Requires	FLY COMBAT WEAPON 2hlarge TAUNT VARIATIONA SPAWNBACK
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 30
	  PlayFX	CustomizeablePowers\TitanWeapon\DrawWeapon.fx
	TEnd
MEnd

Move A_2hlarge_Taunt
	Requires	FLY COMBAT WEAPON 2hlarge TAUNT VARIATIONA
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_Taunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\air_2hlarge_taunt 1 30
	TEnd

	Type		Huge
	  Anim          male\air_2hlarge_taunt 1 30
	TEnd

	Type		fem
	  Anim          male\air_2hlarge_taunt 1 30
	TEnd
MEnd

Move A_2hlarge_Taunt_Post
	Requires	FLY COMBAT WEAPON 2hlarge
	Priority        4
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		NOSIZESCALE FixArmReg


	Type		male
	  Anim          male\air_2hlarge_taunt 31
	TEnd

	Type		Huge
	  Anim          male\air_2hlarge_taunt 31
	TEnd

	Type		fem
	  Anim          male\air_2hlarge_taunt 31
	TEnd
MEnd

## AIR BRAWL ###########################################################

Move A_2hlarge_brawl
	Requires	COMBAT WEAPON 2hlarge PUNCH FLY BRAWL
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_brawl_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_2hlarge_brawl 1 15
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_brawl 1 15
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_brawl 1 15
	TEnd
MEnd

Move A_2hlarge_brawl_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_2hlarge_brawl 16 ##49
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_brawl 16 ##49
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_brawl 16 ##49
	TEnd
MEnd

#############################################################

Move A_2hlarge_ThrowBrawl
	Requires	FLY WEAPON COMBAT 2hlarge THROWBRAWL
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\air_titanweapon_knife_throw 1 25
	 PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_knife_throw 1 25
	 PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_knife_throw 1 25
	 PitchAngle	0.5
	TEnd
MEnd

Move A_2hlarge_ThrowBrawl_post
	Requires	FLY WEAPON COMBAT 2hlarge
	Priority        4
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        A_2hlargeReady
	Flags		PitchToTarget

	Type		male
	  Anim          male\air_titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_titanweapon_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd

#############################

Include sequencers\player\WEAPONS\Weapons_LongStaff_a.inc