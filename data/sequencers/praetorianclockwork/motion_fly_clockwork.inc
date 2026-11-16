#############################################################
## FLIGHT MOVEMENT
#############################################################

## FLY FORWARD FROM RUN ###########################################################

Move flypre_Launch
	Requires	FORWARD	FLY
	Priority	12
	Interrupts	RunPre RunCycle <GroundMoveALL> <Launch> <iceslide>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt>
	Flags		ReqInputs
	NextMove	fly
	Wind		FLY

	Type		male
	  Anim		male\f_launch 3
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		huge
	  Anim		huge\f_launch 3
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		fem
	  Anim		fem\launch 3
	  PlayFx	FEET\FlightForward.FX
	TEnd

	Type		fem_floater
	  Anim		fem\hover 1 54
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		male_floater
	  Anim		male\hover 1 54
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		Wisp
	  Anim		wisp\fly_forward_cycle 1
	TEnd
MEnd

## FLY FORWARD ###########################################################

Move flypre
	Requires	FORWARD	FLY
	Priority	12
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <Fall> <Jumps> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt>
	Flags		ReqInputs
	NextMove	fly
	Wind		FLY

	Type		male
	  Anim		male\fly 1 25
	  PlayFx	FEET\FlightForward.FX
	TEnd

	Type		fem_floater
	  Anim		fem\hover 1 54
	TEnd

	Type		male_floater
	  Anim		male\hover 1 54
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\fly_forward 1 50
	TEnd
MEnd

Move fly
	Requires	FORWARD	FLY
	Priority	12
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt>
	Flags		Cycle ReqInputs
	NextMove	flypost
	Wind		FLY

	Type		male
	  Anim		male\fly 25 81
	TEnd

	Type		fem_floater
	  Anim		fem\hover 57 106
	TEnd

	Type		male_floater
	  Anim		male\hover 57 106
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\fly_forward_cycle 1
	TEnd
MEnd

Move flyPost
	Requires	FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Priority	11
	Scale		1.30

	Type		male
	  Anim		male\fly 82
	  PlayFx	FEET\FlightHover.fx
	TEnd

	Type		fem_floater
	  Anim		fem\hover 107
	TEnd

	Type		male_floater
	  Anim		male\hover 107
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\fly_forward 51
	TEnd
MEnd



## FALL FORWARD ###########################################################

Move Ffly_PreFall
	Requires	FORWARD	AIR
	Priority	2
	Interrupts	flyPost	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	Ffly_PreFall_2

	Type		male
	  Anim		male\flydown 1 3
	TEnd

	Type		huge
	  Anim		huge\flydown 1 3
	TEnd

	Type		fem
	  Anim		fem\fly_down 1 3
	TEnd
MEnd

Move Ffly_PreFall_2
	Requires	AIR
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	JumpPost
	Priority	1

	Type		male
	  Anim		male\flydown 4
	TEnd

	Type		huge
	  Anim		huge\flydown 4
	TEnd

	Type		fem
	  Anim		fem\fly_down 4
	TEnd
MEnd

#
### FLY BACKWARD	###########################################################
#
#Move BackflyPre
#	Requires	BACKWARD FLY
#	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	BackflyCycle
#	Priority	12
#
#	Type		male
#	  Anim		male\flyback 3 13
#	  PlayFx	FEET\FlightBackward.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\flyback 3 13
#	  PlayFx	FEET\FlightBackward.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\flyback 3 13
#	  PlayFx	FEET\FlightBackward.fx
#	TEnd
#MEnd
#
#Move BackflyCycle
#	Requires	BACKWARD FLY
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	BackflyPost
#	Priority	12
#
#	Type		male
#	  Anim		male\flyback 14	63
#	TEnd
#
#	Type		huge
#	  Anim		huge\flyback 14	63
#	TEnd
#
#	Type		fem
#	  Anim		fem\flyback 14 63
#	TEnd
#MEnd
#
#Move BackflyPost
#	Requires	FLY
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	flyready
#	Priority	11
#	Scale		1.30
#
#	Type		male
#	  Anim		male\flyback 64
# 	  PlayFx	FEET\FlightHover.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\flyback 64
#	  PlayFx	FEET\FlightHover.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\flyback 64
#	  PlayFx	FEET\FlightHover.fx
#	TEnd
#MEnd


## FLY UP ###########################################################

Move flyUPpre
	Requires	JUMP FLY
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	flyUPCycle
	Priority	12
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\flyup 5 34
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		huge
	  Anim		huge\flyup 5 34
	  PlayFx	FEET\FlightForward.fx
	TEnd

	Type		fem
	  Anim		fem\fly_up 5 34
	  PlayFx	FEET\FlightForward.FX
	TEnd
