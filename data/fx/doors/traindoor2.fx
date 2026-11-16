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
	Time	0
	Event
		Type Local
		At origin
		Sound traindoorclose 90 90 .65
	End
End

