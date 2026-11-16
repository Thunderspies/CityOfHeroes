#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Lights
		Type	local
		At	FootR
		BhvrOverride
			Scale	10 10 10
		End
		Geom	GEO_FX_TestSphere
	End

	Event
		Ename	Lights
		Type	local
		At	FootL
		BhvrOverride
			Scale	10 10 10
		End
		Geom	GEO_FX_TestSphere
	End

End

#########################################################

End		