#########################################################
##	tech_sparkbolt.fx
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
		Part1	world/Lairs/tech/tech_sparkbolt_1.part
		Part2	world/Lairs/tech/tech_sparkbolt_2.part
		#Sound	zapper_loop 60 50 .33
		Sound energycore2_loop 40 40 .2
	End
End


End			