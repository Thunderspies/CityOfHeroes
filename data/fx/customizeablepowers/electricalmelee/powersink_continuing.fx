#############################################################
## PowerSink_Continuing.FX
#############################################################

FxInfo

Lifespan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest
		bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\ElectricityManipulation\enduranceBodyGlows2a.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	FootR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceBodyGlowsa.part
		PMagnet LLEGR
		LifeSpan 28
	End

	Event
		Type	Local
		At	ULEGL
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceMotionGlowsa.part
		PMagnet LLEGL
		LifeSpan 28
	End

	Event
		Type	Local
		At	LLEGL
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceMotionGlowsa.part
		PMagnet FootL
		LifeSpan 28
	End

	Event
		Type	Local
		At	FootL
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceBodyGlowsa.part
		PMagnet LLEGL
		LifeSpan 28
	End

	Event
		Type	Local
		At	ULEGR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceMotionGlowsa.part
		PMagnet LlegR
		LifeSpan 28

	End

	Event
		Type	Local
		At	LLEGR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceMotionGlowsa.part
		PMagnet FootR
		LifeSpan 28
	End

	Event
		Type	Local
		At	Hair
		bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\ElectricityManipulation\enduranceHeadGlowsa.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	UARML
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmMotionGlowsa.part
		PMagnet LARML
		LifeSpan 28
	End

	Event
		Type	Local
		At	LARML
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmMotionGlowsa.part
		PMagnet WepL
		LifeSpan 28
	End

	Event
		Type	Local
		At	WepL
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmGlowsa.part
		LifeSpan 28
	End

	Event
		Type	Local
		At	UARMR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmMotionGlowsa.part
		PMagnet LARMR
		LifeSpan 28
	End

	Event
		Type	Local
		At	LARMR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmMotionGlowsa.part
		PMagnet WepR
		LifeSpan 28
	End

	Event
		EName	15
		Type	Local
		At	WepR
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\ElectricityManipulation\enduranceArmGlowsa.part
		LifeSpan 28
	End
End

#############################################################

End