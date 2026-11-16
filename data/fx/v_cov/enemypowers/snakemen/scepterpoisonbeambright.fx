#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	Spike
		Type	Local
		At	wepR
		
		BhvrOverride
			PositionOffset	0 0 2.3
		End
		Sound beam4 70 70 .66
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
#		SOund	poisonprojectile 80 30 .88
	End

	Event
		EName	Prime
		Type	start
		At	Spike
		Altpiv	1


		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:BrightSnakeQuillTrail.part
		Part2	:BrightSnakeQuillTrailSpecks.part
		Part3	:BrightSnakeQuillBubblesTrail.part
		Part5	:BrightSnakeQuillBallzTrail.part
		Part	:BrightSnakeQuillBallsTrail.part
	End

	Event
		EName	Prime2
		Type	start
		At	Spike
		Altpiv	1
	
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:BrightSnakeQuillTrail2.part
		Part1	:BrightSnakeQuillTrail2.part
		Part1	:BrightSnakeQuillTrail2.part
		Part1	:BrightSnakeQuillTrail2.part

		Part2	:BrightSnakePoisonHead.part
		Part2	:BrightSnakePoisonHeadCore.part
		Part2	:BrightSnakePoisonHeadCore2.part
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		Childfx	:SnakeStaffBlastHit.fx
		lifeSpan	20
	End

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