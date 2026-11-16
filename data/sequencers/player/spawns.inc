####################################################################
## SPAWNS.INC - Sequences for Spawning animations and FX
####################################################################

#####################
### Basic Spawn  #####
#####################

Move Basic_Spawn
	Requires	SPAWNEDASPET
	Interrupts	<MonsterGroup> <Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
	NextMove        Spawned_post
	Priority        10
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\basic_runout 47 88   ##absorb 1 60
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx        FEET\MALELEFT.FX  8
	  PlayFx        FEET\MALERIGHT.FX  21
	  PlayFx        FEET\MALELEFT.FX  34
	TEnd

	Type		skeleton
	  Anim          male\zombie_monster_popup 1 40
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  2
	  PlayFx        V_COV\Powers\Necromacy\SkeletonSummonHit.fx  0
	TEnd

	Type		mastermind_skeleton
	  Anim          male\zombie_monster_popup 1 40
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  2
#	  PlayFx        V_COV\Powers\Necromacy\SkeletonSummonHit.fx  0
	TEnd

	Type		Lich
	  Anim          male\raisedead 1 ##60
	  PlayFX	Generic\entityFadeIn.fx 0
	  Scale		0.5
	TEnd

	Type		scrapyarders
	  Anim          male\hole_jump_out 1
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  2
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Drop 1 80
	  PlayFx        GENERIC\RobotSpawn_fadein.fx  0 CONSTANT
	  PlayFX	V_COV\Powers\RobotSpawns\FX_RobotSpawn_Drop.fx
	TEnd

	Type		ninja
	  Anim          male\Ninja_spawn 1
	  PlayFX	Generic\ninja_entityFadeIn.fx 0
	TEnd

	Type		Oni
	  Anim          male\heroic_laugh_delay 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		fem_widow
	  Anim          male\heroic_laugh_delay 1
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		Rappel
	  Anim          male\Paramilitary_SoldiersRappel 1 135
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SoldierFadeIn.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\Paramilitary\FX_RappelRope.fx
	TEnd

	Type		Parachute
	  Anim          male\Parachute 1
	  PlayFx	V_COV\Powers\Paramilitary\FX_Parachute.fx
	  PlayFx        FEET\MALELEFT.FX  101
	  PlayFx        FEET\MALERIGHT.FX  105
	  PlayFx        FEET\MALELEFT.FX  112
	  PlayFx        FEET\MALERIGHT.FX  118
	TEnd

	Type		Gollem
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  12
	TEnd

	Type		Gollem_Water
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        Volumes\Water\DeepSplash\Encounter_DeepSplash.fx 12
	  PlayFx	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit.fx 12
	TEnd

	Type		Gollem_Water_Tintable
	  Anim          male\zombie_monster_popup 1 24
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		male_fadein
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		fem_fadein
	  Anim          male\absorb 1 60
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		Sprocket
	  Anim          male\sprocket_spawn 1 101
	  PlayFx        GENERIC\FX_sprocket_spawn.FX  0
	TEnd

	Type		Motorcycle
	  Anim          male\Thug_MotoSpawn 1
	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnFadInMoto.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\Thugs\FX_Motorcycle.fx 0 CONSTANT
	TEnd

	Type		huge_Motorcycle
	  Anim          male\Thug_MotoSpawn2 1
	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnFadInMoto.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\Thugs\FX_Motorcycle.fx 1 CONSTANT
	TEnd

	Type		Manhole
	  Anim          male\Thugs_SpawnManHole 1
	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnFadIn.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnManHoleCover.fx 0 CONSTANT
	TEnd

	Type		xmas_male
	  Anim          5cw\box_unfold 1   ###Spawn_Unfold 1 79
	  PlayFX	V_COV\Powers\Robotics\RobotBox.fx 0
	  PlayFx	V_COV\Powers\RobotSpawns\bot_impact.fx 0 Constant
	  PlayFx	V_COV\Powers\RobotSpawns\bot_invisible.fx 0 Constant
	TEnd

	Type		GhostWidow
	  Anim          fem\raisedead 1 98
	TEnd

	Type		Monster
	  Anim          male\Jump_in_Place_spawn 1 56
	TEnd

	Type		fem_coralax
	  Anim          male\Jump_in_Place_spawn 1 56
	  PlayFx        Volumes\Water\DeepSplash\Encounter_DeepSplash.fx 0
	  PlayFx	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit.fx
	TEnd

	Type		fem_coralax_Tintable
	  Anim          male\Jump_in_Place_spawn 1 56
	TEnd

	Type		Arachnos_Technician
	  Anim		male\Skel_ready2 1
	  PlayFx	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_DeployedSoldier.fx
	TEnd

	Type		male_incarnate
	  Anim          male\absorb 1
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		Leprechaun
	  Anim          male\absorb 30 60
	  PlayFX	NPCFX\Pets\Leprechaun\LepreSpawn.fx
	TEnd
