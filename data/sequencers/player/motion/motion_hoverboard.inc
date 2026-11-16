#############################################################
## FLIGHT MOVEMENT
#############################################################
#
## FLY READY ###########################################################

Move Hoverboard_Preflyready
	Requires	HOVERBOARD FLY
	Priority	12
	Interrupts	<PostMoveCycle> <Ready> <BasicMove> <iceslide> <Swim> <jumpFall> <Block> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <DualHandMove> <IRQEmotesDualhand> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <BlockIRQ>
	NextMove	Hoverboard_flyready
	Flags		Nosizescale #hideshield #hideweapon

	Type		male
	  Anim		male\hoverboard_prefly 2 78
	  PlayFX	Feet\HoverboardLaunch.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_prefly 2 78
	  PlayFX	Feet\HoverboardLaunch.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_prefly 2 78
	  PlayFX	Feet\HoverboardLaunch.fx
	TEnd

MEnd

Move Hoverboard_flyready
	Requires	HOVERBOARD FLY
	Priority	12
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <BlockIRQ> #<FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle #hideshield #hideweapon
	NextMove	Hoverboard_PreFall

	Type		male
	  Anim		male\hoverboard_forward 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_forward 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_forward 1 31
	TEnd

MEnd

## FLY FORWARD FROM RUN ###########################################################

Move Hoverboard_flypre_Launch
	Requires	FORWARD FLY HOVERBOARD
	Priority	14
	Interrupts	RunPre RunCycle <GroundMoveALL> <Launch> <FlyCombat> <FlyCombatAttack> <PostFLYCycle> <iceslide> <MoveIRQ> <FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand>  <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_fly
	Wind		FLY

	Type		male
	  Anim		male\hoverboard_prefly 5
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_prefly 5
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_prefly 5
	  PlayFx	FEET\HoverboardForward.fx
	TEnd


MEnd

## FLY FORWARD ###########################################################

Move Hoverboard_flypre
	Requires	FORWARD	FLY HOVERBOARD
	Priority	13
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <Jumps> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <iceslide> <FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <TwoHandCombat> <TwoHandMove> hoverboard_flyready <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <IRQSlide> <ShieldFLYMove>  Hoverboard_flypre Hoverboard_Preflyready <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low #<FlyDualHandMove>
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_fly
	Wind		FLY

	Type		male
	  Anim		male\hoverboard_forward 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_forward 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_forward 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

MEnd

Move Hoverboard_fly
	Requires	FORWARD	FLY HOVERBOARD
	Priority	13
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove> Hoverboard_flypre
	Flags		Cycle ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flypost
	Wind		FLY

	Type		male
	  Anim		male\hoverboard_forward 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_forward 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_forward 1 31
	TEnd

MEnd

Move Hoverboard_flyPost
	Requires	FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove> <FlyWepMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyready
	Priority	12
	Scale		1.30

	Type		male
	  Anim		male\hoverboard_forward 1 31
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_forward 1 31
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_forward 1 31
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

MEnd


## FLY BACKWARD	###########################################################

Move Hoverboard_BackflyPre
	Requires	BACKWARD FLY HOVERBOARD
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> Hoverboard_fly Hoverboard_flyready <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> DualWield_BackflyPre <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_BackflyCycle
	Priority	13

	Type		male
	  Anim		male\hoverboard_backward 15 31
	  PlayFx	FEET\HoverboardBackward.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_backward 15 31
	  PlayFx	FEET\HoverboardBackward.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_backward 15 31
	  PlayFx	FEET\HoverboardBackward.fx
	TEnd

MEnd

Move Hoverboard_BackflyCycle
	Requires	BACKWARD FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle #hideshield #hideweapon
	NextMove	Hoverboard_BackflyPost
	Priority	13

	Type		male
	  Anim		male\hoverboard_backward 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_backward 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_backward 1 31
	TEnd

MEnd

