#############################################################
## Water Blast
#############################################################

## Aqua Bolt A #############################################

Move Aqua_Bolt_A
	Requires	COMBAT WATERBLAST QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Aqua_Bolt_A_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A 1 30
	TEnd
MEnd

Move Aqua_Bolt_A_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A 31
	TEnd
MEnd

## Aqua_Bolt_A air #############################################

Move A_Aqua_Bolt_A
	Requires	FLY COMBAT WATERBLAST QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Aqua_Bolt_A_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A_air 1 30
	TEnd
MEnd

Move A_Aqua_Bolt_A_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A_air 31
	TEnd
MEnd

## Quick Strike B #############################################

Move Aqua_Bolt_B
	Requires	COMBAT WATERBLAST QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Aqua_Bolt_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT
	Type		male
	  Anim          male\kinetic_quick_strike_B 1 30
	TEnd
MEnd

Move Aqua_Bolt_B_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_quick_strike_B 31
	TEnd
MEnd

## Aqua_Bolt B air #############################################

Move A_Aqua_Bolt_B
	Requires	FLY COMBAT WATERBLAST QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Aqua_Bolt_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT
	Type		male
	  Anim          male\kinetic_quick_strike_B_air 1 30
	TEnd
MEnd

Move A_Aqua_Bolt_B_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_quick_strike_B_air 31
	TEnd
MEnd

## Hydro Blast #############################################

Move Hydro_Blast
	Requires	COMBAT WATERBLAST BLOW
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Hydro_Blast_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	Type		male
	  Anim          male\hydro_blast 1 50
	TEnd
MEnd

Move Hydro_Blast_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\hydro_blast 51
	TEnd
MEnd

### Hydro Blast air #############################################

Move A_Hydro_Blast
	Requires	FLY COMBAT WATERBLAST BLOW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Hydro_Blast_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	Type		male
	  Anim          male\hydro_blast_air 1 50
	TEnd
MEnd

Move A_Hydro_Blast_post
	Requires	FLY COMBAT
	Include 	sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\hydro_blast_air 51
	TEnd
MEnd

## Water Burst #############################################

Move Water_Burst
	Requires	COMBAT WATERBLAST BURST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Water_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\water_burst 1 51
	TEnd
MEnd

Move Water_Burst_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\water_burst 52
	TEnd
MEnd

## Water Burst Air#############################################

Move A_Water_Burst
	Requires	FLY COMBAT WATERBLAST BURST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Water_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\water_burst_air 1 51
	TEnd
MEnd

Move A_Water_Burst_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\water_burst_air 52
	TEnd
MEnd


## Dehydrate #############################################

Move Dehydrate
	Requires	COMBAT WATERBLAST DRAIN
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Dehydrate_post
	Sets            CANTMOVE
	Flags		PitchToTarget


	Type		male
	  Anim          male\Dehydrate 1 56
	TEnd
MEnd

Move Dehydrate_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\Dehydrate 57
	TEnd
MEnd

## Dehydrate Air#############################################

Move A_Dehydrate
	Requires	FLY COMBAT WATERBLAST DRAIN
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Dehydrate_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\Dehydrate_air 1 56
	TEnd
MEnd

Move A_Dehydrate_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\Dehydrate_air 57
	TEnd
MEnd

## Water Jet #############################################

Move WaterJet
	Requires	COMBAT WATERBLAST BLAST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	WaterJet_post
	Flags		PitchToTarget
	Sets            CANTMOVE

	Type		male
	  Anim          male\Water_jet 1 30
	TEnd
MEnd

Move WaterJet_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\water_jet 31
	TEnd
MEnd

## Water Jet Air #############################################

Move A_WaterJet
	Requires	FLY COMBAT WATERBLAST BLAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_WaterJet_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\water_jet_air 1 30
	TEnd
MEnd

Move A_WaterJet_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\water_jet_air 31
	TEnd
MEnd

## Water Jet Long #############################################

Move WaterJet_long
	Requires	COMBAT WATERBLAST MEGABLAST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	WaterJet_long_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\Water_jet_long 1 43
	TEnd
MEnd

Move WaterJet_long_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\water_jet_long 44
	TEnd
MEnd

## Water Jet Air #############################################

Move A_WaterJet_long
	Requires	FLY COMBAT WATERBLAST MEGABLAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_WaterJet_long_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\water_jet_long_air 1 43
	TEnd
MEnd

Move A_WaterJet_long_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\water_jet_long_air 44
	TEnd
MEnd

## Steam Spray #############################################

Move Steam_Spray
	Requires	COMBAT WATERBLAST CONE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Steam_Spray_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\steam_spray 1 76

	TEnd
MEnd

Move Steam_Spray_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\steam_spray 77
	TEnd
MEnd

## Steam Spray Air #############################################

Move A_Steam_Spray
	Requires	FLY COMBAT WATERBLAST CONE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Steam_Spray_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\steam_spray_air 1 76

	TEnd
MEnd

Move A_Steam_Spray_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc
	Type		male
	  Anim          male\steam_spray_air 77
	TEnd
MEnd


## Geyser #############################################

Move Geyser
	Requires	COMBAT WATERBLAST STRONG BURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Geyser_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Geyser 1 85
	TEnd
MEnd

Move Geyser_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\Geyser 86
	TEnd
MEnd

## Geyser Air #############################################

Move A_Geyser
	Requires	FLY COMBAT WATERBLAST STRONG BURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Geyser_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\Geyser_air 1 85
	TEnd
MEnd

Move A_Geyser_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc

	Type		male
	  Anim          male\Geyser_air 86
	TEnd
MEnd
