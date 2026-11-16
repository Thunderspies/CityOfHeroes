#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################


Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany 1

	Event
		EName	MaleAura
		Type	Local
		At	Root
		WHILE	MALE
		Flags	OneAtATime
		ChildFX	:CHILD_Cracks_Circle_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany 1

	Event
		EName	FemaleAura
		Type	Local
		At	Root
		WHILE	FEMALE
		Flags	OneAtATime
		ChildFX	:CHILD_Cracks_Circle_MovementOnly.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany 1

	Event
		EName	HugeAura
		Type	Local
		At	Root
		WHILE	HUGE
		Flags	OneAtATime
		ChildFX	:CHILD_Cracks_Circle_MovementOnly.fx
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
		ChildFX	:CHILD_Cracks_Trail2.fx
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
		ChildFX	:CHILD_Cracks_Trail2.fx
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
		ChildFX	:CHILD_Cracks_Trail2.fx
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
		ChildFX	:CHILD_Cracks_Trail2.fx
	End
End



#########################################################

End