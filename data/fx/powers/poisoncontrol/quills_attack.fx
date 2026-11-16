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
		Bhvr	:Quills.bhvr
		Anim	FX_Spines_SpineBurst
		Sound QuillExplo2 90 90 0.25
		LifeSpan 10
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Waist
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Chest
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Col_L
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	UArmL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	LArmL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	HandL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ULegL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_LLegL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_FootL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ToeL
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End

	Event
		Ename	Spine01
		Type	local
		At	SpineBurstRig
		animpiv	Fore_ULegR
		Bhvr	:Quills_SpinesFade.bhvr
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		Geom	Spines_ProjectileSmall01
		Geom	Spines_ProjectileSmall02
		Geom	Spines_ProjectileSmall03
		Geom	Spines_ProjectileSmall04
		LifeSpan 4
		LifespanJitter 1
	End
End

#########################################################

End