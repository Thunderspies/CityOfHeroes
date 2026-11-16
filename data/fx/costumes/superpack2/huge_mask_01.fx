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
		At	Chin
		BhvrOverride
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Chin_SuperPack2_01_huge
	End

End

#############################################################

End