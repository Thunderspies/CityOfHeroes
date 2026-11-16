#########################################################
##	BuildingRuinSmolder
##
FxInfo
	
LifeSpan 30

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
		Part1	:MediumGroundSmoke.part 


	End

End

End			