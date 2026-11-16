#############################################################
## Ascension Armor Set - Male Gloves Left Only
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
			Scale		1.0 1.0 1.0
			TintGeom	1
		End
		GEOM	FX_Ascn_Forearm_L_Male
	End
End

#############################################################

End