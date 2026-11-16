#############################################################
## MartialArtsHitsDarkness.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	local
		At	Chest

		Part1	Powers\MartialArts\MartialArtsHit.part
		Part2	CustomizeablePowers\DarkMelee\MartialArtsHit1Black.part
		Part3	CustomizeablePowers\DarkMelee\MartialArtsHit2Black.part
		Part5	CustomizeablePowers\DarkMelee\MartialArtsHitRaysDark.part
		Sound Shadowpunch3 60 60 .76
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part4	Powers\MartialArts\MAHitGlow01Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
		Sound Hit3 100.0 100.0 .8

	End
End


End
