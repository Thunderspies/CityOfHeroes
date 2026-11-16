#############################################################
## Kinetic Melee
#############################################################

## Quick Strike A #############################################

Move Quick_strike_A
	Requires	COMBAT ENERGY DRAIN QUICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Quick_strike_A_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A 1 25
	TEnd
MEnd

Move Quick_strike_A_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A 26
	TEnd
MEnd

## Quick Strike A air #############################################

Move A_Quick_strike_A
	Requires	FLY COMBAT ENERGY DRAIN QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Quick_strike_A_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A_air 1 25
	TEnd
MEnd

Move A_Quick_strike_A_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A_air 26
	TEnd
MEnd

## Quick Strike A Shield #############################################

Move Shield_Quick_strike_A
	Requires	SHIELD COMBAT ENERGY DRAIN QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Quick_strike_A_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A_Shield 1 25
	TEnd
MEnd

Move Shield_Quick_strike_A_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A_Shield 26
	TEnd
MEnd

## Quick Strike A Shield Air#############################################

Move A_Shield_Quick_strike_A
	Requires	FLY SHIELD COMBAT ENERGY DRAIN QUICK
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Quick_strike_A_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\kinetic_quick_strike_A_Shield_air 1 25
	TEnd
MEnd

Move A_Shield_Quick_strike_A_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc
	Type		male
	  Anim          male\kinetic_quick_strike_A_Shield_air 26
	TEnd
MEnd

## Quick Strike B #############################################

Move Quick_strike_B
	Requires	COMBAT ENERGY DRAIN QUICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Quick_strike_B_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH LEFT
	Type		male
	  Anim          male\kinetic_quick_strike_B 1 25
	TEnd
MEnd

Move Quick_strike_B_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_quick_strike_B 26
	TEnd
MEnd

## Quick Strike B air #############################################

Move A_Quick_strike_B
	Requires	FLY COMBAT ENERGY DRAIN QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Quick_strike_B_post
	Sets            CANTMOVE
	SticksOnChild	PUNCH LEFT
	Type		male
	  Anim          male\kinetic_quick_strike_B_air 1 25
	TEnd
MEnd

Move A_Quick_strike_B_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_quick_strike_B_air 26
	TEnd
MEnd

## BodyBlow #############################################

Move Bodyblow
	Requires	COMBAT ENERGY DRAIN BLOW
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Bodyblow_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\kinetic_bodyblow 1 32
	TEnd
MEnd

Move Bodyblow_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_bodyblow 32
	TEnd
MEnd

## BodyBlow air #############################################

Move A_Bodyblow
	Requires	FLY COMBAT ENERGY DRAIN BLOW
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Bodyblow_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\kinetic_bodyblow_air 1 32
	TEnd
MEnd

Move A_Bodyblow_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_bodyblow_Air 32
	TEnd
MEnd

## BodyBlow Shield #############################################

Move Shield_Bodyblow
	Requires	SHIELD COMBAT ENERGY DRAIN BLOW
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Bodyblow_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\kinetic_bodyblow_Shield 1 32
	TEnd
MEnd

Move Shield_Bodyblow_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc
	Type		male
	  Anim          male\kinetic_bodyblow_Shield 32
	TEnd
MEnd

## BodyBlow Shield Air#############################################

Move A_Shield_Bodyblow
	Requires	FLY SHIELD COMBAT ENERGY DRAIN BLOW
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Bodyblow_post
	Sets            CANTMOVE
	Type		male
	  Anim          male\kinetic_bodyblow_Shield_air 1 32
	TEnd
MEnd

Move A_Shield_Bodyblow_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc
	Type		male
	  Anim          male\kinetic_bodyblow_Shield_air 32
	TEnd
MEnd

## Smashing Blow #############################################

Move Smashingblow
	Requires	COMBAT ENERGY DRAIN SMASH
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Smashingblow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_smashingblow 1 36
	TEnd
MEnd

