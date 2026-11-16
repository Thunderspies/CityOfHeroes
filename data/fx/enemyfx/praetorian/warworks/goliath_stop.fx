#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 15

#############################################################
Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound WW_GoliathStopServo_01 500 500 .9
		Sound WW_GoliathStopServo_02 500 500 .9
		Sound WW_GoliathStopServo_03 500 500 .9
	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_GoliathFF_01 400 400 .65
		Sound WW_GoliathFF_02 400 400 .65
		Sound WW_GoliathFF_03 400 400 .65
		Sound WW_GoliathFF_04 400 400 .65
		Sound WW_GoliathFF_05 400 400 .65
		Sound WW_GoliathFF_06 400 400 .65
		Sound WW_GoliathFF_07 400 400 .65
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_GoliathFFClose_01 75 75 .9
		Sound WW_GoliathFFClose_02 75 75 .9
		Sound WW_GoliathFFClose_03 75 75 .9
		Sound WW_GoliathFFClose_04 75 75 .9
		Sound WW_GoliathFFClose_05 75 75 .9
		Sound WW_GoliathFFClose_06 75 75 .9
		Sound WW_GoliathFFClose_07 75 75 .9
	End

End
