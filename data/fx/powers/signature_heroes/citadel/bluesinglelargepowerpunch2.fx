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
		Part1	:BlueLargepowerpunch1.part
		Part2	:BlueLargepowerpunch2.part
		Part3   :BlueLargepowerpunchGlow.part
		Sound PPenergytrans3 100 100 .77
	End


	Event
		EName 	hand
		Type	Local 
		At	WepR
		Part1	:BlueLargepowerpunch3.part
		Part3   :BlueLargepowerpunchRings2.part
		Part4   :BlueLargepowerpunchRingsExpanding.part
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