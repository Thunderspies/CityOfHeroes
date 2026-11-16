#############################################################
## Swim.inc - Swimming Moves
#############################################################

#############################################################
## Note: All Priorites are artifically set 9 higher to take
## precedence over all modes
#############################################################

##  SWIM READY ###########################################################

Move swim_readypre
	Requires	SWIM
	Priority        10
	Interrupts	<Ready> Ready
	Include sequencers\Members\Swim.inc
	Flags		ReqInputs FixHipsReg
	NextMove        swim_ready
	Scale           0.75

	Type		male
	  Anim          male\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX
	TEnd

	Type		huge
	  Anim          huge\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX
	TEnd

	Type		fem
	  Anim          fem\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX
	TEnd
MEnd

Move swim_ready
	Requires	SWIM
	Include sequencers\Members\Swim.inc 
	Flags		Cycle FixHipsReg
	NextMove        Ready
	Scale           0.75

	Type		male
	  Anim          male\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX 30
	  PlayFx        FEET\SWIMREADY.FX 70
	TEnd

	Type		huge
	  Anim          huge\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX 30
	  PlayFx        FEET\SWIMREADY.FX 70
	TEnd

	Type		fem
	  Anim          fem\swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX 30
	  PlayFx        FEET\SWIMREADY.FX 70
	TEnd
MEnd


## SWIM FORWARD ###########################################################

Move SwimPre
	Requires	FORWARD SWIM
	Priority        11
	Interrupts	<SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	CycleMove	SwimCycle
	NextMove        SwimPost
	Scale           0.75

	Type		male
	  Anim          male\swim 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		huge
	  Anim          huge\swim 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		fem
	  Anim          fem\swim 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move SwimCycle
	Requires	FORWARD SWIM
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        SwimPost
	Scale           0.75

	Type		male
	  Anim          male\swim 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		huge
	  Anim          huge\swim 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		fem
	  Anim          fem\swim 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move SwimPost
	Requires	SWIM
	Include sequencers\Members\Swim.inc
	NextMove        swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     15

	Type		male
	  Anim          male\swim 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		huge
	  Anim          huge\swim 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		fem
	  Anim          fem\swim 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM BACK ###########################################################

Move SwimBackPre
	Requires	BACKWARD SWIM
	Priority        11
	Interrupts	<SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	CycleMove	SwimBackCycle
	NextMove        SwimBackPost
	Scale           0.75

	Type		male
	  Anim          male\swim_back 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		huge
	  Anim          huge\swim_back 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		fem
	  Anim          fem\swim_back 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move SwimBackCycle
	Requires	BACKWARD SWIM
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        SwimBackPost
	Scale           0.75

	Type		male
	  Anim          male\swim_back 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		huge
	  Anim          huge\swim_back 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		fem
	  Anim          fem\swim_back 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move SwimBackPost
	Requires	SWIM
	Include sequencers\Members\Swim.inc
	NextMove        swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     15

	Type		male
	  Anim          male\swim_back 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		huge
	  Anim          huge\swim_back 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		fem
	  Anim          fem\swim_back 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM RIGHT ###########################################################

Move SwimRightPre
	Requires	STEPRIGHT SWIM
	Priority        11
	Interrupts	SwimPre <SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	CycleMove	SwimRightCycle
	NextMove        SwimRightPost
	Scale           0.75

	Type		male
	  Anim          male\swim_right 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		huge
	  Anim          huge\swim_right 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		fem
	  Anim          fem\swim_right 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move SwimRightCycle
	Requires	STEPRIGHT SWIM
	Priority        11
	Interrupts	SwimCycle <SwimReady> <GroundMoveALL> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        SwimRightPost
	Scale           0.75

	Type		male
	  Anim          male\swim_right 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		huge
	  Anim          huge\swim_right 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		fem
	  Anim          fem\swim_right 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move SwimRightPost
	Requires	SWIM
	Include sequencers\Members\Swim.inc
	NextMove        swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     15

	Type		male
	  Anim          male\swim_right 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		huge
	  Anim          huge\swim_right 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		fem
	  Anim          fem\swim_right 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM LEFT ###########################################################

Move SwimLeftPre
	Requires	STEPLEFT SWIM
	Priority        11
	Interrupts	SwimPre <SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	CycleMove	SwimLeftCycle
	NextMove        SwimLeftPost
	Scale           0.75

	Type		male
	  Anim          male\swim_left 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		huge
	  Anim          huge\swim_left 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd

	Type		fem
	  Anim          fem\swim_left 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move SwimLeftCycle
	Requires	STEPLEFT SWIM
	Priority	11
	Interrupts	SwimCycle <SwimReady> <GroundMoveALL> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        SwimLeftPost
	Scale           0.75

	Type		male
	  Anim          male\swim_left 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		huge
	  Anim          huge\swim_left 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd

	Type		fem
	  Anim          fem\swim_left 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move SwimLeftPost
	Requires	SWIM
	Include sequencers\Members\Swim.inc
	NextMove        swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     15

	Type		male
	  Anim          male\swim_left 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		huge
	  Anim          huge\swim_left 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd

	Type		fem
	  Anim          fem\swim_left 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM POSTJUMP ###########################################################

