#############################################################
## Blood Widow "Mental Blast" FX
#############################################################

FxInfo

Flags InheritAnimScale

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

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	core
		Type	Local
		At	Emitter
		Geom	PsionicRigging
		Sound psionic3 80 80 .9
		LifeSpan 60
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		geom	EyeBeams02
		bhvr	behaviors\PsionicsScale2.bhvr
		LifeSpan 60
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		geom	EyeBeams01
		bhvr	behaviors\PsionicsScale.bhvr
		LifeSpan 60
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		Altpiv	2
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
		LifeSpan 60
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		Altpiv	1
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		LifeSpan 18

	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		Altpiv	3
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		LifeSpan 18

	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		Altpiv	2
		Part1	POWERS\Psionics\EnergyCore.part
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
		Altpiv	1
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		LifeSpan 60
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		Altpiv	2
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		LifeSpan 60
	End
End

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	50

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		BhvrOverride
			TrackRate		1.7
		End
		Bhvr	POWERS\Psionics\Medianprojectile3.bhvr
		Geom	FrontFacingLocalPivot
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		Altpiv	1
		Part1	POWERS\Psionics\BlastStreak.part
		Part2	POWERS\Psionics\BlastStreakGlowHead.part
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