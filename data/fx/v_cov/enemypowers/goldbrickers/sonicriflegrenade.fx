#############################################################
## SonicRifleGrenade.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		Type	local
		At	head
		Sound SonicBlastRifle 90 90 .89
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local
		At	wepR
		Geom	SonicSpinnerOffset02
		LifeSpan 30

	End

	Event
		Type	Local
		At	Spinner
		altpiv	1
		BHVR	:SonicGunSpinner.bhvr
		Geom	Goldbricker_Spinner_02
		LifeSpan 30
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
		Type	local
		At	Spinner
		altpiv	2
		BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 6
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	prime
		Type	Start
		At	rigging
		Altpiv	1
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
		Part	:SonicGrenadeRingCore.part
		Part	:SonicGrenadeRingGlow.part
	End

	Event
		EName	p2
		Type	Local
		At	Prime
		Part	:SonicGrenadeRingCoreTrail.part
		Part	:SonicGrenadeRingGlowTrail.part
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