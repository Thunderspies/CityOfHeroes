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
			Scale    	.7 .7 .7
			PositionOffset	0 0 0
			PYRrotate 	0 0 0
			TintGeom	1
		End
		GEOM	GEO_UarmR_PostApoc_02_Male
	End
End

#############################################################

End