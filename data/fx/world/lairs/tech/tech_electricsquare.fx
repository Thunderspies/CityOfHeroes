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
		Part1	world/Lairs/tech/tech_electricsquare.part
		##Part2	world/Lairs/tech/tech_lighttentacles2.part
		Sound lighttentaclesroom_loop 140 140 .7
	End
End


End			