#############################################################
## DrainPsyche_Hit.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type	start
		At	Root
		BHVR	CustomizeablePowers\MentalManipulation\ProjectileSlow.bhvr
		BhvrOverride
			TrackRate		0.75
		End
		Part	:EnergyRingTrail.part
		Part	:EnergyRingTrailGlow.part
		Part	:EnergyRingTrailGlowFlat.part
		Magnet	T_Root
	End

	Event
		EName	Prime2
		Type	start
		At	Head
		BHVR	CustomizeablePowers\MentalManipulation\ProjectileSlow.bhvr
		BhvrOverride
			TrackRate		0.75
		End
		Part1	:BlastStreakLight.part
		Part2	:BlastStreakGlowHead.part
		Part2	:ScrambleThoughtsTenticles.part
		Magnet	T_Head
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

#############################################################

End