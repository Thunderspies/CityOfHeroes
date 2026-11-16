#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	15
	
	Event
		EName 	Prime
		Type	start 
		At	Origin		
	End
End

Condition
	On 	cycle
	Time 	30
	Chance	1

	Event
		EName 	light
		Type	start 
		At	Prime
		Part1	world/City/SmallerWhiteLightPulsing.part
		Part2	world/City/LargeWhiteLightPulsing2.part
		LifeSpan	30
		
	End
End

End			