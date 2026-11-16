#############################################################
## Roach_BodyAndTrail.fx
#############################################################

FxInfo
#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		Type	Local
		At	Root
		WHILE	MALE
		ChildFX	:Water_Splash_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		ChildFX	:Water_Splash_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		Type	Local
		At	Root
		WHILE	HUGE
		ChildFX	:Water_Splash_CHILD.fx
	End
End

#########################################################

Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FORWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		WHILE 	FORWARD
		Flags	OneAtATime
		ChildFx :Water_Path_CHILD.fx
	End
End

Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     BACKWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		WHILE 	BACKWARD
		Flags	OneAtATime
		ChildFx :Water_Path_CHILD.fx
	End
End

Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPLEFT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		WHILE 	STEPLEFT
		Flags	OneAtATime
		ChildFx :Water_Path_CHILD.fx
	End
End

Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPRIGHT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		WHILE 	STEPRIGHT
		Flags	OneAtATime
		ChildFx :Water_Path_CHILD.fx
	End
End

#############################################################

End