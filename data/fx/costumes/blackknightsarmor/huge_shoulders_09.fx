#############################################################
## Black Knights Armor - Shoulder - Extra Right Spikes Left
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
		GEOM	FX_BK_UarmR_03_Huge
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
End

#############################################################

End