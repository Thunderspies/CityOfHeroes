#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

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
	Time 	10

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Part1	:HandGlows.part
				
	End
	
	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	:HandGlows.part
		Sound ForceField3_loop 80.0 80.0 .39
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End	

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble2a.bhvr
		
		Part1	powers/forcefield/bubblesphere.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble3.bhvr
		
		Geom	FX_ForceBubble
				
	End


End

End
