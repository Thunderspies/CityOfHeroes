#############################################################
## Endurance.fx
#############################################################

FxInfo

LifeSpan 60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\IntelligenceRingUp.part
		sound	heal1 80 20 .8
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\IntelligenceRingUp.part
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\IntelligenceRingUp.part
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Empathy\endurance01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 40
	End

	Event
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\enduranceBodyGlows.part
		part2	CustomizeablePowers\Empathy\enduranceBodyGlowsFlat.part
		Lifespan 40
	End

	Event
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet LLEGL
		Lifespan 40
	End

	Event
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet FootL
		Lifespan 40
	End

	Event
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\enduranceBodyGlows.part
		part2	CustomizeablePowers\Empathy\enduranceBodyGlowsFlat.part
		Lifespan 40
	End

	Event
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet LlegR
		Lifespan 40
	End

	Event
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\enduranceMotionGlows.part
		PMagnet FootR
		Lifespan 40
	End

	Event
		Type	Local
		At	Hair
		part2	CustomizeablePowers\Empathy\enduranceHeadGlows.part
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet LArmL
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet WepL
		Lifespan 40
	End

	Event
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Empathy\enduranceArmGlows.part
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet LArmR
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Empathy\enduranceArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\enduranceArmMotionGlows2.part
		PMagnet WepR
		Lifespan 40
	End

	Event
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Empathy\enduranceArmGlows.part
		Lifespan 40
	End
End

#############################################################

End