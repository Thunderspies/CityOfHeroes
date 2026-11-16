#############################################################
## FX System Name
#############################################################

FxInfo

## OFFSETS #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup1
		Type	PositOnly
		At	Hips
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 0 0
			PositionOffset	0 3 0
		End
	End

	Event
		EName	Hookup2
		Type	PositOnly 
		At	Hips
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 90 0
			PositionOffset	0 3 0
		End
	End
End

## SONIC WAVEFORM RING #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.0625 0
		End
		Geom	SonicBuff_WaveformRing1
		#Lifespan	60
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.0625 0
		End
		Geom	SonicBuff_WaveformRing1
		#Lifespan	60
	End
End

#############################################################

End