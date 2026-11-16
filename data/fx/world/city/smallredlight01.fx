#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	7
	Chance	1

	Event
		EName 	Prime
		Type	local 
		At	Origin
		
		BhvrOverride
			PositionOffset	.5 5.65 1.7
		End

		Part1	world/City/RedLightPulsingFast.part
		Part2	world/City/WhiteLightPulsingFast.part
		Part	:WhiteLightRingPulsingFast.part
		Part	:RedLightRays.part
		Part	:WhiteLightRays.part
		LifeSpan	35
		
	End
End

End			