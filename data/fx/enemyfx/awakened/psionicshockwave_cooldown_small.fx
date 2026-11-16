#############################################################
## PsychicLash_Windup_Small.fx
#############################################################

FxInfo
Lifespan 130

#############################################################

Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Windup1
		Type	Local
		At	Root
		ChildFX	:CHILD_PsionicShockwave_Cooldown_Small.fx
		Lifespan 110
	End

	Event
		EName	Windup2
		Type	Local
		At	Root
		ChildFX	:CHILD_PsionicShockwave_Cooldown_Small_Offset.fx
		Lifespan 110
	End
End

#############################################################

End