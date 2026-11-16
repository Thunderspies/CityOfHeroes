#############################################################
## T9i24_01 - Shoulder Pad Plugs Both
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmL
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Male_SPadL_Capsules_T9i24_01
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Male_SPadR_Capsules_T9i24_01
	End

End

#############################################################

End