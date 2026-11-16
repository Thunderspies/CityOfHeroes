#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	15
	Chance	1

	Event
		EName 	Prime
		Type	start 
		At	Origin
		Part1	world/City/LargeBlueLightPulsingFast.part
		Part2	world/City/LargeWhiteLightPulsingFast.part
		LifeSpan	35
		
	End
End

End			