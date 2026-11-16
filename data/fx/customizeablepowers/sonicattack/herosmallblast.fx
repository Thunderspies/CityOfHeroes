#############################################################
## HeroSmallBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	head
		Sound sonicsmallblast3 80 80 .97
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local
		At	head
		Geom	PsiTele02
	End

	Event
		EName 	MussleFlashRigging
		Type	local
		At	head
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	MussleFlashRigging
		altpiv	3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SonicAttack\MussleFlashRingCore.part
		Part	CustomizeablePowers\SonicAttack\MussleFlashRing.part
		LifeSpan 8
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		EName	prime
		Type	Start
		At	rigging
		Altpiv	1
		bhvr	CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		20
			PhysForcePowerJitter	10
			##TrackRate		0.5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	p2
		Type	Local
		At	Prime
		AltPiv	1
		Part	CustomizeablePowers\SonicAttack\SMLSonicRingCore.part
		Part	CustomizeablePowers\SonicAttack\SMLSonicRingCoreTrail.part
		Part	CustomizeablePowers\SonicAttack\SMLSonicRingGlow.part
		Part	CustomizeablePowers\SonicAttack\SMLSonicRingGlowTrail.part
		Part	CustomizeablePowers\SonicAttack\SMLSonicRingCoreTrailBrightGreen.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName 	Prime
		type	Destroy
	End

	Event
		EName 	P2
		type	Destroy
	End
End

#############################################################

End