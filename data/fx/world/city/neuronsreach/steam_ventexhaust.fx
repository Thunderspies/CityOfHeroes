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
		Part	:Steam_High_Directional.part
		Part	:Steam_High_Highlight_Directional.part
	#	Sound Steam4_Loop 35.0 35.0 .1
	End
End

#############################################################

End