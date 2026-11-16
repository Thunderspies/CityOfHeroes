#######################################################################################################
## MOTIONS - basic player movement sequences
#######################################################################################################

## RUN FORWARD	###########################################################

Move RunPre
	Requires	FORWARD
	Priority	1
	Interrupts	<Ready> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale SmoothSprint
	CycleMove	RunCycle
	NextMove	Idle_Walk_Post
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	Interpolate	5

	Type		male
	  Anim		male\run 1 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	Feet\StartScuff.fx 1
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		PraetorianPolice
	  Anim		male\run 1 10
	  PlayFx	FEET\PPD_Left.fx 1
	  PlayFx	FEET\PPD_Right.fx	7
	  PlayFx	Feet\StartScuff.fx 1
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		ResistanceMale
	  Anim		male\run 1 10
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx	7
	  PlayFx	Feet\StartScuff.fx 1
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		ResistanceFem
	  Anim		male\run 1 10
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx	7
	  PlayFx	Feet\StartScuff.fx 1
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		CameraShaker
	  Anim		male\run 4 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 10
	TEnd

	Type		huge
	  Anim		huge\run 18 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	3
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		BlackScorpion
	  Anim		huge\BS_run 18 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	3
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\fem_run 4 9
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		Crystal
	  Anim		Beast\run 1 25
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		RockSuit
	  Anim		Beast\run 18
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 24
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		Monster
	  Anim		male\goblin_run	1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	22
	  Scale		1
	  MoveRate	0.5
	TEnd


	Type		InvisibleMaleTarget_Silent
	  Anim		male\run 1 10
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	10
	  #PlayFx	Feet\StartScuff.fx 1
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\fem_run 4 9
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\fem_run 4 9
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\fem_run 4 9
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\fem_run 4 9
	  SmoothSprint	3.2
	TEnd

	Type		Abomination
	  Anim		huge\run 18 25
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Walk_Right.fx 1
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

MEnd

Move RunCycle
	Requires	FORWARD
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	RunPost

	Type		male
	  Anim		male\run 10 34
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	22
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		PraetorianPolice
	  Anim		male\run 10 34
	  PlayFx	FEET\PPD_Left.fx 10
	  PlayFx	FEET\PPD_Right.fx	22
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		ResistanceMale
	  Anim		male\run 10 34
	  PlayFx	FEET\RES_Left.fx 10
	  PlayFx	FEET\RES_Right.fx	22
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		ResistanceFem
	  Anim		male\run 10 34
	  PlayFx	FEET\RES_Left.fx 10
	  PlayFx	FEET\RES_Right.fx	22
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		CameraShaker
	  Anim		male\run 10 34
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	22
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 21
	TEnd

	Type		huge
	  Anim		huge\run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		BlackScorpion
	  Anim		huge\BS_run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		fem
	  Anim		fem\fem_run 10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		Crystal
	  Anim		Beast\run 1 25
	  PlayFx	FEET\ROBOFOOT1.FX 1
	  PlayFx	FEET\ROBOFOOT2.FX 13
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		RockSuit
	  Anim		Beast\run 1 25
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		Monster
	  Anim		male\goblin_run	1
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  Scale		1
	  MoveRate	0.5
	TEnd

	Type		Shivan
	  Anim		male\goblin_run	1
	  Scale		1
	  MoveRate	0.5
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\run 10 34
	  #PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	22
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		Abomination
	  Anim		huge\run 1 25
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Walk_Left.fx 10
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Walk_Right.fx 22
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd


MEnd

## RUN POST ###########################################################

Move RunPost
	Include sequencers\Members\readies.inc
	NextMove	Ready
	Interpolate	8

	Type		male
	  Anim		male\run 40
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	TEnd

	Type		PraetorianPolice
	  Anim		male\run 40
	  PlayFx	FEET\PPD_Land1.FX:LAND  3
	  PlayFx	FEET\PPD_Right.fx 12
	TEnd

	Type		ResistanceMale
	  Anim		male\run 40
	  PlayFx	FEET\RES_Land1.FX:LAND  3
	  PlayFx	FEET\RES_Right.fx 12
	TEnd

	Type		ResistanceFem
	  Anim		male\run 40
	  PlayFx	FEET\RES_Land1.FX:LAND  3
	  PlayFx	FEET\RES_Right.fx 12
	TEnd

	Type		CameraShaker
	  Anim		male\run 40
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		huge
	  Anim		huge\postrun 1 23
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	TEnd

	Type		fem
	  Anim		fem\postrun 3
	  PlayFx	FEET\LAND4.FX:LAND  3
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_postrun 4 28
	TEnd

	Type		Crystal
	  Anim		male\MonComb_postwalk 1
	TEnd

	Type		RockSuit
	  Anim		huge\postrun 1 23
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 3
	TEnd

	Type		Monster
	  Anim		male\moncomb_postwalk 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		1.7
	TEnd

	Type		Shivan
	  Anim		male\goblin_run	1
	  Scale		1
	  MoveRate	0.5
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		fem\postrun 3
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\postrun 3
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\postrun 3
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\postrun 3
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\postrun 3
	TEnd

	Type		Abomination
	  Anim		huge\postrun 1 23
#	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Walk_Right.fx 1
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Walk_Left.fx 5
	TEnd


MEnd

## STUMBLE ###########################################################

