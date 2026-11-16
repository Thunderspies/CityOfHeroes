#############################################################
## Super Pack 2 - Hook Arm Left - Large Hook
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
		GEOM	FX_LarmL_SuperPack2_02_huge
	End

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			PositionOffset	-1.4 0 0.015
#			PYRrotate 	-18 0.0 0
			Scale		1.6 1.6 1.6
			TintGeom	1
		End
		GEOM	FX_LarmL_SuperPack2_03_male
	End

End

#############################################################

End