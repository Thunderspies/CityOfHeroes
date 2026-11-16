#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
		SOund	poisonprojectile 80 30 .88
	End

	Event
		EName	Prime
		Type	start
		At	Spike
		Altpiv	1

		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:SnakeQuillTrail.part
		Part2	:SnakeQuillTrailSpecks.part
		Part3	:SnakeQuillBubblesTrail.part
		Part5	:SnakeQuillBallzTrail.part
		Part	:SnakeQuillBallsTrail.part
	End

	Event
		EName	Prime2
		Type	start
		At	Spike
		Altpiv	1
	
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:SnakeQuillTrail2.part
		Part2	:SnakePoisonHead.part
		Part2	:SnakePoisonHeadCore.part
		Part2	:SnakePoisonHeadCore2.part
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

End				