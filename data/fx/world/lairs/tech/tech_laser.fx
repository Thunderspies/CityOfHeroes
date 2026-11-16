#########################################################
##	tech_laser
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
		Part1	world/Lairs/tech/tech_laser.part
		###Part2	world/Lairs/tech/tech_warper2.part
		#Sound	taser2 70 50 .4
		
	End
End


End			