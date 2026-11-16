#############################################################
## Death_Fire_Demon.fx
#############################################################

FxInfo
Lifespan 116

#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound Demon_SpawnGroundBurst_01 200 200 1
		Sound Demon_SpawnGroundBurst_02 200 200 1
		Sound Demon_SpawnGroundBurst_03 200 200 1
	End
End