FxInfo

Condition
	On 	Time
	Time 	0
	Event
		EName 	fire1
		Type	Local
		At	Col_R
		
		Part1	:MinionFlame.part
		Part2	:MinionSpark.part
		Sound torch1_loop 70 70 .25
		
	End
	
	Event
		EName 	fire2
		Type	Local
		At	Col_L
		
		Part1	:MinionFlame.part
		Part2	:MinionSpark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	Back
		
		Part1	:MinionFlameBack.part
		Part2	:MinionSpark.part
		
	End
	

	Event
		EName 	fire2
		Type	Local
		At	UarmR
		
		Part1	:MinionFlameArm.part
		Part2	:MinionSpark.part
		
	End

	Event
		EName 	fire2
		Type	Local
		At	UarmL
		
		Part1	:MinionFlameArm.part
		Part2	:MinionSpark.part
		
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
		part1	:CalebEyesGlow.part
		part2	:CalebPupil.part
		Part3	:CalebEyesMistLeft.part
		bhvr	:prince_eyes_left.bhvr
	End

	Event
		EName 	headhook2
		Type	Local
		At	head
		##altpiv	2
		part1	:CalebEyesGlow.part
		part2	:CalebPupil.part
		Part3	:CalebEyesMistRight.part
		bhvr	:prince_eyes_right.bhvr
	End

	

End
End

