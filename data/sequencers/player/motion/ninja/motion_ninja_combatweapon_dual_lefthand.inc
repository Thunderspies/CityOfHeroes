#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## READY ###########################################################

Move Ninja_LefthandWep_Pre
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Priority	5
	Interrupts	<LefthandCombat> <Ready>
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Ready
	NextMove	Katana_Ready
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_leftweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_LefthandWep_Ready
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	NextMove	Bow_ready
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_leftweapon 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_LefthandWep_Forward
	Requires	NINJARUN FORWARD COMBAT WEAPON DUAL REV
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <TravelDualHandMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Forward_Post
	CycleMove	Ninja_LefthandWep_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_leftweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_LefthandWep_Forward_Cycle
	Requires	NINJARUN FORWARD WEAPON
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_leftweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_LefthandWep_Forward_Post
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_leftweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_LefthandWep_Back
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL REV
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <TravelDualHandMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_LefthandWep_Back_Cycle
	NextMove	Ninja_LefthandWep_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_leftweapon 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_LefthandWep_Back_Cycle
	Requires	NINJARUN BACKWARD COMBAT WEAPON DUAL REV
 	Priority	7 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_leftweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_LefthandWep_Back_Post
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_leftweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_LefthandWep_Left
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL REV
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <TravelDualHandMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Left_Post
	CycleMove	Ninja_LefthandWep_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_leftweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_LefthandWep_Left_Cycle
	Requires	NINJARUN STEPLEFT COMBAT WEAPON DUAL REV
	Priority	7 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Left_Post

	Type		male
	  Anim		male\ninja_run_left_leftweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_LefthandWep_Left_Post
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_leftweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_LefthandWep_Right
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL REV
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <TravelDualHandMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Right_Post
	CycleMove	Ninja_LefthandWep_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_leftweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_LefthandWep_Right_Cycle
	Requires	NINJARUN STEPRIGHT COMBAT WEAPON DUAL REV
	Priority	7 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelLeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_LefthandWep_Right_Post

	Type		male
	  Anim		male\ninja_run_right_leftweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_LefthandWep_Right_Post
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_leftweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_LefthandWep_Jump_Pre
	Requires	NINJARUN JUMP AIR COMBAT WEAPON DUAL REV #ARCHERY
	Priority	8
	Interrupts	<MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelLeftWepJump>
	NextMove	Ninja_LefthandWep_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump_leftweapon 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Jump
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelLeftWepJump>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Apex ##Apex is for short jumps
	CycleMove	Ninja_LefthandWep_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_leftweapon 6 10
	TEnd
MEnd

Move Ninja_LefthandWep_Jump_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelLeftWepJump>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_leftweapon 6 40
	TEnd
MEnd

#############################################################

Move Ninja_LefthandWep_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_LefthandWep_Fall
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_leftweapon 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_LefthandWep_Fall2

	Type		male
	  Anim		male\ninja_jump_leftweapon 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_LefthandWep_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Fall_Cycle
	NextMove	Ninja_LefthandWep_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_leftweapon 46 53
	TEnd
MEnd

Move Ninja_LefthandWep_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Jump_Post

	Type		male
	  Anim		male\ninja_jump_leftweapon 54 54
	TEnd
MEnd

#############################################################

Move Ninja_LefthandWep_Fall2
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Fall2_Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_leftweapon 66 93
	TEnd
MEnd

Move Ninja_LefthandWep_Fall2_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_leftweapon 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_LefthandWep_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_LefthandWep_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_leftweapon 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_LefthandWep_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_LefthandWep_Ready
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\ninja_jump_leftweapon 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd
MEnd

Move Ninja_LefthandWep_Jump_PostCycle
	Requires	NINJARUN COMBAT WEAPON DUAL REV
	Include sequencers\Members\NinjaCombat_Weapon_LeftHand.inc
	Flags		Cycle
	NextMove	postReady_back
	Interpolate	15

	Type		male
	  Anim		male\ninja_cycle_leftweapon 17 47
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_LefthandWep_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL REV #ARCHERY
	Priority	9
	Interrupts	<TravelLeftWepJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_leftweapon 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_LefthandWep_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR COMBAT WEAPON DUAL REV #ARCHERY
	Priority	9
	Interrupts	<MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_leftweapon 1 2
	TEnd
MEnd

Move Ninja_LefthandWep_Hop
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_leftweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_leftweapon 82 82
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_LefthandWep_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_leftweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_leftweapon 85 124
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_leftweapon 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_LefthandWep_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR COMBAT WEAPON DUAL REV #ARCHERY
	Priority	9
	Interrupts	<MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 1 2
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Right
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 82 82
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Right_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_LefthandWep_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Right_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Hop_Right_Fall_Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 85 124
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_leftweapon 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_LefthandWep_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR COMBAT WEAPON DUAL REV #ARCHERY
	Priority	9
	Interrupts	<MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 1 2
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Left
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_LefthandWep_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 82 82
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Left_Apex
	Requires	NINJARUN APEX AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_LefthandWep_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Left_Fall
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_LefthandWep_Hop_Left_Fall_Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 85 124
	TEnd
MEnd

Move Ninja_LefthandWep_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR #COMBAT WEAPON DUAL REV #ARCHERY
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_LefthandWep_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_leftweapon 124
	TEnd
MEnd