#############################################################
## T9i24_01 - Boot Plugs Both
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
		GEOM	FX_Huge_LlegL_Capsules_T9i24_01
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LlegR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Huge_LlegR_Capsules_T9i24_01
	End

End

#############################################################

End