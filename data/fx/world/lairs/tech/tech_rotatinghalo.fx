#########################################################
##	tech_rotatinghalo
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
		Part1	world/Lairs/tech/tech_rotatinghalo1.part
		##Part2	world/Lairs/tech/tech_rotatinghalo2.part
		Sound Glowroom_loop 70 70 .43
	End
End


End			