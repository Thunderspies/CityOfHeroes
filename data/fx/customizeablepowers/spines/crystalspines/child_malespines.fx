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
		Geom	Male_CrystalSpines_Chest
	End

	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_Hips
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
		Geom	Male_CrystalSpines_UArmL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_UArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_ULegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_ULegR
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
		Geom	Male_CrystalSpines_LArmL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_LArmR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_LLegL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_LLegR
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
		Geom	Male_CrystalSpines_HandL
	End

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_HandR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\Scale1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_CrystalSpines_FootR
	End
End

#############################################################

End