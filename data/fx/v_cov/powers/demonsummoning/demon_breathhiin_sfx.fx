#############################################################
## Death_Fire_Demon.fx
#############################################################

FxInfo
Lifespan 1

#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound Demon_BreathHi_In_01 50 50 .65
		Sound Demon_BreathHi_In_02 50 50 .65
		Sound Demon_BreathHi_In_03 50 50 .65
		Sound Demon_BreathHi_In_04 50 40 .65
		Sound Demon_BreathHi_In_05 50 40 .65
	End


#########################################################

End