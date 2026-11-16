#############################################################
## Physics_Sparks.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest

		Bhvr	V_COV\PhysicsEnabled\Ash_Demon.bhvr
		Part	:Physics_Hellfire_Sparks.part
		Lifespan	90
		LifespanJitter	30
		CThresh 	.001
		CDestroy 	0
	End

End

#############################################################

End
