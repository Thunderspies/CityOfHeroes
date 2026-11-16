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
		Sound psionic6 88 88 0.8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		bhvr	behaviors\PsionicsScale2.bhvr
		BhvrOverride
			Alpha			25
			TintGeom		1
		End
		geom	Tintable_Dark_EyeBeams02
		Lifespan 40
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		bhvr	behaviors\PsionicsScale.bhvr
		BhvrOverride
			Alpha			25
			TintGeom		1
		End
		geom	Tintable_Dark_EyeBeams01
		Lifespan 40
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		Part1	:EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	1
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		Lifespan 40
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	1
		Part1	:EnergyCore.part
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
		Part2	:EnergyCoreFlash.part
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
		Altpiv	1
		Geom	FrontFacingLocalPivot
		bhvr	behaviors\MedianProjectile3.bhvr
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
		Part1	:BlastStreakSubdue.part
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