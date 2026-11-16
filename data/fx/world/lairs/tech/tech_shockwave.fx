#########################################################
##	tech_pillarsmoke
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
		Part1	world/Lairs/tech/tech_shockwave.part
		Sound zapper_loop 60 60 .4
		
		
	End
		
End

End			