Move Hoverboard_BackflyPost
	Requires	FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyready
	Priority	12
	Scale		1.30

	Type		male
	  Anim		male\hoverboard_backward_post 1
 	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_backward_post 1
 	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_backward_post 1
 	  PlayFx	FEET\HoverboardHover.fx
	TEnd

MEnd


## FLY UP ###########################################################

Move Hoverboard_flyUPpre
	Requires	JUMP FLY HOVERBOARD
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <Jumps> <FlyCombatAttack> Hoverboard_fly Hoverboard_flyready <WepMove> <Jumps> <AttackIRQ> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> DualWield_jumpPre <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyUPCycle
	Priority	13
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\hoverboard_up 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_up 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_up 15 31
	  PlayFx	FEET\HoverboardForward.fx
	TEnd

MEnd

Move Hoverboard_flyUPCycle
	Requires	JUMP FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove> Hoverboard_flyUPpre
	Flags		Cycle #hideshield #hideweapon
	NextMove	Hoverboard_flyUPPost
	Priority	13
	Wind		FLYUP

	Type		male
	  Anim		male\hoverboard_up 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_up 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_up 1 31
	TEnd

MEnd

Move Hoverboard_flyUPPost
	Requires	FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyready
	Priority	12
	Wind		FLY

	Type		male
	  Anim		male\hoverboard_up_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_up_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_up_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

MEnd

## FLY RIGHT ###########################################################

Move Hoverboard_RightflyPre
	Requires	STEPRIGHT FLY HOVERBOARD
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <TwoHandCombat> <TwoHandMove> Hoverboard_fly Hoverboard_flyready leftflyCycle <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_RightflyCycle
	Priority	13

	Type		male
	  Anim		male\hoverboard_right 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_right 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_right 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

MEnd

Move Hoverboard_RightflyCycle
	Requires	STEPRIGHT FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle #hideshield #hideweapon
	NextMove	Hoverboard_RightflyPost
	Priority	13

	Type		male
	  Anim		male\hoverboard_right 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_right 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_right 1 31
	TEnd

MEnd

Move Hoverboard_RightflyPost
	Requires	FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyready
	Priority	12

	Type		male
	  Anim		male\hoverboard_right_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_right_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_right_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

MEnd

## FLY LEFT ###########################################################

Move Hoverboard_LeftflyPre
	Requires	STEPLEFT FLY HOVERBOARD
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> Hoverboard_fly Hoverboard_flyready rightflyCycle <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_LeftflyCycle
	Priority	13

	Type		male
	  Anim		male\hoverboard_left 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_left 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_left 15 31
	  PlayFx	FEET\HoverboardLateral.fx
	TEnd

MEnd

Move Hoverboard_LeftflyCycle
	Requires	STEPLEFT FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle #hideshield #hideweapon
	NextMove	Hoverboard_LeftflyPost
	Priority	13

	Type		male
	  Anim		male\hoverboard_left 1 31
	TEnd

	Type		huge
	  Anim		huge\hoverboard_left 1 31
	TEnd

	Type		fem
	  Anim		fem\hoverboard_left 1 31
	TEnd

MEnd

Move Hoverboard_LeftflyPost
	Requires	FLY HOVERBOARD
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs #hideshield #hideweapon
	NextMove	Hoverboard_flyready
	Priority	12

	Type		male
	  Anim		male\hoverboard_left_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		huge
	  Anim		huge\hoverboard_left_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

	Type		fem
	  Anim		fem\hoverboard_left_post 1
	  PlayFx	FEET\HoverboardHover.fx
	TEnd

MEnd


## FALL ###########################################################

Move Hoverboard_PreFall
	Requires	HOVERBOARD AIR
	Priority	2
	#Interrupts	<MoveIRQ>
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <Fall>
	NextMove	Hoverboard_PreFall_Post

	Type		male
	  Anim		male\hoverboard_flydown 1 15
	TEnd

	Type		huge
	  Anim		huge\hoverboard_flydown 1 15
	TEnd

	Type		fem
	  Anim		fem\hoverboard_flydown 1 15
	TEnd

