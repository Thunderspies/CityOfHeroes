#############################################################
## ForceCage_Conditional.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		Flags	OneAtATime
		While	MALE
		ChildFX	:Child_ForceCage_Male.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	MotionBlur_Fem
		Type	Local
		At	Root
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_ForceCage_Female.fx
	End
End

Condition
	On 	TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	MotionBlur_Huge
		Type	Local
		At	Root
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_ForceCage_Huge.fx
	End
End

#############################################################

End