#############################################################
## FrozenFists_Hit.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
	End


End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
		Sound 	N_Hit_07 100 100 .8
		Sound 	N_Hit_08 100 100 .8
	End

End

#############################################################

End