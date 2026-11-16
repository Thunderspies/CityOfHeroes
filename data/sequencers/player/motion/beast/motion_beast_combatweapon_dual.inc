#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## Dual Weild READY ###########################################################

Move Beast_DualWield_Pre
	Requires	BEASTRUN COMBAT WEAPON DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat>
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		Cycle
	CycleMove	Beast_DualWield_Ready_Fast_One
	NextMove	DualWield_Ready
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle_dual_sword 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Beast_DualWield_Ready_Fast_One
	Requires	BEASTRUN COMBAT WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Hideweapon
	NextMove	Beast_DualWield_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_sword 10 50
	TEnd
MEnd

Move Beast_DualWield_Ready_Fast_Two
	Requires	BEASTRUN COMBAT WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Hideweapon
	NextMove	Beast_DualWield_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_sword 10 50
	TEnd
MEnd

Move Beast_DualWield_Ready_Fast_Three
	Requires	BEASTRUN COMBAT WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	#Flags		Hideweapon
	NextMove	Beast_DualWield_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_dual_sword 10 50
	TEnd
MEnd

Move Beast_DualWield_Ready_Slow
	Requires	BEASTRUN COMBAT WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle_dual_sword 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_DualWield_Forward
	Requires	BeastRUN FORWARD COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Forward_Post
	CycleMove	Beast_DualWield_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_DualWield_Forward_Cycle
	Requires	BeastRUN FORWARD #WEAPON CLAW
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Forward_Post

	Type		male
	  Anim		male\Beast_run_forward 1 22
	 # PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_DualWield_Forward_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_DualWield_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_DualWield_Back
	Requires	BEASTRUN BACKWARD COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_DualWield_Back_Cycle
	NextMove	Beast_DualWield_Back_Post

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_DualWield_Back_Cycle
	Requires	BEASTRUN BACKWARD COMBAT WEAPON DUAL
 	Priority	6 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_DualWield_Back_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_DualWield_Ready_Fast_One
	Interpolate	8

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_DualWield_Left
	Requires	BeastRUN STEPLEFT COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Beast_DualWield_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Left_Post
	CycleMove	Beast_DualWield_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_DualWield_Left_Cycle
	Requires	BeastRUN STEPLEFT COMBAT WEAPON DUAL
	Priority	6 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	  #PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_DualWield_Left_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL CLAW
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_DualWield_Ready_Fast_One
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_DualWield_Right
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON DUAL
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelMove> <TravelWepMove> <MoveIRQ> Beast_DualWield_Forward
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Right_Post
	CycleMove	Beast_DualWield_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_DualWield_Right_Cycle
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON DUAL
	Priority	6 ##+1 to take priority over pre-move
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelDualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_DualWield_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	  ##PlayFx	POWERS\BeastRun\MALELEFT
	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_DualWield_Right_Post
	Requires	BeastRUN COMBAT #WEAPON DUAL
	Include sequencers\Members\BeastCombat_DualWeapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_DualWield_Ready_Fast_One
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_DualWield_Jump_Pre
	Requires	BeastRUN JUMP AIR COMBAT WEAPON DUAL
	Priority	6
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	Flags		HideWeapon HideShield
	NextMove	Beast_DualWield_JumpUp

	Type		male
	  Anim		male\beast_run_jump 3 6
 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

MEnd

Move Beast_DualWield_JumpUp
	Requires	BeastRUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_DualWield_jumpApex

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd

MEnd


Move Beast_DualWield_jumpApex
	Requires	BEASTRUN APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	beast_DualWield_jumpFall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd

MEnd

#############################################################

Move Beast_DualWield_jumpFall
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_DualWield_jumpFall_Cycle

	Type		male
	  Anim		male\beast_run_jump 66 93
	TEnd

MEnd

Move Beast_DualWield_jumpFall_Cycle
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_DualWield_Jump_Post

	Type		male
	  Anim		male\beast_run_jump 93
	TEnd

MEnd

## JUMP POST ###########################################################

Move Beast_DualWield_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Beast_DualWield_Ready_Fast_One
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
Move Beast_DualWield_JumpToHop
	Requires	BEASTRUN FORWARD JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<TravelDualWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_DualWield_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
	TEnd

MEnd

## Beast JUMP FORWARD ###########################################################

Move Beast_DualWield_Hop_Pre
	Requires	BEASTRUN FORWARD JUMP AIR COMBAT WEAPON DUAL
	Priority	7
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_DualWield_Hop
	Interpolate	5

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_DualWield_Hop
	Requires	BEASTRUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_DualWield_Hop_Cycle
	Scale		0.75

	Type		male
	  Anim		male\beast_run_hop 11 91
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

Move Beast_DualWield_Hop_Cycle
	Requires	BEASTRUN JUMP AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_DualWield_Hop_Apex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_DualWield_Hop_Apex
	Requires	BEASTRUN APEX AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Beast_DualWield_Hop_Fall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_DualWield_Hop_Fall
	Requires	BEASTRUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_DualWield_Hop_Fall_Cycle

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_DualWield_Hop_Fall_Cycle
	Requires	BEASTRUN AIR #COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_DualWield_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd