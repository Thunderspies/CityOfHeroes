#########################################################
##	tech_lighttentacles
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
		Part1	world/Lairs/tech/tech_lighttentacles_long_1.part
		#Part2	world/Lairs/tech/tech_lighttentacles_long_2.part
		Sound lighttentaclesroom_loop 70 70 .25
	End
End


End			