#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	chest
		Sound heal1_loop 60 60 .45
		Bhvr	Behaviors\soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End

End





Condition
	On 	Time
	Time 	35

	Event
		EName	dummy2
		Type	Local
		At	chest
		Part2	CustomizeablePowers\Regeneration\SparkliesTrail.part

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	:core4.part
		Part2	:Electronsfast.part
		Part3	:StarStreaks.part
		Part4	:StarStreaksDown.part
		Part5	:StarStreaksRightc.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Pmagnet	Chest
	End

	Event
		EName	dummy2
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\Integration01Fast.part
		Part3	:StarStreaksRightb.part
		Part4	:StarStreaksLeftb.part
		Part5	:StarStreaksLeftc.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End


End
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Root
		Sound instantheal 70 70 .8
		LifeSpan	300
	End

End


Condition
	On	Time
	Time	3

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet FootL

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet Kneer

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet FootR

	End


End

End
##################################

