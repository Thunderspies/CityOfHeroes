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
		Part1	world/City/BlueLightPulsing.part
		Part2	world/City/WhiteLightPulsing.part
		Sound BlueLightBeep_loop 22 22 .65
	
	End
End

End			