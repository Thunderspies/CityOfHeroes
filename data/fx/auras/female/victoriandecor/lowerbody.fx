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

End

#############################################################

End