#############################################################
## FireHands.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Part	:FireHandz_Alpha.part
		Sound Firehand_loop 100.0 100.0 0.45
		LifeSpan 90
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Part	:FireHandz_Alpha.part

		Sound Ignite2 100.0 100.0 0.8
		LifeSpan 90
	End

	Event
		Type	Local
		At	WepL
		Sound Rumble7 100. 100. 0.73
	End
End

##############################################################

End