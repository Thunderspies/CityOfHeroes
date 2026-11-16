#############################################################
## Apparition_Monarch_Reabsorb.fx
#############################################################

FxInfo
Lifespan 180

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Body_SmokeAura.part
		POther	UArmL
	End

	Event
		EName	FlashAnchor
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Body_SmokeAura.part
		Lifespan 140
	End

	Event
		Type	Local
		At	FlashAnchor
		BhvrOverride
			Scale		0.1 0.1 0.1
			PositionOffset	0.0 1.0 0.0
		End
		ChildFX	:Boss_RippedSpecter_Flash.fx
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	8
	Chance 	.8

	Event
		Type	Local
		At	FlashAnchor
		BhvrOverride
			Scale		0.1 0.1 0.1
			PositionOffset	0.0 1.0 0.0
		End
		ChildFX	:Boss_RippedSpecter_Flash.fx
		Lifespan 5
	End
End


Condition
	On	Time
	Time	150

	Event
		Type	PositOnly
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		CHILDFX	:CHILD_Reabsorb.fx
		Lifespan 30
	End
End

#########################################################

End