MEnd

Move flyUPCycle
	Requires	JUMP FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	flyUPPost
	Priority	12
	Wind		FLYUP

	Type		male
	  Anim		male\flyup 36 93
	TEnd

	Type		huge
	  Anim		huge\flyup 36 93
	TEnd

	Type		fem
	  Anim		fem\fly_up 36 93
	TEnd
MEnd

Move flyUPPost
	Requires	FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Priority	11
	Wind		FLY

	Type		male
	  Anim		male\flyup 94
	  PlayFx	FEET\FlightHover.fx
	TEnd

	Type		huge
	  Anim		huge\flyup 94
	  PlayFx	FEET\FlightHover.fx
	TEnd

	Type		fem
	  Anim		fem\fly_up 94
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd


## FLY RIGHT ###########################################################

Move RightflyPre
	Requires	STEPRIGHT FLY
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> LeftflyCycle <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	RightflyCycle
	Priority	12

	Type		male
	  Anim		male\ssflyR 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd

	Type		huge
	  Anim		huge\ssflyR 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd

	Type		fem
	  Anim		fem\fly_right 3	9
	  PlayFx	FEET\FlightLateral.FX
	TEnd
MEnd

Move RightflyCycle
	Requires	STEPRIGHT FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	RightflyPost
	Priority	12

	Type		male
	  Anim		male\ssflyR 10 59
	TEnd

	Type		huge
	  Anim		huge\ssflyR 10 59
	TEnd

	Type		fem
	  Anim		fem\fly_right 10 59
	TEnd
MEnd

Move RightflyPost
	Requires	FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Priority	11

	Type		male
	  Anim		male\ssflyR 60
	  PlayFx	FEET\FlightHover.FX
	TEnd

	Type		huge
	  Anim		huge\ssflyR 60
	  PlayFx	FEET\FlightHover.FX
	TEnd

	Type		fem
	  Anim		fem\fly_right 60
	  PlayFx	FEET\FlightHover.FX
	TEnd
MEnd


## FLY LEFT ###########################################################

Move LeftflyPre
	Requires	STEPLEFT FLY
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> RightflyCycle <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	LeftflyCycle
	Priority	12

	Type		male
	  Anim		male\ssflyL 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd

	Type		huge
	  Anim		huge\ssflyL 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd

	Type		fem
	  Anim		fem\fly_left 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd
MEnd

Move LeftflyCycle
	Requires	STEPLEFT FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	LeftflyPost
	Priority	12

	Type		male
	  Anim		male\ssflyL 10 59
	TEnd

	Type		huge
	  Anim		huge\ssflyL 10 59
	TEnd

	Type		fem
	  Anim		fem\fly_left 10	59
	TEnd
MEnd

Move LeftflyPost
	Requires	FLY
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Priority	11

	Type		male
	  Anim		male\ssflyL 60
	  PlayFx	FEET\FlightHover.FX
	TEnd

	Type		huge
	  Anim		huge\ssflyL 60
	  PlayFx	FEET\FlightHover.FX
	TEnd

	Type		fem
	  Anim		fem\fly_left 60
	  PlayFx	FEET\FlightHover.FX
	TEnd
MEnd


