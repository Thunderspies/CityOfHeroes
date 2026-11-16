#############################################################
## PsionicLanceBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	core
		Type	Local
		At	WepR
		#bhvr	behaviors\PsionicsScale.bhvr
		Geom	PsionicRigging
		Sound psionic4 88 88 .8
		Lifespan 32
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Core
		altpiv	1
		Part2	CustomizeablePowers\PsychicBlast\EnergyCoreFlash.part
		Lifespan 15
	End

	Event
		Type	Local
		At	Core
		altpiv	2
		Part2	CustomizeablePowers\PsychicBlast\EnergyCoreFlash.part
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	prime
		Type	Start
		At	Core
	#	Altpiv	2
		Geom	FrontFacingLocalPivot
		bhvr	CustomizeablePowers\PsychicBlast\PsionicLance.bhvr
		BhvrOverride
			TrackRate		3.5
		End
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
	#	AltPiv	1
		BhvrOverride
			Alpha			25
			TintGeom		1
		End
		Geom	tintable_PsiLance
		Part1	CustomizeablePowers\PsychicBlast\BlastStreakSmall.part
		Part2	CustomizeablePowers\PsychicBlast\PsionicLanceProjectile_Glow.part
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