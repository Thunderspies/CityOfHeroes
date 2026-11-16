#############################################################
## Edit22
#############################################################

## WALK FORWARD###########################################################

Move WalkPre
	Requires	FORWARD
	Priority        1
	Interrupts	<Ready> <PostMoveCycle> <Combat> <CombatAttack> <MoveIRQ> <HitIRQ>
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs AlwaysSizeScale MoveScale #SmoothSprint
	NextMove        WalkCycle
	Scale		1.2

	Type		5cw
	  Anim          Beast\run 3 25
	  PlayFx        Feet\MM_Bot_FF_Start.fx
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Warworks_Warwalker
	  Anim          Beast\run 3 25
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Left.fx 8
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Right.fx 20
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Sentinel
	  Anim          Beast\run 3 25
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Left.fx 8
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Right.fx  20
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd


	Type		Malta_Kronos
	  Anim          Beast\run 3 25
	  PlayFx        FEET\ROBOFOOT4.FX  11
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Unfold
	  Anim          Beast\run 3 25
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Drop
	  Anim          Beast\run 3 25
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Assemble
	  Anim          Beast\run 3 25
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

MEnd

Move WalkCycle
	Requires	FORWARD
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale AlwaysSizeScale ##SmoothSprint
	NextMove        WalkPost
	Scale		1.2
	Priority        1

	Type		5cw
	  Anim          Beast\run 1 25
	  PlayFx        FEET\MM_Bot_FF_Left.fx  1
	  PlayFx        FEET\MM_Bot_FF_Right.fx  13
	  PlayFx		Feet\MM_Bot_FF_Loco.fx 1
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Warworks_Warwalker
	  Anim          Beast\run 1 25
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Left.fx 8
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Right.fx  20
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Sentinel
	  Anim          Beast\run 1 25
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Left.fx 8
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Right.fx  20
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Malta_Kronos
	  Anim          Beast\run 1 25
	  PlayFx        FEET\RoboFootKronos3.fx  1
	  PlayFx        FEET\RoboFootKronos4.fx  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Unfold
	  Anim          Beast\run 1 25
	  PlayFx        FEET\ROBOFOOT1.FX  1
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Drop
	  Anim          Beast\run 1 25
	  PlayFx        FEET\ROBOFOOT1.FX  1
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

	Type		Assemble
	  Anim          Beast\run 1 25
	  PlayFx        FEET\ROBOFOOT1.FX  1
	  PlayFx        FEET\ROBOFOOT2.FX  13
	  MoveRate      0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	  SmoothSprint  3.3 ##feet from root to hip position you want to use to smooth sprinting
	TEnd

MEnd

Move WalkPost
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		5cw
	  Anim          5cw\monster_postrun 1
	  PlayFx        Feet\MM_Bot_FF_Stop.fx
	TEnd

	Type		Warworks_Warwalker
	  Anim          5cw\monster_postrun 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Left.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Walk_Right.fx 8
	TEnd

	Type		Sentinel
	  Anim          5cw\monster_postrun 1
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Left.fx 1
	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Right.fx 8
	TEnd

	Type		Malta_kronos
	  Anim          5cw\monster_postrun 1
	  PlayFx        FEET\RoboFootKronos3.fx  1
	  PlayFx        FEET\RoboFootKronos4.fx  2
	TEnd

	Type		Unfold
	  Anim          5cw\monster_postrun 1
	  PlayFx        FEET\ROBOFOOT1.FX  3
	  PlayFx        FEET\ROBOFOOT2.FX  4
	TEnd

	Type		Drop
	  Anim          5cw\monster_postrun 1
	  PlayFx        FEET\ROBOFOOT1.FX  3
	  PlayFx        FEET\ROBOFOOT2.FX  4
	TEnd

	Type		Assemble
	  Anim          5cw\monster_postrun 1
	  PlayFx        FEET\ROBOFOOT1.FX  3
	  PlayFx        FEET\ROBOFOOT2.FX  4
	TEnd

MEnd


###################
###### Jump  ######
###################

Move jumpPre
	Requires	JUMP
	Interrupts	<PostMoveCycle> <Ready> <BasicMove> <Combat> <CombatMove> <CombatAttack> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        jumpUp
	Priority        1

	Type		5cw
	  Anim          5cw\jump 10 90
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jump 10 90
		#PLAYFX	V_COV\Powers\RobotSpawns\RocketBoots_Fly_JetL.fx 0 constant
		#PLAYFX	V_COV\Powers\RobotSpawns\RocketBoots_Fly_Jetr.fx 0 constant
	  PlayFX		ENEMYFX\Praetorian\Warworks\Fly_Thrusters.fx 0 CONSTANT

	TEnd
MEnd

Move jumpUp
	Requires	JUMP
	Member		<DeathIRQ> <Jumps>
	Flags		Cycle
	NextMove        jumpApex
	Priority        1

	Type		5cw
	  Anim          5cw\jump 91 92
	  #PlayFx        FEET\SUPERJUMP1.FX
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jump 91 92
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
	TEnd

MEnd

Move jumpApex
	Requires	APEX
	Member		<DeathIRQ> <Jumps>
	NextMove        jumpFall
	Priority        1

	Type		5cw
	  Anim          5cw\jump 93 94
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jump 93 94
	TEnd

MEnd

Move jumpFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        jumpFall2
	Priority        1

	Type		5cw
	  Anim          5cw\jump 95 117
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jump 95 117
	TEnd

MEnd

Move jumpFall2
	Requires	AIR
	Priority        1
	Member		<DeathIRQ> <Jumps> <jumpFall>
	Flags		Cycle
	NextMove        JumpPost

	Type		5cw
	  Anim          5cw\jump 118 120
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jump 118 120
	TEnd

MEnd

Move JumpPost
	Include sequencers\Members\Readies.inc
	Interpolate     0
	NextMove	Ready

	Type		5cw
	  Anim          5cw\monster_postjump 1
	  PlayFx        FEET\ROBOLAND1.FX
	TEnd

	Type		Malta_kronos
	  Anim          5cw\monster_postjump 1
	  PlayFx        FEET\RoboFootKronos1.fx  1
	  PlayFx        FEET\RoboFootKronos2.fx  2
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jumppost 1 29
	  PlayFX	ENEMYFX\Praetorian\Warworks\Landing_Dust.fx   0
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Walk.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Walk.fx 4
	TEnd

MEnd

###################
### Fly forward  ####
###################

Move flypre
	Requires	FORWARD FLY
	Priority        2
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <FlyFall> <Jumps>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs
	NextMove        flyCycle
	Interpolate	20
	Wind		FLY

	Type		5cw
	  Anim          5cw\fly_forward 1
	TEnd
MEnd

Move flyCycle
	Requires	FORWARD FLY
	Priority        2
	Member		<ReactIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        flyPost
	Wind		FLY

	Type		5cw
	  Anim          5cw\fly_forward 1
	TEnd
MEnd

Move flyPost
	Requires	FLY
	Priority        1
	Include sequencers\Members\Readies_Air.inc
	Flags		ReqInputs
	NextMove        flyready
	Interpolate	8

	Type		5cw
	  Anim          5cw\post_fly_forward 15
	TEnd
MEnd


###################
###### Fly Down  ######
###################

Move PreFall
	Requires	AIR
	Member		<ReactIRQ> <DeathIRQ> <FlyFall> <AttackIRQ>
	Flags		Cycle
	NextMove        JumpPost
	Priority        1
	Interpolate	20

	Type		5cw
	  Anim          5cw\jump 118 120
	TEnd
MEnd

