#############################################################
## Post Apoc - Gloves Spikes - Both Sides Only
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	GEO_LarmL_Post_Apoc_Tires_SPIKES_01
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
//			Scale		1 1 1
			TintGeom	1
		End
		GEOM	GEO_LarmR_Post_Apoc_Tires_SPIKES_01
	End
End

#############################################################

End