#########################################################
## 	Flight
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 30

Condition
	Event
		Type Local
		At origin
		Sound FlightHover1 80.0 40.0 .15
		Sound FlightHover2 80.0 40.0 .15
		Sound FlightHover3 80.0 40.0 .15
	End

End