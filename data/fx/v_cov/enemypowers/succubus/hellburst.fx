#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 90
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	:HellBurst.bhvr
		Part1	:HellFireCandle.part

		CThresh 	.0001
		CDestroy 	0
		CEvent		:HellFire.fx

		Lifespan	60
		LifespanJitter	15
	End

End

End
