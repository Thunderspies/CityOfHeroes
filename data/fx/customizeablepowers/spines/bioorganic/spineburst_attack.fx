#############################################################
## SpineBurst_Attack.fx
#############################################################

FxInfo

LifeSpan 87

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	HandL
		Sound Quills1 70 70 0.7
	End
End

Condition
	On 	Time
	Time 	46

	Event

		Type	Local
		At	HandL
		Sound Quills4 80 80 0.95
	End
End

## HAND FX #######################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandL
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandR
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		LifeSpan 14
	End
End

## REPEATING SPINE BURST #######################################################

Condition
	On 	Time
	Time 	48

	Event
		Ename	SpineBurstRig
		Type	Create
		Inherit	Position
		Update	None
		At	chest
		Anim	FX_Spines_SpineBurst
		Sound QuillExplo2 90 90 0.25
		LifeSpan 15
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Waist
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Chest
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Col_L
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	UArmL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	LArmL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	HandL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ULegL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Fore_LLegL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Fore_FootL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ToeL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegR
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_LLegL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_FootL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ToeL
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegR
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	CrystalSpines_Projectile02
		LifeSpan 8
		LifespanJitter 1
	End
End

#############################################################

End