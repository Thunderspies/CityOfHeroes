#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 2

#############################################################
Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FF_01 200 200 .9
		Sound WW_FF_02 200 200 .9
		Sound WW_FF_03 200 200 .9
		Sound WW_FF_04 200 200 .9
		Sound WW_FF_05 200 200 .9
		Sound WW_FF_06 200 200 .9
		Sound WW_FF_07 200 200 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_Smash_01 200 200 .9
		Sound WW_Smash_02 200 200 .9
		Sound WW_Smash_03 200 200 .9
	End

End
