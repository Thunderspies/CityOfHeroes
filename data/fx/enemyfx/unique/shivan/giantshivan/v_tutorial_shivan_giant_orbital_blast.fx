#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo
LifeSpan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	BigMeteo
		Type	Start
		At	T_Root
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	BigMeteo
		ChildFX	:Meteor_Attack_CHILD.fx
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	BigMeteo
		ChildFX	:Meteor_Crash.fx
	End
End

#############################################################

End

