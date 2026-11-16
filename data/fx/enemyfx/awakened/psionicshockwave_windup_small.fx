#############################################################
## PsychicLash_Windup_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Windup1
		Type	Local
		At	Root
		ChildFX	:CHILD_PsionicShockwave_Windup_Small.fx
		Lifespan 60
	End

	Event
		EName	Windup2
		Type	Local
		At	Root
		ChildFX	:CHILD_PsionicShockwave_Windup_Small_Offset.fx
		Lifespan 60
	End
End

#############################################################

End