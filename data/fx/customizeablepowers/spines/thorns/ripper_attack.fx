#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type	Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Fast6 80.0 80.0 0.8
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillLeft
		LifeSpan 52
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillRight
		LifeSpan 52
	End
End

#############################################################

Condition
	On 	Time
	Time 	62

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleDown.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillLeft
		Lifespan 30
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillSpikeScaleDown.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_QuillRight
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	49

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part1	CustomizeablePowers\Spines\Thorns\EviscerateStreak.part
		lifespan 1
	End
End

#############################################################

End