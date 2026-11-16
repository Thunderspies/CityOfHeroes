#
FxInfo

	LifeSpan 200

Input
	InpName	Hips
End

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_Dehydrate 100 100 .8
		ChildFX :Dehydrate_Child.fx
	End
End



End
