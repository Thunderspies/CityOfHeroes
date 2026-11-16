#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	chest
End  

#########################################################

Condition
	On 	Time
	Time 	0

	
	Event

		EName 	Hit
		Type	local 
		At	chest
		
		part1	CustomizeablePowers\FireControl\smoke.part
		part2	CustomizeablePowers\FireControl\Smoke2.part
		Sound Soot2_loop 80 80 .7

	End
	

End



End	


			