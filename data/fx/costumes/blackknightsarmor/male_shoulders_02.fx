#############################################################
## Black Knights Armor - Shoulder - Base Spikes Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	Local
#		At	UArmL
#		BhvrOverride
#//			Scale		0.7 0.7 0.7
#			TintGeom	1
#		End
#		GEOM	FX_BK_UarmL_01_Male
#	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_01_Male
	End

End

#############################################################

End