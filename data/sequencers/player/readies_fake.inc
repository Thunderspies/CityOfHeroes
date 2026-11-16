#############################################################
## FakeReadies.inc - Duplicates of various 'ready' cycles that don't require all AnimBits
#############################################################

## FAKE COMBAT CYCLE (DOESN'T REQUIRE COMBAT BIT) ###########################################################

Move Fake_CombatCycle_Half
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\combat_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 15 31
	TEnd
MEnd

Move Fake_CombatCycle
	Include sequencers\Members\Readies_Fake.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
	Type		huge
	  Anim          huge\combat_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 1 31
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd

## FAKE C_FLYREADY (DOESN'T REQUIRE COMBAT BIT) ###########################################################

Move Fake_CFlyReady
	Requires	FLY
	Include sequencers\Members\Readies_Fake_Air.inc
	Interpolate	15
	Flags		Cycle
	NextMove	C_PreFall
	Wind		FLY

	Type		male
	  Anim		male\air_combatready 25	65
	TEnd
MEnd

## FAKE COMBAT FLY CYCLE (DOESN'T REQUIRE COMBAT BIT) ###########################################################

Move Fake_C_flycycle_Half
	Requires	FLY
	Include sequencers\Members\Readies_Fake_Air.inc
	NextMove	C_PreFall
	Interpolate	15
	Wind		FLY
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 15 31
	TEnd

	Type		huge
	  Anim		huge\air_combat_cycle 15 31
	TEnd

	Type		fem
	  Anim		fem\air_combat_cycle 15 31
	TEnd
MEnd

Move Fake_C_flycycle
	Requires	FLY
	Include sequencers\Members\Readies_Fake_Air.inc
	Flags		Cycle
	NextMove	C_PreFall
	Wind		FLY
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 1	31
	TEnd

	Type		huge
	  Anim		huge\air_combat_cycle 1	31
	TEnd

	Type		fem
	  Anim		fem\air_combat_cycle 1 31
	TEnd
MEnd

## FAKE	DUAL MODE (GOBLIN DUAL POSE) ###########################################################

Move Fake_DUALReady
	Requires	COMBAT WEAPON DUAL
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <DualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\goblin_dual_cycle 1
	TEnd
MEnd