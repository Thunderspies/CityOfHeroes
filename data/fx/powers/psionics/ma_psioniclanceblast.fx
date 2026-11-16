#############################################################
## PsionicLanceBlast.fx
#############################################################

FxInfo

LifeSpan 110

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	WindupFX
		Type	Local
		At	Root
		ChildFX	:PsionicLanceBlastWindUp.fx
	End
End

Condition
	On	Time
	Time	90

	Event
		EName	core
		Type	Local
		At	head
		#bhvr	behaviors/PsionicsScale.bhvr
		Geom	PsionicRigging
		Sound psionic4 88 88 .8
		Lifespan 32
	End
End

Condition
	On	Time
	Time	90

	Event
		EName	Flash1
		Type	Local
		At	core
		altpiv	1
		Part2	:EnergyCoreFlash.part
		Lifespan 22
	End

	Event
		EName	Flash2
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
	Time	90

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors\PsionicLance.bhvr
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
		Geom	PsiLance
		Part1	:BlastStreakSmall.part
		Part2	:BlastStreakGlow.part
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