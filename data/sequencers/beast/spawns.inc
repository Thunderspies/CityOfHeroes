#############################################################
## spawns.inc
#############################################################

## BASIC SPAWN ###########################################################

Move Basic_Spawn
	Requires	SPAWNEDASPET
	Priority	1
	Interrupts	<Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
	NextMove        Spawned_post
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		Beast
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX 12
	TEnd

	Type		Mini
	  Anim          male\RockBeastMini_Popup 15 24
	  PlayFx        GENERIC\groundburst_RockBeastMini.fx 0
	TEnd

	Type		CrystalMini
	  Anim          male\RockBeastMini_Popup 15 24
	TEnd

	Type		Natterling
	  Anim          Beast\lookquick 1
	  PlayFX	Generic\entityFadeIn.fx 0
	  PlayFx	V_COV\Powers\Ninjitsu\Zero_OniSummon.fx 0
	TEnd

	Type		Snowbeast
	  Anim          Beast\getup_belly 1 105
	  PlayFX	Generic\entityFadeIn.fx 0
	TEnd
MEnd

Move Spawned_post
	Include sequencers\Members\Readies.inc
	NextMove	Spawned_cycle
	Flags		NoSizeScale NotSelectable

	Type		Beast
	  Anim          Beast\postjump 1 35
	TEnd

	Type		Natterling
	  Anim          Beast\lookquick 1
	TEnd

	Type		Snowbeast
	  Anim          Beast\getup_belly 106
	TEnd
MEnd

Move Spawned_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		Beast
	  Anim          Beast\postjump 35
	TEnd

	Type		Natterling
	  Anim          Beast\lookquick 1
	TEnd

	Type		Snowbeast
	  Anim          Beast\lookquick 1
	TEnd
MEnd

## GOLEM AMBUSH ###########################################################

Move GolemAmbush_sleep
	Requires	SPAWN GOLEMSPAWN
	Priority	2
	Interrupts	<Ready> <Combat>
	Flags		Cycle NotSelectable
	NextMove	GolemAmbush_popup
	Interpolate	0
	Sets            NOCOLLISION CANTMOVE

	Type		Beast
	  Anim          Beast\lava_jump_short 1 5
	TEnd
MEnd

Move GolemAmbush_popup
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	NextMove	GolemAmbush_post
	Interpolate	0
	Flags		NoSizeScale
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\lava_jump_short 5 49
	TEnd
MEnd

Move GolemAmbush_post
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL> <PostMoveCycle>
	Interpolate	1
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\postjump 1 30
	  PlayFx        FEET\LAND1.FX
	TEnd
MEnd

#############################################################
### Spawn Pet (popup in combat)  #####
##################################

Move B_Spawn_Pet
	Requires	LOW SPAWN SPAWNEDASPET
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat>
	NextMove        B_Spawn
	Interpolate     0
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\zombie_sleep 1 2
	TEnd
MEnd

Move B_Spawn
	NextMove        B_Spawn2
	Interpolate	0

	Type		Beast
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  11
	TEnd
MEnd

Move B_Spawn2
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        B_Spawn_cycle
	Interpolate     3

	Type		Beast
	  Anim          male\zombie_monster_popup 25
	TEnd
MEnd

Move B_Spawn_cycle
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle

	Type		Beast
	  Anim          male\zombie_monster_popup 58
	TEnd
MEnd

###################
### Transform Spawn  #####
###################

Move Transform_Spawn
	Requires	QUICK SPAWN SPAWNEDASPET
	Priority	3
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	NextMove        Transform_cycle
	Interpolate     0
	Flags		NOCOLLISION NoSizeScale

	Type		Beast
	  Anim          Beast\pre_poison 40
	  PlayFx        EnemyFx\5thColumn\WereWolfTransform\WereWolfTransform.fx  0
	TEnd
MEnd

Move Transform_cycle
	Member		<Ready> <ReactIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ>
	Flags		Cycle

	Type		Beast
	  Anim          Beast\combat_cycle 1 31
	TEnd
MEnd

## AMBUSH_SLEEP ###########################################################

Move Spawn_Ambush_Sleep
	Requires	LOW SPAWN HIGH
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		CYCLE NotSelectable PICKRANDOMLY
	NextMove        Spawn_Ambush
	Interpolate     0
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\impact_belly 46 47
	TEnd
MEnd

Move Spawn_Ambush
	NextMove        KnockDown_cycle
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\getup_belly 1
	TEnd
MEnd

#############################################################

Move Spawn_Ambush_Sleep_B
	Requires	LOW SPAWN HIGH
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		CYCLE PICKRANDOMLY
	NextMove        Spawn_Ambush_B
	Interpolate     0
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\impact_back 46 47
	TEnd
MEnd

Move Spawn_Ambush_B
	NextMove        Ready
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\getup_back 1
	TEnd
MEnd


########################`
### groundburst Spawn  #####
########################

