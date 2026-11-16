#########################################################
##	FireRingCastersHands
##
FxInfo
	
LifeSpan 0

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Condition
	On 	Time
	Time 	0
	Event
		EName 	Next
		Type	Local 
		At	WepL
		Part1	powers/firecontrol/fireringCastersHands.part	
		
	End

	
	Event
		EName 	Last
		Type	Local 
		At	WepR
		Part1	powers/firecontrol/fireringCastersHands.part	
		
	End
End

End			