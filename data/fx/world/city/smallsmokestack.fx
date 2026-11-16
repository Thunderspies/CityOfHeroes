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
		Part1	:SmallSmoke.part
		Part2	:SmallSmoke2.part
		Sound smoke2_loop 200 200 .33
	End
End

End			