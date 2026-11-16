#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 20

#############################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound Abomination_Servo_01 150 150 .5
		Sound Abomination_Servo_02 150 150 .5
		Sound Abomination_Servo_03 150 150 .5
		Sound Abomination_Servo_04 150 150 .5
		Sound Abomination_Servo_05 150 150 .5
		Sound Abomination_Servo_06 150 150 .5
	End

End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 100 100 .65
		Sound WW_FFClose_02 100 100 .65
		Sound WW_FFClose_03 100 100 .65
		Sound WW_FFClose_04 100 100 .65
		Sound WW_FFClose_05 100 100 .65
		Sound WW_FFClose_06 100 100 .65
		Sound WW_FFClose_07 100 100 .65
	End

End
