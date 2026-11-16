#############################################################
## PsionicBlast_SlowCast.fx
#############################################################

FxInfo

Flags	InheritAnimScale

LifeSpan 260

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterOffset
		Type	PositOnly
		At	Head
		LookAt	Target
		LifeSpan 60
	End

	Event
		EName	Emitter
		Type	Local
		At	EmitterOffset
		BhvrOverride
			PYRRotate	-15 0 0
			PositionOffset	  0 .2 0
		End
		LifeSpan 60
	End
End

###########################################################

Condition
	On	Time
	Time	30

	Event
		EName	core
		Type	Local
		At	Emitter
		Geom	PsionicRigging
		Sound psionic3 80 80 .9
		LifeSpan 90
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		LifeSpan 60
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		Part1	:EnergyCoreLocal.part
		LifeSpan 18
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		Part1	:EnergyCore.part
		LifeSpan 18
	End
End

Condition
	On	Time
	Time	40

	Event
		EName	Flash1
		Type	Local
		At	core
		altpiv	1
		Part2	:EnergyCoreFlash.part
		LifeSpan 60
	End
End

#############################################################

Condition
	On	Time
	Time	50

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		bhvr	:Medianprojectile3.bhvr
		BhvrOverride
			TrackRate		1.7
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv 1
		Part1	:BlastStreak.part
		Part2	:BlastStreakGlowHead.part
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