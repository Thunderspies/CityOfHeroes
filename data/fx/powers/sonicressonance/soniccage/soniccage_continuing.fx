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
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		Sound soniccage_loop 70 70 .77
		bhvr	behaviors/soundFade3.bhvr
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
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PyrRotate	0 0 0
		End
	End

	Event
		EName	Hookup2
		Type	PositOnly
		At	Hips
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PyrRotate	0 90 0
		End
	End
End

### SPLAT #######################################################
#
#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	:SonicCage_Splat.bhvr
#		Splat	Sonic_Ring.tga
#	End
#End

## SMALL BUBBLE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0 0
		End
		Geom	SonicBuff_Bubble1
	End
End

## SONIC NOISE RING CAGE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Scale		1 1 1
			Alpha		64
			Spin		0 0.125 0
		End
		Geom	SonicBuff_Ring1
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Scale		1 1 1
			Alpha		64
			Spin		0 0.125 0
		End
		Geom	SonicBuff_Ring1
	End
End

## SONIC WAVEFORM RING2 #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0.125 0
		End
		Geom	SonicBuff_WaveformRing2
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 -0.25 0
			Alpha		255
			Spin		0 0.125 0
		End
		Geom	SonicBuff_WaveformRing2
	End
End

#############################################################

End