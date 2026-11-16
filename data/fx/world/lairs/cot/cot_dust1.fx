#########################################################
##	cot_dust1.fx
##
FxInfo
	
Condition
	On 	Cycle
	Time 	130
	Chance	.4
	Event
		EName 	Pr
		Type	Local 
		At	Origin
		Part1	:cot_dust_2.part
		Part2	:cot_dust_3.part
		Part3	:cot_dust_1.part
		Lifespan 60
	End

End

End