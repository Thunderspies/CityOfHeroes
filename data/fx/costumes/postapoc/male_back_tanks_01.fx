#############################################################
## Post Apoc - Back-Tanks
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0 -.35
			PYRrotate 	0 0 0
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	GEO_Back_Post_Apoc_Tank_02
	End
End

#############################################################

End