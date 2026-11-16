#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Mist
		Type	PositOnly
		At	Root
		Part	:Mist.part
		Part	:Sparkles.part
	End
End

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
		ChildFX	:CHILD_Frost_Circle2.fx

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
		ChildFX	:CHILD_Frost_Circle2.fx

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
		ChildFX	:CHILD_Frost_Circle2.fx

	End
End




#########################################################

End