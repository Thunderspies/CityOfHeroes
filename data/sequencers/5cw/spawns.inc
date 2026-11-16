###################################
###### Spawn Teleport        ######
##################################

Move Teleport_hide
	Requires	SPAWN TELEPORT
	Priority        3
	Interrupts	<DeathIRQ>
	NextMove        Teleport_Spawn
	Sets            CANTMOVE NOCOLLISION
	Flags		Cycle NotSelectable NoDraw

	Type		5cw
	  Anim          male\drain 150 160
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move Teleport_Spawn
	Member		<ReactIRQ> <DeathIRQ>
	NextMove        Teleport_lookquick
	Priority        1
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\absorb 1
	  PlayFx        POWERS\Teleport\Teleport.fx  0
	  PlayFx        GENERIC\entityFadeIn.fx  0
	TEnd
MEnd

Move Teleport_lookquick
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Teleport_SpawnReady

	Type		5cw
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Teleport_SpawnReady
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	Scale		0.5

	Type		5cw
	  Anim          male\suspicious 1
	TEnd
MEnd

##  Spawns  ###########################################


###################
### Robot off  #####
###################

Move Off
	Requires	QUICK SPAWN SLEEP
	Priority        3
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		Cycle NotSelectable
	Sets            CANTMOVE
	NextMove        Activate

	Type		5cw
	  Anim          5cw\activate 1 5
	  playFX	V_COV\EnemyPowers\Arachnos\bot_activate_launchpad_Freedom.fx 0 CONSTANT
	TEnd
MEnd


###################
### Robot Activate   #####
###################

Move Activate
	Requires	QUICK SPAWN SLEEP NO
	Interrupts	Off
	Priority        4
	Flags		NotSelectable
	NextMove	Ready
	Sets            CANTMOVE
	Scale		0.75

	Type		5cw
	  Anim          5cw\activate 6
	  PlayFx	POWERS\Ability\Intelligence.fx
	  PlayFx	Powers\ElectricityControl\ElectricityBoltHit3.fx
	  PlayFx        FEET\MM_Bot_FF_Left.FX 80
	  PlayFx        FEET\MM_Bot_FF_Right.FX 107
	TEnd
MEnd

#####################
### Basic Spawn  #####
#####################

Move Basic_Spawn
	Requires	SPAWNEDASPET
	Interrupts	<Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
	NextMove        Spawned_post
	Priority        1
	Interpolate     0
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\Spawn_Ready 1 199
	TEnd

	Type		Unfold
	  Anim          5cw\box_unfold 1
	  PlayFX	V_COV\Powers\Robotics\RobotBox.fx 0
	  PlayFx	V_COV\Powers\RobotSpawns\bot_impact.fx 0 CONSTANT
	  PlayFx	V_COV\Powers\RobotSpawns\bot_invisible.fx 0 CONSTANT
	TEnd

	Type		Assemble
	  Anim          5cw\spawn_hatch 1
	  PlayFx        V_COV\Powers\Robotics\RoboHatch.fx 0 CONSTANT
	  PlayFx        FEET\MM_Bot_FF_Left.FX 102
	  PlayFx        FEET\MM_Bot_FF_Right.FX 128
	TEnd

	Type		Drop
	  Anim          5cw\Spawn_Drop 1 117
	  PlayFx        GENERIC\RobotSpawn_fadein.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\RobotSpawns\FX_RobotSpawn_Drop.fx
	TEnd

	Type		Malta_ZeusTitan_Combined
	  Anim          5cw\pre_poison 43 97
	  PlayFx        GENERIC\quick_ENTITYFADEIN.FX 0 CONSTANT
	  PlayFx        ENEMYFX\Malta\TitanTransformFX\TitanTransformMerged.fx
	TEnd
MEnd

Move Spawned_post
	Include sequencers\Members\Readies.inc
	NextMove	Spawned_cycle
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Unfold
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Assemble
	  Anim          5cw\skel_monster_cycle 1
	TEnd

	Type		Drop
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Malta_ZeusTitan_Combined
	  Anim          5cw\pre_poison 96
	TEnd
MEnd

Move Spawned_cycle_Loop
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Spawned_cycle_Loop
	CycleMove	Spawned_cycle_Loop
	CycleMove	Spawned_cycle

	Type		5cw
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Unfold
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Assemble
	  Anim          5cw\skel_monster_cycle 1
	TEnd

	Type		Drop
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Malta_ZeusTitan_Combined
	  Anim          5cw\combat_cycle 1
	TEnd
MEnd

Move Spawned_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Spawned_cycle_Loop

	Type		5cw
	  Anim          5cw\Spawn_Ready 200
	TEnd

	Type		Unfold
	  Anim          5cw\Spawn_Ready 1 199
	TEnd

	Type		Assemble
	  Anim          5cw\skel_monster_cycle 1
	TEnd

	Type		Drop
	  Anim          5cw\Spawn_Ready 1 199
	TEnd

	Type		Malta_ZeusTitan_Combined
	  Anim          5cw\combat_cycle 1
	TEnd
MEnd