#############################################################
## COMBAT MODE
#############################################################
#
### COMBAT FLY FORWARD ###########################################################
#
#Move C_flypre
#	Requires	FORWARD	FLY COMBAT
#	Priority	13
#	Interrupts	<FlyCombat> <FlyCombatAttack> <PostFLYCycle> C_RunJumpCycle C_RunJumpCycleB C_Ffly_PreFall <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	C_flyingCycle
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_comb_fly 4 24
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_fly 4 24
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_fly 4 24
#	TEnd
#
#	Type		male_floater
#	  Anim		male\combat_hover 1 54
#	TEnd
#
#	Type		Rularuu_male
#	  Anim		male\mystic_float 1
#	TEnd
#MEnd
#
#Move C_flyingCycle
#	Requires	FORWARD	FLY COMBAT
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	C_flyPost
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_comb_fly 25 81
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_fly 25 81
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_fly 25 81
#	TEnd
#
#	Type		fem_floater
#	  Anim		fem\combat_hover 57 106
#	TEnd
#
#	Type		male_floater
#	  Anim		male\combat_hover 57 106
#	TEnd
#
#	Type		Rularuu_male
#	  Anim		male\mystic_float 1
#	TEnd
#MEnd
#
#Move C_flyPost
#	Requires	FLY COMBAT
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	C_flyready
#	Scale		1.30
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_comb_fly 82
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_fly 82
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_fly 82
#	TEnd
#
#	Type		fem_floater
#	  Anim		fem\combat_hover 107
#	TEnd
#
#	Type	       male_floater
#	  Anim		male\combat_hover 107
#	TEnd
#
#	Type		Rularuu_male
#	  Anim		male\mystic_float 1
#	TEnd
#MEnd
#
#
####################
### Combat fly Backward ##
####################
#
#Move C_BackflyPre
#	Requires	BACKWARD FLY COMBAT
#	Priority	13
#	Interrupts	<FlyCombat> <FlyCombatAttack> <PostFLYCycle> C_BackRunCycle <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	C_BackflyCycle
#
#	Type		male
#	  Anim		male\air_comb_flyback 10 13
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyback 10 13
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyback 10	13
#	TEnd
#MEnd
#
#Move C_BackflyCycle
#	Requires	BACKWARD FLY COMBAT
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	C_BackflyPost
#
#	Type		male
#	  Anim		male\air_comb_flyback 14 63
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyback 14 63
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyback 14	63
#	TEnd
#MEnd
#
#Move C_BackflyPost
#	Requires	FLY COMBAT
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	C_flyready
#
#	Type		male
#	  Anim		male\air_comb_flyback 64
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyback 64
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyback 64
#	TEnd
#MEnd
#
#
####################
#### Combat Fly Up  ####
####################
#
#Move C_flyUPpre
#	Requires	JUMP FLY COMBAT
#	Priority	13
#	Interrupts	<FlyCombat> <Jumps> <FlyCombatAttack> <PostFLYCycle> <iceslide>	<MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	C_flyUPCycle
#	Scale		0.90
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\air_comb_flyup 5 34
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyup 5 34
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyup 5 34
#	TEnd
#MEnd
#
#Move C_flyUPCycle
#	Requires	JUMP FLY COMBAT
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	C_flyUPPost
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\air_comb_flyup 36 93
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyup 36 93
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyup 36 93
#	TEnd
#MEnd
#
#Move C_flyUPPost
#	Requires	FLY COMBAT
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	C_flyready
#	Scale		1.30
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_comb_flyup 94
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyup 94
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyup 94
#	TEnd
#MEnd
#
#
####################
#### Combat fly Right ####
####################
#
#Move C_RightflyPre
#	Requires	STEPRIGHT FLY COMBAT
#	Priority	13
#	Interrupts	<FlyCombat> <FlyCombatAttack> <PostFLYCycle> C_LeftflyCycle C_Ffly_Fready <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	C_RightflyCycle
#
#	Type		male
#	  Anim		male\air_comb_flyright 3 9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyright 3 9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyright 3	9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#MEnd
#
#Move C_RightflyCycle
#	Requires	STEPRIGHT FLY COMBAT
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	C_RightflyPost
#
#	Type		male
#	  Anim		male\air_comb_flyright 10 59
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyright 10 59
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyright 10 59
#	TEnd
#MEnd
#
#Move C_RightflyPost
#	Requires	FLY COMBAT
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	C_flyready
#
#	Type		male
#	  Anim		male\air_comb_flyright 60
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyright 60
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyright 60
#	TEnd
#MEnd
#
#
####################
#### Combat fly Left  ####
####################
#
#Move C_LeftflyPre
#	Requires	STEPLEFT FLY COMBAT
#	Priority	13
#	Interrupts	<FlyCombat> <FlyCombatAttack> <PostFLYCycle> C_RightflyCycle C_Ffly_Fready <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	C_LeftflyCycle
#
#	Type		male
#	  Anim		male\air_comb_flyleft 3	9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyleft 3	9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyleft 3 9
#	  PlayFx	FEET\FlightLateral.FX
#	TEnd
#MEnd
#
#Move C_LeftflyCycle
#	Requires	STEPLEFT FLY COMBAT
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	C_LeftflyPost
#
#	Type		male
#	  Anim		male\air_comb_flyleft 10 59
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyleft 10 59
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyleft 10	59
#	TEnd
#MEnd
#
#Move C_LeftflyPost
#	Requires	FLY COMBAT
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyCombatMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	C_flyready
#
#	Type		male
#	  Anim		male\air_comb_flyleft 60
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_comb_flyleft 60
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_comb_flyleft 60
#	TEnd
#MEnd


###################
### Weapon Fly forward	####
###################

