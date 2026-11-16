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
		Part1	world/Lairs/Sewers/PipeWaterOuterMist_lge_OLD.part
		Part2	world/Lairs/Sewers/PipeWater_lge_OLD.part
		Part3	world/Lairs/Sewers/PipeWater_lge2_OLD.part
		Part4	world/Lairs/Sewers/PipeWaterOuterMist2_lge_OLD.part
		Sound Waterfall2_loop 150.0 150.0 .7
		
	End
End


End			