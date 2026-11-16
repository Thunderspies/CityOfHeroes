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

##################################################################################################3

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prim
		Type	local
		At	HIPS
		BHVR	:ForceBubbleoffset.bhvr
		geom	centerdummy	
				
	End

	Event
		EName 	Pri
		Type	Posit
		At	Prim
		altpiv	1
		Bhvr 	:ForceBubble.bhvr
		
		Geom	FX_ForceBubble
				
	End

End

End
