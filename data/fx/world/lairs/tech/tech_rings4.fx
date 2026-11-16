#########################################################
##	tech_rings1
##
FxInfo
	


Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	glowroom_loop 100 70 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	beeprise2m_loop 100 70 .36
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	world/Lairs/tech/tech_rings4.part
		#Sound	energycore4_loop 50 40 .12
		
	End
End

End			