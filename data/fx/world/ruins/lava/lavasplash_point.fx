#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################


Condition
	On		Cycle
	Time		15
	Chance		0.5

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		CHILDFX	:CHILD_LavaBurst.fx
		SoundNoRepeat 2
		Sound Lava_Spew_01 50 50 .35
		Sound Lava_Spew_02 50 50 .35
		Sound Lava_Spew_03 50 50 .35
		Sound Lava_Spew_04 50 50 .35
		Sound Lava_Spew_05 50 50 .35
		Lifespan 	5
	End
End


#############################################################

End