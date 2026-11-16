#############################################################
## Sonic Cage - Continuing FX
#############################################################

FxInfo

Lifespan 90

#############################################################

#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		Type	Local
#		At	T_Root
#		Sound	sonic3 70 60 .5
#	End
#End

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
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		part	:DisruptionField_Ring1.part
		Lifespan	60
	End
End

## LARGE BUBBLE #######################################################

#Condition
#	On 	Time
#	Time	1
#	Chance	0.25
#
#	Event
#		Type	Local 
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicBubble_Fade3.bhvr
#		BhvrOverride
#			PyrRotateJitter	180 180 180
#			Alpha		255
#			Spin		0 0.0125 0
#		End
#		Geom	SonicBuff_WaveformBubble1
#		Lifespan	45
#	End
#End

#############################################################

End