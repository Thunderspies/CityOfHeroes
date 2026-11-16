#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	Brawling_InitialStrike_Attack_01 200 100 .8
	End
End

Condition
	On		TRIGGERBITS
	TriggerBits	PUNCH LEFT

	Event
		EName	Punch_Left
		Type	Local
		At	WepL
		Until	PUNCH RIGHT
		ChildFX	:Child_InitialStrike_Left.fx
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
		ChildFX	:Child_InitialStrike_Right.fx
	End
End
#############################################################

End