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
		Sound WW_GoliathMoveServo_01 150 150 .5
		Sound WW_GoliathMoveServo_02 150 150 .5
		Sound WW_GoliathMoveServo_03 150 150 .5
	End

End
