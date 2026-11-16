#############################################################
## T9i24_01 - Boot Gems Left
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LlegL
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Fem_LlegL_Gems_T9i24_01
	End
End

#############################################################

End