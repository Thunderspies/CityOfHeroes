#############################################################
## Black Knights Armor - Gloves Spikes - Left Side Only
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	FX_BK_LarmL_01_Male
	End
End

#############################################################

End