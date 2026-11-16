#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	CustomizeablePowers\MentalManipulation\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	CustomizeablePowers\MentalManipulation\SpinningHandRings.part
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End