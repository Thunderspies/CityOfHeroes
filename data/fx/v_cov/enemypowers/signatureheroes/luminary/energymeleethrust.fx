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
		
		Bhvr	behaviors/genericParticleFade.bhvr
		#Part1	:Largepowerpunch1.part
		Part3   :LargepowerpunchGlow.part
		Sound PPenergytrans3 100 100 .77
	End


	Event
		EName 	hand
		Type	Local 
		At	WepR
		
		Bhvr	behaviors/genericParticleFade.bhvr
		Part1	:Largepowerpunch3.part
		Part4   :LargepowerpunchRingsExpanding.part
	End


End

Condition
	On 	Time
	Time 	20

	
	

	Event
		EName 	ring2
		Type	Local 
		At	WepR
		
		Bhvr	behaviors/genericParticleFade.bhvr
		Part3   :LargepowerpunchRings.part
		
		Part2	:Largepowerpunch2.part
	End
End

Condition
	On 	Time
	Time 	30	#37

	Event
		EName 	all
		Type	Destroy

	End



End		

End		