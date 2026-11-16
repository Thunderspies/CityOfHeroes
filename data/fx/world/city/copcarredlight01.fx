#########################################################
##	SStreetSteeam01
##
FxInfo

#		LifeSpan	35

Flags    InheritAlpha

Condition
	On	Cycle
	Time	10

	Event
		EName 	Prime
		Type	local 
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:LargeCopRedLightPulsingFast.part
		Part2	:LargeWhiteLightPulsingFast.part
		LifeSpan	35
		
	End
	
	Event
		EName 	streaks
		Type	local 
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:CopCarWhiteLightStreaks.part
		Part	:CopCarWhiteLightStreaks.part
		Part	:CopCarWhiteLightStreaksLarger.part
		LifeSpan	10
		
	End

End

End			