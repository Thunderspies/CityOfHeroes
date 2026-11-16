#############################################################
## SonicBarrier.fx
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
		Sound SonicBarrier 80 80 .99
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
		Geom	PsionicRigging
	End
End

Condition
	On 	Time
	Time 	27

	Event
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicRings1.part
		Lifespan 11
	End
End

Condition
	On 	Time
	Time 	27

	Event
		Type	Start
		At	HeadOffset
		AltPiv	3
		Part	:SonicBarrier_Waveform1.part
		POther	Target
		Lifespan 15
	End
End

## SONIC BEAM ###########################################################

Condition
	On	Time
	Time	27

	Event
		EName	Prime
		Type	Start
		At	HeadOffset
		AltPiv	3
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