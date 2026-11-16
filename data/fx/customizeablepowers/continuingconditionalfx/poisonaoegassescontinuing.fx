#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Condition

	Event
		Type	local
		At	Root

		Part	:BlastAOETrailBlack.part
		Part	:BlastAOETrail.part
		Part	:BlastAOETrailLight.part

		LifeSpan	30
	End

	Event
		Type	local
		At	Root

		Part	CustomizeablePowers/Poison/AOETrailBlack.part
		Part	CustomizeablePowers/Poison/AOETrail.part
		Part	CustomizeablePowers/Poison/AOETrailLight.part

		Part	CustomizeablePowers/Poison/AOETrailStreak.part
		Part2	CustomizeablePowers/Poison/AOEYellowSmallBallzTrail.part
		Part3	CustomizeablePowers/Poison/AOEYellowSmallBubble.part
		Part	CustomizeablePowers/Poison/AOEYellowSmallstars.part
		Part5	CustomizeablePowers/Poison/AOEBallzTrail.part
		Part5	CustomizeablePowers/Poison/AOEBallzTrailBlack.part
		Sound Poisongas1_loop 50 50 .33
		#LifeSpan	100
	End

End

End