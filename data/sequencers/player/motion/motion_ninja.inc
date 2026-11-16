#############################################################
## motion_Ninja.inc - Covers Ready and Combat modes
#############################################################

## READY ###########################################################

Move Ninja_Ready_Pre
	Requires	NINJARUN #COMBAT
	Priority	2
	Interrupts	<Ready> <Combat>
	Include sequencers\Members\NinjaCombat.inc
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_Ready
	Requires	NINJARUN #COMBAT
	Include sequencers\Members\NinjaCombat.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_Forward
	Requires	NINJARUN FORWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ForwardPost
	CycleMove	Ninja_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Forward_Cycle
	Requires	NINJARUN FORWARD #COMBAT
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ForwardPost

	Type		male
	  Anim		male\ninja_run_forward 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_ForwardPost
	Include sequencers\Members\NinjaCombat.inc
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_Back
	Requires	NINJARUN BACKWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_Back_Cycle
	NextMove	Ninja_ForwardPost

	Type		male
	  Anim		male\ninja_run_backward_pre 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Back_Cycle
	Requires	NINJARUN BACKWARD #COMBAT
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_Back_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat.inc
	CycleMove	Ninja_Ready
	NextMove	postReady_back
	Flags		Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_Left
	Requires	NINJARUN STEPLEFT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	CycleMove	Ninja_Left_Cycle
	NextMove	Ninja_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Left_Cycle
	Requires	NINJARUN STEPLEFT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_Left_Post

	Type		male
	  Anim		male\ninja_run_left 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_Left_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat.inc
	Flags		Cycle
	CycleMove	Ninja_Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_Right
	Requires	NINJARUN STEPRIGHT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_ForwardPost
	CycleMove	Ninja_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Right_Cycle
	Requires	NINJARUN STEPRIGHT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_Right_Post

	Type		male
	  Anim		male\ninja_run_right 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_Right_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat.inc
	Flags		Cycle
	CycleMove	Ninja_Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_Jump_Pre
	Requires	NINJARUN JUMP AIR #COMBAT
	Priority	4
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Ninja_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_Jump
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Ninja_Jump_Apex #Apex is for short jumps
	CycleMove	Ninja_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump 6 10
	TEnd
MEnd

Move Ninja_Jump_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Ninja_Jump_Apex2 #Apex2 is for higher jumps

	Type		male
	  Anim		male\ninja_jump 6 40
	TEnd
MEnd

#############################################################

Move Ninja_Jump_Apex #Apex is for short jumps
	Requires	NINJARUN APEX AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Jump_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Jump_Apex2 #Apex2 is for higher jumps
	Requires	NINJARUN APEX AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Jump_Fall2

	Type		male
	  Anim		male\ninja_jump 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_Jump_Fall
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Jump_Fall_Cycle
	NextMove	Ninja_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump 46 53
	TEnd
MEnd

Move Ninja_Jump_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Jump_Post

	Type		male
	  Anim		male\ninja_jump 54 54
	TEnd
MEnd

#############################################################

Move Ninja_Jump_Fall2
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Jump_Fall_Cycle2
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_jump 66 93
	TEnd
MEnd

Move Ninja_Jump_Fall_Cycle2
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_jump 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_Jump_Post2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_Jump_PostCycle
	Requires	NINJARUN #COMBAT
	Include sequencers\Members\NinjaCombat.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	15

	Type		male
	  Anim		male\ninja_cycle 17 47
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR #COMBAT
	Priority	5
	Interrupts	<TravelJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop2
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx 2
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_HopPre
	Requires	NINJARUN FORWARD JUMP AIR #COMBAT
	Priority	5
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop 1 2
	TEnd
MEnd

Move Ninja_Hop
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop2
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Hop2
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_HopApex

	Type		male
	  Anim		male\ninja_hop 82 82
	TEnd
MEnd

Move Ninja_HopApex
	Requires	NINJARUN APEX AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_HopFall

	Type		male
	  Anim		male\ninja_hop 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_HopFall
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_HopFall_2

	Type		male
	  Anim		male\ninja_hop 85 124
	TEnd
MEnd

Move Ninja_HopFall_2
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR #COMBAT
	Priority	5
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop_Right

	Type		male
	  Anim		male\ninja_hop_right 1 2
	TEnd
MEnd

Move Ninja_Hop_Right
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop2_Right
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Hop2_Right
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_HopApex_Right

	Type		male
	  Anim		male\ninja_hop_right 82 82
	TEnd
MEnd

Move Ninja_HopApex_Right
	Requires	NINJARUN APEX AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_HopFall_Right

	Type		male
	  Anim		male\ninja_hop_right 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_HopFall_Right
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_HopFall2_Right
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_right 85 124
	TEnd
MEnd

Move Ninja_HopFall2_Right
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_right 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_HopPre_Left
	Requires	NINJARUN STEPLEFT JUMP AIR #COMBAT
	Priority	5
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_Left 1 2
	TEnd
MEnd

Move Ninja_Hop_Left
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Hop2_Left
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_Left 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Hop2_Left
	Requires	NINJARUN JUMP AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_HopApex_Left

	Type		male
	  Anim		male\ninja_hop_Left 82 82
	TEnd
MEnd

Move Ninja_HopApex_Left
	Requires	NINJARUN APEX AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_HopFall_Left

	Type		male
	  Anim		male\ninja_hop_Left 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_HopFall_Left
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_HopFall2_Left
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_Left 85 124
	TEnd
MEnd

Move Ninja_HopFall2_Left
	Requires	NINJARUN AIR #COMBAT
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_Left 124
	TEnd
MEnd

#############################################################

Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Righthand.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Righthand_Gun.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Dual.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Dual_Pistols.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Dual_Claws.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Dual_Lefthand.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Shield.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_ShieldWeapon.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_Two_Handed_Weapon.inc
Include sequencers\player\Motion\Ninja\motion_Ninja_CombatWeapon_BeamRifle.inc