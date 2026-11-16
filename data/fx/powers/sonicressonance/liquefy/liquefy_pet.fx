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
		Type	Start
		At	Root
		Sound soniccage_loop 70 70 .77
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

## LARGE BUBBLE #######################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr	:LiquefyBubble_Initial.bhvr
		Geom	SonicBuff_Bubble2
		Lifespan	45
	End
End

Condition
	On 	Cycle
	Time	45

	Event
		Type	Start
		At	Root
		Bhvr	:LiquefyBubble.bhvr
		Geom	SonicBuff_Bubble2
		Lifespan	70
	End
End

## SMALL BUBBLE #######################################################

Condition
	On 	Time
	Time	30

	Event
		Type	Start
		At	Root
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		BhvrOverride
			PositionOffset	0 1.5 0
			Scale		20 20 20
			Alpha		255
			#Spin		0 0.01 0
		End
		Geom	SonicBuff_WaveformRay1
	End

	Event
		Type	Start
		At	Root
		part1	:Liquefy_Ring1.part
	End
End

## SONIC WAVEFORM RING2 #######################################################

#Condition
#	On 	Time
#	Time	1
#
#	Event
#		Type	Local
#		At	Hookup1
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
#		BhvrOverride
#			PositionOffset	0 -0.25 0
#			Alpha		255
#			Spin		0 0.125 0
#		End
#		Geom	SonicBuff_WaveformRing2
#	End
#
#	Event
#		Type	Local
#		At	Hookup2
#		Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
#		BhvrOverride
#			PositionOffset	0 -0.25 0
#			Alpha		255
#			Spin		0 0.125 0
#		End
#		Geom	SonicBuff_WaveformRing2
#	End
#End

#############################################################

End