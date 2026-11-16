#############################################################
## Korea.inc - korean 2nd Brawl powers
#############################################################

## THROWBRAWL ###########################################################

Move ThrowBrawl
	Requires	COMBAT THROWBRAWL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\frisbee_turn 4 25
	  PitchAngle	0.3
	  PitchStart	9
	  PitchEnd	20
	TEnd
MEnd

Move ThrowBrawl_post
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        Lazy_Combat_half

	Type		male
	  Anim          male\frisbee_turn  26
	TEnd
MEnd

#############################################################

Move A_ThrowBrawl
	Requires	FLY COMBAT THROWBRAWL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\air_frisbe_turn  4 25
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

MEnd

Move A_ThrowBrawl_post
	Requires	FLY COMBAT
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ> <FlyCombatAttack>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_frisbe_turn  26
	TEnd
MEnd


## WEAPON THROWBRAWL ###########################################################

Move W_ThrowBrawl
	Requires	WEAPON COMBAT THROWBRAWL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        W_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\wep_frisbee_turn 4 25
	  PitchAngle	0.3
	  PitchStart	9
	  PitchEnd	20
	TEnd
MEnd

Move W_ThrowBrawl_post
	Requires	WEAPON COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        Lazy_WepReady
	Priority        2

	Type		male
	  Anim          male\wep_frisbee_turn 26
	TEnd
MEnd

#############################################################

Move A_W_ThrowBrawl
	Requires	FLY WEAPON COMBAT THROWBRAWL
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_W_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\air_wep_frisbee_turn 4 25
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move A_W_ThrowBrawl_post
	Requires	FLY WEAPON COMBAT
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_frisbee_turn 26
	TEnd
MEnd


## DUAL THROWBRAWL ###########################################################

Move D_ThrowBrawl
	Requires	WEAPON COMBAT DUAL THROWBRAWL
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        D_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_knife_throw 1 25
	  PitchAngle	0.5
	TEnd
MEnd

Move D_ThrowBrawl_post
	Requires	WEAPON COMBAT DUAL
	Priority        3
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        DualWield_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_D_ThrowBrawl
	Requires	FLY WEAPON COMBAT DUAL THROWBRAWL
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_D_ThrowBrawl_post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\air_dual_knife_throw 1 25
	 PitchAngle	0.5
	TEnd
MEnd

Move A_D_ThrowBrawl_post
	Requires	FLY WEAPON COMBAT DUAL
	Priority        4
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        DualWield_FlyReady
	Flags		PitchToTarget

	Type		male
	  Anim          male\air_dual_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd