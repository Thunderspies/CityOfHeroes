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
		Part1	WORLD/Ruins/Mediumfire_flame_A2.part
		Part2	world/Ruins/Mediumfire_flame_B2.part
		Part3	WORLD/Ruins/Mediumfire_Smoke2.part
		Part4	:spark2.part
		Sound torch1_loop 60 60 .6
		
	End
End

End			