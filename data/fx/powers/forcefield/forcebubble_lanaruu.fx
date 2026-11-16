#########################################################
##	forceBubble
########################################################
FxInfo




	
#####################
##Hands
############################################


Condition
	On	Time	
	Time 	4

	Event
		Type 	Local
		At	Origin
		Sound forcehands5 80 80 .22
	End

End

Condition
	On	Time	
	Time 	10

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
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble2_lanaruu.bhvr		
		Part1	powers/forcefield/bubblesphere.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble_lanaruu.bhvr
		
		Geom	FX_ForceBubble
				
	End


End

End
