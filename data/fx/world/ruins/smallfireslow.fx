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
		Part1	WORLD/Ruins/Smallfire_flame_A2.part
		Part2	world/Ruins/Smallfire_flame_B2.part
		Part3	WORLD/Ruins/Smallfire_flame_Flat2.part
		Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/Smallfire_Smoke2.part
		Sound Torch1_loop 33 33 .38
		
	End
End

End			