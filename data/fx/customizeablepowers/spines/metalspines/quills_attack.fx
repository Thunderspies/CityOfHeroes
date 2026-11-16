#############################################################
## Quills_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 30

## REPEATING SPINE BURST #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	SpineBurstRig
		Type	Create
		Inherit	Position
		Update	None
		At	chest
		Anim	FX_Spines_SpineBurst
		Sound QuillExplo2 90 90 0.25
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
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
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ULegR
		Bhvr	CustomizeablePowers\Spines\Spines\SpineBurst_SpinesFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBits.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsB.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsB.part
		Geom	MetalSpine_Projectile01
		LifeSpan 3
		LifespanJitter 1
	End
End

#########################################################

End