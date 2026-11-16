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
		Part1	world/Lairs/cot/cot_waterfall_sml_1.part
		Part2	world/Lairs/cot/cot_waterfall_sml_2.part
		Sound waterfallsmall_loop 70 70 .4
	End
End


End			