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
		BhvrOverride
			InitialVelocityJitter	0.4 0.2 0.4
		End
		Part	:Physics_Cold_Sparks.part
		Lifespan	90
		LifespanJitter	30
		CThresh 	.001
		CDestroy 	0
	End

End

#############################################################

End
