#############################################################
## Consume_Attack.fx
#############################################################

FxInfo
Lifespan 60

#############################################################


Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Windup1
		Type	Local
		At	Root
		ChildFX	:CHILD_Consume.fx
		Lifespan 25
	End

	Event
		EName	Windup2
		Type	Local
		At	Root
		ChildFX	:CHILD_Consume_Offset.fx
		Lifespan 25
	End

End

Condition
	On	Cycle
	Time	30
	Random	1

	Event
		EName	Windup1
		Type	Local
		At	Root
		ChildFX	:CHILD_Consume.fx
		Lifespan 25
	End

	Event
		EName	Windup2
		Type	Local
		At	Root
		ChildFX	:CHILD_Consume_Offset.fx
		Lifespan 25
	End
End

#############################################################

End