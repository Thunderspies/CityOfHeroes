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
#			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		Geom	FX_Head_Retro_SciFi_01_Huge
	End

End

#############################################################

End