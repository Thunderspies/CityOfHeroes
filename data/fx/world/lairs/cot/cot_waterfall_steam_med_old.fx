#########################################################
##	cot_waterfall.fx
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
		Part1	world/Lairs/cot/cot_waterfallsteam_med_1_OLD.part
		Part2	world/Lairs/cot/cot_waterfallsteam_med_Flat_OLD.part
		Part3	world/Lairs/cot/cot_waterfallMist_med_1_OLD.part
		
	End
End


End			