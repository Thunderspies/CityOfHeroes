#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	3
	Event
		Type	Local
		At	Root
		Sound 	Brawling_HeavyBlow_Attack_01 200 100 .8
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
		ChildFX	:Child_HeavyBlow_Left.fx
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
		ChildFX	:Child_HeavyBlow_Right.fx
	End
End
#############################################################

End