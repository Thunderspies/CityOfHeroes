#############################################################
## Binary_Eyes.fx
#############################################################

FxInfo

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Binary_Sm.fx
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Binary_Sm.fx
	End

End

#############################################################

End