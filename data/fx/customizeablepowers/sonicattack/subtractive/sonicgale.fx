#############################################################
## SonicGale.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local
		At	Root
		Geom	PsiTele02
		Sound sonicgale 100 100 .88
	End

	Event
		EName 	MussleFlashRigging
		Type	local
		At	head
		Geom	PsionicRigging
	End
End

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		EName 	MussleFlash
		Type	start
		At	MussleFlashRigging
		Altpiv	3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 20
	End

End

#############################################################

Condition
	On 	Time
	Time 	33

	Event
		EName	Prime
		Type	Start
		At	rigging
		Altpiv	1
		Bhvr	CustomizeablePowers\SonicAttack\GaleProjectile.bhvr
		Geom	FrontFacingLocalPivot
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	PrimeA
		Type	Local
		At	Prime
		Altpiv	1
		Part	:SonicCoreElipse.part
		Part	:SonicElipse.part
		Part	:GaleMist.part
	End

	Event
		Ename	PrimeB
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		150
			PhysForcePowerJitter	25
		End
	End
End

Condition
	On 	Time
	Time 	36

	Event
		EName	Prime1
		Type	Start
		At	rigging
		Altpiv	1
		Geom	FrontFacingLocalPivot
		bhvr	CustomizeablePowers\SonicAttack\GaleProjectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	Prime1A
		Type	Local
		At	Prime1
		Altpiv	1
		Part	:SonicCoreElipse.part
		Part	:SonicElipse.part
		Part	:GaleMist.part
	End
End

#########################################################################################

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
End

#############################################################

End