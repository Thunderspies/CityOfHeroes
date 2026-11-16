#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	Domany 1

	Event
		EName	MaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	MALE
		ChildFX	:CHILD_Frost_Circle.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Domany 1

	Event
		EName	FemaleAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	FEMALE
		ChildFX	:CHILD_Frost_Circle.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	Domany 1

	Event
		EName	HugeAura
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	HUGE
		ChildFX	:CHILD_Frost_Circle.fx
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
		ChildFX	:CHILD_Ice_Trail_NOAIR.fx
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
		ChildFX	:CHILD_Ice_Trail_NOAIR.fx
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
		ChildFX	:CHILD_Ice_Trail_NOAIR.fx
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
		ChildFX	:CHILD_Ice_Trail_NOAIR.fx
	End
End



#########################################################

End