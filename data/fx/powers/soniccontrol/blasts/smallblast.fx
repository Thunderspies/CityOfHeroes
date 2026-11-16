#############################################################
## SmallBlast.fx
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local
		At	wepR
		Geom	SonicSpinnerOffset
		Sound SonicSmallBlast2 95 95 .9
	End

	Event
		Type	Local
		At	Spinner
		altpiv	1
		BHVR	Behaviors\SonicGunSpinner.bhvr
		Geom	SonicSpinner2
		LifeSpan 9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	riggingoffset
		Type	local
		At	Spinner
		altpiv	2
	End

	Event
		EName 	rigging
		Type	local
		At	riggingoffset
		BHVR	:rotate.bhvr
		Geom	PsiTele02
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	riggingoffset
		Type	start
		At	Spinner
		altpiv	2
		BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 6
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName	prime
		Type	Start
		At	rigging
		Altpiv	1
		bhvr	:SMLProjectile.bhvr
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
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingCoreTrailBright.part
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