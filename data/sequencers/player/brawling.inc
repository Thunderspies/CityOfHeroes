#############################################################
## Brawling
#############################################################

## Initial Strike  #############################################

Move Initial_Strike
	Requires	COMBAT BRAWLING QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>  #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Initial_Strike_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_initial_strike 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Initial_Strike_post
	Requires	COMBAT
	Include 	sequencers\Members\Combat.inc
	NextMove	Combatready
	Flags		ReqInputs

	Type		male
	  Anim          male\brawling_initial_strike 21
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike 21
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike 21
	TEnd

MEnd

## Initial Strike B  #############################################

Move Initial_Strike_B
	Requires	COMBAT BRAWLING QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Initial_Strike_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\brawling_initial_strike_B 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_B 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_B 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Initial_Strike_B_post
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_initial_strike_B 21
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_B 21
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_B 21
	TEnd
MEnd

## Initial Strike C  #############################################

Move Initial_Strike_C
	Requires	COMBAT BRAWLING QUICK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Initial_Strike_C_post
	Sets            CANTMOVE
	Flags 		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_initial_strike_C 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_C 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_C 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Initial_Strike_C_post
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_initial_strike_C 21
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_C 21
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_C 21
	TEnd
MEnd

## Initial Strike air #############################################

Move A_Initial_Strike
	Requires	FLY COMBAT BRAWLING QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Initial_Strike_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_initial_strike_air 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Initial_Strike_post
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_initial_strike_air 21
	TEnd
MEnd


## Initial Strike B air #############################################

Move A_Initial_Strike_B
	Requires	FLY COMBAT BRAWLING QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Initial_Strike_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\brawling_initial_strike_B_air 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Initial_Strike_B_post
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_initial_strike_B_air 21
	TEnd
MEnd


## Initial Strike C air #############################################

Move A_Initial_Strike_C
	Requires	FLY COMBAT BRAWLING QUICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Initial_Strike_C_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_initial_strike_C_air 1 20
 	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Initial_Strike_C_post
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\brawling_initial_strike_C_air 21
	TEnd
MEnd

## Initial Strike Hit Quick #############################################

Move Initial_Strike_HQ
	Requires	BRAWLING QUICK HIT
	Priority        3
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        Fake_CombatCycle
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	  PlayFX	Feet\MaleRight.fx 1
	  PlayFX	Feet\StartScuff.fx 25
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	  PlayFX	Feet\FemRight.fx 1
	  PlayFX	Feet\StartScuff.fx 25
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	  PlayFX	Feet\HugeRight.fx 1
	  PlayFX	Feet\StartScuff.fx 25
	TEnd

MEnd

Move C_Initial_Strike_HQ
	Requires	COMBAT BRAWLING QUICK HIT
	Priority        4
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

## Initial Strike Hit Quick Weapon #############################################

Move W_Initial_Strike_HQ
	Requires	BRAWLING QUICK HIT WEAPON
	Priority        4
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        WepReady
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

Move W_C_Initial_Strike_HQ
	Requires	COMBAT BRAWLING QUICK HIT WEAPON
	Priority        5
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        WepReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

## Initial Strike Hit Quick Shot Gun #############################################

Move SG_Initial_Strike_HQ
	Requires	BRAWLING QUICK HIT WEAPON GUN
	Priority        5
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

Move SG_C_Initial_Strike_HQ
	Requires	COMBAT BRAWLING QUICK HIT WEAPON GUN
	Priority        6
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

## Initial Strike Hit Quick Air #############################################

Move A_Initial_Strike_HQ
	Requires	FLY BRAWLING QUICK HIT
	Priority        14
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions_air.inc
	NextMove        C_flyready
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ_air 1
	TEnd

MEnd

Move A_C_Initial_Strike_HQ
	Requires	FLY COMBAT BRAWLING QUICK HIT
	Priority        15
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions_air.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ_air 1
	TEnd

MEnd

