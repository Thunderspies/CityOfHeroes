#############################################################
## Death_Fire_Demonling.fx
#############################################################

FxInfo
LifeSpan 5

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound Demon_Swipe_01 100 100 .8
		Sound Demon_Swipe_02 100 100 .8
		Sound Demon_Swipe_03 100 100 .8

	End

End