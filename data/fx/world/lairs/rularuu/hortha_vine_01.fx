#########################################################
##  Hortha Vine FX
##
FxInfo
	

Input  
	InpName	origin
End

#########################################################


Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	origin
		Sound hortha1_loop 155 155 .87
	End
End

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	origin
		Sound hortha2_loop 99 99 .77
	End
End


Condition
	On 	Time
	Time 	0
##	Chance	.8

	Event
		EName 	Clouds
		Type	Local 
		At	origin
		##Part	:HorthaStreaks1.part
		Part	:Hortha1.part
				
	End

	Event
		EName 	bubbles
		Type	Local 
		At	origin
		Part	:HorthaBubbles.part
		Bhvr	:HorthaOffset.bhvr
		Pmagnet Clouds
			
	End

	Event
		EName 	rings
		Type	Local 
		At	origin
		Part	:HorthaRings1.part
		Bhvr	:HorthaRingOffset.bhvr
		Pmagnet Clouds
			
	End

End

Condition
	On 	Cycle
	Time 	60
	Chance	.5

	Event
		EName 	Flashes
		Type	Local 
		At	origin
		##Part	:HorthaStreaks1.part
		Part	:Hortha2.part
		Sound horthaburst 50 50 .8
		Sound horthaburst2 50 50 .66
		Sound horthaburst3 50 50 .7
		Sound horthaburst2 50 50 .6
		Sound horthaburst 50 50 .6
		Lifespan 10
				
	End


End

End			