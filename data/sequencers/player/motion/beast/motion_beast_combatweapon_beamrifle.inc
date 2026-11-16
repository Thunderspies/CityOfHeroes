#############################################################
## motion_Ninja.inc - Covers Ready and Combat modes
#############################################################

## Weapon Right Hand GUN READY ###########################################################

Move Beast_BeamRifle_Ready_Pre
	Requires	BEASTRUN COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> #<TravelCombat>
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	Flags		Cycle
	CycleMove	Beast_BeamRifle_Ready_Fast_One
	NextMove	BeamRifle_Ready
	Interpolate	5

	Type		male
	  Anim		male\beast_run_cycle_right_pistol 1 10
	 PlayFx		POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

Move Beast_BeamRifle_Ready_Fast_One
	Requires	BEASTRUN COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	#Flags		Cycle
	NextMove	Beast_BeamRifle_Ready_Fast_Two
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_pistol 10 50
	TEnd
MEnd

Move Beast_BeamRifle_Ready_Fast_Two
	Requires	BEASTRUN COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	#Flags		Cycle
	NextMove	Beast_BeamRifle_Ready_Fast_Three
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_pistol 10 50
	TEnd
MEnd

Move Beast_BeamRifle_Ready_Fast_Three
	Requires	BEASTRUN COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	#Flags		Cycle
	NextMove	Beast_BeamRifle_Ready_Slow
	Scale		1

	Type		male
	  Anim		male\beast_run_cycle_right_pistol 10 50
	TEnd
MEnd

Move Beast_BeamRifle_Ready_Slow
	Requires	BEASTRUN COMBAT WEAPON BEAMRIFLE
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	Flags		Cycle
	NextMove	BeamRifle_Ready
	Scale		0.5

	Type		male
	  Anim		male\beast_run_cycle_right_pistol 10 50
	TEnd
MEnd

## RUN FORWARD	###########################################################

Move Beast_BeamRifle_Forward
	Requires	BeastRUN FORWARD COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	7
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
#	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
#	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_BeamRifle_ForwardPost
	CycleMove	Beast_BeamRifle_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
#	  PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_BeamRifle_Forward_Cycle
	Requires	BeastRUN FORWARD COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Beast_BeamRifle_ForwardPost

	Type		male
	  Anim		male\Beast_run_forward 1 22
#	  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
#	#PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Beast_BeamRifle_ForwardPost
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	NextMove	Beast_BeamRifle_Ready_Fast_One
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_forward_post 1 29
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Beast_BeamRifle_Back
	Requires	BeastRUN BACKWARD COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	7
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide>  <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Beast_BeamRifle_Back_Cycle
	NextMove	Beast_BeamRifle_ForwardPost

	Type		male
	  Anim		male\Beast_run_backward_pre 1 14
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_BeamRifle_Back_Cycle
	Requires	BeastRUN BACKWARD COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.125  ##0.7 x 30 = I animated  this to move at 21 feet per second
	NextMove	Beast_BeamRifle_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11

	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Beast_BeamRifle_Back_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	CycleMove	Beast_BeamRifle_Ready_Fast_One
	NextMove	postReady_back
	Flags		Cycle HideWeapon HideShield
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_backward_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Beast_BeamRifle_Left
	Requires	BeastRUN STEPLEFT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	7
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	CycleMove	Beast_BeamRifle_Left_Cycle
	NextMove	Beast_BeamRifle_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_left 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_BeamRifle_Left_Cycle
	Requires	BeastRUN STEPLEFT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_BeamRifle_Left_Post

	Type		male
	  Anim		male\Beast_run_left 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Beast_BeamRifle_Left_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_BeamRifle_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_left_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Beast_BeamRifle_Right
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	7
	Interrupts	<Ready> <Combat> <WepCombat> <BasicMove> <CombatMove> <TravelMove> <TravelWepMove> <MoveIRQ> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_BeamRifle_ForwardPost
	CycleMove	Beast_BeamRifle_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Beast_run_right 1 2
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	 # PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Beast_BeamRifle_Right_Cycle
	Requires	BeastRUN STEPRIGHT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelGunMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint HideWeapon HideShield
	MoveRate	1.375
	NextMove	Beast_BeamRifle_Right_Post

	Type		male
	  Anim		male\Beast_run_right 1 22
	#  PlayFx	POWERS\BeastRun\MALELEFT_Beast.FX:FOOTFALLLEFT 5
	  #PlayFx	POWERS\BeastRun\MALERIGHT_Beast.FX:FOOTFALLRIGHT 11
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Beast_BeamRifle_Right_Post
	Requires	BeastRUN
	Include sequencers\Members\BeastCombat_Weapon_TwoHand.inc
	Flags		Cycle HideWeapon HideShield
	CycleMove	Beast_BeamRifle_Ready_Fast_One
	NextMove	postReady_back
	Interpolate	5
	Scale		2

	Type		male
	  Anim		male\Beast_run_right_post 1
	  PlayFx	POWERS\NinjaRun\STOP_NINJA.FX:LAND
	TEnd
