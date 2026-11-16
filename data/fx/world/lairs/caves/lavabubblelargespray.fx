#########################################################
##	SStreetSteeam01
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
		#Part1	:LavaBubblePopping.part
		Part2	:LavaSprayLarge.part
		#Part3	:LavaBubbleFlat.part
		
	End
End

End			