Move Stumble
	Requires	FORWARD	STUMBLE
	Priority	2
	Interrupts	<Ready>	<BasicMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		MoveScale SmoothSprint
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	RunPost

	Type		male
	  Anim		bis\scaredrun 1	10
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 21
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint	3.3 ##feet from	root to	hip position you want to use to	smooth sprinting
	TEnd
MEnd

## RUN BACKWARD	###########################################################

Move Run_Back_Pre
	Requires	BACKWARD
	Priority	1
	Interrupts	<Ready> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	CycleMove	Run_Back_Cycle
	NextMove	Idle_Walk_Post
	Interpolate	5
	MoveRate	0.525

	Type		male
	  Anim		male\runback 5 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runback 5 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\fem_runback	5 9
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd

	Type		RockSuit
	  Anim		beast\runback 18 24
	TEnd
MEnd

Move Run_Back_Cycle
	Requires	BACKWARD
	Priority	1
	Interrupts	<BackMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	RunPost

	Type		male
	  Anim		male\runback 10	33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runback 10	33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 21
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\fem_runback	10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	21
	  SmoothSprint	3.2
	TEnd

	Type		RockSuit
	  Anim		beast\runback 1	24
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	6
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 18
	TEnd
MEnd

## RUN LEFT ###########################################################

Move Run_Left_Pre
	Requires	STEPLEFT
	Priority	1
	Interrupts	<Ready> <MoveIRQ> RunPre
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	CycleMove	Run_Left_Cycle
	NextMove	Idle_Walk_Post
	MoveRate	0.7
	Interpolate	5

	Type		male
	  Anim		male\runleft 6 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runleft 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsLEFT.FX:FOOTFALLLEFT	10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 6
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\ssrunL 6 9
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move Run_Left_Cycle
	Requires	STEPLEFT
	Priority	1
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	NextMove	RunPost

	Type		male
	  Anim		male\runleft 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runleft 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 1	25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\ssrunL 10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd
MEnd


## RUN RIGHT ###########################################################

Move Run_Right_Pre
	Requires	STEPRIGHT
	Priority	1
	Interrupts	<Ready> <MoveIRQ> RunPre
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale SmoothSprint
	CycleMove	Run_Right_Cycle
	NextMove	Idle_Walk_Post
	MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	Interpolate	5

	Type		male
	  Anim		male\runright 1 25
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runright 13 25
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\ssrunR 6 9
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move Run_Right_Cycle
	Requires	STEPRIGHT
	Priority	1
	Interrupts	<SideMoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <BackMoveIRQ>
	Flags		Cycle MoveScale	 SmoothSprint
	MoveRate	0.7
	NextMove	RunPost

	Type		male
	  Anim		male\runright 1	25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		CameraShaker
	  Anim		male\runright 1	25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsLEFT.FX:FOOTFALLLEFT	10
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\ssrunR 10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


#############################################################
## COMBAT MODE
#############################################################

## COMBAT RUN FORWARD ###########################################################

Move C_RunPre
	Requires	FORWARD	COMBAT
	Priority	2
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	C_RunCycle

	Type		male
	  Anim		male\comb_run 4	9
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		PraetorianPolice
	  Anim		male\comb_run 4	9
	  PlayFx	FEET\PPD_Left.fx 1
	  PlayFx	FEET\PPD_Right.fx 7
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		ResistanceMale
	  Anim		male\comb_run 4	9
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx 7
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		ResistanceFem
	  Anim		male\comb_run 4	9
	  PlayFx	FEET\RES_Left.fx 1
	  PlayFx	FEET\RES_Right.fx 7
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\run 13 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		blackscorpion
	  Anim		huge\BS_run 13 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
	TEnd

	Type		fem
	  Anim		fem\fem_run 20 33
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim		Beast\run 18
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT 24
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\comb_run 4	9
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	 # PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\fem_run 20 33
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\fem_run 20 33
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\fem_run 20 33
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\fem_run 20 33
	TEnd

	Type		Abomination
	  Anim		huge\run 13 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

MEnd

Move C_RunCycle
	Requires	FORWARD	COMBAT
	Priority	2
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	C_RunPost

	Type		male
	  Anim		male\comb_run 10 34
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		PraetorianPolice
	  Anim		male\comb_run 10 34
	  PlayFx	FEET\PPD_Left.fx  10
	  PlayFx	FEET\PPD_Right.fx	 22
	  SmoothSprint	3.3
	TEnd

	Type		ResistanceMale
	  Anim		male\comb_run 10 34
	  PlayFx	FEET\RES_Left.fx  10
	  PlayFx	FEET\RES_Right.fx	 22
	  SmoothSprint	3.3
	TEnd

	Type		ResistanceFem
	  Anim		male\comb_run 10 34
	  PlayFx	FEET\RES_Left.fx  10
	  PlayFx	FEET\RES_Right.fx	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		blackscorpion
	  Anim		huge\BS_run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\fem_run 10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim		Beast\run 1 25
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\comb_run 10 34
	 #PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\fem_run 10 33
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		Abomination
	  Anim		huge\run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd


MEnd

#############################################################

Move C_RunPost
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Flags		ReqInputs

	Type		male
	  Anim		male\comb_run 39
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		PraetorianPolice
	  Anim		male\comb_run 39
	  PlayFx	FEET\PPD_Land1.fx  3
	  PlayFx	FEET\PPD_Right.fx 12
	  scale		0.7
	TEnd

	Type		ResistanceMale
	  Anim		male\comb_run 39
	  PlayFx	FEET\RES_Land1.fx  3
	  PlayFx	FEET\RES_Right.fx 12
	  scale		0.7
	TEnd

	Type		ResistanceFem
	  Anim		male\comb_run 39
	  PlayFx	FEET\RES_Land1.fx  3
	  PlayFx	FEET\RES_Right.fx 12
	  scale		0.7
	TEnd

	Type		huge
	  Anim		huge\combat_postrun 2
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  scale		0.5
	TEnd

	Type		fem
	  Anim		fem\combat_postrun 1
	  PlayFx	FEET\LAND4.FX:LAND  3
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_postrun 4 28
	  scale		0.5
	TEnd

	Type		RockSuit
	  Anim		huge\combat_postrun 2
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT  2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	3
	  scale		0.5
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\comb_run 39
	 # PlayFx	FEET\LAND3.FX:LAND  3
	 # PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\combat_postrun 1
#	  PlayFx	FEET\LAND4.FX:LAND  3
  	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\combat_postrun 1
#	  PlayFx	FEET\LAND4.FX:LAND  3
  	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\combat_postrun 1
#	  PlayFx	FEET\LAND4.FX:LAND  3
  	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\combat_postrun 1
#	  PlayFx	FEET\LAND4.FX:LAND  3
  	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  scale		0.7
	TEnd

	Type		Abomination
	  Anim		huge\combat_postrun 2
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  scale		0.5
	TEnd


MEnd

## COMBAT RUN BACKWARD ###########################################################

Move C_RunbackPre
	Requires	BACKWARD COMBAT
	Priority	2
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove>
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	C_RunbackCycle
	Flags		ReqInputs

	Type		male
	  Anim		male\runback 5 9
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\runback 3 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 6
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 18
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		RockSuit
	  Anim		beast\runback 18 24
	TEnd
MEnd

Move C_RunbackCycle
	Requires	BACKWARD COMBAT
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	C_RunPost

	Type		male
	  Anim		male\runback 10	33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\runback 1 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd

	Type		RockSuit
	  Anim		beast\runback 1	24
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	6
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 18
	TEnd
MEnd

## COMBAT RUN LEFT ###########################################################

Move C_RunLeftPre
	Requires	STEPLEFT COMBAT
	Priority	2
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> C_RunPre
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	C_RunLeftCycle
	MoveRate	0.7
	Flags		ReqInputs MoveScale SmoothSprint

	Type		male
	  Anim		male\runleft 24
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 6
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_L 12 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move C_RunLeftCycle
	Requires	STEPLEFT COMBAT
	Priority	2
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> C_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	C_RunPost

	Type		male
	  Anim		male\runleft 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 1	25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_L 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


## COMBAT RUN RIGHT ###########################################################

Move C_RunRightPre
	Requires	STEPRIGHT COMBAT
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> C_RunPre
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	C_RunRightCycle
	Flags		ReqInputs MoveScale SmoothSprint
	Priority	2
	MoveRate	0.7

	Type		male
	  Anim		male\runright 24
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 6
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_R 12 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move C_RunRightCycle
	Requires	STEPRIGHT COMBAT
	Priority	2
	Interrupts	<Ready> <Combat> <MoveIRQ> <BasicMove> C_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <CombatMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	C_RunPost

	Type		male
	  Anim		male\runright 1	25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_R 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


#############################################################
## WEAPON MODE
#############################################################

## WEAPON RUN FORWARD ###########################################################

Move W_RunPre
	Requires	FORWARD	COMBAT WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	W_RunCycle
	MoveRate	0.7

	Type		male
	  Anim		male\weprun 4 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_run 15	25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 6
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim		Beast\run 18
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 24
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\weprun 4 10
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  SmoothSprint	3.3
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd
MEnd

Move W_RunCycle
	Requires	FORWARD	COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprun 10 34
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim		Beast\run 1 25
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 12
	  MoveRate	0.7  ##	0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\weprun 10 34
	 # PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\wep_run 1 25
	 # PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	TEnd

MEnd

Move W_RunPost
	Requires	COMBAT WEAPON
	Include sequencers\Members\combat_weapon.inc
	Flags		ReqInputs
	NextMove	WepReady

	Type		male
	  Anim		male\weprun 39
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim		huge\wep_postrun 3
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.5
	TEnd

	Type		fem
	  Anim		fem\wep_postrun	2
	  PlayFx	FEET\LAND4.FX:LAND  3
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.5
	TEnd

	Type		Vahzilok
	  Anim		Vahzilok\vah_postrun 4 28
	TEnd

	Type		RockSuit
	  Anim		huge\wep_postrun 3
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 3
	  Scale		0.5
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\weprun 39
	  #PlayFx	FEET\LAND3.FX:LAND  3
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\wep_postrun	2
	 # PlayFx	FEET\LAND4.FX:LAND  3
  	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.5
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wep_postrun	2
	 # PlayFx	FEET\LAND4.FX:LAND  3
  	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.5
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\wep_postrun	2
	 # PlayFx	FEET\LAND4.FX:LAND  3
  	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.5
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim		fem\wep_postrun	2
	 # PlayFx	FEET\LAND4.FX:LAND  3
  	  #PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.5
	TEnd

MEnd

## WEAPON RUN BACKWARD ###########################################################

Move W_RunbackPre
	Requires	BACKWARD COMBAT	WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> W_RunPre
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	W_RunbackCycle
	Interpolate	5

	Type		male
	  Anim		male\weprunback	5 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\wep_runback	1 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		RockSuit
	  Anim		huge\runback 20	24
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 21
	TEnd
MEnd

