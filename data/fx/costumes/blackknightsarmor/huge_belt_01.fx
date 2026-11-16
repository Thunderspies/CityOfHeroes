#############################################################
## Black Knights Armor - Thigh - Left Side Only
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
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_BK_UlegL_01_Huge
	End

End

#############################################################

End