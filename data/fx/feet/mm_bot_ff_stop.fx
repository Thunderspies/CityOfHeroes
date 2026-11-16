#########################################################
##	MH
## 	Female Left Foot

FxInfo

Input
	InpName Origin
End

LifeSpan 5

Condition
	Event
		Type	Local
		At	origin
		SoundNoRepeat 4
		Sound MM_Bot_FF_01 50 50 .5
		Sound MM_Bot_FF_03 50 50 .5
		Sound MM_Bot_FF_05 50 50 .5
		Sound MM_Bot_FF_07 50 50 .5
		Sound MM_Bot_FF_09 50 50 .5
		Sound MM_Bot_FF_11 50 50 .5
		Sound MM_Bot_FF_13 50 50 .5
	End
End

Condition
	On Time
	Time 5
	Chance 0.5
	Event
		Type	Local
		At	origin
		SoundNoRepeat 3
		Sound MM_Bot_Loco_Stop_01 50 50 .25
		Sound MM_Bot_Loco_Stop_02 50 50 .25
		Sound MM_Bot_Loco_Stop_03 50 50 .25
		Sound MM_Bot_Loco_Stop_04 50 50 .25
		Sound MM_Bot_Loco_Stop_05 50 50 .25
	End
End
