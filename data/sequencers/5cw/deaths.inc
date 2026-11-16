#### Deaths  ########################################################################


#######################
###### Vanish Death   ######
######################

Move Vanish_Death
	Requires	DEATH HIDE
	Priority        5
	Interrupts	<DeathIRQ>
	NextMove        Vanish_postdeath
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		5cw
	  Anim          5cw\freezestun 20 30
	 PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Vanish_postdeath
	Requires	DEATH
	Priority        1
	Member		<RaiseDeadIRQ>
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW
	Flags		NoDraw

	Type		5cw
	  Anim          5cw\freezestun 20 30
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

###################
###### Default Death   ######
###################

Move Default_Death
	Requires	DEATH
	Interrupts	<DeathIRQ>
	NextMove        Default_postdeath
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\death_front 47 129
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\death_front 47 129
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd
MEnd

Move Default_postdeath
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_front 129 130
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\death_front 129 130
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

MEnd

###################
###### Hit Death   ######
###################

Move HitDeath
	Requires	HIT DEATH
	Priority        2
	Interrupts	<DeathIRQ> <ReactIRQ>
	NextMove        postdeath2
	Sets            DIENOW TARGET

	Type		5cw
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\Maltadeath3.FX
	TEnd

	Type		clockwork_assembler
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\clockdeath1.FX
	TEnd

	Type		clockwork_king
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\clockdeath2.FX
	TEnd

	Type		Malta_Kronos
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\Maltadeath1.FX
	TEnd

	Type		Malta_Hercules
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\Maltadeath3.FX
	TEnd

	Type		BPTotem
	  Anim          5cw\death_sideL 1 99
	  PlayFx        FEET\BPTotemDeathSide.fx
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	  PlayFx        FEET\Maltadeath1.FX
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd



MEnd

Move postdeath2
	Requires	DEATH
	Priority        1
	Flags		Cycle
	Sets            NOCOLLISION
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_sideL 99 100
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	TEnd

MEnd

###############################################################################
###### Hit Death 5 ######  this is a new death animation so I added it for the other mech's to use randomly
#########################################################################################

Move HitDeath5
	Requires	HIT DEATH
	Priority        2
	Interrupts	<DeathIRQ> <ReactIRQ>
	NextMove        Death_PostCycle2
	Sets            DIENOW TARGET

	Type		5cw
	  Anim          5cw\goliath_warwalker_death 1 71
	  PlayFx        FEET\Maltadeath3.FX
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	  PlayFx        FEET\Maltadeath1.FX
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd
MEnd

Move Default_Death2
	Requires	DEATH
	Priority	1
	Interrupts	<DeathIRQ>
	NextMove        Death_PostCycle2
	Sets		DIENOW TARGET

	Type		5cw
	  Anim          5cw\goliath_warwalker_death 1 71
	  PlayFx        FEET\Maltadeath3.FX
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	TEnd
MEnd

Move Death_PostCycle2
	Requires	DEATH
	Priority        1
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\goliath_warwalker_death 71 75
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### Hit Death 4  ######
###################

Move HitDeath4
	Requires	HIT DEATH
	Priority        2
	Interrupts	<DeathIRQ> <ReactIRQ>
	NextMove        postdeath4
	Sets            DIENOW TARGET
	Flags		nosizescale

	Type		5cw
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\maltadeath4.FX
	TEnd

	Type		clockwork_assembler
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\clockdeath1.FX
	TEnd

	Type		clockwork_king
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\clockdeath2.FX
	TEnd


	Type		Malta_kronos
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\maltadeath2.FX
	TEnd

	Type		Malta_Hercules
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\maltadeath4.FX
	TEnd

	Type		BPTotem
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\BPTotemDeath.fx
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	  PlayFx        FEET\Maltadeath1.FX
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd
MEnd

Move postdeath4
	Requires	DEATH
	Priority        1
	Flags		Cycle
