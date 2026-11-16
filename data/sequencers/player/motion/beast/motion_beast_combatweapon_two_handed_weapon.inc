#############################################################
## motion_Beast_CombatWeapon_Two_Handed_Weapon.inc
#############################################################

## READY ###########################################################

Move Beast_2hLargeReady_Pre
	Requires	BEASTRUN COMBAT WEAPON 2HLARGE
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <TravelCombat>
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Beast_2hLargeReady_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\beast_2hlarge_ready 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1
	TEnd

	Type		fem
	  Anim		fem\beast_2hlarge_ready 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1
	TEnd
MEnd

Move Beast_2hLargeReady_Ready_Fast_One
	Requires	BEASTRUN COMBAT WEAPON 2HLARGE
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Beast_2hLargeReady_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_2hlarge_ready 10 50
	TEnd

	Type		fem
	  Anim		fem\beast_2hlarge_ready 10 50
	TEnd
MEnd

Move Beast_2hLargeReady_Ready_Fast_Two
	Requires	BEASTRUN COMBAT WEAPON 2HLARGE
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Beast_2hLargeReady_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_2hlarge_ready 10 50
	TEnd

	Type		fem
	  Anim		fem\beast_2hlarge_ready 10 50
	TEnd
MEnd

Move Beast_2hLargeReady_Ready_Fast_Three
	Requires	BEASTRUN COMBAT WEAPON 2HLARGE
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	CycleMove	Beast_2hLargeReady_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_2hlarge_ready 10 50
	TEnd

	Type		fem
	  Anim		fem\beast_2hlarge_ready 10 50
	TEnd
MEnd

Move Beast_2hLargeReady_Ready_Slow
	Requires	BEASTRUN COMBAT WEAPON 2HLARGE
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	NextMove	2hlargeReady
	Scale		0.5

	Type		male
	  Anim		male\beast_2hlarge_ready 10 50
	TEnd

	Type		fem
	  Anim		fem\beast_2hlarge_ready 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_2HLARGE_Forward
	Requires	BeastRUN FORWARD COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <TwoHandCombat> <WepMove> <TwoHandMove>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_2HLARGE_ForwardPost
	CycleMove	Beast_2HLARGE_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
##	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
#	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_2HLARGE_Forward_Cycle
	Requires	BeastRUN FORWARD COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_2HLARGE_ForwardPost

	Type		male
	  Anim		male\Beast_run_forward 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_2HLARGE_ForwardPost
	Include sequencers\Members\BeastCombat_Weapon.inc
	NextMove	Beast_2hLargeReady_Ready_Fast_One
	Flags		HideWeapon HideShield
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_2HLARGE_Back
	Requires	BeastRUN BACKWARD COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <TwoHandMove>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide>  <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_2HLARGE_Back_Cycle
	NextMove	Beast_2HLARGE_ForwardPost

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_2HLARGE_Back_Cycle
	Requires	BeastRUN BACKWARD COMBAT WEAPON 2HLARGE
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_2HLARGE_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_2HLARGE_Back_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon.inc
	NextMove	Beast_2hLargeReady_Ready_Fast_One
	Flags		HideWeapon HideShield
	Interpolate	8

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_2HLARGE_Left
	Requires	BeastRUN STEPLEFT COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ> <TwoHandMove>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	CycleMove	Beast_2HLARGE_Left_Cycle
	NextMove	Beast_2HLARGE_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_2HLARGE_Left_Cycle
	Requires	BeastRUN STEPLEFT COMBAT WEAPON 2HLARGE
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_2HLARGE_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_2HLARGE_Left_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_2hLargeReady_Ready_Fast_One
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_2HLARGE_Right
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON 2HLARGE
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ> <TwoHandMove>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_2HLARGE_ForwardPost
	CycleMove	Beast_2HLARGE_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_2HLARGE_Right_Cycle
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON 2HLARGE
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <Travel2HLARGEMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_2HLARGE_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_2HLARGE_Right_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon.inc
	NextMove	Beast_2hLargeReady_Ready_Fast_One
	Flags		HideWeapon HideShield
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_2HLARGE_Jump_Pre
	Requires	BeastRUN JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Beast_2HLARGE_Jump
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 3 6
# 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd

Move Beast_2HLARGE_Jump
	Requires	BeastRUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_2HLARGE_Jump_Cycle

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd
MEnd

Move Beast_2HLARGE_Jump_Cycle
	Requires	BeastRUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		HideWeapon HideShield
	NextMove	Beast_2HLARGE_Jump_Fall

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd
MEnd

#############################################################

Move Beast_2HLARGE_Jump_Fall
	Requires	BeastRUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		HideWeapon HideShield ReqInputs
	CycleMove	Beast_2HLARGE_Jump_Fall_Cycle


	Type		male
	  Anim		male\beast_run_jump 66 93
	TEnd
MEnd

Move Beast_2HLARGE_Jump_Fall_Cycle
	Requires	BeastRUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_2HLARGE_Jump_Post

	Type		male
	  Anim		male\beast_run_jump 93
	TEnd
MEnd

## JUMP POST ###########################################################

Move Beast_2HLARGE_Jump_Post
	Include sequencers\Members\BeastCombat.inc
	NextMove	Beast_2hLargeReady_Ready_Fast_One
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

Move Beast_2HLARGE_JumpToHop
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	<TravelJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_2HLARGE_Hop2
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## Beast JUMP FORWARD ###########################################################

Move Beast_2HLARGE_HopPre
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON 2HLARGE
	Priority	7
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_2HLARGE_Hop
	Interpolate	5

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_2HLARGE_Hop
	Requires	BeastRUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_2HLARGE_Hop2
	Scale		0.75

	Type		male
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\FEMJUMP1.FX:JUMP
#	  PlayFx	FEET\Land3.fx 15
	TEnd

	Type		huge
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\HUGEJUMP1.FX:JUMP
#	  PlayFx	FEET\Land3.fx 15
	TEnd

MEnd

Move Beast_2HLARGE_Hop2
	Requires	BeastRUN JUMP AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_2HLARGE_HopApex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_2HLARGE_HopApex
	Requires	BeastRUN APEX AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Beast_2HLARGE_HopFall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_2HLARGE_HopFall
	Requires	BeastRUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_2HLARGE_HopFall_2

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_2HLARGE_HopFall_2
	Requires	BeastRUN AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_2HLARGE_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd