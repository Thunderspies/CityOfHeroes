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
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	Powers/MartialArts/MartialArtsHitRaysSmall.part
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		Sound riflbutthit3 60.0 60.0 .55

	End
End


End
