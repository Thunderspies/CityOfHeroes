#############################################################
## Sonic Cage - Attack
#############################################################

FxInfo

Lifespan 90

## CAMERA SHAKE ###########################################################

Condition
	On	Time
	Time	17

	Event
		EName 	CameraShake
		Type	Start
		At	Chest
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake		0.25	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.005	#how long shake lasts
			ShakeRadius	24	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	Origin
		Sound maestro2 80 80 .99
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	T_Chest
		#Bhvr	POWERS\SonicRessonance\SonicRing_Fade3.bhvr
		Sound sonic4 70 70 .77
		#Lifespan	5
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	HeadOffset
		Type	local
		At	Head
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicRings1.part
		LifeSpan	25
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicRepulsion_Waveform1.part
		POther	Target
		LifeSpan	15
	End
End

#############################################################

End