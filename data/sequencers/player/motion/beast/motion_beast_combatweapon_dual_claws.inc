#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## Claws READY ###########################################################

Move Beast_Claws_Pre
	Requires	BEASTRUN COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <DualHandCombat>
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Beast_Claws_Ready_Fast_One
	NextMove	ClawReady
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle_dual_pistol 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Beast_Claws_Ready_Fast_One
	Requires	BEASTRUN COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Cycle
	NextMove	Beast_Claws_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_pistol 10 50
	TEnd
MEnd

Move Beast_Claws_Ready_Fast_Two
	Requires	BEASTRUN COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Cycle
	NextMove	Beast_Claws_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_pistol 10 50
	TEnd
MEnd

Move Beast_Claws_Ready_Fast_Three
	Requires	BEASTRUN COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Cycle
	NextMove	Beast_Claws_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_pistol 10 50
	TEnd
MEnd

Move Beast_Claws_Ready_Slow
	Requires	BEASTRUN COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		Cycle
	NextMove	ClawReady
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle_dual_pistol 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_Claws_Forward
	Requires	BeastRUN FORWARD COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Forward_Post
	CycleMove	Beast_Claws_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Claws_Forward_Cycle
	Requires	BeastRUN FORWARD #WEAPON CLAW
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Forward_Post

	Type		male
	  Anim		male\Beast_run_forward 1 22
	 # PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_Claws_Forward_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Claws_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_Claws_Back
	Requires	BeastRUN BACKWARD COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_Claws_Back_Cycle
	NextMove	Beast_Claws_Back_Post

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Claws_Back_Cycle
	Requires	BeastRUN BACKWARD COMBAT WEAPON DUAL CLAW
 	Priority	6 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_Claws_Back_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Claws_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_Claws_Left
	Requires	BeastRUN STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Beast_Claws_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Left_Post
	CycleMove	Beast_Claws_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
#	  PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Claws_Left_Cycle
	Requires	BeastRUN STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	7 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_Claws_Left_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Claws_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_Claws_Right
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Beast_Claws_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Right_Post
	CycleMove	Beast_Claws_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Claws_Right_Cycle
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	7 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Claws_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_Claws_Right_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Claws_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_Claws_Jump_Pre
	Requires	BeastRUN JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	NextMove	Beast_Claws_Jump
	Interpolate	5
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 3 6
 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd

Move Beast_Claws_Jump
	Requires	BeastRUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_Claws_Jump_Cycle ##Apex is for short jumps

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd
MEnd

Move Beast_Claws_Jump_Cycle
	Requires	BeastRUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		HideWeapon HideShield
	NextMove	Beast_Claws_Apex ##Apex is for higher jumps

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd
MEnd

#############################################################

Move Beast_Claws_Apex ##Apex is for short jumps
	Requires	BeastRUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Beast_Claws_Apex2
	Flags		ReqInputs HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 66 93
	#  PlayFX	POWERS\BeastRun\Jump_Beast.fx
	TEnd
MEnd

Move Beast_Claws_Apex2 ##Apex is for higher jumps
	Requires	BeastRUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Beast_Claws_Jump_Post
	Flags		Cycle HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 93
	#  PlayFX	POWERS\BeastRun\Jump_Beast.fx
	TEnd
MEnd


## JUMP POST ###########################################################

Move Beast_Claws_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Beast_Claws_Ready_Fast_One
	Flags		HideWeapon HideShield
	Scale		1.1
	SticksOnChild	LAND

	Type		male
	  Anim		male\beast_run_jumppost 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

MEnd

## Beast JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Beast_Claws_JumpToHop
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<TravelClawsJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_Claws_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## Beast JUMP FORWARD ###########################################################

Move Beast_Claws_Hop_Pre
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON DUAL CLAW
	Priority	8
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_Claws_Hop
	Interpolate	5

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_Claws_Hop
	Requires	BeastRUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_Claws_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\beast_run_hop 11 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\beast_run_hop 11 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	  PlayFx	FEET\Land3.fx 15
	TEnd

	Type		huge
	  Anim		male\beast_run_hop 11 91
	  PlayFx	FEET\HUGEJUMP1.FX:JUMP
	  PlayFx	FEET\Land3.fx 15
	TEnd

MEnd

Move Beast_Claws_Hop_Cycle
	Requires	BeastRUN JUMP AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_Claws_Hop_Apex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_Claws_Hop_Apex
	Requires	BeastRUN APEX AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Beast_Claws_Hop_Fall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_Claws_Hop_Fall
	Requires	BeastRUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_Claws_Hop_Fall_Cycle

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_Claws_Hop_Fall_Cycle
	Requires	BeastRUN AIR #COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_Claws_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd