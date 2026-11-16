#############################################################
## Post Apoc - Huge Belt bag
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			Scale		0 0 0
			PYRRotate	0 0 0
			PositionOffset	0 0 -.1

			TintGeom	1
		End
		GEOM	GEO_Belt_Post_Apoc_Bag_01
	End
End

#############################################################

End