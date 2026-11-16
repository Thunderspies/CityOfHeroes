###############################################################
## PhysFX_Rock_Falling.fx
###############################################################

FxInfo
Lifespan 80
#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\RockProjectile.bhvr
		BhvrOverride
			PositionOffset	0 20 0
		End
		Part	:Offset_Rocks.part
		Lifespan 	40
		LifespanJitter 	10
		CThresh 	.001
		CDestroy 	0
	End
#########################################################

End