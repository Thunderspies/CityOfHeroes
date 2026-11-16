#########################################################
##	SS_launcher_01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	:SS_smoke_01.part
		#Part2	:SmallSmoke2.part
		#Sound	smoke2_loop 240 90 .75
	End
End

End			