#############################################################
## T9i24_01 - Glove Plugs Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Male_LarmR_Capsules_T9i24_01
	End

End

#############################################################

End