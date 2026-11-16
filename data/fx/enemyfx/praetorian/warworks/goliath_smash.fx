#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 2

#############################################################
Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_GoliathFF_01 400 400 .9
		Sound WW_GoliathFF_02 400 400 .9
		Sound WW_GoliathFF_03 400 400 .9
		Sound WW_GoliathFF_04 400 400 .9
		Sound WW_GoliathFF_05 400 400 .9
		Sound WW_GoliathFF_06 400 400 .9
		Sound WW_GoliathFF_07 400 400 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_GoliathSmash_01 400 400 .9
		Sound WW_GoliathSmash_02 400 400 .9
		Sound WW_GoliathSmash_03 400 400 .9
	End

End
