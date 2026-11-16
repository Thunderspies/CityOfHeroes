#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo
Flags	InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-0.2 0.0 0.0
			Scale		0.9 0.9 0.9
			TintGeom	1
		End
		Geom	FX_Victorian_LArmL

		Lifespan 0
	End

	Event
		Type	Local
		At	UArmL
		Geom	FX_Victorian_UArmL
		BhvrOverride
			PositionOffset	-0.1 0.0 0.0
			Scale		1.2 1.0 1.0
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End