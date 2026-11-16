#############################################################
## MinorDamageProjectile.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Core
		Type	Local
		At	LArmR
		BhvrOverride
			PyrRotate	-70 30 0
			PositionOffset	0.0 0.2 0.0

		End

		Lifespan 60
	End

	Event
		EName	Core2
		Type	Local
		At	LArmR
		BhvrOverride
			PyrRotate	-70 30 0
			PositionOffset	1.5 0.0 0.0
		End
		Lifespan 60
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	WepR
		Sound psionic4 88 88 .8
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	RingsAndParticles
		Type	Create
		Inherit	All
		Update	Position
		At	Core
		Part	CustomizeablePowers\PsychicBlast\Subtractive\PsiGlow.part
		LifeSpan 20
	End

	Event
		EName	RingsAndParticles3
		Type	Create
		Inherit	All
		Update	Position
		At	Core2
		Part	CustomizeablePowers\PsychicBlast\Subtractive\EnergyCore.part
		LifeSpan 10
	End
End

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	25

	Event
		EName	prime
		Type	Start
		At	Core2
		Bhvr	CustomizeablePowers\PsychicBlast\PsiProjectile_01.bhvr
		Geom	FrontFacingLocalPivot
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PsiLance
		Part1	CustomizeablePowers\PsychicBlast\Subtractive\BlastStreakSmall.part
		Part2	CustomizeablePowers\PsychicBlast\Subtractive\BlastStreakGlow.part
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