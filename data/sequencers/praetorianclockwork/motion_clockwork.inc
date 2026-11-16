#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## RUN FORWARD	###########################################################

Move RunPre
	Requires	FORWARD
	Priority	1
	Interrupts	<Ready> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <GroundMoveALL>
	Flags		ReqInputs ##MoveScale SmoothSprint
	NextMove	RunCycle
	Interpolate	5

	Type		male
	  Anim		male\run 4 10
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 0
	  # PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		female
	  Anim		male\run 4 10
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetFemale.fx 0
	  # PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		male\run 4 10
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 0
	  # PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move RunCycle
	Requires	FORWARD
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	RunPost

	Type		male
	  Anim		male\run 10 34
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 0
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx	12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		female
	  Anim		male\run 10 34
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetFemale.fx 0
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetFemale.fx	12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		huge
	  Anim		male\run 10 34
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 0
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx	12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd
MEnd

## RUN POST ###########################################################

Move RunPost ##Need anim
	Include sequencers\Members\readies.inc
	NextMove	Ready
	Interpolate	5

	Type		male
	  Anim		male\clockwork_run_post 40 56
	  # PlayFx	FEET\LAND3.FX:LAND 3
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 2
	TEnd

	Type		female
	  Anim		male\clockwork_run_post 40 56
	  # PlayFx	FEET\LAND3.FX:LAND 3
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetFemale.fx 2
	TEnd

	Type		huge
	  Anim		male\clockwork_run_post 40 56
	  # PlayFx	FEET\LAND3.FX:LAND 3
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 2
	TEnd
MEnd

#############################################################
## COMBAT MODE
#############################################################

## COMBAT RUN FORWARD ###########################################################

#Move C_RunPre
#	Requires	FORWARD	COMBAT
#	Priority	2
#	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
#	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <GroundMoveALL>
#	Flags		ReqInputs
#	NextMove	C_RunCycle
#
#	Type		male
#	  Anim		male\comb_run 4	9
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#
#	Type		huge
#	  Anim		huge\run 13 25
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#	Type		blackscorpion
#	  Anim		huge\BS_run 13 25
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
#	TEnd
#
#	Type		fem
#	  Anim		fem\fem_run 20 33
#	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#
#	Type		Vahzilok
#	  Anim		Vahzilok\vah_run 1 25
#	TEnd
#
#	Type		RockSuit
#	  Anim		Beast\run 18
#	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT 24
#	TEnd
#MEnd
#
#Move C_RunCycle
#	Requires	FORWARD	COMBAT
#	Priority	2
#	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <GroundMoveALL>
#	Flags		Cycle MoveScale	SmoothSprint
#	MoveRate	0.7
#	NextMove	C_RunPost
#
#	Type		male
#	  Anim		male\comb_run 10 34
#	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
#	  SmoothSprint	3.3
#	TEnd
#
#	Type		huge
#	  Anim		huge\run 1 25
#	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
#	  SmoothSprint	3.3
#	TEnd
#
#	Type		blackscorpion
#	  Anim		huge\BS_run 1 25
#	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
#	  SmoothSprint	3.3
#	TEnd
#
#	Type		fem
#	  Anim		fem\fem_run 10 33
#	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
#	  SmoothSprint	3.2
#	TEnd
#
#	Type		Vahzilok
#	  Anim		Vahzilok\vah_run 1 25
#	TEnd
#
#	Type		RockSuit
#	  Anim		Beast\run 1 25
#	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT
#	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	12
#	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
#	TEnd
#MEnd
#
##############################################################
#
#Move C_RunPost
#	Requires	COMBAT
#	Member		<DeathIRQ> <AttackIRQ> <HitIRQ> <ReactIRQ> <MoveIRQ>
#	NextMove	CombatReady
#	Flags		ReqInputs
#
#	Type		male
#	  Anim		male\comb_run 39
#	  PlayFx	FEET\LAND3.FX:LAND  3
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
#	  scale		0.7
#	TEnd
#
#	Type		huge
#	  Anim		huge\combat_postrun 2
#	  PlayFx	FEET\HUGELAND1.FX:LAND	3
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
#	  scale		0.5
#	TEnd
#
#	Type		fem
#	  Anim		fem\combat_postrun 1
#	  PlayFx	FEET\LAND4.FX:LAND  3
#  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
#	  scale		0.7
#	TEnd
#
#	Type		Vahzilok
#	  Anim		Vahzilok\vah_postrun 4 28
#	  scale		0.5
#	TEnd
#
#	Type		RockSuit
#	  Anim		huge\combat_postrun 2
#	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT  2
#	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	3
#	  scale		0.5
#	TEnd
#MEnd

