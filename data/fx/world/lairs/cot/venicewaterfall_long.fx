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
		Part1	world/Lairs/cot/VeniceWaterfall_long_1.part
		Part2	world/Lairs/cot/VeniceWaterfall_long_2.part
		Sound venicewaterfall_loop 370 370 .65
		
	End
End


End			