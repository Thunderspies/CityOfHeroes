#############################################################
## Shields_Powers.inc - Shield powers sequences
#############################################################

#############################################################
## Note: All Priorites are artifically set 2 higher to take
## precedence over normal WEAPON & COMBAT moves
#############################################################

## SHIELD BRAWL - KICK ###########################################################

Move Shield_KickBrawl
	Requires	SHIELD COMBAT PUNCH BRAWL #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_KickBrawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PICKRANDOMLY

	Type		male
	  Anim          male\Dual_Brawl 5 29
	TEnd
MEnd

Move Shield_KickBrawl_Post
	Requires	SHIELD COMBAT #WEAPON
	Priority	3
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend

	Type		male
	  Anim          male\Dual_Brawl 30 50
	TEnd
MEnd

#############################################################

Move A_Shield_KickBrawl
	Requires	FLY SHIELD COMBAT PUNCH BRAWL #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_KickBrawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PICKRANDOMLY

	Type		male
	  Anim          male\air_dual_brawl 5 30
	TEnd
MEnd

Move A_Shield_KickBrawl_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Priority	14
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_Blend

	Type		male
	  Anim          male\air_dual_brawl 31 50
	TEnd
MEnd

## SHIELD BRAWL - BASH ###########################################################

Move Shield_BashBrawl
	Requires	SHIELD COMBAT PUNCH BRAWL #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_BashBrawl_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_magic_throw 6 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	  PlayFx	POWERS\Shields\Shield_Brawl.fx 0
	TEnd
MEnd

Move Shield_BashBrawl_Post
	Requires	SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_magic_throw 31 70
	TEnd
MEnd

#############################################################

Move A_Shield_BashBrawl
	Requires	FLY SHIELD COMBAT PUNCH BRAWL #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_BashBrawl_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_air_magic_throw 6 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	  PlayFx	POWERS\Shields\Shield_Brawl.fx 0
	TEnd
MEnd

Move A_Shield_BashBrawl_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_throw 31
	TEnd
MEnd

## PUNCH ###########################################################

Move Shield_BrawlRight
	Requires	SHIELD COMBAT PUNCH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_BrawlRight_Post
	Sets            CANTMOVE
	Flags		NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_brawlright 1 25
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Shield_BrawlRight_Post
	Requires	SHIELD COMBAT #WEAPON
	Priority	3
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_brawlright 26 48
	TEnd
MEnd

#############################################################

Move Shield_BrawlLeft
	Requires	SHIELD COMBAT PUNCH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_BrawlLeft_Post
	Sets            CANTMOVE
	Flags		NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_brawlright 50 74
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Shield_BrawlLeft_Post
	Requires	SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_brawlright 75
	TEnd
MEnd

#############################################################

Move A_Shield_BrawlRight
	Requires	FLY SHIELD COMBAT PUNCH #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_BrawlRight_Post
	Sets            CANTMOVE
	SticksOnChild   Fly Love
	Flags		NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_air_brawlright 1 25
	TEnd
MEnd

Move A_Shield_BrawlRight_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_brawlright 26 48
	TEnd
MEnd

#############################################################

Move A_Shield_BrawlLeft
	Requires	FLY SHIELD COMBAT PUNCH #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_BrawlLeft_Post
	Sets            CANTMOVE
	SticksOnChild   Fly Love
	Flags		NoSizeScale PICKRANDOMLY

	Type		male
	  Anim          male\shield_air_brawlright 50 74
	TEnd
MEnd

Move A_Shield_BrawlLeft_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_brawlright 75
	TEnd
MEnd


## DOOR KICK ###########################################################

Move Shield_DoorKick
	Requires	SHIELD COMBAT STRONG KICK #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_DoorKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Dual_Brawl 1 50
	TEnd
MEnd

Move Shield_DoorKick_Post
	Requires	SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\Dual_Brawl 51
	TEnd
MEnd

#############################################################

Move A_Shield_DoorKick
	Requires	FLY SHIELD COMBAT STRONG KICK #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_DoorKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_Dual_Brawl 1 50
	TEnd
MEnd

Move A_Shield_DoorKick_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        A_Shield_CombatCycle_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_Dual_Brawl 51
	TEnd
MEnd


## THROW BRAWL ###########################################################

Move Shield_ThrowBrawl
	Requires	COMBAT SHIELD THROWBRAWL #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ThrowBrawl_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_knife_throw 1 25
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_ThrowBrawl_Post
	Requires	COMBAT SHIELD #WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Shield_ThrowBrawl
	Requires	FLY COMBAT SHIELD THROWBRAWL #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_ThrowBrawl_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\air_dual_knife_throw 1 25
	 PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_ThrowBrawl_Post
	Requires	FLY COMBAT SHIELD #WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        A_Shield_CombatCycle_Blend
	Flags		PitchToTarget

	Type		male
	  Anim          male\air_dual_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd


## BOOSTUP ###########################################################

Move Shield_BoostUp
	Requires	BOOSTUP SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_boostup 1
	TEnd
MEnd

#############################################################

Move A_Shield_BoostUp
	Requires	FLY BOOSTUP SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	Shield_CombatFly_Blend #Shield_FlyReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_air_boostup 1
	TEnd
MEnd


## BOOSTDOWN ###########################################################

Move Shield_BoostDown
	Requires	SHIELD BOOSTDOWN #COMBAT WEAPON
	Priority	14
	Interrupts	<ReactIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_boostdown 1
	TEnd
MEnd

#############################################################

Move A_Shield_BoostDown
	Requires	FLY SHIELD BOOSTDOWN #COMBAT WEAPON
	Priority	25
	Interrupts	<ReactIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_boostdown 1
	TEnd
MEnd

#############################################################
## BOOST AURA
#############################################################

Move Shield_Boostaura_FastCast
	Requires	SHIELD BOOSTAURA NOTROOTED HIGH #COMBAT WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Boostaura_NoRoot_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 1 47
	TEnd
MEnd

## BOOSTAURA - NON ROOTED ###########################################################

Move Shield_Boostaura_NoRoot_Pre
	Requires	SHIELD BOOSTAURA NOTROOTED #COMBAT WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Boostaura_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 1 40
	TEnd
MEnd

Move Shield_Boostaura_NoRoot
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Boostaura_NoRoot_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 1 47
	TEnd
MEnd

Move Shield_Boostaura_NoRoot_Post
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Ready #Shield_PostReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 48
	TEnd
MEnd

## BOOSTAURA - ROOTED ###########################################################

Move Shield_Boostaura_Pre
	Requires	SHIELD BOOSTAURA STARTOFFENSESTANCE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
#	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Boostaura
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 1 40
	TEnd
MEnd

Move Shield_Boostaura
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Boostaura_NoRoot_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 1 47
	TEnd
MEnd

#############################################################

Move A_Shield_Boostaura_FastCast
	Requires	FLY SHIELD BOOSTAURA NOTROOTED HIGH #COMBAT WEAPON
	Priority	27
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	A_Shield_BoostAura_NoRoot_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 1 47
	TEnd
MEnd

#############################################################

Move A_Shield_BoostAura_NoRoot_Pre
	Requires	FLY SHIELD BOOSTAURA NOTROOTED #COMBAT WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        A_Shield_BoostAura
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_groundsummon 1 40
	TEnd
