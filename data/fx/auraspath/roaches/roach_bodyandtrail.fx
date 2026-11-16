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
		ChildFX	:Roach_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		ChildFX	:Roach_CHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		Type	Local
		At	Root
		WHILE	HUGE
		ChildFX	:Roach_CHILD.fx
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
			ChildFx :Roach_Trail_CHILD.fx
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
			ChildFx :Roach_Trail_CHILD.fx
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
			ChildFx :Roach_Trail_CHILD.fx
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
			ChildFx :Roach_Trail_CHILD.fx
	End
End

###############

	Condition
		On Cycle
		Time 5
		Random 1
		Chance 0.6

		Event
			At Chest
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At WepL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At WepR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LLegL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LLegR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At UArmL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At UArmR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LArmL
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

		Event
			At LArmR
			Type Local
			Part :Roach_Fall.part
			LifeSpan 3
		End

	End


#########################################################
End