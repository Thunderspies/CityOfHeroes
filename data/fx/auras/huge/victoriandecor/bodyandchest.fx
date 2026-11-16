#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo
Flags	InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Geom	FX_Victorian_ChestL
		BhvrOverride
			Scale		1.5 1.5 1.5
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victorian_ChestR
		BhvrOverride
			Scale		1.5 1.5 1.5
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-0.2 0.0 0.0
			Scale		1.5 1.8 1.5
			TintGeom	1
		End
		Geom	FX_Victorian_LArmL

		Lifespan 0
	End

	Event
		Type	Local
		At	UArmL
		Geom	FX_Victorian_UArmL
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
			Scale		2.0 2.2 2.3
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	0.2 0.0 0.0
			Scale		1.5 1.8 1.5
			TintGeom	1
		End
		Geom	FX_Victorian_LArmR

		Lifespan 0
	End

	Event
		Type	Local
		At	UArmR
		Geom	FX_Victorian_UArmR
		BhvrOverride
			PositionOffset	-0.1 0.0 0.05
			Scale		2.0 2.2 2.3
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	ULegL
		Geom	FX_Victorian_ULegL
		BhvrOVerride
			PositionOffset	0.0 0.4 0.0
			Scale		1.4 1.45 1.25
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegL
		Geom	FX_Victorian_LLegL
		BhvrOVerride
			PositionOffset	-0.1 0.0 0.0
			Scale		1.25 1.4 1.3
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	ULegR
		Geom	FX_Victorian_ULegR
		BhvrOVerride
			PositionOffset	0.0 0.4 0.0
			Scale		1.4 1.45 1.25
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegR
		Geom	FX_Victorian_LLegR
		BhvrOVerride
			PositionOffset	0.1 0.0 0.0
			Scale		1.25 1.4 1.3
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victiorian_Spine
		BhvrOVerride
			PositionOffset	0 0.0 -0.2
			Scale		1.8 1.8 1.8
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End