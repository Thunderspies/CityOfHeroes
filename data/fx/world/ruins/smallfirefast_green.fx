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
		Part1	WORLD/Ruins/Smallfire_flame_A3.part
		Part2	world/Ruins/Smallfire_flame_B3.part
		Part3	WORLD/Ruins/Smallfire_flame_Flat3.part
		Part4	WORLD/Fire/spark1.part
		#Part5	WORLD/Ruins/Smallfire_Smoke.part
		Sound Torch2_loop 30 30 .24
	End
End

End			