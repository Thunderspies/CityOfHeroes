#############################################################
## WALKS
#############################################################

## WALK FORWARD ###########################################################

Move WalkCycle_Pre
	Requires	FORWARD	WALK
	Priority	2
	Interrupts	<Ready> <MoveIRQ> <RunsALL>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		MoveScale
	NextMove	WalkCycle

	Type		male
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		PraetorianPolice
	  Anim		male\walk 1 48
	  PlayFx	FEET\PPD_Left.fx 1
	  PlayFx	FEET\PPD_Right.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd


	Type		ResistanceMale
	  Anim		male\walk 1 48
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd


	Type		ResistanceFem
	  Anim		male\walk 1 48
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		fem
	  Anim		BF\walk	10 57
	  PlayFx	FEET\NPCFEMLEFT.FX:FOOTFALLLEFT 9
	  PlayFx	FEET\NPCFEMRIGHT.FX:FOOTFALLRIGHT 28
	  MoveRate	0.146
	  Scale		1.45
	TEnd

	Type		Sprocket
	  Anim		male\gurder_walk 1
	  PlayFx	GENERIC\PROP_GURDER.FX	0 Constant
	  Scale		0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		Crystal
	  Anim		male\MonComb_walk 1
	  Scale		0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		PraetorianPolice
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move WalkCycle
	Requires	FORWARD	WALK
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	WalkPost
	Priority	2

	Type		male
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		PraetorianPolice
	  Anim		male\walk 1 48
	  PlayFx	FEET\PPD_Left.fx 1
	  PlayFx	FEET\PPD_Left.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		ResistanceMale
	  Anim		male\walk 1 48
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Left.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		ResistanceFem
	  Anim		male\walk 1 48
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Left.fx 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		fem
	  Anim		BF\walk	10 57
	  PlayFx	FEET\NPCFEMLEFT.FX:FOOTFALLLEFT	9
	  PlayFx	FEET\NPCFEMRIGHT.FX:FOOTFALLRIGHT 28
	  MoveRate	0.146
	  Scale		1.45
	TEnd

	Type		Sprocket
	  Anim		male\gurder_walk 1
	  PlayFx	GENERIC\PROP_GURDER.FX 0 Constant
	  Scale		0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		Crystal
	  Anim		male\MonComb_walk 1
	  Scale		0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd

	Type		PraetorianPolice
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  MoveRate	0.146
	  Scale		1.1
	TEnd

MEnd

Move WalkPost
	Include sequencers\Members\readies.inc
	NextMove	Ready
	Interpolate	8

	Type		male
	  Anim		male\postback_right 28
	TEnd

	Type		huge
	  Anim		huge\postback_right 28
	TEnd

	Type		fem
	  Anim		fem\post_ready 28
	TEnd

	Type		PraetorianPolice
	  Anim		male\postback_right 28
	TEnd
MEnd

#############################################################
## PLAYER WALK
#############################################################

## WALK FORWARD ###########################################################

Move Player_Walk_Forward
	Requires	PLAYER FORWARD PLAYERWALK
	Priority	3
	Interrupts	<Ready> <MoveIRQ> <RunsALL>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveStrafeIRQ> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	NextMove	Player_Walk_Forward_Cycle
	Flags		ReqInputs MoveScale
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_forward 24
	  MoveRate	0.1625 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_forward 6
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_forward 24
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

Move Player_Walk_Forward_Cycle
	Requires	PLAYER FORWARD PLAYERWALK
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveStrafeIRQ> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	NextMove	Player_Walk_Post
	Flags		Cycle MoveScale
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_forward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd

	Type		fem
	  Anim		fem\Player_Walk_forward 1
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.175 #5 feet per second
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_forward 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	TEnd
MEnd

#############################################################

Move Player_Walk_Post
	Include sequencers\Members\readies.inc
	NextMove	Ready
	Interpolate	5

	Type		male
	  Anim		male\postback_right 28
	TEnd

	Type		huge
	  Anim		huge\postback_right 28
	TEnd

	Type		fem
	  Anim		fem\post_ready 28
	TEnd
MEnd

## WALK BACKWARD ###########################################################

Move Player_Walk_Backward
	Requires	PLAYER BACKWARD PLAYERWALK
	Priority	3
	Interrupts	<Ready> <MoveIRQ> <RunsALL>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	NextMove	Player_Walk_Backward_Cycle
	Flags		ReqInputs MoveScale
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward 6
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.15 #5 feet per second
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

Move Player_Walk_Backward_Cycle
	Requires	PLAYER BACKWARD PLAYERWALK
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	NextMove	Player_Walk_Post
	Flags		Cycle MoveScale
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.15 #5 feet per second
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

Move Walk_Left_Pre
	Requires	PLAYER PLAYERWALK STEPLEFT
	Priority	3
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveStrafeIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Left_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_LeftStrafe 20
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Left_Cycle
	Requires	PLAYER PLAYERWALK STEPLEFT
	Priority	3
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_LeftStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Pre
	Requires	PLAYER PLAYERWALK STEPRIGHT
	Priority	3
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveStrafeIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Right_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_RightStrafe 4
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Cycle
	Requires	PLAYER PLAYERWALK STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <MoveDiagonalIRQ> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_RightStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Left_Forward_Pre
	Requires	PLAYER PLAYERWALK FORWARD STEPLEFT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Left_Forward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Forward_LeftStrafe 20
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Left_Forward_Cycle
	Requires	PLAYER PLAYERWALK FORWARD STEPLEFT
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Forward_LeftStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Forward_Pre
	Requires	PLAYER PLAYERWALK FORWARD STEPRIGHT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Right_Forward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Forward_RightStrafe 4
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Forward_Cycle
	Requires	PLAYER PLAYERWALK FORWARD STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Forward_RightStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Left_Backward_Pre
	Requires	PLAYER PLAYERWALK BACKWARD STEPLEFT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Left_Backward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward_LeftStrafe 15
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Left_Backward_Cycle
	Requires	PLAYER PLAYERWALK BACKWARD STEPLEFT
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward_LeftStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Backward_Pre
	Requires	PLAYER PLAYERWALK BACKWARD STEPRIGHT
	Priority	4
	Interrupts	<Ready> <MoveIRQ> <RunsALL> <MoveDiagonalIRQ>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		ReqInputs MoveScale
	NextMove	Walk_Right_Backward_Cycle
	Interpolate	10
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward_RightStrafe 8
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

Move Walk_Right_Backward_Cycle
	Requires	PLAYER PLAYERWALK BACKWARD STEPRIGHT
	Member		<DeathIRQ> <AttackIRQ> <WalkMove> <GroundMoveALL> <Jumps> <IRQJumps> <IRQSwim> <IRQSlide>
	Flags		Cycle MoveScale
	NextMove	Player_Walk_Post
	Wind		WALKFORWARD

	Type		male
	  Anim		male\Player_Walk_Backward_RightStrafe 1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.167 #5 feet per second
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

#############################################################

#Include sequencers\player\Motion\motion_walk_combat.inc