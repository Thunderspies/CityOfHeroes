#############################################################
## Wealthy_Head.fx
#############################################################

FxInfo

###########################################################

Condition
	On Cycle
	Time 25

	Event
		At Origin
		Type Start
		ChildFx :Wealthy_LeftHand_Child.fx
		EName Money2
		LifeSpan 90
	End

End

###########################################################

End
