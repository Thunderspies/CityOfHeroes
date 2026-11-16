#############################################################
## Mecha Armor - Glove Detail 1 Left Only
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
		GEOM	FX_LarmL_detail1_Mecha_01_Female
	End
End

#############################################################

End