#############################################################
## FX_Bat_01.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Time
	Time	5
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Fish_Leap_01 35 35 .15
		Sound 	Fish_Leap_02 35 35 .15
		Sound 	Fish_Leap_03 35 35 .15
		Sound 	Fish_Leap_04 35 35 .15
		Sound 	Fish_Leap_05 35 35 .15
		lifespan 1
	End
End
Condition
	On 	Time
	Time 	25
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Fish_Splash_02 35 35 .25
		Sound 	Fish_Splash_03 35 35 .25
		Sound 	Fish_Splash_04 35 35 .25
		Sound 	Fish_Splash_05 35 35 .25
		lifespan 1
	End
End


#############################################################
End