Move W_RunbackCycle
	Requires	BACKWARD COMBAT	WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> W_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprunback	10 33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_runback	1 24
	PlayFx	      FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	PlayFx	      FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	 TEnd

	Type		RockSuit
	  Anim		huge\runback 1 24
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 10
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 22
	TEnd
MEnd


## WEAPON RUN LEFT ###########################################################

Move W_RunLeftPre
	Requires	STEPLEFT COMBAT	WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	W_RunPre
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	W_RunLeftCycle
	MoveRate	0.7
	Interpolate	5

	Type		male
	  Anim		male\weprunleft	15 25
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runleft 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_L 15 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move W_RunLeftCycle
	Requires	STEPLEFT COMBAT	WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	W_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprunleft	1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runleft 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_L 1	25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


## WEAPON RUN RIGHT ###########################################################

Move W_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	W_RunPre
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	W_RunRightCycle
	MoveRate	0.7
	Interpolate	5

	Type		male
	  Anim		male\weprunright 15 25
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runright 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_R 15 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move W_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	W_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprunright 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runright 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_R 1	25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


#############################################################
## DUAL WIELD MODE
#############################################################

## DUAL RUN FORWARD ###########################################################

Move DualWield_RunPre
	Requires	FORWARD	COMBAT WEAPON DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <BasicMove> <CombatMove> <WepMove> #<MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_RunCycle

	Type		male
	  Anim		male\dual_run 12 25
	  PlayFx	FEET\MALERIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\dual_run 12 25
	  PlayFx	FEET\FEMRIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		male\dual_run 12 25
	  PlayFx	FEET\HUGERIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move DualWield_RunCycle
	Requires	FORWARD	COMBAT WEAPON DUAL
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\dual_run 1	25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\dual_run 1	25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\dual_run 1	25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd

#############################################################

Move DualWield_RunPost
	Requires	COMBAT WEAPON DUAL
	Priority	3
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		fem
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\LAND4.FX:LAND  3
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.7
	TEnd
MEnd

## DUAL RUN BACK ###########################################################

Move DualWield_RunbackPre
	Requires	BACKWARD COMBAT	WEAPON DUAL
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	DualWield_RunbackCycle
	Priority	4

	Type		male
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\MALERIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\FEMRIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\HUGERIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move DualWield_RunbackCycle
	Requires	BACKWARD COMBAT	WEAPON DUAL
	Interrupts	<BasicMove> <CombatMove> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\Dual_run_back 1 24
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd

## DUAL RUN LEFT ###########################################################

Move DualWield_RunLeftPre
	Requires	STEPLEFT COMBAT	WEAPON DUAL
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> DualWield_RunPre
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	DualWield_RunLeftCycle
	Priority	4
	MoveRate	0.7

	Type		male
	  Anim		male\Dual_ss_L 24

	  SmoothSprint	3.3
	TEnd
MEnd

Move DualWield_RunLeftCycle
	Requires	STEPLEFT COMBAT	WEAPON DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> DualWield_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd

## DUAL RUN RIGHT ###########################################################

Move DualWield_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON	DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> DualWield_RunPre
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	DualWield_RunRightCycle
	MoveRate	0.7

	Type		male
	  Anim		male\Dual_ss_R 24
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd
MEnd

Move DualWield_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON	DUAL
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> DualWield_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd


#############################################################
## CLAWS MODE
#############################################################

## CLAWS FORWARD ###########################################################

Move Claw_RunPre
	Requires	FORWARD	COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove>
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	Claw_RunCycle

	Type		male
	  Anim		male\comb_run 4	9
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\run 13 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 23
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\fem_run 20 33
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move Claw_RunCycle
	Requires	FORWARD	COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Claw_RunPost

	Type		male
	  Anim		male\comb_run 10 34
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\fem_run 10 33
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd

#############################################################

Move Claw_RunPost
	Requires	COMBAT WEAPON DUAL CLAW
	Priority	4
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		ReqInputs
	NextMove	Claw_PostBlend

	Type		male
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		fem
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\LAND4.FX:LAND  3
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim		male\dual_post_run 1
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.7
	TEnd
MEnd

## CLAWS BACKWARD ###########################################################

Move Claw_RunbackPre
	Requires	BACKWARD COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove>
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	Claw_RunbackCycle
	Flags		ReqInputs

	Type		male
	  Anim		male\runback 5 9
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	TEnd

	Type		fem
	  Anim		fem\runback 3 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  6
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	18
	TEnd
MEnd

Move Claw_RunbackCycle
	Requires	BACKWARD COMBAT WEAPON DUAL CLAW
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	Claw_RunPost

	Type		male
	  Anim		male\runback 10	33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\runback 1 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


## CLAWS LEFT ###########################################################

Move Claw_RunLeftPre
	Requires	STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Claw_RunPre
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	Claw_RunLeftCycle
	MoveRate	0.7
	Flags		ReqInputs MoveScale SmoothSprint

	Type		male
	  Anim		male\runleft 24
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 6
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_L 12 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  22
	  SmoothSprint	3.2
	TEnd
MEnd

Move Claw_RunLeftCycle
	Requires	STEPLEFT COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Claw_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Claw_RunPost

	Type		male
	  Anim		male\runleft 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_left 1	25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_L 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd


## CLAWS RIGHT ###########################################################

Move Claw_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Claw_RunPre
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	Claw_RunRightCycle
	Flags		ReqInputs MoveScale SmoothSprint
	MoveRate	0.7

	Type		male
	  Anim		male\runright 24
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 6
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_R 12 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	10
	  SmoothSprint	3.2
	TEnd
