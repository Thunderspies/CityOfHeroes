#############################################################
## Mecha Armor - Glove Detail 2 Left Only
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
		GEOM	FX_LarmL_detail2_Mecha_01
	End
End

#############################################################

End