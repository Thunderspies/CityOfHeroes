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
		Until	AIR
		ChildFX	:Water_Splash_CHILD3.fx
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
		ChildFX	:Water_Splash_CHILD3.fx
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
		ChildFX	:Water_Splash_CHILD3.fx
	End
End

#########################################################

End