#########################################################
##	forceBubble
########################################################
FxInfo

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
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubbleb.bhvr
		
		Geom	FX_ForceBubble
				
	End


End

End
