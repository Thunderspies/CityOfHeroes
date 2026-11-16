#########################################################
##	tech_warper
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound Glow6_loop 70 70 .6
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	world/Lairs/tech/tech_warper1.part
		##Part2	world/Lairs/tech/tech_warper2.part
		Sound Glow7_loop 80 80 .9
		
	End
End


End			