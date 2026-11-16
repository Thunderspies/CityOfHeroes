#############################################################
## Super Pack 2 - Sleeve Cuff - Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	Local
#		At	LarmL
#		BhvrOverride
##			Scale		0.7 0.7 0.7
#			TintGeom	1
#		End
#		GEOM	FX_LarmL_SuperPack2_01_male
#	End

	Event
		Type	Local
		At	LarmR
		BhvrOverride
#			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_LarmR_SuperPack2_01_huge
	End

End

#############################################################

End