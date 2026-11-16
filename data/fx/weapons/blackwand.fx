#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	InitialStaff
		Type	Local
		At	WepR
		Until	NONHUMAN
		ChildFX	:Child_Blackwand.fx
		Sound DiviningRodOut 50 50 0.65
	End
End

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		EName 	InitialStaff
		Type	Destroy
	End
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	MALE
	DoMany	1

	Event
		EName 	Male_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	MALE
		ChildFX	:Child_Blackwand.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	FEMALE
	DoMany	1

	Event
		EName 	Female_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_Blackwand.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	HUGE
	DoMany	1

	Event
		EName 	Huge_PostFormShift
		Type	Local
		At	WepR
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_Blackwand.fx
	End
End

#############################################################

End