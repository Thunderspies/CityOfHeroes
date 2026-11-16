#############################################################
## Wealthy_Head.fx
#############################################################

FxInfo

###########################################################

Condition
#	On	Time
#	Time	0

	On Cycle
	Time 25

	Event
		At Origin
		Type Start
		ChildFx :Wealthy_Head_Child2.fx
		EName Money
		LifeSpan 90
	End

	Event
		At Origin
		Type Start
		ChildFx :Wealthy_LeftHand_Child.fx
		EName Money2
		LifeSpan 90
	End

	Event
		At Origin
		Type Start
		ChildFx :Wealthy_LeftHand_Child.fx
		EName Money3
		LifeSpan 90
	End
End

###########################################################

End
