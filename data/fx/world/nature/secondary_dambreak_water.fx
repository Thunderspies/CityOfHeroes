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
		Part1	:Secondary_Waterfall_long_1.part
		Part2	:Secondary_Waterfall_long_2.part
		Part2	:Secondary_Waterfall_long_3.part
		Part	:Secondary_waterfall_Vshape_Scroller.part

		#Sound	waterfall2_loop 190 100 .66
		
	End
End


End			