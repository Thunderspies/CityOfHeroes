#########################################################
##	cot_waterfall_long.fx
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	:Waterfall_long_1.part
		Part2	:Waterfall_long_2.part
		Sound waterfall2_loop 190 190 .66
		
	End
End


End			