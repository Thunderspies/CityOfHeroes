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
		Sound WW_AttackServo_01 200 200 .9
		Sound WW_AttackServo_02 200 200 .9
		Sound WW_AttackServo_03 200 200 .9
	End

End
