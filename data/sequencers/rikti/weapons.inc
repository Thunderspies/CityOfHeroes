#########################################################
## WEAPON.INC - Sequencers related to Weapon animations
#########################################################

## SWORD-GUN READY ###########################################################

Move Rikti_SwordGun_Ready
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        postReady_back
	Scale           0.50
	Flags		Cycle

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

## AIR SWORD-GUN READY ###########################################################

Move A_Rikti_SwordGun_Ready
	Requires	FLY COMBAT WEAPON
	Priority        3
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd

## SWORD-GUN RANGED ATTACK #######################################################

Move Rikti_SwordGun_Draw
	Requires	COMBAT WEAPON SPAWNBACK AXE SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rikti_SwordGun
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Rikti_SwordGun
	Requires	COMBAT WEAPON AXE SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rikti_SwordGun_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\Rikti_SwordGun_Ranged1 1 35
	  PitchAngle	-0.1
	TEnd
MEnd

Move Rikti_SwordGun_Post
	Requires	COMBAT WEAPON
	Priority        4
	Member		<ReactIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <PostMoveCycle>
	NextMove        Rikti_SwordGun_Ready
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\Rikti_SwordGun_Ranged1 36
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd


## AIR SWORD GUN RANGED ATTACK #######################################################

Move A_Rikti_SwordGun_Draw
	Requires	FLY COMBAT WEAPON SPAWNBACK AXE SHOOT
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Rikti_SwordGun
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Rikti_SwordGun
	Requires	FLY COMBAT WEAPON AXE SHOOT
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Rikti_SwordGun_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\air_Rikti_SwordGun_Ranged1 1 35
	  PitchAngle	-0.1
	TEnd
MEnd

Move A_Rikti_SwordGun_Post
	Requires	FLY COMBAT
	Priority        5
	Member		<ReactIRQ> <DeathIRQ> <FlyWepAttack> <AttackIRQ> <FlyDualHandAttack>
	NextMove        A_Rikti_SwordGun_Ready #C_FlyReady
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\air_Rikti_SwordGun_Ranged1 36
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd