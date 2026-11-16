#############################################################
## Black Knights Armor - Shoulder - Extra Left Clean
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UArmL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmL_03_Male
	End


End

#############################################################

End