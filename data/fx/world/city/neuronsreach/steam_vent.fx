#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	:Steam_Vent.part
		Part	:Steam_Vent_Alpha.part
		Part	:Steam_Vent_Highlight.part
		Sound ReactorVent1_Loop 50 50 .25
	End
End

#############################################################

End