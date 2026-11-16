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
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_PoisonSpikeLeft
	End

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_PoisonSpikeRight
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound Fast6 80.0 80.0 0.8
	End
End

Condition
	On 	Time
	Time 	48

	Event
		Type	Start
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\EviscerateStreak.part
		Lifespan 3
	End
End

#############################################################

End