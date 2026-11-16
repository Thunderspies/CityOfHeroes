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
		Sound WW_GoliathServo_01 300 300 .75
		Sound WW_GoliathServo_02 300 300 .75
		Sound WW_GoliathServo_03 300 300 .75
		Sound WW_GoliathServo_04 300 300 .75
		Sound WW_GoliathServo_05 300 300 .75
		Sound WW_GoliathServo_06 300 300 .75
	End

End
