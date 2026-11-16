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
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	:powerpunch1.part
		Part2	:powerpunch2.part
		Part3   :powerpunchGlow.part
		Sound powerup7 50 50 .5
	End


	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	:powerpunch1.part
		Part2	:powerpunch2.part
		Part3   :powerpunchGlow.part
		Sound luminaryhands2_loop 35 35 .38
	End

		Event
		EName 	Prime3
		Type	Local 
		At	WepL
		Part1	:powerpunch3.part
		#Part2	:powerpunch2.part
		Part3   :powerpunchRings.part
	End
	

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		Part1	:powerpunch3.part
		#Part2	:powerpunch2.part
		Part3   :powerpunchRings.part

	End

End		

End		