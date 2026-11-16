#############################################################
## deaths.inc
#############################################################

## DEATH ###########################################################

Move Default_Death
	Requires	DEATH
	Priority	1
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ> <RaiseDeadIRQ>
	NextMove        Default_postdeath
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 47 129
	TEnd

	Type		Electro_Beast
	  Anim          Beast\electro 1
	  PlayFX	POWERS\Red_ElectricityControl\Cap_death.fx
	TEnd

	Type		Shivan
	  Anim          Beast\death_front 3 122
#	  PlayFX	ENEMYFX\Unique\Shivan\GiantShivan\Death.fx
	  PlayFX	ENEMYFX\Unique\Shivan\Shivan_Death.fx 0 constant
	TEnd
MEnd

Move Default_postdeath
	Requires	DEATH
	Priority	1
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 129 130
	TEnd

	Type		Electro_Beast
	  Anim          Beast\electro 1
	TEnd
MEnd

###############################
###### SPECIAL VANISH DEATH   ######
###############################

Move R_Vanish_Death
	Requires	DEATH ROCK
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        R_Vanish_postdeath
	Priority	6
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		MindsOfMayhem_CrystalTitan
	  Anim          Beast\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd



MEnd

Move R_Vanish_postdeath
	Requires	DEATH ROCK
	Flags		Cycle
	Priority	6
	Sets            NOCOLLISION DIENOW

	Type		Beast
	  Anim          Beast\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		MindsOfMayhem_CrystalTitan
	  Anim          Beast\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

#######################
###### Vanish Death   ######
######################

Move Vanish_Death
	Requires	DEATH HIDE
	Priority	10
	Interrupts	<DeathIRQ>
	NextMove        Vanish_postdeath
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		Beast
	  Anim          Beast\freeze 20 30
	  PlayFx	GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Vanish_postdeath
	Requires	DEATH
	Priority	9
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		Beast
	  Anim          Beast\freeze 20 30
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

###################
###### Hit Death  ######
###################

Move HitDeath4
	Requires	HIT DEATH
	Priority	2
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        postdeath4
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 3 122
	TEnd

	Type		Electro_Beast
	  Anim          Beast\electro 1
	  PlayFX	POWERS\Red_ElectricityControl\Cap_death.fx
	TEnd

	Type		Shivan
	  Anim          Beast\death_front 3 15
#	  PlayFX	ENEMYFX\Unique\Shivan\GiantShivan\Death.fx
	  PlayFX	ENEMYFX\Unique\Shivan\Shivan_Death.fx 0
	TEnd

	Type		MindsOfMayhem_Deathsurge
	  Anim          Beast\electro 1
	  PlayFX	POWERS\Red_ElectricityControl\Cap_death.fx
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx 0
	TEnd

	Type		MindsOfMayhem_Jurassik
	  Anim          Beast\death_front 3 122
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_LordWinter
	  Anim          Beast\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

MEnd

Move postdeath4
	Requires	DEATH
	Priority	1
	Flags		Cycle notselectable
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 123 124
	TEnd

	Type		Electro_Beast
	  Anim          Beast\electro 1
	TEnd

	Type		Shivan
	  Anim          Beast\death_front 15 15
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		MindsOfMayhem_Jurassik
	  Anim          Beast\death_front 123 124
	TEnd

	Type		MindsOfMayhem_LordWinter
	  Anim          Beast\electro 1
	TEnd

MEnd

###################
###### Electro Death  ######
###################

Move ElectroDeath
	Requires	HIT DEATH ENERGY
	Priority	3
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        ElectroDeath2
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\electro 1
	TEnd
MEnd

Move ElectroDeath2
	Requires	DEATH
	Priority	1
	NextMove        ElectroDeath3
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 53 127
	TEnd
MEnd

Move ElectroDeath3
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 128 129
	TEnd
MEnd

###################
###### Disrupt Death  ######
###################

Move DisruptDeath
	Requires	HIT DEATH DISRUPT
	Priority	3
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        DisruptDeath2
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\electro 1 47
	TEnd
MEnd

Move DisruptDeath2
	Requires	DEATH
	Priority	1
	NextMove        DisruptDeath3
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 53 129
	TEnd
MEnd

Move DisruptDeath3
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 129 130
	TEnd
MEnd

###################
###### Choke Death  ######
###################

Move ChokeDeath
	Requires	HIT DEATH MAGIC
	Priority	3
	Interrupts	<DeathIRQ> <BasicDeath>
	Member		<ExplodeIRQ>
	NextMove        ChokeDeath2
	Interpolate     8
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\electro 1 47
	TEnd
MEnd

Move ChokeDeath2
	Requires	DEATH
	Priority	1
	NextMove        ChokeDeath3
	Interpolate     12
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 46
	TEnd
