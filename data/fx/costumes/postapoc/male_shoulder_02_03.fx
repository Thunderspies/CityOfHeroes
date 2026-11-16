#############################################################
## Post Apoc - Shoulderpad Extras
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Col_L
		BhvrOverride
			TintGeom	1
		End
		GEOM	GEO_UarmL_PostApoc_02_Male
	End

	Event
		Type	Local
		At	Col_L
		BhvrOverride
			TintGeom	1
		End
		GEOM	GEO_UarmL_PostApoc_02_Male_Bottom
	End
End

#############################################################

End