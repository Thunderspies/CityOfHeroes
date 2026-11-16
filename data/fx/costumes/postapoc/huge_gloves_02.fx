#############################################################
## Post Apoc - Gloves Spikes - Right Side Only
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			Scale		1.5 1.5 1.5
			PYRRotate	0 0 0
			PositionOffset	0 .02 0
			TintGeom	1
		End
		GEOM	GEO_LarmR_Post_Apoc_Tires_SPIKES_01
	End
End

#############################################################

End