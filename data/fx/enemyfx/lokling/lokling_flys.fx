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
		Part1	:lokling_flys.part
		Part2	EnemyFx/Devouringearth/Fly2.part
		Part2	EnemyFx/Devouringearth/Locusts2.part
		Sound flies_loop 35 35 .21

	End
End



End			