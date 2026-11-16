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
			Scale		1.3 1.3 1.3
			PYRRotate	0 0 0
			PositionOffset	.2 .1 -.2
			TintGeom	1
		End
		GEOM	GEO_UarmL_PostApoc_01_Male
	End
End

#############################################################

End