#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 75

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		Type	start
		At	origin
		Bhvr	:Droplet.bhvr
		BhvrOverride
			physRadius 		0.25
		End
		Part1	:PoisonDropStreak.part	#:Sparks_Splash.part
		Lifespan	60
		LifespanJitter	15
		CEvent 	:PoisonSplashHit.fx
		CThresh 	.00001
		CDestroy 	1
	End
End

#############################################################

End