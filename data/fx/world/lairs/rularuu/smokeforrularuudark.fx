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
		Part	:SmokeBottom01.part
		Part	:SmokeBottom02.part
		#Part	:GreySmokeBottemNonFlat.part
	End

	Event
		Type	Local 
		At	Origin
		
		Part	:DarkSmoke01.part 
		Part	:DarkSmoke02.part 
		Part	:Flames01.part
		Sound firesword_loop 390 390 .5
		
		#magnet	Node
	End

End

End			