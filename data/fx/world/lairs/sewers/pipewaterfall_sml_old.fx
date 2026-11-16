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
		Part1	world/Lairs/Sewers/PipeWaterOuterMist_sml_OLD.part
		Part2	world/Lairs/Sewers/PipeWater_sml_OLD.part
		Part3	world/Lairs/Sewers/PipeWater_sml2_OLD.part
		Part4	world/Lairs/Sewers/PipeWaterOuterMist2_sml_OLD.part
		Sound Waterfall2_loop 80.0 80.0 .6
	End
End


End			