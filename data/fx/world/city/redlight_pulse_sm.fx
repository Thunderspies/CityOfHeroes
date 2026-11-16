#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	50
	Chance	1

	Event
		EName 	light
		Type	start 
		At	Origin
		Part1	world/City/LargeRedLightPulsing4.part
		Part2	world/City/LargeWhiteLightPulsing2.part
		LifeSpan	30
		
	End
End

End			