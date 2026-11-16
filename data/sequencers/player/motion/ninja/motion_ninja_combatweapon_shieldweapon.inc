#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## READY ###########################################################

Move Ninja_ShieldWepCombat_Pre
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Priority	24
	Interrupts	<Ready> <ShieldCombat> <TravelWepCombat> <TravelShieldCombat>
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_shieldweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Ready
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.85

	Type		male
	  Anim		male\ninja_cycle_shieldweapon 17 47
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Ninja_ShieldWepCombat_Forward
	Requires	NINJARUN FORWARD SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Forward_Post
	CycleMove	Ninja_ShieldWepCombat_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_forward_shieldweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Forward_Cycle
	Requires	NINJARUN FORWARD WEAPON
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Forward_Post

	Type		male
	  Anim		male\ninja_run_forward_shieldweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Ninja_ShieldWepCombat_Forward_Post
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_cycle_shieldweapon 1 16
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Ninja_ShieldWepCombat_Back
	Requires	NINJARUN BACKWARD SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Ninja_ShieldWepCombat_Back_Cycle
	NextMove	Ninja_ShieldWepCombat_Back_Post

	Type		male
	  Anim		male\ninja_run_backward_pre_shieldweapon 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Back_Cycle
	Requires	NINJARUN BACKWARD SHIELD COMBAT WEAPON
 	Priority	26 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_shieldweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Ninja_ShieldWepCombat_Back_Post
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_backward_post_shieldweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Ninja_ShieldWepCombat_Left
	Requires	NINJARUN STEPLEFT SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Left_Post
	CycleMove	Ninja_ShieldWepCombat_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_shieldweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT  5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Left_Cycle
	Requires	NINJARUN STEPLEFT SHIELD COMBAT WEAPON
	Priority	26 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Left_Post

	Type		male
	  Anim		male\ninja_run_left_shieldweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Ninja_ShieldWepCombat_Left_Post
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_left_post_shieldweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Ninja_ShieldWepCombat_Right
	Requires	NINJARUN STEPRIGHT SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Right_Post
	CycleMove	Ninja_ShieldWepCombat_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_shieldweapon 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Right_Cycle
	Requires	NINJARUN STEPRIGHT SHIELD COMBAT WEAPON
	Priority	26 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Ninja_ShieldWepCombat_Right_Post

	Type		male
	  Anim		male\ninja_run_right_shieldweapon 1 17
	  PlayFx	POWERS\NinjaRun\MALELEFT_NINJA.FX:FOOTFALLLEFT 5
	  PlayFx	POWERS\NinjaRun\MALERIGHT_NINJA.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Ninja_ShieldWepCombat_Right_Post
	Requires	NINJARUN SHIELD COMBAT WEAPON
	Include sequencers\Members\NinjaCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Ready
	NextMove	Ninja_Ready #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\ninja_run_right_post_shieldweapon 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Ninja_ShieldWepCombat_Jump_Pre
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Priority	26
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	NextMove	Ninja_ShieldWepCombat_Jump
	Interpolate	5

	Type		male
	  Anim		male\ninja_jump_shieldweapon 3 6
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Jump
	Requires	NINJARUN JUMP AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Apex ##Apex is for short jumps
	CycleMove	Ninja_ShieldWepCombat_Jump_Cycle

	Type		male
	  Anim		male\ninja_jump_shieldweapon 6 10
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Jump_Cycle
	Requires	NINJARUN JUMP AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Apex2 ##Apex is for higher jumps

	Type		male
	  Anim		male\ninja_jump_shieldweapon 6 40
	TEnd
MEnd

#############################################################

Move Ninja_ShieldWepCombat_Apex ##Apex is for short jumps
	Requires	NINJARUN APEX AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <ShieldFall>
	NextMove	Ninja_ShieldWepCombat_Fall
	Interpolate	5
	Scale		1.2

	Type		male
	  Anim		male\ninja_jump_shieldweapon 40 46
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Apex2 ##Apex is for higher jumps
	Requires	NINJARUN APEX AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldFall> <Jumps>
	NextMove	Ninja_ShieldWepCombat_Fall2

	Type		male
	  Anim		male\ninja_jump_shieldweapon 65 66
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

