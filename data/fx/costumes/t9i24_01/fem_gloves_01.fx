#############################################################
## T9i24_01 - Glove Plugs Both
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
		GEOM	FX_Fem_LarmL_Capsules_T9i24_01
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Fem_LarmR_Capsules_T9i24_01
	End

End

#############################################################

End