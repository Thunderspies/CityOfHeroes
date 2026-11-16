#############################################################
## Thorns_Activation.fx
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
		Sound Quills1b 70 70 0.76
	End
End

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		Lifespan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	GasSpurts
		Type	Local
		At	HandL
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitBA.part
		Lifespan 16
	End

	Event
		EName	GasSpurtsb
		Type	Local
		At	HandR
		Part	CustomizeablePowers\Spines\Thorns\MustardGasHitBA.part
		Part	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitBA.part
		Lifespan 16
	End
End

#############################################################


Condition
	On 	Time
	Time 	2

	Event
		EName	Spike02b
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_Chest01
	End

	Event
		EName	Spike02b
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScaleQuick.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_chest02
	End

	Event
		EName	Spike02b
		Type	Local
		At	waist
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_Waist
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName	Spike02b
		Type	Local
		At	UlegL
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_UlegL
	End

	Event
		EName	Spike02b
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_UlegR
	End

	Event
		EName	Spike02b
		Type	Local
		At	SPadL
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_SPadL
	End


	Event
		EName	Spike02b
		Type	Local
		At	SpadR
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_SpadR
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName	Spike02b
		Type	Local
		At	LarmL
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_LarmL
	End

	Event
		EName	Spike02b
		Type	Local
		At	LarmR
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_LarmR
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegR
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_LlegR
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_LlegL
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_WepL
	End

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Thorns\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_V_Quill_WepR
	End
End

#############################################################

End