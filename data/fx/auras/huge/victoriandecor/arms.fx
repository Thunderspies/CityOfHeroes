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
			Scale		1.5 1.8 1.5
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
			PositionOffset	0.1 0.0 0.0
			Scale		2.0 2.2 2.3
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	0.2 0.0 0.0
			Scale		1.5 1.8 1.5
			TintGeom	1
		End
		Geom	FX_Victorian_LArmR

		Lifespan 0
	End

	Event
		Type	Local
		At	UArmR
		Geom	FX_Victorian_UArmR
		BhvrOverride
			PositionOffset	-0.1 0.0 0.05
			Scale		2.0 2.2 2.3
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End