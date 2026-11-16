#############################################################
## Sonic Cage - Continuing FX
#############################################################

FxInfo

Lifespan 90

## OFFSETS #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup1
		Type	Create
		Inherit	Position
		Update	Position
		At	T_Hips
		BhvrOverride
			PositionOffset	0 3 0
		End
		Lifespan	60
	End
End

## RING PULSE #######################################################

Condition
	On 	Time
	Time	1
	#Chance	0.5

	Event
		Type	Local
		At	Hookup1
		Bhvr	CustomizeablePowers\SonicResonance\GenericParticleFadeQuick.bhvr
		part	:SonicRepulsion_Ring1.part
		Lifespan	60
	End
End

#############################################################

End