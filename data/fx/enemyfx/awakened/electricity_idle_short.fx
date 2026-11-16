#############################################################
## Electricity_Idle_Short.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	30
	Chance 	0.75
	Random	1

	Event
		EName	Idle1
		Type	Local
		At	Root
		ChildFX	:CHILD_Electricity_Idle_Short.fx
		Lifespan 25
	End

	Event
		EName	Idle2
		Type	Local
		At	Root
		ChildFX	:CHILD_Electricity_Idle_Short_Offset.fx
		Lifespan 25
	End
End

#############################################################

End