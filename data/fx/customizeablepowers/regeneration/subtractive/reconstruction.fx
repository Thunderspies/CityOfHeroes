#############################################################
## Reconstruction.fx
#############################################################

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
		Part2	:SparkliesTrail.part

	End

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	:core.part
		Part2	:Electrons.part
		Part3	:StarStreaks.part
		Part4	:ReconStreaksDown.part
		Part5	:ReconStreaksRight.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound reconstruction 60 60 .8
		Lifespan	35
	End

	Event
		EName	dummy2
		Type	Local
		At	chest

		Part1	:Integration01.part
		Part2	:CoreRings.part
		Part3	:ReconStreaksRighta.part
		Part4	:ReconStreaksLefta.part
		Part5	:ReconStreaksLeft.part
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
		part1	:RegenBodyGlows.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:RegenBodyGlows.part
		PMagnet Knee1
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:RegenMotionGlows.part
		PMagnet FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:RegenBodyGlows.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:RegenMotionGlows.part
		PMagnet Kneer
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:RegenMotionGlows.part
		PMagnet FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End


End
##################################

