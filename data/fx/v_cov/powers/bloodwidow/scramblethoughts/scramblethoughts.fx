#############################################################
## Blood Widow "Scramble Thoughts" FX
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 200

## OFFSETS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterOffset
		Type	PositOnly
		At	Head
		LookAt	Target
	End

	Event
		EName	Emitter
		Type	Local
		At	EmitterOffset
		BhvrOverride
			PYRRotate	-15 0 0
			PositionOffset	  0 .2 0
		End
	End
End

## SOUND ###########################################################

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

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	core
		Type	Local
		At	Emitter
		Geom	PsionicRigging
		Lifespan 70
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		geom	EyeBeams02
		bhvr	behaviors\PsionicsScale2.bhvr
		Lifespan 70
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		geom	EyeBeams01
		bhvr	behaviors\PsionicsScale.bhvr
		Lifespan 70
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		Altpiv	1
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan 70
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		Altpiv	3
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan 70
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
		Lifespan 70
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		Part1	POWERS\Psionics\EnergyCore.part
		Lifespan 70
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
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		Lifespan 60
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		altpiv	2
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		Lifespan 60
	End
End

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	47

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		bhvr	POWERS\Psionics\MedianProjectile3.bhvr
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
		Altpiv	1
		Part1	POWERS\Psionics\BlastStreak.part
		Part2	POWERS\Psionics\ScrambleThoughtsTenticles.part
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