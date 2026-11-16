#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fountain01
		Type	start 
		At	root
		
		part1	:WaterStreamBody.part
						
	End

	
	

End
End			