## Heavy Blow #############################################

Move Heavyblow
	Requires	COMBAT BRAWLING STRONG SLASH
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Heavyblow_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_heavy_blow 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_heavy_blow 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_heavy_blow 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Heavyblow_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_heavy_blow 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

	Type		fem
	  Anim          fem\brawling_heavy_blow 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	TEnd

	Type		huge
	  Anim          huge\brawling_heavy_blow 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	TEnd

MEnd

## Heavy Blow air #############################################

Move A_Heavyblow
	Requires	FLY COMBAT BRAWLING STRONG SLASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Heavyblow_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\brawling_heavy_blow_air 1 32
	  PitchAngle	0.4
	  PitchEnd	50
	TEnd
MEnd

Move A_Heavyblow_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_heavy_blow_air 33 50
	TEnd
MEnd

## Heavy Blow B #############################################

Move Heavyblow_B
	Requires	COMBAT BRAWLING STRONG SLASH
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Heavyblow_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\brawling_heavy_blow_B 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.4
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_heavy_blow_B 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.4
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_heavy_blow_B 1 32
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.4
	  PitchEnd	50
	TEnd
MEnd

Move Heavyblow_B_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_heavy_blow_B 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

	Type		fem
	  Anim          fem\brawling_heavy_blow_B 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	TEnd

	Type		huge
	  Anim          huge\brawling_heavy_blow_B 33 50
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	TEnd

MEnd

## Heavy Blow air #############################################

Move A_Heavyblow_B
	Requires	FLY COMBAT BRAWLING STRONG SLASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Heavyblow_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\brawling_heavy_blow_B_air 1 32
	  PitchAngle	0.4
	  PitchEnd	50
	TEnd
MEnd

Move A_Heavyblow_B_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_heavy_blow_B_air 33 50
	TEnd
MEnd

## Sweep Cross #############################################

Move Sweep_cross
	Requires	COMBAT BRAWLING SWEEPATTACK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Sweep_cross_post
	Flags		PitchToTarget
	Sets            CANTMOVE

	Type		male
	  Anim          male\brawling_sweep_cross 1 35
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_sweep_cross 1 35
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_sweep_cross 1 35
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Sweep_cross_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_sweep_cross 36 69
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	  PlayFX	Feet\StartScuff.fx 20
	  PlayFX	Feet\MaleRight.fx 25
	TEnd

	Type		fem
	  Anim          fem\brawling_sweep_cross 36 69
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	  PlayFX	Feet\StartScuff.fx 20
	  PlayFX	Feet\FemRight.fx 25
	TEnd

	Type		huge
	  Anim          huge\brawling_sweep_cross 36 69
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	  PlayFX	Feet\StartScuff.fx 20
	  PlayFX	Feet\HugeRight.fx 25
	TEnd

MEnd

## Sweep Cross Air#############################################

Move A_Sweep_cross
	Requires	FLY COMBAT BRAWLING SWEEPATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Sweep_cross_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_sweep_cross_air 1 35
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Sweep_cross_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\brawling_sweep_cross_air 36 69
	TEnd
MEnd

## Rib Cracker #############################################

Move Rib_Cracker
	Requires	COMBAT BRAWLING MID FIERCE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Rib_Cracker_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_rib_cracker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\MaleRight.fx 37
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_rib_cracker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\FemRight.fx 37
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_rib_cracker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\HugeRight.fx 37
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Rib_Cracker_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_rib_cracker 41 49
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

	Type		fem
	  Anim          fem\brawling_rib_cracker 41 49
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	TEnd

	Type		huge
	  Anim          huge\brawling_rib_cracker 41 49
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	TEnd

MEnd

## Rib Cracker Air #############################################

Move A_Rib_Cracker
	Requires	FLY COMBAT BRAWLING MID FIERCE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Rib_Cracker_post
	Sets            CANTMOVE
	Flags 		PitchToTarget

	Type		male
	  Anim          male\brawling_rib_cracker_air 1 40
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Rib_Cracker_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_rib_cracker_air 41 49
	TEnd
