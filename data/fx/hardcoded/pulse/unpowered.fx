#########################################################
## Unpowered
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound luminous1_loop 33 33 .43
	End
End



Condition
	On Cycle
	Time 5
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	:unpowered.bhvr
		LifeSpan 10

	End
End
