#############################################################
## motion_Ninja.inc - Covers Ready and Combat modes
#############################################################

#Move Ready_To_NinjaShotgunReady
#	Requires	NINJARUN COMBAT WEAPON GUN
#	Priority        4
#	Interrupts	<TravelCombat>
#	Include sequencers\Members\NinjaCombat_Weapon.inc
#	NextMove        D_Shotgun_cycle #Ninja_Gun_Ready_Pre
#	Flags		FixArmReg NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\draw_shotgun_basic 1 #30
#	TEnd
#MEnd

## READY ###########################################################

Move Ninja_Gun_Ready_Pre
	Requires	NINJARUN COMBAT WEAPON GUN
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <TravelCombat>
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_Gun_Ready
	NextMove	D_Shotgun_cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_rightgun 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_Gun_Ready
	Requires	NINJARUN COMBAT WEAPON GUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	D_Shotgun_cycle
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_rightgun 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_Gun_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON GUN
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Gun_ForwardPost
	CycleMove	Ninja_Gun_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_rightgun 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Gun_Forward_Cycle
	Requires	NINJARUN FORWARD COMBAT WEAPON GUN
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Gun_ForwardPost

	Type		male
	  Anim		male\ninja_run_forward_rightgun 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_Gun_ForwardPost
	Include sequencers\Members\NinjaCombat_Weapon.inc
	NextMove	Ninja_Gun_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_rightgun 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_Gun_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON GUN
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide>  <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_Gun_Back_Cycle
	NextMove	Ninja_Gun_ForwardPost

	Type		male
	  Anim		male\ninja_run_backward_pre_rightgun 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Gun_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON GUN
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_Gun_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_rightgun 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_Gun_Back_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	CycleMove	Ninja_Gun_Ready
	NextMove	Ninja_Ready
	Flags		Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_rightgun 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_Gun_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON GUN
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	CycleMove	Ninja_Gun_Left_Cycle
	NextMove	Ninja_Gun_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_rightgun 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Gun_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON GUN
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_Gun_Left_Post

	Type		male
	  Anim		male\ninja_run_left_rightgun 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_Gun_Left_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_Gun_Ready
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_rightgun 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_Gun_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON GUN
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_Gun_ForwardPost
	CycleMove	Ninja_Gun_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_rightgun 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_Gun_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON GUN
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Ninja_Gun_Right_Post

	Type		male
	  Anim		male\ninja_run_right_rightgun 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_Gun_Right_Post
	Requires	NINJARUN
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_Gun_Ready
	NextMove	Ninja_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_rightgun 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_Gun_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	NextMove	Ninja_Gun_Jump

	Type		male
	  Anim		male\ninja_jump_rightgun 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_Jump
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Apex #Apex is for short jumps
	CycleMove	Ninja_Gun_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_rightgun 6 10
	TEnd
MEnd

Move Ninja_Gun_Jump_Cycle
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Apex2 #Apex2 is for higher jumps

	Type		male
	  Anim		male\ninja_jump_rightgun 6 40
	TEnd
MEnd

#############################################################

Move Ninja_Gun_Jump_Apex #Apex is for short jumps
	Requires	NINJARUN APEX AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Gun_Jump_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_rightgun 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_Jump_Apex2 #Apex2 is for higher jumps
	Requires	NINJARUN APEX AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Gun_Jump_Fall2

	Type		male
	  Anim		male\ninja_jump_rightgun 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_Gun_Jump_Fall
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Gun_Jump_Fall_Cycle
	NextMove	Ninja_Gun_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_rightgun 46 53
	TEnd
MEnd

Move Ninja_Gun_Jump_Fall_Cycle
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Post

	Type		male
	  Anim		male\ninja_jump_rightgun 54 54
	TEnd
MEnd

#############################################################

Move Ninja_Gun_Jump_Fall2
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Gun_Jump_Fall_Cycle2
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_jump_rightgun 66 93
	TEnd
MEnd

Move Ninja_Gun_Jump_Fall_Cycle2
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_jump_rightgun 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_Gun_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Gun_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_rightgun 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_Gun_Jump_Post2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_Gun_Ready
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_rightgun 130 150
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

Move Ninja_Gun_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON GUN
	Priority	7
	Interrupts	<TravelGunJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop2
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_rightgun 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx 2
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_Gun_HopPre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON GUN
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_rightgun 1 2
	TEnd
MEnd

Move Ninja_Gun_Hop
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop2
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_rightgun 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_Hop2
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_HopApex

	Type		male
	  Anim		male\ninja_hop_rightgun 82 82
	TEnd
MEnd

Move Ninja_Gun_HopApex
	Requires	NINJARUN APEX AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Gun_HopFall

	Type		male
	  Anim		male\ninja_hop_rightgun 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_HopFall
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_HopFall_2

	Type		male
	  Anim		male\ninja_hop_rightgun 85 124
	TEnd
MEnd

Move Ninja_Gun_HopFall_2
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_rightgun 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_Gun_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON GUN
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop_Right
	NextMove	Ninja_Gun_Hop

	Type		male
	  Anim		male\ninja_hop_right_rightgun 1 2
	TEnd
MEnd

Move Ninja_Gun_Hop_Right
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop2_Right
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_rightgun 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_Hop2_Right
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_HopApex_Right

	Type		male
	  Anim		male\ninja_hop_right_rightgun 82 82
	TEnd
MEnd

Move Ninja_Gun_HopApex_Right
	Requires	NINJARUN APEX AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Gun_HopFall_Right

	Type		male
	  Anim		male\ninja_hop_right_rightgun 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_HopFall_Right
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Gun_HopFall2_Right
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_right_rightgun 85 124
	TEnd
MEnd

Move Ninja_Gun_HopFall2_Right
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_right_rightgun 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_Gun_HopPre_Left
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON GUN
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop_Left
	NextMove	Ninja_Gun_Hop

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 1 2
	TEnd
MEnd

Move Ninja_Gun_Hop_Left
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_Gun_Hop2_Left
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_Hop2_Left
	Requires	NINJARUN JUMP AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_HopApex_Left

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 82 82
	TEnd
MEnd

Move Ninja_Gun_HopApex_Left
	Requires	NINJARUN APEX AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_Gun_HopFall_Left

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_Gun_HopFall_Left
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_Gun_HopFall2_Left
	NextMove	Ninja_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 85 124
	TEnd
MEnd

Move Ninja_Gun_HopFall2_Left
	Requires	NINJARUN AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_Gun_Jump_Post2

	Type		male
	  Anim		male\ninja_hop_Left_rightgun 124
	TEnd
MEnd