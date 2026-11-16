#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## CLAW	READIES	###########################################################

#Move Ready_To_NinjaClawReady
#	Requires	NINJARUN COMBAT WEAPON DUAL CLAW
#	Priority	5
#	Interrupts	<NinjaCombat> <NinjaWepCombat>
#	Include sequencers\Members\NinjaCombat_DualWeapon.inc
#	NextMove	Ninja_Claws_Pre
#	Flags		NoSizeScale
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim		male\claw_out 1 19
#	TEnd
#MEnd

## READY ###########################################################

Move Ninja_Claws_Pre
	Requires	NINJARUN COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <DualHandCombat>
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_Claws_Ready
	NextMove	ClawReady
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_dualpistols 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_Claws_Ready
	Requires	NINJARUN COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	NextMove	ClawReady
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_dualpistols 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_Claws_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Forward_Post
	CycleMove	Ninja_Claws_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_dualpistols 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Claws_Forward_Cycle
	Requires	NINJARUN FORWARD #WEAPON CLAW
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_dualpistols 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_Claws_Forward_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_Claws_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_dualpistols 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_Claws_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_Claws_Back_Cycle
	NextMove	Ninja_Claws_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_dualpistols 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Claws_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL CLAW
 	Priority	6 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_dualpistols 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_Claws_Back_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_Claws_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_dualpistols 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_Claws_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Ninja_Claws_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Left_Post
	CycleMove	Ninja_Claws_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_dualpistols 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Claws_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	7 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Left_Post

	Type		male
	  Anim		male\ninja_run_left_dualpistols 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_Claws_Left_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_Claws_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_dualpistols 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_Claws_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Ninja_Claws_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Right_Post
	CycleMove	Ninja_Claws_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_dualpistols 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Claws_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	7 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Claws_Right_Post

	Type		male
	  Anim		male\ninja_run_right_dualpistols 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_Claws_Right_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_Claws_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_dualpistols 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_Claws_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	NextMove	Ninja_Claws_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump_dualpistols 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Jump
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	Flags		Cycle
	NextMove	Ninja_Claws_Apex ##Apex is for short jumps
	CycleMove	Ninja_Claws_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_dualpistols 6 10
	TEnd
MEnd

Move Ninja_Claws_Jump_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	Flags		Cycle
	NextMove	Ninja_Claws_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_dualpistols 6 40
	TEnd
MEnd

#############################################################

Move Ninja_Claws_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Claws_Fall
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_dualpistols 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Claws_Fall2

	Type		male
	  Anim		male\ninja_jump_dualpistols 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_Claws_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Claws_Fall_Cycle
	NextMove	Ninja_Claws_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_dualpistols 46 53
	TEnd
MEnd

Move Ninja_Claws_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Jump_Post

	Type		male
	  Anim		male\ninja_jump_dualpistols 54 54
	TEnd
MEnd

#############################################################

Move Ninja_Claws_Fall2
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Claws_Fall2_Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_dualpistols 66 93
	TEnd
MEnd

Move Ninja_Claws_Fall2_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_dualpistols 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_Claws_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Claws_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_dualpistols 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_Claws_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Claws_Ready
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\ninja_jump_dualpistols 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_Claws_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<TravelClawsJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_dualpistols 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_Claws_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_dualpistols 1 2
	TEnd
MEnd

Move Ninja_Claws_Hop
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_dualpistols 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_dualpistols 82 82
	TEnd
MEnd

Move Ninja_Claws_Hop_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Claws_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_dualpistols 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_dualpistols 85 124
	TEnd
MEnd

Move Ninja_Claws_Hop_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_dualpistols 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_Claws_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 1 2
	TEnd
MEnd

Move Ninja_Claws_Hop_Right
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 82 82
	TEnd
MEnd

Move Ninja_Claws_Hop_Right_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Claws_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Right_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Claws_Hop_Right_Fall_Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 85 124
	TEnd
MEnd

Move Ninja_Claws_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_dualpistols 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_Claws_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 1 2
	TEnd
MEnd

Move Ninja_Claws_Hop_Left
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Claws_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 82 82
	TEnd
MEnd

Move Ninja_Claws_Hop_Left_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Claws_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Claws_Hop_Left_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Claws_Hop_Left_Fall_Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 85 124
	TEnd
MEnd

Move Ninja_Claws_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Claws_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_dualpistols 124
	TEnd
MEnd