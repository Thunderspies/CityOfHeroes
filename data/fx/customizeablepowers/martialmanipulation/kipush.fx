#############################################################
## QuickStrike_Activate.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On		TRIGGERBITS
	TriggerBits	PUNCH LEFT

	Event
		EName	Punch_Left
		Type	Local
		At	WepR
		Sound KM_QuickStrike_Attack_01 200 200 .8
		Until	PUNCH RIGHT
		ChildFX	:KiPush_Child_Left.fx
	End
End

Condition
	On		TRIGGERBITS
	TriggerBits	PUNCH RIGHT

	Event
		EName	Punch_Right
		Type	Local
		At	WepR
		Sound KM_QuickStrike_Attack_01 200 200 .8
		Until	PUNCH LEFT
		ChildFX	:KiPush_Child_Right.fx
	End
End

#############################################################

End