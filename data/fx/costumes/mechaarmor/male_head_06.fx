#############################################################
## Mecha Armor - Antennas
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Head_Antenna02L_Mecha_01
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_Head_Antenna02R_Mecha_01
	End
End

#############################################################

End