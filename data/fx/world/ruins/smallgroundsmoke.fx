#########################################################
##	BuildingRuinSmolder
##
FxInfo
	
LifeSpan 30000

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
		Part1	:SmallGroundSmoke.part 
		#Part2	:SmallGroundSmoke2.part

	End
End

End			