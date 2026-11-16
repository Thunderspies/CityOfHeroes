#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 100

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	:Icecubes.bhvr
		BhvrOverride
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End

		Part1	:RockDebrisCoral02.part
		Lifespan	75
		LifespanJitter	15
		
		CThresh 	.00001

		End

End

End
