#############################################################
## InstantHealing.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound instantheal 70 70 0.8
		LifeSpan 100
	End

End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\soundFade1.bhvr
		Sound heal1_loop 60 60 0.45
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\Regeneration\SparkliesTrail.part
		Part1	CustomizeablePowers\Regeneration\Ring01.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	dummy
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Regeneration\core2.part
		Part2	CustomizeablePowers\Regeneration\Electronsfast.part
		Part3	CustomizeablePowers\Regeneration\StarStreaks.part
		Part4	CustomizeablePowers\Regeneration\StarStreaksDown.part
		Part5	CustomizeablePowers\Regeneration\StarStreaksRightc.part
		LifeSpan 30
	End

	Event
		EName	dummy2
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Regeneration\Integration01Fast.part
		Part2	CustomizeablePowers\Regeneration\CoreRings.part
		Part3	CustomizeablePowers\Regeneration\StarStreaksRightb.part
		Part4	CustomizeablePowers\Regeneration\StarStreaksLeftb.part
		Part5	CustomizeablePowers\Regeneration\StarStreaksLeftc.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet LLEGL
		LifeSpan 30
	End

	Event
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet FootL
		LifeSpan 30
	End

	Event
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet LLEGR
		LifeSpan 30
	End

	Event
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Regeneration\RegenMotionGlows.part
		PMagnet FootR
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part
		LifeSpan 30
	End
End

#############################################################

End