MEnd

Move Spawned_post
	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Spawned_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\lookquick 1  ##absorb 61    #
	TEnd

	Type		skeleton
	  Anim          male\stand_2Hhips 1 64
	TEnd

	Type		Lich
	  Anim          male\raisedead_post 1
	  Scale		0.7
	TEnd

	Type		scrapyarders
	  Anim          male\lookquick 1
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Drop 81 117
	TEnd

	Type		ninja
	  Anim          male\quick_bow 1 103
	TEnd

	Type		soldier
	  Anim          male\formal_salute2 1 40
	TEnd

	Type		Oni
	  Anim          male\stand_Xarms 1 68
	TEnd

	Type		fem_widow
	  Anim          fem\stand_Xarms 1 68
	TEnd

	Type		Rappel
	  Anim          male\salute_formal 1
	TEnd

	Type		Rappel_bow
	  Anim          male\quick_bow 1 103
	TEnd

	Type		Gollem
	  Anim          Beast\postjump 1 35
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	TEnd

	Type		male_fadein
	  Anim          male\absorb 61
	TEnd

	Type		fem_fadein
	  Anim          male\absorb 61
	TEnd

	Type		Sprocket
	  Anim          male\sprocket_spawn 102 126
	  Scale           0.50
	TEnd

	Type		Motorcycle
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge_Motorcycle
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		Manhole
	  Anim          male\Skel_ready2 1
	TEnd

	Type		GhostWidow
	  Anim          fem\raisedead_post 1
	TEnd

	Type		Monster
	  Anim          male\Jump_in_Place_spawn 57
	TEnd

	Type		fem_coralax
	  Anim          male\Jump_in_Place_spawn 57
	TEnd

	Type		male_incarnate
	  Anim          male\boost 1
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\absorb 1
	 # PlayFx        ENEMYFX\Seers\Summon_Portal.fx 0
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\absorb 1
	 # PlayFx        ENEMYFX\Seers\Summon_Portal.fx 0
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\absorb 1
	#  PlayFx        ENEMYFX\Seers\Summon_Portal.fx 0
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim          male\absorb 1
	 # PlayFx        ENEMYFX\Seers\Summon_Portal.fx 0
	TEnd


MEnd

Move Spawned_cycle
	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle

	Type		male
	  Anim          male\smackhands 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		skeleton
	  Anim          male\idle_2hips_look 1
	TEnd

	Type		Lich
	  Anim          male\Skel_ready2 1
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Ready 1
	TEnd

	Type		ninja
	  Anim          male\quick_bow 104 163
	TEnd

	Type		soldier
	  Anim          male\formal_salute2 41 45
	TEnd

	Type		Oni
	  Anim          male\idle_xarms_look 1
	  Scale		0.5
	TEnd

	Type		fem_widow
	  Anim          fem\idle_xarms_look 1
	  Scale		0.5
	TEnd

	Type		Rappel
	  Anim          male\salute_formal 1 5
	TEnd

	Type		Rappel_bow
	  Anim          male\quick_bow 104 163
	TEnd

	Type		Gollem
	  Anim          Beast\postjump 35
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	TEnd

	Type		male_fadein
	  Anim          male\Skel_ready2 1
	TEnd

	Type		fem_fadein
	  Anim          male\Skel_ready2 1
	TEnd

	Type		Sprocket
	  Anim          male\sprocket_spawn 127 186
	  Scale           0.50
	TEnd

	Type		GhostWidow
	  Anim          fem\Skel_ready2 1
	TEnd

	Type		Monster
	  Anim          male\Jump_in_Place_spawn 57
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\Skel_ready2 1
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\Skel_ready2 1
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\Skel_ready2 1
	TEnd

	Type		P_Apparitions_Lieutenant_Merged
	  Anim          male\Skel_ready2 1
	TEnd

MEnd


#####################
### Flying Basic Spawn  #####
#####################

