#############################################################
## Martial Arts - Custom Animations 1 - Shield versions
#############################################################

## MARTIAL ARTS - TIER 1 SHIELD (Thunder Kick)###########################################################

Move Shield_MA_Tier1_Alt1
	Requires	SHIELD COMBAT KUNGFU KICK ALTERNATE ONE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Shield_MA_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_kick 1 25
	TEnd
MEnd

Move Shield_MA_Tier1_Alt1_Post
	Requires	SHIELD COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ>
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_kick 25
	TEnd
MEnd

#############################################################

Move Shield_A_MA_Tier1_Alt1
	Requires	SHIELD FLY COMBAT KUNGFU KICK ALTERNATE ONE
	Priority	17
	Interrupts	<FLYAttackAll>
	Member		<DeathIRQ>
	NextMove        Shield_A_MA_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_kick 1 25
	TEnd
MEnd

Move Shield_A_MA_Tier1_Alt1_Post
	Requires	SHIELD FLY COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <FLYAttackAll>
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_kick 25
	TEnd
MEnd

## MARTIAL ARTS - TIER 2 SHIELD (Storm Kick) ###########################################################

Move MA_Tier2_Shield_Alt1
	Requires	COMBAT SHIELD KUNGFU KICK STRONG ALTERNATE ONE
	Priority        18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier2_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow_shield 1 25
	TEnd
MEnd

Move MA_Tier2_Shield_Alt1_Post
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow_shield 25
	TEnd
MEnd

#############################################################

Move A_MA_Tier2_Shield_Alt1
	Requires	COMBAT SHIELD KUNGFU KICK STRONG ALTERNATE ONE FLY
	Priority        28
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier2_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow_shield_air 1 25
	TEnd
MEnd

Move A_MA_Tier2_Shield_Alt1_Post
	Requires	COMBAT SHIELD FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow_shield_air 25
	TEnd
MEnd

## MARTIAL ARTS - TIER 3 SHIELD (Cobra Strike) ###########################################################

Move MA_Tier3_Shield_Alt1
	Requires	COMBAT KUNGFU PUNCH KUNGFUSTUN ALTERNATE ONE SHIELD
	Priority        18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier3_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_spin_left_backfist_shield 1 46
	TEnd
MEnd

Move MA_Tier3_Shield_Alt1_Post
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_spin_left_backfist_shield 46
	TEnd
MEnd

#############################################################

Move A_MA_Tier3_Shield_Alt1
	Requires	FLY COMBAT KUNGFU PUNCH KUNGFUSTUN ALTERNATE ONE SHIELD
	Priority        29
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier3_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Wind		ATTACK

	Type		male
	  Anim          male\MA_spin_left_backfist_shield_air 1 46
	TEnd
MEnd

Move A_MA_Tier3_Shield_Alt1_Post
	Requires	FLY COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_spin_left_backfist_shield_air 46
	TEnd
MEnd

## MARTIAL ARTS - TIER 5 SHIELD (Crane Kick) ###########################################################

Move Shield_MA_Tier5_Alt1
	Requires	SHIELD COMBAT KUNGFU HIGH KICK ALTERNATE ONE
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Shield_MA_Tier5_Alt1_Post
	Flags		NoSizeScale
	Sets            CANTMOVE
	Wind		ATTACK

	Type		male
	  Anim          male\shield_IronKick 34 83
	TEnd
MEnd

Move Shield_MA_Tier5_Alt1_Post
	Requires	SHIELD COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ>
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_IronKick 83
	TEnd
MEnd

#############################################################

Move A_Shield_MA_Tier5_Alt1
	Requires	SHIELD FLY COMBAT KUNGFU HIGH KICK ALTERNATE ONE
	Priority	28
	Interrupts	<FLYAttackAll>
	Member		<DeathIRQ>
	NextMove        A_Shield_MA_Tier5_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Wind		ATTACK

	Type		male
	  Anim          male\shield_air_IronKick 34 83
	TEnd
MEnd

Move A_Shield_MA_Tier5_Alt1_Post
	Requires	SHIELD FLY COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <FLYAttackAll>
	NextMove        Shield_CombatFly_HalfBlend
	Interpolate     0
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_IronKick 83
	TEnd
MEnd

## MARTIAL ARTS - TIER 7 SHIELD (Crippling Axe Kick)###########################################################

Move MA_Tier7_Shield_Alt1_Right
	Requires	COMBAT KUNGFU LOW KICK FEET ALTERNATE ONE SHIELD
	Priority        19
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier7_Shield_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker_shield 1 40
	TEnd
MEnd

Move MA_Tier7_Shield_Alt1_Right_Post
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker_shield 40
	TEnd
MEnd

#############################################################

Move A_MA_Tier7_Shield_Alt1_Right
	Requires	FLY COMBAT KUNGFU LOW KICK FEET ALTERNATE ONE SHIELD
	Priority        30
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier7_Shield_Alt1_Post_Right
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\haymaker_shield_air 1 40
	TEnd
MEnd

Move A_MA_Tier7_Shield_Alt1_Post_Right
	Requires	FLY COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\haymaker_shield_air 40 58
	TEnd
MEnd

## MARTIAL ARTS - TIER 8 SHIELD (Dragons Tail) ###########################################################

Move Shield_MA_Tier8_Alt1
	Requires	SHIELD COMBAT KUNGFU KUNGFUAREA ALTERNATE ONE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Shield_MA_Tier8_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_SweepKick 1 45
	TEnd
