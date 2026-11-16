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
		Sound Hamidon_StopVox_01 200 200 .9
		Sound Hamidon_StopVox_02 200 200 .9
		Sound Hamidon_StopVox_03 200 200 .9
	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound Hamidon_FF_01 200 200 .8
		Sound Hamidon_FF_03 200 200 .8
		Sound Hamidon_FF_05 200 200 .8
	End

End