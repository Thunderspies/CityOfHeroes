###############################################################
## PhysFX_Rock_Falling.fx
###############################################################

FxInfo
Lifespan 40

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin
		Bhvr	:Rock_Falling.bhvr
		Lifespan	200
		BhvrOverride
			PositionOffset	0 0.5 0
			Scale		0.2 0.2 0.2
		End
		Geom	GEO_FX_TESTSPHERE
		CEvent 	:Physics_WaterDroplet_CEvent.fx
		Lifespan 	50
		CThresh 	.001
		CDestroy 	1
	End
End