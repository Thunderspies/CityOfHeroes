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
			Scale    	.7 .7 .7
			PositionOffset	-.05 .04 -.02
			PYRrotate 	0 -8 -40
			TintGeom	1
		End
		GEOM	GEO_UarmL_PostApoc_01_Male
	End

#	Event
#		Type	Local
#		At	Col_R
#		BhvrOverride
#			PositionOffset	.09 .3 .03
#			PYRrotate 	-0 8 40
#			TintGeom	1
#		End
#		GEOM	GEO_UarmR_PostApoc_01_Male
#	End

	Event
		Type	Local
		At	Col_L
		BhvrOverride
			Scale    	.7 .7 .7
			PositionOffset	-.05 .04 -.02
			PYRrotate 	0 -8 -40
			TintGeom	1
		End
		GEOM	GEO_SpadL_Post_Apoc_SPIKES
	End

#	Event
#		Type	Local
#		At	Col_R
#		BhvrOverride
#			PositionOffset	.09 .3 .03
#			PYRrotate 	-0 8 40
#			TintGeom	1
#		End
#		GEOM	GEO_SpadR_Post_Apoc_SPIKES
#	End
End

#############################################################

End