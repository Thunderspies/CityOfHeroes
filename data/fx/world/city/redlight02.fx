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
		Type	local 
		At	Origin		
	End
End

Condition
	On 	cycle
	Time 	30
	Chance	1

	Event
		EName 	light
		Type	local
		At	Prime
		Part1	world/City/LargeRedLightPulsing2.part
		Part2	world/City/LargeWhiteLightPulsing2.part
		Sound LightRelay 50 50 .05
		LifeSpan	30
		
	End
End

End			