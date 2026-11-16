#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging
		Sound Scare_01 88 88 .9
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\MentalManipulation\EnergyCore.part
		Lifespan 25
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\MentalManipulation\PsionicBubbles.part
		Part2	CustomizeablePowers\MentalManipulation\PsiGlow.part
		Lifespan 40
	End

	Event
		Type	Local
		At	core
		altpiv	3
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocal.part
		Lifespan 28
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Flash1
		Type	Local
		At	core
		altpiv	1
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		Lifespan 21
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		altpiv	2
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		Lifespan 21
	End
End

#############################################################

End