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
		Geom	Male_MetalSpines_Chest
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	Col_L
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_SpadL
	End

	Event
		Type	Local
		At	Col_R
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_SpadR
	End

	Event
		Type	Local
		At	Waist
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_Waist
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
		Geom	Male_MetalSpines_UArmL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_UArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_ULegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_ULegR
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
		Geom	Male_MetalSpines_LArmL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_LArmR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_LLegL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_LLegR
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
		Geom	Male_MetalSpines_HandL
	End

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_MetalSpines_HandR
	End
End

#############################################################

End