MEnd

Move ChokeDeath3
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 128 129
	TEnd
MEnd

###################
###### Sonic Death  ######
###################

Move SonicDeath
	Requires	HIT DEATH PSIONIC
	Priority	3
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        SonicDeath2
	Interpolate     8
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\electro 1 47
	TEnd
MEnd

Move SonicDeath2
	Requires	DEATH
	Priority	1
	NextMove        SonicDeath3
	Interpolate     12
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\death_front 46
	TEnd
MEnd

Move SonicDeath3
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\death_front 128 129
	TEnd
MEnd

###################
###### Speed Death  ######
###################

Move SpeedDeath
	Requires	HIT DEATH SPEEDPUNCH
	Priority	3
	Interrupts	<DeathIRQ>
	NextMove        SpeedDeath2
	Scale           0.75
	Interpolate     2
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\speed_react 3 65
	TEnd
MEnd

Move SpeedDeath2
	Requires	DEATH
	Priority	1
	Member		<ExplodeIRQ>
	NextMove        SpeedDeath3
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\knockdown 3 38
	TEnd
MEnd

Move SpeedDeath3
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\knockdown 38 40
	TEnd
MEnd

###################
###### Kungfu_speed Death  ######
###################

Move KungfuSpeed_death
	Requires	HIT DEATH KUNGFUSPEED
	Interrupts	<ReactIRQ>
	NextMove        KungfuSpeed_death2
	Priority	3

	Type		Beast
	  Anim          Beast\speed_react 3 39
	TEnd
MEnd

Move KungfuSpeed_death2
	Requires	DEATH
	NextMove        KungfuSpeed_death3
	Priority	1
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\drain 40 52
	TEnd
MEnd

Move KungfuSpeed_death3
	Requires	DEATH
	Member		<ExplodeIRQ>
	NextMove        KungfuSpeed_death4
	Priority	1
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\knockdown 3 38
	TEnd
MEnd

Move KungfuSpeed_death4
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Interpolate     0
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\knockdown 39 40
	TEnd
MEnd

###################
###### Claw_speed Death  ######
###################

Move ClawSpeed_death
	Requires	HIT DEATH CLAWSPEED
	Priority	3
	Interrupts	<ReactIRQ>
	NextMove        ClawSpeed_death2

	Type		Beast
	  Anim          Beast\speed_react 3 19
	TEnd
MEnd

Move ClawSpeed_death2
	Requires	DEATH
	Priority	1
	NextMove        ClawSpeed_death3
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\drain 40 53
	TEnd
MEnd

Move ClawSpeed_death3
	Requires	DEATH
	Priority	1
	Member		<ExplodeIRQ>
	NextMove        ClawSpeed_death4
	Sets            NOCOLLISION

	Type		Beast
	  Anim          Beast\knockdown 3 38
	TEnd
MEnd

Move ClawSpeed_death4
	Requires	DEATH
	Priority	1
	Flags		Cycle
	Interpolate     0
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          Beast\knockdown 39 40
	TEnd
MEnd

######## air deaths  #############################################

###################
###### Default air Death   ######
###################

Move Air_Death
	Requires	AIR DEATH
	Interrupts	<DeathIRQ>
	Flags		ReqInputs
	NextMove        Air_death_fall
	Priority	3

	Type		Beast
	  Anim          male\air_death 6 51
	TEnd

MEnd


Move Air_death_fall
	Requires	AIR DEATH
	Flags		Cycle
	NextMove        Air_death_impact
	Priority	2

	Type		Beast
	  Anim          male\air_death 53 78
	TEnd

MEnd


Move Air_death_impact
	Requires	DEATH
	NextMove        Air_death_cycle
	Priority	1
	Sets            NOCOLLISION

	Type		Beast
	  Anim          male\impact_belly 4 47
	TEnd

MEnd


Move Air_death_cycle
	Requires	DEATH
	Flags		Cycle
	Priority	1
	Sets            NOCOLLISION DIENOW TARGET

	Type		Beast
	  Anim          male\impact_belly 48 50
	TEnd

MEnd

###############################################
###### Merge Natterling Death (the sick guy)   ######
###############################################

Move Transform_death
	Requires	HEAL MENACE STRONG DEATH
	Interrupts	<AttackIRQ> <ReactIRQ> <DeathIRQ>
	NextMove        Transform_death_cycle
	Priority	7
	Sets            CANTMOVE

	Type		Beast
	  Anim          5cw\malta_merge 1 30
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move Transform_death_cycle
	Member		<DeathIRQ>
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW CANTMOVE

	Type		Beast
	  Anim          5cw\titan_transform_merge_A 10
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


