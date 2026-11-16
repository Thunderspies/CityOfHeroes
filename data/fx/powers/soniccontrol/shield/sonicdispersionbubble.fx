#########################################################
##	forceBubble
########################################################
FxInfo



#####################
##Hands
############################################


Condition
	On	Time	
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound sonichands 80 80 .8
	End

End


Condition
	On	Time	
	Time 	17

	Event
		Type 	Local
		At	Origin
		Sound sonic2_loop 100.0 100.0 .77
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On 	Time
	Time 	17

	Event
		
		Type	PositOnly 
		At	HIPS
		Bhvr 	:SonicField.bhvr
		
		Geom	SonicShield
				
	End

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		#Part1	:RepulsionBubbleRing.part
				
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		#Part1	:RepulsionBubbleRingBurst.part
				
	End

End

End
