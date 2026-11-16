#############################################################
## Banespawn_VoxDeath.fx
#############################################################

FxInfo

FxInfo
Lifespan 1


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_VoxDeath_01 100 100 .8
	End
End
