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
		Part1	world/City/Steam01_Pulse_Sideways.part
		Part2	world/City/Steam02_Pulse_Sideways.part
		sound	steam3_loop 50 40 .3
		Lifespan 70 # From behavior
	End
End

End			