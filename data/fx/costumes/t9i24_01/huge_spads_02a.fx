#############################################################
## T9i24_01 - Shoulder Pad Gems Left
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
		GEOM	FX_Huge_SPadL_Gems_T9i24_01
	End
End

#############################################################

End