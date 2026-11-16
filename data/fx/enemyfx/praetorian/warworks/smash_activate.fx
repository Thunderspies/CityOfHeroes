#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################


Condition
	On		TRIGGERBITS
	TriggerBits	PUNCH LEFT

	Event
		EName	Punch_Left
		Type	Local
		At	WepR
		Until	PUNCH RIGHT
		ChildFX	:Child_Smash_Left.fx
	End
End

Condition
	On		TRIGGERBITS
	TriggerBits	PUNCH RIGHT

	Event
		EName	Punch_Right
		Type	Local
		At	WepR
		Until	PUNCH LEFT
		ChildFX	:Child_Smash_Right.fx
	End
End

End
