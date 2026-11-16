#############################################################
## PsiBlast_SlowerCast.fx
#############################################################

FxInfo
Flags InheritAnimScale
LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterOffset
		Type	PositOnly
		At	LArmR
		LookAt	Target
	End

	Event
		EName	Emitter
		Type	Local
		At	EmitterOffset
		BhvrOverride
			PyrRotate	-10 0 0
			PositionOffset	0.0 0.25 0.5
		End
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	Sound
		Type	Local
		At	Head
		Sound scramble1 80 80 .7
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Core
		Type	Local
		At	Emitter
		Geom	PsionicRigging
		Lifespan 70
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Part1	:EnergyCoreLocal.part
		Lifespan 70
	End

	Event
		Type	Local
		At	core
		altpiv	3
		Part1	:EnergyCoreLocal.part
		Lifespan 70
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		Lifespan 70
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	:EnergyCore.part
		Lifespan 70
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	core
		altpiv	1
		Part2	:EnergyCoreFlash.part
		Lifespan 60
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part2	:EnergyCoreFlash.part
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	47

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		bhvr	CustomizeablePowers\PsychicBlast\MedianProjectile3.bhvr
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
		AltPiv	1
		Part1	:BlastStreak.part
		Part2	:ScrambleThoughtsTenticles.part
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