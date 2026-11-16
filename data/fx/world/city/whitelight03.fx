#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	80
	Chance	1

	Event
		EName 	Prime
		Type	start 
		At	Origin
		Part1	world/City/SmallerWhiteLightPulsing.part
		Part2	world/City/LargeWhiteLightPulsing2.part
		LifeSpan	80
		
	End
End

End			