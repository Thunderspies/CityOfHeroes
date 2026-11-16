#############################################################
## HandPatSmall_SFX.fx
#############################################################

FxInfo

Lifespan 48

Condition
	On Time
	Time 0
	Event
		Type	Local
		At	WepR
		SoundNoRepeat 1
		Sound HandPatSmall1 30 30 .35
		Sound HandPatSmall2 30 30 .35
		Sound HandPatSmall3 30 30 .35
		Sound HandPatSmall4 30 30 .35
	End
End

Condition
	On Time
	Time 2
	Event
		Type	Local
		At	WepR
		SoundNoRepeat 1
		Sound HandPatSmall1 30 30 .45
		Sound HandPatSmall2 30 30 .45
		Sound HandPatSmall3 30 30 .45
		Sound HandPatSmall4 30 30 .45
	End
End

Condition
	On Time
	Time 8
	Event
		Type	Local
		At	WepR
		SoundNoRepeat 1
		Sound Hand_Rub_01 30 30 .05
		Sound Hand_Rub_02 30 30 .05
		Sound Hand_Rub_03 30 30 .05
		Sound Hand_Rub_04 30 30 .05
	End
End


Condition
	On Time
	Time 35
	Event
		Type	Local
		At	WepR
		SoundNoRepeat 1
		Sound ClothMovement_Short_01 30 30 .05
		Sound ClothMovement_Short_02 30 30 .05
		Sound ClothMovement_Short_03 30 30 .05
		Sound ClothMovement_Short_04 30 30 .05
	End
End
#############################################################
