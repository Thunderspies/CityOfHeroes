#############################################################
## Sonic Cage - Attack
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

## SMALL BUBBLE #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup1
		Type	PositOnly
		At	Hips
		Bhvr	CustomizeablePowers\SonicResonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 0 0
			PositionOffset	0 3 0
		End
	End

	Event
		EName	Hookup2
		Type	PositOnly
		At	Hips
		Bhvr	CustomizeablePowers\SonicResonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 90 0
			PositionOffset	0 3 0
		End
	End
End

## SONIC WAVEFORM RING2 #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	CustomizeablePowers\SonicResonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.03125 0
			TintGeom	1
		End
		Geom	Tintable_Dark_SonicBuff_WaveformRing2
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	CustomizeablePowers\SonicResonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.03125 0
			TintGeom	1
		End
		Geom	Tintable_Dark_SonicBuff_WaveformRing2
	End
End

#############################################################

End