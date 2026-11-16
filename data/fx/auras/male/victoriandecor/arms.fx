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
			Spin	0.0 0.0 0.0
			Scale	1.2 1.2 1.2
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
			Scale	1.5 1.6 1.6
			TintGeom	1
		End
		Lifespan 0
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			Scale	1.2 1.2 1.2
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
			Scale	1.5 1.6 1.6
			TintGeom	1
		End
		Lifespan 0
	End
End

#############################################################

End