#############################################################
## BarbSwipe_Attack.fx
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
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_PoisonSpikeLeft
		Sound Quills2b 70.0 70.0 0.8
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