#############################################################
## Super Pack 2 - Pegleg Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LlegR
		BhvrOverride
#			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_LlegR_SuperPack2_01_fem
	End

End

#############################################################

End