Move W_flypre
	Requires	FORWARD	FLY COMBAT WEAPON
	Priority	14
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_RunJumpCycle W_RunJumpCycleB W_Ffly_PreFall <iceslide>	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	W_flyingCycle
	Wind		FLY

	Type		male
	  Anim		male\air_wep_fly 4 24
	TEnd

	Type		fem_floater
	  Anim		fem\wep_hover 1	54
	TEnd

	Type		male_floater
	  Anim		male\wep_hover 1 54
	TEnd
MEnd

Move W_flyingCycle
	Requires	FORWARD	FLY COMBAT WEAPON
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	W_flyPost
	Wind		FLY

	Type		male
	  Anim		male\air_wep_fly 25 82
	TEnd

	Type		fem_floater
	  Anim		fem\wep_hover 57 106
	TEnd

	Type		male_floater
	  Anim		male\wep_hover 57 106
	TEnd
MEnd

Move W_flyPost
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	W_flyready
	Scale		1.30
	Wind		FLY

	Type		male
	  Anim		male\air_wep_fly 83
	TEnd

	Type		fem_floater
	  Anim		fem\wep_hover 107
	TEnd

	Type		male_floater
	  Anim		male\wep_hover 107
	TEnd
MEnd

#
####################
### Weapon fly Backward ##
####################
#
#Move W_BackflyPre
#	Requires	BACKWARD FLY COMBAT WEAPON
#	Priority	14
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	W_BackflyCycle
#
#	Type		male
#	  Anim		male\air_wep_flyback 3 13
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_wep_flyback 3 13
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_wep_flyback 3 13
#	TEnd
#MEnd
#
#Move W_BackflyCycle
#	Requires	BACKWARD FLY COMBAT WEAPON
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	W_BackflyPost
#
#	Type		male
#	  Anim		male\air_wep_flyback 14	63
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_wep_flyback 14	63
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_wep_flyback 14 63
#	TEnd
#MEnd
#
#Move W_BackflyPost
#	Requires	FLY COMBAT WEAPON
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	W_flyready
#	Scale		1.30
#
#	Type		male
#	  Anim		male\air_wep_flyback 64
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_wep_flyback 64
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_wep_flyback 64
#	TEnd
#MEnd


###################
### Weapon Fly Up  ####
###################

Move W_flyUPpre
	Requires	JUMP FLY COMBAT	WEAPON
	Priority	14
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <Jumps> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	W_flyUPCycle
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\air_wep_flyup 5 34
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyup 5 34
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyup 5 34
	TEnd
MEnd

Move W_flyUPCycle
	Requires	JUMP FLY COMBAT	WEAPON
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	W_flyUPPost
	Wind		FLYUP

	Type		male
	  Anim		male\air_wep_flyup 36 93
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyup 36 93
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyup 36 93
	TEnd
MEnd

Move W_flyUPPost
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	W_flyready
	Wind		FLY

	Type		male
	  Anim		male\air_wep_flyup 94
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyup 94
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyup 94
	TEnd
MEnd


###################
### Weapon fly Right ####
###################

Move W_RightflyPre
	Requires	STEPRIGHT FLY COMBAT WEAPON
	Priority	14
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_Ffly_Fready <iceslide>	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	W_RightflyCycle

	Type		male
	  Anim		male\air_wep_flyR 3 9
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyR 3 9
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyR 3 9
	TEnd
MEnd

Move W_RightflyCycle
	Requires	STEPRIGHT FLY COMBAT WEAPON
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	W_RightflyPost

	Type		male
	  Anim		male\air_wep_flyR 10 59
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyR 10 59
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyR 10 59
	TEnd
MEnd

Move W_RightflyPost
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	W_flyready

	Type		male
	  Anim		male\air_wep_flyR 60
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyR 60
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyR 60
	TEnd
MEnd


###################
### Weapon fly Left  ####
###################

Move W_LeftflyPre
	Requires	STEPLEFT FLY COMBAT WEAPON
	Priority	14
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_Ffly_Fready <iceslide>	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	W_LeftflyCycle

	Type		male
	  Anim		male\air_wep_flyL 3 9
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyL 3 9
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyL 3 9
	TEnd
MEnd

Move W_LeftflyCycle
	Requires	STEPLEFT FLY COMBAT WEAPON
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	W_LeftflyPost

	Type		male
	  Anim		male\air_wep_flyL 10 59
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyL 10 59
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyL 10 59
	TEnd
MEnd

Move W_LeftflyPost
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	W_flyready

	Type		male
	  Anim		male\air_wep_flyL 60
	TEnd

	Type		huge
	  Anim		huge\air_wep_flyL 60
	TEnd

	Type		fem
	  Anim		fem\air_wep_flyL 60
	TEnd
MEnd


