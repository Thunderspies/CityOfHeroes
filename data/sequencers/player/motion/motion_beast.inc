#############################################################
## motion_beast.inc
#############################################################

## READY ###########################################################

Move Beast_Ready_Pre
	Requires	BEASTRUN #COMBAT
	Priority	2
	Interrupts	<Ready> <Combat>
	Include sequencers\Members\BeastCombat.inc
	NextMove	Beast_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1
	TEnd
MEnd

Move Beast_Ready_Fast_One
	Requires	BEASTRUN #COMBAT
	Include sequencers\Members\BeastCombat.inc
	#Flags		Cycle
	NextMove	Beast_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle 10 50
	TEnd
MEnd

Move Beast_Ready_Fast_Two
	Requires	BEASTRUN #COMBAT
	Include sequencers\Members\BeastCombat.inc
	#Flags		Cycle
	NextMove	Beast_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle 10 50
	TEnd
MEnd

Move Beast_Ready_Fast_Three
	Requires	BEASTRUN #COMBAT
	Include sequencers\Members\BeastCombat.inc
	#Flags		Cycle
	NextMove	Beast_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle 10 50
	TEnd
MEnd

Move Beast_Ready_Slow
	Requires	BEASTRUN #COMBAT
	Include sequencers\Members\BeastCombat.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_Forward
	Requires	BEASTRUN FORWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ForwardPost
	CycleMove	Beast_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
	  # PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	  PlayFx	FEET\MaleLeft.fx 1
	  SmoothSprint	2.5 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Forward_Cycle
	Requires	BEASTRUN FORWARD #COMBAT
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ForwardPost

	Type		male
	  Anim		male\Beast_run_forward 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	2.5 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_ForwardPost
	Requires	BEASTRUN
	Include sequencers\Members\BeastCombat.inc
	NextMove	Beast_Ready_Fast_One
	Flags		HideWeapon HideShield
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	FEET\MaleLeft.fx 1
	  PlayFx	FEET\Land3.fx 15
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_Back
	Requires	BEASTRUN BACKWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_Back_Cycle
	NextMove	Beast_Back_Post

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
	  PlayFx	FEET\MaleLeft.fx 1
	  #PlayFx	POWERS\BEASTRUN\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Back_Cycle
	Requires	BEASTRUN BACKWARD #COMBAT
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	2.2 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_Back_Post
	Requires	BEASTRUN
	Include sequencers\Members\BeastCombat.inc
	NextMove	Beast_Ready_Fast_One
	Flags		HideWeapon HideShield
	Interpolate	8

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	FEET\MaleLeft.fx 1
	  PlayFx	FEET\Land3.fx 15
	 Scale		1.1
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_Left
	Requires	BEASTRUN STEPLEFT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	CycleMove	Beast_Left_Cycle
	NextMove	Beast_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
	  PlayFx	FEET\MaleLeft.fx 1
	  SmoothSprint	2.2 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Left_Cycle
	Requires	BEASTRUN STEPLEFT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15
	  SmoothSprint	2.2 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_Left_Post
	Requires	BEASTRUN
	Include sequencers\Members\BeastCombat.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Ready_Fast_One
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	FEET\MaleLeft.fx 1
	  PlayFx	FEET\Land3.fx 15
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_Right
	Requires	BEASTRUN STEPRIGHT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	CycleMove	Beast_Right_Cycle
	NextMove	Beast_Right_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
	  PlayFx	FEET\MaleLeft.fx 1
	  SmoothSprint	2.2 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_Right_Cycle
	Requires	BEASTRUN STEPRIGHT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	  PlayFx	FEET\BeastRun_Left.fx 1
	  PlayFx	FEET\BeastRun_Right.fx 4
	  PlayFx	FEET\MaleLeft.fx 11
	  PlayFx	FEET\MaleRight.fx 15

	  SmoothSprint	2.2 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_Right_Post
	Requires	BEASTRUN
	Include sequencers\Members\BeastCombat.inc
	Flags		HideWeapon HideShield
	NextMove	Beast_Ready_Fast_One
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	FEET\MaleLeft.fx 1
	  PlayFx	FEET\Land3.fx 15
	TEnd
MEnd

## JUMP UP ###########################################################

Move beast_run_jumpPre
	Requires	BEASTRUN JUMP AIR
	Priority	3
	Interrupts	<MoveIRQ> <IRQJumps> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		HideWeapon HideShield
	NextMove	beast_run_jumpUp

	Type		male
	  Anim		male\beast_run_jump 3 6
	  PlayFx	FEET\MaleLeft.fx 1
	TEnd

MEnd

Move beast_run_jumpUp
	Requires	BEASTRUN JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	beast_run_jumpApex

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd

MEnd

Move beast_run_jumpApex
	Requires	BEASTRUN APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	beast_run_jumpFall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd

MEnd

#######################################

Move beast_run_jumpFall
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	beast_run_jumpFall2

	Type		male
	  Anim		male\beast_run_jump 66 93
	TEnd

MEnd

Move beast_run_jumpFall2
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	beast_run_jumpPost

	Type		male
	  Anim		male\beast_run_jump 93
	TEnd

MEnd

## JUMP POST ###########################################################

Move beast_run_jumpPost
	Include sequencers\Members\BeastCombat.inc
	NextMove	Beast_Ready_Fast_One
	Flags		HideWeapon HideShield
	Scale		1.1
	SticksOnChild	LAND

	Type		male
	  Anim		male\beast_run_jumppost 1
	  PlayFx	FEET\Land3.fx 1
	  PlayFx	FEET\MaleLeft.fx 6
	TEnd

MEnd

## JUMP UP INTO FORWARD TRANSITION ###########################################################

Move beast_run_jumpToHop
	Requires	BEASTRUN FORWARD JUMP AIR
	Priority	4
	Interrupts	<TravelJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	beast_run_hop_cycle
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
	  PlayFx	FEET\MaleLeft.fx 1
	  PlayFx	FEET\MaleRight.fx 15
	TEnd

MEnd

## HOP FORWARD ###########################################################

Move beast_run_hopPre
	Requires	BEASTRUN FORWARD JUMP AIR
	Priority	4
	Interrupts	<IRQJumps> <PostMoveCycle> <Ready> <BasicMove> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	beast_run_hop

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd

MEnd

Move beast_run_hop
	Requires	BEASTRUN JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	beast_run_hop_cycle
	Scale		0.75

	Type		male
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\JUMP1.FX:JUMP
	  PlayFx	FEET\Land3.fx 15
	TEnd

	Type		fem
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	  PlayFx	FEET\Land3.fx 15
	TEnd

	Type		huge
	  Anim		male\beast_run_hop 11 91
#	  PlayFx	FEET\HUGEJUMP1.FX:JUMP
	  PlayFx	FEET\Land3.fx 15
	TEnd
MEnd

Move beast_run_hop_cycle
	Requires	BEASTRUN JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	beast_run_hopApex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd

MEnd

Move beast_run_hopApex
	Requires	BEASTRUN APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	beast_run_hopFall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd

MEnd

Move beast_run_hopFall
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	beast_run_hopFall_Cycle

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd

MEnd

Move beast_run_hopFall_Cycle
	Requires	BEASTRUN AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	beast_run_jumpPost

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd

MEnd

#############################################################

Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Righthand.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Righthand_Gun.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Dual.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Dual_Pistols.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Dual_Claws.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Dual_Lefthand.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Shield.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_ShieldWeapon.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_Two_Handed_Weapon.inc
Include sequencers\player\Motion\Beast\motion_Beast_CombatWeapon_BeamRifle.inc