#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HIPS
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End



	
#####################
##Hands
############################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble2.bhvr
		Sound ForceField3_loop 100.0 100.0 .23
		Part1	powers/forcefield/bubblesphere.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		
		Geom	FX_ForceBubble
				
	End


End


Condition	
	On 	Time
	Time 	150

	Event	
	Ename	all
	Type	destroy

	End

End

End
