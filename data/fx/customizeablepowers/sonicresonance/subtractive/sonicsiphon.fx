#############################################################
## FX System Name
#############################################################

FxInfo
Lifespan 90

## CAMERA SHAKE ###########################################################

Condition
	On	Time
	Time	24

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
	Time	19

	Event
		Type	Local
		At	Origin
		Sound SonicWave4 90 90 .99
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
	Time 	25

	Event
		EName	SonicRings
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicRings1.part
		LifeSpan 25
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	SonicWaveform
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicWaveform1.part
		POther	Target
		LifeSpan 25
	End
End

## SONIC BEAM ###########################################################

Condition
	On	Time
	Time	35

	Event
		EName	Prime
		Type	Start
		At	HeadOffset
		AltPiv	3
		BhvrOverride
			TrackRate		3.0
			TrackMethod		2
		End
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	Prime2
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate	90 0 0
		End
		Part	:Projectile_Ripples1.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	Prime2
		Type	Destroy
	End
End

#############################################################

End