MEnd

Move Claw_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON DUAL CLAW
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Claw_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <ClawMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Claw_RunPost

	Type		male
	  Anim		male\runright 1	25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\run_right 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\run_R 1 25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd

#############################################################
## LEFTHAND MODE
#############################################################

## REV RUN FORWARD ###########################################################

Move Lefthand_Run_Pre
	Requires	FORWARD COMBAT WEAPON DUAL REV
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove>
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove        Lefthand_Run_Cycle
	MoveRate        0.7

	Type		male
	  Anim          male\bow_run 12 25
	  PlayFx        FEET\MALERIGHT.FX 10
	  PlayFx        FEET\MALERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\fem_run 20 33
	  #Anim          fem\bow_run 12 25
	  PlayFx        FEET\FEMRIGHT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          male\bow_run 12 25
	  PlayFx        FEET\HUGERIGHT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd
MEnd

Move Lefthand_Run_Cycle
	Requires	FORWARD COMBAT WEAPON DUAL REV
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove        DualWield_RunPost
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7

	Type		male
	  Anim          male\bow_run 1 25
	  PlayFx        FEET\MALELEFT.FX 10
	  PlayFx        FEET\MALERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  #Anim          fem\bow_run 1 25
	  Anim          fem\fem_run 10 33
	  PlayFx        FEET\FEMLEFT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          male\bow_run 1 25
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd
MEnd

#############################################################

Move Lefthand_RunLeft_Pre
	Requires	STEPLEFT COMBAT	WEAPON DUAL REV
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Lefthand_Run_Pre
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	Lefthand_RunLeft_Cycle
	MoveRate	0.7

	Type		male
	  Anim		male\Dual_ss_L 24
	  SmoothSprint	3.3
	TEnd
MEnd

Move Lefthand_RunLeft_Cycle
	Requires	STEPLEFT COMBAT	WEAPON DUAL REV
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Lefthand_Run_Cycle
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\Dual_ss_L 1 25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd

#############################################################

Move Lefthand_RunRight_Pre
	Requires	STEPRIGHT COMBAT WEAPON	DUAL REV
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Lefthand_Run_Pre
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	Lefthand_RunRight_Cycle
	MoveRate	0.7

	Type		male
	  Anim		male\Dual_ss_R 24
	  SmoothSprint	3.3
	TEnd
MEnd

Move Lefthand_RunRight_Cycle
	Requires	STEPRIGHT COMBAT WEAPON	DUAL REV
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> Lefthand_Run_Cycle
	Member		<DeathIRQ> <AttackIRQ> <LeftHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\Dual_ss_R 1 25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  22
	  SmoothSprint	3.3
	TEnd
MEnd


#############################################################
## DUAL PISTOLS
#############################################################

## RUN ###########################################################

Move DualWield_Guns_RunPre
	Requires	FORWARD COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> #<MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Guns_RunCycle

	Type		male
	  Anim          male\comb_run 4 9
	  PlayFx        FEET\MALERIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\run 13 25
	  PlayFx        FEET\HUGERIGHT.FX  23
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\fem_run 20 33
	  PlayFx        FEET\FEMRIGHT.FX  10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim          Beast\run 18
	  PlayFx        FEET\giantfoot2.FX 24
	TEnd
MEnd

Move DualWield_Guns_RunCycle
	Requires	FORWARD COMBAT WEAPON DUAL PISTOLS
	Member		<DeathIRQ> <AttackIRQ> <BasicMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <SideMoveIRQ>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim          male\comb_run 10 34
	  PlayFx        FEET\MALELEFT.FX  10
	  PlayFx        FEET\MALERIGHT.FX  22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\run 1 25
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\fem_run 10 33
	  PlayFx        FEET\FEMLEFT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.2
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_run 1 25
	TEnd

	Type		RockSuit
	  Anim          Beast\run 1 25
	  PlayFx        FEET\giantfoot1.FX
	  PlayFx        FEET\giantfoot2.FX 10
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	TEnd
MEnd

#############################################################

Move DualWield_Guns_RunbackPre
	Requires	BACKWARD COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove>
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	DualWield_Guns_RunbackCycle
	Flags		ReqInputs

	Type		male
	  Anim          male\runback 5 9
	  PlayFx        FEET\MALERIGHT.FX 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\runback 1 24
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\runback 3 24
	  PlayFx        FEET\FEMLEFT.FX 6
	  PlayFx        FEET\FEMRIGHT.FX 18
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		RockSuit
	  Anim          beast\runback 18 24
	TEnd
MEnd

Move DualWield_Guns_RunbackCycle
	Requires	BACKWARD COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.525
	NextMove	DualWield_RunPost

	Type		male
	  Anim          male\runback 10 33
	  PlayFx        FEET\MALELEFT.FX 10
	  PlayFx        FEET\MALERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\runback 1 24
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\runback 1 24
	  PlayFx        FEET\FEMLEFT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.2
	TEnd

	Type		RockSuit
	  Anim          beast\runback 1 24
	  PlayFx        FEET\giantfoot1.FX 6
	  PlayFx        FEET\giantfoot2.FX 18
	TEnd
MEnd

#############################################################

