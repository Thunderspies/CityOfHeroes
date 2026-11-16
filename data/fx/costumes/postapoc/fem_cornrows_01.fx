#############################################################
## Post Apoc - Cornrows-Tail
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	GEO_Hair_Post_Apoc_CornRows_Tail_01
	End
End

#############################################################

End