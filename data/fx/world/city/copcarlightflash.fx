#########################################################
##	SStreetSteeam01
##
FxInfo

#		LifeSpan	35

Flags    InheritAlpha

Condition
	On	Time
	Time	3

	Event
		EName 	streaks
		Type	local 
		At	Origin

		BhvrOverride
			
			PositionOffset	-.6 0 0
		
		End

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:CopCarWhiteLightBurst.part
		Part	:CopCarWhiteLightStreaksBurst.part
		Part	:CarRedLightPulsingFast.part
		Part	:LargeRedLightBurst.part
		LifeSpan	10
		
	End

End

End			