#########################################################
##	Edited Smoke -RROTH.  Used for Smoke stacks in MERCY ISLAND.
##
FxInfo
	
LifeSpan 0

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		#Part	:SmokeBottom01.part
		#Part	:SmokeBottom02.part
		#Part	:GreySmokeBottemNonFlat.part
	End

	Event
		Type	Local 
		At	Origin
		
		Part	:DarkSmoke01.part  
		Part	:LightSmoke02.part 
		Sound smoke2_loop 390 390 .5
		
		#magnet	Node
	End

End

End	
