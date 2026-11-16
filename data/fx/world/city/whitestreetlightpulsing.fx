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
		Type	start 
		At	Origin
		Part1	world/City/whiteLightPulsing2.part
		Part2	world/City/WhiteLightPulsing.part
	
	End
End

End			