#########################################################
##	
## 	Dummy Fx

FxInfo


##LifeSpan 

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound Item_Heart1_loop 60 60 .4
	End
End

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound Heartbeat1_loop 70 70 1.0
	End
End