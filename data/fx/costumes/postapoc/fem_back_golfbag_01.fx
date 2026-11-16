#############################################################
## Post Apoc - Back-Golf-Bag
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
			Scale		0.85 0.85 0.85
			PYRRotate	-10 0 0
			PositionOffset	0 0 .05
			TintGeom	1
		End
		GEOM	GEO_Back_Post_Apoc_Golfbag_01
	End
End

#############################################################

End