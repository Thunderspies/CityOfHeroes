#########################################################
##
## 	Dummy Fx

FxInfo


LifeSpan 40

Condition
	On	Time
	Time	19

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
	End


End
Condition
	On	Time
	Time	22
	Event
		Type startpositonly
		At handR
#		Part1	powers\superspeed\Speedflash.part
		Part2	powers\superspeed\SpeedStar.part
	End


End



