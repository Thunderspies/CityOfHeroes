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
			Scale		0.95 0.95 0.95
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victorian_ChestR
		BhvrOverride
			Scale		0.95 0.95 0.95
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-0.2 0.0 0.0
			Scale		0.9 0.9 0.9
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
			PositionOffset	-0.1 0.0 0.0
			Scale		1.2 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	0.2 0.0 0.0
			Scale		0.9 0.9 0.9
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
			PositionOffset	0.1 0.0 0.0
			Scale		1.2 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	ULegL
		Geom	FX_Victorian_ULegL
		BhvrOVerride
			PositionOffset	-0.07 -0.1 0.05
			Scale		0.7 1.2 0.7
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegL
		Geom	FX_Victorian_LLegL
		BhvrOVerride
			PositionOffset	-0.1 -0.15 -0.1
			Scale		0.65 1.15 0.65
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	ULegR
		Geom	FX_Victorian_ULegR
		BhvrOVerride
			PositionOffset	0.07 -0.1 0.05
			Scale		0.7 1.2 0.7
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegR
		Geom	FX_Victorian_LLegR
		BhvrOVerride
			PositionOffset	0.1 -0.15 -0.1
			Scale		0.65 1.15 0.65
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Head
		Geom	FX_Victorian_FaceL
		BhvrOVerride
			PositionOffset	0.0 0.06 0.0
			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Head
		Geom	FX_Victorian_FaceL
		BhvrOVerride
			PositionOffset	0.0 0.06 0.0
			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victiorian_Spine
		BhvrOVerride
			Scale	1.0 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End