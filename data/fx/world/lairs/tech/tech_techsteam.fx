#########################################################
##	tech_pillar
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
		Part1	world/Lairs/tech/tech_pillarsmoke1.part
		Sound steam4_loop 60 60 .28
		
	End
		
End

End			