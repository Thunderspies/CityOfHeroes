#############################################################
## HeroStandardBlast2.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	local
		At	head
		Sound SonicScream1 80 80 .99
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
	Time 	20

	Event
		Type	start
		At	MussleFlashRigging
		altpiv	3
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	27

	Event
		EName	Prime
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	PrimeA
		Type	Local
		At	Prime
		AltPiv	1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
	End

	Event
		EName	PrimeB
		Type	Local
		At	Prime
		AltPiv	1
		Part	:SonicRingCore.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRing_Trail_Whispy1.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	Prime1
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	Prime1A
		Type	Local
		At	Prime1
		AltPiv	1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
	End

	Event
		EName	Prime1B
		Type	Local
		At	Prime1
		AltPiv	1
		Part	:SonicRingCore.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRing_Trail_Whispy1.part
	End
End

Condition
	On 	Time
	Time 	34

	Event
		EName	Prime2
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	Prime2A
		Type	Local
		At	Prime2
		AltPiv	1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
	End

	Event
		EName	Prime2B
		Type	Local
		At	Prime2
		AltPiv	1
		Part	:SonicRingCore.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRing_Trail_Whispy1.part
	End
End

Condition
	On 	Time
	Time 	41

	Event
		EName	Prime3
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	Prime3A
		Type	Local
		At	Prime3
		AltPiv	1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
	End

	Event
		EName	Prime3B
		Type	Local
		At	Prime3
		AltPiv	1
		Part	:SonicRingCore.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRing_Trail_Whispy1.part
	End
End

Condition
	On 	Time
	Time 	48

	Event
		EName	Prime4
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	Prime4A
		Type	Local
		At	Prime4
		AltPiv	1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
	End

	Event
		EName	Prime4B
		Type	Local
		At	Prime4
		AltPiv	1
		Part	:SonicRingCore.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRing_Trail_Whispy1.part
	End
End

######################################################################################

Condition
	On	PrimeHit

	Event
		EName 	Prime
		type	Destroy
	End

	Event
		EName 	PrimeA
		type	Destroy
	End

	Event
		EName 	PrimeB
		type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		EName 	Prime1
		type	Destroy
	End

	Event
		EName 	Prime1A
		type	Destroy
	End

	Event
		EName 	Prime1B
		type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName 	Prime2
		type	Destroy
	End

	Event
		EName 	Prime2A
		type	Destroy
	End

	Event
		EName 	Prime2B
		type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		EName 	Prime3
		type	Destroy
	End

	Event
		EName 	Prime3A
		type	Destroy
	End

	Event
		EName 	Prime3B
		type	Destroy
	End
End

Condition
	On	Prime4Hit

	Event
		EName 	Prime4
		type	Destroy
	End

	Event
		EName 	Prime4A
		type	Destroy
	End

	Event
		EName 	Prime4B
		type	Destroy
	End
End

#############################################################

End