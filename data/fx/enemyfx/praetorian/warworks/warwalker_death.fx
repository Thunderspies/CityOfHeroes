#############################################################
## Warwalker_Death.fx
#############################################################

FxInfo
Lifespan 2

#############################################################
Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Root
		Sound WW_StopServo_01 300 300 .9
		Sound WW_StopServo_02 300 300 .9
		Sound WW_StopServo_03 300 300 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_Death_01 200 200 .9
	End

End
