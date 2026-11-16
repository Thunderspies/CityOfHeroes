#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## Shield + Weapon READY ###########################################################

Move Beast_ShieldWepCombat_Pre
	Requires	BEASTRUN SHIELD COMBAT WEAPON
	Priority	24
	Interrupts	<Ready> <ShieldCombat> <TravelWepCombat> <TravelShieldCombat>
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle
	CycleMove	Beast_ShieldWepCombat_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle_sword_and_shield 1 10
	 PlayFx		POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Beast_ShieldWepCombat_Ready_Fast_One
	Requires	BEASTRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_ShieldWepCombat_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_sword_and_shield 10 50
	TEnd
MEnd

Move Beast_ShieldWepCombat_Ready_Fast_Two
	Requires	BEASTRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_ShieldWepCombat_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_sword_and_shield 10 50
	TEnd
MEnd

Move Beast_ShieldWepCombat_Ready_Fast_Three
	Requires	BEASTRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	#Flags		Cycle
	NextMove	Beast_ShieldWepCombat_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_sword_and_shield 10 50
	TEnd
MEnd

Move Beast_ShieldWepCombat_Ready_Slow
	Requires	BEASTRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle_sword_and_shield 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_ShieldWepCombat_Forward
	Requires	BeastRUN FORWARD SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Forward_Post
	CycleMove	Beast_ShieldWepCombat_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
#	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_ShieldWepCombat_Forward_Cycle
	Requires	BeastRUN FORWARD WEAPON
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Forward_Post

	Type		male
	  Anim		male\Beast_run_forward 1 22
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
#	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_ShieldWepCombat_Forward_Post
	Requires	BeastRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_ShieldWepCombat_Ready_Fast_One
	NextMove	postReady_back #postReady_back
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_ShieldWepCombat_Back
	Requires	BeastRUN BACKWARD SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_ShieldWepCombat_Back_Cycle
	NextMove	Beast_ShieldWepCombat_Back_Post

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_ShieldWepCombat_Back_Cycle
	Requires	BeastRUN BACKWARD SHIELD COMBAT WEAPON
 	Priority	26 ##1 higher to take priority
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_ShieldWepCombat_Back_Post
	Requires	BeastRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_ShieldWepCombat_Ready_Fast_One
	NextMove	postReady_back #postReady_back
	Interpolate	8

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_ShieldWepCombat_Left
	Requires	BeastRUN STEPLEFT SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Left_Post
	CycleMove	Beast_ShieldWepCombat_Left_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT  5
	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_ShieldWepCombat_Left_Cycle
	Requires	BeastRUN STEPLEFT SHIELD COMBAT WEAPON
	Priority	26 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_ShieldWepCombat_Left_Post
	Requires	BeastRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_ShieldWepCombat_Ready_Fast_One
	NextMove	postReady_back #postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_ShieldWepCombat_Right
	Requires	BeastRUN STEPRIGHT SHIELD COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldCombat> <BasicMove> <CombatMove> <WepMove> <DualHandMove> <TravelWepMove> <TravelShieldMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Right_Post
	CycleMove	Beast_ShieldWepCombat_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_ShieldWepCombat_Right_Cycle
	Requires	BeastRUN STEPRIGHT SHIELD COMBAT WEAPON
	Priority	26 ##1 higher to take priority
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelShieldWepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_ShieldWepCombat_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_ShieldWepCombat_Right_Post
	Requires	BeastRUN SHIELD COMBAT WEAPON
	Include sequencers\Members\BeastCombat_Shield_Weapon.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_ShieldWepCombat_Ready_Fast_One
	NextMove	postReady_back #postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_ShieldWepCombat_Jump_Pre
	Requires	BeastRUN JUMP AIR SHIELD COMBAT WEAPON
	Priority	26
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	NextMove	Beast_ShieldWepCombat_Jump
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 3 6
 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd

Move Beast_ShieldWepCombat_Jump
	Requires	BeastRUN JUMP AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Jump_Cycle ##Apex is for short jumps

	Type		male
	  Anim		male\beast_run_jump 6 65
	TEnd
MEnd

Move Beast_ShieldWepCombat_Jump_Cycle
	Requires	BeastRUN APEX AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Apex ##Apex is for higher jumps

	Type		male
	  Anim		male\beast_run_jump 65 66
	TEnd
MEnd

#############################################################

Move Beast_ShieldWepCombat_Apex ##Apex is for short jumps
	Requires	BeastRUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <ShieldFall>
	NextMove	Beast_ShieldWepCombat_Apex2
	Flags		HideWeapon HideShield ReqInputs

	Type		male
	  Anim		male\beast_run_jump 66 93
	#  PlayFX	POWERS\BeastRun\Jump_Beast.fx
	TEnd
MEnd

Move Beast_ShieldWepCombat_Apex2 ##Apex is for higher jumps
	Requires	BeastRUN AIR #SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldFall> <Jumps>
	NextMove	Beast_ShieldWepCombat_Jump_Post
	Flags		HideWeapon HideShield cycle

	Type		male
	  Anim		male\beast_run_jump 93
	#  PlayFX	POWERS\BeastRun\Jump_Beast.fx
	TEnd
MEnd

## JUMP POST ###########################################################

Move Beast_ShieldWepCombat_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Beast_ShieldWepCombat_Ready_Fast_One
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

Move Beast_ShieldWepCombat_JumpToHop
	Requires	BeastRUN FORWARD JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<TravelShieldWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Hop_Cycle
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## Beast JUMP FORWARD ###########################################################

Move Beast_ShieldWepCombat_Hop_Pre
	Requires	BeastRUN FORWARD JUMP AIR SHIELD COMBAT WEAPON
	Priority	27
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Hop

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_ShieldWepCombat_Hop
	Requires	BeastRUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Hop_Cycle
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

Move Beast_ShieldWepCombat_Hop_Cycle
	Requires	BeastRUN JUMP AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Hop_Apex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_ShieldWepCombat_Hop_Apex
	Requires	BeastRUN APEX AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Beast_ShieldWepCombat_Hop_Fall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_ShieldWepCombat_Hop_Fall
	Requires	BeastRUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Hop_Fall_Cycle

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_ShieldWepCombat_Hop_Fall_Cycle
	Requires	BeastRUN AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_ShieldWepCombat_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd