#############################################################
## Black Knights Armor - Thigh - Both Sides
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UlegL
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	FX_BK_UlegL_01_Huge
	End

	Event
		Type	Local
		At	UlegR
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	FX_BK_UlegR_01_Huge
	End

End

#############################################################

End