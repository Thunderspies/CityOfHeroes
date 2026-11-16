#########################################################
##	BuildingRuinSmolder
##
FxInfo
	
#LifeSpan 30000

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
		Part1	BuildingRuinSmolder.part 
		Part2	BuildingRuinSmolder2.part

	End
End

End			