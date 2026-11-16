#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	Chest
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	Chest
		Part1	:Flys.part
		Sound flies_loop 24 24 .16

	End
End



End			