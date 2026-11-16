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

		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			InitialVelocity		0.0 0.01 0.0	
			InitialVelocityJitter	0.05 0.05 0.05
			physRadius 		0.25
			physGravity 		.2
			Gravity			0.01
			FadeOutLength 		160
		End

		Part1	:PoisonDropStreakSmall.part
		Lifespan	160
		LifespanJitter	55
		
		CEvent 	:PoisonSplashHitSmall.fx
		CThresh 	.00001
		CDestroy 	1
	End

End

End
