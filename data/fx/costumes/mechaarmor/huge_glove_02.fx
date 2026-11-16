#############################################################
## Mecha Armor - Glove Extras
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmL
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_LarmL_detail2_Mecha_01_Huge
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LarmR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_LarmR_detail2_Mecha_01_Huge
	End
End

#############################################################

End