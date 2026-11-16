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
		At	Head
		Geom	FX_Victorian_FaceL
		BhvrOVerride
			PositionOffset	0.0 0.0 -0.1
			Scale		1.15 1.15 1.15
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End