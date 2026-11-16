#############################################################
## Post Apoc - Rotate Base Right
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
			PositionOffset	.05 -.04 .02
			PYRrotate 	0 8 40
			TintGeom	1
		End
		GEOM	GEO_UarmR_PostApoc_01_Male
	End
End

#############################################################

End