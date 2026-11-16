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
		SoundNoRepeat 2
		Sound NewHugeDeathVOX_01 100 100 .8
		Sound NewHugeDeathVOX_02 100 100 .8
		Sound NewHugeDeathVOX_03 100 100 .8
		Sound NewHugeDeathVOX_04 100 100 .8
		Sound NewHugeDeathVOX_05 100 100 .8
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
