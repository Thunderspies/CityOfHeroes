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
			PositionOffset	2.1 0 0
		
		End

		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarRedLight01.fx
		LifeSpan	20
		
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	local 
		At	origin

		BhvrOverride
			PositionOffset	2.1 0 0
		
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarLightFlash.fx
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
			PositionOffset	-1.8 0 0
		
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarBlueLight01.fx
		LifeSpan	20
		
	End
	
End

Condition
	On 	Time
	Time 	7


	Event
		Type	local 
		At	Origin
		
		BhvrOverride
			PositionOffset	-1.8 0 0
		
		End

		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFx	:CopCarLightFlashBlue.fx
		LifeSpan	10
		
	End
	
End

End			