Move Fly_Basic_Spawn
	Requires	Fly SPAWNEDASPET
	Interrupts	<Ready> <Combat> <WepCombat> <FlyCombat> <FlyWepCombat> <FlyReady> <RaiseDeadIRQ>
	NextMove        Fly_Spawned_post
	Priority        12
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\absorb 1 60    #basic_runout 47 88
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		Ghost
	  Anim          male\fallout_soul_extract 1 60
	TEnd

	Type		GhostSoul
	  Anim          male\fallout_soul_extract 1 48
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Drop 1 80
	  PlayFx        GENERIC\RobotSpawn_fadein.fx  0 CONSTANT
	  PlayFX	V_COV\Powers\RobotSpawns\FX_RobotSpawn_Drop.fx
	TEnd
MEnd

Move Fly_Spawned_post
	Requires	Fly
	NextMove	Fly_Spawned_cycle
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE
	Priority        2

	Type		male
	  Anim          male\absorb 61
	TEnd

	Type		Ghost
	  Anim          male\fallout_soul_extract 61
	TEnd

	Type		GhostSoul
	  Anim          male\fallout_soul_extract 49
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Drop 81 117
	TEnd
MEnd

Move Fly_Spawned_cycle
	Requires	FLY
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyReady> <AttackIRQ>
	Flags		Cycle

	Type		male
	  Anim          male\smackhands 1
	TEnd

	Type		Ghost
	  Anim          male\float2 1
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		wisp
	  Anim          wisp\float 1
	TEnd

	Type		Sky_jumpbot   ##Drop
	  Anim          5cw\Spawn_Ready 1
	TEnd

MEnd

##################################
### Spawn Ghost (popup in combat)  #####
##################################

Move Spawn_Ghost
	Requires	ENCOUNTER FLY LOW SPAWN
	Priority	4
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Spawn_Ghost_post
	Scale		0.7
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable
	Interpolate     0

	Type		male
	  Anim          male\raisedead 1 92
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd
MEnd

Move Spawn_Ghost_post
	Requires	FLY
	Priority	1
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	FlyReady
	Interpolate	15

	Type		male
	  Anim          male\pre_fly 29
	TEnd
MEnd


##################################
### Spawn Pet (popup in combat)  #####
##################################

Move Spawn_Pet
	Requires	ENCOUNTER LOW SPAWN
	Priority	3
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Spawn
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable
	Scale		0.5
	Interpolate     0

	Type		male
	  Anim          male\zombie_sleep 1 50
	  PlayFx        GENERIC\FX_sprocket_spawn.FX  0
	TEnd

	Type		Gollem
	  Anim          male\zombie_sleep 1 50
	  PlayFx        GENERIC\FX_gollem_spawn.FX  0
	TEnd

	Type		skeleton
	  Anim          male\raisedead 1 32 #98
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd
MEnd

Move Spawn
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Spawn2
	Scale           0.50
	Interpolate	0
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\air_drain 4 35
	TEnd
MEnd

Move Spawn2
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Spawn_cycle_half
	Scale           0.50
	Interpolate     3

	Type		male
	  Anim          male\monster_postjump 1 35
	TEnd
MEnd

Move Spawn_cycle_half
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Spawn_cycle

	Type		male
	  Anim          male\monster_postjump 35
	TEnd
MEnd

Move Spawn_cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Priority	0

	Type		male
	  Anim          male\monster_postjump 35
	TEnd
MEnd


###################################
###### Spawn Teleport        ######
##################################

Move Teleport_hide
	Requires	ENCOUNTER SPAWN TELEPORT
	Priority	3
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Teleport_Spawn
	Sets            CANTMOVE
	Flags		Cycle NotSelectable

	Type		male
	  Anim          male\drain 150 160
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Teleport_Spawn
	Include sequencers\Members\Encounters_Idle.inc
	NextMove        Teleport_lookquick
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim          male\absorb 1
	  PlayFx        POWERS\Teleport\Teleport.fx  0
	  PlayFx        GENERIC\entityFadeIn.fx  0
	TEnd
MEnd

Move Teleport_lookquick
	Include sequencers\Members\Encounters_Idle.inc
	NextMove	Teleport_SpawnReady

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Teleport_SpawnReady
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd

###################
### Faathim Free  #####
###################

