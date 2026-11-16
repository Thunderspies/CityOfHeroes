#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## READY ###########################################################

Move Ninja_ShieldCombat_Pre
	Requires	NINJARUN SHIELD #COMBAT
	Priority	23
	Interrupts	<Ready> <ShieldCombat>
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_shield 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_ShieldCombat_Ready
	Requires	NINJARUN SHIELD #COMBAT
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_shield 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_ShieldCombat_Forward
	Requires	NINJARUN FORWARD SHIELD #COMBAT
	Priority	24
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Forward_Post
	CycleMove	Ninja_ShieldCombat_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_shield 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldCombat_Forward_Cycle
	Requires	NINJARUN FORWARD SHIELD #COMBAT
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_shield 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_ShieldCombat_Forward_Post
	Requires	NINJARUN SHIELD #COMBAT
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_shield 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_ShieldCombat_Back
	Requires	NINJARUN BACKWARD SHIELD #COMBAT
	Priority	24
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_ShieldCombat_Back_Cycle
	NextMove	Ninja_ShieldCombat_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_shield 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldCombat_Back_Cycle
	Requires	NINJARUN BACKWARD SHIELD #COMBAT
 	Priority	25 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_shield 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_ShieldCombat_Back_Post
	Requires	NINJARUN SHIELD #COMBAT
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_shield 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_ShieldCombat_Left
	Requires	NINJARUN STEPLEFT SHIELD #COMBAT
	Priority	24
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Left_Post
	CycleMove	Ninja_ShieldCombat_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_shield 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldCombat_Left_Cycle
	Requires	NINJARUN STEPLEFT SHIELD #COMBAT
	Priority	24 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Left_Post

	Type		male
	  Anim		male\ninja_run_left_shield 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_ShieldCombat_Left_Post
	Requires	NINJARUN SHIELD #COMBAT
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_shield 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_ShieldCombat_Right
	Requires	NINJARUN STEPRIGHT SHIELD #COMBAT
	Priority	24
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Right_Post
	CycleMove	Ninja_ShieldCombat_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_shield 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldCombat_Right_Cycle
	Requires	NINJARUN STEPRIGHT SHIELD #COMBAT
	Priority	25 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldCombat_Right_Post

	Type		male
	  Anim		male\ninja_run_right_shield 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_ShieldCombat_Right_Post
	Requires	NINJARUN SHIELD #COMBAT
	Include sequencers\Members\NinjaCombat_Shield.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_shield 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_ShieldCombat_Jump_Pre
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Priority	24
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldJump>
	NextMove	Ninja_ShieldCombat_Jump

	Type		male
	  Anim		male\ninja_jump_shield 3 6
	TEnd
MEnd

Move Ninja_ShieldCombat_Jump
	Requires	NINJARUN JUMP AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldJump>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Apex ##Apex is for short jumps
	CycleMove	Ninja_ShieldCombat_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_shield 6 10
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Jump_Cycle
	Requires	NINJARUN JUMP AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldJump>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_shield 6 40
	TEnd
MEnd

#############################################################

Move Ninja_ShieldCombat_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldCombat_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_shield 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldCombat_Fall2

	Type		male
	  Anim		male\ninja_jump_shield 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_ShieldCombat_Fall
	Requires	NINJARUN AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Fall_Cycle
	NextMove	Ninja_ShieldCombat_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_shield 46 53
	TEnd
MEnd

Move Ninja_ShieldCombat_Fall_Cycle
	Requires	NINJARUN AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Jump_Post

	Type		male
	  Anim		male\ninja_jump_shield 54 54
	TEnd
MEnd

#############################################################

Move Ninja_ShieldCombat_Fall2
	Requires	NINJARUN AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Fall2_Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_shield 66 93
	TEnd
MEnd

Move Ninja_ShieldCombat_Fall2_Cycle
	Requires	NINJARUN AIR #SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_shield 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_ShieldCombat_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_ShieldCombat_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_shield 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd
MEnd

Move Ninja_ShieldCombat_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_ShieldCombat_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_shield 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_ShieldCombat_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR SHIELD #COMBAT
	Priority	26
	Interrupts	<TravelShieldJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_shield 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_ShieldCombat_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR SHIELD #COMBAT
	Priority	26
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_shield 1 2
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_shield 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Cycle
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_shield 82 82
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Apex
	Requires	NINJARUN APEX AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldCombat_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_shield 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Fall
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_shield 85 124
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Fall_Cycle
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_shield 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_ShieldCombat_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR SHIELD #COMBAT
	Priority	26
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_shield 1 2
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Right
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_shield 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_shield 82 82
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Right_Apex
	Requires	NINJARUN APEX AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldCombat_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_shield 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Right_Fall
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Hop_Right_Fall_Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_shield 85 124
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_shield 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_ShieldCombat_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR SHIELD #COMBAT
	Priority	26
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_shield 1 2
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Left
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldCombat_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_shield 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_shield 82 82
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Left_Apex
	Requires	NINJARUN APEX AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldCombat_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_shield 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Left_Fall
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldCombat_Hop_Left_Fall_Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_shield 85 124
	TEnd
MEnd

Move Ninja_ShieldCombat_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR SHIELD #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_shield 124
	TEnd
MEnd