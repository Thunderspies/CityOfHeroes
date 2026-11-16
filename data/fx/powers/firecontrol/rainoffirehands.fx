#############################################################
## RainOfFireHands.fx
#############################################################

FxInfo

Lifespan 70

#################################################################

Condition
	On	Time
	Time	20

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	60

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .8

		LifeSpan	60
	End

End


End

##############################################################

