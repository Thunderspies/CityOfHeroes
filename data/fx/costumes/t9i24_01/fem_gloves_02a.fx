#############################################################
## T9i24_01 - Glove Gems Left
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Fem_LarmL_Gems_T9i24_01
	End
End

#############################################################

End