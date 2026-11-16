#############################################################
## Clarity.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Origin
		Sound HeroSonicBlast3 80 80 .99
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
		BhvrOverride
			PositionOffset	0 -0.5 4.0
			PYRRotate	0 0 0
		End
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Start
		At	HeadOffset
		AltPiv	1
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
		AltPiv	1
		Part	:Clarity_Waveform1.part
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
		AltPiv	1
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
End

#############################################################

End