#############################################################
## Banespawn_VoxDeath.fx
#############################################################

FxInfo

FxInfo
Lifespan 30


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BP_Death_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	17
	Event
		Type	Local
		At	Root
		Sound 	BP_Death_01 200 100 .8
	End
End
