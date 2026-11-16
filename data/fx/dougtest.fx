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
		Part1	Dougtest1.part
		#Part2	Dougtest2.part
		#Part3	Dougtest3.part
	End
End

End			