#############################################################
## motion_Parkour.inc - Covers Ready and Combat modes
#############################################################


## RUN FORWARD	###########################################################

Move Parkour_Forward
	Requires	PARKOURRUN FORWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Parkour_ForwardPost
	CycleMove	Parkour_Forward_Cycle
	Interpolate	5

	Type		male
	  Anim		male\parkour_run 1 20
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Parkour_Forward_Cycle
	Requires	PARKOURRUN FORWARD #COMBAT
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	RunPost

	Type		male
	  Anim		male\parkour_run 1 20
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move Parkour_ForwardPost
	Include sequencers\Members\ParkourCombat.inc
	NextMove	Ready
	Interpolate	5

	Type		male
	  Anim		male\run 40
	  Scale		0.85
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Parkour_Back
	Requires	PARKOURRUN BACKWARD #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375 ##0.7 x 30 = I animated this to move at 21 feet per second
	CycleMove	Parkour_Back_Cycle
	NextMove	Parkour_ForwardPost

	Type		male
	  Anim		male\Parkour_run_backward 1 17
	 #Anim		male\Parkour_run_backward_pre 1 14
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Parkour_Back_Cycle
	Requires	PARKOURRUN BACKWARD #COMBAT
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <FlyReady> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.125  ##0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	Parkour_Back_Post
	Interpolate	5

	Type		male
	  Anim		male\Parkour_run_backward 1 17
	  SmoothSprint	2.891 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_BACK POST ###########################################################

Move Parkour_Back_Post
	Requires	PARKOURRUN
	Include sequencers\Members\ParkourCombat.inc
	CycleMove	Ready
	NextMove	postReady_back
	Flags		Cycle
	Interpolate	5

	Type		male
	  Anim		male\Parkour_run_backward 1
	 #Anim		male\Parkour_run_backward_post 1
	  Scale		1.2
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Parkour_Left
	Requires	PARKOURRUN STEPLEFT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	CycleMove	Parkour_Left_Cycle
	NextMove	Parkour_Left_Post
	Interpolate	5

	Type		male
	  Anim		male\Parkour_run_left 1 2
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Parkour_Left_Cycle
	Requires	PARKOURRUN STEPLEFT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Parkour_Left_Post

	Type		male
	  Anim		male\Parkour_run_left 1 17
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_LEFT POST ###########################################################

Move Parkour_Left_Post
	Requires	PARKOURRUN
	Include sequencers\Members\ParkourCombat.inc
	Flags		Cycle
	CycleMove	Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\run 40
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Parkour_Right
	Requires	PARKOURRUN STEPRIGHT #COMBAT
	Priority	3
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> <SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Parkour_ForwardPost
	CycleMove	Parkour_Right_Cycle
	Interpolate	5

	Type		male
	  Anim		male\Parkour_run_right 1 2
	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move Parkour_Right_Cycle
	Requires	PARKOURRUN STEPRIGHT #COMBAT
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <TravelMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	1.375
	NextMove	Parkour_Right_Post

	Type		male
	  Anim		male\Parkour_run_right 1 17

	  SmoothSprint	3.154 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

## RUN_RIGHT POST ###########################################################

Move Parkour_Right_Post
	Requires	PARKOURRUN
	Include sequencers\Members\ParkourCombat.inc
	Flags		Cycle
	CycleMove	Ready
	NextMove	postReady_back
	Interpolate	5

	Type		male
	  Anim		male\run 40
	TEnd
MEnd

## JUMP UP ###########################################################

