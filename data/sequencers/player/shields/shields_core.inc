#############################################################
## Shields_Core.inc - Shield Core Animations
#############################################################

#############################################################
## Note: All Priorites are artifically set 2 higher to take
## precedence over normal WEAPON & COMBAT moves
#############################################################

## DRAW SHIELD ###########################################################

#Move Draw_Shield
#	Requires	SHIELD SPAWNHIP
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Shield_CombatReady
#	Sets		CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\shield_reveal 1
#	TEnd
#MEnd
#
#Move A_Draw_Shield
#	Requires	FLY SHIELD SPAWNHIP
#	Priority        5
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Shield_CombatFly
#	Sets		CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_shield_reveal 1
#	TEnd
#MEnd

## READIES ###########################################################

Move Shield_Ready
	Requires	SHIELD #COMBAT WEAPON
	Priority	13
	Interrupts	<Ready>
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale ReqInputs

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

## COMBAT READY ###########################################################

Move Shield_CombatReady
	Requires	SHIELD #COMBAT WEAPON
	Priority	13
	Interrupts	<Ready>
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 1 31
	TEnd
MEnd

Move Shield_CombatReady_Blend
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Interpolate	15
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 1 31
	TEnd
MEnd

Move Shield_CombatReady_Half
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 15 31
	TEnd
MEnd

Move Shield_CombatReady_HalfBlend
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Interpolate	15
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 15 31
	TEnd
MEnd


## FLY READIES ###########################################################

Move Shield_PreFly
	Requires	FLY SHIELD #COMBAT WEAPON
	Priority	24
	Interrupts	<Ready> <ShieldReady> <ShieldFall> <ShieldJump> <PostMoveCycle>
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		Cycle
	CycleMove	Shield_FlyReady
	NextMove	Shield_FlyFall

	Type		male
	  Anim		male\shield_PreFlyReady 5
	TEnd
MEnd

Move Shield_FlyReady
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		Cycle
	NextMove	Shield_FlyFall

	Type		male
	  Anim		male\shield_FlyReady 1
	TEnd
MEnd

Move Shield_FlyReady_Blend
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		Cycle
	Interpolate	15
	NextMove	Shield_FlyReady

	Type		male
	  Anim		male\shield_FlyReady 1
	TEnd
MEnd

## FLY COMBAT CYCLE (Post Power Cycle) ###########################################################

Move Shield_CombatFly_Half
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Wind		FLY
	Flags		Cycle
	CycleMove	Shield_CombatFly
	NextMove	Shield_FlyFall
	Scale		0.50

	Type		male
	  Anim		male\shield_CombatFly 15 31
	TEnd
MEnd

Move Shield_CombatFly_HalfBlend
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Wind		FLY
	Flags		Cycle
	CycleMove	Shield_CombatFly
	NextMove	Shield_FlyFall
	Interpolate	15
	Scale		0.50

	Type		male
	  Anim		male\shield_CombatFly 15 31
	TEnd
MEnd

Move Shield_CombatFly_Blend
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		Cycle
	CycleMove	Shield_CombatFly
	NextMove	Shield_FlyFall
	Interpolate	15
	Scale		0.50
	Wind		FLY

	Type		male
	  Anim		male\shield_CombatFly 1 31
	TEnd
MEnd

Move Shield_CombatFly
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		Cycle
	NextMove	Shield_FlyFall
	Scale		0.50
	Wind		FLY

	Type		male
	  Anim		male\shield_CombatFly 1 31
	TEnd
MEnd

## BRAWL CYCLE ###########################################################

Move Shield_BrawlCycle
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle NoSizeScale
	Interpolate	15
	NextMove        Shield_CombatReady_Blend
	Scale           0.50

	Type		male
	  Anim          male\brawlcycle 1 31
	TEnd
MEnd

Move Shield_BrawlCycle_Half
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	Interpolate	15
	NextMove        Shield_BrawlCycle
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawlcycle 15 31
	TEnd
MEnd

#############################################################

Move A_Shield_BrawlCycle
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_Air.inc
	Interpolate     15
	NextMove        Shield_FlyFall
	Flags		Cycle NoSizeScale
	Scale           0.50

	Type		male
	  Anim          male\air_brawl_cycle 1 31
	TEnd
MEnd

Move A_Shield_BrawlCycle_Half
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_Air.inc
	Interpolate     15
	Flags		Cycle NoSizeScale
	CycleMove	A_Shield_BrawlCycle
	NextMove	Shield_FlyFall
	Scale           0.50

	Type		male
	  Anim          male\air_brawl_cycle 16 31
	TEnd
MEnd

## BLOCKS ###########################################################

Move Shield_Block_R
	Requires	BLOCK SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BlockIRQ> <ShieldReady> Shield_Block_L
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Flags		PickRandomly

	Type		male
	  Anim		male\shield_CombatBlock 1 49
	TEnd
MEnd

Move Shield_Block_L
	Requires	BLOCK SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BlockIRQ> <ShieldReady> Shield_Block_R
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady

	Type		male
	  Anim		male\shield_CombatBlock 52 104
	TEnd
MEnd

Move A_Shield_Block_R
	Requires	FLY BLOCK SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<BlockIRQ> <ShieldReady> A_Shield_Block_L
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	Shield_FlyReady_Blend

	Type		male
	  Anim		male\shield_Air_CombatBlock 1 49
	TEnd
MEnd

Move A_Shield_Block_L
	Requires	FLY BLOCK SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<BlockIRQ> <ShieldReady> A_Shield_Block_R
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_FlyReady_Blend

	Type		male
	  Anim		male\shield_Air_CombatBlock 52 104
	TEnd
MEnd

## DODGES ###########################################################

Move Shield_Dodge_L
	Requires	BLOCK SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BlockIRQ> <ShieldReady> Shield_Dodge_R
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Flags		PickRandomly

	Type		male
	  Anim		male\shield_CombatDodge 1 49
	TEnd
MEnd

Move Shield_Dodge_R
	Requires	BLOCK SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BlockIRQ> <ShieldReady> Shield_Dodge_L
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady

	Type		male
	  Anim		male\shield_CombatDodge 52 104
	TEnd
MEnd

## HIT REACTIONS ###########################################################

Move Shield_HitQuick
	Requires	SHIELD HIT QUICK
	Priority	15
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	#Flags		HitReact

	Type		male
	  Anim		male\shield_HitQuick 1
	TEnd
MEnd

Move A_Shield_HitQuick
	Requires	FLY SHIELD HIT QUICK
	Priority	26
	Interrupts	<HitIRQ>
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove	Shield_FlyReady_Blend
	#Flags		HitReact

	Type		male
	  Anim		male\shield_Air_HitQuick 1
	TEnd
MEnd

## TELEPORT ###########################################################

Move Shield_Teleport
	Requires	SHIELD SUPERTELEPORT
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Teleport_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 1 54
	TEnd
MEnd

Move Shield_Teleport_Post
	Requires	SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_FlyReady #Teleporting triggers a brief hover
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_FlameAura 55
	TEnd
MEnd

#############################################################

Move A_Shield_Teleport
	Requires	FLY SHIELD SUPERTELEPORT
	Priority        25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Teleport_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 1 54
	TEnd
MEnd

Move A_Shield_Teleport_Post
	Requires	FLY SHIELD
	Include sequencers\Members\Combat_Shield_Air.inc
	NextMove        Shield_FlyReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_Air_Flameaura 55
	TEnd
MEnd