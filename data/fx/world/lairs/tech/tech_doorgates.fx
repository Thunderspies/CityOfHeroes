#########################################################
##	tech_doorgates
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
		Part1	world/Lairs/tech/tech_doorgates_1.part
		Part2	world/Lairs/tech/tech_doorgates_2.part
	End
End


End			