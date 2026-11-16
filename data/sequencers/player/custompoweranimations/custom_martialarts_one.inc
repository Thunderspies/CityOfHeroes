#############################################################
## Martial Arts - Custom Animations 1
#############################################################

## MARTIAL ARTS - TIER 1 (Thunder Kick)###########################################################

Move MA_Tier1_Alt1
	Requires	COMBAT KUNGFU KICK ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_kick 1 25
	TEnd
MEnd

Move MA_Tier1_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_kick 26
	TEnd
MEnd

#############################################################

Move A_MA_Tier1_Alt1
	Requires	FLY COMBAT KUNGFU KICK ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_kick 1 25
	TEnd
MEnd

Move A_MA_Tier1_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_kick 25
	TEnd
MEnd


## MARTIAL ARTS - TIER 2 (Storm Kick) ###########################################################

Move MA_Tier2_Alt1
	Requires	COMBAT KUNGFU KICK STRONG ALTERNATE ONE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier2_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow 1 25
	TEnd
MEnd

Move MA_Tier2_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        BrawlCycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_left_elbow 25
	TEnd
MEnd

#############################################################

Move A_MA_Tier2_Alt1
	Requires	FLY COMBAT KUNGFU KICK STRONG ALTERNATE ONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier2_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_left_elbow_air 1 25
	TEnd
MEnd

Move A_MA_Tier2_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_left_elbow_air 25
	TEnd
MEnd

## MARTIAL ARTS - TIER 3 (Cobra Strike) ###########################################################

Move MA_Tier3_Alt1
	Requires	COMBAT KUNGFU PUNCH KUNGFUSTUN ALTERNATE ONE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier3_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_spin_left_backfist 1 46
	TEnd
MEnd

Move MA_Tier3_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_spin_left_backfist 46
	TEnd
MEnd

#############################################################

Move A_MA_Tier3_Alt1
	Requires	FLY COMBAT KUNGFU PUNCH KUNGFUSTUN ALTERNATE ONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier3_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_spin_left_backfist_air 1 46
	TEnd
MEnd

Move A_MA_Tier3_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_spin_left_backfist_air 46
	TEnd
MEnd

## MARTIAL ARTS - TIER 5 (Crane Kick) ###########################################################

Move MA_Tier5_Alt1
	Requires	COMBAT KUNGFU HIGH KICK ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier5_Alt1_Post
	Interpolate	8
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_ironkick 34 83
	TEnd
MEnd

Move MA_Tier5_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_Combat_half
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_ironkick 83
	TEnd
MEnd

#############################################################

Move A_MA_Tier5_Alt1
	Requires	FLY COMBAT KUNGFU HIGH KICK ALTERNATE ONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier5_Alt1_Post
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_ironkick 34 83
	TEnd
MEnd

Move A_MA_Tier5_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_ironkick 83
	TEnd
MEnd


## MARTIAL ARTS - TIER 7 (Crippling Axe Kick)###########################################################

Move MA_Tier7_Alt1_Right
	Requires	COMBAT KUNGFU LOW KICK FEET ALTERNATE ONE
	Priority        7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier7_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker 1 46
	TEnd
MEnd

Move MA_Tier7_Alt1_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\haymaker 46
	TEnd
MEnd

#############################################################

Move A_MA_Tier7_Alt1_Right
	Requires	FLY COMBAT KUNGFU LOW KICK FEET ALTERNATE ONE
	Priority        18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier7_Alt1_Post_Right
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\haymaker_air 1 45
	TEnd
MEnd

Move A_MA_Tier7_Alt1_Post_Right
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\haymaker_air 45 58
	TEnd
MEnd

## MARTIAL ARTS - TIER 8 (Dragons Tail) ###########################################################

Move MA_Tier8_Alt1
	Requires	COMBAT KUNGFU KUNGFUAREA ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        MA_Tier8_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\short_sweepkick 1 45
	TEnd
MEnd

Move MA_Tier8_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\short_sweepkick 45
	TEnd
MEnd

#############################################################

Move A_MA_Tier8_Alt1
	Requires	FLY COMBAT KUNGFU KUNGFUAREA ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier8_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_sweepkick 40 85
	TEnd
MEnd

Move A_MA_Tier8_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_sweepkick 85
	TEnd
MEnd


### MARTIAL ARTS - TIER 9 (Eagles Claw) ###########################################################

Move MA_Tier9_Alt1
	Requires	COMBAT KUNGFU KICK KUNGFUSTUN ALTERNATE ONE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Tier9_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\baseball_punch 1 44
	TEnd
MEnd

Move MA_Tier9_Alt1_2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove        MA_Tier9_Alt1_Post
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\baseball_punch 44 50
	TEnd
MEnd

Move MA_Tier9_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\baseball_punch 50
	TEnd
MEnd

#############################################################

Move A_MA_Tier9_Alt1
	Requires	FLY COMBAT KUNGFU KICK KUNGFUSTUN ALTERNATE ONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_MA_Tier9_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch 1 44
	TEnd
MEnd

Move A_MA_Tier9_Alt1_2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove        A_MA_Tier9_Alt1_Post
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\air_baseball_punch 44 60
	TEnd
MEnd

Move A_MA_Tier9_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Half
	Flags		ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch 60 82
	TEnd
MEnd


## MARTIAL ARTS - TIER 3 (Assassin's Blow) ###########################################################

Move MA_Assassins_Blow_Alt1_Windup
	Requires	COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		NoSizeScale PitchToTarget
	NextMove        MA_Assassins_Blow_Alt1_WindupCycle

	Type		male
	  Anim          male\MA_assassins_punch 1 60
	TEnd
MEnd

Move MA_Assassins_Blow_Alt1_WindupCycle
	Requires	COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		NoSizeScale Cycle PitchToTarget
	NextMove        MA_Assassins_Blow_Alt1

	Type		male
	  Anim          male\MA_assassins_punch 60 60
	TEnd
MEnd

Move MA_Assassins_Blow_Alt1
	Requires	COMBAT MARTBACKSTAB ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ> MA_Assassins_Blow_Alt1_Windup MA_Assassins_Blow_Alt1_WindupCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Assassins_Blow_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\MA_assassins_punch 60 90
	TEnd
MEnd

Move MA_Assassins_Blow_Alt1_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_assassins_punch 90
	TEnd
MEnd

#############################################################

Move A_MA_Assassins_Blow_Alt1_Windup
	Requires	FLY COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
	Flags		NoSizeScale PitchToTarget
	NextMove        A_MA_Assassins_Blow_Alt1_WindupCycle

	Type		male
	  Anim          male\MA_assassins_punch_air 1 60
	TEnd
MEnd


Move A_MA_Assassins_Blow_Alt1_WindupCycle
	Requires	FLY COMBAT WINDUPMARTBACKSTAB ALTERNATE ONE
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove        A_MA_Assassins_Blow_Alt1

	Type		male
	  Anim          male\MA_assassins_punch_air 60 60
	TEnd
MEnd

Move A_MA_Assassins_Blow_Alt1
	Requires	FLY COMBAT MARTBACKSTAB ALTERNATE ONE
	Priority        15
	Interrupts	<AttackIRQ> A_MA_Assassins_Blow_Alt1_Windup
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MA_Assassins_Blow_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\MA_assassins_punch_air 60 90
	TEnd
MEnd

Move A_MA_Assassins_Blow_Alt1_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_assassins_punch_air 90
	TEnd
MEnd