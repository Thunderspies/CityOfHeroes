#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Spike
		Type	Local
		At	head

		Geom	LocalPivotz
		bhvr	CustomizeablePowers\Poison\scale1a.bhvr
		Sound poisongasspew 90 90 .88
	End

	Event
		EName	Prime
		Type	local
		At	Spike
		Altpiv	1

		Part	CustomizeablePowers\Poison\coneQuillTrailBlack.part
		Part	CustomizeablePowers\Poison\coneQuillTrailBlack2.part
		Part	CustomizeablePowers\Poison\coneQuillTrail.part
		Part	CustomizeablePowers\Poison\coneQuillTrailLight.part

		Part	CustomizeablePowers\Poison\coneQuillTrailStreak.part
		Part2	CustomizeablePowers\Poison\coneYellowSmallBallzTrail.part

		Part3	CustomizeablePowers\Poison\coneQuillBubbleTrail.part
		Part3	CustomizeablePowers\Poison\coneQuillStarTrail.part

		Part5	CustomizeablePowers\Poison\coneQuillBallzTrail.part
		Part5	CustomizeablePowers\Poison\coneQuillBallzTrailBlack.part

	End


End

End