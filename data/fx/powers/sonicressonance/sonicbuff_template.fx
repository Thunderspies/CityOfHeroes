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

## SMALL BUBBLE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local 
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0 0
		End
		Geom	SonicBuff_Bubble1
	End
End

## LARGE BUBBLE #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local 
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PositionOffset	0 3 0
			Alpha		255
			Scale		30 30 30
		End
		Geom	SonicBuff_Bubble2
#		part1	POWERS\SonicRessonance\DispersionBubble_Motes1.part
#		part1	POWERS\SonicRessonance\DispersionBubble_Motes2.part
	End
End

#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local 
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			PositionOffset	0 3 0
#			Alpha		255
#			Scale		30 30 30
#			Spin		0 -0.0125 0
#		End
#		Geom	SonicBuff_WaveformBubble1
#	End
#End
#
#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local 
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			PositionOffset	0 3 0
#			Alpha		255
#			Scale		30 30 30
#			Spin		0 0.0125 0
#		End
#		Geom	SonicBuff_WaveformBubble2
#	End
#End

#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local 
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			PositionOffset	0 3 0
#			Alpha		255
#			Scale		30 30 30
#			Spin		0 -0.0125 0
#		End
#		Geom	SonicBuff_WaveformBubble1
#	End
#End
#
#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local 
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			PositionOffset	0 3 0
#			Alpha		255
#			Scale		30 30 30
#			Spin		0 0.0125 0
#		End
#		Geom	SonicBuff_WaveformBubble2
#	End
#End

## SONIC NOISE RING CAGE #######################################################

#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			Scale		1 1 1
#			Alpha		128
#			Spin		0 0.0625 0
#		End
#		Geom	SonicBuff_Ring1
#	End
#
#	Event
#		Type	Local
#		At	Hookup2
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			Scale		1 1 1
#			Alpha		128
#			Spin		0 0.0625 0
#		End
#		Geom	SonicBuff_Ring1
#	End
#End

## SONIC WAVEFORM RING #######################################################

#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			Alpha		255
#			Spin		0 0.0625 0
#		End
#		Geom	SonicBuff_WaveformRing1
#		#Lifespan	60
#	End
#
#	Event
#		Type	Local
#		At	Hookup2
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
#		BhvrOverride
#			Alpha		255
#			Spin		0 0.0625 0
#		End
#		Geom	SonicBuff_WaveformRing1
#		#Lifespan	60
#	End
#End

## SONIC WAVEFORM RING2 #######################################################

Condition
	On 	Time
	Time	1

	Event
		Type	Local
		At	Hookup1
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.03125 0
		End
		Geom	SonicBuff_WaveformRing2
		#Lifespan	60
	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade1.bhvr
		BhvrOverride
			Alpha		255
			Spin		0 0.03125 0
		End
		Geom	SonicBuff_WaveformRing2
		#Lifespan	60
	End
End

### BODY AURA 1 - Orange Alpha Glow ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	Neck
	End

	Event
		Type	Local
		At	Chest
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Large1.part
		POther	Waist
	End

	Event
		Type	Local
		At	UArmL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	HandR
	End

	Event
		Type	Local
		At	HandL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
	End

	Event
		Type	Local
		At	HandR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
	End

	Event
		Type	Local
		At	ULegL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Medium1.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Medium1.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Medium1.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Medium1.part
		POther	FootR
	End

	Event
		Type	Local
		At	ToeL
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	FootL
	End

	Event
		Type	Local
		At	ToeR
		part1	POWERS\SonicRessonance\Aura_SoftGlow_Small1.part
		POther	FootR
	End
End

### BODY AURA 2 - Elliptical Rings ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Large1.part
	End

	Event
		Type	Local
		At	UArmL
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Small1.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Small1.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Small1.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Small1.part
		POther	HandR
	End

	Event
		Type	Local
		At	ULegL
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Medium1.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Medium1.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Medium1.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part1	POWERS\SonicRessonance\Aura_SonicRipples_Medium1.part
		POther	FootR
	End
End

### BODY AURA 3 - Random Waveforms ###########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Chest
#		part1	POWERS\SonicRessonance\Aura_SonicWaveform1.part
#		pOther	Chest
#	End
#End

#############################################################

End