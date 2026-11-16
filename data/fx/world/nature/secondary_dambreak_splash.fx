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
		Part1	:Secondary_Waterfall_Splash_1.part
		Part2	:Secondary_Waterfall_Splash_2.part
		Part2	:Secondary_Waterfall_Splash_3.part

		Part2	:Secondary_Waterfall_Splash_ripples.part
		Part	:Secondary_Waterfall_Splash_smallripples.part
		
	End
End


End			