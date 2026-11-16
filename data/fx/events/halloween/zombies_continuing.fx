#############################################################
## Zombies_Continuing.fx
#############################################################

FxInfo

#Lifespan 300

#############################################################

Condition
	On 	Cycle
	Time 	90
	Chance	0.125

	Event
		Type	Local
		At	Head
#		Sound Zombie_Vox_01 80 80 .8
#		Sound Zombie_Vox_02 80 80 .8
#		Sound Zombie_Vox_03 80 80 .8
#		Sound Zombie_Vox_04 80 80 .8
#		Sound Zombie_Vox_05 80 80 .8
#		Sound Zombie_Vox_06 80 80 .8
#		Sound Zombie_Vox_07 80 80 .8
		Lifespan 150
	End
End

#############################################################

End