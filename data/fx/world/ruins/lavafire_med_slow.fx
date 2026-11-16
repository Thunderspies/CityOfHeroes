#########################################################
##	SStreetSteeam01
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
		Part1	WORLD/Ruins/LavaMediumfire_flame_A2.part
		Part2	world/Ruins/Lavafire_flame_B2.part
		#Part3	WORLD/Ruins/Mediumfire_Steam.part
		Part4	:spark2.part
		Sound lavasteam4_loop 80 80 .6
		
	End
End

End			