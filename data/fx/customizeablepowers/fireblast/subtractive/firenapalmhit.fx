#############################################################
## FireNapalmHit.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		BhvrOverride
			StartJitter	0.1 0.1 0.1
		End
		Part1	:FireSplashSmoke.part
		Part2	:FireSplashEmbers.part
		Lifespan	75
		LifespanJitter	30

	End
End

#############################################################

End