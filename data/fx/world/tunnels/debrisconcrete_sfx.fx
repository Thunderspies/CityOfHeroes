#############################################################
## DebrisMetal_SFX.fx
#############################################################

FxInfo
Lifespan 1

Condition
	On 	Time
	Time	0
	Chance 0.4
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Earthquake_DebrisConcrete_01 100 100 .8
		Sound 	Earthquake_DebrisConcrete_02 100 100 .8
		Sound 	Earthquake_DebrisConcrete_03 100 100 .8
		Sound 	Earthquake_DebrisConcrete_04 100 100 .8
	End
End
