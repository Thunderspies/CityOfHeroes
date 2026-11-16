#############################################################
## StoneFist_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	BodySpines_Male
		Type	Local
		Flags	OneAtATime
		At	Root
		While	MALE
		ChildFX	:Child_Male_MagmaFist.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	BodySpines_Female
		Type	Local
		At	Root
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_Fem_MagmaFist.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	BodySpines_Huge
		Type	Local
		At	Root
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_Huge_MagmaFist.fx
	End
End

#############################################################

End