#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 2

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_GoliathAttackServo_01 400 400 .9
		Sound WW_GoliathAttackServo_02 400 400 .9
		Sound WW_GoliathAttackServo_03 400 400 .9
	End

End