MEnd

## Assassins Strike #############################################

Move Assassins_Strike_Windup
	Requires	COMBAT BRAWLING WINDUP
	Priority	3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove	Assassins_Strike_Windup_Cycle
	Flags            PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike 1 14
	  PlayFX	Feet\StartScuff.fx 5
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike 1 14
	  PlayFX	Feet\StartScuff.fx 5
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike 1 14
	  PlayFX	Feet\StartScuff.fx 5
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Assassins_Strike_Windup_Cycle
	Requires	COMBAT BRAWLING WINDUP
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale Cycle PitchToTarget
	NextMove	CombatReady

	Type		male
	  Anim          male\brawling_assassins_strike 15 30
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike 15 30
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike 15 30
	TEnd

MEnd

#############################################################

Move Assassins_Strike
	Requires	COMBAT BRAWLING LAUNCH
	Priority	3
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Assassins_Strike_Post
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	TEnd
MEnd

## Assassins Strike Air #############################################

Move A_Assassins_Strike_Windup
	Requires	FLY COMBAT BRAWLING WINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_air.inc
	NextMove	A_Assassins_Strike_Windup_Cycle
	#Sets            CANTMOVE
	Flags		PitchToTarget nosizescale

	Type		male
	  Anim          male\brawling_assassins_strike_air 1 14
  	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Assassins_Strike_Windup_Cycle
	Requires	FLY COMBAT BRAWLING WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle PitchToTarget
	NextMove	C_flyready

	Type		male
	  Anim          male\brawling_assassins_strike_air 15 30
	TEnd
MEnd

#############################################################

Move A_Assassins_Strike
	Requires	FLY COMBAT BRAWLING LAUNCH
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_air 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Assassins_Strike_Post
	Include 	sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_air 65 77
	TEnd
MEnd

## Assassins Strike B#############################################


Move Assassins_Strike_B
	Requires	COMBAT BRAWLING LAUNCH
	Priority	3
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Assassins_Strike_B_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_B 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike_B 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike_B 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Assassins_Strike_B_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_B 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

	Type		fem
	  Anim          fem\brawling_assassins_strike_B 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	TEnd

	Type		huge
	  Anim          huge\brawling_assassins_strike_B 65 77
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	TEnd
MEnd

## Assassins Strike Air #############################################

Move A_Assassins_Strike_B
	Requires	FLY COMBAT BRAWLING LAUNCH
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Assassins_Strike_B_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_B_air 31 64
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Assassins_Strike_B_Post
	Include 	sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_assassins_strike_B_air 65 77
	TEnd
MEnd

## Shin Breaker #############################################

Move Shin_Breaker
	Requires	COMBAT BRAWLING LOW KICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Shin_Breaker_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_shin_breaker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\StartScuff.fx 35
	  PlayFX	Feet\MaleRight.fx 35
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_shin_breaker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\StartScuff.fx 35
	  PlayFX	Feet\FemRight.fx 35
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_shin_breaker 1 40
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\StartScuff.fx 35
	  PlayFX	Feet\HugeRight.fx 35
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Shin_Breaker_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_shin_breaker 41 45
	  PlayFX	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\brawling_shin_breaker 41 45
	  PlayFX	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\brawling_shin_breaker 41 45
	  PlayFX	Feet\StartScuff.fx 1
	TEnd
MEnd

## Shin Breaker Air #############################################

Move A_Shin_Breaker
	Requires	FLY COMBAT BRAWLING LOW KICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Shin_Breaker_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_shin_breaker_air 1 40
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Shin_Breaker_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc
	Type		male
	  Anim          male\brawling_shin_breaker_air 41 45
	TEnd
MEnd

## Spinning Strike #############################################

