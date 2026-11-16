#########################################################
##	MH
## 	Male choke

FxInfo

Input
	InpName Origin
End

LifeSpan 20


Condition

	On	Time
	Time	15
	Event
		Type Local
		At origin
		Sound traindooropen 90 90 .65
	End
End

