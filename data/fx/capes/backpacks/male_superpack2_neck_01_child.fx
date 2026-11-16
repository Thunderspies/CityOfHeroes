#############################################################
## Super Pack 2 - Mask Tube
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Chest_SuperPack2_01_Male
	End

End

#############################################################

End