#	Sets            DIENOW TARGET NOCOLLISION
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_front 123 124
	TEnd

	Type		MindsOfMayhem_Babbage
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_KronosTitan
	  Anim          5cw\electro 1
	TEnd

	Type		MindsOfMayhem_Paladin
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### Electro Death  ######
###################

Move ElectroDeath
	Requires	HIT DEATH ENERGY
	Interrupts	<DeathIRQ>
	NextMove        ElectroDeath2
	Priority        3
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

Move ElectroDeath2
	Requires	DEATH
	NextMove        ElectroDeath3
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\death_front 53 127
	TEnd
MEnd


Move ElectroDeath3
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            DIENOW TARGET NOCOLLISION
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_front 128 129
	TEnd
MEnd

###################
###### Disrupt Death  ######
###################

Move DisruptDeath
	Requires	HIT DEATH DISRUPT
	Interrupts	<DeathIRQ>
	NextMove        DisruptDeath2
	Priority        3
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\electro 1 47
	TEnd
MEnd


Move DisruptDeath2
	Requires	DEATH
	NextMove        DisruptDeath3
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\death_sideL 1 99
	TEnd
MEnd


Move DisruptDeath3
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            DIENOW TARGET
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_sideL 99 100
	TEnd
MEnd

###################
###### Choke Death  ######
###################

Move ChokeDeath
	Requires	HIT DEATH MAGIC
	Interrupts	<DeathIRQ> <BasicDeath>
	NextMove        ChokeDeath2
	Priority        3
	Interpolate     8
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\electro 1 47
	TEnd
MEnd


Move ChokeDeath2
	Requires	DEATH
	NextMove        ChokeDeath3
	Priority        1
	Interpolate     12
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\death_front 46
	TEnd
MEnd


Move ChokeDeath3
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            DIENOW TARGET
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_front 128 129
	TEnd
MEnd

###################
###### Sonic Death  ######
###################

Move SonicDeath
	Requires	HIT DEATH PSIONIC
	Interrupts	<DeathIRQ>
	NextMove        SonicDeath2
	Priority        3
	Interpolate     8
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\electro 1 47
	TEnd
MEnd


Move SonicDeath2
	Requires	DEATH
	NextMove        SonicDeath3
	Priority        1
	Interpolate     12
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\death_front 46
	TEnd
MEnd


Move SonicDeath3
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            DIENOW TARGET
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          5cw\death_front 128 129
	TEnd
MEnd

###################
###### Speed Death  ######
###################

Move SpeedDeath
	Requires	HIT DEATH SPEEDPUNCH
	Interrupts	<DeathIRQ>
	NextMove        SpeedDeath2
	Priority        3
	Scale           0.75
	Interpolate     2
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\speed_react 3 65
	TEnd
MEnd


Move SpeedDeath2
	Requires	DEATH
	NextMove        SpeedDeath3
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\knockbackdown 3 38
	TEnd
MEnd


Move SpeedDeath3
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Interpolate     0
	Sets            DIENOW TARGET
	NextMove	getup_back

	Type		5cw
	  Anim          5cw\knockbackdown 39 40
	TEnd
MEnd

###################
###### Kungfu_speed Death  ######
###################

Move KungfuSpeed_death
	Requires	HIT DEATH KUNGFUSPEED
	Interrupts	<ReactIRQ>
	NextMove        KungfuSpeed_death2
	Priority        3

	Type		5cw
	  Anim          5cw\speed_react 3 39
	TEnd
MEnd


Move KungfuSpeed_death2
	Requires	DEATH
	NextMove        KungfuSpeed_death3
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\drain 40 52
	TEnd
MEnd


Move KungfuSpeed_death3
	Requires	DEATH
	NextMove        KungfuSpeed_death4
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\knockbackdown 3 38
	TEnd
MEnd


Move KungfuSpeed_death4
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Interpolate     0
	Sets            DIENOW TARGET
	NextMove	getup_back

	Type		5cw
	  Anim          5cw\knockbackdown 39 40
	TEnd
