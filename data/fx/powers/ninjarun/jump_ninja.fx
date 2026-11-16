#############################################################
## Jump_Ninja.fx
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		Sound NinjaRun_WhooshJump 100 100 0.35
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Root
		Sound NinjaRun_WhooshFlip 100 100 0.2
	End
End

#############################################################

End