Move Spinning_Strike
	Requires	COMBAT BRAWLING SPIN TWO
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Spinning_Strike_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_spinning_strike 1 53
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\MaleRight.fx 35
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\MaleRight.fx 50
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike 1 53
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\FemRight.fx 35
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\FemRight.fx 50
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike 1 53
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	  PlayFX	Feet\StartScuff.fx 30
	  PlayFX	Feet\HugeRight.fx 35
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\HugeRight.fx 50
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Spinning_Strike_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc


	Type		male
	  Anim          male\brawling_spinning_strike 54 58
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 4
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike 54 58
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 4
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike 54 58
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 4
	TEnd
MEnd

## Spinning Strike Air #############################################

Move A_Spinning_Strike
	Requires	FLY COMBAT BRAWLING SPIN TWO
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Spinning_Strike_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_spinning_strike_air 1 53
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Spinning_Strike_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc
	Type		male
	  Anim          male\brawling_spinning_strike_air 54 58
	TEnd
MEnd

## Spinning Strike Hit #############################################

Move Spinning_Strike_Hit
	Requires	BRAWLING HIT SPIN
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Fake_CombatCycle
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

Move C_Spinning_Strike_Hit
	Requires	COMBAT BRAWLING HIT SPIN
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

## Spinning Strike Hit Weapon #############################################

Move W_Spinning_Strike_Hit
	Requires	BRAWLING HIT SPIN WEAPON
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        WepReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

Move W_C_Spinning_Strike_Hit
	Requires	COMBAT BRAWLING HIT SPIN WEAPON
	Priority        5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        WepReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

## Spinning Strike Hit Shot Gun #############################################

Move SG_Spinning_Strike_Hit
	Requires	BRAWLING HIT SPIN WEAPON GUN
	Priority        5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

Move SG_C_Spinning_Strike_Hit
	Requires	COMBAT BRAWLING HIT SPIN WEAPON GUN
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

## Spinning Strike Hit Air#############################################

Move A_Spinning_Strike_Hit
	Requires	FLY BRAWLING HIT SPIN
	Priority        14
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_air.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_air 1
	TEnd

MEnd

Move A_C_Spinning_Strike_Hit
	Requires	FLY COMBAT BRAWLING HIT SPIN
	Priority        15
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_air.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_air 1
	TEnd

MEnd

## Crushing uppercut #############################################

Move Crushing_Uppercut
	Requires	COMBAT BRAWLING UPPERCUT FIERCE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Crushing_Uppercut_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_crushing_uppercut 1 73
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\MaleRight.fx 55
	  PlayFX	Feet\StartScuff.fx 65
	  PlayFX	Feet\MaleRight.fx 70
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\brawling_crushing_uppercut 1 73
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\FemRight.fx 10
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\FemRight.fx 55
	  PlayFX	Feet\StartScuff.fx 65
	  PlayFX	Feet\FemRight.fx 70
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\brawling_crushing_uppercut 1 73
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\HugeRight.fx 10
	  PlayFX	Feet\StartScuff.fx 45
	  PlayFX	Feet\HugeRight.fx 55
	  PlayFX	Feet\StartScuff.fx 65
	  PlayFX	Feet\HugeRight.fx 70
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move Crushing_Uppercut_post
	Requires	COMBAT
	NextMove	CombatReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\brawling_crushing_uppercut 74 88
	  PlayFX	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\brawling_crushing_uppercut 74 88
	  PlayFX	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\brawling_crushing_uppercut 74 88
	  PlayFX	Feet\StartScuff.fx 1
	TEnd
MEnd

## Crushing uppercut Air #############################################

Move A_Crushing_Uppercut
	Requires	FLY COMBAT BRAWLING UPPERCUT FIERCE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Crushing_Uppercut_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\brawling_crushing_uppercut_air 1 73
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd
MEnd

Move A_Crushing_Uppercut_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_air.inc

	Type		male
	  Anim          male\brawling_crushing_uppercut_air 74 88
	TEnd
MEnd



