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
		ChildFX	:Water_Path_Child2.fx

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
		ChildFX	:Water_Path_Child2.fx

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
		ChildFX	:Water_Path_Child2.fx

	End
End


#########################################################

End