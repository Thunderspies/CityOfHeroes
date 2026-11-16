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
		Part1	:Oblisk_flame_A2.part
		Part2	:Oblisk_flame_B2.part
		Part3	:Oblisk_Smoke2.part
		Part4	:ObliskSpark2.part
		
		Sound torch1_loop 60 60 .6
		
	End
End

End			