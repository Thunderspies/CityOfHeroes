#############################################################
## Blood Widow "Subdue" FX
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
		Lifespan 40
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		Bhvr	behaviors\PsionicsScale2.bhvr
		geom	EyeBeams02
		Lifespan 40
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		Bhvr	behaviors\PsionicsScale.bhvr
		geom	EyeBeams01
		Lifespan 40
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		Altpiv	1
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		Altpiv	3
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan 28
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		Altpiv	2
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
		Lifespan 40
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		Altpiv	2
		Part1	POWERS\Psionics\EnergyCore.part
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
		Altpiv	1
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		Lifespan 21
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		Altpiv	2
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		Lifespan 21
	End
End

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	20

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		Bhvr	behaviors\MedianProjectile3.bhvr
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
		Part1	POWERS\Psionics\BlastStreakSubdue.part
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