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
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_Chest
	End

	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_Hips
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	UArmL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_UArmL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_UArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_ULegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_ULegR
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	LArmL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_LArmL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_LArmR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_LLegL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_LLegR
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
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_HandL
	End

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_HandR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_SlateSpines_FootR
	End
End

#############################################################

End