#############################################################
## DUAL MODE
#############################################################

## DUAL FLY FORWARD ###########################################################

Move DualWield_Flying_Pre
	Requires	FORWARD	FLY COMBAT WEAPON DUAL
	Priority	15
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <DualHandMove> W_ForwardFly_PreFall D_ForwardFly_PreFall	<iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_FlyingCycle
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly 4 24
	TEnd
MEnd

Move DualWield_FlyingCycle
	Requires	FORWARD	FLY COMBAT WEAPON DUAL
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	DualWield_Flying_Post
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly 25 82
	TEnd
MEnd

Move DualWield_Flying_Post
	Requires	FLY COMBAT WEAPON DUAL
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> #<MoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_flyready
	Scale		1.30
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly 83
	TEnd
MEnd

#
##############################################################
#
#Move DualWield_BackflyPre
#	Requires	BACKWARD FLY COMBAT WEAPON DUAL
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <DualHandMove> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	DualWield_BackflyCycle
#
#	Type		male
#	  Anim		male\air_dual_fly_back 3 13
#	TEnd
#MEnd
#
#Move DualWield_BackflyCycle
#	Requires	BACKWARD FLY COMBAT WEAPON DUAL
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	DualWield_BackflyPost
#	Priority	15
#
#	Type		male
#	  Anim		male\air_dual_fly_back 14 63
#	TEnd
#MEnd
#
#Move DualWield_BackflyPost
#	Requires	FLY COMBAT WEAPON DUAL
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	DualWield_flyready
#
#	Type		male
#	  Anim		male\air_dual_fly_back 64
#	TEnd
#MEnd


#############################################################

Move DualWield_flyUPpre
	Requires	JUMP FLY COMBAT	WEAPON DUAL
	Priority	15
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <Jumps> <iceslide> <DualHandMove> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_flyUPCycle
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\air_dual_fly_up 5 34
	TEnd
MEnd

Move DualWield_flyUPCycle
	Requires	JUMP FLY COMBAT	WEAPON DUAL
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	DualWield_flyUPPost
	Wind		FLYUP

	Type		male
	  Anim		male\air_dual_fly_up 36	93
	TEnd
MEnd

Move DualWield_flyUPPost
	Requires	FLY COMBAT WEAPON DUAL
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_flyready
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly_up 94
	TEnd
MEnd

#############################################################

Move DualWield_RightflyPre
	Requires	STEPRIGHT FLY COMBAT WEAPON DUAL
	Priority	15
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> D_ForwardFly_Fready <iceslide> <DualHandMove> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_RightflyCycle

	Type		male
	  Anim		male\air_dual_fly_R 3 9
	TEnd
MEnd

Move DualWield_RightflyCycle
	Requires	STEPRIGHT FLY COMBAT WEAPON DUAL
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	DualWield_RightflyPost

	Type		male
	  Anim		male\air_dual_fly_R 10 59
	TEnd
MEnd

Move DualWield_RightflyPost
	Requires	FLY COMBAT WEAPON DUAL
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_flyready

	Type		male
	  Anim		male\air_dual_fly_R 60
	TEnd
MEnd


#############################################################

Move DualWield_LeftflyPre
	Requires	STEPLEFT FLY COMBAT WEAPON DUAL
	Priority	15
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> D_ForwardFly_Fready <iceslide> <DualHandMove> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_LeftflyCycle
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly_L 3 9
	TEnd
MEnd

Move DualWield_LeftflyCycle
	Requires	STEPLEFT FLY COMBAT WEAPON DUAL
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL>
	Flags		Cycle
	NextMove	DualWield_LeftflyPost
	Wind		FLY

	Type		male
	  Anim		male\air_dual_fly_L 10 59
	TEnd
MEnd

Move DualWield_LeftflyPost
	Requires	FLY COMBAT WEAPON DUAL
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyDualHandMove> <FlyMoveALL> <MoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_flyready

	Type		male
	  Anim		male\air_dual_fly_L 60
	TEnd
MEnd


#############################################################
## HOVERS
#############################################################

## HOVER FORWARD ###########################################################

Move Hover_Forward_Pre
	Requires	FORWARD	FLY HOVER
	Priority	13
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicFlyMove> <Fall> <Jumps> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hover_ForwardCycle
	Scale		0.875

	Type		male
	  Anim		male\hover_forward 1 10
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move Hover_ForwardCycle
	Requires	FORWARD	FLY HOVER
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Hover_Forward_Post
	Scale		0.625

	Type		male
	  Anim		male\hover_forward 10 70
	TEnd
MEnd

