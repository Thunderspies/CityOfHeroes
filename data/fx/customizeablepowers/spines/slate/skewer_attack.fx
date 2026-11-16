#############################################################
## Skewer_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

LifeSpan 62

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	Root
		Sound thornywhoosh 70 70 0.4
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Root
		Anim	FX_clawspeed
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillLeft
		LifeSpan 42
	End

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillRight
		LifeSpan 42
	End
End

#############################################################

Condition
	On 	Time
	Time 	42

	Event
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleDown.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillLeft
	End

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleDown.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillRight

	End
End

#############################################################

End