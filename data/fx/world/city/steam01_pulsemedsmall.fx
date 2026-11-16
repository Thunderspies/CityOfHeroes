#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	30
	Chance	.30

	Event
		Type	Local 
		At	Origin
		Part1	world/City/Steam01MedSmall.part
		Lifespan 70 # From behavior
	End
End

End			