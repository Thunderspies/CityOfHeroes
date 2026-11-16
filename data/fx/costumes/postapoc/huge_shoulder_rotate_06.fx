#############################################################
## Post Apoc - Shoulderpad Extras
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	Local
#		At	Col_L
#		BhvrOverride
#			PositionOffset	-.09 .3 -.03
#			PYRrotate 	0 -8 -40
#			TintGeom	1
#			TintGeom	1
#		End
#		GEOM	GEO_UarmL_PostApoc_01_Male
#	End

	Event
		Type	Local
		At	Col_R
		BhvrOverride
			Scale		1.3 1.3 1.3
			PositionOffset	.09 .3 -.17
			PYRrotate 	0 8 40
			TintGeom	1
		End
		GEOM	GEO_UarmR_PostApoc_01_Male
	End

#	Event
#		Type	Local
#		At	Col_L
#		BhvrOverride
#			PositionOffset	-.09 .3 -.03
#			PYRrotate 	0 -8 -40
#			TintGeom	1
#			TintGeom	1
#		End
#		GEOM	GEO_SpadL_Post_Apoc_SPIKES
#	End

	Event
		Type	Local
		At	Col_R
		BhvrOverride
			Scale		1.3 1.3 1.3
			PositionOffset	.09 .3 -.17
			PYRrotate 	0 8 40
			TintGeom	1
		End
		GEOM	GEO_SpadR_Post_Apoc_SPIKES
	End
End

#############################################################

End