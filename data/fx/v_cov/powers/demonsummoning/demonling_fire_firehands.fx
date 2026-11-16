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
		Part1	:Demonling_Hands_Fire.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan 90
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:Demonling_Hands_Fire.part
		Sound Ignite2 100.0 100.0 .8
		LifeSpan 90
	End

	Event
		Type	Local
		At	WepL
		Sound Rumble7 100. 100. .73
	End
End

##############################################################

End