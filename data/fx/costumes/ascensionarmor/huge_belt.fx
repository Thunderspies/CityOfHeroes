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
			Scale		1.1 1.1 1.1
			PositionOffset	0 -0.17 0
			TintGeom	1
		End
		GEOM	FX_Ascn_Waist_Male
	End

End

#############################################################

End