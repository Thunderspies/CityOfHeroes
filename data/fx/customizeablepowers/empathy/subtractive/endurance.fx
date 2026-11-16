#############################################################
## Endurance.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

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
		part1	:endurance01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 40
	End

	Event
		Type	Local
		At	FootR
		part1	:enduranceBodyGlows_Sub.part
		part2	:enduranceBodyGlowsFlat_Sub.part
		Lifespan 40
	End

	Event
		Type	Local
		At	ULEGL
		part1	:enduranceMotionGlows_Sub.part
		PMagnet LLEGL
		Lifespan 40
	End

	Event
		Type	Local
		At	LLEGL
		part1	:enduranceMotionGlows_Sub.part
		PMagnet FootL
		Lifespan 40
	End

	Event
		Type	Local
		At	FootL
		part1	:enduranceBodyGlows_Sub.part
		part2	:enduranceBodyGlowsFlat_Sub.part
		Lifespan 40
	End

	Event
		Type	Local
		At	ULEGR
		part1	:enduranceMotionGlows_Sub.part
		PMagnet LlegR
		Lifespan 40
	End

	Event
		Type	Local
		At	LLEGR
		part1	:enduranceMotionGlows_Sub.part
		PMagnet FootR
		Lifespan 40
	End

	Event
		Type	Local
		At	Hair
		part2	:enduranceHeadGlows_Sub.part
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmL
		part1	:enduranceArmMotionGlows_Sub.part
		part2	:enduranceArmMotionGlows2_Sub.part
		PMagnet LArmL
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmL
		part1	:enduranceArmMotionGlows_Sub.part
		part2	:enduranceArmMotionGlows2_Sub.part
		PMagnet WepL
		Lifespan 40
	End

	Event
		Type	Local
		At	WepL
		part1	:enduranceArmGlows_Sub.part
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmR
		part1	:enduranceArmMotionGlows_Sub.part
		part2	:enduranceArmMotionGlows2_Sub.part
		PMagnet LArmR
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmR
		part1	:enduranceArmMotionGlows_Sub.part
		part2	:enduranceArmMotionGlows2_Sub.part
		PMagnet WepR
		Lifespan 40
	End

	Event
		Type	Local
		At	WepR
		part1	:enduranceArmGlows_Sub.part
		Lifespan 40
	End
End

#############################################################

End