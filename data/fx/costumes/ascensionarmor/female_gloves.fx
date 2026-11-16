#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo

Flags InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Forearm_L_Male
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Ascn_Forearm_R_Male
	End
End

#############################################################

End