Move Hover_Forward_Post
	Requires	FLY
	Priority	11
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Scale		0.875

	Type		male
	  Anim		male\hover_forward 71
	TEnd
MEnd

## FLIGHT TO HOVER TRANSITION ###########################################################

Move Flight_to_Hover_Trans
	Requires	FORWARD	FLY HOVER
	Priority	13
	Interrupts	flypre fly
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Flight_to_Hover_Blend
	Scale		1.5

	Type		male
	  Anim		male\fly 82 97
	TEnd
MEnd

Move Flight_to_Hover_Blend
	Requires	FORWARD	FLY HOVER
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hover_Forward
	Interpolate	20
	Scale		0.75

	Type		male
	  Anim		male\hover_forward 5 70
	TEnd
MEnd


#############################################################

## HOVER UP ###########################################################

Move Hover_Up_Pre
	Requires	JUMP FLY HOVER
	Priority	13
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicFlyMove> <Fall> <Jumps> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hover_Up_Cycle
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\flyup 5 34
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move Hover_Up_Cycle
	Requires	JUMP FLY HOVER
	Priority	13
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
	Flags		Cycle
	NextMove	Hover_Up_Post
	Wind		FLYUP

	Type		male
	  Anim		male\flyup 36 93
	TEnd
MEnd

Move Hover_Up_Post
	Requires	FLY HOVER
	Priority	12
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
	Flags		ReqInputs
	NextMove	flyready
	Wind		FLY

	Type		male
	  Anim		male\flyup 94
	TEnd
MEnd

## COMBAT HOVER FORWARD ###########################################################

Move C_HoverForward_Pre
	Requires	FLY HOVER COMBAT FORWARD
	Priority	14
	Interrupts	<FlyCombat> <FlyCombatMove> <PostFLYCycle> <HoverMove> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverCombatMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_HoverForward_Cycle
	Scale		0.875

	Type		male
	  Anim		male\combat_hover_forward 1 10
	  PlayFx	FEET\FLIGHT2.FX
	TEnd
MEnd

Move C_HoverForward_Cycle
	Requires	FLY HOVER COMBAT FORWARD
	Priority	14
	Interrupts	Hover_ForwardCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverCombatMove> <FlyMoveALL> <AttackIRQ>
	Flags		Cycle
	NextMove	C_HoverForward_Post
	Scale		0.625

	Type		male
	  Anim		male\combat_hover_forward 10 70
	TEnd
MEnd

Move C_HoverForward_Post
	Requires	FLY HOVER COMBAT
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverCombatMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
	Flags		ReqInputs
	NextMove	C_flyready
	Scale		1.3125

	Type		male
	  Anim		male\combat_hover_forward 71
	TEnd
MEnd

## FLIGHT TO COMBAT HOVER TRANSITION ###########################################################

Move C_Flight2Hover_Trans
	Requires	FLY HOVER COMBAT FORWARD
	Priority	14
	Interrupts	C_flypre C_flyingCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverCombatMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Combat_Flight2Hover_Blend
	Scale		1.5

	Type		male
	  Anim		male\air_wep_fly 83 88
	TEnd
MEnd

Move C_Flight2Hover_Blend
	Requires	FLY HOVER COMBAT FORWARD
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverCombatMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_HoverForward_Cycle
	Interpolate	15
	Scale		0.75

	Type		male
	  Anim		male\combat_hover_forward 10 70
	TEnd
MEnd

## WEAPON HOVER FORWARD ###########################################################

Move Wep_Hover_Forward_Pre
	Requires	FLY HOVER COMBAT WEAPON FORWARD
	Priority	15
	Interrupts	<FlyWepMove> <HoverMove> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Wep_Hover_ForwardCycle
	Scale		0.875

	Type		male
	  Anim		male\wep_hover_forward 1 10
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move Wep_Hover_ForwardCycle
	Requires	FLY HOVER COMBAT WEAPON FORWARD
	Interrupts	Hover_ForwardCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Wep_Hover_Forward_Post
	Scale		0.625

	Type		male
	  Anim		male\wep_hover_forward 10 70
	TEnd
MEnd

Move Wep_Hover_Forward_Post
	Requires	FLY HOVER COMBAT WEAPON
	Priority	14
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
	Flags		ReqInputs
	NextMove	W_flyready
	Scale		0.875

	Type		male
	  Anim		male\wep_hover_forward 71
	TEnd
MEnd

## FLIGHT TO WEAPON HOVER TRANSITION ###########################################################

