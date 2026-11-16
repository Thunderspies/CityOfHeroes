#############################################################
## motion.inc
#############################################################

###################
### Run Forward ##
###################

Move RunPre
	Requires	FORWARD
	Priority	1
	Interrupts	<Ready> <PostMoveCycle> <Combat> <CombatAttack> <MoveIRQ>
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs  MoveScale ##SmoothSprint
	NextMove        RunCycle
	Scale		1.2

	Type		Beast
	  Anim          Beast\run 3 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		CameraShaker
	  Anim          Beast\run 3 25
	  PlayFx        FEET\giantright2.FX  13
	  PlayFx        EnemyFx\BanishedPantheon\GiantEnemyTremorsRight.fx  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Behemoth
	  Anim          Beast\run 3 25
	  PlayFx        FEET\giantfoot2.FX 6
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Crystal
	  Anim          male\monster_run 3 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Natterling
	  Anim          Beast\run 3 25
	  PlayFx        FEET\NatterRight.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Shivan
	  Anim          male\goblin_run 1
	  PlayFx        FEET\Shivan_Left.FX  10
	  PlayFx        FEET\Shivan_Right.FX  21
	  Scale		1.2
	  MoveRate      0.2
	TEnd
MEnd

Move RunCycle
	Requires	FORWARD
	Priority	1
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale ##SmoothSprint
	NextMove        RunPost
	Scale		1.2

	Type		Beast
	  Anim          Beast\run 1 25
	  PlayFx        FEET\HUGELEFT2.FX  1
	  PlayFx        FEET\HUGERIGHT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		CameraShaker
	  Anim          Beast\run 1 25
	  PlayFx        FEET\giantleft2.FX  1
	  PlayFx        FEET\giantright2.FX  13
	  PlayFx        EnemyFx\BanishedPantheon\GiantEnemyTremorsLeft.fx  1
	  PlayFx        EnemyFx\BanishedPantheon\GiantEnemyTremorsRight.fx  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Behemoth
	  Anim          Beast\run 1 25
	  PlayFx        FEET\giantfoot1.FX
	  PlayFx        FEET\giantfoot2.FX  10
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Crystal
	  Anim          male\monster_run 1 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		natterling
	  Anim          Beast\run 1 25
	  PlayFx        FEET\natterleft.FX  1
	  PlayFx        FEET\natterright.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Shivan
	  Anim          male\goblin_run 1
	  PlayFX		ENEMYFX\Unique\Shivan\Shivan_BreathRun_SFX.fx
	  PlayFx        FEET\Shivan_Left.FX  10
	  PlayFx        FEET\Shivan_Right.FX  21
	  Scale		1.2
	  MoveRate      0.2
	TEnd
MEnd

Move RunPost
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Interpolate     8

	Type		Beast
	  Anim          Beast\postrun 3 32
	  PlayFx        FEET\LAND1.FX	1
	TEnd

	Type		CameraShaker
	  Anim          Beast\postrun 3 32
	  PlayFx        FEET\giantland3.FX  7
	  PlayFx        EnemyFx\BanishedPantheon\GiantEnemyTremorsRight.fx
	TEnd

	Type		Behemoth
	  Anim          Beast\postrun 3 32
	  PlayFx        FEET\giantland4.FX  7
	TEnd

	Type		Crystal
	  Anim          male\MonComb_postwalk 5
	  Scale		2
	TEnd

	Type		Natterling
	  Anim          Beast\postrun 3 32
	  PlayFx        FEET\natterland.FX	1
	TEnd

	Type		Shivan
	  Anim          Beast\postrun 3 32
	  PlayFx        FEET\Shivan_Left.FX  1
	  PlayFx        FEET\Shivan_Right.FX  6
	TEnd

MEnd

###################
## run Backward ##
###################

Move BackRunPre
	Requires	BACKWARD
	Priority	1
	Interrupts	<Ready> <PostMoveCycle> <Combat> <CombatAttack> <Block> <HitIRQ> <MoveIRQ>
	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs  MoveScale
	NextMove        BackRunCycle
	Scale		1.5

	Type		beast
	  Anim          beast\runback 18 24
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		Behemoth
	  Anim          beast\runback 18 24
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	TEnd
MEnd

