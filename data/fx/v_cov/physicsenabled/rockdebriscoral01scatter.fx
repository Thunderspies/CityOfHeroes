#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 1100

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

		Part1	:RockDebrisCoral01.part
		Lifespan	1175
		LifespanJitter	115
		
		CThresh 	.00001

		End

End

End