Move Wep_Flight2Hover_Trans
	Requires	FLY HOVER COMBAT WEAPON FORWARD
	Priority	15
	Interrupts	W_flypre W_flyingCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Wep_Flight2Hover_Blend
	Scale		1.5

	Type		male
	  Anim		male\air_wep_fly 83 88
	TEnd
MEnd

Move Wep_Flight2Hover_Blend
	Requires	FLY HOVER COMBAT WEAPON FORWARD
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Wep_Hover_ForwardCycle
	Interpolate	15
	Scale		0.75

	Type		male
	  Anim		male\wep_hover_forward 10 70
	TEnd
MEnd


## DUAL HOVER FORWARD ###########################################################

Move DualWield_Hover_Forward_Pre
	Requires	FLY HOVER COMBAT WEAPON DUAL FORWARD
	Priority	16
	Interrupts	<FlyDualHandCombat> <FlyDualHandMove> <HoverMove> <iceslide> <MoveIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <HoverDualMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Hover_ForwardCycle
	Scale		0.875

	Type		male
	  Anim		male\dual_hover_forward 1 10
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move DualWield_Hover_ForwardCycle
	Requires	FLY HOVER COMBAT WEAPON DUAL FORWARD
	Priority	16
	Interrupts	Wep_Hover_ForwardCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverDualMove> <FlyMoveALL> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	DualWield_Hover_Forward_Post
	Scale		0.625

	Type		male
	  Anim		male\dual_hover_forward 10 70
	TEnd
MEnd

Move DualWield_Hover_Forward_Post
	Requires	FLY HOVER COMBAT WEAPON DUAL
	Priority	15
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverDualMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_flyready
	Scale		0.875

	Type		male
	  Anim		male\dual_hover_forward 71
	TEnd
MEnd

## FLIGHT TO DUAL HOVER TRANSITION ###########################################################

Move DualWield_Flight2Hover_Trans
	Requires	FLY HOVER COMBAT WEAPON DUAL FORWARD
	Priority	16
	Interrupts	DualWield_Flying_Pre DualWield_FlyingCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverDualMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Flight2Hover_Blend
	Scale		1.5

	Type		male
	  Anim		male\air_dual_fly 83 88
	TEnd
MEnd

Move DualWield_Flight2Hover_Blend
	Requires	FLY HOVER COMBAT WEAPON DUAL FORWARD
	Priority	16
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverDualMove> <FlyMoveALL> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Hover_ForwardCycle
	Interpolate	15
	Scale		0.75

	Type		male
	  Anim		male\dual_hover_forward 10 70
	TEnd
