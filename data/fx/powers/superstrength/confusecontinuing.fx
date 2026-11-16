#############################################################################
##Head dizzy
########################################################
FxInfo

Input  
	Inpname	head
End

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		
		#Part1	:HeadDizziesFast.part
		Part2	:HeadDizzyRings.part
		#part3	:MindGlowHit2.part
		part4	:HeadDizzyBalls.part
		part4	:HeadDizzyBursts.part
		Sound whispers_loop 30 30 .6
	End

	Event
		EName	core
		Type	Local
		At	head
		
		Part1	:HeadDizzyBurstsRings.part
		Part2	:HeadDizzyRings2.part
	End


End


End
