#############################################################
## MassiveSonicBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	local
		At	head
		Sound Sonic14 80 80 .99
	End
End

Condition
	On 	Time
	Time 	37

	Event
		Type	local
		At	head
		Sound SonicMassiveBlast 100 100 .99
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
		Type	Local
		At	Head
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	MussleFlashRigging
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\SonicAttack\MussleFlashRingCore.part
		Part	CustomizeablePowers\SonicAttack\MussleFlashRing.part
		LifeSpan 11
	End
End

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	Prime
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\Projectile.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		30
			PhysForcePower		50
			PhysForcePowerJitter	10
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
		Part	:MassiveSonicRingCore.part
		Part	:MassiveSonicRingCoreTrail.part
		Part	:MassiveSonicRingGlowTrail.part
		Part	:MassiveSonicRingGlow.part
		Part	:MassiveSonicRingCoreTrailBrightGreen2.part
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
		EName 	PrimeA
		type	Destroy
	End
End

#############################################################

End