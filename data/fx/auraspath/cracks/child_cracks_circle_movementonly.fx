#############################################################
## StormCloud_Trail.fx
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
		Until	FLY
		ChildFX	:CHILD_Cracks_Circle2_MovementOnly.fx

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
		Until	FLY
		ChildFX	:CHILD_Cracks_Circle2_MovementOnly.fx

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
		Until	FLY
		ChildFX	:CHILD_Cracks_Circle2_MovementOnly.fx

	End
End


#########################################################

End