#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
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
		Sound WW_GoliathStopServo_01 500 500 .9
		Sound WW_GoliathStopServo_02 500 500 .9
		Sound WW_GoliathStopServo_03 500 500 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_GoliathDeath_01 400 400 .9
	End

End