Move Faathim_Imprison_free
	Requires	ENCOUNTER POSE SLEEP STRONG MAGIC SPAWN
	Priority	6
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Ready
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable
	Interpolate	0

	Type		male
	  Anim          male\flameaura 13
	  PlayFX	ENEMYFX\DevouringEarth\Crystals\CrystalPod\CrystalPodExplode01.fx 0
	TEnd

	Type		huge
	  Anim          huge\fireaura 13
	  PlayFX	ENEMYFX\DevouringEarth\Crystals\CrystalPod\CrystalPodExplode01.fx 0
	TEnd

	Type		fem
	  Anim          fem\fireaura 13
	  PlayFX	ENEMYFX\DevouringEarth\Crystals\CrystalPod\CrystalPodExplode01.fx 0
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_imprison_free 12
	  PlayFX	ENEMYFX\Rularuu\Faathim\CrystalExplodeFX\CrystalExplode01.fx 0
	  PlayFX	POWERS\EnergyBlast\Faathim_Free.fx 0
	  PlayFX	POWERS\ColdControl\EnergyAbsorption.fx
	TEnd
MEnd

###############################
###### Vampire Birth 	 ######
###############################

Move VampireBirth
	Requires	ENCOUNTER BUSY POSE MAGIC SPAWN
	Priority	5
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NotSelectable
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\vampire_stand 1 60
	TEnd
MEnd


###### Dismiss #######################################################

####################
#### Dismiss    ####
####################

Move Dismiss
	Requires	DISMISS DEATH
	Interrupts	<AttackIRQ> <ReactIRQ> <HitIRQ> <DeathIRQ>
	NextMove        Dismiss_cycle
	Priority        102
	Flags		NotSelectable NOCOLLISION

	Type		male
	  Anim          male\run_away 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

	Type		ninja
	  Anim          male\smokeflash 1
	  PlayFX	V_COV\Powers\Ninjitsu\Zero_AOEWhiteSmoke.fx 45
	  PlayFx	Generic\entityVanish.fx 46 Constant
	TEnd

	Type		soldier
	  Anim          male\formal_salute2 1 40
	TEnd

	Type		Oni
	  Anim          male\heroic_laugh_delay 1 10
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		fem_widow
	  Anim          fem\no 1 90
	  PlayFx	V_COV\EnemyPowers\GhostWidow\MinionDismiss.fx 40
	  PlayFx        GENERIC\ENTITYVANISH.FX  42 CONSTANT
	TEnd

	Type		fem_widow_Tintable
	  Anim          fem\no 1 90
	  PlayFx        GENERIC\ENTITYVANISH.FX  42 CONSTANT
	TEnd

	Type		skeleton
	  Anim          male\death_front 47 108
	  PlayFx	powers\DarknessControl\Zero_DarkArmorCloak.fx 0 Constant
	  PlayFX	V_COV\Powers\Necromacy\Dismiss_zombie.fx 60 Constant
	TEnd

	Type		mastermind_skeleton
	  Anim          male\death_front 47 108
#	  PlayFx	powers\DarknessControl\Zero_DarkArmorCloak.fx 0 Constant
#	  PlayFX	V_COV\Powers\Necromacy\Dismiss_zombie.fx 60 Constant
	TEnd

	Type		Lich
	  Anim          male\heroic_laugh 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 10 constant
	TEnd

	Type		fem_coralax
	  Anim          male\goblin_dive 1 42
	  PlayFx	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit_mystic.fx 39
	TEnd

	Type		fem_coralax_Tintable
	  Anim          male\goblin_dive 1 42
	TEnd

	Type		gollem_water
	  Anim          male\goblin_dive 1 42
	  PlayFx	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit_mystic.fx 39
	TEnd

	Type		gollem_water_tintable
	  Anim          male\goblin_dive 1 42
	TEnd
MEnd

Move Dismiss_cycle
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		NotSelectable NOCOLLISION Cycle
	Interpolate     5

	Type		male
	  Anim          male\run_away 76
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

	Type		ninja
	  Anim          male\smokeflash 1
	  PlayFx	Generic\entityVanish.fx 0 Constant
	TEnd

	Type		soldier
	  Anim          male\run_away2 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

	Type		Oni
	  Anim          male\heroic_laugh_delay 1 10
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		fem_widow
	  Anim          fem\no 91
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		skeleton
	  Anim          male\death_front 108
	  PlayFx	Generic\entityVanish.fx 0 Constant
	TEnd

	Type		Lich
	  Anim          male\heroic_laugh_delay 1 10
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

	Type		fem_coralax
	  Anim          male\goblin_dive 42
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		gollem_water
	  Anim          male\goblin_dive 42
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

####################
#### Air Dismiss    ####
####################

