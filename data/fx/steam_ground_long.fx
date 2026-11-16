#########################################################
##	Steam_ground_long
##
FxInfo
	
Name 	 Steam_ground_long
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
		Part1	world/Steam_Ground_Long.part
	End
End

End			