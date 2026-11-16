#############################################################
## Mecha Armor - Shoulderpad Extras
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmL
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_SpadL_Mecha_01B_Huge
	End
End

#############################################################

End