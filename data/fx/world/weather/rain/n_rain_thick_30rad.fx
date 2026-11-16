#############################################################
## N_Rain_30rad.fx
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
#		Part :N_Rain_ThickDrops_30rad.part
		Part :N_Rain_Thick_30rad.part
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

Condition
	On Cycle
	Time 1
	Repeat 3
	RepeatJitter 2

	Event
		EName Splashes
		At Root
		Type Start
		#Part :N_Rain_Thick_AreaSplashes_30rad.part
		Part :N_Rain_Thick_AreaRipples_30rad.part
		LifeSpan 1
	End
End
#########################################################
End
