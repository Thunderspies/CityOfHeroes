#############################################################
## Shrubbery_IdleAndTrail.fx
#############################################################

FxInfo
###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	Domany 1

	Event
		EName	MaleAura
		Type	Local
		At	Root
		WHILE	MALE
		Flags	OneAtATime
		ChildFX	:Shrubbery_Idle_CHILD.fx
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
		WHILE	FEMALE
		Flags	OneAtATime
		ChildFX	:Shrubbery_Idle_CHILD.fx
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
		WHILE	HUGE
		Flags	OneAtATime
		ChildFX	:Shrubbery_Idle_CHILD.fx
	End
End
###########################################################

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
			ChildFx :Shrubbery_Trail_CHILD.fx
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
			ChildFx :Shrubbery_Trail_CHILD.fx
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
			ChildFx :Shrubbery_Trail_CHILD.fx
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
			ChildFx :Shrubbery_Trail_CHILD.fx
	End
End

End