#Move Parkour_Jump_Pre
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Priority	4
#	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
#	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
#	NextMove	Parkour_Jump
#	Interpolate	5
#
#	Type		male
#	  Anim		male\Parkour_jump 3 6
#	  PlayFX	POWERS\ParkourRun\Move_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_Jump
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Apex #Apex is for short jumps
#	CycleMove	Parkour_Jump_Cycle
#
#	Type		male
#	  Anim		male\Parkour_jump 6 10
#	TEnd
#MEnd
#
#Move Parkour_Jump_Cycle
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Apex2 #Apex2 is for higher jumps
#
#	Type		male
#	  Anim		male\Parkour_jump 6 40
#	TEnd
#MEnd
#
##############################################################
#
#Move Parkour_Jump_Apex #Apex is for short jumps
#	Requires	ParkourRUN APEX AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	NextMove	Parkour_Jump_Fall
#	Interpolate	5
#	Scale		1.2
#
#	Type		male
#	  Anim		male\Parkour_jump 40 46
#	  PlayFX	POWERS\ParkourRun\Jump_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_Jump_Apex2 #Apex2 is for higher jumps
#	Requires	ParkourRUN APEX AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	NextMove	Parkour_Jump_Fall2
#
#	Type		male
#	  Anim		male\Parkour_jump 65 66
#	  PlayFX	POWERS\ParkourRun\Jump_Parkour.fx
#	TEnd
#MEnd
#
##############################################################
#
#Move Parkour_Jump_Fall
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	CycleMove	Parkour_Jump_Fall_Cycle
#	NextMove	Parkour_Jump_Post
#	Interpolate	2
#
#	Type		male
#	  Anim		male\Parkour_jump 46 53
#	TEnd
#MEnd
#
#Move Parkour_Jump_Fall_Cycle
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Post
#
#	Type		male
#	  Anim		male\Parkour_jump 54 54
#	TEnd
#MEnd
#
##############################################################
#
#Move Parkour_Jump_Fall2
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	CycleMove	Parkour_Jump_Fall_Cycle2
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_jump 66 93
#	TEnd
#MEnd
#
#Move Parkour_Jump_Fall_Cycle2
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_jump 93 130
#	TEnd
#MEnd
#
### JUMP POST ###########################################################
#
#Move Parkour_Jump_Post
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
#	NextMove	Parkour_Ready
#	SticksOnChild	LAND
#	Interpolate	1
#
#	Type		male
#	  Anim		male\Parkour_jump 54 64
#	  PlayFx	POWERS\ParkourRun\LAND1_Parkour.FX:LAND
#	TEnd
#
#	Type		CameraShaker
#	  Anim		male\comb_postjump 1
#	  PlayFx	POWERS\ParkourRun\LAND1_Parkour.FX:LAND
#	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
#	TEnd
#MEnd
#
#Move Parkour_Jump_Post2
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
#	NextMove	Parkour_Ready
#	SticksOnChild	LAND
#	Interpolate	1
#
#	Type		male
#	  Anim		male\Parkour_jump 130 150
#	  PlayFx	POWERS\ParkourRun\LAND1_Parkour.FX:LAND
#	  PlayFx	fx\Feet\MaleLand_Parkour.fx:LAND 1
#	TEnd
#
#	Type		CameraShaker
#	  Anim		male\comb_postjump 1
#	  PlayFx	POWERS\ParkourRun\LAND1_Parkour.FX:LAND
#	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
#	TEnd
#MEnd
#
#Move Parkour_Jump_PostCycle
#	Requires	ParkourRUN #COMBAT
#	Include sequencers\Members\ParkourCombat.inc
#	Flags		Cycle
#	NextMove	postReady_back
#	Interpolate	15
#
#	Type		male
#	  Anim		male\Parkour_cycle 17 47
#	TEnd
#MEnd
#
### Parkour JUMP UP INTO FORWARD TRANSITION ###########################################################
#
#Move Parkour_JumpToHop
#	Requires	ParkourRUN FORWARD JUMP AIR #COMBAT
#	Priority	5
#	Interrupts	<TravelJump>
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop2
#	Interpolate	15
#
#	Type		male
#	  Anim		male\Parkour_hop 2 82
#	  PlayFX	POWERS\ParkourRun\Move_Parkour.fx 2
#	TEnd
#MEnd
#
### Parkour JUMP FORWARD ###########################################################
#
#Move Parkour_HopPre
#	Requires	ParkourRUN FORWARD JUMP AIR #COMBAT
#	Priority	5
#	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop
#	Interpolate	5
#
#	Type		male
#	  Anim		male\Parkour_hop 1 2
#	TEnd
#MEnd
#
#Move Parkour_Hop
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop2
#	Scale		0.75
#
#	Type		male
#	  Anim		male\Parkour_hop 2 82
#	  PlayFX	POWERS\ParkourRun\Hop_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_Hop2
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_HopApex
#
#	Type		male
#	  Anim		male\Parkour_hop 82 82
#	TEnd
#MEnd
#
#Move Parkour_HopApex
#	Requires	ParkourRUN APEX AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	NextMove	Parkour_HopFall
#
#	Type		male
#	  Anim		male\Parkour_hop 82 85
#	  PlayFX	POWERS\ParkourRun\Jump_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_HopFall
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_HopFall_2
#
#	Type		male
#	  Anim		male\Parkour_hop 85 124
#	TEnd
#MEnd
#
#Move Parkour_HopFall_2
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_hop 124
#	TEnd
#MEnd
#
### Parkour JUMP RIGHT ###########################################################
#
#Move Parkour_Hop_Right_Pre
#	Requires	ParkourRUN STEPRIGHT JUMP AIR #COMBAT
#	Priority	5
#	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop_Right
#
#	Type		male
#	  Anim		male\Parkour_hop_right 1 2
#	TEnd
#MEnd
#
#Move Parkour_Hop_Right
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop2_Right
#	Scale		0.75
#
#	Type		male
#	  Anim		male\Parkour_hop_right 2 82
#	  PlayFX	POWERS\ParkourRun\Hop_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_Hop2_Right
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_HopApex_Right
#
#	Type		male
#	  Anim		male\Parkour_hop_right 82 82
#	TEnd
#MEnd
#
#Move Parkour_HopApex_Right
#	Requires	ParkourRUN APEX AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	NextMove	Parkour_HopFall_Right
#
#	Type		male
#	  Anim		male\Parkour_hop_right 82 85
#	  PlayFX	POWERS\ParkourRun\Jump_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_HopFall_Right
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	CycleMove	Parkour_HopFall2_Right
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_hop_right 85 124
#	TEnd
#MEnd
#
#Move Parkour_HopFall2_Right
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_hop_right 124
#	TEnd
#MEnd
#
### Parkour JUMP LEFT ###########################################################
#
#Move Parkour_HopPre_Left
#	Requires	ParkourRUN STEPLEFT JUMP AIR #COMBAT
#	Priority	5
#	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop_Left
#	Interpolate	5
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 1 2
#	TEnd
#MEnd
#
#Move Parkour_Hop_Left
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		ReqInputs
#	NextMove	Parkour_Hop2_Left
#	Scale		0.75
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 2 82
#	  PlayFX	POWERS\ParkourRun\Hop_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_Hop2_Left
#	Requires	ParkourRUN JUMP AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_HopApex_Left
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 82 82
#	TEnd
#MEnd
#
#Move Parkour_HopApex_Left
#	Requires	ParkourRUN APEX AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <AttackIRQ>
#	NextMove	Parkour_HopFall_Left
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 82 85
#	  PlayFX	POWERS\ParkourRun\Jump_Parkour.fx
#	TEnd
#MEnd
#
#Move Parkour_HopFall_Left
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	CycleMove	Parkour_HopFall2_Left
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 85 124
#	TEnd
#MEnd
#
#Move Parkour_HopFall2_Left
#	Requires	ParkourRUN AIR #COMBAT
#	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
#	Flags		Cycle
#	NextMove	Parkour_Jump_Post2
#
#	Type		male
#	  Anim		male\Parkour_hop_Left 124
#	TEnd
#MEnd

#############################################################

#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Righthand.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Righthand_Gun.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Dual.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Dual_Pistols.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Dual_Claws.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Dual_Lefthand.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Shield.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_ShieldWeapon.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_Two_Handed_Weapon.inc
#Include sequencers\player\Motion\Parkour\motion_Parkour_CombatWeapon_BeamRifle.inc