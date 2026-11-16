#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Core
		Type	Local
		At	head
		Geom	PsionicRigging
		#LookAt	Target
		Lifespan 60
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	head
		Sound psionic4 88 88 .8
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	RingsAndParticles
		Type	Create
		Inherit	All
		Update	Position
		At	Core
		Altpiv	2
		Part1	CustomizeablePowers\PsionicAssault\PsionicBubbles.part
		Part2	CustomizeablePowers\PsionicAssault\PsiGlow.part
		LifeSpan 20
	End

	Event
		EName	RingsAndParticles1
		Type	Create
		Inherit	All
		Update	Position
		At	Core
		altpiv	1
		Part1	CustomizeablePowers\PsionicAssault\EnergyCoreLocal.part
		LifeSpan 5
	End

	Event
		EName	RingsAndParticles2
		Type	Create
		Inherit	All
		Update	Position
		At	Core
		altpiv	3
		Part1	CustomizeablePowers\PsionicAssault\EnergyCoreLocal.part
		LifeSpan 5
	End

	Event
		EName	RingsAndParticles3
		Type	Create
		Inherit	All
		Update	Position
		At	Core
		altpiv	2
		Part1	CustomizeablePowers\PsionicAssault\EnergyCore.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		Bhvr	CustomizeablePowers\PsionicAssault\PsiProjectile_01.bhvr
		Geom	FrontFacingLocalPivot
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		Geom	PsiLance
		Part1	CustomizeablePowers\PsionicAssault\BlastStreakSmall.part
		Part2	CustomizeablePowers\PsionicAssault\BlastStreakGlow.part
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