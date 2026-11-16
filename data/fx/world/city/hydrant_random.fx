#############################################################
## Geyser_Hydrant.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	root
End

#############################################################

Condition
	On	Time
	Time	0
	Random	1

	Event
		Type	Start
		At	Origin
		ChildFX	:Geyser_Hydrant.fx
	End

	Event
		Type	Start
		At	Origin
		ChildFX	:Parent_Hydrant.fx
	End
End

#############################################################

End