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
		bhvr	behaviors/scale1a.bhvr	
		SOund	poisongasspew 90 30 .88
	End

	Event
		EName	Prime
		Type	local
		At	Spike
		Altpiv	1
		
		Part	:coneQuillTrailBlack.part
		Part	:coneQuillTrailBlack2.part
		Part	:coneQuillTrail.part
		Part	:coneQuillTrailLight.part

		Part	:coneQuillTrailStreak.part
		Part2	:coneYellowSmallBallzTrail.part
		
		Part3	:coneQuillBubbleTrail.part
		Part3	:coneQuillStarTrail.part
		
		Part5	:coneQuillBallzTrail.part
		Part5	:coneQuillBallzTrailBlack.part

	End


End

End				