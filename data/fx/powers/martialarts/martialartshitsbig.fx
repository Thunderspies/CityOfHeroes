#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	Chest
End

Lifespan 10

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part	:MartialArtsHit.part
		Part	:MartialArtsHit1.part
		Part	:MartialArtsHit2.part
		Part	:MartialArtsHitRaysSmall.part
		Part	:MAHitGlow01.part
	End
	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
	End


End


End
