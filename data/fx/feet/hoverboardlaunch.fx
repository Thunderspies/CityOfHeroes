#########################################################
##	MH
## 	Male Flight

FxInfo

Input
	InpName Origin
End

LifeSpan 30


Condition
	On Time
	Time 0
	Event
		Type Local
		At origin
		Sound HoverboardLaunch1 100 50 .9
	End
End


Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	FF_Metal_01 100 100 .3
	End
End
Condition
	On 	Time
	Time	26
	Event
		Type	Local
		At	Root
		Sound 	FF_Concrete_01 100 100 .4
	End
End
