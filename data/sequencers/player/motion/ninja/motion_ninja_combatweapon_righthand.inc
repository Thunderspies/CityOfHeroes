#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

#Move Ready_To_NinjaWepReady
#	Requires	NINJARUN COMBAT WEAPON
#	Priority	3
#	Interrupts	<NinjaCombat>
#	Include sequencers\Members\NinjaCombat_Weapon.inc
#	NextMove        Ninja_RightWepCombat_Pre
#	Flags		NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\draw_axe 1 30
#	TEnd
#MEnd

## READY ###########################################################

Move Ninja_RightWepCombat_Pre
	Requires	NINJARUN COMBAT WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat>
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Ready
	NextMove	PreWep
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_rightweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_RightWepCombat_Ready
	Requires	NINJARUN COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	PreWep
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_rightweapon 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_RightWepCombat_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Forward_Post
	CycleMove	Ninja_RightWepCombat_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_rightweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_RightWepCombat_Forward_Cycle
	Requires	NINJARUN FORWARD COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_rightweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_RightWepCombat_Forward_Post
	Requires	NINJARUN COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_rightweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_RightWepCombat_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_RightWepCombat_Back_Cycle
	NextMove	Ninja_RightWepCombat_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_rightweapon 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_RightWepCombat_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON
	Priority	4
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_rightweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_RightWepCombat_Back_Post
	Requires	NINJARUN COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Back_PostCycle
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_rightweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

Move Ninja_RightWepCombat_Back_PostCycle
	Requires	NINJARUN COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	10

	Type		male
	  Anim		male\ninja_cycle_rightweapon 17 47
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_RightWepCombat_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Left_Post
	CycleMove	Ninja_RightWepCombat_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_rightweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_RightWepCombat_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON
	Priority	4
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Left_Post

	Type		male
	  Anim		male\ninja_run_left_rightweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_RightWepCombat_Left_Post
	Requires	NINJARUN COMBAT #WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Left_PostCycle
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_rightweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

Move Ninja_RightWepCombat_Left_PostCycle
	Requires	NINJARUN COMBAT #WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	15

	Type		male
	  Anim		male\ninja_cycle_rightweapon 17 47
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_RightWepCombat_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Right_Post
	CycleMove	Ninja_RightWepCombat_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_rightweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_RightWepCombat_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON
	Priority	4
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_RightWepCombat_Right_Post

	Type		male
	  Anim		male\ninja_run_right_rightweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_RightWepCombat_Right_Post
	Requires	NINJARUN COMBAT #WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Right_PostCycle
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_rightweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

Move Ninja_RightWepCombat_Right_PostCycle
	Requires	NINJARUN COMBAT #WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	15

	Type		male
	  Anim		male\ninja_cycle_rightweapon 17 47
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_RightWepCombat_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Priority	5
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	NextMove	Ninja_RightWepCombat_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump_rightweapon 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Jump
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Apex ##Apex is for short jumps
	CycleMove	Ninja_RightWepCombat_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_rightweapon 6 10
	TEnd
MEnd

Move Ninja_RightWepCombat_Jump_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_rightweapon 6 40
	TEnd
MEnd

#############################################################

Move Ninja_RightWepCombat_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_RightWepCombat_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_rightweapon 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_RightWepCombat_Fall2

	Type		male
	  Anim		male\ninja_jump_rightweapon 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_RightWepCombat_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Fall_Cycle
	NextMove	Ninja_RightWepCombat_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_rightweapon 46 53
	TEnd
MEnd

Move Ninja_RightWepCombat_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Jump_Post

	Type		male
	  Anim		male\ninja_jump_rightweapon 54 54
	TEnd
MEnd

#############################################################

Move Ninja_RightWepCombat_Fall2
	Requires	NINJARUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Fall2_Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_rightweapon 66 93
	TEnd
MEnd

Move Ninja_RightWepCombat_Fall2_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_rightweapon 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_RightWepCombat_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_RightWepCombat_Jump_PostCycle
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_rightweapon 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_RightWepCombat_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_RightWepCombat_Jump_PostCycle
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\ninja_jump_rightweapon 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_RightWepCombat_Jump_PostCycle
	Requires	NINJARUN COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	15

	Type		male
	  Anim		male\ninja_cycle_rightweapon 17 47
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_RightWepCombat_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<TravelRightWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_rightweapon 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_RightWepCombat_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_rightweapon 1 2
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_rightweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Cycle
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_rightweapon 82 82
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Apex
	Requires	NINJARUN APEX AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_RightWepCombat_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_rightweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Fall
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_rightweapon 85 124
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Fall_Cycle
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_rightweapon 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_RightWepCombat_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 1 2
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Right
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 82 82
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Right_Apex
	Requires	NINJARUN APEX AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_RightWepCombat_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Right_Fall
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Hop_Right_Fall_Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 85 124
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_rightweapon 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_RightWepCombat_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 1 2
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Left
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_RightWepCombat_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 82 82
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Left_Apex
	Requires	NINJARUN APEX AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_RightWepCombat_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Left_Fall
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_RightWepCombat_Hop_Left_Fall_Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 85 124
	TEnd
MEnd

Move Ninja_RightWepCombat_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_RightWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_rightweapon 124
	TEnd
MEnd