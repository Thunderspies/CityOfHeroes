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
		Type	StartPositOnly
		At	Root #Origin
		Part1	:Steam_Low_Directional.part
		Part	:Steam_Low_Highlight_Directional.part
	#	Sound Steam4_Loop 35.0 35.0 .1
	End
End

#############################################################

End