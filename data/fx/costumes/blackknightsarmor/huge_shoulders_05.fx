#############################################################
## Black Knights Armor - Shoulder - Extra Left Spikes Left
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
		GEOM	FX_BK_UarmL_03_Huge
	End

	Event
		Type	Local
		At	UArmL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmL_01_Huge
	End

	Event
		Type	Local
		At	UArmL
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmL_02_Huge
	End
End

#############################################################

End