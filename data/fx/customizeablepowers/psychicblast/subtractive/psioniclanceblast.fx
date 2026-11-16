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
		At	head
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
		At	core
		altpiv	1
		Part2	:EnergyCoreFlash.part
		Lifespan 22
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part2	:EnergyCoreFlash.part
		Lifespan 22
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
		Altpiv	2
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
		AltPiv	1
		BhvrOverride
			Alpha			60
			TintGeom		1
		End
		Geom	Tintable_Dark_PsiLance
		Part1	:BlastStreakSmall.part
		Part2	:PsionicLanceProjectile_Glow.part
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