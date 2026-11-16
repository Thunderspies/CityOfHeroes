#########################################################
## Staff_Powers.INC - Staff Power related sequencers
#########################################################

#######################################
## Staff - Air Summon (point staff in air) ##
#######################################

Move Staff_AirSummon_Draw
	Requires	COMBAT WEAPON CLUB STAFF SPAWNSIDE SUMMON
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Staff_AirSummon
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\firesword_draw 1 25
	TEnd
MEnd

Move Staff_AirSummon
	Requires	COMBAT WEAPON CLUB STAFF SUMMON
	Priority        5
	Interrupts	<AttackIRQ> #2swordcycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Staff_AirSummon_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.5
	Interpolate	8

	Type		male
	  Anim          male\wep_mace_summon 1 60
	TEnd
MEnd

Move Staff_AirSummon_Post
	Requires	COMBAT WEAPON CLUB STAFF
	Priority        4
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <PostMoveCycle>
	NextMove        WepReady
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\wep_mace_summon 61
	TEnd
MEnd


#######################################
## Air Staff - Air Summon (point staff in air) ##
#######################################

Move A_Staff_AirSummon_Draw
	Requires	FLY COMBAT WEAPON CLUB STAFF SPAWNSIDE SUMMON
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Staff_AirSummon
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_firesword_draw 1 25
	TEnd
MEnd

Move A_Staff_AirSummon
	Requires	FLY COMBAT WEAPON CLUB STAFF SUMMON
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Staff_AirSummon_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Interpolate	8
	Scale		1.5

	Type		male
	  Anim          male\air_wep_mace_summon 1 60
	TEnd
MEnd

Move A_Staff_AirSummon_Post
	Requires	FLY COMBAT WEAPON CLUB STAFF
	Priority        15
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        Lazy_WepReady
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\air_wep_mace_summon 61
	TEnd
MEnd


###########################
## Staff - Ground Summon ##
###########################

Move Staff_GroundSummon_Draw
	Requires	COMBAT WEAPON CLUB STAFF BURNSUMMON SPAWNSIDE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Staff_GroundSummon
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\firesword_draw 1 25
	TEnd
MEnd

Move Staff_GroundSummon
	Requires	COMBAT WEAPON CLUB STAFF BURNSUMMON
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Staff_GroundSummon_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 45
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 45
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 45
	TEnd
MEnd

Move Staff_GroundSummon_Post
	Requires	COMBAT WEAPON CLUB STAFF
	Priority        4
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        CombatBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 46
	TEnd

	Type		huge
	  Anim          huge\ground_summon 46
	TEnd

	Type		fem
	  Anim          fem\ground_summon 46
	TEnd
MEnd


###############################
## Air Staff - Ground Summon ##
###############################

Move A_Staff_GroundSummon_Draw
	Requires	FLY COMBAT WEAPON CLUB STAFF SPAWNSIDE BURNSUMMON
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Staff_GroundSummon
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\firesword_draw 1 25
	TEnd
MEnd

Move A_Staff_GroundSummon
	Requires	FLY COMBAT WEAPON CLUB STAFF BURNSUMMON
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Staff_GroundSummon_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_terrify 1 45
	TEnd

	Type		huge
	  Anim          huge\air_terrify 1 45
	TEnd

	Type		fem
	  Anim          fem\air_terrify 1 45
	TEnd
MEnd

Move A_Staff_GroundSummon_Post
	Requires	FLY COMBAT WEAPON CLUB STAFF
	Priority        15
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_terrify 46
	TEnd

	Type		huge
	  Anim          huge\air_terrify 46
	TEnd

	Type		fem
	  Anim          fem\air_terrify 46
	TEnd
MEnd