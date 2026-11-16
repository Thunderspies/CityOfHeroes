#############################################################
## PoisonSplash.fx
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			physRadius 		0.25
		End
		Part1	V_COV\PhysicsEnabled\PoisonDropStreak.part
		Lifespan	60
		LifespanJitter	15
		CEvent 	V_COV\PhysicsEnabled\PoisonSplashHit.fx
		CThresh 	0.00001
		CDestroy 	1
	End
End

#############################################################

End