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
		At	Root
		Part1	GENERIC\Garbage_flys.part
		Bhvr	Behaviors\ShpCrt_Flys.bhvr
		Sound flies_loop 35 35 .01
		
	End
End



End			