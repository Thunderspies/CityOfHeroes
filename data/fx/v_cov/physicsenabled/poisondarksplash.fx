#############################################################
## PoisonDarkSplash.fx
#############################################################

FxInfo

LifeSpan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	:Icecubes.bhvr
		BhvrOverride
			physRadius 		0.25
		End
		Part1	:PoisonDarkDropStreak.part
		Lifespan	60
		LifespanJitter	15
		CEvent 	:PoisonDarkSplashHit.fx
		CThresh 	0.00001
		CDestroy 	1
	End
End

#############################################################

End