#############################################################
## encounters.inc
#############################################################

## IDLE WALK FORWARD ###########################################################

Move IdleWalkPre
	Requires	WALK FORWARD
	Interrupts	<Ready> <PostMoveCycle> <Combat> <CombatAttack> <MoveIRQ> <MoveIRQ>
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs  MoveScale SmoothSprint AlwaysSizeScale
	NextMove        IdleWalkCycle
	Scale		2.4
	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	Priority        2

	Type		5cw
	  Anim          5cw\hunchwalk 15 25
	TEnd
MEnd

Move IdleWalkCycle
	Requires	WALK FORWARD
	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale SmoothSprint AlwaysSizeScale
	NextMove        IdleWalkPost
	Scale		2.4
	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
	Priority        2

	Type		5cw
	  Anim          5cw\hunchwalk 1 25
	  PlayFx        FEET\MM_Bot_FF_Left.FX  1
	  PlayFx        FEET\MM_Bot_FF_Right.FX  12
	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
	TEnd
MEnd

Move IdleWalkPost
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Interpolate     8

	Type		5cw
	  Anim          5cw\monster_postrun 1
	  PlayFx        FEET\ROBOLAND1.FX  3
	TEnd
MEnd

#############################################################
## ENCOUNTERS
#############################################################

###################
#### Diesel Wakeup  #####
####################

Move Wakeup
	Requires	COMBAT POISON STRONG HIGH
	Priority        7
	Interrupts	<Ready> <Combat>
	Member		<DeathIRQ>
	Sets            CANTMOVE
	NextMove	Wakeup2
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\diesel_wakeup 1
	  PlayFx	ENEMYFX\Clockwork\Diesel_wakeup.Fx 16
	  PlayFx	ENEMYFX\Clockwork\Diesel_wakeup.Fx 17
	  PlayFx	ENEMYFX\Clockwork\Diesel_wakeup.Fx 19
	  PlayFx	ENEMYFX\Clockwork\Diesel_wakeup.Fx 21
	  PlayFx	ENEMYFX\Clockwork\Diesel_wakeup.Fx 23
	  PlayFx	ENEMYFX\Clockwork\FX_Diesel_wake.fx 0
	TEnd
MEnd

Move Wakeup2
	Include sequencers\Members\Readies.inc
	NextMove	Stomp2

	Type		5cw
	  Anim          5cw\stomp 1 62
	TEnd
MEnd

###################
### Battle Rifle  #####
###################

Move Batt_Rifle_A1
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_A2

	Type		5cw
	  Anim          5cw\pistol_hipshoot 1
	  PlayFx        WEAPONFX\Gunfire_Warhulk_Blaster.fx 0
	TEnd
MEnd

Move Batt_Rifle_A2
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Batt_Rifle_A2
	CycleMove       Batt_Rifle_A1
	CycleMove       Batt_Rifle_A3
	CycleMove       Batt_Rifle_A2

	Type		5cw
	  Anim          5cw\wep_cycle 1 31
	TEnd
MEnd

Move Batt_Rifle_A3
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Batt_Rifle_A4

	Type		5cw
	  Anim          5cw\flamethrower 1
	  PlayFx	WEAPONFX\Gunfire_Nemesis_FlameThrower2.fx 0
	TEnd
MEnd

Move Batt_Rifle_A4
	Requires	ENCOUNTER ATTACK RIFLE DEALING
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50
	CycleMove       Batt_Rifle_A2
	CycleMove       Batt_Rifle_A1
	CycleMove       Batt_Rifle_A4

	Type		5cw
	  Anim          5cw\combat_cycle 1 31
	TEnd
MEnd