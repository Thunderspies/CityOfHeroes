#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 550

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type	Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Fast6 80.0 80.0 .8
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
		Bhvr	Behaviors\QuillSpikeScaleQuick.bhvr
		Geom	V_QuillLeft
		LifeSpan 52
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		Bhvr	Behaviors\QuillSpikeScaleQuick.bhvr
		Geom	V_QuillRight
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
		Bhvr	Behaviors\QuillSpikeScaleDown.bhvr
		Geom	V_QuillLeft
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		Bhvr	Behaviors\QuillSpikeScaleDown.bhvr
		Geom	V_QuillRight
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
		Part1	Powers\Claws\EviscerateStreak.part
		lifespan 250
	End
End

#############################################################

End