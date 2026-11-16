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
	Time	20

	Event
		Type	Local
		At	Origin
		Sound soniccage1 80 80 .99
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Origin
		Sound sonicbeam3 80 80 .99
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	HeadOffset
		Type	local
		At	Mystic
		BhvrOverride
			PositionOffset	-1.25 -1 2.5
			PYRRotate	250 -30 0
		End
	#	Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Start
		At	HeadOffset
	#	AltPiv	3
		Part	:SonicRings1.part
		LifeSpan 25
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Start
		At	HeadOffset
	#	AltPiv	3
		Part	:SonicCage_Waveform1.part
		POther	Target
		LifeSpan 15
	End
End

## SONIC BEAM ###########################################################

Condition
	On	Time
	Time	17

	Event
		EName	Prime
		Type	Start
		At	HeadOffset
	#	AltPiv	3
		BhvrOverride
			TrackRate		2.0
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

#	Event
#		EName	Prime2
#		Type	Destroy
#	End
End

#############################################################

End