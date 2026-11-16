#############################################################
## T9i24_01 - Boot Gems Right
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LlegR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Fem_LlegR_Gems_T9i24_01
	End

End

#############################################################

End