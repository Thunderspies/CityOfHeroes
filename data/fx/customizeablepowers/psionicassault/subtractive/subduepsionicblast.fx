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
		#bhvr	behaviors/PsionicsScale.bhvr
		Sound psionic6 88 88 .8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		geom	EyeBeams02
		bhvr	CustomizeablePowers\PsionicAssault\PsionicsScale2.bhvr
		Lifespan 40
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		geom	EyeBeams01
		bhvr	CustomizeablePowers\PsionicAssault\PsionicsScale.bhvr
		Lifespan 40
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		Part1	CustomizeablePowers\PsionicAssault\EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		altpiv	3
		Part1	CustomizeablePowers\PsionicAssault\EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\PsionicAssault\PsionicBubbles.part
		Part2	CustomizeablePowers\PsionicAssault\PsiGlow.part
		Lifespan 40
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\PsionicAssault\EnergyCore.part
		Lifespan 25
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
		Part2	CustomizeablePowers\PsionicAssault\EnergyCoreFlash.part
		Lifespan 21
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		altpiv	2
		Part2	CustomizeablePowers\PsionicAssault\EnergyCoreFlash.part
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
		bhvr	CustomizeablePowers\PsionicAssault\MedianProjectile3.bhvr
		BhvrOverride
			TrackRate		1.7
		End
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		Part1	CustomizeablePowers\PsionicAssault\BlastStreakSubdue.part
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