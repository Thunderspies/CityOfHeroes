#########################################################
##	Lightning Bolt
##
FxInfo


Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End


Condition
	On 	Time
	Time 	10

	Event
		EName 	hand2
		Type	Local 
		At	WepL
		Part1	:BlueLargepowerpunch1.part
		Part2	:BlueLargepowerpunch2.part
		Part3   :BlueLargepowerpunchGlow.part
		Sound PunchA 100 100 .6

	End


	Event
		EName 	hand
		Type	Local 
		At	WepL
		Part1	:BlueLargepowerpunch3.part
		Part3   :BlueLargepowerpunchRings.part

	End
	

		Event
		EName 	hand2
		Type	Local 
		At	WepR
		Part1	:BlueLargepowerpunch1.part
		Part2	:BlueLargepowerpunch2.part
		Part3   :BlueLargepowerpunchGlow.part

	End


	Event
		EName 	hand
		Type	Local 
		At	WepR
		Part1	:BlueLargepowerpunch3.part
		Part3   :BlueLargepowerpunchRings.part

	End


End

Condition
	On 	Time
	Time 	55

	Event
		EName 	all
		Type	Destroy

	End



End		

End		