Move BackRunCycle
	Requires	BACKWARD
	Priority	1
	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale
	NextMove        RunPost
	Scale		1.5

	Type		beast
	  Anim          beast\runback 1 24
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	TEnd

	Type		Behemoth
	  Anim          beast\runback 1 24
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  PlayFx        FEET\giantfoot1.FX 6
	  PlayFx        FEET\giantfoot2.FX 19
	TEnd
MEnd

## RUN LEFT ###########################################################
##
##Move LeftPre
##	Requires	STEPLEFT
##	Interrupts	<Ready> <Combat> <CombatAttack> <PostMoveCycle> RunCycle <HitIRQ> <Block> <MoveIRQ>
##	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs MoveScale
##	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
##	NextMove        LeftCycle
##	Priority	1
##
##	Type		beast
##	  Anim          beast\run_L 3 25
##	TEnd
##MEnd
##
##Move LeftCycle
##	Requires	STEPLEFT
##	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
##	Flags		Cycle MoveScale
##	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
##	NextMove        RunPost
##	Priority	1
##
##	Type		beast
##	  Anim          beast\run_L 1 25
##	TEnd
##MEnd
##
### RUN RIGHT ###########################################################
##
##Move RightPre
##	Requires	STEPRIGHT
##	Interrupts	<Ready> <Combat> <CombatAttack> <PostMoveCycle> RunCycle <HitIRQ> <Block> <MoveIRQ>
##	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs MoveScale
##	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
##	NextMove        RightCycle
##	Priority	1
##
##
##	Type		beast
##	  Anim          beast\run_R 3 25
##	TEnd
##
##MEnd
##
##
##Move RightCycle
##	Requires	STEPRIGHT
##	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
##	Flags		Cycle MoveScale
##	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
##	NextMove        RunPost
##	Priority	1
##
##
##	Type		beast
##	  Anim          beast\run_R 1 25
##	TEnd
##
##MEnd

## WALK FORWARD ###########################################################

Move IdleWalkPre
	Requires	WALK FORWARD
	Priority	2
	Interrupts	<Ready> <PostMoveCycle> <Combat> <CombatAttack> <MoveIRQ>
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs  MoveScale SmoothSprint AlwaysSizeScale
	NextMove        IdleWalkCycle

	Type		Beast
	  Anim          Beast\walk 6 25
	  MoveRate      0.3  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Crystal
	  Anim          male\MonComb_walk 1
	  MoveRate	0.146
	TEnd
MEnd

Move IdleWalkCycle
	Requires	WALK FORWARD
	Priority	2
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale SmoothSprint AlwaysSizeScale
	NextMove        IdleWalkPost

	Type		Beast
	  Anim          Beast\walk 1 25
	  MoveRate      0.3  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Crystal
	  Anim          male\MonComb_walk 1
	  MoveRate	0.146
	TEnd
MEnd

Move IdleWalkPost
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Interpolate     8

	Type		Beast
	  Anim          Beast\postwalk 5 19
	  PlayFx        FEET\LAND1.FX
	TEnd
MEnd

###################
###### Jump  ######
###################

Move jumpPre
	Requires	JUMP
	Priority	1
	Interrupts	<PostMoveCycle> <Ready> <BasicMove> <WepMove> <jumpFall> <Block> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        jumpUp

	Type		Beast
	  Anim          Beast\jump 10 90
	TEnd
MEnd

Move jumpUp
	Requires	JUMP
	Priority	1
	Member		<DeathIRQ> <Jumps>
	Flags		Cycle
	NextMove        jumpApex

	Type		Beast
	  Anim          Beast\jump 91 92
	TEnd
MEnd

Move jumpApex
	Requires	APEX
	Priority	1
	Member		<DeathIRQ> <Jumps>
	NextMove        jumpFall

	Type		Beast
	  Anim          Beast\jump 93 94
	TEnd
MEnd

Move jumpFall
	Requires	AIR
	Priority	1
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        jumpFall2

	Type		Beast
	  Anim          Beast\jump 95 117
	TEnd
MEnd

Move jumpFall2
	Requires	AIR
	Priority	1
	Member		<DeathIRQ> <Jumps> <jumpFall>
	Flags		Cycle
	NextMove        JumpPost

	Type		Beast
	  Anim          Beast\jump 118 120
	TEnd
