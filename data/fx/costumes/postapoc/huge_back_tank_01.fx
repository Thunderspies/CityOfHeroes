#############################################################
## Post Apoc - Back-Tank
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		BhvrOverride
			Scale		1.1 1.1 1.1
			PYRRotate	0 0 0
			PositionOffset	0 0 -.3
			TintGeom	1
		End
		GEOM	GEO_Back_Post_Apoc_Tank_01
	End
End

#############################################################

End