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
		At	UArmL
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Upperarm_L_Male
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Upperarm_R_Male
	End
End

#############################################################

End