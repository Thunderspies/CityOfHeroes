#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 35

#############################################################

Condition
	On 	Time
	Time 	6

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
	End
End

#############################################################

End