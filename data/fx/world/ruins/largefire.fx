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
		Part1	WORLD/Ruins/Largefire_flame_A2.part
		Part2	world/Ruins/Largefire_flame_B2.part
		Part3	WORLD/Ruins/Largefire_Smoke2.part
		Part4	WORLD/Ruins/spark3.part
		Part5	WORLD/Ruins/Largefire_Smoke1.part
		Sound lgfire_loop 90 90 .62
		
		
	End

	#Event
	#	Type	Local 
	#	At	Origin
		#Part1	WORLD/Ruins/Largefire_flame_Flat.part
				
		
	#End

End

End			