Move groundburst_sleep
	Requires	SPAWN GROUNDSUMMON
	Priority	2
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		Cycle NotSelectable
	NextMove	groundburst_Spawn
	Interpolate	0
	Sets            NOCOLLISION CANTMOVE

	Type		Beast
	  Anim          male\zombie_alt_popup 1 2
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move groundburst_Spawn
	NextMove        groundburst_land
	Interpolate     0
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        GENERIC\GROUNDBURST_ZERO.FX  12
	TEnd

MEnd

Move groundburst_land
	NextMove	groundburst_Roar
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\postjump 1 35
	  PlayFx        FEET\LAND1.FX
	TEnd
MEnd

Move groundburst_Roar
	Include sequencers\Members\Readies.inc
	NextMove	groundburst_cycle
	Interpolate	9

	Type		Beast
	  Anim          Beast\Growl 1
	TEnd
MEnd

Move groundburst_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		Beast
	  Anim          Beast\Skel_ready 1
	TEnd
MEnd


########################
### Waterburst Spawn  #####
########################

Move Waterburst_sleep
	Requires	SPAWN GROUNDSUMMON LOW
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE
	NextMove	Waterburst_Spawn
	Interpolate	0

	Type		Beast
	  Anim          male\zombie_alt_popup 1 2
	TEnd
MEnd

Move Waterburst_Spawn
	NextMove        Waterburst_land
	Interpolate     0
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\zombie_monster_popup 1 24
	  PlayFx        Volumes\Water\DeepSplash\Encounter_DeepSplash.fx 14
	TEnd
MEnd

Move Waterburst_land
	NextMove	groundburst_Roar
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\postjump 1 35
	  PlayFx        Volumes\Water\ShallowSplash\Encounter_shallowsplash.fx 1
	TEnd
MEnd

######################
###### Shivan Spawn  ######
#####################

Move Shivan_Spawn_Sleep
	Requires	LOW SPAWN WAIT
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		Cycle NotSelectable
	NextMove        Shivan_Spawn_Ambush
	Interpolate     0
	Sets            CANTMOVE NOCOLLISION

	Type		Beast
	  Anim          Beast\shivan_spawn 1 6
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Shivan_Spawn_Ambush
	Sets            CANTMOVE
	Scale           1.5
	Type		Beast
	  Anim          Beast\shivan_spawn 7 90
	  Playfx	WORLD\Nature\enc_MuckBubblesLarge.fx
	TEnd
MEnd

######################
###### Fishman Leap from water  ######
#####################

Move B_Fishman_sleep
	Requires	LOW SPAWN WAIT HIGH
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat>
	Flags		Cycle NoSizeScale NotSelectable
	NextMove        B_Fishman_Leap
	Interpolate     0
	Sets            CANTMOVE NOCOLLISION
	Scale           0.02

	Type		Beast
	  Anim          Beast\fishman_leap 1 2
	 PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move B_Fishman_Leap
	Sets            CANTMOVE
	Flags		NoSizeScale
	Interpolate	0

	Type		Beast
	  Anim          Beast\fishman_leap 3
	TEnd
MEnd

## MALTA FINISHED TRANSFORMED MEK (PRE POISON) ###########################################################

Move Double_Transformed	##finished guy
	Requires	COMBAT POISON STRONG SPAWNEDASPET
	Priority	4
	Interrupts	<AttackIRQ>
	NextMove        Double_Transformed_2
	Interpolate	0
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\double_spawn 5 6
	TEnd
MEnd

Move Double_Transformed_2
	NextMove        Double_Transformed_3
	Sets            CANTMOVE
	Interpolate	0

	Type		Beast
	  Anim          Beast\double_spawn 7 30
	  PlayFx	EnemyFx\Rularuu\Natterling\hold\BigNaterlingHit2.fx 0
	TEnd
MEnd

Move Double_Transformed_3
	Member		<DeathIRQ> <ReactIRQ>
	NextMove	Double_Transformed_cycle
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\double_spawn 31
	TEnd
MEnd

Move Double_Transformed_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		Beast
	  Anim          male\float2 1
	TEnd
MEnd

#####################
### Basic Spawn  #####
#####################

Move Shivan_Groundburst_Spawn
	Requires	SPAWN ENDGAME LOW
	Priority        3
	Interrupts	<Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
	NextMove        Shivan_Groundburst_Spawn_post
	Interpolate     0
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\shivan_spawn_reveal 1 60
	#  PlayFx	Events\Halloween\ShivanSpawn.fx 0
	  PlayFX	ENEMYFX\Unique\Shivan\GiantShivan\ShivanSpawn.fx 0
	TEnd
MEnd

Move Shivan_Groundburst_Spawn_post
	NextMove	Shivan_Groundburst_Spawn_cycle
	Flags		NoSizeScale NotSelectable
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\shivan_spawn_reveal 61
	TEnd
MEnd

Move Shivan_Groundburst_Spawn_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		Shivan
	  Anim          Beast\combat_cycle 1 31
	  PlayFX		ENEMYFX\Unique\Shivan\Shivan_BreathIdle_SFX.fx 1 CONSTANT
	TEnd
MEnd