MEnd

Move A_Shield_BoostAura_NoRoot
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	A_Shield_BoostAura_NoRoot_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 1 47
	TEnd
MEnd

Move A_Shield_BoostAura_NoRoot_Post
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	Shield_FlyReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 47
	TEnd
MEnd

#############################################################

Move A_Shield_BoostAura_Pre
	Requires	FLY SHIELD BOOSTAURA STARTOFFENSESTANCE #COMBAT WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        A_Shield_BoostAura
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_groundsummon 1 40
	TEnd
MEnd

Move A_Shield_BoostAura
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	A_Shield_BoostAura_NoRoot_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 1 47
	TEnd
MEnd

## FLURRY ###########################################################

Move Shield_Flurry
	Requires	COMBAT SHIELD PUNCH SPEEDPUNCH
	Priority	15
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Flurry_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_flurry 1 92
	TEnd
MEnd

Move Shield_Flurry_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_flurry 93
	TEnd
MEnd

#############################################################

Move A_Shield_Flurry
	Requires	FLY COMBAT SHIELD PUNCH SPEEDPUNCH
	Priority	26
	Interrupts	<AttackIRQ> A_Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Flurry_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_flurry 1 88
	TEnd
MEnd

Move A_Shield_Flurry_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_flurry 89 119
	TEnd
MEnd


## SPIN ###########################################################

Move Shield_Spin_Quick
	Requires	SHIELD WHIRLWIND #COMBAT WEAPON
	Priority	14
	Interrupts	<AttackIRQ> <Ready> <Combat> #<WepCombat> <TwoHandCombat>
	Member		<DeathIRQ> <ShieldCombat> <AttackIRQ>
	NextMove        Shield_Spin_Cycle
	Wind		FREEZE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_spin 1 24
	TEnd
MEnd

#############################################################

Move Shield_Spin
	Requires	SHIELD WHIRLWIND QUICK #COMBAT WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Spin_Cycle
	Wind		FREEZE
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\shield_spin 1 45
	TEnd
MEnd

Move Shield_Spin_Cycle
	Requires	SHIELD WHIRLWIND #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <AttackIRQ> <ReactIRQ>
	NextMove        Shield_Jump_Post
	Wind		FREEZE
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\shield_spin 25 53
	TEnd
MEnd

#############################################################

Move A_Shield_Spin_Quick
	Requires	FLY SHIELD WHIRLWIND #COMBAT WEAPON
	Priority	25
	Interrupts	<AttackIRQ> <FlyReady> <FlyCombat> #<WepCombat> <TwoHandCombat>
	Member		<DeathIRQ> <ShieldCombat> <AttackIRQ>
	NextMove        A_Shield_Spin_Cycle
	Wind		FREEZE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_spin 1 24
	TEnd
MEnd

#############################################################

Move A_Shield_Spin
	Requires	FLY SHIELD WHIRLWIND QUICK #COMBAT WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Spin_Cycle
	Wind		FREEZE
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\shield_air_spin 1 45
	TEnd
MEnd

Move A_Shield_Spin_Cycle
	Requires	FLY SHIELD WHIRLWIND #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <AttackIRQ> <ReactIRQ>
	NextMove        Shield_Jump_Post
	Wind		FREEZE
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\shield_air_spin 25 53
	TEnd
MEnd


## SMASHCAST ###########################################################

Move Shield_Smashcast
	Requires	SHIELD COMBAT SMASH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Smashcast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_smashcast 1 43
	TEnd
MEnd

Move Shield_Smashcast_Post
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Scale           0.80
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_smashcast 44
	TEnd
MEnd

#############################################################

Move A_Shield_Smashcast
	Requires	FLY SHIELD COMBAT SMASH #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_Smashcast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_smashcast 1 45
	TEnd
MEnd

Move A_Shield_Smashcast_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_smashcast 46
	TEnd
MEnd


## RADIUS EFFECT ###########################################################

Move Shield_RadiusEffect
	Requires	SHIELD COMBAT RADIUSEFFECT NOTROOTED #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_radiuseffect 1
	TEnd
MEnd

#############################################################

Move A_Shield_RadiusEffect
	Requires	FLY SHIELD COMBAT RADIUSEFFECT NOTROOTED #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_radiuseffect 1
	TEnd
MEnd


## GUTPUNCH ###########################################################

Move Shield_Gutpunch
	Requires	SHIELD COMBAT GUTPUNCH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Gutpunch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_gutpunch 1 31
	TEnd
MEnd

Move Shield_Gutpunch_Post
	Requires	SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_gutpunch 32
	TEnd
MEnd

#############################################################

Move A_Shield_Gutpunch
	Requires	FLY SHIELD COMBAT GUTPUNCH #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Gutpunch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_gutpunch 1 31
	TEnd
MEnd

Move A_Shield_Gutpunch_Post
	Requires	FLY SHIELD COMBAT #WEAPON
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_gutpunch 32
	TEnd
MEnd


## TAUNT A ###########################################################

Move Shield_Taunt_A
	Requires	SHIELD COMBAT TAUNT #VARIATIONA
	Priority	15 #This move is 1 higher than it should be to cover all variations
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Taunt_A_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Taunt 1 45
  	  PlayFX	feet\MaleTaunt13.fx
	TEnd

	Type		huge
	  Anim          male\shield_Taunt 1 45
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          male\shield_Taunt 1 45
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move Shield_Taunt_A_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Taunt 46 51
	TEnd
MEnd

#############################################################

Move A_Shield_Taunt_A
	Requires	FLY SHIELD COMBAT TAUNT #VARIATIONA
	Priority	26 #This move is 1 higher than it should be to cover all variations
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Taunt_A_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_shield_Taunt 1 45
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          male\air_shield_Taunt 1 45
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          male\air_shield_Taunt 1 45
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move A_Shield_Taunt_A_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_shield_Taunt 46
	TEnd
MEnd


## SHIELD BEATCHEST ###########################################################

Move Shield_BeatChest
	Requires	SHIELD COMBAT TAUNTA NOTROOTED #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_BeatChest_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          male\shield_beatchest 1 91
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          male\shield_beatchest 1 91
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move Shield_BeatChest_Cycle
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle
	NextMove        Shield_Ready

	Type		male
	  Anim          male\shield_beatchest 92 144
	TEnd
MEnd

#############################################################

Move A_Shield_BeatChest
	Requires	FLY SHIELD COMBAT TAUNTA NOTROOTED #WEAPON
	Priority        26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_beatchest 1
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          male\shield_air_beatchest 1
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          male\shield_air_beatchest 1
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd


## WEAVE ###########################################################

Move Shield_Weave_Pre
	Requires	SHIELD PREMODE BRAWL
	Priority	15
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Weave
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_weave 86 87
	TEnd
MEnd

Move Shield_Weave
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Weave_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_weave 88 142
	TEnd
MEnd

Move Shield_Weave_PostCycle
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\shield_weave 35 142
	TEnd
MEnd

#############################################################

Move A_Shield_Weave_Pre
	Requires	FLY SHIELD PREMODE BRAWL
	Priority	26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_Weave
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_weave 86 87
	TEnd
MEnd

Move A_Shield_Weave
	Requires	FLY SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_Weave_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_weave 88 142
	TEnd
MEnd

Move A_Shield_Weave_PostCycle
	Requires	FLY SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\shield_air_weave 35 142
	TEnd
MEnd


## LEADERSHIP ###########################################################

Move Shield_Leadership
	Requires	SHIELD LEADERSHIP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_leadership 1
	TEnd
MEnd

#############################################################

Move A_Shield_Leadership
	Requires	FLY SHIELD LEADERSHIP
	Priority	25
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_leadership 1
	TEnd
MEnd


## MED OTHER ###########################################################

Move Shield_MedOther
	Requires	SHIELD MEDOTHER SPAWNSIDE
	Priority	65
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_MedOther_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_medother 1 138
	TEnd

	Type		huge
	  Anim          huge\shield_medother 1 138
	TEnd

	Type		fem
	  Anim          fem\shield_medother 1 138
	TEnd
MEnd

Move Shield_MedOther_Post
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_medother 139
	TEnd

	Type		huge
	  Anim          huge\shield_medother 139
	TEnd

	Type		fem
	  Anim          fem\shield_medother 139
	TEnd
MEnd

#############################################################

Move A_Shield_MedOther
	Requires	FLY SHIELD MEDOTHER SPAWNSIDE
	Priority	66
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_MedOther_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_medother 1 138
	TEnd

	Type		huge
	  Anim          huge\shield_air_medother 1 138
	TEnd

	Type		fem
	  Anim          fem\shield_air_medother 1 138
	TEnd
MEnd

Move A_Shield_MedOther_Post
	Requires	FLY SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_medother 139
	TEnd

	Type		huge
	  Anim          huge\shield_air_medother 139
	TEnd

	Type		fem
	  Anim          fem\shield_air_medother 139
	TEnd
MEnd


## MED SELF ###########################################################

Move Shield_MedSelf
	Requires	SHIELD MEDSELF
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_MedSelf_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 1 244
	  PlayFX	POWERS\Medicine\MSelf.fx 17 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 1 244
	  PlayFX	POWERS\Medicine\MSelf.fx 17 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 1 244
	  PlayFX	POWERS\Medicine\MSelf.fx 17 CONSTANT
	TEnd
MEnd

Move Shield_MedSelf_Post
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 245
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 245
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 245
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Shield_MedSelf
	Requires	FLY SHIELD MEDSELF
	Priority	25
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_MedSelf_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_med_self 1 244
	TEnd

	Type		huge
	  Anim          huge\air_med_self 1 244
	TEnd

	Type		fem
	  Anim          fem\air_med_self 1 244
	TEnd
MEnd

Move A_Shield_MedSelf_Post
	Requires	FLY SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_FlyReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_med_self 245
	TEnd

	Type		huge
	  Anim          huge\air_med_self 245
	TEnd

	Type		fem
	  Anim          fem\air_med_self 245
	TEnd
MEnd


## REVIVE ###########################################################

Move Shield_Revive_Windup_Pre
	Requires	SHIELD MEDDEAD WINDUP #COMBAT WEAPON
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <Jumps> <MoveIRQ> <Windups>
	NextMove        Shield_Revive_Windup_Cycle
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\shield_med_dead 1 99
	TEnd
MEnd

Move Shield_Revive_Windup_Cycle
	Requires	SHIELD MEDDEAD WINDUP
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <Jumps> <MoveIRQ> <Windups>
	Flags		Cycle NoSizeScale
	NextMove        Shield_Revive

	Type		male
	  Anim          male\shield_med_dead 100 200
	TEnd
MEnd

Move Shield_Revive
	Requires	SHIELD MEDDEAD GETUP
	Priority        15
	Interrupts	<Windups> Revive_Windup_Pre Revive_Windup_Cycle
	Include sequencers\Members\Combat_Shield.inc
	NextMove        shield_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_med_dead 150
	TEnd
MEnd

#############################################################

Move A_Shield_Revive_Windup
	Requires	FLY SHIELD MEDDEAD WINDUP #COMBAT WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <Jumps> <MoveIRQ> <Windups>
	NextMove        A_Shield_Revive_WindupCycle
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\shield_air_med_dead 1 99
	TEnd
MEnd

Move A_Shield_Revive_WindupCycle
	Requires	FLY SHIELD MEDDEAD WINDUP
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <Jumps> <MoveIRQ> <Windups>
	Flags		Cycle NoSizeScale
	NextMove        A_Shield_Revive

	Type		male
	  Anim          male\shield_air_med_dead 100 200
	TEnd
MEnd

Move A_Shield_Revive
	Requires	FLY SHIELD MEDDEAD GETUP
	Priority        26
	Interrupts	<Windups> A_Revive_Windup A_Revive_WindupCycle
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_med_dead 150
	TEnd
MEnd


## MOUTH ###########################################################

Move Shield_Mouth
	Requires	SHIELD COMBAT HEADCAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move Shield_Mouth_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_mouth 50
	TEnd
MEnd

#############################################################

Move A_Shield_Mouth
	Requires	FLY SHIELD COMBAT HEADCAST
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_mouth 1 55
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move A_Shield_Mouth_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\shield_air_mouth 56 70
	TEnd
MEnd


## MOUTHSFX (Scream with sound FX) ###########################################################

Move Shield_MouthSFX
	Requires	COMBAT SHIELD MOUTHSFX
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage1.fx
	TEnd
MEnd

#############################################################

Move A_Shield_MouthSFX
	Requires	FLY SHIELD COMBAT MOUTHSFX
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_mouth 1 55
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage1.fx
	TEnd
MEnd


## HEADCAST STRONG ###########################################################

Move Shield_Headcast_Strong
	Requires	SHIELD COMBAT HEADCAST HIGH STRONG
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Mouth_Post
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

#############################################################

Move A_Shield_Headcast_Strong
	Requires	FLY SHIELD COMBAT HEADCAST HIGH STRONG
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Mouth_Post
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

## SCREAM ###########################################################

Move Shield_Scream
	Requires	SHIELD COMBAT SCREAM
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Scream_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_scream 10 58
	  PlayFX	ENEMYFX\Romans\Male_ShoutOfCommand.fx
	TEnd

	Type		huge
	  Anim          male\shield_scream 10 58
	  PlayFX	ENEMYFX\Romans\Huge_ShoutOfCommand.fx
	TEnd

	Type		fem
	  Anim          male\shield_scream 10 58
	  PlayFX	ENEMYFX\Romans\Female_ShoutOfCommand.fx
	TEnd
MEnd

Move Shield_Scream_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Scream_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_scream 59
	TEnd
MEnd

Move Shield_Scream_PostCycle
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale
	NextMove        Shield_CombatReady_Blend

	Type		male
	  Anim          male\shield_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Shield_Scream
	Requires	FLY SHIELD COMBAT SCREAM
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Scream_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Male_ShoutOfCommand.fx
	TEnd

	Type		huge
	  Anim          male\shield_air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Huge_ShoutOfCommand.fx
	TEnd

	Type		fem
	  Anim          male\shield_air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Female_ShoutOfCommand.fx
	TEnd
MEnd

Move A_Shield_Scream_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_scream 59
	TEnd
MEnd


## SHOUT OF COMMAND ###########################################################

Move Shield_ShoutOfCommand
	Requires	SHIELD COMBAT SCREAM COMMAND
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ShoutOfCommand_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\scream 10 58
	  PlayFX	ENEMYFX\Romans\Male_ShoutOfCommand.fx
	TEnd

	Type		huge
	  Anim          huge\scream 10 58
	  PlayFX	ENEMYFX\Romans\Huge_ShoutOfCommand.fx
	TEnd

	Type		fem
	  Anim          fem\scream 10 58
	  PlayFX	ENEMYFX\Romans\Female_ShoutOfCommand.fx
	TEnd
MEnd

Move Shield_ShoutOfCommand_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_ShoutOfCommand_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\scream 59
	TEnd

	Type		huge
	  Anim          huge\scream 59
	TEnd

	Type		fem
	  Anim          fem\scream 59
	TEnd
MEnd

Move Shield_ShoutOfCommand_PostCycle
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale
	NextMove        Shield_CombatReady_Blend

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Shield_ShoutOfCommand
	Requires	FLY SHIELD COMBAT SCREAM COMMAND
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_ShoutOfCommand_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Male_ShoutOfCommand.fx
	TEnd

	Type		huge
	  Anim          huge\air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Huge_ShoutOfCommand.fx
	TEnd

	Type		fem
	  Anim          fem\air_scream 10 58
	  PlayFX	ENEMYFX\Romans\Female_ShoutOfCommand.fx
	TEnd
MEnd

Move A_Shield_ShoutOfCommand_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_FlyReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_scream 59
	TEnd

	Type		huge
	  Anim          huge\air_scream 59
	TEnd

	Type		fem
	  Anim          fem\air_scream 59
	TEnd
MEnd


## STEAL ###########################################################

Move Shield_Steal
	Requires	SHIELD COMBAT STEAL
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Steal_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_steal 1 57
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_Steal_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_steal 58
	TEnd
MEnd

#############################################################

Move A_Shield_Steal
	Requires	FLY SHIELD COMBAT STEAL
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Steal_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_steal 1 57
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_Steal_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd
MEnd

## THROW KNIFE ###########################################################

Move Shield_ThrowKnife_Pre
	Requires	SHIELD COMBAT THROWKNIFE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ThrowKnife
	Flags		CANTMOVE NOSIZESCALE

	Type		male
	  Anim          male\shield_throwknife 1 21
	TEnd
MEnd

Move Shield_ThrowKnife
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ThrowKnife_Post
	Scale           0.375
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_throwknife 22 31
	TEnd
MEnd

Move Shield_ThrowKnife_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.5

	Type		male
	  Anim          male\shield_throwknife 32
	TEnd
MEnd

#############################################################

Move A_Shield_ThrowKnife_Pre
	Requires	FLY SHIELD COMBAT THROWKNIFE
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_ThrowKnife
	Flags		CANTMOVE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\shield_air_throwknife 1 21
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_ThrowKnife
	Requires	FLY SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_ThrowKnife_Post
	Scale		0.375

	Type		male
	  Anim          male\shield_air_throwknife 22 31
	TEnd
MEnd

Move A_Shield_ThrowKnife_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Scale		0.5

	Type		male
	  Anim          male\shield_air_throwknife 32
	TEnd
MEnd

## CAST ###########################################################

Move Shield_Cast
	Requires	SHIELD COMBAT CAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Cast_Post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Shield_Cast_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_palmcast 31 45
	TEnd
MEnd

#############################################################

Move A_Shield_Cast
	Requires	FLY SHIELD COMBAT CAST
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Cast_Post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_Shield_Cast_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_palmcast 31
	TEnd
MEnd


## WALL ###########################################################

Move Shield_Wall
	Requires	SHIELD COMBAT WALL
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Wall_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_wall 2 30
	TEnd
MEnd

Move Shield_Wall_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_wall 31
	TEnd
MEnd

#############################################################

Move A_Shield_Wall
	Requires	FLY SHIELD COMBAT WALL
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Wall_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_air_wall 1 30
	TEnd
MEnd

Move A_Shield_Wall_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_wall 31
	TEnd
MEnd


## SUMMON SKY ###########################################################

Move Shield_SkySummon
	Requires	SHIELD COMBAT SKYSUMMON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_SkySummon_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_skysummon 1 60
	TEnd
MEnd

Move Shield_SkySummon_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_skysummon 61 73
	TEnd
MEnd

#############################################################

Move A_Shield_SkySummon
	Requires	FLY SHIELD COMBAT SKYSUMMON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_SkySummon_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_air_skysummon 1 60
	TEnd
MEnd

Move A_Shield_SkySummon_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_skysummon 61
	TEnd
MEnd


## GROUND SUMMON ###########################################################

Move Shield_GroundSummon
	Requires	SHIELD COMBAT GROUNDSUMMON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_GroundSummon_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 1 104
	TEnd
MEnd

Move Shield_GroundSummon_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 105
	TEnd
MEnd

#############################################################

Move A_Shield_GroundSummon
	Requires	FLY SHIELD COMBAT GROUNDSUMMON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_GroundSummon_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 1 60
	TEnd
MEnd

Move A_Shield_GroundSummon_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 61
	TEnd
MEnd


## GROUND SUMMON - NOROOT ###########################################################

Move Shield_GroundSummon_NoRoot
	Requires	SHIELD COMBAT GROUNDSUMMON NOTROOTED #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 1
	TEnd
MEnd

#############################################################

Move A_Shield_GroundSummon_NoRoot
	Requires	FLY SHIELD COMBAT GROUNDSUMMON NOTROOTED #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 1
	TEnd
MEnd


## MAGIC WINDUP ###########################################################

Move Shield_Magic_WindUp
	Requires	SHIELD COMBAT MAGICWINDUP
	Priority	14
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <ShieldCombat> <ReactIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesLefthand>
	NextMove        Shield_Magic_WindUp_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 1 64
	TEnd
MEnd

Move Shield_Magic_WindUp_Cycle
	Requires	SHIELD COMBAT MAGICWINDUP
	Member		<ReactIRQ> <DeathIRQ> <ShieldCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesLefthand>
	Flags		Cycle NoSizeScale
	NextMove        Shield_Magic_WindUp_Post

	Type		male
	  Anim          male\shield_groundsummon 65 104
	TEnd
MEnd

Move Shield_Magic_WindUp_Post
	Requires	SHIELD COMBAT
	Member		<ReactIRQ> <DeathIRQ> <ShieldCombat> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesLefthand>
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_groundsummon 105
	TEnd
MEnd

#############################################################

Move A_Shield_Magic_WindUp
	Requires	FLY SHIELD COMBAT MAGICWINDUP
	Priority	25
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <ShieldCombat> <ReactIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesLefthand>
	NextMove        A_Shield_Magic_WindUp_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_windup 1 64
	TEnd
MEnd

Move A_Shield_Magic_WindUp_Cycle
	Requires	FLY SHIELD COMBAT MAGICWINDUP
	Member		<ReactIRQ> <DeathIRQ> <ShieldCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesLefthand>
	Flags		Cycle NoSizeScale
	NextMove        A_Shield_Magic_WindUp_Post

	Type		male
	  Anim          male\shield_air_magic_windup 65 104
	TEnd
MEnd

Move A_Shield_Magic_WindUp_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_windup 105
	TEnd
MEnd


## MAGIC THROW ###########################################################

Move Shield_Magic_Throw
	Requires	SHIELD COMBAT MAGICTHROW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Magic_Throw_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_magic_throw 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move Shield_Magic_Throw_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_magic_throw 31 70
	TEnd
MEnd

#############################################################

Move A_Shield_Magic_Throw
	Requires	FLY SHIELD COMBAT MAGICTHROW
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Magic_Throw_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_throw 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move A_Shield_Magic_Throw_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_throw 31
	TEnd
MEnd


## MAGIC CAST LEFT ###########################################################

Move Shield_Magic_CastL
	Requires	SHIELD COMBAT MAGIC CAST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Magic_CastL_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_magic_castl 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_Magic_CastL_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_magic_castl 31 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Shield_Magic_CastL
	Requires	FLY SHIELD COMBAT MAGIC CAST
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Magic_CastL_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_castl 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_Magic_CastL_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_FlyReady_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_magic_castl 31 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd


## RADIUS ATTACK ###########################################################

Move Shield_RadiusAttack
	Requires	SHIELD COMBAT RADIUSATTACK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_RadiusAttack_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_radiusattack 1 30
	TEnd
MEnd

Move Shield_RadiusAttack_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_radiusattack 31
	TEnd
MEnd

#############################################################

Move A_Shield_RadiusAttack
	Requires	FLY SHIELD COMBAT RADIUSATTACK
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_RadiusAttack_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_air_radiusattack 1 30
	TEnd
MEnd

Move A_Shield_RadiusAttack_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_radiusattack 31
	TEnd
MEnd

## STOP CAST ###########################################################

Move Shield_Stop_Pre
	Requires	SHIELD COMBAT STOPCAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Stop
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\shield_stop 1 31
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_Stop
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Stop_Post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_stop 32 60
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_Stop_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_stop 61
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Shield_Stop_Pre
	Requires	FLY SHIELD COMBAT STOPCAST
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Stop
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_stop 1 31
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_Stop
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_Stop_Post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_stop 32 60
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_Stop_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_stop 61
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd


## STOMP ###########################################################

Move Shield_Stomp
	Requires	SHIELD COMBAT STOMP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove        Shield_Stomp_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_stomp 1 62
	TEnd
MEnd

Move Shield_Stomp_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_stomp 63
	TEnd
MEnd

#############################################################

Move A_Shield_Stomp
	Requires	FLY SHIELD COMBAT STOMP
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Stomp_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_stomp 1 62
	TEnd
MEnd

Move A_Shield_Stomp_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_stomp 63
	TEnd
MEnd


## 1-2 PUNCH ###########################################################

Move Shield_OneTwo_Punch
	Requires	SHIELD COMBAT PUNCH COMBO2
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_OneTwo_Punch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_onetwo_punch 1 39
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX
	TEnd
MEnd

Move Shield_OneTwo_Punch_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_onetwo_punch 40
	TEnd
MEnd

#############################################################

Move A_Shield_OneTwo_Punch
	Requires	FLY SHIELD COMBAT PUNCH COMBO2
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_OneTwo_Punch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_onetwo_punch 1 39
	TEnd
MEnd

Move A_Shield_OneTwo_Punch_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_onetwo_punch 40
	TEnd
MEnd


## JUMPSPIN ###########################################################

Move Shield_JumpSpin_Pre
	Requires	SHIELD COMBAT CLAW CLAWAREA QUICK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_JumpSpin
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_spin 1 32
	TEnd
MEnd

Move Shield_JumpSpin
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_JumpSpin_Post
	Interpolate     0
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_spinpost 1 10
	  PlayFx        FEET\LAND1.FX 1
	TEnd
MEnd

Move Shield_JumpSpin_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_spinpost 11
	TEnd
MEnd

#############################################################

Move A_Shield_JumpSpin_Pre
	Requires	FLY SHIELD COMBAT CLAW CLAWAREA QUICK
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_JumpSpin
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 1 32
	TEnd
MEnd

Move A_Shield_JumpSpin
	Requires	FLY SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_JumpSpin_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 33 78
	  PlayFx        FEET\LAND1.FX  1
	TEnd
MEnd

Move A_Shield_JumpSpin_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_spin 79
	TEnd
MEnd


## BASEBALL PUNCH ###########################################################

Move Shield_BaseballPunch
	Requires	SHIELD COMBAT BASEBALLPUNCH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_BaseballPunch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_baseballpunch 1 43
	TEnd
MEnd

Move Shield_BaseballPunch_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_baseballpunch 44
	TEnd
MEnd

#############################################################

Move A_Shield_BaseballPunch
	Requires	FLY SHIELD COMBAT BASEBALLPUNCH
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_BaseballPunch_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_baseballpunch 1 43
	TEnd
MEnd

Move A_Shield_BaseballPunch_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_baseballpunch 44
	TEnd
MEnd


## JUMP SMASH ###########################################################

Move Shield_JumpSmash
	Requires	SHIELD COMBAT JUMPNSMASH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_JumpSmash_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_jumpsmash 1 97
	TEnd
MEnd

Move Shield_JumpSmash_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Scale           0.50
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_jumpsmash 98
	TEnd
MEnd

#############################################################

Move A_Shield_JumpSmash
	Requires	FLY SHIELD COMBAT JUMPNSMASH
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_JumpSmash_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_air_jumpsmash 1 97
	TEnd
MEnd

Move A_Shield_JumpSmash_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_jumpsmash 98
	 Scale           0.50
	TEnd
MEnd

## BLINDING CAST ###########################################################

Move Shield_Blinding
	Requires	SHIELD COMBAT BLINDING
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Blinding_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_blinding 1 88
	TEnd
MEnd

Move Shield_Blinding_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_blinding 89
	TEnd
MEnd

#############################################################

Move A_Shield_Blinding
	Requires	FLY SHIELD COMBAT BLINDING
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Blinding_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_blinding 1 88
	TEnd
MEnd

Move A_Shield_Blinding_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_blinding 89
	TEnd
MEnd


## BLOW CAST ###########################################################

Move Shield_BlowCast
	Requires	SHIELD COMBAT BLOW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_BlowCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_blowcast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd

Move Shield_BlowCast_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.5
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_blowcast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd
MEnd

#############################################################

Move A_Shield_BlowCast
	Requires	FLY SHIELD COMBAT BLOW
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_BlowCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_blowcast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd

Move A_Shield_BlowCast_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_blowcast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd
MEnd

## TWOHAND CAST ###########################################################

Move Shield_2HandCast
	Requires	SHIELD COMBAT 2HAND CAST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_2HandCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_2hand_cast 1 50
	  PitchEnd	50
	  PitchAngle	0
	TEnd
MEnd

Move Shield_2HandCast_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_2hand_cast 51
	TEnd
MEnd

#############################################################

Move A_Shield_2HandCast
	Requires	FLY SHIELD COMBAT 2HAND CAST
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_2HandCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