Move DualWield_Guns_RunLeftPre
	Requires	STEPLEFT COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> DualWield_Guns_RunPre
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	DualWield_Guns_RunLeftCycle
	MoveRate        0.7
	Flags		ReqInputs MoveScale SmoothSprint

	Type		male
	  Anim          male\runleft 24
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\run_left 15 25
	  PlayFx        FEET\HUGERIGHT.FX 6
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\run_L 12 25
	  PlayFx        FEET\FEMRIGHT.FX 10
	  PlayFx        FEET\FEMLEFT.FX 22
	  SmoothSprint  3.2
	TEnd
MEnd

Move DualWield_Guns_RunLeftCycle
	Requires	STEPLEFT COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> DualWield_Guns_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim          male\runleft 1 25
	  PlayFx        FEET\MALELEFT.FX 10
	  PlayFx        FEET\MALERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\run_left 1 25
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\run_L 1 25
	  PlayFx        FEET\FEMLEFT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.2
	TEnd
MEnd

#############################################################

Move DualWield_Guns_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> DualWield_Guns_RunPre
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	NextMove	DualWield_Guns_RunRightCycle
	Flags		ReqInputs MoveScale SmoothSprint
	MoveRate        0.7

	Type		male
	  Anim          male\runright 24
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\run_right 15 25
	  PlayFx        FEET\HUGERIGHT.FX 6
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\run_R 12 25
	  PlayFx        FEET\FEMRIGHT.FX 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd
MEnd

Move DualWield_Guns_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON DUAL PISTOLS
	Priority        5
	Interrupts	<Ready> <Combat> <WepCombat> <DualHandCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> DualWield_Guns_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <DualPistolsMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove	DualWield_RunPost

	Type		male
	  Anim          male\runright 1 25
	  PlayFx        FEET\MALELEFT.FX 10
	  PlayFx        FEET\MALERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\run_right 1 25
	  PlayFx        FEET\HUGELEFT.FX 10
	  PlayFx        FEET\HUGERIGHT.FX 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\run_R 1 25
	  PlayFx        FEET\FEMLEFT.FX 10
	  PlayFx        FEET\FEMRIGHT.FX 22
	  SmoothSprint  3.2
	TEnd
MEnd


#############################################################
## "SHOTGUN" MODE
#############################################################

## SHOTGUN FORWARD ###########################################################

Move Sg_RunPre
	Requires	FORWARD COMBAT WEAPON GUN
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs FixArmReg
	NextMove        Sg_RunCycle

	Type		male
	  Anim          male\shotgun_run 4 9
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\shotgun_run 15 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\shotgun_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move Sg_RunCycle
	Requires	FORWARD COMBAT WEAPON GUN
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        Sg_RunPost

	Type		male
	  Anim          male\shotgun_run 10 34
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_run 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd

#############################################################

Move Sg_RunPost
	Requires	COMBAT WEAPON GUN
	Interrupts	W_RunPost W_RunbackPost
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		ReqInputs FixArmReg
	NextMove        ShotgunReady

	Type		male
	  Anim          male\rocket_postrun 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim          huge\rocket_postrun 1
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.7
	TEnd

	Type		fem
	  Anim          fem\rocket_postrun 1
	  Scale		0.7
	TEnd
MEnd
##############################################################
#
#Move SG_RunbackPre
#	Requires	BACKWARD COMBAT	WEAPON GUN
#	Priority	4
#	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
#	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
#	Flags		ReqInputs
#	NextMove	SG_RunbackCycle
#	Interpolate	5
#
#	Type		male
#	  Anim		male\runback 1 24
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#
#	Type		huge
#	  Anim		huge\runback 1 24
#	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_runback	1 24
#	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
#	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
#	  PlayFx	Feet\StartScuff.fx 1
#	TEnd
#
#	Type		RockSuit
#	  Anim		huge\runback 20	24
#	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 21
#	TEnd
#MEnd
#
#Move SG_RunbackCycle
#	Requires	BACKWARD COMBAT	WEAPON GUN
#	Priority	4
#	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> Sg_RunPre
#	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
#	Flags		Cycle MoveScale	SmoothSprint
#	MoveRate	0.525
#	NextMove	Sg_RunPost
#
#	Type		male
#	  Anim		male\runback 1 24
#	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
#	  SmoothSprint	3.3
#	TEnd
#
#	Type		huge
#	  Anim		huge\runback 1 24
#	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
#	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
#	  SmoothSprint	3.3
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_runback	1 24
#	PlayFx	      FEET\FEMLEFT.FX:FOOTFALLLEFT  10
#	PlayFx	      FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
#	  SmoothSprint	3.2
#	 TEnd
#
#	Type		RockSuit
#	  Anim		huge\runback 1 24
#	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 10
#	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 22
#	TEnd
#MEnd

###################
### Shotgun run Left  ####
###################

Move SG_RunLeftPre
	Requires	STEPLEFT COMBAT WEAPON GUN
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> Sg_RunPre
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        SG_RunLeftCycle
	MoveRate        0.7

	Type		male
	  Anim          male\shotgun_runleft 15 25
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runleft 15 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runleft 15 25
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd
MEnd

Move SG_RunLeftCycle
	Requires	STEPLEFT COMBAT WEAPON GUN
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> Sg_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        Sg_RunPost

	Type		male
	  Anim          male\shotgun_runleft 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runleft 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runleft 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd


###################
### Shotgun run Right ####
###################

Move SG_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON GUN
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> Sg_RunPre
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        SG_RunRightCycle
	MoveRate        0.7

	Type		male
	  Anim          male\shotgun_runright 3 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runright 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runright 3 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd
MEnd

