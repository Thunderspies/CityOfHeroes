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
		At	Hips
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Waist_Male
	End

End

#############################################################

End