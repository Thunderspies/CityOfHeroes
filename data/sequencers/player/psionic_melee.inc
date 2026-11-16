#############################################################
## Water Blast
#############################################################

## Aqua Bolt A #############################################

Move Mental_Strike
	Requires	COMBAT PSIONICMELEE QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Mental_Strike_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\mental_strike 1 30
	TEnd
MEnd

Move Mental_Strike_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\mental_strike 31
	TEnd
MEnd

## mental_strike air #############################################

Move A_mental_strike
	Requires	FLY COMBAT PSIONICMELEE QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_mental_strike_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\mental_strike_air 1 30
	TEnd
MEnd

Move A_mental_strike_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\mental_strike_air 31
	TEnd
MEnd


## Psi Blade #############################################

Move Psi_blade
	Requires	COMBAT PSIONICMELEE SLASH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Psi_blade_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\psi_blade 1 40
	TEnd
MEnd

Move Psi_blade_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\psi_blade 41
	TEnd
MEnd

### Psi_blade air #############################################

Move A_psi_blade
	Requires	FLY COMBAT PSIONICMELEE SLASH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_psi_blade_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\psi_blade_air 1 40
	TEnd
MEnd

Move A_psi_blade_post
	Requires	FLY COMBAT
	Include 	sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\psi_blade_air 41
	TEnd
MEnd

## Telekinetic Blow #############################################

Move Telekinetic_Blow
	Requires	COMBAT PSIONICMELEE BLOW
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Telekinetic_Blow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Telekinetic_Blow 1 44
	TEnd
MEnd

Move Telekinetic_Blow_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\Telekinetic_Blow 45
	TEnd
MEnd

##Telekinetic_Blow Air#############################################

Move A_Telekinetic_Blow
	Requires	FLY COMBAT PSIONICMELEE BLOW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Telekinetic_Blow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Telekinetic_Blow_air 1 44
	TEnd
MEnd

Move A_Telekinetic_Blow_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\Telekinetic_Blow_air 45
	TEnd
MEnd


## Psi balde sweep #############################################

Move Psi_Blade_Sweep
	Requires	COMBAT PSIONICMELEE SWIPE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Psi_Blade_Sweep_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Psi_Blade_Sweep 1 53
	TEnd
MEnd

Move Psi_Blade_Sweep_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\Psi_Blade_Sweep 54
	TEnd
MEnd

##Psi_Blade_Sweep Air#############################################

Move A_Psi_Blade_Sweep
	Requires	FLY COMBAT PSIONICMELEE SWIPE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Psi_Blade_Sweep_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Psi_Blade_Sweep_air 1 53
	TEnd
MEnd

Move A_Psi_Blade_Sweep_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\Psi_Blade_Sweep_air 54
	TEnd
MEnd

## Greater Psi Blade #############################################

Move Psi_Blade_Great
	Requires	COMBAT PSIONICMELEE CHOP
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Psi_Blade_Great_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Psi_Blade_Great 1 75
	TEnd
MEnd

Move Psi_Blade_Great_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\Psi_Blade_Great 76
	TEnd
MEnd

## Psi_Blade_Great Air #############################################

Move A_Psi_Blade_Great
	Requires	FLY COMBAT PSIONICMELEE CHOP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Psi_Blade_Great_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Psi_Blade_Great_air 1 30
	TEnd
MEnd

Move A_Psi_Blade_Great_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\Psi_Blade_Great_air 31
	TEnd
MEnd


## Mass_Levitate #############################################

Move Mass_Levitate
	Requires	COMBAT PSIONICMELEE LEVITATE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Mass_Levitate_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Mass_Levitate 1 75

	TEnd
MEnd

Move Mass_Levitate_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\Mass_Levitate 76
	TEnd
MEnd

## Steam Spray Air #############################################

Move A_Mass_Levitate
	Requires	FLY COMBAT PSIONICMELEE LEVITATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Mass_Levitate_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Mass_Levitate_air 1 75

	TEnd
MEnd

Move A_Mass_Levitate_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc
	Type		male
	  Anim          male\Mass_Levitate_air 76
	TEnd
MEnd


## Assassins Psi Blade #############################################

Move Assassins_Psi_Blade_Windup
	Requires	COMBAT PSIONICMELEE WINDUP
	Priority	3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	Assassins_Psi_Blade_Windup_Cycle

	Type		male
	  Anim		male\Assassins_Psi_Blade 1 15
	TEnd
MEnd

Move Assassins_Psi_Blade_Windup_Cycle
	Requires	COMBAT PSIONICMELEE WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	CombatReady

	Type		male
	  Anim		male\Assassins_Psi_Blade 15 30
	TEnd
MEnd

#############################################################

Move Assassins_Psi_Blade
	Requires	COMBAT PSIONICMELEE STAB
	Priority	3
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Assassins_Psi_Blade_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\Assassins_Psi_Blade 30 72
	TEnd
MEnd

Move Assassins_Psi_Blade_Post
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim		male\Assassins_Psi_Blade 73
	TEnd
MEnd

## Assassins Psi Blade Air #############################################

Move A_Assassins_Psi_Blade_Windup
	Requires	FLY COMBAT PSIONICMELEE WINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Assassins_Psi_Blade_Windup_Cycle

	Type		male
	  Anim		male\Assassins_Psi_Blade_air 1 15
	TEnd
MEnd

Move A_Assassins_Psi_Blade_Windup_Cycle
	Requires	FLY COMBAT PSIONICMELEE WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	CombatReady

	Type		male
	  Anim		male\Assassins_Psi_Blade_air 15 30
	TEnd
MEnd

#############################################################

Move A_Assassins_Psi_Blade
	Requires	FLY COMBAT PSIONICMELEE STAB
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Assassins_Psi_Blade_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\Assassins_Psi_Blade_air 30 72
	TEnd
MEnd

Move A_Assassins_Psi_Blade_Post
	Include sequencers\Members\Combat_air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim		male\Assassins_Psi_Blade_air 73
	TEnd
MEnd
