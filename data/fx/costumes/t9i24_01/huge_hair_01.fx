#############################################################
## T9i24_01 - Head Plugs
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
		GEOM	FX_Huge_Hair_Capsules_T9i24_01
	End

End

#############################################################

End