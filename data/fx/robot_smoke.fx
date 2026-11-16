#########################################################
##	
##
FxInfo
	
Name 	 robot_smoke
LifeSpan 30

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
		##Part1	smoke_trail.part
	End

End

End			