MEnd

Move Hoverboard_PreFall_Post
	Requires	HOVERBOARD AIR
	Priority	2
	#Interrupts	<MoveIRQ>
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	PreFall

	Type		male
	  Anim		male\flydown 15
	TEnd

	Type		huge
	  Anim		huge\flydown 15
	TEnd

	Type		fem
	  Anim		fem\fly_down 15
	TEnd
MEnd

## FLY FROM FALL ###########################################################s

Move Hoverboard_Fall_Fready
	Requires	FLY HOVERBOARD
	Priority	2
	Interrupts	<Fall>
	Include sequencers\Members\Readies_Air.inc
	NextMove	Hoverboard_flyready

	Type		male
	  Anim		male\hoverboard_flydown 180 194
	TEnd

	Type		huge
	  Anim		huge\hoverboard_flydown 180 194
	TEnd

	Type		fem
	  Anim		fem\hoverboard_flydown 180 194
	TEnd

MEnd

##############################################################
### HOVERS
##############################################################
#
### HOVER FORWARD ###########################################################
#
#Move Hover_Forward_Pre
#	Requires	FORWARD	FLY HOVER
#	Priority	13
#	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicFlyMove> <Fall> <Jumps> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Hover_ForwardCycle
#	Scale		0.875
#
#	Type		male
#	  Anim		male\hover_forward 1 10
#	  PlayFx	FEET\HoverboardForward.fx
#	TEnd
#MEnd
#
#Move Hover_ForwardCycle
#	Requires	FORWARD	FLY HOVER
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		Cycle ReqInputs
#	NextMove	Hover_Forward_Post
#	Scale		0.625
#
#	Type		male
#	  Anim		male\hover_forward 10 70
#	TEnd
#MEnd
#
#Move Hover_Forward_Post
#	Requires	FLY
#	Priority	11
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	flyready
#	Scale		0.875
#
#	Type		male
#	  Anim		male\hover_forward 71
#	TEnd
#MEnd
#
### FLIGHT TO HOVER TRANSITION ###########################################################
#
#Move Flight_to_Hover_Trans
#	Requires	FORWARD	FLY HOVER
#	Priority	13
#	Interrupts	flypre fly
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Flight_to_Hover_Blend
#	Scale		1.5
#
#	Type		male
#	  Anim		male\fly 82 97
#	TEnd
#MEnd
#
#Move Flight_to_Hover_Blend
#	Requires	FORWARD	FLY HOVER
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Hover_Forward
#	Interpolate	20
#	Scale		0.75
#
#	Type		male
#	  Anim		male\hover_forward 5 70
#	TEnd
#MEnd
#
#
##############################################################
#
### HOVER UP ###########################################################
#
#Move Hover_Up_Pre
#	Requires	JUMP FLY HOVER
#	Priority	13
#	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicFlyMove> <Fall> <Jumps> <MoveIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Hover_Up_Cycle
#	Scale		0.90
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\flyup 5 34
#	  PlayFx	FEET\HoverboardForward.fx
#	TEnd
#MEnd
#
#Move Hover_Up_Cycle
#	Requires	JUMP FLY HOVER
#	Priority	13
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Hover_Up_Post
#	Wind		FLYUP
#
#	Type		male
#	  Anim		male\flyup 36 93
#	TEnd
#MEnd
#
#Move Hover_Up_Post
#	Requires	FLY HOVER
#	Priority	12
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <HoverMove> <FlyMoveALL> <AttackIRQ> <MoveIRQ>
#	Flags		ReqInputs
#	NextMove	flyready
#	Wind		FLY
#
#	Type		male
#	  Anim		male\flyup 94
#	TEnd
#MEnd