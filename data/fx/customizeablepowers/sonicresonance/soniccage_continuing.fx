#############################################################
## Sonic Cage - Continuing FX
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		bhvr	Behaviors\soundFade3.bhvr
		Sound soniccage_loop 70 70 .77
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

## OFFSETS #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup1
		Type	PositOnly
		At	Hips
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PyrRotate	0 0 0
		End
	End

	Event
		EName	Hookup2
		Type	PositOnly
		At	Hips
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PyrRotate	0 90 0
		End
	End
End

## SMALL BUBBLE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	CustomizeablePowers/SonicResonance/SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0 0
			TintGeom	1
		End
		Geom	Tintable_SonicBuff_Bubble1
	End
End

## SONIC NOISE RING CAGE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Scale		1 1 1
			Alpha		64
			Spin		0 0.125 0
			TintGeom	1
		End
		Geom	Tintable_SonicBuff_Ring1
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Scale		1 1 1
			Alpha		64
			Spin		0 0.125 0
			TintGeom	1
		End
		Geom	Tintable_SonicBuff_Ring1
	End
End

## SONIC WAVEFORM RING2 #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0.125 0
			TintGeom	1
		End
		Geom	Tintable_SonicBuff_WaveformRing2
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	:SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0.125 0
			TintGeom	1
		End
		Geom	Tintable_SonicBuff_WaveformRing2
	End
End

#############################################################

End