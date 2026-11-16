#########################################################
##	MH
## 	Hand clap

FxInfo

Input
	InpName Origin
End

Lifespan 10

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound clap1 30 30 .45
		Sound clap2 30 30 .40
		Sound clap3 30 30 .50
	End

End


End
