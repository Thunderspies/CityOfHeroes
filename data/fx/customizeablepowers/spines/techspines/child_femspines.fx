#############################################################
## Spines_Activation.fx
#############################################################

FxInfo

Flags InheritAlpha

## BODY ###########################################################

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Chest
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_Chest
	End

	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_Hips
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	UArmL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_UArmL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_UArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_ULegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_ULegR
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	LArmL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_LArmL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_LArmR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_LLegL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_LLegR
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_HandL
	End

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_HandR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_FootR
	End

	Event
		Type	Local
		At	ToeL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_ToeL
	End

	Event
		Type	Local
		At	ToeR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		Geom	Fem_TechSpines_ToeR
	End
End

#############################################################

End