MEnd

Move Shield_MA_Tier8_Alt1_Post
	Requires	SHIELD COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ>
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_SweepKick 45
	TEnd
MEnd

#############################################################

Move A_Shield_MA_Tier8_Alt1
	Requires	SHIELD FLY COMBAT KUNGFU KUNGFUAREA ALTERNATE ONE
	Priority	27
	Interrupts	<FLYAttackAll>
	Member		<DeathIRQ>
	NextMove        A_Shield_MA_Tier8_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_SweepKick 40 85
	TEnd
MEnd

Move A_Shield_MA_Tier8_Alt1_Post
	Requires	SHIELD FLY COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <FLYAttackAll>
	NextMove        Shield_CombatFly_HalfBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\shield_air_SweepKick 85
	TEnd
MEnd

## MARTIAL ARTS - TIER 9 SHIELD (Eagles Claw) ###########################################################

Move Shield_MA_Tier9_Alt1
	Requires	SHIELD COMBAT KUNGFU KICK KUNGFUSTUN ALTERNATE ONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_MA_Tier9_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\baseball_punch_shield 1 44
	TEnd
MEnd

Move Shield_MA_Tier9_Alt1_2
	Requires	SHIELD COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove        Shield_MA_Tier9_Alt1_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\baseball_punch_shield 44 60
	TEnd
MEnd

Move Shield_MA_Tier9_Alt1_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\baseball_punch_shield 60 82
	TEnd
MEnd

#############################################################

Move A_Shield_MA_Tier9_Alt1
	Requires	SHIELD FLY COMBAT KUNGFU KICK KUNGFUSTUN ALTERNATE ONE
	Priority        28
	Interrupts	<DeathIRQ> <FlyAttackALL>
	Member		<DeathIRQ>
	NextMove        A_Shield_MA_Tier9_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch_shield 1 44
	TEnd
MEnd

Move A_Shield_MA_Tier9_Alt1_2
	Requires	SHIELD FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_MA_Tier9_Alt1_Post
	Sets            CANTMOVE
	Flags		ReqInputs NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\air_baseball_punch_shield 44 60
	TEnd
MEnd

Move A_Shield_MA_Tier9_Alt1_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Shield_CombatFly_Blend
	Flags		ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch_shield 60 82
	TEnd
MEnd

### MARTIAL ARTS - TIER 3 (Assassin's Blow) ###########################################################
#
#Move Shield_MA_Assassins_Blow_Alt1_Windup
#	Requires	SHIELD COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
#	Priority        5
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <CombatAttack>
#	NextMove        Shield_MA_Assassins_Blow_Alt1_Windup_Cycle
#	Flags		NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_right_uppercut_punch 1 29
#	TEnd
#MEnd
#
#Move Shield_MA_Assassins_Blow_Alt1_Windup_Cycle
#	Requires	SHIELD COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
#	Priority        5
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <CombatAttack>
#	Flags		Cycle NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_right_uppercut_punch 30 59
#	TEnd
#MEnd
#
##############################################################
#
#Move Shield_MA_Assassins_Blow_Alt1
#	Requires	SHIELD COMBAT MARTBACKSTAB ALTERNATE ONE
#	Priority        5
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ>
#	NextMove        Shield_MA_Assassins_Blow_Alt1_2
#	Sets            CANTMOVE
#	Flags		NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_right_uppercut_punch 60 90
#	TEnd
#MEnd
#
#Move Shield_MA_Assassins_Blow_Alt1_2
#	Requires	SHIELD COMBAT
#	Priority        2
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
#	NextMove        Shield_CombatReady_Blend
#	Flags		NoSizeScale #PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_right_uppercut_punch 91
#	TEnd
#MEnd
#
##############################################################
#
#Move Shield_A_MA_Assassins_Blow_Alt1_Windup
#	Requires	FLY SHIELD COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
#	Priority        6
#	Interrupts	<ReactIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <FLYAttackAll> <FlyCombat>
#	NextMove        Shield_A_MA_Assassins_Blow_Alt1_Windup_Cycle
#	Flags		NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_air_right_uppercut_punch 1 29
#	TEnd
#MEnd
#
#Move Shield_A_MA_Assassins_Blow_Alt1_Windup_Cycle
#	Requires	FLY SHIELD COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
#	Priority        6
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FLYAttackAll> <FlyCombat>
#	Flags		Cycle NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_air_right_uppercut_punch 30 59
#	TEnd
#MEnd
#
##############################################################
#
#Move Shield_A_MA_Assassins_Blow_Alt1
#	Requires	FLY SHIELD COMBAT MARTBACKSTAB ALTERNATE ONE
#	Priority        6
#	Interrupts	<FLYAttackAll>
#	Member		<DeathIRQ>
#	NextMove        Shield_A_MA_Assassins_Blow_Alt1_2
#	Sets            CANTMOVE
#	Flags		NoSizeScale PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_air_right_uppercut_punch 60 90
#	TEnd
#MEnd
#
#Move Shield_A_MA_Assassins_Blow_Alt1_2
#	Requires	FLY SHIELD COMBAT
#	Priority        3
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <FLYAttackAll> <BlockIRQ> <MoveIRQ>
#	NextMove        Shield_CombatFly_Blend
#	Flags		NoSizeScale #PitchToTarget
#
#	Type		male
#	  Anim          male\shield_MA_air_right_uppercut_punch 91
#	TEnd
#MEnd