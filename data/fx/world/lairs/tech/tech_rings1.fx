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
		Sound glowroom_loop 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound beeprise2m_loop 100 100 .36
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		##Part1	world/Lairs/tech/tech_rings4.part
		##Part2	world/Lairs/tech/tech_rings2.part
		Part3	world/Lairs/tech/tech_rings3.part
		##Part4	world/Lairs/tech/tech_rings4.part
	End
End

End			