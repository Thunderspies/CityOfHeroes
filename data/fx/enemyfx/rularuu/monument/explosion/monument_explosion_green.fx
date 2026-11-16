#########################################################
##	Template

FxInfo

Lifespan	130

Condition
	On 	Time
	Time 	0	
	Event
		EName	Chest
		Type	Local
		At	Chest
		Geom	Monument_Explosion_Green
		Sound rularuublast 90 90 .8
		Bhvr	:GeomExpand.bhvr
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	0
	
	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Burst.part
		Lifespan	10
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Explosion_Green_Shell_02.part
		part1	:Explosion_Green_Shell_01.part
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	0	
	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Explosion_Green_Beams_01.part
		Lifespan	60
	End
End

Condition
	On 	Time
	Time 	0	
	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Aftermath_Green_02.part
		part1	:Aftermath_Green_01.part
	End
End
