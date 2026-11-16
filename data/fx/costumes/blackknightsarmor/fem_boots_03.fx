#############################################################
## Black Knights Armor - Knee - Extra with Spikes
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
		GEOM	FX_BK_LlegL_01_Fem
	End

	Event
		Type	Local
		At	LlegR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegR_01_Fem
	End

	Event
		Type	Local
		At	LlegL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegL_02_Fem
	End

	Event
		Type	Local
		At	LlegR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegR_02_Fem
	End

	Event
		Type	Local
		At	LlegL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegL_03_Fem
	End

	Event
		Type	Local
		At	LlegR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_LlegR_03_Fem
	End

End

#############################################################

End