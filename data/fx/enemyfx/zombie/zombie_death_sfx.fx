#########################################################
##	MH
## 	Female Right Foot

FxInfo

Input
	InpName Origin
End

LifeSpan 30
Condition
	 On Time
	 Time 0
	Event
		Type Local
		At origin
		SoundNoRepeat 3
		Sound Zombie_Vox_01 75 75 .25
		Sound Zombie_Vox_02 75 75 .25
		Sound Zombie_Vox_03 75 75 .25
		Sound Zombie_Vox_04 75 75 .25
		Sound Zombie_Vox_05 75 75 .25
		Sound Zombie_Vox_06 75 75 .25
	End
End

Condition
	 On Time
	 Time 25
	Event
		Type Local
		At origin
		SoundNoRepeat 2
		Sound DeathBodyFall_01 100 100 .25
		Sound DeathBodyFall_02 100 100 .25
		Sound DeathBodyFall_03 100 100 .25
		Sound DeathBodyFall_04 100 100 .25
		Sound DeathBodyFall_05 100 100 .25
		Sound DeathBodyFall_06 100 100 .25
	End
End


End

