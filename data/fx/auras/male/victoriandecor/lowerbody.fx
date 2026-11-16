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
			Scale	1.15 1.25 1.15
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegL
		Geom	FX_Victorian_LLegL
		BhvrOVerride
			Scale	1.15 1.35 1.15
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	ULegR
		Geom	FX_Victorian_ULegR
		BhvrOVerride
			Scale	1.15 1.25 1.15
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LLegR
		Geom	FX_Victorian_LLegR
		BhvrOVerride
			Scale	1.15 1.35 1.15
			TintGeom	1
		End
		Lifespan 0
	End

End

#############################################################

End