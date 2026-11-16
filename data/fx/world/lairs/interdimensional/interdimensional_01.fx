#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	origin
End

#########################################################

Condition
	On 	Time
	Time 	0
##	Chance	.8

	Event
		EName 	Clouds
		Type	start 
		At	origin
		Part	:InterClouds.part
		Part	:InterBubbles.part
		Part	:InterStreaks3.part
		#Part	:InterFloorGlows1.part
		
				
	End


End

Condition
	On 	Cycle
	Time 	60
	Chance	.5

	Event
		EName 	Flashes
		Type	start 
		At	origin
		Part	:InterFlash1.part
		Part	:InterStreaks1.part
		Part	:InterStreaks2.part
		Lifespan 60
				
	End


End


End			