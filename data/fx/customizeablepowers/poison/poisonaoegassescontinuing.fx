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

		Part	:AOETrailBlack.part
		Part	:AOETrail.part
		Part	:AOETrailLight.part

		Part	:AOETrailStreak.part
		Part2	:AOEYellowSmallBallzTrail.part
		Part3	:AOEYellowSmallBubble.part
		Part	:AOEYellowSmallstars.part
		Part5	:AOEBallzTrail.part
		Part5	:AOEBallzTrailBlack.part
		Sound Poisongas1_loop 50 50 .33
		#LifeSpan	100
	End

End

End