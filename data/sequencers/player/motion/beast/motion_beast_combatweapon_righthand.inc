#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## Weapon Right Hand READY ###########################################################

Move Beast_RightWepCombat_Pre
	Requires	BEASTRUN COMBAT WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat>
	Include sequencers\Members\BeastCombat_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_RightWepCombat_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle_right_sword 1 10
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Beast_RightWepCombat_Ready_Fast_One
	Requires	BEASTRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_RightWepCombat_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_sword 10 50
	TEnd
MEnd

Move Beast_RightWepCombat_Ready_Fast_Two
	Requires	BEASTRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_RightWepCombat_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_sword 10 50
	TEnd
MEnd


Move Beast_RightWepCombat_Ready_Fast_Three
	Requires	BEASTRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_RightWepCombat_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_sword 10 50
	TEnd
MEnd


Move Beast_RightWepCombat_Ready_Slow
	Requires	BEASTRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle
	NextMove	PreWep
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle_right_sword 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_RightWepCombat_Forward
	Requires	BeastRUN FORWARD COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Forward_Post
	CycleMove	Beast_RightWepCombat_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
##	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
#	  PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_RightWepCombat_Forward_Cycle
	Requires	BeastRUN FORWARD COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Forward_Post

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

Move Beast_RightWepCombat_Forward_Post
	Requires	BeastRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_RightWepCombat_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_RightWepCombat_Back
	Requires	BeastRUN BACKWARD COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_RightWepCombat_Back_Cycle
	NextMove	Beast_RightWepCombat_Back_Post

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_RightWepCombat_Back_Cycle
	Requires	BeastRUN BACKWARD COMBAT WEAPON
	Priority	4
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Back_Post
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

Move Beast_RightWepCombat_Back_Post
	Requires	BeastRUN COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_RightWepCombat_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_RightWepCombat_Left
	Requires	BeastRUN STEPLEFT COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Left_Post
	CycleMove	Beast_RightWepCombat_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
#	  PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT	 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_RightWepCombat_Left_Cycle
	Requires	BeastRUN STEPLEFT COMBAT WEAPON
	Priority	4
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Left_Post

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

Move Beast_RightWepCombat_Left_Post
	Requires	BeastRUN COMBAT #WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_RightWepCombat_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd


## RUN RIGHT ###########################################################

Move Beast_RightWepCombat_Right
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Right_Post
	CycleMove	Beast_RightWepCombat_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_RightWepCombat_Right_Cycle
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON
	Priority	4
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_RightWepCombat_Right_Post

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

Move Beast_RightWepCombat_Right_Post
	Requires	BeastRUN COMBAT #WEAPON
	Include sequencers\Members\BeastCombat_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_RightWepCombat_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_RightWepCombat_Jump_Pre
	Requires	BeastRUN JUMP AIR COMBAT WEAPON
	Priority	5
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	NextMove	Beast_RightWepCombat_Jump
	Flags		HideWeapon HideShield
	Interpolate	5

	Type		male
	  Anim		male\beast_run_jump 3 6
# 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd

Move Beast_RightWepCombat_Jump
	Requires	BeastRUN JUMP AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Jump_Cycle ##Apex is for short jumps

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd
MEnd

Move Beast_RightWepCombat_Jump_Cycle
	Requires	BeastRUN APEX AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Apex ##Apex is for higher jumps

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd
MEnd

#############################################################

Move Beast_RightWepCombat_Apex
	Requires	BeastRUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Beast_RightWepCombat_Apex2
	Flags		HideWeapon HideShield ReqInputs

	Type		male
	  Anim		male\beast_run_jump 66 93
	TEnd
MEnd

Move Beast_RightWepCombat_Apex2
	Requires	BeastRUN AIR #COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Beast_RightWepCombat_Jump_Post
	Flags		HideWeapon HideShield Cycle

	Type		male
	  Anim		male\beast_run_jump 93
	TEnd
MEnd

## JUMP POST ###########################################################

Move Beast_RightWepCombat_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Beast_RightWepCombat_Ready_Fast_One
	Flags		HideWeapon HideShield
	Scale		1.1
	SticksOnChild	LAND

	Type		male
	  Anim		male\beast_run_jumppost 1
#	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	TEnd
MEnd

## Beast JUMP UP INTO FORWARD TRANSITION ###########################################################

Move Beast_RightWepCombat_JumpToHop
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<TravelRightWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
#	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## Beast JUMP FORWARD ###########################################################

Move Beast_RightWepCombat_Hop_Pre
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON
	Priority	6
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_RightWepCombat_Hop
	Requires	BeastRUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop_Cycle
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

Move Beast_RightWepCombat_Hop_Cycle
	Requires	BeastRUN JUMP AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop_Apex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_RightWepCombat_Hop_Apex
	Requires	BeastRUN APEX AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop_Fall

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_RightWepCombat_Hop_Fall
	Requires	BeastRUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Hop_Fall_Cycle

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_RightWepCombat_Hop_Fall_Cycle
	Requires	BeastRUN AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_RightWepCombat_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd