#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	Chest
End


Condition
	On 	cycle
	Time 	17
	Chance	.1

	Event
		EName 	flash
		Type	Local
		At	Chest
		Part1	:FallingLeavesForestWall45_01.part

		LifeSpan	15
	End
End



End			