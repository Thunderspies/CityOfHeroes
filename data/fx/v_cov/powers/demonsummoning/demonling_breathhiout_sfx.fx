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
		Sound Demon_BreathHi_Out_01 50 50 .4
		Sound Demon_BreathHi_Out_02 50 50 .4
		Sound Demon_BreathHi_Out_03 50 50 .4
		Sound Demon_BreathHi_Out_04 50 40 .4
		Sound Demon_BreathHi_Out_05 50 40 .4
	End


#########################################################

End