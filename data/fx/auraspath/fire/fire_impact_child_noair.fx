#############################################################
## Fire_Impact_CHILD.fx
#############################################################

FxInfo

#########################################################

Condition
	ON		TRIGGERBITS
	TRIGGERBITS 	MALE
	DoMany 	1

	Event
		EName	3
		Type	Start
		At	Root
		Flags	OneAtATime
		Until	AIR
		ChildFX	:Fire_Impact_CHILD_NOFLY.fx

	End
End

Condition
	ON		TRIGGERBITS
	TRIGGERBITS 	FEMALE
	DoMany 	1

	Event
		EName	3
		Type	Start
		At	Root
		Flags	OneAtATime
		Until	AIR
		ChildFX	:Fire_Impact_CHILD_NOFLY.fx

	End
End

Condition
	ON		TRIGGERBITS
	TRIGGERBITS 	HUGE
	DoMany 	1

	Event
		EName	3
		Type	Start
		At	Root
		Flags	OneAtATime
		Until	AIR
		ChildFX	:Fire_Impact_CHILD_NOFLY.fx

	End
End
#########################################################

End