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
		Part1	world/City/Steam01_Pulse_MedSml_Sideways.part
		Part2	world/City/Steam02_Pulse_MedSml_Sideways.part
		Sound SteamBlast1 60.0 60.0 .8
		Sound SteamBlast2 60.0 60.0 .66
		Lifespan 70 # From behavior
	End
End

End			