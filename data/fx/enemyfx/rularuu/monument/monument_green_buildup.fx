#########################################################
##	Template

FxInfo

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	Chest
		Sound RularuuMonumentBuild_loop 80 80 .84
	End		
End

Condition
	On 	Time
	Time 	0	
	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Purple_Glow_01.part
		part1	:Green_01.part
		Sound MonumentActivate 80 80 .4
	End
End

Condition
	On 	Cycle
	Time 	20	
	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Green_02.part
		Lifespan 20
	End
End
