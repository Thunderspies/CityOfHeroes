#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Streak9
		Type	local
		At	HandL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	PoisonSpikeLeft
		Sound Quills2b 70.0 70.0 .8
	End

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
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
		Geom	swoosh
		bhvr	behaviors\Claws.bhvr
		LifeSpan 3
	End
End

#############################################################

End