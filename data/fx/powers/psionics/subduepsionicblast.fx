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
		bhvr	behaviors/PsionicsScale2.bhvr
		Lifespan 40
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		geom	EyeBeams01
		bhvr	behaviors/PsionicsScale.bhvr
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
		EName	RingsAndParticles2
		Type	Local
		At	core
		altpiv	3
		Part1	:EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		Lifespan 40
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
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

	Event
		EName	Flash2
		Type	Local
		At	core
		altpiv	2
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
		Altpiv	2
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