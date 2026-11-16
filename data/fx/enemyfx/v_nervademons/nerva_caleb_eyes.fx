FxInfo

Condition
	On 	Time
	Time 	0

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
		bhvr	:caleb_eyes_left.bhvr
	End

	Event
		EName 	headhook2
		Type	Local
		At	head
		##altpiv	2
		part1	:CalebEyesGlow.part
		part2	:CalebPupil.part
		Part3	:CalebEyesMistRight.part
		bhvr	:caleb_eyes_right.bhvr
	End

	

End
End

