#############################################################
## SubduePsionicBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging
		Sound psionic6 88 88 .8
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

Condition
	On	Time
	Time	20

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		Geom	FrontFacingLocalPivot
		bhvr	CustomizeablePowers\MentalManipulation\MedianProjectile3.bhvr
		BhvrOverride
			TrackRate 1.7
		End
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		Part1	CustomizeablePowers\MentalManipulation\BlastStreakSubdue.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End