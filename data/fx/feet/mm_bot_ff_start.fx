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
		Sound MM_Bot_FF_01 50 50 .25
		Sound MM_Bot_FF_03 50 50 .25
		Sound MM_Bot_FF_05 50 50 .25
		Sound MM_Bot_FF_07 50 50 .25
		Sound MM_Bot_FF_09 50 50 .25
		Sound MM_Bot_FF_11 50 50 .25
		Sound MM_Bot_FF_13 50 50 .25
	End
End

Condition
	Chance 0.5
	Event
		Type	Local
		At	origin
		SoundNoRepeat 3
		Sound MM_Bot_Loco_Start_01 100 100 .25
		Sound MM_Bot_Loco_Start_02 100 100 .25
		Sound MM_Bot_Loco_Start_03 100 100 .25
		Sound MM_Bot_Loco_Start_04 100 100 .25
		Sound MM_Bot_Loco_Start_05 100 100 .25
	End
End
