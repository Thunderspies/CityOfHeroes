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
		Part1	world/Lairs/cot/cot_waterfall_Long_steam_1.part
		Part2	world/Lairs/cot/cot_waterfall_Long_steamFlat.part
		Part3	world/Lairs/cot/cot_waterfall_Long_Mist_1.part
	End

End


End			