Move SG_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON GUN
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> Sg_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        Sg_RunPost

	Type		male
	  Anim          male\shotgun_runright 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runright 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runright 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd

#############################################################
## TWO HANDED CLUB
#############################################################

## 2HCLUB RUN FORWARD ###########################################################

Move 2HCLUB_RunPre
	Requires	FORWARD COMBAT WEAPON CLUB
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove        2HCLUB_Run_Cycle

	Type		male
	  Anim          male\2hclub_run 15 25
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\wep_run 15 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\wep_run 15 25
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move 2HCLUB_Run_Cycle
	Requires	FORWARD COMBAT WEAPON CLUB
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        W_RunPost

	Type		male
	  Anim          male\2hclub_run 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\wep_run 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\wep_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd

## 2HCLUB RUN LEFT ###########################################################

Move 2HCLUB_RunLeft_Pre
	Requires	STEPLEFT COMBAT	WEAPON CLUB
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	2HCLUB_RunPre
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	2HCLUB_RunLeft_Cycle
	MoveRate	0.7
	Interpolate	5

	Type		male
	  Anim		male\weprunleft	15 25
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runleft 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_L 15 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move 2HCLUB_RunLeft_Cycle
	Requires	STEPLEFT COMBAT	WEAPON CLUB
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> 2HCLUB_Run_Cycle
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprunleft	1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runleft 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_L 1	25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd

## 2HCLUB RUN RIGHT ###########################################################

Move 2HCLUB_RunRight_Pre
	Requires	STEPRIGHT COMBAT WEAPON CLUB
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> 2hCLUB_RunPre
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	2HCLUB_RunRight_Cycle
	MoveRate	0.7
	Interpolate	5

	Type		male
	  Anim		male\weprunright 15 25
	  SmoothSprint	3.3
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\wep_runright 15 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_R 15 25
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint	3.2
	TEnd
MEnd

Move 2HCLUB_RunRight_Cycle
	Requires	STEPRIGHT COMBAT WEAPON CLUB
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove>	2HCLUB_Run_Cycle
	Member		<DeathIRQ> <AttackIRQ> <ClubMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	W_RunPost

	Type		male
	  Anim		male\weprunright 1 25
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\wep_runright 1 25
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_run_R 1	25
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT	22
	  SmoothSprint	3.2
	TEnd
MEnd

## 2hlarge FORWARD ###########################################################

Move 2hlarge_RunPre
	Requires	FORWARD COMBAT WEAPON 2HLARGE
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs FixArmReg
	NextMove        2hlarge_RunCycle

	Type		male
	  Anim          male\2hlarge_run 1 25
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run 1 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run 1 25
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

MEnd

Move 2hlarge_RunCycle
	Requires	FORWARD COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <TwoHandRunCycle>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        2hlarge_RunPost

	Type		male
	  Anim          male\2hlarge_run 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd

MEnd

#####2hlarge ########################################################

Move 2hlarge_RunPost
	Requires	COMBAT WEAPON 2HLARGE
	Interrupts	W_RunPost W_RunbackPost
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		ReqInputs FixArmReg
	NextMove        2hlargeReady
	Interpolate	20

	Type		male
	  Anim          male\2hlarge_runpost 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim          huge\2hlarge_runpost 1
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.7
	TEnd

	Type		fem
	  Anim          fem\2hlarge_runpost 1
	  Scale		0.7
	TEnd

MEnd
#############################################################

Move 2hlarge_RunbackPre
	Requires	BACKWARD COMBAT	WEAPON 2HLARGE
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL> <TwoHandRun>
	Flags		ReqInputs
	NextMove	2hlarge_RunbackCycle
	Interpolate	5

	Type		male
	  Anim		male\2hlarge_run_backward 1 24
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\2hlarge_run_backward 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\2hlarge_run_backward 1 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

MEnd

Move 2hlarge_RunbackCycle
	Requires	BACKWARD COMBAT	WEAPON 2HLARGE
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandRun>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	2hlarge_RunPost

	Type		male
	  Anim		male\2hlarge_run_backward 1 24
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\2hlarge_run_backward 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\2hlarge_run_backward	1 24
       		PlayFx	      FEET\FEMLEFT.FX:FOOTFALLLEFT  10
 		PlayFx	      FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	 TEnd

MEnd

###################
### 2hlarge run Left  ####
###################

Move 2hlarge_RunLeftPre
	Requires	STEPLEFT COMBAT WEAPON 2HLARGE
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandRun>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        2hlarge_RunLeftCycle
	MoveRate        0.7

	Type		male
	  Anim          male\2hlarge_run_left 1 25
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run_left 1 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run_left 1 25
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd

MEnd

Move 2hlarge_RunLeftCycle
	Requires	STEPLEFT COMBAT WEAPON 2HLARGE
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandRunCycle>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        2hlarge_RunPost

	Type		male
	  Anim          male\2hlarge_run_left 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run_left 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run_left 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd

MEnd


###################
### 2hlarge run Right ####
###################

Move 2hlarge_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON 2HLARGE
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandRun>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        2hlarge_RunRightCycle
	MoveRate        0.7

	Type		male
	  Anim          male\2hlarge_run_right 3 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run_right 3 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run_right 3 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd

MEnd

Move 2hlarge_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON 2HLARGE
	Priority        4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandRunCycle>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        2hlarge_RunPost

	Type		male
	  Anim          male\2hlarge_run_right 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\2hlarge_run_right 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\2hlarge_run_right 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd

MEnd

#############################################################
## "BEAMRIFLE" MODE
#############################################################

