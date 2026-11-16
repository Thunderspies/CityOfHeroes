#############################################################
## Death_Fire_Demon.fx
#############################################################

FxInfo
Lifespan 5

#############################################################


Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound DemonLieu_DismissVOX_01 200 200 1
		Sound DemonLieu_DismissVOX_02 200 200 1
		Sound DemonLieu_DismissVOX_03 200 200 1
	End
End
Condition
	On	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound DemonLieu_Dismiss_01 200 200 1
	End
End