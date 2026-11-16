#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

#Move Ready_To_NinjaDualWield
#	Requires	NINJARUN COMBAT WEAPON DUAL
#	Priority	4
#	Interrupts	<NinjaCombat> <NinjaWepCombat>
#	Include sequencers\Members\NinjaCombat_DualWeapon.inc
#	NextMove        Ninja_DualWield_Pre
#	Flags		NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#	Scale		1.25
#
#	Type		male
#	  Anim		male\DualBlade_Draw 1 25
#	TEnd
#MEnd

## READY ###########################################################

Move Ninja_DualWield_Pre
	Requires	NINJARUN COMBAT WEAPON DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat>
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_DualWield_Ready
	NextMove	DualWield_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_weapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_DualWield_Ready
	Requires	NINJARUN COMBAT WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	NextMove	DualWield_Ready
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_weapon 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_DualWield_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Forward_Post
	CycleMove	Ninja_DualWield_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_Weapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_DualWield_Forward_Cycle
	Requires	NINJARUN FORWARD #WEAPON CLAW
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_Weapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_DualWield_Forward_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_DualWield_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_Weapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_DualWield_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_DualWield_Back_Cycle
	NextMove	Ninja_DualWield_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_Weapon 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_DualWield_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL
 	Priority	6 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_Weapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_DualWield_Back_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_DualWield_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_Weapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_DualWield_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Ninja_DualWield_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Left_Post
	CycleMove	Ninja_DualWield_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_Weapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_DualWield_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL
	Priority	6 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Left_Post

	Type		male
	  Anim		male\ninja_run_left_Weapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_DualWield_Left_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_DualWield_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_Weapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_DualWield_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Ninja_DualWield_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Right_Post
	CycleMove	Ninja_DualWield_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_Weapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_DualWield_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL
	Priority	6 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_DualWield_Right_Post

	Type		male
	  Anim		male\ninja_run_right_Weapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_DualWield_Right_Post
	Requires	NINJARUN COMBAT #WEAPON DUAL
	Include sequencers\Members\NinjaCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Ninja_DualWield_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_Weapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_DualWield_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON DUAL
	Priority	6
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	NextMove	Ninja_DualWield_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump_Weapon 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Jump
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Apex ##Apex is for short jumps
	CycleMove	Ninja_DualWield_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_Weapon 6 10
	TEnd
MEnd

Move Ninja_DualWield_Jump_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	Flags		Cycle
	NextMove	Ninja_DualWield_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_Weapon 6 40
	TEnd
MEnd

#############################################################

Move Ninja_DualWield_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_DualWield_Fall
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_Weapon 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_DualWield_Fall2

	Type		male
	  Anim		male\ninja_jump_Weapon 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_DualWield_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_DualWield_Fall_Cycle
	NextMove	Ninja_DualWield_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_Weapon 46 53
	TEnd
MEnd

Move Ninja_DualWield_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Jump_Post

	Type		male
	  Anim		male\ninja_jump_Weapon 54 54
	TEnd
MEnd

#############################################################

Move Ninja_DualWield_Fall2
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_DualWield_Fall2_Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_Weapon 66 93
	TEnd
MEnd

Move Ninja_DualWield_Fall2_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_Weapon 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_DualWield_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_DualWield_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_Weapon 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_DualWield_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_DualWield_Ready
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\ninja_jump_Weapon 130 150
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

Move Ninja_DualWield_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<TravelDualWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_Weapon 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_DualWield_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_Weapon 1 2
	TEnd
MEnd

Move Ninja_DualWield_Hop
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_Weapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_Weapon 82 82
	TEnd
MEnd

Move Ninja_DualWield_Hop_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_DualWield_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_Weapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_Weapon 85 124
	TEnd
MEnd

Move Ninja_DualWield_Hop_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_Weapon 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_DualWield_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_Weapon 1 2
	TEnd
MEnd

Move Ninja_DualWield_Hop_Right
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_Weapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_Weapon 82 82
	TEnd
MEnd

Move Ninja_DualWield_Hop_Right_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_DualWield_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_Weapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Right_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_DualWield_Hop_Right_Fall_Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_Weapon 85 124
	TEnd
MEnd

Move Ninja_DualWield_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_Weapon 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_DualWield_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_Weapon 1 2
	TEnd
MEnd

Move Ninja_DualWield_Hop_Left
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_DualWield_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_Weapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_Weapon 82 82
	TEnd
MEnd

Move Ninja_DualWield_Hop_Left_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_DualWield_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_Weapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_DualWield_Hop_Left_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_DualWield_Hop_Left_Fall_Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_Weapon 85 124
	TEnd
MEnd

Move Ninja_DualWield_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_DualWield_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_Weapon 124
	TEnd
MEnd