## BEAM RIFLE FORWARD ###########################################################

Move BeamRifle_RunPre
	Requires	FORWARD COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Priority        6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove>
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs FixArmReg
	NextMove        BeamRifle_RunCycle

	Type		male
	  Anim          male\shotgun_run 4 9
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim          huge\shotgun_run 15 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim          fem\shotgun_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd
MEnd

Move BeamRifle_RunCycle
	Requires	FORWARD COMBAT WEAPON BEAMRIFLE TWOHAND GUN
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        BeamRifle_RunPost

	Type		male
	  Anim          male\shotgun_run 10 34
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_run 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_run 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd

#############################################################

Move BeamRifle_RunPost
	Requires	COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Interrupts	BeamRifle_RunPost BeamRifle_RunbackPost
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		ReqInputs FixArmReg
	NextMove        BeamRifle_Ready

	Type		male
	  Anim          male\rocket_postrun 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  Scale		0.7
	TEnd

	Type		huge
	  Anim          huge\rocket_postrun 1
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	  Scale		0.7
	TEnd

	Type		fem
	  Anim          fem\rocket_postrun 1
	  Scale		0.7
	TEnd
MEnd

###################
### BeamRifle run Left  ####
###################

Move BeamRifle_RunLeftPre
	Requires	STEPLEFT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority        6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> BeamRifle_RunPre
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        BeamRifle_RunLeftCycle
	MoveRate        0.7

	Type		male
	  Anim          male\shotgun_runleft 15 25
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runleft 15 25
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runleft 15 25
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 21
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd
MEnd

Move BeamRifle_RunLeftCycle
	Requires	STEPLEFT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority        6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> BeamRifle_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        BeamRifle_RunPost

	Type		male
	  Anim          male\shotgun_runleft 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runleft 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runleft 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd


###################
### BeamRifle run Right ####
###################

Move BeamRifle_RunRightPre
	Requires	STEPRIGHT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority        6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> BeamRifle_RunPre
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint FixArmReg
	NextMove        BeamRifle_RunRightCycle
	MoveRate        0.7

	Type		male
	  Anim          male\shotgun_runright 3 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runright 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runright 3 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	  SmoothSprint  3.2
	TEnd
MEnd

Move BeamRifle_RunRightCycle
	Requires	STEPRIGHT COMBAT WEAPON GUN BEAMRIFLE TWOHAND
	Priority        6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> BeamRifle_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <TwoHandMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale SmoothSprint FixArmReg
	MoveRate        0.7
	NextMove        BeamRifle_RunPost

	Type		male
	  Anim          male\shotgun_runright 1 25
	  PlayFx        FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\MALERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_runright 1 25
	  PlayFx        FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.3
	TEnd

	Type		fem
	  Anim          fem\shotgun_runright 1 25
	  PlayFx        FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	  PlayFx        FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint  3.2
	TEnd
MEnd

## WEAPON RUN BACKWARD ###########################################################

Move BeamRifle_RunbackPre
	Requires	BACKWARD COMBAT	WEAPON GUN BEAMRIFLE TWOHAND
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> BeamRifle_RunPre
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	BeamRifle_RunbackCycle
	Interpolate	5

	Type		male
	  Anim		male\weprunback	5 10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 10
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		fem
	  Anim		fem\wep_runback	1 24
	  PlayFx	FEET\FEMLEFT.FX:FOOTFALLLEFT 10
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  PlayFx	Feet\StartScuff.fx 1
	TEnd

	Type		RockSuit
	  Anim		huge\runback 20	24
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 21
	TEnd
MEnd

Move BeamRifle_RunbackCycle
	Requires	BACKWARD COMBAT	WEAPON GUN BEAMRIFLE TWOHAND
	Priority	6
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> BeamRifle_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <WepMove> <IRQJumps> <IRQSwim> <IRQSlide> <RunsALL> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.525
	NextMove	BeamRifle_RunPost

	Type		male
	  Anim		male\weprunback	10 33
	  PlayFx	FEET\MALELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		huge\runback 1 24
	  PlayFx	FEET\HUGELEFT.FX:FOOTFALLLEFT  10
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	 22
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\wep_runback	1 24
	PlayFx	      FEET\FEMLEFT.FX:FOOTFALLLEFT  10
	PlayFx	      FEET\FEMRIGHT.FX:FOOTFALLRIGHT 22
	  SmoothSprint	3.2
	 TEnd

	Type		RockSuit
	  Anim		huge\runback 1 24
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 10
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 22
	TEnd
MEnd

#############################################################

Include sequencers\player\Motion\motion_doors.inc
Include sequencers\player\Motion\motion_fall.inc
Include sequencers\player\Motion\motion_fly.inc
Include sequencers\player\Motion\motion_hoverboard.inc
Include sequencers\player\Motion\motion_magiccarpet.inc
Include sequencers\player\Motion\motion_jump.inc
Include sequencers\player\Motion\motion_slide.inc
Include sequencers\player\Motion\motion_walk.inc
Include sequencers\player\Motion\motion_stunned.inc
Include sequencers\player\Shields\Shields_Motion.inc
Include sequencers\player\Motion\motion_jump_PrestigeJumpJetPack.inc
Include sequencers\player\Motion\motion_jump_SteamJumpJetPack.inc
Include sequencers\player\Motion\motion_ninja.inc
Include sequencers\player\Motion\motion_beast.inc
#Include sequencers\player\Motion\motion_parkour.inc
