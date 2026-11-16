#############################################################
## SonicSiphon_Hit.fx
#############################################################

FxInfo
Lifespan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################

### Sound ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound ChargeLight3 80 80 .7
	End
End

## CAMERA SHAKE ###########################################################

Condition
	On	Time
	Time	5

	Event
		EName 	CameraShake
		Type	Start
		At	Chest
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake		0.375	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.0125	#how long shake lasts
			ShakeRadius	24	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

### BODY AURA 3 - Random Waveforms ###########################################################
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Chest
#		part1	:Hit_SonicWaveform1.part
#	End
#End

## BODY AURA ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Large1.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	UArmL
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Small1.part
		POther	LArmL
		Lifespan	15
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Small1.part
		POther	LArmR
		Lifespan	15
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Small1.part
		POther	HandL
		Lifespan	15
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Small1.part
		POther	HandR
		Lifespan	15
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Medium1.part
		POther	LLegL
		Lifespan	15
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Medium1.part
		POther	LLegR
		Lifespan	15
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Medium1.part
		POther	FootL
		Lifespan	15
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Hit_SonicRipples_Medium1.part
		POther	FootR
		Lifespan	15
	End
End

#############################################################

End