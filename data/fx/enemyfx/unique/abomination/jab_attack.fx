#############################################################
## Jab_Attack.fx
#############################################################

FxInfo

######################################################################


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
	On 	Time
	Time 	30

	Event
		EName	All
		Type	Destroy

	End


End

End
##################################

