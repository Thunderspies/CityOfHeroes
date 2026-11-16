#############################################################
## motion_Ninja_CombatWeapon_Two_Handed_Weapon.inc
#############################################################

## READY ###########################################################

Move Ninja_2hLarge_Ready_Pre
	Requires	NINJARUN COMBAT WEAPON 2hLarge
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <TravelCombat>
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_Ready
	NextMove	2hlargeReady
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_2HLARGE 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd

	Type		fem
	  Anim		fem\ninja_cycle_2HLARGE 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_2HLARGE_Ready
	Requires	NINJARUN COMBAT WEAPON 2HLARGE
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	2hlargeReady
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_2HLARGE 17 47
	TEnd

	Type		fem
	  Anim		fem\ninja_cycle_2HLARGE 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_2HLARGE_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_2HLARGE_ForwardPost
	CycleMove	Ninja_2HLARGE_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_forward_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_2HLARGE_Forward_Cycle
	Requires	NINJARUN FORWARD COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_2HLARGE_ForwardPost

	Type		male
	  Anim		male\ninja_run_forward_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_forward_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_2HLARGE_ForwardPost
	Include sequencers\Members\NinjaCombat_Weapon.inc
	NextMove	Ninja_2HLARGE_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_2HLARGE 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd

	Type		fem
	  Anim		fem\ninja_cycle_2HLARGE 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_2HLARGE_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide>  <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_2HLARGE_Back_Cycle
	NextMove	Ninja_2HLARGE_ForwardPost

	Type		male
	  Anim		male\ninja_run_backward_pre_2HLARGE 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

MEnd

Move Ninja_2HLARGE_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON 2HLARGE
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_2HLARGE_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_backward_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_2HLARGE_Back_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	CycleMove	Ninja_2HLARGE_Ready
	NextMove	Ninja_Ready
	Flags		Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_2HLARGE 1 20
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd

MEnd

## RUN LEFT ###########################################################

Move Ninja_2HLARGE_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	CycleMove	Ninja_2HLARGE_Left_Cycle
	NextMove	Ninja_2HLARGE_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_left_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_2HLARGE_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON 2HLARGE
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_2HLARGE_Left_Post

	Type		male
	  Anim		male\ninja_run_left_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_left_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_2HLARGE_Left_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_Ready
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_2HLARGE 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd

MEnd

## RUN RIGHT ###########################################################

Move Ninja_2HLARGE_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_2HLARGE_ForwardPost
	CycleMove	Ninja_2HLARGE_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_right_2HLARGE 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_2HLARGE_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON 2HLARGE
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_2HLARGE_Right_Post

	Type		male
	  Anim		male\ninja_run_right_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\ninja_run_right_2HLARGE 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_2HLARGE_Right_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_Ready
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_2HLARGE 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd

MEnd

## JUMP UP ###########################################################

Move Ninja_2HLARGE_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_Jump

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_Jump
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Apex #Apex is for short jumps
	CycleMove	Ninja_2HLARGE_Jump_Cycle

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 6 10
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 6 10
	TEnd
MEnd

Move Ninja_2HLARGE_Jump_Cycle
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Apex2 #Apex2 is for higher jumps

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 6 40
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 6 40
	TEnd
MEnd

#############################################################

Move Ninja_2HLARGE_Jump_Apex #Apex is for short jumps
	Requires	NINJARUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_Jump_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_Jump_Apex2 #Apex2 is for higher jumps
	Requires	NINJARUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_Jump_Fall2

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_2HLARGE_Jump_Fall
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_Jump_Fall_Cycle
	NextMove	Ninja_2HLARGE_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 46 53
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 46 53
	TEnd
MEnd

Move Ninja_2HLARGE_Jump_Fall_Cycle
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Post

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 54 54
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 54 54
	TEnd
MEnd

#############################################################

Move Ninja_2HLARGE_Jump_Fall2
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_Jump_Fall_Cycle2
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 66 93
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 66 93
	TEnd
MEnd

Move Ninja_2HLARGE_Jump_Fall_Cycle2
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 93 130
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_2HLARGE_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_2HLARGE_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_2HLARGE_Jump_Post2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_2HLARGE_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_run_jump_2HLARGE 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd

	Type		fem
	  Anim		fem\ninja_run_jump_2HLARGE 130 150
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

Move Ninja_2HLARGE_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	Ninja_2HLARGE_Jump_Pre Ninja_2HLARGE_Jump Ninja_2HLARGE_Jump_Cycle
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop2
	Interpolate	15

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx 2
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_2HLARGE_HopPre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 1 2
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 1 2
	TEnd
MEnd

Move Ninja_2HLARGE_Hop
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop2
	Scale		0.75

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_Hop2
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_HopApex

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 82 82
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 82 82
	TEnd
MEnd

Move Ninja_2HLARGE_HopApex
	Requires	NINJARUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_HopFall

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_HopFall_2

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 85 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 85 124
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall_2
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_hop_2HLARGE 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_2HLARGE 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_2HLARGE_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop_Right
	NextMove	Ninja_2HLARGE_Hop

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 1 2
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 1 2
	TEnd
MEnd

Move Ninja_2HLARGE_Hop_Right
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop2_Right
	Scale		0.75

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_Hop2_Right
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_HopApex_Right

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 82 82
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 82 82
	TEnd
MEnd

Move Ninja_2HLARGE_HopApex_Right
	Requires	NINJARUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_HopFall_Right

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall_Right
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_HopFall2_Right
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 85 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 85 124
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall2_Right
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_hop_right_2HLARGE 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_right_2HLARGE 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_2HLARGE_HopPre_Left
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop_Left
	NextMove	Ninja_2HLARGE_Hop

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 1 2
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 1 2
	TEnd
MEnd

Move Ninja_2HLARGE_Hop_Left
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_2HLARGE_Hop2_Left
	Scale		0.75

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_Hop2_Left
	Requires	NINJARUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_HopApex_Left

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 82 82
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 82 82
	TEnd
MEnd

Move Ninja_2HLARGE_HopApex_Left
	Requires	NINJARUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_2HLARGE_HopFall_Left

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall_Left
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_2HLARGE_HopFall2_Left
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 85 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 85 124
	TEnd
MEnd

Move Ninja_2HLARGE_HopFall2_Left
	Requires	NINJARUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_2HLARGE_Jump_Post2

	Type		male
	  Anim		male\ninja_run_hop_Left_2HLARGE 124
	TEnd

	Type		fem
	  Anim		fem\ninja_run_hop_Left_2HLARGE 124
	TEnd
MEnd