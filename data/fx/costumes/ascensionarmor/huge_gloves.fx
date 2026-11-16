#############################################################
## Ascension Armor Set - Huge Gloves Both
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
			Scale		1.2 1.2 1.2
			PositionOffset	0 0.2 0
			TintGeom	1
		End
		GEOM	FX_Ascn_Forearm_L_Male
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			Scale		1.2 1.2 1.2
			PositionOffset	0 0.2 0
			TintGeom	1
		End
		GEOM	FX_Ascn_Forearm_R_Male
	End
End

#############################################################

End