#############################################################
## Mecha Armor - Mouthguard
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
		GEOM	FX_Head_Chin01_Mecha_01_Huge
	End
End

#############################################################

End