#############################################################
## N_Rain_Drizzle_30rad_NoSplash.fx
#############################################################

FxInfo
###########################################################

Condition
	On Time
	Time 0

	Event
		EName Drizzle
		At Root
		Type Start
		BhvrOverride
			PositionOffset 0 45 0
		End
		Part :N_Rain_30rad.part
	End

	Event
		EName SCALETEST_GEO
		At Root
		Type Start
		BhvrOverride
			PositionOffset 0 30 0
			Scale 30 30 30
		End
#		Geom Geo_Fx_TestSphere
	End
End
#########################################################
End
