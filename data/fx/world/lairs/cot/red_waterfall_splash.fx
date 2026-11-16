#########################################################
##	cot_waterfall_med.fx
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
		Part1	world/Lairs/cot/cot_waterfall_med_1.part
		Part2	world/Lairs/cot/cot_waterfall_med_2.part
		Sound waterfall_loop 150 150 .4
		
	End
End


End			