MEnd

###################
###### Claw_speed Death  ######
###################

Move ClawSpeed_death
	Requires	HIT DEATH CLAWSPEED
	Interrupts	<ReactIRQ>
	NextMove        ClawSpeed_death2
	Priority        3

	Type		5cw
	  Anim          5cw\speed_react 3 19
	TEnd
MEnd


Move ClawSpeed_death2
	Requires	DEATH
	NextMove        ClawSpeed_death3
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\drain 40 53
	TEnd
MEnd


Move ClawSpeed_death3
	Requires	DEATH
	NextMove        ClawSpeed_death4
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\knockbackdown 3 38
	TEnd
MEnd


Move ClawSpeed_death4
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Interpolate     0
	Sets            DIENOW TARGET
	NextMove	getup_back

	Type		5cw
	  Anim          5cw\knockbackdown 39 40
	TEnd
MEnd


###################
### Dismiss Death   ####
###################


Move DismissDeath
	Requires	DISMISS DEATH
	Interrupts	<DeathIRQ>
	NextMove        DismissDeath_cycle
	Priority        9
	Scale           0.75
	Interpolate     2
	Flags		NotSelectable NOCOLLISION

	Type		5cw
	  Anim          5cw\dismiss 1 150
##	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	  PlayFX	V_COV\Powers\RobotSpawns\Robot_Dismiss.fx 0
	  PlayFx        FEET\MM_Bot_FF_Left.FX 17
	  PlayFx        FEET\MM_Bot_FF_Right.FX 37

	TEnd
MEnd

Move DismissDeath_cycle
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Priority        1
	Interpolate     0
	Flags		NotSelectable NOCOLLISION Cycle

	Type		5cw
	  Anim          5cw\dismiss 149 150
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
##	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	TEnd
MEnd


###############################################
###### Malta Trans Death (the sick guy)   ######
###############################################

Move Malta_Transform_death
	Requires	HEAL MENACE STRONG DEATH
	Interrupts	<AttackIRQ> <ReactIRQ> <DeathIRQ>
	NextMove        Malta_Transform_death_cycle
	Priority        7
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\malta_merge 1 30
	  PlayFx        ENEMYFX\Malta\TitanTransformFX\TitanTransform.fx 0 CONSTANT
	TEnd
MEnd

Move Malta_Transform_death_cycle
	Member		<DeathIRQ>
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		5cw
	  Anim          5cw\titan_transform_merge_A 10
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd
######## air deaths  #############################################

###################
###### Default air Death   ######
###################

Move Air_Death
	Requires	FLY DEATH
	Priority        3
	Interrupts	<DeathIRQ>
	NextMove        Air_Death2

	Type		5cw
	  Anim          male\air_death 6 7
	  Ragdoll	1
	TEnd
MEnd

Move Air_Death2
	Requires	AIR DEATH
	Priority        2
	NextMove        Air_death_fall

	Type		5cw
	  Anim          male\air_death 8 51
	  Ragdoll	1
	TEnd
MEnd

Move Air_death_fall
	Requires	AIR DEATH
	Priority        2
	Flags		Cycle
	NextMove        Air_death_impact

	Type		5cw
	  Anim          male\air_death 53 78
	  Ragdoll	1
	TEnd
MEnd

Move Air_death_impact
	Requires	DEATH
	Priority        1
	NextMove        Air_death_cycle
	Sets            NOCOLLISION

	Type		5cw
	  Anim          male\impact_belly 4 47
	  Ragdoll	1
	TEnd
MEnd

Move Air_death_cycle
	Requires	DEATH
	Priority        1
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET
	Wind		DEATHWIND
	NextMove	Revive_belly_Pre_rooted

	Type		5cw
	  Anim          male\impact_belly 48 50
	  Ragdoll	1
	TEnd
MEnd