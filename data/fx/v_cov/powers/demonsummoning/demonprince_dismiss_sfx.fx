#############################################################
## Death_Fire_Demon.fx
#############################################################

FxInfo
Lifespan 60

#############################################################


Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound DemonPrince_DismissVOX_01 200 200 1
	End

End

Condition
	On	Time
	Time 0
	Event
		Type	Local
		At	Root
		Sound DemonPrince_Dismiss 200 200 1
	End

End