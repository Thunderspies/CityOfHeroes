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
	Event
		Type	Local
		At	Root
		Sound DemonLieu_Spawn_01 200 200 1
	End
End
Condition
	On	Time
	Time	115
	Event
		Type	Local
		At	Root
		Sound DemonLieu_SpawnVOX_01 200 200 1
		Sound DemonLieu_SpawnVOX_02 200 200 1
		Sound DemonLieu_SpawnVOX_03 200 200 1
	End
End