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
		EName 	1
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
		Sound powerup4 100 100 .88
	End


	Event
		EName 	2
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
	End

		Event
		EName 	3
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch3.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchRings.part
	End
	

	Event
		EName 	4
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch3.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchRings.part

	End

End		


Condition
	On 	Time
	Time 	100

	Event
		EName 	1
		Type	Destroy

	End

	Event
		EName 	2
		Type	Destroy

	End

	Event
		EName 	3
		Type	Destroy

	End

	Event
		EName 	4
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
		Sound powerup2 100 100 .8
	End


	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
	End

		Event
		EName 	Prime3
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch3.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchRings.part
	End
	

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch3.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchRings.part

	End

End		


End		