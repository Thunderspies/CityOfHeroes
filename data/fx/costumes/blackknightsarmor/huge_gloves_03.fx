#############################################################
## Black Knights Armor - Gloves Spikes - Both Sides
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmR
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	FX_BK_LarmR_01_Huge
	End

	Event
		Type	Local
		At	LArmL
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	FX_BK_LarmL_01_Huge
	End
End

#############################################################

End