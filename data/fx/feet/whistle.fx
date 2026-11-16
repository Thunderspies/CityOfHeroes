#########################################################
##	MH
## 	Whistle

FxInfo

Input
	InpName Origin
End

LifeSpan 5

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound Whistle1 200 200 .8
	End
End