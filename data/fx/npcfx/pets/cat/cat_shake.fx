#########################################################
## 	Huge Left Foot
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 30

Condition
	On Time
	Time 15
	Event
		Type	Local
		At	Origin
		Sound Cat_Shake_01 50 50 .25
	End

End

