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
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
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
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
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