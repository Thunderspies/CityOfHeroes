#########################################################
##	FireRing
##
FxInfo
	
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Dirt
		Type	Posit 
		At	Origin
		Part1	World/Lairs/COT/Cot_dust_2.part
		
	End


End

Condition
	On 	Time
	Time 	1

	Event
		EName 	Dust
		Type	Posit 
		At	Origin
		Part1	World/Lairs/COT/Cot_dust_1.part
				
	End


End


End			