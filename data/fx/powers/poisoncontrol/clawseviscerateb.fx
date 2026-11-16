#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 48

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak9
		Type	local
		At	HandL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	PoisonSpikeLeft
	End

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound Fast6 80.0 80.0 .8
	End
End

Condition
	On 	Time
	Time 	48

	Event
		Type	Start
		At	Chest
		Part1	Powers\Claws\EviscerateStreak.part
		Lifespan 3
	End
End

#############################################################

End