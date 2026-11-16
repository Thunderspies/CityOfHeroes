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
		Part1	:Waterfall_Stream_1.part
		Part2	:Waterfall_Stream_2.part
		Part2	:Waterfall_Stream_3.part
		Part	:Waterfall_Stream_Drips.part

		#Sound	waterfall2_loop 190 100 .66
		
	End
End


End			