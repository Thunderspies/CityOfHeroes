#############################################################
## PLAYER WALK - COMBAT
#############################################################

## WALK FORWARD ###########################################################

Move Player_Walk_Combat_Forward
	Requires	COMBAT PLAYER FORWARD PLAYERWALK
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveStrafeIRQ> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	NextMove	Player_Walk_Combat_Forward_Cycle
	Flags		ReqInputs MoveScale
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_forward 1
	  MoveRate	0.15 #5 feet per second
	TEnd

	Type		fem
	  Anim		male\Player_Walk_Combat_Forward 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		male\Player_Walk_Combat_Forward 1
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

Move Player_Walk_Combat_Forward_Cycle
	Requires	COMBAT PLAYER FORWARD PLAYERWALK
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveStrafeIRQ> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	NextMove	Player_Walk_Combat_Post
	Flags		Cycle MoveScale
	Wind		WALKFORWARD
	Interpolate	0

	Type		male
	  Anim		male\combat_walk_forward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.14 #5 feet per second
	TEnd

	Type		fem
	  Anim		male\Player_Walk_Combat_Forward 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		male\Player_Walk_Combat_Forward 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

## WALK POST ###########################################################

Move Player_Walk_Combat_Post
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs
	Interpolate	5

	Type		male
	  Anim		male\combat_walk_forward_post 1
	  #scale		0.5
	TEnd
MEnd

Move Player_Walk_Combat_Left_Post
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs
	Interpolate	5

	Type		male
	  Anim		male\combat_walk_left_post 1
	  #scale		0.5
	TEnd
MEnd

Move Player_Walk_Combat_Left_forward_Post
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs
	Interpolate	5

	Type		male
	  Anim		male\combat_walk_left_forward_post 1
	  #scale		0.5
	TEnd
MEnd

Move Player_Walk_Combat_Right_Post
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs
	Interpolate	5

	Type		male
	  Anim		male\combat_walk_right_post 1
	  #scale		0.5
	TEnd
MEnd

Move Player_Walk_Combat_Right_Forward_Post
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs
	Interpolate	5

	Type		male
	  Anim		male\combat_walk_right_forward_post 1
	  #scale		0.5
	TEnd
MEnd

## WALK BACKWARD ###########################################################

Move Player_Walk_Combat_Backward
	Requires	COMBAT PLAYER BACKWARD PLAYERWALK
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	NextMove	Player_Walk_Combat_Backward_Cycle
	Flags		ReqInputs MoveScale
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_backward 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.12 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back 20
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward 6
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

Move Player_Walk_Combat_Backward_Cycle
	Requires	COMBAT PLAYER BACKWARD PLAYERWALK
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	NextMove	Player_Walk_Combat_Post
	Flags		Cycle MoveScale
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_backward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.12 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd


## WALK LEFT ###########################################################

Move Walk_Combat_Left_Pre
	Requires	COMBAT PLAYER PLAYERWALK STEPLEFT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveStrafeIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	CycleMove	Walk_Combat_Left_Cycle
	NextMove	Player_Walk_Combat_Left_Post
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left 1
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_LeftStrafe 4
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_LeftStrafe 20
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Left_Cycle
	Requires	COMBAT PLAYER PLAYERWALK STEPLEFT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Combat_Left_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_LeftStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_LeftStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

## WALK RIGHT ###########################################################

Move Walk_Combat_Right_Pre
	Requires	COMBAT PLAYER PLAYERWALK STEPRIGHT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveStrafeIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	CycleMove	Walk_Combat_Right_Cycle
	NextMove	Player_Walk_Combat_Right_Post
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_RightStrafe 20
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_RightStrafe 4
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Right_Cycle
	Requires	COMBAT PLAYER PLAYERWALK STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Combat_Right_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_RightStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_RightStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

## WALK LEFT FORWARD ###########################################################

Move Walk_Combat_Left_Forward_Pre
	Requires	COMBAT PLAYER PLAYERWALK FORWARD STEPLEFT
	Priority	5
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Walk_Combat_Left_Forward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left_forward 1
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Forward_LeftStrafe 4
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Forward_LeftStrafe 20
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Left_Forward_Cycle
	Requires	COMBAT PLAYER PLAYERWALK FORWARD STEPLEFT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Combat_Left_forward_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left_forward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Forward_LeftStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Forward_LeftStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

## WALK RIGHT FORWARD ###########################################################

Move Walk_Combat_Right_Forward_Pre
	Requires	COMBAT PLAYER PLAYERWALK FORWARD STEPRIGHT
	Priority	5
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Walk_Combat_Right_Forward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right_forward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Forward_RightStrafe 20
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Forward_RightStrafe 4
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Right_Forward_Cycle
	Requires	COMBAT PLAYER PLAYERWALK FORWARD STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Combat_Right_Forward_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right_forward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Forward_RightStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Forward_RightStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

## WALK LEFT BACKWARD ###########################################################

Move Walk_Combat_Left_Backward_Pre
	Requires	COMBAT PLAYER PLAYERWALK BACKWARD STEPLEFT
	Priority	5
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Walk_Combat_Left_Backward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left_backward 1
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back_LeftStrafe 26
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward_LeftStrafe 15
	  MoveRate	0.145 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Left_Backward_Cycle
	Requires	COMBAT PLAYER PLAYERWALK BACKWARD STEPLEFT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Walk_Combat_Right_Forward_Cycle
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_left_backward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back_LeftStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward_LeftStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.145 #5 feet per second
	TEnd
MEnd


## WALK RIGHT BACKWARD ###########################################################

Move Walk_Combat_Right_Backward_Pre
	Requires	COMBAT PLAYER PLAYERWALK BACKWARD STEPRIGHT
	Priority	5
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <WalkMove> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Walk_Combat_Right_Backward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right_backward 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back_RightStrafe 13
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward_RightStrafe 8
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

Move Walk_Combat_Right_Backward_Cycle
	Requires	COMBAT PLAYER PLAYERWALK BACKWARD STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <CombatWalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Combat_Left_forward_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\combat_walk_right_backward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.13 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_Back_RightStrafe 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_Backward_RightStrafe 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd