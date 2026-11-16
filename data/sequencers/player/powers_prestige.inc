#############################################################
## powers_prestige.inc
#############################################################

## PRESTIGE - SELF DESTRUCT ###########################################################

Move Prestige_SelfDestruct_Windup
	Requires	COMBAT SELFDESTRUCT
	Priority        2
	Interrupts	<Ready> <ReactIRQ> <AttackIRQ>
	##Member		<DeathIRQ> ##<ReactIRQ>
	NextMove        Prestige_SelfDestruct
	Sets            CANTMOVE TARGET
	Flags		NoSizeScale

	Type		male
	  Anim          male\Prestige_SelfDestruct 1 294
	  PlayFX	POWERS\ItemsAndTempPowers\SelfDestruction\SelfDestruction_Windup.fx
	TEnd
MEnd

Move Prestige_SelfDestruct
	##Member		<DeathIRQ>
	NextMove        Prestige_SelfDestruct_Post
	Sets            CANTMOVE TARGET
	Flags		NoSizeScale

	Type		male
	  Anim          male\Prestige_SelfDestruct 295 301
	  PlayFX	POWERS\ItemsAndTempPowers\SelfDestruction\SelfDestruction_Attack.fx
	TEnd
MEnd

Move Prestige_SelfDestruct_Post
	##Member		<DeathIRQ>
	NextMove        Prestige_SelfDestruct_PostCycle
	Sets            CANTMOVE TARGET
	Flags		NoSizeScale NotSelectable
	Interpolate	0

	Type		male
	  Anim          male\Prestige_SelfDestruct 301 331
	  PlayFX	GENERIC\entityVanish.fx 0 CONSTANT
	TEnd
MEnd

Move Prestige_SelfDestruct_PostCycle
	Requires	DEATH
	Priority        1
	Member		<RaiseDeadIRQ> <MoveIRQ>
	NextMove	RaiseDead_Pre
	Sets            CANTMOVE TARGET
	Flags		CYCLE NoSizeScale NotSelectable NoDraw
	Interpolate	0

	Type		male
	  Anim          male\Prestige_SelfDestruct 301 301
	  PlayFX	GENERIC\entityVanish.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Prestige_SelfDestruct_Windup
	Requires	FLY COMBAT SELFDESTRUCT
	Priority        13
	Interrupts	<FlyReady> <AttackIRQ>  <ReactIRQ>
	NextMove        Prestige_SelfDestruct
	Sets            CANTMOVE TARGET
	Flags		NoSizeScale

	Type		male
	  Anim          male\Prestige_SelfDestruct 1 294
	  PlayFX	POWERS\ItemsAndTempPowers\SelfDestruction\SelfDestruction_Windup.fx
	TEnd
MEnd

#############################################################
## Mystic Fortune
#############################################################

Move Prestige_TarotCard_Pre
	Requires	COMBAT CARDS SKYSUMMON
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Prestige_TarotCard
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 60
	TEnd
MEnd

Move Prestige_TarotCard
	Requires	COMBAT
	Priority	1
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Prestige_TarotCard_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\short_toss 75 115
	TEnd
MEnd

Move Prestige_TarotCard_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\short_toss 116
	TEnd
MEnd

#############################################################

Move A_Prestige_TarotCard_Pre
	Requires	FLY COMBAT CARDS SKYSUMMON
	Priority        14
	Interrupts	<FlyReady> <AttackIRQ>  <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Prestige_TarotCard
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 5 53
	TEnd
MEnd

Move A_Prestige_TarotCard
	Requires	FLY COMBAT
	Priority	12
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Prestige_TarotCard_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\air_windblast 20 60
	TEnd
MEnd

Move A_Prestige_TarotCard_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        C_flycycle_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_windblast 60
	TEnd
MEnd