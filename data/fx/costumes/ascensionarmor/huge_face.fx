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
			Scale		1.1 1.1 1.1
			PositionOffset	0 0 -0.15
			TintGeom	1
		End
		GEOM	FX_Ascn_Head_Male
	End
End

#############################################################

End