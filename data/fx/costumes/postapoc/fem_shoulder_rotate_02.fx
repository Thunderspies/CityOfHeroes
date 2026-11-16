#############################################################
## Post Apoc - Rotate Base Left
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
			Scale    	.7 .7 .7
			PositionOffset	-.05 .04 -.02
			PYRrotate 	0 -8 -40
			TintGeom	1
		End
		GEOM	GEO_UarmL_PostApoc_01_Male
	End
End

#############################################################

End