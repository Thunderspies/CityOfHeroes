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
			Scale		1.3 1.3 1.3
			PYRRotate	0 0 0
			PositionOffset	-.2 .1 -.2
			TintGeom	1
		End
		GEOM	GEO_UarmR_PostApoc_01_Male
	End

	Event
		Type	Local
		At	Col_R
		BhvrOverride
			Scale		1.3 1.3 1.3
			PYRRotate	0 0 0
			PositionOffset	-.2 .1 -.2
			TintGeom	1
		End
		GEOM	GEO_SpadR_Post_Apoc_SPIKES
	End
End

#############################################################

End