MEnd
#
####################
#### 2hlarge_Weapon Fly forward	####
####################
#
#Move A_2hlarge_flypre
#	Requires	FORWARD	FLY COMBAT WEAPON 2HLARGE
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <MoveIRQ> #W_RunJumpCycle W_RunJumpCycleB W_Ffly_PreFall
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <TwoHandFly>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyingCycle
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_forward 4 24
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_forward 4 24
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_forward 4 24
#	TEnd
#
#MEnd
#
#Move A_2hlarge_flyingCycle
#	Requires	FORWARD	FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <TwoHandFly>
#	Flags		Cycle
#	NextMove	2hlarge_flyPost
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_forward 25 82
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_forward 25 82
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_forward 25 82
#	TEnd
#
#MEnd
#
#Move A_2hlarge_flyPost
#	Requires	FLY COMBAT WEAPON 2hlarge
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#	Scale		1.30
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_forward 83
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_forward 83
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_forward 83
#	TEnd
#
#MEnd
#
#
####################
### 2hlarge_Weapon fly Backward ##
####################
#
#Move A_2hlarge_BackflyPre
#	Requires	BACKWARD FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	2hlarge_BackflyCycle
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_backward 3 13
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_backward 3 13
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_backward 3 13
#	TEnd
#
#MEnd
#
#Move A_2hlarge_BackflyCycle
#	Requires	BACKWARD FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_BackflyPost
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_backward 14 63
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_backward 14 63
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_backward 14 63
#	TEnd
#
#MEnd
#
#Move A_2hlarge_BackflyPost
#	Requires	FLY COMBAT WEAPON 2hlarge
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#	Scale		1.30
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_backward 64
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_backward 64
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_backward 64
#	TEnd
#
#MEnd
#
#
####################
#### 2hlarge_Weapon Fly Up  ####
####################
#
#Move A_2hlarge_flyUPpre
#	Requires	JUMP FLY COMBAT	WEAPON 2hlarge
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <Jumps> <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyUPCycle
#	Scale		0.90
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_up 5 34
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_up 5 34
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_up 5 34
#	TEnd
#
#MEnd
#
#Move A_2hlarge_flyUPCycle
#	Requires	JUMP FLY COMBAT	WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_flyUPPost
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_up 36 93
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_up 36 93
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_up 36 93
#	TEnd
#
#MEnd
#
#Move A_2hlarge_flyUPPost
#	Requires	FLY COMBAT WEAPON 2hlarge
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#	Wind		FLY
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_up 94
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_up 94
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_up 94
#	TEnd
#
#MEnd
#
#
####################
#### 2hlarge_Weapon fly Right ####
####################
#
#Move A_2hlarge_RightflyPre
#	Requires	STEPRIGHT FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <MoveIRQ> #W_Ffly_Fready
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	2hlarge_RightflyCycle
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_right 3 9
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_right 3 9
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_right 3 9
#	TEnd
#
#MEnd
#
#Move A_2hlarge_RightflyCycle
#	Requires	STEPRIGHT FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_RightflyPost
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_right 10 59
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_right 10 59
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_right 10 59
#	TEnd
#
#MEnd
#
#Move A_2hlarge_RightflyPost
#	Requires	FLY COMBAT WEAPON 2hlarge
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_right 60
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_right 60
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_right 60
#	TEnd
#
#MEnd
#
####################
#### 2hlarge_Weapon fly Left  ####
####################
#
#Move A_2hlarge_LeftflyPre
#	Requires	STEPLEFT FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <iceslide> <MoveIRQ> #W_Ffly_Fready
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		ReqInputs
#	NextMove	2hlarge_LeftflyCycle
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_left 3 9
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_left 3 9
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_left 3 9
#	TEnd
#
#MEnd
#
#Move A_2hlarge_LeftflyCycle
#	Requires	STEPLEFT FLY COMBAT WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_LeftflyPost
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_left 10 59
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_left 10 59
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_left 10 59
#	TEnd
#
#MEnd
#
#Move A_2hlarge_LeftflyPost
#	Requires	FLY COMBAT WEAPON 2hlarge
#	Priority	14
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepMove> <FlyMoveALL> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_left 60
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_left 60
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_left 60
#	TEnd
#
#MEnd
#
### 2hlarge_WEAPON HOVER FORWARD ###########################################################
#
#Move A_2hlarge_Hover_Forward_Pre
#	Requires	FLY HOVER COMBAT WEAPON FORWARD 2hlarge
#	Priority	16
#	Interrupts	<FlyWepMove> <HoverMove> <iceslide> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_Hover_ForwardCycle
#	Scale		0.875
#
#	Type		male
#	  Anim		male\air_2hlarge_hover_forward 1 54
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_hover_forward 1 54
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_hover_forward 1 54
#	  PlayFx	FEET\FlightForward.fx
#	TEnd
#MEnd
#
#Move A_2hlarge_Hover_ForwardCycle
#	Requires	FLY HOVER COMBAT WEAPON FORWARD 2hlarge
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
#	Flags		Cycle ReqInputs
#	NextMove	2hlarge_Hover_Forward_Post
#	Scale		0.625
#
#	Type		male
#	  Anim		male\air_2hlarge_hover_forward 57 106
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_hover_forward 57 106
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_hover_forward 57 106
#	TEnd
#MEnd
#
#Move A_2hlarge_Hover_Forward_Post
#	Requires	FLY HOVER COMBAT WEAPON 2hlarge
#	Priority	15
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_flyready
#	Scale		0.875
#
#	Type		male
#	  Anim		male\air_2hlarge_hover_forward 107
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_hover_forward 107
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_hover_forward 107
#	TEnd
#MEnd
#
### FLIGHT TO WEAPON HOVER TRANSITION ###########################################################
#
#Move A_2hlarge_Flight2Hover_Trans
#	Requires	FLY HOVER COMBAT WEAPON FORWARD 2hlarge
#	Priority	16
#	Interrupts	<TwoHandFly>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_Flight2Hover_Blend
#	Scale		1.5
#
#	Type		male
#	  Anim		male\air_2hlarge_fly_forward 83 88
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_forward 83 88
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_fly_forward 83 88
#	TEnd
#MEnd
#
#Move A_2hlarge_Flight2Hover_Blend
#	Requires	FLY HOVER COMBAT WEAPON FORWARD 2hlarge
#	Priority	16
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverWepMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	2hlarge_Hover_ForwardCycle
#	Interpolate	15
#	Scale		0.75
#
#	Type		male
#	  Anim		male\air_2hlarge_hover_forward 57 106
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_2hlarge_fly_hover_forward 57 106
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_2hlarge_hover_forward 57 106
#	TEnd
#MEnd