Move A_Shield_2HandCast_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_2hand_cast 51
	TEnd
MEnd


## HEAL CAST ###########################################################

Move Shield_HealCast
	Requires	SHIELD COMBAT HEAL
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_HealCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_healcast 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move Shield_HealCast_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_healcast 36
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd

#############################################################

Move A_Shield_HealCast
	Requires	FLY SHIELD COMBAT HEAL
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_HealCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_healcast 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move A_Shield_HealCast_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_healcast 36
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd


## SHORT TOSS ###########################################################

Move Shield_ShortToss_Squat
	Requires	SHIELD COMBAT SHORTTOSS
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ShortToss_Lift
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\shield_short_toss 1 30
	TEnd
MEnd

Move Shield_ShortToss_Lift
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ShortToss_Throw
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale           1.625

	Type		male
	  Anim          male\shield_short_toss 31 85
	TEnd
MEnd

Move Shield_ShortToss_Throw
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ShortToss_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale           1.3

	Type		male
	  Anim          male\shield_short_toss 86 115
	TEnd
MEnd

Move Shield_ShortToss_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_short_toss 116
	TEnd
MEnd

## ONE HAND SMASH ###########################################################

Move Shield_1HSmash_Right_Pre
	Requires	SHIELD COMBAT PUNCH SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_1HSmash_Right
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 12 36
	TEnd
MEnd

Move Shield_1HSmash_Right
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_1HSmash_Right_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 37 58
	TEnd
MEnd

Move Shield_1HSmash_Right_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 59 82
	TEnd
MEnd

#############################################################

Move Shield_1HSmash_Left_Pre
	Requires	SHIELD COMBAT PUNCH SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_1HSmash_Left
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 96 119
	TEnd
MEnd

Move Shield_1HSmash_Left
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_1HSmash_Left_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 120 140
	TEnd
MEnd

Move Shield_1HSmash_Left_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_1H_smash 141
	TEnd
MEnd

#############################################################

Move A_Shield_1HSmash_Right_Pre
	Requires	FLY SHIELD COMBAT PUNCH SMASH
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_1HSmash_Right
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 12 36
	TEnd
MEnd

Move A_Shield_1HSmash_Right
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_1HSmash_Right_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 37 58
	TEnd
MEnd

Move A_Shield_1HSmash_Right_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 59 82
	TEnd
MEnd

#############################################################

Move A_Shield_1HSmash_Left_Pre
	Requires	FLY SHIELD COMBAT PUNCH SMASH
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_1HSmash_Left
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 96 119
	TEnd
MEnd

Move A_Shield_1HSmash_Left
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_1HSmash_Left_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 120 140
	TEnd
MEnd

Move A_Shield_1HSmash_Left_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_1Hsmash 141
	TEnd
MEnd


## CAGE ###########################################################

Move Shield_Cage
	Requires	SHIELD COMBAT CAGE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Cage_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_cage 1 35
	TEnd
MEnd

Move Shield_Cage_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_cage 36
	TEnd
MEnd

#############################################################

Move A_Shield_Cage
	Requires	FLY SHIELD COMBAT CAGE
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Cage_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_cage 1 30
	TEnd
MEnd

Move A_Shield_Cage_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_cage 31
	TEnd
MEnd


## WINDUP UPERCUT ###########################################################

Move Shield_WindupUppercut
	Requires	SHIELD COMBAT WINDUPPERCUT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WindupUppercut_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_windupuppercut 1 65
	TEnd
MEnd

Move Shield_WindupUppercut_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_windupuppercut 67
	TEnd
MEnd

#############################################################

Move A_Shield_WindupUppercut
	Requires	FLY SHIELD COMBAT WINDUPPERCUT
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_WindupUppercut_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_windupuppercut 1 65
	TEnd
MEnd

Move A_Shield_WindupUppercut_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_windupuppercut 67
	TEnd
MEnd

## FORCE RAM ###########################################################

Move Shield_ForceRam
	Requires	SHIELD COMBAT FORCE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_ForceRam_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_forceram 1 30
	  PitchAngle	0.1
	TEnd
MEnd

Move Shield_ForceRam_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_forceram 31 57
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd
MEnd

#############################################################

Move A_Shield_ForceRam
	Requires	FLY SHIELD COMBAT FORCE
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_ForceRam_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_forceram 1 30
	  PitchAngle	0.1
	TEnd
MEnd

Move A_Shield_ForceRam_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_forceram 31
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd
MEnd

## GRENADE LOB ###########################################################

Move Shield_GrenadeLob
	Requires	SHIELD COMBAT WEAPON GRENADE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_GrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\grenade_lob 1 40
	TEnd
MEnd

Move Shield_GrenadeLob_Post
	Requires	SHIELD COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend

	Type		male
	  Anim          male\grenade_lob 41
	TEnd
MEnd

#############################################################

Move A_Shield_GrenadeLob
	Requires	FLY SHIELD COMBAT WEAPON GRENADE
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_GrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_air_weapongrenadelob 1 40
	TEnd
MEnd

Move A_Shield_GrenadeLob_Post
	Requires	FLY SHIELD COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend

	Type		male
	  Anim          male\shield_air_weapongrenadelob 41
	TEnd
MEnd


## TWOHAND SWIPE ###########################################################

Move Shield_2H_swipe_Right
	Requires	SHIELD COMBAT 2HAND SWIPE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_CombatReady_Blend
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_2H_swipe 1 31
	TEnd
MEnd

#############################################################

Move A_Shield_2H_swipe_Right
	Requires	FLY SHIELD COMBAT 2HAND SWIPE
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_CombatFly_Blend
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_2H_swipe 1 31
	TEnd
MEnd


## WINDBLAST ###########################################################

Move Shield_WindBlast
	Requires	SHIELD COMBAT WINDBLAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WindBlast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_windblast 1 70
	TEnd
MEnd

Move Shield_WindBlast_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_windblast 70
	TEnd
MEnd

#############################################################

Move A_Shield_WindBlast
	Requires	FLY SHIELD COMBAT WINDBLAST
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_WindBlast_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_windblast 1 70
	TEnd
MEnd

Move A_Shield_WindBlast_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_windblast 70
	TEnd
MEnd

## JACOB'S LADDER ###########################################################

Move Shield_JacobsLadder
	Requires	SHIELD COMBAT JACOBSLADDER
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_JacobsLadder_Post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\shield_jacobsladder 1 47
	  PitchEnd	41
	TEnd
MEnd

Move Shield_JacobsLadder_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_jacobsladder 48
	TEnd
MEnd

#############################################################

Move A_Shield_JacobsLadder
	Requires	FLY SHIELD COMBAT JACOBSLADDER
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_JacobsLadder_Post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_air_jacobsladder 1 47
	  PitchEnd	41
	TEnd
MEnd

Move A_Shield_JacobsLadder_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_jacobsladder 48
	TEnd
MEnd

## LIGHTNING TELEPORT ###########################################################

Move Shield_LightningTeleport_Pre
	Requires	SHIELD COMBAT LIGHTNINGTELEPORT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_LightningTeleport
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\glory 1 30
	  #Anim          male\shield_lightningteleport 1 30
	TEnd
MEnd

Move Shield_LightningTeleport
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_LightningTeleport_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\shield_lightningteleport 30 58
	TEnd
MEnd

Move Shield_LightningTeleport_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_lightningteleport 59
	TEnd
MEnd

#############################################################

#Move A_Shield_LightningTeleport_Pre
#	Requires	FLY SHIELD COMBAT LIGHTNINGTELEPORT
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Shield_LightningTeleport
#	Priority	14
#	Flags		CANTMOVE NoSizeScale
#
#	Type		male
#	  Anim          male\air_glory 1 30
#	TEnd
#MEnd
#
#Move A_Shield_LightningTeleport
#	Requires	FLY SHIELD COMBAT
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_Shield_LightningTeleport_Post
#	Priority	13
#	Flags		CANTMOVE NoSizeScale
#
#	Type		male
#	  Anim          male\air_pre_poison 30 58
#	TEnd
#MEnd
#
#Move A_Shield_LightningTeleport_Post
#	Requires	FLY SHIELD COMBAT
#	Member		<ReactIRQ> <DeathIRQ> <ShieldCombat> <FlyCombatAttack> <AttackIRQ> <BlockIRQ>
#	NextMove        Shield_CombatFly_Blend
#	Priority	13
##	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_pre_poison 59
#	TEnd
#MEnd


## LEVITATE ###########################################################

Move Shield_Levitate
	Requires	SHIELD COMBAT LEVITATE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Levitate_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_levitate 1 50
	TEnd
MEnd

Move Shield_Levitate_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_levitate 51
	TEnd
MEnd

#############################################################

Move A_Shield_Levitate
	Requires	FLY SHIELD COMBAT LEVITATE
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Levitate_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_levitate 1 50
	TEnd
MEnd

Move A_Shield_Levitate_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_levitate 51
	TEnd
MEnd

## SHIELD SPIN ###########################################################

Move Shield_Claws_Spin
	Requires	SHIELD COMBAT WEAPON DUAL CLAW CLAWAREA
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Claws_Spin_Land
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_claws_spin 1 32
	TEnd
MEnd

Move Shield_Claws_Spin_Land
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Claws_Spin_Post
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_claws_spin 32 45
	  PlayFx	FEET\LAND1.FX 1
	TEnd
MEnd

Move Shield_Claws_Spin_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatReady_HalfBlend
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_claws_spin 45
	TEnd
MEnd

#############################################################

Move A_Shield_Claws_Spin
	Requires	FLY SHIELD COMBAT WEAPON DUAL CLAW CLAWAREA
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Claws_Spin_Land
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_claws_spin 1 32
	TEnd
MEnd

Move A_Shield_Claws_Spin_Land
	Requires	FLY SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Claws_Spin_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_claws_spin 33 78
	TEnd
MEnd

Move A_Shield_Claws_Spin_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_claws_spin 79
	TEnd
MEnd


## SHIELD LEAPKICK ###########################################################

Move Shield_LeapKick_Pre
	Requires	SHIELD COMBAT LEAPKICK #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_LeapKick
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_leapkick 1 46
	  PlayFx        FEET\LAND1.FX 42
	TEnd

	Type		huge
	  Anim          male\leapkick 1 46
	  PlayFx        FEET\LAND1.FX 42
	TEnd
MEnd

Move Shield_LeapKick
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_LeapKick_Post
	Flags		NoSizeScale
	Scale		4.0

	Type		male
	  Anim          male\shield_leapkick 47 71
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		huge
	  Anim          male\shield_leapkick 47 71
	  PlayFx        FEET\HUGELAND2.FX
	TEnd
MEnd

Move Shield_LeapKick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_leapkick 72
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		huge
	  Anim          male\shield_leapkick 72
	  PlayFx        FEET\HUGELAND2.FX
	TEnd
MEnd


## SHIELD HOBBLEKICK ###########################################################

Move Shield_Hobblekick
	Requires	SHIELD COMBAT KUNGFU LOW KICK FEET #WEAPON
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Hobblekick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\shield_HobbleKick 1 60
	TEnd
MEnd

Move Shield_Hobblekick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_HobbleKick 61
	TEnd
MEnd

#############################################################

Move A_Shield_Hobblekick
	Requires	FLY SHIELD COMBAT KUNGFU LOW KICK FEET #WEAPON
	Priority	28
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Hobblekick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\shield_Air_HobbleKick 1 60
	TEnd
MEnd

Move A_Shield_Hobblekick_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale
	Scale		1.125

	Type		male
	  Anim          male\shield_Air_HobbleKick 61
	TEnd
MEnd


## SHIELD IRONKICK ###########################################################

Move Shield_Ironkick
	Requires	SHIELD COMBAT KUNGFU HIGH KICK #WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Ironkick_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_IronKick 34 83
	TEnd
MEnd

Move Shield_Ironkick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_IronKick 84
	TEnd
MEnd

#############################################################

Move A_Shield_Ironkick
	Requires	SHIELD FLY COMBAT KUNGFU HIGH KICK #WEAPON
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Ironkick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Wind		ATTACK

	Type		male
	  Anim          male\shield_air_IronKick 34 83
	TEnd
MEnd

Move A_Shield_Ironkick_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale
	Wind		ATTACK

	Type		male
	  Anim          male\shield_air_IronKick 84
	TEnd
MEnd


## SHIELD KICK ###########################################################

Move Shield_Kick
	Requires	SHIELD COMBAT KUNGFU KICK #WEAPON
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Kick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_kick 1 25
	TEnd
MEnd

Move Shield_Kick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_kick 26
	TEnd
MEnd

#############################################################

Move A_Shield_Kick
	Requires	SHIELD FLY COMBAT KUNGFU KICK #WEAPON
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Kick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_kick 1 25
	TEnd
MEnd

Move A_Shield_Kick_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_kick 26
	TEnd
MEnd


## SHIELD PALMSTUN ###########################################################

Move Shield_PalmStun
	Requires	SHIELD COMBAT KUNGFU PUNCH KUNGFUSTUN #WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_PalmStun_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_PalmStun 35 84
	TEnd
MEnd

Move Shield_PalmStun_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_PalmStun 85
	TEnd
MEnd

#############################################################

Move A_Shield_PalmStun
	Requires	SHIELD FLY COMBAT KUNGFU PUNCH KUNGFUSTUN #WEAPON
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_PalmStun_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_PalmStun 35 84
	TEnd
MEnd

Move A_Shield_PalmStun_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_PalmStun 85
	TEnd
MEnd


## SHIELD ROUNDHOUSE ###########################################################

Move Shield_Roundhouse
	Requires	SHIELD COMBAT KUNGFU KICK STRONG #WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Roundhouse_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Roundhouse 1 25
	TEnd
MEnd

Move Shield_Roundhouse_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_Roundhouse_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Roundhouse 26 39
	TEnd
MEnd

Move Shield_Roundhouse_PostCycle
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale
	NextMove        Shield_CombatReady_Blend
	Scale           0.50

	Type		male
	  Anim          male\shield_Roundhouse 41
	TEnd
MEnd

#############################################################

Move A_Shield_Roundhouse
	Requires	SHIELD FLY COMBAT KUNGFU KICK STRONG #WEAPON
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Roundhouse_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_Roundhouse 1 25
	TEnd
MEnd

Move A_Shield_Roundhouse_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        A_Shield_Roundhouse_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_Roundhouse 26 34
	TEnd
MEnd

Move A_Shield_Roundhouse_PostCycle
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	Flags		Cycle NoSizeScale
	NextMove        Shield_FlyFall

	Type		male
	  Anim          male\shield_air_Roundhouse 35
	TEnd
MEnd


## SHIELD STUNKICK ###########################################################

Move Shield_StunKick
	Requires	SHIELD COMBAT KUNGFU KICK KUNGFUSTUN #WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_StunKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_StunKick 25 100
	  PlayFx        FEET\LAND1.FX 80
	TEnd

	Type		huge
	  Anim		male\shield_StunKick 25 100
	  PlayFx        FEET\HUGELAND2.FX 80
	TEnd
MEnd

Move Shield_StunKick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend

	Type		male
	  Anim          male\shield_StunKick 101
	TEnd
MEnd

#############################################################

Move A_Shield_StunKick
	Requires	SHIELD FLY COMBAT KUNGFU KICK KUNGFUSTUN #WEAPON
	Priority	27
	Interrupts	<FlyAttackALL> <AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Stunkick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_StunKick 25 100
	TEnd
MEnd

Move A_Shield_StunKick_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_StunKick 101
	TEnd
MEnd


## SHIELD SWEEPKICK ###########################################################

Move Shield_SweepKick
	Requires	SHIELD COMBAT KUNGFU KUNGFUAREA ATTACK #WEAPON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_SweepKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_SweepKick 1 44
	TEnd
MEnd

Move Shield_SweepKick_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_SweepKick 45
	TEnd
MEnd

#############################################################

Move A_Shield_SweepKick
	Requires	SHIELD FLY COMBAT KUNGFU KUNGFUAREA ATTACK #WEAPON
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_SweepKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_SweepKick 40 84
	TEnd
MEnd

Move A_Shield_SweepKick_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_SweepKick 85
	TEnd
MEnd

## SHIELD DANGER SCENSE ###########################################################

Move Shield_Danger_Sense
	Requires	SHIELD KUNGFUAREA KUNGFU LOOKOUT #WEAPON
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Danger_Sense_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Shield_Danger_Sense_PostCycle
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale
	Scale		0.7

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################

Move A_Shield_DangerSense
	Requires	SHIELD FLY KUNGFUAREA KUNGFU LOOKOUT #WEAPON
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Danger_Sense_PostCycle
	Wind		FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_lookQUICK 1
	TEnd
MEnd

Move A_Shield_Danger_Sense_PostCycle
	Requires	SHIELD FLY
	Include sequencers\Members\Combat_Shield_air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_lookQUICK 1
	TEnd
MEnd

## SHIELD BULLRUSH ###########################################################

Move Shield_Bullrush_Pre
	Requires	SHIELD COMBAT BULLRUSH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_charge 1 24
	TEnd
MEnd

Move Shield_Bullrush
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Bullrush_Post
	Flags		NoSizeScale
	Sets            CANTMOVE
	Scale		0.75

	Type		male
	  Anim          male\shield_charge_post 20 35
	TEnd
MEnd

Move Shield_Bullrush_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_charge_post 36
	TEnd
MEnd

## Leaping: Spring Attack Shield ###########################################################

Move Shield_SpringAttack_Pre
	Requires	SHIELD COMBAT BULLRUSH ALTERNATE HIGH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_SpringAttack_Hidden
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\leaping_spring_attack 1 24

	TEnd
MEnd

Move Shield_SpringAttack_Hidden
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_SpringAttack
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\leaping_spring_attack 24 28
	 PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Shield_SpringAttack
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_SpringAttack_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\leaping_spring_attack 28 48
	TEnd
MEnd


Move Shield_SpringAttack_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat.inc
	NextMove        Shield_CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\leaping_spring_attack 49 88
	TEnd
MEnd

#############################################################

Move A_Shield_Bullrush_Pre
	Requires	SHIELD FLY COMBAT BULLRUSH #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_charge 1
	TEnd
MEnd

Move A_Shield_Bullrush
	Requires	SHIELD FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Bullrush_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_charge_post 10 46
	TEnd
MEnd

Move A_Shield_Bullrush_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_charge_post 47
	TEnd
MEnd

## SHIELD 2H POWER THRUST ###########################################################

Move Shield_2HPowerThrust_Pre
	Requires	SHIELD COMBAT 2HPOWERTHRUST
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_2HPowerThrust
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_2HPowerThrust 1 57
	TEnd
MEnd

Move Shield_2HPowerThrust
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_2HPowerThrust_Post
	Sets		CANTMOVE
	Flags		NoSizeScale
	Interpolate	8
	Scale		0.9

	Type		male
	  Anim          male\shield_mart_backstab 125 145
	TEnd
MEnd

Move Shield_2HPowerThrust_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale #PitchToTarget

	Type		male
	  Anim          male\shield_mart_backstab 146
	TEnd
MEnd

#############################################################

Move A_Shield_2HPowerThrust_Pre
	Requires	FLY SHIELD COMBAT 2HPOWERTHRUST
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_2HPowerThrust
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\a_shield_2HPowerThrust 1 57
	TEnd
MEnd

Move A_Shield_2HPowerThrust
	Requires	FLY SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_2HPowerThrust_Post
	Interpolate     8
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\shield_air_mart_backstab 125 145
	TEnd
MEnd

Move A_Shield_2HPowerThrust_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_mart_backstab 146
	TEnd
MEnd

##### SHIELD ONE_WITH_THE_SHIELD ########################################################

Move Shield_OneWithTheShield
	Requires	SHIELD COMBAT ONEWITHTHESHIELD
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_CombatReady_Blend
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_onewiththeshield 1
	TEnd
MEnd

Move A_Shield_OneWithTheShield
	Requires	SHIELD FLY COMBAT ONEWITHTHESHIELD
	Priority	24
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_CombatFly_Blend
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_onewiththeshield 1
	TEnd
MEnd

## Destiny BULLRUSH ###########################################################

Move Shield_Destiny_Bullrush_Pre
	Requires	SHIELD COMBAT ENDGAME BULLRUSH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Destiny_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_incarnate_destiny_attack 1 31
	TEnd
MEnd

Move Shield_Destiny_Bullrush
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Destiny_Bullrush_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\shield_incarnate_destiny_attack 32 45
	TEnd
MEnd

Move Shield_Destiny_Bullrush_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_incarnate_destiny_attack 46
	TEnd
MEnd

## Destiny BULLRUSH AIR ###########################################################

Move A_Shield_Destiny_Bullrush_Pre
	Requires	SHIELD FLY COMBAT ENDGAME BULLRUSH #WEAPON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Destiny_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_shield_incarnate_destiny_attack 1 31
	TEnd
MEnd

Move A_Shield_Destiny_Bullrush
	Requires	SHIELD FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Destiny_Bullrush_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_shield_incarnate_destiny_attack 32 45
	TEnd
MEnd

Move A_Shield_Destiny_Bullrush_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_shield_incarnate_destiny_attack 46
	TEnd
MEnd


