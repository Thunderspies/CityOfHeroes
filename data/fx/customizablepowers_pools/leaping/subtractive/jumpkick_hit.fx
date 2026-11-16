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
		Type	local
		At	Chest
		Part1	:MartialArtsHit.part
		Part2	:MartialArtsHit1.part
		Part3	:MartialArtsHit2.part
		Part4	:MartialArtsHitRaysSmall.part
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	:MAHitStar01Sml.part
		Part2	:MAHitStar02Sml.part
		Part3	:MAHitStar03Sml.part
		Part4	:MAHitGlow01Sml.part
		Part5	:MAHitRays01Sml.part
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
