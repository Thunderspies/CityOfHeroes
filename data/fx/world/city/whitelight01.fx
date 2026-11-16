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
		Type	local 
		At	Origin
		Part1	world/City/SmallerWhiteLightPulsingFast.part
		Part2	world/City/LargeWhiteLightPulsingFast.part
		LifeSpan	35
		
	End
End

End			