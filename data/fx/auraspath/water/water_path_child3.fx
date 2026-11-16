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
		ChildFX	:Water_Trail_GRANDCHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		ChildFX	:Water_Trail_GRANDCHILD.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		Type	Local
		At	Root
		WHILE	HUGE
		ChildFX	:Water_Trail_GRANDCHILD.fx
	End
End

#########################################################

End