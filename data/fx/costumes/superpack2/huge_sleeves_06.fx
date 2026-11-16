#############################################################
## Super Pack 2 - Sleeve Cuff - Both
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmL
		BhvrOverride
#			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_LarmL_SuperPack2_04_huge
	End

	Event
		Type	Local
		At	LarmR
		BhvrOverride
#			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_LarmR_SuperPack2_04_huge
	End

End

#############################################################

End