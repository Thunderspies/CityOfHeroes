#############################################################
## PsychicBattery_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	45

	Event
		EName	Idle1
		Type	Local
		At	Root
		ChildFX	:CHILD_PsychicBattery_Continuing.fx
		Lifespan 38
	End
End

#############################################################

End