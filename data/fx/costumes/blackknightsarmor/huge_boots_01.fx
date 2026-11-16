#############################################################
## Black Knights Armor - Knee - Base Spikes
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LlegL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegL_01_Huge
	End

	Event
		Type	Local
		At	LlegR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegR_01_Huge
	End

End

#############################################################

End