MEnd

## JUMP UP ###########################################################

Move Beast_BeamRifle_Jump_Pre
	Requires	BeastRUN JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	8
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	NextMove	beast_run_jumpUp
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_jump 3 6
 	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd
#
#Move Beast_BeamRifle_Jump
#	Requires	BeastRUN JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
#	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
#	Flags		Cycle HideWeapon HideShield
#	CycleMove	Beast_BeamRifle_Jump_Cycle
#
#	Type		male
#	  Anim		male\beast_run_jump 6 65
#	TEnd
#MEnd
#
#Move Beast_BeamRifle_Jump_Cycle
#	Requires	BeastRUN APEX AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		Cycle HideWeapon HideShield
#	NextMove	Beast_BeamRifle_Jump_Apex #Apex2 is for higher jumps
#
#	Type		male
#	  Anim		male\beast_run_jump 65 66
#	TEnd
#MEnd
#
##############################################################
#
#Move Beast_BeamRifle_Jump_Apex #Apex is for short jumps
#	Requires	BeastRUN AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	NextMove	Beast_BeamRifle_Jump_Apex2
#	Flags		HideWeapon HideShield ReqInputs
#
#	Type		male
#	  Anim		male\beast_run_jump 66 93
#	TEnd
#MEnd
#
#Move Beast_BeamRifle_Jump_Apex2 #Apex2 is for higher jumps
#	Requires	BeastRUN AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	NextMove	Beast_BeamRifle_Jump_Post
#	Flags		HideWeapon HideShield Cycle
#
#	Type		male
#	  Anim		male\beast_run_jump 93
#	TEnd
#MEnd

## JUMP POST ###########################################################

Move Beast_BeamRifle_Jump_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	Beast_BeamRifle_Ready_Fast_One
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

Move Beast_BeamRifle_JumpToHop
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	9
	Interrupts	<TravelGunJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_BeamRifle_Hop2
	Interpolate	15

	Type		male
	  Anim		male\beast_run_hop 10 91
	  PlayFX	POWERS\NinjaRun\Move_Ninja.fx
	TEnd
MEnd

## Beast JUMP HOP FORWARD ###########################################################

Move Beast_BeamRifle_HopPre
	Requires	BeastRUN FORWARD JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority	9
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_BeamRifle_Hop

	Type		male
	  Anim		male\beast_run_hop 10 11
	TEnd
MEnd

Move Beast_BeamRifle_Hop
	Requires	BeastRUN JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_BeamRifle_Hop2
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

Move Beast_BeamRifle_Hop2
	Requires	BeastRUN JUMP AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_BeamRifle_HopApex

	Type		male
	  Anim		male\beast_run_hop 91 91
	TEnd
MEnd

Move Beast_BeamRifle_HopApex
	Requires	BeastRUN APEX AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Beast_BeamRifle_HopFall
	Flags		HideWeapon HideShield

	Type		male
	  Anim		male\beast_run_hop 91 93
	TEnd
MEnd

Move Beast_BeamRifle_HopFall
	Requires	BeastRUN AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs HideWeapon HideShield
	NextMove	Beast_BeamRifle_HopFall_2

	Type		male
	  Anim		male\beast_run_hop 93 128
	TEnd
MEnd

Move Beast_BeamRifle_HopFall_2
	Requires	BeastRUN AIR COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle HideWeapon HideShield
	NextMove	Beast_BeamRifle_Jump_Post

	Type		male
	  Anim		male\beast_run_hop 128
	TEnd
MEnd