Move Smashingblow_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\kinetic_smashingblow 36
	TEnd
MEnd

## Smashing Blow Air#############################################

Move A_Smashingblow
	Requires	FLY COMBAT ENERGY DRAIN SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Smashingblow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_smashingblow_air 1 36
	TEnd
MEnd

Move A_Smashingblow_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\kinetic_smashingblow_air 36
	TEnd
MEnd

## Smashing Blow Shield#############################################

Move Shield_Smashingblow
	Requires	SHIELD COMBAT ENERGY DRAIN SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Smashingblow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_smashingblow_Shield 1 36
	TEnd
MEnd

Move Shield_Smashingblow_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc

	Type		male
	  Anim          male\kinetic_smashingblow_Shield 36
	TEnd
MEnd

## Smashing Blow Shield Air #############################################

Move A_Shield_Smashingblow
	Requires	FLY SHIELD COMBAT ENERGY DRAIN SMASH
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Smashingblow_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_smashingblow_Shield_air 1 36
	TEnd
MEnd

Move A_Shield_Smashingblow_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc

	Type		male
	  Anim          male\kinetic_smashingblow_Shield_air 36
	TEnd
MEnd

## Repulsing Torrent #############################################

Move Repulsing_Torrent
	Requires	COMBAT ENERGY DRAIN REPULSION
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Repulsing_Torrent_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_repulsing_torrent 1 60
	TEnd
MEnd

Move Repulsing_Torrent_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_repulsing_torrent 61 80
	TEnd
MEnd

## Repulsing Torrent Air#############################################

Move A_Repulsing_Torrent
	Requires	FLY COMBAT ENERGY DRAIN REPULSION
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Repulsing_Torrent_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_repulsing_torrent_air 1 60
	TEnd
MEnd

Move A_Repulsing_Torrent_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_repulsing_torrent_air 61 80
	TEnd
MEnd

## Repulsing Torrent Shield#############################################

Move Shield_Repulsing_Torrent
	Requires	SHIELD COMBAT ENERGY DRAIN REPULSION
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Repulsing_Torrent_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_repulsing_torrent_Shield 1 60
	TEnd
MEnd

Move Shield_Repulsing_Torrent_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc
	Type		male
	  Anim          male\kinetic_repulsing_torrent_Shield 61 80
	TEnd
MEnd

## Repulsing Torrent Shield Air#############################################

Move A_Shield_Repulsing_Torrent
	Requires	FLY SHIELD COMBAT ENERGY DRAIN REPULSION
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Repulsing_Torrent_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_repulsing_torrent_Shield_air 1 60
	TEnd
MEnd

Move A_Shield_Repulsing_Torrent_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc
	Type		male
	  Anim          male\kinetic_repulsing_torrent_Shield_air 61 80
	TEnd
MEnd

## Burst #############################################

Move Kinetic_Burst
	Requires	COMBAT ENERGY DRAIN BURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Kinetic_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_burst 1 80
	TEnd
MEnd

Move Kinetic_Burst_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_burst 81
	TEnd
MEnd

## Burst Air #############################################

Move A_Kinetic_Burst
	Requires	FLY COMBAT ENERGY DRAIN BURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Kinetic_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_burst_air 1 80
	TEnd
MEnd

Move A_Kinetic_Burst_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\kinetic_burst_air 81
	TEnd
MEnd

## Burst Shield #############################################

Move Shield_Kinetic_Burst
	Requires	SHIELD COMBAT ENERGY DRAIN BURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Kinetic_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_burst_Shield 1 80
	TEnd
MEnd

Move Shield_Kinetic_Burst_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc
	Type		male
	  Anim          male\kinetic_burst_Shield 81
	TEnd
MEnd

## Burst Shield Air #############################################

Move A_Shield_Kinetic_Burst
	Requires	FLY SHIELD COMBAT ENERGY DRAIN BURST
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Kinetic_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_burst_Shield_air 1 80
	TEnd
MEnd

Move A_Shield_Kinetic_Burst_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc
	Type		male
	  Anim          male\kinetic_burst_Shield_air 81
	TEnd
MEnd

## Focused Burst #############################################

Move Focused_Burst
	Requires	COMBAT ENERGY DRAIN HEAVYBURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Focused_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_focused_burst 1 60

	TEnd
MEnd

Move Focused_Burst_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc
	Type		male
	  Anim          male\kinetic_focused_burst 61
	TEnd
MEnd

## Focused Burst Air #############################################

Move A_Focused_Burst
	Requires	FLY COMBAT ENERGY DRAIN HEAVYBURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Focused_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_focused_burst_air 1 60

	TEnd
MEnd

Move A_Focused_Burst_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc
	Type		male
	  Anim          male\kinetic_focused_burst_air 61
	TEnd
MEnd

## Focused Burst Shield #############################################

Move Shield_Focused_Burst
	Requires	SHIELD COMBAT ENERGY DRAIN HEAVYBURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Focused_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_focused_burst_Shield 1 60

	TEnd
MEnd

Move Shield_Focused_Burst_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc
	Type		male
	  Anim          male\kinetic_focused_burst_Shield 61
	TEnd
MEnd

## Focused Burst Shield Air #############################################

Move A_Shield_Focused_Burst
	Requires	FLY SHIELD COMBAT ENERGY DRAIN HEAVYBURST
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Focused_Burst_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_focused_burst_Shield_air 1 60

	TEnd
MEnd

Move A_Shield_Focused_Burst_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc
	Type		male
	  Anim          male\kinetic_focused_burst_Shield_air 61
	TEnd
MEnd

## Total Focus #############################################

Move Total_Focus
	Requires	COMBAT ENERGY DRAIN WHIRLWIND
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Total_Focus_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_total_focus 1 85
	TEnd
MEnd

Move Total_Focus_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\kinetic_total_focus 86
	TEnd
MEnd

## Total Focus Air #############################################

Move A_Total_Focus
	Requires	FLY COMBAT ENERGY DRAIN WHIRLWIND
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Total_Focus_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_total_focus_air 1 85
	TEnd
MEnd

Move A_Total_Focus_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc

	Type		male
	  Anim          male\kinetic_total_focus_air 86
	TEnd
MEnd

## Total Focus Shield #############################################

Move Shield_Total_Focus
	Requires	SHIELD COMBAT ENERGY DRAIN WHIRLWIND
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shield_Total_Focus_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_total_focus_Shield 1 85
	TEnd
MEnd

Move Shield_Total_Focus_post
	Requires	SHIELD COMBAT
	NextMove	Shield_CombatReady_Blend
	Include 	sequencers\Members\Combat_Shield.inc

	Type		male
	  Anim          male\kinetic_total_focus_Shield 86
	TEnd
MEnd

## Total Focus Shield Air #############################################

Move A_Shield_Total_Focus
	Requires	FLY SHIELD COMBAT ENERGY DRAIN WHIRLWIND
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shield_Total_Focus_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\kinetic_total_focus_Shield_air 1 85
	TEnd
MEnd

Move A_Shield_Total_Focus_post
	Requires	FLY SHIELD COMBAT
	NextMove	Shield_CombatFly_Blend
	Include 	sequencers\Members\Combat_Shield_air.inc

	Type		male
	  Anim          male\kinetic_total_focus_Shield_air 86
	TEnd
MEnd

## Kinetic Assassins Strike #############################################

Move Kinetic_Assassins_Windup
	Requires	COMBAT KINETICBACKSTAB WINDUP
	Priority	3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	Kinetic_Assassins_Windup_Cycle

	Type		male
	  Anim		male\kinetic_assassins_strike 1 42
	TEnd
MEnd

Move Kinetic_Assassins_Windup_Cycle
	Requires	COMBAT KINETICBACKSTAB WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	CombatReady

	Type		male
	  Anim		male\kinetic_assassins_strike 43 60
	TEnd
