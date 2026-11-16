#############################################################
## Black Knights Armor - Shoulder - Extra Both Spikes Left
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
		GEOM	FX_BK_UarmL_03_Fem
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_03_Fem
	End

	Event
		Type	Local
		At	UArmL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmL_01_Fem
	End

	Event
		Type	Local
		At	UArmL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmL_02_Fem
	End
End

#############################################################

End