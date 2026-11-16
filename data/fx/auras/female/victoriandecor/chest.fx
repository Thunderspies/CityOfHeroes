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