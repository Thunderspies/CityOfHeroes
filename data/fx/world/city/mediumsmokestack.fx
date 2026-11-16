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
		Part1	:mediumSmoke.part
		Part2	:mediumSmoke2.part
		Sound smoke2_loop 300 300 .8
	End
End

End			