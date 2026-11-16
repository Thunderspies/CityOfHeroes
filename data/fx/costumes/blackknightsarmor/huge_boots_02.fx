#############################################################
## Black Knights Armor - Knee - Extra Armor
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
		GEOM	FX_BK_LlegL_03_Huge
	End

	Event
		Type	Local
		At	LlegR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegR_03_Huge
	End

End

#############################################################

End