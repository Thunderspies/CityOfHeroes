#############################################################
## ThornBurst_Attack.fx
#############################################################

FxInfo

LifeSpan 87

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandL
		Sound ThornRumble 70 70 0.8
	End
End

Condition
	On 	Time
	Time 	44

	Event
		Type	Local
		At	HandL
		Sound thornblast5 80 80 0.95
	End
End

## HAND FX #######################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandL
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitBA.part
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandR
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitBA.part
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
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Waist
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Chest
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Col_L
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	UArmL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	LArmL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	HandL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ULegL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_LLegL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_FootL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ToeL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegR
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_LLegL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_FootL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ToeL
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Hind_ULegR
#		Bhvr	CustomizeablePowers\Spines\Thorns\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsB.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsB.part
		Geom	Tintable_Thorns_ProjectileLarge01
		Geom	Tintable_Thorns_ProjectileLarge02
		Geom	Tintable_Thorns_ProjectileLarge03
		Geom	Tintable_Thorns_ProjectileLarge04
		LifeSpan 8
		LifespanJitter 1
	End
End

#############################################################

End