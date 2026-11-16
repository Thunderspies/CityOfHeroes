#############################################################################
##Fear Continuing 2
########################################################
FxInfo

Input  
	Inpname	Hair
End

Condition
	On 	Time
	Time	0
	
	Event	
		Type	Local
		At	Hair	
		Sound DClifedrain3 70 70 .75
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Posit
		At	Hair
		Part1	:FearSkull01.part
		Part2	:FearSkullGlow01.part
		Part3	:FearSkullRings01.part
		Sound darkfear_loop 30 30 .25
	End


End



End
