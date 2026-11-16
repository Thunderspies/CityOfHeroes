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
		Part1	WORLD/Ruins/LavaLargefire_flame_A2.part
		Part2	world/Ruins/LavaLargefire_flame_B2.part
		#Part3	WORLD/Ruins/Lavafire_Smoke2.part
		Part4	WORLD/Ruins/Lavaspark3.part
		#Part5	WORLD/Ruins/Lavafire_Smoke1.part
		Sound lavasteam5_loop 50 50 .47
		
		
	End

	
End

End			