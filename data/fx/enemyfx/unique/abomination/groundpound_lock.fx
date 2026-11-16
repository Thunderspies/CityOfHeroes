#############################################################
## GroundPound_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################


Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	Sewer_ElectricBurstHigh_Loop 100 100 .8
		lifespan 55
	End
End


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 150 150 .5
		Sound WW_MoveServo_02 150 150 .5
		Sound WW_MoveServo_03 150 150 .5
	End
End

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End

Condition
	On 	Time
	Time	50
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End

Condition
	On 	Time
	Time	70
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FF_01 200 200 .65
		Sound WW_FF_02 200 200 .65
		Sound WW_FF_03 200 200 .65
		Sound WW_FF_04 200 200 .65
		Sound WW_FF_05 200 200 .65
		Sound WW_FF_06 200 200 .65
		Sound WW_FF_07 200 200 .65
	End
End




Condition
	On	Time
	Time	40


	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .8
		Sound WW_FFClose_02 75 75 .8
		Sound WW_FFClose_03 75 75 .8
		Sound WW_FFClose_04 75 75 .8
		Sound WW_FFClose_05 75 75 .8
		Sound WW_FFClose_06 75 75 .8
		Sound WW_FFClose_07 75 75 .8
	End
End

Condition
	On	Time
	Time	61



	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .8
		Sound WW_FFClose_02 75 75 .8
		Sound WW_FFClose_03 75 75 .8
		Sound WW_FFClose_04 75 75 .8
		Sound WW_FFClose_05 75 75 .8
		Sound WW_FFClose_06 75 75 .8
		Sound WW_FFClose_07 75 75 .8
	End
End

#############################################################

End
