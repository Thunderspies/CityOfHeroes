#########################################################
##	SStreetSteeam01
##
FxInfo

Flags    InheritAlpha

Condition
	On 	Time
	Time 	3

	Event
		Type	local 
		At	origin

		BhvrOverride
			PositionOffset	1.8 -2.5 8.8
		
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarLightFlashWhite.fx
		LifeSpan	10
		
	End
End

Condition
	On 	Time
	Time 	7


	Event
		Type	local 
		At	Origin
		
		BhvrOverride
			PositionOffset	-2.8 -2.5 8.8
		
		End

		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarLightFlashWhite.fx
		LifeSpan	10
		
	End
	
End

End			