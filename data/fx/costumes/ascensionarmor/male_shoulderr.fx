#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UArmL
		BhvrOverride
			Scale		0.8 0.8 0.8
			PositionOffset	0.1 0.75 0.0
			TintGeom	1
		End
		GEOM	FX_Ascn_Upperarm_R_Male
	End
End

#############################################################

End