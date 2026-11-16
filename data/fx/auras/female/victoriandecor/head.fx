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
			PositionOffset	0.0 0.06 0.0
			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End