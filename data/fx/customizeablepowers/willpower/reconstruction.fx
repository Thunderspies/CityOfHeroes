#########################################################
##	Fireweapon
########################################################
FxInfo
Lifespan	65

########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	dummy2
		Type	Local
		At	chest
		Part2	CustomizeablePowers\Willpower\SparkliesTrail.part

	End

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Willpower\core.part
		Part2	CustomizeablePowers\Willpower\Electrons.part
		Part3	CustomizeablePowers\Willpower\StarStreaks.part
		Part4	CustomizeablePowers\Willpower\ReconStreaksDown.part
		Part5	CustomizeablePowers\Willpower\ReconStreaksRight.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound reconstruction 60 60 .8
		Lifespan	35
	End

	Event
		EName	dummy2
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Willpower\Integration01.part
		Part2	CustomizeablePowers\Willpower\CoreRings.part
		Part3	CustomizeablePowers\Willpower\ReconStreaksRighta.part
		Part4	CustomizeablePowers\Willpower\ReconStreaksLefta.part
		Part5	CustomizeablePowers\Willpower\ReconStreaksLeft.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan	35
	End

End

Condition
	On	Time
	Time	15

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Willpower\RegenMotionGlows.part
		PMagnet Knee1
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Willpower\RegenMotionGlows.part
		PMagnet FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Willpower\RegenMotionGlows.part
		PMagnet Kneer
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Willpower\RegenMotionGlows.part
		PMagnet FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End


End
##################################

