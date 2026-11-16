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
		At	LLegL
		BhvrOverride
			Scale		1.1 1.1 1.1
			TintGeom	1
		End
		GEOM	FX_Ascn_LowerLeg_L_Male
	End

	Event
		Type	Local
		At	LLegR
		BhvrOverride
			Scale		1.1 1.1 1.1
			TintGeom	1
		End
		GEOM	FX_Ascn_LowerLeg_R_Male
	End
End

#############################################################

End