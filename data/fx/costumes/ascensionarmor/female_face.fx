#############################################################
## Ascension Armor Set - Female Face Detail 1
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
			Scale		0.8 1.2 0.8
			TintGeom	1
		End
		GEOM	FX_Ascn_Head_Male
	End
End

#############################################################

End