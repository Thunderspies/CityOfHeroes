#############################################################
## encounters.inc
#############################################################

#############################################################
## IDLE MODE
#############################################################

Move B_IdleReady
	Requires	IDLE
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

## IDLE WALK FORWARD ###########################################################

Move B_IWalkPre
	Requires	FORWARD IDLE
	Priority	2
	Interrupts	B_IdleReady <MonsterGroup> <Ready> <BasicMove> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ>
	Flags		ReqInputs
	NextMove        B_IWalkCycle
	Interpolate	12

	Type		Beast
	  Anim          male\MonComb_walk 1
	  Scale         0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move B_IWalkCycle
	Requires	FORWARD IDLE
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ>
	Flags		Cycle
	NextMove        B_IWalkPost

	Type		Beast
	  Anim          male\MonComb_walk 1
	  Scale         0.5
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move B_IWalkPost
	Requires	IDLE
	Priority	1
	Include sequencers\Members\Readies.inc
	NextMove        B_IdleReady

	Type		Beast
	  Anim          male\MonComb_postwalk 1
	  Scale		2.0
	TEnd
MEnd

#############################################################
## ANIMLISTS
#############################################################

## MAGIC RUMBLE B ###########################################################

Move Beast_Magic_Rumble_B
	Requires	ENCOUNTER RUMBLEB MAGIC CAST
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\zombie_ranged_rumble_B 1 254
	TEnd
MEnd

## TOTEM LIFT ###########################################################

Move Totem_lift
	Requires	ENCOUNTER WORK 2HAND LOW MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove	Totem_lift_toss
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\goblin_totem_lift 1
	  PlayFx        GENERIC\Fx_totem.fx 0 CONSTANT
	TEnd
MEnd

Move Totem_lift_toss
	NextMove        Ready
	Flags		NoSizeScale
	Sets            CANTMOVE
	Interpolate	8

	Type		Beast
	  Anim          Beast\goblin_totem_lift_toss 1 90
	  PlayFx        GENERIC\Fx_totem_toss.fx 0
	TEnd
MEnd

## TOTEM LIFT SIDE ###########################################################

Move Totem_lift_side
	Requires	ENCOUNTER WORK 2HAND LOW MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale
	NextMove	Totem_lift_drop
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\goblin_totem_liftside 1
	  PlayFx        GENERIC\Fx_totem_side.fx  0 CONSTANT
	TEnd
MEnd

Move Totem_lift_drop	## (This move must finish)
	NextMove        Ready
	Flags		NoSizeScale
	Sets            CANTMOVE
	Interpolate	8

	Type		Beast
	  Anim          Beast\goblin_totem_liftside_drop 1 65
	  PlayFx        GENERIC\Fx_totem_side_drop.fx  0
	TEnd
MEnd

## DIG ###########################################################

Move 2Hdig
	Requires	ENCOUNTER WORK HAND STRONG
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          Beast\goblin_2Hdig 1 32
	  PlayFx        GENERIC\2Hdig_dirt.fx  0
	TEnd
MEnd

#############################################################

Move 1Hdig
	Requires	ENCOUNTER WORK HAND STRONG
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle ##NoSizeScale

	Type		Beast
	  Anim          Beast\goblin_1Hdig 1 40
	  PlayFx        GENERIC\1Hdig_dirt.fx  0
	TEnd
MEnd

## EAT ###########################################################

Move Menace_eat
	Requires	ENCOUNTER EAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Menace_nibble
	CycleMove       Menace_lookquick
	CycleMove       Menace_lookquick_slow

	Type		Beast
	  Anim          Beast\eat 1
	  PlayFx        GENERIC\meatbone.fx  0 CONSTANT
	TEnd
MEnd

Move Menace_nibble
	Requires	ENCOUNTER EAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_lookquick
	CycleMove       Menace_nibble_blend

	Type		Beast
	  Anim          Beast\nibble 1
	  PlayFx        GENERIC\meatbone.fx  0 CONSTANT
	TEnd
MEnd

Move Menace_nibble_blend
	Requires	ENCOUNTER EAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_lookquick_slow

	Type		Beast
	  Anim          Beast\nibble 11
	  PlayFx        GENERIC\meatbone.fx  0 CONSTANT
	TEnd
MEnd

Move Menace_lookquick
	Requires	ENCOUNTER EAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_eat

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx  0 CONSTANT
	TEnd
MEnd

Move Menace_lookquick_slow
	Requires	ENCOUNTER EAT
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Menace_eat
	Scale		0.6

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx  0 CONSTANT
	TEnd
MEnd

## EAT JUNK ###########################################################

Move Junk_eat
	Requires	ENCOUNTER EAT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove	Junk_nibble
	CycleMove       Junk_lookquick
	CycleMove       Junk_lookquick_slow

	Type		Beast
	  Anim          Beast\eat 1
	  PlayFx        GENERIC\prop_pipe2.fx  0 CONSTANT
	TEnd
