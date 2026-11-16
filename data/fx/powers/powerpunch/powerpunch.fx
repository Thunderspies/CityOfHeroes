#########################################################
##	Lightning Bolt
##
FxInfo

lifespan	40

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
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
		Sound powerup3 100 100 .5
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