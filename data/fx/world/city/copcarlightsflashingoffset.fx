#########################################################
##	SStreetSteeam01
##
FxInfo


Flags    InheritAlpha

Condition	
	On	cycle
	Time	15

	Event
		EName 	LeftBlue
		Type	local 
		At	origin
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 4.875 -1.1375
		End

		ChildFx	:CopCarLightsFlashingChild.fx
		LifeSpan	22
		
	End

	Event
		EName 	LeftBlue
		Type	local 
		At	origin
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 4.875 -1.1375
		End

		ChildFx	:CopCarHeadLightsFlashingChild.fx
		LifeSpan	22
		
	End
End	

End	