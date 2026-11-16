#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		GEOM	FX_CoW_FaceMask_01
	End

End

#############################################################

End