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
		ChildFx :Wealthy_Head_Child2.fx
		EName Money
		LifeSpan 90
	End
End

###########################################################

End
