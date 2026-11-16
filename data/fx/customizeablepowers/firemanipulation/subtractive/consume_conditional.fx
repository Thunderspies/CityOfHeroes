#############################################################
## Consume_Conditional.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On	Time
	Time	33.5 #25+8.5

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\FireManipulation\enduranceBodyGlows2a.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceBodyGlowsa.part
		PMagnet LLEGR
		LifeSpan 28
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceMotionGlowsa.part
		PMagnet LLEGL
		LifeSpan 28

	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceMotionGlowsa.part
		PMagnet FootL
		LifeSpan 28
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceBodyGlowsa.part
		PMagnet LLEGL
		LifeSpan 28
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceMotionGlowsa.part
		PMagnet LlegR
		LifeSpan 28
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceMotionGlowsa.part
		PMagnet FootR
		LifeSpan 28
	End

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\FireManipulation\enduranceHeadGlowsa.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmMotionGlowsa.part
		PMagnet LARML
		LifeSpan 28
	End

	Event
		Type	Local
		At	LARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmMotionGlowsa.part
		PMagnet WepL
		LifeSpan 28
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmGlowsa.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmMotionGlowsa.part
		PMagnet LARMR
		LifeSpan 28
	End

	Event
		Type	Local
		At	LARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmMotionGlowsa.part
		PMagnet WepR
		LifeSpan 28
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireManipulation\enduranceArmGlowsa.part
		LifeSpan 28
	End
End

#############################################################

End