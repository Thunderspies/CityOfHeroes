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
		Sound NewFemDeathVOX_01 100 100 .8
		Sound NewFemDeathVOX_02 100 100 .8
		Sound NewFemDeathVOX_03 100 100 .8
		Sound NewFemDeathVOX_04 100 100 .8
		Sound NewFemDeathVOX_05 100 100 .8
		Sound NewFemDeathVOX_06 100 100 .8
		Sound NewFemDeathVOX_07 100 100 .8
	End
End

Condition
	 On Time
	 Time 25
	Event
		Type Local
		At origin
		SoundNoRepeat 2
		Sound DeathBodyFall_01 100 100 .15
		Sound DeathBodyFall_02 100 100 .15
		Sound DeathBodyFall_03 100 100 .15
		Sound DeathBodyFall_04 100 100 .15
		Sound DeathBodyFall_05 100 100 .15
		Sound DeathBodyFall_06 100 100 .15
	End
End


End
