#############################################################
## Black Knights Armor - Shoulder - Extra Both Spikes Both
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
		GEOM	FX_BK_UarmL_01_Huge
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_01_Huge
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

	Event
		Type	Local
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_02_Huge
	End

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
		At	UArmR
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UarmR_03_Huge
	End
End

#############################################################

End