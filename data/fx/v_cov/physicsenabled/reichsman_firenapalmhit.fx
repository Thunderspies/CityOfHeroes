#########################################################
##	PhysicsRing
##
FxInfo

LifeSpan 120

Condition
	On 	Time
	Time 	0


	Event
		Type	start
		At	origin

		BhvrOverride
			StartJitter	.1 .1 .1
		End
		Part1	:Reichsman_FireSplashSmoke.part
		Part2	:Reichsman_FireSplashEmbers.part
		Lifespan	75
		LifespanJitter	30

	End
End

Condition
	On 	Time
	Time 	0
	Chance	.5


	Event
		Type	start
		At	origin

		BhvrOverride
			StartJitter	.1 .1 .1
		End
		Part1	:Reichsman_FireSplashEmbers.part
		Lifespan	60
		LifespanJitter	30

	End
End

Condition
	On 	Time
	Time 	0
	Repeat	2
	RepeatJitter	2

	Event
		Type	start
		At	origin

		BhvrOverride
			StartJitter	.3 .3 .3
		End
		Part1	:Reichsman_FireSplashBig.part
		Lifespan	75
		LifespanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	2
	RepeatJitter	2

	Event
		Type	start
		At	origin

		BhvrOverride
			StartJitter	.5 .5 .5
		End
		Part2	:Reichsman_FireSplash.part
		Lifespan	90
		LifespanJitter	30

	End

End

End
