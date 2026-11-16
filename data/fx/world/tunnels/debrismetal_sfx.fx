#############################################################
## DebrisMetal_SFX.fx
#############################################################

FxInfo
Lifespan 1

Condition
	On 	Time
	Time	0
	Chance 0.6
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Earthquake_DebrisMetal_01 100 100 .6
		Sound 	Earthquake_DebrisMetal_02 100 100 .6
		Sound 	Earthquake_DebrisMetal_03 100 100 .6
		Sound 	Earthquake_DebrisMetal_04 100 100 .6
	End
End
