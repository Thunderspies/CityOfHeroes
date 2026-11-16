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

		Part1	:BaleFireSparks.part
				
		Lifespan	45
		LifespanJitter	15
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		Type	start
		At	origin

		BhvrOverride
			StartJitter	1 .1 1
		End
		Part1	:HellFire.part


		Lifespan	60
		LifespanJitter	15
	End

End

End
