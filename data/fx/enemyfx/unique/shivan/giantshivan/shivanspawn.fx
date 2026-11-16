#############################################################
## ShivanSpawn.fx
#############################################################

FXInfo
Lifespan 100
Flags	IsArmor

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -2.0 0.0
		End
		Part	 :Spawn_Rubble.part
		Sound Zombie_Emerge_01 80 80 .85
		Lifespan 30
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -2.0 0.0
		End
		Part	:Spawn_Dust.part
		Part	:Spawn_Dust_Thick.part
		Part	:Spawn_Dust_Thick2.part

		Lifespan 30
	End

	Event
		Type	Start
		At	Root
		Part	:Spawn_Crater.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -2.0 0.0
		End
		Part	:Spawn_Rubble.part
		Sound Zombie_Emerge_01 80 80 .85
		Lifespan 30
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -2.0 0.0
		End
		Part	:Spawn_Dust.part
		Part	:Spawn_Dust_Thick.part
		Part	:Spawn_Dust_Thick2.part
		Geom	FX_Test_Testsphere
		Lifespan 30
	End
End

#############################################################

End