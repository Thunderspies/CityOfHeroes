#########################################################
##	guts
##
FxInfo
	
LifeSpan 0

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

Condition
	On 	Time
	Time 	11

	Event
		EName 	Gut_R
		Type	Local 
		At	WepR
		Part1	:gut1.part

	End

End

Condition
	On 	Time
	Time 	11

	Event
		EName 	Gut_L
		Type	Local 
		At	WepL
		Part1	:gut1.part

	End
End

Condition
	On 	Time
	Time 	30  ##52

	Event
		EName 	Gut_R
		Type	Destroy 
	End

End


Condition
	On 	Time
	Time 	70  ##90

	Event
		EName 	Gut_L
		Type	Destroy 
	End

End

End			