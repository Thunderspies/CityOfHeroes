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
		Part1	world/City/Steam01.part
		#Part2	world/City/Steam02_Pulse.part
		#Sound SteamBlast1 40.0 30.0 .29
		#Sound SteamBlast2 40.0 30.0 .29
		Sound SteamBlast3 40.0 40.0 .29
		Sound SteamBlast4 40.0 40.0 .29
		Sound SteamBlast5 40.0 40.0 .29
		Sound SteamBlast3 40.0 40.0 .29
		Sound SteamBlast5 40.0 40.0 .29
		Sound SteamBlast4 40.0 40.0 .29
		Lifespan 70 # From behavior
	End
End

End			