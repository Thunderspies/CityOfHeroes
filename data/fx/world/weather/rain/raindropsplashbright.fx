#########################################################
##	onfire
##
FxInfo  

LifeSpan 50

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	sleeticesplat
		Type	start
		At	origin
		
		BHVR	:RainDropRippleSplat.bhvr
		Splat	generic_Ring.tga
		
		LifeSpan	5

	End
	
	Event
		Type	start
		At	origin
		
		Part	:WaterDropSplashBright.part
		
		LifeSpan	20 
	End
	
End

Condition
	On 	Time
	Time 	8
	
	Event
		Type	start
		At	origin
		
		BHVR	:RainDropRippleSplat2.bhvr
		Splat	generic_Ring.tga
		
		LifeSpan	7 

	End
	
End	

End