#############################################################
## WALK FORWARD
#############################################################

Move WalkCycle_Pre
	Requires	FORWARD	WALK
	Priority	2
	Interrupts	<Ready> <MoveIRQ> #<GroundMoveALL>
	Member		<DeathIRQ> <AttackIRQ> <WalkMove>
	Flags		MoveScale
	NextMove	WalkCycle

	Type		male
	  Anim		male\Player_Walk_forward 24
	  MoveRate	0.1625 #5 feet per second
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 1
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_forward 24
	  MoveRate	0.175 #5 feet per second
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 1
	TEnd
MEnd

Move WalkCycle
	Requires	FORWARD	WALK
	Member		<DeathIRQ> <AttackIRQ> <WalkMove>
	Flags		Cycle MoveScale
	NextMove	WalkPost
	Priority	2

	Type		male
	  Anim		male\Player_Walk_forward 1
	  # PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 1
	  # PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 14
	  MoveRate	0.1625 #5 feet per second
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 1
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 18
	TEnd

	Type		huge
	  Anim		huge\Player_Walk_forward 1
	  # PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 1
	  # PlayFx	FEET\HUGELEFT.FX:FOOTFALLRIGHT 14
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 1
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 14
	  MoveRate	0.175 #5 feet per second
	TEnd
MEnd

Move WalkPost ##Need anim
	Include sequencers\Members\readies.inc
	NextMove	Ready
	Interpolate	8

	Type		male
	  Anim		male\clockwork_walk_post 1
	  PlayFx	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 1
	TEnd

#	Type		huge
#	  Anim		huge\postback_right 28
#	TEnd
MEnd

#############################################################
## JUMPS
#############################################################

## JUMP UP ###########################################################

Move jumpPre
	Requires	JUMP AIR
	Priority	2
	Interrupts	<Ready> <GroundMoveALL> <iceslide> <Swim> <jumpFall> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	jumpUp

	Type		male
	  Anim		male\superjump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd
MEnd

Move jumpUp
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	jumpApex

	Type		male
	  Anim		male\superjump 6 65

	TEnd
MEnd

Move jumpApex
	Requires	APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	jumpFall

	Type		male
	  Anim		male\superjump 65 66
	TEnd
MEnd

Move jumpFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	jumpFall2

	Type		male
	  Anim		male\superjump 66 93
	TEnd
MEnd

Move jumpFall2
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpPost

	Type		male
	  Anim		male\superjump 93
	TEnd
MEnd

## JUMP POST ###########################################################

Move JumpPost ##Needs New Anim
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <IRQEmotesALL>
	NextMove	Ready
	Interpolate	5
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\clockwork_jump_post 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 1
	TEnd
MEnd

## JUMP UP INTO FORWARD TRANSITION ###########################################################

Move JumpToHop
	Requires	FORWARD	JUMP AIR
	Priority	3
	Interrupts	jumppre	jumpup
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop2
	Interpolate	15

	Type		male
	  Anim		male\test_hop 10 91
	TEnd
MEnd

## JUMP FORWARD ###########################################################

Move Hop_Pre
	Requires	FORWARD	JUMP AIR
	Priority	3
	Interrupts	<Ready> <GroundMoveALL> <iceslide> <Swim> <jumpFall> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop

	Type		male
	  Anim		male\test_hop 10 11
	  PlayFx	FEET\LAND1.FX
	TEnd
MEnd

Move Hop
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop2
	Scale		0.75

	Type		male
	  Anim		male\test_hop 11 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd
MEnd

Move Hop2
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	HopApex

	Type		male
	  Anim		male\test_hop 91 91
	TEnd
MEnd

Move HopApex
	Requires	APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	HopFall

	Type		male
	  Anim		male\test_hop 91 93
	TEnd
MEnd

Move HopFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	HopFall_2

	Type		male
	  Anim		male\test_hop 93 128
	TEnd
MEnd

Move HopFall_2
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpPost

	Type		male
	  Anim		male\test_hop 128
	TEnd
MEnd

#############################################################
## STUN FORWARD
#############################################################

Move dizzy_forward
	Requires	FORWARD STUN
	Priority	11
	Interrupts	<MoveIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <StunMove> <GroundMoveALL>
	NextMove	Stun_cycle
	Scale		0.5
	Flags		Cycle
	Interpolate	20

	Type		male
	  Anim		male\stun_walk 1 27
	TEnd
MEnd

#############################################################

#Include sequencers\player\Motion\motion_doors.inc
#Include sequencers\player\Motion\motion_fall.inc
#Include sequencers\player\Motion\motion_slide.inc