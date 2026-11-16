#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 75

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		Type	start
		At	origin

		Bhvr	:SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
			InitialVelocity		0.0 0.3 0.0	
			InitialVelocityJitter	0.2 0.25 0.2
			physRadius 		0.15
		End


		Part1	:PoisonDarkDropletStreak.part
		Lifespan	60
		LifespanJitter	15
		
		CThresh 	.00001
		CDestroy 	1
	End

	
	Event
		HardwareOnly
		Type	start
		At	origin

		Bhvr	:SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
			InitialVelocity		0.0 0.3 0.0	
			InitialVelocityJitter	0.2 0.25 0.2
			physRadius 		0.15
		End


		Part1	:PoisonDarkDropletStreak.part
		Lifespan	60
		LifespanJitter	15
		
		CThresh 	.00001
		CDestroy 	1
	End

End

End
