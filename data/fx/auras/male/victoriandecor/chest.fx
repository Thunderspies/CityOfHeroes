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
			PositionOffset	0.0 0.0 0.0
			Scale		1.2 1.2 1.2
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victorian_ChestR
		BhvrOverride
			PositionOffset	0.0 -0.0 0.0
			Scale		1.2 1.2 1.2
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	Chest
		Geom	FX_Victiorian_Spine
		BhvrOVerride
			Scale	1.3 1.3 1.5
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End