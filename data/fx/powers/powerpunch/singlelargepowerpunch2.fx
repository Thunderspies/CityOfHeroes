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


Input  
	Inpname	Mystic
End

Condition
	On 	Time
	Time 	10

	
	

		Event
		EName 	hand2
		Type	Local 
		At	WepR
		Part1	:Largepowerpunch1.part
		Part2	:Largepowerpunch2.part
		Part3   :LargepowerpunchGlow.part
		Sound PPenergytrans4 180 180 .7
	End


	Event
		EName 	hand
		Type	Local 
		At	WepR
		Part1	:Largepowerpunch3.part
		Part3   :LargepowerpunchRings2.part
		Part4   :LargepowerpunchRingsExpanding.part
	End


End

Condition
	On 	Time
	Time 	50	#37

	Event
		EName 	all
		Type	Destroy

	End



End		

End		