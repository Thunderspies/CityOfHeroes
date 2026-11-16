#############################################################
## BarbSwipe_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

LifeSpan 75

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
		Sound Quills2b 70.0 70.0 0.8
		LifeSpan 55
	End

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillRight
		LifeSpan 55
	End
End

#############################################################

Condition
	On 	Time
	Time 	60

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

## SWOOSHES ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	swipe
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End


#############################################################

End