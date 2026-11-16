#############################################################
## T9i24_01 - Glove Plugs Left
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
		GEOM	FX_Huge_LarmL_Capsules_T9i24_01
	End
End

#############################################################

End