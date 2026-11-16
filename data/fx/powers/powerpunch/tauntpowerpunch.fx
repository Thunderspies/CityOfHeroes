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
		EName 	hand1
		Type	Local 
		At	WepL
		Part1	:Largepowerpunch1.part
		Part2	:Largepowerpunch2.part
		Part3   :LargepowerpunchGlow.part
		Part4	:Largepowerpunch3.part
		Sound fizzer 100 100 .88
	End


	Event
		EName 	handa
		Type	Local 
		At	WepL
		
		Part3   :TauntpowerpunchRings.part

	End
	

		Event
		EName 	hand2
		Type	Local 
		At	WepR
		Part1	:Largepowerpunch1.part
		Part2	:Largepowerpunch2.part
		Part3   :LargepowerpunchGlow.part
		Part4	:Largepowerpunch3.part
	End


	Event
		EName 	handb
		Type	Local 
		At	WepR
		
		Part3   :TauntpowerpunchRings.part

	End


End

Condition
	On 	Time
	Time 	52

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

End



Condition
	On 	Time
	Time 	58

	Event
		EName 	Handa
		Type	Destroy

	End

	Event
		EName 	Handb
		Type	Destroy

	End

End		


Condition
	On 	Time
	Time 	58

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3   powers/powerpunch/powerpunchGlow.part
		Sound powerup 100 100 .7
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