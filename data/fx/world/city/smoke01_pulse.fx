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
	Time 	100
	Chance	.65

	Event
		Type	Local 
		At	Origin
		Part1	world/City/Smoke01_Pulse.part
		Lifespan 70 # From behavior
	End
End

End			