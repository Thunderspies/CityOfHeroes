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
		At	Col_R
		BhvrOverride
			TintGeom	1
		End
		GEOM	GEO_UarmR_PostApoc_01_Male
	End


End

#############################################################

End