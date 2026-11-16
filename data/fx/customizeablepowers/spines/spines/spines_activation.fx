#############################################################
## Spines_Activation.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandL
		Sound Quills1 70 70 0.7
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	GasSpurts
		Type	Local
		At	HandL
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		Lifespan 16
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	HandR
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitBA.part
		lifeSpan 16
	End
End

## BODY ###########################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_WepL
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_WepR
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	waist
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_Waist
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_Chest01
	End

	Event
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_chest02
	End
End

## LEGS ###########################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	UlegL
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_UlegL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_UlegR
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LlegR
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_LlegR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Quills_LlegL
	End
End

#############################################################

End