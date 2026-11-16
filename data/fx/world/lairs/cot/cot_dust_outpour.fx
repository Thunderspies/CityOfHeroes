#########################################################
##	cot_dust1.fx
##
FxInfo
	
Condition
	On 	Cycle
	Time 	130
	Chance  .4
	Event
		EName 	Pr
		Type	Local 
		At	Origin
		Part1	:cot_dust_outpour.part
		Part2	:cot_dust_1.part
		Sound smallrock2 40 40 .46
		Lifespan 75
	End

End

End