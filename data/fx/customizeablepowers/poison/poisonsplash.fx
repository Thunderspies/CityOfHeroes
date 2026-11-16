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
		Part1	:PoisonDropStreak.part
		Lifespan	60
		LifespanJitter	15
		CEvent 	CustomizeablePowers\Poison\PoisonSplashHit.fx
		CThresh 	0.00001
		CDestroy 	1
	End
End

#############################################################

End