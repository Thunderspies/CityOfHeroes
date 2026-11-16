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
		ChildFX	:BigSplash.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE

	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		ChildFX	:BigSplash.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE

	Event
		Type	Local
		At	Root
		WHILE	HUGE
		ChildFX	:BigSplash.fx
	End
End

#########################################################

End