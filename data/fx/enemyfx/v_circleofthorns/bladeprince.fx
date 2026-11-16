FxInfo

Condition
	On 	Time
	Time 	0
	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:BehemothFlame.part
		Part2	:Behemothspark.part
		#PMagnet	UarmR
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:BehemothFlame.part
		Part2	:Behemothspark.part
		#PMagnet	UarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:BehemothFlameBack.part
		Part2	:Behemothspark.part
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Neck
		
		Part1	:BehemothFlameNeck.part
		Part2	:Behemothspark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:BehemothFlame.part
		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		PMagnet	LarmL
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmR
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		PMagnet	UarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	LarmL
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		PMagnet	UarmL
	End


	Event
		EName 	fire2
		Type	Local
		At	WepR
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		PMagnet	LarmR
	End

	Event
		EName 	fire2
		Type	Local
		At	WepL
		
		Part1	:BehemothFlame.part
		Part2	:BehemothsparkArm.part
		PMagnet	LarmL
	End

	Event
		EName 	fire1
		Type	Local
		At	FootL
		
		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footIceMist.part
		Part2	:footIceMist_02.part
		Part3	:BehemothsparkArm.part
		
	End
	
	Event
		EName 	fire1
		Type	Local
		At	FootR

		BHVR	Behaviors/FootOffset.bhvr
		Part1	:footIceMist.part
		Part2	:footIceMist_02.part
		Part3	:BehemothsparkArm.part
		
	End


########################################################
#Eyes
########################################################	
	
	Event
		EName 	hookup
		Type	Local
		At	Head
		Geom	Minion_Eyey

		
	End

	Event
		EName 	headhook1
		Type	Local
		At	head
		##altpiv	1
		part1	:DaemonEyes1x.part
		part2	:DaemonPupilx.part
		Part3	:BehemothEyesMistLeftx.part
		bhvr	:prince_eyes_left.bhvr
	End

	Event
		EName 	headhook2
		Type	Local
		At	head
		##altpiv	2
		part1	:DaemonEyes1x.part
		part2	:DaemonPupilx.part
		Part3	:BehemothEyesMistRightx.part
		bhvr	:prince_eyes_right.bhvr
	End

	

End
End

