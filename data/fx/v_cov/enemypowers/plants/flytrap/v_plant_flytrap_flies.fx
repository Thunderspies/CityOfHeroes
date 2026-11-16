#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	Chest
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	Chest
		Part1	:Flytrap_Flies.part
		Bhvr	:V_Flytrap_Flies.bhvr
		Sound flies_loop 31 31 .13

	End
End



End			