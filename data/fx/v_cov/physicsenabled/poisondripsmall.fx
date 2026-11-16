#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 75

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	:Icecubes.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			InitialVelocity		0.0 0.1 0.0	
			InitialVelocityJitter	0.15 0.15 0.15
			physRadius 		0.25
		End

		Part1	:PoisonDropStreakSmall.part
		Lifespan	60
		LifespanJitter	15
		
		CEvent 	:PoisonSplashHitSmall.fx
		CThresh 	.00001
		CDestroy 	1
	End

End

End
