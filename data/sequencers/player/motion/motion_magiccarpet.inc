#############################################################
## FLIGHT MOVEMENT
#############################################################
#
## FLY READY ###########################################################

Move MagicCarpet_Preflyready
	Requires	MAGICCARPET FLY
	Priority	12
	Interrupts	<PostMoveCycle> <Ready> <BasicMove> <iceslide> <Swim> <jumpFall> <Block> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <DualHandMove> <IRQEmotesDualhand> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <BlockIRQ>
	NextMove	MagicCarpet_flyready
	Flags		Nosizescale hideshield hideweapon
	SticksOnChild   MAGICCARPET FLY

	Type		male
	  Anim		male\magiccarpet_prefly 2 78
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_prefly 2 78
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_prefly 2 78
	TEnd

MEnd

Move MagicCarpet_flyready
	Requires	MAGICCARPET FLY
	Priority	12
	Member		<DeathIRQ> <MoveIRQ> #<ReactIRQ> <HitIRQ> <BlockIRQ> #<FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle hideshield hideweapon
	NextMove	MagicCarpet_PreFall
#	SticksOnChild   MAGICCARPET FLY

	Type		male
	  Anim		male\magiccarpet_flycycle 1 60
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle 1 60
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle 1 60
	TEnd

MEnd

### FLY FORWARD FROM RUN ###########################################################
#
#Move MagicCarpet_flypre_Launch
#	Requires	FORWARD FLY MAGICCARPET
#	Priority	14
#	Interrupts	RunPre RunCycle <GroundMoveALL> <Launch> <FlyCombat> <FlyCombatAttack> <PostFLYCycle> <iceslide> <MoveIRQ> <FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand>  <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove>
#	Flags		ReqInputs hideshield hideweapon
#	NextMove	MagicCarpet_fly
#	Wind		FLY
#
#	Type		male
#	  Anim		male\magiccarpet_prefly 5
#	TEnd
#
#	Type		huge
#	  Anim		huge\magiccarpet_prefly 5
#	TEnd
#
#	Type		fem
#	  Anim		fem\magiccarpet_prefly 5
#	TEnd
#
#
#MEnd

## FLY FORWARD ###########################################################

Move MagicCarpet_flypre
	Requires	FORWARD	FLY MAGICCARPET
	Priority	13
	Interrupts	<FlyReady> <MoveIRQ> <MagicCarpet_flyPost> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <Jumps> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <iceslide> <FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <IRQSlide> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low #<FlyDualHandMove> MAGICCARPET_flyready MAGICCARPET_flypre MAGICCARPET_Preflyready
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_fly
	Wind		FLY
	Scale		1.5

	Type		male
	  Anim		male\magiccarpet_flycycle_forward 1 30
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_forward 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_forward 1 30
	TEnd


MEnd

Move MagicCarpet_fly
	Requires	FORWARD	FLY MAGICCARPET
	Priority	13
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyForwardAlt> <FlyCombatMove> <FlyWepMove> MAGICCARPET_flypre
	Flags		Cycle ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flypost
	Wind		FLY

	Type		male
	  Anim		male\magiccarpet_flycycle_forward 31 90
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_forward 31 90
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_forward 31 90
	TEnd

MEnd

Move MagicCarpet_flyPost
	Requires	FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove>
	Flags		ReqInputs hideshield hideweapon
	Priority	12
	NextMove	MagicCarpet_flyready


	Type		male
	  Anim		male\magiccarpet_flycycle_forward 90 110
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_forward 90 110
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_forward 90 110
	TEnd

MEnd


## FLY BACKWARD	###########################################################

Move MagicCarpet_BackflyPre
	Requires	BACKWARD FLY MAGICCARPET
	Interrupts	<FlyReady> <MagicCarpet_flyPost> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> DualWield_BackflyPre <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low #MAGICCARPET_fly MAGICCARPET_flyready
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_BackflyCycle
	Priority	13
	Scale		1.5

	Type		male
	  Anim		male\magiccarpet_flycycle_backward 1 30
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_backward 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_backward 1 30
	TEnd

MEnd

Move MagicCarpet_BackflyCycle
	Requires	BACKWARD FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle hideshield hideweapon
	NextMove	MagicCarpet_BackflyPost
	Priority	13

	Type		male
	  Anim		male\magiccarpet_flycycle_backward 31 90
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_backward 31 90
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_backward 31 90
	TEnd

MEnd

Move MagicCarpet_BackflyPost
	Requires	FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flyready
	Priority	12

	Type		male
	  Anim		male\magiccarpet_flycycle_backward 91 110
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_backward 91 110
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_backward 91 110
	TEnd

MEnd


## FLY UP ###########################################################

Move MagicCarpet_flyUPpre
	Requires	JUMP FLY MAGICCARPET
	Interrupts	<FlyReady> <MagicCarpet_flyPost> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <Jumps> <FlyCombatAttack> <WepMove> <Jumps> <AttackIRQ> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> DualWield_jumpPre <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <FlyReady> <SwimMove> <IRQEmotesALL> a_sweep_low #MAGICCARPET_fly MAGICCARPET_flyready
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flyUPCycle
	Priority	13
	Wind		FLYUP
	Scale		1.5

	Type		male
	  Anim		male\magiccarpet_flycycle_up 1 30
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_up 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_up 1 30
	TEnd

