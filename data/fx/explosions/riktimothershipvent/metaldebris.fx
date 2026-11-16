#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Lifespan 300

## METAL DEBRIS #######################################################

Condition
	On	Time
	Time 	0
	Repeat	3

	Event
		EName 	metal
		Type	Local
		At	Origin
		Bhvr	:MetalDebris.bhvr
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	90
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	0
	Repeat	3

	Event
		EName 	metal
		Type	Local
		At	Origin
		Bhvr	:MetalDebris.bhvr
		BhvrOverride
			Scale			0.025 0.025 0.025
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	90
		LifespanJitter	30
	End
End

#######################################################################

End