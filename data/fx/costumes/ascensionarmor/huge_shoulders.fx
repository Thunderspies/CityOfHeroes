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
			Scale		1.5 1.1 1.3
			PositionOffset	0.5 0.1 0
			PYRrotate 	0 0 -20
			TintGeom	1
		End
		GEOM	FX_Ascn_Upperarm_L_Male
	End

	Event
		Type	Local
		At	UArmR
		BhvrOverride
			Scale		1.5 1.1 1.3
			PositionOffset	-0.5 0.1 0
			PYRrotate 	0 0 20
			TintGeom	1
		End
		GEOM	FX_Ascn_Upperarm_R_Male
	End
End

#############################################################

End