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
		At	Chest
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Chest_Male
	End

End

#############################################################

End