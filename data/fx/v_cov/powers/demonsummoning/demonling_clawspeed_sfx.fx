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
		Sound Demonling_SwipeFlurry_01 100 100 .8
		Sound Demonling_SwipeFlurry_02 100 100 .8
		Sound Demonling_SwipeFlurry_03 100 100 .8

	End

End