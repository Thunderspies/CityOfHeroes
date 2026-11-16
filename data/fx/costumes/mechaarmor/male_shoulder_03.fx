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
		GEOM	FX_SpadL_Mecha_01B
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmR
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_SpadR_Mecha_01B
	End
End

#############################################################

End