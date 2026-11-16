#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		Part	:DarkSmokeBottem.part 
		Part	:GreySmokeBottemNonFlat.part
	End

	Event
		Type	Local 
		At	Origin
		
		Part	:DarkSmokeMagnet.part 
		Part	:DarkSmokeCoreMagnet.part
		Part	:DarkSmokeFlamesBright.part
		Sound firesword_loop 390 390 .5
		
		#magnet	Node
	End

End

End			