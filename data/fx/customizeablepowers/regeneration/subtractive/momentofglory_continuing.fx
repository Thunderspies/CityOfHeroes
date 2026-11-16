#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 	Time
	Time 	37

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
	Time 	37

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\MomentofGloryCollapse.part

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\Healing01.part
		Sound glory 88 88 .8

	End

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\core4.part

		#Part1	:core.part
		Part2	CustomizeablePowers\Regeneration\Electronsfast.part
		Part3	CustomizeablePowers\Regeneration\StarStreaks1.part
		Part4	CustomizeablePowers\Regeneration\StarStreaksDown1.part
		Part5	CustomizeablePowers\Regeneration\StarStreaksRightc1.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	dummy2
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\Integration01Fast.part
		Part2	CustomizeablePowers\Regeneration\CoreRings.part
		Part3	CustomizeablePowers\Regeneration\StarStreaksRightb1.part
		Part4	CustomizeablePowers\Regeneration\StarStreaksLeftb1.part
		Part5	CustomizeablePowers\Regeneration\StarStreaksLeftc1.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	dummy3
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Regeneration\RegenLightRays.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

	Event
		EName	dummy2
		Type	Local
		At	chest


		Part2	CustomizeablePowers\Regeneration\MomentOfGlorySparkliesTrail.part

	End

	Event
		EName	dummy2
		Type	Local
		At	Root

		Part1	CustomizeablePowers\Regeneration\MomentOfGlorySparkliesRising.part

	End


End

Condition
	On	Time
	Time	28

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

