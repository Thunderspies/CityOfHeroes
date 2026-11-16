#########################################################
##	tech_lighning
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
		##Part1	world/Lairs/tech/tech_lighting.part
		Part2	world/Lairs/tech/tech_lighting2.part
	End
End


End			