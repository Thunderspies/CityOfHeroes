#########################################################
##	PhysicsRing
##
FxInfo  

LifeSpan 75

Condition
	On 	Time
	Time 	0
	Repeat	2

	Event
		Type	start
		At	origin

		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
			InitialVelocity		0.0 0.07 0.0	
			InitialVelocityJitter	0.05 0.03 0.05
			physRadius 		0.15
			physGravity 		.2
			Gravity			0.01
			FadeOutLength 		160

		End


		Part1	:PoisonDropletStreakSmall.part
		Lifespan	160
		LifespanJitter	55
		
		#CEvent 	:PoisonSplashHit.fx
		CThresh 	.00001
		CDestroy 	1
	End

	
	Event
		HardwareOnly
		Type	start
		At	origin

		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
			InitialVelocity		0.0 0.07 0.0	
			InitialVelocityJitter	0.05 0.03 0.05
			physRadius 		0.15
		End


		Part1	:PoisonDropletStreakSmall.part
		Lifespan	160
		LifespanJitter	55
		
		#CEvent 	:PoisonSplashHit.fx
		CThresh 	.00001
		CDestroy 	1
	End

End

End
