#############################################################
## T9i24_01 - Head Gems
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hair
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Fem_Hair_Gems_T9i24_01
	End

End

#############################################################

End