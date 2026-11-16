#############################################################
## Super Pack 2 - Hook Arm Left - Normal Hook
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
		GEOM	FX_LarmL_SuperPack2_02_fem
	End

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			PositionOffset	-0.9 0 0.045
#			PYRrotate 	-18 0.0 0
			Scale		0.8 0.8 0.8
			TintGeom	1
		End
		GEOM	FX_LarmL_SuperPack2_03_male
	End

End

#############################################################

End