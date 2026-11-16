#########################################################
##	PhysicsRing
##
FxInfo  

Lifespan 	60

Condition
	On 	Time
	Time 	0
	Repeat 	63

	Event
		Type	start
		At	origin

		Bhvr	behaviors/ringburst.bhvr
		Part1	:FireHands2Small.part
		Part2	:FireHands2Small.part
		Part3	:FireHands2Small.part
		Lifespan	40
		Lifespanjitter 	20
	End

End


End			
