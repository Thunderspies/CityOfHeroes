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
		Part1	WORLD/Ruins/Mediumfire_flame_A.part
		Part2	world/Ruins/Mediumfire_flame_B.part
		
		Part3	WORLD/Ruins/Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .22
	End
End

End			