MEnd

Move JumpPost
	Include sequencers\Members\Readies.inc
	Interpolate     0

	Type		Beast
	  Anim          Beast\postjump 1
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		Behemoth
	  Anim          Beast\postjump 1
	  PlayFx        FEET\giantfoot2.FX
	TEnd

	Type		Crystal
	  Anim          male\monster_postjump 1
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		Magmite
	  Anim          Beast\postjump 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 3
	TEnd
MEnd

###################
### Combat run Forward ##
###################

Move C_RunPre
	Requires	FORWARD COMBAT
	Interrupts	<Combat> <PostMoveCycle> <Claw> PreClaw <CombatAttack> <Block> <MoveIRQ>
	Member		<CombatMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs MoveScale
	NextMove        C_RunCycle
	Priority	2

	Type		Beast
	  Anim          Beast\run 3 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		Behemoth
	  Anim          Beast\run 3 25
	  PlayFx        FEET\giantfoot2.FX 6
	  MoveRate      0.7  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		CameraShaker
	  Anim          Beast\walk 6 25
	  PlayFx        FEET\giantright2.FX  13
	  MoveRate      0.3  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		Crystal
	  Anim          male\monster_run 3 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type           Natterling
	  Anim          Beast\run 3 25
	  PlayFx        FEET\NatterRight.FX  3
	  MoveRate      0.7  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		Shivan
	  Anim          male\goblin_run 1
		PlayFX 		ENEMYFX\Unique\Shivan\Shivan_BreathRun_SFX.fx
	  PlayFx        FEET\Shivan_Left.FX  10
	  PlayFx        FEET\Shivan_Right.FX  21
	  Scale		1.2
	  MoveRate      0.2
	TEnd
MEnd

Move C_RunCycle
	Requires	FORWARD COMBAT
	Member		<CombatMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle  MoveScale
	NextMove        C_RunPost
	Priority	2

	Type		Beast
	  Anim          Beast\run 1 25
	  PlayFx        FEET\HUGELEFT2.FX  1
	  PlayFx	FEET\HUGERIGHT2.FX  13
	  MoveRate      0.7  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		Behemoth
	  Anim          Beast\run 1 25
	  PlayFx        FEET\giantfoot1.FX 1
	  PlayFx        FEET\giantfoot2.FX 10
	  MoveRate      0.7  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		CameraShaker
	  Anim          Beast\walk 1 25
	  PlayFx        FEET\giantleft2.FX  1
	  PlayFx        FEET\giantright2.FX  13
	  MoveRate      0.3  ## 0.3 x 30 = I animated this to move at 9 feet per second
	TEnd

	Type		Crystal
	  Anim          male\monster_run 1 25
	  PlayFx        FEET\HUGERIGHT2.FX  3
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		natterling
	  Anim          Beast\run 1 25
	  PlayFx        FEET\natterleft.FX  1
	  PlayFx        FEET\natterright.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Shivan
	  Anim          male\goblin_run 1
	  PlayFx        FEET\Shivan_Left.FX  10
	  PlayFx        FEET\Shivan_Right.FX  21
	  Scale		1.2
	  MoveRate      0.2
	TEnd
MEnd

Move C_RunPost
	Interrupts	RunPost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale		0.6

	Type		Beast
	  Anim          Beast\combat_postrun 3
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	TEnd

	Type		Behemoth
	  Anim          Beast\combat_postrun 3
	  PlayFx        FEET\giantland4.FX
	TEnd

	Type		CameraShaker
	  Anim          Beast\combat_postrun 3
	  PlayFx        FEET\giantland3.FX  7
	  PlayFx        EnemyFx\BanishedPantheon\GiantEnemyTremorsRight.fx
	TEnd

	Type		Crystal
	  Anim          male\MonComb_postwalk 5
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	TEnd

	Type		Natterling
	  Anim          Beast\combat_postrun 3
	  PlayFx        FEET\Natterland.FX
	TEnd
	
	Type		Shivan
	  Anim          male\goblin_run 1
	  PlayFx        FEET\Shivan_Left.FX  1
	  Scale		1.2
	  MoveRate      0.2
	TEnd

MEnd