MEnd

Move Junk_nibble
	Requires	ENCOUNTER EAT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Junk_lookquick
	CycleMove       Junk_nibble_blend

	Type		Beast
	  Anim          Beast\nibble 1
	  PlayFx        GENERIC\prop_pipe2.fx  0 CONSTANT
	TEnd
MEnd

Move Junk_nibble_blend
	Requires	ENCOUNTER EAT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Junk_lookquick_slow

	Type		Beast
	  Anim          Beast\nibble 11
	  PlayFx        GENERIC\prop_pipe2.fx  0 CONSTANT
	TEnd
MEnd

Move Junk_lookquick
	Requires	ENCOUNTER EAT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Junk_eat

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\prop_pipe2.fx  0 CONSTANT
	TEnd
MEnd

Move Junk_lookquick_slow
	Requires	ENCOUNTER EAT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Junk_eat
	Scale		0.6

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\prop_pipe2.fx  0 CONSTANT
	TEnd
MEnd

## SURROUNDED ###########################################################

Move B_Surrounded_A
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	CycleMove       B_Surrounded_Aa

	Type		Beast
	  Anim          male\suspicious 1 50
	TEnd
MEnd

Move B_Surrounded_Aa
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       B_Surrounded_Ab

	Type		Beast
	  Anim          male\suspicious 1 50
	TEnd
MEnd

Move B_Surrounded_Ab
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.70
	Interpolate     8
	CycleMove       B_Surrounded_A2

	Type		Beast
	  Anim          male\suspicious 63
	TEnd
MEnd

Move B_Surrounded_A2
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       B_Surrounded_A3

	Type		Beast
	  Anim          male\suspicious 63
	TEnd
MEnd

Move B_Surrounded_A3
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       B_Surrounded_A4

	Type		Beast
	  Anim          male\lookquick 1
	TEnd
MEnd

Move B_Surrounded_A4
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       B_Surrounded_A5
	Interpolate	15

	Type		Beast
	  Anim          Beast\feetclaw 1
	TEnd
MEnd

Move B_Surrounded_A5
	Requires	ENCOUNTER STRONG MENACE LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Interpolate	15
	CycleMove       B_Surrounded_A

	Type		Beast
	  Anim          male\vandal_kick_low 1
	TEnd
MEnd

## LAVAMAN POPUP STATIC ###########################################################

Move Lava_popup_Static_sleep
	Requires	ENCOUNTER SLEEP SNEAK LAUNCH LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Lava_popup_Static
	Interpolate	0
	Flags		CYCLE NotSelectable
	Sets            NOCOLLISION CANTMOVE

	Type		Beast
	  Anim          Beast\lava_jump_static 1 5
	  PlayFx	WORLD\Ruins\MediumFireFast.fx 0 CONSTANT
	TEnd
MEnd

Move Lava_popup_Static
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Lava_popup_post
	Interpolate	0
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\lava_jump_static 11 31
	  PlayFx	POWERS\FireControl\FireBallTarget1.fx 1 CONSTANT
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 3
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 5
	TEnd
MEnd

## LAVAMAN POPUP SHORT ###########################################################

Move Lava_popup_Short_sleep
	Requires	ENCOUNTER SLEEP SNEAK LAUNCH MID
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NotSelectable
	NextMove	Lava_popup_Short
	Interpolate	0
	Sets            NOCOLLISION CANTMOVE

	Type		Beast
	  Anim          Beast\lava_jump_short 1 5
	TEnd
MEnd

Move Lava_popup_Short
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Lava_popup_post
	Interpolate	0
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\lava_jump_short 5 49
	  PlayFx	POWERS\FireControl\FireAura.fx 6 CONSTANT
	  PlayFx	POWERS\FireControl\FireBallTarget1.fx 1 CONSTANT
	  PlayFx	Volumes\Lava\DeepSplash\Encounter_DeepSplash.fx 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 7
	TEnd
MEnd

## LAVAMAN POPUP LONG ###########################################################

Move Lava_popup_long_sleep
	Requires	ENCOUNTER SLEEP SNEAK LAUNCH HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NotSelectable
	NextMove	Lava_popup_long
	Interpolate	0
	Sets            NOCOLLISION CANTMOVE

	Type		Beast
	  Anim          Beast\lava_jump 1 4
	TEnd
MEnd

Move Lava_popup_long
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Lava_popup_post
	Interpolate	0
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\lava_jump 5 54
	  PlayFx	POWERS\FireControl\FireAura.fx 6 CONSTANT
	  PlayFx	Volumes\Lava\DeepSplash\Encounter_DeepSplash.fx 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 7
	TEnd
MEnd

Move Lava_popup_post
	Include sequencers\Members\Encounters_Idle.inc
	Interpolate	1
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\postjump 1 30
	  PlayFx        FEET\LAND1.FX
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 1
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 3
	TEnd
MEnd