#############################################################
## Super Pack 2 - Hook Arm Left - Double Hook
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
		GEOM	FX_LarmL_SuperPack2_02_male
	End

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			PositionOffset	-1.15 0 -0.010
			PYRrotate 	-0 25 0
			Scale		0.8 0.8 0.8
			TintGeom	1
		End
		GEOM	FX_LarmL_SuperPack2_03_male
	End

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			PositionOffset	-1.15 0 -0.040
			PYRrotate 	-0 -20 0
			Scale		0.8 0.8 0.8
			TintGeom	1
		End
		GEOM	FX_LarmL_SuperPack2_03_male
	End

End

#############################################################

End