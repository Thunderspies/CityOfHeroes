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
		Sound DemonLing_DismissVOX_01 200 200 1
		Sound DemonLing_DismissVOX_02 200 200 1
		Sound DemonLing_DismissVOX_03 200 200 1
	End
End