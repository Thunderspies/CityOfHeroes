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

############################################

Condition
	On	Time	
	Time 	5

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 80.0 80.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End


Condition
	On	Time	
	Time 	5

	Event
		Type 	Local
		At	Origin
		Sound ForceField6_loop 80.0 80.0 .32
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
		Bhvr 	behaviors/powers/forcefield/ForceBubble2.bhvr
		#Sound	ForceField3_loop 80.0 30.0 .23
		Part1	:bubblesphere2.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubbleA.bhvr
		#Sound	Forcefield6_loop 100 30 .17
		
		Geom	FX_ForceBubble
				
	End


End

End