Move Swim_JumpPost
	Requires	SWIM
	Priority        10
	Interrupts	<GroundJumpPost>
	Include sequencers\Members\Swim.inc
	NextMove        swim_ready
	Flags		FixHipsReg
	Interpolate     2

	Type		male
	  Anim          male\swim_postjump 1
	  # PlayFx        FEET\SPLASH.FX
	TEnd

	Type		huge
	  Anim          huge\swim_postjump 1
	TEnd

	Type		fem
	  Anim          fem\swim_postjump 1
	TEnd
MEnd


##  SWIM SHIELD READY ###########################################################

Move shield_swim_readypre
	Requires	SWIM SHIELD
	Priority        11
	Interrupts	<Ready> Ready
	Include sequencers\Members\Swim.inc
	Flags		ReqInputs FixHipsReg
	NextMove        shield_swim_ready
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX
	TEnd
MEnd

Move shield_swim_ready
	Requires	SWIM SHIELD
	Include sequencers\Members\Swim.inc
	Flags		Cycle FixHipsReg
	NextMove        Ready
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_ready 1
	  PlayFx        FEET\SWIMREADY.FX 30
	  PlayFx        FEET\SWIMREADY.FX 70
	TEnd
MEnd


## SWIM SHIELD FORWARD ###########################################################

Move shield_SwimPre
	Requires	FORWARD SWIM SHIELD
	Priority        12
	Interrupts	<SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		ReqInputs FixHipsReg
	NextMove        shield_SwimCycle
	Scale           0.75

	Type		male
	  Anim          male\shield_swim 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move shield_SwimCycle
	Requires	FORWARD SWIM SHIELD
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        shield_SwimPost
	Scale           0.75

	Type		male
	  Anim          male\shield_swim 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move shield_SwimPost
	Requires	SWIM SHIELD
	Include sequencers\Members\Swim.inc
	NextMove        shield_swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     8

	Type		male
	  Anim          male\shield_swim 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd

## SWIM SHIELD BACK ###########################################################

Move shield_SwimBackPre
	Requires	BACKWARD SWIM SHIELD
	Priority        12
	Interrupts	<SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		ReqInputs FixHipsReg
	NextMove        shield_SwimBackCycle
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_back 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move shield_SwimBackCycle
	Requires	BACKWARD SWIM SHIELD
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        shield_SwimBackPost
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_back 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move shield_SwimBackPost
	Requires	SWIM SHIELD
	Include sequencers\Members\Swim.inc
	NextMove        shield_swim_ready
	Flags		FixHipsReg
	Scale           0.75
	Interpolate     8

	Type		male
	  Anim          male\shield_swim_back 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM SHIELD RIGHT ###########################################################

Move shield_SwimRightPre
	Requires	STEPRIGHT SWIM SHIELD
	Priority        12
	Interrupts	SwimPre <SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		ReqInputs FixHipsReg
	NextMove        shield_SwimRightCycle
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_right 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move shield_SwimRightCycle
	Requires	STEPRIGHT SWIM SHIELD
	Priority        12
	Interrupts	SwimCycle <SwimReady> <GroundMoveALL> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        shield_SwimRightPost
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_right 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move shield_SwimRightPost
	Requires	SWIM SHIELD
	Include sequencers\Members\Swim.inc
	NextMove        shield_swim_ready
	Flags		FixHipsReg
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_right 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd


## SWIM SHIELD LEFT ###########################################################

Move shield_SwimLeftPre
	Requires	STEPLEFT SWIM SHIELD
	Priority        12
	Interrupts	SwimPre <SwimReady> <GroundMoveALL> <IRQSwim> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		ReqInputs FixHipsReg
	NextMove        shield_SwimLeftCycle
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_left 1 35
	  PlayFx        FEET\SWIMPRE.FX
	TEnd
MEnd

Move shield_SwimLeftCycle
	Requires	STEPLEFT SWIM SHIELD
	Priority	12
	Interrupts	SwimCycle <SwimReady> <GroundMoveALL> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <SwimMove>
	Flags		Cycle FixHipsReg
	NextMove        shield_SwimLeftPost
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_left 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd

Move shield_SwimLeftPost
	Requires	SWIM SHIELD
	Include sequencers\Members\Swim.inc
	NextMove        shield_swim_ready
	Flags		FixHipsReg
	Scale           0.75

	Type		male
	  Anim          male\shield_swim_left 45
	  PlayFx        FEET\SWIMPOST.FX
	TEnd
MEnd

## SWIM SHIELD POSTJUMP ###########################################################

Move shield_Swim_JumpPost
	Requires	SWIM SHIELD
	Priority        11
	Interrupts	<GroundJumpPost> Swim_JumpPost
	Include sequencers\Members\Swim.inc
	NextMove        shield_swim_ready
	Flags		FixHipsReg
	Interpolate     2

	Type		male
	  Anim          male\shield_swim_postjump 1
	  # PlayFx        FEET\SPLASH.FX
	TEnd
MEnd