MEnd

Move MagicCarpet_flyUPCycle
	Requires	JUMP FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove> MAGICCARPET_flyUPpre
	Flags		Cycle hideshield hideweapon
	NextMove	MagicCarpet_flyUPPost
	Priority	13
	Wind		FLYUP

	Type		male
	  Anim		male\magiccarpet_flycycle_up 31 90
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_up 31 90
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_up 31 90
	TEnd

MEnd

Move MagicCarpet_flyUPPost
	Requires	FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flyready
	Priority	12
	Wind		FLY

	Type		male
	  Anim		male\magiccarpet_flycycle_up 91 110
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_up 91 110
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_up 91 110
	TEnd

MEnd

## FLY RIGHT ###########################################################

Move MagicCarpet_RightflyPre
	Requires	STEPRIGHT FLY MAGICCARPET
	Interrupts	<FlyReady> <MagicCarpet_flyPost> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <SwimMove> <IRQEmotesALL> a_sweep_low leftflyCycle #MAGICCARPET_fly MAGICCARPET_flyready
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_RightflyCycle
	Priority	13
	Scale		1.5

	Type		male
	  Anim		male\magiccarpet_flycycle_right 1 30
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_right 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_right 1 30
	TEnd

MEnd

Move MagicCarpet_RightflyCycle
	Requires	STEPRIGHT FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle hideshield hideweapon
	NextMove	MagicCarpet_RightflyPost
	Priority	13

	Type		male
	  Anim		male\magiccarpet_flycycle_right 31 90
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_right 31 90
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_right 31 90
	TEnd

MEnd

Move MagicCarpet_RightflyPost
	Requires	FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flyready
	Priority	12

	Type		male
	  Anim		male\magiccarpet_flycycle_right 91 110
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_right 91 110
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_right 91 110
	TEnd

MEnd

## FLY LEFT ###########################################################

Move MagicCarpet_LeftflyPre
	Requires	STEPLEFT FLY MAGICCARPET
	Interrupts	<FlyReady> <MagicCarpet_flyPost> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide> <MoveIRQ> <FlyCombat> <FlyCombatAttack> <TwoHandCombat> <TwoHandMove> <FlyDualHandCombat> <IRQEmotesDualhand> <DualHandMove> <RunsALL> <GroundMoveALL> <FlyDualHandMove> <FlyMoveALL> <FlyWepCombat> <IRQEmotesRighthand> <FlyCombat> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEncounters> <ShieldMove> <ShieldFLYMove> <SwimMove> <IRQEmotesALL> a_sweep_low rightflyCycle #MAGICCARPET_fly MAGICCARPET_flyready
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_LeftflyCycle
	Priority	13
	Scale		1.5

	Type		male
	  Anim		male\magiccarpet_flycycle_left 1 30
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_left 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_left 1 30
	TEnd

MEnd

Move MagicCarpet_LeftflyCycle
	Requires	STEPLEFT FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <FlyCombatMove>
	Flags		Cycle hideshield hideweapon
	NextMove	MagicCarpet_LeftflyPost
	Priority	13

	Type		male
	  Anim		male\magiccarpet_flycycle_left 31 90
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_left 31 90
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_left 31 90
	TEnd

MEnd

Move MagicCarpet_LeftflyPost
	Requires	FLY MAGICCARPET
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <BasicFlyMove> <FlyMoveALL> <MoveIRQ> <FlyCombatMove>
	Flags		ReqInputs hideshield hideweapon
	NextMove	MagicCarpet_flyready
	Priority	12

	Type		male
	  Anim		male\magiccarpet_flycycle_left 91 110
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle_left 91 110
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle_left 91 110
	TEnd

MEnd


## FALL ###########################################################

Move MagicCarpet_PreFall
	Requires	MAGICCARPET AIR
	Priority	2
	#Interrupts	<MoveIRQ>
	Flags		hideshield hideweapon
	Member		<DeathIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ> <Fall>
	NextMove	MagicCarpet_PreFall_Post

	Type		male
	  Anim		male\magiccarpet_flycycle 1 2
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle 1 30
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle 1 30
	TEnd

MEnd

Move MagicCarpet_PreFall_Post
	Requires	MAGICCARPET AIR
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

Move MagicCarpet_Fall_Fready
	Requires	FLY MAGICCARPET
	Priority	2
	Interrupts	<Fall>
	Flags		hideshield hideweapon
	Include sequencers\Members\Readies_Air.inc
	NextMove	MagicCarpet_flyready

	Type		male
	  Anim		male\magiccarpet_flycycle 1 10
	TEnd

	Type		huge
	  Anim		huge\magiccarpet_flycycle 1 10
	TEnd

	Type		fem
	  Anim		fem\magiccarpet_flycycle 1 10
	TEnd

MEnd