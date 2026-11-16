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
		Part	:InterShardStreaks1.part
		Part	:InterShardBubbles.part
		Part	:InterShard1.part
			
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
		Part	:InterShardStreaks1.part
		Part	:InterShard2.part
		Lifespan 10
				
	End


End

End			