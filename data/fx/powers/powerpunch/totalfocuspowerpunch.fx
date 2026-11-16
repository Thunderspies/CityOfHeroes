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
		Part4   :powerpunchRings.part
		Part5	:Largepowerpunch3.part
		Sound PPfocus2 100 100 .7

	End


	Event
		EName 	hand2
		Type	Local 
		At	WepR
		Part1	:Largepowerpunch1.part
		Part2	:Largepowerpunch2.part
		Part3   :LargepowerpunchGlow.part
		Part4   :powerpunchRings.part
		Part5	:Largepowerpunch3.part
	End

End

Condition
	On 	Time
	Time 	63

	Event
		EName 	handA
		Type	Local 
		At	WepR
		
		Part3   :QuickRings.part

	End

	Event
		EName 	handB
		Type	Local 
		At	WepL
		
		Part3   :QuickRings.part

	End

End

Condition
	On 	Time
	Time 	95

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
	Time 	200

	Event
		EName 	all
		Type	Destroy

	End

End


End		