MEnd

#############################################################

Move Kinetic_Assassins_Strike
	Requires	COMBAT KINETICBACKSTAB ATTACK
	Priority	3
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Kinetic_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike 61 81
	TEnd
MEnd

Move Kinetic_Assassins_Strike_Post
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike 81
	TEnd
MEnd

## Kinetic Assassins Strike Air #############################################

Move A_Kinetic_Assassins_Windup
	Requires	FLY COMBAT KINETICBACKSTAB WINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Kinetic_Assassins_Windup_Cycle

	Type		male
	  Anim		male\kinetic_assassins_strike_air 1 42
	TEnd
MEnd

Move A_Kinetic_Assassins_Windup_Cycle
	Requires	FLY COMBAT KINETICBACKSTAB WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	C_flyready

	Type		male
	  Anim		male\kinetic_assassins_strike_air 43 60
	TEnd
MEnd

#############################################################

Move A_Kinetic_Assassins_Strike
	Requires	FLY COMBAT KINETICBACKSTAB ATTACK
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Kinetic_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_air 61 81
	TEnd
MEnd

Move A_Kinetic_Assassins_Strike_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_air 81
	TEnd
MEnd

## Kinetic Assassins Strike Shield #############################################

Move Shield_Kinetic_Assassins_Windup
	Requires	SHIELD COMBAT KINETICBACKSTAB WINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	Shield_Kinetic_Assassins_Windup_Cycle

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield 1 42
	TEnd
MEnd

Move Shield_Kinetic_Assassins_Windup_Cycle
	Requires	SHIELD COMBAT KINETICBACKSTAB WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	Shield_CombatReady_Blend

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield 43 60
	TEnd
MEnd

#############################################################

Move Shield_Kinetic_Assassins_Strike
	Requires	SHIELD COMBAT KINETICBACKSTAB ATTACK
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Kinetic_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield 61 81
	TEnd
MEnd

Move Shield_Kinetic_Assassins_Strike_Post
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield 81
	TEnd
MEnd

## Kinetic Assassins Strike Shield Air #############################################

Move A_Shield_Kinetic_Assassins_Windup
	Requires	FLY SHIELD COMBAT KINETICBACKSTAB WINDUP
	Priority	25
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Shield_Kinetic_Assassins_Windup_Cycle

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield_air 1 42
	TEnd
MEnd

Move A_Shield_Kinetic_Assassins_Windup_Cycle
	Requires	FLY SHIELD COMBAT KINETICBACKSTAB WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	#CycleMove	Kinetic_Assassins_Strike
	NextMove	Shield_CombatFly_Blend

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield_air 43 60
	TEnd
MEnd

#############################################################

Move A_Shield_Kinetic_Assassins_Strike
	Requires	FLY SHIELD COMBAT KINETICBACKSTAB ATTACK
	Priority	25
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Kinetic_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield_air 61 81
	TEnd
MEnd

Move A_Shield_Kinetic_Assassins_Strike_Post
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim		male\kinetic_assassins_strike_Shield_air 81
	TEnd
MEnd

###################
###### Power Siphon Steal power  ######
###################

Move Power_Siphon_NotRooted
	Requires	SIPHON NOTROOTED
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<attackirq> <deathirq>
	Include sequencers\Members\Combat.inc
	NextMove        Power_Siphon_post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\graspearth 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\graspearth 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move Power_Siphon_post
	Include sequencers\Members\Combat.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\graspearth 58
	TEnd

	Type		huge
	  Anim          huge\graspearth 58
	TEnd

	Type		fem
	  Anim          fem\steal 58
	TEnd
MEnd

###################
###### Power Siphon Air Steal power  ######
###################

Move A_Power_Siphon_NotRooted
	Requires	FLY SIPHON NOTROOTED
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<attackirq> <deathirq>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Power_Siphon_post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Power_Siphon_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Fake_C_flycycle
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd
MEnd