Move A_Dismiss
	Requires	Fly DISMISS DEATH
	Interrupts	<AttackIRQ> <ReactIRQ> <HitIRQ> <DeathIRQ>
	NextMove        A_Dismiss_cycle
	Priority        111
	Flags		NOCOLLISION

	Type		male
	  Anim          male\air_boostup 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 10 constant
	TEnd
MEnd

Move A_Dismiss_cycle
	Requires	Fly DEATH
	Member		<RaiseDeadIRQ>
	Flags		NotSelectable NOCOLLISION Cycle
	Interpolate     5

	Type		male
	  Anim          male\air_boostup 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd

#############################################################
## Ninja Ambush
#############################################################

Move Ninja_Ambush_sleep
	Requires	SPAWN NINJAAMBUSH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE
	NextMove	Ninja_Ambush
	Interpolate	0

	Type		male
	  Anim          male\zombie_alt_popup 1 2
	  PlayFx        GENERIC\ENTITYVANISH.fx 0 CONSTANT
	TEnd
MEnd

Move Ninja_Ambush
	NextMove        Ninja_Ambush_cycle
	Interpolate     0
	Flags		NotSelectable NOCOLLISION CANTMOVE

	Type		male
	  Anim          male\Ninja_spawn 1
	  PlayFX	Generic\ninja_entityFadeIn.fx 0
	TEnd
MEnd

Move Ninja_Ambush_cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
##  PARACHUTE AMBUSH
#############################################################

Move parachute_Ambush_sleep
	Requires	SPAWN PARACHUTEAMBUSH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove	parachute_Ambush
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE
	Interpolate	0

	Type		male
	  Anim          male\zombie_alt_popup 1 2
	  PlayFx        GENERIC\ENTITYVANISH.fx 0 CONSTANT
	TEnd
MEnd

Move parachute_Ambush
	NextMove        parachute_Ambush_cycle
	Interpolate     0
	Flags		NotSelectable NOCOLLISION CANTMOVE

	Type		male
	  Anim          male\Parachute 1
	  PlayFx	V_COV\Powers\Paramilitary\FX_Parachute.fx
	  PlayFx        FEET\MALELEFT.fx 101
	  PlayFx        FEET\MALERIGHT.fx 105
	  PlayFx        FEET\MALELEFT.fx 112
	  PlayFx        FEET\MALERIGHT.fx 118
	TEnd
MEnd

Move parachute_Ambush_cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
##  REPEL AMBUSH
#############################################################

Move Repel_Ambush_sleep
	Requires	SPAWN REPELAMBUSH
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE
	NextMove	Repel_Ambush
	Interpolate	0

	Type		male
	  Anim          male\zombie_alt_popup 1 2
	  PlayFx        GENERIC\ENTITYVANISH.fx 0 CONSTANT
	TEnd
MEnd

Move Repel_Ambush
	NextMove        Repel_Ambush_cycle
	Interpolate     0
	Flags		NotSelectable NoSizeScale NOCOLLISION CANTMOVE

	Type		male
	  Anim          male\Paramilitary_SoldiersRappel 1 135
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SoldierFadeIn.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\Paramilitary\FX_RappelRope.fx 0
	TEnd
MEnd

Move Repel_Ambush_cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
## RIKTI SPAWN - WITH PORTAL FX
#############################################################

Move Spawn_RiktiTeleport_Pre
	Requires	SPAWN RIKTI
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Scripted.inc
	CycleMove	Spawn_RiktiTeleport
	Sets            CANTMOVE
	Flags		Cycle NotSelectable NoDraw

	Type		male
	  Anim          male\absorb 1
	  PlayFx        GENERIC\entityVanish.fx 0 CONSTANT
	  PlayFX	ENEMYFX\rikti\riktiportal.fx 0
	TEnd
MEnd

Move Spawn_RiktiTeleport
	Requires	SPAWN RIKTI
	Priority        2
	Include sequencers\Members\Encounters_Scripted.inc
	CycleMove	Spawn_RiktiTeleport_Post
	Sets            CANTMOVE
	Flags		Cycle NotSelectable

	Type		male
	  Anim          male\absorb 15
	  PlayFx        GENERIC\entityFadeIn.fx 0 CONSTANT
	TEnd
MEnd

Move Spawn_RiktiTeleport_Post
	Include sequencers\Members\Encounters_Cycles.inc
	CycleMove	Spawn_RiktiTeleport_PostCycle
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Spawn_RiktiTeleport_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd