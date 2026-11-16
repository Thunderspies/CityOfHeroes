#############################################################
## HandPatSmall_SFX.fx
#############################################################

FxInfo

Lifespan 100

Condition
	On Time
	Time 1
	Event
		Type	Local
		At	WepR
		Sound Grenade_Arm_01 50 50 .35
	End
End

Condition
	On Time
	Time 50
	Event
		Type	Local
		At	WepR
		Sound whooshsmall 50 50 .35
	End
End

Condition
	On Time
	Time 53
	Event
		Type	Local
		At	WepR
		Sound Grenade_Release_01 50 50 .35
	End
End

Condition
	On Time
	Time 57
	Event
		Type	Local
		At	WepR
		Sound BaneBomb_DownHole 50 50 .35
	End
End







#############################################################
