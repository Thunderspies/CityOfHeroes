#############################################################
## Black Knights Armor - Shoulder - Extra Right Spikes Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_03_Male
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_01_Male
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_02_Male
	End
End

#############################################################

End