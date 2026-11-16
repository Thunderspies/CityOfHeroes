#############################################################
## STUN MODE
#############################################################

## STUN FORWARD ###########################################################

Move dizzy_forward
	Requires	STUN FORWARD
	Priority	11
	Interrupts	<MoveIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <StunMove> <GroundMoveALL>
	NextMove	Stun_cycle
	Scale		0.5
	Flags		Cycle
	Interpolate	20

	Type		male
	  Anim		male\stun_walk 1 27
	TEnd

	Type		Wisp
	  Anim		wisp\sleep 55 85
	TEnd
MEnd

## STUN BACKWARD ###########################################################

Move dizzy_backwards
	Requires	STUN BACKWARD
	Priority	11
	Interrupts	<MoveIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <StunMove> <GroundMoveALL>
	NextMove	Stun_cycle
	Scale		0.5
	Flags		Cycle

	Type		male
	  Anim		male\stun_walkback 1 27
	TEnd
MEnd

## STUN LEFT ###########################################################

Move dizzy_Left
	Requires	STUN STEPLEFT
	Priority	11
	Interrupts	<MoveIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <StunMove> <GroundMoveALL>
	NextMove	Stun_cycle
	Scale		0.5
	Flags		Cycle

	Type		male
	  Anim		male\stun_SSL 1	27
	TEnd
MEnd

## STUN RIGHT ###########################################################

Move dizzy_Right
	Requires	STUN STEPRIGHT
	Priority	11
	Interrupts	<MoveIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <StunMove> <GroundMoveALL>
	NextMove	Stun_cycle
	Scale		0.5
	Flags		Cycle

	Type		male
	  Anim		male\stun_SSR 1	27
	TEnd
MEnd