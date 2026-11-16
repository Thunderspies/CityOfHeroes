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
		ChildFX	:Roach_TrailOnlySplode_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		ChildFX	:Roach_TrailOnlySplode_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		Type	Local
		At	Root
		WHILE	HUGE
		ChildFX	:Roach_TrailOnlySplode_CHILD.fx
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
			ChildFx :Roach_TrailOnly_CHILD.fx
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
			ChildFx :Roach_TrailOnly_CHILD.fx
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
			ChildFx :Roach_TrailOnly_CHILD.fx
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
			ChildFx :Roach_TrailOnly_CHILD.fx
	End
End

###############


#########################################################
End