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

		Bhvr	V_COV\PhysicsEnabled\Fire_Demon.bhvr
	#	Part	:Physics_Fire.part
		Part	:Physics_Fire.part
		Part	:Physics_Fire_Core.part
		Lifespan	90
		LifespanJitter	30

		CEvent 	:Physics_Fire_CEvent.fx
		CThresh 	.001
		CDestroy 	0
	End

End

#############################################################

End