#############################################################

Move Ninja_ShieldWepCombat_Fall
	Requires	NINJARUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Fall_Cycle
	NextMove	Ninja_ShieldWepCombat_Jump_Post
	Interpolate	2

	Type		male
	  Anim		male\ninja_jump_shieldweapon 46 53
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Fall_Cycle
	Requires	NINJARUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Jump_Post

	Type		male
	  Anim		male\ninja_jump_shieldweapon 54 54
	TEnd
MEnd

#############################################################

Move Ninja_ShieldWepCombat_Fall2
	Requires	NINJARUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	CycleMove	Ninja_ShieldWepCombat_Fall2_Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_shieldweapon 66 93
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Fall2_Cycle
	Requires	NINJARUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_jump_shieldweapon 93 130
	TEnd
MEnd

## JUMP POST ###########################################################

Move Ninja_ShieldWepCombat_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_ShieldWepCombat_Ready
	Scale		0.7
	SticksOnChild	LAND
	Interpolate	1

	Type		male
	  Anim		male\ninja_jump_shieldweapon 54 64
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Jump2_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Ninja_ShieldWepCombat_Ready
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\ninja_jump_shieldweapon 130 150
	  PlayFx	POWERS\NinjaRun\LAND1_NINJA.FX:LAND
	  PlayFx	fx\Feet\MaleLand_Ninja.fx:LAND 1
	TEnd
MEnd

## NINJA JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Ninja_ShieldWepCombat_JumpToHop
	Requires	NINJARUN FORWARD JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<TravelShieldWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\ninja_hop_shieldweapon 2 82
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## NINJA JUMP FORWARD ###########################################################

Move Ninja_ShieldWepCombat_Hop_Pre
	Requires	NINJARUN FORWARD JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_shieldweapon 1 2
	  PlayFx	FEET\LAND1.FX
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_shieldweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Cycle
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Hop_Apex

	Type		male
	  Anim		male\ninja_hop_shieldweapon 82 82
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Apex
	Requires	NINJARUN APEX AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldWepCombat_Hop_Fall

	Type		male
	  Anim		male\ninja_hop_shieldweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Fall
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Fall_Cycle

	Type		male
	  Anim		male\ninja_hop_shieldweapon 85 124
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Fall_Cycle
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_shieldweapon 124
	TEnd
MEnd

## NINJA JUMP RIGHT ###########################################################

Move Ninja_ShieldWepCombat_Hop_Right_Pre
	Requires	NINJARUN STEPRIGHT JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Right
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 1 2
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Right
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Right_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Right_Cycle
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Hop_Right_Apex

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 82 82
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Right_Apex
	Requires	NINJARUN APEX AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldWepCombat_Hop_Right_Fall

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Right_Fall
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Hop_Right_Fall_Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 85 124
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Right_Fall_Cycle
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_right_shieldweapon 124
	TEnd
MEnd

## NINJA JUMP LEFT ###########################################################

Move Ninja_ShieldWepCombat_Hop_Left_Pre
	Requires	NINJARUN STEPLEFT JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Left
	Interpolate	5

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 1 2
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Left
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Ninja_ShieldWepCombat_Hop_Left_Cycle
	Scale		0.75

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 2 82
	  PlayFX	POWERS\NinjaRun\Hop_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Left_Cycle
	Requires	NINJARUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Hop_Left_Apex

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 82 82
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Left_Apex
	Requires	NINJARUN APEX AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Ninja_ShieldWepCombat_Hop_Left_Fall

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 82 85
	  PlayFX	POWERS\NinjaRun\Jump_Ninja.fx
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Left_Fall
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Hop_Left_Fall_Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 85 124
	TEnd
MEnd

Move Ninja_ShieldWepCombat_Hop_Left_Fall_Cycle
	Requires	NINJARUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Ninja_ShieldWepCombat_Jump2_Post

	Type		male
	  Anim		male\ninja_hop_left_shieldweapon 124
	TEnd
MEnd